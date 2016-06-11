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
#include "hp41ucg.h"

void main(int argc, char *argv[])
{
	int h, i, j, k;
	char in = 0, out = 0;
	char *name = 0;
	char *infile = 0, *outfile = 0;

	// parser
	for (h = 0, i = 1; i < argc && !h; ++i) {
		// input & output parameters
		if (argv[i][0] == '-' || argv[i][0] == '/') {
			j = 1;
			if (argv[i][2] == ':' || argv[i][2] == '=')
				k = 3;
			else
				k = 2;
		}
		else if (argv[i][1] == ':' || argv[i][1] == '=') {
			j = 0;
			k = 2;
		}
		else {
			k = 0;
		}

		// start parsing
		if (_stricmp(argv[i], "?") == 0)
			h = 1;
		else if (_stricmp(argv[i], "-a") == 0 ||
			_stricmp(argv[i], "/a") == 0) {
			text_append = 1;
		}
		else if (_stricmp(argv[i], "-g") == 0 ||
			_stricmp(argv[i], "/g") == 0) {
			force_global = 1;
		}
		else if (_stricmp(argv[i], "-h") == 0 ||
			_stricmp(argv[i], "/h") == 0) {
			bc_printer = PRINTER_HP;
		}
		else if (_stricmp(argv[i], "-s") == 0 ||
			_stricmp(argv[i], "/s") == 0) {
			bc_printer = PRINTER_POSTCRIPT;
		}
		else if (_stricmp(argv[i], "-n") == 0 ||
			_stricmp(argv[i], "/n") == 0) {
			line_numbers = 1;
		}
		else if (_stricmp(argv[i], "-k") == 0 ||
			_stricmp(argv[i], "/k") == 0) {
			raw_checksum = 0;
		}
		else if (_stricmp(argv[i], "-x") == 0 ||
			_stricmp(argv[i], "/x") == 0) {
			do_xrom23 = 0;
			do_xrom25 = 0;
			do_xrom26 = 0;
			do_xrom28 = 0;
		}
		else if (_stricmp(argv[i], "-x23") == 0 ||
			_stricmp(argv[i], "/x23") == 0) {
			do_xrom23 = 0;
		}
		else if (_stricmp(argv[i], "-x25") == 0 ||
			_stricmp(argv[i], "/x25") == 0) {
			do_xrom25 = 0;
		}
		else if (_stricmp(argv[i], "-x26") == 0 ||
			_stricmp(argv[i], "/x26") == 0) {
			do_xrom26 = 0;
		}
		else if (_stricmp(argv[i], "-x28") == 0 ||
			_stricmp(argv[i], "/x28") == 0) {
			do_xrom28 = 0;
		}
		else if (k) {
			if (argv[i][j] == 'b' || argv[i][j] == 'B') {
				if (in == 0) {
					in = 'B';
					infile = &argv[i][k];
				}
				else if (in != 'B' && out == 0) {
					out = 'B';
					outfile = &argv[i][k];
				}
				else {
					h = 1;
				}
			}
			else if (argv[i][j] == 'd' || argv[i][j] == 'D') {
				if (in == 0) {
					in = 'D';
					infile = &argv[i][k];
				}
				else if (in != 'D' && out == 0) {
					out = 'D';
					outfile = &argv[i][k];
				}
				else {
					h = 1;
				}
			}
			else if (argv[i][j] == 'l' || argv[i][j] == 'L') {
				if (in == 0) {
					in = 'L';
					infile = &argv[i][k];
				}
				else if (in != 'L' && out == 0) {
					out = 'L';
					outfile = &argv[i][k];
				}
				else {
					h = 1;
				}
			}
			else if (argv[i][j] == 'p' || argv[i][j] == 'P') {
				if (in == 0) {
					in = 'P';
					infile = &argv[i][k];
				}
				else if (in != 'P' && out == 0) {
					out = 'P';
					outfile = &argv[i][k];
				}
				else {
					h = 1;
				}
			}
			else if (argv[i][j] == 'r' || argv[i][j] == 'R') {
				if (in == 0) {
					in = 'R';
					infile = &argv[i][k];
				}
				else if (in != 'R' && out == 0) {
					out = 'R';
					outfile = &argv[i][k];
				}
				else {
					h = 1;
				}
			}
			else if (argv[i][j] == 't' || argv[i][j] == 'T') {
				if (in == 0) {
					in = 'T';
					infile = &argv[i][k];
				}
				else if (in != 'T' && out == 0) {
					out = 'T';
					outfile = &argv[i][k];
				}
				else {
					h = 1;
				}
			}
			else if (argv[i][j] == 'h' || argv[i][j] == 'H') {
				if (out == 0) {
					out = 'W';
					outfile = &argv[i][k];
					bc_printer = PRINTER_HP;
				}
				else {
					h = 1;
				}
			}
			else if (argv[i][j] == 's' || argv[i][j] == 'S') {
				if (out == 0) {
					out = 'W';
					outfile = &argv[i][k];
					bc_printer = PRINTER_POSTCRIPT;
				}
				else {
					h = 1;
				}
			}
			else if (argv[i][j] == 'w' || argv[i][j] == 'W') {
				if (out == 0) {
					out = 'W';
					outfile = &argv[i][k];
				}
				else {
					h = 1;
				}
			}
			else {
				h = 1;
			}
		}
		else if (name == 0) {
			name = argv[i];
		}
		else {
			h = 1;
		}
	}

	// dispatcher
	if (h == 0) {
		// generate barcode to default file?
		if (out == 0 && bc_printer != PRINTER_NONE)
			out = 'W';

		// set default help
		h = in ? in : (out ? out : 1);

		// have input file spec?
		if (!infile || *infile == '\0') {
			if (in == 'L' && out == 0 && name && *name != '\0') {
				lifdump(name, NULL);
				h = 0;
			}
			else if (in == 'P' && out == 0 && name && *name != '\0') {
				p41dump(name);
				h = 0;
			}
			else if (in && out) {
				printf("Error: missing input path specification.\n");
			}
		}
		else {
			if (in == 'B') {
				if (out == 'L') {
					bintoxxx(infile, outfile, name, &lif);
					h = 0;
				}
				else if (out == 'P') {
					bintoxxx(infile, outfile, name, &p41);
					h = 0;
				}
				else if (name == 0) {
					if (out == 'D') {
						bintoxxx(infile, outfile, NULL, &dat);
						h = 0;
					}
					else if (out == 'R') {
						bintoxxx(infile, outfile, NULL, &raw);
						h = 0;
					}
					else if (out == 'T') {
						bintoxxx(infile, outfile, NULL, &txt);
						h = 0;
					}
				}
			}
			else if (in == 'D') {
				if (out == 'L') {
					dattoxxx(infile, outfile, name, &lif);
					h = 0;
				}
				else if (out == 'P') {
					dattoxxx(infile, outfile, name, &p41);
					h = 0;
				}
				else if (name == 0) {
					if (out == 'B') {
						dattoxxx(infile, outfile, NULL, &bin);
						h = 0;
					}
					else if (out == 'R') {
						dattoxxx(infile, outfile, NULL, &raw);
						h = 0;
					}
					else if (out == 'T') {
						dattoxxx(infile, outfile, NULL, &txt);
						h = 0;
					}
				}
			}
			else if (in == 'L') {
				if (out == 'B') {
					liftoxxx(infile, outfile, name, &bin);
					h = 0;
				}
				else if (out == 'D') {
					liftoxxx(infile, outfile, name, &dat);
					h = 0;
				}
				else if (out == 'P') {
					liftoxxx(infile, outfile, name, &p41);
					h = 0;
				}
				else if (out == 'R') {
					liftoxxx(infile, outfile, name, &raw);
					h = 0;
				}
				else if (out == 'T') {
					liftoxxx(infile, outfile, name, &txt);
					h = 0;
				}
				else if (out == 0) {
					lifdump(infile, name);
					h = 0;
				}
			}
			else if (in == 'P') {
				if (name == 0) {
					if (out == 'B') {
						p41toxxx(infile, outfile, &bin);
						h = 0;
					}
					else if (out == 'D') {
						p41toxxx(infile, outfile, &dat);
						h = 0;
					}
					else if (out == 'L') {
						p41toxxx(infile, outfile, &lif);
						h = 0;
					}
					else if (out == 'R') {
						p41toxxx(infile, outfile, &raw);
						h = 0;
					}
					else if (out == 'T') {
						p41toxxx(infile, outfile, &txt);
						h = 0;
					}
					else if (out == 0) {
						p41dump(infile);
						h = 0;
					}
				}
			}
			else if (in == 'R') {
				if (out == 'L') {
					rawtoxxx(infile, outfile, name, &lif);
					h = 0;
				}
				else if (out == 'P') {
					rawtoxxx(infile, outfile, name, &p41);
					h = 0;
				}
				else if (out == 'W') {
					barcode(infile, outfile, name);
					h = 0;
				}
				else if (name == 0) {
					if (out == 'B') {
						rawtoxxx(infile, outfile, NULL, &bin);
						h = 0;
					}
					else if (out == 'D') {
						rawtoxxx(infile, outfile, NULL, &dat);
						h = 0;
					}
					else if (out == 'T') {
						rawtoxxx(infile, outfile, NULL, &txt);
						h = 0;
					}
				}
			}
			else if (in == 'T') {
				if (out == 'L') {
					txttoxxx(infile, outfile, name, &lif);
					h = 0;
				}
				else if (out == 'P') {
					txttoxxx(infile, outfile, name, &p41);
					h = 0;
				}
				else if (name == 0) {
					if (out == 'B') {
						txttoxxx(infile, outfile, NULL, &bin);
						h = 0;
					}
					else if (out == 'D') {
						txttoxxx(infile, outfile, NULL, &dat);
						h = 0;
					}
					else if (out == 'R') {
						txttoxxx(infile, outfile, NULL, &raw);
						h = 0;
					}
				}
			}
		}
	}

	// need help?
	help(h);
}

