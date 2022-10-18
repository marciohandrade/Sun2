#pragma once

#include <Singleton.h>
#include <MemoryPoolFactory.h>

#include "AreaConquestSection.h"

class AchievementFactory : public util::Singleton<AchievementFactory>
{
public:
	AreaConquestFactory(void);
	~AreaConquestFactory(void);

	void Init( DWORD dwPoolSize );
	void Release();

private:
	util::CMemoryPoolFactory<AreaConquestSection> m_pACSectionPool;
};
