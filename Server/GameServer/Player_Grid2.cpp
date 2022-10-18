#include "stdafx.h"

//==================================================================================================
//
#include "Player.h"
#include "GameField.h"
#include "Sector.h"
#include "ObjectDefine.h"
#include "Item.h"
#include "GameZone.h"
#include "TriggerManager.h"
#include "StatusManager.h"
#include "PacketStruct_AG.h"
#include "ServerOptionParser.h"
#include "ChaoState.h"
#include "DragonTransStatus.h"
#include "MovementChecker.h"
#include "TriggerManagerEx.h"
#include "View.h"
#include <ServerOptionParserEx.h>


#include "Player_Grid_Operator.h"
#include "Collection.h"
#include "CollectionManager.h"

//==================================================================================================
//
// 주변 정보를 자신에게
void
Player::OnLeaveNeighbor(const Sector& IN rSector)
{
	// 주변정보를 바로 받을 필요가 없다. 어차피 주기적으로 주변을 체크해서 정보를 갱신한다.
	const BOOLEAN bPlayerInfoSend = true; //!m_pViewPort->IsUsingViewPort();

	if(bPlayerInfoSend)
	{
		const DWORD playerObjectKey = GetObjectKey();
		MSG_CG_SYNC_PLAYER_LEAVE_BRD msgBRD;
        BYTE count = 0;

        ns_object::SearchNodeConstIterator it = rSector.GetPlayerRoot();
        const ns_object::SearchNodeConstIterator end = rSector.GetPlayerEnd();

        while (const ns_object::SearchNode::LinkNode* link_node = it.ClimbToNext(end))
        {
            Player* pPlayer = link_node->select.player_node.player;
            if (obj_util::IsValid(pPlayer) == false)
            {
                continue;
            }
			if(this == pPlayer)
				continue;

			msgBRD.m_PlayerKey[count++] = (WORD)pPlayer->GetObjectKey();	// (0, 50000]

			if(MSG_CG_SYNC_PLAYER_LEAVE_BRD::_MAX_PLAYER_SIZE == count)
			{
				msgBRD.m_byCount = count;
				this->SendPacket(&msgBRD, (WORD)msgBRD.GetSize());
				count = 0;
			}
		}

		if(count)
		{
			msgBRD.m_byCount = count;
			this->SendPacket(&msgBRD, (WORD)msgBRD.GetSize());
		}
	};

	//if(bPlayerInfoSend)
	{	// 몬스터 정보
		MSG_CG_SYNC_MONSTER_LEAVE_BRD msgBRD;
        BYTE count = 0;

        ns_object::SearchNodeConstIterator it = rSector.GetNpcRoot();
        const ns_object::SearchNodeConstIterator end = rSector.GetNpcEnd();

        while (const ns_object::SearchNode::LinkNode* link_node = it.ClimbToNext(end))
        {
            NPC* pNpc = link_node->select.npc_node.npc;
            if (obj_util::IsValid(pNpc) == false)
            {
                continue;
            }

			msgBRD.m_dwObjectKey[count++] = pNpc->GetObjectKey();

			if(MSG_CG_SYNC_MONSTER_LEAVE_BRD::_MAX_MONSTER_SIZE == count)
			{
				msgBRD.m_byCount = count;
				this->SendPacket(&msgBRD, (WORD)msgBRD.GetSize());
				count = 0;
			}
		}

		if(count)
		{
			msgBRD.m_byCount = count;
			this->SendPacket(&msgBRD, (WORD)msgBRD.GetSize());
		}
	};

	//if(bPlayerInfoSend)
	{	// 채집물 정보
		MSG_CG_COLLECT_LEAVE_SECTOR_CMD msgCMD;
		CollectionManager* pMgr = GetField()->GetCollectManeger();
        BYTE count = 0;

        ns_object::SearchNodeConstIterator it = rSector.GetEtcRoot();
        const ns_object::SearchNodeConstIterator end = rSector.GetEtcEnd();

        while (const ns_object::SearchNode::LinkNode* link_node = it.ClimbToNext(end))
        {
            const eOBJECT_TYPE object_type = link_node->select.object_node.object_type;
            if ((object_type & COLLECTION_OBJECT) != COLLECTION_OBJECT) {
                continue;
            }
            Collection* pCollection = static_cast<Collection*>(link_node->select.object_node.object);
            if (obj_util::IsValid(pCollection) == false)
            {
                continue;
            }
            //_NA_004777_20120428_ADD_COMPLETE_COLLECTION_TRIGGER
            if (pCollection->is_active() == false)
            {
                continue;
            }

            //
			MAPOBJECT_INFO* pMapInfo = pCollection->GetMapObjectInfo();
			RC::eCOLLECT_RESULT rc   = pMgr->ChkCondision(this,pMapInfo->CollectionID);
			if(RC::RC_COLLECT_SUCCESS != rc)
				continue;

			msgCMD.m_ObjKeyCollection[count++] = pCollection->GetObjectKey();

			if(MSG_CG_COLLECT_LEAVE_SECTOR_CMD::_MAX_FIELDCOLLECT_INFO_SIZE == count)
			{
				msgCMD.m_byCount = count;
				this->SendPacket(&msgCMD, (WORD)msgCMD.GetSize());
				count = 0;
			}
		}

		if(count)
		{
			msgCMD.m_byCount = count;
			this->SendPacket(&msgCMD, (WORD)msgCMD.GetSize());
		}
	};
	//if(bPlayerInfoSend)
	{	// 아이템 정보
		MSG_CG_SYNC_FIELDITEM_LEAVE_BRD msgBRD;
        BYTE count = 0;

        ns_object::SearchNodeConstIterator it = rSector.GetItemRoot();
        const ns_object::SearchNodeConstIterator end = rSector.GetItemEnd();

        while (const ns_object::SearchNode::LinkNode* link_node = it.ClimbToNext(end))
        {
            Item* pItem = link_node->select.item_node.item;
            if (obj_util::IsValid(pItem) == false)
            {
                continue;
            }

			msgBRD.m_dwObjectKey[count++] = pItem->GetObjectKey();

			if(MSG_CG_SYNC_FIELDITEM_LEAVE_BRD::_MAX_FIELDITEM_SIZE == count)
			{
				msgBRD.m_byCount = count;
				this->SendPacket(&msgBRD, (WORD)msgBRD.GetSize());
				count = 0;
			}
		}

		if(count)
		{
			msgBRD.m_byCount = count;
			this->SendPacket(&msgBRD, (WORD)msgBRD.GetSize());
		}
	};
}

