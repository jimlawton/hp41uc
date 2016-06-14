/*
HP41UC.EXE
User-Code File Converter/Compiler/De-compiler/Barcode Generator.
Copyright (c) Leo Duran, 2000-2016.  All rights reserved.

Build environment: Microsoft Visual Studio 32-bit compiler.
*/

/*
This file is part of HP41UC.EXE.

HP41UC.EXE is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

HP41UC.EXE is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with HP41UC.EXE.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "hp41uc.h"
#include "decomp.h"

int decompile(unsigned char *out_buffer, int out_size,
	unsigned char **pin_buffer, int *pin_count,
	int *ppending, int *pend)
{
	int done = 0;
	int consumed = 0;
	int produced = 0;
	int i, j, n, m;
	unsigned char c, *inp, *outp;
	static int end = 0;
	static int count;
	static int line = 1;
	static int numeric = 0;
	static int synth_flag;
	static int synth_count;
	static DECOMPILE_STATE state = BYTE1;

	inp = *pin_buffer;
	outp = out_buffer;
	do {
		switch (state) {
		case BYTE1:
			c = *inp;
			if (c >= 0x10 && c <= 0x1C) {
				i = c - 1;
				n = strlen(single01_1C[i]);
				j = produced + n;
				if (line_numbers && !numeric) {
					if (line > 999)
						j += 5;
					else
						j += 4;
				}
				if (j > out_size)
					done = 1;
				else {
					if (line_numbers && !numeric) {
						if (line > 99)
							sprintf(buffer6, "%d ", line);
						else
							sprintf(buffer6, " %02d ", line);
						++line;
						if (line > 1999)
							line = 1;
						j = strlen(buffer6);
						memcpy(outp, buffer6, j);
						outp += j;
						produced += j;
					}
					memcpy(outp, single01_1C[i], n);
					outp += n;
					produced += n;
					++inp;
					++consumed;
					numeric = 1;
				}
			}
			else if (numeric) {
				numeric = 0;
				state = APPEND_CR;
			}
			else {
				if (c == 0x00) {
					++consumed;
					++inp;
				}
				else if (c >= 0x01 && c <= 0x0F) {
					i = c - 1;
					n = strlen(single01_1C[i]);
					j = produced + n;
					if (line_numbers) {
						if (line > 999)
							j += 5;
						else
							j += 4;
					}
					if (j > out_size)
						done = 1;
					else {
						if (line_numbers) {
							if (line > 99)
								sprintf(buffer6, "%d ", line);
							else
								sprintf(buffer6, " %02d ", line);
							++line;
							if (line > 1999)
								line = 1;
							j = strlen(buffer6);
							memcpy(outp, buffer6, j);
							outp += j;
							produced += j;
						}
						memcpy(outp, single01_1C[i], n);
						outp += n;
						produced += n;
						++inp;
						++consumed;
						state = APPEND_CR;
					}
				}
				else if (c == 0x1D) {
					n = strlen(prefixGTO);
					j = produced + n;
					if (line_numbers) {
						if (line > 999)
							j += 5;
						else
							j += 4;
					}
					if (j > out_size)
						done = 1;
					else {
						if (line_numbers) {
							if (line > 99)
								sprintf(buffer6, "%d ", line);
							else
								sprintf(buffer6, " %02d ", line);
							++line;
							if (line > 1999)
								line = 1;
							j = strlen(buffer6);
							memcpy(outp, buffer6, j);
							outp += j;
							produced += j;
						}
						memcpy(outp, prefixGTO, n);
						outp += n;
						produced += n;
						++inp;
						++consumed;
						synth_flag = 0;
						synth_count = 1;
						synth_buffer[0] = c;
						state = ALPHA_OPEN_QUOTE;
					}
				}
				else if (c == 0x1E) {
					n = strlen(prefixXEQ);
					j = produced + n;
					if (line_numbers) {
						if (line > 999)
							j += 5;
						else
							j += 4;
					}
					if (j > out_size)
						done = 1;
					else {
						if (line_numbers) {
							if (line > 99)
								sprintf(buffer6, "%d ", line);
							else
								sprintf(buffer6, " %02d ", line);
							++line;
							if (line > 1999)
								line = 1;
							j = strlen(buffer6);
							memcpy(outp, buffer6, j);
							outp += j;
							produced += j;
						}
						memcpy(outp, prefixXEQ, n);
						outp += n;
						produced += n;
						++inp;
						++consumed;
						synth_flag = 0;
						synth_count = 1;
						synth_buffer[0] = c;
						state = ALPHA_OPEN_QUOTE;
					}
				}
				else if (c == 0x1F) {
					n = strlen(prefixW);
					j = produced + n;
					if (line_numbers) {
						if (line > 999)
							j += 5;
						else
							j += 4;
					}
					if (j > out_size)
						done = 1;
					else {
						if (line_numbers) {
							if (line > 99)
								sprintf(buffer6, "%d ", line);
							else
								sprintf(buffer6, " %02d ", line);
							++line;
							if (line > 1999)
								line = 1;
							j = strlen(buffer6);
							memcpy(outp, buffer6, j);
							outp += j;
							produced += j;
						}
						memcpy(outp, prefixW, n);
						outp += n;
						produced += n;
						++inp;
						++consumed;
						synth_flag = 0;
						synth_count = 1;
						synth_buffer[0] = c;
						state = ALPHA_OPEN_QUOTE;
					}
				}
				else if (c >= 0x20 && c <= 0x8F) {
					i = c - 0x20;
					n = strlen(single20_8F[i]);
					j = produced + n;
					if (line_numbers) {
						if (line > 999)
							j += 5;
						else
							j += 4;
					}
					if (j > out_size)
						done = 1;
					else {
						if (line_numbers) {
							if (line > 99)
								sprintf(buffer6, "%d ", line);
							else
								sprintf(buffer6, " %02d ", line);
							++line;
							if (line > 1999)
								line = 1;
							j = strlen(buffer6);
							memcpy(outp, buffer6, j);
							outp += j;
							produced += j;
						}
						memcpy(outp, single20_8F[i], n);
						outp += n;
						produced += n;
						++inp;
						++consumed;
						state = APPEND_CR;
					}
				}
				else if (c >= 0x90 && c <= 0x9F) {
					i = c - 0x90;
					n = strlen(prefix90_9F[i]);
					j = produced + n;
					if (line_numbers) {
						if (line > 999)
							j += 5;
						else
							j += 4;
					}
					if (j > out_size)
						done = 1;
					else {
						if (line_numbers) {
							if (line > 99)
								sprintf(buffer6, "%d ", line);
							else
								sprintf(buffer6, " %02d ", line);
							++line;
							if (line > 1999)
								line = 1;
							j = strlen(buffer6);
							memcpy(outp, buffer6, j);
							outp += j;
							produced += j;
						}
						memcpy(outp, prefix90_9F[i], n);
						outp += n;
						produced += n;
						++inp;
						++consumed;
						state = BYTE2_POSTFIX;
					}
				}
				else if (c >= 0xA0 && c <= 0xA7) {
					++inp;
					++consumed;
					count = c & 0x07;
					state = BYTE2_XROM;
				}
				else if (c >= 0xA8 && c <= 0xAD) {
					i = c - 0xA8;
					n = strlen(prefixA8_AD[i]);
					j = produced + n;
					if (line_numbers) {
						if (line > 999)
							j += 5;
						else
							j += 4;
					}
					if (j > out_size)
						done = 1;
					else {
						if (line_numbers) {
							if (line > 99)
								sprintf(buffer6, "%d ", line);
							else
								sprintf(buffer6, " %02d ", line);
							++line;
							if (line > 1999)
								line = 1;
							j = strlen(buffer6);
							memcpy(outp, buffer6, j);
							outp += j;
							produced += j;
						}
						memcpy(outp, prefixA8_AD[i], n);
						outp += n;
						produced += n;
						++inp;
						++consumed;
						state = BYTE2_POSTFIX;
					}
				}
				else if (c == 0xAE) {
					++inp;
					++consumed;
					state = BYTE2_IND;
				}
				else if (c == 0xAF) {
					++inp;
					++consumed;
					synth_buffer[0] = c;
					state = BYTE2_IND_NOP;
				}
				else if (c == 0xB0) {
					++inp;
					++consumed;
					synth_buffer[0] = c;
					state = BYTE2_NOP;
				}
				else if (c >= 0xB1 && c <= 0xBF) {
					i = c - 0xB1;
					n = strlen(prefixB1_BF[i]);
					j = produced + n;
					if (line_numbers) {
						if (line > 999)
							j += 5;
						else
							j += 4;
					}
					if (j > out_size)
						done = 1;
					else {
						if (line_numbers) {
							if (line > 99)
								sprintf(buffer6, "%d ", line);
							else
								sprintf(buffer6, " %02d ", line);
							++line;
							if (line > 1999)
								line = 1;
							j = strlen(buffer6);
							memcpy(outp, buffer6, j);
							outp += j;
							produced += j;
						}
						memcpy(outp, prefixB1_BF[i], n);
						outp += n;
						produced += n;
						++inp;
						++consumed;
						state = BYTE2_GTO_IGNORE;
					}
				}
				else if (c >= 0xC0 && c <= 0xCD) {
					++inp;
					++consumed;
					synth_flag = 0;
					synth_count = 1;
					synth_buffer[0] = c;
					state = BYTE2_GLOBAL_IGNORE;
				}
				else if (c == 0xCE || c == 0xCF) {
					i = c - 0xCE;
					n = strlen(prefixCE_CF[i]);
					j = produced + n;
					if (line_numbers) {
						if (line > 999)
							j += 5;
						else
							j += 4;
					}
					if (j > out_size)
						done = 1;
					else {
						if (line_numbers) {
							if (line > 99)
								sprintf(buffer6, "%d ", line);
							else
								sprintf(buffer6, " %02d ", line);
							++line;
							if (line > 1999)
								line = 1;
							j = strlen(buffer6);
							memcpy(outp, buffer6, j);
							outp += j;
							produced += j;
						}
						memcpy(outp, prefixCE_CF[i], n);
						outp += n;
						produced += n;
						++inp;
						++consumed;
						state = BYTE2_POSTFIX;
					}
				}
				else if (c >= 0xD0 && c <= 0xDF) {
					n = strlen(prefixGTO);
					j = produced + n;
					if (line_numbers) {
						if (line > 999)
							j += 5;
						else
							j += 4;
					}
					if (j > out_size)
						done = 1;
					else {
						if (line_numbers) {
							if (line > 99)
								sprintf(buffer6, "%d ", line);
							else
								sprintf(buffer6, " %02d ", line);
							++line;
							if (line > 1999)
								line = 1;
							j = strlen(buffer6);
							memcpy(outp, buffer6, j);
							outp += j;
							produced += j;
						}
						memcpy(outp, prefixGTO, n);
						outp += n;
						produced += n;
						++inp;
						++consumed;
						state = BYTE2_OF3_IGNORE;
					}
				}
				else if (c >= 0xE0 && c <= 0xEF) {
					n = strlen(prefixXEQ);
					j = produced + n;
					if (line_numbers) {
						if (line > 999)
							j += 5;
						else
							j += 4;
					}
					if (j > out_size)
						done = 1;
					else {
						if (line_numbers) {
							if (line > 99)
								sprintf(buffer6, "%d ", line);
							else
								sprintf(buffer6, " %02d ", line);
							++line;
							if (line > 1999)
								line = 1;
							j = strlen(buffer6);
							memcpy(outp, buffer6, j);
							outp += j;
							produced += j;
						}
						memcpy(outp, prefixXEQ, n);
						outp += n;
						produced += n;
						++inp;
						++consumed;
						state = BYTE2_OF3_IGNORE;
					}
				}
				else {
					++inp;
					++consumed;
					count = c & 0x0F;
					synth_flag = 0;
					synth_count = 1;
					synth_buffer[0] = c;
					state = TEXT_OPEN_QUOTE;
				}
			}
			break;

		case BYTE2_IND_NOP:
			c = *inp;
			if (c < 0x80) {
				n = strlen(prefixGTO_IND_NOP);
				j = produced + n;
				if (line_numbers) {
					if (line > 999)
						j += 5;
					else
						j += 4;
				}
				if (j > out_size)
					done = 1;
				else {
					if (line_numbers) {
						if (line > 99)
							sprintf(buffer6, "%d ", line);
						else
							sprintf(buffer6, " %02d ", line);
						++line;
						if (line > 1999)
							line = 1;
						j = strlen(buffer6);
						memcpy(outp, buffer6, j);
						outp += j;
						produced += j;
					}
					memcpy(outp, prefixGTO_IND_NOP, n);
					outp += n;
					produced += n;
					++inp;
					++consumed;
					synth_flag = 1;
					synth_count = 2;
					synth_buffer[1] = c;
					state = APPEND_SYNTH;
				}
			}
			else {
				n = strlen(prefixXEQ_IND_NOP);
				j = produced + n;
				if (line_numbers) {
					if (line > 999)
						j += 5;
					else
						j += 4;
				}
				if (j > out_size)
					done = 1;
				else {
					if (line_numbers) {
						if (line > 99)
							sprintf(buffer6, "%d ", line);
						else
							sprintf(buffer6, " %02d ", line);
						++line;
						if (line > 1999)
							line = 1;
						j = strlen(buffer6);
						memcpy(outp, buffer6, j);
						outp += j;
						produced += j;
					}
					memcpy(outp, prefixXEQ_IND_NOP, n);
					outp += n;
					produced += n;
					++inp;
					++consumed;
					synth_flag = 1;
					synth_count = 2;
					synth_buffer[1] = c;
					state = APPEND_SYNTH;
				}
			}
			break;

		case BYTE2_NOP:
			c = *inp;
			n = strlen(prefixGTO_NOP);
			j = produced + n;
			if (line_numbers) {
				if (line > 999)
					j += 5;
				else
					j += 4;
			}
			if (j > out_size)
				done = 1;
			else {
				if (line_numbers) {
					if (line > 99)
						sprintf(buffer6, "%d ", line);
					else
						sprintf(buffer6, " %02d ", line);
					++line;
					if (line > 1999)
						line = 1;
					j = strlen(buffer6);
					memcpy(outp, buffer6, j);
					outp += j;
					produced += j;
				}
				memcpy(outp, prefixGTO_NOP, n);
				outp += n;
				produced += n;
				++inp;
				++consumed;
				synth_flag = 1;
				synth_count = 2;
				synth_buffer[1] = c;
				state = APPEND_SYNTH;
			}
			break;

		case BYTE2_GTO_IGNORE:
			++inp;
			++consumed;
			state = APPEND_CR;
			break;

		case BYTE2_XROM:
			c = *inp;
			m = (count << 2) + (c >> 6);
			i = c & 0x3F;
			if (do_xrom23 && m == 23 &&
				((i >= 1 && i <= 7) || (i >= 9 && i <= 17) ||
				(i >= 19 && i <= 52) || (i >= 54 && i <= 62))) {
				n = strlen(xrom23[i]);
				j = produced + n;
				if (line_numbers) {
					if (line > 999)
						j += 5;
					else
						j += 4;
				}
				if (j > out_size)
					done = 1;
				else {
					if (line_numbers) {
						if (line > 99)
							sprintf(buffer6, "%d ", line);
						else
							sprintf(buffer6, " %02d ", line);
						++line;
						if (line > 1999)
							line = 1;
						j = strlen(buffer6);
						memcpy(outp, buffer6, j);
						outp += j;
						produced += j;
					}
					memcpy(outp, xrom23[i], n);
					outp += n;
					produced += n;
					++inp;
					++consumed;
					state = APPEND_CR;
				}
			}
			else if (do_xrom25 && m == 25 &&
				((i >= 1 && i <= 47) || (i >= 49 && i <= 62))) {
				n = strlen(xrom25[i]);
				j = produced + n;
				if (line_numbers) {
					if (line > 999)
						j += 5;
					else
						j += 4;
				}
				if (j > out_size)
					done = 1;
				else {
					if (line_numbers) {
						if (line > 99)
							sprintf(buffer6, "%d ", line);
						else
							sprintf(buffer6, " %02d ", line);
						++line;
						if (line > 1999)
							line = 1;
						j = strlen(buffer6);
						memcpy(outp, buffer6, j);
						outp += j;
						produced += j;
					}
					memcpy(outp, xrom25[i], n);
					outp += n;
					produced += n;
					++inp;
					++consumed;
					state = APPEND_CR;
				}
			}
			else if (do_xrom26 && m == 26 &&
				((i >= 1 && i <= 29) || (i >= 31 && i <= 35))) {
				n = strlen(xrom26[i]);
				j = produced + n;
				if (line_numbers) {
					if (line > 999)
						j += 5;
					else
						j += 4;
				}
				if (j > out_size)
					done = 1;
				else {
					if (line_numbers) {
						if (line > 99)
							sprintf(buffer6, "%d ", line);
						else
							sprintf(buffer6, " %02d ", line);
						++line;
						if (line > 1999)
							line = 1;
						j = strlen(buffer6);
						memcpy(outp, buffer6, j);
						outp += j;
						produced += j;
					}
					memcpy(outp, xrom26[i], n);
					outp += n;
					produced += n;
					++inp;
					++consumed;
					state = APPEND_CR;
				}
			}
			else if (do_xrom28 && m == 28 &&
				((i >= 1 && i <= 2) || (i >= 4 && i <= 24) ||
				(i >= 27 && i <= 41))) {
				n = strlen(xrom28[i]);
				j = produced + n;
				if (line_numbers) {
					if (line > 999)
						j += 5;
					else
						j += 4;
				}
				if (j > out_size)
					done = 1;
				else {
					if (line_numbers) {
						if (line > 99)
							sprintf(buffer6, "%d ", line);
						else
							sprintf(buffer6, " %02d ", line);
						++line;
						if (line > 1999)
							line = 1;
						j = strlen(buffer6);
						memcpy(outp, buffer6, j);
						outp += j;
						produced += j;
					}
					memcpy(outp, xrom28[i], n);
					outp += n;
					produced += n;
					++inp;
					++consumed;
					state = APPEND_CR;
				}
			}
			else {
				n = strlen(prefixXROM);
				j = produced + n;
				if (line_numbers) {
					if (line > 999)
						j += 5;
					else
						j += 4;
				}
				if (j > out_size)
					done = 1;
				else {
					sprintf(buffer6, "%02d", m);
					memcpy(&prefixXROM[5], buffer6, 2);
					sprintf(buffer6, "%02d", i);
					memcpy(&prefixXROM[8], buffer6, 2);
					if (line_numbers) {
						if (line > 99)
							sprintf(buffer6, "%d ", line);
						else
							sprintf(buffer6, " %02d ", line);
						++line;
						if (line > 1999)
							line = 1;
						j = strlen(buffer6);
						memcpy(outp, buffer6, j);
						outp += j;
						produced += j;
					}
					memcpy(outp, prefixXROM, n);
					outp += n;
					produced += n;
					++inp;
					++consumed;
					state = APPEND_CR;
				}
			}
			break;

		case BYTE2_IND:
			c = *inp;
			if (c < 0x80) {
				i = c;
				n = strlen(prefixGTO_IND);
				m = strlen(postfix00_7F[i]);
				j = produced + n + m;
				if (line_numbers) {
					if (line > 999)
						j += 5;
					else
						j += 4;
				}
				if (j > out_size)
					done = 1;
				else {
					if (line_numbers) {
						if (line > 99)
							sprintf(buffer6, "%d ", line);
						else
							sprintf(buffer6, " %02d ", line);
						++line;
						if (line > 1999)
							line = 1;
						j = strlen(buffer6);
						memcpy(outp, buffer6, j);
						outp += j;
						produced += j;
					}
					memcpy(outp, prefixGTO_IND, n);
					outp += n;
					memcpy(outp, postfix00_7F[i], m);
					outp += m;
					produced += n + m;
					++inp;
					++consumed;
					state = APPEND_CR;
				}
			}
			else {
				i = c & 0x7F;
				n = strlen(prefixXEQ_IND);
				m = strlen(postfix00_7F[i]);
				j = produced + n + m;
				if (line_numbers) {
					if (line > 999)
						j += 5;
					else
						j += 4;
				}
				if (j > out_size)
					done = 1;
				else {
					if (line_numbers) {
						if (line > 99)
							sprintf(buffer6, "%d ", line);
						else
							sprintf(buffer6, " %02d ", line);
						++line;
						if (line > 1999)
							line = 1;
						j = strlen(buffer6);
						memcpy(outp, buffer6, j);
						outp += j;
						produced += j;
					}
					memcpy(outp, prefixXEQ_IND, n);
					outp += n;
					memcpy(outp, postfix00_7F[i], m);
					outp += m;
					produced += n + m;
					++inp;
					++consumed;
					state = APPEND_CR;
				}
			}
			break;

		case BYTE2_POSTFIX:
			i = *inp;
			if (i < 0x80) {
				n = strlen(postfix00_7F[i]);
				j = produced + n;
				if (j > out_size)
					done = 1;
				else {
					memcpy(outp, postfix00_7F[i], n);
					outp += n;
					produced += n;
					++inp;
					++consumed;
					state = APPEND_CR;
				}
			}
			else {
				i &= 0x7F;
				n = strlen(postfixIND);
				m = strlen(postfix00_7F[i]);
				j = produced + n + m;
				if (j > out_size)
					done = 1;
				else {
					memcpy(outp, postfixIND, n);
					outp += n;
					memcpy(outp, postfix00_7F[i], m);
					outp += m;
					produced += n + m;
					++inp;
					++consumed;
					state = APPEND_CR;
				}
			}
			break;

		case BYTE2_OF3_IGNORE:
			++inp;
			++consumed;
			state = BYTE3_POSTFIX;
			break;

		case BYTE3_POSTFIX:
			i = (*inp) & 0x7F;
			n = strlen(postfix00_7F[i]);
			j = produced + n;
			if (j > out_size)
				done = 1;
			else {
				memcpy(outp, postfix00_7F[i], n);
				outp += n;
				produced += n;
				++inp;
				++consumed;
				state = APPEND_CR;
			}
			break;

		case BYTE2_GLOBAL_IGNORE:
			c = *inp++;
			++consumed;
			++synth_count;
			synth_buffer[1] = c;
			state = BYTE3_GLOBAL;
			break;

		case BYTE3_GLOBAL:
			c = *inp;
			if (c < 0xF0) {
				n = strlen(prefixEND);
				j = produced + n;
				if (line_numbers) {
					if (line > 999)
						j += 5;
					else
						j += 4;
				}
				if (j > out_size)
					done = 1;
				else {
					if (line_numbers) {
						if (line > 99)
							sprintf(buffer6, "%d ", line);
						else
							sprintf(buffer6, " %02d ", line);
						++line;
						if (line > 1999)
							line = 1;
						j = strlen(buffer6);
						memcpy(outp, buffer6, j);
						outp += j;
						produced += j;
					}
					memcpy(outp, prefixEND, n);
					outp += n;
					produced += n;
					++inp;
					++consumed;
					end = 1;
					state = APPEND_CR;
				}
			}
			else {
				n = strlen(prefixLBL);
				j = produced + n + 1;
				if (line_numbers) {
					if (line > 999)
						j += 5;
					else
						j += 4;
				}
				if (j > out_size)
					done = 1;
				else {
					if (line_numbers) {
						if (line > 99)
							sprintf(buffer6, "%d ", line);
						else
							sprintf(buffer6, " %02d ", line);
						++line;
						if (line > 1999)
							line = 1;
						j = strlen(buffer6);
						memcpy(outp, buffer6, j);
						outp += j;
						produced += j;
					}
					memcpy(outp, prefixLBL, n);
					outp += n;
					*outp++ = '\"';
					produced += n + 1;
					++inp;
					++consumed;
					++synth_count;
					synth_buffer[2] = c;
					count = c & 0x0F;
					if (count)
						state = BYTE4_GLOBAL_IGNORE;
					else {
						synth_flag = 1;
						state = CLOSE_QUOTE;
					}
				}
			}
			break;

		case BYTE4_GLOBAL_IGNORE:
			c = *inp++;
			++consumed;
			++synth_count;
			synth_buffer[3] = c;
			--count;
			if (count)
				state = BYTE_ALPHA;
			else {
				synth_flag = 1;
				state = CLOSE_QUOTE;
			}
			break;

		case ALPHA_OPEN_QUOTE:
			c = *inp++;
			++consumed;
			*outp++ = '\"';
			++produced;
			++synth_count;
			synth_buffer[1] = c;
			if (c <= 0xF0) {
				synth_flag = 1;
				state = CLOSE_QUOTE;
			}
			else {
				count = c & 0x0F;
				state = BYTE_ALPHA;
			}
			break;

		case TEXT_OPEN_QUOTE:
			if (count)
				j = produced + 2;
			else
				j = produced + 1;
			if (line_numbers) {
				if (line > 999)
					j += 5;
				else
					j += 4;
			}
			if (j > out_size)
				done = 1;
			else {
				if (line_numbers) {
					if (line > 99)
						sprintf(buffer6, "%d ", line);
					else
						sprintf(buffer6, " %02d ", line);
					++line;
					if (line > 1999)
						line = 1;
					j = strlen(buffer6);
					memcpy(outp, buffer6, j);
					outp += j;
					produced += j;
				}

				if (count) {
					c = *inp++;
					++consumed;
					if (c == 0x7F) {
						if (text_append) {
							*outp++ = '\"';
							*outp++ = 0xC3;
						}
						else {
							*outp++ = '>';
							*outp++ = '\"';
						}
					}
					else {
						*outp++ = '\"';
						if (is_nodisplay(c)) {
#if DO_ESC
							*outp++ = '\\';
							sprintf(buffer6, "%02X", c);
							memcpy(outp, buffer6, 2);
							outp += 2;
							produced += 2;
#else
							*outp++ = 0x04;
#endif
							synth_flag = 1;
						}
						else {
							*outp++ = c;
						}
					}
					--count;
					produced += 2;
					++synth_count;
					synth_buffer[1] = c;
				}
				else {
					*outp++ = '\"';
					++produced;
					synth_flag = 1;
				}
				if (count)
					state = BYTE_ALPHA;
				else
					state = CLOSE_QUOTE;
			}
			break;

		case BYTE_ALPHA:
			c = *inp++;
			++consumed;
			if (is_nodisplay(c)) {
#if DO_ESC
				*outp++ = '\\';
				sprintf(buffer6, "%02X", c);
				memcpy(outp, buffer6, 2);
				outp += 2;
				produced += 2;
#else
				*outp++ = 0x04;
#endif
				synth_flag = 1;
			}
			else {
				*outp++ = c;
			}
			++produced;
			--count;
			synth_buffer[synth_count++] = c;
			if (count == 0)
				state = CLOSE_QUOTE;
			break;

		case CLOSE_QUOTE:
			*outp++ = '\"';
			++produced;
			if (synth_flag)
				state = APPEND_SYNTH;
			else if (room_for_key(synth_buffer, synth_count))
				state = APPEND_KEY;
			else
				state = APPEND_CR;
			break;

		case APPEND_SYNTH:
			j = produced + (synth_count * 3) + 2;
			j += room_for_key(synth_buffer, synth_count);
			if (j > out_size)
				done = 1;
			else {
				// "text"
				if (synth_buffer[0] > 0xF0 && synth_count > 1)
					i = 1;
				// GTO/XEQ/W "alpha"
				else if (synth_buffer[0] >= 0x1D && synth_buffer[0] <= 0x1F &&
					synth_buffer[1] > 0xF0 && synth_count > 2)
					i = 2;
				// LBL "alpha"
				else if (synth_buffer[0] >= 0xC0 && synth_buffer[0] <= 0xCD &&
					synth_buffer[2] > 0xF1 && synth_count > 4)
					i = 4;
				else
					i = 0;

				*outp++ = 0x20;
				*outp++ = 0x20;
				*outp++ = ';';
				produced += 3;
				if (i) {
					j = room_for_key(synth_buffer, synth_count);
					*outp++ = '\"';
					++produced;
					synth_count -= i;
					do {
						c = synth_buffer[i++];
						if (c >= 0x20 && c <= 0x7E) {
							*outp++ = c;
							++produced;
						}
						else {
#if DO_ESC
							*outp++ = 0x04;
							++produced;
#else
							*outp++ = '\\';
							sprintf(buffer6, "%02X", c);
							memcpy(outp, buffer6, 2);
							outp += 2;
							produced += 3;
#endif
						}
					} while (--synth_count);
					*outp++ = '\"';
					++produced;
					if (j) {
						copy_key(outp, synth_buffer[3]);
						outp += j;
						produced += j;
					}
					state = APPEND_CR;
				}
				else {
					do {
						c = synth_buffer[i++];
						sprintf(buffer6, "%02X", c);
						memcpy(outp, buffer6, 2);
						outp += 2;
						produced += 2;
						--synth_count;
						if (synth_count) {
							*outp++ = ',';
							++produced;
						}
					} while (synth_count);
					state = APPEND_CR;
				}
			}
			break;

		case APPEND_KEY:
			i = room_for_key(synth_buffer, synth_count);
			j = produced + i + 3;
			if (j > out_size)
				done = 1;
			else {
				*outp++ = 0x20;
				*outp++ = 0x20;
				*outp++ = ';';
				produced += 3;
				copy_key(outp, synth_buffer[3]);
				outp += i;
				produced += i;
				state = APPEND_CR;
			}
			break;

		case APPEND_CR:
			*outp++ = 0x0D;
			++produced;
			state = APPEND_LF;
			break;

		case APPEND_LF:
			*outp++ = 0x0A;
			++produced;
			if (end)
				done = 1;
			state = BYTE1;
			break;
		}

		// exhausted buffers?
		if (produced == out_size ||
			(consumed == *pin_count &&
			state != CLOSE_QUOTE &&
			state != APPEND_SYNTH &&
			state != APPEND_KEY &&
			state != APPEND_CR &&
			state != APPEND_LF &&
			(state != TEXT_OPEN_QUOTE || count)))
			done = 1;

	} while (!done);

	// output pending?
	*ppending = state == CLOSE_QUOTE ||
		state == APPEND_SYNTH ||
		state == APPEND_KEY ||
		state == APPEND_CR ||
		state == APPEND_LF ||
		(state == TEXT_OPEN_QUOTE && !count);

	*pin_buffer += consumed;
	*pin_count -= consumed;
	*pend = end;
	return(produced);
}

int is_nodisplay(unsigned char code)
{
	return(code == 0x00 || code == 0x07 || code == 0x08 ||
		code == 0x09 || code == 0x0A || code == 0x0D ||
		code == 0x1A || code == 0x1B || code == 0xFF);

}

int room_for_key(unsigned char *buffer, int count)
{
	char row;

	if (buffer[0] >= 0xC0 && buffer[0] <= 0xCD &&
		buffer[2] > 0xF1 && buffer[3] && count > 4) {
		row = buffer[3] & 0x0F;
		return((row > 0 && row < 9) ? strlen(pKey) : strlen(psKey));
	}

	return(0);
}

void copy_key(char *buffer, char code)
{
	char row, col;

	row = code & 0x0F;
	if (row > 0 && row < 9) {
		col = (code >> 4) + 1;
		pKey[6] = row + '0';
		pKey[7] = col + '0';
		memcpy(buffer, pKey, strlen(pKey));
	}
	else {
		code -= 8;
		row = code & 0x0F;
		col = (code >> 4) + 1;
		psKey[7] = row + '0';
		psKey[8] = col + '0';
		memcpy(buffer, psKey, strlen(psKey));
	}
}

