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

/* globals for the project */
PRINTER_TYPE bc_printer = PRINTER_NONE;

char inpath[_MAX_PATH];
char outpath[_MAX_PATH];
char dirpath[_MAX_PATH];
char drive[_MAX_DRIVE];
char dir[_MAX_DIR];
char fname[_MAX_FNAME];
char ext[_MAX_EXT];

int line_numbers = 0;
int text_append = 0;
int raw_checksum = 1;
int do_xrom23 = 1;
int do_xrom25 = 1;
int do_xrom26 = 1;
int do_xrom28 = 1;
int force_global = 0;

char ascii[80];
unsigned char buf1_16[16];
unsigned char buf2_16[16];
unsigned char buf1_256[256];
unsigned char buf2_256[256];
unsigned char buf_512[512];
unsigned char buf_1024[1024];

/* output descriptors */
FILE_DESC bin = {
	FILE_BIN,
	2,
	".bin",
	"bin",
};
FILE_DESC dat = {
	FILE_DAT,
	4,
	".dat",
	"dat",
};
FILE_DESC lif = {
	FILE_LIF,
	768,
	".lif",
	"lif",
};
FILE_DESC p41 = {
	FILE_P41,
	32,
	".p41",
	"p41",
};
FILE_DESC raw = {
	FILE_RAW,
	0,
	".raw",
	"raw",
};
FILE_DESC txt = {
	FILE_TXT,
	0,
	".txt",
	"txt",
};

