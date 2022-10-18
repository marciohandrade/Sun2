#include "stdafx.h"
//////////////////////////////////////////////////////////////////////////
#include <AgentServer.h>
//////////////////////////////////////////////////////////////////////////
#include "UserManager.h"
#include "User.h"
#include "TempUser.h"

#include <PacketControl/PacketControl.h>
#include <Function/CCUControl.h>
#include <ServerSessions/ServerSessionEx.h>

//////////////////////////////////////////////////////////////////////////
static UserManager*		s_pUserManager = NULL;

//////////////////////////////////////////////////////////////////////////
VOID
UserManager::CreateInstance()
{
	if(!s_pUserManager)
		s_pUserManager = new UserManager();
}

VOID
UserManager::DestroyInstance()
{
	SAFE_DELETE(s_pUserManager);
}

UserManager*
UserManager::Instance()
{
	return s_pUserManager;
}

//////////////////////////////////////////////////////////////////////////
//
UserManager::UserManager()
	: m_pObjectKeyStream(new ObjectKeyStream())
{
}

UserManager::~UserManager()
{
	ASSERT("ReleaseUser()를 하지 않았다." && !m_UserHashTable.size());
	SAFE_DELETE(m_pObjectKeyStream);
}

VOID
UserManager::ReleaseAllUser()
{
	FOREACH_CONTAINER(const USER_HASH_PAIR& rNode, m_UserHashTable, USER_HASH)
	{
		User* const pUser = rNode.second;
		FreeUser(pUser);
	}

	m_UserHashTable.clear();
	m_pObjectKeyStream->Clear();
	m_CharGuidUserHash.clear();

	m_UserAuthIDs.clear();
	m_CharNameHashTable.clear();
}

struct sDUPLICATED_CHECK
{
	static const DWORD UNKNOWN_FAIL			= 0;
	static const DWORD SUCCESS				= 1;
	// AddUser시
	static const DWORD DUPLICATED_USERKEY	= 2;
	static const DWORD INVALID_OBJKEY		= 3;
	static const DWORD DUPLICATED_AUTHID	= 4;
	// RemoveUser시
	static const DWORD CANT_FIND_USER		= 5;
};

BOOL
UserManager::AddUser(User* pUserSession)
{
    const DWORD userKey	  = pUserSession->GetUserGUID();
    const DWORD objectKey = pUserSession->GetPlayerKey();
    const DWORD authID    = pUserSession->GetAuthID();

	//////////////////////////////////////////////////////////////////////////
	//	<LOGIC CONTROL>
	FlowControl flow;
	FLOWCONTROL_START()
    {
		IfFailThenBreak(userKey && objectKey && authID, sDUPLICATED_CHECK::UNKNOWN_FAIL);

        USER_HASH_IT itUser = m_UserHashTable.find(userKey);
        IfFailThenBreak(itUser == m_UserHashTable.end(), sDUPLICATED_CHECK::DUPLICATED_USERKEY);
        USER_HASH_IT itAuth = m_UserAuthIDs.find(authID);
        IfFailThenBreak(itAuth == m_UserAuthIDs.end(), sDUPLICATED_CHECK::DUPLICATED_AUTHID);

		m_UserHashTable[userKey] = pUserSession;
		m_UserAuthIDs[authID] = pUserSession;
		m_pObjectKeyStream->AddObjectInfo(objectKey, pUserSession, PlayerObjectKeyStreamAddin());

#ifdef _JP_0_20101123_PMS
        AgentServer* agent_server = AgentServer::GetInstance();
        agent_server->SetTotalNumberOfConnections();
        agent_server->GetPMSManager()->SetUserCount(GetNumberOfUsers());
#endif
		
		return TRUE;
    }
    FLOWCONTROL_END;
	
	//////////////////////////////////////////////////////////////////////////
	//	<ERROR CONTROL>
	MessageOut(
		eCRITICAL_LOG, _T("ASSERTION SESSION CONTROL VIOLATION : {KEY,OBJ,AUTH} = {%u:%u:%u} : Reason [%u][%s]")
		, userKey, objectKey, authID
		, flow.GetErrorCode()
		, flow.GetErrorString()
		);
	
	return FALSE;
}

