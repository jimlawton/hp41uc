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

/* de-compiler states */
typedef enum {
	BYTE1,
	BYTE2_NOP,
	BYTE2_IND_NOP,
	BYTE2_GTO_IGNORE,
	BYTE2_XROM,
	BYTE2_IND,
	BYTE2_POSTFIX,
	BYTE2_OF3_IGNORE,
	BYTE3_POSTFIX,
	BYTE2_GLOBAL_IGNORE,
	BYTE3_GLOBAL,
	BYTE4_GLOBAL_IGNORE,
	TEXT_OPEN_QUOTE,
	ALPHA_OPEN_QUOTE,
	BYTE_ALPHA,
	CLOSE_QUOTE,
	APPEND_SYNTH,
	APPEND_KEY,
	APPEND_CR,
	APPEND_LF
} DECOMPILE_STATE;

/* decoder states */
typedef enum {
	SEEK_BYTE1,
	SEEK_BYTE2_OF_2,
	SEEK_BYTE2_OF_3,
	SEEK_BYTE3_OF_3,
	SEEK_BYTE2_GLOBAL,
	SEEK_BYTE3_GLOBAL,
	SEEK_BYTE4_GLOBAL,
	SEEK_BYTE2_ALPHA,
	SEEK_BYTE_ALPHA,
} SEEK_STATE;

#define MAX_XROM_MODULES	32
#define MAX_ROM_FUNCTIONS	64

char *xrom17[] = {	/* PLOTTER 1A- */
	"", "CLIPUU", "CSIZE", "CSIZEO", "DGTIZE", "DRAW", "FRAME", "GCLEAR",
	"IDRAW", "IMOVE", "IPLOT", "LABEL", "LDIR", "LIMIT", "LOCATD", "LOCATE",
	"LORG", "LTYPE", "LTYPEO", "LXAXIS", "LYAXIS", "MOVE", "PEN", "PENDN",
	"PENUP", "PINIT", "PLOT", "PLREGX", "RATIO", "RPLOT", "SCALE", "SETGU",
	"SETUU", "TICLEN", "UNCLIP", "WHERE", "XAXIS", "XAXISO", "YAXIS", "YAXISO",
};

char *xrom18[] = {	/* PLOTTER 2A- */
	"", "PCLBUF", "PDIR", "PRCL", "NEWPLOT", "REPLOT", "PLINIT", "PLTUXY",
	"PLANOT", "Y?", "X?", "BC", "BCA", "BCAA", "BCCKSM", "BCO",
	"BCP", "BCREGX", "BCSIZE", "BCX", "BCXS",
};

char *xrom22[] = {	/* -HP-IL DEV */
	"", "A-BUF", "A=BUF?", "A=BUFX?", "AAD", "AAU", "AIPT", "BSIZE?",
	"BSIZEX", "BUF-AX", "BUF-RGX", "BUF-XA", "BUF-XB", "CF33", "CMD", "DDL",
	"DDT", "FRAV?", "FRNS?", "GET", "GTL", "IDY", "IFCR?", "IFC",
	"INBIN", "INBUFX", "LAD", "LPD", "MIPT", "MONITOR", "NRD", "NRE",
	"ORAV?", "OUTBIN", "OUTBINY", "OUTBUFX", "PT=", "PT?", "PRBYTES", "PRFRMS",
	"REN", "RFRM", "RG-BUFX", "RG=BUF?", "RREG", "SAI", "SCOPE", "SDA",
	"SDC", "SDI", "SF33", "SRQR?", "SST", "TAD", "TCT", "UNL",
	"UNT", "WFRM", "WREG", "X-BUF", "X=BUF?", "X<>FLAG",
};

char *xrom23[] = {	/* -X MASS 1A, -X EXT FCN, -X CTL FNS, -ADV CTL FN */
	"", "COPYFL", "DIRX", "FLLENG", "FLTYPE", "MCOPY", "MCOPYPV", "MVERIFY",
	"", "ALENGIO", "ANUNDEL", "ATOXL", "ATOXR", "ATOXX", "XTOAL", "XTOAR",
	"X<>FIO", "YTOAX", "", "AID", "CLRDEV", "CLRLOOP", "DEVL", "DEVT",
	"FINDAID", "ID", "INAC", "INACL", "INAE", "INAN", "INXB", "INP",
	"LOCK", "NLOOP", "NOTREM", "OUTAC", "OUTACL", "OUTAE", "OUTAN", "OUTXB",
	"OUTP", "POLL", "POLLD", "POLLE", "POLLUNC", "RCLSEL", "SRQ?", "STAT",
	"XFER", "XFERC", "XREFCL", "XFERE", "XFERN", "", "ADROFF", "ADRON",
	"DDL", "DDT", "LAD", "SEND", "TAD", "UNL", "UNT",
};

char *xrom24[] = {	/* -HP-IL DEV */
	"", "AND", "ASIZE?", "A-XL", "A-XR", "A-XX", "BININ", "BINVIEW",
	"BIT?", "HEXIN", "HEXVIEW", "NOT", "OR", "OCTIN", "OCTVIEW", "ROMCHKX",
	"ROTXY", "XOR", "X-AL", "X-AR", "Y-AX",
};

