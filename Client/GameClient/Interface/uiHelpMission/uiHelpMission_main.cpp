#include "SunClientPrecompiledHeader.h"

#include "uiHelpMission.h"
#include "uiHelpMan/uiHelpMan.h"
#include "uiSystemMan/uiSystemMan.h"
#include "Hero.h"
#include "GlobalFunc.h"
#include "ResourceManager.h"
#include "IconMgr.h"
#include "IconImage.h"
#include "interfacemanager.h"

#include "uiToolTipMan/uiToolTipMan.h"
#include "Mouse.h"


const DWORD c_ListSelBarColor	= RGBA(255,255,255,0);
const DWORD c_ListBKColor		= RGBA(255,255,255,0);
const DWORD c_ListTextColor		= RGBA(255,255,255,255);
const DWORD c_ListSelTextColor	= RGBA(255,255,255,255);

//------------------------------------------------------------------------------
WzID uiHelpMission::m_wzId[DIALOG_HM_MAX] = 
{
        StrToWzID("B001"), // DLGPOS_HM_BACK_1,
		StrToWzID("B002"), // DLGPOS_HM_BACK_2,
		StrToWzID("B003"), // DLGPOS_HM_BACK_3,
		StrToWzID("B004"), // DLGPOS_HM_BACK_4,

		StrToWzID("B006"), // DLGPOS_HM_ITEM_1,
		StrToWzID("B007"), // DLGPOS_HM_ITEM_2,
		StrToWzID("B008"), // DLGPOS_HM_ITEM_3,
		StrToWzID("B009"), // DLGPOS_HM_ITEM_4,

		StrToWzID("B011"), // DLGPOS_HM_BTN_OFF_1,
		StrToWzID("B013"), // DLGPOS_HM_BTN_OFF_2,
		StrToWzID("B015"), // DLGPOS_HM_BTN_OFF_3,
		StrToWzID("B017"), // DLGPOS_HM_BTN_OFF_4,

		StrToWzID("B010"), // DLGPOS_HM_BTN_ON_1,
		StrToWzID("B012"), // DLGPOS_HM_BTN_ON_2,
		StrToWzID("B014"), // DLGPOS_HM_BTN_ON_3,
		StrToWzID("B016"), // DLGPOS_HM_BTN_ON_4,

		StrToWzID("B020"), // DLGPOS_HM_BTN_WIN_OFF,
		StrToWzID("B019"), // DLGPOS_HM_BTN_WIN_ON,

		StrToWzID("S001"), // DLGPOS_HM_VSCR,

		StrToWzID("T001"), // DLGPOS_HM_TXT_ITEM_NAME_1,
		StrToWzID("T003"), // DLGPOS_HM_TXT_ITEM_NAME_2,
		StrToWzID("T005"), // DLGPOS_HM_TXT_ITEM_NAME_3,
		StrToWzID("T007"), // DLGPOS_HM_TXT_ITEM_NAME_4,

		StrToWzID("L001"), // DLGPOS_HM_LST_ITEM_DESC_1,
		StrToWzID("L002"), // DLGPOS_HM_LST_ITEM_DESC_2,
		StrToWzID("L003"), // DLGPOS_HM_LST_ITEM_DESC_3,
		StrToWzID("L004"), // DLGPOS_HM_LST_ITEM_DESC_4,

		StrToWzID("B005"), // DLGPOS_DLG_EVENTRECT
#ifdef _NA_000000_20130114_RENEWER_UI
		StrToWzID("P017"), //DLGPOS_PICTURE_ITEM_1
		StrToWzID("P018"), //DLGPOS_PICTURE_ITEM_2
		StrToWzID("P019"), //DLGPOS_PICTURE_ITEM_3
		StrToWzID("P020"), //DLGPOS_PICTURE_ITEM_4
#endif//#ifdef _NA_000000_20130114_RENEWER_UI
};

