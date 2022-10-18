#include "SunClientPrecompiledHeader.h"

#include "uiSocketRemove.h"
#include "MouseHandler.h"
#include "GlobalFunc.h"
#include <SCSlotContainer.h>
#include <SCItemSlot.h>
#include "ItemManager.h"
#include "InterfaceManager.h"
#include "KeyQueueManager.h"
#include "GameConst_Color.h"

#include "ItemInfoParser.h"
#include "OptionListParser.h"

#include "mouse.h"
#include "uiToolTipMan/uiToolTipMan.h"

WzID uiSocketRemove::m_wzID[DLGPOS_MAX] = 
{
	StrToWzID("BT00"),		//eDIALOG_BTN_EXIT,
	StrToWzID("BT01"),		//eDIALOG_BTN_OK,
	StrToWzID("BT02"),		//eDIALOG_BTN_CANCLE,
	StrToWzID("ST00"),		//eDIALOG_TXT_CAPTION,
	StrToWzID("LS00"),		//eDIALOG_LIST,

};
//------------------------------------------------------------------------------
/**
*/
uiSocketRemove::uiSocketRemove(InterfaceManager *pUIMan) :
    uiBase(pUIMan)
{

}
//------------------------------------------------------------------------------
/**
*/
uiSocketRemove::~uiSocketRemove()
{

}
//------------------------------------------------------------------------------
/**
*/
void uiSocketRemove::Init(CDrawBase* pDrawBase)
{
	for ( int i =0 ; i < BTN_MAX ; ++i )
	{
		m_pButton[i] = static_cast<CCtrlButtonWZ *> ( getControl(i) );
	}

	m_pStatic = static_cast<CCtrlStaticWZ * >( getControl(eDIALOG_TXT_CAPTION) );
	m_pList	= static_cast<CCtrlListWZ *> ( getControl(eDIALOG_LIST) );
	m_pList->SetUseReport(FALSE);
    m_pList->SetTextReducing(true);

	for ( int i =0 ; i < SOCKET_STATE_MAX; ++i )
	{
		m_iSocketState[i].socketPos = 0;
		m_iSocketState[i].socketType = 0;
	}
	m_bShow = FALSE;
	m_bSendPacket = FALSE;
	m_TargetSocketItemPos = 0;
}
//------------------------------------------------------------------------------
/**
*/
void uiSocketRemove::Release()
{

}
//------------------------------------------------------------------------------
/**
*/
void uiSocketRemove::OnUpdateSolarDialog()
{
	if( !m_bShow )
	{
		ShowWindow(FALSE);
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiSocketRemove::OnRenderSolarDialog()
{
    POINT mouse;
    mouse.x = Mouse::Instance()->GetMouseX();
    mouse.y = Mouse::Instance()->GetMouseY();

    if (m_pList && DEF_IntersectRect(m_pList->GetSizeRect(), mouse))
    {
        int over_sel = m_pList->GetOverSel();
        if (over_sel >= 0 && over_sel < SOCKET_STATE_MAX)
        {
            if (_tcslen(socket_names[over_sel]) > 0)
            {
                uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
                if (tooltip_manager)
                {
                    tooltip_manager->RegisterTooltipSimpleText(socket_names[over_sel]);
                    tooltip_manager->SetTooltipDialogID(GetDialogKey());
                }
            }
        }
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiSocketRemove::Process(DWORD dwTick)
{

}
//------------------------------------------------------------------------------
/**
*/
void uiSocketRemove::OnShowWindow(BOOL val)
{
	if( val )
	{
		m_bShow  = TRUE;
		MoveCenter();

		TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};
        DWORD string_code = 0;
        switch(MouseHandler::Instance()->GetSocketRemoveType())
        {
        case kZardGrade1:
            string_code = 158022; // 하급 소멸의 룬
            break;
        case kZardGrade2:
            string_code = 158162; // 중급 소멸의 룬
            break;
        case kZardGrade3:
            string_code = 158052; // 상급 소멸의 룬
            break;
        case kZardGrade4:
            string_code = 158167; // 최상급 소멸의 룬
            break;
        }
		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType = eDoType_dialog_Socket_Remove_cancel;
		msg.wParam = InterfaceManager::DIALOG_SOCKET_REMOVE;
		msg.DoSomething = GlobalFunc::DoShowWindow;
		g_KeyQueueManager.PushBack(msg);
	}
	else
	{
		m_bShow = FALSE;
		m_pList->DeleteItemAll();
		g_KeyQueueManager.DeleteMsg(eDoType_dialog_Socket_Remove_cancel);
	}

}
//------------------------------------------------------------------------------
/**
*/
void uiSocketRemove::MessageProc(SI_MESSAGE * pMessage)
{
	switch( pMessage->eResultMsg)
	{
	case RT_MSG_LBUTTONCLICK:
		{
		switch ( getControlIDToPos(pMessage->dwCtrlID) )
			{
			case eDIALOG_BTN_OK:
				{
					// 패킷 보내기
					NET_SEND_CG_ITEM_SOCKET_EXTRACT_SYN();
					m_bShow = FALSE;
					
				}
				break;

			case eDIALOG_BTN_CANCLE:
			case eDIALOG_BTN_EXIT:
				{
					m_bShow = FALSE;
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
void uiSocketRemove::NetworkProc(MSG_BASE * pMsg)
{
	
}

//------------------------------------------------------------------------------
/**
*/
BOOL uiSocketRemove::IsAvaliableRemoveSocket(SLOTIDX SlotIdx, POSTYPE TargetPos)
{
	m_TargetSocketItemPos = TargetPos;
	if ( MouseHandler::Instance()->GetTryRemoveSocket() )
	{
		return FALSE;
	}

	SCSlotContainer * pAtContainer = ItemManager::Instance()->GetContainer(SlotIdx);

	assert(pAtContainer);
	if (!pAtContainer)
		return FALSE;

	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,} , szItemName[INTERFACE_STRING_LENGTH] = {0,};
	// 비워있느냐?
	if (pAtContainer->IsEmpty(TargetPos))
	{
		//	해당 슬롯이 비어있습니다!
		g_InterfaceManager.GetInterfaceString( eST_TARGET_SLOT_IS_EMPTY, szMessage, INTERFACE_STRING_LENGTH );
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
		return FALSE;
	}

	// 조합아이템이 적법한지 검사
	SCItemSlot & rItemSlot = (SCItemSlot & )pAtContainer->GetSlot(TargetPos);

    if (rItemSlot.CanEquip() == false)
	{
		//	해당 아이템에는 사용이 불가능 합니다.
		return FALSE;
	}

	BOOL bEqualSocketLevel  = FALSE;
	BOOL bHaveSocketOption = FALSE;
	int  iSocketType = MouseHandler::Instance()->GetSocketRemoveType();
    if (rItemSlot.GetSocketIdentifyType() == eSOCKET_UNIDENTIFIED_ITEM)//미확인
    {
        // 소켓 없음
    }
    else if (rItemSlot.GetSocketIdentifyType() == eSOCKET_IDENTIFIED_ITEM)//소켓갯수확인됨
    {
        for(int sock = SOCKET_1; sock < SOCKET_MAX; ++sock)
        {
            zard_grades_[sock] = 0;
        }
        for( int sock = SOCKET_1 ; sock < rItemSlot.GetSocketNum() ; ++sock)
        {
            const sSOCKET_OPTION* socket_option = rItemSlot.GetSocketOption((eSOCKET)sock);
            if (socket_option != NULL)
            {
                if (socket_option->zard_grade > 0)
                {
                    bHaveSocketOption = TRUE;
                    zard_grades_[sock] = socket_option->zard_grade;
                   /*if (iSocketType == socket_option->zard_grade)
                    {
                        bEqualSocketLevel = TRUE;
                    }*/
                }
            }
        }
    }

	if( bHaveSocketOption == FALSE)
	{
		// 5875 소켓 옵션이 없어 제거할 수 없습니다.
		g_InterfaceManager.GetInterfaceString( 5875 , szMessage ,INTERFACE_STRING_LENGTH);
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
		return FALSE;
	}   

	return TRUE;
}

BOOL uiSocketRemove::InsertSocketOption(SLOTIDX SlotIdx, POSTYPE TargetPos)
{
	m_pList->DeleteItemAll();
	SCSlotContainer * pAtContainer = ItemManager::Instance()->GetContainer(SlotIdx);

	if( !pAtContainer )
		return FALSE;

	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,} , szMessage[INTERFACE_STRING_LENGTH] ={ 0,};

	SCItemSlot & rItemSlot = (SCItemSlot & )pAtContainer->GetSlot(TargetPos);
    if (rItemSlot.GetSocketIdentifyType() == eSOCKET_UNIDENTIFIED_ITEM)//미확인
    {
        // 소켓 없음
    }
    else if (rItemSlot.GetSocketIdentifyType() == eSOCKET_IDENTIFIED_ITEM)//소켓갯수확인됨
    {
        const int zard_options_max = sSOCKET_OPTION::OptionPointInfo::kMaxListSize;
        TCHAR socket_buffer_string[zard_options_max][INTERFACE_STRING_LENGTH] = {0,};
        TCHAR socket_options_string[INTERFACE_STRING_LENGTH] = {0};
        TCHAR buffer_string[INTERFACE_STRING_LENGTH] = {0,};
        TCHAR option_format_string[INTERFACE_STRING_LENGTH] = {0,};
        // 12118: [%s %d 포인트]
        g_InterfaceManager.GetInterfaceString(12118, option_format_string);

        WzColor  wzColrText = RGBA(255, 128, 0, 192);
        WzColor selected_text_color = RGBA(255, 255, 0, 255);

        for (int name_index = 0;name_index < SOCKET_STATE_MAX; ++name_index)
        {
            sprintf(socket_names[name_index], "");
        }

        for(int sock = SOCKET_1 ; sock < rItemSlot.GetSocketNum() ; ++sock)
        {
            for (sSOCKET_OPTION::OptionPointInfo::ListSizeType j = 0; 
                j < sSOCKET_OPTION::OptionPointInfo::kMaxListSize; ++j)
            {
                socket_buffer_string[j][0] = '\0';//초기화 ""
            }

            BOOL bEqualLevel = FALSE;
            const sSOCKET_OPTION* socket_option = rItemSlot.GetSocketOption((eSOCKET)sock);
            if (socket_option != NULL)
            {
                m_iSocketState[sock].socketType = socket_option->zard_grade;
                m_iSocketState[sock].socketPos = sock;

                if( m_iSocketState[sock].socketType == MouseHandler::Instance()->GetSocketRemoveType())
                {
                    bEqualLevel = TRUE;
                    //wzColrText = c_InterfaceTextColor;
                }

                TCHAR string_grade[128] = {0,}, string_name[128] = {0,}, string_option[128] = {0,};
                /*g_InterfaceManager.GetInterfaceString(string_grade, )
                socket_option->zard_grade_;
                socket_option->name_code_;
                socket_option->option_kind_;
                socket_option->option_value_;
                */
                if (socket_option != NULL)
                {
                    TCHAR string_grade[128] = {0,}, string_name[128] = {0,}, string_option[128] = {0,};
                    if (socket_option->zard_grade > 0)
                    {
                        // 12119 : 하급, 12120 중급 12121 상급 12122 최상급
                        g_InterfaceManager.GetInterfaceString((12118 + socket_option->zard_grade), 
                            string_grade, INTERFACE_STRING_LENGTH);
                    }
                    else
                    {   // 잘못된 등급이 쟈드
                        g_InterfaceManager.GetInterfaceString(0, string_grade, INTERFACE_STRING_LENGTH);
                        assert(string_grade);
                    }

                    BASE_ITEMINFO* socket_item =
                        ItemInfoParser::Instance()->GetItemInfo(socket_option->item_code);
                    if (socket_item != NULL)
                    {   // 쟈드 아이템 이름
                        g_InterfaceManager.GetInterfaceString(socket_item->m_NCode, string_name,
                            INTERFACE_STRING_LENGTH);
                    }

                    bool is_check = false;

                    const SocketOptionParser* const zard_option_parser = SocketOptionParser::Instance();

                    int socket_option_name_code = 0;
                    if (socket_option != NULL)
                    {
                        // 소켓 옵션의 각 옵션 포인트에 대해
                        for (sSOCKET_OPTION::OptionPointInfo::ListSizeType i = 0; 
                            i < socket_option->option_point_list_size; ++i)
                        {
                            // 포인트 정보가 유효한지 검사한다.
                            const sSOCKET_OPTION::OptionPointInfo* const option_point_info = socket_option->GetPointInfo(i);
                            if (option_point_info == NULL)
                            {
                                continue;
                            }

                            // 옵션 정보가 유효한지 검사한다.
                            const ZardOptionInfo* const zard_option_info = option_point_info->zard_option;
                            if (zard_option_info == NULL || 
                                zard_option_info->attr_option_code == eATTR_TYPE_INVALID || 
                                zard_option_info->item_option_code == eATTR_TYPE_INVALID)
                            {
                                continue;
                            }

                            const BASE_OptionList* const zard_option_desc = OptionListParser::Instance()->GetOptionList(zard_option_info->item_option_code);
                            if (zard_option_desc == NULL)
                            {
                                continue;
                            }
                            // %s에 들어갈 옵션 종류 이름
                            socket_option_name_code = zard_option_desc->mdwNameCode;
                            //option_point_info->option_point; // %d에 들어갈 옵션 포인트
                            TCHAR option_string[INTERFACE_STRING_LENGTH] = {0,};
                            TCHAR comma_string[INTERFACE_STRING_LENGTH] = {0,};

                            g_InterfaceManager.GetInterfaceString(socket_option_name_code, option_string);
                            sprintf(comma_string, "%c%s", (is_check && i < (zard_options_max - 1))?',':' ', option_format_string);
                            sprintf(socket_buffer_string[i], comma_string, option_string, option_point_info->option_point);
                            is_check = true;  
                        }
                    }
                    if (is_check == false)
                    {
                    }
                    else
                    {
                        sprintf(socket_options_string, "%s%s%s%s%s", socket_buffer_string[0], socket_buffer_string[1],
                            socket_buffer_string[2],socket_buffer_string[3],socket_buffer_string[4]);
                        sprintf(szMessage, _T("%s %s %s"), string_grade, string_name, socket_options_string);
                    }
                }
                /*
                // 쟈드 옵션 이름
                g_InterfaceManager.GetInterfaceString(socket_option->name_code, string_option,
                    INTERFACE_STRING_LENGTH);

                if (socket_option->option_type == VALUE_TYPE_VALUE)
                {
                    Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, _T("%s %s %s +%d"), 
                        string_grade, string_name, string_option, socket_option->option_value);
                }
                else
                {
                    Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, _T("%s %s %s +%d%%"), 
                        string_grade, string_name, string_option, socket_option->option_value);
                }
                */
                int index = m_pList->InsertItemBack();
                GlobalFunc::SetItem(m_pList , index , 0 , szMessage , StrToWzID("mn12") , 
                    DT_VCENTER |  DT_LEFT ,  c_InterfaceBKColor , wzColrText , selected_text_color);
                m_pList->SetItemData(index ,&m_iSocketState[sock]);

                sprintf(socket_names[index], szMessage);    // 이름 저장 하기
            }
        }
    }

	if( m_pList->GetListRowCnt() > 0)
	{
		m_pList->SetCurSel(0);
		return TRUE;
	}
	else
	{
		// 5875 소켓 옵션이 없어 제거할 수 없습니다.
		g_InterfaceManager.GetInterfaceString(5875 , szMessage , INTERFACE_STRING_LENGTH);
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT , szMessage );
		return FALSE;
	}
}

void uiSocketRemove::NET_SEND_CG_ITEM_SOCKET_EXTRACT_SYN()
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};
	if( MouseHandler::Instance()->GetTryRemoveSocket() )
		return;

	int index = m_pList->GetCurSel();
	if( index < 0)
		return;
	SocketState * pSocketState =  (SocketState *) m_pList->GetItemData(index) ;
	if( !pSocketState)
		return;
    int real_socket_index = 0;
    int step = 0;
    for (int i = 0; i < SOCKET_MAX; ++i)
    {
        if (zard_grades_[i] == 0)
        {
            continue;
        }
        if (step == index)
        {
            real_socket_index = zard_grades_[i];  // 인덱스와 실제 소켓 위치와 매치
            break;
        }
        ++step;
    }
    MouseHandler::Instance()->SetSocketRemoveType(real_socket_index);
	MouseHandler::Instance()->SetTryRemoveSocket(TRUE);
	MSG_CG_ITEM_SOCKET_EXTRACT_SYN sendMsg;
	sendMsg.m_ChargeIntemPos	= MouseHandler::Instance()->GetPendingSocketItemPos();
	sendMsg.m_TargetPos			= m_TargetSocketItemPos;
	sendMsg.m_SocketNum			= pSocketState->socketPos;

	GlobalFunc::SendPacket(CI_GAMESERVERIDX , &sendMsg , sizeof( MSG_CG_ITEM_SOCKET_EXTRACT_SYN) );
}