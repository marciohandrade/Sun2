// WarehouseSlotContainer.cpp: implementation of the CGuildWarehouseSlotContainer class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "GuildWarehouseSlotContainer.h"

#include "Player.h"
#include "GameGuild.h"
#include <PacketStruct_DG.h>
#include <PacketStruct_GZ.h>
#include "ItemManager.h"
#include "FunctionalRestrictManager.h"
#include "StatisticsManager.h"
#include "GameServerEx.h"
#include <SCItemSlot.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CGuildWarehouseSlotContainer::CGuildWarehouseSlotContainer() 
    : m_pPlayer(NULL)
    , m_pOwnerGuild(NULL)
    , m_Money(0)
    , m_bOpened(false)
    , m_bChanged(false)
    , m_SlotTabNum(0)
    , m_AutoCloseTime(INVALID_AUTO_CLOSETIME)
{
    Init(MAX_GUILD_WAREHOUSE_SLOT_NUM, SI_GUILDWAREHOUSE);
    //SetMaxSlotNum((MAX_GUILDWAREHOUSE_TAB_NUM + m_SlotTabNum) * MAX_GUILDWAREHOUSE_SLOT_PER_TAB);
}

CGuildWarehouseSlotContainer::~CGuildWarehouseSlotContainer()
{
}

void CGuildWarehouseSlotContainer::ClearAll()
{
	if(m_pPlayer)
		m_pPlayer->SetGuildWarehouseRight(FALSE);

	m_pPlayer = NULL;
	m_bOpened = FALSE;
	m_bChanged = FALSE;
	m_AutoCloseTime = INVALID_AUTO_CLOSETIME;
	SCItemSlotContainer::ClearAll();
}

void CGuildWarehouseSlotContainer::Release()
{
	if(m_pPlayer)
		m_pPlayer->SetGuildWarehouseRight(FALSE);

	m_pPlayer = NULL;
	m_bOpened = FALSE;
	m_bChanged = FALSE;
	m_pOwnerGuild = NULL;
	m_SlotTabNum = 0;
	m_Money = 0;
	m_AutoCloseTime = INVALID_AUTO_CLOSETIME;
	SCItemSlotContainer::ClearAll();
}

void CGuildWarehouseSlotContainer::OpenInit(Player* pPlayer, BYTE SlotTab, BOOL Right)
{
	if(m_pPlayer && m_pPlayer->GetField())
	{
		SUNLOG(eCRITICAL_LOG, "OpenWarehouse Player Error[%u][%d][%d]", m_pPlayer->GetCharGuid(), m_bOpened, m_bChanged);
	}

	m_bOpened = TRUE;
	m_pPlayer = pPlayer;
	SetSlotTabNum(SlotTab);
	m_AutoCloseTime = MAX_AUTO_CLOSE_TIMELIMIT + GetTickCount();
	m_pPlayer->SetGuildWarehouseRight(Right);
}
void CGuildWarehouseSlotContainer::SetSlotTabNum(BYTE count)
{
	if(m_SlotTabNum != count)
	{
		m_SlotTabNum = count;
#if !defined(_NA_003923_20120130_GUILD_RENEWAL)
		SetMaxSlotNum((MAX_GUILDWAREHOUSE_TAB_NUM + m_SlotTabNum) * MAX_GUILDWAREHOUSE_SLOT_PER_TAB);
#endif //_NA_003923_20120130_GUILD_RENEWAL
	}
}

BOOL CGuildWarehouseSlotContainer::ValidState()
{
	return TRUE;
}

RC::eITEM_RESULT CGuildWarehouseSlotContainer::IsUseRight(Player* actor, bool withdrawal)
{
    if (m_pOwnerGuild == NULL || m_pPlayer == NULL) {
        return RC::RC_ITEM_FAILED;
    };
    // CHANGES: f110627.2L, prevent unauthorized invader's request.
    if (m_pPlayer != actor) {
        return RC::RC_ITEM_INVALIDSTATEOFPLAYER;
    };
    if ((withdrawal) && m_pPlayer->IsGuildWarehouseRight() == false) {
        return RC::RC_ITEM_GUILDWAREHOUSE_NOEN_OUT_RIGHT;
    };

    return RC::RC_ITEM_SUCCESS;
}

