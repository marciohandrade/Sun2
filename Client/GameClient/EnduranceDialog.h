
#pragma once
#include "solardialog.h"

#define	RENEW_TIME	1000

struct UPDATE_STATUS
{
	DWORD	dwCurTick;
	BOOL	bUpdate;
	BOOL	bChangeData;
};

#ifdef __NA_001062_20080623_STRENGTH_PENALTY
struct sSTRENGTH_PENALTY;
#endif//__NA_001062_20080623_STRENGTH_PENALTY

enum EnduranceDIalog_Pos
{
	ENDURANCE_PIC1	= 0,
	ENDURANCE_PIC2,
	ENDURANCE_PIC3,
	ENDURANCE_PIC4,
	ENDURANCE_PIC5,
	ENDURANCE_PIC6,
	ENDURANCE_PIC7,
	ENDURANCE_PIC8,
	ENDURANCE_PIC9,
	ENDURANCE_PIC10,
	ENDURANCE_PIC11,
	ENDURANCE_PIC12,
	ENDURANCE_PIC13,
	ENDURANCE_PIC14,
	ENDURANCE_PIC15,
	ENDURANCE_PIC16,
	ENDURANCE_PIC17,
	ENDURANCE_PIC18,
	ENDURANCE_PIC19,
	ENDURANCE_PIC20,
	ENDURANCE_PIC21,	
	ENDURANCE_PIC22,
	ENDURANCE_PIC23,
	ENDURANCE_PIC24,
	ENDURANCE_PIC25,
	ENDURANCE_PIC26,
	ENDURANCE_PIC27,
	MAX_PICTURE_CONTROL,
};

class SCItemSlot;

class EnduranceDIalog : public SolarDialog
{

	enum
	{
		DUR_HELMET = 0,
		DUR_PROTECTOR,
		DUR_ARMOR,
		DUR_GLOVER,
		DUR_BELT,
		DUR_PANTS,
		DUR_BOOTS,
		DUR_WEAPON,
		DUR_SHIRTS,
		MAX_EQUIPMENT_TYPE,
	};

	enum
	{
		DUR_STATUS_FULL = 0,
		DUR_STATUS_MIDDLE,
		DUR_STATUS_LOW,
	};
public:
	EnduranceDIalog(void);
	~EnduranceDIalog(void);

	VOID					Init( CDrawBase * pDrawBase );
	virtual VOID			Release();
	virtual VOID			MessageProc( SI_MESSAGE * pMessage );
	virtual VOID			NetworkProc( MSG_BASE * pMsg );
	virtual VOID    		Process(DWORD dwTick);
	virtual VOID			OnRenderSolarDialog();
	virtual VOID			OnShowWindow(BOOL val);

	void					UpdateEquipEnduranceState();
	BOOL					IsWounded();

#ifdef __NA_001062_20080623_STRENGTH_PENALTY
	WzColor					GetEnduraLevelColor(SCItemSlot& rItemSlot);
	
#endif//__NA_001062_20080623_STRENGTH_PENALTY
#ifdef _NA_000000_20130114_RENEWER_UI
	void					GetMessage(DWORD messageID,LPARAM lParam,WPARAM wParam);
	DWORD                   GetDialogKey() { return m_DialogKey; }
	void                    SetDialogKey( DWORD DialogKey ) { m_DialogKey = DialogKey; }

#endif
protected:

#ifdef __NA_001062_20080623_STRENGTH_PENALTY
	void					RenderDuraEquip();
	sSTRENGTH_PENALTY*		GetPenaltyInfo(SCItemSlot& rItemSlot);
	int						GetEnduraPictureIndex(SCItemSlot& rItemSlot);
#endif//__NA_001062_20080623_STRENGTH_PENALTY

	CControlWZ *			getControl( POSTYPE AtPos );
	POSTYPE					getCtrlIDToPos( WzID wzId );
	void					AllPictureControlShowOff();
	int						GetEquipContainerPos(int iNum);
	int						EachPictureControlSelectLevel(SCItemSlot& rItemSlot,int curEquip);

private:
	static WzID				m_wzId[MAX_PICTURE_CONTROL];
	CCtrlPictureWZ*			m_pPicture[MAX_PICTURE_CONTROL];

	int						m_iEquipmentStatus[MAX_EQUIPMENT_TYPE];
	UPDATE_STATUS			m_kUpdate;

	//이미지 실제 좌표값
#ifdef _NA_000000_20130114_RENEWER_UI
	POINT					equip_real_pos_[9];
	DWORD					m_DialogKey;
#endif

#ifdef __NA_001062_20080623_STRENGTH_PENALTY
	HANDLE                  m_hDuraTexture;	
#endif//__NA_001062_20080623_STRENGTH_PENALTY
};
