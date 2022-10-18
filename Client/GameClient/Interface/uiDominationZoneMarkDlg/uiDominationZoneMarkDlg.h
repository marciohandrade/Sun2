#pragma once
#include "uibase.h"
#include "ZoneMarkRewardInfo.h"

class uiDominationZoneMarkDlg: public uiBase
{
#ifdef _NA_000000_20130114_RENEWER_UI
public:
	struct CalculatedOption
	{
		DWORD OptionKind;
		int OptionValue;
		int OptionRatio;
		DWORD OptionNC;

		CalculatedOption()
		{
			OptionKind = 0;
			OptionValue = 0;
			OptionRatio = 0;
			OptionNC = 0;
		}
	};

	struct OptionStringMark
	{
		TCHAR option_name_[INTERFACE_STRING_LENGTH];

		OptionStringMark()
		{
			memset(option_name_,0,sizeof(TCHAR)*INTERFACE_STRING_LENGTH);
		}

		void SetOptionMark(const TCHAR* option_string)
		{
			memcpy(option_name_,option_string,sizeof(TCHAR)*strlen(option_string));
		}
	};

	typedef STLX_MAP<DWORD, CalculatedOption> CalculatedOptionMap;
#endif

public:
	typedef std::vector<REWARD_OPTION>				REWARD_OPTION_VECTOR;
	typedef std::vector<REWARD_OPTION>::iterator	REWARD_OPTION_VECTOR_ITR;

private:
	enum eDIALOG_POS
	{
#ifdef _NA_000000_20130114_RENEWER_UI
		DLGPOS_ZONEMARK_PICl00,
		DLGPOS_ZONEMARK_PIC000,
		DLGPOS_ZONEMARK_PIC001,
  #ifdef _NA_008334_20150608_SONNENSCHEIN
        DLGPOS_ZONEMARK_PIC002,
        DLGPOS_ZONEMARK_PIC003,
  #endif //_NA_008334_20150608_SONNENSCHEIN
#else
		DLGPOS_ZONEMARK_PIC1,		
		DLGPOS_ZONEMARK_PIC2,		
		DLGPOS_ZONEMARK_PIC3,		
		DLGPOS_ZONEMARK_PIC4,		
		DLGPOS_ZONEMARK_PIC5,		
		DLGPOS_ZONEMARK_PIC6,		
		DLGPOS_ZONEMARK_PIC7,		
		DLGPOS_ZONEMARK_PIC8,	
		DLGPOS_ZONEMARK_PIC9,		
		DLGPOS_ZONEMARK_PIC10,		
		DLGPOS_ZONEMARK_PIC11,		
		DLGPOS_ZONEMARK_PIC12,		
		DLGPOS_ZONEMARK_PIC13,		
		DLGPOS_ZONEMARK_PIC14,		
		DLGPOS_ZONEMARK_PIC15,		
		DLGPOS_ZONEMARK_PIC16,
#endif


		DIALOG_MAX,
	};
    enum
    {
#ifdef _NA_000000_20130114_RENEWER_UI
		MAX_ZONEMARK = 16, 
#else
		MAX_ZONEMARK = (DLGPOS_ZONEMARK_PIC16 - DLGPOS_ZONEMARK_PIC1) + 1,
#endif
    };

public:
	uiDominationZoneMarkDlg(InterfaceManager *pUIMan);
	virtual ~uiDominationZoneMarkDlg();

public:
	virtual void Init(CDrawBase* pDrawBase);
	virtual void OnShowWindow(BOOL val);
	virtual void OnRenderSolarDialog();

	void Clear();

    void SetZoneMarkInfo(STLX_VECTOR<MAPCODE>& domination_state_info
#ifdef _NA_008334_20150608_SONNENSCHEIN
        , BYTE sonnenschein_level
#endif //_NA_008334_20150608_SONNENSCHEIN
        );
	
	REWARD_OPTION_VECTOR& GetRewardOptionInfo()	{ return player_zonemark_reward_info_.GetRewardOptionInfo(); }
    
    const DominationInfoParser::SelectedRewardOptions& GetSelectedRewardOptionInfo()	
    {
        return player_zonemark_reward_info_.GetSelectedRewardOptionInfo();
    }

#ifdef _NA_000000_20130114_RENEWER_UI
	virtual VOID	Release();
	void returnImageIndextoRect(int mark_count,int* score_index);
	int CheckModifyReturnValue(int mark_count,int* array_count);
	void render_mark_count_Image();
	void option_domination_calu();
	virtual void OnUpdateSolarDialog();
	CalculatedOptionMap& calculated_option_map() { return calculated_option_map_; }
#endif

private:

private:
	static WzID m_wzId[DIALOG_MAX];
	

    DWORD max_count_zonemark_;

    ZoneMarkRewardInfo player_zonemark_reward_info_;

#ifdef _NA_000000_20130114_RENEWER_UI
	HANDLE				            handle_text_Texture_;
	int								size_zone_marker_;
	CalculatedOptionMap				calculated_option_map_;
	std::vector<OptionStringMark>	calculate_option_list_;			
#else
	CCtrlPictureWZ* m_pZoneMarkPic[MAX_ZONEMARK];
#endif
#ifdef _NA_008334_20150608_SONNENSCHEIN
    BYTE sonnenschein_level_;
#endif //_NA_008334_20150608_SONNENSCHEIN

};
