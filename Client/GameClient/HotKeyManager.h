#ifndef __HOT_KEY_MANAGER_H__
#define __HOT_KEY_MANAGER_H__

//=============================================================================================================================
///  ����Ű ó�� Ŭ���� 
/**
	@author	Kim Min Wook < taiyo@webzen.co.kr >
	@since	2004. 1. 14
	@remarks
		- ����Ű, HotKeyó���� ���
		- eOPE_TYPE�� �߰��� �ٸ� ������ execute��� ����
		- Unregister �߰� - �翵
*/
//=============================================================================================================================

#pragma once

#include <Singleton.h>
#include "HotKeyDefine.h"

class SolarDialog;

class KeyCommand;

class HotKeyManager : public util::Singleton<HotKeyManager>
{
    typedef STLX_MAP<HOT_KEY_FUNCTION_INDEX, KeyCommand*> KEY_COMMAND_MAP;

public:
    HotKeyManager();
    ~HotKeyManager();

    void Initialize();  // �⺻�ʱ�ȭ/����ƮŰ����/

    void Process();
    void ProcessHotkeyFunction(HOT_KEY_FUNCTION_INDEX hot_key_function_index);
    
    // ++ Ű���� ���� ���� �Լ�
    KEY_BIND_PAIR* GetKeyBindPair(HOT_KEY_FUNCTION_INDEX hot_key_function_index);
    static bool CheckInputBindKey(KEY_BIND_PAIR& key_bind_pair, int key_state, BOOL force_input = FALSE);

    // ++ Ű���� ����/���� ���� �Լ�
    void StartUserSettingMode();
    void EndUserSettingMode(WORD input_key_code);
    WORD CheckUserKeyInput();

    KEY_BIND_INFO& current_using_key_bind_info() { return current_using_key_bind_info_;}
    KEY_BIND_INFO& account_key_bind_info() { return account_key_bind_info_; }
    KEY_BIND_INFO& defalut_key_bind_info() { return defalut_key_bind_info_;}
    KEY_BIND_INFO& temp_key_bind_info() { return temp_key_bind_info_;}

    // ++ ���������� ���� �Լ�
    bool CompareHotKeyInfo(KEY_BIND_INFO& left_Info, KEY_BIND_INFO& right_Info);
    bool MakeToStream(GameOption& game_option);
    bool MakeToHotKeyInfo(GameOption& game_option, KEY_BIND_INFO& apply_info);
    void SaveHotKeyInfoToServer(bool is_check_character);
    void LoadHotKeyInfoFromServer();
    void LoadHotKeyInfoFromServerByCharacter();
    void RecvHotKeyInfo(GameOption& game_option);
    void RecLoadNak(GameOption::Type option_type);
#ifdef _NA_008168_20150330_IMPROVED_REGISTRATION_OF_NEW_SHORTCUTS
	void InValidDefaultShortCutCheck(KEY_BIND_INFO& dest_key_bind, GameOption::Type  option_type); // ���� �߰��� Ű�� �ִ��� �˻�
#endif // _NA_008168_20150330_IMPROVED_REGISTRATION_OF_NEW_SHORTCUTS

    bool is_character_setting() const { return is_character_setting_; }
    void set_is_character_setting(bool val) { is_character_setting_ = val; }

    bool is_user_setting_mode() const { return is_user_setting_mode_; }

private:
    void BuildDefalutKeyBindInfo();
    void BuildKeyCommands();

    KeyCommand* GetKeyCommand(HOT_KEY_FUNCTION_INDEX hot_key_function_index);
    KEY_COMMAND_MAP& key_command_container() { return key_command_container_; }

    
    void set_is_user_setting_mode(bool val) { is_user_setting_mode_ = val; }

private:
    KEY_BIND_INFO defalut_key_bind_info_;
    KEY_BIND_INFO account_key_bind_info_;    
    KEY_BIND_INFO current_using_key_bind_info_;
    KEY_BIND_INFO temp_key_bind_info_;
    KEY_COMMAND_MAP key_command_container_;
    bool is_character_setting_;
    bool is_user_setting_mode_;
};


#else

enum eOPE_TYPE
{
	OPE_SHOW,			// SHOW <-> HIDE
	OPE_QUICK,			// ����â ����Ű
	OPE_NOACTION,
};

struct HOT_CODE
{
    BOOL		bUsedSys;
	int			iSysKey;
	int			iCmdKey;
	eOPE_TYPE	iOpeType;
};

class HotKeyManager : public util::Singleton<HotKeyManager>
{		
public:
	enum eHOTKEYCODE
	{
        eHOTKEY_SYSTEMDLG,
		eHOTKEY_INVENTORYDLG,	
		eHOTKEY_SKILLDLG,
        eHOTKEY_QUICKDLG,
		eHOTKEY_RADARDLG,
		eHOTKEY_CHARACTERSKILLDLG,
		eHOTKEY_FRIENDDLG,

		// ������� �ʴ� Ű
		eHOTKEY_DLG_MAX,
		eHOTKEY_MAX,
	};

public:
	HotKeyManager();
	~HotKeyManager();

	VOID			Initialize();
	VOID			RegisterHotKey( eHOTKEYCODE code, SolarDialog * pDlg );
	VOID			UnregisteredHotKey(eHOTKEYCODE code);
	VOID			Process();
	VOID			ExcuteHotKey( eHOTKEYCODE code, eOPE_TYPE opeType );

private:
	SolarDialog* m_pDlg[eHOTKEY_DLG_MAX];
	static HOT_CODE m_HotCode[eHOTKEY_MAX];
};
#endif // __HOT_KEY_MANAGER_H__