RC::eITEM_RESULT CGuildWarehouseSlotContainer::IsValidPos(POSTYPE atPos)
{
	ASSERT(m_pOwnerGuild);
	BYTE grade = m_pOwnerGuild->GetGuildGrade();
	
	// 길드 등급 3부터 사용할 수 있는 슬롯
	if(GUILDWAREHOUSE_SLOTEXPOS <= atPos && 
		GUILDWAREHOUSE_SLOTEXPOS_BY_CASHITEM > atPos)
	{
		if(GUILDWAREHOUSE_SLOTEX_GUILD_GRADE  <= grade) {}
		else
        {
#ifdef _NA_003923_20120130_GUILD_RENEWAL
            if (GetSlotTabNum() >= MAX_GUILDWAREHOUSE_TAB_NUM) {
                // ok...
            }
            else {
                return RC::RC_ITEM_GUILDWAREHOUSE_USENOT_SLOT_BY_GRADE;
            }
#else
            return RC::RC_ITEM_GUILDWAREHOUSE_USENOT_SLOT_BY_GRADE;
#endif //_NA_003923_20120130_GUILD_RENEWAL
        }
	}

	// 캐쉬템을 이용해야 사용가능한 슬롯
	if(GUILDWAREHOUSE_SLOTEXPOS_BY_CASHITEM <= atPos &&
        (MAX_GUILDWAREHOUSE_TAB_TOTAL_NUM * MAX_GUILDWAREHOUSE_SLOT_PER_TAB) > atPos)
	{
		if(GUILDWAREHOUSE_SLOTEX_GUILD_GRADE <= grade &&
			GetSlotTabNum() > MAX_GUILDWAREHOUSE_TAB_NUM) {}
		else return RC::RC_ITEM_GUILDWAREHOUSE_USENOT_SLOT_BY_CASHITEM;
	}

    if((MAX_GUILDWAREHOUSE_TAB_TOTAL_NUM * MAX_GUILDWAREHOUSE_SLOT_PER_TAB) <= atPos)
        return RC::RC_ITEM_INVALIDPOS;

	return RC::RC_ITEM_SUCCESS;
}

RC::eGUILD_RESULT CGuildWarehouseSlotContainer::IsEnableService(GameGuild* pGuild)
{
	if(pGuild->GetGuildGuid() != m_pOwnerGuild->GetGuildGuid())
		return RC::RC_GUILD_WAREHOUSE_PLAYER_DATA_DISCORD;

	if( pGuild->GetGuildGrade() <= GUILDWAREHOUSE_USING_LEVEL)
		return RC::RC_GUILD_WAREHOUSE_UNABLE_CURR_GRADE;

	if(m_pPlayer && IsOpened())
		return RC::RC_GUILD_WAREHOUSE_ALEADY_USGIN_PLAYER;
	
	if(m_pPlayer && !IsOpened())
	{
		if(TRUE == m_bChanged) SUNLOG(eCRITICAL_LOG, "GuildWarehouse OpenState Change Error1");
		SUNLOG(eCRITICAL_LOG, "GuildWarehouse OpenState Error(%u)(%u)", m_pPlayer->GetCharGuid(), m_pOwnerGuild->GetGuildGuid());
		return RC::RC_GUILD_WAREHOUSE_ALEADY_USGIN_PLAYER;
	}

	if(!m_pPlayer && IsOpened())
	{
		if(m_bChanged) SUNLOG(eCRITICAL_LOG, "GuildWarehouse OpenState Change Error2");
		SUNLOG(eCRITICAL_LOG, "GuildWarehouse OpenState Error(%d)(%d)", m_pOwnerGuild->GetGuildGuid());
		return RC::RC_GUILD_WAREHOUSE_ALEADY_USGIN_PLAYER;
	}

	return RC::RC_GUILD_SUCCESS;
}

BOOL CGuildWarehouseSlotContainer::DepositMoney(MONEY PlusMoney) 
{ 
	if( !IsOpened() ) 
		return FALSE; 
	
	m_Money += PlusMoney; 
	m_bChanged = true;
	g_StatisticsManager.PlusStatisticType(eSTATISTIC_MONEY_WAREHOUSE_PLUS, PlusMoney);
	return TRUE;
}

BOOL CGuildWarehouseSlotContainer::WithDrawMoney( MONEY MinusMoney )
{
	if(!IsOpened()) 
		return FALSE;

	if( m_Money < MinusMoney ) return FALSE;
	m_Money -= MinusMoney;
	m_bChanged = true;
	g_StatisticsManager.PlusStatisticType(eSTATISTIC_MONEY_WAREHOUSE_MINUS, MinusMoney);
	return TRUE;
}

