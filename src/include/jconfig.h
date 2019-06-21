/* jconfig.vc --- jconfig.h for Microsoft Visual C++ on Windows 9x or NT. */
/* This file also works for Borland C++ 32-bit (bcc32) on Windows 9x or NT. */
/* see jconfig.txt for explanations */

#ifndef HAVE_PROTOTYPES
#define HAVE_PROTOTYPES
#endif
#define HAVE_UNSIGNED_CHAR
#define HAVE_UNSIGNED_SHORT
/* #define void char */
/* #define const */
#undef CHAR_IS_UNSIGNED
#ifndef HAVE_STDDEF_H
#define HAVE_STDDEF_H
#endif
#ifndef HAVE_STDLIB_H
#define HAVE_STDLIB_H
#endif
#undef NEED_BSD_STRINGS
#undef NEED_SYS_TYPES_H
#undef NEED_FAR_POINTERS	/* we presume a 32-bit flat memory model */
#undef NEED_SHORT_EXTERNAL_NAMES
#undef INCOMPLETE_TYPES_BROKEN

/* Define "boolean" as unsigned char, not enum, per Windows custom */
#ifndef __RPCNDR_H__		/* don't conflict if rpcndr.h already read */
#ifndef HAVE_BOOLEAN
typedef unsigned char boolean;
#define HAVE_BOOLEAN		/* prevent jmorecfg.h from redefining it */
#endif
#endif
#ifndef FALSE			/* in case these macros already exist */
#define FALSE	0		/* values of boolean */
#endif
#ifndef TRUE
#define TRUE	1
#endif


#ifdef JPEG_INTERNALS

#undef RIGHT_SHIFT_IS_UNSIGNED

#endif /* JPEG_INTERNALS */

#ifdef JPEG_CJPEG_DJPEG

#define BMP_SUPPORTED		/* BMP image file format */
#define GIF_SUPPORTED		/* GIF image file format */
#define PPM_SUPPORTED		/* PBMPLUS PPM/PGM image file format */
#undef RLE_SUPPORTED		/* Utah RLE image file format */
#define TARGA_SUPPORTED		/* Targa image file format */

#define TWO_FILE_COMMANDLINE	/* optional */
#define USE_SETMODE		/* Microsoft has setmode() */
#undef NEED_SIGNAL_CATCHER
#undef DONT_USE_B_MODE
#undef PROGRESS_REPORT		/* optional */

#endif /* JPEG_CJPEG_DJPEG */