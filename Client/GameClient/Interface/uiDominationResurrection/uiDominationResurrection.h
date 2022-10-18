#pragma once

// 2버튼 시스템창 uiSystemConfirm 에서 조금만 개조.

#ifdef _NA_007029_20140120_DOMINATION_RESURRECTION_REFORM

#include "uibase.h"
#include "interfacemanager.h"


class uiDominationResurrection : public uiBase
{
    enum UIControlPos
    {
        kControl_Button_TI00 = 0,
        kControl_Button_TI01,
        kControl_List_L000,

        kControl_Size
    };

public:
    uiDominationResurrection(InterfaceManager *pUIMan);
    virtual ~uiDominationResurrection();

public:
    virtual void            Init(CDrawBase* pDrawBase);
    virtual void            Release();

protected:
    virtual void	        OnShowWindow( BOOL val );
    virtual void            OnUpdateSolarDialog();
    virtual void            OnRenderSolarDialog();

public:
    virtual void	        NetworkProc( MSG_BASE * pMsg ) {};         // Pure
    virtual void	        MessageProc( SI_MESSAGE * pMessage );   // Pure

protected:
    bool                    OnLButtonClick(SI_MESSAGE* message);
};













#endif //_NA_007029_20140120_DOMINATION_RESURRECTION_REFORM