/*
HP41UC
User-Code File Converter/Compiler/De-compiler/Barcode Generator.
Copyright (c) Leo Duran, 2000-2016.  All rights reserved.

Build environment: Microsoft Visual Studio or GNU C compiler.
*/

/*
This file is part of HP41UC.

HP41UC is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

HP41UC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with HP41UC.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "hp41uc.h"
#include "compile.h"

#define MAX_ARGS 5
#define MAX_CODE 18
#define MAX_LINE 128

static int global_label = 0;
static int global_count = 0;
static int global_end = 0;

static int fnumeric = 0;

static COMPILE_STATE state = COMPILE_SEEK_START_LINE;
static int source_line = 0;
static int line_end = 0;
static int line_index = 0;
static int last_block = 0;

static int line_argc;
static int code_count;
static unsigned char *line_ptr;
static unsigned char *line_argv[MAX_ARGS];
static unsigned char line_buffer[MAX_LINE];
static unsigned char code_buffer[MAX_CODE];

void compile_init(void)
{
	global_label = 0;
	global_count = 0;
	global_end = 0;

	fnumeric = 0;

	source_line = 0;
	line_end = 0;
	line_index = 0;
	last_block = 0;
	state = COMPILE_SEEK_START_LINE;
}

int compile(unsigned char *out_buffer, int out_size,
	unsigned char **pin_buffer, int *pin_count,
	COMPILE_FLAG *pflag, int *ppending, int *pend_count)
{
	COMPILE_FLAG flag;
	int done = 0;
	int consumed = 0;
	int produced = 0;
	unsigned char c, *inp, *outp;

	flag = *pflag;
	if (flag == COMPILE_FLAG_RESTART ||
		flag == COMPILE_FLAG_RESTART_EOF) {
		line_index = 0;
		source_line = 0;
		last_block = (flag == COMPILE_FLAG_RESTART_EOF) ? 1 : 0;
		flag = COMPILE_FLAG_NONE;
		state = COMPILE_SEEK_START_LINE;
	}
	else if (flag == COMPILE_FLAG_EOF) {
		last_block = 1;
		flag = COMPILE_FLAG_NONE;
	}

	inp = *pin_buffer;
	outp = out_buffer;
	do {
		switch (state) {
		case COMPILE_SEEK_START_LINE:
			c = *inp++;
			++consumed;
			if (c == '\n' || c == '\f' || c == 0x1A) {
				++source_line;
			}
			else if (c != '\r') {
				line_buffer[line_index++] = c;
				state = COMPILE_GET_LINE;
			}
			break;

		case COMPILE_GET_LINE:
			c = *inp++;
			++consumed;
			if (c == '\n' || c == '\f' || c == 0x1A) {
				line_end = 1;
			}
			else if (c == '\r') {
				state = COMPILE_SEEK_END_LINE;
			}
			else if (line_index == MAX_LINE) {
				line_index = 0;
				++source_line;
				flag = COMPILE_FLAG_ERROR;
				printf("Error: line too long!\n");
				state = COMPILE_IGNORE_BAD_LINE;
			}
			else {
				line_buffer[line_index++] = c;
			}
			break;

		case COMPILE_SEEK_END_LINE:
			c = *inp++;
			++consumed;
			if (c == '\n' || c == '\f' || c == 0x1A) {
				line_end = 1;
			}
			else if (c != '\r') {
				line_index = 0;
				++source_line;
				flag = COMPILE_FLAG_ERROR;
				printf("Error: found[ %c ] expecting end-of-line character.\n", c);
				state = COMPILE_IGNORE_BAD_LINE;
			}
			break;

		case COMPILE_LINE_OUTPUT:
			/* got room in output buffer? */
			if (produced + code_count > out_size)
				done = 1;
			else if (code_count) {
				/* flush code buffer */
				memcpy(outp, code_buffer, code_count);
				outp += code_count;
				produced += code_count;
				code_count = 0;
			}
			else if (!global_end &&
				(line_argc = get_line_args(line_argv, &line_ptr))) {
				if (line_argc == MAX_ARGS && strlen((char *)line_ptr)) {
					printf("Error: too many arguments[");
					for (line_argc = 0; line_argc < MAX_ARGS; ++line_argc)
						printf(" %s", line_argv[line_argc]);
					printf(" %s ]\n", line_ptr);
					code_count = 0;
				}
				else {
					/* compile next instruction */
					code_count = compile_args(code_buffer,
						line_argc, line_argv);
				}

				/* any errors? */
				if (code_count == 0) {
					flag = COMPILE_FLAG_ERROR;
					state = COMPILE_SEEK_START_LINE;
				}
				else if (global_end) {
					flag = COMPILE_FLAG_END;
					printf(".END. found on line %d.\n", source_line);
				}
			}
			else {
				/* we're done with this line */
				state = COMPILE_SEEK_START_LINE;
			}
			break;

		case COMPILE_IGNORE_BAD_LINE:
			c = *inp++;
			++consumed;
			/* end-of-line? */
			if (c == '\n' || c == '\f' || c == 0x1A) {
				state = COMPILE_SEEK_START_LINE;
			}
			break;
		}

		/* end-of-line? */
		if (line_end ||
			((state == COMPILE_GET_LINE ||
			state == COMPILE_SEEK_END_LINE) &&
			consumed == *pin_count && last_block)) {
			line_buffer[line_index] = '\0';
			line_ptr = line_buffer;
			code_count = 0;
			line_index = 0;
			line_end = 0;
			++source_line;
			state = COMPILE_LINE_OUTPUT;
		}

		/* any errors? */
		if (flag == COMPILE_FLAG_ERROR) {
			done = 1;
			printf("Compiler error on line %d.\n", source_line);
		}

		/* exhausted buffers? */
		if (produced == out_size ||
			(consumed == *pin_count && state != COMPILE_LINE_OUTPUT)) {
			done = 1;
		}

	} while (!done);

	*pflag = flag;
	*ppending = (state == COMPILE_LINE_OUTPUT) ? 1 : 0;
	*pin_buffer += consumed;
	*pin_count -= consumed;
	return(produced);
}


