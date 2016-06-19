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

void bintoxxx(char *infile, char *outfile, char *name, FILE_DESC *pout)
{
	char pname[11];
	long inlength;
	long outlength = 0;
	int files;
	FILE *fin, *fout = NULL;
	FIND_FILE binfile;

	/* get input and output paths */
	if ((files = getiopath(infile, ".bin", &binfile,
		pout->type == FILE_LIF ? 448 : 1, dirpath,
		outfile, pout->ext, outpath)) == 0) {
		findfile_close(&binfile);
		return;
	}

	/* ignore NAME parameter, use input filename instead */
	if (files > 1)
		name = NULL;

	do {
		/* set input path */
		getfullpath(inpath, dirpath, binfile.name);

		/* set input length */
		inlength = binfile.size - 2;

		/* open input file */
		if ((fin = open_input(inpath, inlength, infile, outpath))) {
			/* read program size */
			if ((inlength = read_bin_size(fin, inpath, inlength))) {
				/* open output file */
				if (fout || (fout = open_output(outpath))) {
					/* show banner */
					printf("bin[ %s ] => %s[ %s ]\n",
						inpath, pout->banner, outpath);

					/* get program name */
					get_lif_name(pname, name, binfile.name);

					/* write output file */
					copy_file(fout, outpath, pout->type, &outlength,
						pout->type == FILE_RAW ? NULL : pname,
						files, inlength,
						fin, inpath, DATA_RAW);
				}
			}

			fclose(fin);
		}
		--files;
	} while (fout && files && findfile_next(&binfile) == 0);

	findfile_close(&binfile);
	closefiles(NULL, fout, outlength, pout->header);
}

void dattoxxx(char *infile, char *outfile, char *name, FILE_DESC *pout)
{
	char pname[11];
	long inlength;
	long outlength = 0;
	int files;
	FILE *fin, *fout = NULL;
	FIND_FILE datfile;

	/* get input and output paths */
	if ((files = getiopath(infile, ".dat", &datfile,
		pout->type == FILE_LIF ? 448 : 1, dirpath,
		outfile, pout->ext, outpath)) == 0) {
		findfile_close(&datfile);
		return;
	}

	/* ignore NAME parameter, use input filename instead */
	if (files > 1)
		name = NULL;

	do {
		/* set input path */
		getfullpath(inpath, dirpath, datfile.name);

		/* set input length */
		inlength = datfile.size - 4;

		/* open input file */
		if ((fin = open_input(inpath, inlength, infile, outpath))) {
			/* read program size */
			if ((inlength = read_dat_size(fin, inpath, inlength))) {
				/* open output file */
				if (fout || (fout = open_output(outpath))) {
					/* show banner */
					printf("dat[ %s ] => %s[ %s ]\n",
						inpath, pout->banner, outpath);

					/* get program name */
					get_lif_name(pname, name, datfile.name);

					/* write output file */
					copy_file(fout, outpath, pout->type, &outlength,
						pout->type == FILE_RAW ? NULL : pname,
						files, inlength,
						fin, inpath, DATA_DAT);
				}
			}

			fclose(fin);
		}
		--files;
	} while (fout && files && findfile_next(&datfile) == 0);

	findfile_close(&datfile);
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
	FIND_FILE liffile;

	/* get input and output paths */
	if (getiopath(infile, ".lif", &liffile, 1, dirpath,
		outfile, pout->ext, outpath) == 0) {
		findfile_close(&liffile);
		return;
	}

	/* set input path */
	getfullpath(inpath, dirpath, liffile.name);

	/* set input length */
	inlength = liffile.size;

	/* open input file */
	if ((fin = open_input(inpath, inlength, infile, outpath))) {
		/* read LIF header */
		if ((dirblks = read_lif_hdr(fin, inpath, inlength))) {

			/* get program name */
			get_lif_name(pname, name, liffile.name);

			/* read LIF directory */
			if ((startblk = read_lif_dir(fin, inpath, &inlength, pname,
				dirblks))) {

				/* open output file */
				if ((fout = open_output(outpath))) {
					/* show banner */
					printf("lif[ %s ] => %s[ %s ]\n",
						inpath, pout->banner, outpath);

					/* write output file */
					fseek(fin, startblk * 256, SEEK_SET);
					copy_file(fout, outpath, pout->type, &outlength,
						pname, 1, inlength,
						fin, inpath, DATA_RAW);
				}
			}
		}
	}

	findfile_close(&liffile);
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
	FIND_FILE p41file;

	/* get input and output paths */
	if ((files = getiopath(infile, ".p41", &p41file,
		pout->type == FILE_LIF ? 448 : 1, dirpath,
		outfile, pout->ext, outpath)) == 0) {
		findfile_close(&p41file);
		return;
	}

	do {
		/* set input path */
		getfullpath(inpath, dirpath, p41file.name);

		/* set input length */
		inlength = p41file.size - 32;

		/* open input file */
		if ((fin = open_input(inpath, inlength, infile, outpath))) {
			/* read P41 directory */
			if (read_p41_dir(fin, inpath, &inlength, name)) {
				/* open output file */
				if (fout || (fout = open_output(outpath))) {
					/* show banner */
					printf("p41[ %s ] => %s[ %s ]\n",
						inpath, pout->banner, outpath);

					/* get program name */
					get_lif_name(pname, name, p41file.name);

					/* write output file */
					copy_file(fout, outpath, pout->type, &outlength,
						pname, files, inlength,
						fin, inpath, DATA_RAW);
				}
			}
			fclose(fin);
		}
		--files;
	} while (fout && files && findfile_next(&p41file) == 0);

	findfile_close(&p41file);
	closefiles(NULL, fout, outlength, pout->header);
}

