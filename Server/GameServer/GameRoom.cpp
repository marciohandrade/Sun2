#include "StdAfx.h"
#include ".\gameroom.h"
#include "Player.h"
#include "GameServerEx.h"
#include <ServerOptionParserEx.h>
#include "ChangeMonsterAbility.h"

#include "TriggerManagerEx.h"
#include "BattlezoneMissionTreeParser.h"


GameRoom::GameRoom(void)
	: m_MasterUserKey(0)
#ifdef _NA_008012_20150130_RANKING_SYSTEM
	, m_pRankingSystemCtl(0)
#endif // _NA_008012_20150130_RANKING_SYSTEM
    , party_zone_type_(PartyOfMax)
{
}

GameRoom::~GameRoom(void)
{
}

//	(WARNING)	- 이것이 호출 되기 전에 ChangeHandler가 호출되어야 한다.
void	GameRoom::OnCreate()
{
	GameZone::OnCreate();
	if(Policyment.FIELD & Policyment.ENABLE_TRIGGER)
	{
        CreateTrigger();
	}
#ifdef _NA_008012_20150130_RANKING_SYSTEM
	if(Policyment.FIELD & Policyment.ENABLE_RANKING)
		m_pRankingSystemCtl->OnCreate(this);
#endif // _NA_008012_20150130_RANKING_SYSTEM

#ifdef _NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
    if(party_zone_type_ != PartyOfMax) {
        static BattlezoneMissionTreeParser* paser_instance = BattlezoneMissionTreeParser::Instance();
        ObtainType obtain_type = paser_instance->GetObtainType(m_MapCode, party_zone_type_);
        if( obtain_type == kObtainType_Enable) {
            Policyment.SetField(Policyment.FIELD | Policyment.ENABLE_ITEM_OBTAIN_ALL_PARTY_MEMBER);
        }
    }
#endif //_NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN

}

void	GameRoom::OnDestroy()
{
	if(Policyment.FIELD & Policyment.ENABLE_TRIGGER)
	{
        DestroyTrigger();
	}

#ifdef _NA_008012_20150130_RANKING_SYSTEM
	if(Policyment.FIELD & Policyment.ENABLE_RANKING)
	{
		m_pRankingSystemCtl->OnDestroy();
        TAllocDelete(RankingSystemCtl, m_pRankingSystemCtl);
		m_pRankingSystemCtl = NULL;
	}
#endif // _NA_008012_20150130_RANKING_SYSTEM

	GameZone::OnDestroy();
}

VOID
GameRoom::Update( DWORD dwDeltaTick )
{
    GameZone::Update( dwDeltaTick );
}

