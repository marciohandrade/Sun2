#include "SunClientPrecompiledHeader.h"
#include "uiDominationTowerManagerDlg.h"

#ifdef _NA_006953_20131114_DOMINATION_MANAGER_UI_RENEWAL
#else

#include "interfacemanager.h"
#include "globalfunc.h"
#include "uiSystemMan/uiSystemMan.h"
#include "ItemManager.h"


//------------------------------------------------------------------------------
 WzID uiDominationTowerManagerDlg::m_wzId[DIALOG_MAX] = 
 {
	StrToWzID("ST05"),//	DLGPOS_MANAGE_NOTICE_TEXT,
	StrToWzID("ST01"),//	DLGPOS_REMAINPOINT_TEXT,

	StrToWzID("BT05"),//	DLGPOS_LEFTGATE_DEC_BUTTON,
	StrToWzID("BT02"),//	DLGPOS_LEFTGATE_INC_BUTTON,
	StrToWzID("BT06"),//	DLGPOS_CENTERGATE_DEC_BUTTON,
	StrToWzID("BT07"),//	DLGPOS_CENTERGATE_INC_BUTTON,
	StrToWzID("BT08"),//	DLGPOS_RIGHTGATE_DEC_BUTTON,	
	StrToWzID("BT09"),//	DLGPOS_RIGHTGATE_INC_BUTTON,

    StrToWzID("B000"),//	DLGPOS_POINT_ADD_BUTTON,
    StrToWzID("P000"),//	DLGPOS_POINT_ADD_EFFECT,

	StrToWzID("CT00"),//	DLGPOS_LEFTSTAT1_PIC,
	StrToWzID("CT01"),//	DLGPOS_LEFTSTAT2_PIC,
	StrToWzID("CT02"),//	DLGPOS_LEFTSTAT3_PIC,
	StrToWzID("CT03"),//	DLGPOS_LEFTSTAT4_PIC,
	StrToWzID("CT04"),//	DLGPOS_LEFTSTAT5_PIC,

	StrToWzID("CT09"),//	DLGPOS_CENTERSTAT1_PIC,
	StrToWzID("CT08"),//	DLGPOS_CENTERSTAT2_PIC,
	StrToWzID("CT07"),//	DLGPOS_CENTERSTAT3_PIC,
	StrToWzID("CT06"),//	DLGPOS_CENTERSTAT4_PIC,
	StrToWzID("CT05"),//	DLGPOS_CENTERSTAT5_PIC,

	StrToWzID("CT14"),//	DLGPOS_RIGHTSTAT1_PIC,
	StrToWzID("CT13"),//	DLGPOS_RIGHTSTAT2_PIC,
	StrToWzID("CT12"),//	DLGPOS_RIGHTSTAT3_PIC,
	StrToWzID("CT11"),//	DLGPOS_RIGHTSTAT4_PIC,
	StrToWzID("CT10"),//	DLGPOS_RIGHTSTAT5_PIC,	

	//
	StrToWzID("PI00"),//	DLGPOS_LEFTSTAT1_GRAPH_PIC,
	StrToWzID("PI02"),//	DLGPOS_LEFTSTAT2_GRAPH_PIC,
	StrToWzID("PI03"),//	DLGPOS_LEFTSTAT3_GRAPH_PIC,
	StrToWzID("PI04"),//	DLGPOS_LEFTSTAT4_GRAPH_PIC,
	StrToWzID("PI05"),//	DLGPOS_LEFTSTAT5_GRAPH_PIC,

	StrToWzID("PI06"),//	DLGPOS_CENTERSTAT1_GRAPH_PIC,
	StrToWzID("PI07"),//	DLGPOS_CENTERSTAT1_GRAPH_PIC,
	StrToWzID("PI08"),//	DLGPOS_CENTERSTAT1_GRAPH_PIC,
	StrToWzID("PI09"),//	DLGPOS_CENTERSTAT1_GRAPH_PIC,
	StrToWzID("PI10"),//	DLGPOS_CENTERSTAT1_GRAPH_PIC,

	StrToWzID("PI11"),//	DLGPOS_RIGHTSTAT1_GRAPH_PIC,
	StrToWzID("PI12"),//	DLGPOS_RIGHTSTAT2_GRAPH_PIC,
	StrToWzID("PI13"),//	DLGPOS_RIGHTSTAT3_GRAPH_PIC,
	StrToWzID("PI14"),//	DLGPOS_RIGHTSTAT4_GRAPH_PIC,
	StrToWzID("PI15"),//	DLGPOS_RIGHTSTAT5_GRAPH_PIC,	

	StrToWzID("BT00"),//	DLGPOS_SETTING_BUTTON,
	StrToWzID("BT01"),//	DLGPOS_INIT_BUTTON,
	StrToWzID("BT04"),//	DLGPOS_CANCEL_BUTTON,
	StrToWzID("B999"),//	DLGPOS_CLOSE_BUTTON, 
 };

