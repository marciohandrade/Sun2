#include "stdafx.h"

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM

#include "StatueManager.h"
#include "SCItemSlotContainer.h"
#include <SCItemSlot.h>
#include "ItemScoreParser.h"

StatueManager::StatueManager()
{

}

StatueManager::~StatueManager()
{

}

void StatueManager::SetStatue( eSTATUE_TYPE type, eSTATUE_INDEX index, StatueInfo* info )
{
    StatueKey key = std::make_pair(type, index);
    Statues::iterator cur_iter = m_statues.find(key);
    if (cur_iter != m_statues.end())
    {
        m_statues.erase(cur_iter);
    }
    m_statues.insert(std::make_pair(key, *info));
}

StatueInfo* StatueManager::GetStatue( eSTATUE_TYPE type, eSTATUE_INDEX index )
{
    StatueKey key = std::make_pair(type, index);
    Statues::iterator cur_iter = m_statues.find(key);
    if (cur_iter != m_statues.end())
    {
        return &cur_iter->second;
    }
    return NULL;
}
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM