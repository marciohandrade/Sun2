#include "SunClientPrecompiledHeader.h"
#include "CreateCharRotateButton.h"
#include "characterscene.h"
#include "Object.h"
#include "Objectmanager.h"
#include "Camera.h"
#include "Mouse.h"

#define ZOOM_SPEED	10

WzID CreateCharRotateButton::m_wzId[MAX_BUTTON] = 
{
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    StrToWzID("BT04"),
    StrToWzID("BT05"),
	StrToWzID("BT00"),
	StrToWzID("B002"),
#else
	StrToWzID("BT01"),
	StrToWzID("BT02"),
	StrToWzID("BT00"),
    StrToWzID("B002"),
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
};

CreateCharRotateButton::CreateCharRotateButton(InterfaceManager * pUIman)
	:uiBase(pUIman)
{
}

CreateCharRotateButton::~CreateCharRotateButton()
{
}

VOID CreateCharRotateButton::Init( CDrawBase * pDrawBase )
{
	uiBase::Init(pDrawBase);

    m_RotateType = NONE_BUTTON;
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
#else
	pZoomInBtn	 = getControl( ZOOM_IN_BUTTON );
	pZoomOutBtn  = getControl( ZOOM_OUT_BUTTON );
#endif//#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
}

#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
#else
void CreateCharRotateButton::SetZoomButton( BOOL bShow )
{
	if (bShow)
	{
		pZoomInBtn->ShowWindowWZ(WZ_SHOW);
		pZoomOutBtn->ShowWindowWZ(WZ_SHOW);
	}
	else
	{
		pZoomInBtn->ShowWindowWZ(WZ_HIDE);
		pZoomOutBtn->ShowWindowWZ(WZ_HIDE);
	}
}
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

VOID CreateCharRotateButton::Release()
{

}

VOID CreateCharRotateButton::MessageProc(SI_MESSAGE * pMessage)
{
	switch (pMessage->eResultMsg) 
	{
	case RT_MSG_LBUTTONDOWN:
		{
            if(m_RotateType == NONE_BUTTON)
            {
			    switch (getCtrlIDToPos(pMessage->dwCtrlID))
                {
			    case LEFT_BUTTON:
				    m_RotateType = LEFT_BUTTON;
				    break;

			    case RIGHT_BUTTON:
				    m_RotateType = RIGHT_BUTTON;
				    break;
			    case ZOOM_IN_BUTTON:
				    m_RotateType = ZOOM_IN_BUTTON;
				    break;

			    case ZOOM_OUT_BUTTON:
				    m_RotateType = ZOOM_OUT_BUTTON;
				    break;
			    }
            }
		}
        break;
	case RT_MSG_LBUTTONUP:
		{
			m_RotateType = NONE_BUTTON;
		}
		break;

	case RT_MSG_LBUTTONCLICK:
		{
			m_RotateType = NONE_BUTTON;
		}
		break;

	default:
		{
			SolarDialog::MessageProc(pMessage);
		}
		break;
	
	}

}

VOID CreateCharRotateButton::OnUpdateSolarDialog()
{
    
    if (m_RotateType == NONE_BUTTON)
    {
        return;
    }

    bool isOut = true;
    int mosueX = Mouse::Instance()->GetMouseX();
    int mosuey = Mouse::Instance()->GetMouseY();

    CControlWZ* controlRect = getControl(m_RotateType);
    if(controlRect)
    {
        RECT consize = controlRect->GetSizeRect();

        if(consize.left < mosueX && consize.right > mosueX &&
            consize.top < mosuey && consize.bottom > mosuey)
        {
           isOut = false;
        }
    }
	
    if(isOut)
    {
        return;
    }

    float temp = 0.0f;
    if (m_RotateType == LEFT_BUTTON)
    {
        temp = 0.03f;			
    }
    else if (m_RotateType == RIGHT_BUTTON)
    {
        temp = -0.03f;			
    }
    g_CharSelectScene.RotateCinematicCharacter(temp);

	int dis = 0;

#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
	if (m_RotateType == ZOOM_IN_BUTTON)
	{
		g_CharSelectScene.ZoomCamera(0.05f);
	}
	else if (m_RotateType == ZOOM_OUT_BUTTON)
	{
		g_CharSelectScene.ZoomCamera(-0.05f);
	}
#else
	if (m_RotateType == ZOOM_IN_BUTTON)
	{
		dis = ZOOM_SPEED;
	}
	else if (m_RotateType == ZOOM_OUT_BUTTON)
	{
		dis = -ZOOM_SPEED;
	}

	g_Camera.MoveFrontBack(dis);
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
	g_Camera.UpdateCamera();

}

CControlWZ * CreateCharRotateButton::getControl( POSTYPE AtPos )
{
	assert( AtPos < MAX_BUTTON );

	return GetControlWZ( m_wzId[AtPos] );
}

POSTYPE	CreateCharRotateButton::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < MAX_BUTTON; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}
	return MAX_BUTTON;

}