// wzgdiplus.h

#ifndef _COMMON_WZGDIPLUS_H_
#define _COMMON_WZGDIPLUS_H_

#include "wzimagetype.h"

class CDiscAccess;

namespace GDIPlus 
{
    void Startup();
    void Shutdown();

    // 텍스처를 위한 크기 얻기(주어진 크기보다 크거나 같은 2의 승수) 
    int GetSizeForTexture( int iImageSize );

    // 이미지 파일 로딩
    BOOL LoadImage( CDiscAccess* pDiscAccess,
                    const char* lpszFilePath,
                    BYTE** ppbyBuffer,
                    int* piWidth,
                    int* piHeight,
                    int* piImageWidth,
                    int* piImageHeight,
                    int* piNumComponents,
                    WORD wOption = 0, 
                    St_ImageFileInfo* pImageFileInfo = NULL );

}; // namespace GDIPlus

#endif // _COMMON_WZGDIPLUS_H_
