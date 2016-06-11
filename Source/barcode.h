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

// postcript constants
#define PS_TOP_MARGIN    748.00
#define PS_LEFT_MARGIN   86.00
#define PS_CENTER        ( PS_LEFT_MARGIN - 4.20 )
#define PS_BAR_HEIGHT    25.20
#define PS_START_ROW     27.20
#define PS_NEXT_ROW      11.00
#define PS_PEN_UNIT      0.80
#define PS_ROW_HEIGHT    ( PS_NEXT_ROW + PS_START_ROW )
#define PS_BOTTOM_MARGIN ( PS_TOP_MARGIN - ( PS_ROW_HEIGHT * 18.0 ))
#define PS_MAX_PAGES     281
#define PS_ZERO_PEN      1
#define PS_ZERO_BITS     3
#define PS_ONE_PEN       3
#define PS_ONE_BITS      5

// barcode raster
typedef struct {
	unsigned char bitmap;
	char bits;
} RASTER8;

// barcode decoder flags
typedef enum {
	DECODE_FLAG_NONE,
	DECODE_FLAG_PENDING,
	DECODE_FLAG_END,
} DECODE_FLAG;

// barcode decoder states
typedef enum {
	DECODE_BYTE1,
	DECODE_BYTE2_OF_2,
	DECODE_BYTE2_OF_3,
	DECODE_BYTE3_OF_3,
	DECODE_BYTE2_GLOBAL,
	DECODE_BYTE3_GLOBAL,
	DECODE_BYTE4_GLOBAL,
	DECODE_BYTE2_ALPHA,
	DECODE_BYTE_ALPHA,
} DECODE_STATE;

void get_bc_ext(char *ext);

void get_bc_type(char *type);

int decode_row(unsigned char **pout_buffer, int out_size,
	unsigned char **pin_buffer, int *pin_count,
	int *pstart_line, int *pend_line,
	int *ptrail_count, int *plead_count,
	DECODE_FLAG *pflag);

void printbc(int row, int start_line, int end_line,
	unsigned char *buffer, int count,
	FILE *fout);

void render(unsigned char **pbuffer, RASTER8 font,
	int *pbytes, int *pbit);

int checksum8(int old_sum, int update);