//------------------------------------------------------------------------------
/**
*/
uiHelpMission::uiHelpMission(InterfaceManager *pUIMan) :
    uiBase(pUIMan),
    m_bControlsInitialized(false)
{
    this->m_pHelpMan =
        static_cast<uiHelpMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_HELP));
    assert (m_pHelpMan);

    if (!this->m_pHelpMan)
    {
#ifdef _DEBUG
		 uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
			if(pSystemMan)
				pSystemMan->Sys_VerifyLock(_T("m_pHelpMan�� �����ϴ�."));
#endif
        this->ShowInterface(FALSE);
        return;
    }

    ZeroMemory(&m_HelpMissionItem, sizeof(m_HelpMissionItem));
}

//------------------------------------------------------------------------------
/**
*/
uiHelpMission::~uiHelpMission()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpMission::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

    this->InitControls();
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpMission::Release()
{
	_removeAllIconImage();
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpMission::InitControls()
{
	int i = 0;
	for(i=0; i<c_iMaxHelpNum; ++i)
	{
		m_pListItemDesc[i] = static_cast<CCtrlListWZ *>(this->getControl(DLGPOS_HM_LST_ITEM_DESC_1+i));
		assert (m_pListItemDesc[i]);
		
		if(m_pListItemDesc[i])
		{
			m_pListItemDesc[i]->SetUseReport(false);
			m_pListItemDesc[i]->SetSellHeight(14);
			m_pListItemDesc[i]->ShowWindowWZ(WZ_HIDE);
		}
	}

	for(i=0; i<c_iMaxHelpNum; ++i)
	{
	    m_pBtnBack[i] = static_cast<CCtrlButtonWZ *>(this->getControl(DLGPOS_HM_BACK_1+i));
		assert (m_pBtnBack[i]);

		if(m_pBtnBack[i])
			m_pBtnBack[i]->ShowWindowWZ(WZ_HIDE);
	}

	for(i=0; i<c_iMaxHelpNum; ++i)
	{
	    m_pBtnItemImage[i] = static_cast<CCtrlButtonWZ *>(this->getControl(DLGPOS_HM_ITEM_1+i));
		assert (m_pBtnItemImage[i]);

		if(m_pBtnItemImage[i])
			m_pBtnItemImage[i]->ShowWindowWZ(WZ_HIDE);
	}

	for(i=0; i<c_iMaxHelpNum; ++i)
	{
	    m_pBtnItemOff[i] = static_cast<CCtrlButtonWZ *>(this->getControl(DLGPOS_HM_BTN_OFF_1+i));
		assert (m_pBtnItemOff[i]);

		if(m_pBtnItemOff[i])
			m_pBtnItemOff[i]->ShowWindowWZ(WZ_HIDE);
	}

	for(i=0; i<c_iMaxHelpNum; ++i)
	{
	    m_pBtnItemOn[i] = static_cast<CCtrlButtonWZ *>(this->getControl(DLGPOS_HM_BTN_ON_1+i));
		assert (m_pBtnItemOn[i]);
	}
#ifdef _NA_000000_20130114_RENEWER_UI
	for(i=0; i<c_iMaxHelpNum; ++i)
	{
		m_pPictureItem[i] = static_cast<CCtrlPictureWZ *>(this->getControl(DLGPOS_PICTURE_ITEM_1+i));
		assert (m_pPictureItem[i]);

		if(m_pPictureItem[i])
		{
			m_pPictureItem[i]->ShowWindowWZ(WZ_HIDE);
		}
	}
#endif//#ifdef _NA_000000_20130114_RENEWER_UI
	for(i=0; i<c_iMaxHelpNum; ++i)
	{
	    m_pTxtItemName[i] = static_cast<CCtrlStaticWZ *>(this->getControl(DLGPOS_HM_TXT_ITEM_NAME_1+i));
		assert (m_pTxtItemName[i]);

		if(m_pTxtItemName[i])
			m_pTxtItemName[i]->ShowWindowWZ(WZ_HIDE);
	}

	m_pBtnOff = static_cast<CCtrlButtonWZ *>(this->getControl(DLGPOS_HM_BTN_WIN_OFF));
	if(m_pBtnOff)
		m_pBtnOff->ShowWindowWZ(WZ_HIDE);

	m_pBtnOn = static_cast<CCtrlButtonWZ *>(this->getControl(DLGPOS_HM_BTN_WIN_ON));
	assert (m_pBtnOn);
	
	m_pScroll = static_cast<CCtrlVScrollWZ *>(this->getControl(DLGPOS_HM_VSCR));
	assert (m_pScroll);
	
	

	m_pEventRect = static_cast<CCtrlButtonWZ *>(this->getControl(DLGPOS_DLG_EVENTRECT));
	if(m_pEventRect)
    {
		m_pEventRect->ShowWindowWZ(WZ_HIDE);
    }

	assert (m_pEventRect);


    this->m_bControlsInitialized = true;
}


//------------------------------------------------------------------------------
void uiHelpMission::renderToolTip(int nIndex,CControlWZ *pCtrl,RECT &rc)
{
	int iMouseX = Mouse::Instance()->GetMouseX();
	int iMouseY = Mouse::Instance()->GetMouseY();

	//�����ִ°͸� üũ�Ѵ�.
	if (rc.left <= iMouseX && rc.right >= iMouseX)
	{
		if (rc.top <= iMouseY && rc.bottom >= iMouseY)
		{
			uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);

			if(pUIMan)
			{
				TCHAR szDesc[INTERFACE_STRING_LENGTH] = {0,};
				g_InterfaceManager.GetInterfaceString(m_HelpMissionItem[nIndex].dwDCode, szDesc);
				pUIMan->RegisterTooltipSimpleText(szDesc);
				pUIMan->SetTooltipDialogID(GetDialogKey());
			}
		}
	}
}


