// eelementdecal.h

#ifndef _EFFECT_EELEMENTDECAL_H_
#define _EFFECT_EELEMENTDECAL_H_

#include "eelementbase.h"
#include <client/3dengine/common/wzsimplemesh.h>

//------------------------------------------------------------------------------
/**
    @class CEElementDecal
*/
class CEElementDecal : public CEElementBase
{
public:
    // ������/�Ҹ���
    CEElementDecal();
    virtual ~CEElementDecal();

    // �ʱ�ȭ
    virtual void Clear();

    // ���� ���� ����
    virtual void SetElementInfo( St_ElementCreate* pElement );

    // ����/�ı�
    virtual bool Create( bool bFirst );
    virtual void Destroy();
    // �޸� �ݳ�
    virtual void Free();

    // Move (�ùķ��̼�)
    virtual _ENUM_RESULT_EFFECT_MOVE Move( DWORD dwTick, bool& bSimulate );
    // ������
    virtual void Render();

    // ���� �� ����
    virtual void SetResultMatrix( const WzVector& wvPos, const WzVector& wvRot, const WzVector& wvScale );
    virtual void SetResultMatrix( const WzMatrix* pwmPos, const WzMatrix* pwmRot = NULL, const WzMatrix* pwmScale = NULL );

    // Ÿ�� ���
    virtual ENUM_EELEMENT_TYPE GetType() const;

    virtual void GetTextureHandle( std::vector<HANDLE>& HandleArray );

    virtual void ReloadTexture();

protected:
    // �¾ �� ó��
    virtual void OnBeingBorn();

    // ���� ���
    void CalculateAll( DWORD dwTime );

    // �޽� ����
    bool GenerateMesh();
    // �޽� ����
    void DestroyMesh();

protected:
    St_ElementCreate_Decal* m_pDecal;

    WzSimpleMesh* m_pMesh;

    float m_fScale;
    float m_fPrevScale;

    float m_fRotZ;
    float m_fPrevRotZ;

    WzVector m_wvRealPos;
    WzVector m_wvPrevRealPos;

    WzVector m_wvLocalPos;
    WzVector m_wvPrevParentPos;

    float m_fScaleW;
    WzMatrix m_wmRotW;
    WzVector m_wvPosW;

    bool m_bSetResultMat;
};

//------------------------------------------------------------------------------
/**
*/
inline ENUM_EELEMENT_TYPE CEElementDecal::GetType() const
{
    return EET_DECAL;
}

#endif // _EFFECT_EELEMENTDECAL_H_