VOID
UserManager::RemoveUser(DWORD dwUserKey)
{
	DWORD eCHECK = sDUPLICATED_CHECK::SUCCESS;
	User* pUser = NULL;

	if(sDUPLICATED_CHECK::SUCCESS == eCHECK)
	{	// UserGUID Control
		USER_HASH_IT itFind = m_UserHashTable.find(dwUserKey);
		if(itFind == m_UserHashTable.end())
			eCHECK = sDUPLICATED_CHECK::CANT_FIND_USER;
		else
			pUser = itFind->second;
	}

	if(sDUPLICATED_CHECK::SUCCESS == eCHECK)
	{
		DWORD dwObjectKey = pUser->GetPlayerKey();
		DWORD dwAuthID = pUser->GetAuthID();

		m_UserHashTable.erase(dwUserKey);
		m_pObjectKeyStream->RemoveObjectInfo(dwObjectKey);
		m_UserAuthIDs.erase(dwAuthID);

#ifdef _JP_0_20101123_PMS
        AgentServer* agent_server = AgentServer::GetInstance();
        agent_server->SetTotalNumberOfConnections();
        agent_server->GetPMSManager()->SetUserCount(GetNumberOfUsers());
#endif

		return;
	}

	// Error Control
	MessageOut(eFULL_LOG, _T("[UserManager::RemoveUser] UserSession[%d] is NULL!!"), dwUserKey);
}

VOID
UserManager::DisconnectLinkedServerUser(eSERVER_TYPE Type)
{
	User* pUser = NULL;
	USER_HASH_IT it;
	for(it = m_UserHashTable.begin(); it != m_UserHashTable.end(); ++it)
	{
		pUser = it->second;
		if(pUser)
		{
			ServerSessionEx* pServer = pUser->GetLinkedServer();
			if(!pServer)
			{
				continue;
			}

			if(pServer->GetServerType() == Type)
				pUser->DisconnectLinkedServerUser();
		}
	}
}

// CHANGES: f110516.3L, changes to the pair information insertion on select character
bool UserManager::AddUserForSelectedCharInfoHashes(User* const user)
{
    const DWORD user_key = user->GetUserGUID();
    const DWORD object_key = user->GetPlayerKey();
    const DWORD char_guid = user->GetSelectedCharGuid();
    const char* char_name = user->GetSelectedCharName();
    const DWORD char_name_hash = user->GetSelectedCharNameHash();

    if (FlowControl::FCAssert(char_guid != 0 && char_name_hash != 0) == false)
    {
        SUNLOG(eCRITICAL_LOG,
               _T("["__FUNCTION__"][FAIL] UID:CID:OBJ = [%u:%u:%u] Name[%s]"),
               user_key, char_guid, object_key, char_name);
        return false;
    };

    const ObjectKeyStream::sNode* const object_node = m_pObjectKeyStream->GetObjectInfo(object_key);
    bool updated = (object_node != NULL) &&
        m_pObjectKeyStream->UpdateObjectAddinInfo(object_key, PlayerObjectKeyStreamAddin(char_guid));
    if (FlowControl::FCAssert(updated != false) == false)
    {
        SUNLOG(eCRITICAL_LOG,
               _T("["__FUNCTION__"][FAIL] UID:CID:OBJ = [%u:%u:%u] Name[%s], exist key"),
               user_key, char_guid, object_key, char_name);
        return false;
    };
    //
    m_CharGuidUserHash[char_guid] = object_key;
    //
    //__NA003322_20090217_NOT_FIND_ENGLISHNAME_BUG
#ifdef _DEBUG
    char lower_char_name[MAX_CHARNAME_LENGTH + 1];
    strncpy(lower_char_name, char_name, _countof(lower_char_name));
    lower_char_name[_countof(lower_char_name) - 1] = '\0';
    _strlwr(lower_char_name);
    util::HashKey hash_value = util::StreamHashKey::GetStringHashKey(lower_char_name);
    ASSERT(char_name_hash == hash_value);
#endif
    m_CharNameHashTable[char_name_hash] = object_key;
    //
    return true;
};

