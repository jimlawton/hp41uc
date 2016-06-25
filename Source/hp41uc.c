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
#include "hp41ucg.h"

int main(int argc, char *argv[])
{
	int h, i, j, k;
	char in = 0, out = 0;
	char *name = 0;
	char *infile = 0, *outfile = 0;

	/* parser */
	for (h = 0, i = 1; i < argc && !h; ++i) {
		/* input & output parameters */
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

		/* start parsing */
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

	/* dispatcher */
	if (h == 0) {
		/* generate barcode to default file? */
		if (out == 0 && bc_printer != PRINTER_NONE)
			out = 'W';

		/* set default help */
		h = in ? in : (out ? out : 1);

		/* have input file spec? */
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
					convert(infile, &bin, outfile, &lif, name);
					h = 0;
				}
				else if (out == 'P') {
					convert(infile, &bin, outfile, &p41, name);
					h = 0;
				}
				else if (name == 0) {
					if (out == 'D') {
						convert(infile, &bin, outfile, &dat, NULL);
						h = 0;
					}
					else if (out == 'R') {
						convert(infile, &bin, outfile, &raw, NULL);
						h = 0;
					}
					else if (out == 'T') {
						convert(infile, &bin, outfile, &txt, NULL);
						h = 0;
					}
				}
			}
			else if (in == 'D') {
				if (out == 'L') {
					convert(infile, &dat, outfile, &lif, name);
					h = 0;
				}
				else if (out == 'P') {
					convert(infile, &dat, outfile, &p41, name);
					h = 0;
				}
				else if (name == 0) {
					if (out == 'B') {
						convert(infile, &dat, outfile, &bin, NULL);
						h = 0;
					}
					else if (out == 'R') {
						convert(infile, &dat, outfile, &raw, NULL);
						h = 0;
					}
					else if (out == 'T') {
						convert(infile, &dat, outfile, &txt, NULL);
						h = 0;
					}
				}
			}
			else if (in == 'L') {
				if (out == 'B') {
					convert(infile, &lif, outfile, &bin, name);
					h = 0;
				}
				else if (out == 'D') {
					convert(infile, &lif, outfile, &dat, name);
					h = 0;
				}
				else if (out == 'P') {
					convert(infile, &lif, outfile, &p41, name);
					h = 0;
				}
				else if (out == 'R') {
					convert(infile, &lif, outfile, &raw, name);
					h = 0;
				}
				else if (out == 'T') {
					convert(infile, &lif, outfile, &txt, name);
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
						convert(infile, &p41, outfile, &bin, NULL);
						h = 0;
					}
					else if (out == 'D') {
						convert(infile, &p41, outfile, &dat, NULL);
						h = 0;
					}
					else if (out == 'L') {
						convert(infile, &p41, outfile, &lif, NULL);
						h = 0;
					}
					else if (out == 'R') {
						convert(infile, &p41, outfile, &raw, NULL);
						h = 0;
					}
					else if (out == 'T') {
						convert(infile, &p41, outfile, &txt, NULL);
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
					convert(infile, &raw, outfile, &lif, name);
					h = 0;
				}
				else if (out == 'P') {
					convert(infile, &raw, outfile, &p41, name);
					h = 0;
				}
				else if (out == 'W') {
					barcode_init();
					barcode(infile, outfile, name);
					h = 0;
				}
				else if (name == 0) {
					if (out == 'B') {
						convert(infile, &raw, outfile, &bin, NULL);
						h = 0;
					}
					else if (out == 'D') {
						convert(infile, &raw, outfile, &dat, NULL);
						h = 0;
					}
					else if (out == 'T') {
						convert(infile, &raw, outfile, &txt, NULL);
						h = 0;
					}
				}
			}
			else if (in == 'T') {
				if (out == 'L') {
					convert(infile, &txt, outfile, &lif, name);
					h = 0;
				}
				else if (out == 'P') {
					convert(infile, &txt, outfile, &p41, name);
					h = 0;
				}
				else if (name == 0) {
					if (out == 'B') {
						convert(infile, &txt, outfile, &bin, NULL);
						h = 0;
					}
					else if (out == 'D') {
						convert(infile, &txt, outfile, &dat, NULL);
						h = 0;
					}
					else if (out == 'R') {
						convert(infile, &txt, outfile, &raw, NULL);
						h = 0;
					}
				}
			}
		}
	}

	/* need help? */
	help(h);

	return 0;
}

