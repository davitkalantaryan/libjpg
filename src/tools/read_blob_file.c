/*
 * read_blob_file.c
 *
 * Author:   Davit Kalantaryan
 * Created:  2016.02.13
 *
 * This file illustrates how to use the IJG code as a subroutine library
 * to read or write JPEG image files.  You should look at this code in
 * conjunction with the documentation file libjpeg.txt.
 *
 * This code will not do anything useful as-is, but it may be helpful as a
 * skeleton for constructing routines that call the JPEG library.  
 *
 * We present these routines in the same coding style used in the JPEG code
 * (ANSI function definitions, etc); but you are of course free to code your
 * routines in a different style if you prefer.
 */





#include "functions_corrector.h"
#include <stdio.h>
#include <malloc.h>

/*
 * Include file for users of JPEG library.
 * You will need to have included system headers that define at least
 * the typedefs FILE and size_t before you can include jpeglib.h.
 * (stdio.h is sufficient on ANSI-conforming systems.)
 * You may also wish to include "jerror.h".
 */

#include "jerror.h"
#include "jpeglib.h"
#include "decompress_headers.h"

/*
 * <setjmp.h> is used for the optional error recovery mechanism shown in
 * the second part of the example.
 */

#include <setjmp.h>
#include <memory.h>

#ifdef WIN32
#include <process.h>
#ifdef MS_VER
#pragma warning( disable : 4996)
#endif
#endif



/******************** JPEG COMPRESSION SAMPLE INTERFACE *******************/

/* This half of the example shows how to feed data into the JPEG compressor.
 * We present a minimal version that does not worry about refinements such
 * as error recovery (the JPEG code will just exit() if it gets an error).
 */


/*
 * IMAGE DATA FORMATS:
 *
 * The standard input image format is a rectangular array of pixels, with
 * each pixel having the same number of "component" values (color channels).
 * Each pixel row is an array of JSAMPLEs (which typically are unsigned chars).
 * If you are working with color data, then the color values for each pixel
 * must be adjacent in the row; for example, R,G,B,R,G,B,R,G,B,... for 24-bit
 * RGB color.
 *
 * For this example, we'll assume that this data structure matches the way
 * our application has stored the image in memory, so we can just pass a
 * pointer to our image buffer.  In particular, let's say that the image is
 * RGB color and is described by:
 */


static void put_scanline_someplace(JSAMPLE *a_buf_0, JDIMENSION a_len, const struct jpeg_decompress_struct* a_pDec, 
	JDIMENSION* a_imgl_ptr, JSAMPLE* a_image_buffer)
{
	if ((*a_imgl_ptr) + a_len < ((a_pDec->image_height)* (a_pDec->image_width) * 3))
	{
		memcpy(a_image_buffer + (*a_imgl_ptr), a_buf_0, a_len);
		(*a_imgl_ptr) += a_len;
	}
}


/*
 * Sample routine for JPEG compression.  We assume that the target file name
 * and a compression quality factor are passed in.
 */


/*
 * SOME FINE POINTS:
 *
 * In the above loop, we ignored the return value of jpeg_write_scanlines,
 * which is the number of scanlines actually written.  We could get away
 * with this because we were only relying on the value of cinfo.next_scanline,
 * which will be incremented correctly.  If you maintain additional loop
 * variables then you should be careful to increment them properly.
 * Actually, for output to a stdio stream you needn't worry, because
 * then jpeg_write_scanlines will write all the lines passed (or else exit
 * with a fatal error).  Partial writes can only occur if you use a data
 * destination module that can demand suspension of the compressor.
 * (If you don't know what that's for, you don't need it.)
 *
 * If the compressor requires full-image buffers (for entropy-coding
 * optimization or a multi-scan JPEG file), it will create temporary
 * files for anything that doesn't fit within the maximum-memory setting.
 * (Note that temp files are NOT needed if you use the default parameters.)
 * On some systems you may need to set up a signal handler to ensure that
 * temporary files are deleted if the program is interrupted.  See libjpeg.txt.
 *
 * Scanlines MUST be supplied in top-to-bottom order if you want your JPEG
 * files to be compatible with everyone else's.  If you cannot readily read
 * your data in that order, you'll need an intermediate array to hold the
 * image.  See rdtarga.c or rdbmp.c for examples of handling bottom-to-top
 * source data using the JPEG code's internal virtual-array mechanisms.
 */



