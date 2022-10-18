#pragma once

#include "typedef.h"

class CDrawCtrl;


// Gradation ���� ����ü
struct SI_Gradation
{
    COLORREF m_coColor[4];  // 4���� ���� �迭

    void initialize()
    {   
        m_coColor[0] = RGBA( 0, 0, 0, 255 );
        m_coColor[1] = RGBA( 0, 0, 0, 255 );
        m_coColor[2] = RGBA( 0, 0, 0, 255 );
        m_coColor[3] = RGBA( 0, 0, 0, 255 ); 
    }
};

// Sub �׸� ����ü �����̴�. ���� �׸� ������ ������ �ִ´�.
// �׸� ������ �κ� ���� ����ü. �ϳ��� �׸� ������ ������ �ִ�.
struct SI_TSUBINFO
{   
    enum eDrawType
    { 
        DrawType_Whole = 0,     // 0. ������ - �̹��� ��ü �׸��� ���
        DrawType_3Divide,       // 1. 3��� - �� �� �� �Ӥ� �� �̹����� 3����ؼ� ����� ũ�⿡ �°� �÷��༭ �׸���.
        DrawType_Tiling,        // 2. Ÿ�Ϸ� - ũ�⿡ �°� Ÿ�� ������� �׸���.
        DrawType_TileExpand,    // 3. Ÿ�ϸ� ����(9��� �ؼ� �ø�)
    };

    bool m_blIs;                // ������ ���� �Ǿ� �ִ�. ����.
    std::string  m_sImageName;   // �̹��� ���� �̸�.
    SI_FRECT m_sFRect;          // �ҽ��� ��� ����. 
    short m_nType;              // �ҽ��� ��Ÿ���� ������ �ִ�. (eDrawType)
    void* m_pVoid;              // Texture ID or Handle ����.

    COLORREF m_color;           // ��� Į��. 

    bool m_blUseGradation;      // �׶��̼��� ��� ����.
    SI_Gradation m_sGradation;  // �׶��̼� �� ������.

    //"ver.r2.1"
    float m_fTileCornerX;   //!< Ÿ�ϸ� �϶��� ������ �޴´�.       
    float m_fTileCornerY;   //!< Ÿ�ϸ� �϶��� ������ �޴´�.       

    SI_TSUBINFO()
    {
        Initialize();
    }

    // ���� �ʱ�ȭ
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

// �׸� ���� ����ü. ���� �������� �׸� ���� ����ü�� ������ �ִ�.
struct SI_TFORMAT                                   
{
    // �̹����� ������.  ex) ����, ���콺�� ���� ������, Ŭ�� ������
    SI_TSUBINFO m_sImageState[E_NUM];
};