char *xrom25[] = {	/* -EXT FCN 2D, -CX EXT FCN */
	"", "ALENG", "ANUM", "APPCHR", "APPREC", "ARCLREC", "AROT", "ATOX",
	"CLFL", "CLKEYS", "CRFLAS", "CRFLD", "DELCHR", "DELREC", "EMDIR", "FLSIZE",
	"GETAS", "GETKEY", "GETP", "GETR", "GETREC", "GETRX", "GETSUB", "GETX",
	"INSCHR", "INSREC", "PASN", "PCLPS", "POSA", "POSFL", "PSIZE", "PURFL",
	"RCLFLAG", "RCLPT", "RCLPTA", "REGMOVE", "REGSWAP", "SAVEAS", "SAVEP", "SAVER",
	"SAVERX", "SAVEX", "SEEKPT", "SEEKPTA", "SIZE?", "STOFLAG", "X<>F", "XTOA",
	"", "ASROOM", "CLRGX", "ED", "EMDIRX", "EMROOM", "GETKEYX", "RESZFL",
	"SREG?", "X=NN?", "X#NN?", "X<NN?", "X<=NN?", "X>NN?", "X>=NN?",
};

char *xrom26[] = {	/* -TIME 2C, -CX TIME */
	"", "ADATE", "ALMCAT", "ALMNOW", "ATIME", "ATIME24", "CLK12", "CLK24",
	"CLKT", "CLKTD", "CLOCK", "CORRECT", "DATE", "DATE+", "DDAYS", "DMY",
	"DOW", "MDY", "RCLAF", "RCLSW", "RUNSW", "SETAF", "SETDATE", "SETIME",
	"SETSW", "STOPSW", "SW", "T+X", "TIME", "XYZALM", "", "CLALMA",
	"CLALMX", "CLRALMS", "RCLALM", "SWPT",
};

char *xrom27[] = {	/* -WAND 1F */
	"", "WNDDTA", "WNDDTX", "WNDLNK", "WNDSUB", "WNDSCN", "WNDTST",
};

char *xrom28[] = {	/* -MASS ST 1H, -CTL FNS */
	"", "CREATE", "DIR", "", "PURGE", "READA", "READK", "READP",
	"READR", "READRX", "READS", "READSUB", "RENAME", "SEC", "SEEKR", "UNSEC",
	"VERIFY", "WRTA", "WRTK", "WRTP", "WRTPV", "WRTR", "WRTRX", "WRTS",
	"ZERO", "", "", "AUTOIO", "FINDIO", "INA", "IND", "INSTAT",
	"LISTEN", "LOCAL", "MANIO", "OUTA", "PWRDN", "PWRUP", "REMOTE", "SELECT",
	"STOPIO", "TRIGGER",
};

char *xrom29[] = {	/* -PRINTER 2E */
	"", "ACA", "ACCHR", "ACCOL", "ACSPEC", "ACX", "BLDSPEC", "",
	"PRA", "PRAXIS", "PRBUF", "PRFLAGS", "PRKEYS", "", "PRPLOT", "PRPLOTP",
	"PRREG", "PRREGX", "PRS", "PRSTK", "PRX", "REGPLOT", "SKPCHR", "SKPCOL",
	"STKPLOT", "FMT",
};

char *xrom30[] = {	/* CARD RDR 1E */
	"", "MRG", "RDTA", "RDTAX", "RSUB", "VER",	"WALL", "WDTA",
	"WDTAX", "WPRV", "WSTS", "7CLREG", "7DSP0",	"7DSP1", "7DSP2", "7DSP3",
	"7DSP4", "7DSP5", "7DSP6", "7DSP7", "7DSP8", "7DSP9", "7DSPI", "7DSZ",
	"7DSZI", "7ENG", "7FIX", "7GSBI", "7GTOI", "7ISZ", "7ISZI", "7P<>S",
	"7PRREG", "7PRSTK", "7PRTX", "7RCL", "7SCI",
};

typedef struct {
	int module_id;
	int functions;
	char **xrom;
} XROM_DECOMP;

XROM_DECOMP xrom_list[] = {
	{ 17, 40, xrom17 },
	{ 18, 21, xrom18 },
	{ 22, 62, xrom22 },
	{ 23, 63, xrom23 },
	{ 24, 21, xrom24 },
	{ 25, 63, xrom25 },
	{ 26, 36, xrom26 },
	{ 27, 7, xrom27 },
	{ 28, 42, xrom28 },
	{ 29, 26, xrom29 },
	{ 30, 37, xrom30 },
};

char pKey[] = " Key: rc";
char psKey[] = " Key: -rc";
unsigned char synth_buffer[18];
unsigned char buffer6[6];

int copy_prefix(char *prefix, int out_size);
int copy_prefix2(char *prefix1, char *prefix2, int out_size);
int copy_postfix(char *postfix, int out_size);
int copy_postfix2(char *postfix1, char *postfix2, int out_size);
int is_nodisplay(unsigned char code);
int room_for_key(unsigned char *buffer, int count);
void copy_key(unsigned char *buffer, unsigned char code);