//──────────────────────────────────────────────────────────
//	<Not Using ViewPort Logic>
void
Player::OnSendAllPlayersRenderInfoAllKinds(const Sector& IN rSector)
{
    GameZone* pGameZone = GetGameZonePtr();
    if(pGameZone == 0)
        return;

    VarMsg2* const pVarMsg = &(m_VarMsg2[SENDINFO_PLAYER_RENDER]);
    VarMsg2* const pVarMsgEquip = &(m_VarMsg2[SENDINFO_PLAYER_EQUIP]);
    VarMsg2* const pVarMsgSRender = &(m_VarMsg2[SENDINFO_PLAYER_SPECIAL_RENDER]);

    const BOOLEAN isVillageRender = pGameZone->GetZoneType() == eZONETYPE_VILLAGE;
    const BYTE MAX_PACKET_COUNT = MSG_CG_SYNC_ALLPLAYERS_CMD::_MAX_PLAYER_INFO_SIZE;

    // Equip & Effect Render Packet Size는 MSG_CG_SYNC_ALLPLAYERS_CMD::_MAX_PLAYER_INFO_SIZE 와 동일하게 설정되어 있다.
    MSG_CG_SYNC_ALLPLAYERS_CMD* pRenderInfo = NULL;
    MSG_CG_SYNC_ALLPLAYERS_EQUIPINFO_CMD* pEquipMsg = NULL;
    MSG_CG_STATUS_RENDER_EFFECT_ITEM_LIST_CMD* pSpecialRender = NULL;  // var of var packet

    pRenderInfo = pVarMsg->StartTypePtr(Param2Class<MSG_CG_SYNC_ALLPLAYERS_CMD>());
    pVarMsg->SetSize(pRenderInfo->GetSize());
    //
    {
        pEquipMsg = pVarMsgEquip->StartTypePtr(Param2Class<MSG_CG_SYNC_ALLPLAYERS_EQUIPINFO_CMD>());
        pVarMsgEquip->SetSize(pEquipMsg->GetSize());
    };
    {
        pSpecialRender = pVarMsgSRender->StartTypePtr(Param2Class<MSG_CG_STATUS_RENDER_EFFECT_ITEM_LIST_CMD>());
        pVarMsgSRender->SetSize(pSpecialRender->GetSize());
    }

    BYTE count = 0;
    BYTE count_srender = 0;

    ns_object::SearchNodeConstIterator it = rSector.GetPlayerRoot();
    const ns_object::SearchNodeConstIterator end = rSector.GetPlayerEnd();

    while (const ns_object::SearchNode::LinkNode* link_node = it.ClimbToNext(end))
    {
        Player* pPlayer = link_node->select.player_node.player;
        if (obj_util::IsValid(pPlayer) == false)
        {
            continue;
        }
         
        if(this == pPlayer)
            continue;

        StatusManager* const pStatusManager = pPlayer->GetStatusManager();
        StatusField& rStatus = pStatusManager->Status;
        const BOOLEAN bNeedNtf = !(rStatus.FIELD & rStatus.ENABLE_OBSERVER_MODE);
        if(!bNeedNtf)
            continue;

        const WORD playerKey = (WORD)pPlayer->GetObjectKey();
        {   // Make 'PLAYER_RENDER_INFO_EX' -> packed info.
            PLAYER_RENDER_INFO_EX* pack_info = (PLAYER_RENDER_INFO_EX*)pVarMsg->NextPtr();
            int packed_len = pPlayer->GetPlayerRenderInfo(isVillageRender == 0, pack_info);
            pVarMsg->SetSize((WORD)packed_len);
        };
        // 서버에서 부여한 상태 정보
        {   //__NA_001290_20090525_SHIELD_SYSTEM_NOTIFY_NOTABILITY_STATE
            ETC_STATE_TOTAL_INFO *pStateRenderMsg = (ETC_STATE_TOTAL_INFO*)pVarMsg->NextPtr();
            pPlayer->GetEtcStateTotalInfo(pStateRenderMsg);
            pVarMsg->SetSize(pStateRenderMsg->GetSize());
        };
        //__NA_0_20090720_GUILD_RENDER_INFO_MODIFY
        // GUILD_RENDER_INFO는 다른패킷으로 보냄
        {
            // 개인 상점 타이틀 
            VENDOR_RENDER_INFO* pVendorRenderInfo = (VENDOR_RENDER_INFO*)pVarMsg->NextPtr();
            pPlayer->GetVendorRenderInfo(pVendorRenderInfo);
            pVarMsg->SetSize(pVendorRenderInfo->GetSize());
        }

        //  EQUIPMENT
        {
            // 기본 EQUIP_ITEM_BASEINFO를 채운다.
            EQUIP_ITEM_BASEINFO* pResultMsg = (EQUIP_ITEM_BASEINFO*)pVarMsgEquip->NextPtr();
            pResultMsg->m_dwObjectKey = playerKey;

            // ITEMSLOT을 채운다.
            EQUIP_ITEM_BASEINFO::data_type* pItemSlot = 
                (EQUIP_ITEM_BASEINFO::data_type*)((BYTE*)pResultMsg + sizeof(EQUIP_ITEM_BASEINFO));
            pResultMsg->m_Count = pPlayer->SerializeRenderEquipItemBaseInfo(pItemSlot);
            pVarMsgEquip->SetSize(pResultMsg->GetSize());
        };
        //  SPECIAL RENDER
        {
            sMSG_RENDER_EFFECT_ITEM_LIST* pEffectNode = (sMSG_RENDER_EFFECT_ITEM_LIST*)pVarMsgSRender->NextPtr();
            pEffectNode->m_PlayerKey = playerKey;
            const DWORD alignedCount = pPlayer->RENDER_EFFECT.SerializeRenderInfo(pEffectNode->m_BitRender, pEffectNode->m_ItemCodes, sMSG_RENDER_EFFECT_ITEM_LIST::MAX_EFFECT_ITEMS);
            pEffectNode->m_ItemCount = (BYTE)alignedCount;
            pVarMsgSRender->SetSize(pEffectNode->GetSize());
            ++count_srender;
        };

        //////////////////////////////////////////////////////////////////////////
        ++count;
        //////////////////////////////////////////////////////////////////////////

        if(MAX_PACKET_COUNT == count)
        {
            pRenderInfo->m_byCount = count;
            this->SendPacket((MSG_BASE_FORWARD*)*pVarMsg->GetMsgPPtr(),
                             *pVarMsg->GetMsgSizePtr(), TRUE);
            pRenderInfo = pVarMsg->StartTypePtr(Param2Class<MSG_CG_SYNC_ALLPLAYERS_CMD>());
            pVarMsg->SetSize(pRenderInfo->GetSize());
            //
            {
                pEquipMsg->m_byCount = count;
                this->SendPacket((MSG_BASE_FORWARD*)*pVarMsgEquip->GetMsgPPtr(), *pVarMsgEquip->GetMsgSizePtr(), TRUE);
                pEquipMsg = pVarMsgEquip->StartTypePtr(Param2Class<MSG_CG_SYNC_ALLPLAYERS_EQUIPINFO_CMD>());
                pVarMsgEquip->SetSize(pEquipMsg->GetSize());
            }
            count = 0;
        }

        if(MSG_CG_STATUS_RENDER_EFFECT_ITEM_LIST_CMD::_MAX_PLAYER_INFO_SIZE == count_srender)
        {
            pSpecialRender->m_Count = count_srender;
            this->SendPacket((MSG_BASE_FORWARD*)*pVarMsgSRender->GetMsgPPtr(),
                             *pVarMsgSRender->GetMsgSizePtr(), TRUE);
            pSpecialRender = pVarMsgSRender->StartTypePtr(Param2Class<MSG_CG_STATUS_RENDER_EFFECT_ITEM_LIST_CMD>());
            pVarMsgSRender->SetSize(pSpecialRender->GetSize());

            count_srender = 0;
        };

    }

    if(count)
    {
        pRenderInfo->m_byCount = count;
        this->SendPacket((MSG_BASE_FORWARD*)*pVarMsg->GetMsgPPtr(), *pVarMsg->GetMsgSizePtr(), TRUE);
        //
        {
            pEquipMsg->m_byCount = count;
            this->SendPacket((MSG_BASE_FORWARD*)*pVarMsgEquip->GetMsgPPtr(),
                             *pVarMsgEquip->GetMsgSizePtr(), TRUE);
        }
    }

    if(count_srender)
    {
        pSpecialRender->m_Count = count_srender;
        this->SendPacket((MSG_BASE_FORWARD*)*pVarMsgSRender->GetMsgPPtr(),
                         *pVarMsgSRender->GetMsgSizePtr(), TRUE);
    }

    it = rSector.GetPlayerRoot();
    while (const ns_object::SearchNode::LinkNode* link_node = it.ClimbToNext(end))
    {
        Player* pPlayer = link_node->select.player_node.player;
        if (obj_util::IsValid(pPlayer) == false)
        {
            continue;
        }
        if(this == pPlayer)
            continue;

        pPlayer->SendMoveInfo(this, rSector);
    }
}


