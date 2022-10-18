#include "stdafx.h"
#include "ReAuthManager.h"
#include "ReAuthInfo.h"
#include <ObjKeyGenerator.h>

#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__

ReAuthManager::ReAuthManager()
{
    m_pReAuthKeyGenerator = new ReAuthKeyGenerator(ReAuthKeyStart, ReAuthKeyEnd - 1);
    ZeroMemory(m_ReAuthInfos, sizeof(m_ReAuthInfos));
    m_UpdateTimer.SetTimer(20 * 1000);
}

ReAuthManager::~ReAuthManager()
{
    ReAuthInfo** pIt = m_ReAuthInfos;
    ReAuthInfo** pEnd = &m_ReAuthInfos[ReAuthKeyRange];
    for( ; pIt != pEnd; ++pIt)
    {
        ReAuthInfo* info = *pIt;
        if (info) {
            TAllocDelete(ReAuthInfo, info);
        };
    }
    ZeroMemory(m_ReAuthInfos, sizeof(m_ReAuthInfos));
    SAFE_DELETE(m_pReAuthKeyGenerator);
}

const ReAuthInfo*
ReAuthManager::AllocReAuthInfo(DWORD userGuid, DWORD ipAddress,
							   const char account_id[ID_MAX_LEN + 1],
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
							   GAMECHUUSN gamechu_usn,
#endif
							   AccountInfo* account_info/*for global*/)
{
	ReAuthKeyType allocKey = m_pReAuthKeyGenerator->GetKey();
	if(allocKey == 0)
		return 0;

	ReAuthInfo* info = TAllocNew(ReAuthInfo);
	info->Clear();
	info->m_ReAuthKey = allocKey;
	info->m_UserGuid = userGuid;
	info->m_IpAddress = ipAddress;
	info->m_Timeout.SetNextTimeoutByCurTickBased(2 * 60 * 1000);
	strncpy(info->m_AccountID, account_id, _countof(info->m_AccountID));
	info->m_AccountID[_countof(info->m_AccountID) - 1] = '\0';

#ifdef __NA000000_GLOBAL_AUTH_PROCESS__
	assert(account_info != 0);
	info->m_AccountInfo = *account_info;
#endif
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
	info->gamechu_usn_ = gamechu_usn;
#endif

	ReAuthKeyType offset = allocKey - ReAuthKeyStart;
	m_ReAuthInfos[offset] = info;

	if(AuthStepAuthToken::MakeReAuthToken(userGuid, allocKey, &info->m_ReAuthTokenInfo))
		return info;

	FreeReAuthInfo(info);
	return 0;
}

BOOLEAN
ReAuthManager::FreeReAuthInfo(ReAuthInfo* pReAuthInfo)
{
    ReAuthKeyType key = pReAuthInfo->m_ReAuthKey;
    BOOLEAN bInRange = (ReAuthKeyStart <= key &&
                                          key < ReAuthKeyEnd);
    if(bInRange == false)
        return false;

    ReAuthKeyType offset = key - ReAuthKeyStart;
    if(FlowControl::FCAssert(m_ReAuthInfos[offset] == pReAuthInfo) == 0)
        return false;

    TAllocDelete(ReAuthInfo, pReAuthInfo);
    m_pReAuthKeyGenerator->RestoreKey(key);

    m_ReAuthInfos[offset] = 0;
    return true;
}

void
ReAuthManager::Randomize()
{
    ReAuthKeyType size = ReAuthKeyType(m_pReAuthKeyGenerator->GetSize());
    ASSERT(size == ReAuthKeyRange);
    typedef STLX_VECTOR<ReAuthKeyType> key_randomizer;
    key_randomizer keys;
    keys.reserve(ReAuthKeyRange);
    for(ReAuthKeyType loop = size; loop; --loop) {
        ReAuthKeyType key = m_pReAuthKeyGenerator->GetKey();
        keys.push_back(key);
    }
    random_shuffle(keys.begin(), keys.end());
    FOREACH_CONTAINER(ReAuthKeyType key, keys, key_randomizer)
    {
        m_pReAuthKeyGenerator->RestoreKey(key);
    }
}

ReAuthManager::ReAuthKeyType
ReAuthManager::NumberOfFreeReAuthKey() const
{
    return ReAuthKeyType(m_pReAuthKeyGenerator->GetSize());
}

void
ReAuthManager::Update()
{
    if(m_UpdateTimer.IsExpired() == 0)
        return;

    ReAuthInfo** pIt = m_ReAuthInfos;
    ReAuthInfo** pEnd = &m_ReAuthInfos[ReAuthKeyRange];
    for( ; pIt != pEnd; ++pIt)
    {
        ReAuthInfo* info = *pIt;
        if(info == 0)
            continue;

        if(info->IsExpired())
            FreeReAuthInfo(info);
    }
}

BOOLEAN
ReAuthManager::OnAcceptReAuthClient(const ReAuthInfo* pReAuthInfoC)
{
    ReAuthInfo* pReAuthInfo = const_cast<ReAuthInfo*>(pReAuthInfoC);
    pReAuthInfo->ChangeStep(pReAuthInfo->eReAuthSlot_ClientSide, pReAuthInfo->eReAuthStep_Accepted);
    //pReAuthInfo->m_Timeout.Disabled(); // by QueryManager->QueryAuth->on_result
    // 10' timeout설정한다. 그동안 재접이 안된다면 심하다...
    return true;
}

BOOLEAN
ReAuthManager::OnAcceptQueryLoginUser(const ReAuthInfo* pReAuthInfoC) // T : free pReAuthInfo
{   //by QueryManager->QueryAuth->on_result
    ReAuthInfo* pReAuthInfo = const_cast<ReAuthInfo*>(pReAuthInfoC);
    return FreeReAuthInfo(pReAuthInfo);
}

BOOLEAN
ReAuthManager::OnCancelByGameServer(const ReAuthInfo* pReAuthInfoC)
{
    ReAuthInfo* pReAuthInfo = const_cast<ReAuthInfo*>(pReAuthInfoC);
    return FreeReAuthInfo(pReAuthInfo);
}

#endif //__NA001334_090420_SERVER_SELECT_FACILITY__