int get_line_args(unsigned char *line_argv[], unsigned char **line_ptr)
{
	unsigned char *pc;
	int i, j, count, done;

	count = 0;
	pc = *line_ptr;
	done = (*pc == '\0') ? 1 : 0;
	while (!done) {
		/* ignore leading spaces */
		while (*pc == '\t' || *pc == 0x20)
			++pc;

		/* ignore comment line */
		if (*pc == ';' ||
			*pc == '#' ||
			*pc == '\0') {
			done = 1;
		}
		else {
			/* get argument */
			i = 0;
			do {
				/* consider quotes as a unit */
				j = is_inquotes(&pc[i]);
				i += j+1;

				/* end of line? */
				if (pc[i] == '\0') {
					done = 2;
				}
				/* end of instruction? */
				else if (pc[i] == ',') {
					if (pc[i + 1] == '\t' ||
						pc[i + 1] == 0x20) {
						pc[i] = '\0';
						done = 1;
					}
				}
				/* end of argument? */
				else if (pc[i] == '\t' ||
					pc[i] == 0x20) {
					pc[i] = '\0';
				}
			} while (pc[i] != '\0');

			/* put argument in list */
			line_argv[count++] = pc;

			/* point to next argument */
			pc += strlen((char *)pc);
			if (done != 2)
				++pc;

			/* full list? */
			if (count == MAX_ARGS) {
				done = 1;
			}
		}
	}

	/* update line pointer */
	*line_ptr = pc;

	return(count);
}

#define MAX_NUMERIC     16
#define MAX_DIGITS      10
#define MAX_INT         8
#define MAX_EXP         2

#define MAX_ALPHA       15


int get_numeric_prefix(unsigned char *numeric, unsigned char *buffer)
{
	int i;
	int error = 0;
	int num_index = 0;
	int num_decimal = 0;
	int num_digits = 0;
	int exp_entry = 0;
	int exp_sign = 0;
	int exp_digits = 0;

	for (i = 0; (size_t)i < strlen((char *)buffer) && !error; ++i) {
		if (exp_entry) {
			if (buffer[i] == '+' ||
				buffer[i] == '-') {
				if (exp_sign || exp_digits)
					error = 1;
				else {
					++exp_sign;
					if (buffer[i] == '-') {
						numeric[num_index++] = '-';
					}
				}
			}
			else if (buffer[i] >= '0' &&
				buffer[i] <= '9') {
				if (exp_digits == MAX_EXP)
					error = 1;
				else {
					++exp_digits;
					numeric[num_index++] = buffer[i];
				}
			}
			else {
				error = 1;
			}
		}
		else if (buffer[i] >= '0' &&
			buffer[i] <= '9') {
			if (num_digits == MAX_DIGITS)
				error = 1;
			else {
				++num_digits;
				numeric[num_index++] = buffer[i];
			}
		}
		else if (buffer[i] == '.' ||
			buffer[i] == ',') {
			if (num_decimal)
				error = 1;
			else {
				++num_decimal;
				numeric[num_index++] = '.';
			}
		}
		else if (buffer[i] == 'E' ||
			buffer[i] == 'e') {
			++exp_entry;
			numeric[num_index++] = 'E';
		}
		else if (buffer[i] == '+' ||
			buffer[i] == '-') {
			if (i == 0) {
				if (buffer[i] == '-' &&
					strlen((char *)buffer) > 1) {
					numeric[num_index++] = buffer[i];
				}
			}
			else if (num_digits || num_decimal) {
				/* replace decimal point */
				if (!num_digits) {
					numeric[num_index - 1] = '1';
				}

				++exp_sign;
				++exp_entry;
				numeric[num_index++] = 'E';

				/* exponent sign */
				if (buffer[i] == '-') {
					numeric[num_index++] = '-';
				}
			}
			else {
				error = 1;
			}
		}
		else {
			error = 1;
		}
	}

	/* terminate string */
	numeric[num_index] = '\0';

	return(!error && strlen((char *)numeric));
}


