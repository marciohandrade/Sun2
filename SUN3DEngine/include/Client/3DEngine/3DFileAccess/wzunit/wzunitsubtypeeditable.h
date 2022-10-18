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
    // ������/�Ҹ���
    CWzUnitSubTypeEditable();
    virtual ~CWzUnitSubTypeEditable();

    // �� ũ�� ����/����
    WZ3DA_API void SetBoneScaling( const WzBoneScaling* pBoneScaling, int idx );
    WZ3DA_API void DelBoneScaling( int idx );

    // ��� ����/����
    WZ3DA_API void SetDecoration( const WzDecoration* pDecoration, int idx );
    WZ3DA_API void DelDecoration( int idx );

    // ������ ���� ����/����
    WZ3DA_API void SetRenderInfo( const WzRenderInfo* pRenderInfo, int idx );
    WZ3DA_API void DelRenderInfo( int idx );
};

#endif // _3DFILEACCESS_WZUNITSUBTYPEEDITABLE_H_