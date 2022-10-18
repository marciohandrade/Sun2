//=======================================================================================================================
/// SummonTargetDialog
/**
	@author
			±èÁö¿Ï 
	@since

	@remarks
			- 		
*/


#pragma once

#include "uiBase.h"

#define	MAX_SUMMON_COMMAND		6

class Object;
class Character;
class InterfaceManager;

class SummonTargetDialog :
	public uiBase
{

#ifdef _NA_000000_20130114_RENEWER_UI
public:
	struct stTARGETOVER_
	{
		RECT image_source_;
		POSTYPE screen_position;
		bool is_over_mouse;
	};
#endif

public:
	enum PACKET_STATUS_SUMMONTARGET                        
	{ 
		PACKET_STATUS_SUMMONTARGET_NONE				= 0, 
		PACKET_STATUS_SUMMONTARGET_ATTACK			= 1,        
		PACKET_STATUS_SUMMONTARGET_FOLLOW			= (2 << 1), 
		PACKET_STATUS_SUMMONTARGET_DELEGATE_ATTACK	= (2 << 2), 
		PACKET_STATUS_SUMMONTARGET_DELEGATE_DEFENSE	= (2 << 3), 
		PACKET_STATUS_SUMMONTARGET_DESTROY			= (2 << 4), 
		PACKET_STATUS_SUMMONTARGET_NON_PK_ATTACK	= (2 << 5), 
	};

	enum eSUMMON_TARGET_POS
	{
		SUMMON_TARGET_PIC_GAUGE = 0, 
		SUMMON_TARGET_PIC_MP_GAUGE, 
		SUMMON_TARGET_SKILL_EFFECT1, 
		SUMMON_TARGET_SKILL_EFFECT2, 
		SUMMON_TARGET_SKILL_EFFECT3, 
		SUMMON_TARGET_SKILL_EFFECT4, 
		SUMMON_TARGET_SKILL_EFFECT5, 
		SUMMON_TARGET_SKILL_EFFECT6, 
		SUMMON_TARGET_SKILL_EFFECT7, 
		SUMMON_TARGET_SKILL_EFFECT8, 
		SUMMON_TARGET_SKILL_EFFECT9, 
		SUMMON_TARGET_SKILL_EFFECT10, 
		SUMMON_TARGET_TEXT_INFO,
		SUMMON_TARGET_PIC_MON,
		SUMMON_TARGET_CHECK_ATTACK,
		SUMMON_TARGET_CHECK_FOLLOW,
		SUMMON_TARGET_CHECK_DELEGATE_ATTACK,
		SUMMON_TARGET_CHECK_DELEGATE_DEFENSE,
		SUMMON_TARGET_CHECK_DESTROY,
		SUMMON_TARGET_CHECK_NON_PK_ATTACK,
        SUMMON_TARGET_TEXT_INFO2,

		Dialog_Max
	};

#define MAX_SUMMON_TARGETDLG_SKILL_EFFECT 10

public:
	SummonTargetDialog(InterfaceManager* pUIMan);
	virtual ~SummonTargetDialog(void);

	VOID			Init( CDrawBase * pDrawBase );
	virtual VOID	Release();

	virtual VOID	MessageProc( SI_MESSAGE * pMessage );
	virtual VOID	NetworkProc( MSG_BASE * pMsg );

	virtual VOID	OnUpdateSolarDialog();
	virtual VOID	OnRenderSolarDialog();

	VOID			SetTargetKey(DWORD dwTargetKEY);
    DWORD           GetTargetKey();

	BYTE            ConvertCommandPos(BYTE bCommand);
	void            ResetOhterCheckButton(BYTE bCommand);
	void            ResetCheckButton(BYTE bCommand);

	BOOL			CommandSummonMoster(eSUMMON_TARGET_POS pos);

	BOOL			SendSummonCommandSyn( BYTE CommandType );

    void            UpdateStatusIcon();
    virtual void MoveDefaultWindowPos();

protected:
	DWORD           GetBit(BYTE bPos);

	CControlWZ *	getControl( POSTYPE AtPos );
	POSTYPE			getCtrlIDToPos( WzID wzId );

	virtual VOID	OnShowWindow( BOOL val );
	
	VOID			SetTargetInfo();
    VOID            SetHP( float fHPPercent ,float fRadio);
	VOID            SetMP( float fMpPercent ,float fRadio);

#ifdef _NA_000000_20130114_RENEWER_UI
	void RenderMouseOver();
#endif
private:
	DWORD			m_dwTargetKEY;
	DWORD			m_dwPrevTargetKEY;
	DWORD           m_dwPacketStatus;
	static WzID	    m_wzId[Dialog_Max];

	TCHAR			m_szResultInfo[512];


    float			m_fGaugeSize;

	
	RECT			m_rcGauge;

	int             m_oldIndex;

    vector<SKILL_EFFECT_IMAGE> status_icon_container_;
#ifdef _NA_000000_20130114_RENEWER_UI
	stTARGETOVER_ render_over_iamge_[6];
	RECT		  isFillRectPos;
	HANDLE	      render_over_texture_;
#endif

};

inline
DWORD
SummonTargetDialog::GetTargetKey()
{
    return m_dwTargetKEY;
}
