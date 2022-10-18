#pragma once

#include <MemoryPoolFactory.h>
#include <Singleton.h>

class BaseZone;
class ChatZone;
class ViewPortZone;
class VillageZone;

class ChatZoneFactory : public util::Singleton<ChatZoneFactory>
{
public:
	ChatZoneFactory();
	~ChatZoneFactory();

	VOID							Init();
	VOID							Release();
	ChatZone*						AllocZone();
	VOID							FreeZone( ChatZone *pZone );
private:
	util::CMemoryPoolFactory<ChatZone>	*m_pZonePool;
};

class ViewPortZoneFactory : public util::Singleton<ViewPortZoneFactory>
{
public:
	ViewPortZoneFactory();
	~ViewPortZoneFactory();

	VOID							Init();
	VOID							Release();
	ViewPortZone*					AllocZone();
	VOID							FreeZone( ViewPortZone *pZone );
private:
	util::CMemoryPoolFactory<ViewPortZone>	*m_pZonePool;
};

class VillageZoneFactory : public util::Singleton<VillageZoneFactory>
{
public:
	VillageZoneFactory();
	~VillageZoneFactory();

	VOID							Init();
	VOID							Release();
	VillageZone*					AllocZone();
	VOID							FreeZone( VillageZone *pZone );
private:
	util::CMemoryPoolFactory<VillageZone>	*m_pZonePool;
};
