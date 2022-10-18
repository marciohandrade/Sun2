// drawunitrenderer.h

#ifndef _3DRENDERER_DRAWUNITRENDERER_H_
#define _3DRENDERER_DRAWUNITRENDERER_H_

#include "drawrenderetcs.h"

//------------------------------------------------------------------------------
/**
    @class CDrawUnitRenderer

    - 일단은 단순히 코드만 정리한 수준이다.
      후에, 렌더러 분석시 정확하게 구현할 것
*/
class CDrawUnitRenderer : public CDrawRenderEtcs
{
public:
    // 생성자/소멸자
    CDrawUnitRenderer();
    virtual ~CDrawUnitRenderer();

    // 유닛 프레임 진행 (xxx: 기존 호환)
    virtual ENUM_ANI_FRAME_STATUS ProgressUnitFrame( CWzUnitDraw* pUnitDraw, 
                                                     float fFrameAdd,
                                                     WzEvent* pEvent = NULL,
                                                     float* pfProgressRate = NULL );

    // 유닛 애니메이션 (xxx: 기존 호환)
    virtual void AnimateUnit( CWzUnitDraw* pUnitDraw, 
                              int iLOD = 0, 
                              const WzMatrix* pwmParent = NULL,
                              BOOL bUpdateBVExtra = FALSE );

    // 유닛 렌더링
    virtual BOOL RenderUnit(CWzUnitDraw* pUnitDraw, St_RenderCommand* prc = NULL, int iLOD = 0);

    virtual BOOL RenderUnitForOutline(CWzUnitDraw* unit_draw);

    // 유닛 본 렌더링
    virtual BOOL RenderUnitBone( CWzUnitDraw* pUnitDraw, int iStart = 0, int iEnd = -1 );

    // 풀 그리기
    virtual BOOL RenderGrass( CWzUnitDraw* pUnitDraw );

    // 2D 모드 설정
    virtual void Set2DMode( BOOL b2D = TRUE, BOOL bInRealSize = FALSE );

    // 유닛 이펙트 생성/이동
    virtual void CheckEffect( CWzUnitDraw* pUnitDraw, BOOL bCreateNow = FALSE );
    virtual void MoveUnitEffect( CWzUnitDraw* pUnitDraw );

    // 유닛 이펙트 보이기/감추기
    virtual void ShowSubTypeEffect( CWzUnitDraw* pUnitDraw, BOOL bShow );

protected:
    // -------- 렌더링 커맨드 생성/설정/추가 --------

    // 효과 렌더링 정보 설정
    void FillRenderInfoForSet( WzRenderInfo* pwri );

    // 렌더링 정보 -> 렌더링 커맨드
    void New_AnalyzeRenderInfo( St_RenderCommand* prc, const WzRenderInfo* pwri );
    // 맵핑 정보 -> 렌더링 커맨드
    void New_AnalyzeMappingInfo( St_RenderCommand* prc, const WzRenderInfo* pwri );

    // 렌더링 커맨드 적용 (전체 메시 그룹)
    void New_ApplyRenderCommandForMesh( const St_RenderCommand* prc, 
                                        const WzRenderInfo* pwri );

    // 렌더링 커맨드 적용 (해당 메시 그룹)
    void New_ApplyRenderCommandForMeshGroup( const St_RenderCommand* prc, 
                                             const WzRenderInfo* pwri, 
                                             int iMeshGroup );

    // -------- 렌더링 커맨드 실제 렌더링 --------

    // 예약된 drawcall 전부 처리
    virtual void FlushReservedDrawCallEx( BOOL bLastFlush );

    // 모든 렌더링 커맨드 처리
    void FlushRenderCommandArray();
    // 그림자 주기 커맨드 처리
    void FlushShadowCastCommand();

    // 렌더링 커맨드 처리
    BOOL RealizeRenderCommmand( St_BaseRenderCommand* prc ); 
    BOOL New_RealizeRenderCommmand( St_RenderCommand* prc );

    // 적용할 버텍스/픽셀 셰이더 조사
    virtual int New_CheckVertexShaderInRealizing( St_RenderCommand* prc );
    virtual int New_CheckPixelShaderInRealizing( St_RenderCommand* prc );

    // 버텍스 셰이더 조정
    void AdjustVertexShaderForSkinning( const St_RenderCommand* prc, 
                                        ENUM_VERTEX_SHADER_TYPE* eVstTry );

    // 렌더링 스타일 적용/복구
    void New_RealizeRenderStyle( St_RenderCommand* prc, int iVS = 0, int iPS = 0 );
    void New_FinishRealizeRenderStyle( St_RenderCommand* prc );

    // 그림자 렌더링
    BOOL RealizeShadowCastCommmand( St_RenderCommand* prc ); 
    // 그림자 받기 (self shadow)
    BOOL RealizeShadowReceiveCommmand( St_RenderCommand* prc );
    // 그림자 렌더링 (xy 평면)
    BOOL RealizeShadowCastToProjectedTexture( St_RenderCommand* prc );

    // 유닛 이펙트 제거
    void DestroyUnitEffect( CWzUnitDraw* pUnitDraw );
};

#endif // _3DRENDERER_DRAWUNITRENDERER_H_