//------------------------------------------------------------------------------
//  PartyMemberOptionDialog.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "SceneBase.h"
#include "BattleScene.h"
#include "InterfaceManager.h"
#include ".\PartyMemberOptionDialog.h"
#include <Protocol_CG.h>
#include <PacketStruct_CG.h>
#include "Party.h"
#include "Hero.h"

WzID PartyMemberOptionDialog::m_wzId[DIALOG_MAX] = 
{ 
    StrToWzID("B999"),
    StrToWzID("B001"),
    StrToWzID("B002"),
    StrToWzID("B003"),
    StrToWzID("B004"),
    StrToWzID("B005"),
    StrToWzID("B006"),
    StrToWzID("B007"),
};

//------------------------------------------------------------------------------
/**
*/
PartyMemberOptionDialog::PartyMemberOptionDialog()
{
    this->Clear();
}

//------------------------------------------------------------------------------
/**
*/
PartyMemberOptionDialog::~PartyMemberOptionDialog()
{
}

//------------------------------------------------------------------------------
/**
*/
VOID
PartyMemberOptionDialog::Init( CDrawBase * pDrawBase )
{
}

//------------------------------------------------------------------------------
/**
*/
void
PartyMemberOptionDialog::showWindow( BOOL val )
{
    if (val)
    {
        SolarDialog* dlg = g_InterfaceManager.GetDialog((InterfaceManager::eDIALOG_ID)(g_Party.GetStartPartyDialog() + this->m_iIdx));
        RECT rc;
        dlg->GetDialogWZ()->GetClientRect(&rc);
        GetDialogWZ()->MoveWindowWZ( rc.right, rc.top );
    }
    else
    {
        _asm nop;
    }
}

//------------------------------------------------------------------------------
/**
*/
VOID PartyMemberOptionDialog::MessageProc( SI_MESSAGE * pMessage )
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH];

    switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_LBUTTONCLICK:
        {
            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
            case DIALOG_EXIT:
                {
                    g_InterfaceManager.ShowDialog( (InterfaceManager::eDIALOG_ID)(InterfaceManager::DIALOG_PARTYOPTION0 + this->m_iIdx), FALSE );
                }
                break;

                // ��ü
            case DIALOG_PARTY_DESTROY:
                {
                    if (g_Party.actDestroyParty())
                        g_InterfaceManager.ShowDialog( (InterfaceManager::eDIALOG_ID)(InterfaceManager::DIALOG_PARTYOPTION0 + this->m_iIdx), FALSE );
                }
                break;

                // �ʴ�
            case DIALOG_INVITE:
                {
                    //g_Party.Invite(NULL);
					//	ä��â�� '/�ʴ� ���̵�' ��ɾ ����ϼ���.
					g_InterfaceManager.GetInterfaceString( eST_INVITE_COMMAND_HELP, szMessage, INTERFACE_STRING_LENGTH );
                    g_Party.Notice( szMessage );
                }
                break;

                // Ż��
            case DIALOG_LEAVE:
                {
                    if (g_pHero && g_pHero->GetObjectKey() == this->m_dwObjKey)
                    {
                        if (g_Party.actLeave())
                            g_InterfaceManager.ShowDialog( (InterfaceManager::eDIALOG_ID)(InterfaceManager::DIALOG_PARTYOPTION0 + this->m_iIdx), FALSE );
                    }
                    else
					{
						//	Ż��� ���ο��Ը� �����մϴ�. ���� Ż�� �ϼ���. -0-
						g_InterfaceManager.GetInterfaceString( eST_PARTY_LEAVE_HELP, szMessage, INTERFACE_STRING_LENGTH );
						g_Party.Notice( szMessage );
					}
                }
                break;

                // ���� Ż��
            case DIALOG_FORCE_LEAVE:
                {
                    if (g_pHero && g_pHero->GetObjectKey() != this->m_dwObjKey)
                    {
                        if (g_Party.actForcedExplusion(this->m_dwObjKey))
                            g_InterfaceManager.ShowDialog( (InterfaceManager::eDIALOG_ID)(InterfaceManager::DIALOG_PARTYOPTION0 + this->m_iIdx), FALSE );
                    }
                    else
					{
						//	���� Ż��� Ÿ�ο��Ը� �����մϴ�. Ż�� �ϼ���. -0-
						g_InterfaceManager.GetInterfaceString( eST_PARTY_FORCED_EXPULSION_HELP, szMessage, INTERFACE_STRING_LENGTH );
                        g_Party.Notice( szMessage );
					}
                }
                break;

                // ���� ����
            case DIALOG_MIGRATION_LEADER:
                {
                    if (g_Party.actMigration(this->m_dwObjKey))
                        g_InterfaceManager.ShowDialog( (InterfaceManager::eDIALOG_ID)(InterfaceManager::DIALOG_PARTYOPTION0 + this->m_iIdx), FALSE );
                }
                break;

                // ���� ���� ON/OFF
            case DIALOG_BUFF_INFO:
                {
                    g_Party.actToggleShowBuffInfo();
                }
                break;

                // �й� ��� ����
            case DIALOG_ITEM_SHARE_SETTING:
                {
                    // �λ���� ����: �й� ��� ��ɱ��� ������
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
POSTYPE PartyMemberOptionDialog::getCtrlIDToPos( WzID wzId )
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
CControlWZ * PartyMemberOptionDialog::getControl( POSTYPE AtPos )
{
    ASSERT( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}
