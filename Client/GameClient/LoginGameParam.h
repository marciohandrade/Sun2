#pragma once

//////////////////////////////////////////////////////////////////////////
// GameParameter 들...


#define LOGINPARAM CLoginGameParam::Instance()
class CLoginGameParam : public CGameParameter, public util::Singleton<CLoginGameParam>
{
private:
    util::_tstring	m_LoginServerIP;
    int				m_iLoginServerPort;
    int				m_iLoginServerType;

    util::_tstring  m_ServiceLoginServerIP_Head;
    util::_tstring  m_ServiceLoginServerIP_Tail;
    int             m_iServiceLoginServerPort;
    int             m_iServiceLoginServerNum;
    int             m_iServiceLoginTryCounts;
    util::_tstring  m_SelectedServiceLoginServerIP;
    int             m_SelectedServiceLoginServerPort;

	BOOL			m_bLiveTestServer;
    BOOL            m_buseLuancher;
    //////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//로그인씬 카메라 방향
	//Looking
	float look_x_;
	float look_y_;
	float look_z_;

	float eye_x_;
	float eye_y_;
	float eye_z_;

	//해당 옵션을 사용함?
	BOOL  use_user_setting_camera_;

public:
    CLoginGameParam();

    inline 			TCHAR *	GetLoginServerIP() 		{ return (TCHAR *)m_LoginServerIP.c_str(); }
    inline 			void    SetLoginServerIP(const TCHAR* lsip) { m_LoginServerIP = lsip; }
    inline			int		GetLoginServerPort()	{ return m_iLoginServerPort; }
    inline			int		GetLoginServerType()	{ return m_iLoginServerType; }

    inline          TCHAR * GetServiceLoginServerIP_Head() { return (TCHAR *)m_ServiceLoginServerIP_Head.c_str(); }
    inline          TCHAR * GetServiceLoginServerIP_Tail() { return (TCHAR *)m_ServiceLoginServerIP_Tail.c_str(); }
    inline          int     GetServiceLoginServerPort() { return m_iServiceLoginServerPort; }
    inline          int     GetServiceLoginServerNum() { return m_iServiceLoginServerNum; }
    inline          int     GetServiceLoginTryCounts() { return m_iServiceLoginTryCounts; }

    inline          TCHAR * GetSelectedServiceLoginServerIP() { return (TCHAR *)m_SelectedServiceLoginServerIP.c_str(); }
    inline          int     GetSelectedServiceLoginServerPort() { return m_SelectedServiceLoginServerPort; }
    inline          void    SetSelectedServiceLoginServerIP(const TCHAR* lsip) { m_SelectedServiceLoginServerIP = lsip; }
    inline          void    SetSelectedServiceLoginPort(int port) { m_SelectedServiceLoginServerPort = port; }

	inline			BOOL	IsLiveTestServer()	{ return m_bLiveTestServer; }
					void	Load( void );

    inline          void    SetUsingLuancher(BOOL isLuancher) { m_buseLuancher = isLuancher; }
    inline          BOOL    GetUsingLuancher() { return m_buseLuancher; }
	inline			float   GetLogineCameraLookX() { return look_x_; }
	inline			float   GetLogineCameraLookY() { return look_y_; }
	inline			float   GetLogineCameraLookZ() { return look_z_; }

	
	inline			float   GetLogineCameraEyeX() { return eye_x_; }
	inline			float   GetLogineCameraEyeY() { return eye_y_; }
	inline			float   GetLogineCameraEyeZ() { return eye_z_; }

	inline			BOOL    GetUserCamera() { return use_user_setting_camera_; }

	
};