int get_text_prefix(unsigned char *text, unsigned char *buffer, int *pcount)
{
	int i, j, k;
	int error = 1, istext = 1;
	unsigned char lbuffer[MAX_LINE];

	/* get start-quote */
	for (i = 0, k = 0; (size_t)i < strlen((char *)buffer) && k == 0; ++i) {
		if (buffer[i] == '\"' ||
			buffer[i] == '\'') {
			j = i;
			k = 1;
		}
	}

	/* start-quote? */
	if (k) {
		/* end-quote? */
		if ((i = is_inquotes(&buffer[j]))) {
			/* make string copy */
			strcpy((char *)lbuffer, (char *)buffer);

			/* remove end-quote */
			lbuffer[i + j] = '\0';

			/* append text? */
			if (j) {
				/* remove start-quote */
				lbuffer[j] = '\0';
				if (is_append(lbuffer)) {
					lbuffer[j] = 0x7F;
					--j;
				}
				else if (!is_text(lbuffer)) {
					istext = 0;
				}
			}

			/* parse for esc-sequence after start-quote */
			if (istext) {
				if (parse_text(text, &lbuffer[j + 1], pcount)) {
					text[*pcount] = '\0';
					error = 0;
				}
			}
		}
	}

	return(!error);
}

int get_alpha_postfix(unsigned char *alpha, unsigned char *buffer)
{
	int i;
	unsigned char lbuffer[MAX_LINE];

	/* end-quote? */
	if ((i = is_inquotes(buffer))) {
		/* make string copy */
		strcpy((char *)lbuffer, (char *)buffer);

		/* remove end-quote */
		lbuffer[i] = '\0';

		/* copy alpha string after start-quote */
		if (strlen((char *)&lbuffer[1]) <= MAX_ALPHA) {
			strcpy((char *)alpha, (char *)&lbuffer[1]);
			return(1);
		}
	}

	return(0);
}


int compile_num(unsigned char *code, unsigned char *num)
{
	int i, count;

	count = strlen((char *)num);
	for (i = 0; i < count; ++i) {
		if (num[i] == '-')
			code[i] = 0x1C;
		else if (num[i] == 'E')
			code[i] = 0x1B;
		else if (num[i] == '.')
			code[i] = 0x1A;
		else {
			code[i] = 0x10 + num[i] - '0';
		}
	}

	return(count);
}


int compile_text(unsigned char *code, unsigned char *text, int count)
{
	/* TEXT0..15 */
	code[0] = 0xF0 + count;
	if (count)
		memcpy(&code[1], text, count);

	return(count + 1);
}


int compile_alpha(unsigned char *code, unsigned char *prefix, unsigned char *alpha)
{
	int i, j;
	int local, count;
	unsigned char mm, ff;
	unsigned char lbuffer[MAX_LINE];

	strcpy((char *)lbuffer, (char *)alpha);
	if (parse_text(alpha, lbuffer, &count)) {
		alpha[count] = '\0';
	}

	local = is_local_label((char *)alpha);

	/* LBL "alpha" */
	if (_stricmp((char *)prefix, "LBL") == 0) {
		if (strlen((char *)alpha) >= MAX_ALPHA) {
			printf("Error: alpha (global) postfix[ %s \"%s\" ] too long.\n",
				prefix, alpha);
			return(0);
		}
		else if (force_global || !local) {
			code[0] = 0xC0;
			code[1] = 0x00;
			code[2] = 0xF1 + count;
			code[3] = 0x00;
			if (count)
				memcpy(&code[4], alpha, count);

			/* set LABEL flag */
			global_label = 1;
			global_count = 0;
			return(count + 4);
		}
		else {
			code[0] = 0xCF;
			code[1] = local;
			return(2);
		}
	}

	/*  GTO "alpha" */
	if (_stricmp((char *)prefix, "GTO") == 0 ||
		_stricmp((char *)prefix, "GOTO") == 0) {
		if (force_global || !local) {
			code[0] = 0x1D;
			code[1] = 0xF0 + count;
			if (count)
				memcpy(&code[2], alpha, count);
			return(count + 2);
		}
		else {
			code[0] = 0xD0;
			code[1] = 0x00;
			code[2] = local;
			return(3);
		}
	}

	/*  XEQ "alpha" */
	if (_stricmp((char *)prefix, "XEQ") == 0) {
		if (force_global || !local) {
			code[0] = 0x1E;
			code[1] = 0xF0 + count;
			if (count)
				memcpy(&code[2], alpha, count);
			return(count + 2);
		}
		else {
			code[0] = 0xE0;
			code[1] = 0x00;
			code[2] = local;
			return(3);
		}
	}

	/*  W "alpha" */
	if (_stricmp((char *)prefix, "W") == 0) {
		code[0] = 0x1F;
		code[1] = 0xF0 + count;
		if (count)
			memcpy(&code[2], alpha, count);
		return(count + 2);
	}

	/* XROM "alpha" */
	if (_stricmp((char *)prefix, "XROM") == 0) {
		j = sizeof(xrom_fcn) / sizeof(XROM);
		for (i = 0; i < j; ++i) {
			if (_stricmp((char *)alpha, xrom_fcn[i].prefix) == 0) {
				mm = (unsigned char)xrom_fcn[i].code[0];
				ff = (unsigned char)xrom_fcn[i].code[1];
				code[0] = 0xA0 + (mm >> 2);
				code[1] = ((mm & 0x03) << 6) + ff;
				return(2);
			}
		}

		printf("Error: unrecognized alpha postfix[ %s \"%s\" ], try: [ XROM mm,ff ]\n",
			prefix, alpha);
		return(0);
	}

	printf("Error: unrecognized prefix[ %s \"%s\" ]\n", prefix, alpha);
	return(0);
}