// decoder states
enum {
	SEEK_BYTE1,
	SEEK_BYTE2_OF_2,
	SEEK_BYTE2_OF_3,
	SEEK_BYTE3_OF_3,
	SEEK_BYTE2_GLOBAL,
	SEEK_BYTE3_GLOBAL,
	SEEK_BYTE4_GLOBAL,
	SEEK_BYTE2_ALPHA,
	SEEK_BYTE_ALPHA,
};

int seek_end(unsigned char *buffer, int count)
{
	unsigned char c;
	int end = 0;
	int i = 0;
	static int alpha_count;
	static int state = SEEK_BYTE1;

	while (count && !end) {
		--count;
		c = buffer[i++];
		switch (state) {
		case SEEK_BYTE1:
			if (c >= 0x1D && c <= 0x1F) {
				state = SEEK_BYTE2_ALPHA;
			}
			else if ((c >= 0x90 && c <= 0xBF) ||
				(c >= 0xCE && c <= 0xCF)) {
				state = SEEK_BYTE2_OF_2;
			}
			else if (c >= 0xC0 && c <= 0xCD) {
				state = SEEK_BYTE2_GLOBAL;
			}
			else if (c >= 0xD0 && c <= 0xEF) {
				state = SEEK_BYTE2_OF_3;
			}
			else if (c >= 0xF0 && c <= 0xFF) {
				alpha_count = c & 0x0F;
				if (alpha_count)
					state = SEEK_BYTE_ALPHA;
			}
			break;

		case SEEK_BYTE2_OF_2:
			state = SEEK_BYTE1;
			break;

		case SEEK_BYTE2_OF_3:
			state = SEEK_BYTE3_OF_3;
			break;

		case SEEK_BYTE3_OF_3:
			state = SEEK_BYTE1;
			break;

		case SEEK_BYTE2_GLOBAL:
			state = SEEK_BYTE3_GLOBAL;
			break;

		case SEEK_BYTE3_GLOBAL:
			if (c < 0xF0) {
				end = i;
				state = SEEK_BYTE1;
			}
			else {
				alpha_count = c & 0x0F;
				state = alpha_count ? SEEK_BYTE4_GLOBAL : SEEK_BYTE1;
			}
			break;

		case SEEK_BYTE4_GLOBAL:
			--alpha_count;
			state = alpha_count ? SEEK_BYTE_ALPHA : SEEK_BYTE1;
			break;

		case SEEK_BYTE2_ALPHA:
			if (c <= 0xF0)
				state = SEEK_BYTE1;
			else {
				alpha_count = c & 0x0F;
				state = SEEK_BYTE_ALPHA;
			}
			break;

		case SEEK_BYTE_ALPHA:
			--alpha_count;
			if (alpha_count == 0)
				state = SEEK_BYTE1;
			break;
		}
	}

	return(end);
}
int hextoascii(char *ascii_buffer,
	unsigned char *hex_buffer, int hex_count)
{
	int i, j, k;

	for (i = 0, j = 0; i < hex_count; ++i, j += 2) {
		k = hex_buffer[i] >> 4;
		ascii_buffer[j] = k > 9 ? k + 'A' - 10 : k + '0';
		k = hex_buffer[i] & 0x0F;
		ascii_buffer[j + 1] = k > 9 ? k + 'A' - 10 : k + '0';
	}

	return(j);
}

int asciitohex(unsigned char *hex_buffer,
	char *ascii_buffer, int ascii_count)
{
	int i, j, k, n;

	for (i = 0, j = 0; i<ascii_count; i += 2, ++j) {
		k = toupper(ascii_buffer[i]);
		n = k > '9' ? k - 'A' + 10 : k - '0';
		n <<= 4;
		k = toupper(ascii_buffer[i + 1]);
		n += k > '9' ? k - 'A' + 10 : k - '0';
		hex_buffer[j] = n;
	}

	return(j);
}

int nonxdigit_buffer(unsigned char *buffer, int count)
{
	int i;

	for (i = 0; i < count; ++i) {
		if (!isxdigit(buffer[i]))
			return(i + 1);
	}

	return(0);
}

int oktowrite(char *path)
{
	int c;

	if (_access(path, 0) != -1) {
		if (_access(path, 2) != -1) {
			printf("Overwrite %s (Yes/No)?", path);
			do {
				c = _getch();
				if (c == 'y' || c == 'Y') {
					printf("%c\n", c);
					return 1;
				}
				else if (c == 'n' || c == 'N') {
					printf("%c\nNo output file written.\n", c);
					return 0;
				}
				// function or cursor key?
				else if (c == 0 || c == 0xE0)
					_getch();
			} while (1);
		}
		else {
			printf("No permission to write: %s.\n", path);
			return 0;
		}
	}

	return 1;
}

