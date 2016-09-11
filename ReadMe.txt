HP41UC - User-Code Utility for HP-41 Calculators
Version 2.42 - September 11, 2016
Copyright (c) Leo Duran, 2000-2016.  All rights reserved.

Contributed under: GNU General Public License, Version 3.

Build environment: Microsoft Visual Studio or GNU C compiler.

This utility provides means for editing and archiving code outside of the HP-41 calculator, using file transfer and barcode scanning mechanisms. To that end, the utility provides the following functions:

* File Converter *
The file format converter is used to package compiled code with wrappers compliant with various popular file formats. In some cases that may require preceding the code with a "header", appending a "checksum", or adding "filler" (don't care) bytes to satisfy file size requirements.

* Compiler *
The compiler takes code in readable text format, and compiles it into one of the supported file formats.

* De-compiler *
The de-compiler takes compiled code, packaged in any of the supported file formats, and creates a readable text file that can be edited or printed out.

* Barcode Generator *
The barcode generator creates a printable file with raster graphics data for PCL3(tm), or Postscript(tm) compatible printers.
