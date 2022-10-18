#include "SunClientPrecompiledHeader.h"
#include "uiCommunityMan.h"

#include "interfacemanager.h"

#include "uiCommunityDlg/uiCommunityDlg.h"
#include "uiCommunityInfo/uiCommunityInfo.h"
#include "uiCommunityReadMail/uiCommunityReadMail.h"
#include "uiCommunityWriteMail/uiCommunityWriteMail.h"

const char  c_strDlgFname_CommunityDlg[]           = ("Data\\Interface\\66_community_friend.iwz");
const DWORD c_dwCompareID_CommunityDlg             = StrToWzID("0662");

const char  c_strDlgFname_CommunityInfo[]           = ("Data\\Interface\\66_5_community_information.iwz");
const DWORD c_dwCompareID_CommunityInfo             = StrToWzID("0665");

const char  c_strDlgFname_CommunityReadMail[]       = ("Data\\Interface\\66_7_Message_read.iwz");
const DWORD c_dwCompareID_CommunityReadMail         = StrToWzID("0667");

const char  c_strDlgFname_CommunityWriteMail[]      = ("Data\\Interface\\66_6_Message_write.iwz");
const DWORD c_dwCompareID_CommunityWriteMail        = StrToWzID("0666");



void uiCommunityMan::_LoadUI()
{
	m_pUICommunityDlg = CreateUIDialog<uiCommunityDlg>( 
										c_dwCompareID_CommunityDlg, 
										c_strDlgFname_CommunityDlg, 
										this);
	m_pUICommunityInfo = CreateUIDialog<uiCommunityInfo>(
										c_dwCompareID_CommunityInfo,
										c_strDlgFname_CommunityInfo, 
										this);
	m_pUICommunityReadMail = CreateUIDialog<uiCommunityReadMail>( 
										c_dwCompareID_CommunityReadMail, 
										c_strDlgFname_CommunityReadMail, 			
										this);
	m_pUICommunityWriteMail = CreateUIDialog<uiCommunityWriteMail>( 
										c_dwCompareID_CommunityWriteMail, 
										c_strDlgFname_CommunityWriteMail, 			
										this);
}