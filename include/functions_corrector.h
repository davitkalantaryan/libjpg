#ifndef __functions_corrector_h__
#define	__functions_corrector_h__

#ifdef _MSC_VER
#if(_MSC_VER >= 1400)

#include <stdio.h>
/// fopen
static _inline FILE* fopen_new(const char *a_filename, const char *a_mode)
{
	FILE* fpReturn;
	errno_t nError = fopen_s(&fpReturn, a_filename, a_mode);
	if (nError) return NULL;
	return fpReturn;
}
#ifdef fopen
#undef fopen
#endif
#define fopen fopen_new

/// tmpfile
static _inline FILE* tmpfile_new(void)
{
	FILE* fpReturn;
	errno_t nError = tmpfile_s(&fpReturn);
	if (nError) return NULL;
	return fpReturn;
}
#ifdef tmpfile
#undef tmpfile
#endif
#define tmpfile tmpfile_new

/// sprintf
#ifdef sprintf
#undef sprintf
#endif
#define sprintf(_a_buffer_,...)	sprintf_s((_a_buffer_),8196,__VA_ARGS__)

#include <sys/stat.h>
/// fileno
#ifdef fileno
#undef fileno
#endif
#define fileno _fileno

#include <string.h>
/// strncpy
static _inline char *strncpy_new(char *a_strDest, const char *a_strSource, size_t a_count)
{
	strncpy_s(a_strDest, a_count, a_strSource, a_count);
	return a_strDest;
}
#ifdef strncpy
#undef strncpy
#endif
#define strncpy strncpy_new


#endif  // #if(_MSC_VER >= 1400)
#endif  // #ifdef _MSC_VER

#endif  // #ifndef __functions_corrector_h__