void
Player::OnSendAllMonsterInfo(const Sector& IN rSector)
{
	VarMsg2* pVarMsgMonsters = &(m_VarMsg2[/*Player::*/SENDINFO_NPCS_MONSTER]);
	VarMsg2* pVarMsgSummoneds = &(m_VarMsg2[/*Player::*/SENDINFO_NPCS_SUMMONED]);
	VarMsg2* pVarMsgMapNpcs = &(m_VarMsg2[/*Player::*/SENDINFO_NPCS_MAPNPC]);
	VarMsg2* pVarMsgTotemNpcs = &(m_VarMsg2[/*Player::*/SENDINFO_NPCS_TOTEMNPC]);
    VarMsg2* pVarMsgMerchantNpcs = &(m_VarMsg2[/*Player::*/SENDINFO_NPCS_MERCHANTNPC]);

	// 주변 몬스터 정보
	MSG_CG_SYNC_ALLMONSTERS_CMD* pMonsterMsg = 
        pVarMsgMonsters->StartTypePtr(Param2Class<MSG_CG_SYNC_ALLMONSTERS_CMD>());
	pVarMsgMonsters->SetSize(pMonsterMsg->GetSize());

	// 주변 소환체 정보
	MSG_CG_SYNC_ALLSUMMONEDS_CMD* pSummonedMsg = 
        pVarMsgSummoneds->StartTypePtr(Param2Class<MSG_CG_SYNC_ALLSUMMONEDS_CMD>());
	pVarMsgSummoneds->SetSize(pSummonedMsg->GetSize());

	// 주변 MapNpc 정보
	MSG_CG_SYNC_ALLMAPNPC_CMD* pMapNpcMsg = 
        pVarMsgMapNpcs->StartTypePtr(Param2Class<MSG_CG_SYNC_ALLMAPNPC_CMD>());
	pVarMsgMapNpcs->SetSize(pMapNpcMsg->GetSize());

	// 주변 MapNpc 정보
	MSG_CG_SYNC_ALLTOTEMNPC_CMD* pTotemNpcMsg = 
        pVarMsgTotemNpcs->StartTypePtr(Param2Class<MSG_CG_SYNC_ALLTOTEMNPC_CMD>());
	pVarMsgTotemNpcs->SetSize(pTotemNpcMsg->GetSize());

    // 주변 Merchant 정보
    MSG_CG_SYNC_ALLMERCHANTNPC_CMD* pMerchantNpcMsg = 
        pVarMsgMerchantNpcs->StartTypePtr(Param2Class<MSG_CG_SYNC_ALLMERCHANTNPC_CMD>());
    pVarMsgMerchantNpcs->SetSize(pMerchantNpcMsg->GetSize());

	BYTE& byCntMonsters = pMonsterMsg->m_byCount;
	BYTE& byCntSummoneds = pSummonedMsg->m_byCount;
	BYTE& byCntMapNpcs = pMapNpcMsg->m_byCount;
	BYTE& byCntTotemNpcs = pTotemNpcMsg->m_byCount;
    BYTE& byCntMerchantNpcs = pMerchantNpcMsg->m_byCount;
    byCntMerchantNpcs = 0;

	byCntMonsters = byCntSummoneds = byCntMapNpcs = byCntTotemNpcs = 0;

	VarMsg2* pVarMsgSharedFN;
	DWORD dwCntCurShared;
	DWORD dwCntMaxShared;

    ns_object::SearchNodeConstIterator it = rSector.GetNpcRoot();
    const ns_object::SearchNodeConstIterator end = rSector.GetNpcEnd();

    while (const ns_object::SearchNode::LinkNode* link_node = it.ClimbToNext(end))
    {
        NPC* pNPC = link_node->select.npc_node.npc;
        if (obj_util::IsValid(pNPC) == false)
        {
            continue;
        }

		union
		{
			MONSTER_RENDER_INFO* pMonsterRender;
            ETC_STATE_TOTAL_INFO* pStateRender;
			SUMMONED_RENDER_INFO* pSummonedRender;
			MAPNPC_RENDER_INFO* pMapNpcRender;
			TOTEMNPC_RENDER_INFO* pTotemNpcRender;
            MERCHANT_RENDER_INFO* pMerchantNpcRender;
		};

        DWORD dwObjectT = pNPC->GetObjectType();

		dwObjectT &= MONSTER_OBJECT | SUMMON_OBJECT | 
                     MAPNPC_OBJECT  | TOTEMNPC_OBJECT | 
                    CRYSTALWARP_OBJECT | FRIEND_MONSTER_OBJECT |
                    SYNC_MERCHANT_OBJECT;

		switch(dwObjectT)
		{
        case FRIEND_MONSTER_OBJECT: //_NA_0_20100222_UNIT_TRIGGERS_REGEN
		case MONSTER_OBJECT:	/*case LUCKYMON_OBJECT:*/ 
			pMonsterRender = (MONSTER_RENDER_INFO*)pVarMsgMonsters->NextPtr();
			pNPC->GetNPCRenderInfo(pMonsterRender);
			pVarMsgMonsters->SetSize(pMonsterRender->GetSize());
            //{__NA_001290_20090525_SHIELD_SYSTEM_NOTIFY_NOTABILITY_STATE
            pStateRender = (ETC_STATE_TOTAL_INFO*)pVarMsgMonsters->NextPtr();
            pNPC->GetEtcStateTotalInfo(pStateRender);
            pVarMsgMonsters->SetSize(pStateRender->GetSize());
            //}
			dwCntCurShared = ++byCntMonsters;
			dwCntMaxShared = MSG_CG_SYNC_ALLMONSTERS_CMD::_MAX_MONSTER_INFO_SIZE;
			pVarMsgSharedFN = pVarMsgMonsters;;
			break;
        case CRYSTALWARP_OBJECT:
		case SUMMON_OBJECT:
			pSummonedRender = (SUMMONED_RENDER_INFO*)pVarMsgSummoneds->NextPtr();
			pNPC->GetSummonedRenderInfo(pSummonedRender);
			pVarMsgSummoneds->SetSize(pSummonedRender->GetSize());
			dwCntCurShared = ++byCntSummoneds;
			dwCntMaxShared = MSG_CG_SYNC_ALLSUMMONEDS_CMD::_MAX_SUMMONED_INFO_SIZE;
			pVarMsgSharedFN = pVarMsgSummoneds;
			break;
		case MAPNPC_OBJECT:
			pMapNpcRender = (MAPNPC_RENDER_INFO*)pVarMsgMapNpcs->NextPtr();
			pMapNpcRender->m_dwMapNpcID = ((MapNpc*)(pNPC))->GetMapObjectID();
			//클라에서 200000만 더해서 씀. (1~)값으로 보냄.
			pMapNpcRender->m_dwMapNpcID = Conv_TriggerObjectKey(pMapNpcRender->m_dwMapNpcID);
			pNPC->GetNPCRenderInfo(pMapNpcRender->m_sMonsterRenderInfo);
			pVarMsgMapNpcs->SetSize(pMapNpcRender->GetSize());
			dwCntCurShared = ++byCntMapNpcs;
			dwCntMaxShared = MSG_CG_SYNC_ALLMAPNPC_CMD::_MAX_MAPNPC_INFO_SIZE;
			pVarMsgSharedFN = pVarMsgMapNpcs;;
			break;
		case TOTEMNPC_OBJECT:
			pTotemNpcRender = (TOTEMNPC_RENDER_INFO*)pVarMsgTotemNpcs->NextPtr();
			pNPC->GetTotemNPCRenderInfo(pTotemNpcRender);
			pVarMsgTotemNpcs->SetSize(pTotemNpcRender->GetSize());
			dwCntCurShared = ++byCntTotemNpcs;
			dwCntMaxShared = MSG_CG_SYNC_ALLTOTEMNPC_CMD::_MAX_TOTEMNPC_INFO_SIZE;
			pVarMsgSharedFN = pVarMsgTotemNpcs;;
			break;
        case SYNC_MERCHANT_OBJECT:
            pMerchantNpcRender = (MERCHANT_RENDER_INFO*)pVarMsgMerchantNpcs->NextPtr();
            pNPC->GetNPCRenderInfo(pMerchantNpcRender->m_sMonsterRenderInfo);
            pVarMsgMerchantNpcs->SetSize(pMerchantNpcRender->GetSize());
            dwCntCurShared = ++byCntMerchantNpcs;
            dwCntMaxShared = MSG_CG_SYNC_ALLMERCHANTNPC_CMD::_MAX_MERCHANTNPC_INFO_SIZE;
            pVarMsgSharedFN = pVarMsgMerchantNpcs;;
            break;
		default:
			// Error?  ex) 배틀존에 XX이 있을 수 있는가?
			return;
		};

		// Next Block
		// <THINKING> Shared?
		//    다음 처리는 이전에 잘못된 타입을 제거했다고 판단한다.
		if(dwCntMaxShared == dwCntCurShared)
		{
			this->SendPacket((MSG_BASE_FORWARD*)*pVarMsgSharedFN->GetMsgPPtr(), *pVarMsgSharedFN->GetMsgSizePtr(), TRUE);
			switch(dwObjectT)
			{
			case MONSTER_OBJECT:	case LUCKYMON_OBJECT:
				pMonsterMsg = pVarMsgSharedFN->StartTypePtr(Param2Class<MSG_CG_SYNC_ALLMONSTERS_CMD>());
				pVarMsgSharedFN->SetSize(pMonsterMsg->GetSize());
				byCntMonsters = 0;
				break;
            case CRYSTALWARP_OBJECT:
			case SUMMON_OBJECT:
				pSummonedMsg = pVarMsgSharedFN->StartTypePtr(Param2Class<MSG_CG_SYNC_ALLSUMMONEDS_CMD>());
				pVarMsgSharedFN->SetSize(pSummonedMsg->GetSize());
				byCntSummoneds = 0;
				break;
			case MAPNPC_OBJECT:
				pMapNpcMsg = pVarMsgSharedFN->StartTypePtr(Param2Class<MSG_CG_SYNC_ALLMAPNPC_CMD>());
				pVarMsgSharedFN->SetSize(pMapNpcMsg->GetSize());
				byCntMapNpcs = 0;
				break;
			case TOTEMNPC_OBJECT:
				pTotemNpcMsg = pVarMsgSharedFN->StartTypePtr(Param2Class<MSG_CG_SYNC_ALLTOTEMNPC_CMD>());
				pVarMsgSharedFN->SetSize(pTotemNpcMsg->GetSize());
				byCntTotemNpcs = 0;
				break;
            case SYNC_MERCHANT_OBJECT:
                pMerchantNpcMsg = pVarMsgSharedFN->StartTypePtr(Param2Class<MSG_CG_SYNC_ALLMERCHANTNPC_CMD>());
                pVarMsgSharedFN->SetSize(pMerchantNpcMsg->GetSize());
                byCntMerchantNpcs = 0;
                break;
			}
		}
	}

	// 남은 정보 처리
	if(byCntMonsters)		this->SendPacket((MSG_BASE_FORWARD*)*pVarMsgMonsters->GetMsgPPtr(),	    *pVarMsgMonsters->GetMsgSizePtr(),  TRUE);
	if(byCntSummoneds)	    this->SendPacket((MSG_BASE_FORWARD*)*pVarMsgSummoneds->GetMsgPPtr(),	*pVarMsgSummoneds->GetMsgSizePtr(), TRUE);
	if(byCntMapNpcs)		this->SendPacket((MSG_BASE_FORWARD*)*pVarMsgMapNpcs->GetMsgPPtr(),		*pVarMsgMapNpcs->GetMsgSizePtr(),   TRUE);
	if(byCntTotemNpcs)	    this->SendPacket((MSG_BASE_FORWARD*)*pVarMsgTotemNpcs->GetMsgPPtr(),	*pVarMsgTotemNpcs->GetMsgSizePtr(), TRUE);
    if(byCntMerchantNpcs)	this->SendPacket((MSG_BASE_FORWARD*)*pVarMsgMerchantNpcs->GetMsgPPtr(),	*pVarMsgMerchantNpcs->GetMsgSizePtr(), TRUE);

    // 몬스터 이동정보를 보낸다.
    it = rSector.GetNpcRoot();
	while (const ns_object::SearchNode::LinkNode* link_node = it.ClimbToNext(end))
	{
		NPC* pNPC = link_node->select.npc_node.npc;
        if (obj_util::IsValid(pNPC) == false)
        {
            continue;
        }
		pNPC->SendMoveInfo(this, rSector);
	}
}


void
Player::OnSendAllFieldItemInfo(const Sector& IN rSector)
{
	MSG_CG_SYNC_ALLFIELDITEMS_CMD msgBRD;

	const BYTE MAX_PACKET_COUNT = MSG_CG_SYNC_ALLFIELDITEMS_CMD::_MAX_FIELDITEM_INFO_SIZE;
    BYTE count = 0;

    ns_object::SearchNodeConstIterator it = rSector.GetItemRoot();
    const ns_object::SearchNodeConstIterator end = rSector.GetItemEnd();

    while (const ns_object::SearchNode::LinkNode* link_node = it.ClimbToNext(end))
    {
        Item* pItem = link_node->select.item_node.item;
        if (obj_util::IsValid(pItem) == false)
        {
            continue;
        }
		pItem->GetItemRenderInfo(msgBRD.m_pFieldItemRenderInfo[count]);
		++count;

		if(MAX_PACKET_COUNT == count)
		{
			msgBRD.m_byCount = count;
			this->SendPacket(&msgBRD, (WORD)msgBRD.GetSize(), TRUE);
			count = 0;
		}
	}

	if(count)
	{
		msgBRD.m_byCount = count;
		this->SendPacket(&msgBRD, (WORD)msgBRD.GetSize(), TRUE);
	}
}