int hextoascii(unsigned char *ascii_buffer,
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
	unsigned char *ascii_buffer, int ascii_count)
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

int find_input_files(FIND_FILE *ff, char *in_dir, char *in_file, char *in_ext)
{
	char path[_MAX_PATH];
	int files = 1;

	/* valid input path? */
	findfile_init(ff);
	if (file_fullpath(path, in_file) == NULL) {
		printf("Error: invalid input path[ %s ]\n", in_file);
		return 0;
	}

	/* find FIRST file, allow wildcards */
	strcpy(in_dir, path);
	if (findfile_first(in_dir, ff) == -1) {
		strcat(in_dir, in_ext);
		if (findfile_first(in_dir, ff) == -1) {
			strcpy(in_dir, path);
			strcat(in_dir, "\\*.*");
			if (findfile_first(in_dir, ff) == -1) {
				files = 0;
			}
		}
	}

	/* find NEXT files */
	if (files) {
		/* count number of files in directory */
		while (findfile_next(ff) == 0)
			++files;

		/* rewind search to the beginning */
		findfile_close(ff);
		if (findfile_first(in_dir, ff) == -1) {
			files = 0;
		}
	}

	/* found any files? */
	if (!files)
		printf("Error: could not find input file[ %s ]\n", in_file);

	return(files);
}

int get_output_path(char *out_path, char *out_file, char *out_ext)
{
	if (file_fullpath(out_path, out_file) == NULL) {
		printf("Error: invalid output path[ %s ]\n", out_file);
		return -1;
	}

	/* use default .ext on new output path? */
	if (file_access(out_path, FILE_EXIST) == -1 && out_ext) {
		file_splitpath(out_path, drive, dir, fname, ext);
		if (*ext == '\0') {
			strcat(out_path, out_ext);
		}
	}
	return 0;
}

int exists_as_directory(char *path)
{
	FILE *file;

	if (file_access(path, FILE_EXIST) != -1 &&
		file_access(path, FILE_WRITE_OK) != -1) {
		/* try to open it as a file */
		if ((file = fopen(path, "w+b")) == NULL)
			return 0;
		fclose(file);
	}

	return - 1;
}

FILE *open_input(char *inpath, long inlength, char *infile, char *outpath)
{
	FILE *fin = NULL;

	if (outpath && _stricmp(inpath, outpath) == 0) {
		printf("Error: input file cannot be output file[ %s ]\n",
			outpath);
	}
	else if (inlength <= 0) {
		printf("Error: empty input file[ %s ]\n", inpath);
	}
	else if ((fin = fopen(inpath, "rb")) == NULL) {
		printf("Error opening input file[ %s ]", inpath);
		if (_stricmp(inpath, infile) != 0)
			printf(", from[ %s ]", infile);
		printf("\n");
	
	}

	return(fin);
}

