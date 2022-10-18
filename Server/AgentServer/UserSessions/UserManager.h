#pragma once

//////////////////////////////////////////////////////////////////////////
// <STL>

//////////////////////////////////////////////////////////////////////////
// <UTILITY>

//////////////////////////////////////////////////////////////////////////
//	<COMMON>
#include <ObjectKeyStream.h>

//////////////////////////////////////////////////////////////////////////
// <PROTOTYPES>
struct MSG_AM_CONNECTION_CCU_REPORT_NTF;

struct PlayerObjectKeyStreamAddin
{
	DWORD		CharGuid;
	PlayerObjectKeyStreamAddin()
		: CharGuid(0)
	{}
	PlayerObjectKeyStreamAddin(const DWORD charGuid)
		: CharGuid(charGuid)
	{}
	PlayerObjectKeyStreamAddin(const PlayerObjectKeyStreamAddin& rSrc)
		: CharGuid(rSrc.CharGuid)
	{}

	PlayerObjectKeyStreamAddin& operator = (const PlayerObjectKeyStreamAddin& rSrc)
	{
		CharGuid = rSrc.CharGuid;
		return *this;
	}
};

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<UserManager>
class UserManager
{
public:
	//////////////////////////////////////////////////////////////////////////
	// <TYPE DECLARATION>
private:
	typedef USER_HASH						USER_AUTH_IDS;
	typedef USER_HASH_IT					USER_AUTH_IDS_IT;
	typedef USER_HASH_PAIR					USER_AUTH_IDS_PAIR;
	typedef STLX_LIST<TempUser*>			TEMPUSER_LIST;
	typedef TEMPUSER_LIST::iterator			TEMPUSER_LIST_IT;
	typedef STLX_MAP<DWORD, DWORD>			CONNECT_BLOCK_MAP;
	typedef CONNECT_BLOCK_MAP::iterator		CONNECT_BLOCK_MAP_IT;

	class UserDelegator
	{
	private:
        User*	m_pUserRef;
	public:
        UserDelegator() : m_pUserRef(NULL) {}
        UserDelegator(User* pUserRef) : m_pUserRef(pUserRef) {}
        UserDelegator(const UserDelegator& REF) : m_pUserRef(REF.m_pUserRef) {}
        inline User*& USER() { return m_pUserRef; }
	};

	typedef ObjectKeyStream<User,
                            nsKeyInfo::PLAYER_OBJECT_KEY_START, nsKeyInfo::PLAYER_OBJECT_KEY_LAST,
                            PlayerObjectKeyStreamAddin> ObjectKeyStream;
	typedef STLX_HASH_MAP<DWORD, DWORD> CHARGUID_HASH;
    typedef CHARGUID_HASH               CHARNAME_HASH; // CHANGES: f110517.3L
	typedef STLX_HASH_MAP<STLX_STRING, UserDelegator>	M_USER_LIST;
	typedef M_USER_LIST::iterator						M_USER_LIST_IT;
	typedef std::pair<STLX_STRING, UserDelegator>			M_USER_LIST_PAIR;

private:
    UserManager();
    ~UserManager();
public:
    static VOID CreateInstance();
    static VOID DestroyInstance();
    static UserManager* Instance();

//////////////////////////////////////////////////////////////////////////
// <MANAGER INTERFACE>
public:		VOID    ReleaseAllUser();

public:
	inline	User*   GetUser(const DWORD userKey) const;
	inline	User*   GetUserByObjKey(const DWORD objectKey) const;
	inline	User*   GetUserByCharGuid(const DWORD charKey) const;
	inline	User*   GetUserByAuthID(const DWORD authKey) const;
public:
    BOOL    AddUser(User* pUserSession);
    VOID    RemoveUser(DWORD dwUserKey);
    User*   FindUserForCharName(const TCHAR* pszName);
    // CHANGES: f110516.3L, changes to the pair information insertion on select character
    bool AddUserForSelectedCharInfoHashes(User* const user);
    void RemoveUserForSelectedCharInfoHashes(User* const user);

    inline DWORD    GetNumberOfUsers() { return (DWORD)m_UserHashTable.size(); }

    // <TEMP USER MANAGEMENT>
    //   TempUser::OnAccept시에 추가되고, Timeout Routine에 의해 삭제된다.
    inline VOID	    __AddTempUser(TempUser* pTempUser)
    { if(pTempUser) m_TempUserList4Timeout.push_back(pTempUser); }

    // CHANGES: f110516.3L, renewaled 'Foreach' interface
    void Foreach(ns_functor::IUserForeachFunctor* functor) const;
    // CHANGES: f110516.3L, unity function for all user notification
    // CHANGES: f110609.4L, changes the method action that include the packing
    // whether the message need packing
    void SendMessageToAllUsers(MSG_BASE* msg, uint16_t msg_size);

