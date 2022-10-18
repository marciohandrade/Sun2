#include "uibase.h"

#ifdef __CN_269_MATRIX_CARD_AUTH
#include <SunAuth/AuthProtocol_CF.h>
#include "FTextRender.h"

#define NUM_INPUT_MATRIX_CODE	3

struct INPUT_DATA
{
	BOOL	bInput;
	int		Value;
};

class MatrixCardAuth : public uiBase
{
	enum eDIALOG_POS
    {
		DLGPOS_TXT_INPUT1,
		DLGPOS_TXT_INPUT2,
		DLGPOS_TXT_INPUT3,
		DLGPOS_TXT_MAIN,
		DLGPOS_TXT_SUB_TITLE,
		DLGPOS_TXT_SYSTEM,
		DLGPOS_BTN_EXIT,
		DLGPOS_CHK_INPUT1,
		DLGPOS_CHK_INPUT2,
		DLGPOS_CHK_INPUT3,
		DLGPOS_MAX,
    };

	enum
	{
		MAX_TXT_CONTROL = DLGPOS_BTN_EXIT,
		START_CHECK_CONTROL = DLGPOS_CHK_INPUT1,
		MAX_CHECK_CONTROL = DLGPOS_CHK_INPUT3 - START_CHECK_CONTROL +1,
	};

public:
		MatrixCardAuth(InterfaceManager * pUIMan );
		~MatrixCardAuth();
		virtual void		Init(CDrawBase * pDrawBase);
		virtual void		Release();
		virtual void		OnUpdateSolarDialog();
		virtual void		OnRenderSolarDialog();
		virtual void		Process(DWORD dwTick);
		virtual void		OnShowWindow(BOOL val);
		virtual void		MessageProc(SI_MESSAGE * pMessage);
		virtual void		NetworkProc(MSG_BASE * pMsg);

public:
		void				InitializeMatrixCode(SC_INFO * pSCInfo);
#ifdef __CHINA_MATRIXCARD_CODE_ONE_SHOT_SUBMIT
		void				SEND_AUTH_MATRIX_CARD_ACK(int iMatrixCode);
		void				SetNoticeMessage();
#else
		void				SEND_AUTH_MATRIX_CARD_ACK();	
#endif
protected:
		CControlWZ *		getControl(POSTYPE AtPos);
		POSTYPE				getControlIDToPos(WzID wzID);

private:
		void				InitFormatText();

private:
		static WzID			m_wzID[DLGPOS_MAX];
		CCtrlStaticWZ *		m_pStatic[MAX_TXT_CONTROL];
		CCtrlButtonWZ *		m_pButton;
		CCtrlButtonCheckWZ * m_pCheck[MAX_CHECK_CONTROL];
		INPUT_DATA			m_InputData[NUM_INPUT_MATRIX_CODE];
		FTextRender			m_FtextRender;
};

inline CControlWZ * MatrixCardAuth::getControl(POSTYPE AtPos)
{
	assert(AtPos < DLGPOS_MAX);
	return GetControlWZ(m_wzID[AtPos]);
}

inline POSTYPE MatrixCardAuth::getControlIDToPos(WzID wzID)
{
	for ( int i = 0; i < DLGPOS_MAX ; ++i)
	{
		if ( m_wzID[i] == wzID)
			return i;
    }
	return DLGPOS_MAX;
}
#endif // __CN_269_MATRIX_CARD_AUTH
