#pragma once

#ifdef _NA_008350_20150617_SPA_NEW_STYLE_MODE_ANIMATION

#include <SolarHashTable.h>
#include <Singleton.h>

// style_type 1 일 때 
// special_animation[4]   0:앉는중, 1:앉아있기, 2:앉기아이들, 3:일어나기
// style_type 2 일 때 
// special_animation[4]   0:앉기 단계별 사라지고 해당 애니메이션 반복
struct SpaZoneInfo
{
	BYTE        index;
    DWORD       map_code;
    BYTE        style_type;             // 특수 애니메이션 스타일
    tagWZID     special_animation[4];   // 특수 애니메이션 코드  0:앉는중, 1:앉아있기, 2:앉기아이들, 3:일어나기
    DWORD       base_costume_code;      // 기본 입장권 아이템 (초기값 복장)
};


class SpaZoneInfoParser	: public Singleton<SpaZoneInfoParser> ,public IParser
{
public:
    enum SpaAnimationStyle            // 특수 애니메이션 스타일
    {
        AnimationStyle_None = 0,

        AnimationStyle_SitIdle = 1,   // 앉기IDLE 을 변경 - 온천스타일
        AnimationStyle_Sit,           // 앉기 자체를 변경

        AnimationStyle_End,
    };

	SpaZoneInfoParser();
	~SpaZoneInfoParser();


	VOID                    Init(DWORD dwPoolSize , char *szPackfileName);
	VOID                    Init(DWORD dwPoolSize);

	VOID                    Release();
	virtual	BOOL            LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);


	SpaZoneInfo*    GetSpaZoneInfo(DWORD map_code);
    SpaAnimationStyle GetSpaAnimationStyle(DWORD map_code);
private:
	VOID		            Unload();
	util::SolarHashTable<SpaZoneInfo*>*     m_pDataTable;
};

#endif //_NA_008350_20150617_SPA_NEW_STYLE_MODE_ANIMATION