int compile_arg1(unsigned char *code, unsigned char *prefix)
{
	int i, j;
	unsigned char mm, ff;

	/* .END. */
	if (_stricmp((char *)prefix, "END") == 0 ||
		_stricmp((char *)prefix, ".END.") == 0) {
		compile_end(code, global_count);

		/* set .END. flag */
		global_end = 1;
		return(3);
	}

	/* XROM functions */
	j = sizeof(xrom_fcn) / sizeof(XROM);
	for (i = 0; i < j; ++i) {
		if (_stricmp((char *)prefix, xrom_fcn[i].prefix) == 0) {
			mm = (unsigned char)xrom_fcn[i].code[0];
			ff = (unsigned char)xrom_fcn[i].code[1];
			code[0] = 0xA0 + (mm >> 2);
			code[1] = ((mm & 0x03) << 6) + ff;
			return(2);
		}
	}

	/* alternate-form functions */
	j = sizeof(alt_fcn1) / sizeof(FCN);
	for (i = 0; i < j; ++i) {
		if (_stricmp((char *)prefix, alt_fcn1[i].prefix) == 0) {
			code[0] = (unsigned char)alt_fcn1[i].code;
			return(1);
		}
	}

	/* single-byte functions */
	for (i = 0x40; i <= 0x8F; ++i) {
		if (_stricmp((char *)prefix, (char *)single20_8F[i - 0x20]) == 0) {
			code[0] = i;
			return(1);
		}
	}

	printf("Error: unrecognized or incomplete function[ %s ]\n", prefix);
	printf("If [ %s ] is an external module function, try: [ XROM mm,ff ]\n",
		prefix);
	return(0);
}


