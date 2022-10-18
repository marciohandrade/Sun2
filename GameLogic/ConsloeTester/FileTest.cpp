#include "stdafx.h"

#include "FileTest.h"


void CFileTest::Test_EquipItem(const char* szfileName, ICSunOnlinePtr ptr, int nBinVer)
{
	int nsize, iNewVer11;
	_bstr_t bstr;

	int nSize = MAX_EQUIPMENT_SLOT_NUM*sizeof(ITEMSTREAMEX);
	printf("%d\n", nSize);
	byte buffEquipItem[MAX_EQUIPMENT_SLOT_NUM*sizeof(ITEMSTREAMEX)] = {0};
	char xmlBuff[4096 * 128] = {0,};

	ReadFromFile(szfileName, xmlBuff, nsize);
	bstr = xmlBuff;

	ptr->encodeEquipItem(bstr, buffEquipItem,&iNewVer11);
	bstr = ptr->decodeEquipItem(buffEquipItem, nBinVer);

	char szTmp[4096] = {0};
	char szT[16] = {0};
	char* sz = (char*)bstr;

	for(int i = 0 ; i < MAX_EQUIPMENT_SLOT_NUM*sizeof(ITEMSTREAMEX) ; i++)
	{
		sprintf(szT,"%x",buffEquipItem[i]);
		strcat(szTmp,szT);
	}
	int nnn = 0;
}


void CFileTest::Test_SKill(const char* szfileName, ICSunOnlinePtr ptr, int nBinVer)
{
	int nsize;
	int nRet;
	_bstr_t bstr;

	byte szSkill[MAX_SKILL_SLOT_NUM*2] = {0};	
	
	char xmlBuff[50000] = {0};

	ReadFromFile(szfileName, xmlBuff, nsize);
	bstr = xmlBuff;

	nRet = ptr->encodeSkill(bstr,szSkill,1);

}

void CFileTest::Test_ACStreamt(const char* szfileName, ICSunOnlinePtr ptr, int nBinVer)
{

	int nsize;

	_bstr_t bstr;

	BYTE ACStream[MAX_ACSTREAM_SIZE] = {0};
	BYTE Mission[MAX_MISSIONSTREAM_SIZE] = {0}; 
	BYTE ComplQStream[MAX_COMPL_QUESTSTREAM_SIZE] = {0};

	char xmlBuff[50000] = {0};

	ReadFromFile(szfileName, xmlBuff, nsize);
	bstr = xmlBuff;

	ptr->encodeACStream(bstr, ACStream);
	ptr->decodeACStream(ACStream,1,0,0);

}

void CFileTest::Test_Quest(const char* szfileName, ICSunOnlinePtr ptr, int nBinVer)
{
	
	int nsize;

	_bstr_t bstr;

	byte ProgrQStream[MAX_PROGR_QUESTSTREAM_SIZE] = {0};
	byte ComplQStream[MAX_COMPL_QUESTSTREAM_SIZE] = {0};
	//char xmlBuff[105000] = {0};
    char* xmlBuff = new char[200000];
    ZeroMemory(xmlBuff, 200000);

	ReadFromFile(szfileName, xmlBuff, nsize);
	bstr = xmlBuff;

	int nRet = ptr->encodeQuest(bstr, ProgrQStream, ComplQStream);
    printf("RESULT: %d\n", nRet);
	ptr->decodeQuest(ProgrQStream,ComplQStream);

    delete[] xmlBuff;

}

void CFileTest::Test_GuildWareHouse(const char* szfileName, ICSunOnlinePtr ptr, int nBinVer)
{
#ifdef __NA000584_20080201_GUILD_WAREHOUSE
	int nsize, iNewVer11;
	_bstr_t bstr;	

	byte buffwarehouse[MAX_GUILD_WAREHOUSE_SLOT_NUM*sizeof(ITEMSTREAMEX)] = {0};
	char xmlBuff[70000] = {0,};

	ReadFromFile(szfileName, xmlBuff, nsize);
	bstr = xmlBuff;

	ptr->encodeGuildWarehouseItem(bstr, buffwarehouse,&iNewVer11);
	
	bstr = ptr->decodeGuildWarehouseItem(buffwarehouse, nBinVer,  _bstr_t(""));
#endif
	
}

void CFileTest::Test_WareHouse(const char* szfileName, ICSunOnlinePtr ptr, int nBinVer)
{
	int nsize, iNewVer11;
	_bstr_t bstr;

	int nSize = MAX_WAREHOUSE_SLOT_NUM*sizeof(ITEMSTREAMEX);
	printf("%d\n", nSize);
	byte buffwarehouse[MAX_WAREHOUSE_SLOT_NUM*sizeof(ITEMSTREAMEX)] = {0};
	char xmlBuff[50000] = {0,};

	ReadFromFile(szfileName, xmlBuff, nsize);
	bstr = xmlBuff;

	ptr->encodeWarehouseItem(bstr, buffwarehouse,&iNewVer11);
	bstr = ptr->decodeWarehouseItem(buffwarehouse, nBinVer,  _bstr_t(""));
}

