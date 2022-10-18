#include "SunClientPrecompiledHeader.h"

#include "uiCommunityDlg.h"
#include "uiCommunityMan/uiCommunityMan.h"
#include "uiCommunityReadMail/uiCommunityReadMail.h"
#include "uiCommunityInfo/uiCommunityInfo.h"

#include "uiSystemMan/uiSystemMan.h"
#include "Hero.h"
#include "GlobalFunc.h"
#include "ItemManager.h"
#include "interfacemanager.h"
#include "GameFramework.h"
#include "KeyQueueManager.h"
#include "DlgPosInfoParser.h"
#include "SceneBase.h"
#include "ResultCode.h"
#include "uiPartyMan/uiPartyMan.h"
#include "SolarDateTime.h"
#include "time.h"

using namespace RC;

const int c_iSellHeight		= 23;

const int c_iNameWidth		= 190;
const int c_iLvWidth		= 59;
const int c_iClassWidth		= 107;
const int c_iChannelWidth	= 74;
const int c_iDateWidth		= 130;
const int c_iMemoWidth		= 200;

bool CheckSpace( const char *pString )
{
	size_t strLen = strlen(pString);
	for (size_t n = 0; n < strLen; ++n)
	{
		if (pString[n] == ' ' || pString[n] == '\t' )
			return true;		
	}
	return false;
}
//------------------------------------------------------------------------------
/**
	친구 추가 
*/
void _CallBack_Friend_Add(bool bYes, const char *pString )
{
	uiCommunityMan* pCommMan = static_cast<uiCommunityMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_COMMUNITY));

	if(!pCommMan)
		return;

	if(bYes)
	{
#if defined(_KOREA)
        if (g_slangNameFilter.CheckIncludeString(pString))
#else
        if (GlobalFunc::CheckIncludeStringOnAbroad(pString))
#endif
        {
            // 공백이 있으면 관련 메시지 출력하고 패킷보내지 않음
            TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
            // 5346	잘못된 정보 입니다.
            g_InterfaceManager.GetInterfaceString( 5346, szMessage, MAX_MESSAGE_LENGTH );
            GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_CHAT, szMessage );			
        } 
		else 
		{	// 공백없으면 친구추가 패킷전송
			if (g_pNetwork && (g_pNetwork->IsCommunicatable(CI_CHATSERVERIDX) == TRUE))
			{		
				MSG_CW_FRIEND_ADD_SYN sync;
				ZeroMemory(sync.ptszCharNameTo,sizeof(sync.ptszCharNameTo));

				TCHAR szString[MAX_CHARNAME_LENGTH+1] ={0,};
				memcpy(szString,pString,MAX_CHARNAME_LENGTH);
				StrnCopy(sync.ptszCharNameTo , szString, MAX_CHARNAME_LENGTH);

				GlobalFunc::SendPacket(CI_CHATSERVERIDX, &sync, sizeof (sync));
			}
			else
			{
				TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
				g_InterfaceManager.GetInterfaceString(eST_SERVER_STATUS_NOT_SMOOTH, szMessage, INTERFACE_STRING_LENGTH);
				GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_TOP_SHOW, szMessage);  // REQSTRING
			}
		}		
	}
	pCommMan->SetSystemDialog(false);
}

//------------------------------------------------------------------------------
/**
	친구 삭제 
*/
void _CallBack_Friend_Delete(bool bYes)
{
	uiCommunityMan* pCommMan = static_cast<uiCommunityMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_COMMUNITY));

	if(!pCommMan)
		return;

	if (bYes)
	{	
		if(pCommMan->GetPendingDelFriend())
		{
			MSG_CW_FRIEND_DEL_SYN sync;
			ZeroMemory(sync.ptszFriendName,sizeof(sync.ptszFriendName));
			StrnCopy(sync.ptszFriendName , pCommMan->GetPendingDelFriend(), MAX_CHARNAME_LENGTH);
			
			GlobalFunc::SendPacket(CI_CHATSERVERIDX, &sync, sizeof (sync));

		}
	}

	pCommMan->SetPendingDelFriend(NULL);
	pCommMan->SetSystemDialog(false);
}

//------------------------------------------------------------------------------
/**
리벤지 목록 삭제 
*/
void _CallBack_Revenge_Delete(bool bYes)
{
	uiCommunityMan* pCommMan = static_cast<uiCommunityMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_COMMUNITY));

	if(!pCommMan)
		return;

	if (bYes)
	{	
		if(pCommMan->GetPendingDelRevenge())
		{
			MSG_CW_REVENGE_DEL_SYN sync;
			ZeroMemory(sync.m_ptszCharName,sizeof(sync.m_ptszCharName));
			StrnCopy(sync.m_ptszCharName , pCommMan->GetPendingDelRevenge(), MAX_CHARNAME_LENGTH);
			GlobalFunc::SendPacket(CI_CHATSERVERIDX, &sync, sizeof (sync));
		}
	}

	pCommMan->SetPendingDelRevenge(NULL);
	pCommMan->SetSystemDialog(false);
}

//------------------------------------------------------------------------------
/**
	메신져 대화거부  
*/
void _CallBack_MessageRejection(bool bYes)
{
	uiCommunityMan* pCommMan = static_cast<uiCommunityMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_COMMUNITY));

	if(!pCommMan)
		return;

	if (bYes)
	{	
		MSG_CW_FRIEND_CHATBLOCK_SYN sync;
		sync.m_bBlock = 1;
		GlobalFunc::SendPacket(CI_CHATSERVERIDX, &sync, sizeof (sync));	

		pCommMan->RemoveAllChatDialog();
	}
	else
	{
		BOOL rt = pCommMan->IsRecvMessage();
		pCommMan->SetMessageRejection(!rt);
	}

}

//------------------------------------------------------------------------------
/**
	차단 목록 추가 
*/
void _CallBack_BlockList_Add(bool bYes, const char * pString)
{
	uiCommunityMan* pCommMan = static_cast<uiCommunityMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_COMMUNITY));

	if(!pCommMan)
		return;

	if (bYes)
	{	
#if defined(_KOREA)
        if (g_slangNameFilter.CheckIncludeString(pString))
#else
        if (GlobalFunc::CheckIncludeStringOnAbroad(pString))
#endif
        {
            // 공백이 있으면 관련 메시지 출력하고 패킷보내지 않음
            TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
            // 5346	잘못된 정보 입니다.
            g_InterfaceManager.GetInterfaceString( 5346, szMessage, MAX_MESSAGE_LENGTH );
            GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_CHAT, szMessage );			
        }
        else
        {
            MSG_CW_FRIEND_BLOCK_SYN sync;
            ZeroMemory(sync.ptszBlockName,sizeof(sync.ptszBlockName));
            TCHAR szString[MAX_CHARNAME_LENGTH+1] ={0,};
            StrnCopy(szString, pString, MAX_CHARNAME_LENGTH);
            StrnCopy(sync.ptszBlockName ,szString, MAX_CHARNAME_LENGTH);

            GlobalFunc::SendPacket(CI_CHATSERVERIDX, &sync, sizeof (sync));
        }		
	}

	pCommMan->SetSystemDialog(false);
}

//------------------------------------------------------------------------------
/**
	차단 목록 삭제 
*/
void _CallBack_BlockList_Delete(bool bYes)
{
	uiCommunityMan* pCommMan = static_cast<uiCommunityMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_COMMUNITY));

	if(!pCommMan)
		return;

	if (bYes)
	{	
		if(pCommMan->GetPendingDelBlock())
		{
			MSG_CW_FRIEND_BLOCK_FREE_SYN sync;
			ZeroMemory(sync.ptszBlockName,sizeof(sync.ptszBlockName));

			StrnCopy(sync.ptszBlockName , pCommMan->GetPendingDelBlock(), MAX_CHARNAME_LENGTH);
			
			GlobalFunc::SendPacket(CI_CHATSERVERIDX, &sync, sizeof (sync));	
		}
	}

	pCommMan->SetPendingDelBlock(NULL);
	pCommMan->SetSystemDialog(false);
}

//------------------------------------------------------------------------------
/**
	그룹 멤버 추가 
*/
void _CallBack_GroupMember_Add(bool bYes, const char * pString)
{
	uiCommunityMan* pCommMan = static_cast<uiCommunityMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_COMMUNITY));

	if(!pCommMan)
		return;

	if (bYes)
	{	
#if defined(_KOREA)
        if (g_slangNameFilter.CheckIncludeString(pString))
#else
        if (GlobalFunc::CheckIncludeStringOnAbroad(pString))
#endif
        {
            // 공백이 있으면 관련 메시지 출력하고 패킷보내지 않음
            TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
            // 5346	잘못된 정보 입니다.
            g_InterfaceManager.GetInterfaceString( 5346, szMessage, MAX_MESSAGE_LENGTH );
            GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_CHAT, szMessage );			
        }
        else
        {
		    MSG_CW_GROUP_ADD_SYN sync;
		    ZeroMemory(sync.ptszMemeberName,sizeof(sync.ptszMemeberName));
		    TCHAR szString[MAX_CHARNAME_LENGTH+1] ={0,};
		    StrnCopy(szString, pString, MAX_CHARNAME_LENGTH);
		    StrnCopy(sync.ptszMemeberName ,szString, MAX_CHARNAME_LENGTH);

		    GlobalFunc::SendPacket(CI_CHATSERVERIDX, &sync, sizeof (sync));
        }
	}

	pCommMan->SetSystemDialog(false);
}

//------------------------------------------------------------------------------
/**
	그룹 멤버 삭제 
*/
void _CallBack_GroupMember_Delete(bool bYes)
{
	uiCommunityMan* pCommMan = static_cast<uiCommunityMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_COMMUNITY));

	if(!pCommMan)
		return;

	if (bYes)
	{	
		if(pCommMan->GetPendingDelGroup())
		{
			MSG_CW_GROUP_DEL_SYN sync;
			ZeroMemory(sync.ptszMemeberName,sizeof(sync.ptszMemeberName));

			StrnCopy(sync.ptszMemeberName , pCommMan->GetPendingDelGroup(), MAX_CHARNAME_LENGTH);
			
			GlobalFunc::SendPacket(CI_CHATSERVERIDX, &sync, sizeof (sync));	
		}
	}

	pCommMan->SetPendingDelGroup(NULL);
	pCommMan->SetSystemDialog(false);
}
//------------------------------------------------------------------------------
/**
	쪽지 삭제
*/
void _CallBack_Mail_Delete(bool bYes)
{
	uiCommunityMan* pCommMan = static_cast<uiCommunityMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_COMMUNITY));
	if(!pCommMan)
		return;

	if (bYes)
	{	
		if(pCommMan->GetPendingDelMemoGuid())
		{
			pCommMan->SEND_CW_MEMO_DELETE_SYN(pCommMan->GetPendingDelMemoGuid());
		}
	}

	pCommMan->SetPendingDelMemoGuid(0);
	pCommMan->SetSystemDialog(false);
}

//------------------------------------------------------------------------------
/**
	전체 쪽지 삭제
*/
void _CallBack_All_Mail_Delete(bool bYes)
{
	uiCommunityMan* pCommMan = static_cast<uiCommunityMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_COMMUNITY));

	if(!pCommMan)
		return;

	if (bYes)
	{	
		pCommMan->SEND_CW_MEMO_DELETE_SYN(0);
	}

	pCommMan->SetPendingDelMemoGuid(0);
	pCommMan->SetSystemDialog(false);
	
}