int compile_arg2(unsigned char *code, unsigned char *prefix, unsigned char *postfix)
{
	int i, j, k;
	long m, f;
	char *stop;
	unsigned char mm, ff;
	unsigned char *pm, *pf;
	unsigned char lbuffer[MAX_LINE];
	unsigned char num_postfix[] = "0#";
	unsigned char *ppostfix = postfix;

	/* XROM mm,ff */
	if (_stricmp((char *)prefix, "XROM") == 0) {
		if ((pf = (unsigned char *)strchr((char *)postfix, ','))) {
			*pf++ = '\0';
			pm = postfix;
			if (strlen((char *)pm) && strlen((char *)pf)) {
				for (i = 0, j = 1; i < (int)strlen((char *)pm) && j; ++i)
					j = isdigit(pm[i]);
				if (j) {
					m = strtol((char *)pm, &stop, 10);
					if (m >= 0 && m <= 31) {
						for (i = 0, j = 1; i < (int)strlen((char *)pf) && j; ++i)
							j = isdigit(pf[i]);
						if (j) {
							f = strtol((char *)pf, &stop, 10);
							if (f >= 0 && f <= 63) {
								mm = (unsigned char)m;
								ff = (unsigned char)f;
								code[0] = 0xA0 + (mm >> 2);
								code[1] = ((mm & 0x03) << 6) + ff;
								return(2);
							}
						}
					}
				}
			}
		}
	}
	else {
		/* add leading "0" */
		if (strlen((char *)postfix) == 1 &&
			isdigit(postfix[0])) {
			num_postfix[1] = postfix[0];
			ppostfix = num_postfix;
		}

		/* */
		/* single-byte functions */
		/* */
		strcpy((char *)lbuffer, (char *)prefix);
		strcat((char *)lbuffer, " ");
		strcat((char *)lbuffer, (char *)ppostfix);

		/* LBL 00..14 */
		for (i = 0x01; i <= 0x0F; ++i) {
			j = i - 0x01;
			if (_stricmp((char *)lbuffer, (char *)single01_1C[j]) == 0) {
				code[0] = i;
				return(1);
			}
		}

		/* RCL 00..15, STO 00..15 */
		for (i = 0x20; i <= 0x3F; ++i) {
			j = i - 0x20;
			if (_stricmp((char *)lbuffer, (char *)single20_8F[j]) == 0) {
				code[0] = i;
				return(1);
			}
		}

		/* GTO 00..14 */
		for (i = 0xB1; i <= 0xBF; ++i) {
			j = i - 0xB1;
			if (_stricmp((char *)lbuffer, (char *)prefixB1_BF[j]) == 0) {
				code[0] = i;
				code[1] = 0x00;
				return(2);
			}
		}

		/* */
		/* mutiple-byte functions */
		/* */
		if (is_postfix(ppostfix, &i)) {
			/* */
			/* 2-byte functions */
			/* */
			strcpy((char *)lbuffer, (char *)prefix);
			strcat((char *)lbuffer, " ");

			/* RCL __..TONE __ */
			for (j = 0x90; j <= 0x9F; ++j) {
				k = j - 0x90;
				if (_stricmp((char *)lbuffer, (char *)prefix90_9F[k]) == 0) {
					code[0] = j;
					code[1] = i;
					return(2);
				}
			}

			/* SF __..FC? __ */
			for (j = 0xA8; j <= 0xAD; ++j) {
				k = j - 0xA8;
				if (_stricmp((char *)lbuffer, (char *)prefixA8_AD[k]) == 0) {
					code[0] = j;
					code[1] = i;
					return(2);
				}
			}

			/* X<> __..LBL __ */
			for (j = 0xCE; j <= 0xCF; ++j) {
				k = j - 0xCE;
				if (_stricmp((char *)lbuffer, (char *)prefixCE_CF[k]) == 0) {
					code[0] = j;
					code[1] = i;
					return(2);
				}
			}

			/* alternate-form functions */
			k = sizeof(alt_fcn2) / sizeof(FCN);
			for (j = 0; j < k; ++j) {
				if (_stricmp((char *)prefix, alt_fcn2[j].prefix) == 0) {
					code[0] = (unsigned char)alt_fcn2[j].code;
					code[1] = i;
					return(2);
				}
			}

			/* */
			/* 3-byte functions */
			/* */
			/* GTO __ */
			if (_stricmp((char *)prefix, "GTO") == 0 ||
				_stricmp((char *)prefix, "GOTO") == 0) {
				code[0] = 0xD0;
				code[1] = 0x00;
				code[2] = i;
				return(3);
			}

			/* XEQ __ */
			if (_stricmp((char *)prefix, "XEQ") == 0) {
				code[0] = 0xE0;
				code[1] = 0x00;
				code[2] = i;
				return(3);
			}
		}
	}

	printf("Error: unrecognized function[ %s %s ]\n", prefix, postfix);
	return(0);
}


int compile_arg3(unsigned char *code, unsigned char *prefix, unsigned char *ind, unsigned char *postfix)
{
	int i, j, k;
	unsigned char lbuffer[MAX_LINE];
	unsigned char num_postfix[] = "0#";
	unsigned char *ppostfix = postfix;

	/* add leading "0" */
	if (strlen((char *)postfix) == 1 &&
		isdigit(postfix[0])) {
		num_postfix[1] = postfix[0];
		ppostfix = num_postfix;
	}

	if (_stricmp((char *)ind, "IND") == 0 &&
		is_postfix(ppostfix, &i)) {
		/* */
		/* 2-byte functions */
		/* */
		strcpy((char *)lbuffer, (char *)prefix);
		strcat((char *)lbuffer, " ");

		/* RCL IND __..TONE IND __ */
		for (j = 0x90; j <= 0x9F; ++j) {
			k = j - 0x90;
			if (_stricmp((char *)lbuffer, (char *)prefix90_9F[k]) == 0) {
				code[0] = j;
				code[1] = i + 0x80;
				return(2);
			}
		}

		/* alternate-form IND functions */
		k = sizeof(alt_fcn2) / sizeof(FCN);
		for (j = 0; j < k; ++j) {
			if (_stricmp((char *)prefix, alt_fcn2[j].prefix) == 0) {
				code[0] = (unsigned char)alt_fcn2[j].code;
				code[1] = i + 0x80;
				return(2);
			}
		}

		/* SF IND __..FC? IND __ */
		for (j = 0xA8; j <= 0xAD; ++j) {
			k = j - 0xA8;
			if (_stricmp((char *)lbuffer, (char *)prefixA8_AD[k]) == 0) {
				code[0] = j;
				code[1] = i + 0x80;
				return(2);
			}
		}

		/* X<> IND __..LBL IND __ */
		for (j = 0xCE; j <= 0xCF; ++j) {
			k = j - 0xCE;
			if (_stricmp((char *)lbuffer, (char *)prefixCE_CF[k]) == 0) {
				code[0] = j;
				code[1] = i + 0x80;
				return(2);
			}
		}

		/* GTO IND __ */
		if (_stricmp((char *)prefix, "GTO") == 0 ||
			_stricmp((char *)prefix, "GOTO") == 0) {
			code[0] = 0xAE;
			code[1] = i;
			return(2);
		}

		/* XEQ IND __ */
		if (_stricmp((char *)prefix, "XEQ") == 0) {
			code[0] = 0xAE;
			code[1] = i + 0x80;
			return(2);
		}
	}

	printf("Error: unrecognized function[ %s %s %s ]\n",
		prefix, ind, postfix);
	return(0);
}


