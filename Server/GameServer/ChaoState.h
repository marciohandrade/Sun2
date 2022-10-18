#pragma once

#include <Timer.h>
#include <time.h>
#include <set>

class Player;	class NPC;

	#define	CHAO_USE_STORE			(1<<0)				//상점이용가능	
	#define	CHAO_USE_RANKUP			(1<<1)				//랭크업 이용가능
	#define	CHAO_USE_CRYSTALIZE		(1<<2)				//결정화 이용가능
	#define	CHAO_USE_ENCHANT		(1<<3)				//인첸트 이용가능
	#define	CHAO_USE_COMPOSE		(1<<4)				//조합 이용가능
	#define	CHAO_USE_PORTAL			(1<<5)				//포탈 이용가능
	#define	CHAO_USE_QUEST			(1<<6)				//퀘스트 이용가능
	#define	CHAO_USE_EVENT			(1<<7)				//이벤트 이용가능

 #ifdef __CN_0_20090617_PRE_CHAO_USE_FLAG
  	#define PRE_CHAO_USE_FLAG ( (1<<8)-1 )              // 중국은 모두 이용가능
 #else
    #define PRE_CHAO_USE_FLAG ( CHAO_USE_QUEST )
 #endif
	#define FUL_CHAO_USE_FLAG ( 0 )


class ChaoState
{
    typedef STLX_HASH_MAP<DWORD, STRUCT_REVENGE_INFO>		HASHMAP_BADFRIEND;
	enum { NOTIFY_CHAOTIME_UPDATE_CYCLE = 60 * 60 * 1000 };
public:
	ChaoState( Player *pPlayer );
	~ChaoState();

	VOID				Init();
	VOID				Update();
	VOID				OnDead();

	VOID				StartBoradoriState( Player *pTarget );
	VOID				DecreaseChaoTime( WORD wDecSeconds );
	VOID				KillMonster( NPC *pNPC );

	eCHAO_STATE			GetChaoState();
	VOID				ClearChaoState();
	VOID				SendLeavedChaoTime();

	BOOL				IsBoradori()	{ return m_bBoradori; }
	BOOL				IsChaoState()	{ return GetChaoState() == eCHAO_STATE_CHAO; }

	// 리벤지 리스트
	BOOL				FindBadFriend( DWORD dwCharGuid );
	BOOL				AddBadFriend( STRUCT_REVENGE_INFO & RevengeInfo );
	BOOL				RemoveBadFriend( DWORD dwCharGuid );
	inline VOID			ClearBadFriend()	{ m_mapBadFriend.clear(); }

	BOOL				IsRevengeName( const TCHAR * ptszRevengeName );
	VOID				SetRevengeName( const TCHAR * ptszRevengeName );

	RC::eREVENGE_RESULT	TrackRevenge( const TCHAR * ptszRevengeName, POSTYPE atPos );	// 리벤지 대상으로 워프
	BOOL				DeleteRevengeStone( POSTYPE atPos );

	// 카오 상태
	VOID				UpChaoState();
	VOID				DownChaoState();
	// 보라돌이 상태
	VOID				EndBoradoriState();
	// 리벤지 리스트
	VOID				RequestRemoveBadFriend( DWORD dwCharGuid );		// 리벤지 대상 삭제 요청
	BOOL				CanUseFuction( DWORD Flag );

	bool HostilityRelationCheck(Player *pTarget);


private:
	VOID				SetChaoState( eCHAO_STATE eChaoState );

public:
	VOID				SetChaoEndTime( time_t EndTime );

public:
	time_t				GetChaoEndTime();

private:
	Player *			m_pPlayer;
	util::Timer			m_NotifyTimer;			// 1시간 간격으로 카오의 남은시간을 알려준다.
	BOOL				m_bBoradori;			// 보라돌이 상태인가
	util::Timer			m_BoradoriTimer;		// 보라돌이 유지시간
	HASHMAP_BADFRIEND	m_mapBadFriend;			// 리벤지(나쁜친구) 리스트
	TCHAR				m_ptszRevengeName[MAX_CHARNAME_LENGTH+1];	// 리벤지 등록 예상 유저이름
};

