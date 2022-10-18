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
    // 생성자/소멸자
    CManagerD3DLight();
    virtual ~CManagerD3DLight();

    // 생성/파괴
    virtual BOOL Create( CDrawBase* pParentDraw, void* pParam );
    virtual void Destroy();

    // irradiance 얻기
    virtual HANDLE GetIrradiance( ENUM_LIGHT_IRRA_TYPE iLit, ENUM_GROUP_MATERIAL_TYPE iGmt ) const;
    virtual HANDLE GetActiveIrradiance( int iActive, ENUM_GROUP_MATERIAL_TYPE iGmt );

    // irradiance 재로드
    virtual BOOL ReloadIrradiance();

protected:
    // Wzd 라이트 정보 갱신
    virtual void UpdateWzdLightInfo();

    // irradiance 로드
    BOOL LoadIrradiance( ENUM_LIGHT_IRRA_TYPE iLit, ENUM_GROUP_MATERIAL_TYPE iGmt );
    BOOL LoadIrradiance();

    // irradiance 파괴
    void DestroyIrradiance();

protected:
    static const char m_szIrradiance[NUM_ELIT][NUM_EGMT][48];

    HANDLE m_hIrradiance[NUM_ELIT][NUM_EGMT];
};

#endif // _3DRENDERER_MANAGERD3DLIGHT_H_