void rawtoxxx(char *infile, char *outfile, char *name, FILE_DESC *pout)
{
	char pname[11];
	long inlength;
	long outlength = 0;
	int files;
	FILE *fin, *fout = NULL;
	FIND_FILE rawfile;

	/* get input and output paths */
	if ((files = getiopath(infile, ".raw", &rawfile,
		pout->type == FILE_LIF ? 448 : 1, dirpath,
		outfile, pout->ext, outpath)) == 0) {
		findfile_close(&rawfile);
		return;
	}

	/* ignore NAME parameter, use input filename instead */
	if (files > 1)
		name = NULL;

	do {
		/* set input path */
		getfullpath(inpath, dirpath, rawfile.name);

		/* set input length */
		inlength = rawfile.size;

		/* open input file */
		if ((fin = open_input(inpath, inlength, infile, outpath))) {
			/* open output file */
			if (fout || (fout = open_output(outpath))) {
				/* show banner */
				printf("raw[ %s ] => %s[ %s ]\n",
					inpath, pout->banner, outpath);

				/* get program name */
				get_lif_name(pname, name, rawfile.name);

				/* write output file */
				copy_file(fout, outpath, pout->type, &outlength,
					pname, files, inlength,
					fin, inpath, DATA_RAW);
			}

			fclose(fin);
		}
		--files;
	} while (fout && files && findfile_next(&rawfile) == 0);

	findfile_close(&rawfile);
	closefiles(NULL, fout, outlength, pout->header);
}

void txttoxxx(char *infile, char *outfile, char *name, FILE_DESC *pout)
{
	char pname[11];
	long inlength;
	long outlength = 0;
	int files;
	FILE *fin, *fout = NULL;
	FIND_FILE txtfile;

	/* get input and output paths */
	if ((files = getiopath(infile, ".txt", &txtfile,
		pout->type == FILE_LIF ? 448 : 1, dirpath,
		outfile, pout->ext, outpath)) == 0) {
		findfile_close(&txtfile);
		return;
	}

	/* ignore NAME parameter, use input filename instead */
	if (files > 1)
		name = NULL;

	do {
		/* set input path */
		getfullpath(inpath, dirpath, txtfile.name);

		/* set input length */
		inlength = txtfile.size;

		/* open input file */
		if ((fin = open_input(inpath, inlength, infile, outpath))) {
			/* open output file */
			if (fout || (fout = open_output(outpath))) {
				/* show banner */
				printf("txt[ %s ] => %s[ %s ]\n",
					inpath, pout->banner, outpath);

				/* get program name */
				get_lif_name(pname, name, txtfile.name);

				/* write output file */
				copy_file(fout, outpath, pout->type, &outlength,
					pout->type == FILE_RAW ? NULL : pname,
					files, inlength,
					fin, inpath, DATA_TXT);
			}

			fclose(fin);
		}
		--files;
	} while (fout && files && findfile_next(&txtfile) == 0);

	findfile_close(&txtfile);
	closefiles(NULL, fout, outlength, pout->header);
}

void p41dump(char *infile)
{
	int files;
	FILE *fin;
	FIND_FILE p41file;

	/* get input and output paths */
	if ((files = getiopath(infile, ".p41", &p41file, 0, dirpath,
		NULL, NULL, NULL)) == 0) {
		findfile_close(&p41file);
		return;
	}

	do {
		/* set input path */
		getfullpath(inpath, dirpath, p41file.name);

		/* open input file */
		if ((fin = open_input(inpath, p41file.size, infile, NULL))) {
			/* dump LIF directry */
			dump_lif_dir(fin, inpath, 1, NULL);

			fclose(fin);
		}

		--files;
	} while (files && findfile_next(&p41file) == 0);

	findfile_close(&p41file);
}

void lifdump(char *infile, char *name)
{
	long dirblks;
	long inlength;
	int files;
	FILE *fin;
	FIND_FILE liffile;

	/* get input path */
	if ((files = getiopath(infile, ".lif", &liffile, 0, dirpath,
		NULL, NULL, NULL)) == 0) {
		findfile_close(&liffile);
		return;
	}

	do {
		/* set input path */
		getfullpath(inpath, dirpath, liffile.name);

		/* set input length */
		inlength = liffile.size;

		/* open input file */
		if ((fin = open_input(inpath, inlength, infile, outpath))) {
			/* read LIF header */
			if ((dirblks = read_lif_hdr(fin, inpath, inlength))) {
				/* dump LIF directry */
				dump_lif_dir(fin, inpath, dirblks * 8, name);
			}

			fclose(fin);
		}

		--files;
	} while (files && findfile_next(&liffile) == 0);

	findfile_close(&liffile);
}

