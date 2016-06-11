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

void bintoxxx(char *infile, char *outfile, char *name, FILE_DESC *pout)
{
	char pname[11];
	long inlength;
	long outlength = 0;
	int files;
	FILE *fin, *fout = NULL;
	struct _finddata_t binfile;
	intptr_t hFind = -1L;

	// get input and output paths
	if ((files = getiopath(infile, ".BIN", &binfile, &hFind,
		pout->type == FILE_LIF ? 448 : 1, dirpath,
		outfile, pout->ext, outpath)) == 0) {
		if (hFind != -1L)
			_findclose(hFind);
		return;
	}

	// ignore NAME parameter, use input filename instead
	if (files > 1)
		name = NULL;

	do {
		// set input path
		getfullpath(inpath, dirpath, binfile.name);

		// set input length
		inlength = binfile.size - 2;

		// open input file
		if ((fin = open_input(inpath, inlength, infile, outpath))) {
			// read program size
			if ((inlength = read_bin_size(fin, inpath, inlength))) {
				// open output file
				if (fout || (fout = open_output(outpath))) {
					// show banner
					printf("bin[ %s ] => %s[ %s ]\n",
						inpath, pout->banner, outpath);

					// get program name
					get_lif_name(pname, name, binfile.name);

					// write output file
					copy_file(fout, outpath, pout->type, &outlength,
						pout->type == FILE_RAW ? NULL : pname,
						files, inlength,
						fin, inpath, DATA_RAW);
				}
			}

			fclose(fin);
		}
		--files;
	} while (fout && files && _findnext(hFind, &binfile) == 0);

	if (hFind != -1L)
		_findclose(hFind);
	closefiles(NULL, fout, outlength, pout->header);
}

void dattoxxx(char *infile, char *outfile, char *name, FILE_DESC *pout)
{
	char pname[11];
	long inlength;
	long outlength = 0;
	int files;
	FILE *fin, *fout = NULL;
	struct _finddata_t datfile;
	intptr_t hFind = -1L;

	// get input and output paths
	if ((files = getiopath(infile, ".DAT", &datfile, &hFind,
		pout->type == FILE_LIF ? 448 : 1, dirpath,
		outfile, pout->ext, outpath)) == 0) {
		if (hFind != -1L)
			_findclose(hFind);
		return;
	}

	// ignore NAME parameter, use input filename instead
	if (files > 1)
		name = NULL;

	do {
		// set input path
		getfullpath(inpath, dirpath, datfile.name);

		// set input length
		inlength = datfile.size - 4;

		// open input file
		if ((fin = open_input(inpath, inlength, infile, outpath))) {
			// read program size
			if ((inlength = read_dat_size(fin, inpath, inlength))) {
				// open output file
				if (fout || (fout = open_output(outpath))) {
					// show banner
					printf("dat[ %s ] => %s[ %s ]\n",
						inpath, pout->banner, outpath);

					// get program name
					get_lif_name(pname, name, datfile.name);

					// write output file
					copy_file(fout, outpath, pout->type, &outlength,
						pout->type == FILE_RAW ? NULL : pname,
						files, inlength,
						fin, inpath, DATA_DAT);
				}
			}

			fclose(fin);
		}
		--files;
	} while (fout && files && _findnext(hFind, &datfile) == 0);

	if (hFind != -1L)
		_findclose(hFind);
	closefiles(NULL, fout, outlength, pout->header);
}

void liftoxxx(char *infile, char *outfile, char *name, FILE_DESC *pout)
{
	char pname[11];
	long inlength;
	long outlength = 0;
	long dirblks;
	long startblk;
	FILE *fin = NULL;
	FILE *fout = NULL;
	struct _finddata_t liffile;
	intptr_t hFind = -1L;

	// get input and output paths
	if (getiopath(infile, ".LIF", &liffile, &hFind, 1, dirpath,
		outfile, pout->ext, outpath) == 0) {
		if (hFind != -1L)
			_findclose(hFind);
		return;
	}

	// set input path
	getfullpath(inpath, dirpath, liffile.name);

	// set input length
	inlength = liffile.size;

	// open input file
	if ((fin = open_input(inpath, inlength, infile, outpath))) {
		// read LIF header
		if ((dirblks = read_lif_hdr(fin, inpath, inlength))) {

			// get program name
			get_lif_name(pname, name, liffile.name);

			// read LIF directory
			if ((startblk = read_lif_dir(fin, inpath, &inlength, pname,
				dirblks))) {

				// open output file
				if ((fout = open_output(outpath))) {
					// show banner
					printf("lif[ %s ] => %s[ %s ]\n",
						inpath, pout->banner, outpath);

					// write output file
					fseek(fin, startblk * 256, SEEK_SET);
					copy_file(fout, outpath, pout->type, &outlength,
						pname, 1, inlength,
						fin, inpath, DATA_RAW);
				}
			}
		}
	}

	if (hFind != -1L)
		_findclose(hFind);
	closefiles(fin, fout, outlength, pout->header);
}

