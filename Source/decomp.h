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

// de-compiler states
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
	APPEND_LF,
} DECOMPILE_STATE;

char pKey[] = " Key: rc";
char psKey[] = " Key: -rc";
unsigned char synth_buffer[18];
unsigned char buffer6[6];

int is_nodisplay(unsigned char code);
int room_for_key(unsigned char *buffer, int count);
void copy_key(char *buffer, char code);

