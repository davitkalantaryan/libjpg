/*
 *
 *  decompress_header.h
 *
 *  Created on: Feb 19, 2016
 *	Author: Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *  
 *
 */
#ifndef __decompress_headers_h__
#define __decompress_headers_h__

#include "functions_corrector.h"
#include "jpeglib.h"
#include "jmorecfg.h"

#ifndef JERR_INPUT_EMPTY
#define JERR_INPUT_EMPTY 1
#endif

#ifndef JERR_UNGETC_FAILED
#define JERR_UNGETC_FAILED 2
#endif

#ifdef READ_FROM_BUFFER

typedef struct SMetaFileStruct2
{
	const char*	file_buffer;
	size_t		file_buffer_size;
	size_t		file_offset;
}SMetaFileStruct2;

#define FILE_STRUCT SMetaFileStruct2*

#else  // #ifdef READ_FROM_BUFFER

#define FILE_STRUCT FILE*

#endif // #ifdef READ_FROM_BUFFER

typedef struct SDecompressOutput
{
	JDIMENSION	heigth;
	JDIMENSION	width;
	JDIMENSION	dataSize;
	JSAMPLE*	out_buffer;  // In/Out

	J_COLOR_SPACE jpeg_color_space; /* colorspace of JPEG image */

	/* Decompression processing parameters --- these fields must be set before
	 * calling jpeg_start_decompress().  Note that jpeg_read_header() initializes
	 * them to default values.
	 */

	J_COLOR_SPACE out_color_space; /* colorspace for output */
}SDecompressOutput;

#ifdef __cplusplus
extern "C"
{
#endif

	GLOBAL(int)
#ifdef READ_FROM_BUFFER
		read_BLOB_file(const void * a_jpeg_buffer, size_t a_jpeg_buf_size, SDecompressOutput* a_output);
#else
		read_BLOB_file(const char * filename, SDecompressOutput* a_output);
#endif

#ifdef __cplusplus
}
#endif

#endif  // #ifndef __decompress_headers_h__
