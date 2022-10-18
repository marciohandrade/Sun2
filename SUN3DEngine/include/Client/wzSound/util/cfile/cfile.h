#ifndef __CFILE_H__
#define __CFILE_H__

#include "PreProcDefines.h"

#include "util/basetype.h"
//#include "util/ObjectPool.h"

#define DIR_SEPARATOR_CHAR '\\'
#define DIR_SEPARATOR_STR  "\\"

#define CF_EOF (-1)
#define CF_SEEK_SET (0)
#define CF_SEEK_CUR (1)
#define CF_SEEK_END (2)


typedef struct CFILE 
{
	FILE		*fp;		 // File pointer if opening an individual file
	int		type;				// CFILE_BLOCK_UNUSED, CFILE_BLOCK_USED
	
	HANDLE  hVFSFile;	// Handle from VFS

	int		lib_offset;
	int		raw_position;
	int		size;

//	static void* operator new(size_t size)
//	{
//		return m_ObjectPool.alloc();
//	}
//
//	static void operator delete(void* ptr, size_t size)
//	{
//		m_ObjectPool.release(static_cast<CFILE*>(ptr)) ;
//	}
//
//	static ObjectPool<CFILE> m_ObjectPool;
} CFILE;

#define CFILE_BLOCK_UNUSED		0
#define CFILE_BLOCK_USED		1

#define MAX_CFILE_BLOCKS	64
extern CFILE Cfile_list[MAX_CFILE_BLOCKS];

#define CF_MAX_FILENAME_LENGTH 	32		// Includes null terminater, so real length is 31
#define CF_MAX_PATHNAME_LENGTH 	256	// Includes null terminater, so real length is 255

#define CFILE_NORMAL				0			// open file normally
#define CFILE_VIRTUAL	(1<<0)	//	open file as a virtual file

// Opens the file.  If no path is given, use the extension to look into the
// default path.  If mode is NULL, delete the file.  
CFILE *cfopen(char *filename, char *mode, int type = CFILE_NORMAL);

// Closes the file
int cfclose(CFILE *cfile);

// Returns size of file...
int cfilelength(CFILE *fp);

// Reads data
int cfread(void *buf, int elsize, int nelem, CFILE *fp);

// Moves the file pointer
int cfseek(CFILE *fp, int offset, int where);

// Returns current position of file.
int cftell(CFILE *fp);

//////////////////////////////////////////////////////////////////////////
int cfgetc(CFILE *fp);
char *cfgets(char *buf, size_t n, CFILE *fp);
char cfread_char(CFILE *file, char deflt = 0);
ubyte cfread_ubyte(CFILE *file, ubyte deflt = 0);
short cfread_short(CFILE *file, short deflt = 0);
ushort cfread_ushort(CFILE *file, ushort deflt = 0);
int cfread_int(CFILE *file, int deflt = 0);
uint cfread_uint(CFILE *file, uint deflt = 0);
float cfread_float(CFILE *file, float deflt = 0.0f);

// Reads variable length, null-termined string.   Will only read up
// to n characters.
void cfread_string(char *buf,int n, CFILE *file);
// Read a fixed length that is null-terminatedm, and has the length
// stored in file
void cfread_string_len(char *buf,int n, CFILE *file);

CFILE* cfget_cfile_block();
void cffree_cfile_block(CFILE *pCP);
#endif	/* __CFILE_H__ */
