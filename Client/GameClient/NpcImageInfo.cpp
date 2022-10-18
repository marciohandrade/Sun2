#include "SunClientPrecompiledHeader.h"

#include "NpcImageInfo.h"
#include "GlobalFunc.h"

const DWORD kTempNpcImageCode = 1;


NPCImageInfoParser::NPCImageInfoParser():	m_pDataTable ( NULL )
{
}

NPCImageInfoParser::~NPCImageInfoParser()
{
	assert( m_pDataTable == NULL);
}

VOID NPCImageInfoParser::Release()
{
	Unload();
	if (m_pDataTable)
		delete m_pDataTable;
	m_pDataTable = NULL;
}

VOID NPCImageInfoParser::Init(DWORD dwPoolSize)
{
	assert( m_pDataTable == NULL );
	m_pDataTable = new util::SolarHashTable<BASE_NPCImageInfo*>;
	m_pDataTable->Initialize( dwPoolSize );
}

VOID NPCImageInfoParser::Init( DWORD dwPoolSize, char * pszPackFileName )
{
	Init( dwPoolSize );
	SetPackFileName( pszPackFileName );
}

VOID NPCImageInfoParser::Unload()
{
	BASE_NPCImageInfo * pInfo = NULL;
	if( m_pDataTable == NULL )
	{
		return;
	}
	m_pDataTable->SetFirst();
	while( pInfo = m_pDataTable->GetNext() )
	{
		delete pInfo;
	}
	m_pDataTable->RemoveAll();
}

BASE_NPCImageInfo* NPCImageInfoParser::GetNpcImageInfo(DWORD dwKey)
{
    BASE_NPCImageInfo* npc_image_info = m_pDataTable->GetData( dwKey );
    if ((npc_image_info == NULL) && 
        (dwKey != 0))
    {
        npc_image_info = m_pDataTable->GetData(kTempNpcImageCode);
        WRITELOG(eLOG_LEVEL_ERR, 
            c_wOUTPUT_LOG_FILE | c_wOUTPUT_LOG_SYSCHAT, 
            _T("NPCImageInfo.txt 를 확인해주세요. 잘못된 인덱스입니다[%d]."), dwKey);
    }

    return npc_image_info;
}

#pragma warning ( push )
#pragma warning ( disable : 4244)

BOOL NPCImageInfoParser::LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload)
{
	int nRowSize = GetRowSize();

	for (int nRow = 0; nRow < nRowSize; ++nRow)
	{
		BASE_NPCImageInfo* pInfo = new BASE_NPCImageInfo;

		//Index
		pInfo->wIndex	= GetDataWORD("Index", nRow);

		//BClassCode
		pInfo->bClassCode	= GetDataBYTE("BClassCode", nRow);

        //QAcceptAni
        pInfo->questAcceptAni = StrToWzID(GetDataString("QAcceptAni", nRow));

        //QCompleteAni
        pInfo->questCompleteAni = StrToWzID(GetDataString("QCompleteAni", nRow));

        //TalkAni
        pInfo->talkAni = StrToWzID(GetDataString("TalkAni", nRow));

        //FaceImage
        pInfo->faceImageID = GetDataBYTE("FaceImage", nRow);
        if (pInfo->faceImageID == 0)
        {
            pInfo->faceImageID = 1;
        }

        //HairImage
        pInfo->hairImageID = GetDataBYTE("HairImage", nRow);
        if (pInfo->hairImageID == 0)
        {
            pInfo->hairImageID = 1;
        }

        //Acc1
        pInfo->AccCode1 = GetDataWORD("Acc1", nRow);

        //Acc2
        pInfo->AccCode2 = GetDataWORD("Acc2", nRow);

        //Acc3
        pInfo->AccCode3 = GetDataWORD("Acc3", nRow);

        //Enchant
        pInfo->Enchant = GetDataInt("Enchant", nRow);

        //showhelmet
        pInfo->showhelmet = GetDataBYTE("ShowHelmet", nRow);

		//WeaponImage
		pInfo->weaponCode	= GetDataWORD("WeaponImage", nRow);
		
		//ArmorImage
		pInfo->armorCode	= GetDataWORD("ArmorImage", nRow);
		
		//ProtectImage
		pInfo->protectCode  = GetDataWORD("ProtectImage", nRow);

		//HelmImage
		pInfo->helmetCode	= GetDataWORD("HelmImage", nRow);

		//PantsImage
		pInfo->pantsCode	= GetDataWORD("PantsImage", nRow);
		
		//BootImage
		pInfo->bootsCode	= GetDataWORD("BootImage", nRow);

		//GloveImage
		pInfo->gloveCode	= GetDataWORD("GloveImage", nRow);

		//BeltImage
		pInfo->beltCode		= GetDataWORD("BeltImage", nRow);

		//ShirtImage	
		pInfo->shirtsCode	= GetDataWORD("ShirtImage", nRow);

#ifdef _NA_008185_20150402_COSTUME_ITEM_FOR_NPC
        pInfo->npc_costume_item = GetDataWORD("NpcCostumeItem", nRow);
#endif //_NA_008185_20150402_COSTUME_ITEM_FOR_NPC

		m_pDataTable->Add(pInfo, pInfo->wIndex);

	}
	return TRUE;
}

#pragma warning ( pop )