int getiopath(char *in_file,
	char *in_ext,
struct _finddata_t *ffd,
	intptr_t *hFindFile,
	int max_in_files,
	char *in_dir,
	char *out_file,
	char *out_ext,
	char *out_path)
{
	char path[_MAX_PATH];
	intptr_t hFind;
	int files = 1;

	// valid input path?
	*hFindFile = -1L;
	if (_fullpath(path, in_file, _MAX_PATH) == NULL) {
		printf("Error: invalid input path: %s\n", in_file);
		return 0;
	}

	// find FIRST file, allow wildcards
	strcpy(in_dir, path);
	hFind = _findfirst(in_dir, ffd);
	if (hFind == -1L) {
		strcat(in_dir, in_ext);
		hFind = _findfirst(in_dir, ffd);
		if (hFind == -1L) {
			strcpy(in_dir, path);
			strcat(in_dir, "\\*.*");
			hFind = _findfirst(in_dir, ffd);
			if (hFind == -1L)
				files = 0;
		}
	}

	// find NEXT files
	if (files) {
		while (_findnext(hFind, ffd) != -1L)
			++files;
		hFind = _findfirst(in_dir, ffd);
		if (hFind == -1L)
			files = 0;
	}

	// found any files?
	*hFindFile = hFind;
	if (!files) {
		printf("Error: could not find input file: %s\n", in_file);
		return 0;
	}

	// expect a single input file?
	if (max_in_files == 1) {
		// multiple input files?
		if (files > 1) {
			printf("Error: invalid input path, found[ %d ] files searching: %s\n",
				files, in_dir);
			return 0;
		}

		// output file specified?
		if (out_file && *out_file != '\0') {
			if (_fullpath(out_path, out_file, _MAX_PATH) == NULL) {
				printf("Error: invalid output path: %s\n", out_file);
				return 0;
			}

			if (out_ext) {
				_splitpath(out_path, drive, dir, fname, ext);
				if (*ext == '\0')
					strcat(out_path, out_ext);
			}
		}
		else if (out_ext) {
			// clone output path in current directory
			_splitpath(ffd->name, drive, dir, fname, ext);
			strcpy(path, fname);
			strcat(path, out_ext);
			_fullpath(out_path, path, _MAX_PATH);
		}
	}
	else if (max_in_files) {
		// output file specified?
		if (out_file && *out_file != '\0') {
			if (_fullpath(out_path, out_file, _MAX_PATH) == NULL) {
				printf("Error: invalid output path: %s\n", out_file);
				return 0;
			}

			if (out_ext) {
				_splitpath(out_path, drive, dir, fname, ext);
				if (*ext == '\0')
					strcat(out_path, out_ext);
			}
		}
		else if (out_ext) {
			// multiple input files?
			if (files > 1) {
				printf("Error: output path not specified, found[ %d ] files searching: %s\n",
					files, in_dir);
				return 0;
			}

			// clone output path in current directory
			_splitpath(ffd->name, drive, dir, fname, ext);
			strcpy(path, fname);
			strcat(path, out_ext);
			_fullpath(out_path, path, _MAX_PATH);
		}

		// set files @ max
		if (files > max_in_files)
			files = max_in_files;
	}

	return(files);
}

void getfullpath(char *fullpath, char *dirpath, char *name)
{
	_splitpath(dirpath, drive, dir, fname, ext);
	strcpy(fullpath, drive);
	strcat(fullpath, dir);
	strcat(fullpath, name);
}

FILE *open_input(char *inpath, long inlength, char *infile, char *outpath)
{
	FILE *fin = NULL;

	if (outpath && _stricmp(inpath, outpath) == 0) {
		printf("Error: input file cannot be output file: %s => %s\n",
			inpath, outpath);
	}
	else if (inlength <= 0) {
		printf("Error: empty input file: %s\n", inpath);
	}
	else if ((fin = fopen(inpath, "rb")) == NULL) {
		printf("Error opening input file: %s\n", infile);
	}

	return(fin);
}

FILE *open_output(char *outpath)
{
	FILE *fout = NULL;

	if (oktowrite(outpath)) {
		if ((fout = fopen(outpath, "w+b")) == NULL) {
			printf("Error opening output file: %s\n", outpath);
		}
	}

	return(fout);
}

long read_bin_size(FILE *fin, char *inpath, long length)
{
	unsigned char buffer2[2];

	// read 2-byte header
	if (fread(buffer2, 1, 2, fin) != 2) {
		printf("Error reading 2-byte header from: %s\n", inpath);
		return(0);
	}

	// get program size
	return(get_lif_size(buffer2, length));
}

long read_dat_size(FILE *fin, char *inpath, long length)
{
	unsigned char buffer4[4];
	unsigned char buffer2[2];
	int i;

	// read 4-byte header
	if (fread(buffer4, 1, 4, fin) != 4) {
		printf("Error reading 4-byte header from: %s\n", inpath);
		return(0);
	}

	// validate hex digits
	if ((i = nonxdigit_buffer(buffer4, 4))) {
		printf("Error: invalid ASCII hex-digit in 4-byte header[ %02X ]\n",
			buffer4[i - 1]);
		return(0);
	}

	// convert ASCII to hex
	asciitohex(buffer2, buffer4, 4);

	// get program size
	return(get_lif_size(buffer2, length / 2) * 2);
}

long read_p41_dir(FILE *fin, char *inpath, long *plength, char *name)
{
	unsigned char dir[sizeof(lifdir)];
	unsigned short *pus;
	int i;

	// init complete name with NULL
	if (name) {
		memset(name, 0, 11);
	}

	// read 32-byte directory
	if (fread(dir, 1, sizeof(lifdir), fin) != sizeof(lifdir)) {
		printf("Error reading 32-byte directory from: %s\n", inpath);
		return(0);
	}

	// get program name
	if (name) {
		i = 0;
		while (dir[i] != 0x20 && i < 10)
			name[i] = dir[i++];
	}

	// check LIF type
	if (*(pus = (unsigned short *)&dir[10]) != 0x80E0) {
		printf("Error: invalid file type in 32-byte header: expected[ E080 ], found[ %02X%02X ]\n",
			dir[10], dir[11]);
		return(0);
	}

	// get program size
	return((*plength = get_lif_size(&dir[28], *plength)));
}

long read_lif_dir(FILE *fin, char *inpath, long *plength, char *name,
	long dirblks)
{
	unsigned char *puc, *plif;
	unsigned short *pus;
	long length, size;
	long startblk;
	int i, match;

	match = 0;
	do {
		// read 256-byte diretory block
		if (fread(buf1_256, 1, 256, fin) != 256) {
			printf("Error reading directory block from: %s\n", inpath);
			dirblks = 0;
		}
		else {
			plif = buf1_256;
			for (i = 0; i < 8 && dirblks && !match; ++i) {
				// get file type
				pus = (unsigned short *)&plif[10];

				// empty directory?
				if (*pus == 0xFFFF) {
					dirblks = 0;
				}
				// HP-41 program type?
				else if (*pus == 0x80E0) {
					// search for matching name
					if ((match = is_lif_name(plif, name))) {
						// get program start block
						puc = (unsigned char *)&startblk;
						puc[0] = plif[15];
						puc[1] = plif[14];
						puc[2] = plif[13];
						puc[3] = plif[12];

						// get program size
						puc = (unsigned char *)&size;
						puc[3] = puc[2] = 0;
						puc[1] = plif[28];
						puc[0] = plif[29];

						if (size) {
							// minimum file size
							length = startblk * 256 + size;
							if (*plength < length) {
								printf("Error: input file too short: expected[ %ld+ ], found[ %ld ] bytes\n",
									length, *plength);
								size = 0;
							}
						}
						else {
							printf("Error: invalid program size[ 0000 ] for[ %s ]\n", name);
						}
					}
				}

				// next 32-byte directory
				plif += sizeof(lifdir);
			}

			// next 256-byte directory block;
			if (dirblks)
				--dirblks;
		}
	} while (dirblks && !match);

	if (match) {
		if (size) {
			*plength = size;
			return(startblk);
		}
	}
	else {
		printf("No match found for: name[ %s ], file type[ E080 ]\n", name);
	}
	return(0);
}

