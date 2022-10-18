#pragma once

#include <SolarHashTable.h>
#include <Singleton.h>


struct BASE_HeadMeshInfo {

	DWORD     m_dwID;
	DWORD     m_dwIcon;

};

enum
{
    HEAD_MESH_NONE = 0,
    HEAD_MESH_PICK_GROUND,
    HEAD_MESH_EXCLAMATION_MARK,
    HEAD_MESH_PICK_CHARACTER,
    HEAD_MESH_QUESTION_MARK,
    HEAD_MESH_SCROLL_DEACTIVE,
    HEAD_MESH_SCROLL_ACTIVE,
    HEAD_MESH_DOCUMENT_DEACTIVE,
    HEAD_MESH_DOCUMENT_ACTIVE,
    // _NA_004921_20120628_ADD_QUEST_NPC_EMOTICON_FOR_QUEST_TYPE
    HEAD_MESH_DOCUMENT_DAILY,
    HEAD_MESH_DOCUMENT_GUILD,
    HEAD_MESH_DOCUMENT_REPEAT,
    HEAD_MESH_SCROLL_DAILY,
    HEAD_MESH_SCROLL_GUILD,
    HEAD_MESH_SCROLL_REPEAT,
};

class HeadMeshInfoParser	: public Singleton<HeadMeshInfoParser> ,public IParser
{
public:
	HeadMeshInfoParser();
	~HeadMeshInfoParser();

	VOID                Init(DWORD dwPoolSize, char *szPackfileName);
	VOID                Init(DWORD dwPoolSize);

	VOID                Release();
	virtual	BOOL        LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);
	

	BASE_HeadMeshInfo*  GetHeadMeshInfo(DWORD dwKey);

private:
	VOID		        Unload();
	util::SolarHashTable<BASE_HeadMeshInfo*>*   m_pDataTable;
};