void CGuildWarehouseSlotContainer::OpenWarehouse()
{
    nsSlot::ItemManagerUpdateImpl* const item_updater_impl =
        m_pPlayer->GetItemManager()->GetUpdaterImpl();

	for(POSTYPE i=0;i<GetMaxSlotNum();++i)
	{
		if( !IsEmpty(i) )
		{
			item_updater_impl->AddSerialCode( eSERIAL_WAREHOUSE_START, GetSlotIdx(), i, ((SCItemSlot&)GetSlot(i)).GetWorldSerial() );
		}
	}
}

void CGuildWarehouseSlotContainer::CloseWarehouse()
{
    nsSlot::ItemManagerUpdateImpl* const item_updater_impl =
        m_pPlayer->GetItemManager()->GetUpdaterImpl();

    if (m_pPlayer != NULL)
    {
        for(POSTYPE i=0;i<GetMaxSlotNum();++i)
        {
            if( !IsEmpty(i) )
            {
                SCItemSlot & rSlot = (SCItemSlot &)GetSlot(i);
                item_updater_impl->RemoveExpirationItem( rSlot );
                item_updater_impl->RemoveSerialCode( GetSlotIdx(), i, rSlot.GetWorldSerial() );
            }
        }
    }

	ClearAll();
}

RC::eSLOT_INSERT_RESULT CGuildWarehouseSlotContainer::InsertSlot( POSTYPE AtPos, SCSlot & IN rSlot )
{
    if (!m_pOwnerGuild || !m_pPlayer) {
        return RC::RC_INSERT_SLOT_DISABLE_SERVICE; // CHANGES: f110627.1L, fixed a missing keyword
    }

	if(!IsOpened())	return RC::RC_INSERT_SLOT_DISABLE_SERVICE;

    if (((SCItemSlot &)rSlot).IsPossessionItem() &&
        ((SCItemSlot &)rSlot).GetIdentifyPossession() &&
        (!((SCItemSlot &)rSlot).IsDestroyItem()))
    {
        return RC::RC_INSERT_SLOT_UNABLE_GUILDWAREHOUSE_POSSESSION_ITEM;
    }

	RC::eSLOT_INSERT_RESULT rt = SCSlotContainer::InsertSlot( AtPos, rSlot );
	if( RC::RC_INSERT_SLOT_SUCCESS == rt )
	{
		SCItemSlot & rCurSlot = (SCItemSlot &)GetSlot(AtPos);
        if (nsSlot::ItemManagerUpdateImpl* const item_updater_impl =
                m_pPlayer->GetItemManager()->GetUpdaterImpl())
        {
            item_updater_impl->AddSerialCode( eSERIAL_WAREHOUSE, GetSlotIdx(), AtPos, rCurSlot.GetWorldSerial() );
        }
		m_bChanged = true;
	}

	return rt;

}

BOOL CGuildWarehouseSlotContainer::DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut )
{
	ASSERT(m_pOwnerGuild && m_pPlayer);
	if(!m_pOwnerGuild || !m_pPlayer) return FALSE;

	if( !IsOpened())	 return FALSE;
	SCItemSlot TmpDeleteItemSlot;
	BOOL rt = SCSlotContainer::DeleteSlot( AtPos, &TmpDeleteItemSlot);
	if( rt )
	{
		if( pSlotOut ) 
		{
			((SCItemSlot*)pSlotOut)->Copy(TmpDeleteItemSlot);
		}
        if (nsSlot::ItemManagerUpdateImpl* const item_updater_impl =
                m_pPlayer->GetItemManager()->GetUpdaterImpl())
        {
            item_updater_impl->RemoveSerialCode( GetSlotIdx(), AtPos, TmpDeleteItemSlot.GetWorldSerial() );
        }
		m_bChanged = true;

	}
	return rt;
}

// CHANGES: f101120.1L, fixed guild warehouse update problem
// which is not applied on quantity changes by the item division move
void CGuildWarehouseSlotContainer::UpdateSlot(POSTYPE at_pos, int changed_num)
{
    SCItemSlotContainer::UpdateSlot(at_pos, changed_num);
    m_bChanged = true;
}

void CGuildWarehouseSlotContainer::OnInsert( SCSlot & IN rSlot )
{
	ASSERT(m_pOwnerGuild && m_pPlayer);
    if (nsSlot::ItemManagerUpdateImpl* const item_updater_impl =
            m_pPlayer->GetItemManager()->GetUpdaterImpl())
    {
        item_updater_impl->AddExpirationItem( (SCItemSlot &)rSlot );
    }
}
void CGuildWarehouseSlotContainer::OnDelete( SCSlot & IN rSlot )
{
	ASSERT(m_pOwnerGuild && m_pPlayer);
    if (nsSlot::ItemManagerUpdateImpl* const item_updater_impl =
            m_pPlayer->GetItemManager()->GetUpdaterImpl())
    {
        item_updater_impl->RemoveExpirationItem( (SCItemSlot &)rSlot );
    }
}


RC::eITEM_RESULT CGuildWarehouseSlotContainer::ExtendWarehouseTabCount(Player* pPlayer)
{
	if(m_pOwnerGuild->GetGuildGuid() != pPlayer->GetGuildGuid())
		return RC::RC_ITEM_GUILDWAREHOUSE_NONE_USENOT_CASHITEM;

	if(m_bOpened)
		return RC::RC_ITEM_GUILDWAREHOUSE_OPEN_USENOT_CASHITEM;

	if(GUILDWAREHOUSE_SLOTEX_GUILD_GRADE > m_pOwnerGuild->GetGuildGrade())
		return RC::RC_ITEM_GUILDWAREHOUSE_USENOT_SLOT_BY_GRADE;

	if(2 == m_SlotTabNum) 
		return RC::RC_ITEM_CANNOT_EXTEND_CASH_SLOT;

	return RC::RC_ITEM_SUCCESS;

}


UPDATE_RESULT CGuildWarehouseSlotContainer::SaveWarehouseToDBP(Player* pPlayer, eGUILDWAREHOUSE_UPDATE_FACTOR UpdateFactor)
{
	if(!m_pPlayer ||  UPDATE_FACTOR_COMMON == UpdateFactor)
	{
		return UPADTE_PLAYER_NONE;
	}

	if(pPlayer->GetCharGuid() != m_pPlayer->GetCharGuid())
	{
		SUNLOG(eCRITICAL_LOG, "UPDATE_ON_CLOSE_PALYER_DIFF(%u)(%u)(%u)", 
			m_pOwnerGuild->GetGuildGuid(), m_pPlayer->GetCharGuid(), pPlayer->GetCharGuid());
		return UPDATE_ON_CLOSE_PALYER_DIFF;
	}

	if(!g_pGameServer->IsConnected(GAME_DBPROXY))
	{
		SUNLOG(eCRITICAL_LOG, "UPDATE_CANNOT_CONNECT_DBP(%u)(%u)", m_pOwnerGuild->GetGuildGuid(), m_pPlayer->GetCharGuid());
		return UPDATE_CANNOT_CONNECT_DBP;
	}

	if((m_AutoCloseTime && GetTickCount() >= m_AutoCloseTime)) 
	{
		SUNLOG(eCRITICAL_LOG, "UPDATE_LONG_OPEN_STATE(%u)(%u)(%d)", 
			m_pOwnerGuild->GetGuildGuid(), m_pPlayer->GetCharGuid(), UpdateFactor);

		UpdateFactor = UPDATE_FACTOR_LOGOUT;
	}

	UPDATE_RESULT result = UPDATE_NO_CHANGE;
	if(m_bChanged)
	{
		MSG_DG_GUILD_WAREHOUSE_CMD msg;
		msg.m_dwKey = m_pPlayer->GetUserKey();
        msg.char_guid_ = m_pPlayer->GetCharGuid();
		msg.m_WarehouseMoney = GetCurMoney();
		msg.m_GuildGuid = m_pOwnerGuild->GetGuildGuid();
		msg.m_UpdateFactor = UpdateFactor;
		msg.m_ChannelID = g_pGameServer->GetKey().GetChannelID();
		SerializeItemInfo(msg.m_ItemInfo, SERIALIZE_LOAD);
		m_pPlayer->SendToGameDBPServer( &msg, msg.GetSize());
		m_bChanged = FALSE;
		result =  UPDATE_BY_CHNAGE_SUCCESS;
	}

	if(UPDATE_FACTOR_CHAR_UPDATE != UpdateFactor)
	{
		{
			MSG_CG_GUILD_WAREHOUSE_CLOSE_ACK msg;
			m_pPlayer->SendPacket(&msg, sizeof(msg));
		}

		MSG_DG_GUILD_WAREHOUSE_CLOSE_SYN msg;
		msg.m_dwKey = m_pPlayer->GetUserKey();
		msg.m_GuildGuid = m_pOwnerGuild->GetGuildGuid();
		m_pPlayer->SendToGameDBPServer(&msg, sizeof(msg));

        // 중국 복사 버그 수정 (유저 창고의 방식 사용) [10/12/2009 lst1024]
        CloseWarehouse();
	}

	return result;
}

