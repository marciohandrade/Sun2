//------------------------------------------------------------------------------
//  uiTradeMan_main.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiTradeMan.h"
#include "uitargetman/uitargetman.h"
#include "TargetDialog.h"
#include "ObjectManager.h"
#include "AskReplyTradeDialog.h"
#include "InputNumTradeDialog.h"
#include "MouseHandler.h"
#include "InventoryDialog.h"
#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
#else
#include "uiTrade/uiTrade.h"
#endif // _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
#include "SCItemSlot.h"
#include "hero.h"
#include "globalfunc.h"
#include "InterfaceManager.h"
#include "uiInPutNumTradeUIDialog/uiInPutNumTradeUIDialog.h"

#pragma warning ( push )
#pragma warning(disable : 4800)

//------------------------------------------------------------------------------
/**
*/
uiTradeMan::uiTradeMan(InterfaceManager *pUIMan) :
    uiBaseMan(pUIMan)
{
    m_bPutOneStaring = false;
    m_pUITrade = NULL;
}

void
uiTradeMan::OnInitialize()
{
    _LoadUI();
}


//------------------------------------------------------------------------------
/**
*/
void
uiTradeMan::Clear()
{
    this->m_bModifyPending = false;
    this->m_uiFunctionIndex = 0;

    this->m_tradeInfo1.Clear();
    this->m_tradeInfo2.Clear();
}

