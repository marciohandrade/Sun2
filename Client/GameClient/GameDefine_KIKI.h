//////////////////////////////////////////////////////////////////////////
//
// ������ �۾� ������
//
//////////////////////////////////////////////////////////////////////////

///////////////// [�׽�Ʈ �Ϸ� �� ����] /////////////////
//#define _KIKI_MODIFY_TOOLTIP					// 2006. 02. 20 / ������ / ���� ����
//#define _KIKI_CHAR_POPUP_MENU					// 2006. 03. 07 / ������ / ĳ���� �˾� �޴�
//#define _KIKI_HELP_MENU						// 2006. 03. 13 / ������ / ���� �޴�
//#define _KIKI_UI_ADD_SOUND_OPTION				// 2006. 03. 28 / ������ / ���� �ɼ�â �߰�
//#define _KIKI_UI_MODIFY_GRAPHIC_OPTION		// 2006. 03. 29 / ������ / �׷��ȿɼ� ����
//#define _KIKI_UI_MODIFY_GAME_OPTION			// 2006. 03. 29 / ������ / ���ӿɼ� ����
//#define _KIKI_UPDATE_WZSOUND_LIB				// 2006. 04. 08 / ������ / wzsound lib �� ���� �߰��� ��� ����
//#define _KIKI_MODIFY_HELP_INFO				// 2006. 04. 17 / ������ / ���� ��ũ��Ʈ ����
//#define _KIKI_ADD_PARTY_MEMBER_STATUS			// 2006. 04. 15 / ������ / ��Ƽ�� ���� ���õ� �޽��� �޾ƿ� ó�� (����,ü��,����)
//#define _KIKI_ADD_TOOLTIP_LIMITVIT			// 2006. 04. 04 / ������ / ������ ü�� ���� �߰�	
//#define _KIKI_ADD_TOOLTIP_LIMITSPR			// 2006. 04. 04 / ������ / ������ ���ŷ� ���� �߰�
//#define _KIKI_ADD_TOOLTIP_NEXT_GRADE_LIMIT	// 2006. 04. 07 / ������ / ��æƮ,��ũ�� �� ������� ���� ǥ��

//#define _KIKI_CHANGE_LOADING_IMAGE_FORMAT		// 2006. 04. 14 / ������ / �ε� �̹��� ���� ����
//#define _KIKI_APPLY_HELP_OPTION				// 2006. 04. 13 / ������ / ä��â ���� ���� �� ���� �ɼ� ����
//#define _KIKI_VOICE_CHAT_GET_ID				// 2006. 04. 06 / ������ / ��Ƽ������ ���� �߰�
//#define _KIKI_ADD_MIC_STATUS					// 2006. 04. 24 / ������ / ����ũ ���� on/off
//#define _KIKI_PARTY_VOICE_CHAT_MIC_VOL_OFF	// 2006. 04. 28 / ������ / ��Ƽ����ä�� �����Ҷ� ����ũ ���� 0���� ����
//#define _KIKI_MODIFY_CHAT_MIC_BTN				// ä��â ����ũ ��ư -> üũ��ư���� ����
//#define _KIKI_UI_MODIFY_CHAT_DIALOG			// 2006. 03. 31 / ������ / ä��â UI ����
//#define _KIKI_MODIFY_CHAT_MENU				// ä�� �޴��� ��ġ��, �ӼӸ� GUI �߰�
//#define _KIKI_MODIFY_HP_MIC_BTN				// hp ������ â�� ����ũ üũ��ư �߰�
//#define _KIKI_CHECK_READ_HELP					// 2006.04.17 / ������ / ���� �о����� ����

//#define _KIKI_NOPTION_TOOLTIP					// 2006. 03. 20 / ������ / ������ ����Ʈ�� ���� �ɼ� ���� �߰�
//#define _KIKI_BUG_FIX_TARGET_HP				// 2006. 04. 22	/ ������ / Ÿ��â ������ ���� ��Ƽ���϶� HP�������ֱ�
//#define _KIKI_MODIFY_BATTLE_ROOM_PASSWORD		// 2006. 04. 25 / ������ / ��й� ���� �� ���� ���� �۾�
//#define _KIKI_NEW_WZSOUND						// 2006. 05. 16 // ���� ���̺귯�� ���� 
//#define _KIKI_MODIFY_TRADE					// 2006. 05. 03 / ������ / �ŷ� ����
//#define _KIKI_DEL_NORMAL_VOICE_CHAT			// 2006. 04. 27 / ������ / 1:1 ���� ä�� ����
//#define _KIKI_UI_EVENT_DIALOG					// 2006. 04. 11 / ������ / �̺�Ʈ ���̾�α� �۾�

//#define _KIKI_UI_VOICE_CHAT					// 2006. 03. 20 / ������ / ����ä�� ��Ʈ�� ������
//#ifdef _KIKI_UI_VOICE_CHAT
//	#define _KIKI_VOICE_CHAT					// 2006. 02. 17 / ������ / ����ä�� ��� �߰�
//#endif
//#define _KIKI_VOICE_CHAT_RUN_MODIFY			// 2006. 04. 13 / ������ / WzVoiceClient ���� ���� ����
//#define _KIKI_MODIFY_VOICE_CHAT				// ����ä�� ��� ����
//#define _KIKI_UI_ANNOUNCE_WINDOW				// 2006. 05. 19 / ȭ�� �ϴ� �˸�â

