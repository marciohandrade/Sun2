#pragma once

const int	c_iMaxHelpIcon						= 3;
const char  c_strDlgFname_HelpMenu[]			= ("Data\\Interface\\49_helpbox.iwz");
const DWORD c_dwCompareID_HelpMenu				= StrToWzID("0490");

const char  c_strDlgFname_HelpWindow[]			= ("Data\\Interface\\45_9_Helpmessage.iwz");
const DWORD c_dwCompareID_HelpWindow			= StrToWzID("0459");

const char  c_strDlgFname_HelpAnnounce[]		= ("Data\\Interface\\43_Announce_Window.iwz");
const DWORD c_dwCompareID_HelpAnnounce			= StrToWzID("0430");

//-----------------------------------------------------------------------------
// ���� (���������� �߰��Ǵ°�, �ϴ� �˸�â�ΰ�)
//-----------------------------------------------------------------------------
enum HELP_METHOD
{
	eHM_ICON		= 0,
	eHM_ANNOUNCE,
};