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
		bReady = FALSE;				//< �̼ǿ� ���� �� DB���� Point�� �޾ƿ��� TRUE
	}
};

//	(WAVERIX)(080928)(NOTE) key_type�� charguid�� �صΰ�, ������ userguid�� ó���ϰ� �ִ�. -.-;;;;
typedef STLX_HASH_MAP<CHARGUID, st_PlayerMissionPoint>	MISSION_POINT_HASH;

class MissionPointManager
{
public:		VOID				OnCreate();
			VOID				OnDestroy();
			VOID				KillMonster( eNPC_GRADE grade, GameZone * pZone, WORD PointIndex );
			VOID				JoinPlayer( Player *pPlayer, MAPCODE MapCode );
			VOID				LeavePlayer( Player *pPlayer, MAPCODE MapCode );
			VOID				SetPlayerPoint( USERGUID UserGUID, DWORD Point, RANKINGKEY DBKey );
			//	(SpecialCmd) Ÿ���� ����� ���� ���� ó�� ����
			//	- Ÿ���� ����� ó�� ��� (KillMonster�� ȣ����� �ʴ´�. ���� ���� ���� ���� �ϰ� ó���ȴ�.)
			VOID				SetLastSettleAccount(
										Player* const pPlayer
										, const MAPCODE mapCode
										, const WORD indexOfPoint
										, const WORD totalPoint
										);

private:	MISSION_POINT_HASH	m_HashPlayerPoint;	
};
#endif // _NA_008012_20150130_RANKING_SYSTEM