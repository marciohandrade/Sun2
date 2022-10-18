/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sun Aug 06 01:25:44 2000
 */
/* Compiler settings for q:\SPHINX\SQLREPL\SQLMERGX\sqlmergx.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

const IID IID_IAlternateSyncPartner = {0x0A98450D,0x04A9,0x11D3,{0xA8,0xFB,0x00,0xC0,0x4F,0x9F,0xC4,0x36}};


const IID IID_IAlternateSyncPartners = {0x0A98450F,0x04A9,0x11D3,{0xA8,0xFB,0x00,0xC0,0x4F,0x9F,0xC4,0x36}};


const IID IID_ISQLMerge = {0x84923188,0xF94B,0x11d0,{0xA8,0xE8,0x00,0xC0,0x4F,0xC2,0xDE,0xB3}};


const IID LIBID_SQLMERGXLib = {0x08b0b2e7,0x3fb3,0x11d3,{0xa4,0xde,0x00,0xc0,0x4f,0x61,0x01,0x89}};


const IID DIID__SQLMergeEvents = {0x84923186,0xF94B,0x11d0,{0xA8,0xE8,0x00,0xC0,0x4F,0xC2,0xDE,0xB3}};


const CLSID CLSID_SQLMerge = {0x08b0b2e8,0x3fb3,0x11d3,{0xa4,0xde,0x00,0xc0,0x4f,0x61,0x01,0x89}};


const CLSID CLSID_AlternateSyncPartner = {0x0A98450E,0x04A9,0x11D3,{0xA8,0xFB,0x00,0xC0,0x4F,0x9F,0xC4,0x36}};


const CLSID CLSID_AlternateSyncPartners = {0x0A984510,0x04A9,0x11D3,{0xA8,0xFB,0x00,0xC0,0x4F,0x9F,0xC4,0x36}};


#ifdef __cplusplus
}
#endif