//------------------------------------------------------------------------------
WzID uiCommunityDlg::m_wzId[COMMUNITY_DIALOG_MAX] = 
{
        StrToWzID("B999"), //COMMUNITY_DIALOG_EXIT	= 0,


		// 각종 탭
#ifdef _NA_000000_20130114_RENEWER_UI
#else
		StrToWzID("C001"),		//COMMUNITY_DIALOG_TAB_FRIEND,
#endif//#ifdef _NA_000000_20130114_RENEWER_UI
		StrToWzID("C002"),		//COMMUNITY_DIALOG_TAB_MAIL,
		StrToWzID("CT04"),		//COMMUNITY_DIALOG_TAB_GROUP,
		StrToWzID("CT00"),		//COMMUNITY_DIALOG_TAB_SUB_FRIEND,
		StrToWzID("CT01"),		//COMMUNITY_DIALOG_TAB_SUB_BLOCK,
		StrToWzID("CT02"),		//COMMUNITY_DIALOG_TAB_SUB_REVENGE,

		//// 텍스트
		StrToWzID("T004"),		//COMMUNITY_DIALOG_TXT_1,
		StrToWzID("ST02"),		//COMMUNITY_DIALOG_TXT_2,
		StrToWzID("ST01"),		//COMMUNITY_DIALOG_TXT_3,
		StrToWzID("ST04"),		//COMMUNITY_DIALOG_TXT_REVENGE_POINT,
		StrToWzID("ST00"),		//COMMUNITY_DIALOG_TXT_REJECTION_CHAT,
		StrToWzID("T003"),		//COMMUNITY_DIALOG_TXT_HIDE_OFFLINE,
		StrToWzID("ST03"),		//COMMUNITY_DIALOG_TXT_SHOW_NEW_MAIL,
		StrToWzID("ST05"),		//COMMUNITY_DIALOG_TXT_SHOW_NEW_MAIL,

		//// 체크버튼
		StrToWzID("C003"),		//COMMUNITY_DIALOG_CHECK_REJECTION_CHAT,
		StrToWzID("CT03"),		//COMMUNITY_DIALOG_CHECK_HIDE_OFFLINE,
		StrToWzID("CT28"),		//COMMUNITY_DIALOG_CHECK_SHOW_NEW_MAIL,
		StrToWzID("CT05"),		//COMMUNITY_DIALOG_CHECK_REJECTION_GROUP,

		//// 스크롤
		StrToWzID("VS00"),		//COMMUNITY_DIALOG_VSCROLL,

		//// 리스트
		StrToWzID("LS00"),		//COMMUNITY_DIALOG_LST_FRIEND,
		StrToWzID("L001"),		//COMMUNITY_DIALOG_LST_MAIL,

		//// 버튼
		//// ON/OFF 아이콘
		StrToWzID("BT13"),		//COMMUNITY_DIALOG_BTN_ON1,
		StrToWzID("BT14"),		//COMMUNITY_DIALOG_BTN_ON2,
		StrToWzID("BT15"),		//COMMUNITY_DIALOG_BTN_ON3,
		StrToWzID("BT16"),		//COMMUNITY_DIALOG_BTN_ON4,
		StrToWzID("BT17"),		//COMMUNITY_DIALOG_BTN_ON5,
		StrToWzID("BT18"),		//COMMUNITY_DIALOG_BTN_ON6,
		StrToWzID("BT19"),		//COMMUNITY_DIALOG_BTN_ON7,
		StrToWzID("BT20"),		//COMMUNITY_DIALOG_BTN_ON8,
		StrToWzID("BT21"),		//COMMUNITY_DIALOG_BTN_ON9,
		StrToWzID("BT22"),		//COMMUNITY_DIALOG_BTN_ON10,
		
		StrToWzID("BT01"),		//COMMUNITY_DIALOG_BTN_OFF1,
		StrToWzID("BT02"),		//COMMUNITY_DIALOG_BTN_OFF2,
		StrToWzID("BT03"),		//COMMUNITY_DIALOG_BTN_OFF3,
		StrToWzID("BT04"),		//COMMUNITY_DIALOG_BTN_OFF4,
		StrToWzID("BT05"),		//COMMUNITY_DIALOG_BTN_OFF5,
		StrToWzID("BT06"),		//COMMUNITY_DIALOG_BTN_OFF6,
		StrToWzID("BT07"),		//COMMUNITY_DIALOG_BTN_OFF7,
		StrToWzID("BT08"),		//COMMUNITY_DIALOG_BTN_OFF8,
		StrToWzID("BT09"),		//COMMUNITY_DIALOG_BTN_OFF9,
		StrToWzID("BT10"),		//COMMUNITY_DIALOG_BTN_OFF10,
		
		////새쪽지/읽은 쪽지 아이콘
		StrToWzID("BT37"),		//COMMUNITY_DIALOG_BTN_NEW1,
		StrToWzID("BT38"),		//COMMUNITY_DIALOG_BTN_NEW2,
		StrToWzID("BT39"),		//COMMUNITY_DIALOG_BTN_NEW3,
		StrToWzID("BT40"),		//COMMUNITY_DIALOG_BTN_NEW4,
		StrToWzID("BT41"),		//COMMUNITY_DIALOG_BTN_NEW5,
		StrToWzID("BT42"),		//COMMUNITY_DIALOG_BTN_NEW6,
		StrToWzID("BT43"),		//COMMUNITY_DIALOG_BTN_NEW7,
		StrToWzID("BT44"),		//COMMUNITY_DIALOG_BTN_NEW8,
		StrToWzID("BT45"),		//COMMUNITY_DIALOG_BTN_NEW9,
		StrToWzID("BT46"),		//COMMUNITY_DIALOG_BTN_NEW10,
		

		StrToWzID("BT25"),		//COMMUNITY_DIALOG_BTN_READ1,
		StrToWzID("BT26"),		//COMMUNITY_DIALOG_BTN_READ2,
		StrToWzID("BT27"),		//COMMUNITY_DIALOG_BTN_READ3,
		StrToWzID("BT28"),		//COMMUNITY_DIALOG_BTN_READ4,
		StrToWzID("BT29"),		//COMMUNITY_DIALOG_BTN_READ5,
		StrToWzID("BT30"),		//COMMUNITY_DIALOG_BTN_READ6,
		StrToWzID("BT31"),		//COMMUNITY_DIALOG_BTN_READ7,
		StrToWzID("BT32"),		//COMMUNITY_DIALOG_BTN_READ8,
		StrToWzID("BT33"),		//COMMUNITY_DIALOG_BTN_READ9,
		StrToWzID("BT34"),		//COMMUNITY_DIALOG_BTN_READ10,
		
		// 친구초대 이벤트용 ON/OFF 아이콘
		StrToWzID("BT49"),		//COMMUNITY_DIALOG_BTN_EVTOFF1,
		StrToWzID("BT48"),		//COMMUNITY_DIALOG_BTN_EVTOFF2,
		StrToWzID("BT47"),		//COMMUNITY_DIALOG_BTN_EVTOFF3,
		StrToWzID("BT36"),		//COMMUNITY_DIALOG_BTN_EVTOFF4,
		StrToWzID("BT35"),		//COMMUNITY_DIALOG_BTN_EVTOFF5,
		StrToWzID("BT24"),		//COMMUNITY_DIALOG_BTN_EVTOFF6,
		StrToWzID("BT23"),		//COMMUNITY_DIALOG_BTN_EVTOFF7,
		StrToWzID("BT69"),		//COMMUNITY_DIALOG_BTN_EVTOFF8,
		StrToWzID("BT67"),		//COMMUNITY_DIALOG_BTN_EVTOFF9,
		StrToWzID("BT65"),		//COMMUNITY_DIALOG_BTN_EVTOFF10,
		
		StrToWzID("BT50"),		//COMMUNITY_DIALOG_BTN_EVTON1,
		StrToWzID("BT51"),		//COMMUNITY_DIALOG_BTN_EVTON2,
		StrToWzID("BT52"),		//COMMUNITY_DIALOG_BTN_EVTON3,
		StrToWzID("BT53"),		//COMMUNITY_DIALOG_BTN_EVTON4,
		StrToWzID("BT54"),		//COMMUNITY_DIALOG_BTN_EVTON5,
		StrToWzID("BT55"),		//COMMUNITY_DIALOG_BTN_EVTON6,
		StrToWzID("BT56"),		//COMMUNITY_DIALOG_BTN_EVTON7,
		StrToWzID("BT57"),		//COMMUNITY_DIALOG_BTN_EVTON8,
		StrToWzID("BT58"),		//COMMUNITY_DIALOG_BTN_EVTON9,
		StrToWzID("BT59"),		//COMMUNITY_DIALOG_BTN_EVTON10,

		StrToWzID("TI00"),		//COMMUNITY_DIALOG_BTN_ADD_FRIEND,
		StrToWzID("TI01"),		//COMMUNITY_DIALOG_BTN_DEL_FRIEND,
		StrToWzID("TI02"),		//COMMUNITY_DIALOG_BTN_ADD_BLOCK,
		StrToWzID("TI03"),		//COMMUNITY_DIALOG_BTN_DEL_BLOCK,
		StrToWzID("TI07"),		//COMMUNITY_DIALOG_BTN_DEL_REVENGE,
		StrToWzID("TI08"),		//COMMUNITY_DIALOG_BTN_WARP,
		StrToWzID("TI05"),		//COMMUNITY_DIALOG_BTN_DEL_MAIL,
		StrToWzID("TI06"),		//COMMUNITY_DIALOG_BTN_DEL_ALL_MAIL,
		StrToWzID("TI04"),		//COMMUNITY_DIALOG_BTN_WRITE_MAIL,
		StrToWzID("TI09"),		//COMMUNITY_DIALOG_BTN_INVITE,

		StrToWzID("BT60"),		//COMMUNITY_DIALOG_LIST_BTN_ID,
		StrToWzID("BT61"),		//COMMUNITY_DIALOG_LIST_BTN_LV,
		StrToWzID("BT63"),		//COMMUNITY_DIALOG_LIST_BTN_CLASS,
		StrToWzID("BT62"),		//COMMUNITY_DIALOG_LIST_BTN_CHANNEL,
		StrToWzID("BT64"),		//COMMUNITY_DIALOG_LIST_BTN_LOGON,

		StrToWzID("BT00"),		//COMMUNITY_DIALOG_LIST_BTN_SENDER,
		StrToWzID("BT11"),		//COMMUNITY_DIALOG_LIST_BTN_DATE,
		StrToWzID("BT12"),		//COMMUNITY_DIALOG_LIST_BTN_MEMO,

		StrToWzID("TI11"),		//COMMUNITY_DIALOG_BTN_ADD_MEMBER,
		StrToWzID("TI12"),		//COMMUNITY_DIALOG_BTN_DEL_MEMBER,
};

//------------------------------------------------------------------------------
/**
*/
uiCommunityDlg::uiCommunityDlg(InterfaceManager *pUIMan) :
    uiBase(pUIMan),
    m_bControlsInitialized(false)
{
    m_pCommunityMan = static_cast<uiCommunityMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_COMMUNITY));
    assert (m_pCommunityMan);

    if (!this->m_pCommunityMan)
    {
#ifdef _DEBUG
		 uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
			if(pSystemMan)
				pSystemMan->Sys_VerifyLock(_T("m_pCommunityMan이 없습니다."));
#endif
        this->ShowInterface(FALSE);
        return;
    }
}

//------------------------------------------------------------------------------
/**
*/
uiCommunityDlg::~uiCommunityDlg()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityDlg::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

    this->InitControls();
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityDlg::Release()
{
}

//------------------------------------------------------------------------------
/**
*/
uiCommunityMan* uiCommunityDlg::GetManager()
{
	if(m_pCommunityMan)
		return this->m_pCommunityMan;
	else
	{
		this->m_pCommunityMan =
        static_cast<uiCommunityMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_COMMUNITY));
		assert (m_pCommunityMan);

		return this->m_pCommunityMan;
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityDlg::InitControls()
{
	int i=0;
	for(i=0; i<COMMUNITY_TAB_MAX; ++i)
	{
#ifdef _NA_000000_20130114_RENEWER_UI
		m_pBtnCheckTabs[i] = static_cast<CCtrlButtonCheckWZ *>(this->getControl(COMMUNITY_DIALOG_TAB_MAIL+i));
#else
		m_pBtnCheckTabs[i] = static_cast<CCtrlButtonCheckWZ *>(this->getControl(COMMUNITY_DIALOG_TAB_FRIEND+i));
#endif//#ifdef _NA_000000_20130114_RENEWER_UI
		assert(m_pBtnCheckTabs[i]);

		if(m_pBtnCheckTabs[i])
			m_pBtnCheckTabs[i]->ShowWindowWZ(WZ_HIDE);
	}

	
	for(i=0; i<COMMUNITY_TXT_MAX; ++i)
	{
		m_pTxtCtrls[i] = static_cast<CCtrlStaticWZ *>(this->getControl(COMMUNITY_DIALOG_TXT_1+i));
		assert(m_pTxtCtrls[i]);

		if(m_pTxtCtrls[i])
			m_pTxtCtrls[i]->ShowWindowWZ(WZ_HIDE);
	}

	for(i=0; i<COMMUNITY_CHECK_MAX; ++i)
	{
		m_pBtnChecks[i] = static_cast<CCtrlButtonCheckWZ *>(this->getControl(COMMUNITY_DIALOG_CHECK_REJECTION_CHAT+i));
		assert(m_pBtnChecks[i]);

		if(m_pBtnChecks[i])
			m_pBtnChecks[i]->ShowWindowWZ(WZ_HIDE);
	}

	for(i=0; i<COMMUNITY_LST_MAX; ++i)
	{
		m_pListCtrls[i] = static_cast<CCtrlListWZ *>(this->getControl(COMMUNITY_DIALOG_LST_FRIEND+i));
		assert(m_pListCtrls[i]);

		if(m_pListCtrls[i])
			m_pListCtrls[i]->ShowWindowWZ(WZ_HIDE);
	}

	for(i=0; i<COMMUNITY_BTN_MAX; ++i)
	{
		m_pBtnCtrls[i] = static_cast<CCtrlButtonWZ *>(this->getControl(COMMUNITY_DIALOG_BTN_ADD_FRIEND+i));
		assert(m_pBtnCtrls[i]);

		if(m_pBtnCtrls[i])
			m_pBtnCtrls[i]->ShowWindowWZ(WZ_HIDE);
	}

	for(i=0; i<COMMUNITY_BTN_ON_MAX; ++i)
	{
		m_pOnBtn[i] = static_cast<CCtrlButtonWZ *>(this->getControl(COMMUNITY_DIALOG_BTN_ON1+i));
		assert(m_pOnBtn[i]);

		if(m_pOnBtn[i])
			m_pOnBtn[i]->ShowWindowWZ(WZ_HIDE);
	}

	for(i=0; i<COMMUNITY_BTN_OFF_MAX; ++i)
	{
		m_pOffBtn[i] = static_cast<CCtrlButtonWZ *>(this->getControl(COMMUNITY_DIALOG_BTN_OFF1+i));
		assert(m_pOffBtn[i]);

		if(m_pOffBtn[i])
			m_pOffBtn[i]->ShowWindowWZ(WZ_HIDE);
	}

	for(i=0; i<COMMUNITY_BTN_NEW_MAX; ++i)
	{
		m_pNewBtn[i] = static_cast<CCtrlButtonWZ *>(this->getControl(COMMUNITY_DIALOG_BTN_NEW1+i));
		assert(m_pNewBtn[i]);

		if(m_pNewBtn[i])
			m_pNewBtn[i]->ShowWindowWZ(WZ_HIDE);
	}

	for(i=0; i<COMMUNITY_BTN_READ_MAX; ++i)
	{
		m_pReadBtn[i] = static_cast<CCtrlButtonWZ *>(this->getControl(COMMUNITY_DIALOG_BTN_READ1+i));
		assert(m_pReadBtn[i]);

		if(m_pReadBtn[i])
			m_pReadBtn[i]->ShowWindowWZ(WZ_HIDE);
	}

		for(i=0; i<COMMUNITY_BTN_EVTON_MAX; ++i)
		{
			m_pEvtOnBtn[i] = static_cast<CCtrlButtonWZ *>(this->getControl(COMMUNITY_DIALOG_BTN_EVTON1+i));
			assert(m_pEvtOnBtn[i]);

			if(m_pEvtOnBtn[i])
				m_pEvtOnBtn[i]->ShowWindowWZ(WZ_HIDE);
		}

		for(i=0; i<COMMUNITY_BTN_EVTOFF_MAX; ++i)
		{
			m_pEvtOffBtn[i] = static_cast<CCtrlButtonWZ *>(this->getControl(COMMUNITY_DIALOG_BTN_EVTOFF1+i));
			assert(m_pEvtOffBtn[i]);

			if(m_pEvtOffBtn[i])
				m_pEvtOffBtn[i]->ShowWindowWZ(WZ_HIDE);
		}

#ifdef _NA_000000_20130114_RENEWER_UI
#else
	if(m_pBtnCheckTabs[COMMUNITY_TAB_FRIEND])
		m_pBtnCheckTabs[COMMUNITY_TAB_FRIEND]->ShowWindowWZ(WZ_SHOW);
#endif
	if(m_pBtnCheckTabs[COMMUNITY_TAB_MAIL])
		m_pBtnCheckTabs[COMMUNITY_TAB_MAIL]->ShowWindowWZ(WZ_SHOW);


    if (m_pBtnCheckTabs[COMMUNITY_TAB_GROUP] != NULL)
    {
#ifdef _DH_NOAPPLY_GROUPINGSYSTEM_
        m_pBtnCheckTabs[COMMUNITY_TAB_GROUP]->ShowWindowWZ(WZ_HIDE);
#else
        m_pBtnCheckTabs[COMMUNITY_TAB_GROUP]->ShowWindowWZ(WZ_SHOW);
#endif
    }

	if(m_pBtnCtrls[COMMUNITY_LIST_BTN_ID])
		m_pBtnCtrls[COMMUNITY_LIST_BTN_ID]->ShowWindowWZ(WZ_SHOW);
	if(m_pBtnCtrls[COMMUNITY_LIST_BTN_LV])
		m_pBtnCtrls[COMMUNITY_LIST_BTN_LV]->ShowWindowWZ(WZ_SHOW);
	if(m_pBtnCtrls[COMMUNITY_LIST_BTN_CLASS])
		m_pBtnCtrls[COMMUNITY_LIST_BTN_CLASS]->ShowWindowWZ(WZ_SHOW);
	if(m_pBtnCtrls[COMMUNITY_LIST_BTN_CHANNEL])
		m_pBtnCtrls[COMMUNITY_LIST_BTN_CHANNEL]->ShowWindowWZ(WZ_SHOW);
	if(m_pBtnCtrls[COMMUNITY_LIST_BTN_LOGON])
		m_pBtnCtrls[COMMUNITY_LIST_BTN_LOGON]->ShowWindowWZ(WZ_SHOW);

	if(m_pBtnCtrls[COMMUNITY_BTN_WRITE_MAIL])
		m_pBtnCtrls[COMMUNITY_BTN_WRITE_MAIL]->ShowWindowWZ(WZ_SHOW);

	m_pScroll = static_cast<CCtrlVScrollWZ *>(getControl(COMMUNITY_DIALOG_VSCROLL));
	assert(m_pScroll);

    RECT control_rect;
    for (int column_index = FRIEND_LIST_NAME; column_index < FRIEND_LIST_MAX; ++column_index)
    {
        control_rect = m_pBtnCtrls[COMMUNITY_LIST_BTN_ID + column_index]->GetSize();
        if (column_index == FRIEND_LIST_NAME)
        {
            control_rect.right -= 32;
        }
        friend_list_column_[column_index] = control_rect.right - 2;
    }

    for (int column_index = MAIL_LIST_NAME; column_index < MAIL_LIST_MAX; ++column_index)
    {
        control_rect = m_pBtnCtrls[COMMUNITY_LIST_BTN_SENDER + column_index]->GetSize();
        if (column_index == MAIL_LIST_NAME)
        {
            control_rect.right -= 32;
        }
        mail_list_column_[column_index] = control_rect.right - 2;
    }

    this->m_bControlsInitialized = true;
}


//------------------------------------------------------------------------------
void uiCommunityDlg::OnShowWindow( BOOL val )
{
    if (val)
    {
        if (GameFramework::GetCurScene() == SCENE_TYPE_SSQ)
        {
            ShowInterface(FALSE);
            return;
        }

        if (!this->m_bControlsInitialized)
        {
            this->ShowInterface(FALSE);
            return;
        }
    
        //if (m_pScroll)
        //{
        //    m_pScroll->SetScrollPosWZ(0);
        //}

		if(GetManager())
		{
			_clearSelection();
			GetManager()->SEND_CW_FRIEND_NEWLIST_SYN();

			GetManager()->SEND_CW_MEMO_LIST_REQ();

			GetManager()->SEND_CW_GROUP_NEWLIST_SYN();

		}

        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = eDoType_dialog_community_dlg;
        msg.wParam = InterfaceManager::DIALOG_COMMUNITY_DLG;
        msg.DoSomething = GlobalFunc::DoShowWindow;
        g_KeyQueueManager.PushBack(msg);

		GetManager()->SetSystemDialog(false);

		//POINT pt;
		//WzID DlgID = GetDialogWZ()->GetDlgID();
		//if ( DlgPosInfoParser::Instance()->GetDlgPoint(DlgID, &pt) )
		//{
		//	GetDialogWZ()->MoveWindowWZ( (float)pt.x, (float)pt.y );
		//}

        ChangeMode(GetManager()->last_community_mode(), true);

	}
    else
    {
		GetManager()->ShowFriendInfoDlg(FALSE);
        g_KeyQueueManager.DeleteMsg(eDoType_dialog_community_dlg);
    }
}

