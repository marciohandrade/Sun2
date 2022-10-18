#pragma once

#include "../gameLogic_Sun/_gameLogic_Sun_i.c"

class CFileTest
{
public:
	void Test_EquipItem(const char* szfileName, ICSunOnlinePtr ptr, int nBinVer);
	void Test_SKill(const char* szfileName, ICSunOnlinePtr ptr, int nBinVer);
	void Test_ACStreamt(const char* szfileName, ICSunOnlinePtr ptr, int nBinVer);
	void Test_Quest(const char* szfileName, ICSunOnlinePtr ptr, int nBinVer);
	void Test_WareHouse(const char* szfileName, ICSunOnlinePtr ptr, int nBinVer);
	void Test_TempInventory(const char* szfileName, ICSunOnlinePtr ptr, int nBinVer);
	void Test_Inventory(const char* szfileName, ICSunOnlinePtr ptr, int nBinVer);
    void Test_DecodeInventory(const char* szfileName, ICSunOnlinePtr ptr, int nBinVer);
	void Test_GuildWareHouse(const char* szfileName, ICSunOnlinePtr ptr, int nBinVer);
    void Test_Item(const char* szfileName, ICSunOnlinePtr ptr);

    void ConvertBinToFile(const char* szfilename, BYTE* bin);
protected:
	BOOL ReadFromFile(const char* filepath, char* lpData, int& nSize);	
    BOOL ReadFromBin(const char* filepath, byte* lpData, int& nSize);
};