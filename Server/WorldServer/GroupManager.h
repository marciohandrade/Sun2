// implemented by __KR_001355_20090608_GROUPINGSYSTEM_DATA
#pragma once

#include <Singleton.h>
#include <vector>
#include <MemoryPoolFactory.h>

class User;

struct STRUCT_GROUP_MEMBER_INFO;

typedef STLX_MAP<DWORD, STRUCT_GROUP_MEMBER_INFO*>	 GROUP_MEMBER_MAP;
typedef GROUP_MEMBER_MAP::iterator					 GROUP_MEMBER_MAP_ITER;
typedef std::pair< DWORD, STRUCT_GROUP_MEMBER_INFO*> GROUP_MEMBER_MAP_PAIR;

class GroupManager : public  util::Singleton<GroupManager, util::SingletonRelease::kAuto>
{
public:
    enum eSTAT_ALAM
    {
        LOG_OUT     = 1,
        LOG_IN,
        GET_MONEY,
        GET_EXP,
        MEMBER_LEVELUP,
    };

    GroupManager();
    ~GroupManager();

    STRUCT_GROUP_MEMBER_INFO* FindGroupMember(User* pUser, DWORD dwCharGuid);
    STRUCT_GROUP_MEMBER_INFO* FindGroupMember(User* pUser, TCHAR* ptszMemberName);
    STRUCT_GROUP_MEMBER_INFO* InsertMemberInfo(User* pUser, STRUCT_GROUP_MEMBER_INFO* pMember, 
                                               eGROUP_STAT eMemberStat);

    eGROUP_STAT GetGroupHighMember(User* pUser);
    eGROUP_STAT GetGroupLowMember(User* pUser);

    //- 현재 그룹 상하위 멤버 상태를 알린다.
    void UpdateGroupInfo(User* pUser);
    //- 상위 멤버 상태를 Update.
    void SetGroupHighMember(User* pUser, eGROUP_STAT eHightMember);
    //- 멤버를 삭제 한다.
    void RealeseMember(User *pUser);
    //- 최초 멤버 정보를 셋팅한다.
    bool SetMemberInfo(User* pUser, const STRUCT_GROUP_MEMBER_INFO& Member);
    //- 그룹 멤버에게 정보를 알린다.
    void SendAroundMember(User* pUser, eSTAT_ALAM memberStat);
    //- 그룹 멤버를 추가 한다.
    void AddMemberInfo(User* pUser, const STRUCT_GROUP_MEMBER_INFO& pMember);
    //- 그룹 멤버를 삭제 한다.
    void RemoveMemberInfo(User* pUser, DWORD charGuid);
    //- 접속할때 하위 멤버가 쌓아놓은 경험치를 반영하기 위해 쪽지를 만든다.
    void SendAccumEXPMemo(User* pUser);
    //- 그룹내 하위 멤버의 인원 수를 구한다.
    int GetLowMemberCount(User* pUser);
    //- 현제 시간과 인자 값간의 날짜 차이와 결과를 알아낸다.
    bool IsOverBonusDate(DWORD64 lastDate, INT chDate);
    //- 그룹 리스트를 보낸다(gameluluWorking)
    void SendGroupList(User* pUser);
    //- 멤버 추가시에 추가 멤버에 대한 정보를 유저에게 알려준다.
    void SendGroupMemberInfo(User* pUser, const STRUCT_GROUP_MEMBER_INFO& stGroupMemberInfo, 
                             eGROUP_STAT stat);
    //- 맴버 리스트를 보낸다.
    void SendMemberList(User* pUser);
    //- 맴버 추가가 가능한가?
    bool CanAddGroupMember(User* request_user, User* dest_user, MSG_BASE* msg);

private:
    STRUCT_GROUP_MEMBER_INFO* AllocMemberInfo();
    void SetGroupLowMember(User* pUser, eGROUP_STAT eLowMember);
    void SendGroupInfoSet(User* pUser, eGROUP_STAT eHighMember, eGROUP_STAT eLowMember);
    bool SetMemeberConnectStat(User* pUser, DWORD dwCharGuid, BOOL bConnectStat);
    bool SendAccumEXP(DWORD userGuid, CHARGUID highGuid, EXPTYPE AccumEXP);
    void NotifyLoginState(User* pUser);
    void NotifyLogOutState(User* pUser);
    void NotifyEXPState(User* pUser);
    void NotifyMoneyState(User* pUser);
    void NotifyLevelUpState(User* pUser);

private:
    util::CMemoryPoolFactory<STRUCT_GROUP_MEMBER_INFO>	*m_pGroupPool;

};
