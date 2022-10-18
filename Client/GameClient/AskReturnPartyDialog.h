#pragma once

#include "askdialog.h"

class AskReturnPartyDialog: public AskDialog
{
public:
    AskReturnPartyDialog(InterfaceManager* ui_man);
    virtual ~AskReturnPartyDialog(void);

protected:
    virtual void OnUpdateSolarDialog();
    virtual void MessageProc(SI_MESSAGE* pMessage);
    virtual void OnShowWindow(BOOL val);
    virtual void OnRenderSolarDialog();

    void SetTexts();
    void DoIt();
    void Cancel();

private:
};