/* 32-byte LIF header */
unsigned char lifhdr[32] = {
	0x80, 0x00, 0x00, 0x00, 0x02, 0x10, 0x00, 0x80,
	0x00, 0x00, 0x00, 0x02, 0x10, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

/* 32-byte LIF directory */
unsigned char lifdir[32] = {
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
	0x20, 0x20, 0xE0, 0x80, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x20,
};

char *single01_1C[] = {
	"LBL 00", "LBL 01", "LBL 02", "LBL 03", "LBL 04", "LBL 05", "LBL 06",
	"LBL 07", "LBL 08", "LBL 09", "LBL 10", "LBL 11", "LBL 12", "LBL 13", "LBL 14",
	"0", "1", "2", "3", "4", "5", "6", "7",
	"8", "9", ".", " E", "-",
};

char *single20_8F[] = {
	"RCL 00", "RCL 01", "RCL 02", "RCL 03", "RCL 04", "RCL 05", "RCL 06", "RCL 07",
	"RCL 08", "RCL 09", "RCL 10", "RCL 11", "RCL 12", "RCL 13", "RCL 14", "RCL 15",
	"STO 00", "STO 01", "STO 02", "STO 03", "STO 04", "STO 05", "STO 06", "STO 07",
	"STO 08", "STO 09", "STO 10", "STO 11", "STO 12", "STO 13", "STO 14", "STO 15",
	"+", "-", "*", "/", "X<Y?", "X>Y?", "X<=Y?", "S+",
	"S-", "HMS+", "HMS-", "MOD", "%", "%CH", "P-R", "R-P",
	"LN", "X^2", "SQRT", "Y^X", "CHS", "E^X", "LOG", "10^X",
	"E^X-1", "SIN", "COS", "TAN", "ASIN", "ACOS", "ATAN", "DEC",
	"1/X", "ABS", "FACT", "X#0?", "X>0?", "LN1+X", "X<0?", "X=0?",
	"INT", "FRC", "D-R", "R-D", "HMS", "HR", "RND", "OCT",
	"CLS", "X<>Y", "PI", "CLST", "R^", "RDN", "LASTX", "CLX",
	"X=Y?", "X#Y?", "SIGN", "X<=0?", "MEAN", "SDEV", "AVIEW", "CLD",
	"DEG", "RAD", "GRAD", "ENTER", "STOP", "RTN", "BEEP", "CLA",
	"ASHF", "PSE", "CLRG", "AOFF", "AON", "OFF", "PROMPT", "ADV",
};

char *prefix90_9F[] = {
	"RCL ", "STO ", "ST+ ", "ST- ", "ST* ", "ST/ ", "ISG ", "DSE ",
	"VIEW ", "SREG ", "ASTO ", "ARCL ", "FIX ", "SCI ", "ENG ", "TONE ",
};

char *prefixA8_AD[] = {
	"SF ", "CF ", "FS?C ", "FC?C ", "FS? ", "FC? ",
};

char *prefixB1_BF[] = {
	"GTO 00", "GTO 01", "GTO 02", "GTO 03", "GTO 04", "GTO 05", "GTO 06",
	"GTO 07", "GTO 08", "GTO 09", "GTO 10", "GTO 11", "GTO 12", "GTO 13", "GTO 14",
};

char *prefixCE_CF[] = {

	"X<> ", "LBL ",
};

char prefixXROM[] = "XROM xx,xx";

char prefixGTO[] = "GTO ";

char prefixXEQ[] = "XEQ ";

char prefixGTO_IND[] = "GTO IND ";

char prefixXEQ_IND[] = "XEQ IND ";

char prefixLBL[] = "LBL ";

char prefixW[] = "W ";

char prefixEND[] = "END";

char prefixGTO_NOP[] = ";GTO __ (SPARE)";

char prefixGTO_IND_NOP[] = ";GTO IND __ (SPARE)";

char prefixXEQ_IND_NOP[] = ";XEQ IND __ (SPARE)";

char postfixIND[] = "IND ";

char *postfix00_7F[] = {
	"00", "01", "02", "03", "04", "05", "06", "07",
	"08", "09", "10", "11", "12", "13", "14", "15",
	"16", "17", "18", "19", "20", "21", "22", "23",
	"24", "25", "26", "27", "28", "29", "30", "31",
	"32", "33", "34", "35", "36", "37", "38", "39",
	"40", "41", "42", "43", "44", "45", "46", "47",
	"48", "49", "50", "51", "52", "53", "54", "55",
	"56", "57", "58", "59", "60", "61", "62", "63",
	"64", "65", "66", "67", "68", "69", "70", "71",
	"72", "73", "74", "75", "76", "77", "78", "79",
	"80", "81", "82", "83", "84", "85", "86", "87",
	"88", "89", "90", "91", "92", "93", "94", "95",
	"96", "97", "98", "99", "100", "101", "A", "B",
	"C", "D", "E", "F", "G", "H", "I", "J",
	"T", "Z", "Y", "X", "L", "M", "N", "O",
	"P", "Q", "R", "a", "b", "c", "d", "e",
};

char *alt_postfix102_111[] = {
	"102", "103",
	"104", "105", "106", "107", "108", "109", "110", "111",
};

char *alt_postfix117_122[] = {
	"[", "\\", "]",
	"^", "_", "`",
};

char *xrom23[] = {
	"", "COPYFL", "DIRX", "FLLENG", "FLTYPE", "MCOPY", "MCOPYPV", "MVERIFY",
	"", "ALENGIO", "ANUNDEL", "ATOXL", "ATOXR", "ATOXX", "XTOAL", "XTOAR",
	"X<>FIO", "YTOAX", "", "AID", "CLRDEV", "CLRLOOP", "DEVL", "DEVT",
	"FINDAID", "ID", "INAC", "INACL", "INAE", "INAN", "INXB", "INP",
	"LOCK", "NLOOP", "NOTREM", "OUTAC", "OUTACL", "OUTAE", "OUTAN", "OUTXB",
	"OUTP", "POLL", "POLLD", "POLLE", "POLLUNC", "RCLSEL", "SRQ?", "STAT",
	"XFER", "XFERC", "XREFCL", "XFERE", "XFERN", "", "ADROFF", "ADRON",
	"DDL", "DDT", "LAD", "SEND", "TAD", "UNL", "UNT",
};

char *xrom25[] = {
	"", "ALENG", "ANUM", "APPCHR", "APPREC", "ARCLREC", "AROT", "ATOX",
	"CLFL", "CLKEYS", "CRFLAS", "CRFLD", "DELCHR", "DELREC", "EMDIR", "FLSIZE",
	"GETAS", "GETKEY", "GETP", "GETR", "GETREC", "GETRX", "GETSUB", "GETX",
	"INSCHR", "INSREC", "PASN", "PCLPS", "POSA", "POSFL", "PSIZE", "PURFL",
	"RCLFLAG", "RCLPT", "RCLPTA", "REGMOVE", "REGSWAP", "SAVEAS", "SAVEP", "SAVER",
	"SAVERX", "SAVEX", "SEEKPT", "SEEKPTA", "SIZE?", "STOFLAG", "X<>F", "XTOA",
	"", "ASROOM", "CLRGX", "ED", "EMDIRX", "EMROOM", "GETKEYX", "RESZFL",
	"SREG?", "X=NN?", "X#NN?", "X<NN?", "X<=NN?", "X>NN?", "X>=NN?",
};

char *xrom26[] = {
	"", "ADATE", "ALMCAT", "ALMNOW", "ATIME", "ATIME24", "CLK12", "CLK24",
	"CLKT", "CLKTD", "CLOCK", "CORRECT", "DATE", "DATE+", "DDAYS", "DMY",
	"DOW", "MDY", "RCLAF", "RCLSW", "RUNSW", "SETAF", "SETDATE", "SETIME",
	"SETSW", "STOPSW", "SW", "T+X", "TIME", "XYZALM", "", "CLALMA",
	"CLALMX", "CLRALMS", "RCLALM", "SWPT",
};

char *xrom28[] = {
	"", "CREATE", "DIR", "", "PURGE", "READA", "READK", "READP",
	"READR", "READRX", "READS", "READSUB", "RENAME", "SEC", "SEEKR", "UNSEC",
	"VERIFY", "WRTA", "WRTK", "WRTP", "WRTPV", "WRTR", "WRTRX", "WRTS",
	"ZERO", "", "", "AUTOIO", "FINDIO", "INA", "IND", "INSTAT",
	"LISTEN", "LOCAL", "MANIO", "OUTA", "PWRDN", "PWRUP", "REMOTE", "SELECT",
	"STOPIO", "TRIGGER",
};