//------------------------------------------------------------------------------
/**
*/
BOOL uiCommunityDlg::_isEnable()
{
	if(GENERALPARAM->IsNet())
	{
		if (this->IsLock())
			return FALSE;

		if(!g_pHero)
			return FALSE;

		if(g_pHero && g_pHero->IsShowFullMap())
			return FALSE;

		if(!GetManager())
			return FALSE;

		return TRUE;
	}
	else
	{
		return TRUE;
	}

}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityDlg::Clear()
{
	if(GetManager())
	{
		GetManager()->ClearFriendData();
		GetManager()->SetMode(eCM_FRIEND);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityDlg::Reset()
{
    SolarDialog::Reset();

	GetManager()->SetSystemDialog(false);

	if(GetManager())
	{
		GetManager()->SetRqstJoinGame(false);
		GetManager()->RemoveAllChatDialog();
		GetManager()->ClearFriendData();
		GetManager()->SetMode(eCM_FRIEND);
		_clearSelection();
	}
}

//------------------------------------------------------------------------------
/**
*/

void uiCommunityDlg::_setDefaultFriend(ENUM_STATEWZ eState
#ifdef _NA_000000_20130114_RENEWER_UI
										,bool ShowingTap
#endif
										)
{
#ifdef _NA_000000_20130114_RENEWER_UI
	CControlWZ* pControl = getControl(COMMUNITY_DIALOG_TAB_SUB_FRIEND);
	if(ShowingTap == true)
	{
		if (pControl)
		{
			pControl->ShowWindowWZ(eState);
		}

		pControl = getControl(COMMUNITY_DIALOG_TAB_SUB_BLOCK);
		if (pControl)
		{
			pControl->ShowWindowWZ(eState);
		}

		pControl = getControl(COMMUNITY_DIALOG_TAB_SUB_REVENGE);
		if (pControl)
		{
			pControl->ShowWindowWZ(eState);
		}
	}
#else
	CControlWZ* pControl = getControl(COMMUNITY_DIALOG_TAB_SUB_FRIEND);
    if (pControl)
    {
        pControl->ShowWindowWZ(eState);
    }

	pControl = getControl(COMMUNITY_DIALOG_TAB_SUB_BLOCK);
    if (pControl)
    {
        pControl->ShowWindowWZ(eState);
    }

	pControl = getControl(COMMUNITY_DIALOG_TAB_SUB_REVENGE);
    if (pControl)
    {
        pControl->ShowWindowWZ(eState);
    }

#endif //#ifdef _NA_000000_20130114_RENEWER_UI
	
	pControl = getControl(COMMUNITY_DIALOG_LST_FRIEND);
    if (pControl)
    {
        pControl->ShowWindowWZ(eState);
    }

	pControl = getControl(COMMUNITY_DIALOG_TXT_HIDE_OFFLINE);
    if (pControl)
    {
        pControl->ShowWindowWZ(eState);
    }

	pControl = getControl(COMMUNITY_DIALOG_CHECK_HIDE_OFFLINE);
    if (pControl)
    {
        pControl->ShowWindowWZ(eState);
    }

	pControl = getControl(COMMUNITY_DIALOG_TXT_REJECTION_CHAT);
    if (pControl)
    {
        pControl->ShowWindowWZ(eState);
    }

	pControl = getControl(COMMUNITY_DIALOG_CHECK_REJECTION_CHAT);
    if (pControl)
    {
        pControl->ShowWindowWZ(eState);
    }

	if(eState == WZ_HIDE)
	{
		pControl = getControl(COMMUNITY_DIALOG_BTN_ADD_FRIEND);
		if (pControl)
		{
			pControl->ShowWindowWZ(eState);
		}

		pControl = getControl(COMMUNITY_DIALOG_BTN_DEL_FRIEND);
		if (pControl)
		{
			pControl->ShowWindowWZ(eState);
		}

		pControl = getControl(COMMUNITY_DIALOG_BTN_ADD_BLOCK);
		if (pControl)
		{
			pControl->ShowWindowWZ(eState);
		}

		pControl = getControl(COMMUNITY_DIALOG_BTN_DEL_BLOCK);
		if (pControl)
		{
			pControl->ShowWindowWZ(eState);
		}

		pControl = getControl(COMMUNITY_DIALOG_BTN_DEL_REVENGE);
		if (pControl)
		{
			pControl->ShowWindowWZ(eState);
		}

		pControl = getControl(COMMUNITY_DIALOG_BTN_WARP);
		if (pControl)
		{
			pControl->ShowWindowWZ(eState);
		}

	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityDlg::RefreshOnlineMember(eCOMMUNITY_MODE community_mode)
{
    if (m_pTxtCtrls[COMMUNITY_TXT_3])
	{
		TCHAR channel_connect_string[INTERFACE_STRING_LENGTH] ={0,};
		// 접속자수....
        int total_member = 0;
        int connect_member = 0;
        if (community_mode == eCM_FRIEND)
        {
            total_member = GetManager()->m_FriendArray.Size();
            connect_member = GetManager()->onlined_friend_count();
        }
        else if (connect_member == eCM_GROUP)
        {
            total_member = GetManager()->m_GroupArray.Size();
            connect_member = GetManager()->onlined_group_count();
        }
        Snprintf(channel_connect_string, INTERFACE_STRING_LENGTH-1, _T("(%d/%d)"), connect_member, total_member);
		m_pTxtCtrls[COMMUNITY_TXT_3]->ShowWindowWZ(WZ_SHOW);
		m_pTxtCtrls[COMMUNITY_TXT_3]->SetTextWZ(channel_connect_string);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityDlg::_setModeFriend(ENUM_STATEWZ eState)
{
	if (!_isEnable())
        return;

	_setDefaultFriend(eState);

	CControlWZ* pControl = getControl(COMMUNITY_DIALOG_BTN_ADD_FRIEND);
    if (pControl)
    {
        pControl->ShowWindowWZ(eState);
    }

	pControl = getControl(COMMUNITY_DIALOG_BTN_DEL_FRIEND);
    if (pControl)
    {
        pControl->ShowWindowWZ(eState);
    }

	pControl = getControl(COMMUNITY_DIALOG_BTN_WRITE_MAIL);
    if (pControl)
    {
        pControl->ShowWindowWZ(eState);
    }

	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};

	if(eState == WZ_SHOW)
	{
		_hideMailIcons();

		if(m_pTxtCtrls[COMMUNITY_TXT_REVENGE_POINT])
			m_pTxtCtrls[COMMUNITY_TXT_REVENGE_POINT]->ShowWindowWZ(WZ_HIDE);
		
		if(m_pTxtCtrls[COMMUNITY_TXT_2])
			m_pTxtCtrls[COMMUNITY_TXT_2]->ShowWindowWZ(WZ_HIDE);

        if(m_pTxtCtrls[COMMUNITY_TXT_1])
		{
			TCHAR channel_connect_string[INTERFACE_STRING_LENGTH] ={0,};
			//5797	%s서버 : %d채널 접속 중
			g_InterfaceManager.GetInterfaceString( 5797, channel_connect_string ,INTERFACE_STRING_LENGTH);
			Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, channel_connect_string, GetManager()->GetMyServer(), GetManager()->GetMyChannel() );
			m_pTxtCtrls[COMMUNITY_TXT_1]->ShowWindowWZ(WZ_SHOW);
			m_pTxtCtrls[COMMUNITY_TXT_1]->SetTextWZ(szTemp);
		}

		if(m_pBtnCtrls[COMMUNITY_BTN_INVITE])
        {
			m_pBtnCtrls[COMMUNITY_BTN_INVITE]->ShowWindowWZ(WZ_SHOW);
        }
		
        if (m_pTxtCtrls[COMMUNITY_TXT_3])
		{
			// 접속자수....
            int total_friend = GetManager()->m_FriendArray.Size();
            int connect_friend = GetManager()->onlined_friend_count();
            Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("(%d/%d)"), connect_friend, total_friend);
	        Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, 15, 50);
			m_pTxtCtrls[COMMUNITY_TXT_3]->ShowWindowWZ(WZ_SHOW);
			m_pTxtCtrls[COMMUNITY_TXT_3]->SetTextWZ(szTemp);
		}
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiCommunityDlg::_setModeBlock(ENUM_STATEWZ eState)
{
	if (!_isEnable())
        return;

	_setDefaultFriend(eState);

	CControlWZ* pControl = getControl(COMMUNITY_DIALOG_BTN_ADD_BLOCK);
    if (pControl)
    {
        pControl->ShowWindowWZ(eState);
    }

	pControl = getControl(COMMUNITY_DIALOG_BTN_DEL_BLOCK);
    if (pControl)
    {
        pControl->ShowWindowWZ(eState);
    }

	if(eState == WZ_SHOW)
	{
		_hideOnOffIcons();	// 차단 목록에선 표시 온/오프 아이콘 표시 안함
		_hideMailIcons();

		pControl = getControl(COMMUNITY_DIALOG_BTN_WRITE_MAIL);	// 차단 목록에선 쪽지쓰기 표시안함
		if (pControl)
		{
			pControl->ShowWindowWZ(WZ_HIDE);
		}

		if(m_pTxtCtrls[COMMUNITY_TXT_REVENGE_POINT])
			m_pTxtCtrls[COMMUNITY_TXT_REVENGE_POINT]->ShowWindowWZ(WZ_HIDE);
		
		if(m_pTxtCtrls[COMMUNITY_TXT_2])
			m_pTxtCtrls[COMMUNITY_TXT_2]->ShowWindowWZ(WZ_HIDE);

		if(m_pBtnCtrls[COMMUNITY_BTN_INVITE])
			m_pBtnCtrls[COMMUNITY_BTN_INVITE]->ShowWindowWZ(WZ_HIDE);

		TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
		TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};

        if(m_pTxtCtrls[COMMUNITY_TXT_1])
		{
			TCHAR channel_connect_string[INTERFACE_STRING_LENGTH] ={0,};
			//5797	%s서버 : %d채널 접속 중
			g_InterfaceManager.GetInterfaceString(5797, channel_connect_string,INTERFACE_STRING_LENGTH);
			Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, channel_connect_string, GetManager()->GetMyServer(), 
                GetManager()->GetMyChannel());
			m_pTxtCtrls[COMMUNITY_TXT_1]->ShowWindowWZ(WZ_SHOW);
			m_pTxtCtrls[COMMUNITY_TXT_1]->SetTextWZ(szTemp);
		}
        if(m_pTxtCtrls[COMMUNITY_TXT_3])
		{
			m_pTxtCtrls[COMMUNITY_TXT_3]->ShowWindowWZ(WZ_HIDE);
		}
	}
#ifdef _100331_LTJ_MOD_COMMUNITY_BLOCK_MODE_JAPAN
    ENUM_STATEWZ show_state = (eState == WZ_SHOW) ? WZ_HIDE : WZ_SHOW;
    if(m_pBtnCtrls[COMMUNITY_LIST_BTN_LV])
        m_pBtnCtrls[COMMUNITY_LIST_BTN_LV]->ShowWindowWZ(show_state);
    if(m_pBtnCtrls[COMMUNITY_LIST_BTN_CLASS])
        m_pBtnCtrls[COMMUNITY_LIST_BTN_CLASS]->ShowWindowWZ(show_state);
    if(m_pBtnCtrls[COMMUNITY_LIST_BTN_CHANNEL])
        m_pBtnCtrls[COMMUNITY_LIST_BTN_CHANNEL]->ShowWindowWZ(show_state);
    if(m_pBtnCtrls[COMMUNITY_LIST_BTN_LOGON])
        m_pBtnCtrls[COMMUNITY_LIST_BTN_LOGON]->ShowWindowWZ(show_state);
#endif //_100331_LTJ_MOD_COMMUNITY_BLOCK_MODE_JAPAN
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityDlg::_setModeRevenge(ENUM_STATEWZ eState)
{
	if (!_isEnable())
        return;

	_setDefaultFriend(eState);

	CControlWZ* pControl = getControl(COMMUNITY_DIALOG_BTN_DEL_REVENGE);
    if (pControl)
    {
        pControl->ShowWindowWZ(eState);
    }

	pControl = getControl(COMMUNITY_DIALOG_BTN_WARP);
    if (pControl)
    {
        pControl->ShowWindowWZ(eState);
    }

	if(eState == WZ_SHOW)
	{
		_hideMailIcons();
		
		pControl = getControl(COMMUNITY_DIALOG_BTN_WRITE_MAIL);	// 리벤지 목록에선 쪽지쓰기 표시안함
		if (pControl)
		{
			pControl->ShowWindowWZ(WZ_HIDE);
		}

		if(m_pTxtCtrls[COMMUNITY_TXT_1])
			m_pTxtCtrls[COMMUNITY_TXT_1]->ShowWindowWZ(WZ_HIDE);
		
		if(m_pTxtCtrls[COMMUNITY_TXT_2])
			m_pTxtCtrls[COMMUNITY_TXT_2]->ShowWindowWZ(WZ_HIDE);

		if(m_pTxtCtrls[COMMUNITY_TXT_3])
			m_pTxtCtrls[COMMUNITY_TXT_3]->ShowWindowWZ(WZ_HIDE);
		
		if(m_pBtnCtrls[COMMUNITY_BTN_INVITE])
			m_pBtnCtrls[COMMUNITY_BTN_INVITE]->ShowWindowWZ(WZ_HIDE);
		
		if(m_pTxtCtrls[COMMUNITY_TXT_REVENGE_POINT])
		{
			TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
			TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
			// 1631	리벤지 목록<포인트: %d>
			g_InterfaceManager.GetInterfaceString(1631, szTemp, INTERFACE_STRING_LENGTH);
			Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, GetManager()->m_byRevengePoint);

			m_pTxtCtrls[COMMUNITY_TXT_REVENGE_POINT]->ShowWindowWZ(WZ_SHOW);
			m_pTxtCtrls[COMMUNITY_TXT_REVENGE_POINT]->SetTextWZ(szMessage);
		}
		
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityDlg::_setModeMail(ENUM_STATEWZ eState)
{
	if (!_isEnable())
        return;

	CControlWZ* pControl = getControl(COMMUNITY_DIALOG_LST_MAIL);
    if (pControl)
    {
        pControl->ShowWindowWZ(eState);
    }

	pControl = getControl(COMMUNITY_DIALOG_BTN_DEL_MAIL);
    if (pControl)
    {
        pControl->ShowWindowWZ(eState);
    }

	pControl = getControl(COMMUNITY_DIALOG_BTN_DEL_ALL_MAIL);
    if (pControl)
    {
        pControl->ShowWindowWZ(eState);
    }

	pControl = getControl(COMMUNITY_DIALOG_TXT_SHOW_NEW_MAIL);
    if (pControl)
    {
        pControl->ShowWindowWZ(eState);
    }

	pControl = getControl(COMMUNITY_DIALOG_CHECK_SHOW_NEW_MAIL);
    if (pControl)
    {
        pControl->ShowWindowWZ(eState);
    }

	pControl = getControl(COMMUNITY_DIALOG_BTN_WRITE_MAIL);	// 차단 목록에선 쪽지쓰기 표시안함
	if (pControl)
	{
		pControl->ShowWindowWZ(eState);
	}

	if(eState == WZ_SHOW)
	{
		_hideOnOffIcons();

		if(m_pBtnCtrls[COMMUNITY_BTN_INVITE])
			m_pBtnCtrls[COMMUNITY_BTN_INVITE]->ShowWindowWZ(WZ_SHOW);


		if(m_pTxtCtrls[COMMUNITY_TXT_REVENGE_POINT])
		{
			m_pTxtCtrls[COMMUNITY_TXT_REVENGE_POINT]->ShowWindowWZ(WZ_HIDE);
		}
		TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
		if(m_pTxtCtrls[COMMUNITY_TXT_1])
		{
			m_pTxtCtrls[COMMUNITY_TXT_1]->ShowWindowWZ(WZ_SHOW);
			m_pTxtCtrls[COMMUNITY_TXT_1]->SetTextWZ(szTemp);
		}

		_refreshMailNum();

	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityDlg::_setModeGroup(ENUM_STATEWZ eState)
{
	if (!_isEnable())
        return;

	enum eDIALOG_POS eControls[] = 
	{
		COMMUNITY_DIALOG_LST_FRIEND,
		COMMUNITY_DIALOG_BTN_ADD_MEMBER,
		COMMUNITY_DIALOG_BTN_DEL_MEMBER,
		COMMUNITY_DIALOG_CHECK_REJECTION_GROUP,
		COMMUNITY_DIALOG_TXT_REJECTION_GROUPS,
	};
	int eControlsNum = sizeof(eControls)/sizeof(enum eDIALOG_POS);

	CControlWZ* pControl = NULL;

	for(int i = 0; i < eControlsNum; ++i)
	{
		pControl = getControl(eControls[i]);
		if (pControl)
		{
			pControl->ShowWindowWZ(eState);
		}
	}

	if(eState == WZ_SHOW)
	{
		_hideOnOffIcons();
		_hideMailIcons();

		TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
		TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};

		if(m_pTxtCtrls[COMMUNITY_TXT_REVENGE_POINT])
			m_pTxtCtrls[COMMUNITY_TXT_REVENGE_POINT]->ShowWindowWZ(WZ_HIDE);
		
		if(m_pTxtCtrls[COMMUNITY_TXT_2])
			m_pTxtCtrls[COMMUNITY_TXT_2]->ShowWindowWZ(WZ_HIDE);

		if(m_pTxtCtrls[COMMUNITY_TXT_1])
		{
			TCHAR channel_connect_string[INTERFACE_STRING_LENGTH] ={0,};
			//5797	%s서버 : %d채널 접속 중
			g_InterfaceManager.GetInterfaceString(5797, channel_connect_string ,INTERFACE_STRING_LENGTH);
			Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, channel_connect_string, GetManager()->GetMyServer(), GetManager()->GetMyChannel());
			m_pTxtCtrls[COMMUNITY_TXT_1]->ShowWindowWZ(WZ_SHOW);
			m_pTxtCtrls[COMMUNITY_TXT_1]->SetTextWZ(szTemp);
		}

		if(m_pBtnCtrls[COMMUNITY_BTN_INVITE])
			m_pBtnCtrls[COMMUNITY_BTN_INVITE]->ShowWindowWZ(WZ_HIDE);

        if(m_pTxtCtrls[COMMUNITY_TXT_3])
		{
			TCHAR szChannelConnect[INTERFACE_STRING_LENGTH] ={0,};
			// 접속자수....
            int total_group_member = GetManager()->m_GroupArray.Size();
            int connect_group_member = GetManager()->onlined_group_count();
            Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("(%d/%d)"), 
                connect_group_member, total_group_member);
	        Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, 15, 50);
			m_pTxtCtrls[COMMUNITY_TXT_3]->ShowWindowWZ(WZ_SHOW);
			m_pTxtCtrls[COMMUNITY_TXT_3]->SetTextWZ(szTemp);
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityDlg::_SortByType(int eModeType, int eSortType)
{
	bool bOrderBy = GetManager()->GetOrderBy(eModeType);
	int iSortReady = GetManager()->GetSortReady();
	
	if(iSortReady == eCOMSORT_BSAMETAB_SAMEBTN)// TODO: 그룹도 같고 버튼도 같을 때
	{
		bOrderBy = !bOrderBy;
		_clearSelection();
	}
	else if(iSortReady == eCOMSORT_BSAMETAB_OTHERBTN)
	{
		bOrderBy = false;
		_clearSelection();
	}
	GetManager()->SetSortReady(eCOMSORT_BNONE); // 그룹 갱신시 적용되지 않도록 꺼준다. (버튼을 누른경우만 true)가 될 수 있다.
	GetManager()->SetOrderBy(eModeType, bOrderBy);
	
	switch(eModeType)
	{
	case eCM_FRIEND:
		GetManager()->SortbyList(GetManager()->m_FriendArray, eSortType, bOrderBy);
		break;
	case eCM_BLOCK:
		GetManager()->SortbyList(GetManager()->m_BlockFriendArray, eSortType, bOrderBy);
		break;
	case eCM_REVENGE:
		GetManager()->SortbyList(GetManager()->m_RevengeArray, eSortType, bOrderBy);
		break;
	case eCM_MAIL:
		GetManager()->SortbyListMail(GetManager()->m_vecMailData, eSortType, bOrderBy);
		break;
	case eCM_GROUP:
		GetManager()->SortbyList(GetManager()->m_GroupArray, eSortType, bOrderBy);
		break;
	}
}					

//------------------------------------------------------------------------------
/**
*/
void uiCommunityDlg::_refreshMailNum()
{
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};

	Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("(%d/%d)"), GetManager()->GetCurMailNum(), MAX_MEMO_NUM);
	Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, 15, 50);
	if(m_pTxtCtrls[COMMUNITY_TXT_3])
	{
		m_pTxtCtrls[COMMUNITY_TXT_3]->ShowWindowWZ(WZ_SHOW);
		m_pTxtCtrls[COMMUNITY_TXT_3]->SetTextWZ(szMessage);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityDlg::_refreshOnOffIcon()
{
	int iGetMode = GetManager()->GetMode();
	if((iGetMode == eCM_REVENGE) || (iGetMode == eCM_FRIEND) || (iGetMode == eCM_GROUP))
	{
		if(m_pListCtrls[COMMUNITY_LST_FRIEND] && m_pScroll)
		{
			for(int i=0; i<SHOW_DEFAULT_FRIEND_LINE; ++i)
			{
				FRIEND_DATA* pData = (FRIEND_DATA*)m_pListCtrls[COMMUNITY_LST_FRIEND]->GetItemData(i);
				if(pData)
				{
					if(pData->bOnline)
					{
						_setOnline(i, pData->byFriendSts);
					}
					else
					{
						_setOffline(i, pData->byFriendSts);
					}
				}
				else
				{
					this->_hideOnOffIcon(i);
				}
			}
	
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityDlg::_refreshMailIcon()
{
	if(GetManager()->GetMode() == eCM_MAIL)
	{
		if(m_pListCtrls[COMMUNITY_LST_MAIL] && m_pScroll)
		{
			for(int i=0; i<SHOW_DEFAULT_FRIEND_LINE; ++i)
			{
				MAIL_DATA* pData = (MAIL_DATA*)m_pListCtrls[COMMUNITY_LST_MAIL]->GetItemData(i);
				if(pData)
				{
					if(pData->m_subject.m_byRead)
						this->_setReadMail(i);
					else
						this->_setNewMail(i);
				}
				else
				{
					this->_hideMailIcon(i);
				}
			}
		}
	}			
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityDlg::_ShowListIcons()
{
	for(int i= COMMUNITY_LIST_BTN_ID; i <= COMMUNITY_LIST_BTN_LOGON; ++i)
	{
		if(m_pBtnCtrls[i])
		{
			m_pBtnCtrls[i]->ShowWindowWZ(WZ_SHOW);
		}
	}
	for(int i= COMMUNITY_LIST_BTN_SENDER; i <= COMMUNITY_LIST_BTN_MEMO; ++i)
	{
		if(m_pBtnCtrls[i])
		{
			m_pBtnCtrls[i]->ShowWindowWZ(WZ_HIDE);
		}
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiCommunityDlg::_HideListIcons()
{

	for(int i= COMMUNITY_LIST_BTN_ID; i <= COMMUNITY_LIST_BTN_LOGON; ++i)
	{
		if(m_pBtnCtrls[i])
		{
			m_pBtnCtrls[i]->ShowWindowWZ(WZ_HIDE);
		}
	}
	for(int i= COMMUNITY_LIST_BTN_SENDER; i <= COMMUNITY_LIST_BTN_MEMO; ++i)
	{
		if(m_pBtnCtrls[i])
		{
			m_pBtnCtrls[i]->ShowWindowWZ(WZ_SHOW);
		}
	}

}

time_t uiCommunityDlg::GetLastTime(INT64 lastDate)
{
	tm stLastDate;
    SYSTEMTIME LASTTIME;
    time_t t_LastTime, t_NowTime, t_AccumTime;

    lastDate = lastDate * 100;//초시간을 맞추기 위해 100을 곱해준다.
    util::YYYYMMDDHHMMSSToSYSTEMTIME( lastDate, LASTTIME );//DWORD64를 LASTTIME으로 변경

    stLastDate.tm_year  = LASTTIME.wYear - 1900;
    stLastDate.tm_mon   = LASTTIME.wMonth - 1;
    stLastDate.tm_mday  = LASTTIME.wDay;
    stLastDate.tm_hour  = LASTTIME.wHour;
    stLastDate.tm_min   = LASTTIME.wMinute;
    stLastDate.tm_sec   = LASTTIME.wSecond;

    t_LastTime = mktime( &stLastDate );

    util::TimeSync::time( &t_NowTime );

    t_AccumTime = t_NowTime - t_LastTime;

	return t_AccumTime;    
}
//------------------------------------------------------------------------------
/**
*/
void uiCommunityDlg::_hideMailIcons()
{
	for(int i=0; i<MAX_ICON_NUM; ++i)
	{
		if(m_pNewBtn[i])
			m_pNewBtn[i]->ShowWindowWZ(WZ_HIDE);

		if(m_pReadBtn[i])
			m_pReadBtn[i]->ShowWindowWZ(WZ_HIDE);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityDlg::_hideOnOffIcons()
{
	for(int i=0; i<MAX_ICON_NUM; ++i)
	{
		if(m_pOnBtn[i])
			m_pOnBtn[i]->ShowWindowWZ(WZ_HIDE);

		if(m_pOffBtn[i])
			m_pOffBtn[i]->ShowWindowWZ(WZ_HIDE);

		if(m_pEvtOnBtn[i])
			m_pEvtOnBtn[i]->ShowWindowWZ(WZ_HIDE);

		if(m_pEvtOffBtn[i])
			m_pEvtOffBtn[i]->ShowWindowWZ(WZ_HIDE);
	}	
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityDlg::_setNewMail(int index)
{
	if(index >= MAX_ICON_NUM)
		return;

	if(m_pNewBtn[index])
		m_pNewBtn[index]->ShowWindowWZ(WZ_SHOW);

	if(m_pReadBtn[index])
		m_pReadBtn[index]->ShowWindowWZ(WZ_HIDE);
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityDlg::_setReadMail(int index)
{
	if(index >= MAX_ICON_NUM)
		return;

	if(m_pNewBtn[index])
		m_pNewBtn[index]->ShowWindowWZ(WZ_HIDE);

	if(m_pReadBtn[index])
		m_pReadBtn[index]->ShowWindowWZ(WZ_SHOW);
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityDlg::_setOnline(int index, BYTE byFriendSts)
{
	if(index >= MAX_ICON_NUM)
		return;

    if (byFriendSts == STRUCT_FRIEND_DB_INFO::INFO_INVIAT 
        || byFriendSts == STRUCT_FRIEND_DB_INFO::INFO_BEINVIATED)// 친구초대이벤트 친구
	{
		if (m_pOffBtn[index])
		{
			m_pOffBtn[index]->ShowWindowWZ(WZ_HIDE);
		}
		if (m_pOnBtn[index])
		{
			m_pOnBtn[index]->ShowWindowWZ(WZ_HIDE);
		}
		if (m_pEvtOffBtn[index])
		{
			m_pEvtOffBtn[index]->ShowWindowWZ(WZ_HIDE);
		}
		if (m_pEvtOnBtn[index])
		{
			m_pEvtOnBtn[index]->ShowWindowWZ(WZ_SHOW); // SHOW
		}
	}
	else// 일반 친구
	{
		if (m_pOffBtn[index])
		{
			m_pOffBtn[index]->ShowWindowWZ(WZ_HIDE);
		}
		if (m_pOnBtn[index])
		{
			m_pOnBtn[index]->ShowWindowWZ(WZ_SHOW);	// SHOW
		}
		if (m_pEvtOffBtn[index])
		{
			m_pEvtOffBtn[index]->ShowWindowWZ(WZ_HIDE);
		}
		if (m_pEvtOnBtn[index])
		{
			m_pEvtOnBtn[index]->ShowWindowWZ(WZ_HIDE);
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityDlg::_setOffline(int index, BYTE byFriendSts)
{
	if(index >= MAX_ICON_NUM)
		return;

    if (byFriendSts == STRUCT_FRIEND_DB_INFO::INFO_INVIAT 
    || byFriendSts == STRUCT_FRIEND_DB_INFO::INFO_BEINVIATED)	// 친구초대이벤트 친구
	{
		if (m_pOffBtn[index])
		{
			m_pOffBtn[index]->ShowWindowWZ(WZ_HIDE);
		}
		if (m_pOnBtn[index])
		{
			m_pOnBtn[index]->ShowWindowWZ(WZ_HIDE);
		}
		if (m_pEvtOffBtn[index])
		{
			m_pEvtOffBtn[index]->ShowWindowWZ(WZ_SHOW);	// SHOW
		}
		if (m_pEvtOnBtn[index])
		{
			m_pEvtOnBtn[index]->ShowWindowWZ(WZ_HIDE);
		}
	}
	else//일반 친구
	{
		if (m_pOffBtn[index])
		{
			m_pOffBtn[index]->ShowWindowWZ(WZ_SHOW);	// SHOW
		}
		if (m_pOnBtn[index])
		{
			m_pOnBtn[index]->ShowWindowWZ(WZ_HIDE);
		}
		if (m_pEvtOffBtn[index])
		{
			m_pEvtOffBtn[index]->ShowWindowWZ(WZ_HIDE);
		}
		if (m_pEvtOnBtn[index])
		{
			m_pEvtOnBtn[index]->ShowWindowWZ(WZ_HIDE);
		}
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiCommunityDlg::_hideMailIcon(int index)
{
	if(index >= MAX_ICON_NUM)
		return;

	if(m_pNewBtn[index])
		m_pNewBtn[index]->ShowWindowWZ(WZ_HIDE);

	if(m_pReadBtn[index])
		m_pReadBtn[index]->ShowWindowWZ(WZ_HIDE);
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityDlg::_hideOnOffIcon(int index)
{
	if(index >= MAX_ICON_NUM)
		return;

	if(m_pOnBtn[index])
		m_pOnBtn[index]->ShowWindowWZ(WZ_HIDE);

	if(m_pOffBtn[index])
		m_pOffBtn[index]->ShowWindowWZ(WZ_HIDE);

	if(m_pEvtOnBtn[index])
		m_pEvtOnBtn[index]->ShowWindowWZ(WZ_HIDE);

	if(m_pEvtOffBtn[index])
		m_pEvtOffBtn[index]->ShowWindowWZ(WZ_HIDE);
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityDlg::_updateCheckState()
{
	if(!GetManager())
		return;

	if(GetManager()->GetMode() == eCM_MAIL)
	{
#ifdef _NA_000000_20130114_RENEWER_UI
#else
		if(m_pBtnCheckTabs[COMMUNITY_TAB_FRIEND])
		{
			m_pBtnCheckTabs[COMMUNITY_TAB_FRIEND]->SetCheckState(false);
		}
#endif
		if(m_pBtnCheckTabs[COMMUNITY_TAB_MAIL])
		{
			m_pBtnCheckTabs[COMMUNITY_TAB_MAIL]->SetCheckState(true);
		}

		if(m_pBtnChecks[COMMUNITY_CHECK_SHOW_NEW_MAIL])
		{
			m_pBtnChecks[COMMUNITY_CHECK_SHOW_NEW_MAIL]->SetCheckState(GetManager()->GetDisplayNewMail());
		}

		if(m_pBtnCheckTabs[COMMUNITY_TAB_GROUP])
		{
			m_pBtnCheckTabs[COMMUNITY_TAB_GROUP]->SetCheckState(false);
		}
#ifdef _NA_000000_20130114_RENEWER_UI
		if(m_pBtnCheckTabs[COMMUNITY_TAB_SUB_BLOCK])
		{
			m_pBtnCheckTabs[COMMUNITY_TAB_SUB_BLOCK]->SetCheckState(false);
		}

		if(m_pBtnCheckTabs[COMMUNITY_TAB_SUB_REVENGE])
		{
			m_pBtnCheckTabs[COMMUNITY_TAB_SUB_REVENGE]->SetCheckState(false);
		}

		if(m_pBtnCheckTabs[COMMUNITY_TAB_SUB_FRIEND])
		{
			m_pBtnCheckTabs[COMMUNITY_TAB_SUB_FRIEND]->SetCheckState(false);
		}
#endif//#ifdef _NA_000000_20130114_RENEWER_UI
	}
	else if(GetManager()->GetMode() == eCM_GROUP)
	{
#ifdef _NA_000000_20130114_RENEWER_UI
#else
		if(m_pBtnCheckTabs[COMMUNITY_TAB_FRIEND])
		{
			m_pBtnCheckTabs[COMMUNITY_TAB_FRIEND]->SetCheckState(false);
		}
#endif//#ifdef _NA_000000_20130114_RENEWER_UI

		if(m_pBtnCheckTabs[COMMUNITY_TAB_MAIL])
		{
			m_pBtnCheckTabs[COMMUNITY_TAB_MAIL]->SetCheckState(false);
		}

		if(m_pBtnCheckTabs[COMMUNITY_TAB_GROUP])
		{
			m_pBtnCheckTabs[COMMUNITY_TAB_GROUP]->SetCheckState(true);
		}

		if(m_pBtnCheckTabs[COMMUNITY_TAB_MAIL])
		{
			m_pBtnCheckTabs[COMMUNITY_TAB_MAIL]->SetCheckState(false);
		}
#ifdef _NA_000000_20130114_RENEWER_UI
		if(m_pBtnCheckTabs[COMMUNITY_TAB_SUB_BLOCK])
		{
			m_pBtnCheckTabs[COMMUNITY_TAB_SUB_BLOCK]->SetCheckState(false);
		}

		if(m_pBtnCheckTabs[COMMUNITY_TAB_SUB_REVENGE])
		{
			m_pBtnCheckTabs[COMMUNITY_TAB_SUB_REVENGE]->SetCheckState(false);
		}
		if(m_pBtnCheckTabs[COMMUNITY_TAB_SUB_FRIEND])
		{
			m_pBtnCheckTabs[COMMUNITY_TAB_SUB_FRIEND]->SetCheckState(false);
		}
#endif//#ifdef _NA_000000_20130114_RENEWER_UI
	}
	else
	{
		if(GetManager()->GetMode() == eCM_FRIEND)
		{
#ifdef _NA_000000_20130114_RENEWER_UI
#else
			if(m_pBtnCheckTabs[COMMUNITY_TAB_FRIEND])
			{
				m_pBtnCheckTabs[COMMUNITY_TAB_FRIEND]->SetCheckState(true);
			}
#endif//#ifdef _NA_000000_20130114_RENEWER_UI
			if(m_pBtnCheckTabs[COMMUNITY_TAB_MAIL])
			{
				m_pBtnCheckTabs[COMMUNITY_TAB_MAIL]->SetCheckState(false);
			}

			if(m_pBtnCheckTabs[COMMUNITY_TAB_SUB_FRIEND])
			{
				m_pBtnCheckTabs[COMMUNITY_TAB_SUB_FRIEND]->SetCheckState(true);
			}

			if(m_pBtnCheckTabs[COMMUNITY_TAB_SUB_BLOCK])
			{
				m_pBtnCheckTabs[COMMUNITY_TAB_SUB_BLOCK]->SetCheckState(false);
			}

			if(m_pBtnCheckTabs[COMMUNITY_TAB_SUB_REVENGE])
			{
				m_pBtnCheckTabs[COMMUNITY_TAB_SUB_REVENGE]->SetCheckState(false);
			}
		}
		else if(GetManager()->GetMode() == eCM_BLOCK)
		{
#ifdef _NA_000000_20130114_RENEWER_UI
#else
			if(m_pBtnCheckTabs[COMMUNITY_TAB_FRIEND])
			{
				m_pBtnCheckTabs[COMMUNITY_TAB_FRIEND]->SetCheckState(true);
			}
#endif//#ifdef _NA_000000_20130114_RENEWER_U

			if(m_pBtnCheckTabs[COMMUNITY_TAB_MAIL])
			{
				m_pBtnCheckTabs[COMMUNITY_TAB_MAIL]->SetCheckState(false);
			}

			if(m_pBtnCheckTabs[COMMUNITY_TAB_SUB_FRIEND])
			{
				m_pBtnCheckTabs[COMMUNITY_TAB_SUB_FRIEND]->SetCheckState(false);
			}

			if(m_pBtnCheckTabs[COMMUNITY_TAB_SUB_BLOCK])
			{
				m_pBtnCheckTabs[COMMUNITY_TAB_SUB_BLOCK]->SetCheckState(true);
			}

			if(m_pBtnCheckTabs[COMMUNITY_TAB_SUB_REVENGE])
			{
				m_pBtnCheckTabs[COMMUNITY_TAB_SUB_REVENGE]->SetCheckState(false);
			}
		}
		else if(GetManager()->GetMode() == eCM_REVENGE)
		{
#ifdef _NA_000000_20130114_RENEWER_UI
#else
			if(m_pBtnCheckTabs[COMMUNITY_TAB_FRIEND])
			{
				m_pBtnCheckTabs[COMMUNITY_TAB_FRIEND]->SetCheckState(true);
			}
#endif//#ifdef _NA_000000_20130114_RENEWER_UI

			if(m_pBtnCheckTabs[COMMUNITY_TAB_MAIL])
			{
				m_pBtnCheckTabs[COMMUNITY_TAB_MAIL]->SetCheckState(false);
			}

			if(m_pBtnCheckTabs[COMMUNITY_TAB_SUB_FRIEND])
			{
				m_pBtnCheckTabs[COMMUNITY_TAB_SUB_FRIEND]->SetCheckState(false);
			}

			if(m_pBtnCheckTabs[COMMUNITY_TAB_SUB_BLOCK])
			{
				m_pBtnCheckTabs[COMMUNITY_TAB_SUB_BLOCK]->SetCheckState(false);
			}

			if(m_pBtnCheckTabs[COMMUNITY_TAB_SUB_REVENGE])
			{
				m_pBtnCheckTabs[COMMUNITY_TAB_SUB_REVENGE]->SetCheckState(true);
			}
		}

		if(m_pBtnChecks[COMMUNITY_CHECK_REJECTION_CHAT])
		{
			m_pBtnChecks[COMMUNITY_CHECK_REJECTION_CHAT]->SetCheckState(GetManager()->GetRejectChat());
		}

		if(m_pBtnChecks[COMMUNITY_CHECK_HIDE_OFFLINE])
		{
			m_pBtnChecks[COMMUNITY_CHECK_HIDE_OFFLINE]->SetCheckState(GetManager()->GetNotDisplayOffLine());
		}

		if(m_pBtnCheckTabs[COMMUNITY_TAB_GROUP])
		{
			m_pBtnCheckTabs[COMMUNITY_TAB_GROUP]->SetCheckState(false);
		}

	}
	
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityDlg::_clearSelection()
{
	if(GetManager())
	{
		GetManager()->ClearSelection();	
		if(m_pListCtrls[COMMUNITY_LST_FRIEND])	// 친구와 그룹 리스트 같이 사용
				m_pListCtrls[COMMUNITY_LST_FRIEND]->SetCurSel(-1);
		if(m_pListCtrls[COMMUNITY_LST_MAIL])
				m_pListCtrls[COMMUNITY_LST_MAIL]->SetCurSel(-1);

	}
}

//------------------------------------------------------------------------------
/**
*/
FRIEND_DATA * uiCommunityDlg::GetCurData(int iSelect)
{
	if(m_pListCtrls[COMMUNITY_LST_FRIEND])
	{
		return (FRIEND_DATA*)m_pListCtrls[COMMUNITY_LST_FRIEND]->GetItemData(iSelect);
	}

	return NULL;
}

//------------------------------------------------------------------------------
/**
*/
MAIL_DATA *	uiCommunityDlg::GetCurMailData(int iSelect)
{
	if(m_pListCtrls[COMMUNITY_LST_MAIL])
	{
		return (MAIL_DATA*)m_pListCtrls[COMMUNITY_LST_MAIL]->GetItemData(iSelect);
	}

	return NULL;
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityDlg::ChangeMode(int iMode, bool bForce /* = false*/)
{
	if (!GetManager())
    {
		return;
    }

	if (!bForce)
	{
		if (GetManager()->GetMode() == iMode)
        {
			return;
        }
	}

    GetManager()->SetMode(iMode);
    if (m_pScroll)
    {
        m_pScroll->SetScrollPosWZ(0);
    }
	
	_ShowListIcons();

	switch(iMode)
	{
	case eCM_FRIEND:
		{
			_setModeGroup(WZ_HIDE);
			_setModeMail(WZ_HIDE);
			_setModeBlock(WZ_HIDE);
			_setModeRevenge(WZ_HIDE);
			_setModeFriend(WZ_SHOW);

			RefreshFriendList();
		}
		break;

	case eCM_BLOCK:
		{
			_setModeGroup(WZ_HIDE);
			_setModeMail(WZ_HIDE);
			_setModeFriend(WZ_HIDE);
			_setModeRevenge(WZ_HIDE);
			_setModeBlock(WZ_SHOW);

			RefreshFriendList();

		}
		break;

	case eCM_REVENGE:
		{
			_setModeGroup(WZ_HIDE);
			_setModeMail(WZ_HIDE);
			_setModeFriend(WZ_HIDE);
			_setModeBlock(WZ_HIDE);
			_setModeRevenge(WZ_SHOW);

			RefreshFriendList();

		}
		break;

	case eCM_MAIL:
		{
			_HideListIcons();
			_setModeGroup(WZ_HIDE);
#ifdef _NA_000000_20130114_RENEWER_UI
			_setDefaultFriend(WZ_HIDE,false);
#else
			_setDefaultFriend(WZ_HIDE);
#endif//#ifdef _NA_000000_20130114_RENEWER_UI
			_setModeMail(WZ_SHOW);

			RefreshMailList();
		}
		break;

	case eCM_GROUP:
		{
#ifdef _NA_000000_20130114_RENEWER_UI
			_setDefaultFriend(WZ_HIDE,false);
#else
			_setDefaultFriend(WZ_HIDE);
#endif//#ifdef _NA_000000_20130114_RENEWER_UI
			_setModeGroup(WZ_SHOW);
			_setModeMail(WZ_HIDE);

			RefreshFriendList();	// Group랑 Friend랑 메소드 같이 사용
		}
		break;

	}
	_clearSelection();

    GetManager()->set_last_community_mode((eCOMMUNITY_MODE)(GetManager()->GetMode()));
    if (GetManager()->last_community_mode() <= eCM_REVENGE)
    {
        GetManager()->set_last_community_friend_subject_mode(GetManager()->last_community_mode());
    }
    if (m_pScroll)
    {
        m_pScroll->SetScrollPosWZ(GetManager()->last_scrollbar_position((eCOMMUNITY_MODE)GetManager()->GetMode()));
    }
}

//------------------------------------------------------------------------------
/**
*/
int	uiCommunityDlg::GetCurListSel()
{
	if(!GetManager())
		return -1;

	int iCurSel = -1;

	if(GetManager()->GetMode() == eCM_MAIL)
	{
		if(m_pListCtrls[COMMUNITY_LST_MAIL])
			iCurSel = m_pListCtrls[COMMUNITY_LST_MAIL]->GetCurSel();
	}
	else
	{
		if(m_pListCtrls[COMMUNITY_LST_FRIEND])
			iCurSel = m_pListCtrls[COMMUNITY_LST_FRIEND]->GetCurSel();
	}

	return iCurSel;
}

//=====================================================================================
//	1.리스트컨트롤 데이터삽입
//=====================================================================================
void uiCommunityDlg::_SettListCtrlData(COMMUNITY_LSTS eType,int nIndex,int nSubIndex,TCHAR* pszText,BOOL bOnLine, BYTE byMaster)
{
	DWORD nTextStyle = DT_VCENTER | DT_LEFT;	// 기본값
	if(nSubIndex == 0 || (nSubIndex != 1 && GetManager()->GetMode() == eCM_MAIL))
	{
		nTextStyle = DT_VCENTER | DT_LEFT;
	}
	else
	{
		nTextStyle = DT_VCENTER | DT_CENTER;
	}
		

	if(bOnLine)
	{
		GlobalFunc::SetItem(m_pListCtrls[eType], nIndex, nSubIndex,
			pszText,
			c_wiCommunityFont,
			nTextStyle,
			RGBA(255,255,255,0), 
			RGBA(255,255,255,255), 
			RGBA(255,255,255,255));
	}
	else
	{
		GlobalFunc::SetItem(m_pListCtrls[eType], nIndex, nSubIndex,
			pszText,
			c_wiCommunityFont,
			nTextStyle,
			RGBA(255,255,255,0), 
			RGBA(128,128,128,255), 
			RGBA(128,128,128,255));

	}

	if(GetManager()->GetMode() == eCM_GROUP &&
		byMaster == GROUP_LEVEL_HIGH)
	{
		if(bOnLine)
		{
			GlobalFunc::SetItem(m_pListCtrls[eType], nIndex, nSubIndex,
			pszText,
			c_wiCommunityFont,
			nTextStyle,
			RGBA(255,255,255,0), 
			RGBA(255,255,102,255), 
			RGBA(255,255,102,255));
		}
		else
		{
			GlobalFunc::SetItem(m_pListCtrls[eType], nIndex, nSubIndex,
			pszText,
			c_wiCommunityFont,
			nTextStyle,
			RGBA(255,255,255,0), 
			RGBA(192,192,102,255), 
			RGBA(192,192,102,255));
		}
	}
}
//=====================================================================================
//	2.컨트롤러 정보입력
//=====================================================================================
void uiCommunityDlg::_RefreshControl(nArray<FRIEND_DATA> &ArrayType)
{
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp2[INTERFACE_STRING_LENGTH] = {0,};

	//0.리스트 목록정보삽입
	m_pListCtrls[COMMUNITY_LST_FRIEND]->SetSellHeight(c_iSellHeight);
	m_pListCtrls[COMMUNITY_LST_FRIEND]->SetUseReport(true);
	m_pListCtrls[COMMUNITY_LST_FRIEND]->SetReportUseCnt(FRIEND_LIST_MAX);
	m_pListCtrls[COMMUNITY_LST_FRIEND]->SetHideReportRender(true);
	m_pListCtrls[COMMUNITY_LST_FRIEND]->SetFontID(c_wiCommunityFont);

    GlobalFunc::SetReportButtonInfo( m_pListCtrls[COMMUNITY_LST_FRIEND], FRIEND_LIST_NAME, friend_list_column_[FRIEND_LIST_NAME], _T("name"));
    GlobalFunc::SetReportButtonInfo( m_pListCtrls[COMMUNITY_LST_FRIEND], FRIEND_LIST_LEVEL, friend_list_column_[FRIEND_LIST_LEVEL], _T("lv"));	
    GlobalFunc::SetReportButtonInfo( m_pListCtrls[COMMUNITY_LST_FRIEND], FRIEND_LIST_CLASS, friend_list_column_[FRIEND_LIST_CLASS], _T("class"));
    GlobalFunc::SetReportButtonInfo( m_pListCtrls[COMMUNITY_LST_FRIEND], FRIEND_LIST_CHANNEL, friend_list_column_[FRIEND_LIST_CHANNEL], _T("channel"));

	//1.ON.OffLine시 친구목록 카운팅
	int iFriendArrayCnt=ArrayType.Size();
	if(GetManager()->GetNotDisplayOffLine())
	{	
		iFriendArrayCnt=0;
		for(int i=0;i<ArrayType.Size();++i)
		{
			if(ArrayType[i].bOnline)
				++iFriendArrayCnt;
		}
	}
	//2.친구목록수를 토대로 페이지계산을한다(1라인씩끊는다)
	int iPage=iFriendArrayCnt-SHOW_DEFAULT_FRIEND_LINE;
	int iScrollRange = 1;

	if(iPage<=0) iPage=0; 

	m_pScroll->SetScrollRangeWZ(0,iPage);

	int iAdd = 0;
	int iOnlineCount=0;
	//3.목록전체를 루프하면서 데이터를삽입한다.
	for(int iLine=0;iLine<ArrayType.Size();++iLine)
	{
		//3_1.오프라인모드이고,친구비접속상태일때 SKIP
		if(GetManager()->GetNotDisplayOffLine() && (GetManager()->GetMode() != eCM_GROUP)) // 그룹 맺기는 오프라인 표시 안함 적용하지 않음
		{
			if(!(ArrayType[iLine].bOnline))
				continue;
			else
				++iOnlineCount;

		}
		else
		{
			++iOnlineCount;
		}

		//3_2.삽입된사람이 SHOW_DEFAULT_FRIEND_LINE 넘으면 스탑!
		if(iAdd>=SHOW_DEFAULT_FRIEND_LINE)
			break;

		//3_3.시작포인트를잡는다
		if(GetManager()->GetNotDisplayOffLine())
		{
			if(iOnlineCount <= (m_pScroll->GetScrollPosWZ() * iScrollRange))
				continue;
		}
		else
		{
			if(iLine < (m_pScroll->GetScrollPosWZ() * iScrollRange))
				continue;
		}

		int index = m_pListCtrls[COMMUNITY_LST_FRIEND]->InsertItemBack();
		m_pListCtrls[COMMUNITY_LST_FRIEND]->SetItemData(index, &(ArrayType[iLine]));
		++iAdd;

		//3_4.이부분에서 셋팅을해준다
		for(int iCnt=0; iCnt<FRIEND_LIST_MAX; ++iCnt)
		{
			switch(iCnt)
			{
			case FRIEND_LIST_NAME://아이디
				{
					TCHAR *pName=(TCHAR *)ArrayType[iLine].name.Get();
					Snprintf(szTemp,INTERFACE_STRING_LENGTH-1,_T("%s"),pName);
				}
				break;

			case FRIEND_LIST_LEVEL://레벨
				{
					Snprintf(szTemp,INTERFACE_STRING_LENGTH-1,_T("Lv:%d"),ArrayType[iLine].lv);
				}
				break;

			case FRIEND_LIST_CLASS://클래스
				{
					Snprintf(szTemp, 
                        INTERFACE_STRING_LENGTH-1, 
                        _T("%s"), 
                        GlobalFunc::GetClassString(static_cast<eCHAR_TYPE>(ArrayType[iLine].bClassCode)));
				}
				break;

			case FRIEND_LIST_CHANNEL://채널
				{
					TCHAR szChannel[INTERFACE_STRING_LENGTH] ={0,};
					ZeroMemory(szTemp, sizeof(szTemp));
					// 5783	%d채널
					g_InterfaceManager.GetInterfaceString( 5783	 , szChannel ,INTERFACE_STRING_LENGTH);
					if(ArrayType[iLine].bOnline)
						Snprintf(szTemp,INTERFACE_STRING_LENGTH-1, szChannel , ArrayType[iLine].byChannel);
				}
				break;
				case FRIEND_LIST_ONLINE://접속정보
				{
					TCHAR szChannel[INTERFACE_STRING_LENGTH] ={0,};
					ZeroMemory(szTemp, sizeof(szTemp));
					
					if(ArrayType[iLine].bOnline)
					{
						// 70500: 접속중
						g_InterfaceManager.GetInterfaceString( 70500, szTemp ,INTERFACE_STRING_LENGTH);
					}
					else
					{
						if(GetManager()->GetMode() == eCM_GROUP)
						{
							time_t t_AccumTime = GetLastTime(ArrayType[iLine].lastDate);
							TCHAR szTime[INTERFACE_STRING_LENGTH] ={0,};
							ZeroMemory(szTemp, sizeof(szTemp));
							
							if(t_AccumTime < 3600)
							{
								// 1928: %d분전
								g_InterfaceManager.GetInterfaceString( 1928, szTime ,INTERFACE_STRING_LENGTH);
								Snprintf(szTemp,INTERFACE_STRING_LENGTH-1, szTime , (t_AccumTime / 60));
							}
							else if(t_AccumTime < 86400) // 3600 * 24 = 86400
							{
								// 1929: %d시간전
								g_InterfaceManager.GetInterfaceString( 1929, szTime ,INTERFACE_STRING_LENGTH);
								Snprintf(szTemp,INTERFACE_STRING_LENGTH-1, szTime , (t_AccumTime / 3600));
							}
							else
							{
								// 1930: %d일전
								g_InterfaceManager.GetInterfaceString( 1930, szTime ,INTERFACE_STRING_LENGTH);
								Snprintf(szTemp,INTERFACE_STRING_LENGTH-1, szTime , (t_AccumTime / 86400));
							}
						}
						else
						{
							// 70501: 접속안함
							g_InterfaceManager.GetInterfaceString( 70501, szTemp ,INTERFACE_STRING_LENGTH);
						}
					}
				}
				break;
			}
			_SettListCtrlData(COMMUNITY_LST_FRIEND,index, iCnt,szTemp,ArrayType[iLine].bOnline, ArrayType[iLine].byMaster);
		}
	}
}

//=====================================================================================
//	3.쪽지함refresh
//=====================================================================================
void uiCommunityDlg::_RefreshMailControl()
{
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
	
	//1.메일수체크	
	int iMailCnt = GetManager()->m_vecMailData.size();
	if(GetManager()->GetDisplayNewMail())
	{
		iMailCnt =0;
		for(unsigned int i=0;i<GetManager()->m_vecMailData.size();++i)
		{	//안읽은메일 모드시에는 체크한다
			if(GetManager()->m_vecMailData[i].m_subject.m_byRead == 0)
				++iMailCnt;
		}
	}

	//2.메일을토대로 페이지계산
	int iPage=(iMailCnt-SHOW_DEFAULT_FRIEND_LINE);
	int iScrollRange = 1;

	if(iPage<=0) iPage=0; 
	m_pScroll->SetScrollRangeWZ(0,iPage);

	//3.루프돌며 데이터삽입
	int iAdd = 0;
	int iOnlineCount=0;
	for(unsigned int iLine=0; iLine<GetManager()->m_vecMailData.size(); ++iLine)
	{
		if(GetManager()->GetDisplayNewMail())
		{
			if(GetManager()->m_vecMailData[iLine].m_subject.m_byRead==1)
				continue;
			else
				++iOnlineCount;
		}

		//3_1.삽입된메일이 SHOW_DEFAULT_FRIEND_LINE 넘으면 스탑!
		if(iAdd>=SHOW_DEFAULT_FRIEND_LINE)
			break;
		
		//3_2.시작페이지를 계산한다
		if(GetManager()->GetDisplayNewMail())
		{
			if(iOnlineCount <= (m_pScroll->GetScrollPosWZ() * iScrollRange))
				continue;
		}
		else
		{
			if(iLine < (unsigned int)(m_pScroll->GetScrollPosWZ() * iScrollRange))
				continue;
		}
		

		//3_3,정보얻기
		int index = m_pListCtrls[COMMUNITY_LST_MAIL]->InsertItemBack();
		m_pListCtrls[COMMUNITY_LST_MAIL]->SetItemData(index, &(GetManager()->m_vecMailData[iLine]));
		++iAdd;

		//3_4.실제적으로 셋팅한다
		for(int iCnt=0; iCnt<MAIL_LIST_MAX; ++iCnt)
		{
			ZeroMemory(szTemp, sizeof(szTemp));

			switch(iCnt)
			{
			case MAIL_LIST_NAME:
				{
					if(GetManager()->m_vecMailData[iLine].m_subject.m_MemoType==STRUCT_SUBJECT_PACKETINFO::SYSTEM_MEMO)
					{
						char *pStr = GetManager()->m_vecMailData[iLine].m_subject.m_ptszSubject;

						//0번째인덱스는 어떤종류의 시스템메시지인지 나타내는값이다(서버와의 규약)
						int nSystemType = (int)GetManager()->ParseMemoInfo(pStr,0,MAX_SUBJECT_STRLENGTH-1);

						switch(nSystemType)
						{
						case STRUCT_SUBJECT_PACKETINFO::LOTTO_TYPE://하임로또일경우:발신자명을 맵핑시켜준다.
                        case STRUCT_SUBJECT_PACKETINFO::LOTTO_TYPE_NEW://하임로또일경우:발신자명을 맵핑시켜준다.
							{
								//1885:하임행운
								g_InterfaceManager.GetInterfaceString(1885,szTemp);
								StrnCopy(GetManager()->m_vecMailData[iLine].m_subject.m_SendName,szTemp, MAX_CHARNAME_LENGTH);
							}
							break;

						case STRUCT_SUBJECT_PACKETINFO::GROUP_TYPE ://그룹일경우:발신자명을 맵핑시켜준다.
							{
	
								//314: 그룹관리자
								g_InterfaceManager.GetInterfaceString(314,szTemp);
								StrnCopy(GetManager()->m_vecMailData[iLine].m_subject.m_SendName,szTemp, MAX_CHARNAME_LENGTH);
							}
							break;
						case STRUCT_SUBJECT_PACKETINFO::DOMINATION_TYPE_RETURN:
						case STRUCT_SUBJECT_PACKETINFO::DOMINATION_TYPE_REWARD:
                        case STRUCT_SUBJECT_PACKETINFO::DOMINATION_TYPE_RETURN_NEW:
                        case STRUCT_SUBJECT_PACKETINFO::DOMINATION_TYPE_REWARD_NEW:
							{
								// 발신자맵핑 3377 피의성전 관리자
								g_InterfaceManager.GetInterfaceString(3377,szTemp);
								StrnCopy(GetManager()->m_vecMailData[iLine].m_subject.m_SendName,szTemp, MAX_CHARNAME_LENGTH);
							}
							break;
#if defined(_NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR)
                        case STRUCT_SUBJECT_PACKETINFO::WORLDRANKING_TYPE_REWARD_ALARAM:
                            {
                                //5478 월드랭킹
                                g_InterfaceManager.GetInterfaceString(5478,szTemp);                                
                                StrnCopy(GetManager()->m_vecMailData[iLine].m_subject.m_SendName, szTemp, MAX_CHARNAME_LENGTH);
                            }
                            break;
#endif // _NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
                        case STRUCT_SUBJECT_PACKETINFO::GUILD_JOIN_REQUEST:
                            {
                                // 1444 : 길드 관리인
                                g_InterfaceManager.GetInterfaceString(1444, szTemp);
                                StrnCopy(GetManager()->m_vecMailData[iLine].m_subject.m_SendName, szTemp, MAX_CHARNAME_LENGTH);
                            }
                            break;
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM
						case STRUCT_SUBJECT_PACKETINFO::SUN_RANKING_SETTLE_RESULT_TYPE:
							{
								// 3599 : SUN 랭킹
								g_InterfaceManager.GetInterfaceString(3599, szTemp);
								StrnCopy(GetManager()->m_vecMailData[iLine].m_subject.m_SendName, szTemp, MAX_CHARNAME_LENGTH);
							}
							break;
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM
						default:
							break;
						}
					}
					StrnCopy(szTemp, GetManager()->m_vecMailData[iLine].m_subject.m_SendName, MAX_CHARNAME_LENGTH);
				}
				break;
			case MAIL_LIST_DATE:StrnCopy(szTemp, GetManager()->m_vecMailData[iLine].m_subject.m_tszCreateDate, MAX_DATE_SIZE);break;
			case MAIL_LIST_MEMO:
				{
					if(GetManager()->m_vecMailData[iLine].m_subject.m_MemoType==STRUCT_SUBJECT_PACKETINFO::SYSTEM_MEMO)
					{
						char *pStr = GetManager()->m_vecMailData[iLine].m_subject.m_ptszSubject;
						 
						//0번째인덱스는 어떤종류의 시스템메시지인지 나타내는값이다(서버와의 규약)
						int nSystemType = (int)GetManager()->ParseMemoInfo(pStr,0,MAX_SUBJECT_STRLENGTH-1);

						switch(nSystemType)
						{
						case STRUCT_SUBJECT_PACKETINFO::LOTTO_TYPE://로또타입
                        case STRUCT_SUBJECT_PACKETINFO::LOTTO_TYPE_NEW:
							{
								//5285:로또당첨안내입니다.
								g_InterfaceManager.GetInterfaceString(5285,szTemp);
							}
							break;

						case STRUCT_SUBJECT_PACKETINFO::GROUP_TYPE://그룹누적경험치 타입
							{	
								//315 그룹관리자 안내메시지
								g_InterfaceManager.GetInterfaceString(315,szTemp);
							}
							break;

						case STRUCT_SUBJECT_PACKETINFO::FRINVATE_START_TYPE://친구초대한 플레이어가 캐릭터를 만들어서 접속
						case STRUCT_SUBJECT_PACKETINFO::FRBEINVATED_START_TYPE://초대된 플레이어가 캐릭터를 만들어서 접속
							{
								//6082 친구로 등록되었습니다.
								g_InterfaceManager.GetInterfaceString(6082,szTemp);
							}
							break;
						case STRUCT_SUBJECT_PACKETINFO::FRINVATE_END_TYPE://친구초대한 캐릭터가 80레벨이 되어 만료
						case STRUCT_SUBJECT_PACKETINFO::FRINVATE_PRESENT_TYPE://친구초대한 캐릭터가 20/40/60레벨이 되었을 때
						case STRUCT_SUBJECT_PACKETINFO::FRBEINVATED_END_TYPE://초대된 플레이어가 80레벨이 되어 만료
						case STRUCT_SUBJECT_PACKETINFO::FRBEINVATED_PRESENT_TYPE://초대된 캐릭터가 20/40/60레벨이 되었을 때
							{
								//6083 이벤트 선물상자를 받아가시기 바랍니다.
								g_InterfaceManager.GetInterfaceString(6083,szTemp);
							}
							break;							
							// 쪽지지 미리보기 맵핑
						case STRUCT_SUBJECT_PACKETINFO::DOMINATION_TYPE_RETURN:
                        case STRUCT_SUBJECT_PACKETINFO::DOMINATION_TYPE_RETURN_NEW:
							{
								g_InterfaceManager.GetInterfaceString(3379,szTemp);
							}
							break;
						case STRUCT_SUBJECT_PACKETINFO::DOMINATION_TYPE_REWARD:
                        case STRUCT_SUBJECT_PACKETINFO::DOMINATION_TYPE_REWARD_NEW:
							{
								g_InterfaceManager.GetInterfaceString(3380,szTemp);
							}
							break;
#if defined(_NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR)
                        case STRUCT_SUBJECT_PACKETINFO::WORLDRANKING_TYPE_REWARD_ALARAM:
                            {
                                // 5479 월드랭킹 당첨 안내 쪽지입니다.
                                g_InterfaceManager.GetInterfaceString(5479, szTemp);
                            }
                            break;
#endif // _NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
                        case STRUCT_SUBJECT_PACKETINFO::GUILD_JOIN_REQUEST:
                            {
                                // 1444 : 길드 관리인
                                g_InterfaceManager.GetInterfaceString(1444, szTemp);
                            }
                            break;                            
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM
						case STRUCT_SUBJECT_PACKETINFO::SUN_RANKING_SETTLE_RESULT_TYPE:
							{
								// 3625 : SUN랭킹 결과안내 쪽지입니다.
								g_InterfaceManager.GetInterfaceString(3625, szTemp);
							}
							break;
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM
						default:
							{
								//TODO:타입이 늘어나면 그에 맞게 세팅해주면됨(현재는1이로또)
							}
							break;
						}
					}
					else
					{
						StrnCopy(szTemp, GetManager()->m_vecMailData[iLine].m_subject.m_ptszSubject, MAX_SUBJECT_STRLENGTH-1);
					}

					if(strlen(szTemp) >= (MAX_SUBJECT_WLENGTH-1))
					{
						strcat(szTemp, "...");	// 메모 메시지 '...' 처리
					}
				}
				break;
			}
			_SettListCtrlData(COMMUNITY_LST_MAIL,index, iCnt,szTemp,!(GetManager()->m_vecMailData[iLine].m_subject.m_byRead));
		}

	}
}
//------------------------------------------------------------------------------
/**
*/
void uiCommunityDlg::RefreshFriendList()
{
	int iGetMode = GetManager()->GetMode();

	// sort를 기본으로 함
	_SortByType(iGetMode, GetManager()->GetSortBy(iGetMode));

	if(!m_pListCtrls[COMMUNITY_LST_FRIEND]) return;
	if(!m_pScroll) return;
	if(!GetManager()) return;

	this->_hideOnOffIcons();

	m_pListCtrls[COMMUNITY_LST_FRIEND]->DeleteItemAll();
	m_pListCtrls[COMMUNITY_LST_FRIEND]->SetColorSelBar(RGBA(0,0,0,255));

	int index = 0;
	int iCnt = 0;

	if(iGetMode == eCM_FRIEND)// 그룹리펙토링: if(GetManager()->GetMode() == eCM_FRIEND)
	{
		_RefreshControl(GetManager()->m_FriendArray);
	}
	else if(iGetMode == eCM_REVENGE)// 그룹리펙토링: if(GetManager()->GetMode() == eCM_FRIEND)
	{
		_RefreshControl(GetManager()->m_RevengeArray);
	}
	else if(iGetMode == eCM_GROUP)// 그룹리펙토링: if(GetManager()->GetMode() == eCM_FRIEND)
	{
  		_RefreshControl(GetManager()->m_GroupArray);
	}
	else if(iGetMode == eCM_BLOCK)	// 차단목록 // 그룹리펙토링: if(GetManager()->GetMode() == eCM_FRIEND)
	{
		m_pListCtrls[COMMUNITY_LST_FRIEND]->SetSellHeight(23);
		m_pListCtrls[COMMUNITY_LST_FRIEND]->SetUseReport(false);
		m_pListCtrls[COMMUNITY_LST_FRIEND]->SetFontID(c_wiCommunityFont);


		int iPage = GetManager()->m_BlockFriendArray.Size()-SHOW_DEFAULT_FRIEND_LINE;
		int iScrollRange = 1;

		if(iPage<=0)iPage=0;

		if(m_pScroll)
			m_pScroll->SetScrollRangeWZ(0,iPage);	//스크롤 범위

		int iLine = 0;
		for(int insert=0; insert<SHOW_DEFAULT_FRIEND_LINE; ++insert)
		{

			iLine = (m_pScroll->GetScrollPosWZ() * iScrollRange) + insert;

			if(iLine >= GetManager()->m_BlockFriendArray.Size())
				break;

			TCHAR *pName = (TCHAR *)GetManager()->m_BlockFriendArray[iLine].name.Get();

			index = m_pListCtrls[COMMUNITY_LST_FRIEND]->InsertItemBack();
			GlobalFunc::SetItemText( m_pListCtrls[COMMUNITY_LST_FRIEND], index, pName );
			m_pListCtrls[COMMUNITY_LST_FRIEND]->SetItemFontID( index,  0, c_wiCommunityFont);
			
			m_pListCtrls[COMMUNITY_LST_FRIEND]->SetItemTextColor(
				index, 
				0, 
				RGBA(255,0,0,255)
				);

			m_pListCtrls[COMMUNITY_LST_FRIEND]->SetItemSelTextColor(
				index, 
				0, 
				RGBA(255,0,0,255)
				);
			
			m_pListCtrls[COMMUNITY_LST_FRIEND]->SetItemBKColor(
				index, 
				0, 
				RGBA(255,255,255,0)
				);
		}
	}

	_refreshOnOffIcon();
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityDlg::RefreshMailList()
{
	if(!m_pListCtrls[COMMUNITY_LST_MAIL]) return;
	if(!m_pScroll) return;
	if(!GetManager()) return;

	// sort를 기본으로 함
	int iGetMode = GetManager()->GetMode();
	_SortByType(iGetMode, GetManager()->GetSortBy(iGetMode));

	this->_hideMailIcons();

	m_pListCtrls[COMMUNITY_LST_MAIL]->DeleteItemAll();
	m_pListCtrls[COMMUNITY_LST_MAIL]->SetColorSelBar(RGBA(0,0,0,255));

	int index = 0;
	int iCnt = 0;

	if(GetManager()->GetMode() == eCM_MAIL)
	{
		TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
		TCHAR szTemp2[INTERFACE_STRING_LENGTH] = {0,};
		
		m_pListCtrls[COMMUNITY_LST_MAIL]->SetSellHeight(c_iSellHeight);
		m_pListCtrls[COMMUNITY_LST_MAIL]->SetUseReport(true);
		m_pListCtrls[COMMUNITY_LST_MAIL]->SetReportUseCnt(MAIL_LIST_MAX);
		m_pListCtrls[COMMUNITY_LST_MAIL]->SetHideReportRender(true);
		m_pListCtrls[COMMUNITY_LST_MAIL]->SetFontID(c_wiCommunityFont);

        GlobalFunc::SetReportButtonInfo( m_pListCtrls[COMMUNITY_LST_MAIL], MAIL_LIST_NAME, mail_list_column_[MAIL_LIST_NAME], _T("name"));
        GlobalFunc::SetReportButtonInfo( m_pListCtrls[COMMUNITY_LST_MAIL], MAIL_LIST_DATE, mail_list_column_[MAIL_LIST_DATE], _T("date"));
        GlobalFunc::SetReportButtonInfo( m_pListCtrls[COMMUNITY_LST_MAIL], MAIL_LIST_MEMO, mail_list_column_[MAIL_LIST_MEMO], _T("memo"));
	
		int iMailCnt = 0;

		_RefreshMailControl();

		_refreshMailIcon();
		_refreshMailNum();
	}
}
				
//------------------------------------------------------------------------------
/**
*/
void uiCommunityDlg::MessageProc( SI_MESSAGE * pMessage )
{
    if (!_isEnable())
        return;

	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};

    switch( pMessage->eResultMsg ) 
    {
	
	case RT_MSG_SB_PAGETOP:			//. 윗쪽 몸통을 눌렀다.
	case RT_MSG_SB_LINETOP:			//.윗쪽 화살표를 눌렀다.
		{
			if(m_pScroll)
			{
				UpScroll(SHOW_DEFAULT_FRIEND_LINE/2);
			}
		}
		break;

	case RT_MSG_SB_PAGEBOTTOM:				//. 아래쪽 몸통을 눌렀다.
	case RT_MSG_SB_LINEBOTTOM:				//.아랫쪽 화살표를 눌렀다
		{
			if(m_pScroll)
			{
				DownScroll(SHOW_DEFAULT_FRIEND_LINE/2);
			}
		}
		break;
	
	
	case RT_MSG_SB_THUMBTRACK:				//. 썸을 드래그 하고 있다.
	case RT_MSG_SB_THUMBPOSITION:			//. 썸을 드래그한후 마우스를 놓았다.
	case RT_MSG_SB_ENDSCROLL:				//. 스크롤이 종료 되었다.
		{
			if(m_pScroll)
			{
				_clearSelection();

				if(GetManager()->GetMode() == eCM_MAIL)
				{
					this->RefreshMailList();
				}
				else
				{
					this->RefreshFriendList();
				}

                GetManager()->set_last_scrollbar_position(static_cast<eCOMMUNITY_MODE>(GetManager()->GetMode()),
                                                            m_pScroll->GetScrollPosWZ());
			}
		}
		break;

	case RT_MSG_LBUTTONDCLICK:
		{
			switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
			{
			case COMMUNITY_DIALOG_LST_FRIEND:
				{
					// 상세 정보 보기
					if(GetManager()->GetMode() == eCM_BLOCK)	// 차단탭에선 지원안함
						break;

					TCHAR *pName = GetManager()->GetSelectString();
					if(pName)
					{
#ifdef _NA_0_20100527_ADD_CHAR_RELATION_MEMO
                        GetManager()->Send_CW_FRIEND_STATUS_REQ(pName, GetManager()->GetGUID(pName));

                        if (GetManager()->GetCommunityinfo_Dialog())
                        {
                            GetManager()->GetCommunityinfo_Dialog()->set_request_uimanager_id(UIMAN_COMMUNITY);
                        }
#else
                        if(GetManager()->IsOnline(pName))
						{
							{
                                GetManager()->Send_CW_FRIEND_STATUS_REQ(pName, GetManager()->GetGUID(pName));
							}
						}

#endif //_NA_0_20100527_ADD_CHAR_RELATION_MEMO

					}
					else
					{
					}
				}
				break;
            case COMMUNITY_DIALOG_LST_MAIL:
                {
                    ViewMemoOnList();
                }
                break;
			}
		}

    case RT_MSG_LBUTTONCLICK:
        {
            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
            case COMMUNITY_DIALOG_EXIT:
                {
                    ShowInterfaceWithSound(FALSE);
                }
                break;
#ifdef _NA_000000_20130114_RENEWER_UI
#else
			case COMMUNITY_DIALOG_TAB_FRIEND:
				{
                    ChangeMode(GetManager()->last_community_friend_subject_mode());
				}
				break;
#endif//#ifdef _NA_000000_20130114_RENEWER_UI

			case COMMUNITY_DIALOG_TAB_MAIL:
				{
					ChangeMode(eCM_MAIL);
				}
				break;
			
			case COMMUNITY_DIALOG_TAB_GROUP:
				{
					ChangeMode(eCM_GROUP);
				}
				break;

			case COMMUNITY_DIALOG_CHECK_REJECTION_GROUP:
				{
					// 메시지 거부
					if(m_pBtnChecks[COMMUNITY_CHECK_REJECTION_GROUP])
					{
						MSG_CW_GROUP_ADDBLOCK_SYN sync;
						sync.m_bBlock = !GetManager()->GetRejectGroup();
						GlobalFunc::SendPacket(CI_CHATSERVERIDX, &sync, sizeof (sync));	
					}
				}
				break;

			case COMMUNITY_DIALOG_LIST_BTN_ID:
				{
					if(GetManager()->GetSortBy(GetManager()->GetMode()) == eCOMSORT_NAME) // 버튼이 같을때
						GetManager()->SetSortReady(eCOMSORT_BSAMETAB_SAMEBTN);	// 반전이 필요
					else
						GetManager()->SetSortReady(eCOMSORT_BSAMETAB_OTHERBTN);	// 무조건 true;
					GetManager()->SetSortBy(GetManager()->GetMode(), eCOMSORT_NAME);
					RefreshFriendList();
				}
				break;
			case COMMUNITY_DIALOG_LIST_BTN_LV:
				{
					if(GetManager()->GetSortBy(GetManager()->GetMode()) == eCOMSORT_LV)
						GetManager()->SetSortReady(eCOMSORT_BSAMETAB_SAMEBTN);	// 반전이 필요
					else
						GetManager()->SetSortReady(eCOMSORT_BSAMETAB_OTHERBTN);	// 무조건 true;

					GetManager()->SetSortBy(GetManager()->GetMode(), eCOMSORT_LV);
					RefreshFriendList();
				}
				break;
			case COMMUNITY_DIALOG_LIST_BTN_CLASS:
				{
					if(GetManager()->GetSortBy(GetManager()->GetMode()) == eCOMSORT_CLASS)
						GetManager()->SetSortReady(eCOMSORT_BSAMETAB_SAMEBTN);	// 반전이 필요
					else
						GetManager()->SetSortReady(eCOMSORT_BSAMETAB_OTHERBTN);	// 무조건 true;

					GetManager()->SetSortBy(GetManager()->GetMode(), eCOMSORT_CLASS);
					RefreshFriendList();
				}
				break;
			case COMMUNITY_DIALOG_LIST_BTN_CHANNEL:
				{
					if(GetManager()->GetSortBy(GetManager()->GetMode()) == eCOMSORT_CHANNEL)
						GetManager()->SetSortReady(eCOMSORT_BSAMETAB_SAMEBTN);	// 반전이 필요
					else
						GetManager()->SetSortReady(eCOMSORT_BSAMETAB_OTHERBTN);	// 무조건 true;

					GetManager()->SetSortBy(GetManager()->GetMode(), eCOMSORT_CHANNEL);
					RefreshFriendList();
				}
				break;
			case COMMUNITY_DIALOG_LIST_BTN_LOGON:
				{
					if(GetManager()->GetSortBy(GetManager()->GetMode()) == eCOMSORT_ONLINE)
						GetManager()->SetSortReady(eCOMSORT_BSAMETAB_SAMEBTN);	// 반전이 필요
					else
						GetManager()->SetSortReady(eCOMSORT_BSAMETAB_OTHERBTN);	// 무조건 true;

					GetManager()->SetSortBy(GetManager()->GetMode(), eCOMSORT_ONLINE);
					RefreshFriendList();
				}
				break;
			case COMMUNITY_DIALOG_LIST_BTN_SENDER:
				{
					if(GetManager()->GetSortBy(GetManager()->GetMode()) == eCOMSORT_SENDER)
						GetManager()->SetSortReady(eCOMSORT_BSAMETAB_SAMEBTN);	// 반전이 필요
					else
						GetManager()->SetSortReady(eCOMSORT_BSAMETAB_OTHERBTN);	// 무조건 true;

					GetManager()->SetSortBy(GetManager()->GetMode(), eCOMSORT_SENDER);
					RefreshMailList();
				}
				break;
			case COMMUNITY_DIALOG_LIST_BTN_DATE:
				{
					if(GetManager()->GetSortBy(GetManager()->GetMode()) == eCOMSORT_DATE)
						GetManager()->SetSortReady(eCOMSORT_BSAMETAB_SAMEBTN);	// 반전이 필요
					else
						GetManager()->SetSortReady(eCOMSORT_BSAMETAB_OTHERBTN);	// 무조건 true;

					GetManager()->SetSortBy(GetManager()->GetMode(), eCOMSORT_DATE);
					RefreshMailList();
				}
				break;
			case COMMUNITY_DIALOG_LIST_BTN_MEMO:
				{
					if(GetManager()->GetSortBy(GetManager()->GetMode()) == eCOMSORT_MEMO)
						GetManager()->SetSortReady(eCOMSORT_BSAMETAB_SAMEBTN);	// 반전이 필요
					else
						GetManager()->SetSortReady(eCOMSORT_BSAMETAB_OTHERBTN);	// 무조건 true;

					GetManager()->SetSortBy(GetManager()->GetMode(), eCOMSORT_MEMO);
					RefreshMailList();
				}
				break;

			case COMMUNITY_DIALOG_TAB_SUB_FRIEND:
				{
					ChangeMode(eCM_FRIEND);
				}
				break;

			case COMMUNITY_DIALOG_TAB_SUB_BLOCK:
				{
					ChangeMode(eCM_BLOCK);
				}	
				break;

			case COMMUNITY_DIALOG_TAB_SUB_REVENGE:
				{
					ChangeMode(eCM_REVENGE);
				}
				break;

			case COMMUNITY_DIALOG_CHECK_REJECTION_CHAT:
				{
					// 메시지 거부
					if(m_pBtnChecks[COMMUNITY_CHECK_REJECTION_CHAT])
					{
						if(!GetManager()->GetRejectChat() && GetManager()->m_chatDlgArray.Size()>0)
						{
							//	열려 있는 대화창이 있습니다.대화창을 모두 닫고 대화 수신을 거부 하시겠습니까?
							g_InterfaceManager.GetInterfaceString( eST_MESSENGER_REJECTION_CHECK, szMessage, INTERFACE_STRING_LENGTH );

							uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
							if(pSystemMan)
								pSystemMan->Sys_ConfirmUnLock(szMessage,_CallBack_MessageRejection);

						}
						else
						{
							MSG_CW_FRIEND_CHATBLOCK_SYN sync;
							sync.m_bBlock = !GetManager()->GetRejectChat();
							GlobalFunc::SendPacket(CI_CHATSERVERIDX, &sync, sizeof (sync));	
						}
					}
				}
				break;

			case COMMUNITY_DIALOG_CHECK_HIDE_OFFLINE:
				{
					// 오프라인 유저 안보기
					GetManager()->ToggleNotDisplayOffLine();
					RefreshFriendList();
				}
				break;

			case COMMUNITY_DIALOG_CHECK_SHOW_NEW_MAIL:
				{
					// 확인하지 않은 쪽지만 표시
					GetManager()->ToggleDisplayNewMail();
					RefreshMailList();
				}
				break;

			case COMMUNITY_DIALOG_BTN_ADD_FRIEND:
				{
					if(GetManager()->IsShowSystemDialog())
					{
						break;
					}

					if (GlobalFunc::IsSSQScene())
					{
						break;
					}
					// 친구 추가
					g_InterfaceManager.GetInterfaceString( 1307, szMessage, INTERFACE_STRING_LENGTH );

					uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
					if(pSystemMan)
					{
						pSystemMan->Sys_ConfirmMessageUnLock (szMessage,_CallBack_Friend_Add);
						pSystemMan->Sys_ConfirmMessageSetFocus();
					}

					GetManager()->SetSystemDialog(true);
				}
				break;

			case COMMUNITY_DIALOG_BTN_DEL_FRIEND:
				{
					if(GetManager()->IsShowSystemDialog())
						break;

					// 친구 삭제
					TCHAR *pSzName = GetManager()->GetFriendSelectString();

					if(pSzName)
					{
						GetManager()->SetPendingDelFriend(pSzName);

						TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
						g_InterfaceManager.GetInterfaceString( 1308, szTemp, INTERFACE_STRING_LENGTH );

						Snprintf(szMessage,INTERFACE_STRING_LENGTH-1,szTemp,pSzName);

						uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
						if(pSystemMan)
							pSystemMan->Sys_ConfirmUnLock(szMessage,_CallBack_Friend_Delete);						

						GetManager()->SetSystemDialog(true);
					}
					else
					{
						g_InterfaceManager.GetInterfaceString( 70029, szMessage, INTERFACE_STRING_LENGTH );

						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);
					}
				}
				break;

			case COMMUNITY_DIALOG_BTN_ADD_BLOCK:
				{
					if(GetManager()->IsShowSystemDialog())
						break;

					// 차단 추가
					g_InterfaceManager.GetInterfaceString( 1309, szMessage, INTERFACE_STRING_LENGTH );

					uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
					if(pSystemMan)
						pSystemMan->Sys_ConfirmMessageUnLock(szMessage,_CallBack_BlockList_Add);

					GetManager()->SetSystemDialog(true);

				}
				break;

			case COMMUNITY_DIALOG_BTN_DEL_BLOCK:
				{
					if(GetManager()->IsShowSystemDialog())
						break;

					// 차단 해제
					TCHAR *pSzName = GetManager()->GetBlockFriendSelectString();

					if(pSzName)
					{
						GetManager()->SetPendingDelBlock(pSzName);

						TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
						g_InterfaceManager.GetInterfaceString( 1308, szTemp, INTERFACE_STRING_LENGTH );
		
						Snprintf(szMessage,INTERFACE_STRING_LENGTH-1,szTemp,pSzName);

						uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
						if(pSystemMan)
							pSystemMan->Sys_ConfirmUnLock(szMessage,_CallBack_BlockList_Delete);

						GetManager()->SetSystemDialog(true);
						
					}
					else
					{
						g_InterfaceManager.GetInterfaceString( 70029, szMessage, INTERFACE_STRING_LENGTH );

						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);
					}
				}
				break;

			case COMMUNITY_DIALOG_BTN_DEL_REVENGE:
				{
					if(GetManager()->IsShowSystemDialog())
						break;

					// 리벤지 해제
					TCHAR *pSzName = GetManager()->GetRevengeSelectString();

					if(pSzName)
					{
						GetManager()->SetPendingDelRevenge(pSzName);

						TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
						g_InterfaceManager.GetInterfaceString( 1308, szTemp, INTERFACE_STRING_LENGTH );

						Snprintf(szMessage,INTERFACE_STRING_LENGTH-1,szTemp,pSzName);

						uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
						if(pSystemMan)
							pSystemMan->Sys_ConfirmUnLock(szMessage,_CallBack_Revenge_Delete);						

						GetManager()->SetSystemDialog(true);
					}
					else
					{
						g_InterfaceManager.GetInterfaceString( 70029, szMessage, INTERFACE_STRING_LENGTH );

						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);
					}
				}
				break;

			case COMMUNITY_DIALOG_BTN_WARP:
				{
					// 워프하기
					TCHAR *pName =GetManager()->GetSelectString();
					if(pName)
					{
						int index=-1;				
						index=ItemManager::Instance()->GetSlotIndex(SI_INVENTORY, eITEMWASTE_REVENGESTONE);

						//Todo:복수의 돌 아이템 체크 후 날릴것 
						if(index>0)
						{
							MSG_CG_CHAO_TRACK_REVENGE_CHAR_SYN sync;
							ZeroMemory(sync.m_ptszRevengeName,sizeof(sync.m_ptszRevengeName));
							StrnCopy(sync.m_ptszRevengeName , pName, MAX_CHARNAME_LENGTH);
							sync.m_atItemPos=index;
							//Todo:워프 하였을 경우 무적 시간도 필요 하다.
							GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof (sync));
						}
						else
						{
							//5681	[복수의 돌]이 필요합니다.
							g_InterfaceManager.GetInterfaceString(eST_NEDD_REVENGE_STONE , szMessage ,INTERFACE_STRING_LENGTH);
							GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage); 
						}
					}
					else
					{
						g_InterfaceManager.GetInterfaceString( 70029, szMessage, INTERFACE_STRING_LENGTH );

						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);
					}
				}
				break;


			case COMMUNITY_DIALOG_BTN_DEL_MAIL:
				{
					if(GetManager()->IsShowSystemDialog())
						break;

					if(GetManager()->GetCurMailGuid())
					{
						GetManager()->SetPendingDelMemoGuid(GetManager()->GetCurMailGuid());
						// 5784	쪽지를 삭제하시겠습니까?
						g_InterfaceManager.GetInterfaceString( 5784 , szMessage ,INTERFACE_STRING_LENGTH);


						uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
						if(pSystemMan)
							pSystemMan->Sys_ConfirmUnLock(szMessage,_CallBack_Mail_Delete);						

						GetManager()->SetSystemDialog(true);
					}

				}
				break;

			case COMMUNITY_DIALOG_BTN_DEL_ALL_MAIL:
				{
					if(GetManager()->IsShowSystemDialog())
						break;

					if(GetManager()->m_vecMailData.size() == 0)
						break;

					// 5785	전체 쪽지를 삭제하시겠습니까?
					g_InterfaceManager.GetInterfaceString( 5785 , szMessage ,INTERFACE_STRING_LENGTH);


					uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
					if(pSystemMan)
						pSystemMan->Sys_ConfirmUnLock(szMessage,_CallBack_All_Mail_Delete);						

					GetManager()->SetSystemDialog(true);
				}
				break;
		
			case COMMUNITY_DIALOG_BTN_WRITE_MAIL:
				{
					if(GetManager()->IsShowSystemDialog())
						break;

					// 쪽지 쓰기
					if(GetManager()->GetMode() == eCM_MAIL)
					{
						GetManager()->m_iCurMailSelect = GetCurListSel();

						if((GetManager()->m_iCurMailSelect >= 0) && ((unsigned int)GetManager()->m_iCurMailSelect < GetManager()->m_vecMailData.size()))
						{
							MAIL_DATA *pData = GetCurMailData(GetManager()->m_iCurMailSelect);
							if(pData)
							{
								GetManager()->ShowWriteMailDlg(pData->m_subject.m_SendName);
							}
							else
							{
								GetManager()->ShowWriteMailDlg();					
							}
						}
						else
						{
							GetManager()->ShowWriteMailDlg();
						}
					}
					else
					{
						GetManager()->ShowWriteMailDlg();					
					}
				}
				break;
				
			case COMMUNITY_DIALOG_BTN_ADD_MEMBER:
				{					
					if(GetManager()->IsShowSystemDialog())
						break;

					TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};

					// 그룹 멤버 추가 (7031: 멤버 등록할 아이디를 입력하십시오.)
					g_InterfaceManager.GetInterfaceString( 7031, szMessage, INTERFACE_STRING_LENGTH );
					g_InterfaceManager.GetInterfaceString( 1924, szTemp, INTERFACE_STRING_LENGTH );

					uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);

					if(pSystemMan)
					{
						pSystemMan->Sys_ConfirmMessageUnLock(szMessage,_CallBack_GroupMember_Add, NULL, szTemp);
					}
					GetManager()->SetSystemDialog(true);
				}
				break;

			case COMMUNITY_DIALOG_BTN_DEL_MEMBER:
				{
					if(GetManager()->IsShowSystemDialog())
						break;

					TCHAR *pSzName = GetManager()->GetGroupSelectString();

					if(pSzName)
					{
						GetManager()->SetPendingDelGroup(pSzName);

						TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
						// 그룹 멤버 해제 (7033: %s님을 멤버에서 해제하겠습니까?)
						g_InterfaceManager.GetInterfaceString( 7033, szTemp, INTERFACE_STRING_LENGTH );
		
						Snprintf(szMessage,INTERFACE_STRING_LENGTH-1,szTemp,pSzName);

						uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
						if(pSystemMan)
						{
							pSystemMan->Sys_ConfirmUnLock(szMessage,_CallBack_GroupMember_Delete);
						}
						GetManager()->SetSystemDialog(true);
						
					}
					else
					{	// 70029 사용자를 찾을 수 없습니다.
						g_InterfaceManager.GetInterfaceString( 70029, szMessage, INTERFACE_STRING_LENGTH );
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);
					}
				}
				break;

			case COMMUNITY_DIALOG_BTN_INVITE:
				{
					TCHAR* name = GetManager()->GetSelectString();

					if (name)
					{
						if (GetManager()->IsOnline(name))
						{

							uiPartyMan* PartyMan =	GET_DIALOG_MANAGER(uiPartyMan, UIMAN_PARTY);


							if (PartyMan)
							{
								if (PartyMan->IsParty() == false)
                                {
									PartyMan->AskJoinParty(name);
                                }
								else if	(PartyMan->IsParty() && PartyMan->IsLeader())
                                {
									PartyMan->AskJoinParty(name);
                                }
								else 
								{
									g_InterfaceManager.GetInterfaceString(eST_PARTY_ISNOT_MASTEROFPARTY, szMessage, INTERFACE_STRING_LENGTH);
									GlobalFunc::OutputMsg_Warning(eOUTPUT_MSG_CHAT, szMessage);
								}									
							}
						}
					}

				}
				break;

            }
        }
        break;

	default:
		{
			SolarDialog::MessageProc( pMessage );
		}
		break;
    }
}

//----------------------------------------------------------------------------
/**	스크롤 다운
*/
void uiCommunityDlg::DownScroll(int nRange)
{
	int pos = m_pScroll->GetScrollPosWZ();//현재 스크롤 값을 받아 온다.
	int mmin = 0 , mmax = 0;
	m_pScroll->GetScrollRangeWZ( &mmin, &mmax );//스크롤 값의 최대 최소를 얻어 온다.

	if(pos<mmax)// 현재 입력된 라인수 보다 작으면 
	{	
		((pos+nRange)>mmax ? (pos=mmax):(pos+=nRange));
		m_pScroll->SetScrollPosWZ(pos);
		_clearSelection();

		if(GetManager()->GetMode() == eCM_MAIL)
		{
			this->RefreshMailList();
		}
		else
		{
			this->RefreshFriendList();
		}
        GetManager()->set_last_scrollbar_position(static_cast<eCOMMUNITY_MODE>(GetManager()->GetMode()),
                                                    m_pScroll->GetScrollPosWZ());
	}
}
//----------------------------------------------------------------------------
/** 스크롤 업
*/
void uiCommunityDlg::UpScroll(int nRange)
{
	int pos = m_pScroll->GetScrollPosWZ();//현재 스크롤 값을 받아 온다.
	int mmin = 0 , mmax = 0;
	m_pScroll->GetScrollRangeWZ( &mmin, &mmax );//스크롤 값의 최대 최소를 얻어 온다.

	if(pos>mmin)// 현재 입력된 라인수 보다 작으면 
	{

		( (pos-nRange)<mmin ? (pos=mmin):(pos-=nRange));
		
		m_pScroll->SetScrollPosWZ(pos);

		_clearSelection();

		if(GetManager()->GetMode() == eCM_MAIL)
		{
			this->RefreshMailList();
		}
		else
		{
			this->RefreshFriendList();
		}

        GetManager()->set_last_scrollbar_position(static_cast<eCOMMUNITY_MODE>(GetManager()->GetMode()),
                                                    m_pScroll->GetScrollPosWZ());
	}
}
//----------------------------------------------------------------------------
/** 업데이트 스크롤
*/
void uiCommunityDlg::UpdateMouseWheel()
{
	//1.영역체크는 다이얼로그 전체사이즈로 하겠씀
	if(!m_pListCtrls) return;
		
	RECT rc = m_pListCtrls[COMMUNITY_LST_FRIEND]->GetSizeRect();

	switch(GetMouseWheelStatus(&rc))
	{
	case MOUSE_WHEEL_UP: UpScroll(1);break;

	case MOUSE_WHEEL_DOWN:DownScroll(1);break;
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiCommunityDlg::OnUpdateSolarDialog()
{
	
	UpdateMouseWheel();
	
	if (!IsVisible())
    {
		ShowInterface(FALSE);
    }

    if (!_isEnable())
        return;

	_updateCheckState();

	
}


//------------------------------------------------------------------------------
/** 쪽지 보기 코드를 함수로 만듬 
    define _DH_DBLCLICK_FRIEND_STATUS - 관련 쪽지보기 더블클릭/클릭 변경에 따른 코드관리를 위해서
*/
void uiCommunityDlg::ViewMemoOnList()
{
    if (GetManager()->IsShowSystemDialog())
    {
        return;
    }

    // 쪽지 보기
    int current_mail_select = GetCurListSel();
    int mail_data_size = static_cast<int>(GetManager()->m_vecMailData.size());
    GetManager()->m_iCurMailSelect = current_mail_select;
    if ((current_mail_select >= 0) && 
        (current_mail_select < mail_data_size))
    {
        MAIL_DATA* mail_data = GetCurMailData(current_mail_select);
        if (mail_data == NULL)
        {
            return;
        }
        
        // 이미 상세내용을 받은 상태면 보여줌
        if (mail_data->m_bRecvMemo == true)
        {
            GetManager()->ShowReadMailDlg(mail_data);
            if (mail_data->m_amountCost != 0)
            {
                GetManager()->SEND_CG_EVENT_SYSTEMMEMO_ITEM_REWARD_REQ(mail_data);
            }
            return;
        }

        // 서버에게 상세 내용을 요청한다
        bool success_send = GetManager()->SEND_CW_MEMO_VIEW_REQ(mail_data->m_subject.m_MemoGuid);
        if (success_send == false)
        {
            return;
        }

        // 시스템메모 중 하임로또 보상쪽지를 읽지 않았다면 보상을 요청
        if ((mail_data->m_subject.m_MemoType != STRUCT_SUBJECT_PACKETINFO::SYSTEM_MEMO) ||
            (mail_data->m_subject.m_byRead != FALSE))
        {
            return;
        }
        int system_type = static_cast<int>(GetManager()->ParseMemoInfo(mail_data->m_subject.m_ptszSubject, 
                                                                       0, 
                                                                       MAX_SUBJECT_STRLENGTH));
        
        switch (system_type)
        {
        case STRUCT_SUBJECT_PACKETINFO::LOTTO_TYPE:
            {
                MSG_CW_LOTTO_NOTICE_CONFIRM send_packet;
                send_packet.i64Index = GetManager()->ParseMemoInfo(mail_data->m_subject.m_ptszSubject, 
                                                                   1, 
                                                                   MAX_SUBJECT_STRLENGTH - 1);
                GlobalFunc::SendPacket(CI_CHATSERVERIDX, &send_packet, sizeof(send_packet));
            }
            break;
        case STRUCT_SUBJECT_PACKETINFO::DOMINATION_TYPE_RETURN:
        case STRUCT_SUBJECT_PACKETINFO::DOMINATION_TYPE_REWARD:
            {
                MSG_CW_DOMINATION_RETURN_HEIM_CMD send_packet;
                memcpy(send_packet.m_ptszMemo, 
                       mail_data->m_subject.m_ptszSubject, 
                       sizeof(mail_data->m_subject.m_ptszSubject));
                GlobalFunc::SendPacket(CI_CHATSERVERIDX, &send_packet, sizeof(send_packet));	
            }
            break;
        }        
    }
}
