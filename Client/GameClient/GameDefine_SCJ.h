#pragma once
//////////////////////////////////////////////////////////////////////////
//		������ �۾� ������ SCJ, naklu
//////////////////////////////////////////////////////////////////////////
//------------------------------- 2015�� -------------------------------//
//#define _NA_008757_20160329_DOMINATION_GRAPHIC_CONTROL            // 2016.03.29 / ������ / ������ �׷��� �������� ��� ����
//#define _NA_000000_20160315_ETHERWEAPON_SPECIALMOVE               // 2016.03.15 / ������ / ��Ű�� ���׸������� ����ȹ��� �߰�
//#define _NA_008721_20160308_AUTOMOVE_TO_PORTAL_BUG_FIX            // 2016.03.08 / ������ / �ڵ��̵����� ��Ż�� �� �� �÷��̾� ��ũ �ȸ´� ���� ����
//#define _NA_000000_20151125_MOUSECLICK_AREA_SELECT_CANCLE         // 2015.11.25 / ������ / ���콺 Ŭ������ ��ųť���� Ǯ���� �ʰ� ����
//#define _NA_008546_20151029_SITTING_PACKET_DELAY_POSITION_SYNC_BUG// 2015.10.29 / ������ / �ɱ� ��Ŷ SYN�� ACK������ �����̵��� �����̸鼭 ��ġ �ȸ´� ���� ����
//#define _NA_000000_20151015_CALC_PORTAL_OFFSET_PROBLEM_MODIFY     // 2015.10.15 / ������ / ��Ż�� ��� ��ġ ���� �� ���ѷ��� ���� ����
//#define _NA_000000_20150714_MOVEMENT_DISTANCE_CHECK               // 2015.07.14 / ����� / ���ǵ��� üũ. �̵� ���� �Ÿ� üũ
//#define _NA_008395_20150710_EVENT_CRYSTAL_UI_TEXT_CONTROL         // 2015.07.10 / ������ / ������� ui�� �ؽ�Ʈ �� ���� ���� ��ũ��Ʈ���� �о�ͼ� ���
//#define _NA_008350_20150617_SPA_NEW_STYLE_MODE_ANIMATION          // 2015.06.17 / ������ / ��õ �ű� ��Ÿ�� ��忡 ���� Ư�� �ִϸ��̼� �۾�
//#define _NA_000000_20150616_SPA_COSTUME_SHOW_HAIR                 // 2015.06.16 / ������ / ��õ�� �Ӹ� ���� �� �ֵ��� �÷��׷� �����ϵ��� ����
//#define _NA_000000_20150609_CHARACTER_ROTATE_RUNNING_BUG_FIX      // 2015.06.09 / ������ / ȸ�� �̵� �� ������ ���� ����
//#define _NA_000000_20150604_BUG_FIND_TEST                         // 2015.06.04 / ������ / ���� ã��� �׽�Ʈ �ڵ� (CTRL+F10 ���� ����Ʈ �׺���̼� ��ġ �ʱ�ȭ)
//#define _NA_008302_20150601_QUEST_COMPLET_SYN_RECAST              // 2015.06.01 / ������ / ����Ʈ �Ϸ� ���� �� ���û ��� �߰�
//#define _NA_008316_20150604_DEV_VER_SHOW_DAMAGE_INFO                  // 2015.06.04 / ������ / ���߸�忡�� ������ ���ط��� ǥ�����ִ� �۾�
//#define _NA_008298_20150519_ALLOW_HEROMOVE_TO_DOWN_STATE          // 2015.05.19 / ������ / �̵��߿� �ٿ��� �Ȱɸ��� ���� ����(��������ҽð�4�ʿ� ���ؼ� �߻�)
//#define _NA_008296_20150518_HELLOID_SPIN_CANCLE_BUG_FIX           // 2015.05.18 / ������ / ����̵� ����ĵ�� ���� ����
//#define _NA_000000_20150428_ATTENDANCE_CHECK_SYSTEM_SUPERPOSITION_TYPE_MODIFY  // 2015.04.28 / ������ / �⼮üũ �ý��� ������ ���Ĺ�� ���� (�޷����� �����ϰ�)
//#define _NA_008252_20150427_AURASKILL_PACKET_PENDING              // 2015.04.27 / ������ / ������ų ��Ŷ �ߺ�ó�� ����
//#define _NA_008235_BOSS_MONSTER_CRITICAL_DAMAGE_ANIMATION_IGNORE  // 2015.04.23 / ������ / ����(ĸƾ)���� ũ��Ƽ�� Ÿ�� �ִϸ��̼� ����
//#define _NA_008226_AC_PARENTMAP_LIST_SKIP                         // 2015.04.15 / ������ / AC ������� ����Ʈ ����� ����� �� �����ϱ� ��� �߰�
//#define _NA_008196_20150407_GM_REMOVE_STATUS                      // 2015.04.07 / ������ / ���� �Ұ����� �������� ���ִ� GM���Ǳ�� �߰�
//#define _NA_008185_20150402_COSTUME_ITEM_FOR_NPC                  // 2015.04.02 / ������ / �ڽ�Ƭ NPC������
//#define _NA_008139_20150318_MODIFY_BUFF_ICON_UI                   // 2015.02.10 / ������ / ����������(Status) UI ����
//#define _NA_008069_20150210_ADD_SPA_SYSTEM                        // 2015.02.10 / �ΰ��, ������ / ��õ�ý��� �߰�
//#define _NA_008090_20150226_GAMEOPTION_RECVPACKET_DONT_WAIT       // 2015.02.26 / ������ / �������� �� ���������� ����Ϸ� ��Ŷ�� ��ٸ��� �ʰ� �ٷ� ����
//#define _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM               // 2015.01.13 / �ӻ�� / �⼮ �ý��� �߰�
//#define _NA_007942_20150115_ADD_MAINMENU_DIALOG_ICON              // 2015.01.15 / ������ / ���θ޴� ������� ��ư �߰�
//#define _NA_000000_20150114_MODIFY_MISSION_REWARD_DIALOG_SLOT     // 2015.01.14 / ������ / �̼Ǻ���â ���� ���,���� ����
//#define _NA_007942_20150113_MODIFY_RUNE_STONE_WAYPOINT_MAXCOUNT   // 2015.01.13 / ������ / ���� ��������Ʈ �ִ� ���� ���� ���� (10->15)
//------------------------------- 2014�� -------------------------------//
//#define _NA_007865_20141403_LIGHTMAP_BUILD_EXCLUDE_OBJECTS        // 2014.12.03 / ������ / ����Ʈ�� ���� �� ������Ʈ ���� ���� & ��� ��Ȱ
//#define _NA_007732_20141024_RIDER_MOVESPEED_SYNC                  // 2014.10.24 / ������ / �ٸ��÷��̾��� Ż�� �̼� ��ũ ����
//#define _NA_007726_20141015_PROTECTION_SCREENSHOT                 // 2014.10.15 / ������ / ��ũ���� ��ȣȭ(����) ���
//#define _SCJ_SHOW_NPC_ID                                          // 2014.07.23 / ������ / NPC ID �����ֱ�
//#define _SCJ_TOGGLE_EDITBOX_CONTROL                               // 2014.07.22 / ������ / ��� ����Ʈ�ڽ� ��Ʈ�� �߰�
//#define _NA_000000_20140520_MAPLOAD_FAIL_TO_FTP                   // 2014.05.20 / ������ / �ʷε� ���� �� FTP������ �뺸
//#define _NA_007192_20140428_CLIENT_LUA_MACRO_SYSTEM               // 2014.04.28 / ������ / Ŭ���̾�Ʈ ��� ��ũ�� �ý���
//#define _NA_000000_20140423_SUMMON_BACKGROUND_PICKING             // 2014.04.23 / ������ / ��ȯ�� �ļ� ��ŷ
//#define _NA_000000_20140409_ADD_SHADOW_AREA_SKILL_POINTER         // 2014.04.08 / ������ / ������ ���� ������ �߰�
//#define _NA_000000_20140408_SkILL_USE_AFTER_AUTOATTACK_OPTION     // 2014.04.08 / ������ / ��ų ��� �� �ڵ����� ��ȯ ���� �ɼ�ȭ
//#define _NA_007113_20140324_SHADOW_HPSUCTION_RENEWAL              // 2014.03.24 / ������ / ������ ��ų �� ���������� ����ȭ (������)
//#define _NA_007113_20140324_SHADOW_SKILL_BALLANCE_RENEWAL         // 2014.03.24 / ������ / ������ ��ų �뷱�� ������
//#define _NA_007086_20140318_MONSTERBOOK                           // 2014.03.18 / ������,������ / ���͵���
//#define _NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX    // 2014.02.10 / ������ / ���� �ٸ� ����Ʈ â�� ������ ���� ����
//#define _NA_000000_20140120_SMART_NPC_SYSTEM
//#define _NA_007029_20140120_DOMINATION_RESURRECTION_REFORM        // 2014.01.20 / ������ / ������ ��Ȱ ��� ���� (������ �˾�â)
//#define _NA_007029_20140114_CHAOS_ZONE_ENTER_BLIND                // 2014.01.14 / ������ / ī������ ���� Ȱ��ȭ/��Ȱ��ȭ �߰�
//------------------------------- 2013�� -------------------------------//
//#define _NA_006953_20131114_DOMINATION_MANAGER_UI_RENEWAL         // 2013.11.14 / ������ / ������ �������� ������
//#define _NA_000000_20130903_CHANNEL_GAGE_INCREASE                 // 2013.09.13 / ������ / ä�� ȥ�� �������� ���ƺ��̰� ����
//#define _NA_000000_20130828_GUILDMARK_SIZE_MODIFY                 // 2013.08.28 / ������ / ��帶ũ ũ�� ����
//#define _TEMP_0068_20130820_DOMINATION_MANAGE_PACKET_CONTROL // �ӽ÷� �������� �ӽ���Ŷ ���� ���ø���Ʈ�� ���������� ������
//#define _NA_006826_20130722_DOMINATION_RENEWAL                    // 2013.07.22 / ������, ������ / ���Ǽ��� ������ ����
//#define _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM             // 2013.07.03 / ������, ������ / ���� ��ǥ �ý��� �߰�
//#define _NA_006744_20130531_COLLECT_NAK_TO_REMOVE_COLLECT         // 2013.05.31 / naklu / ä�� ���� ��, �����ڵ忡 ���� ä���� ��Ȱ��ȭ ��Ŵ
//#define _NA_006658_20130422_ADD_ACTION_TRIGGER_CHANGE_OBJECT_HP   // 2013.04.22 / naklu / Ʈ���� �߰� - ������Ʈ HP ����
//#define _NA_006619_20130422_ADD_ACTION_TRIGGER_CHANGE_RADAR_MAP   // 2013.04.22 / naklu / Ʈ���� �߰� - ���̴���(�̴ϸ�) ����
//#define _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO        // 2013.04.15 / naklu / ���� ǥ�� (Ÿ��â ui�� �÷��̾��� ��� ���� ǥ��)