/******************** JPEG DECOMPRESSION SAMPLE INTERFACE *******************/

/* This half of the example shows how to read data from the JPEG decompressor.
 * It's a bit more refined than the above, in that we show:
 *   (a) how to modify the JPEG library's standard error-reporting behavior;
 *   (b) how to allocate workspace using the library's memory manager.
 *
 * Just to make this example a little different from the first one, we'll
 * assume that we do not intend to put the whole image into an in-memory
 * buffer, but to send it line-by-line someplace else.  We need a one-
 * scanline-high JSAMPLE array as a work buffer, and we will let the JPEG
 * memory manager allocate it for us.  This approach is actually quite useful
 * because we don't need to remember to deallocate the buffer separately: it
 * will go away automatically when the JPEG object is cleaned up.
 */


/*
 * ERROR HANDLING:
 *
 * The JPEG library's standard error handler (jerror.c) is divided into
 * several "methods" which you can override individually.  This lets you
 * adjust the behavior without duplicating a lot of code, which you might
 * have to update with each future release.
 *
 * Our example here shows how to override the "error_exit" method so that
 * control is returned to the library's caller when a fatal error occurs,
 * rather than calling exit() as the standard error_exit method does.
 *
 * We use C's setjmp/longjmp facility to return control.  This means that the
 * routine which calls the JPEG library must first execute a setjmp() call to
 * establish the return point.  We want the replacement error_exit to do a
 * longjmp().  But we need to make the setjmp buffer accessible to the
 * error_exit routine.  To do this, we make a private extension of the
 * standard JPEG error handler object.  (If we were using C++, we'd say we
 * were making a subclass of the regular error handler.)
 *
 * Here's the extended error handler struct:
 */

struct my_error_mgr {
  struct jpeg_error_mgr pub;	/* "public" fields */

  jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;

/*
 * Here's the routine that will replace the standard error_exit method:
 */

#if 1

METHODDEF(void)
my_error_exit (j_common_ptr cinfo)
{
  /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  /* Always display the message. */
  /* We could postpone this until after returning, if we chose. */
  (*cinfo->err->output_message) (cinfo);

  /* Return control to the setjmp point */
  longjmp(myerr->setjmp_buffer, 1);
}
#endif


#ifdef READ_FROM_BUFFER
#if 1
#define NGETC(_a_data_) (((_a_data_)->file_buffer)[0])
#else
#define NGETC(_a_data_)	\
	(((_a_data_)->file_offset < (_a_data_)->file_buffer_size) ? \
		((int)((_a_data_)->file_buffer)[(_a_data_)->file_offset++]) : EOF)
#endif
#if 1
#define NUNGETC(_c_,_a_data_)	(_c_)
#else
static _inline int NUNGETC(int _c_, FILE_STRUCT _a_data_)
{
	if ((_a_data_)->file_offset == 0) return EOF;
	((_a_data_)->file_buffer)[--(_a_data_)->file_offset] = (_c_);
	return (_c_);
}
#endif
#define NFCLOSE(_a_file_)
#else  // #ifdef READ_FROM_BUFFER
#define NGETC(_a_infile_)	getc((_a_infile_))
#define NUNGETC(_a_infile_)	ungetc((_a_infile_))
#define NFCLOSE flocse
#endif  // #ifdef READ_FROM_BUFFER


/*
 * Sample routine for JPEG decompression.  We assume that the source file name
 * is passed in.  We want to return 1 on success, 0 on error.
 */

LOCAL(int) 
read_JPEG_file_stat(FILE_STRUCT a_initData, SDecompressOutput* a_output, struct jpeg_decompress_struct* a_cinfo);


//#define WORK_REALLY

GLOBAL(int)
#ifdef READ_FROM_BUFFER
read_BLOB_file(const void * a_jpeg_buffer, size_t a_jpeg_buf_size, SDecompressOutput* a_output)
#else
read_BLOB_file(const char * filename, SDecompressOutput* a_output)
#endif
{
	int nReturn = 0;
	int type; // type of compressed buffer
	/* This struct contains the JPEG decompression parameters and pointers to
	 * working space (which is allocated as needed by the JPEG library).
	 */
	struct jpeg_decompress_struct cinfo;
	/* We use our private extension JPEG error handler.
	 * Note that this struct must live as long as the main JPEG parameter
	 * struct, to avoid dangling-pointer problems.
	 */

	/* More stuff */
#ifdef READ_FROM_BUFFER
	SMetaFileStruct2  metaFile;
	SMetaFileStruct2* infile = &metaFile;
	metaFile.file_buffer = a_jpeg_buffer;
	metaFile.file_buffer_size = a_jpeg_buf_size;
	metaFile.file_offset = 0;
#else
	FILE * infile;		/* source file */
	/* In this example we want to open the input file before doing anything else,
	* so that the setjmp() error recovery below can assume the file is open.
	* VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
	* requires it in order to read binary files.
	*/
	if ((infile = fopen(filename, "rb")) == NULL) {
		fprintf(stderr, "can't open %s\n", filename);
		return 0;
}
#endif  // #ifndef READ_FROM_BUFFER

	type = NGETC(infile);
	NUNGETC(type, infile);

	switch (type) {
	case 'B':
#ifdef WORK_REALLY
		nReturn = jinit_read_bmp(cinfo);
#else
		break;
#endif
#if 1
	case 'G':
#ifdef WORK_REALLY
		nReturn = jinit_read_gif(cinfo);
#else
		break;
#endif
	case 'P':
#ifdef WORK_REALLY
		nReturn = jinit_read_ppm(cinfo);
#else
		break;
#endif
	case 'R':
#ifdef WORK_REALLY
		nReturn = jinit_read_rle(cinfo);
#else
		break;
#endif
	case 0x00:
#ifdef WORK_REALLY
		nReturn = jinit_read_targa(cinfo);
#else
		break;
#endif
#endif
	default:
		nReturn = read_JPEG_file_stat(infile, a_output, &cinfo);
		break;
	}

	NFCLOSE(infile);

	return nReturn;			/* suppress compiler warnings */
}



LOCAL(int)
read_JPEG_file_stat(FILE_STRUCT a_initData, SDecompressOutput* a_output, struct jpeg_decompress_struct* a_cinfo)
{
	JSAMPLE* out_buffer;

	/* We use our private extension JPEG error handler.
	 * Note that this struct must live as long as the main JPEG parameter
	 * struct, to avoid dangling-pointer problems.
	 */
	struct my_error_mgr jerr;
	
	JSAMPARRAY buffer;		/* Output row buffer */
	int row_stride;		/* physical row width in output buffer */
	JDIMENSION impl = 0;
	
	/* Step 1: allocate and initialize JPEG decompression object */
	/* We set up the normal JPEG error routines, then override error_exit. */
	a_cinfo->err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	/* Establish the setjmp return context for my_error_exit to use. */
	if (setjmp(jerr.setjmp_buffer))
	{
		/* If we get here, the JPEG code has signaled an error.
		 * We need to clean up the JPEG object, close the input file, and return.
		 */
		jpeg_destroy_decompress(a_cinfo);
		return 0;
	}
	/* Now we can initialize the JPEG decompression object. */
	jpeg_create_decompress(a_cinfo);
	
	/* Step 2: specify data source (eg, a file) */
#ifdef READ_FROM_BUFFER
	jpeg_stdio_src(a_cinfo, a_initData->file_buffer, a_initData->file_buffer_size);
#else
	jpeg_stdio_src(&cinfo, infile);
#endif
	
	/* Step 3: read file parameters with jpeg_read_header() */	
	(void) jpeg_read_header(a_cinfo, TRUE);
	
	/* Step 4: set parameters for decompression */
	
	/* Step 5: Start decompressor */
	(void) jpeg_start_decompress(a_cinfo);
	
	row_stride = a_cinfo->output_width * a_cinfo->output_components;
	
	/* Make a one-row-high sample array that will go away when done with image */
	buffer = (*a_cinfo->mem->alloc_sarray)((j_common_ptr) a_cinfo, JPOOL_IMAGE, row_stride, 1);
	
	//(*a_out_buffer) = (*cinfo.mem->alloc_sarray)
	//  ((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, cinfo.output_height);
	
	out_buffer = (JSAMPLE*)realloc(a_output->out_buffer,row_stride*a_cinfo->output_height);
	if (!out_buffer)
	{
		// Some error reporting
		return 0;
	}
	a_output->out_buffer = out_buffer;
	
	a_output->heigth = a_cinfo->output_height;
	a_output->width = a_cinfo->output_width;
	a_output->dataSize = a_cinfo->output_components;
	
	/* Step 6: while (scan lines remain to be read) */
	/*           jpeg_read_scanlines(...); */
	
	/* Here we use the library's state variable cinfo.output_scanline as the
	 * loop counter, so that we don't have to keep track ourselves.
	 */
	while (a_cinfo->output_scanline < a_cinfo->output_height) 
	{
		/* jpeg_read_scanlines expects an array of pointers to scanlines.
		 * Here the array is only one element long, but you could ask for
		 * more than one scanline at a time if that's more convenient.
		 */
		(void) jpeg_read_scanlines(a_cinfo, buffer, 1);
		/* Assume put_scanline_someplace wants a pointer and sample count. */
		put_scanline_someplace(buffer[0], row_stride, a_cinfo, &impl, a_output->out_buffer);
	}
	
	/* Step 7: Finish decompression */
	(void) jpeg_finish_decompress(a_cinfo);
	
	/* Step 8: Release JPEG decompression object */
	/* This is an important step since it will release a good deal of memory. */
	jpeg_destroy_decompress(a_cinfo);
	
	a_output->jpeg_color_space = a_cinfo->jpeg_color_space;
	a_output->out_color_space = a_cinfo->out_color_space;
	
	/* At this point you may want to check to see whether any corrupt-data
	 * warnings occurred (test whether jerr.pub.num_warnings is nonzero).
	 */
	
	/* And we're done! */
	return 1;
}


/*
 * SOME FINE POINTS:
 *
 * In the above code, we ignored the return value of jpeg_read_scanlines,
 * which is the number of scanlines actually read.  We could get away with
 * this because we asked for only one line at a time and we weren't using
 * a suspending data source.  See libjpeg.txt for more info.
 *
 * We cheated a bit by calling alloc_sarray() after jpeg_start_decompress();
 * we should have done it beforehand to ensure that the space would be
 * counted against the JPEG max_memory setting.  In some systems the above
 * code would risk an out-of-memory error.  However, in general we don't
 * know the output image dimensions before jpeg_start_decompress(), unless we
 * call jpeg_calc_output_dimensions().  See libjpeg.txt for more about this.
 *
 * Scanlines are returned in the same order as they appear in the JPEG file,
 * which is standardly top-to-bottom.  If you must emit data bottom-to-top,
 * you can use one of the virtual arrays provided by the JPEG memory manager
 * to invert the data.  See wrbmp.c for an example.
 *
 * As with compression, some operating modes may require temporary files.
 * On some systems you may need to set up a signal handler to ensure that
 * temporary files are deleted if the program is interrupted.  See libjpeg.txt.
 */



/*
 * This routine determines what format the input file is,
 * and selects the appropriate input-reading module.
 *
 * To determine which family of input formats the file belongs to,
 * we may look only at the first byte of the file, since C does not
 * guarantee that more than one character can be pushed back with ungetc.
 * Looking at additional bytes would require one of these approaches:
 *     1) assume we can fseek() the input file (fails for piped input);
 *     2) assume we can push back more than one character (works in
 *        some C implementations, but unportable);
 *     3) provide our own buffering (breaks input readers that want to use
 *        stdio directly, such as the RLE library);
 * or  4) don't put back the data, and modify the input_init methods to assume
 *        they start reading after the start of file (also breaks RLE library).
 * #1 is attractive for MS-DOS but is untenable on Unix.
 *
 * The most portable solution for file types that can't be identified by their
 * first byte is to make the user tell us what they are.  This is also the
 * only approach for "raw" file types that contain only arbitrary values.
 * We presently apply this method for Targa files.  Most of the time Targa
 * files start with 0x00, so we recognize that case.  Potentially, however,
 * a Targa file could start with any byte value (byte 0 is the length of the
 * seldom-used ID field), so we provide a switch to force Targa input mode.
 */
