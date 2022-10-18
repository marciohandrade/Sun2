//------------------------------------------------------------------------------
//  PartyDialog.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "SceneBase.h"
#include "BattleScene.h"
#include "InterfaceManager.h"
#include ".\PartyDialog.h"
#include <Protocol_CG.h>
#include <PacketStruct_CG.h>
#include "Party.h"
#include "Hero.h"
#include "ObjectManager.h"

WzID PartyDialog::m_wzId[DIALOG_MAX] = 
{ 
    StrToWzID("P000"),
    StrToWzID("B000"),
    StrToWzID("B999"),
    StrToWzID("T000"),
    StrToWzID("T001"),
};

const float c_fGAUGE_SIZE_MARGIN = 110;

//------------------------------------------------------------------------------
/**
*/
PartyDialog::PartyDialog() :
    m_fGaugeSize(-1)
{
    this->Clear();
}

//------------------------------------------------------------------------------
/**
*/
PartyDialog::~PartyDialog()
{
}

//------------------------------------------------------------------------------
/**
*/
void
PartyDialog::Init( CDrawBase * pDrawBase )
{
    GetDialogWZ()->MoveWindowWZ( -1.0f, 120.0f + (60*this->m_iIdx));

    if (this->m_fGaugeSize < 0)
        this->m_fGaugeSize = getControl(this->DIALOG_GAUGE)->GetSize().right + c_fGAUGE_SIZE_MARGIN;

    this->SetObjectInfo();
}

//------------------------------------------------------------------------------
/**
*/
void
PartyDialog::showWindow( BOOL val )
{
    if (val)
    {
        GetDialogWZ()->MoveWindowWZ( -1.0f, 120.0f + (60*this->m_iIdx));

        if (this->m_fGaugeSize < 0)
            this->m_fGaugeSize = getControl(this->DIALOG_GAUGE)->GetSize().right + c_fGAUGE_SIZE_MARGIN;

        this->SetObjectInfo();
    }
    else
    {
        _asm nop;
    }
}