//------------------------------- 2012�� -------------------------------//
//#define _NA_000000_20121113_DRAW_FTEXT_CLIPPING_TEXT        // 2012.11.13 / naklu / FTEXT ��� �� ���� �߸��� ���� �׸���
//#define _NA_006222_20121031_EVENT_WINDOW_DISTRIBUTE_ENABLE_TEXT   // 2012.10.31 / naklu / uiEventWindow �� �̺�Ʈ ���� ���� �ƴ� �� ��縦 ����
//#define _NA_006221_20121031_SPRITSLUG_SKILL_MODIFY          // 2012.10.31 / naklu / �巡�ﳪ��Ʈ ���Ǹ������� ��ų�� �ߵ������� ������ ��ũ�극��ũ�� �����ϰ� ����
//#define _NA_000000_121015_LEAVE_CANCEL_TO_REVIVE_RE_CASTING // 2012.10.15 / naklu / ������ ĳ���� ��� �� �ٽ� ��Ȱ ĳ�����ϱ�
//#define _NA_004923_120618_SKILL_QUEUE_MOVE_CANCEL           // 2012.06.18 / naklu / �̵� �� ��ų ť�� ���� (Ȱ��ȭ�� ������ ����)
//#define _NA_004921_20120628_ADD_QUEST_NPC_EMOTICON_FOR_QUEST_TYPE // ����Ʈ Ÿ��(����,���,�ݺ�)�� ���� NPC�̸�Ƽ��(�Ӹ��� �η縶��) �߰�
//#define _NA_004756_120504_SHOW_SSQ_PLAYER_NAME              // 2012.05.04 / naklu / Ÿ���� ������� �÷��̾� �̸� ���̱� GM��ɾ� �߰�
//#define _SCJ_120104_SORT_QUEST_VIEW_PROGRESS                // 2012.01.04 / naklu / ����Ʈ �̸����� �����ϱ�

