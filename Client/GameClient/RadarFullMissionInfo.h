#ifndef SUN_CLIENT_RADARFULLMISSIONINFO_H_
#define SUN_CLIENT_RADARFULLMISSIONINFO_H_


#if _MSC_VER > 1000
#pragma once
#endif

#include <SolarHashTable.h>
#include <Singleton.h>
#include <SolarFileReader.h>
#include <SCItemSlot.h>
#include <ScriptCode.IParser.h>
#include <IconImage.h>

#ifdef _NA_000000_20120911_FULL_MISSION_HELPER

struct MissionData
{
	int   FieldID_;
	int   MapID_;
	int   IconID_;
	int   OverIconID_;
	float IconPosX;
	float IconPosY;
	float IconPosZ;
	int   StringID;
	TCHAR color_[512];

	MissionData() : 
		FieldID_(-1),
		MapID_(-1),
		IconID_(-1),
		OverIconID_(-1),
		IconPosX(0.0f),
		IconPosY(0.0f),
		IconPosZ(0.0f),
		StringID(0)
	  {
		  memset(color_,0,sizeof(TCHAR)*512);
	  }

};

class RadarFullMissionInfoParser: public util::Singleton<RadarFullMissionInfoParser>, public IParser
{
public:
	typedef STLX_VECTOR<MissionData*> MissionHelper_list;

	RadarFullMissionInfoParser();
	virtual ~RadarFullMissionInfoParser();

	virtual void Release();
	virtual BOOL LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);

	bool FindData(std::vector<MissionData*>* MissionHelpInfo,int mapcode,int fieldcode);

private:
	BOOL _Load(BOOL bReload);
	void Unload();

	MissionHelper_list				MissionHelper_info_;
	HANDLE				            hRadarTexture_;
	POINT				            RadarTextureSize_;

	RECT							RadarBasePos_;
	RECT							RadarOverPos_;

public:
	HANDLE							GetMissionTexture() { return hRadarTexture_;}
	RECT							GetInputReturnRealSize(int Number,bool isMouseOver);
	
private:
	__forceinline void				DeleteStringToAddString(TCHAR* OutText,const TCHAR* InputText , int OutCount,const TCHAR* AddString);
};

#endif //#ifdef _NA_000000_20120911_FULL_MISSION_HELPER

#endif