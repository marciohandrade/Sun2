// wzuniteditable.h

#ifndef _3DFILEACCESS_WZUNITEDITABLE_H_
#define _3DFILEACCESS_WZUNITEDITABLE_H_

#include "wzunit.h"

//------------------------------------------------------------------------------
/**
    @class CWzUnitEditable
*/
class CWzUnitEditable : public CWzUnit
{
public:
    // ������/�Ҹ���
    CWzUnitEditable();
    virtual ~CWzUnitEditable();

    // LOD �޽� ���ϸ� ����/����
    WZ3DA_API void SetWzdFileName( const char* lpszFileName, int iLOD );
    WZ3DA_API void DelWzdFileName( int iLOD );

    // �ִϸ��̼� ���� ����/����
    WZ3DA_API void SetAnimationInfoByIndex( const WzAnimationInfo* pInfo, int iIndex );
    WZ3DA_API void DelAnimationInfoByIndex( int iIndex );

    // �� ���� ����/����
    WZ3DA_API void SetBoneInfoByIndex( const WzBoneInfo* pInfo, int iIndex );
    WZ3DA_API void DelBoneInfoByIndex( int iIndex );

    // �� ���� ����
    WZ3DA_API void SetDiffuseMapInfoByIndex( const WzMappingInfo* pInfo, int iIndex );
    WZ3DA_API void SetSpecularMapInfoByIndex( const WzMappingInfo* pInfo, int iIndex );
    WZ3DA_API void SetNormalMapInfoByIndex( const WzMappingInfo* pInfo, int iIndex );
    WZ3DA_API void SetGlowMapInfoByIndex( const WzMappingInfo* pInfo, int iIndex );

    // ���� �ؽ�ó ����/����
    WZ3DA_API void SetInternalMapInfoByIndex( const WzMappingInfo* pInfo, int iIndex );
    WZ3DA_API void DelInternalMapInfoByIndex( int iIndex );

    // �ؽ�ó �ִϸ��̼� �� ����/����
    WZ3DA_API void SetTexAniMapInfoByIndex( const WzTexAniMappingInfo* pInfo, int iIndex );
    WZ3DA_API void DelTexAniMapInfoByIndex( int iIndex );

    // �ؽ�ó �ִϸ��̼� �� ����/����
    WZ3DA_API void SetTexAniSetInfoByIndex( const WzTexAniSetInfo* pInfo, int iIndex );
    WZ3DA_API void DelTexAniSetInfoByIndex( int iIndex );

    // �ؽ�ó �ִϸ��̼� ID �߰�
    WZ3DA_API void AddTexAniID( WzID wiTexAni );

    // �ؽ�ó �ִϸ��̼� �¿� �ؽ�ó ID �߰�/����
    WZ3DA_API void AddTexIDInTexAniSetInfo( WzID wiTex, int iIndex );
    WZ3DA_API void DelTexIDInTexAniSetInfo( int iTexIdIndex, int iAniSetIndex );

    // ���� Ÿ�� ����/����
    WZ3DA_API CWzUnitSubType* CreateSubType();
    WZ3DA_API void DelSubType( int iIndex );

    // ���� Ÿ�� ����
    WZ3DA_API void CopySubType( int iSrcIndex, int iDstIndex );

    // �ٿ�� ���� ���� ���� ���� ����
    WZ3DA_API void SetHasBVInfo( BOOL bHasBVInfo );

    // �߰� �ٿ�� ���� ����/����
    WZ3DA_API void SetBVExtraInfoByIndex( const WzBVExtraInfo* pInfo, int iIndex );
    WZ3DA_API void DelBVExtraInfoByIndex( int iIndex );

    // ���� LOD Ȱ�� ���� ����
    WZ3DA_API void EnableLodByForce( bool bEnable = true );
};

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitEditable::SetHasBVInfo( BOOL bHasBVInfo )
{
    m_bHasBVInfo = bHasBVInfo; 
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitEditable::EnableLodByForce( bool bEnable )
{
    m_bLodByForce = bEnable; 
}

#endif // _3DFILEACCESS_WZUNITEDITABLE_H_