FILE *open_output(char *outpath)
{
	FILE *fout = NULL;

	if (oktowrite(outpath)) {
		if ((fout = fopen(outpath, "w+b")) == NULL) {
			printf("Error opening output file[ %s ]\n", outpath);
		}
	}

	return(fout);
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

int oktowrite(char *path)
{
	int c;

	if (file_access(path, FILE_EXIST) != -1) {
		if (file_access(path, FILE_WRITE_OK) != -1) {
			printf("Overwrite %s (Yes/[No])?", path);
			do {
				c = _getch();
				if (c == 'y' || c == 'Y') {
					printf("%c\n", c);
					return 1;
				}
				/* function or cursor key? */
				else if (c == 0 || c == 0xE0) {
					/* 2-character sequence */
					_getch();
				}
#ifdef __GNUC__
				else if (c == 0x1B) {
					/* 3-character sequence */
					_getch();
					_getch();
				}
#endif
				else {
					printf("%c", c);
					if (c != 0x0A) {
						printf("\n");
					}
					printf("No output file written.\n");
					return 0;
				}
			} while (1);
		}
		else {
			printf("No permission to write[ %s ]\n", path);
			return 0;
		}
	}

	return 1;
}

void terminate_directory(char *path)
{
	char *end;

	if (path && *path != '\0') {
		end = path + strlen(path) - 1;
		if (*end != '\\' && *end != '/') {
#ifdef __GNUC__
			strcat(path, "/");
#else
			strcat(path, "\\");
#endif
		}
	}
}

void findfile_init(FIND_FILE *ff)
{
#ifdef __GNUC__
	if (ff != NULL) {
		ff->phandle = NULL;
	}
#else	/* _MSC_VER */
	if (ff != NULL) {
		ff->phandle = (void *)-1L;
	}
#endif
}

int findfile_first(char *path, FIND_FILE *ff)
{
#ifdef __GNUC__
	/* NOTE: This is just a partial implementation of _findfirst() */
	/* For example: there's limited support for wildcard search */
	struct dirent *ffd;
	struct stat f_stats;
	DIR *pdir, *f_pdir;
	char f_path[_MAX_PATH];
	char *f_dir;
	int check_path_end = 0;

	if (ff == NULL)
		return -1;

	if (path != NULL) {
		to_unix_path(path);

		/* check for wildcard on fname */
		ff->data.file_ext[0] = '\0';
		file_splitpath(path, drive, dir, fname, ext);
		if (strcmp(fname, "*") == 0) {
			strcpy(f_path, drive);
			strcat(f_path, dir);
			f_dir = f_path;
			if (strcmp(ext, ".*") != 0) 
				strcpy(ff->data.file_ext, ext);
		}
		else {
			f_dir = path;
			check_path_end = 1;
		}

		/* search for directory */
		pdir = opendir(f_dir);
		if (pdir != NULL) {
			do {
				ffd = readdir(pdir);
				if (ffd != NULL && 
					strcmp(ffd->d_name, ".") != 0 &&
					strcmp(ffd->d_name, "..") != 0) {
					/* ensure path is treated as a directory */
					if (check_path_end) {
						terminate_directory(path);
						check_path_end = 0;
					}

					/* save directory path for 'next' search */
					strcpy(ff->data.dir_path, f_dir);
					
					/* append filespec */
					strcpy(f_path, ff->data.dir_path);
					strcat(f_path, ffd->d_name);

					/* searching for specific file.ext? */
					if (ff->data.file_ext[0] != '\0') {
						override_file_ext(f_path, f_path, ff->data.file_ext);
					}

					/* make sure this is not a subdirectoy */
					f_pdir = opendir(f_path);
					if (f_pdir != NULL)
						closedir(f_pdir);
					else if (file_access(f_path, FILE_EXIST) != -1 &&
						stat(f_path, &f_stats) == 0 && f_stats.st_size) {
						/* found first file in directory */
						ff->name = ffd->d_name;
						ff->size = f_stats.st_size;
						ff->phandle = (void *)pdir;
						return 0;
					}
				}
			} while (ffd != NULL);
			closedir(pdir);
		}
		/* search for file */
		else if (file_access(path, FILE_EXIST) != -1 &&
			stat(path, &f_stats) == 0 && f_stats.st_size) {
			/* found a file */
			file_splitpath(path, drive, dir, fname, ext);
			strcpy(ff->data.file_spec, fname);
			strcat(ff->data.file_spec, ext);
			ff->name = ff->data.file_spec;
			ff->size = f_stats.st_size;
			ff->phandle = NULL;
			return 0;
		}
	}

	ff->phandle = NULL;
	return -1;
#else	/* _MSC_VER */
	struct _finddata_t *ffd;
	intptr_t hFind;
	int ret = 0;

	if (ff == NULL)
		return -1;

	if (path != NULL) {
		ffd = (struct _finddata_t *)ff->data;
		hFind = _findfirst(path, ffd);
		if (hFind != -1L) {
			while (ret == 0 &&
				(ffd->size == 0 ||
				strcmp(ffd->name, ".") == 0 ||
				strcmp(ffd->name, "..") == 0)) {
				ret = _findnext(hFind, ffd);
			} 
			if (ret)
				_findclose(hFind);
			else {
				ff->name = ffd->name;
				ff->size = ffd->size;
				ff->phandle = (void *)hFind;
				return 0;
			}
		}
	}

	ff->phandle = (void *)-1L;
	return -1;
#endif
}

int findfile_next(FIND_FILE *ff)
{
#ifdef __GNUC__
	/* NOTE: This is just a partial implementation of _findnext() */
	/* For example: there's limited support for wildcard search */
	struct dirent *ffd;
	struct stat f_stats;
	DIR *pdir, *f_pdir;
	char f_path[_MAX_PATH];

	if (ff != NULL) {
		pdir = (DIR *)ff->phandle;
		if (pdir != NULL) {
			do {
				ffd = readdir(pdir);
				if (ffd != NULL && 
					strcmp(ffd->d_name, ".") != 0 &&
					strcmp(ffd->d_name, "..") != 0) {
					/* append filespec */
					strcpy(f_path, ff->data.dir_path);
					strcat(f_path, ffd->d_name);

					/* searching for specific file.ext? */
					if (ff->data.file_ext[0] != '\0') {
						override_file_ext(f_path, f_path, ff->data.file_ext);
					}

					/* make sure this is not a subdirectoy */
					f_pdir = opendir(f_path);
					if (f_pdir != NULL)
						closedir(f_pdir);
					else if (file_access(f_path, FILE_EXIST) != -1 &&	
						stat(f_path, &f_stats) == 0 && f_stats.st_size) {
						/* found next file in directory */
						ff->name = ffd->d_name;
						ff->size = f_stats.st_size;
						return 0;
					}
				}
			} while (ffd != NULL);
		}
	}
	return -1;
#else	/* _MSC_VER */
	struct _finddata_t *ffd;
	intptr_t hFind;
	int ret;

	if (ff != NULL) {
		hFind = (intptr_t)ff->phandle;
		if (hFind != -1L) {
			ffd = (struct _finddata_t *)ff->data;
			do {
				ret = _findnext(hFind, ffd);
			} while (ret == 0 &&
				(ffd->size == 0 ||
				strcmp(ffd->name, ".") == 0 ||
				strcmp(ffd->name, "..") == 0));
			if (ret == 0) {
				ff->name = ffd->name;
				ff->size = ffd->size;
			}
			return ret;
		}
	}
	return -1;
#endif
}

void findfile_close(FIND_FILE *ff)
{
#ifdef __GNUC__
	DIR *pdir;

	if (ff != NULL) {
		pdir = (DIR *)ff->phandle;
		if (pdir != NULL) {
			closedir(pdir);
			ff->phandle = NULL;
		}
	}
#else	/* _MSC_VER */
	intptr_t hFind;

	if (ff != NULL) {
		hFind = (intptr_t)ff->phandle;
		if (hFind != -1L) {
			_findclose(hFind);
			ff->phandle = (void *)-1L;
		}
	}
#endif
}

#ifdef __GNUC__
/* reads from keypress, doesn't echo */
int _getch(void)
{
	struct termios oldattr, newattr;
	int ch;

	tcgetattr( STDIN_FILENO, &oldattr );
	newattr = oldattr;
	newattr.c_lflag &= ~( ICANON | ECHO );
	tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
	ch = getchar();
	tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
	return ch;
}

/* reads from keypress, echoes */
int _getche(void)
{
	struct termios oldattr, newattr;
	int ch;

	tcgetattr( STDIN_FILENO, &oldattr );
	newattr = oldattr;
	newattr.c_lflag &= ~( ICANON );
	tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
	ch = getchar();
	tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
	return ch;
}

char *_strupr(char *str)
{
	char *ret = str;
	for ( ; *str; str++) 
		*str = toupper(*str);
	return ret;
}

int _fileno(FILE *stream)
{
	return fileno(stream);
}

int _stricmp(const char * s1, const char *s2)
{
	return strcasecmp(s1, s2);
}
#endif

long get_filelength(int fd)
{
#ifdef __GNUC__
	struct stat f_stats;

	fstat(fd, &f_stats);
	return (long)f_stats.st_size;
#else	/* _MSC_VER */
	return _filelength(fd);
#endif
}

#ifdef __GNUC__
char *to_unix_path(char *p)
{
	if (p != NULL) {
		char *pp = p;
		while (*pp != 0) {
			if (*pp == '\\')
				*pp = '/';
			++pp;
		}
	}
	return p;
}
#endif

int file_access(
	const char *path,
	FILE_MODE mode
	)
{
#ifdef __GNUC__
	return access(to_unix_path((char *)path), (int)mode);
#else	/* _MSC_VER */
	return _access(path, (int)mode);
#endif
}

char *file_fullpath(
	char *absPath,
	const char *relPath
	)
{
#ifdef __GNUC__
	char *path;
	if (absPath)
		*absPath = '\0';
	path = realpath(to_unix_path((char *)relPath), absPath);
	if (path == NULL && errno == ENOENT && *absPath != '\0')
		path = absPath;
	return path;
#else	/* _MSC_VER */
	return _fullpath(absPath, relPath, _MAX_PATH);
#endif
}

void getfullpath(char *fullpath, char *dirpath, char *filespec)
{
	file_splitpath(dirpath, drive, dir, fname, ext);
	strcpy(fullpath, drive);
	strcat(fullpath, dir);
	strcat(fullpath, filespec);
}

void override_file_ext(char *cur_path, char *new_path, char *new_ext)
{
	file_splitpath(cur_path, drive, dir, fname, ext);
	strcpy(new_path, drive);
	strcat(new_path, dir);
	strcat(new_path, fname);
	strcat(new_path, new_ext);
}

void file_splitpath(
	const char* path,
	char* drv,
	char* dir,
	char* name,
	char* ext
	)
{
	const char* end; /* end of processed string */
	const char* p;   /* search pointer */
	const char* s;   /* copy pointer */

	/* extract drive name */
	if (path[0] && path[1] == ':') {
		if (drv) {
			*drv++ = *path++;
			*drv++ = *path++;
			*drv = '\0';
		}
	}
	else if (drv) {
		*drv = '\0';
	}

	end = path + strlen(path);

	/* search for begin of file extension */
	for (p = end; p > path && *--p != '\\' && *p != '/';) {
		if (*p == '.') {
			end = p;
			break;
		}
	}

	if (ext) {
		for (s = end; (*ext = *s++);)
			ext++;
	}

	/* search for end of directory name */
	for (p = end; p > path;) {
		if (*--p == '\\' || *p == '/') {
			p++;
			break;
		}
	}

	if (name) {
		for (s = p; s < end;)
			*name++ = *s++;

		*name = '\0';
	}

	if (dir) {
		for (s = path; s < p;)
			*dir++ = *s++;
	}
}

void help(int do_help)
{
	int decompiler = 0;

	switch (do_help) {
	case 1:
		printf("User-Code File Converter/Compiler/De-compiler/Barcode Generator - Version 2.30\n");
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
		printf("   /b=infile<.bin>  /r<=outfile.raw>  </k>\n");
		printf("  Use /k option to exclude checksum and trailer bytes in RAW file\n\n");
		printf(" BIN file to DAT file ( hex digits in ASCII format )\n");
		printf("   /b=infile<.bin>  /d<=outfile.dat>\n\n");
		printf(" BIN file to TXT file ( de-compile )\n");
		printf("   /b=infile<.bin>  /t<=outfile.txt>  </a>  </n>  </x>\n");

		decompiler = 1;
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
		printf("   /d=infile<.dat>  /r<=outfile.raw>  </k>\n");
		printf("  Use /k option to exclude checksum and trailer bytes in RAW file\n\n");
		printf(" DAT file to TXT file ( de-compile )\n");
		printf("   /d=infile<.dat>  /t<=outfile.txt>  </a>  </n>  </x>\n");

		decompiler = 1;
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
		printf("   /l=infile<.lif>  /r<=outfile.raw>  <name>  </k>\n");
		printf("  Use /k option to exclude checksum and trailer bytes in RAW file\n\n");
		printf(" LIF file to DAT file ( hex digits in ASCII format )\n");
		printf("   /l=infile<.lif>  /d<=outfile.dat>  <name>\n\n");
		printf(" LIF directory listing ( search for <name> only, if specified )\n");
		printf("   /l=infile<.lif>  <name>\n\n");
		printf(" LIF file to TXT file ( de-compile )\n");
		printf("   /l=infile<.lif>  /t<=outfile.txt>  <name>  </a>  </n>  </x>\n");

		decompiler = 1;
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
		printf("   /p=infile<.p41>  /r<=outfile.raw>  </k>\n");
		printf("  Use /k option to exclude checksum and trailer bytes in RAW file\n\n");
		printf(" P41 file to DAT file ( hex digits in ASCII format )\n");
		printf("   /p=infile<.p41>  /d<=outfile.dat>\n\n");
		printf(" P41 file to TXT file ( de-compile )\n");
		printf("   /p=infile<.p41>  /t<=outfile.txt>  </a>  </n>  </x>\n");

		decompiler = 1;
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

		decompiler = 1;
		break;

	case 'T':
		printf("TXT Format - Compiler:\n");
		printf("1) Prefixes are case-insensitive: [ rcl 00 ] same as [ RCL 00 ]\n");
		printf("2) Postfixes are case-sensitive: [ lbl a ] different than [ lbl A ]\n");
		printf("3) Append text is allowed using: [ >\"text\" ],  [ \"|-text\" ], or [ \"\\-text\" ]\n");
		printf("4) Synthetic instructions are allowed: [ rcl M ], [ sto R ]\n");
		printf("5) Synthetic NOP (TEXT0) is allowed using an empty string: [ \"\" ]\n");
		printf("6) Synthetic strings are allowed using C-style esc-sequences: [ \"\\x0D\\x0A\" ]\n");
		printf("7) Comments are allowed if preceded by [ ; ] or [ # ]: [ # comment ]\n");
		printf("8) [ /n ] option - ignore line numbers: [ 100 STO 00 ]\n");
		printf("9) [ /g ] option - global label for[ \"A..J\", \"a..e\" ] if in quotes: [ XEQ \"A\" ]\n\n");
		printf("Usage: optional parameters are surrounded by <>\n\n");
		printf(" TXT file(s) to LIF file ( LIF name = <name> or [infile] )\n");
		printf("   /t=infile<.txt>  /l<=outfile.lif>  <name>  </g>  </n>\n\n");
		printf(" TXT file to P41 file ( P41 name = <name> or [infile] )\n");
		printf("   /t=infile<.txt>  /p<=outfile.p41>  <name>  </g>  </n>\n\n");
		printf(" TXT file to RAW file\n");
		printf("   /t=infile<.txt>  /r<=outfile.raw>  </g>  </k>  </n>\n\n");
		printf(" TXT file to DAT file ( hex digits in ASCII format )\n");
		printf("   /t=infile<.txt>  /d<=outfile.dat>  </g>  </n>\n");
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

	if (decompiler) {
		printf("\nDe-compiler Options:\n");
		printf("  /a - append text using: [ \"|-text\" ], instead of: [ >\"text\" ]\n");
		printf("  /n - generate line numbers\n");
		printf("  /x - use [ XROM mm,ff ] for all XROM Functions\n");
		printf("  /x23 - use [ XROM 23,ff ]: [ XROM 23,1 ], instead of [ COPYFL ]\n");
		printf("  /x25 - use [ XROM 25,ff ]: [ XROM 25,46 ], instead of [ X<>F ]\n");
		printf("  /x26 - use [ XROM 26,ff ]: [ XROM 26,10 ], instead of [ CLOCK ]\n");
		printf("  /x28 - use [ XROM 28,ff ]: [ XROM 28,27 ], instead of [ AUTOIO ]\n");
	}
}

