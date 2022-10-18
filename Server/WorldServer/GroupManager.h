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

    //- ���� �׷� ������ ��� ���¸� �˸���.
    void UpdateGroupInfo(User* pUser);
    //- ���� ��� ���¸� Update.
    void SetGroupHighMember(User* pUser, eGROUP_STAT eHightMember);
    //- ����� ���� �Ѵ�.
    void RealeseMember(User *pUser);
    //- ���� ��� ������ �����Ѵ�.
    bool SetMemberInfo(User* pUser, const STRUCT_GROUP_MEMBER_INFO& Member);
    //- �׷� ������� ������ �˸���.
    void SendAroundMember(User* pUser, eSTAT_ALAM memberStat);
    //- �׷� ����� �߰� �Ѵ�.
    void AddMemberInfo(User* pUser, const STRUCT_GROUP_MEMBER_INFO& pMember);
    //- �׷� ����� ���� �Ѵ�.
    void RemoveMemberInfo(User* pUser, DWORD charGuid);
    //- �����Ҷ� ���� ����� �׾Ƴ��� ����ġ�� �ݿ��ϱ� ���� ������ �����.
    void SendAccumEXPMemo(User* pUser);
    //- �׷쳻 ���� ����� �ο� ���� ���Ѵ�.
    int GetLowMemberCount(User* pUser);
    //- ���� �ð��� ���� ������ ��¥ ���̿� ����� �˾Ƴ���.
    bool IsOverBonusDate(DWORD64 lastDate, INT chDate);
    //- �׷� ����Ʈ�� ������(gameluluWorking)
    void SendGroupList(User* pUser);
    //- ��� �߰��ÿ� �߰� ����� ���� ������ �������� �˷��ش�.
    void SendGroupMemberInfo(User* pUser, const STRUCT_GROUP_MEMBER_INFO& stGroupMemberInfo, 
                             eGROUP_STAT stat);
    //- �ɹ� ����Ʈ�� ������.
    void SendMemberList(User* pUser);
    //- �ɹ� �߰��� �����Ѱ�?
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
