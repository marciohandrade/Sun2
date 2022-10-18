#pragma once
#ifndef SERVER_GAMESERVER_STATUEMANAGER_H
#define SERVER_GAMESERVER_STATUEMANAGER_H

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM

#include <map>

class StatueManager : public util::Singleton<StatueManager>
{
public:
    StatueManager();;
    ~StatueManager();;

    void SetStatue(eSTATUE_TYPE type, eSTATUE_INDEX index, StatueInfo* info);
    StatueInfo* GetStatue(eSTATUE_TYPE type, eSTATUE_INDEX index);

private:
    typedef std::pair<eSTATUE_TYPE, eSTATUE_INDEX> StatueKey;
    typedef std::map<StatueKey, StatueInfo> Statues;
    Statues m_statues;
};

#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM

#endif // SERVER_GAMESERVER_STATUEMANAGER_H