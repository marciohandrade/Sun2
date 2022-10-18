// eelementweather.h

#ifndef _EFFECT_EELEMENTWEATHER_H_
#define _EFFECT_EELEMENTWEATHER_H_

#include "eelementsprite.h"
#include <list>

// ť�� ����ü
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
    // ������
    CDivideCube()
    {
        // empty
    }

    // �Ҹ���
    ~CDivideCube()
    {
        // empty
    }

    // �ʱ�ȭ
    void Initialize( St_Cube* pCB )
    {
        WzAssert( pCB );

        memcpy( &m_sCB, pCB, sizeof( St_Cube ) );
    }

    // ť�� ���
    void GetCube( DWORD dwID, St_Cube* pOutCB )
    {
        WzAssert( pOutCB );

        static St_Cube cb;

        FindCube( (dwID & 0xff), &cb, &m_sCB );
        FindCube( ((dwID >> 8) & 0xff), pOutCB, &cb );
    }

    // �ε����� ť�� ã��
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
    // ������/�Ҹ���
    CEElementWeather();
    virtual ~CEElementWeather();

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

    // Ÿ�� ���
    virtual ENUM_EELEMENT_TYPE GetType() const;

protected:
    // �ڽĵ� ����
    void CreateElement();
    // ��� �ڽĵ� ����
    void DestroyAllElements();

    // �ڽĵ� Move
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