int compile_label(unsigned char *code, unsigned char *label, unsigned char *alpha, unsigned char *key)
{
	int asn, count;

	asn = get_key(key);
	count = strlen((char *)alpha);
	if (asn && count && count < MAX_ALPHA &&
		_stricmp((char *)label, "LBL") == 0) {
		code[0] = 0xC0;
		code[1] = 0x00;
		code[2] = 0xF1 + count;
		code[3] = asn;
		memcpy(&code[4], alpha, count);
		return(count + 4);
	}

	if (count >= MAX_ALPHA)
		printf("Error: alpha (global) postfix[ %s \"%s\" %s ] too long.\n",
		label, alpha, key);
	else
		printf("Error: invalid key assignment[ %s \"%s\" %s ]\n",
		label, alpha, key);

	return(0);
}


void compile_end(unsigned char *buffer, int bytes)
{
	int a, bc;

	/* */
	/* END: [ Ca bc xx ] */
	/* where: a = 0..D, bc = 00..FF */
	/*   bc + a[ bit0 ] = #regs ( 7 bytes/reg ) */
	/*   a[ bit3..1 ] = #remaining bytes ( up-to 6 ) */
	/* max #bytes = 6 + ( 0x1FF * 7 ) = 3583 bytes */
	/* max #bytes before using a[ bit0 ] = 6 + ( 0xFF * 7 ) = 1791 */
	/* */
	/* xx = 0D: non-private, unpacked */
	/* */
	if (bytes > 3583) {
		a = 0;
		bc = 0;
	}
	else {
		if (bytes < 1792)
			a = 0;
		else {
			bytes -= 1792;
			a = 1;
		}
		bc = bytes / 7;
		a += (bytes - (bc * 7)) * 2;
	}

	buffer[0] = 0xC0 + a;
	buffer[1] = bc;
	buffer[2] = 0x0D;
}


int is_postfix(unsigned char *postfix, int *pindex)
{
	int i, j;

	for (i = 0; i <= 127; ++i) {
		if (strcmp((char *)postfix, (char *)postfix00_7F[i]) == 0) {
			*pindex = i;
			return(1);
		}
	}

	/* case-insensitive: "F..R" */
	for (i = 107; i <= 122; ++i) {
		if (_stricmp((char *)postfix, (char *)postfix00_7F[i]) == 0) {
			*pindex = i;
			return(1);
		}
	}

	for (i = 102, j = 0; i <= 111; ++i, ++j) {
		if (strcmp((char *)postfix, (char *)alt_postfix102_111[j]) == 0) {
			*pindex = i;
			return(1);
		}
	}

	for (i = 117, j = 0; i <= 122; ++i, ++j) {
		if (strcmp((char *)postfix, (char *)alt_postfix117_122[j]) == 0) {
			*pindex = i;
			return(1);
		}
	}

	if (strcmp((char *)postfix, "\xC3") == 0 ||
		strcmp((char *)postfix, ">") == 0 ||
		strcmp((char *)postfix, "|-") == 0 ||
		strcmp((char *)postfix, "\\-") == 0 ||
		strcmp((char *)postfix, ">-") == 0 ||
		strcmp((char *)postfix, "->") == 0) {
		*pindex = 0x7A;
		return(1);
	}

	return(0);
}