//#define _KIKI_KEY_BLOCK						// 2006. 04. 19 / ������ / Alt+F4 ���� (������ Ű ó���� ���߿�...)
//#define _KIKI_ADD_OPTION_LIST					// 2006. 06. 14 / �ɼ� ����Ʈ ��ũ��Ʈ�� �о�ͼ� ó�� ( OptionList.txt )
//#define _KIKI_APPLY_TRADE_SELL_TYPE			// 2006. 04. 14 / ������ / �ŷ� ���ɿ��� üũ�� �޽��� �����ֱ�
//#define _KIKI_ADD_CHUNTING_MODE				// 2006. 04. 29 / ������ / �������� ��� ���� �߰� (Ÿ�Ӿ���,������ġ)
//#define _KIKI_UI_CHUNTING_WIN_DIALOG			// 2006. 05. 01 / ������ / �������� �¸� ������ �߰�
//#define _KIKI_UI_CHUNTING_DOING				// 2006. 05. 02 / ������ / ų����Ʈ ������ �� �߰�
//#define _KIKI_SORT_MAP_LIST					// ��������, �̼� �� ����Ʈ ���巹�� ������ SORT
//#define _KIKI_CHANGE_SURVIVAL_DEAD_MSG		// 2006. 05. 18 // �����̹�  ��忡�� ����� ���ڸ� ��Ȱ ��ư ����
//#define _KIKI_TIMEATTACK_OTHER_TEAM_COLOR		// 2006. 05. 19 / Ÿ�Ӿ��ø�忡�� �ٸ� ��Ƽ ���� �����Ұ�

//#define _KIKI_MODIFY_FRIEND					// 2006. 07. 06 / ģ����ȭ �����۾�
//#define _KIKI_ADD_FRIEND_INFO					// 2006. 08. 04 / ģ�� �������� ��� �߰�
//#define _KIKI_MODIFY_SKILL_DESC				// 2006. 06. 09 / ��ų ���� ����: ǥ�ù���� TYPE_PERCENTAGE �� ���, �Ҽ��� �ڸ����� ���
//#define _KIKI_ADD_KICK_GUILD_MEMBER			// 2006. 06. 14 / ���� ����
//#define _KIKI_ADD_GUILD_NOTICE				// 2006. 06. 20 / ��� ���� ��ɾ� �߰�: ��뿹) /������ ��������
//#define _KIKI_ADD_DEFAULT_BATTLEROOM_NAME		// 2006. 06. 21 / ��Ʋ�� �� ������ ���̸� ����Ʈ ����
//#define _KIKI_ADD_DBLCLICK_JOINROOM			// 2006. 06. 28 / ��Ʋ�� ����Ʈ ���� Ŭ���ϸ� �ٷ� ���� �� �� �ִ�.
//#define _KIKI_MODIFY_EVENT_MAN				// 2006. 08. 01 / �̺�Ʈ���� ��ư�� Show
//#define _KIKI_MODIFY_PARTY_MEMBER_KICK		// 2006. 09. 04 / ��Ƽ���� ��Ƽ�� ���� Ż�� ��� ���� (��Ƽâ x��Ʈ�� HIDE / ����� Ȯ��â ���)
//#define _KIKI_NOT_LEAVE_ON_READY_BUTTON		// 2006. 09. 07	/ �����߿� �泪���� ��ư HIDE
//#define _KIKI_DISPLAY_CHUNTING_TYPE			// 2006. 09. 13	/ ��Ʋ�� ����Ʈâ�� �������� �� Ÿ�� ǥ��
//#define _KIKI_BUG_FIX_HIDE_CHATDIALOG			// 2006. 09. 25 / ��Ʋ�κ񿡼� �ý��� ���̾�α� ��� ���Ŀ� ä�� ���̾�α� ������� ���� ����
//#define _KIKI_ADD_WAYPOINT					// 2006. 06. 26 / ��������Ʈ �߰�
//#define _KIKI_ADD_MISSION_RANK				// 2006. 08. 14 / �̼� ��ŷ
//#define _KIKI_READ_PACKET_BEFORE_CLOSE_SOCKET	// 2007.02.02	/ ������ ������ ����������, �ٷ� ������ ���� �ʰ� ���ۿ� �����ִ� ��Ŷ�� �� ó���� �� �����ֵ��� ó��
//#define _KIKI_MODIFY_PVP_CONFIG				// 2006. 10. 20 / SubBattleInfoDialog -> uiBattleZoneReadyPVPConfig �� ����
//#define _KIKI_MODIFY_BATTLE_ZONE				// 2006. 10. 20 / ��Ʋ�� �����丵
//#define _KIKI_SOUND_BUG_FIX_20345				// 2007.10.10 / 20345, 20349 ���� ���� / NPC Ŭ�� �Ͽ� â ����, ESC�� â �ݱ⸦ ������ ������ �ݺ��ϸ�
                                                // NPC ������ ���� �ݺ� �Ǵ� ���� ����

