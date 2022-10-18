#pragma once

#include <SolarHashTable.h>
#include <Singleton.h>

#define MAX_FILE_NAME 128

struct Specail_Texture
{
     WzID      m_TextureID;
     char      szFilename[MAX_FILE_NAME];
     HANDLE    hSpecialTexture;
};

typedef STLX_HASH_MAP<WzID, Specail_Texture> TextureTable;


class SpecialTexutureInfoParser	: public Singleton<SpecialTexutureInfoParser> ,public IParser
{
    
public:
   SpecialTexutureInfoParser();
   ~SpecialTexutureInfoParser();


    VOID            Init(const char* pack_file_name);

    VOID			Release();

    TextureTable&   GetTextrueInfo(){return m_DataTable;}

private:
	TextureTable	m_DataTable;

	BOOL			_Load( BOOL bReload );

public:
	virtual	BOOL	LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload );
};