//------------------------------------------------------------------------------
/**
*/
uiDominationTowerManagerDlg::uiDominationTowerManagerDlg(InterfaceManager *pUIMan)
:uiBase(pUIMan)
{
	m_pNoticeText = NULL;
	m_pRemainText = NULL;
	m_pLeftDecButton = NULL;
	m_pLeftIncButton = NULL;
	m_pCenterDecButton = NULL;
	m_pCenterIncButton = NULL;
	m_pRightDecButton = NULL;
	m_pRightIncButton = NULL;
	
	for (int i = 0; i < STATPIC_MAX; ++i)
	{
		m_pStatPointPic[i] = NULL;
		m_pStatGraphPic[i] = NULL;
	}

	m_pSettingButton = NULL;
	m_pInitButton = NULL;
	m_pCancelButton = NULL;
	m_pCloseButton = NULL;

    m_pAddEffect = NULL;
    animate_end_tick_ = 0;
    animate_now_tick_ = 0;
    animate_current_tick_ = 0;
    add_block_button_ = NULL;
}

//------------------------------------------------------------------------------
/**
*/
uiDominationTowerManagerDlg::~uiDominationTowerManagerDlg()
{
	this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiDominationTowerManagerDlg::Init(CDrawBase* pDrawBase)
{
	uiBase::Init(pDrawBase);

	m_pNoticeText = static_cast<CCtrlStaticWZ*>(GetControlWZ(m_wzId[DLGPOS_MANAGE_NOTICE_TEXT]));
	assert(m_pNoticeText);
	m_pRemainText = static_cast<CCtrlStaticWZ*>(GetControlWZ(m_wzId[DLGPOS_REMAINPOINT_TEXT]));
	assert(m_pRemainText);
	m_pLeftDecButton = static_cast<CCtrlButtonWZ*>(GetControlWZ(m_wzId[DLGPOS_LEFTGATE_DEC_BUTTON]));
	assert(m_pLeftDecButton);
	m_pLeftIncButton = static_cast<CCtrlButtonWZ*>(GetControlWZ(m_wzId[DLGPOS_LEFTGATE_INC_BUTTON]));
	assert(m_pLeftIncButton);
	m_pCenterDecButton = static_cast<CCtrlButtonWZ*>(GetControlWZ(m_wzId[DLGPOS_CENTERGATE_DEC_BUTTON]));
	assert(m_pCenterDecButton);
	m_pCenterIncButton = static_cast<CCtrlButtonWZ*>(GetControlWZ(m_wzId[DLGPOS_CENTERGATE_INC_BUTTON]));
	assert(m_pCenterIncButton);
	m_pRightDecButton = static_cast<CCtrlButtonWZ*>(GetControlWZ(m_wzId[DLGPOS_RIGHTGATE_DEC_BUTTON]));
	assert(m_pRightDecButton);
	m_pRightIncButton = static_cast<CCtrlButtonWZ*>(GetControlWZ(m_wzId[DLGPOS_RIGHTGATE_INC_BUTTON]));
	assert(m_pRightIncButton);
	
	// TODO:picture 컨트롤로 교체 요청
	for (int index = 0; index < STATPIC_MAX; ++index)
	{
		m_pStatPointPic[index] = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ(m_wzId[DLGPOS_LEFTSTAT1_PIC+index]));
		assert(m_pStatPointPic[index]);
		//m_pStatPointPic[index]->EnableWindowWZ(FALSE);
        m_pStatPointPic[index]->SetCheckState(false);
		m_pStatGraphPic[index] = static_cast<CCtrlPictureWZ*>(GetControlWZ(m_wzId[DLGPOS_LEFTSTAT1_GRAPH_PIC+index]));
		assert(m_pStatGraphPic[index]);
	}

    m_pAddEffect = static_cast<CCtrlPictureWZ*>(GetControlWZ(m_wzId[DLGPOS_POINT_ADD_EFFECT]));
    assert(m_pAddEffect);
    if (m_pAddEffect)
    {
        m_pAddEffect->ShowWindowWZ(WZ_HIDE);
    }

    add_block_button_ = static_cast<CCtrlButtonWZ*>(GetControlWZ(m_wzId[DLGPOS_POINT_ADD_BUTTON]));
    assert(add_block_button_);
    if (add_block_button_)
    {
        add_block_button_->ShowWindowWZ(WZ_HIDE);
    }

	m_pSettingButton = static_cast<CCtrlButtonWZ*>(GetControlWZ(m_wzId[DLGPOS_SETTING_BUTTON]));
	assert(m_pSettingButton);
	m_pInitButton = static_cast<CCtrlButtonWZ*>(GetControlWZ(m_wzId[DLGPOS_INIT_BUTTON]));
	assert(m_pInitButton);
	m_pCancelButton = static_cast<CCtrlButtonWZ*>(GetControlWZ(m_wzId[DLGPOS_CANCEL_BUTTON]));
	assert(m_pCancelButton);
	m_pCloseButton = static_cast<CCtrlButtonWZ*>(GetControlWZ(m_wzId[DLGPOS_CLOSE_BUTTON]));
	assert(m_pCloseButton);
}