long read_lif_hdr(FILE *fin, char *inpath, long size)
{
	unsigned char buffer4[4];
	unsigned char *puc;
	unsigned short *pus;
	long *pul;
	long dirblks = 0;
	int i;

	// fill buffer with NULL for error checking
	memset(buf2_256, 0, sizeof(buf2_256));

	if (fread(buf1_256, 1, 256, fin) != 256) {
		printf("Error reading LIF header from: %s\n", inpath);
	}
	else if (size <= 768) {
		printf("Error: input file too short: expected[ 769+ ], found[ %ld ]\n", size);
	}
	else if (*(pus = (unsigned short *)&buf1_256[0]) != 0x0080) {
		puc = (unsigned char *)pus;
		printf("Error: invalid LIF identifier at offset[ ");
		if (puc[0] != 0x80)
			printf("0 ]: expected[ 80 ], found[ %02X ]\n", puc[0]);
		else
			printf("1 ]: expected[ 00 ], found[ %02X ]\n", puc[1]);
	}
	else if (*(pul = (long *)&buf1_256[8]) != 0x02000000) {
		puc = (unsigned char *)pul;
		printf("Error: invalid directory start block at offset[ ");
		if (puc[0] != 0x00)
			printf("8 ]: expected[ 00 ], found[ %02X ]\n", puc[0]);
		else if (puc[1] != 0x00)
			printf("9 ]: expected[ 00 ], found[ %02X ]\n", puc[1]);
		else if (puc[2] != 0x00)
			printf("10 ]: expected[ 00 ], found[ %02X ]\n", puc[2]);
		else
			printf("11 ]: expected[ 02 ], found[ %02X ]\n", puc[3]);
	}
	else if (*(pus = (unsigned short *)&buf1_256[12]) != 0x0010) {
		puc = (unsigned char *)pus;
		printf("Error: invalid machine identifier at offset[ ");
		if (puc[0] != 0x10)
			printf("12 ]: expected[ 10 ], found[ %02X ]\n", puc[0]);
		else
			printf("13 ]: expected[ 00 ], found[ %02X ]\n", puc[1]);
	}
	else if (*(pus = (unsigned short *)&buf1_256[14]) != 0) {
		puc = (unsigned char *)pus;
		printf("Error: invalid filler at offset[ ");
		if (puc[0] != 0x00)
			printf("14 ]: expected[ 00 ], found[ %02X ]\n", puc[0]);
		else if (puc[1] != 0x00)
			printf("15 ]: expected[ 00 ], found[ %02X ]\n", puc[1]);
	}
	else if (*(pus = (unsigned short *)&buf1_256[22]) != 0) {
		puc = (unsigned char *)pus;
		printf("Error: invalid filler at offset[ ");
		if (puc[0] != 0x00)
			printf("22 ]: expected[ 00 ], found[ %02X ]\n", puc[0]);
		else if (puc[1] != 0x00)
			printf("23 ]: expected[ 00 ], found[ %02X ]\n", puc[1]);
	}
	else if (memcmp(&buf1_256[42], buf2_256, 214) != 0) {
		for (i = 42; i < 256 && buf1_256[i] == 0; ++i);
		printf("Error: invalid filler at offset[ %d ]: expected[ 00 ], found[ %02X ]\n", i, buf1_256[i]);
	}
	else if ((dirblks = *(long *)&buf1_256[16]) == 0) {
		printf("Error: LIF header has 0 directory blocks at offset[ 16..19 ]\n");
	}
	else if (fread(buf1_256, 1, 256, fin) != 256) {
		printf("Error reading LIF header from: %s\n", inpath);
		dirblks = 0;
	}

	// convert to intel format
	puc = (unsigned char *)&dirblks;
	memcpy(buffer4, puc, 4);
	puc[0] = buffer4[3];
	puc[1] = buffer4[2];
	puc[2] = buffer4[1];
	puc[3] = buffer4[0];

	return(dirblks);
}

long get_lif_size(unsigned char *buffer2, long length)
{
	unsigned char *puc;
	long size;

	// get program size
	puc = (unsigned char *)&size;
	puc[3] = puc[2] = 0;
	puc[1] = buffer2[0];
	puc[0] = buffer2[1];
	if (size == 0 || size > length)
		size = length;

	return(size);
}

void get_lif_name(char *pname, char *name, char *filename)
{
	int i, j;

	j = 0;
	*pname = '\0';
	if (name && *name != '\0' && strlen(name) < 11) {
		for (i = 0; i < (int)strlen(name) && j == 0; ++i) {
			if (!isalnum(name[i]))
				++j;
		}
		if (j == 0)
			strcpy(pname, name);
	}
	if (filename) {
		if (*pname == '\0') {
			_splitpath(filename, drive, dir, fname, ext);
			strcpy(pname, fname);
			_strupr(pname);
		}
		if (j) {
			printf("Warning: specified name[ %s ] ignored, using[ %s ] instead\n",
				name, pname);
		}
	}
}

int is_lif_name(char *lifname, char *name)
{
	int i, j;

	// search for matching name
	for (i = 0, j = 0; i < (int)strlen(name) && !j; ++i) {
		if (lifname[i] == 0x20 || name[i] != lifname[i])
			++j;
	}
	return(j == 0 && (i == 10 || lifname[i] == 0x20));
}

long write_lif_hdr(FILE *fout, long *pstartblk, long *pdirpos, int files)
{
	long length, startblk;
	int blk, i;

	// init header ( padded with 0x00 )
	memset(buf1_256, 0, sizeof(buf1_256));
	memcpy(buf1_256, lifhdr, sizeof(lifhdr));

	// directory length in blocks (1..448 files)
	blk = files * 32 / 256;
	if (blk * 256 < files * 32)
		++blk;
	buf1_256[19] = (unsigned char)blk;

	// write header
	length = fwrite(buf1_256, 1, sizeof(buf1_256), fout);
	memset(buf1_256, 0, sizeof(lifhdr));
	length += fwrite(buf1_256, 1, sizeof(buf1_256), fout);
	*pdirpos = length;

	// write empty directory blocks ( filled with 0xFF )
	startblk = 2;
	memset(buf1_256, 0xFF, sizeof(buf1_256));
	for (i = 0; i < blk; ++i) {
		length += fwrite(buf1_256, 1, sizeof(buf1_256), fout);
		++startblk;
	}

	*pstartblk = startblk;
	return(length);
}

long write_lif_dir(FILE *fout, char *name, long startblk, long size)
{
	unsigned char dir[sizeof(lifdir)];
	unsigned char *puc;
	long blk;
	int i;

	// init directory
	memcpy(dir, lifdir, sizeof(lifdir));

	// program name
	for (i = 0; i < (int)strlen(name); ++i)
		dir[i] = name[i];

	// start block ( motorola format )
	puc = (unsigned char *)&startblk;
	dir[12] = puc[3];
	dir[13] = puc[2];
	dir[14] = puc[1];
	dir[15] = puc[0];

	// program length in blocks ( motorola format )
	blk = (size + 1) / 256;
	if (blk * 256 < (size + 1))
		++blk;
	puc = (unsigned char *)&blk;
	dir[18] = puc[1];
	dir[19] = puc[0];

	// program size ( motorola format )
	puc = (unsigned char *)&size;
	dir[28] = puc[1];
	dir[29] = puc[0];

	// write directory
	return(fwrite(dir, 1, sizeof(lifdir), fout));
}

void copy_file(FILE *fout, char *outpath,
	FILE_TYPE outtype, long *poutlength,
	char *pname, int files, long inlength,
	FILE *fin, char *inpath, DATA_TYPE intype)
{
	unsigned char *puc;
	unsigned char chksum;
	long lsize, dirblk;
	static long startblk;
	static long dirpos;

	// write program blocks
	if (outtype == FILE_RAW) {
		if (copy_blocks(fout, outpath, DATA_RAW, poutlength,
			&lsize, &chksum, NULL,
			fin, inpath, intype, inlength)) {

			// write cheksum block
			if (!raw_checksum ||
				write_raw_checksum(fout, outpath, poutlength,
				buf1_256, 256, chksum)) {
				show_lif_info(pname, lsize);
			}
		}
	}
	else if (outtype == FILE_BIN) {
		*poutlength += 2;
		fseek(fout, 2L, SEEK_SET);
		if (copy_blocks(fout, outpath, DATA_RAW, poutlength,
			&lsize, &chksum, NULL,
			fin, inpath, intype, inlength)) {

			// write cheksum block
			puc = (unsigned char *)&lsize;
			chksum += puc[0] + puc[1];
			if (write_raw_checksum(fout, outpath, poutlength,
				buf1_256, 128, chksum)) {
				printf(" size[ %04X ]  ( %ld bytes )\n",
					(unsigned short)lsize, lsize);
			}
		}

		// over-write program size
		fseek(fout, 0L, SEEK_SET);
		write_raw_size(fout, lsize);
	}
	else if (outtype == FILE_P41) {
		fseek(fout, 32L, SEEK_SET);
		if (copy_blocks(fout, outpath, DATA_RAW, poutlength,
			&lsize, &chksum, NULL,
			fin, inpath, intype, inlength)) {

			// write cheksum block
			if (write_raw_checksum(fout, outpath, poutlength,
				buf1_256, 256, chksum)) {
				show_lif_info(pname, lsize);
			}
		}

		// over-write directory
		fseek(fout, 0L, SEEK_SET);
		*poutlength += write_lif_dir(fout, pname, 0, lsize);
	}
	else if (outtype == FILE_LIF) {
		// write LIF header
		if (*poutlength == 0) {
			*poutlength = write_lif_hdr(fout, &startblk, &dirpos, files);
		}

		// write program blocks
		dirblk = startblk;
		fseek(fout, dirblk * 256, SEEK_SET);
		if (copy_blocks(fout, outpath, DATA_RAW, poutlength,
			&lsize, &chksum, &startblk,
			fin, inpath, intype, inlength)) {

			// write cheksum block
			++startblk;
			if (write_raw_checksum(fout, outpath, poutlength,
				buf1_256, 256, chksum)) {
				show_lif_info(pname, lsize);
			}
		}

		// over-write directory
		fseek(fout, dirpos, SEEK_SET);
		dirpos += write_lif_dir(fout, pname, dirblk, lsize);
	}
	else if (outtype == FILE_DAT) {
		fseek(fout, 4L, SEEK_SET);
		if (copy_blocks(fout, outpath, DATA_DAT, poutlength,
			&lsize, &chksum, NULL,
			fin, inpath, intype, inlength)) {

			// write cheksum
			puc = (unsigned char *)&lsize;
			chksum += puc[0] + puc[1];
			if (write_dat_checksum(fout, outpath,
				poutlength, chksum)) {
				printf(" size[ %04X ]  ( %ld bytes )\n",
					(unsigned short)lsize, lsize);
			}
		}

		// over-write program size
		fseek(fout, 0L, SEEK_SET);
		*poutlength += write_dat_size(fout, lsize);
	}
	else if (outtype == FILE_TXT) {
		copy_blocks(fout, outpath, DATA_TXT, poutlength,
			NULL, NULL, NULL,
			fin, inpath, intype, inlength);
	}
}

