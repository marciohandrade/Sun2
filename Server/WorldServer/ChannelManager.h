#pragma once

#include <Singleton.h>
#include <map>
#include <MemoryPoolFactory.h>
#include <SolarFileReader.h>

// 통계 간격
#include <Timer.h>

// 연결 서버 정보
#include <ServerStruct.h>

class Channel;
class User;
class FieldInfo;

// 월드서버 외치기용 연결 존 정보 (references: 'shoutrange.txt')
struct SERVER_LINKEDZONE_INFO
{
    DWORD dwMapCode; // 맵코드
    BYTE  byMKind; // MKind
    DWORD dwLinkedMapCode[7]; // LField1~7
    char  szMapName[MAX_MAPNAME_LENGTH + 1];    // 맵이름
};

typedef STLX_MAP<DWORD, Channel*>		CHANNEL_MAP;
typedef CHANNEL_MAP::iterator			CHANNEL_MAP_ITER;
typedef std::pair<DWORD, Channel*>		CHANNEL_MAP_PAIR;

typedef STLX_MAP<DWORD, SERVER_LINKEDZONE_INFO>		LINKEDZONE_MAP;
typedef LINKEDZONE_MAP::iterator					LINKEDZONE_MAP_ITER;
typedef std::pair<DWORD, SERVER_LINKEDZONE_INFO>	LINKEDZONE_MAP_PAIR;

typedef STLX_MAP<DWORD, FieldInfo*>			FIELDINFO_MAP;
typedef FIELDINFO_MAP::iterator			FIELDINFO_MAP_ITER;
typedef std::pair<DWORD, FieldInfo*>	FIELDINFO_MAP_PAIR;

//==================================================================================================

class ChannelManager : public util::Singleton<ChannelManager, util::SingletonRelease::kAuto>, public IParser
{
public:
	ChannelManager(void);
	~ChannelManager(void);

	VOID							Init();
	VOID							Release();
	VOID							Update();

	Channel*						AllocChannel();
	VOID							FreeChannel( Channel *pChannel );
	VOID							AddChannel( DWORD dwChannelID );
	VOID							RemoveChannel( DWORD dwChannelID );
	Channel*						GetChannel( DWORD dwChannelID );

//	VOID							SendToAllChannel( BYTE *pMsg, WORD wSize );
	VOID							SendToVillages( BYTE *pMsg, WORD wSize );
	VOID							SendToBattleZones( BYTE *pMsg, WORD wSize );
	VOID							DisplayChannelInfo();
	CHANNEL_MAP&					GetChannelMap() { return m_mapChannels; }
	BOOL							LoadFieldInfo();

	FieldInfo*						GetFieldInfo( DWORD dwFieldCode );
	inline BOOL						FindLinkedZoneInfo( DWORD dwMapCode, SERVER_LINKEDZONE_INFO &LinkedZone )
	{
		LINKEDZONE_MAP_ITER		iter = m_mapLinkedZoneInfo.find( dwMapCode );

		if( iter == m_mapLinkedZoneInfo.end() )
		{
			return FALSE;
		}

		LinkedZone =  (*iter).second;

		return TRUE;
	}

public:
	virtual	BOOL			LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload );
			BOOL			_LoadLinkedZoneFile( BOOL bReload );

private:
	CHANNEL_MAP						m_mapChannels;
	util::CMemoryPoolFactory<Channel>*	m_pChannelPool;
	util::Timer						m_StatisticsTimer;	// 통계용 타이머

	LINKEDZONE_MAP					m_mapLinkedZoneInfo;
	FIELDINFO_MAP					m_mapFieldInfo;
};
