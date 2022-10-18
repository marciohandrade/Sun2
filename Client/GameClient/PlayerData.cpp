#include "SunClientPrecompiledHeader.h"
#include "PlayerData.h"


cPlayerData::cPlayerData()
{

}

cPlayerData::~cPlayerData()
{

}

void cPlayerData::Initialize()
{
    m_pPlayerInfo = new BASE_PLAYERINFO;
}

void cPlayerData::Release()
{
    delete m_pPlayerInfo;
}