//BOOL  m_bInOutState;//0 == In, 1 == Out
void CGuildWarehouseSlotContainer::ItemLogDBSave(SCItemSlot* pSlot, BYTE bInOutState)
{
	ASSERT(m_pOwnerGuild && m_pPlayer);
	if(!m_pOwnerGuild || !m_pPlayer) return;

	MSG_DG_GUILD_WAREHOUSELOG_UPDATE_SYN msg;
	msg.m_Money = 0;
    msg.m_LimitedIndex = (!!pSlot->IsLimited()) ? pSlot->GetLimitedIndex() : 0;
    msg.m_GuildGuid = m_pOwnerGuild->GetGuildGuid();
	msg.m_bInOutState  = bInOutState;
	msg.m_ItemCode = pSlot->GetItemCode();
	msg.m_ItemNum = pSlot->GetNum();
	memset(msg.m_CharName, 0, MAX_CHARNAME_LENGTH);														
	_tcsncpy(msg.m_CharName,m_pPlayer->GetCharName(), MAX_CHARNAME_LENGTH);
	msg.m_CharName[MAX_GUILDNAME_LENGTH] = '\0';
	m_pPlayer->SendToGameDBPServer(&msg, sizeof(msg));
}

void CGuildWarehouseSlotContainer::MoneyLogDBSave(MONEY money, BYTE bInOutState)
{
	ASSERT(m_pOwnerGuild && m_pPlayer);
	if(!m_pOwnerGuild || !m_pPlayer) return;

	MSG_DG_GUILD_WAREHOUSELOG_UPDATE_SYN msg;
	msg.m_ItemNum = 0;
	msg.m_ItemCode = 0;
	msg.m_Money = money;
	msg.m_GuildGuid = m_pOwnerGuild->GetGuildGuid();
	msg.m_bInOutState = bInOutState;
    // CHANGES: f110627.1L, fixed invalid string copy processing using different enumerations
    _tcsncpy(msg.m_CharName, m_pPlayer->GetCharName(), _countof(msg.m_CharName));
    msg.m_CharName[_countof(msg.m_CharName) - 1] = _T('\0');
	m_pPlayer->SendToGameDBPServer(&msg, sizeof(msg));
}

//==================================================================================================
// (f100527.4L) WAREHOUSE_TOTAL_INFO_TYPE = { _GUILDWAREHOUSE_TOTAL_INFO }
void CGuildWarehouseSlotContainer::SerializeItemInfo(_GUILDWAREHOUSE_TOTAL_INFO& total_info, eSERIALIZE serialize_type)
{
    POSTYPE number_of_max_slots = GetMaxSlotNum();
    if (number_of_max_slots > MAX_GUILD_WAREHOUSE_SLOT_NUM)
    {
        SUNLOG(eCRITICAL_LOG, _T("[CWarehouseSlotContainer] Invalid MaxSlotNum[%d/%d]"),
               number_of_max_slots, MAX_GUILD_WAREHOUSE_SLOT_NUM);
        SetMaxSlotNum(MAX_GUILD_WAREHOUSE_SLOT_NUM);
        number_of_max_slots = GetMaxSlotNum();
    }
    //

    if (serialize_type == SERIALIZE_LOAD)
    {
        total_info.m_Count = 0;
        _GUILDWAREHOUSE_TOTAL_INFO::SLOT_TYPE* slot_it = total_info.m_Slot;
        for (POSTYPE pos = 0; pos < number_of_max_slots; ++pos)
        {
            if (IsEmpty(pos)) {
                continue;
            }
            slot_it->m_Pos = pos;
            SCItemSlotContainer::SerializeItemStream(slot_it->m_Pos, &slot_it->m_Stream,
                                                     serialize_type);
            ++total_info.m_Count;
            ++slot_it;
        }
    }

    else if (serialize_type == SERIALIZE_STORE)
    {
        ASSERT(GetSlotNum() == 0);
        _GUILDWAREHOUSE_TOTAL_INFO::SLOT_TYPE* slot_it = total_info.m_Slot;
        for (POSTYPE total_count = total_info.m_Count;
             total_count;
             --total_count, ++slot_it)
        {
            SCItemSlotContainer::SerializeItemStream(slot_it->m_Pos, &slot_it->m_Stream,
                                                     serialize_type);
        }
        m_bChanged = false;
    }
}
