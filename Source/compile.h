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

typedef struct {
	char *prefix;
	int  code;
} FCN;

typedef struct {
	char *prefix;
	int  code[2];
} XROM;

/* compiler states */
typedef enum {
	COMPILE_SEEK_START_LINE,
	COMPILE_GET_LINE,
	COMPILE_SEEK_END_LINE,
	COMPILE_LINE_OUTPUT,
	COMPILE_IGNORE_BAD_LINE
} COMPILE_STATE;


int get_line_args(unsigned char *line_argv[], unsigned char **line_ptr);

int compile_args(unsigned char *code_buffer,
	int line_argc,
	unsigned char *line_argv[]);

int compile_num(unsigned char *code, unsigned char *num);
int compile_text(unsigned char *code, unsigned char *text, int count);
int compile_alpha(unsigned char *code, unsigned char *prefix, unsigned char *alpha);
int compile_arg1(unsigned char *code, unsigned char *prefix);
int compile_arg2(unsigned char *code, unsigned char *prefix, unsigned char *postfix);
int compile_arg3(unsigned char *code, unsigned char *prefix, unsigned char *pind, unsigned char *postfix);
int compile_label(unsigned char *code, unsigned char *label, unsigned char *alpha, unsigned char *key);

int get_numeric_prefix(unsigned char *numeric, unsigned char *buffer);

int get_text_prefix(unsigned char *text, unsigned char *buffer, int *pcount);

int get_alpha_postfix(unsigned char *alpha, unsigned char *buffer);

int is_postfix(unsigned char *postfix, int *pindex);

int parse_text(unsigned char *text, unsigned char *buffer, int *pcount);

int is_inquotes(unsigned char *buffer);

int is_append(unsigned char *prefix);

int is_text(unsigned char *prefix);

int is_local_label(char *alpha);

int get_key(unsigned char *key);

char get_xdigit(unsigned char xdigit);

FCN alt_fcn1[] = {
	{"‰+", 0x47},
	{"SIGMA+", 0x47},
	{"‰-", 0x48},
	{"SIGMA-", 0x48},
	{"P->R", 0x4E},
	{"R->P", 0x4F},
	{"X**2", 0x51},
	{"Y**X", 0x53},
	{"E**X", 0x55},
	{"10**X", 0x57},
	{"E**X-1", 0x58},
	{"X!=0?", 0x63},
	{"X<>0?", 0x63},
	{"D->R", 0x6A},
	{"R->D", 0x6B},
	{"CL‰", 0x70},
	{"CLSIGMA", 0x70},
	{"RUP", 0x74},
	{"X!=Y?", 0x79},
	{"X<>Y?", 0x79},
	{"ENTER^", 0x83}
};

FCN alt_fcn2[] = {
	{"STO+", 0x92},
	{"STO-", 0x93},
	{"STO*", 0x94},
	{"STO/", 0x95},
	{"‰REG", 0x99},
	{"SIGREG", 0x99},
	{"SIGMAREG", 0x99}
};

