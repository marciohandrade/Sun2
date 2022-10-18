//------------------------------------------------------------------------------
/**
Work Define ( �۾��� ������ )

@author
    ���翵 (y2jinc@webzen.co.kr)
@since

@remarks
    _MOD : Modify ����
    _ADD : �߰� ����
    _DEL : ���� �� ����.
*/

//////////////////////////////////////////////////////////////////////////
//���� ���� 
//#define _REMOVE_UI_SUSPEND_FOCUS		    // 2006. 5. 17 / ���翵 / ��Ŀ���� �ȳ�~~~
//
//#define _ADD_CHECK_OVERLAP_EXCUTE		    // 2006. 5. 19 / ���翵 / �ߺ� ���� üũ.
//#define _ADD_ATTACK_JUMP                    // 2006. 6. 26 / ���翵 / ���� ���� ( ���� )
//#define _TRY_SEND_LOGOUT_PACKET             // 2006. 8.  7 / ���翵 / ����� �α׾ƿ� ��Ŷ ����.
//#define _ADD_CANNOTMOVE_WHILE_SITTING       // 2006. 8. 25 / ���翵 / �ɱ� �����߿� �̵� �ȵǰ� ��.
//#define _DONT_CHECK_TRADE_ITEM				// 2006. 9. 13 / ���翵 / ������ �ŷ� Ÿ�Կ� ���� üũ Ŭ���̾�Ʈ�� ���ϰ� ��.(������ ������(��ο�븮��))
//#define _ADD_ITEM_WASTE_BASKET				// 2006. 9. 12 / ���翵 / �κ��丮�� ������ ������ ���� �۾�.
//
//#define _MOD_PREVIEW_ACCONQUEST_REWARD_INFO	// 2006. 9. 25 / ���翵 / ����� ������Ʈ �������� �̸����� ��� �ֱ�.
//#define _ADD_LUCKY_MONSTER_RUNAWAY			// 2006. 8. 30 / ���翵 / ��Ű ���Ͱ� Ư�� �ð����� ����ġ�� ������� ����
//#define _ADD_FIND_ROOM_BY_NUMBER			// 2007. 2. 12 / ���翵 / �� ��ȣ�� �� ã����.( ������ ������ �޾Ƴ��� ����Ʈ������ ã������ ������ ��Ŷ�� ������ ���� ã�� �� )
//#define _MOD_POPUP_DAMAGE_INFO				// 2006.11. 29 / ���翵 / ������ ǥ�� �κ� ����( ����, ũ��, ��� )
//
//#define _ADD_ELITE_ITEM_EQUIP_CAP		// 2007. 1.  5 / ���翵 / ����Ʈ ������ ��� ������ Ŀ���͸����� �۾�.
//#define _MOD_CREATE_MISSION_METHOD			// 2006.12. 21 / ���翵 / �̼Ǹʻ����� ���� �̼Ǿ��̵� �����ǰ�

//#define _ADD_ELITE_ITEM						// 2006. 9. 29 / ���翵 / ����Ʈ ������ �߰��� ���� �۾�.
//#define _ADD_UNIQUE_ITEM					// 2006.10. 16 / ���翵 / ����ũ ������ �߰��� ���� �۾�.






//////////////////////////////////////////////////////////////////////////
// �۾� ���� �� 
//#define _MAP_OPTIMIZATION_OUTSIDE_FRUSTUM	// 2007. 1. 30 / ���翵 / �������� �ø��� �ؼ� ���־� ����Ʈ�� ���Դµ� ����Ʈ�� ��ȸ�ϸ鼭 �� �ø� �˻� �ϴ� �κ� ����.
//#define _MOD_CHAGNE_DELETE_HERO				// 2007. 2. 22 / ���翵 / ����� ���� ��� ����.( ������ ����ȯ�� ������ �����ߴ��� �ش� ����� �����ÿ��� �����ϰ� �� ).

//////////////////////////////////////////////////////////////////////////
// ������ �ʴ� ������
//#define _TEST_SYNCRONIZE					// 2006. 9. 25 / ���翵 / ���ε� �н� ���α׷����� ���� ������ �Ƚ��ؼ� ����ȭ ������ �ذ�.

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// ������ ������
//#define _STYLE_QUICK_AUTO_REGISTER	    // ��Ÿ�� �� â�� ��Ÿ�� ���� �ڵ� ���
//#define _ALL_CHAT_TO_WORLD			    // ������ ���� ������ ä�� �ɽ����� ������.
//#define _NPC_PORTAL					    // ������ �̵� ó��
//#define _LIMIT_CREATE_BATTLE			    // ��Ʋ ������ ��ũ��Ʈ�� �����Ͽ� �������� �Ѹ㸸 ���ð���.
//#define _EVENT_INVENTORY
//#define _GUILD_CHAT
//#define _DELAY_LEAVE_FIELD			    // 2006. 2. 15 / ���翵 / �ʵ忡�� ������ ���ư��� �����̸� ��.
//#define _EQUIP_LIMIT_OPTIONS			    // 2006. 2. 16 / ���翵 / ������ Ż/������ ������ �ɼǵ� ���ǿ� + �ȴ�.
//#define _MOD_DEAD_PROCESS				    // 2006. 2. 20 / ���翵 / �׾����� ǥ�� ���� �� ó�� ����.
//#define _MOD_SYS_MSG_DLG				    // 2006. 2. 20 / ���翵 / SystemVerfy, SystemConfrim ǥ�� �κ� ����
//#define SERVER_BASED_HP_MULTIPLIER
//// ���� ����
//#define _ENCHANT_TEST					    // ���� �ۼ��� ��þƮ
//#define _RANKUP_TEST					    // ���� �ۼ��� Rankup
//#define _ADD_NEW_ITEM_COMPOSITE			// 2006. 3. 16 / ���翵 / ���� �ٲ� ������ ���� �߰�
//#if defined( _ADD_NEW_ITEM_COMPOSITE )
//#	define _ADD_ITEM_COMPOSITE_COMPOSITE	// / ���翵 / ����
//#	define _ADD_ITEM_COMPOSITE_CRYSTALIZE	// / ���翵 / ����ȭ 
//#	define _EXTRASTONE_USE					// / ���翵 / ����Ʈ�� ���� ���
//#	define _MOD_ITEM_EXTRACTSOCKET			// / ���翵 / �ɼ� ����
//#endif
//#define _ADD_ITEM_COMPOSITE_FORWARD_INFO	// 2006. 3. 28 / ���翵 / NPC ���� ���� �� �κ� ����.
//#define _CHANGE_MAP_LOAD_METHOD		    // 2006. 4. 27 / ���翵 / �ʷε� ��� ����.
//#define _ADD_MAP_LOAD_OPTION			    // 2006. 6.  2 / ���翵 / �ʷε��� �ɼ� �и�.

//#define _MOD_SCREEN_SHOT				    // 2006. 4.  7 / ���翵 / ��ũ�� �� ���ϸ� ���� ���� ���� 
//#define _ADD_EQUIP_SPECIAL_ACSSESORY	    // 2006. 5.  7 / ���翵 / Ư�� �Ǽ��縮 ���� 
//#define _ADD_LOCK_ITEM_COMPOSITE		    // 2006. 5.  9 / ���翵 / �������� ��ŷ ����.
//#define _CHANGE_GAME_INFO_LOAD			// 2006. 5. 16 / ���翵 / ���� �⺻ ���� �ε� �ϴ� �κ� ����(��ġ ����)
//////////////////////////////////////////////////////////////////////////