//{__NA001283_20090225_COLLECTION_USE
// 주변 9개 섹터의 유저정보를 갱신한다. 
void
Player::SendAllFieldCollectionInfo()
{
	if(NULL == GetField()) return;

	const SECTOR_IDX sector_index = GetSectorIndex();
	

	SectorGrid3x3 OUT grid;
	if(GetField()->SectorGridRef.GetAroundSectors(sector_index, &grid))
	{	//	Center Processing
		Sector* const pSector = GetField()->GetSector(sector_index);
		grid.Grid[ SectorGrid3x3::CENTER ] = INVALID_SECTOR_INDEX;		// Center 제거

		OnSendAllFieldCollectionInfo(*pSector);
	};

	//	주변 섹터만 처리한다. Center는 앞 로직에서 제거되었음.
	FOREACH_ARRAY(const SECTOR_IDX sector_idx, grid.Grid, SectorGrid3x3::GridArray)
	{
		if(INVALID_SECTOR_INDEX == sector_idx)
			continue;

		Sector* const pSector = GetField()->GetSector(sector_idx);
		OnSendAllFieldCollectionInfo(*pSector);
	}
}

void
Player::OnSendAllFieldCollectionInfo(const Sector& IN rSector)
{
	MSG_CG_COLLECT_ALLCOLLECTIVE_CMD msgBRD;

	const BYTE MAX_PACKET_COUNT = MSG_CG_COLLECT_ALLCOLLECTIVE_CMD::_MAX_FIELDCOLLECT_INFO_SIZE;
	BYTE count = 0;

    ns_object::SearchNodeConstIterator it = rSector.GetEtcRoot();
    const ns_object::SearchNodeConstIterator end = rSector.GetEtcEnd();

    while (const ns_object::SearchNode::LinkNode* link_node = it.ClimbToNext(end))
    {
        const eOBJECT_TYPE object_type = link_node->select.object_node.object_type;
        if ((object_type & COLLECTION_OBJECT) != COLLECTION_OBJECT) {
            continue;
        }
        Collection* pCollection = static_cast<Collection*>(link_node->select.object_node.object);
        if (obj_util::IsValid(pCollection) == false)
        {
            continue;
        }

		const MAPOBJECT_INFO* pMapObjInfo = pCollection->GetMapObjectInfo();

#ifdef _DEBUG
        //! 디버깅용
        char str[256] = {NULL, };
        WzIDToString( str, pMapObjInfo->CollectionID );
#endif
        //_NA_004777_20120428_ADD_COMPLETE_COLLECTION_TRIGGER
        if (pCollection->is_active() == false)
        {
            continue;
        }

		RC::eCOLLECT_RESULT rc = GetField()->GetCollectManeger()->ChkCondision(this, pMapObjInfo->CollectionID);
		if(RC::RC_COLLECT_SUCCESS != rc)
			continue;

		pCollection->GetRenderInfo(msgBRD.m_CollectiveInfo[count]);

		SUNLOG(eDEV_LOG, "[Collect 주변 정보 보냄] 받는 케릭[%s] ObjectKey[%d] CollectionID[%d]",
			this->GetCharName(),
			msgBRD.m_CollectiveInfo[count].m_dwObjectKey, 
			msgBRD.m_CollectiveInfo[count].m_CollectionID);

		++count;

		if(MAX_PACKET_COUNT == count)
		{
			msgBRD.m_byCount = count;
			this->SendPacket(&msgBRD, (WORD)msgBRD.GetSize(), TRUE);
			count = 0;
		}
	}

	if(count)
	{
		msgBRD.m_byCount = count;
		this->SendPacket(&msgBRD, (WORD)msgBRD.GetSize(), TRUE);
	}
}
//}__NA001283_20090225_COLLECTION_USE