void UserManager::RemoveUserForSelectedCharInfoHashes(User* const user)
{
    const DWORD user_key = user->GetUserGUID();
    const DWORD object_key = user->GetPlayerKey();
    const DWORD char_guid = user->GetSelectedCharGuid();
    const char* char_name = user->GetSelectedCharName();

    if (FlowControl::FCAssert(char_guid != 0) == false)
    {
        SUNLOG(eCRITICAL_LOG,
               _T("["__FUNCTION__"][FAIL] UID:CID:OBJ = [%u:%u:%u] Name[%s]"),
               user_key, char_guid, object_key, char_name);
        return;
    };

    const ObjectKeyStream::sNode* const object_node = m_pObjectKeyStream->GetObjectInfo(object_key);
    bool updated = (object_node != NULL) &&
        m_pObjectKeyStream->UpdateObjectAddinInfo(object_key, PlayerObjectKeyStreamAddin());
    if (FlowControl::FCAssert(updated != false) == false)
    {
        SUNLOG(eCRITICAL_LOG,
               _T("["__FUNCTION__"][FAIL] UID:CID:OBJ = [%u:%u:%u] Name[%s], exist key"),
               user_key, char_guid, object_key, char_name);
        return;
    };
    //
    m_CharGuidUserHash.erase(char_guid);
    //
    m_CharNameHashTable.erase(user->GetSelectedCharNameHash());
};

User*
UserManager::FindUserForCharName(const TCHAR* char_name)
{
    // CHANGES: f110517.3L, changes hash finding accessibility about a character name
    TCHAR lower_char_name[MAX_CHARNAME_LENGTH + 1];
    _tcsncpy(lower_char_name, char_name, _countof(lower_char_name));
    lower_char_name[_countof(lower_char_name) - 1] = _T('\0');
    _tcslwr(lower_char_name);
    //
    util::HashKey hash_value = util::StreamHashKey::GetStringHashKey(lower_char_name);
    CHARNAME_HASH::const_iterator found_it = m_CharNameHashTable.find(hash_value);
    if (found_it == m_CharNameHashTable.end()) {
        return NULL;
    };
    //
    const DWORD object_key = found_it->second;
    const ObjectKeyStream::sNode* object_node = m_pObjectKeyStream->GetObjectInfo(object_key);
    if (object_node == NULL) {
        return NULL;
    };

    return object_node->pObject;
    //
#if SUN_CODE_BACKUP
#ifdef __NA003322_20090217_NOT_FIND_ENGLISHNAME_BUG
   static char szCharName[MAX_CHARNAME_LENGTH + 1];
    strncpy(szCharName, char_name, _countof(szCharName));
    szCharName[_countof(szCharName) - 1] = '\0';
    _strlwr(szCharName); //성공이든 실패든 입력하기로 한다.

    // find의 경우는 []연산을 수행하지 않는 것으로 한다.
    User* pUser = 0;
    M_USER_LIST::iterator it = m_CharNameHashTable.find(szCharName);
    if(it != m_CharNameHashTable.end()) {
        UserDelegator& rDelegator = it->second;
        pUser = rDelegator.USER();
    }
    return pUser;
#else
	UserDelegator& rDelegator = m_CharNameHashTable[char_name];
    User* pUser = rDelegator.USER();

    return pUser;
#endif
#endif //SUN_CODE_BACKUP
}

//
// <UserFactory>
UserSession*
UserManager::AllocUser(eUSER_TYPE utTYPE)
{
	ObjectPoolRouter* pRouter = ObjectPoolRouter::Instance();
	UserSession* pUser = (UserSession*)pRouter->Alloc((eAGENT_OBJECT_INDEX)utTYPE);

	if(pUser)
	{
		pUser->Init();

		if(UT_USER == utTYPE)
		{	// 추가 초기화 값
			DWORD objectKey = ObjKeyGender::Instance()->GetKey(OBJKEYTYPE_PLAYERKEY);
			ASSERT(objectKey != 0);
			((User*)pUser)->SetPlayerKey(objectKey);
		}
	}
	return pUser;
}

