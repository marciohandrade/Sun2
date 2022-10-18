// eelementweather.h

#ifndef _EFFECT_EELEMENTWEATHER_H_
#define _EFFECT_EELEMENTWEATHER_H_

#include "eelementsprite.h"
#include <list>

// 큐브 구조체
struct St_Cube
{
    WzVector m_wvCenter;
    WzVector m_wvUP;
    WzVector m_wvRight;
    WzVector m_wvDir;
    float m_fUP;
    float m_fRight;
    float m_fDir;
};

//------------------------------------------------------------------------------
/**
    @class CDivideCube
*/
class CDivideCube
{
public:
    // 생성자
    CDivideCube()
    {
        // empty
    }

    // 소멸자
    ~CDivideCube()
    {
        // empty
    }

    // 초기화
    void Initialize( St_Cube* pCB )
    {
        WzAssert( pCB );

        memcpy( &m_sCB, pCB, sizeof( St_Cube ) );
    }

    // 큐브 얻기
    void GetCube( DWORD dwID, St_Cube* pOutCB )
    {
        WzAssert( pOutCB );

        static St_Cube cb;

        FindCube( (dwID & 0xff), &cb, &m_sCB );
        FindCube( ((dwID >> 8) & 0xff), pOutCB, &cb );
    }

    // 인덱스로 큐브 찾기
    void FindCube( int nIndex, St_Cube* pOutCB, const St_Cube* pInCB );

private:
    St_Cube m_sCB;
};

//------------------------------------------------------------------------------
/**
    @class CEElementWeather
*/
class CEElementWeather : public CEElementBase
{
public:
    // 생성자/소멸자
    CEElementWeather();
    virtual ~CEElementWeather();

    // 초기화
    virtual void Clear();

    // 생성 정보 설정
    virtual void SetElementInfo( St_ElementCreate* pElement );

    // 생성/파괴
    virtual bool Create( bool bFirst );
    virtual void Destroy();
    // 메모리 반납
    virtual void Free();

    // Move (시뮬레이션)
    virtual _ENUM_RESULT_EFFECT_MOVE Move( DWORD dwTick, bool& bSimulate );
    // 렌더링
    virtual void Render();

    // 타입 얻기
    virtual ENUM_EELEMENT_TYPE GetType() const;

protected:
    // 자식들 생성
    void CreateElement();
    // 모든 자식들 제거
    void DestroyAllElements();

    // 자식들 Move
    void MoveElement( DWORD dwTick, bool& bSimulate );

protected:
    St_ElementCreate m_ecElem;
    St_Visual_Info m_viElem;

    CDivideCube m_cDivideCube;

    std::list<CEElementSprite*> m_lstSprite;
    int m_maxNumElem;
};

//------------------------------------------------------------------------------
/**
*/
inline ENUM_EELEMENT_TYPE CEElementWeather::GetType() const
{ 
    return EET_WEATHER;
}

#endif // _EFFECT_EELEMENTWEATHER_H_