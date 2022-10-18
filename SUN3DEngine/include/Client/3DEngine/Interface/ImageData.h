#pragma once

#include "typedef.h"

class CDrawCtrl;


// Gradation 정보 구조체
struct SI_Gradation
{
    COLORREF m_coColor[4];  // 4개의 색상 배열

    void initialize()
    {   
        m_coColor[0] = RGBA( 0, 0, 0, 255 );
        m_coColor[1] = RGBA( 0, 0, 0, 255 );
        m_coColor[2] = RGBA( 0, 0, 0, 255 );
        m_coColor[3] = RGBA( 0, 0, 0, 255 ); 
    }
};

// Sub 그림 구조체 포멧이다. 실제 그림 정보를 가지고 있는다.
// 그림 정보의 부분 정보 구조체. 하나의 그림 정보를 가지고 있다.
struct SI_TSUBINFO
{   
    enum eDrawType
    { 
        DrawType_Whole = 0,     // 0. 통으로 - 이미지 전체 그리는 방식
        DrawType_3Divide,       // 1. 3등분 - ㅣ ㅣ ㅣ ㅣㅣ ㅣ 이미지를 3등분해서 가운데만 크기에 맞게 늘려줘서 그린다.
        DrawType_Tiling,        // 2. 타일로 - 크기에 맞게 타일 방식으로 그린다.
        DrawType_TileExpand,    // 3. 타일링 변형(9등분 해서 늘림)
    };

    bool m_blIs;                // 정보가 설정 되어 있다. 없다.
    std::string  m_sImageName;   // 이미지 파일 이름.
    SI_FRECT m_sFRect;          // 소스의 사용 영역. 
    short m_nType;              // 소스의 스타일을 가지고 있다. (eDrawType)
    void* m_pVoid;              // Texture ID or Handle 저장.

    COLORREF m_color;           // 배경 칼라값. 

    bool m_blUseGradation;      // 그라데이션의 사용 여부.
    SI_Gradation m_sGradation;  // 그라데이션 색 정보들.

    //"ver.r2.1"
    float m_fTileCornerX;   //!< 타일링 일때만 영향을 받는다.       
    float m_fTileCornerY;   //!< 타일링 일때만 영향을 받는다.       

    SI_TSUBINFO()
    {
        Initialize();
    }

    // 정보 초기화
    void Initialize()
    {
        m_color = 0xFFFFFFFF;       
        m_sImageName.clear();
        m_nType = DrawType_Whole;   // DrawType_Whole
        m_pVoid = NULL;
        m_blIs = false;

        m_blUseGradation = false;
        m_sGradation.initialize();

        m_fTileCornerX = 0.0f;
        m_fTileCornerY = 0.0f;
    }

    bool IsActive() { return m_blIs == true; }

    const VOID* GetTexture() const { return m_pVoid; }

    SI_TSUBINFO& operator = ( const SI_TSUBINFO& input )
    {
        m_blIs          = input.m_blIs;
        m_sFRect        = input.m_sFRect;
        m_nType         = input.m_nType;
        m_sImageName    = input.m_sImageName;
        m_color         = input.m_color;

        m_blUseGradation    = input.m_blUseGradation;
        m_fTileCornerX		= input.m_fTileCornerX;
        m_fTileCornerY		= input.m_fTileCornerY;

        m_sGradation = input.m_sGradation;
        //memcpy(&m_sGradation, &input.m_sGradation,sizeof(SI_Gradation));

        return *this;
    }

};

// 그림 정보 구조체. 각각 종류별로 그림 서브 구조체를 가지고 있다.
struct SI_TFORMAT                                   
{
    // 이미지의 종류별.  ex) 평상시, 마우스가 위에 있을때, 클릭 했을때
    SI_TSUBINFO m_sImageState[E_NUM];
};