void CFileTest::Test_TempInventory(const char* szfileName, ICSunOnlinePtr ptr, int nBinVer)
{
	int nsize, iNewVer11;
	_bstr_t bstr;
	byte buffTmpInventory[MAX_TEMPINVENTORY_SLOT_NUM*sizeof(ITEMSTREAMEX)] = {0};
	char xmlBuff[50000] = {0,};

	ReadFromFile(szfileName, xmlBuff, nsize);
	bstr = xmlBuff;

	ptr->encodeTempInventoryItem(bstr, buffTmpInventory,&iNewVer11);
	bstr = ptr->decodeTempInventoryItem(buffTmpInventory, nBinVer);
}

void CFileTest::Test_Inventory(const char* szfileName, ICSunOnlinePtr ptr, int nBinVer)
{
	int nsize, iNewVer11;
	_bstr_t bstr;
	byte buffInventory[MAX_INVENTORY_SLOT_NUM*sizeof(ITEMSTREAMEX)] = {0};
	char xmlBuff[200000] = {0,};

	ReadFromFile(szfileName, xmlBuff, nsize);
	bstr = xmlBuff;

	ptr->encodeInventoryItem(bstr, buffInventory,&iNewVer11);
	bstr = ptr->decodeInventoryItem(buffInventory, nBinVer, 0, _bstr_t(""));
}

void CFileTest::Test_DecodeInventory(const char* szfileName, ICSunOnlinePtr ptr, int nBinVer)
{
    int nsize, iNewVer11;
    _bstr_t bstr;
    byte buffInventory[MAX_INVENTORY_SLOT_NUM*sizeof(ITEMSTREAMEX)] = {0};

    ReadFromBin(szfileName, buffInventory, nsize);
    
    bstr = ptr->decodeInventoryItem(buffInventory, nBinVer, 5, _bstr_t(""));
}

void CFileTest::Test_Item(const char* szfileName, ICSunOnlinePtr ptr)
{
    int nsize;
    char xmlBuff[200000] = {0,};
    ReadFromFile(szfileName, xmlBuff, nsize);
    _bstr_t bstr = xmlBuff;

    __int64 serial = 0;
    int code; 
    BYTE dura;
    BYTE version;
    BYTE item_part[sizeof(OPTIONPART)];

    BOOL b = ptr->EncodeItemInfo(bstr, &serial,&code,&dura,&version,item_part);
    ptr->DecodeItemInfo(0,serial, code, dura, version, item_part);

    int nn = 0;
    //ptr->DecodeItemInfo();
}


BOOL CFileTest::ReadFromFile(const char* filepath, char* lpData, int& nSize)
{
	HANDLE hFile; 

	hFile = CreateFile(filepath,           // create MYFILE.TXT 
		GENERIC_READ,                // open for writing 
		FILE_SHARE_READ,                            // do not share 
		NULL,                         // no security 
		OPEN_EXISTING,                // overwrite existing 
		FILE_ATTRIBUTE_NORMAL,      // normal file 
		NULL);                        // no attr. template 

	if (hFile == INVALID_HANDLE_VALUE) 
	{ 
		return FALSE;
	} 

	DWORD dwFileSize = GetFileSize(hFile, NULL);
	DWORD dwRead = 0;
	DWORD dwReaded = 0;
	
	while(true)
	{
		if(ReadFile(hFile, (BYTE*)lpData, 1024, &dwRead, NULL))
		{
			lpData += dwRead;
			dwReaded += dwRead;
			if(dwReaded >= dwFileSize || dwRead <= 0)
			{
				break;
			}
		}
		else
		{
			nSize = dwReaded;
            printf("Last Error %u\n", GetLastError());
            break;
		}		
	}	

	nSize = dwReaded;
	CloseHandle(hFile);

	return TRUE;
}	

BOOL CFileTest::ReadFromBin(const char* filepath, byte* lpData, int& nSize)
{
    HANDLE hFile; 

    hFile = CreateFile(filepath,           // create MYFILE.TXT 
        GENERIC_READ,                // open for writing 
        FILE_SHARE_READ,                            // do not share 
        NULL,                         // no security 
        OPEN_EXISTING,                // overwrite existing 
        FILE_ATTRIBUTE_NORMAL,      // normal file 
        NULL);                        // no attr. template 

    if (hFile == INVALID_HANDLE_VALUE) 
    { 
        return FALSE;
    } 

    DWORD dwFileSize = GetFileSize(hFile, NULL);
    DWORD dwRead = 0;
    DWORD dwReaded = 0;

    while(true)
    {
        if(ReadFile(hFile, lpData, 1024, &dwRead, NULL))
        {
            lpData += dwRead;
            dwReaded += dwRead;
            if(dwReaded >= dwFileSize || dwRead <= 0)
            {
                break;
            }
        }
        else
        {
            nSize = dwReaded;
            printf("Last Error %u\n", GetLastError());
            break;
        }		
    }	

    nSize = dwReaded;
    CloseHandle(hFile);

    return TRUE;
}

void CFileTest::ConvertBinToFile(const char* szfilename, BYTE* bin)
{
    int file_size = 0;
    char file_str[4096 * 128] = {0,};

    ReadFromFile(szfilename, file_str, file_size);

    std::string strtmp = file_str;

    for(int i = 0 , bin_index = 0; i < file_size ; i += 2, bin_index++ )
    {
        //std::string abc = strtmp.substr(i,2).c_str();
        bin[bin_index] = strtoul(strtmp.substr(i,2).c_str(), NULL, 16);        
    }
}

