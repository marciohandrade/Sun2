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
    // 생성자/소멸자
    CWzUnitEditable();
    virtual ~CWzUnitEditable();

    // LOD 메시 파일명 설정/제거
    WZ3DA_API void SetWzdFileName( const char* lpszFileName, int iLOD );
    WZ3DA_API void DelWzdFileName( int iLOD );

    // 애니메이션 정보 설정/제거
    WZ3DA_API void SetAnimationInfoByIndex( const WzAnimationInfo* pInfo, int iIndex );
    WZ3DA_API void DelAnimationInfoByIndex( int iIndex );

    // 본 정보 설정/제거
    WZ3DA_API void SetBoneInfoByIndex( const WzBoneInfo* pInfo, int iIndex );
    WZ3DA_API void DelBoneInfoByIndex( int iIndex );

    // 맵 정보 설정
    WZ3DA_API void SetDiffuseMapInfoByIndex( const WzMappingInfo* pInfo, int iIndex );
    WZ3DA_API void SetSpecularMapInfoByIndex( const WzMappingInfo* pInfo, int iIndex );
    WZ3DA_API void SetNormalMapInfoByIndex( const WzMappingInfo* pInfo, int iIndex );
    WZ3DA_API void SetGlowMapInfoByIndex( const WzMappingInfo* pInfo, int iIndex );

    // 내부 텍스처 설정/제거
    WZ3DA_API void SetInternalMapInfoByIndex( const WzMappingInfo* pInfo, int iIndex );
    WZ3DA_API void DelInternalMapInfoByIndex( int iIndex );

    // 텍스처 애니메이션 맵 설정/제거
    WZ3DA_API void SetTexAniMapInfoByIndex( const WzTexAniMappingInfo* pInfo, int iIndex );
    WZ3DA_API void DelTexAniMapInfoByIndex( int iIndex );

    // 텍스처 애니메이션 셋 설정/제거
    WZ3DA_API void SetTexAniSetInfoByIndex( const WzTexAniSetInfo* pInfo, int iIndex );
    WZ3DA_API void DelTexAniSetInfoByIndex( int iIndex );

    // 텍스처 애니메이션 ID 추가
    WZ3DA_API void AddTexAniID( WzID wiTexAni );

    // 텍스처 애니메이션 셋에 텍스처 ID 추가/제거
    WZ3DA_API void AddTexIDInTexAniSetInfo( WzID wiTex, int iIndex );
    WZ3DA_API void DelTexIDInTexAniSetInfo( int iTexIdIndex, int iAniSetIndex );

    // 서브 타입 생성/제거
    WZ3DA_API CWzUnitSubType* CreateSubType();
    WZ3DA_API void DelSubType( int iIndex );

    // 서브 타입 복사
    WZ3DA_API void CopySubType( int iSrcIndex, int iDstIndex );

    // 바운딩 볼륨 정보 존재 여부 설정
    WZ3DA_API void SetHasBVInfo( BOOL bHasBVInfo );

    // 추가 바운딩 볼륨 설정/제거
    WZ3DA_API void SetBVExtraInfoByIndex( const WzBVExtraInfo* pInfo, int iIndex );
    WZ3DA_API void DelBVExtraInfoByIndex( int iIndex );

    // 강제 LOD 활성 여부 설정
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