int copy_blocks(FILE *fout, char *outpath,
	DATA_TYPE outtype, long *poutlength,
	long *psize, unsigned char *pchksum, long *pstartblk,
	FILE *fin, char *inpath,
	DATA_TYPE intype, long inlength)
{
	if (intype == DATA_RAW) {
		return(copy_raw_blocks(fout, outpath, outtype, poutlength,
			psize, pchksum, pstartblk,
			fin, inpath, inlength));
	}
	else if (intype == DATA_DAT) {
		return(copy_dat_blocks(fout, outpath, outtype, poutlength,
			psize, pchksum, pstartblk,
			fin, inpath, inlength));
	}
	else { // DATA_TXT
		return(copy_txt_blocks(fout, outpath, outtype, poutlength,
			psize, pchksum, pstartblk,
			fin, inpath, inlength));
	}
}

int copy_raw_blocks(FILE *fout, char *outpath, DATA_TYPE outtype,
	long *poutlength, long *psize,
	unsigned char *pchksum, long *pstartblk,
	FILE *fin, char *inpath, long inlength)
{
	size_t blkcnt, incnt;
	size_t inblk, outblk;
	size_t  i, j, k;
	long size;
	int pending, end;
	unsigned char *inbuf;

	// program length in blocks
	blkcnt = (size_t)inlength / 256;
	if (blkcnt * 256 < (size_t)inlength)
		++blkcnt;

	// init checksum
	if (pchksum) {
		*pchksum = 0;
	}

	// init progranm size
	size = inlength;

	// copy blocks
	inblk = sizeof(buf1_256);
	memset(buf1_256, 0, inblk);
	for (i = 0; i < blkcnt && inblk && inlength; ++i) {
		// less than a full block?
		if (inlength < sizeof(buf1_256))
			inblk = (size_t)inlength;

		// read block
		if (fread(buf1_256, 1, inblk, fin) != inblk) {
			inblk = 0;
			printf("Error reading from: %s\n", inpath);
		}
		else {
			// check for END in block
			outblk = seek_end(buf1_256, inblk);
			if (outblk) {
				// adjust program size
				size += outblk - inlength;

				// last input block
				inlength = 0;
			}
			else {
				inlength -= inblk;
				outblk = inblk;
			}

			// update start block
			if (pstartblk) {
				if (outblk == sizeof(buf1_256))
					*pstartblk += 1;
			}

			// update checksum
			if (pchksum) {
				for (j = 0; j < outblk; ++j)
					*pchksum += buf1_256[j];
			}

			// write block
			if (outtype == DATA_TXT) {
				// decompile block
				inbuf = buf1_256;
				incnt = outblk;
				do {
					j = decompile(buf_1024, sizeof(buf_1024),
						&inbuf, &incnt, &pending, &end);
					if (j) {
						k = fwrite(buf_1024, 1, j, fout);
						*poutlength += k;
						if (k != j) {
							inblk = 0;
							printf("Error writing to: %s\n", outpath);
						}
					}
					if (end) {
						incnt = 0;
						inlength = 0;
					}
				} while (inblk && (incnt || (pending && !inlength)));
			}
			else if (outtype == DATA_DAT) {
				j = hextoascii(buf_512, buf1_256, outblk);
				k = fwrite(buf_512, 1, j, fout);
				*poutlength += k;
				if (k != j) {
					inblk = 0;
					printf("Error writing to: %s\n", outpath);
				}
			}
			else {  // DATA_RAW
				j = fwrite(buf1_256, 1, outblk, fout);
				*poutlength += j;
				if (j != outblk) {
					inblk = 0;
					printf("Error writing to: %s\n", outpath);
				}
			}
		}
	}

	// adjusted program size
	if (psize) {
		*psize = size;
	}

	return(inblk && size);
}

int copy_dat_blocks(FILE *fout, char *outpath, DATA_TYPE outtype,
	long *poutlength, long *psize,
	unsigned char *pchksum, long *pstartblk,
	FILE *fin, char *inpath, long inlength)
{
	size_t blkcnt, incnt;
	size_t inblk, outblk;
	size_t i, j, k;
	long size;
	int pending, end;
	unsigned char *inbuf;

	// program length in blocks
	blkcnt = (size_t)inlength / 512;
	if (blkcnt * 512 < (size_t)inlength)
		++blkcnt;

	// init checksum
	if (pchksum) {
		*pchksum = 0;
	}

	// init program size
	size = inlength / 2;

	// copy blocks
	inblk = sizeof(buf_512);
	memset(buf1_256, 0, inblk);
	for (i = 0; i < blkcnt && inblk && inlength; ++i) {
		// less than a full block?
		if (inlength < sizeof(buf_512))
			inblk = (size_t)inlength;

		// read block
		if (fread(buf_512, 1, inblk, fin) != inblk) {
			inblk = 0;
			printf("Error reading from: %s\n", inpath);
		}
		// validate hex digits
		else if ((j = nonxdigit_buffer(buf_512, inblk))) {
			inblk = 0;
			printf("Error: invalid ASCII hex-digit[ %02X ] at offset[ %ld ]\n",
				buf_512[j - 1], (size * 2) - inlength + j + 4 - 1);
		}
		else {
			// convert ASCII to hex
			j = asciitohex(buf1_256, buf_512, inblk);

			// check for END in block
			outblk = seek_end(buf1_256, j);
			if (outblk) {
				// adjust program size
				size += outblk - inlength / 2;

				// last input block
				inlength = 0;
			}
			else {
				inlength -= inblk;
				outblk = inblk / 2;
			}

			// update start block
			if (pstartblk) {
				if (outblk == sizeof(buf1_256))
					*pstartblk += 1;
			}

			// update checksum
			if (pchksum) {
				for (j = 0; j < outblk; ++j)
					*pchksum += buf1_256[j];
			}

			// write block
			if (outtype == DATA_TXT) {
				// decompile block
				inbuf = buf1_256;
				incnt = outblk;
				do {
					j = decompile(buf_1024, sizeof(buf_1024),
						&inbuf, &incnt, &pending, &end);
					if (j) {
						k = fwrite(buf_1024, 1, j, fout);
						*poutlength += k;
						if (k != j) {
							inblk = 0;
							printf("Error writing to: %s\n", outpath);
						}
					}
					if (end) {
						incnt = 0;
						inlength = 0;
					}
				} while (inblk && (incnt || (pending && !inlength)));
			}
			else {  // DATA_RAW
				j = fwrite(buf1_256, 1, outblk, fout);
				*poutlength += j;
				if (j != outblk) {
					inblk = 0;
					printf("Error writing to: %s\n", outpath);
				}
			}
		}
	}

	// adjusted program size
	if (psize) {
		*psize = size;
	}

	return(inblk && size);
}

