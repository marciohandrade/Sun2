#ifndef __XLSSTRUCT_H__
#define __XLSSTRUCT_H__
//#include <xlstypes.h>
#include "ole.h"


//#ifdef __cplusplus
//extern "C" {
//#endif
	
typedef struct BOF
{
    WORD id;
    WORD size;
}
BOF;

typedef struct BIFF
{
    WORD ver;
    WORD type;
    WORD id_make;
    WORD year;
    DWORD flags;
    DWORD min_ver;
    BYTE buf[100];
}
BIFF;

typedef struct BOUNDSHEET
{
    DWORD	filepos;
    BYTE	visibility;
    BYTE	type;
    char	name;
}
BOUNDSHEET;

typedef struct ROW
{
    WORD	index;
    WORD	fcell;
    WORD	lcell;
    WORD	height;
    WORD	notused;
    WORD	notused2; //used only for BIFF3-4
    WORD	flags;
    WORD	xf;
}
ROW;

typedef struct COL
{
    WORD	row;
    WORD	col;
    WORD	xf;
}
COL;


typedef struct FORMULA
{
    WORD	row;
    WORD	col;
    WORD	xf;
    //	ULLONG  res;
    BYTE	resid;
    BYTE	resdata[5];
    WORD	res;
    //	double	res;
    WORD	flags;
    BYTE	chn[4];
    WORD	len;
    BYTE	value[1]; //var
}
FORMULA;

typedef struct RK
{
    WORD	row;
    WORD	col;
    WORD	xf;
    BYTE	value[4];
}
RK;

typedef struct LABELSST
{
    WORD	row;
    WORD	col;
    WORD	xf;
    BYTE	value[4];
}
LABELSST;


typedef struct SST
{
    DWORD	num;
    DWORD	numofstr;
    BYTE	strings;
}
SST;


typedef struct XF
{
    WORD	font;
    WORD	format;
    WORD	type;
    BYTE	align;
    BYTE	rotation;
    BYTE	ident;
    BYTE	usedattr;
    DWORD	linestyle;
    DWORD	linecolor;
    WORD	groundcolor;
}
XF;

typedef struct BR_NUMBER
{
    WORD	row;
    WORD	col;
    WORD	xf;
    double value;
}
BR_NUMBER;

typedef struct COLINFO
{
    WORD	first;
    WORD	last;
    WORD	width;
    WORD	xf;
    WORD	flags;
    WORD	notused;
}
COLINFO;

typedef struct MERGEDCELLS
{
    WORD	rowf;
    WORD	rowl;
    WORD	colf;
    WORD	coll;
}
MERGEDCELLS;

typedef struct FONT
{
    WORD	height;
    WORD	flag;
    WORD	color;
    WORD	bold;
    WORD	escapement;
    BYTE	underline;
    BYTE	family;
    BYTE	charset;
    BYTE	notused;
    char	name;
}
FONT;



//---------------------------------------------------------
typedef struct st_sheet_data
    {
        DWORD filepos;
        BYTE visibility;
        BYTE type;
        char* name;
    }st_sheet_data;

typedef	struct st_sheet
{		//Sheets
    long count;
    
    st_sheet_data* sheet;
}
st_sheet;


typedef struct st_font_data
    {
        WORD	height;
        WORD	flag;
        WORD	color;
        WORD	bold;
        WORD	escapement;
        BYTE	underline;
        BYTE	family;
        BYTE	charset;
        char*	name;
    }st_font_data;

typedef	struct st_font
{
    long count;		//Count of FONT's
    
    st_font_data* font;
}
st_font;


typedef struct st_xf_data
    {
        WORD	font;
        WORD	format;
        WORD	type;
        BYTE	align;
        BYTE	rotation;
        BYTE	ident;
        BYTE	usedattr;
        DWORD	linestyle;
        DWORD	linecolor;
        WORD	groundcolor;
    }st_xf_data;

typedef	struct st_xf
{
    long count;	//Count of XF
    //	XF** xf;
    
    st_xf_data* xf;

}
st_xf;

typedef struct str_sst_string
{
        //	long len;
        char* str;
} str_sst_string;

typedef	struct st_sst
{
    DWORD count;
    DWORD lastid;
    DWORD lastunc;
    str_sst_string* string;
}
st_sst;


typedef struct st_cell_data
    {
        WORD	id;
        WORD	row;
        WORD	col;
        WORD	xf;
        double	d;
        long	l;
        char*	str;		//String value;
        BYTE	ishiden;		//Is cell hident
        WORD	width;		//Width of col
        WORD	colspan;
        WORD	rowspan;
    }st_cell_data;

typedef	struct st_cell
{
    DWORD count;
    
    st_cell_data* cell;
}
st_cell;

typedef struct st_row_data
{
        WORD index;
        WORD fcell;
        WORD lcell;
        WORD height;
        WORD flags;
        WORD xf;
        BYTE xfflags;
        st_cell cells;
} st_row_data;


typedef	struct st_row
{
    //	DWORD count;
    WORD lastcol;
    WORD lastrow;
    st_row_data * row;
    /*struct st_row_data
    {
        WORD index;
        WORD fcell;
        WORD lcell;
        WORD height;
        WORD flags;
        WORD xf;
        BYTE xfflags;
        st_cell cells;
    }
    * row;*/
}
st_row;


typedef struct st_colinfo_data
{
        WORD	first;
        WORD	last;
        WORD	width;
        WORD	xf;
        WORD	flags;
} st_colinfo_data;
 
typedef	struct st_colinfo
{
    long count;	//Count of COLINFO
   
    st_colinfo_data* col;
}
st_colinfo;

typedef struct xlsWorkBook
{
    //FILE*		file;		//
    OLE2Stream*	olest;
    long		filepos;	//position in file

    //From Header (BIFF)
    BYTE		is5ver;
    WORD		type;

    //Other data
    WORD		codepage;	//Charset codepage
    char*		charset;
    st_sheet	sheets;
    st_sst		sst;		//SST table
    st_xf		xfs;		//XF table
    st_font		fonts;
}
xlsWorkBook;

typedef struct xlsWorkSheet
{
    DWORD		filepos;
    WORD		defcolwidth;
    st_row		rows;
    xlsWorkBook * 	workbook;
    st_colinfo	colinfo;
    WORD		maxcol;
}
xlsWorkSheet;

//#ifdef __cplusplus
//}
//#endif

#endif //__XLSSTRUCT_H__