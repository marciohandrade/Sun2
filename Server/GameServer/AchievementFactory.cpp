#include "StdAfx.h"
#include ".\areaconquestfactory.h"
#include ".\AreaConquestSection.h"

AchievementFactory::AchievementFactory(void)
{
}

AchievementFactory::~AchievementFactory(void)
{
}
void AchievementFactory::Init( DWORD dwPoolSize )
{
	m_pACSectionPool.Initialize( dwPoolSize, 0, "AreaConquestSection", AssertionLogger4Pool_Server );
	AreaConquestSection::SetPool( &m_pACSectionPool );
}

void AchievementFactory::Release()
{
	;
}