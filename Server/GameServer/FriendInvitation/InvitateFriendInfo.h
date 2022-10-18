#pragma once
#ifndef GAMESERVER_INVITED_FRIEND_INFO_H
#define GAMESERVER_INVITED_FRIEND_INFO_H
/*

�ۼ���: ������
������: 2009-11-05

�����Ѽ���: Player �� ��õ ģ�� ����� ������ �ִ´�.
// NOTE: declared by _KR_0_20091021_FIRENDINVITATION_EXTEND
*/

#pragma once

typedef STLX_MAP<DWORD, DWORD64>	                INVITATFRIEND_INFO_MAP;
typedef INVITATFRIEND_INFO_MAP::iterator			INVITATFRIEND_INFO_MAP_ITER;
typedef std::pair< DWORD, DWORD64 >		            INVITATFRIEND_INFO_MAP_PAIR;

#include "PacketStruct_DG.h"

class InvitateFriendInfo
{
public:
	enum FRIEND_TYPE {FRIEND_INVIATEINPARTY = 0, FRIEND_NORMAL = 1, FRIEND_INVIATE = 5, FRIEND_BEINVIATED = 6};
public:
	InvitateFriendInfo(Player* player);
	~InvitateFriendInfo(void);

    //��ȯ�ð� ���� ������Ʈ
    VOID UpdateReCallTime(CHARGUID FriendGuid, DWORD64 Recalltime);
    //ģ�� ��ȯ �ð� ����
    VOID InitReCallTime(MSG_DG_EVENT_INVITATEFRIEND_RECALLSELECT_ACK* pRecvMsg);
    // (CHANGES) (f100515.2L) rule out the possibility of a stack overflow
    UINT GetFriendList(const FRIEND_TYPE type,
                       DWORD OUT friend_guid_array[], int number_of_guid_array);
    //��õ ģ�� ��� ����
	VOID AddFriend(FRIEND_TYPE type, DWORD FriendGUID[], WORD FriendCount);
    VOID AddFriend(FRIEND_TYPE type, DWORD FriendGUID);
    //��õ ģ�� ��� �ʱ�ȭ
    VOID ClearFriendList(FRIEND_TYPE type);
    VOID ClearAllFriendList();
    //ģ�� ��Ͽ��� �����Ѵ�.
    VOID RemoveFriend(FRIEND_TYPE type, DWORD FriendGUID);
    //ģ�� ��� ī��Ʈ
    int GetSize(FRIEND_TYPE type);


    //�ش� GUID �� ��õ ģ�� ��Ͽ� �ִ°� ?
    BOOL IsInvitateFriend(CHARGUID charguid);
    //�ش� GUID �� ��Ƽ�� ��õ ģ�� ��Ͽ� �ִ°� ?
    BOOL IsInvitateFriendParty();
    //�߰� ����ġ ������ ��ȯȯ�Ѵ�.
    float GetAddExpRate(const BYTE radius, const WzVector* const pwvNpcPos, const FLOAT fRadius_Q2);
    BYTE GetAddQuestExpRate(FIELDCODE field_code);
    WORD GetAddMissionExpRate(FIELDCODE field_code);
    //�ֱ� ���� �ð��� ��ȯ�Ѵ�.
    DWORD64 GetRecallTime(CHARGUID CharGuid);

private:
    //��ȯ�ð� ���� ������Ʈ
    VOID UpdateReCallTime(FRIEND_TYPE type, CHARGUID FriendGuid, DWORD64 Recalltime);

private:

	INVITATFRIEND_INFO_MAP* const GetFriendList(FRIEND_TYPE type); 

	INVITATFRIEND_INFO_MAP m_vecInvitateFriend;		    //���� ��õ�� ĳ���� GUID
	INVITATFRIEND_INFO_MAP m_vecBeInvitatedFriend;	    //���� ��õ�� ĳ���� GUID
    INVITATFRIEND_INFO_MAP m_vecnvitateFriendInParty;   //��Ƽ���� ��õ���� ĳ�� GUID

    Player* player_;
};

#endif //GAMESERVER_INVITED_FRIEND_INFO_H