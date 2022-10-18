//------------------------------------------------------------------------------				    
//                   	
//
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "SpecialTexutureInfoParser.h"
#include <SolarFileReader.h>


//----------------------------------------------------------------------------                   	
/**                    
*/
SpecialTexutureInfoParser::SpecialTexutureInfoParser()
{
}

//----------------------------------------------------------------------------                   	
/**                    
*/
SpecialTexutureInfoParser::~SpecialTexutureInfoParser()
{

}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID SpecialTexutureInfoParser::Release()
{

}

VOID SpecialTexutureInfoParser::Init(const char* pack_file_name) // null이면 pack file없는 거당
{

}
BOOL SpecialTexutureInfoParser::LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload )
{
	switch( ScriptCode )
	{
	case SCRIPT_SPECIAL_TEXTURE:
		return _Load( bReload );
	}

	return FALSE;
}

//----------------------------------------------------------------------------                   	
/**                    
*/
BOOL SpecialTexutureInfoParser::_Load( BOOL bReload )
{
    char szID[5] = {0,};

    int nRowSize = GetRowSize();

    for( int nRow = 0; nRow < nRowSize; ++nRow )
    {
        //1.TextureID
        strncpy(szID, GetDataString("TextureID",nRow),4);
        WzID TextureID	=  StrToWzID(szID);
        
        TextureTable::iterator it = m_DataTable.find(TextureID);

        if(it != m_DataTable.end()) {
            if(bReload == false) {
                FASSERT(!"데이터에 오류가 있습니다.");
            }
        }
        else {
            std::pair<TextureTable::iterator, bool> ret =
                m_DataTable.insert(std::make_pair(TextureID, Specail_Texture()));
            assert(ret.second);
            it = ret.first;
        }

        Specail_Texture& dest = it->second;
        ZeroMemory(&dest, sizeof(dest));
        dest.m_TextureID = TextureID;
        
        //2.FileName
        strncpy( dest.szFilename, GetDataString( "FileName", nRow ), MAX_FILE_NAME );

        //3.Handle초기화(실제로드시에 임시저장된다)
        dest.hSpecialTexture = INVALID_HANDLE_VALUE;
    }

    return TRUE;
}



