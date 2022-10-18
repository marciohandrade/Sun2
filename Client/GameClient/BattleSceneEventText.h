#pragma once

class cSceneEventText
{
private:
    enum { MAX_EVENT_LINE = 4 };

    int     m_iEventTick;
    int     m_iEventLineTick;
    int     m_iEventPageTick;
    BOOL    m_bEventState;
    int     m_iEventCurScript;
    int     m_iEventCurPage;
    int     m_iEventMaxPage;
    int     m_iEventCurLine;
    int     m_iEventMaxLine;
    int     m_iEventAppearPane;
    int     m_iEventDisappearPane;
    int     m_iEventVoiceHandle;
    TCHAR   m_szEventLine[MAX_EVENT_LINE][MAX_MESSAGE_LENGTH];

public:
    cSceneEventText();

    void Init();

    void SetEventScript(int iScript);
    void ProcessEventScript(DWORD dwTick);
    void RenderEventScript();

    void SkipEventScript();
    bool IsActived() { return m_iEventCurScript != 0; }

private:
    void SetEventPage(int iPage);	
};
