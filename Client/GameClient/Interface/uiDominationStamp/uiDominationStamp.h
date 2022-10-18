#pragma once

#include "uibase.h"


class uiDominationStamp : public uiBase
{
public:
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
    enum eCOLOR_STATE
    {
        eCOLOR_STATE_INC,
        eCOLOR_STATE_DEC,
    };
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL
	enum eDIALOG_DOMINATION_STAMP
	{
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
        eDIALOG_DOMINATION_STAMP_STRING,
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL
		eDIALOG_DOMINATION_STAMP_TEXTURE,
		eDIALOG_DOMINATION_STAMP_MAX,
	};

	uiDominationStamp(InterfaceManager * pUIMan );
	~uiDominationStamp();
	virtual void		Init(CDrawBase * pDrawBase);
	virtual void		Release();
	virtual void		OnUpdateSolarDialog();
	virtual	void		OnRenderSolarDialog();
	virtual void		Process(DWORD dwTick);	
	virtual void		OnShowWindow(BOOL val);

protected:
	CControlWZ *		getControl(POSTYPE AtPos);
	POSTYPE				getControlIDToPos(WzID wzID);
	void				InitControl();

private:

	static WzID		m_wzID[eDIALOG_DOMINATION_STAMP_MAX];

#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
    CCtrlStaticWZ*     string_control_ptr_;
    DWORD              string_tick_;    
    DWORD               string_rate_alpha_color_;
    TCHAR              stamp_guild_name_[MAX_GUILDNAME_LENGTH+1];
    eCOLOR_STATE       alpha_color_state_;
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL
	CCtrlPictureWZ*	   m_pctrlPicture;
	HANDLE			   m_hStampTex;
	SI_FRECT		   m_sRect;
	float			   m_fRate;
	WzID			   m_wiStampID;
	DWORD			   m_dwTick;

	BOOL			m_bShow;

public:
	void			SetRate(float fRate){m_fRate = fRate;}
	void			SetStampID(WzID wiID){m_wiStampID = wiID;}
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
    void            set_stamp_guild_name(TCHAR* stamp_guild_name);
public:
    void            RenderStampGuildText();
private:
    void            ProcessStampGuildString(DWORD tick);
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL
};

inline CControlWZ * uiDominationStamp::getControl(POSTYPE AtPos)
{
	assert(AtPos < eDIALOG_DOMINATION_STAMP_MAX);
	return GetControlWZ(m_wzID[AtPos]);
}

inline POSTYPE uiDominationStamp::getControlIDToPos(WzID wzID)
{
	for ( int i = 0; i < eDIALOG_DOMINATION_STAMP_MAX ; ++i)
	{
		if ( m_wzID[i] == wzID)
			return i;
	}
	return eDIALOG_DOMINATION_STAMP_MAX;
}

