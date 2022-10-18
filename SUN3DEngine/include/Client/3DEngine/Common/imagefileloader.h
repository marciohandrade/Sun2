// imagefileloader.h

#ifndef _COMMON_IMAGEFILELOADER_H_
#define _COMMON_IMAGEFILELOADER_H_

#include "wzgdiplus.h"

// 이미지 크기 재 설정
BOOL ResizeImageBuffer( BYTE* pbyDst, 
                        int iDstWidth,
                        int iDstHeight, 
                        int iNumComponents, 
                        const BYTE* pbySrc, 
                        int iWidth, 
                        int iHeight );

// 이미지 파일 로딩
BOOL LoadImageFile( CDiscAccess* pDiscAccess,
                    const char* lpszFilePath,
                    BYTE** ppbyBuffer, 
                    int* piWidth,
                    int* piHeight,
                    int* piImageWidth,
                    int* piImageHeight, 
                    int* piNumComponents,
                    WORD wOption = 0, 
                    St_ImageFileInfo* pImageFileInfo = NULL );

//------------------------------------------------------------------------------
/**
*/
inline int GetTextureSizeToContain( int iImageSize )
{
    return GDIPlus::GetSizeForTexture( iImageSize );
}

#endif // _COMMON_IMAGEFILELOADER_H_