VOID
GameRoom::ChangeHandler(const eZONETYPE kindOfRoom, const eINSTANCE_DUNGEON_KIND kindOfIndun)
{
#ifdef _NA_008012_20150130_RANKING_SYSTEM
    if (m_pRankingSystemCtl) {
        TAllocDelete(RankingSystemCtl, m_pRankingSystemCtl);
    }
	m_pRankingSystemCtl = NULL;
#endif // _NA_008012_20150130_RANKING_SYSTEM

	switch(kindOfRoom)
	{
	case eZONETYPE_LOBBY:
		{
			Policyment.SetField
				( Policyment.ENABLE_SUPERROOM
				| Policyment.ENABLE_PARTY
				);
		}
		break;
	case eZONETYPE_CHUNTING:
		{
			Policyment.SetField
				( Policyment.ENABLE_GROUP_ROOM_BASE
				);
#ifdef _NA_008012_20150130_RANKING_SYSTEM
			Policyment.SetField	// Skip
				( Policyment.FIELD &
					(~Policyment.ENABLE_RANKING
					//& ~Policyment.ENABLE_SAVE_LOCATION	<- ex
					)
				);
#endif // _NA_008012_20150130_RANKING_SYSTEM

		}
		break;
	case eZONETYPE_PVP:
		{
            Policyment.SetField
                ( Policyment.DISABLE_ALL
                );
		}
		break;
	case eZONETYPE_MISSION:
		{
			Policyment.SetField
				( Policyment.ENABLE_GROUP_ROOM_BASE
				| Policyment.ENABLE_SAVE_LOCATION
				);
#ifdef _NA_008012_20150130_RANKING_SYSTEM
            if(Ruleset.rules & Ruleset.ZONE_RULE_OPTION_ROOM_BY_PORTAL)
                Policyment.SetField(Policyment.FIELD & ~Policyment.ENABLE_RANKING);
            if (Policyment.FIELD & Policyment.ENABLE_RANKING) {
                m_pRankingSystemCtl = TAllocNew(RankingSystemCtl);
            }
#endif // _NA_008012_20150130_RANKING_SYSTEM
		}
		break;
	case eZONETYPE_INSTANCE:
		{
			BOOST_STATIC_ASSERT( INDUN_KIND_MAX == 7 );	// 추가 타입 존재시 하단 내용 추가할 것
			switch(kindOfIndun)
			{
			case INDUN_KIND_TOP_OF_TRIAL:
			case INDUN_KIND_MAYA_SCROLL:
				Policyment.SetField
					( Policyment.ENABLE_GROUP_ROOM_BASE
					| Policyment.ENABLE_SAVE_LOCATION
					);
#ifdef _NA_008012_20150130_RANKING_SYSTEM
				m_pRankingSystemCtl = TAllocNew(RankingSystemCtl);
#endif // _NA_008012_20150130_RANKING_SYSTEM
				break;
			case INDUN_KIND_100LEVEL_QUEST:
				Policyment.SetField
					( Policyment.ENABLE_GROUP_ROOM_BASE
					);
#ifdef _NA_008012_20150130_RANKING_SYSTEM
				Policyment.SetField	// Skip
					( Policyment.FIELD &
						(~Policyment.ENABLE_RANKING
						)
					);
#endif // _NA_008012_20150130_RANKING_SYSTEM
				break;
			case INDUN_KIND_EXT_03		:
			case INDUN_KIND_EXT_04		:
				{
					Policyment.SetField
						( Policyment.ENABLE_GROUP_ROOM_BASE
						);
#ifdef _NA_008012_20150130_RANKING_SYSTEM
					BOOLEAN disabled =
						!(Ruleset.rules
						& (Ruleset.ZONE_RULE_INDUN_RANKING_POINT|Ruleset.ZONE_RULE_INDUN_RANKING_TIME_ATTACK)
						);
					if(disabled)	Policyment.SetField(Policyment.FIELD & ~Policyment.ENABLE_RANKING);
					else			m_pRankingSystemCtl = TAllocNew(RankingSystemCtl);
#endif // _NA_008012_20150130_RANKING_SYSTEM
				}
				break;
			case INDUN_KIND_SS_QUEST:
				// <_ THINKING 기존의 랭킹은 해당 방의 모두에 해당되는 것이다.
				//	개별적으로 처리 가능 여부를 평가한다. (Ranking 추가가 가능한지 타전할 것)
				Policyment.SetField
					( Policyment.ENABLE_TRIGGER
#ifdef _NA_008012_20150130_RANKING_SYSTEM
					| Policyment.ENABLE_RANKING
#endif // _NA_008012_20150130_RANKING_SYSTEM
					| Policyment.ENABLE_NPC_UPDATE_REF_SKIP
					| Policyment.ENABLE_SSQ_CONTROL
					);
#ifdef _NA_008012_20150130_RANKING_SYSTEM
				m_pRankingSystemCtl = TAllocNew(RankingSystemCtl);
#endif // _NA_008012_20150130_RANKING_SYSTEM
				break;
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
            case INDUN_KIND_CHAOS_ZONE:
                Policyment.SetField
                    ( Policyment.ENABLE_TRIGGER
                    | Policyment.ENABLE_SAVE_LOCATION
                    | Policyment.ENABLE_GROUP_ROOM_BASE
                    | Policyment.ENABLE_SAVE_LOCATION
                    );
#ifdef _NA_008012_20150130_RANKING_SYSTEM
                m_pRankingSystemCtl = TAllocNew(RankingSystemCtl);
#endif // _NA_008012_20150130_RANKING_SYSTEM
                break;
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

			default:
				ASSERT(!"ChangeHandler Unexpected Indun Type");
				Policyment.SetField
					(Policyment.DISABLE_ALL
					);
				break;
			}
		}
		break;
	default:
		GameZone::ChangeHandler(kindOfRoom, kindOfIndun);
		break;
	}
}


BOOL GameRoom::Join(Player* pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector* pwvStartPos)
{
	if(GameZone::Join(pPlayer, FieldCode, AreaID, pwvStartPos))
	{
		if(Policyment.FIELD & Policyment.ENABLE_SAVE_LOCATION)
			pPlayer->SaveStartLocation();
        if(Policyment.FIELD & Policyment.ENABLE_CHANGE_MOB_ABILITY)
            g_ChangeMonsterAbility.NotifyChangeAbility(this, pPlayer);
#ifdef _NA_008012_20150130_RANKING_SYSTEM
		if(Policyment.FIELD & Policyment.ENABLE_RANKING)
			m_pRankingSystemCtl->JoinRoom(pPlayer, GetMapCode());
#endif // _NA_008012_20150130_RANKING_SYSTEM
		if(Policyment.FIELD & Policyment.ENABLE_TRIGGER)
		{
			GameField* pField = FindGameField(FieldCode);
			pField->GetTriggerManager()->MAP_STATE.SendInfo(pPlayer);
		}
#ifdef _NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
        if(Policyment.GetField() & Policyment.ENABLE_ITEM_OBTAIN_ALL_PARTY_MEMBER) {
            MSG_CG_PARTY_ON_ENTER_GAME_ROOM_BRD msg;
            msg.zone_policy_item_obtain_all_party_member_ = \
                !!(Policyment.GetField() & Policyment.ENABLE_ITEM_OBTAIN_ALL_PARTY_MEMBER);
            pPlayer->SendPacket(&msg, sizeof(msg));
        };
#endif //_NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
        return TRUE;
	}

	return FALSE;
}

