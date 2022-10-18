#include "SunClientPrecompiledHeader.h"
#include "GameOptionListParser.h"
#include "InterfaceManager.h"


//------------------------------------------------------------------------------ 
OptionItemInfoParser::OptionItemInfoParser()
{
}
//------------------------------------------------------------------------------ 
OptionItemInfoParser::~OptionItemInfoParser()
{
}
//------------------------------------------------------------------------------ 
void OptionItemInfoParser::Release()
{
    std::map<DWORD, sOptionItemInfo*>::iterator it = m_OptionItemMap.begin();

    while( it != m_OptionItemMap.end() )
    {
        delete it->second;
        ++it;
    }

    m_OptionItemMap.clear();
}

//------------------------------------------------------------------------------ 
BOOL OptionItemInfoParser::LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload)
{
    if (bReload == TRUE)
    {
        return FALSE;
    }

    int nRowSize = GetRowSize();
    for (int nRow = 0; nRow < nRowSize; ++nRow)
    {
		std::string ItemTypeText = GetDataString("ItemType", nRow);
        DWORD itemIdxe = 0;

        sOptionItemInfo* info = new sOptionItemInfo;
        info->m_OptionID = GetDataDWORD("OptionID", nRow );
        info->m_PageIndex = GetDataDWORD("PageIndex", nRow );
#ifdef _NA_000000_20130423_GAMEOPTION_STRINGCODE_CHANGE
        itemIdxe = GetDataDWORD("DisplayName", nRow );
        info->m_DisplayName = g_InterfaceManager.GetInterfaceString(itemIdxe);
        itemIdxe = GetDataDWORD("CategoryName", nRow );
        info->m_CategoryName = g_InterfaceManager.GetInterfaceString(itemIdxe);
#else
		info->m_DisplayName = GetDataString("DisplayName", nRow );
        info->m_CategoryName = GetDataString("CategoryName", nRow );
#endif
        info->m_Comment = GetDataString("Comment", nRow );


        if( ItemTypeText == "COMBO" )
        {
            info->m_GaugeRange = 0;
#ifdef _NA_000000_20130423_GAMEOPTION_STRINGCODE_CHANGE
            DWORD itemdata = GetDataDWORD("ItemData", nRow);
            std::string data = g_InterfaceManager.GetInterfaceString(itemdata);
#else
			std::string data = GetDataString("ItemData", nRow);
#endif
            if( data.size() > 2 && data.at(0) == '\"')
            {
                // ""를 제거
                data = data.substr( 1, data.size() -2 );
            }

            info->m_ItemType = 0;

            // , 를 기준으로 나누어 추가
            size_t current = 0;
            size_t pos = data.find_first_of( ',', current );
            while( pos != std::string::npos )
            {
                info->m_ItemDataArray.push_back(data.substr( current, pos - current ));

                current = pos + 1;
                pos = data.find_first_of( ',', current );
            }

            if( current < data.size() )
            {
                info->m_ItemDataArray.push_back(data.substr( current, data.size() - current ));
            }
        }
        else if( ItemTypeText == "GAUGE" )
        {
            info->m_ItemType = 1;
#ifdef _NA_000000_20130423_GAMEOPTION_STRINGCODE_CHANGE
            DWORD itemdata = GetDataDWORD("ItemData", nRow);
            std::string data = g_InterfaceManager.GetInterfaceString(itemdata);
            info->m_GaugeRange = atoi(data.c_str());
#else
            
            info->m_GaugeRange = GetDataDWORD("ItemData", nRow );
#endif
        }
        else
        {
            info->m_ItemType = -1;
            info->m_GaugeRange = 0;
        }

        m_OptionItemMap[info->m_OptionID] = info;
    }

    return TRUE;
}

sOptionItemInfo* OptionItemInfoParser::GetOptionItemInfo( DWORD OptionID )
{
    std::map<DWORD, sOptionItemInfo*>::iterator it = m_OptionItemMap.find( OptionID );
    if( it == m_OptionItemMap.end() )
        return NULL;

    return it->second;
}

void OptionItemInfoParser::GetOptionItemInfoPage( DWORD PageIndex, OUT std::vector<sOptionItemInfo*>& Array )
{
    std::map<DWORD, sOptionItemInfo*>::iterator it = m_OptionItemMap.begin();

    Array.clear();
    while( it != m_OptionItemMap.end() )
    {
        if( it->second->m_PageIndex == PageIndex )
        {
            Array.push_back( it->second );
        }
        
        ++it;
    }
}
