#ifdef _NA_008012_20150130_RANKING_SYSTEM
#pragma once

struct st_PlayerMissionPoint : public st_RANKING_POINT
{
	//Player*						pPlayer;	
    CHARGUID char_guid; //player->GetCharGuid();
	BOOL		bReady;

	st_PlayerMissionPoint()
	{
		//pPlayer = NULL;
        char_guid = 0;
		bReady = FALSE;				//< 미션에 들어온 후 DB에서 Point를 받아오면 TRUE
	}
};

//	(WAVERIX)(080928)(NOTE) key_type은 charguid로 해두고, 실제로 userguid로 처리하고 있다. -.-;;;;
typedef STLX_HASH_MAP<CHARGUID, st_PlayerMissionPoint>	MISSION_POINT_HASH;

class MissionPointManager
{
public:		VOID				OnCreate();
			VOID				OnDestroy();
			VOID				KillMonster( eNPC_GRADE grade, GameZone * pZone, WORD PointIndex );
			VOID				JoinPlayer( Player *pPlayer, MAPCODE MapCode );
			VOID				LeavePlayer( Player *pPlayer, MAPCODE MapCode );
			VOID				SetPlayerPoint( USERGUID UserGUID, DWORD Point, RANKINGKEY DBKey );
			//	(SpecialCmd) 타락한 사원용 최종 정산 처리 목적
			//	- 타락한 사원용 처리 방법 (KillMonster는 호출되지 않는다. 최종 계산된 값을 통해 일괄 처리된다.)
			VOID				SetLastSettleAccount(
										Player* const pPlayer
										, const MAPCODE mapCode
										, const WORD indexOfPoint
										, const WORD totalPoint
										);

private:	MISSION_POINT_HASH	m_HashPlayerPoint;	
};
#endif // _NA_008012_20150130_RANKING_SYSTEM