//----------------------------------------------------------------------------
/**
@desc : BGM을 출력하기 위한 스크립트
@author : 유재영 (y2jinc@webzen.co.kr)
@remark :
*/
#pragma once

#include <SolarHashTable.h>
#include <Singleton.h>
#include <SolarFileReader.h>

#define MAX_BGMDESC_LEN		256

#define MAP_TYPE_DEFAULT	1
#define MAP_TYPE_EVENT		2


struct BASE_BGMSoundInfo 
{

     WORD     m_wNum;
     char     m_szDesc[MAX_BGMDESC_LEN];
     WORD     m_wType;
     WORD     m_wFieldCode;
     DWORD    m_RoomType;
     WORD     m_wBGMNum;

};


class BGMSoundInfoParser	: public Singleton<BGMSoundInfoParser>, public IParser
{
public:
   BGMSoundInfoParser();
   ~BGMSoundInfoParser();


   VOID                 Init(DWORD dwPoolSize);
   VOID			        Init(DWORD dwPoolSize, char * pszPackFileName);
   VOID                 Release();
  
   
private:
	DWORD			    MakeKey(int iType, WORD FieldCode, eZONETYPE eRoomType);
	VOID			    Unload();
	util::SolarHashTable<BASE_BGMSoundInfo *>*  m_pDataTable;
	BOOL			    _Load(BOOL bReload);

public:
	virtual	BOOL	    LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);


	BASE_BGMSoundInfo*	GetBGMInfo(int iType, WORD FieldCode, eZONETYPE eRoomType);
};