int parse_text(unsigned char *text, unsigned char *buffer, int *pcount)
{
	int i, j, k, n;
	int esc_x;

	*pcount = 0;
	k = strlen((char *)buffer);
	if (k == 0)
		return(1);

	/* append text? */
	if (buffer[0] == 0xC3)
		buffer[0] = 0x7F;

	i = j = n = 0;
	do {
		--k;

		if (j == 0) {
			/* esc-sequence */
			if (buffer[i] == '\\' && k) {
				j = 1;
				esc_x = 0;
			}
			/* append: "|-text", ">-text", "->text" */
			else if (k && n == 0 &&
				((buffer[i] == '|' && buffer[i + 1] == '-') ||
				(buffer[i] == '>' && buffer[i + 1] == '-') ||
				(buffer[i] == '-' && buffer[i + 1] == '>'))) {
				text[n++] = 0x7F;
				--k;
				++i;
			}
			/* append: ">text" */
			else if (k && n == 0 && buffer[i] == '>') {
				text[n++] = 0x7F;
			}
			else {
				/* repeat quote */
				if ((buffer[i] == '\"' ||
					buffer[i] == '\'') && k &&
					buffer[i] == buffer[i + 1]) {
					++i;
					--k;
				}
				text[n++] = buffer[i];
			}
		}
		/* start of esc-sequence */
		else if (j == 1) {
			if (isxdigit(buffer[i])) {
				if (k)
					j = 2;
				else {
					text[n++] = get_xdigit(buffer[i]);
					j = 0;
				}
			}
			else if (esc_x) {
				text[n++] = '\\';
				if (n < MAX_ALPHA) {
					text[n++] = 'x';
					if (n < MAX_ALPHA)
						text[n++] = buffer[i];
				}
				j = 0;
			}
			else if (buffer[i] == 'x') {
				esc_x = 1;
			}
			else if (buffer[i] == 'a') {
				text[n++] = '\a';
				j = 0;
			}
			else if (buffer[i] == 'b') {
				text[n++] = '\b';
				j = 0;
			}
			else if (buffer[i] == 'f') {
				text[n++] = '\f';
				j = 0;
			}
			else if (buffer[i] == 'n') {
				text[n++] = '\n';
				j = 0;
			}
			else if (buffer[i] == 'r') {
				text[n++] = '\r';
				j = 0;
			}
			else if (buffer[i] == 't') {
				text[n++] = '\t';
				j = 0;
			}
			else if (buffer[i] == 'v') {
				text[n++] = '\v';
				j = 0;
			}
			else if (buffer[i] == '?') {
				text[n++] = '\?';
				j = 0;
			}
			else if (buffer[i] == '\"') {
				text[n++] = '\"';
				j = 0;
			}
			else if (buffer[i] == '\'') {
				text[n++] = '\'';
				j = 0;
			}
			else if (buffer[i] == '\\') {
				text[n++] = '\\';
				j = 0;
			}
			else if (buffer[i] == '-' && n == 0) {
				/* append: "\-text" */
				text[n++] = 0x7F;
				j = 0;
			}
			else {
				text[n++] = '\\';
				if (n < MAX_ALPHA)
					text[n++] = buffer[i];
				j = 0;
			}
		}
		/* second hex-digit in esc-sequence? */
		else {	/* j == 2 */
			if (isxdigit(buffer[i])) {
				text[n++] = (get_xdigit(buffer[i - 1]) << 4) +
					get_xdigit(buffer[i]);
				j = 0;
			}
			else {
				text[n++] = get_xdigit(buffer[i - 1]);

				/* start next esc-seqeunce? */
				if (buffer[i] == '\\' && k) {
					esc_x = 0;
					j = 1;
				}
				else {
					if (n < MAX_ALPHA)
						text[n++] = buffer[i];
					j = 0;
				}
			}
		}

		/* overflow, underflow? */
		if ((n == MAX_ALPHA && k) || (k == 0 && j))
			return(0);

		++i;
	} while (k);

	*pcount = n;
	return(n);
}


int is_inquotes(unsigned char *buffer)
{
	int i;

	if (buffer[0] == '\"' || buffer[0] == '\'') {
		for (i = 1; (size_t)i < strlen((char *)buffer); ++i) {
			if (buffer[i] == buffer[0] &&
				buffer[i - 1] != '\\') {
				if (buffer[i + 1] == buffer[0])
					++i;
				else if (buffer[i + 1] == '\0' ||
					buffer[i + 1] == '\t' ||
					buffer[i + 1] == 0x20) {
					return(i);
				}
			}
		}
	}

	return(0);
}


int is_append(unsigned char *prefix)
{
	return(strcmp((char *)prefix, "\xC3") == 0 ||
		strcmp((char *)prefix, ">") == 0 ||
		strcmp((char *)prefix, "|-") == 0 ||
		strcmp((char *)prefix, "\\-") == 0 ||
		strcmp((char *)prefix, ">-") == 0 ||
		strcmp((char *)prefix, "->") == 0 ||
		_stricmp((char *)prefix, "APND") == 0 ||
		_stricmp((char *)prefix, "APPND") == 0 ||
		_stricmp((char *)prefix, "APPEND") == 0);
}


