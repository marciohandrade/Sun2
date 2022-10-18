#pragma once

#define MAX_SKILL_RESULT_MESSAGE_LINE 15


class GameMessage
{
    int     m_iMessageDelay;
    int     m_iCenterMessageDelay;
    int     m_iMissionAreaMessageDelay;
    WzID    m_FontIdMessage;
    WzColor	m_TextColor;
    WzColor	m_CenterTextColor;
    WzColor	m_MissionTextColor;
    BOOL    m_bShowMessageBox;
    BOOL    m_bShowCenterMessageBox;
    BOOL    m_bDrawCenterMessageBox;
    int     m_iMessageLineCount;
    int	    m_iCenterMessageLineCount;

    BOOL    m_bShowSkillResultMessage;
    BOOL    m_bShowMissionAreaMessageBox;
    BYTE    m_byShowMissionAreaMessageBoxAlpha;
    TCHAR   m_szMessage[MAX_MESSAGE_LINE][MAX_MESSAGE_LENGTH];
    TCHAR   m_szCenterMessage[MAX_MESSAGE_LINE][MAX_MESSAGE_LENGTH];
    TCHAR   m_szMissionAreaMessage[MAX_MISSION_AREA_LINE][MAX_MESSAGE_LENGTH];


    struct SkillResultText
    {
        std::string m_Text;
        WzColor m_TextColor;
    };
    std::deque<SkillResultText> m_SkillResultMessage;

    BYTE    m_byShowMessageBoxAlpha;
    BYTE    m_byShowCenterMessageBoxAlpha;

    void _InitializeMessageBox();
    void _InitializeCenterMessageBox();
    void _InitializeMissionAreaMessageBox();	
    void _InitializeSkillResultMessage();


    void _RenderMessageBox(WzSimpleMesh* pMessageMesh);
    void _RenderCenterMessageBox(WzSimpleMesh* pMessageMesh);
    void _RenderMissionAreaMessageBox();
    void _RenderSkillResultMessage();
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
    void _RenderMondSchienAreaOwnerMessage();
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL


    int _ProcessWordWrapByLine( TCHAR (*szMessage)[MAX_MESSAGE_LENGTH], LPCTSTR szText, int MaxLine );

public:
    GameMessage();
    virtual ~GameMessage();

public:
	void ShowMessageBox( LPCTSTR szMessage) { ShowMessageBox( szMessage, 1000, NULL, 0 ); }
    void ShowMessageBox( LPCTSTR szMessage,int iDelay, WzColor* pTextColor, WzID FontID);
    void ShowCenterMessageBox( LPCTSTR szMessage,int iDelay, WzColor* pTextColor, BOOL bDrawBox );
    void ShowMissionAreaInfo( LPCTSTR szMessage,int iDelay, WzColor* pTextColor );
    void AddSkillResultMessage( LPCTSTR szMessage, WzColor MessageColor );

    void ToggleSkillResultMessage();



    void HideMessageBox();
    void HideCenterMessageBox();

protected:
    void Init();
    void Process(DWORD dwTick);
    void RenderMessage(WzSimpleMesh* pMessageMesh);

};
