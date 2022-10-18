#ifndef __OLE_H__
#define __OLE_H__


//#ifdef __cplusplus
//extern "C" {
//#endif
	

#include <stdio.h>
#include "xlstypes.h"
#include <Windows.h>
#pragma pack(1)

typedef struct TIME_T
{
    DWORD	LowDate;
    DWORD	HighDate;
}
TIME_T;

typedef struct OLE2Header
{
    DWORD		id[2];		//D0CF11E0 A1B11AE1
    DWORD		clid[4];
    WORD		verminor;	//0x3e
    WORD		verdll;		//0x3
    WORD		byteorder;
    WORD		lsector;
    WORD		lmsector;

    WORD		reserved1;
    DWORD		reserved2;
    DWORD		reserved3;

    DWORD		cfat;
    DWORD		dirstart;

    DWORD		signature;

    DWORD		msectorcutoff;
    DWORD		mfatstart;
    DWORD		cmfat;
    DWORD		difstart;
    DWORD		cdif;
    DWORD		FAT[109];
}
OLE2Header;


//-----------------------------------------------------------------------------------
typedef	struct st_olefiles_data
{
        char*	name;
        DWORD	start;
        DWORD	size;
} st_olefiles_data;
   
typedef	struct st_olefiles
{
    long count;
 
    st_olefiles_data* file;
}
st_olefiles;

/*typedef	struct st_olefiles
{
    long count;
    struct st_olefiles_data
    {
        char*	name;
        DWORD	start;
        DWORD	size;
    }
    * file;
}
st_olefiles;*/

typedef struct OLE2
{
    FILE*			file;
    HANDLE		vFile;
    int			lsector;
    int			lmsector;
    DWORD		cfat;
    DWORD		dirstart;

    DWORD		msectorcutoff;
    DWORD		mfatstart;
    DWORD		cmfat;
    DWORD		difstart;
    DWORD		cdif;
    DWORD*		FAT;
    st_olefiles	files;
}
OLE2;

typedef struct OLE2Stream
{
    OLE2*	ole;
    DWORD	start;
    DWORD	pos;
    int		cfat;
    int		size;
    DWORD	fatpos;
    BYTE*	buf;
    DWORD	bufsize;
    BYTE	eof;
}
OLE2Stream;

typedef struct PSS
{
    BYTE	name[64];
    WORD	bsize;
    BYTE	type;		//STGTY
    BYTE	flag;		//DECOLOR
    DWORD	left;
    DWORD	right;
    DWORD	child;
    WORD	guid[8];
    DWORD	userflags;
    TIME_T	time[2];
    DWORD	sstart;
    DWORD	size;
    DWORD	proptype;
}
PSS;

#pragma pack()

int ole2_read(void* buf,long size,long count,OLE2Stream* olest);
OLE2Stream* ole2_sopen(OLE2* ole,DWORD start);
void ole2_seek(OLE2Stream* olest,DWORD ofs);
OLE2Stream*  ole2_fopen(OLE2* ole,char* file);
void ole2_fclose(OLE2Stream* ole2st);
OLE2* ole2_open(char *file);
void ole2_close(OLE2* ole2);
//void file_ReadFile(string& strData);

//#ifdef __cplusplus
//}
//#endif

#endif //__OLE_H__