//_NA_0_20100208_DOMINATION_AREA_INFO
//자신 -> 주변
void
Player::OnSendAllPlayersGuildRenderInfoToNeighbor(const Sector& IN rSector)
{
    typedef MSG_CG_SYNC_ALLPLAYERS_GUILDRENDERINFO_CMD    MSG_GUILDRENDERINFO_CMD;

    VarMsg2* const pVarMsg = &(m_VarMsg2[SENDINFO_GUILD_RENDER]);

    ns_object::SearchNodeConstIterator it = rSector.GetPlayerRoot();
    const ns_object::SearchNodeConstIterator end = rSector.GetPlayerEnd();

    while (const ns_object::SearchNode::LinkNode* link_node = it.ClimbToNext(end))
    {
        Player* pPlayer = link_node->select.player_node.player;
        if (obj_util::IsValid(pPlayer) == false)
        {
            continue;
        }
        if (this == pPlayer)
            continue;

        StatusManager* const pStatusManager = pPlayer->GetStatusManager();
        StatusField& rStatus = pStatusManager->Status;
        const BOOLEAN bNeedNtf = !(rStatus.FIELD & rStatus.ENABLE_OBSERVER_MODE);
        if(!bNeedNtf)
            continue;

        MSG_GUILDRENDERINFO_CMD* pRenderInfo = 
            pVarMsg->StartTypePtr(Param2Class<MSG_GUILDRENDERINFO_CMD>());

        pVarMsg->SetSize(pRenderInfo->GetSize());
        pRenderInfo->m_byCount = 1;

        GUILD_RENDER_INFO* pack_info = (GUILD_RENDER_INFO*)pVarMsg->NextPtr();

        if (GetGuildRenderInfo(pack_info, pPlayer->GetGuildGuid()) == FALSE)
        {
            continue;
        }

        pVarMsg->SetSize(pack_info->GetSize());
        pPlayer->SendPacket((MSG_BASE_FORWARD*)*pVarMsg->GetMsgPPtr(),
                            *pVarMsg->GetMsgSizePtr(), TRUE);
    }
}

