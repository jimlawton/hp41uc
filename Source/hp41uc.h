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

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#define DO_ESC	1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <io.h>
#include <ctype.h>
#include <conio.h>

typedef unsigned char UCHAR;

// file types
typedef enum {
	FILE_BIN,
	FILE_DAT,
	FILE_LIF,
	FILE_P41,
	FILE_RAW,
	FILE_TXT,
} FILE_TYPE;

// data types
typedef enum {
	DATA_DAT,
	DATA_RAW,
	DATA_TXT,
} DATA_TYPE;

typedef struct {
	FILE_TYPE type;
	int header;
	char *ext;
	char *banner;
} FILE_DESC;

// compiler flags
typedef enum {
	COMPILE_FLAG_NONE,
	COMPILE_FLAG_ERROR,
	COMPILE_FLAG_END,
	COMPILE_FLAG_RESTART,
	COMPILE_FLAG_RESTART_EOF,
	COMPILE_FLAG_EOF,
} COMPILE_FLAG;

// barcode printers
typedef enum {
	PRINTER_NONE,
	PRINTER_HP,
	PRINTER_POSTCRIPT,
} PRINTER_TYPE;

void bintoxxx(char *infile, char *outfile, char *name, FILE_DESC *pout);

void dattoxxx(char *infile, char *outfile, char *name, FILE_DESC *pout);

void liftoxxx(char *infile, char *outfile, char *name, FILE_DESC *pout);
void lifdump(char *infile, char *name);

void p41toxxx(char *infile, char *outfile, FILE_DESC *pout);
void p41dump(char *infile);

void rawtoxxx(char *infile, char *outfile, char *name, FILE_DESC *pout);

void txttoxxx(char *infile, char *outfile, char *name, FILE_DESC *pout);

void barcode(char *infile, char *outfile, char *title);

int decompile(UCHAR *out_buffer, int out_size,
	UCHAR **pin_buffer, int *pin_count,
	int *ppending, int *pend);

int compile(UCHAR *out_buffer, int out_size,
	UCHAR **pin_buffer, int *pin_count,
	COMPILE_FLAG *pflag, int *ppending, int *pend_count);


int hextoascii(char *ascii_buffer,
	UCHAR *hex_buffer, int hex_count);
int asciitohex(UCHAR *hex_buffer,
	char *ascii_buffer, int ascii_count);
int nonxdigit_buffer(UCHAR *buffer, int count);

int seek_end(UCHAR *buffer, int count);

void compile_end(UCHAR *buffer, int bytes);

int oktowrite(char *path);

int getiopath(char *in_file,
	char *in_ext,
struct _finddata_t *ffd,
	intptr_t *hFindFile,
	int max_in_files,
	char *in_dir,
	char *out_file,
	char *out_ext,
	char *out_path);

void getfullpath(char *fullpath, char *dirpath, char *name);

FILE *open_input(char *inpath, long inlength, char *infile, char *outpath);

FILE *open_output(char *outpath);

long read_bin_size(FILE *fin, char *inpath, long length);

long read_dat_size(FILE *fin, char *inpath, long length);

long read_p41_dir(FILE *fin, char *inpath, long *plength, char *name);

long read_lif_dir(FILE *fin, char *inpath, long *plength, char *name,
	long dirblks);

long read_lif_hdr(FILE *fin, char *inpath, long size);

long get_lif_size(UCHAR *buffer2, long length);

void get_lif_name(char *pname, char *name, char *filename);

int is_lif_name(char *lifname, char *name);

long write_lif_hdr(FILE *fout, long *pstartblk, long *pdirpos, int files);

long write_lif_dir(FILE *fout, char *name, long startblk, long size);

void copy_file(FILE *fout, char *outpath,
	FILE_TYPE outtype, long *poutlength,
	char *pname, int files, long inlength,
	FILE *fin, char *inpath, FILE_TYPE intype);

int copy_blocks(FILE *fout, char *outpath,
	FILE_TYPE outtype, long *poutlength,
	long *psize, UCHAR *pchksum, long *pstartblk,
	FILE *fin, char *inpath,
	FILE_TYPE intype, long inlength);

int copy_raw_blocks(FILE *fout, char *outpath, FILE_TYPE outtype,
	long *poutlength, long *psize,
	UCHAR *pchksum, long *pstartblk,
	FILE *fin, char *inpath, long inlength);

int copy_dat_blocks(FILE *fout, char *outpath, FILE_TYPE outtype,
	long *poutlength, long *psize,
	UCHAR *pchksum, long *pstartblk,
	FILE *fin, char *inpath, long inlength);

int copy_txt_blocks(FILE *fout, char *outpath, FILE_TYPE outtype,
	long *poutlength, long *psize,
	UCHAR *pchksum, long *pstartblk,
	FILE *fin, char *inpath, long inlength);

long write_raw_checksum(FILE *fout, char *outpath, long *poutlength,
	UCHAR *buffer, long bufsize,
	unsigned char chksum);

long write_dat_checksum(FILE *fout, char *outpath, long *poutlength,
	unsigned char chksum);

long write_raw_size(FILE *fout, long size);

long write_dat_size(FILE *fout, long size);

void dump_lif_dir(FILE *fin, char *inpath, long count, char *name);

void show_lif_info(char *name, long size);

void closefiles(FILE *fin, FILE *fout,
	long outlength, int header);

void help(int h);

// globals
extern char inpath[_MAX_PATH];
extern char outpath[_MAX_PATH];
extern char dirpath[_MAX_PATH];
extern char drive[_MAX_DRIVE];
extern char dir[_MAX_DIR];
extern char fname[_MAX_FNAME];
extern char ext[_MAX_EXT];

extern int line_numbers;
extern int text_append;
extern int raw_checksum;
extern int do_xrom23;
extern int do_xrom25;
extern int do_xrom26;
extern int do_xrom28;
extern int force_global;
extern PRINTER_TYPE bc_printer;

extern char ascii[80];
extern unsigned char buf1_16[16];
extern unsigned char buf2_16[16];
extern unsigned char buf1_256[256];
extern unsigned char buf2_256[256];
extern unsigned char buf_512[512];
extern unsigned char buf_1024[1024];

// 32-byte LIF header
extern unsigned char lifhdr[32];

// 32-byte LIF directory
extern unsigned char lifdir[32];

extern char *single01_1C[];

extern char *single20_8F[];

extern char *prefix90_9F[];

extern char *prefixA8_AD[];

extern char *prefixB1_BF[];

extern char *prefixCE_CF[];

extern char prefixXROM[];

extern char prefixGTO[];

extern char prefixXEQ[];

extern char prefixGTO_IND[];

extern char prefixXEQ_IND[];

extern char prefixLBL[];

extern char prefixW[];

extern char prefixEND[];

extern char prefixGTO_NOP[];

extern char prefixGTO_IND_NOP[];

extern char prefixXEQ_IND_NOP[];

extern char postfixIND[];

extern char *postfix00_7F[];

extern char *alt_postfix102_111[];

extern char *alt_postfix117_122[];

extern char *xrom23[];
extern char *xrom25[];
extern char *xrom26[];
extern char *xrom28[];

