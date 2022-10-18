#include "SunClientPrecompiledHeader.h"
#include "asknormalenchantdialog.h"
#include "Interfacemanager.h"
#include "iteminfoparser.h"
#include "globalfunc.h"

//------------------------------------------------------------------------------
/**
*/
AskNormalEnchantDialog::AskNormalEnchantDialog(InterfaceManager *pUIMan)
:AskDialog(pUIMan), m_EnchantItemCode(0)
{
	m_eNormalEnchantType = LOW_ENCHANT;
	m_PacketEnchantStatus = 0;
}

//------------------------------------------------------------------------------
/**
*/
AskNormalEnchantDialog::~AskNormalEnchantDialog(void)
{

}

//------------------------------------------------------------------------------
/**
*/
VOID AskNormalEnchantDialog::Process(DWORD dwTick)
{

}


//------------------------------------------------------------------------------
/**
*/
void AskNormalEnchantDialog::OnUpdateSolarDialog()
{
    if (this->GetStatus() == ASK_STATUS_OK)
    {
	    // 인첸트 실행
		if (!BIT_CHECK(this->m_PacketEnchantStatus, ENCHANT))
		{
			Enchant();
		}
			
		SetStatus(ASK_STATUS_NORMAL);
    }
    else if (this->GetStatus() == ASK_STATUS_CANCEL)
    {
        // 취소 
		this->Cancel();
		SetStatus(ASK_STATUS_NORMAL);
    }
}

//------------------------------------------------------------------------------
/**
*/
void AskNormalEnchantDialog::MessageProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_LBUTTONCLICK:
        {
            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
            case ASK_OK:
                {
                    this->m_status = ASK_STATUS_OK;
                }
                break;

            case ASK_CANCEL:
                {
                    this->m_status = ASK_STATUS_CANCEL;
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


//------------------------------------------------------------------------------
/**
*/
VOID AskNormalEnchantDialog::OnShowWindow( BOOL val )
{
    if (val)
    {
        SolarDialog::MoveCenter();

		assert(m_pListCtrl);

		SetTexts();
	}
	else
	{
		BIT_REMOVE(m_PacketEnchantStatus, ENCHANT);
	}
}

VOID AskNormalEnchantDialog::SetTexts()
{
	TCHAR subBuf[512];
	TCHAR szItemName[512];

	assert(m_pListCtrl);

    m_pListCtrl->DeleteItemAll();
    m_pListCtrl->SetBeginDrawIndex(0);

	BASE_ITEMINFO * pItemInfo = ItemInfoParser::Instance()->GetItemInfo(m_EnchantItemCode);
	if (pItemInfo )
	{
		// 여서 진행하는 패킷을 날리자. 
		if (LOW_ENCHANT == m_eNormalEnchantType)
		{
			// 하급 인첸트를 진행해라.
		}
		else
		{
			// 중급 인첸트를 진행해라.
		}

		g_InterfaceManager.GetItemName(m_EnchantItemCode, szItemName, 512);
		g_InterfaceManager.GetInterfaceString(723, subBuf, 512);

		int index = m_pListCtrl->InsertItemBack();
		GlobalFunc::SetItemText( m_pListCtrl, index, 0, szItemName );
		m_pListCtrl->SetItemTextColor(index, 0, RGBA(235,192,38, 255 ));
		m_pListCtrl->SetItemFontID( index,  0, StrToWzID("mn12"));
		m_pListCtrl->SetItemBKColor(index, 0, RGBA(0,0,0,255));

		index = m_pListCtrl->InsertItemBack();
		GlobalFunc::SetItemText( m_pListCtrl, index, 0, subBuf );
		m_pListCtrl->SetItemTextColor(index, 0, RGBA(235,192,38, 255 ));
		m_pListCtrl->SetItemFontID( index,  0, StrToWzID("mn12"));
		m_pListCtrl->SetItemBKColor(index, 0, RGBA(0,0,0,255));
		
	}

    m_pListCtrl->SetUnuseSelBar(TRUE);
    m_pListCtrl->OnFocusOut();
}

VOID AskNormalEnchantDialog::Enchant()
{
	BIT_ADD(m_PacketEnchantStatus, ENCHANT);

	ShowInterface(FALSE);
}


VOID AskNormalEnchantDialog::Cancel()
{
	BIT_REMOVE(m_PacketEnchantStatus, ENCHANT);
	// cancel
	ShowInterface(FALSE);
}


VOID AskNormalEnchantDialog::SetEnchantType(eNormalEnchantType eType)
{
	m_eNormalEnchantType = eType;
}


VOID AskNormalEnchantDialog::SetEnchantInfo(CODETYPE ItemCode, POSTYPE CrystalPos, POSTYPE TargetPos)
{
	this->m_EnchantItemCode = ItemCode;
	this->m_CrystalPos = CrystalPos;
	this->m_TargetPos = TargetPos;
}