//------------------------------------------------------------------------------
/**
*/
void uiDominationTowerManagerDlg::OnShowWindow(BOOL val)
{
	if (val)
	{
		//MoveDefaultWindowPos();
        
        g_InterfaceManager.AttachFirstDialog(GetDialogWZ()->GetDlgID());

		//키큐메시지삽입
		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType=eDoType_dialog_Domination_ManagerDlg;
		msg.wParam=IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_MANAGER_DIALOG;
		msg.DoSomething=GlobalFunc::DoShowWindowEx;
		g_KeyQueueManager.PushBack(msg);
	}
	else
	{
		//키큐메시지삭제
		g_KeyQueueManager.DeleteMsg(eDoType_dialog_Domination_ManagerDlg);

        g_InterfaceManager.DeAttachFirstDialog(GetDialogWZ()->GetDlgID());
	}
}

//------------------------------------------------------------------------------ 
void CallbackSendPacketAddDeffenseWallSYN(bool is_yes)
{
    if (is_yes)
    {
        uiDominationTowerMan* pUIMan = GET_DIALOG_MANAGER(uiDominationTowerMan, UIMAN_DOMINATIONWAR_TOWER);
        if (pUIMan && !pUIMan->IsPKPending())
        {
            pUIMan->SEND_DOMINATION_ADD_DEFFENSEWALL_SYN();
        }
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiDominationTowerManagerDlg::MessageProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_LBUTTONCLICK:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );
			CControlWZ * pControl = GetControlWZ(m_wzId[pos]);
			if (NULL == pControl)
			{
				return;
			}

			// 패킷 펜딩셋 상태면 리턴
			uiDominationTowerMan* pUIMan = (uiDominationTowerMan*)GetMan();
			if (NULL == pUIMan || pUIMan->IsPKPending())
			{
				return;
			}			

			switch(pos)
			{
			case DLGPOS_LEFTGATE_DEC_BUTTON:
				{
					m_byDefenseWall[eLEFT]+= (-1);
					m_byRemainPoint++;
					if (CheckPointIntergrity())
					{						
						ShowPictureControlByPoint();
					}
					else
					{
						m_byDefenseWall[eLEFT]+= (1);
						m_byRemainPoint--;
					}					
				}
				break;
			case DLGPOS_LEFTGATE_INC_BUTTON:
				{
					m_byDefenseWall[eLEFT]+= (1);
					m_byRemainPoint--;
					if (CheckPointIntergrity())
					{						
						ShowPictureControlByPoint();
					}
					else
					{
						m_byDefenseWall[eLEFT]+= (-1);
						m_byRemainPoint++;
					}					
				}
				break;
			case DLGPOS_CENTERGATE_DEC_BUTTON:
				{
					m_byDefenseWall[eCENTER]+= (-1);
					m_byRemainPoint++;
					if (CheckPointIntergrity())
					{						
						ShowPictureControlByPoint();
					}
					else
					{
						m_byDefenseWall[eCENTER]+= (1);
						m_byRemainPoint--;
					}					
				}
				break;
			case DLGPOS_CENTERGATE_INC_BUTTON:
				{
					m_byDefenseWall[eCENTER]+= (1);
					m_byRemainPoint--;
					if (CheckPointIntergrity())
					{						
						ShowPictureControlByPoint();
					}
					else
					{
						m_byDefenseWall[eCENTER]+= (-1);
						m_byRemainPoint++;
					}					
				}
				break;
			case DLGPOS_RIGHTGATE_DEC_BUTTON:
				{
					m_byDefenseWall[eRIGHT]+= (-1);
					m_byRemainPoint++;
					if (CheckPointIntergrity())
					{						
						ShowPictureControlByPoint();
					}
					else
					{
						m_byDefenseWall[eRIGHT]+= (1);
						m_byRemainPoint--;
					}					
				}
				break;
			case DLGPOS_RIGHTGATE_INC_BUTTON:
				{
					m_byDefenseWall[eRIGHT]+= (1);
					m_byRemainPoint--;
					if (CheckPointIntergrity())
					{						
						ShowPictureControlByPoint();
					}
					else
					{
						m_byDefenseWall[eRIGHT]+= (-1);
						m_byRemainPoint++;
					}					
				}
				break;
			case DLGPOS_SETTING_BUTTON:
				{
					// 세팅하기					
					pUIMan->SEND_DOMINATION_SAVE_GATESTAT_SYN(m_byDefenseWall);
					ShowInterface(FALSE);
				}
				break;
			case DLGPOS_INIT_BUTTON:
				{
					// 초기화
					InitPoint(m_byTotalPoint);
					ShowPictureControlByPoint();
				}
				break;
            case DLGPOS_POINT_ADD_BUTTON:
                {
                    uiSystemMan* system_main = (uiSystemMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
                    if (system_main != NULL && animate_end_tick_ == 0)
                    {
                        TCHAR message[INTERFACE_STRING_LENGTH] = {NULL, };						
                        // 3449 방어 블록을 추가하시겠습니까? 확인 시 아이템이 소멸되며 반환이 불가능합니다.
                        g_InterfaceManager.GetInterfaceString(3449, message, INTERFACE_STRING_LENGTH);
                        system_main->Sys_ConfirmLock(message, CallbackSendPacketAddDeffenseWallSYN);
                    }
                }
                break;
			case DLGPOS_CANCEL_BUTTON: 
			case DLGPOS_CLOSE_BUTTON: 
				{
					ShowInterface(FALSE);										
				}
				break;			
			default:
				{

				}
			}
		}	
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiDominationTowerManagerDlg::InitPoint(BYTE byTotalPoint)
{
	m_byTotalPoint = byTotalPoint;
	m_byRemainPoint = m_byTotalPoint;
	for (int index = 0; index < MAX_DOMINATION_DEFENSEWALL_NUM; ++index)
	{
		m_byDefenseWall[index] = 0;
	}
}

void uiDominationTowerManagerDlg::ShowPictureControlByPoint()
{
	for (int index = 0; index < MAX_DOMINATION_DEFENSEWALL_NUM; ++index)
	{
		for (int point = 0; point < eMAXPOINTBYWALL ; ++point)
		{
			if (m_byDefenseWall[index] > point)
			{
				m_pStatPointPic[index*eMAXPOINTBYWALL+point]->SetCheckState(true);
				m_pStatGraphPic[index*eMAXPOINTBYWALL+point]->ShowWindowWZ(WZ_SHOW);
			}
			else
			{
				m_pStatPointPic[index*eMAXPOINTBYWALL+point]->SetCheckState(false);
				m_pStatGraphPic[index*eMAXPOINTBYWALL+point]->ShowWindowWZ(WZ_HIDE);
			}			
		}		
	}
	TCHAR szText[INTERFACE_STRING_LENGTH] = {0, };
	_itot((int)m_byRemainPoint, szText, 10);
	m_pRemainText->SetTextWZ(szText);	
}

//------------------------------------------------------------------------------
/**
*/
void uiDominationTowerManagerDlg::SetDefenseWall(BYTE dwTotalPoint, BYTE* pDefenseWall)
{
	m_byRemainPoint = dwTotalPoint;
	InitPoint(dwTotalPoint);
	if ( NULL != pDefenseWall && 0 != dwTotalPoint)
	{
		m_byTotalPoint = dwTotalPoint;
		BYTE dwPointSum = 0;
		for (int index = 0; index < MAX_DOMINATION_DEFENSEWALL_NUM; ++index)
		{
			m_byDefenseWall[index] = pDefenseWall[index];
			dwPointSum += m_byDefenseWall[index];
		}
		m_byRemainPoint -= dwPointSum;
	}	
	ShowPictureControlByPoint();		
}

bool uiDominationTowerManagerDlg::CheckPointIntergrity()
{
	DWORD dwPointSum = 0;
	for (int index = 0; index < MAX_DOMINATION_DEFENSEWALL_NUM; ++index)
	{
		if (m_byDefenseWall[index] > eMAXPOINTBYWALL)
		{
			return false;
		}
		dwPointSum += m_byDefenseWall[index];
	}
	
	if (dwPointSum > m_byTotalPoint || dwPointSum + m_byRemainPoint > m_byTotalPoint)
	{
		return false;
	}	
	return true;
}

void uiDominationTowerManagerDlg::OnUpdateSolarDialog()
{
    const int kMaxDeffenseWallPoint = (MAX_DOMINATION_DEFENSEWALL_NUM * DOMINATION_DEFFENSWALL_POINT);
    if (add_block_button_ == NULL)
    {
        return;
    }

    POSTYPE inventory_position = INVALID_POSTYPE_VALUE;
    // 일반 방어블럭 아이템을 먼저 찾고
    BOOL result = ItemManager::Instance()->FindFirstItem(eITEMTYPE_DOMINATION_DEFFENSE_POINT, inventory_position);
    if (result == FALSE) // 없으면
    {   // 캐쉬 방어블럭 아이템을 찾는다.
        result = ItemManager::Instance()->FindFirstItem(eITEMTYPE_DOMINATION_DEFFENSE_CASH_POINT, inventory_position);
    }

    if (m_byTotalPoint >= kMaxDeffenseWallPoint)
    {
        result = FALSE;// 15개 이상 추가할 수 없다.
    }

    if (result && inventory_position != INVALID_POSTYPE_VALUE) // 일반 혹은 캐쉬 방어블럭이 있으면
    {
        add_block_button_->ShowWindowWZ(WZ_SHOW);
    }
    else
    {
        add_block_button_->ShowWindowWZ(WZ_HIDE);
    }
}

void uiDominationTowerManagerDlg::OnRenderSolarDialog()
{
    if (animate_end_tick_ > 0)
    {
        animate_now_tick_ = clock_function::GetTickCount();

        if (animate_now_tick_ < animate_end_tick_)
        {
            int remain_tick = (animate_now_tick_ - animate_current_tick_);
            if (remain_tick >= 80)
            {
                remain_tick = (remain_tick - 50) % 50;
                animate_current_tick_ = animate_now_tick_ + remain_tick; // 남은 ms를 추가
                AnimateEffect();
            }
        }
        else
        {
            animate_end_tick_ = 0;
            animate_current_tick_ = 0;
            if (m_pAddEffect)
            {
                m_pAddEffect->ShowWindowWZ(WZ_HIDE);
            }            
        }
    }

    if (m_byRemainPoint > 5)
    {
        m_pRemainText->SetTextColor(RGBA(255,0,0,255));
        //m_pRemainText->SetFontID(Str);
    }
    else
    {
        m_pRemainText->SetTextColor(RGBA(255,255,255,255));
    }
}
void uiDominationTowerManagerDlg::AnimateEffect()
{
    if (m_pAddEffect)
    {
        if (m_pAddEffect->GetState() == WZ_SHOW)
        {
            m_pAddEffect->ShowWindowWZ(WZ_HIDE);
        }
        else
        {
            m_pAddEffect->ShowWindowWZ(WZ_SHOW);
        }
    }
}

void uiDominationTowerManagerDlg::SetAnimation()
{
    animate_current_tick_ = clock_function::GetTickCount();
    animate_end_tick_ = animate_current_tick_ + 1500; // 1.5초간 깜빡 깜빡

    ++m_byTotalPoint;
    ++m_byRemainPoint;

    TCHAR temp_string[128] = {0,};
    _itot((int)m_byRemainPoint, temp_string, 10);
    m_pRemainText->SetTextWZ(temp_string);	
}
//------------------------------------------------------------------------------
/**
*/
#endif //_NA_006953_20131114_DOMINATION_MANAGER_UI_RENEWAL