// drawunitrenderer.h

#ifndef _3DRENDERER_DRAWUNITRENDERER_H_
#define _3DRENDERER_DRAWUNITRENDERER_H_

#include "drawrenderetcs.h"

//------------------------------------------------------------------------------
/**
    @class CDrawUnitRenderer

    - �ϴ��� �ܼ��� �ڵ常 ������ �����̴�.
      �Ŀ�, ������ �м��� ��Ȯ�ϰ� ������ ��
*/
class CDrawUnitRenderer : public CDrawRenderEtcs
{
public:
    // ������/�Ҹ���
    CDrawUnitRenderer();
    virtual ~CDrawUnitRenderer();

    // ���� ������ ���� (xxx: ���� ȣȯ)
    virtual ENUM_ANI_FRAME_STATUS ProgressUnitFrame( CWzUnitDraw* pUnitDraw, 
                                                     float fFrameAdd,
                                                     WzEvent* pEvent = NULL,
                                                     float* pfProgressRate = NULL );

    // ���� �ִϸ��̼� (xxx: ���� ȣȯ)
    virtual void AnimateUnit( CWzUnitDraw* pUnitDraw, 
                              int iLOD = 0, 
                              const WzMatrix* pwmParent = NULL,
                              BOOL bUpdateBVExtra = FALSE );

    // ���� ������
    virtual BOOL RenderUnit(CWzUnitDraw* pUnitDraw, St_RenderCommand* prc = NULL, int iLOD = 0);

    virtual BOOL RenderUnitForOutline(CWzUnitDraw* unit_draw);

    // ���� �� ������
    virtual BOOL RenderUnitBone( CWzUnitDraw* pUnitDraw, int iStart = 0, int iEnd = -1 );

    // Ǯ �׸���
    virtual BOOL RenderGrass( CWzUnitDraw* pUnitDraw );

    // 2D ��� ����
    virtual void Set2DMode( BOOL b2D = TRUE, BOOL bInRealSize = FALSE );

    // ���� ����Ʈ ����/�̵�
    virtual void CheckEffect( CWzUnitDraw* pUnitDraw, BOOL bCreateNow = FALSE );
    virtual void MoveUnitEffect( CWzUnitDraw* pUnitDraw );

    // ���� ����Ʈ ���̱�/���߱�
    virtual void ShowSubTypeEffect( CWzUnitDraw* pUnitDraw, BOOL bShow );

protected:
    // -------- ������ Ŀ�ǵ� ����/����/�߰� --------

    // ȿ�� ������ ���� ����
    void FillRenderInfoForSet( WzRenderInfo* pwri );

    // ������ ���� -> ������ Ŀ�ǵ�
    void New_AnalyzeRenderInfo( St_RenderCommand* prc, const WzRenderInfo* pwri );
    // ���� ���� -> ������ Ŀ�ǵ�
    void New_AnalyzeMappingInfo( St_RenderCommand* prc, const WzRenderInfo* pwri );

    // ������ Ŀ�ǵ� ���� (��ü �޽� �׷�)
    void New_ApplyRenderCommandForMesh( const St_RenderCommand* prc, 
                                        const WzRenderInfo* pwri );

    // ������ Ŀ�ǵ� ���� (�ش� �޽� �׷�)
    void New_ApplyRenderCommandForMeshGroup( const St_RenderCommand* prc, 
                                             const WzRenderInfo* pwri, 
                                             int iMeshGroup );

    // -------- ������ Ŀ�ǵ� ���� ������ --------

    // ����� drawcall ���� ó��
    virtual void FlushReservedDrawCallEx( BOOL bLastFlush );

    // ��� ������ Ŀ�ǵ� ó��
    void FlushRenderCommandArray();
    // �׸��� �ֱ� Ŀ�ǵ� ó��
    void FlushShadowCastCommand();

    // ������ Ŀ�ǵ� ó��
    BOOL RealizeRenderCommmand( St_BaseRenderCommand* prc ); 
    BOOL New_RealizeRenderCommmand( St_RenderCommand* prc );

    // ������ ���ؽ�/�ȼ� ���̴� ����
    virtual int New_CheckVertexShaderInRealizing( St_RenderCommand* prc );
    virtual int New_CheckPixelShaderInRealizing( St_RenderCommand* prc );

    // ���ؽ� ���̴� ����
    void AdjustVertexShaderForSkinning( const St_RenderCommand* prc, 
                                        ENUM_VERTEX_SHADER_TYPE* eVstTry );

    // ������ ��Ÿ�� ����/����
    void New_RealizeRenderStyle( St_RenderCommand* prc, int iVS = 0, int iPS = 0 );
    void New_FinishRealizeRenderStyle( St_RenderCommand* prc );

    // �׸��� ������
    BOOL RealizeShadowCastCommmand( St_RenderCommand* prc ); 
    // �׸��� �ޱ� (self shadow)
    BOOL RealizeShadowReceiveCommmand( St_RenderCommand* prc );
    // �׸��� ������ (xy ���)
    BOOL RealizeShadowCastToProjectedTexture( St_RenderCommand* prc );

    // ���� ����Ʈ ����
    void DestroyUnitEffect( CWzUnitDraw* pUnitDraw );
};

#endif // _3DRENDERER_DRAWUNITRENDERER_H_