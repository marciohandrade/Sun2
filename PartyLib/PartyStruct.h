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
	DWORD m_dwMemberKey;							    // ���Ű(ObjKey)
	char m_szCharName[MAX_CHARNAME_LENGTH + 1];	        // ĳ���� ID
	WORD m_wLevel;								        // ����
	BYTE m_byClassType		: 4;				        // Ŭ���� Ÿ��(eCHAR_TYPE)
	BYTE m_byChaoState		: 4;				        // ī������
	BYTE m_byHPRatio;							        // ���� HP ����
	BYTE m_byMPRatio;							        // ���� MP ����
#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
    BYTE m_bySDRatio;							        // ���� SD ����
#endif // _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
#ifdef _NA_002050_20110216_ADD_GENDER
    BYTE m_byGender;                                    // ����
#endif
#ifdef _DH_BATTLEZONE2_
    bool m_isReady;                                     // �غ� �Ϸ�� ��Ƽ������ Ȯ��
    BYTE m_selecteTeam;                                 // PVP�̼��� ���� ������ ��(0:���þ��� /1,2,3,.. : ����)
    DWORD map_code_;                                    // ������ ��ġ�� �ִ� ���ڵ�
  #ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
    bool m_isConnected;                                 // ���� �������� ����
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
    MAPCODE map_code_;                                      // ��Ƽ�� �̼� �ڵ�
    BYTE special_type_;                                     // eROOM_SPECIAL_TYPE
    TCHAR party_room_title_[MAX_ROOMTITLE_LENGTH + 1];      // ��Ƽ�� ����
    TCHAR party_room_password_[MAX_ROOMPASSWORD_LENGTH + 1];// ��Ƽ�� ��� ��ȣ
    PartyZoneType party_room_type_;                         // ��Ƽ�� Ÿ��

    BasePartyRoomInfo() {
        ZeroMemory(this, sizeof(*this));
    }
};

struct PartyRoomInfo
{
    KEYTYPE room_key_;                                      // ��Ű(index)
    WORD link_party_key_;                                   // �뿡 �Ҽӵ� ��Ƽ Ű
    bool is_mission_start_;                                 // �̼Ƿ� ��� ����
    struct {
        BYTE current_user_num_   : 4;                       // ���� �ο�
        BYTE max_user_num_       : 4;                       // �ִ� �ο�(NonBlock Slot ����)
    };
    BasePartyRoomInfo base_party_room_info;                 // ��Ƽ�� �⺻ ����
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