    // for loop 외부에서 하기 위한 것... 이것에 대한 별도의 연산은 하지 말것! 모듈성 깨진다!
    inline const USER_HASH& RefUserList() { return m_UserHashTable; }

    VOID DisconnectLinkedServerUser(eSERVER_TYPE Type);
//////////////////////////////////////////////////////////////////////////
// <TIMER UPDATER> - by "AgentServer.Timer.WorkLoad"
    VOID GarbageCollector();
    VOID TempUserClosureTimeout();
    // (WAVERIX) (CHANGES) ChangeName + AddinFunction
    // - #1 : CalculationCCU -> Process
    // - #2 : + function for 'server select'
    VOID Process(BOOLEAN bCCUControl, BOOLEAN bServerSelectProcess,
                 MSG_AM_CONNECTION_CCU_REPORT_NTF* OUT pINFO /* + args...*/);

//////////////////////////////////////////////////////////////////////////
// <SESSION POOL>
public:
    UserSession*    AllocUser(eUSER_TYPE type);
    VOID            FreeUser(UserSession* pUser);

private:
    VOID  _FreeUserPair(const USER_HASH_PAIR& rPAIR);

//////////////////////////////////////////////////////////////////////////
// <FIELDs>
private:
    USER_HASH       m_UserHashTable;
    ObjectKeyStream* m_pObjectKeyStream;
    USER_AUTH_IDS   m_UserAuthIDs;
    CHARGUID_HASH   m_CharGuidUserHash;
    CHARNAME_HASH   m_CharNameHashTable;
    TEMPUSER_LIST   m_TempUserList4Timeout;
/////////////////////////////////////////////////////////////////////////////////////////////////////////
};


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//

//////////////////////////////////////////////////////////////////////////
// <FIND INTERFACEs>
inline User*
UserManager::GetUser(const DWORD userKey) const
{
	USER_HASH::const_iterator itFind = m_UserHashTable.find(userKey);
	return itFind != m_UserHashTable.end()
		?	itFind->second
		:	NULL
		;
}

inline User*
UserManager::GetUserByObjKey(const DWORD objectKey) const
{
	const ObjectKeyStream::sNode* pNode = m_pObjectKeyStream->GetObjectInfo(objectKey);
	return (pNode)
		?	pNode->pObject
		:	0
		;
}

inline User*
UserManager::GetUserByCharGuid(const DWORD charKey) const
{
	CHARGUID_HASH::const_iterator itFind = m_CharGuidUserHash.find(charKey);
	if(itFind != m_CharGuidUserHash.end())
	{
		const DWORD objectKey = itFind->second;
		const ObjectKeyStream::sNode* pNode = m_pObjectKeyStream->GetObjectInfo(objectKey);
		return (pNode)
			?	pNode->pObject
			:	0
			;
	}
	return 0;
}

inline User*
UserManager::GetUserByAuthID(const DWORD authKey) const
{
	USER_AUTH_IDS::const_iterator itFind = m_UserAuthIDs.find(authKey);
	return itFind != m_UserAuthIDs.end()
		?	itFind->second
		:	NULL
		;
}
#if SUN_CODE_BACKUP
//////////////////////////////////////////////////////////////////////////
//	<FOREACH ROUTINEs>
//	(WAVERIX)(080704) - class 안에 있는 코드 루틴들 외부로 뺐음.
template <typename ObjectOperator>
inline VOID
UserManager::Foreach(ObjectOperator& op)
{
	FOREACH_CONTAINER(const USER_HASH_PAIR& rPair, m_UserHashTable, USER_HASH)
	{
		User* pUser = rPair.second;
		op(pUser);
	}
}

template<typename _FN, typename _OBJ>
inline VOID
UserManager::Foreach(const _FN& _funcRaw, _OBJ& _Left) const
{
	FOREACH_CONTAINER(const USER_HASH_PAIR& rPair, m_UserHashTable, USER_HASH)
	{
		boost::bind(_funcRaw, &_Left, _1)(rPair);
	}
}
#endif //SUN_CODE_BACKUP

inline void UserManager::Foreach(ns_functor::IUserForeachFunctor* functor) const
{
    USER_HASH::const_iterator it = m_UserHashTable.begin(),
                              end = m_UserHashTable.end();
    for (; it != end; ++it)
    {
        User* user = it->second;
        functor->operator()(user);
    };
}

//////////////////////////////////////////////////////////////////////////
//

template <typename PACKET_TYPE>
class UserPacketSender
{
	PACKET_TYPE& m_rMSG;
public:
	UserPacketSender(PACKET_TYPE& rMSG) : m_rMSG(rMSG) {}
	~UserPacketSender() {}

	VOID operator() (User* pUser)
	{
		pUser->SendPacket(&m_rMSG, sizeof(m_rMSG));
	}
};