//------------------------------------------------------------------------------
/**
*/
void
PartyDialog::SetObjectInfo()
{
    if (this->m_iIdx == 1)
    {
        printf("1\n");
    }

    Object * pObject = g_ObjectManager.GetObject(this->m_dwObjKey);

    if (pObject)
    {
        if (pObject->IsKindOfObject(PLAYER_OBJECT))
        {
            Player * pPlayer = static_cast<Player *>(pObject);

            DWORD maxhp = pPlayer->GetCharInfo()->m_dwMaxHP;
            DWORD curhp = pPlayer->GetHP();

            ASSERT(maxhp > 0);
            if (curhp > maxhp)
                curhp = maxhp;

            SetHP( ( (float)curhp / (float)maxhp ) * 100.0f);
        }
        else if (pObject->IsKindOfObject(MONSTER_OBJECT))
        {
            //// 아직 정의된것이없다.
            //SolarDialog::ShowWindow(FALSE);
        }
        else
        {
            //// 아직 정의된것이없다.
            //SolarDialog::ShowWindow(FALSE);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
PartyDialog::SetHP( float fHPPercent )
{
    ASSERT( fHPPercent <= 100 );

    POSTYPE pos;
    CCtrlStaticWZ * pControl;
    pos = getCtrlIDToPos( m_wzId[DIALOG_GAUGE] );
    pControl = (CCtrlStaticWZ*)getControl(pos);

    RECT rect;
    rect = pControl->GetSizeRect();
    float width;
    float height = 0;

    RECT rcGauge = pControl->GetSize();
    height = (float)rcGauge.bottom;
    rcGauge = pControl->GetSizeRect();

    if (fHPPercent >= 0.0f )
    {
        width = (m_fGaugeSize/100.f) * fHPPercent;
        pControl->MoveWindowWZ( (float)rcGauge.left, (float)rcGauge.top, width, height );
    }
}

//------------------------------------------------------------------------------
/**
*/
void
PartyDialog::MessageProc( SI_MESSAGE * pMessage )
{
    switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_LBUTTONCLICK:
        {
            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
            case DIALOG_B999:
                {
                    g_InterfaceManager.ShowDialog( (InterfaceManager::eDIALOG_ID)(g_Party.GetStartPartyDialog() + this->m_iIdx), FALSE );
                }
                break;

            case DIALOG_B000:
                {
                    this->SubMenuTransaction();
                }
                break;
            }
        }
        break;
    }
}

//------------------------------------------------------------------------------
/**
*/
void
PartyDialog::SubMenuTransaction()
{
    int i;

    for (i=0; i<MAX_PARTYMEMBER_NUM; ++i)
    {
        if (i == this->m_iIdx)
            continue;

        // 일단 다 내리고
        g_InterfaceManager.ShowDialog( (InterfaceManager::eDIALOG_ID)(InterfaceManager::DIALOG_PARTYOPTION0 + i), FALSE );
    }

    // 켜 있으면 내리고
    // 꺼져 있으면 올리고
    BOOL bShow = g_InterfaceManager.IsShowDialog((InterfaceManager::eDIALOG_ID)(InterfaceManager::DIALOG_PARTYOPTION0 + this->m_iIdx));
    g_InterfaceManager.ShowDialog( (InterfaceManager::eDIALOG_ID)(InterfaceManager::DIALOG_PARTYOPTION0 + this->m_iIdx), !bShow );
}

//------------------------------------------------------------------------------
/**
*/
void
PartyDialog::update()
{
    POSTYPE posLevel;
    POSTYPE posName;
    CCtrlStaticWZ * pControlLevel;
    CCtrlStaticWZ * pControlName;

    posLevel = getCtrlIDToPos( m_wzId[DIALOG_T000] );
    pControlLevel = (CCtrlStaticWZ*)getControl(posLevel);
    posName = getCtrlIDToPos( m_wzId[DIALOG_T001] );
    pControlName = (CCtrlStaticWZ*)getControl(posName);

    PARTY_MEMBER_INFO* pminfo = g_Party.GetInfo(this->m_dwObjKey);

	TCHAR	szText[INTERFACE_STRING_LENGTH], szMessage[INTERFACE_STRING_LENGTH];

    if (pminfo)
    {
        _stprintf( szMessage, _T("%d"), pminfo->m_wLevel );
        pControlLevel->SetCaption( szMessage ); 

        if (this->m_dwObjKey == g_Party.GetPartyMasterObjKey())
		{
			//	방장
			g_InterfaceManager.GetInterfaceString( eST_ROOM_MASTER_TEXT, szMessage, INTERFACE_STRING_LENGTH );
            _stprintf( szMessage, _T("[%s] %s"), szMessage, pminfo->m_szCharName );
		}
        else
		{
            _stprintf( szMessage, _T("%s"), pminfo->m_szCharName );
		}

        pControlName->SetCaption( szMessage ); 
    }
    else
    {
        _stprintf( szMessage, _T("??") );
        pControlLevel->SetCaption( szMessage ); 

        if (this->m_dwObjKey == g_Party.GetPartyMasterObjKey())
		{
			//	파티장
			g_InterfaceManager.GetInterfaceString( eST_PARTY_MASTER_TEXT, szText, INTERFACE_STRING_LENGTH );
            _stprintf( szMessage, _T("[%s] ????"), szText );
		}
        else
		{
            _stprintf( szMessage, _T("????") );
		}

        pControlName->SetCaption(szMessage); 
    }

    SetObjectInfo();
}

//------------------------------------------------------------------------------
/**
*/
POSTYPE
PartyDialog::getCtrlIDToPos( WzID wzId )
{
    for( POSTYPE i = 0 ; i < DIALOG_MAX ; ++i )
    {
        if( m_wzId[i] == wzId )
        {
            return i;
        }
    }
    return DIALOG_MAX;
}

//------------------------------------------------------------------------------
/**
*/
CControlWZ *
PartyDialog::getControl( POSTYPE AtPos )
{
    ASSERT( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}