//------------------------------- 2011�� -------------------------------//
//#define _SCJ_111202_UI_RENEWAL                              // 2011.12.02 / naklu / UI ������ �۾� (����Ʈ, ac, ĳ����, ĳ�ü�â)
//#define _SCJ_CHECK_SEND_GAMEOPTION_AUTOHUNTING_PACKET       // 2011.11.10 / naklu / ���뺯�� ���� ������ �ڵ���� �ɼ� ����    
//#define _SCJ_ADD_FORMAT_TEXT_PARAM_QUEST_START_NPC          // 2011.11.04 / naklu / FTEXT Ÿ�� �߰� - ����Ʈ����(�Ϸ�) NPC�̸�
//#define _SCJ_ACSYSTEM_OVERLAP_PARENT                        // 2011.10.05 / naklu / AC���� ����� ��������� ��ġ��
//#define _SCJ_SHOW_ONES_SPECIALACCESSORY_APPEARANCE          // 2011.09.26 / naklu / Ư�� �Ǽ����� ���� ���� (Ư�� ������ ���� ����)
//#define _SCJ_EXP_EVENT_ENABLE_EFFECT                        // 2011.09.23 / naklu / ����ġ �̺�Ʈ �˸��� ���
//#define _NA_003033_20110831_DICECAST_ITEM_ROUTING           // 2011.08.31 / ������, ������ / �ֻ��� �ý���
//#define _SCJ_MAKE_DOUBLE_QUOTATION_MARKS                    // 2011.08.22 / naklu / �޼����� ����ǥ�� ����ǥ �ϳ� �� ���̱�
//#define _SCJ_REMOVE_GENDER_CREATE_SCEANE                    // 2011.06.29 / naklu / �Ϻ� 1102�� ���� �߰� �� ĳ���� ���� ����
//#define _NA_002935_20110704_ITEM_SCORE                      // 2011.07.04 / ������, ������ / ��� �� ���� ǥ��
//#define _SCJ_MODIFY_OPTIONLIST_PARSER_VALUETYPE_FILED_TWO   // 2011.06.22 / naklu / OptionList.txt�� ValueType2 �ʵ尡 �߰��ǰ� ���� 1�� �ɼ��� ������ �̷ο� �ɼ�
//#define _SCJ_ADD_SNS_SHORTCUT                               // 2011.06.22 / naklu / SNS �ٷΰ��� ��ư �߰�
//#define _SCJ_REMOVE_KEY_HELP_WINDOW                         // 2011.06.20 / naklu / ToTalGuide ������ ����ϴ� Ű ���� â�� ������ 
//#define _SCJ_REMOVE_ANNOUNCE_WINDOW                         // 2011.06.20 / naklu / (������ ���� ����) ToTalGuide�� �߰��Ǹ鼭 ������ ����ϴ� �̱״Ͻ� ���� â�� ������
//#define _SCJ_CHANGE_SKILL_COOLTIME_COLOR                    // 2011.06.07 / naklu / ��ų ��Ÿ�� ���� ����
//#deinfe _SCJ_MODIFY_OPTIONLIST_PARSER_SUFFIX_FILED          // 2011.05.23 / naklu / OptionList.txt�� RatioValue �ʵ带 ValueType�� ���� �۾� (���̻��� ���� ��� ����)
//#define _SCJ_SSQ_DIRECT_ENTER                               // 2011.05.20 / naklu / Ÿ���ѻ�� ����� ��� �� �ٷ� ��Ż ����
//#define _SCJ_ADD_TYPE_LAYER_FOR_ITEM                        // 2011.04.11 / naklu / ������ Ÿ�Կ� ���� ���̾� �߰�
//#define _SCJ_SETITEM_TOOLTIP_REFORM                         // 2011.04.05 / naklu / (������) ��Ʈ������ ���� ��� ����
//#define _SCJ_THREE_LEVELUP_PER_EFFECT                       // 2011.04.01 / naklu / 3���� ���� ������ ����Ʈ �߰�/����
//#define _SCJ_TEXT_COLOR_RENEWAL                             // 2011.03.07 / naklu / ä�� �ؽ�Ʈ �۾��� �׵θ� �Ӽ� �ֱ�
//#define _SCJ_GM_CMD_CREATE_ITEM_TO                          // 2011.03.02 / naklu / ������ ������ �������� �����ϵ��� �ϱ�
//#define _SCJ_SHOW_ITEM_ID                                   // 2011.02.01 / naklu / ������ ������ ������ ID �����ֱ�      
//#define _SCJ_PK_POPUP_DIALOG                                // 2011.01.04 / naklu / ���� ���� �� PK���� �ȳ� �˾�â �ٿ� 
//#define _SCJ_LONG_TEXT_REDUCING                             // 2011.01.06 / naklu / ������ �Ѿ�� �� ������ ���ڿ� ���̱