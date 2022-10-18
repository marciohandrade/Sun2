// eelementsprite.h

#ifndef _EFFECT_EELEMENTSPRITE_H_
#define _EFFECT_EELEMENTSPRITE_H_

#include "eelementparticle.h"
#include <client/3dengine/common/wzuvvert.h>
#include <client/3dengine/3drenderer/managertexture.h>

struct UVInterpolation
{
    bool bDrawOverlap;      // OverLap 그리기 상태
    WzUVVert UVVert[4];     // uv좌표
    float fOpacity;         // 투명도
    WORD wInterpolateRate;  // 보간 영역 비율
};
//------------------------------------------------------------------------------
/**
    @class CEElementSprite
*/
class CEElementSprite : public CEElementParticle
{
public:
    // 생성자/소멸자
    CEElementSprite();
    virtual ~CEElementSprite();

    // 초기화
    virtual void Clear();

    // 생성 정보 설정
    virtual void SetElementInfo( St_ElementCreate* pElement );

    // 생성
    virtual bool Create( bool bFirst );
    // 메모리 반납
    virtual void Free();

    // Move (시뮬레이션)
    virtual _ENUM_RESULT_EFFECT_MOVE Move( DWORD dwTick, bool& bSimulate );
    // 렌더링
    virtual void Render();

    // 타입 얻기
    virtual ENUM_EELEMENT_TYPE GetType() const;

    // 텍스쳐를 다시로드한다
    virtual void ReloadTexture();

    virtual void GetTextureHandle( std::vector<HANDLE>& HandleArray ) { HandleArray.push_back(m_hTexture); }


protected:
    // 스프라이트 이동
    void MoveSprite(bool is_first_moved = false);

    // UV 애니메이션
    void UVAnimation( DWORD lnBornCnt );
    // UV 애니메이션 보간
    void UVAniInterpolation( DWORD lnBornCnt );

    // 칼라 설정
    void SetSpriteColor( bool bBackImage );

protected:
    St_ElementCreate_Sprite* m_pSprite;

    HANDLE m_hTexture;          // 텍스처 핸들

    WzVector m_wvVertex[4];     // 정점들
    WzUVVert m_UVVert[4];

    float m_fScale;             // 크기

    long m_lnGaroCnt;           // 텍스쳐 U크기의 가로 갯수
    float m_fGaroRate;          // 가로 비율
    float m_fSeroRate;          // 세로 비율

    long m_lnStartAniNo;        // 시작 애니 번호
    long m_lnCurAniNo;          // 현재 애니 번호
    long m_lnPrevAniNo;         // 이전 애니 번호

    UVInterpolation m_stInterpolate;
};

//------------------------------------------------------------------------------
/**
*/
inline ENUM_EELEMENT_TYPE CEElementSprite::GetType() const
{ 
    return EET_SPRITE;
}

#endif // _EFFECT_EELEMENTSPRITE_H_
