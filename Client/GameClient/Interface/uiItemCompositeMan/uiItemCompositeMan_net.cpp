#include "SunClientPrecompiledHeader.h"

#include "uiItemCompositeMan.h"
#include "SoundEffect.h"
#include "uiItemTotalComposite/uiItemTotalComposite.h"
#include "globalfunc.h"
#include "NPC.h"
#include "ObjectManager.h"
#include "HeroActionInput.h"
//------------------------------------------------------------------------------
/**
*/
void
uiItemCompositeMan::InitPKBooleans()
{
    this->m_vPKBool.clear();

    for (int i=ePKBOOL_ITEM_COMPOSITE_BASE; i<ePKBOOL_ITEM_COMPOSITE_MAX; ++i)
    {
        uicPacketControl packetControl;
        this->m_vPKBool.push_back(packetControl);
    }
}

//-------------------------------------------------------------------------------------------
/**
*/
//BOOL					
//uiItemCompositeMan::NET_SEND_CG_ITEM_COMPOSE_SYN()
//{
//	if (!this->isValidCompositeFlag())
//	{
//		// 특정 메시지
//		return FALSE;
//	}
//
//	if (!this->IsReadyForCompositeMaterials())
//	{
//		// 특정 메시지
//		return FALSE;
//	}
//
//	if (this->SafePKBool(ePKBOOL_ITEM_COMPOSITE_COMPOSE))
//	{
//		g_SoundEffect.PlayUI(UI_SOUND_COMPOSE_CRYSTALIZE_TRY);
//
//		this->SetPKBool(ePKBOOL_ITEM_COMPOSITE_COMPOSE);
//		 m_MsgComposeSyn.m_ComposeListCode = m_ePendingItemCompositeCode = m_eCurItemCompositeCode;	
//
//		return GlobalFunc::SendPacketToGameServer(&m_MsgComposeSyn, sizeof(m_MsgComposeSyn));
//	}
//	
//	return FALSE;
//}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL uiItemCompositeMan::NET_SEND_CG_ITEM_CRYSTALIZE_SYN( const POSTYPE& target_Pos, const SLOTCODE& slot_code )
{
    MSG_CG_ITEM_CRYSTALIZE_SYN MsgCrystalizeSyn;

    MsgCrystalizeSyn.m_CrystalizeListCode = slot_code;
    MsgCrystalizeSyn.m_TargetPos = target_Pos;
    MsgCrystalizeSyn.m_NPCCode = 0;
    return GlobalFunc::SendPacketToGameServer(&MsgCrystalizeSyn, sizeof(MsgCrystalizeSyn));
}

BOOL uiItemCompositeMan::NET_SEND_CG_TOTAL_ITEM_COMPOSITE_SYN()
{
	if (!this->isValidTotalCompositeFlag())
	{
		// 특정 메시지
		return FALSE;
	}

	if (!this->IsReadyForTotalCompositeMaterials())
	{
		// 특정 메시지
		return FALSE;
	}

	if (this->SafePKBool(ePKBOOL_ITEM_COMPOSITE_COMPOSE))
	{
		g_SoundEffect.PlayUI(UI_SOUND_COMPOSE_CRYSTALIZE_TRY);

		this->SetPKBool(ePKBOOL_ITEM_COMPOSITE_COMPOSE);
		m_MsgComposeSyn.m_ComposeListCode = m_ePendingItemCompositeCode = 
			(eITEMCOMPOSITECODE) m_pUIItemTotalComposite->GetCurrentSelectItemCode();	
		m_MsgComposeSyn.m_byMakingNum = m_ManualCompositeNum;
		m_PendingManualCompositeNum = m_ManualCompositeNum;
		m_MsgComposeSyn.m_NPCCode =	g_HeroInput.GetClickedNpcCode();

		return GlobalFunc::SendPacketToGameServer(&m_MsgComposeSyn, sizeof(m_MsgComposeSyn));
	}

	return FALSE;
}