int copy_txt_blocks(FILE *fout, char *outpath, DATA_TYPE outtype,
	long *poutlength, long *psize,
	unsigned char *pchksum, long *pstartblk,
	FILE *fin, char *inpath, long inlength)
{
	long blkcnt;
	size_t incnt;
	size_t inblk, outblk;
	size_t i, j, k;
	long size;
	int pending;
	COMPILE_FLAG flag;
	int dist, max;
	unsigned char *inbuf;
	unsigned char *outbuf;

	// program length in blocks
	blkcnt = inlength / 1024;
	if (blkcnt * 1024 < inlength)
		++blkcnt;

	// init checksum
	if (pchksum)
		*pchksum = 0;

	// init program size
	size = 0;

	// copy blocks
	flag = COMPILE_FLAG_RESTART;
	inblk = sizeof(buf_1024);
	for (i = 0, max = 0; i < (size_t)blkcnt && !max && inblk && inlength; ++i) {
		// less than a full block?
		if (inlength < sizeof(buf_1024))
			inblk = (size_t)inlength;

		// read block
		if (fread(buf_1024, 1, inblk, fin) != inblk) {
			inblk = 0;
			printf("Error reading from: %s\n", inpath);
		}
		else {
			inlength -= inblk;
			inbuf = buf_1024;
			incnt = inblk;
			do {
				// compile block
				outblk = compile(buf1_256, sizeof(buf1_256),
					&inbuf, &incnt, &flag, &pending, &dist);

				// compiler error?
				if (flag == COMPILE_FLAG_ERROR) {
					inblk = 0;
					size = 0;
				}
				else if (outblk) {
					// update program size
					size += outblk;

					// limit output @ 64K
					if (size > 65535 ||
						(size > 65532 && flag != COMPILE_FLAG_END)) {
						size -= outblk;
						max = 1;
					}

					if (!max) {
						// update checksum
						if (pchksum) {
							for (j = 0; j < outblk; ++j)
								*pchksum += buf1_256[j];
						}

						// write block
						if (outtype == DATA_DAT) {
							j = hextoascii(buf_512, buf1_256, outblk);
							outbuf = buf_512;
						}
						else {  // DATA_RAW
							j = outblk;
							outbuf = buf1_256;
						}
						k = fwrite(outbuf, 1, j, fout);
						*poutlength += k;
						if (k != j) {
							inblk = 0;
							printf("Error writing to: %s\n", outpath);
						}
					}
				}

				// the .END.?
				if (flag == COMPILE_FLAG_END || flag == COMPILE_FLAG_EOF) {
					incnt = 0;
					inlength = 0;
				}
				else if (outblk == 0 && inlength == 0) {
					incnt = 0;
				}
				// force last flush with EOF character
				else if (incnt == 0 && inlength == 0 && !pending) {
					incnt = 1;
					inbuf = buf_1024;
					*inbuf = 0x1A;
				}
			} while (inblk && !max && (incnt || (pending && !inlength)));
		}
	}

	// need to append .END.?
	if (size && inblk && flag != COMPILE_FLAG_END) {
		// compile .END.
		compile_end(buf1_256, dist);

		// update checksum
		if (pchksum) {
			for (i = 0; i < 3; ++i)
				*pchksum += buf1_256[i];
		}

		// append .END.
		if (outtype == DATA_DAT) {
			j = hextoascii(buf_512, buf1_256, 3);
			outbuf = buf_512;
		}
		else {  // DATA_RAW
			j = 3;
			outbuf = buf1_256;
		}
		k = fwrite(outbuf, 1, j, fout);
		*poutlength += k;
		size += k;
		if (k != j) {
			inblk = 0;
			printf("Error writing to: %s\n", outpath);
		}
	}

	// update start block
	if (pstartblk) {
		*pstartblk += size / 256;
	}

	// program size
	if (psize) {
		*psize = size;
	}

	return(inblk && size);
}

long write_raw_checksum(FILE *fout, char *outpath, long *poutlength,
	unsigned char *buffer, long bufsize,
	unsigned char chksum)
{
	long n, r;

	// get remaining bytes in block
	n = *poutlength;
	r = bufsize - (n - (n / bufsize * bufsize));

	// copy checksum ( padded with NULL )
	memset(buffer, 0, (size_t)r);
	buffer[0] = chksum;

	// write checksum block
	n = fwrite(buffer, 1, (size_t)r, fout);
	*poutlength += n;
	if (n != r) {
		printf("Error writing to: %s\n", outpath);
		return(0);
	}

	return(n);
}

long write_dat_checksum(FILE *fout, char *outpath,
	long *poutlength,
	unsigned char chksum)
{
	unsigned char buffer2[2];
	long n;

	// copy checksum
	hextoascii(buffer2, &chksum, 1);

	// write checksum bytes
	n = fwrite(buffer2, 1, 2, fout);
	*poutlength += n;
	if (n != 2) {
		printf("Error writing to: %s\n", outpath);
		return(0);
	}

	return(n);
}

long write_raw_size(FILE *fout, long size)
{
	unsigned char buffer2[2];
	unsigned char *puc;

	// motorola format
	puc = (unsigned char *)&size;
	buffer2[0] = puc[1];
	buffer2[1] = puc[0];
	return(fwrite(buffer2, 1, 2, fout));
}

long write_dat_size(FILE *fout, long size)
{
	unsigned char buffer4[4];
	unsigned char buffer2[2];
	unsigned char *puc;

	// motorola format
	puc = (unsigned char *)&size;
	buffer2[0] = puc[1];
	buffer2[1] = puc[0];
	hextoascii(buffer4, buffer2, 2);
	return(fwrite(buffer4, 1, 4, fout));
}

void dump_lif_dir(FILE *fin, char *inpath, long count, char *name)
{
	unsigned char dir[sizeof(lifdir)];
	unsigned short *pus;
	char pname[11];
	long entries;
	int i;

	// get search name ( if any )
	get_lif_name(pname, name, NULL);

	entries = 0;
	while (count) {
		// read 32-byte directory
		--count;
		if (fread(dir, 1, sizeof(lifdir), fin) != sizeof(lifdir)) {
			printf("Error reading 32-byte directory from: %s\n", inpath);
			count = 0;
		}
		else {
			// get directory type
			pus = (unsigned short *)&dir[10];

			// empty directory?
			if (*pus == 0xFFFF) {
				count = 0;
			}
			else if (*pname == '\0' || is_lif_name(dir, name)) {
				// list directory
				++entries;
				if (entries == 1) {
					printf("\n");
					printf("LIF directory listing[ %s ]\n", inpath);
					printf(" name        type  size  implementation\n");
					printf(" --------------------------------------\n");
				}
				printf(" ");
				for (i = 0; i < 10; ++i)
					printf("%c", dir[i]);
				printf("  ");
				for (i = 10; i < 12; ++i)
					printf("%02X", dir[i]);
				printf("  ");
				if (*pus == 0x80E0) {
					for (i = 28; i < 30; ++i)
						printf("%02X", dir[i]);
				}
				else
					printf("xxxx");
				printf("  ");
				for (i = 28; i < 32; ++i)
					printf("%02X", dir[i]);
				printf("\n");
			}
		}
	}

	if (entries == 0 && *pname != '\0') {
		printf("No match found for: name[ %s ] in: %s\n", pname, inpath);
	}
	else if (entries > 1) {
		printf("%ld directory entries found.\n", entries);
	}
}

