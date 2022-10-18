#ifndef _uiFateItemSystem_h_
#define _uiFateItemSystem_h_

#include "uibase.h"

#include "GlobalFunc.h"
#include "InterfaceManager.h"
#include "bink.h"

#include "IconImage.h"
#include "SoundEffect.h"
#include "SCitemSlot.h"

#define BUTTON_EFFECT_DELAY 1000
#define EFFECT_FRAME_UPDATE_TIME 60
#define MOVIE_FRAME_UPDATE_TIME 55

struct stANIMATE_SPRITE
{
	HANDLE				hEffectTexture;
	int					iEffectFrame;
};

class uiFateItemSystem : public uiBase
{
	enum eDIALOG_POS
    {
		DLGPOS_BTN_EXIT,
		DLGPOS_BTN_START,
		DLGPOS_BTN_RESULT_VIEW,
		DLGPOS_BTN_OK,
		DLGPOS_BTN_TARGET_ITEM,

		DLGPOS_PIC_MOVIE,

		DLGPOS_TXT_PHYSICAL,
		DLGPOS_TXT_DURATION,
		DLGPOS_TXT_DATE,
		DLGPOS_MAX,
    };

	enum
	{
		TXT_PHYSICAL,
		TXT_DURATION,
		TXT_DATE,
		MAX_BUTTON = DLGPOS_PIC_MOVIE,
		START_TXT  = DLGPOS_TXT_PHYSICAL,
		MAX_TXT	   = DLGPOS_MAX - DLGPOS_TXT_PHYSICAL,
	};

	enum
	{
		EFFECT_POS_TARGET_SLOT,
		EFFECT_POS_STATIC,
		EFFECT_MAX,
	};

public:
		uiFateItemSystem(InterfaceManager * pUIMan );
		~uiFateItemSystem();
		virtual void		Init(CDrawBase * pDrawBase);
		virtual void		Release();
		virtual void		OnUpdateSolarDialog();
		virtual void		OnRenderSolarDialog();
		virtual void		Process(DWORD dwTick);
		virtual void		OnShowWindow(BOOL val);
		virtual void		MessageProc(SI_MESSAGE * pMessage);
		virtual void		NetworkProc(MSG_BASE * pMsg);

		inline void			SetTargetItemPos(POSTYPE TargetItemPos) { m_iTargetItemPos = TargetItemPos;}
		inline void			SetResultItem(const SCItemSlot& ItemSlot) { m_ResultItem = ItemSlot;}


		void				SetTargetItem(const BASE_ITEMINFO * pInfo, POSTYPE TargetItemPos);
		void				NET_SEND_CG_ITEM_VALIDATION_OF_REIHALT_SYN();

		void				SetFateItemProcessState(eFIS eState);
		void				InitBaseAtributeCaption();
		void				SetTargetItemLock(bool bLock);
		
protected:
		CControlWZ *		getControl(POSTYPE AtPos);
		POSTYPE				getControlIDToPos(WzID wzID);
		void				ProcessCaptionEffect(DWORD dwTick);
		void				ProcessRollingNumber();
		void				UpdateShowNHideButton();
		void				UpdateMovie();
		void				InitBink(bool bConsciousness);
		void				ReleaseBink();
		void				RenderMovie();
#ifdef _INTERNATIONAL_UI
		void				RenderTooltip();
#endif//_INTERNATIONAL_UI
		void				RenderEffectTexture(int id,  int PosX, int PosY, int width, int height, int numX, int numY, bool bLoop);

		void				InsertResultItem();
private:
		static WzID	m_wzID[DLGPOS_MAX];
		UI_CONTROL_STATIC_PROPERTY(TargetName,StrToWzID("ST08"));
		UI_CONTROL_STATIC_PROPERTY(ClassName, StrToWzID("ST09"));
		UI_CONTROL_STATIC_PROPERTY(LevelLimit, StrToWzID("ST10"));

		CCtrlPictureWZ *	m_pPictureMovie;
		CCtrlButtonWZ  *	m_pButton[MAX_BUTTON];
		CCtrlStaticWZ *		m_pStatic[MAX_TXT];

		DWORD				m_dwAccmulateTick;
		int					m_iCaptionEffectCount;
		bool				m_bPlayNotLoopEffect;
		eFIS				m_iState;
		HBINK				m_hBink;
		HANDLE				m_hRenderTargetTexture; // Bink¿ë 

		stANIMATE_SPRITE	m_stAnimateSprite[EFFECT_MAX];
		POSTYPE				m_iTargetItemPos;
		IconImage *			m_pIconImage;
		SCItemSlot			m_ResultItem;
};

inline CControlWZ * uiFateItemSystem::getControl(POSTYPE AtPos)
{
	assert(AtPos < DLGPOS_MAX);
	return GetControlWZ(m_wzID[AtPos]);
}

inline POSTYPE uiFateItemSystem::getControlIDToPos(WzID wzID)
{
	for ( int i = 0; i < DLGPOS_MAX ; ++i)
	{
		if ( m_wzID[i] == wzID)
			return i;
    }
	return DLGPOS_MAX;
}


#endif // _uiFateItemSystem_h_
