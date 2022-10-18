#pragma once
//=============================================================================================================================
/// PartyManager class
/**
	@author	Sung Hyun Chang < whatthehell@webzen.co.kr >
	@since	2006. 03. 15
	@remark
		- ���弭������ ��Ƽ�� �����ϴ� Ŭ����
	@note
		- ������Ʈ �������� �Ѿ�� ��Ƽ ������ �����Ѵ�
		- ��Ƽ �Ἲ/��� �߰�/Ż��/�ػ꿡 ���� ��Ƽ Ŭ���� �ν��Ͻ� ����
	@history 
		-  2006. 03. 15 Create
*/
//=============================================================================================================================

#include <struct.h>
#include <SolarHashTable.h>
#include <ResultCode.h>

class User;
//class Party;
class CommChatZone;

typedef util::SolarHashTable<CommChatZone*>					PARTY_HASH;
typedef util::SolarHashTable<CommChatZone*>::iterator		PARTY_HASH_ITR;

class PartyManager
{
public:
	PartyManager();
	~PartyManager();

	CommChatZone*	CreateParty( WORD wPartyKey );
	BOOL	JoinParty( WORD wPartyKey, User *pJoinUser );
	BOOL	WithdrawParty( User *pWithdrawUser );
	VOID	DestroyParty( WORD wPartyKey );
	VOID	RestoreParty( WORD wPartyKey, User *pUser );
#ifdef _NA_004694_20120412_1202_PARTY_ROOM_KEY
    VOID    SetRoomKey( const WORD party_key, const WORD room_key );
#endif  // _NA_004694_20120412_1202_PARTY_ROOM_KEY

	VOID	SendToParty( WORD wPartyKey, BYTE* pMsg, WORD wSize );
	VOID	SendToAll( BYTE* pMsg, WORD wSize );

private:
	PARTY_HASH							m_HashParty;
	util::CMemoryPoolFactory<CommChatZone>	*m_pPartyPool;
};
