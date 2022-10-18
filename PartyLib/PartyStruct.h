#pragma once
#pragma pack(push,1)

//template < typename SLOTETYPE >
//struct sTEMPLATE_TOTAL_INFO
//{
//	explicit sTEMPLATE_TOTAL_INFO() : m_Count(0) {}
//	typedef SLOTETYPE SLOT_TYPE;
//	enum { MAX_SLOT_NUM = 0xFF, };
//	BYTE		m_Count;
//	SLOTETYPE	m_Slot[MAX_SLOT_NUM];
//	void Clear() { m_Count = 0; }
//	int GetSize()
//	{
//		return (sizeof(*this)-(MAX_SLOT_NUM-m_Count)*sizeof(this->m_Slot[0]));
//	}
//};

struct _PARTY_MEMBER_INFO
{
	DWORD m_dwMemberKey;							    // 멤버키(ObjKey)
	char m_szCharName[MAX_CHARNAME_LENGTH + 1];	        // 캐릭터 ID
	WORD m_wLevel;								        // 레벨
	BYTE m_byClassType		: 4;				        // 클래스 타입(eCHAR_TYPE)
	BYTE m_byChaoState		: 4;				        // 카오상태
	BYTE m_byHPRatio;							        // 현재 HP 비율
	BYTE m_byMPRatio;							        // 현재 MP 비율
#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
    BYTE m_bySDRatio;							        // 현재 SD 비율
#endif // _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
#ifdef _NA_002050_20110216_ADD_GENDER
    BYTE m_byGender;                                    // 성별
#endif
#ifdef _DH_BATTLEZONE2_
    bool m_isReady;                                     // 준비가 완료된 파티원인지 확인
    BYTE m_selecteTeam;                                 // PVP미션을 위해 선택한 팀(0:선택안함 /1,2,3,.. : 각팀)
    DWORD map_code_;                                    // 유저가 위치해 있는 맵코드
  #ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
    bool m_isConnected;                                 // 현재 접속중인 상태
  #endif//_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
#endif //_DH_BATTLEZONE2_

    _PARTY_MEMBER_INFO()
    {
        BOOST_STATIC_ASSERT(GenderType::kAll == 0);
        ZeroMemory(this, sizeof(*this));
    }
};

struct _GUIDANCE_MEMBER_INFO
{
	BYTE m_PartyKey		: 4;
	BYTE m_bMaster		: 4;
	_PARTY_MEMBER_INFO m_MemberInfo;
};

typedef sTEMPLATE_TOTAL_INFO<_PARTY_MEMBER_INFO> PARTY_MEMBER_TOTAL_INFO;
typedef sTEMPLATE_TOTAL_INFO<_GUIDANCE_MEMBER_INFO> GUIDANCE_MEMBER_TOTAL_INFO;

//_NA_20110630_BATTLE_ZONE_RENEWAL
struct BasePartyRoomInfo
{
    MAPCODE map_code_;                                      // 파티룸 미션 코드
    BYTE special_type_;                                     // eROOM_SPECIAL_TYPE
    TCHAR party_room_title_[MAX_ROOMTITLE_LENGTH + 1];      // 파티룸 제목
    TCHAR party_room_password_[MAX_ROOMPASSWORD_LENGTH + 1];// 파티룸 비밀 번호
    PartyZoneType party_room_type_;                         // 파티룸 타입

    BasePartyRoomInfo() {
        ZeroMemory(this, sizeof(*this));
    }
};

struct PartyRoomInfo
{
    KEYTYPE room_key_;                                      // 룸키(index)
    WORD link_party_key_;                                   // 룸에 소속된 파티 키
    bool is_mission_start_;                                 // 미션룸 출발 여부
    struct {
        BYTE current_user_num_   : 4;                       // 현재 인원
        BYTE max_user_num_       : 4;                       // 최대 인원(NonBlock Slot 개수)
    };
    BasePartyRoomInfo base_party_room_info;                 // 파티룸 기본 정보
};

//_NA_003033_20110831_DICECAST_ITEM_ROUTING
struct _DICECAST_RESULT
{
    DWORD member_key;
    WORD  random_num;

    _DICECAST_RESULT()
        : member_key(0), random_num(0)
    {
    }
};

#pragma pack(pop)