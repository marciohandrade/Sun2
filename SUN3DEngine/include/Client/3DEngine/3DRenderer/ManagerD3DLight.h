// managerd3dlight.h

#ifndef _3DRENDERER_MANAGERD3DLIGHT_H_
#define _3DRENDERER_MANAGERD3DLIGHT_H_

#include "managerlight.h"

//------------------------------------------------------------------------------
/**
    @class CManagerD3DLight
*/
class CManagerD3DLight : public CManagerLight
{
public:
    // ������/�Ҹ���
    CManagerD3DLight();
    virtual ~CManagerD3DLight();

    // ����/�ı�
    virtual BOOL Create( CDrawBase* pParentDraw, void* pParam );
    virtual void Destroy();

    // irradiance ���
    virtual HANDLE GetIrradiance( ENUM_LIGHT_IRRA_TYPE iLit, ENUM_GROUP_MATERIAL_TYPE iGmt ) const;
    virtual HANDLE GetActiveIrradiance( int iActive, ENUM_GROUP_MATERIAL_TYPE iGmt );

    // irradiance ��ε�
    virtual BOOL ReloadIrradiance();

protected:
    // Wzd ����Ʈ ���� ����
    virtual void UpdateWzdLightInfo();

    // irradiance �ε�
    BOOL LoadIrradiance( ENUM_LIGHT_IRRA_TYPE iLit, ENUM_GROUP_MATERIAL_TYPE iGmt );
    BOOL LoadIrradiance();

    // irradiance �ı�
    void DestroyIrradiance();

protected:
    static const char m_szIrradiance[NUM_ELIT][NUM_EGMT][48];

    HANDLE m_hIrradiance[NUM_ELIT][NUM_EGMT];
};

#endif // _3DRENDERER_MANAGERD3DLIGHT_H_