#pragma once

//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式

#include <Linkedlist.h>
#include <SolarHashTable.h>

//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式

class MapInfo;
class FieldInfo;
class SectorInfo;
class MAPOBJECT_INFO;
class SharedFieldInfo;
//struct sMAPINFO;
//struct sFIELDINFO;

//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式

typedef util::SolarHashTable<MapInfo *>						MAPINFO_HASH;
typedef util::SolarHashTable<MapInfo *>::iterator			MAPINFO_HASH_IT;
typedef util::SolarHashTable<SharedFieldInfo *>				SHAREDFIELDINFO_HASH;
typedef util::SolarHashTable<SharedFieldInfo *>::iterator	SHAREDFIELDINFO_HASH_IT;

typedef util::SolarHashTable<sFIELDINFO *>					sFIELDINFO_HASH;
typedef util::SolarHashTable<sFIELDINFO *>::iterator		sFIELDINFO_HASH_IT;

//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式