//------------------------------------------------------------------------------
void uiHelpMission::OnShowWindow( BOOL val )
{
    if (val)
    {
        //MoveWindowWZ(0,0);	// TODO: DialogPosInfo.txt �� ��ġ ����� �Ǿ��ֳ� �׽�Ʈ �Ұ�

        if (!this->m_bControlsInitialized)
        {
            this->ShowInterface(FALSE);
            return;
        }
    

	}
    else
    {
		_removeAllIconImage();
    }
}


//------------------------------------------------------------------------------
/**
*/
BOOL uiHelpMission::_isEnable()
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
/** InterSectRect�� �������Ͽ��� ����üũ�� �޸��Ѵ�.
*/
BOOL uiHelpMission::InterSectRect( POINT point )
{
	RECT rc = m_pEventRect->GetSizeRect();

	if(DEF_IntersectRect(rc,point))
	{
		return TRUE;	
	}
	
	return FALSE;

}

//------------------------------------------------------------------------------
/**
*/
void uiHelpMission::_addIconImage(int iIndex, DWORD dwCode,DWORD dwDCode)
{

	m_HelpMissionItem[iIndex].dwDCode = dwDCode;	
	// �̼� ������ �̹���
	m_HelpMissionItem[iIndex].dwCode = dwCode;
	
	_removeIconImage(iIndex);
	
	if (dwCode != 0)
	{
		m_HelpMissionItem[iIndex].pImage  = ICONMGR()->Add(dwCode);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpMission::_removeIconImage(int iIndex)
{
	if(m_HelpMissionItem[iIndex].pImage)
	{
		ICONMGR()->RemoveIcon(m_HelpMissionItem[iIndex].pImage);
				
		m_HelpMissionItem[iIndex].pImage = NULL;
		
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpMission::_removeAllIconImage()
{
	for(int i=0; i<c_iMaxHelpNum;  ++i)
	{
		_removeIconImage(i);
	}
	ZeroMemory(&m_HelpMissionItem, sizeof(m_HelpMissionItem));
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpMission::UpdateDisplay(int iNewItemTextID/* = -1*/)
{
	if(!GetManager())
		return;

	_clearMissionItemList();	// ��ü�� Ŭ����
	
	int iSize = GetManager()->m_vecHelpMissionInfo.size() - 1;
	int iPage = 0;
	if(GetManager()->m_vecHelpMissionInfo.size() > c_iMaxHelpNum)
		iPage = GetManager()->m_vecHelpMissionInfo.size() - c_iMaxHelpNum;

	m_pScroll->SetScrollRangeWZ(0,iPage);	//��ũ�� ����

	int iMax=0;
	int iIndex = 0;
	bool bOpen = false;

	int iCurScrollPos = m_pScroll->GetScrollPosWZ();//���� ��ũ�� ���� �޾� �´�
	
	for(int i= iSize; i >= 0; --i)
	{
		if(iMax >= c_iMaxHelpNum)
			break;

		iIndex = i - iCurScrollPos;

		if((iIndex >= 0) && (iIndex <= iSize))
		{
			_setMissionItem(iMax, 
							GetManager()->m_vecHelpMissionInfo[iIndex].dwNCode, 
							GetManager()->m_vecHelpMissionInfo[iIndex].dwDCode, 
							GetManager()->m_vecHelpMissionInfo[iIndex].dwImgCode);
			
			
			if(iNewItemTextID == GetManager()->m_vecHelpMissionInfo[iIndex].iTextID)
			{
				// ���� �߰��Ǹ� Open
				bOpen = true;
			}

			++iMax;

		}
	}

	if(bOpen)
		this->ShowAllMissionItem(true);
	

}

void uiHelpMission::RefreshDisplay()
{
	if(!GetManager())
		return;
	
	_clearMissionItemList();	// ��ü�� Ŭ����

	int iSize = GetManager()->m_vecHelpMissionInfo.size() - 1;
	int iCurScrollPos = m_pScroll->GetScrollPosWZ();//���� ��ũ�� ���� �޾� �´�
	
	int iMax=0;
	int iIndex = 0;
	for(int i= iSize; i >= 0; --i)
	{
		if(iMax >= c_iMaxHelpNum)
			break;

		iIndex = i - iCurScrollPos;

		if((iIndex >= 0) && (iIndex <= iSize))
		{
			_setMissionItem(iMax, 
							GetManager()->m_vecHelpMissionInfo[iIndex].dwNCode, 
							GetManager()->m_vecHelpMissionInfo[iIndex].dwDCode, 
							GetManager()->m_vecHelpMissionInfo[iIndex].dwImgCode);
			++iMax;
		}

	}

	

}

//------------------------------------------------------------------------------
/**
*/
void uiHelpMission::_clearMissionItemList()
{
	for(int i=0; i<c_iMaxHelpNum; ++i)
	{
		if(m_pTxtItemName[i])
		{
			GlobalFunc::SetCaption(m_pTxtItemName[i], _T(""));
		}

		if(m_pListItemDesc[i])
		{
			m_pListItemDesc[i]->DeleteItemAll();
		}

		this->_removeIconImage(i);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpMission::_setMissionItem(int iIndex, DWORD dwNCode, DWORD dwDCode, DWORD dwImgCode)
{
	TCHAR szName[INTERFACE_STRING_LENGTH] = {0,};
	
	g_InterfaceManager.GetInterfaceString(dwNCode,szName, INTERFACE_STRING_LENGTH);

	if(m_pTxtItemName[iIndex])
	{
		GlobalFunc::SetCaption(m_pTxtItemName[iIndex], szName);
	}

	std::vector<util::_tstring> vstrDesc;
	vstrDesc.clear();
	TCHAR szDesc[INTERFACE_STRING_LENGTH] = {0,};
	g_InterfaceManager.GetInterfaceString(dwDCode, szDesc, INTERFACE_STRING_LENGTH);

	if(!_tcslen(szDesc))
		return;

	if(m_pListItemDesc[iIndex])
	{
		RECT rc = m_pListItemDesc[iIndex]->GetSizeRect();
#ifdef __LTJ_TEXT_OUTPUT_AT_LIST_MOD
		int iListWidth = rc.right - rc.left - 20;
#else
		int iListWidth = rc.right - rc.left - 10;
#endif//__LTJ_TEXT_OUTPUT_AT_LIST_MOD
		vstrDesc.clear();
		GlobalFunc::ProcessWordWrapByFontSize( &vstrDesc, szDesc, c_wiHelpMissionFont, _tcslen(szDesc), iListWidth );

		m_pListItemDesc[iIndex]->DeleteItemAll();
		m_pListItemDesc[iIndex]->SetColorSelBar(c_ListSelBarColor);
		int iListIndex = -1;
		size_t   nColume = 0;		//����Ʈ�� ���޽�������(col)
		BOOL	 bOverMsg= FALSE;	//�޽����� �Ѿ���� flag
		int		 nDotSize= 0;

		for (size_t i=0; i<vstrDesc.size(); ++i)
		{
			//STEP:�ϴ� �ѹ���üũ�Ѵ�(ĳ�������ۼ�����Ʈ���� ���� üũ)
			if(i==0)
			{
				//����Ʈ��� ������ ��´�.
				int nHeight = GlobalFunc::GetTextHeight(_T("A"),m_pListItemDesc[iIndex]->GetFontID());
				nColume = (m_pListItemDesc[iIndex]->GetSize().bottom)/(nHeight);
				if(vstrDesc.size()>1 && vstrDesc.size()>nColume)
				{
					bOverMsg = TRUE;
					nDotSize = GlobalFunc::GetTextWidth(_T("..."), m_pListItemDesc[iIndex]->GetFontID());
				}

			}

			iListIndex = m_pListItemDesc[iIndex]->InsertItemBack();

			TCHAR *pStr = const_cast<TCHAR *>(vstrDesc[i].c_str());

			//STEP2:���� ��ٸ� ����Ʈ ���ٿ� �ִ� �ؽ�Ʈ�� (...)�ٿ��ش�..�׸��� �����ع�����.
			if(bOverMsg)
			{
				if(i!=0 && (i+1)==nColume)
				{
					std::vector<util::_tstring> vstr;
					//1.���� �ؽ�Ʈ�� ����� üũ
					int nWidth = GlobalFunc::GetTextWidth(pStr, m_pListItemDesc[iIndex]->GetFontID());
					//2.�����ؽ�Ʈ������� ...�����ŭ �缭 ¥����.
					GlobalFunc::ProcessWordWrapByFontSize( &vstr, pStr, m_pListItemDesc[iIndex]->GetFontID(), _tcslen(pStr) , nWidth-nDotSize );
					//3.¥������ ...�� �ٽ� �ٿ��ش�.
					vstr[0]+=_T("...");
					pStr = const_cast<TCHAR *>(vstr[0].c_str());

					GlobalFunc::SetItem(
						m_pListItemDesc[iIndex], 
						iListIndex, 0, pStr, 
						c_wiHelpMissionFont,
						DT_VCENTER | DT_LEFT, 
						c_ListBKColor, 
						c_ListTextColor, 
						c_ListSelTextColor);
					break;
				}
			}

			GlobalFunc::SetItem(
				m_pListItemDesc[iIndex], 
				iListIndex, 0, pStr, 
				c_wiHelpMissionFont,
				DT_VCENTER | DT_LEFT, 
				c_ListBKColor, 
				c_ListTextColor, 
				c_ListSelTextColor);

		}   

	}
	_addIconImage(iIndex, dwImgCode,dwDCode);
	
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpMission::_showMissionItem(int iIndex, bool bShow)
{	
	ENUM_STATEWZ state;
	if(bShow)
		state = WZ_SHOW;
	else
		state = WZ_HIDE;

	if(m_pBtnBack[iIndex])
	{
		m_pBtnBack[iIndex]->ShowWindowWZ(state);
	}

    if(m_pBtnItemImage[iIndex])
	{
		m_pBtnItemImage[iIndex]->ShowWindowWZ(state);
	}

	if(m_pTxtItemName[iIndex])
	{
		m_pTxtItemName[iIndex]->ShowWindowWZ(state);
	}

	if(m_pListItemDesc[iIndex])
	{
		m_pListItemDesc[iIndex]->ShowWindowWZ(state);
	}

	if(state == WZ_SHOW)
	{
		if(m_pBtnItemOff[iIndex])
		{
			m_pBtnItemOff[iIndex]->ShowWindowWZ(WZ_SHOW);
		}

		if(m_pBtnItemOn[iIndex])
		{
			m_pBtnItemOn[iIndex]->ShowWindowWZ(WZ_HIDE);
		}
	}
	else
	{
		if(m_pBtnItemOff[iIndex])
		{
			m_pBtnItemOff[iIndex]->ShowWindowWZ(WZ_HIDE);
		}

		if(m_pBtnItemOn[iIndex])
		{
			m_pBtnItemOn[iIndex]->ShowWindowWZ(WZ_SHOW);
		}
	}
#ifdef _NA_000000_20130114_RENEWER_UI
	if(m_pPictureItem[iIndex])
	{
		m_pPictureItem[iIndex]->ShowWindowWZ(state);
	}
#endif//#ifdef _NA_000000_20130114_RENEWER_UI

	if(_isAllHideMissionItem())	// �����۸޴��� �� �����ִ�
	{
		if(m_pBtnOff)
			m_pBtnOff->ShowWindowWZ(WZ_HIDE);

		if(m_pBtnOn)
			m_pBtnOn->ShowWindowWZ(WZ_SHOW);
	}
	else
	{
		if(m_pBtnOff)
			m_pBtnOff->ShowWindowWZ(WZ_SHOW);

		if(m_pBtnOn)
			m_pBtnOn->ShowWindowWZ(WZ_HIDE);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpMission::ShowAllMissionItem(bool bShow)
{
	for(int i=0; i<c_iMaxHelpNum; ++i)
	{
		_showMissionItem(i, bShow);
	}

	if(bShow)
	{
		if(m_pBtnOff)
			m_pBtnOff->ShowWindowWZ(WZ_SHOW);

		if(m_pBtnOn)
			m_pBtnOn->ShowWindowWZ(WZ_HIDE);
	}
	else
	{
		if(m_pBtnOff)
			m_pBtnOff->ShowWindowWZ(WZ_HIDE);

		if(m_pBtnOn)
			m_pBtnOn->ShowWindowWZ(WZ_SHOW);
	}

}

//------------------------------------------------------------------------------
/**
*/
bool uiHelpMission::_isAllHideMissionItem()
{
	int iHideItem = 0;

	for(int i=0; i<c_iMaxHelpNum; ++i)
	{
		if(m_pBtnItemOn[i])
		{
			if(m_pBtnItemOn[i]->GetState() == WZ_SHOW)
				++iHideItem;
		}
	}

	if(iHideItem == c_iMaxHelpNum)
		return true;

	return false;
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpMission::MessageProc( SI_MESSAGE * pMessage )
{
    if (!_isEnable())
        return;

    switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_LBUTTONCLICK:
        {
            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
            case DLGPOS_HM_BTN_ON_1:
			    {
                    _showMissionItem(0, true);
                }
                break;

			case DLGPOS_HM_BTN_ON_2:
				{
                    _showMissionItem(1, true);
                }
                break;

			case DLGPOS_HM_BTN_ON_3:
				{
                    _showMissionItem(2, true);
                }
                break;

			case DLGPOS_HM_BTN_ON_4:
				{
                    _showMissionItem(3, true);
                }
                break;
            
			case DLGPOS_HM_BTN_OFF_1:
			    {
                    _showMissionItem(0, false);
                }
                break;

			case DLGPOS_HM_BTN_OFF_2:
				{
                    _showMissionItem(1, false);
                }
                break;

			case DLGPOS_HM_BTN_OFF_3:
				{
                    _showMissionItem(2, false);
                }
                break;

			case DLGPOS_HM_BTN_OFF_4:
				{
                    _showMissionItem(3, false);
                }
                break;

			case DLGPOS_HM_BTN_WIN_OFF:
				{
					ShowAllMissionItem(false);
				}
				break;

			case DLGPOS_HM_BTN_WIN_ON:
				{
					ShowAllMissionItem(true);
				}
				break;
			
            }
        }
        break;
	
	case RT_MSG_SB_PAGETOP:		//.���� ������ ������.
	case RT_MSG_SB_LINETOP:		//.���� ȭ��ǥ�� ������.
		{
			if(m_pScroll)
			{
				UpScroll();
			}
		}
		break;
	case RT_MSG_SB_PAGEBOTTOM:	//.�Ʒ��� ������ ������.
	case RT_MSG_SB_LINEBOTTOM:	//.�Ʒ��� ȭ��ǥ�� ������
		{
			if(m_pScroll)
			{
				DownScroll();
			}
		}
		break;

	case RT_MSG_SB_THUMBTRACK:				//. ���� �巡�� �ϰ� �ִ�.
	case RT_MSG_SB_THUMBPOSITION:			//. ���� �巡������ ���콺�� ���Ҵ�.
	case RT_MSG_SB_ENDSCROLL:				//. ��ũ���� ���� �Ǿ���.
		{
			RefreshDisplay();			
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
/** ��ũ�� DOWN �Լ��и�
*/
void uiHelpMission::DownScroll()
{

	int pos = m_pScroll->GetScrollPosWZ();//���� ��ũ�� ���� �޾� �´�.
	int mmin = 0 , mmax = 0;
	m_pScroll->GetScrollRangeWZ( &mmin, &mmax );//��ũ�� ���� �ִ� �ּҸ� ��� �´�.

	if(pos<mmax)// ���� �Էµ� ���μ� ���� ������ 
	{
		pos++;//���� ���� ��Ű�� 
		m_pScroll->SetScrollPosWZ(pos);
		RefreshDisplay();
	}
}
//----------------------------------------------------------------------------
/** ��ũ�� UP �Լ��и�
*/
void uiHelpMission::UpScroll()
{
	int pos = m_pScroll->GetScrollPosWZ();//���� ��ũ�� ���� �޾� �´�.
	int mmin = 0 , mmax = 0;
	m_pScroll->GetScrollRangeWZ( &mmin, &mmax );//��ũ�� ���� �ִ� �ּҸ� ��� �´�.

	if(pos>mmin)// ���� �Էµ� ���μ� ���� ������ 
	{
		pos--;//���� ���� ��Ű�� 
		m_pScroll->SetScrollPosWZ(pos);
		RefreshDisplay();
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiHelpMission::OnRenderSolarDialog()
{
	if (!_isEnable())
        return;

	for(int iIndex=0; iIndex<c_iMaxHelpNum;  ++iIndex)
	{
		CControlWZ *pControl = getControl(DLGPOS_HM_ITEM_1+iIndex);
		if(pControl)
		{
			if(pControl->GetState() == WZ_SHOW)
			{
				if (m_HelpMissionItem[iIndex].pImage && m_HelpMissionItem[iIndex].dwCode)
				{
					RECT rc = pControl->GetSizeRect();
					m_HelpMissionItem[iIndex].pImage->Render(&rc);
					renderToolTip(iIndex ,pControl,rc);
				}
			}
		}
	}
}