XROM xrom_fcn[] = {
	/* -EXT FCN 2D */
	{"ALENG", { 25, 1 }},
	{"ANUM", { 25, 2 }},
	{"APPCHR", { 25, 3 }},
	{"APPREC", { 25, 4 }},
	{"ARCLREC", { 25, 5 }},
	{"AROT", { 25, 6 }},
	{"ATOX", { 25, 7 }},
	{"CLFL", { 25, 8 }},
	{"CLKEYS", { 25, 9 }},
	{"CRFLAS", { 25, 10 }},
	{"CRFLD", { 25, 11 }},
	{"DELCHR", { 25, 12 }},
	{"DELREC", { 25, 13 }},
	{"EMDIR", { 25, 14 }},
	{"FLSIZE", { 25, 15 }},
	{"GETAS", { 25, 16 }},
	{"GETKEY", { 25, 17 }},
	{"GETP", { 25, 18 }},
	{"GETR", { 25, 19 }},
	{"GETREC", { 25, 20 }},
	{"GETRX", { 25, 21 }},
	{"GETSUB", { 25, 22 }},
	{"GETX", { 25, 23 }},
	{"INSCHR", { 25, 24 }},
	{"INSREC", { 25, 25 }},
	{"PASN", { 25, 26 }},
	{"PCLPS", { 25, 27 }},
	{"POSA", { 25, 28 }},
	{"POSFL", { 25, 29 }},
	{"PSIZE", { 25, 30 }},
	{"PURFL", { 25, 31 }},
	{"RCLFLAG", { 25, 32 }},
	{"RCLPT", { 25, 33 }},
	{"RCLPTA", { 25, 34 }},
	{"REGMOVE", { 25, 35 }},
	{"REGSWAP", { 25, 36 }},
	{"SAVEAS", { 25, 37 }},
	{"SAVEP", { 25, 38 }},
	{"SAVER", { 25, 39 }},
	{"SAVERX", { 25, 40 }},
	{"SAVEX", { 25, 41 }},
	{"SEEKPT", { 25, 42 }},
	{"SEEKPTA", { 25, 43 }},
	{"SIZE?", { 25, 44 }},
	{"STOFLAG", { 25, 45 }},
	{"X<>F", { 25, 46 }},
	{"XTOA", { 25, 47 }},
	/* -CX EXT FCN */
	{"ASROOM", { 25, 49 }},
	{"CLRGX", { 25, 50 }},
	{"ED", { 25, 51 }},
	{"EMDIRX", { 25, 52 }},
	{"EMROOM", { 25, 53 }},
	{"GETKEYX", { 25, 54 }},
	{"RESZFL", { 25, 55 }},
	{"SREG?", { 25, 56 }},
	{"X=NN?", { 25, 57 }},
	{"X#NN?", { 25, 58 }},
	{"X<NN?", { 25, 59 }},
	{"X<=NN?", { 25, 60 }},
	{"X>NN?", { 25, 61 }},
	{"X>=NN?", { 25, 62 }},
	/* -TIME 2C */
	{"ADATE", { 26, 1 }},
	{"ALMCAT", { 26, 2 }},
	{"ALMNOW", { 26, 3 }},
	{"ATIME", { 26, 4 }},
	{"ATIME24", { 26, 5 }},
	{"CLK12", { 26, 6 }},
	{"CLK24", { 26, 7 }},
	{"CLKT", { 26, 8 }},
	{"CLKTD", { 26, 9 }},
	{"CLOCK", { 26, 10 }},
	{"CORRECT", { 26, 11 }},
	{"DATE", { 26, 12 }},
	{"DATE+", { 26, 13 }},
	{"DDAYS", { 26, 14 }},
	{"DMY", { 26, 15 }},
	{"DOW", { 26, 16 }},
	{"MDY", { 26, 17 }},
	{"RCLAF", { 26, 18 }},
	{"RCLSW", { 26, 19 }},
	{"RUNSW", { 26, 20 }},
	{"SETAF", { 26, 21 }},
	{"SETDATE", { 26, 22 }},
	{"SETIME", { 26, 23 }},
	{"SETSW", { 26, 24 }},
	{"STOPSW", { 26, 25 }},
	{"SW", { 26, 26 }},
	{"T+X", { 26, 27 }},
	{"TIME", { 26, 28 }},
	{"XYZALM", { 26, 29 }},
	/* -CX TIME */
	{"CLALMA", { 26, 31 }},
	{"CLALMX", { 26, 32 }},
	{"CLRALMS", { 26, 33 }},
	{"RCLALM", { 26, 34 }},
	{"SWPT", { 26, 35 }},
	/* -PRINTER 2E */
	{"ACA", { 29, 1 }},
	{"ACCHR", { 29, 2 }},
	{"ACCOL", { 29, 3 }},
	{"ACSPEC", { 29, 4 }},
	{"ACX", { 29, 5 }},
	{"BLDSPEC", { 29, 6 }},
	{"PRA", { 29, 8 }},
	{"PRAXIS", { 29, 9 }},
	{"PRBUF", { 29, 10 }},
	{"PRFLAGS", { 29, 11 }},
	{"PRKEYS", { 29, 12 }},
	{"PRPLOT", { 29, 14 }},
	{"PRPLOTP", { 29, 15 }},
	{"PRREG", { 29, 16 }},
	{"PRREGX", { 29, 17 }},
	{"PRS", { 29, 18 }},
	{"PRSTK", { 29, 19 }},
	{"PRX", { 29, 20 }},
	{"REGPLOT", { 29, 21 }},
	{"SKPCHR", { 29, 22 }},
	{"SKPCOL", { 29, 23 }},
	{"STKPLOT", { 29, 24 }},
	{"FMT", { 29, 25 }},
	/* -MASS ST 1H */
	{"CREATE", { 28, 1 }},
	{"DIR", { 28, 2 }},
	{"PURGE", { 28, 4 }},
	{"READA", { 28, 5 }},
	{"READK", { 28, 6 }},
	{"READP", { 28, 7 }},
	{"READR", { 28, 8 }},
	{"READRX", { 28, 9 }},
	{"READS", { 28, 10 }},
	{"READSUB", { 28, 11 }},
	{"RENAME", { 28, 12 }},
	{"SEC", { 28, 13 }},
	{"SEEKR", { 28, 14 }},
	{"UNSEC", { 28, 15 }},
	{"VERIFY", { 28, 16 }},
	{"WRTA", { 28, 17 }},
	{"WRTK", { 28, 18 }},
	{"WRTP", { 28, 19 }},
	{"WRTPV", { 28, 20 }},
	{"WRTR", { 28, 21 }},
	{"WRTRX", { 28, 22 }},
	{"WRTS", { 28, 23 }},
	{"ZERO", { 28, 24 }},
	/* -CTL FNS */
	{"AUTOIO", { 28, 27 }},
	{"FINDIO", { 28, 28 }},
	{"INA", { 28, 29 }},
	{"IND", { 28, 30 }},
	{"INSTAT", { 28, 31 }},
	{"LISTEN", { 28, 32 }},
	{"LOCAL", { 28, 33 }},
	{"MANIO", { 28, 34 }},
	{"OUTA", { 28, 35 }},
	{"PWRDN", { 28, 36 }},
	{"PWRUP", { 28, 37 }},
	{"REMOTE", { 28, 38 }},
	{"SELECT", { 28, 39 }},
	{"STOPIO", { 28, 40 }},
	{"TRIGGER", { 28, 41 }},
	/* -X MASS 1A */
	{"COPYFL", { 23, 1 }},
	{"DIRX", { 23, 2 }},
	{"FLLENG", { 23, 3 }},
	{"FLTYPE", { 23, 4 }},
	{"MCOPY", { 23, 5 }},
	{"MCOPYPV", { 23, 6 }},
	{"MVERIFY", { 23, 7 }},
	/* -X EXT FCN */
	{"ALENGIO", { 23, 9 }},
	{"ANUNDEL", { 23, 10 }},
	{"ATOXL", { 23, 11 }},
	{"ATOXR", { 23, 12 }},
	{"ATOXX", { 23, 13 }},
	{"XTOAL", { 23, 14 }},
	{"XTOAR", { 23, 15 }},
	{"X<>FIO", { 23, 16 }},
	{"YTOAX", { 23, 17 }},
	/* -X CTL FNS*/
	{"AID", { 23, 19 }},
	{"CLRDEV", { 23, 20 }},
	{"CLRLOOP", { 23, 21 }},
	{"DEVL", { 23, 22 }},
	{"DEVT", { 23, 23 }},
	{"FINDAID", { 23, 24 }},
	{"ID", { 23, 25 }},
	{"INAC", { 23, 26 }},
	{"INACL", { 23, 27 }},
	{"INAE", { 23, 28 }},
	{"INAN", { 23, 29 }},
	{"INXB", { 23, 30 }},
	{"INP", { 23, 31 }},
	{"LOCK", { 23, 32 }},
	{"NLOOP", { 23, 33 }},
	{"NOTREM", { 23, 34 }},
	{"OUTAC", { 23, 35 }},
	{"OUTACL", { 23, 36 }},
	{"OUTAE", { 23, 37 }},
	{"OUTAN", { 23, 38 }},
	{"OUTXB", { 23, 39 }},
	{"OUTP", { 23, 40 }},
	{"POLL", { 23, 41 }},
	{"POLLD", { 23, 42 }},
	{"POLLE", { 23, 43 }},
	{"POLLUNC", { 23, 44 }},
	{"RCLSEL", { 23, 45 }},
	{"SRQ?", { 23, 46 }},
	{"STAT", { 23, 47 }},
	{"XFER", { 23, 48 }},
	{"XFERC", { 23, 49 }},
	{"XREFCL", { 23, 50 }},
	{"XFERE", { 23, 51 }},
	{"XFERN", { 23, 52 }},
	/* -ADV CTL FN */
	{"ADROFF", { 23, 54 }},
	{"ADRON", { 23, 55 }},
	{"DDL", { 23, 56 }},
	{"DDT", { 23, 57 }},
	{"LAD", { 23, 58 }},
	{"SEND", { 23, 59 }},
	{"TAD", { 23, 60 }},
	{"UNL", { 23, 61 }},
	/* PLOTTER 1A- */
	{"CLIPUU", { 17, 1 }},
	{"CSIZE", { 17, 2 }},
	{"CSIZEO", { 17, 3 }},
	{"DGTIZE", { 17, 4 }},
	{"DRAW", { 17, 5 }},
	{"FRAME", { 17, 6 }},
	{"GCLEAR", { 17, 7 }},
	{"IDRAW", { 17, 8 }},
	{"IMOVE", { 17, 9 }},
	{"IPLOT", { 17, 10 }},
	{"LABEL", { 17, 11 }},
	{"LDIR", { 17, 12 }},
	{"LIMIT", { 17, 13 }},
	{"LOCATD", { 17, 14 }},
	{"LOCATE", { 17, 15 }},
	{"LORG", { 17, 16 }},
	{"LTYPE", { 17, 17 }},
	{"LTYPEO", { 17, 18 }},
	{"LXAXIS", { 17, 19 }},
	{"LYAXIS", { 17, 20 }},
	{"MOVE", { 17, 21 }},
	{"PEN", { 17, 22 }},
	{"PENDN", { 17, 23 }},
	{"PENUP", { 17, 24 }},
	{"PINIT", { 17, 25 }},
	{"PLOT", { 17, 26 }},
	{"PLREGX", { 17, 27 }},
	{"RATIO", { 17, 28 }},
	{"RPLOT", { 17, 29 }},
	{"SCALE", { 17, 30 }},
	{"SETGU", { 17, 31 }},
	{"SETUU", { 17, 32 }},
	{"TICLEN", { 17, 33 }},
	{"UNCLIP", { 17, 34 }},
	{"WHERE", { 17, 35 }},
	{"XAXIS", { 17, 36 }},
	{"XAXISO", { 17, 37 }},
	{"YAXIS", { 17, 38 }},
	{"YAXISO", { 17, 39 }},
	/* PLOTTER 2A- */
	{"PCLBUF", { 18, 1 }},
	{"PDIR", { 18, 2 }},
	{"PRCL", { 18, 3 }},
	{"NEWPLOT", { 18, 4 }},
	{"REPLOT", { 18, 5 }},
	{"PLINIT", { 18, 6 }},
	{"PLTUXY", { 18, 7 }},
	{"PLANOT", { 18, 8 }},
	{"Y?", { 18, 9 }},
	{"X?", { 18, 10 }},
	{"BC", { 18, 11 }},
	{"BCA", { 18, 12 }},
	{"BCAA", { 18, 13 }},
	{"BCCKSM", { 18, 14 }},
	{"BCO", { 18, 15 }},
	{"BCP", { 18, 16 }},
	{"BCREGX", { 18, 17 }},
	{"BCSIZE", { 18, 18 }},
	{"BCX", { 18, 19 }},
	{"BCXS", { 18, 20 }},
	/* -HP-IL DEV */
	{"A-BUF", { 22, 1 }},
	{"A=BUF?", { 22, 2 }},
	{"A=BUFX?", { 22, 3 }},
	{"AAD", { 22, 4 }},
	{"AAU", { 22, 5 }},
	{"AIPT", { 22, 6 }},
	{"BSIZE?", { 22, 7 }},
	{"BSIZEX", { 22, 8 }},
	{"BUF-AX", { 22, 9 }},
	{"BUF-RGX", { 22, 10 }},
	{"BUF-XA", { 22, 11 }},
	{"BUF-XB", { 22, 12 }},
	{"CF33", { 22, 13 }},
	{"CMD", { 22, 14 }},
	{"DDL", { 22, 15 }},
	{"DDT", { 22, 16 }},
	{"FRAV?", { 22, 17 }},
	{"FRNS?", { 22, 18 }},
	{"GET", { 22, 19 }},
	{"GTL", { 22, 20 }},
	{"IDY", { 22, 21 }},
	{"IFCR?", { 22, 22 }},
	{"IFC", { 22, 23 }},
	{"INBIN", { 22, 24 }},
	{"INBUFX", { 22, 25 }},
	{"LAD", { 22, 26 }},
	{"LPD", { 22, 27 }},
	{"MIPT", { 22, 28 }},
	{"MONITOR", { 22, 29 }},
	{"NRD", { 22, 30 }},
	{"NRE", { 22, 31 }},
	{"ORAV?", { 22, 32 }},
	{"OUTBIN", { 22, 33 }},
	{"OUTBINY", { 22, 34 }},
	{"OUTBUFX", { 22, 35 }},
	{"PT=", { 22, 36 }},
	{"PT?", { 22, 37 }},
	{"PRBYTES", { 22, 38 }},
	{"PRFRMS", { 22, 39 }},
	{"REN", { 22, 40 }},
	{"RFRM", { 22, 41 }},
	{"RG-BUFX", { 22, 42 }},
	{"RG=BUF?", { 22, 43 }},
	{"RREG", { 22, 44 }},
	{"SAI", { 22, 45 }},
	{"SCOPE", { 22, 46 }},
	{"SDA", { 22, 47 }},
	{"SDC", { 22, 48 }},
	{"SDI", { 22, 49 }},
	{"SF33", { 22, 50 }},
	{"SRQR?", { 22, 51 }},
	{"SST", { 22, 52 }},
	{"TAD", { 22, 53 }},
	{"TCT", { 22, 54 }},
	{"UNL", { 22, 55 }},
	{"UNT", { 22, 56 }},
	{"WFRM", { 22, 57 }},
	{"WREG", { 22, 58 }},
	{"X-BUF", { 22, 59 }},
	{"X=BUF?", { 22, 60 }},
	{"X<>FLAG", { 22, 61 }},
	{"AND", { 24, 1 }},
	{"ASIZE?", { 24, 2 }},
	{"A-XL", { 24, 3 }},
	{"A-XR", { 24, 4 }},
	{"A-XX", { 24, 5 }},
	{"BININ", { 24, 6 }},
	{"BINVIEW", { 24, 7 }},
	{"BIT?", { 24, 8 }},
	{"HEXIN", { 24, 9 }},
	{"HEXVIEW", { 24, 10 }},
	{"NOT", { 24, 11 }},
	{"OR", { 24, 12 }},
	{"OCTIN", { 24, 13 }},
	{"OCTVIEW", { 24, 14 }},
	{"ROMCHKX", { 24, 15 }},
	{"ROTXY", { 24, 16 }},
	{"XOR", { 24, 17 }},
	{"X-AL", { 24, 18 }},
	{"X-AR", { 24, 19 }},
	{"Y-AX", { 24, 20 }},
	/* -WAND 1F */
	{"WNDDTA", { 27, 1 }},
	{"WNDDTX", { 27, 2 }},
	{"WNDLNK", { 27, 3 }},
	{"WNDSUB", { 27, 4 }},
	{"WNDSCN", { 27, 5 }},
	{"WNDTST", { 27, 6 }}
};