//주변 -> 자신
void
Player::OnSendAllPlayersGuildRenderInfo(const Sector& IN rSector)
{
    typedef MSG_CG_SYNC_ALLPLAYERS_GUILDRENDERINFO_CMD    MSG_GUILDRENDERINFO_CMD;

    VarMsg2* const pVarMsg = &(m_VarMsg2[SENDINFO_GUILD_RENDER]);

    BYTE count = 0;
    const BYTE MAX_PACKET_COUNT = 
        MSG_GUILDRENDERINFO_CMD::_MAX_GUILD_RENDERINFO_SIZE;

    MSG_GUILDRENDERINFO_CMD* pRenderInfo = 
        pVarMsg->StartTypePtr(Param2Class<MSG_GUILDRENDERINFO_CMD>());

    pVarMsg->SetSize(pRenderInfo->GetSize());

    ns_object::SearchNodeConstIterator it = rSector.GetPlayerRoot();
    const ns_object::SearchNodeConstIterator end = rSector.GetPlayerEnd();

    while (const ns_object::SearchNode::LinkNode* link_node = it.ClimbToNext(end))
    {
        Player* pPlayer = link_node->select.player_node.player;
        if (obj_util::IsValid(pPlayer) == false)
        {
            continue;
        }
        if (this == pPlayer)
            continue;   

        StatusManager* const pStatusManager = pPlayer->GetStatusManager();
        StatusField& rStatus = pStatusManager->Status;
        const BOOLEAN bNeedNtf = !(rStatus.FIELD & rStatus.ENABLE_OBSERVER_MODE);
        if(!bNeedNtf)
            continue;

        GUILD_RENDER_INFO* pack_info = (GUILD_RENDER_INFO*)pVarMsg->NextPtr();

        if (pPlayer->GetGuildRenderInfo(pack_info, this->GetGuildGuid()) == FALSE)
        {
            continue;
        }

        pVarMsg->SetSize(pack_info->GetSize());

        if (MAX_PACKET_COUNT == ++count)
        {
            pRenderInfo->m_byCount = count;
            SendPacket((MSG_BASE_FORWARD*)*pVarMsg->GetMsgPPtr(),
                *pVarMsg->GetMsgSizePtr(), TRUE);

            pRenderInfo = pVarMsg->StartTypePtr(Param2Class<MSG_GUILDRENDERINFO_CMD>());
            pVarMsg->SetSize(pRenderInfo->GetSize());

            count = 0;
        }
    }

    if(count)
    {
        pRenderInfo->m_byCount = count;
        SendPacket((MSG_BASE_FORWARD*)*pVarMsg->GetMsgPPtr(), *pVarMsg->GetMsgSizePtr(), TRUE);
    }
}