//------------------------------------------------------------------------------
/**
*/
bool
uiTradeMan::SetName(DWORD dwKey)
{
    Object *pObject = g_ObjectManager.GetObject(dwKey);
    assert (pObject);
    if (!pObject)
        return false;

    if (pObject->IsKindOfObject(PLAYER_OBJECT))
    {
        Player * pPlayer =
            static_cast<Player *>(pObject);
        assert (pPlayer);

        BASE_PLAYERINFO *bp = pPlayer->GetCharInfo();
        assert (bp);

        this->m_tradeInfo1.m_strName = bp->m_tszCharName;
        this->m_tradeInfo1.m_dwKey = dwKey;
        this->m_tradeInfo1.m_isSet = true;
        this->m_tradeInfo1.m_DealMoney = 0;
        this->m_tradeInfo1.m_pObject = pObject;
        this->m_tradeInfo1.m_status = -1;
        return true;

    }
    else
    {
        assert (!"Object is not kind of player");
        this->m_tradeInfo1.Clear();
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
uiTradeMan::SetNameTo(DWORD dwKey)
{
    Object *pObject = g_ObjectManager.GetObject(dwKey);
    assert (pObject);
    if (!pObject)
        return false;

    if (pObject->IsKindOfObject(PLAYER_OBJECT))
    {
        Player * pPlayer =
            static_cast<Player *>(pObject);
        assert (pPlayer);

        BASE_PLAYERINFO *bp = pPlayer->GetCharInfo();
        assert (bp);

        this->m_tradeInfo2.m_strName = bp->m_tszCharName;
        this->m_tradeInfo2.m_dwKey = dwKey;
        this->m_tradeInfo2.m_isSet = true;
        this->m_tradeInfo2.m_DealMoney = 0;
        this->m_tradeInfo2.m_pObject = pObject;
        this->m_tradeInfo2.m_status = -1;
        return true;

    }
    else
    {
        assert (!"Object is not kind of player");
        this->m_tradeInfo2.Clear();
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
uiTradeMan::Ask(const TCHAR* pname)
{
	if(!g_pHero)
		return false;

    TCHAR       szText[INTERFACE_STRING_LENGTH], szMessage[INTERFACE_STRING_LENGTH];

    if (g_pHero->GetBeHaveState() == PLAYER_BEHAVE_TRADE_STATE)
    {
        // �ŷ� �� �ϰ�� ���� �ŷ���û �� �� ����.
		//	���� �ٸ� �빫 ���̽ʴϴ�.
		g_InterfaceManager.GetInterfaceString( eST_YOU_ARE_BUSY, szMessage, INTERFACE_STRING_LENGTH );
        GlobalFunc::OutputMsg_Notice (eOUTPUT_MSG_CHAT, szMessage);
        return false;
    }

    if (pname == NULL && !(GET_DIALOG(IM_TARGET_MANAGER::TARGET_DIALOG)->IsVisible()))
    {
        //	���õ� �÷��̾ �����ϴ�.
        this->GetUIMan()->GetInterfaceString( eST_NOT_SELECTED_PLAYER, szMessage, INTERFACE_STRING_LENGTH );
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,  szMessage );
        return false;
    }

    DWORD       dwTargetKey = 0;
    Object *    pTargetObj = NULL;

    if (pname == NULL)
    {
        // Ÿ��â�� �ִ� ������ �ŷ�

        TargetDialog * pTargetDlg = GET_CAST_DIALOG(TargetDialog, IM_TARGET_MANAGER::TARGET_DIALOG);
        assert (pTargetDlg);

        dwTargetKey = pTargetDlg->GetTargetKey();

		if(g_pHero->GetObjectKey() == dwTargetKey)
		{
			// �ڽſ��Դ� �ŷ��� ��û�� �� �����ϴ�. : 5370
			g_InterfaceManager.GetInterfaceString(eST_CANNOT_TRADE_MYSELF , szMessage , INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice (eOUTPUT_MSG_CHAT, szMessage);
			this->Clear();
            return false;
		}

        // �ŷ��ϴ� ���� ����
        if (!this->SetNameTo(dwTargetKey))
        {
            this->Clear();
            return false;
        }

    	pTargetObj = g_ObjectManager.GetObject(dwTargetKey);
        assert (pTargetObj);
    }
    else
    {
		if ( !Strcmp( g_pHero->GetName(), pname ) )
		{
			// �ڽſ��Դ� �ŷ��� ��û�� �� �����ϴ�. : 5370
			g_InterfaceManager.GetInterfaceString(eST_CANNOT_TRADE_MYSELF , szMessage , INTERFACE_STRING_LENGTH);
			GlobalFunc::OutputMsg_Notice (eOUTPUT_MSG_CHAT, szMessage);
			return false;
		}

#ifdef	_K2P_OBJECT_MANAGER_MODIFIDY
		// �Է��� ���̵���� �ŷ�
		Object * pObj = g_ObjectManager.GetObject( pname );

		if( pObj != NULL )
		{
			if( this->SetNameTo( pObj->GetObjectKey() ) )
			{
				this->Clear();
				return false;
			}
		}
		else
		{
			g_InterfaceManager.GetInterfaceString( eST_NAMECODE_NOT_FOUND, szMessage, INTERFACE_STRING_LENGTH );
            GlobalFunc::OutputMsg_Notice (eOUTPUT_MSG_CHAT, szMessage, pname);
            return false;
		}
#else
        bool bFounded = false;

        Object_Map_Itr	itr = g_ObjectManager.GetBegin();

        while( g_ObjectManager.GetEnd() != itr )
        {
            Object *pObj = (Object *)itr->second;
            if (!Strcmp(pObj->GetName(), pname) &&
                !((pObj->GetObjectKey() > ONLYCLIENT_OBJECT_KEY) &&  // Ŭ�󿡼� ���� ������ ������ƮŰ ����
                (pObj->GetObjectKey() < MAX_OBJECT_KEY)))
            {
                dwTargetKey = pObj->GetObjectKey();
                pTargetObj = pObj;
                
                if (!this->SetNameTo(dwTargetKey))
                {
                    this->Clear();
                    return false;
                }

                bFounded = true;
                break;
            }
            ++itr;
        }

        if (!bFounded)
        {
            // �׷� ������ ����
			//	'%s' ���� ã�� �� �����ϴ�.
			g_InterfaceManager.GetInterfaceString( eST_NAMECODE_NOT_FOUND, szMessage, INTERFACE_STRING_LENGTH );
            GlobalFunc::OutputMsg_Notice (eOUTPUT_MSG_CHAT, szMessage, pname);
            return false;
        }
#endif
    }

    // �ŷ��ϴ� ���� ����
    this->SetName(g_pHero->GetObjectKey());

    assert (pTargetObj);

    if (!pTargetObj)
        return false;

    Player* pPlayerObj = (Player *)pTargetObj;
    assert (pPlayerObj);

    if (!this->CheckCanTrading(pPlayerObj))
    {
        this->Clear();
        return false;
    }

    //	[%s] �Կ��� �ŷ��� ��û
    g_InterfaceManager.GetInterfaceString( eST_TRADE_REQUEST_TO_NAMECODE, szText, INTERFACE_STRING_LENGTH );
    Sprintf( szMessage, szText, this->m_tradeInfo2.m_isSet ? this->m_tradeInfo2.m_strName.c_str() : _T("??") );
    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,  szMessage );

    this->m_tradeInfo1.m_status = TRADE_STATUS_IDLE;
    g_pHero->SetBeHaveState(PLAYER_BEHAVE_TRADE_STATE);


	MSG_CG_TRADE_OPEN_SYN msgSend;
	msgSend.m_dwPlayerKey = m_tradeInfo2.m_dwKey;
    return GlobalFunc::SendPacket( CI_GAMESERVERIDX, &msgSend, sizeof (msgSend) );

}

//------------------------------------------------------------------------------
/**
*/
bool
uiTradeMan::CheckCanTrading(Player* pPlayer)
{
    assert (pPlayer);

    if (!this->CheckCanTradingIsBusy(pPlayer))
        return false;

    if (!this->CheckCanTradingIsInRanged(pPlayer))
        return false;

    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
uiTradeMan::CheckCanTradingIsBusy(Player* pPlayer)
{
    TCHAR       szText[INTERFACE_STRING_LENGTH], szMessage[INTERFACE_STRING_LENGTH];

    assert (pPlayer);

    if (pPlayer->GetBeHaveState() != PLAYER_BEHAVE_IDLE_STATE)
    {
        //	[%s] ���� ���� �ٸ� �빫 ���̽ʴϴ�.
        const int	c_iBusy = 70414;
        g_InterfaceManager.GetInterfaceString( c_iBusy, szText, INTERFACE_STRING_LENGTH );
        Sprintf (szMessage, szText, pPlayer->GetName());
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
uiTradeMan::CheckCanTradingIsInRanged(Player* pPlayer)
{
    TCHAR     szMessage[INTERFACE_STRING_LENGTH];

    assert (pPlayer);

	if(!g_pHero)
		return false;

    float distance = VectorLength( &(pPlayer->GetPosition() - g_pHero->GetVisiblePos()) );
    if ( distance > DISTANCE_TRADING )
    {
        //	�ŷ��Ϸ��� ������� �Ÿ��� �ʹ� �ָ� ������ �ֽ��ϴ�.
        const int	c_iTooFarDistanceTrading = 70391;
        g_InterfaceManager.GetInterfaceString( c_iTooFarDistanceTrading, szMessage, INTERFACE_STRING_LENGTH );
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
/**
    �ŷ� ��û�� ���� ���� �ǻ�â ����
*/
bool
uiTradeMan::PopupTradeReply(DWORD dwPlayerKey)
{
	if(!g_pHero)
		return false;

    if (g_pHero->GetBeHaveState() == PLAYER_BEHAVE_TRADE_STATE)
    {
        // �� ��� �̻����� �ŷ� ��û �޴� ��

        // �̹� �ŷ����̰ų� �ٸ� �̷� �Ͽ��� �ŷ� ��û �ް� �ִ� ��
        // �̷� ��� �ڵ� �ŷ���û ������ ������.

        this->SendTradeReply(false);
        return false;
    }

    this->Clear();

    if (!this->SetName(g_pHero->GetObjectKey()))
    {
        this->Clear();
        return false;
    }

    if (!this->SetNameTo(dwPlayerKey))
    {
        this->Clear();
        return false;
    }

    TCHAR	szText[INTERFACE_STRING_LENGTH], szMessage[INTERFACE_STRING_LENGTH];

    //	[%s] ���� �ŷ��� ��û
    g_InterfaceManager.GetInterfaceString( eST_TRADE_REQUEST_FROM_NAMECODE, szText, INTERFACE_STRING_LENGTH );
    Sprintf( szMessage, szText, this->m_tradeInfo2.m_isSet ? this->m_tradeInfo2.m_strName.c_str() : _T("??") );
    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );


    //-ck- 2006. 1. 17.
    //������ ���Ѵٴ� ��Ŷ����
    this->SendTradeReply(true);

    g_pHero->SetBeHaveState(PLAYER_BEHAVE_TRADE_STATE);

    return true;
}

//------------------------------------------------------------------------------
/**
    �ŷ��� ���� ���� ��Ŷ ����
*/
void
uiTradeMan::SendTradeReply(bool bDo)
{
}

//------------------------------------------------------------------------------
/**
*/
void
uiTradeMan::TradeStart()
{
    this->m_tradeInfo1.m_status = TRADE_STATUS_IDLE;
    this->m_tradeInfo2.m_status = TRADE_STATUS_IDLE;
    this->m_pUITrade->ShowInterface(TRUE);

    if (this->m_bPutOneStaring)
    {
        this->m_pUITrade->PutOneStart();
        this->m_bPutOneStaring = false;
    }
}

void uiTradeMan::ShowNumTradeUIDialog( BOOL show )
{
#ifdef	_K2P_SOFTWARE_NUMBER_KEYBOARD_000913
	m_pUINumTradeUIDialog->ShowInterface( show );
#endif
}

//------------------------------------------------------------------------------
/**
�ŷ� ��

1. �ŷ�â�� �÷����� �������� ��ȯ�ϸ� �˻�
2. �˻��� �������� �÷��� �ִ� ������ �߰��ϸ� �κ�â���� ���� �������� ã�Ƽ� ����
*/
void
uiTradeMan::TradeEnd(MONEY money, TRADE_TOTAL_INFO tradeInfo)
{
	if(!g_pHero)
		return;

    g_pHero->SetMoney(money);

    this->m_pUITrade->TradeEnd(tradeInfo);
    this->TradeClose();

    g_pHero->SetBeHaveState(PLAYER_BEHAVE_IDLE_STATE);
}

//------------------------------------------------------------------------------
/**
    �ŷ� ��� ��Ŷ ����
*/
void
uiTradeMan::SendTradeCancel()
{
    MSG_CG_TRADE_CANCEL_SYN sendMsg;
    GlobalFunc::SendPacket( CI_GAMESERVERIDX, &sendMsg, sizeof (MSG_CG_TRADE_CANCEL_SYN) );
}

//------------------------------------------------------------------------------
/**
    �ŷ� ���
*/
void
uiTradeMan::TradeClose()
{
	m_pUITrade->ShowInterface( FALSE );
	GET_DIALOG( IM_INVENTORY_MANAGER::INVEN_DLG )->ShowWindow(FALSE);
    this->GetUIMan()->ShowWindow(InterfaceManager::DIALOG_EQUIP, FALSE);
#ifdef	_K2P_SOFTWARE_NUMBER_KEYBOARD_000913
	m_pUINumTradeUIDialog->ShowInterface( FALSE );
#endif

}

//------------------------------------------------------------------------------
/**
*/
void
uiTradeMan::PopupTradeMoney()
{
    TCHAR	szMessage[INTERFACE_STRING_LENGTH];
    if (g_pHero && (g_pHero->GetMoney() <= 0))
    {
        //	���� �����Ͻ� ������ �����ϴ�.
        const int	c_iHaveNotHyme = 70392;
        g_InterfaceManager.GetInterfaceString( c_iHaveNotHyme, szMessage, INTERFACE_STRING_LENGTH );
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
        return;
    }

    InputNumTradeDialog *pInputNumTradeDlg;

    pInputNumTradeDlg =
        (InputNumTradeDialog *)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_INPUT_NUM);

    if (pInputNumTradeDlg)
        g_InterfaceManager.DeleteInterface(pInputNumTradeDlg->rcinfo_GetID());

    pInputNumTradeDlg = new InputNumTradeDialog;
    assert(pInputNumTradeDlg);
    if ( !g_InterfaceManager.LoadInterface(
        pInputNumTradeDlg,
        (char *)pInputNumTradeDlg->rcinfo_GetPath(),
        pInputNumTradeDlg->rcinfo_GetID(),
        FALSE) 
        )
    {
        return;
    }

    //pInputNumTradeDlg->SetDialogKey( pInputNumTradeDlg->rcinfo_GetID() );
    //pInputNumTradeDlg->SetDialogResourceName( (char*)pInputNumTradeDlg->rcinfo_GetPath() );


    g_InterfaceManager.ShowWindow(InterfaceManager::DIALOG_INPUT_NUM, TRUE);
}

//------------------------------------------------------------------------------
/**
*/
bool
uiTradeMan::CheckEndTradingCauseRanged()
{
    if (!m_tradeInfo1.m_isSet || !m_tradeInfo2.m_isSet)
        return false;

    Object* pObj;

    pObj = g_ObjectManager.GetObject(this->m_tradeInfo1.m_dwKey);
    if (!pObj)
    {
        this->SendTradeCancel();
        return false;
    }

    pObj = g_ObjectManager.GetObject(this->m_tradeInfo2.m_dwKey);
    if (!pObj)
    {
        this->SendTradeCancel();
        return false;
    }

    int distance =
        static_cast<int>(VectorLength( &(this->m_tradeInfo2.m_pObject->GetPosition()- this->m_tradeInfo1.m_pObject->GetVisiblePos()) ));

    if ( distance > DISTANCE_TRADING )
    {
        //	�ŷ����� ������� �Ÿ��� �ָ� �ڵ����� �ŷ��� ��ҵ˴ϴ�.
        TCHAR	szMessage[INTERFACE_STRING_LENGTH];
        const int	c_iDistanceTradingCheck = 70390;
        g_InterfaceManager.GetInterfaceString( c_iDistanceTradingCheck, szMessage, INTERFACE_STRING_LENGTH );
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW, szMessage);
        this->SendTradeCancel();
    }

    return true;
}

void uiTradeMan::ErrorCode(DWORD dwErrorCode)
{
	int iMessage = 0;
	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
	switch ( dwErrorCode )
	{
	case RC_TRADE_CANCEL:
		{
			//5824	�ŷ��� ����߽��ϴ�.
			iMessage = 5824;
		}
		break;

	case RC_TRADE_PLAYER1_HAVENOTSPACE:
		{
			//5825	�κ��丮�� ���� ������ �����մϴ�.
			iMessage = 5825;
		}
		break;

	case RC_TRADE_PLAYER2_HAVENOTSPACE:
		{
			//5826	������ �κ��丮�� ���� ������ �����մϴ�.
			iMessage = 5826;
		}
		break;

	case RC_TRADE_PLAYER1_HAVENOTMONEY:
		{
			//551 ������ �����մϴ�.
			iMessage = 551;
		}
		break;

	case RC_TRADE_PLAYER2_HAVENOTMONEY:
		{
			//5827	������ ������ �����մϴ�.
			iMessage = 5827;
		}
		break;

	case RC_TRADE_COUNTERPARTNER_NOTACCEPT:
		{
			//5828	������ ��û�� ����Ͽ����ϴ�.
			iMessage = 5828;
		}
		break;

/*	case RC_TRADE_INVALID_STATE:
		{
			//5830	�߸��� �����Դϴ�.
			iMessage = 5830;
		}
		break;
*/
	case RC_TRADE_COUNTERPARTNER_CANNOT_STATE:
		{
			//5829	������ �ŷ��� �������� ���� �����Դϴ�.
			iMessage = 5829;
		}
		break;

	case RC_TRADE_CANNOT_TRADE_ITEM:
		{
			//5350	�ŷ��� �������� ���� �������Դϴ�.
			iMessage = 5350;
		}	
		break;


	case RC_TRADE_NOT_AVAILABLE_SERVICE:
		{
			//5831	����� �̿��Ͻ� �� ���� �����Դϴ�.
			iMessage = 5831;
		}
		break;

	case RC_TRADE_BLOCKED_ITEM:
		{

			//5811	���� �������Դϴ�.
			iMessage = 5811;
		}
		break;

	case RC_TRADE_INTERCEPTION:
		{
			//	6011	�ŷ� ��밡 �ŷ��� �����ϰ� �־� �ŷ��� �� �����ϴ�.
			iMessage = 6011;
		}
		break;


	case RC_TRADE_DIFFERENT_ITEM_INFO:
		{
			//5832	ó�� ����� �����۰� ���� �������� ������ �ٸ��ϴ�.
			iMessage = 5832;
		}
		break;

	case RC_TRADE_UNABLE_FUNCTION_FOR_CHAOSTATE:
		{
			//5275	ī�� ���¿����� �̿��� �������� �ʽ��ϴ�.
			iMessage = 5275;
		}
		break;

	case RC_TRADE_LENGTH_CANNOT_TRADE:
		{
			//5833	�ŷ��� �� ���� �Ÿ��Դϴ�.
			iMessage = 5833;
		}
		break;

	case RC_TRADE_DIFFERENT_ZONE:
		{
			//5834	������ �ٸ� ������ �ֽ��ϴ�.
			iMessage = 5834;
		}
		break;

	case RC_TRADE_MODIFY_CANCEL:
		{
			//6341	������ ��ǰ������ �����߽��ϴ�.
			iMessage = 6341;
		}
		break;

	case RC_TRADE_MODIFY_FAIL:
		{
			//6342 ������ ��ǰ ���� ��û�� �����Ͽ����ϴ�
			iMessage = 6342;
		}
		break;

	default:
		{
			//5823	�ŷ��� �����߽��ϴ�.
			iMessage = 5823;
		}
		break;
	}
	
	g_InterfaceManager.GetInterfaceString(iMessage , szMessage , INTERFACE_STRING_LENGTH);
	GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_CHAT, szMessage );
	GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW, szMessage);
}
#pragma warning ( pop )

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
