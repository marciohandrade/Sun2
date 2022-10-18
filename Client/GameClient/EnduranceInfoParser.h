#ifndef SUN_CLIENT_ENDURANCEINFOPARSER_H_
#define SUN_CLIENT_ENDURANCEINFOPARSER_H_

#include <SolarHashTable.h>
#include <Singleton.h>
#include <SolarFileReader.h>
#include <SCItemSlot.h>
#include <ScriptCode.IParser.h>
#include <IconImage.h>
#include <SolarHashTable.h>
#include <Singleton.h>
#include <SolarFileReader.h>

#ifdef _NA_000000_20130114_RENEWER_UI

struct EnduranceInfo
{
	int id_;
	int type_;
	int item_index_;
	int x_position;
	int y_position;
	int width_;
	int height_;
	
	EnduranceInfo()
	{
		id_ = 0;
		type_= 0;
		item_index_= 0;
		x_position= 0;
		y_position= 0;
		width_= 0;
		height_= 0;
		
	}
};

class EnduranceInfoParser: public Singleton<EnduranceInfoParser>, public IParser
{
public:
	typedef STLX_VECTOR<EnduranceInfo*> EnduranceInfo_list;

	EnduranceInfoParser();
	virtual ~EnduranceInfoParser();

	virtual void Release();
	virtual BOOL LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);

	bool FindTypeData(std::vector<EnduranceInfo*>* MissionHelpInfo,int type);
	bool FindData(EnduranceInfo& MissionHelpInfo,int type,int itemIndex);
private:
	BOOL _Load(BOOL bReload);
	void Unload();

	EnduranceInfo_list				EnduranceInfo_info_;
};

#endif
#endif