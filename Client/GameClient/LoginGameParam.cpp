#include "SunClientPrecompiledHeader.h"
#include "logingameparam.h"
#include "GlobalFunc.h"

CLoginGameParam::CLoginGameParam()
{
}

void CLoginGameParam::Load( void )
{
	GlobalFunc::WriteLoadingTimeLog(_T("CLoginGameParam::Load Start"));
	DWORD dwStartTime = GlobalFunc::StartTimeCheck();

	// about login server
    m_LoginServerIP		= ReadString(_T("LOGIN_SERVER_IP"), _T("10.1.28.143"));
    m_iLoginServerPort	= ReadInt(_T("LOGIN_SERVER_PORT"), 10100);
    m_iLoginServerType	= ReadInt(_T("LOGIN_SERVER_TYPE"), 0);
    
    m_ServiceLoginServerIP_Head = ReadString(_T("SERVICE_LOGIN_SERVER_IP_HEAD"), _T("connect"));
    m_ServiceLoginServerIP_Tail = ReadString(_T("SERVICE_LOGIN_SERVER_IP_TAIL"), _T("sunonline.co.kr"));
    m_iServiceLoginServerPort   = ReadInt(_T("SERVICE_LOGIN_SERVER_PORT"), 44405);
    m_iServiceLoginServerNum    = ReadInt(_T("SERVICE_LOGIN_SERVER_NUM"), 2);
    m_iServiceLoginTryCounts    = ReadInt(_T("SERVICE_LOGIN_TRY_COUNTS"), 10);

	m_bLiveTestServer	= ReadInt(_T("LIVE_TEST_SERVER"), 0);

	look_x_ = ReadFloat(_T("LOOK_X"),50.0f);
	look_y_ = ReadFloat(_T("LOOK_Y"),50.0f);
	look_z_ = ReadFloat(_T("LOOK_Z"),50.0f);

	eye_x_ = ReadFloat(_T("EYE_X"),50.0f);
	eye_y_ = ReadFloat(_T("EYE_Y"),50.0f);
	eye_z_ = ReadFloat(_T("EYE_Z"),50.0f);

	use_user_setting_camera_ = ReadInt(_T("USING_CAMERA_SET"), 0);

	GlobalFunc::EndTimeCheck(dwStartTime, true, _T("CLoginGameParam::Load End"));

}