VOID	GameRoom::Leave(Player* pPlayer)
{
    if(Policyment.FIELD & Policyment.ENABLE_SAVE_LOCATION)
		pPlayer->SaveStartLocation();
	GameZone::Leave(pPlayer);
#ifdef _NA_008012_20150130_RANKING_SYSTEM
	if(Policyment.FIELD & Policyment.ENABLE_RANKING)
		m_pRankingSystemCtl->LeaveRoom(pPlayer);
#endif // _NA_008012_20150130_RANKING_SYSTEM
}

VOID	GameRoom::LeaveForWarp(Player* pPlayer)
{
    if(Policyment.FIELD & Policyment.ENABLE_SAVE_LOCATION)
		pPlayer->SaveStartLocation();

    GameZone::Leave(pPlayer);
}

BOOL	GameRoom::MoveField(Player* pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector* pwvStartPos)
{
    if(GameZone::Join(pPlayer, FieldCode, AreaID, pwvStartPos))
    {
		if(Policyment.FIELD & Policyment.ENABLE_SAVE_LOCATION)
			pPlayer->SaveStartLocation();
		if(Policyment.FIELD & Policyment.ENABLE_TRIGGER)
		{
			GameField* pField = FindGameField(FieldCode);
			pField->GetTriggerManager()->MAP_STATE.SendInfo(pPlayer);
		}

        return TRUE;
	}
	return FALSE;
}

VOID GameRoom::ChangeMaster(DWORD dwNextMasterUserKey)
{
	if(Policyment.FIELD & Policyment.ENABLE_PARTY)
		SetMasterUserKey(dwNextMasterUserKey);
}

float GameRoom::GetBonusExp(Player* pPlayer)
{
	if(Policyment.FIELD & Policyment.ENABLE_SUPERROOM)
	{
		SERVER_COMMON_OPTION& stServerOptionInfo = ServerOptionParserEx::Instance()->GetServerOption();

		// 슈퍼방인지 체크
		if(m_RoomInfo.m_SpecialType == eROOM_SPECIAL_PCBANG_SUPER_ROOM)   
		{
			if(CanApplySuperMasterBenefit(pPlayer))
				return stServerOptionInfo.m_byPCBangSuperBangMasterAddExp / 100.f;
			else
				return stServerOptionInfo.m_byPCBangSuperBangNormalAddExp / 100.f;
		}
		else if(m_RoomInfo.m_SpecialType == eROOM_SPECIAL_ITEM1_SUPER_ROOM)
		{
			if(CanApplySuperMasterBenefit(pPlayer))
				return stServerOptionInfo.m_byItemSuperBangMasterAddExp / 100.f;
			else
				return stServerOptionInfo.m_byItemSuperBangNormalAddExp / 100.f;
		}
	}
	return 0;
}


BOOL GameRoom::CanApplySuperMasterBenefit(Player* pPlayer)
{
	if(pPlayer == 0)
		return FALSE;

	if(Policyment.FIELD & Policyment.ENABLE_SUPERROOM)
	{
		// PC방 혜택 슈퍼방이라면
		if(m_RoomInfo.m_SpecialType == eROOM_SPECIAL_PCBANG_SUPER_ROOM)
		{
			// 방장은 무조건 혜택
			if(pPlayer->GetUserKey() == GetMasterUserKey())
				return TRUE;

			// 슈퍼방장 아이템이 있는지 체크
			if(pPlayer->GetItemOwnFlag().FindItemInfo(eITEMTYPE_SUPERMASTER))
				return TRUE;
		}
		// 아이템 슈퍼방이라면
		else if(m_RoomInfo.m_SpecialType == eROOM_SPECIAL_ITEM1_SUPER_ROOM)
		{
			// 슈퍼방장 아이템이 있는지 체크
			if(pPlayer->GetItemOwnFlag().FindItemInfo(eITEMTYPE_SUPERMASTER))
				return TRUE;
		}
	}
	return FALSE;
}


























