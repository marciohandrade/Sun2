// wzgdiplus.h

#ifndef _COMMON_WZGDIPLUS_H_
#define _COMMON_WZGDIPLUS_H_

#include "wzimagetype.h"

class CDiscAccess;

namespace GDIPlus 
{
    void Startup();
    void Shutdown();

    // �ؽ�ó�� ���� ũ�� ���(�־��� ũ�⺸�� ũ�ų� ���� 2�� �¼�) 
    int GetSizeForTexture( int iImageSize );

    // �̹��� ���� �ε�
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
