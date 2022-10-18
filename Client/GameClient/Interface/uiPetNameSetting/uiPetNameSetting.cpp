#include "SunClientPrecompiledHeader.h"

#include "uipetnamesetting.h"
#include "uiPetMan/uiPetMan.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "Hero.h"
#include "Pet.h"
#include "ObjectManager.h"
#include "ItemManager.h"
#include "SCItemSlot.h"
#include <SCSlotContainer.h>


//------------------------------------------------------------------------------
WzID uiPetNameSetting::m_wzId[PET_NAME_MAX] = 
{
	StrToWzID("ED00"),	//PET_NAME_EDIT = 0,
	StrToWzID("TI00"),	//PET_NAME_BTN_OK,	
	StrToWzID("TI01"),	//PET_NAME_BTN_CANCLE,
	StrToWzID("BT00"),  //PET_NAME_BTN_EXIT
};


uiPetNameSetting::uiPetNameSetting(InterfaceManager *pUIMan)
:uiBase(pUIMan),
 m_bInitDlg(FALSE),
 m_bWaitPacket(FALSE),
 m_pNameEditCtrl(NULL)
{
	
}
//------------------------------------------------------------------------------
/**
*/
uiPetNameSetting::~uiPetNameSetting(void)
{
	Release();
}
//------------------------------------------------------------------------------
/**
*/
VOID uiPetNameSetting::Init( CDrawBase * pDrawBase )
{
	uiBase::Init(pDrawBase);

	InitControls();
}
//------------------------------------------------------------------------------
/**
*/
void uiPetNameSetting::InitControls()
{
	ZeroMemory(&m_pPetBtnCtrl, sizeof(m_pPetBtnCtrl));

	m_pNameEditCtrl = static_cast<CCtrlDxIMEEditBox *>(getControl(PET_NAME_EDIT));
	assert(m_pNameEditCtrl);
	m_pNameEditCtrl->SetLimitText(MAX_PET_NAME_LENGTH);
	m_pNameEditCtrl->SetFontID( StrToWzID("mn12") );

	for(int i=0; i<PET_BTN_MAX; ++i)
	{
		m_pPetBtnCtrl[i]	= static_cast<CCtrlButtonWZ *>(getControl(PET_NAME_BTN_OK+i));
		assert(m_pPetBtnCtrl[i]);
	}
}
//------------------------------------------------------------------------------
/** ���̾�α� ���������
*/
void uiPetNameSetting::OnShowWindow( BOOL val )
{
	if (val)
	{
		//uiBase::MoveCenter();
		
		//�̸��ʱ�ȭ
		ZeroMemory(m_PetName, sizeof(m_PetName));
		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType = eDoType_dialog_Pet_NameSetting;
		msg.wParam = IM_PET_MANAGER::PET_NAME_DLG;
		msg.DoSomething = GlobalFunc::DoShowWindowEx;
		g_KeyQueueManager.PushBack(msg);
		m_pNameEditCtrl->ClearText();
		m_bInitDlg = TRUE;
	}
	else
	{
		if(!m_bInitDlg) return;
	
		m_bWaitPacket = FALSE;
		m_pNameEditCtrl->ClearText();
		SetLockNameCard(m_InvenPos,FALSE);
		g_KeyQueueManager.DeleteMsg(eDoType_dialog_Pet_NameSetting);
	}
}
//------------------------------------------------------------------------------
/** ����ī�尡 ��������� �������ϰ� �����ɿ����Ѵ�
*/
void uiPetNameSetting::SetLockNameCard(POSTYPE pos,BOOL bLock)
{
	if(!g_pHero) return;

	m_InvenPos = pos;
		
	SCItemSlot & rInvenItemSlot = (SCItemSlot&)(ItemManager::Instance()->GetContainer( SI_INVENTORY ))->GetSlot(m_InvenPos); 

	if(bLock)
		rInvenItemSlot.GetLaxLock()->EnterLock();
	else
		rInvenItemSlot.GetLaxLock()->Clear();
}
//------------------------------------------------------------------------------
/**
*/
VOID uiPetNameSetting::Release()
{


}
//-----------------------------------------------------------------------------
/** ����� ����ó��
*/
void uiPetNameSetting::PetNameError(int iStringCode)
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH];
	g_InterfaceManager.GetInterfaceString(iStringCode,szMessage);
	GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
	g_InterfaceManager.ShowMessageBox(szMessage, 1500, NULL, 0);
}
//-----------------------------------------------------------------------------
/** ACK��Ŷ�� �����ĸ� �̸��� �����ۿ� �����Ѵ�(Only Hero)
*/
void uiPetNameSetting::SetPetName()
{
	//�����ī�� �Ҹ� 
	ItemManager::Instance()->UseInventoryItem(SI_INVENTORY,m_InvenPos);

	//��Ŷ�����°� �ƴ϶�� â�� ������Ȳ? 
	if(!m_bWaitPacket) 
	{
		assert(m_bWaitPacket);
		return;
	}
	
	if(g_pHero->IsPetReturn())
	{
		SCItemSlot & rInvenItemSlot = (SCItemSlot&)(ItemManager::Instance()->GetContainer( SI_INVENTORY ))->GetSlot(g_pHero->GetPetItemPos()); 
		
		//�����ۿ� ���̸��� �����Ѵ�.
		rInvenItemSlot.GetPetInfo()->SetPetName(m_PetName);	
				
		Pet* pPet  = static_cast<Pet*>(g_ObjectManager.GetObject(g_pHero->GetPetObjectKey()));

		if(pPet)
		{
			pPet->RefreshPetName();
		}
	}
	this->ShowInterface(FALSE);
}
//-----------------------------------------------------------------------------
/** ������ ����� ������
*/
VOID uiPetNameSetting::SendPetName()
{
	//���ȯ���¿����� �����Ѵ�.
	if(!g_pHero || g_pHero->IsPetReturn()==FALSE) return;

	MSG_CG_ITEM_USE_CHANGE_PET_NAME_ITEM_SYN SendPacket;
	
	//�����ī���� �κ���ġ
	SendPacket.m_ItemPos = m_InvenPos;

	WCHAR wszPetName[INTERFACE_STRING_LENGTH+1] = {0,};
	TCHAR szPetName[INTERFACE_STRING_LENGTH+1] = {0,};
	m_pNameEditCtrl->GetTextWZW( wszPetName, INTERFACE_STRING_LENGTH );
	GlobalFunc::ChangeMultiEditInput(wszPetName, szPetName);
	

   //������ �����˻�
#if defined(_KOREA)
    if(g_slangNameFilter.CheckIncludeString(szPetName))
    {
        //6401 :�ش��̸��� ����Ҽ������ϴ�.
        PetNameError(6401);
        return;
    } 
#else
	if(GlobalFunc::CheckIncludeStringOnAbroad(szPetName))
    {
        //6401 :�ش��̸��� ����Ҽ������ϴ�.
        PetNameError(6401);
        return;
    } 
#endif//

	if((GlobalFunc::IsSlang(szPetName, false) == TRUE) || g_slangNameFilter.IsHaveReplace(szPetName))
	{
		//6401 :�ش��̸��� ����Ҽ������ϴ�.
		PetNameError(6401);
		return;
	}
   else
   {	
	   if(_tcslen(szPetName)==0)
	   {
		   //6405	�̸��� �Է��ϼ���
		   PetNameError(6405);
		   return;
	   }


	   if(_tcslen(szPetName)>(MAX_PET_NAME_LENGTH))
	   {
		   //6402 :��������
			PetNameError(6402);
	   }
	   else
	   {
		   m_bWaitPacket = TRUE;
		   ZeroMemory(SendPacket.m_PetName, sizeof(SendPacket.m_PetName));
		   StrnCopy(SendPacket.m_PetName, szPetName, MAX_PET_NAME_LENGTH);
		   
		   //���ο�����
		   StrnCopy(m_PetName, szPetName, MAX_PET_NAME_LENGTH);	

		   GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket) );		
	   }
   }
}
//-----------------------------------------------------------------------------
/** �޽��� PROC
*/
VOID uiPetNameSetting::MessageProc( SI_MESSAGE * pMessage )
{
	//��Ŷ�����¿����� �����Ѵ�.
	if(m_bWaitPacket) return;

	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_EDIT_ENTERKEYDOWN:
		{
			SendPetName();
		}
		break;

	case RT_MSG_LBUTTONCLICK:
		{
			switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
			{
			case PET_NAME_BTN_OK:
				{
					SendPetName();
				}
				break;
			
			//â�ݱ�
			case PET_NAME_BTN_EXIT:	
			case PET_NAME_BTN_CANCLE:
				{
					this->ShowInterface(FALSE);
				}
				break;
			}
		}
		break;
	}
}