//#define _KIKI_MODIFY_EXE_CONFIG					// 2007.12.27 / �������� ȯ�� ���� (PACK ������ UNPACK������ ȣȯ) : System.wpk ������ ������ Login.ini�� �о�ͼ� �α���
//																	 General.ini�� USE_PACK_FILE �� 1�̸� Resource.wpk, Sound.wpk �ε�
//																	 General.ini�� USE_PACK_FILE �� 0�̸� Unpack ���ҽ��� �ε�

//#define _KIKI_DELETE_NOT_USE_PARAMETER			// 2007.12.28 / General.ini ���� ������� �ʴ� �Ķ���� ����

//#define _KIKI_MODIFY_MISSION_MSG				// 2006. 09. 14 / �̼� ���� (�̼ǿ��� ���Ǵ� �ý��� �޽����� Ȯ���Ͽ� �������� �κ� ��ȭ)
												// Message.txt ��ũ��Ʈ ����(�޽��� Ÿ�� �и� �� �̼����� ������ ���� UI �߰�)

//#define _KIKI_MOD_SKILLTEXT						// 2007. 01. 25 / ��ų �ؽ�Ʈ ����
//#define _KIKI_LOADING_TIME_TEST					// 2007.03.26	/ ���ӽ���� Ŀ�ǵ�ɼ����� -loading �� ���̸�, log\LoadingTime.log ���Ͽ� �ε��ð� �α׸� ����.

//#define _KIKI_NEW_VOICE_CHAT					// 2006. 07. 26	/ ����ä�� �� ���� ��ü
//#define _KIKI_MOD_VOICE_CHAT_MIC				// 2006. 09. 26 / ����ä�� ����ũ ��� ��Ȱ��ȭ ������ ���, �ٸ� ����� ��Ҹ��� ���� ���ϰ� ����


//#define _KIKI_MOD_STREAM_SIZE_FOR_SERIALIZE_CLIENT_LOAD		// 2008.04.29	/ ������ / eSLOT_SERIALIZE_CLIENT_LOAD �� ��Ʈ�� ����� 0�� ���(�κ��� �ƹ��͵� ������) ��������
															// ������ ���� ���������� MAX_INVENTORY_SLOT_NUM+MAX_TEMPINVENTORY_SLOT_NUM �� �̿��� ������ ����ϵ��� ����


//#define _KIKI_REFACTORING_DEBUG_COMMAND_MANAGER_PARSE_COMMAND		// 2009.04.29~2009.04.30   / DebugCommandManager::ParseCommand �����丵
//#define _KIKI_OUTPUTDBGSTR_PACKET_TRAFFIC		// 2009.3.23	/  ��Ŷ ���۷� ����� �޽��� ��� �ɼ� �߰�
///////////////// [�Ϸ�] //////////////////////////////////

#if defined(_DEBUG) || defined(_GAMECLIENT_VER) 
    #define _KIKI_AUTOTEST
#endif


//#define _NA_20120109_KIKI_ENABLE_MULTI_CLIENT   // 2012.01.09 / ������ / ��Ƽ Ŭ���̾�Ʈ ��� 
                                                // ����� GENERAL.INI�� SPECIAL_MODE�� eSPECIAL_MODE_NONE �� ��, ��Ƽ Ŭ���̾�Ʈ�� ���ǰ�,(���� ���� ����. Sungame.exe, Sungame_Dump.exe)
                                                // SPECIAL_MODE�� 1���� ũ�ų� ������, ��Ƽ Ŭ���̾�Ʈ�� ���ǰ� �ִ�.(���� ����. GameClient.exe)
                                                //
                                                // SPECIAL_MODE ���� ���� �׽�Ʈ�� ���ؼ��� 1���� ū ���� �ο��ϹǷ�,
                                                // SPECIAL_MODE ���� ������� ��Ƽ Ŭ���̾�Ʈ ����� �����ϵ���  �ϴ� �������� �߰�����.


///////////////// [�����丵] //////////////////////////////////
#define _KIKI_DELETE_PROJECTILEID_POWER_WAVE_   // 2010.02.01 / ������ / PROJECTILEID_POWER_WAVE ���� �ڵ� ���� (������� ����)


///////////////// [������] //////////////////////////////////



///////////////// [�׽�Ʈ ��] //////////////////////////////////


///////////////// [������ ������] //////////////////////////////////




///////////////// [GLog - �������] //////////////////////////////////
//#define _KIKI_ADD_G_LOG_MENU					// 2006. 04. 13 / ������ / ĳ�� �˾� â�� G-�α� �޴� �߰�
//----------------------------------
//07.04.09 / ������ / �ּ�ó��.
//#define _KIKI_UI_WEB_DIALOG						// 2006. 04. 13 / ������ / �� ���̾�α� UI �߰�
//----------------------------------
//#define _KIKI_WEB_AUTH							// 2006. 04. 19 / ������ / �α��ν� �� ���� ��û            
//#define _KIKI_G_LOG_PROTOCOL					// 2006. 04. 19 / ������ / G-LOG�� ���̵� ������
