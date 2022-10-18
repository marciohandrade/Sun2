#pragma once

#include <Timer.h>
#include <time.h>
#include <set>

class Player;	class NPC;

	#define	CHAO_USE_STORE			(1<<0)				//�����̿밡��	
	#define	CHAO_USE_RANKUP			(1<<1)				//��ũ�� �̿밡��
	#define	CHAO_USE_CRYSTALIZE		(1<<2)				//����ȭ �̿밡��
	#define	CHAO_USE_ENCHANT		(1<<3)				//��þƮ �̿밡��
	#define	CHAO_USE_COMPOSE		(1<<4)				//���� �̿밡��
	#define	CHAO_USE_PORTAL			(1<<5)				//��Ż �̿밡��
	#define	CHAO_USE_QUEST			(1<<6)				//����Ʈ �̿밡��
	#define	CHAO_USE_EVENT			(1<<7)				//�̺�Ʈ �̿밡��

 #ifdef __CN_0_20090617_PRE_CHAO_USE_FLAG
  	#define PRE_CHAO_USE_FLAG ( (1<<8)-1 )              // �߱��� ��� �̿밡��
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

	// ������ ����Ʈ
	BOOL				FindBadFriend( DWORD dwCharGuid );
	BOOL				AddBadFriend( STRUCT_REVENGE_INFO & RevengeInfo );
	BOOL				RemoveBadFriend( DWORD dwCharGuid );
	inline VOID			ClearBadFriend()	{ m_mapBadFriend.clear(); }

	BOOL				IsRevengeName( const TCHAR * ptszRevengeName );
	VOID				SetRevengeName( const TCHAR * ptszRevengeName );

	RC::eREVENGE_RESULT	TrackRevenge( const TCHAR * ptszRevengeName, POSTYPE atPos );	// ������ ������� ����
	BOOL				DeleteRevengeStone( POSTYPE atPos );

	// ī�� ����
	VOID				UpChaoState();
	VOID				DownChaoState();
	// ������ ����
	VOID				EndBoradoriState();
	// ������ ����Ʈ
	VOID				RequestRemoveBadFriend( DWORD dwCharGuid );		// ������ ��� ���� ��û
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
	util::Timer			m_NotifyTimer;			// 1�ð� �������� ī���� �����ð��� �˷��ش�.
	BOOL				m_bBoradori;			// ������ �����ΰ�
	util::Timer			m_BoradoriTimer;		// ������ �����ð�
	HASHMAP_BADFRIEND	m_mapBadFriend;			// ������(����ģ��) ����Ʈ
	TCHAR				m_ptszRevengeName[MAX_CHARNAME_LENGTH+1];	// ������ ��� ���� �����̸�
};

