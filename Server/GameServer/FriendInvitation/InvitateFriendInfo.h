#pragma once
#ifndef GAMESERVER_INVITED_FRIEND_INFO_H
#define GAMESERVER_INVITED_FRIEND_INFO_H
/*

작성자: 문상현
제작일: 2009-11-05

간단한설명: Player 의 추천 친구 목록을 가지고 있는다.
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

    //소환시간 정보 업데이트
    VOID UpdateReCallTime(CHARGUID FriendGuid, DWORD64 Recalltime);
    //친구 소환 시간 설정
    VOID InitReCallTime(MSG_DG_EVENT_INVITATEFRIEND_RECALLSELECT_ACK* pRecvMsg);
    // (CHANGES) (f100515.2L) rule out the possibility of a stack overflow
    UINT GetFriendList(const FRIEND_TYPE type,
                       DWORD OUT friend_guid_array[], int number_of_guid_array);
    //추천 친구 목록 저장
	VOID AddFriend(FRIEND_TYPE type, DWORD FriendGUID[], WORD FriendCount);
    VOID AddFriend(FRIEND_TYPE type, DWORD FriendGUID);
    //추천 친구 목록 초기화
    VOID ClearFriendList(FRIEND_TYPE type);
    VOID ClearAllFriendList();
    //친구 목록에서 제거한다.
    VOID RemoveFriend(FRIEND_TYPE type, DWORD FriendGUID);
    //친구 목록 카운트
    int GetSize(FRIEND_TYPE type);


    //해당 GUID 가 추천 친구 목록에 있는가 ?
    BOOL IsInvitateFriend(CHARGUID charguid);
    //해당 GUID 가 파티내 추천 친구 목록에 있는가 ?
    BOOL IsInvitateFriendParty();
    //추가 경험치 비율을 반환환한다.
    float GetAddExpRate(const BYTE radius, const WzVector* const pwvNpcPos, const FLOAT fRadius_Q2);
    BYTE GetAddQuestExpRate(FIELDCODE field_code);
    WORD GetAddMissionExpRate(FIELDCODE field_code);
    //최근 리콜 시간을 반환한다.
    DWORD64 GetRecallTime(CHARGUID CharGuid);

private:
    //소환시간 정보 업데이트
    VOID UpdateReCallTime(FRIEND_TYPE type, CHARGUID FriendGuid, DWORD64 Recalltime);

private:

	INVITATFRIEND_INFO_MAP* const GetFriendList(FRIEND_TYPE type); 

	INVITATFRIEND_INFO_MAP m_vecInvitateFriend;		    //내가 추천한 캐릭터 GUID
	INVITATFRIEND_INFO_MAP m_vecBeInvitatedFriend;	    //나를 추천한 캐릭터 GUID
    INVITATFRIEND_INFO_MAP m_vecnvitateFriendInParty;   //파티내의 추천관계 캐릭 GUID

    Player* player_;
};

#endif //GAMESERVER_INVITED_FRIEND_INFO_H