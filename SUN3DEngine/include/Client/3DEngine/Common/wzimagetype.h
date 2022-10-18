// wzimagetype.h

#ifndef _COMMON_WZIMAGETYPE_H_
#define _COMMON_WZIMAGETYPE_H_

#include "wzcommontypedef.h"

#define LIF_TOBETEXTURE     (0x0001)   // 텍스쳐로 쓰일 것 : 크기를 2^n 으로 잡는다.
#define LIF_NONALPHA        (0x0002)   // 알파BIT를 사용하지 않도록(TGA에서만 테스트)
#define LIF_ONLY8BIT        (0x0004)   // 8BIT만 사용하도록(TGA에서만 테스트)

// for DXTn
enum PixFormat
{
    PF_ARGB,
    PF_RGB,
    PF_DXT1,
    PF_DXT2,
    PF_DXT3,
    PF_DXT4,
    PF_DXT5,
    PF_UNKNOWN
};

struct St_ImageNameInfo
{
    BOOL m_bAlphaTest;      // _h
    BOOL m_bDisableZWrite;  // _z
    BOOL m_bOrigin;         // _32
    BOOL m_bXRGB;           // _x
};

struct St_ImageFileInfo
{
    int m_iWithAlpha;       // 0 - 없음, 1 - 보통 alpha, 2 - 뚫어주는 이미지
    PixFormat m_pixFormat;  // 현재 DDS 파일만 유효
    int m_iBufferSize;
    int m_iMipMapCount;
    St_ImageNameInfo m_NameInfo;
};


#pragma pack (1)

// TGA 관련
#define TGA_TRUECOLOR       2
#define TGA_TRUECOLOR_RLE   10  // RLE(Run-Length Encoded)

struct St_TargaHead
{
    BYTE m_byIDLen;
    BYTE m_byColMapPresent;
    BYTE m_byImageType;
    WORD m_wFirstEntry;
    WORD m_wColMapLen;
    BYTE m_byColMapEntSize;

    short m_sOriginX;
    short m_sOriginY;
    WORD m_wWidth;
    WORD m_wHeight;
    BYTE m_byBpp;
    BYTE m_byImageDesc;
};

// Targa origins
#define TGA_IMAGEDESC_ORIGIN_MASK   0x30
#define TGA_IMAGEDESC_TOPLEFT       0x20
#define TGA_IMAGEDESC_BOTLEFT       0x00
#define TGA_IMAGEDESC_BOTRIGHT      0x10
#define TGA_IMAGEDESC_TOPRIGHT      0x30


// DDS 관련
#define DDS_FOURCC 0x00000004  // DDPF_FOURCC
#define DDS_RGB    0x00000040  // DDPF_RGB
#define DDS_RGBA   0x00000041  // DDPF_RGB | DDPF_ALPHAPIXELS

#define DDS_HEADER_FLAGS_TEXTURE    0x00001007  // DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT 
#define DDS_HEADER_FLAGS_MIPMAP     0x00020000  // DDSD_MIPMAPCOUNT
#define DDS_HEADER_FLAGS_VOLUME     0x00800000  // DDSD_DEPTH
#define DDS_HEADER_FLAGS_PITCH      0x00000008  // DDSD_PITCH
#define DDS_HEADER_FLAGS_LINEARSIZE 0x00080000  // DDSD_LINEARSIZE

#define DDS_SURFACE_FLAGS_TEXTURE 0x00001000 // DDSCAPS_TEXTURE
#define DDS_SURFACE_FLAGS_MIPMAP  0x00400008 // DDSCAPS_COMPLEX | DDSCAPS_MIPMAP
#define DDS_SURFACE_FLAGS_CUBEMAP 0x00000008 // DDSCAPS_COMPLEX