void show_lif_info(char *name, long size)
{
	if (name)
		printf(" name[ %s ]\n", name);
	printf(" type[ E080 ]\n");
	printf(" size[ %04X ]  ( %ld bytes )\n", (unsigned short)size, size);
	printf(" implementation[ %04X0020 ]\n", (unsigned short)size);
}

void closefiles(FILE *fin, FILE *fout, long outlength, int header)
{
	if (fin)
		fclose(fin);
	if (fout) {
		fclose(fout);
		if (outlength <= header) {
			outlength = 0;
			remove(outpath);
		}
		printf("%ld bytes written.\n", outlength);
	}
}

void help(int h)
{
	switch (h) {
	case 1:
		printf("User-Code File Converter/Compiler/De-compiler/Barcode Generator - Version 2.00\n");
		printf("Copyright (c) Leo Duran, 2000-2016. All rights reserved. leo.duran@yahoo.com.\n\n");
		printf("Supported File Formats:\n");
		printf("  LIF [ /l ]: transfer file for Trans41\n");
		printf("  P41 [ /p ]: archive file for HP-41 programs on ftp sites\n");
		printf("  RAW [ /r ]: input/output file for LIFUTIL\n");
		printf("  DAT [ /d ]: input/output file for EMU41 (using INP, OUTP)\n");
		printf("  BIN [ /b ]: output file from 41UCC\n");
		printf("  TXT [ /t ]: program listing (text)\n");
		printf("  PCL [ /h ]: barcode printable file (Hewlett Packard PCL)\n");
		printf("  PS  [ /s ]: barcode printable file (Postcript)\n");
		printf("  WND [ /w ]: barcode hex-dump (Wand file)\n\n");
		printf("File Converter:\n");
		printf("  [ LIF, P41, RAW, DAT, BIN ] => [ BIN, DAT, RAW, P41, LIF ]\n");
		printf("Compiler:\n");
		printf("  [ TXT ] => [ LIF, P41, RAW, DAT, BIN ]\n");
		printf("De-compiler:\n");
		printf("  [ LIF, P41, RAW, DAT, BIN ] => [ TXT ]\n");
		printf("Barcode Generator:\n");
		printf("  [ RAW ] => [ PCL, PS, WND ]\n");
		printf("Use a single option[ /b, /d, /h, /l, /p, /r, /s, /t, or /w ] for help.\n");
		break;

	case 'B':
		printf("BIN Format:\n");
		printf("[2-byte header] + [compiled code] + [1-byte checksum] + [trailer]\n");
		printf("  [2-byte header] - size of [compiled code] in big-endian format\n");
		printf("  [1-byte checksum] - sum of all [compiled code]+[2-byte header]\n");
		printf("  [trailer] - filler bytes, so that filesize = multiple 128-byte blocks\n\n");
		printf("Usage: optional parameters are surrounded by <>\n\n");
		printf(" BIN file(s) to LIF file ( LIF name = <name> or [infile] )\n");
		printf("   /b=infile<.bin>  /l<=outfile.lif>  <name>\n\n");
		printf(" BIN file to P41 file ( P41 name = <name> or [infile] )\n");
		printf("   /b=infile<.bin>  /p<=outfile.p41>  <name>\n\n");
		printf(" BIN file to RAW file ( remove 2-byte header )\n");
		printf("   /b=infile<.bin>  /r<=outfile.raw>  </k>\n\n");
		printf(" BIN file to DAT file ( hex digits in ASCII format )\n");
		printf("   /b=infile<.bin>  /d<=outfile.dat>\n\n");
		printf(" BIN file to TXT file ( de-compile )\n");
		printf("   /b=infile<.bin>  /t<=outfile.txt>  </a>  </n>  </x>\n");
		printf("\nDe-compiler Options:\n");
		printf("  /a - append text using: [ \"\xC3text\" ], instead of: [ >\"text\" ]\n");
		printf("  /n - generate line numbers\n");
		printf("  /x - use [ XROM mm,ff ] for all XROM Functions\n");
		printf("  /x23 - use [ XROM 23,ff ]: [ XROM 23,1 ], instead of [ COPYFL ]\n");
		printf("  /x25 - use [ XROM 25,ff ]: [ XROM 25,46 ], instead of [ X<>F ]\n");
		printf("  /x26 - use [ XROM 26,ff ]: [ XROM 26,10 ], instead of [ CLOCK ]\n");
		printf("  /x28 - use [ XROM 28,ff ]: [ XROM 28,27 ], instead of [ AUTOIO ]\n");
		break;

	case 'D':
		printf("DAT Format:\n");
		printf("[4-byte header] + [compiled code] + [2-byte checksum]\n");
		printf("  [4-byte header] - size of [compiled code] in big-endian format\n");
		printf("  [2-byte checksum] - sum of [compiled code]+size of [compiled code]\n");
		printf(" All bytes are hex digits in ASCII format.\n\n");
		printf("Usage: optional parameters are surrounded by <>\n\n");
		printf(" DAT file(s) to LIF file ( LIF name = <name> or [infile] )\n");
		printf("   /d=infile<.dat>  /l<=outfile.lif>  <name>\n\n");
		printf(" DAT file to P41 file ( P41 name = <name> or [infile] )\n");
		printf("   /d=infile<.dat>  /p<=outfile.p41>  <name>\n\n");
		printf(" DAT file to RAW file ( remove 2-byte header )\n");
		printf("   /d=infile<.dat>  /r<=outfile.raw>  </k>\n\n");
		printf(" DAT file to TXT file ( de-compile )\n");
		printf("   /d=infile<.dat>  /t<=outfile.txt>  </a>  </n>  </x>\n");
		printf("\nDe-compiler Options:\n");
		printf("  /a - append text using: [ \"\xC3text\" ], instead of: [ >\"text\" ]\n");
		printf("  /n - generate line numbers\n");
		printf("  /x - use [ XROM mm,ff ] for all XROM Functions\n");
		printf("  /x23 - use [ XROM 23,ff ]: [ XROM 23,1 ], instead of [ COPYFL ]\n");
		printf("  /x25 - use [ XROM 25,ff ]: [ XROM 25,46 ], instead of [ X<>F ]\n");
		printf("  /x26 - use [ XROM 26,ff ]: [ XROM 26,10 ], instead of [ CLOCK ]\n");
		printf("  /x28 - use [ XROM 28,ff ]: [ XROM 28,27 ], instead of [ AUTOIO ]\n");
		break;

	case 'L':
		printf("LIF Format:\n");
		printf("File structure used for HP-IL mass-storage devices.\n");
		printf("LIF: https://www.finseth.com/hpdata/lif.phpl\n");
		printf("The HPDir Project: http://www.hp9845.net/9845/projects/hpdir/\n\n");
		printf("Usage: optional parameters are surrounded by <>\n\n");
		printf(" LIF file to P41 file ( search LIF name: <name>, <outfile> or [infile] )\n");
		printf("   /l=infile<.lif>  /p<=outfile.p41>  <name>\n\n");
		printf(" LIF file to RAW file\n");
		printf("   /l=infile<.lif>  /r<=outfile.raw>  <name>  </k>\n\n");
		printf(" LIF file to DAT file ( hex digits in ASCII format )\n");
		printf("   /l=infile<.lif>  /d<=outfile.dat>  <name>\n\n");
		printf(" LIF directory listing ( search for <name> only, if specified )\n");
		printf("   /l=infile<.lif>  <name>\n\n");
		printf(" LIF file to TXT file ( de-compile )\n");
		printf("   /l=infile<.lif>  /t<=outfile.txt>  <name>  </a>  </n>  </x>\n");
		printf("\nDe-compiler Options:\n");
		printf("  /a - append text using: [ \"\xC3text\" ], instead of: [ >\"text\" ]\n");
		printf("  /n - generate line numbers\n");
		printf("  /x - use [ XROM mm,ff ] for all XROM Functions\n");
		printf("  /x23 - use [ XROM 23,ff ]: [ XROM 23,1 ], instead of [ COPYFL ]\n");
		printf("  /x25 - use [ XROM 25,ff ]: [ XROM 25,46 ], instead of [ X<>F ]\n");
		printf("  /x26 - use [ XROM 26,ff ]: [ XROM 26,10 ], instead of [ CLOCK ]\n");
		printf("  /x28 - use [ XROM 28,ff ]: [ XROM 28,27 ], instead of [ AUTOIO ]\n");
		break;

	case 'P':
		printf("P41 Format:\n");
		printf("[32-byte header] + [compiled code] + [1-byte checksum] + [trailer]\n");
		printf("  [32-byte header] - single LIF directory entry for the program\n");
		printf("  [1-byte checksum] - sum of all [compiled code] bytes\n");
		printf("  [trailer] - filler bytes, so that filesize = 32 + multiple 256-byte blocks\n\n");
		printf("Usage: optional parameters are surrounded by <>\n\n");
		printf(" P41 file(s) to LIF file\n");
		printf("   /p=infile<.p41>  /l<=outfile.lif>\n\n");
		printf(" P41 file to RAW file ( remove 32-byte header )\n");
		printf("   /p=infile<.p41>  /r<=outfile.raw>  </k>\n\n");
		printf(" P41 file to DAT file ( hex digits in ASCII format )\n");
		printf("   /p=infile<.p41>  /d<=outfile.dat>\n\n");
		printf(" P41 file to TXT file ( de-compile )\n");
		printf("   /p=infile<.p41>  /t<=outfile.txt>  </a>  </n>  </x>\n");
		printf("\nDe-compiler Options:\n");
		printf("  /a - append text using: [ \"\xC3text\" ], instead of: [ >\"text\" ]\n");
		printf("  /n - generate line numbers\n");
		printf("  /x - use [ XROM mm,ff ] for all XROM Functions\n");
		printf("  /x23 - use [ XROM 23,ff ]: [ XROM 23,1 ], instead of [ COPYFL ]\n");
		printf("  /x25 - use [ XROM 25,ff ]: [ XROM 25,46 ], instead of [ X<>F ]\n");
		printf("  /x26 - use [ XROM 26,ff ]: [ XROM 26,10 ], instead of [ CLOCK ]\n");
		printf("  /x28 - use [ XROM 28,ff ]: [ XROM 28,27 ], instead of [ AUTOIO ]\n");
		break;

	case 'R':
		printf("RAW Format:\n");
		printf("[compiled code] + [1-byte checksum] + [trailer]\n");
		printf("  [1-byte checksum] - sum of all [compiled code] bytes\n");
		printf("  [trailer] - filler bytes, so that filesize = multiple 256-byte blocks\n\n");
		printf("Usage: optional parameters are surrounded by <>\n\n");
		printf(" RAW file(s) to LIF file ( LIF name = <name> or [infile] )\n");
		printf("   /r=infile<.raw>  /l<=outfile.lif>  <name>\n\n");
		printf(" RAW file to P41 file ( P41 name = <name> or [infile] ) \n");
		printf("   /r=infile<.raw>  /p<=outfile.p41>  <name>\n\n");
		printf(" RAW file to DAT file ( hex digits in ASCII format )\n");
		printf("   /r=infile<.raw>  /d<=outfile.dat>\n\n");
		printf(" RAW file to TXT file ( de-compile )\n");
		printf("   /r=infile<.raw>  /t<=outfile.txt>  </a>  </n>  </x>\n");
		printf("\nDe-compiler Options:\n");
		printf("  /a - append text using: [ \"\xC3text\" ], instead of: [ >\"text\" ]\n");
		printf("  /n - generate line numbers\n");
		printf("  /x - use [ XROM mm,ff ] for all XROM Functions\n");
		printf("  /x23 - use [ XROM 23,ff ]: [ XROM 23,1 ], instead of [ COPYFL ]\n");
		printf("  /x25 - use [ XROM 25,ff ]: [ XROM 25,46 ], instead of [ X<>F ]\n");
		printf("  /x26 - use [ XROM 26,ff ]: [ XROM 26,10 ], instead of [ CLOCK ]\n");
		printf("  /x28 - use [ XROM 28,ff ]: [ XROM 28,27 ], instead of [ AUTOIO ]\n");
		break;

	case 'T':
		printf("TXT Format - Compiler:\n");
		printf("1) Prefixes are case-insensitive: [ rcl 00 ] same as [ RCL 00 ]\n");
		printf("2) Postfixes are case-sensitive: [ lbl a ] different from [ lbl A ]\n");
		printf("3) Append text is allowed with: [ >\"text\" ], [ \"\\-text\" ], or [ \"\xC3text\" ]\n");
		printf("4) Synthetic instructions are allowed: [ rcl M ], [ sto R ]\n");
		printf("5) Synthetic NOP (TEXT0) is allowed using an empty text line: [ \"\" ]\n");
		printf("6) Synthetic text is allowed with C-style esc-sequences: [ \"\\0D\\0A\" ]\n");
		printf("7) Comments are allowed preceded by [ ; ] or [ # ]: [ # comment ]\n");
		printf("8) [ /n ] option - ignore line numbers in text: [ 100 STO 00 ]\n");
		printf("9) [ /g ] option - global for[ \"A..J\", \"a..e\" ] with quotes: [ lbl \"A\" ]\n\n");
		printf("Usage: optional parameters are surrounded by <>\n\n");
		printf(" TXT file(s) to LIF file ( LIF name = <name> or [infile] )\n");
		printf("   /t=infile<.txt>  /l<=outfile.lif>  <name>  </g>  </n>\n\n");
		printf(" TXT file to P41 file ( P41 name = <name> or [infile] )\n");
		printf("   /t=infile<.txt>  /p<=outfile.p41>  <name>  </g>  </n>\n\n");
		printf(" TXT file to RAW file\n");
		printf("   /t=infile<.txt>  /r<=outfile.raw>  </g>  </k>  </n>\n\n");
		printf(" TXT file to DAT file ( hex digits in ASCII format )\n");
		printf("   /t=infile<.txt>  /d<=outfile.dat>  </g>  </n>\n");
		printf("\nDe-compiler Options:\n");
		printf("  /a - append text using: [ \"\xC3text\" ], instead of: [ >\"text\" ]\n");
		printf("  /n - generate line numbers\n");
		printf("  /x - use [ XROM mm,ff ] for all XROM Functions\n");
		printf("  /x23 - use [ XROM 23,ff ]: [ XROM 23,1 ], instead of [ COPYFL ]\n");
		printf("  /x25 - use [ XROM 25,ff ]: [ XROM 25,46 ], instead of [ X<>F ]\n");
		printf("  /x26 - use [ XROM 26,ff ]: [ XROM 26,10 ], instead of [ CLOCK ]\n");
		printf("  /x28 - use [ XROM 28,ff ]: [ XROM 28,27 ], instead of [ AUTOIO ]\n");
		break;

	case 'W':
		printf("Barcode Generator:\n");
		printf("Reference: \"Creating Your Own HP-41 Barcode\"\n\n");
		printf("Usage: optional parameters are surrounded by <>\n\n");

		if (bc_printer == PRINTER_HP) {
			printf(" RAW file to PCL (HP raster graphics) file\n");
			printf("   /r=infile<.raw>  /h<=outfile.pcl>  <\"Program Title\">\n\n");
			printf("To print the barcode, copy the output file to the printer device.\n");
			printf("Example: hp41uc /r=prog.raw /h \"Program Title\"\n");
			printf("         copy /b prog.pcl lpt1\n");
		}
		else if (bc_printer == PRINTER_POSTCRIPT) {
			printf(" RAW file to Postcript file\n");
			printf("   /r=infile<.raw>  /s<=outfile.ps>  <\"Program Title\">\n\n");
			printf("To print the barcode, copy the output file to the printer device.\n");
			printf("Example: hp41uc /r=prog.raw /s \"Program Title\"\n");
			printf("         copy /b prog.ps lpt1\n");
		}
		else {
			printf(" RAW file to Wand (barcode) file\n");
			printf("   /r=infile<.raw>  /w<=outfile.wnd>  <\"Program Title\">\n\n");
			printf("The output file will be a 16-byte per row hex-dump in text format.\n\n");
		}
		break;

	default:
		break;
	}
}