VOID
UserManager::FreeUser(UserSession* pUser)
{
	eUSER_TYPE utTYPE = pUser->GetUserType();

	if(UT_USER == utTYPE)
	{
		const DWORD objectKey = ((User*)pUser)->GetPlayerKey();
		ObjKeyGender::Instance()->RestoreKey(OBJKEYTYPE_PLAYERKEY, objectKey);
	}

	pUser->Release();

	ObjectPoolRouter* pRouter = ObjectPoolRouter::Instance();
	if(!pRouter->Free((eAGENT_OBJECT_INDEX)utTYPE, pUser))
	{
		SUNLOG(eCRITICAL_LOG, _T("잘못된 타입으로 객체 해제 시도 (FreeUser)"));
	}
}


VOID
UserManager::GarbageCollector()
{
#if SUN_CODE_BACKUP
    {   // delete unused char name lists
        M_USER_LIST_IT it = m_CharNameHashTable.begin(),
                       end = m_CharNameHashTable.end();
        while(it != end)
        {
            UserDelegator& rDelegator = it->second;
            if(rDelegator.USER() != 0)
                ++it;
            else
                it = m_CharNameHashTable.erase(it);
        }
    };
#endif //SUN_CODE_BACKUP
}

VOID
UserManager::TempUserClosureTimeout()
{
	TEMPUSER_LIST& tUserList = m_TempUserList4Timeout;

	TEMPUSER_LIST_IT it = tUserList.begin(),
                     end = tUserList.end();
	while(it != end)
	{
		TempUser* pTempUser = *it;
		ITimeout& rTIMEOUT = pTempUser->NoActionTimeout();
		if(rTIMEOUT.IsDisabled())
		{
			// User가 되었거나 종료가 된 케이스
			it = tUserList.erase(it);
			continue;
		}

		if(rTIMEOUT.IsExpired())
		{
			// No Action Timeout, 이 경우는 세션을 끊어야 한다.
			// 애누리없게 무차별 종료시키자.
			pTempUser->Disconnect(FALSE);
			++it;
			continue;
		}

		++it;
	}
}

//==================================================================================================
// CHANGES: f110516.3L, unity function for all user notification
// CHANGES: f110609.4L, changes the method action that include the packing
// whether the message need packing
void UserManager::SendMessageToAllUsers(MSG_BASE* msg, uint16_t msg_size)
{
    if (bool need_packing = (msg_size > 120))
    {
        nsPacket::PacketInfo pack_info = nsPacket::PacketControl::Pack(msg, msg_size);
        msg = pack_info.pMsg;
        msg_size = pack_info.Size;
    };
    USER_HASH::const_iterator it = m_UserHashTable.begin(),
                              end = m_UserHashTable.end();
    for (; it != end; ++it)
    {
        User* user = it->second;
        user->SendPacket(msg, msg_size, false);
    };
}

//==================================================================================================
//==================================================================================================
//==================================================================================================

//#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
namespace ns_functor {

struct MultiFunction : public ns_functor::IUserForeachFunctor
{
    MultiFunction(BOOLEAN bCCUControl, BOOLEAN bServerSelectProcess)
        : m_bCCUControl(bCCUControl)
        , m_bServerSelectProcess(bServerSelectProcess)
    {
    }

    virtual bool operator()(User* const user)
    {
        if (m_bCCUControl) {
            m_CCUControl.Calculation(user);
        };
    #ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
        if (m_bServerSelectProcess) {
            user->OnServerSelectQueryUpdate();
        };
    #endif
        return true;
    }

    BOOLEAN m_bCCUControl, m_bServerSelectProcess;
    CCUControl m_CCUControl;
};

};
//#endif

VOID
UserManager::Process(BOOLEAN bCCUControl, BOOLEAN bServerSelectProcess,
                     MSG_AM_CONNECTION_CCU_REPORT_NTF* OUT pINFO)
{
    ns_functor::MultiFunction multifunction(bCCUControl, bServerSelectProcess);
    this->Foreach(&multifunction);
    if (bCCUControl) {
        multifunction.m_CCUControl.GetCurrentCCU(pINFO);
    };
}