#define DDS_CUBEMAP_POSITIVEX 0x00000600 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEX
#define DDS_CUBEMAP_NEGATIVEX 0x00000a00 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEX
#define DDS_CUBEMAP_POSITIVEY 0x00001200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEY
#define DDS_CUBEMAP_NEGATIVEY 0x00002200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEY
#define DDS_CUBEMAP_POSITIVEZ 0x00004200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEZ
#define DDS_CUBEMAP_NEGATIVEZ 0x00008200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEZ

#define DDS_CUBEMAP_ALLFACES ( DDS_CUBEMAP_POSITIVEX | DDS_CUBEMAP_NEGATIVEX |\
                               DDS_CUBEMAP_POSITIVEY | DDS_CUBEMAP_NEGATIVEY |\
                               DDS_CUBEMAP_POSITIVEZ | DDS_CUBEMAP_NEGATIVEZ )

#define DDS_FLAGS_VOLUME 0x00200000 // DDSCAPS2_VOLUME

struct DDS_PixelFormat
{
    DWORD dwSize;
    DWORD dwFlags;
    DWORD dwFourCC;
    DWORD dwRGBBitCount;
    DWORD dwRBitMask;
    DWORD dwGBitMask;
    DWORD dwBBitMask;
    DWORD dwABitMask;
};

struct DDS_Header
{
    BYTE  bySignature[4];
    DWORD dwSize;
    DWORD dwHeaderFlags;
    DWORD dwHeight;
    DWORD dwWidth;
    DWORD dwPitchOrLinearSize;
    DWORD dwDepth; // only if DDS_HEADER_FLAGS_VOLUME is set in dwHeaderFlags
    DWORD dwMipMapCount;
    DWORD dwReserved1[11];
    DDS_PixelFormat ddspf;
    DWORD dwSurfaceFlags;
    DWORD dwCubemapFlags;
    DWORD dwReserved2[3];
};

const DDS_PixelFormat DDSPF_DXT1     = { sizeof(DDS_PixelFormat), DDS_FOURCC, MAKEFOURCC('D','X','T','1'), 0, 0, 0, 0, 0 };
const DDS_PixelFormat DDSPF_DXT2     = { sizeof(DDS_PixelFormat), DDS_FOURCC, MAKEFOURCC('D','X','T','2'), 0, 0, 0, 0, 0 };
const DDS_PixelFormat DDSPF_DXT3     = { sizeof(DDS_PixelFormat), DDS_FOURCC, MAKEFOURCC('D','X','T','3'), 0, 0, 0, 0, 0 };
const DDS_PixelFormat DDSPF_DXT4     = { sizeof(DDS_PixelFormat), DDS_FOURCC, MAKEFOURCC('D','X','T','4'), 0, 0, 0, 0, 0 };
const DDS_PixelFormat DDSPF_DXT5     = { sizeof(DDS_PixelFormat), DDS_FOURCC, MAKEFOURCC('D','X','T','5'), 0, 0, 0, 0, 0 };
const DDS_PixelFormat DDSPF_A8R8G8B8 = { sizeof(DDS_PixelFormat), DDS_RGBA, 0, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000 };
const DDS_PixelFormat DDSPF_A1R5G5B5 = { sizeof(DDS_PixelFormat), DDS_RGBA, 0, 16, 0x00007c00, 0x000003e0, 0x0000001f, 0x00008000 };
const DDS_PixelFormat DDSPF_A4R4G4B4 = { sizeof(DDS_PixelFormat), DDS_RGBA, 0, 16, 0x00000f00, 0x000000f0, 0x0000000f, 0x0000f000 };
const DDS_PixelFormat DDSPF_R8G8B8   = { sizeof(DDS_PixelFormat), DDS_RGB, 0, 24, 0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000 };
const DDS_PixelFormat DDSPF_R5G6B5   = { sizeof(DDS_PixelFormat), DDS_RGB, 0, 16, 0x0000f800, 0x000007e0, 0x0000001f, 0x00000000 };

#pragma pack ()

#endif // _COMMON_WZIMAGETYPE_H_