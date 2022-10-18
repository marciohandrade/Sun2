#include "SunClientPrecompiledHeader.h"
#pragma once
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

#include "uiBase.h"

class uiChaosZoneResurrection : public uiBase
{
    enum eDIALOG_POS
    {
        DLGPOS_CAPTION = 0,
        DLGPOS_TEXT,

        DIALOG_MAX,
    };

public:
    uiChaosZoneResurrection(InterfaceManager *ui_man);
    virtual ~uiChaosZoneResurrection();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual VOID Process(DWORD tick);
    virtual bool ShowInterface(BOOL val, BOOL showoff = FALSE);

    virtual void NetworkProc(MSG_BASE* /*pMsg*/) {};  // Pure
    virtual void MessageProc(SI_MESSAGE* pMessage);   // Pure

    void set_resurrection_tick(DWORD resurrection_tick);

private:
    void ResurrectionRefuse(); // 何劝 芭何


protected:
    virtual void OnShowWindow(BOOL val);
    virtual void OnRenderSolarDialog();
    virtual void OnUpdateSolarDialog();

protected:
    CControlWZ* getControl(POSTYPE pos);
    POSTYPE getCtrlIDToPos(WzID wzId);
    static WzID m_wzId[DIALOG_MAX];

private:

    DWORD current_tick_; // 泅犁 平
    DWORD resurrection_tick_; // 何劝 平
};

//------------------------------------------------------------------------------
/**
*/
inline POSTYPE uiChaosZoneResurrection::getCtrlIDToPos(WzID wzId)
{
    for (POSTYPE i = 0 ; i < DIALOG_MAX ; ++i)
    {
        if (m_wzId[i] == wzId)
        {
            return i;
        }
    }
    return DIALOG_MAX;
}

//------------------------------------------------------------------------------
/**
*/
inline CControlWZ* uiChaosZoneResurrection::getControl(POSTYPE pos)
{
    assert (pos < DIALOG_MAX);

    return GetControlWZ(m_wzId[pos]);
}

inline void uiChaosZoneResurrection::set_resurrection_tick(DWORD resurrection_tick)
{
    resurrection_tick_ = resurrection_tick + clock_function::GetTickCount();
}
//------------------------------------------------------------------------------

#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND