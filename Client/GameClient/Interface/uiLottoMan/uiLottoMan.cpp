#include "SunClientPrecompiledHeader.h"
#include "uiLottoMan.h"
#include "InterfaceManager.h"
#include "ObjectManager.h"
#include "Character.h"
#include "LottoNPC.h"
#include "GlobalFunc.h"
#include "Hero.h"
#include "uiLottoWindow/uiLottoWindow.h"
#include "uiLottoNumInput/uiLottoNumInput.h"

const DWORD IM_LOTTO_MANAGER::LOTTO_WINDOW_DLG				= StrToWzID("9911");
const DWORD IM_LOTTO_MANAGER::LOTTO_NUMINPUT_DLG			= StrToWzID("9912");


uiLottoMan::uiLottoMan(InterfaceManager * pUIMAN)
:m_pUI_LottoWindow(NULL),
 m_dwLottoObjKey(0),
 m_bWaitPacket(false),
 m_pUI_LottoNumInput(NULL),
 uiBaseMan(pUIMAN)
{
	memset(&m_LottoInfo,0,sizeof(m_LottoInfo));
	
}


//-----------------------------------------------------------------------------
/** �޽������
*/
void uiLottoMan::LottoMessage(LPCTSTR szMessage,BOOL bCenterMSG)
{
	GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
	
	if(bCenterMSG)
		g_InterfaceManager.ShowMessageBox(szMessage, 5000, NULL, 0);
}
//------------------------------------------------------------------------------
/** �ζ� ������û ��Ŷ
*/
void uiLottoMan::SEND_LOTTO_INFO_SYN(BOOL bFlag)
{
	MSG_CW_LOTTO_INFO_SYN SendPacket;
	//������û�̳�?��ȣ��û�̳�
	SendPacket.bFlag = bFlag;
	BASE_PLAYERINFO *	pPlayInfo = g_pHero->GetCharInfo();
	if(pPlayInfo)
	{
		GlobalFunc::SendPacket( CI_CHATSERVERIDX, &SendPacket, sizeof(SendPacket));
#ifdef _DEV_VER
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, _T("MSG_CW_LOTTO_INFO_SYN"));
#endif 
	}
}
//------------------------------------------------------------------------------
/** ��÷�߿��� �ζǿ�����Ʈ ��Ȱ��ȭ�Ѵ�.
*/
void uiLottoMan::SetLottoActive(BOOL bActive)
{
    LottoNPC* lotto_npc_ptr = static_cast<LottoNPC*> (g_ObjectManager.GetObject(m_dwLottoObjKey));

	if(lotto_npc_ptr &&
       lotto_npc_ptr->IsKindOfObject(LOTTO_NPC_OBJECT))
	{
		lotto_npc_ptr->SetLottoActive(bActive);
	}
}
//------------------------------------------------------------------------------
/** �ζǿ�����Ʈ�� ���ΰ����� �Ÿ�üũ(�ָ� true)
*/
BOOL uiLottoMan::CheckDistanceNPC()
{
    LottoNPC* lotto_npc_ptr = static_cast<LottoNPC*> (g_ObjectManager.GetObject(m_dwLottoObjKey));

    if (lotto_npc_ptr &&
        g_pHero &&
        lotto_npc_ptr->IsKindOfObject(LOTTO_NPC_OBJECT))
    {
        WzVector vDiff = lotto_npc_ptr->GetPosition() - g_pHero->GetVisiblePos();

        if ( VectorLength2(&vDiff) > (DISTANCE_NPC_MEET * DISTANCE_NPC_MEET))
        {
            return TRUE;
        }
    }
    return FALSE;
}
//------------------------------------------------------------------------------
/** �ζ�â ������ �ݱ�(��÷���۽ó� �����)
*/
void uiLottoMan::closeAllLottoDlg()
{
	if(m_pUI_LottoWindow)
	{
		if(m_pUI_LottoWindow->IsVisible())
			m_pUI_LottoWindow->ShowInterface(FALSE);
			
	}

	if(m_pUI_LottoNumInput)
	{
		if(m_pUI_LottoNumInput->IsVisible())
			m_pUI_LottoNumInput->ShowInterface(FALSE);
	}
}
//------------------------------------------------------------------------------
/** �ζ� ������ ���
*/
void uiLottoMan::SetLottoNumber(INT64 Number)
{
	LottoNPC* lotto_npc_ptr = static_cast<LottoNPC*> (g_ObjectManager.GetObject(m_dwLottoObjKey));

	if (lotto_npc_ptr &&
        lotto_npc_ptr->IsKindOfObject(LOTTO_NPC_OBJECT))
	{
		lotto_npc_ptr->SetDrawNumber(Number);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiLottoMan::OnInitialize(void)
{
	m_pUI_LottoWindow			= CreateUIDialog<uiLottoWindow>( IM_LOTTO_MANAGER::LOTTO_WINDOW_DLG, "Data\\Interface\\99_11_HeimLotto.iwz", this );
	m_pUI_LottoNumInput			= CreateUIDialog<uiLottoNumInput>( IM_LOTTO_MANAGER::LOTTO_NUMINPUT_DLG, "Data\\Interface\\99_12_HeimLottoPopup.iwz", this );
}