void p41toxxx(char *infile, char *outfile, FILE_DESC *pout)
{
	char name[11];
	char pname[11];
	long inlength;
	long outlength = 0;
	int files;
	FILE *fin, *fout = NULL;
	struct _finddata_t p41file;
	intptr_t hFind = -1L;

	// get input and output paths
	if ((files = getiopath(infile, ".P41", &p41file, &hFind,
		pout->type == FILE_LIF ? 448 : 1, dirpath,
		outfile, pout->ext, outpath)) == 0) {
		if (hFind != -1L)
			_findclose(hFind);
		return;
	}

	do {
		// set input path
		getfullpath(inpath, dirpath, p41file.name);

		// set input length
		inlength = p41file.size - 32;

		// open input file
		if ((fin = open_input(inpath, inlength, infile, outpath))) {
			// read P41 directory
			if (read_p41_dir(fin, inpath, &inlength, name)) {
				// open output file
				if (fout || (fout = open_output(outpath))) {
					// show banner
					printf("p41[ %s ] => %s[ %s ]\n",
						inpath, pout->banner, outpath);

					// get program name
					get_lif_name(pname, name, p41file.name);

					// write output file
					copy_file(fout, outpath, pout->type, &outlength,
						pname, files, inlength,
						fin, inpath, DATA_RAW);
				}
			}
			fclose(fin);
		}
		--files;
	} while (fout && files && _findnext(hFind, &p41file) == 0);

	if (hFind != -1L)
		_findclose(hFind);
	closefiles(NULL, fout, outlength, pout->header);
}

void rawtoxxx(char *infile, char *outfile, char *name, FILE_DESC *pout)
{
	char pname[11];
	long inlength;
	long outlength = 0;
	int files;
	FILE *fin, *fout = NULL;
	struct _finddata_t rawfile;
	intptr_t hFind = -1L;

	// get input and output paths
	if ((files = getiopath(infile, ".RAW", &rawfile, &hFind,
		pout->type == FILE_LIF ? 448 : 1, dirpath,
		outfile, pout->ext, outpath)) == 0) {
		if (hFind != -1L)
			_findclose(hFind);
		return;
	}

	// ignore NAME parameter, use input filename instead
	if (files > 1)
		name = NULL;

	do {
		// set input path
		getfullpath(inpath, dirpath, rawfile.name);

		// set input length
		inlength = rawfile.size;

		// open input file
		if ((fin = open_input(inpath, inlength, infile, outpath))) {
			// open output file
			if (fout || (fout = open_output(outpath))) {
				// show banner
				printf("raw[ %s ] => %s[ %s ]\n",
					inpath, pout->banner, outpath);

				// get program name
				get_lif_name(pname, name, rawfile.name);

				// write output file
				copy_file(fout, outpath, pout->type, &outlength,
					pname, files, inlength,
					fin, inpath, DATA_RAW);
			}

			fclose(fin);
		}
		--files;
	} while (fout && files && _findnext(hFind, &rawfile) == 0);

	if (hFind != -1L)
		_findclose(hFind);
	closefiles(NULL, fout, outlength, pout->header);
}

void txttoxxx(char *infile, char *outfile, char *name, FILE_DESC *pout)
{
	char pname[11];
	long inlength;
	long outlength = 0;
	int files;
	FILE *fin, *fout = NULL;
	struct _finddata_t txtfile;
	intptr_t hFind = -1L;

	// get input and output paths
	if ((files = getiopath(infile, ".TXT", &txtfile, &hFind,
		pout->type == FILE_LIF ? 448 : 1, dirpath,
		outfile, pout->ext, outpath)) == 0) {
		if (hFind != -1L)
			_findclose(hFind);
		return;
	}

	// ignore NAME parameter, use input filename instead
	if (files > 1)
		name = NULL;

	do {
		// set input path
		getfullpath(inpath, dirpath, txtfile.name);

		// set input length
		inlength = txtfile.size;

		// open input file
		if ((fin = open_input(inpath, inlength, infile, outpath))) {
			// open output file
			if (fout || (fout = open_output(outpath))) {
				// show banner
				printf("txt[ %s ] => %s[ %s ]\n",
					inpath, pout->banner, outpath);

				// get program name
				get_lif_name(pname, name, txtfile.name);

				// write output file
				copy_file(fout, outpath, pout->type, &outlength,
					pout->type == FILE_RAW ? NULL : pname,
					files, inlength,
					fin, inpath, DATA_TXT);
			}

			fclose(fin);
		}
		--files;
	} while (fout && files && _findnext(hFind, &txtfile) == 0);

	if (hFind != -1L)
		_findclose(hFind);
	closefiles(NULL, fout, outlength, pout->header);
}

void p41dump(char *infile)
{
	int files;
	FILE *fin;
	struct _finddata_t p41file;
	intptr_t hFind = -1L;

	// get input and output paths
	if ((files = getiopath(infile, ".P41", &p41file, &hFind, 0, dirpath,
		NULL, NULL, NULL)) == 0) {
		if (hFind != -1L)
			_findclose(hFind);
		return;
	}

	do {
		// set input path
		getfullpath(inpath, dirpath, p41file.name);

		// open input file
		if ((fin = open_input(inpath, p41file.size, infile, NULL))) {
			// dump LIF directry
			dump_lif_dir(fin, inpath, 1, NULL);

			fclose(fin);
		}

		--files;
	} while (files && _findnext(hFind, &p41file) == 0);

	if (hFind != -1L)
		_findclose(hFind);
}

void lifdump(char *infile, char *name)
{
	long dirblks;
	long inlength;
	int files;
	FILE *fin;
	struct _finddata_t liffile;
	intptr_t hFind = -1L;

	// get input path
	if ((files = getiopath(infile, ".LIF", &liffile, &hFind, 0, dirpath,
		NULL, NULL, NULL)) == 0) {
		if (hFind != -1L)
			_findclose(hFind);
		return;
	}

	do {
		// set input path
		getfullpath(inpath, dirpath, liffile.name);

		// set input length
		inlength = liffile.size;

		// open input file
		if ((fin = open_input(inpath, inlength, infile, outpath))) {
			// read LIF header
			if ((dirblks = read_lif_hdr(fin, inpath, inlength))) {
				// dump LIF directry
				dump_lif_dir(fin, inpath, dirblks * 8, name);
			}

			fclose(fin);
		}

		--files;
	} while (files && _findnext(hFind, &liffile) == 0);

	if (hFind != -1L)
		_findclose(hFind);
}