int is_text(unsigned char *prefix)
{
	return(_stricmp((char *)prefix, "T") == 0 ||
		_stricmp((char *)prefix, "TXT") == 0 ||
		_stricmp((char *)prefix, "TEXT") == 0);
}


int is_local_label(char *alpha)
{
	int i;

	/* "A..J" */
	for (i = 0x66; i <= 0x6F; ++i) {
		if (strcmp(alpha, (char *)postfix00_7F[i]) == 0) {
			return(i);
		}
	}

	/* "a..e" */
	for (i = 0x7B; i <= 0x7F; ++i) {
		if (strcmp(alpha, (char *)postfix00_7F[i]) == 0) {
			return(i);
		}
	}

	return(0);
}


int get_key(unsigned char *key)
{
	long rc;
	int row, col, shift;
	char *pkey, *stop;
	unsigned char lbuffer[MAX_LINE];

	strcpy((char *)lbuffer, (char *)key);
	if ((pkey = strchr((char *)lbuffer, ':'))) {
		*pkey++ = '\0';
		if (*lbuffer == '\0' ||
			_stricmp((char *)lbuffer, "Key") == 0) {
			rc = strtol((char *)pkey, &stop, 10);

			/* shift key? */
			if (rc >= 0)
				shift = 0;
			else {
				shift = 8;
				rc = -rc;
			}

			/* */
			/* row = 1..8 */
			/* col = 1..5 */
			/* */
			if (rc <= 85) {
				row = rc / 10;
				col = rc - row * 10;
				if (row >= 1 && row <= 8 &&
					col >= 1 && col <= 5 &&
					(row <= 3 || col < 5) &&
					(row != 3 || col != 1)) {
					return(((col - 1) << 4) + row + shift);
				}
			}
		}
	}

	return(0);
}

char get_xdigit(unsigned char xdigit)
{
	if (isdigit(xdigit))
		return(xdigit - '0');
	else
		return(toupper(xdigit) - 'A' + 10);
}


int compile_args(unsigned char *code_buffer,
	int line_argc,
	unsigned char *line_argv[])
{
	int base, size, count;
	unsigned char lbuffer[MAX_LINE];
	unsigned char num_buffer[MAX_NUMERIC + 1];
	unsigned char text_buffer[MAX_ALPHA + 1];

	base = 0;
	if (line_numbers && isdigit(line_argv[0][0])) {
		line_argc -= 1;
		base += 1;
	}

	count = 0;
	if (line_argc == 1) {
		if (get_numeric_prefix(num_buffer, line_argv[base + 0])) {
			if (fnumeric) {
				code_buffer[0] = '\0';
				count = 1 + compile_num(&code_buffer[1], num_buffer);
			}
			else {
				fnumeric = 1;
				count = compile_num(code_buffer, num_buffer);
			}
		}
		else {
			fnumeric = 0;
			if (get_text_prefix(text_buffer, line_argv[base + 0], &size)) {
				count = compile_text(code_buffer, text_buffer, size);
			}
			else {
				count = compile_arg1(code_buffer, line_argv[base + 0]);
			}
		}
	}
	else if (line_argc == 2) {
		/* make string copy */
		strcpy((char *)lbuffer, (char *)line_argv[base + 0]);
		strcat((char *)lbuffer, (char *)line_argv[base + 1]);

		if (get_numeric_prefix(num_buffer, lbuffer)) {
			if (fnumeric) {
				code_buffer[0] = '\0';
				count = 1 + compile_num(&code_buffer[1], num_buffer);
			}
			else {
				fnumeric = 1;
				count = compile_num(code_buffer, num_buffer);
			}
		}
		else {
			fnumeric = 0;
			if (get_text_prefix(text_buffer, lbuffer, &size)) {
				count = compile_text(code_buffer, text_buffer, size);
			}
			else if (get_alpha_postfix(text_buffer, line_argv[base + 1])) {
				count = compile_alpha(code_buffer, line_argv[base + 0],
					text_buffer);
			}
			else {
				count = compile_arg2(code_buffer, line_argv[base + 0],
					line_argv[base + 1]);
			}
		}
	}
	else if (line_argc == 3) {
		if (get_alpha_postfix(text_buffer, line_argv[base + 1])) {
			count = compile_label(code_buffer, line_argv[base + 0],
				text_buffer, line_argv[base + 2]);
		}
		else {
			count = compile_arg3(code_buffer, line_argv[base + 0],
				line_argv[base + 1], line_argv[base + 2]);
		}
	}
	else if (line_argc == 4) {
		/* make string copy */
		strcpy((char *)lbuffer, (char *)line_argv[base + 2]);
		strcat((char *)lbuffer, (char *)line_argv[base + 3]);

		if (get_alpha_postfix(text_buffer, line_argv[base + 1])) {
			count = compile_label(code_buffer, line_argv[base + 0],
				text_buffer, lbuffer);
		}
	}

	return(count);
}

