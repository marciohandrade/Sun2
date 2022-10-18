// wzunitsubtypeeditable.h

#ifndef _3DFILEACCESS_WZUNITSUBTYPEEDITABLE_H_
#define _3DFILEACCESS_WZUNITSUBTYPEEDITABLE_H_

#include "wzunitsubtype.h"

//------------------------------------------------------------------------------
/**
    @class CWzUnitSubTypeEditable
*/
class CWzUnitSubTypeEditable : public CWzUnitSubType
{
public:
    // 생성자/소멸자
    CWzUnitSubTypeEditable();
    virtual ~CWzUnitSubTypeEditable();

    // 본 크기 설정/제거
    WZ3DA_API void SetBoneScaling( const WzBoneScaling* pBoneScaling, int idx );
    WZ3DA_API void DelBoneScaling( int idx );

    // 장식 설정/제거
    WZ3DA_API void SetDecoration( const WzDecoration* pDecoration, int idx );
    WZ3DA_API void DelDecoration( int idx );

    // 렌더링 정보 설정/제거
    WZ3DA_API void SetRenderInfo( const WzRenderInfo* pRenderInfo, int idx );
    WZ3DA_API void DelRenderInfo( int idx );
};

#endif // _3DFILEACCESS_WZUNITSUBTYPEEDITABLE_H_