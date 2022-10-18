#ifndef __RESULT_CODE_H__
#define __RESULT_CODE_H__



namespace RC
{
    // ��� ó���� ���� ��ƿ��Ƽ ���ø�
    template <typename ENUM_TYPE, // ������ ����
              ENUM_TYPE begin_value, // ���۰�
              ENUM_TYPE end_value, // ���ᰪ (*STL �ݺ����� end�� �ǹ̰� ������ ������ ��!)
              ENUM_TYPE success_value, // ������
              ENUM_TYPE default_value, // �⺻��
              typename VALUE_TYPE> // ���� ����� �� ����
    struct ResultUtilTemplate
    {
        typedef VALUE_TYPE ValueType; // ���� ����� �� ����

        // ������ ���� ���� ����� ���� �������� ��ȯ�Ѵ�.
        inline static ValueType EnumToValue(const int enum_value)
        {
            // ���� ��ȿ���� ������
            if (enum_value < begin_value && enum_value >= end_value)
            {
                // �α׿� ����ϰ� �⺻���� ��ȯ�Ѵ�.
                MessageOut(eCRITICAL_LOG, 
                           _T("|ResultUtil|Value's boundary is invalid.|begin:%d|end:%d|value:%d|"), 
                           begin_value, 
                           end_value, 
                           enum_value);
                return GetDefaultValue();
            }
            return static_cast<ValueType>(enum_value);
        }

        // �⺻���� ��´�.
        inline static ValueType GetDefaultValue()
        {
            BOOST_STATIC_ASSERT(default_value >= begin_value && default_value < end_value);
            return static_cast<ValueType>(default_value);
        }

        // ����� ���� �ߴ��� �˻��Ѵ�.
        inline static bool IsSuccess(const int result_value)
        {
            return (result_value == success_value);
        }

        // ����� ���� �ߴ��� �˻��Ѵ�.
        inline static bool IsFail(const int result_value)
        {
            return !IsSuccess(result_value);
        }
    }; // ResultUtilTemplate

	enum ePACKET_SEND_RESULT
	{
		RC_SEND_SUCCESS,
		RC_SEND_FAILED,
		//< ......
	};
	enum eSECTOR_REULT
	{
		RC_SECTOR_SUCCESS,
		RC_SECTOR_FAILED,
		//< ......
	};
	enum eITEM_RESULT
	{
		RC_ITEM_FAILED,
		RC_ITEM_SUCCESS,
		RC_ITEM_INVALIDSTATE,
		RC_ITEM_INVALIDPOS,
		RC_ITEM_EMPTYSLOT,				//< Ư����ġ�� ������ ����ִ�.
		RC_ITEM_NOTEXISTITEMATFIELD,	//< �ʵ忡 �������� �������� �ʴ´�.
		RC_ITEM_NOINFO,					//< Code�� �ش��ϴ� ������ ������ �������� �ʴ´�.
		RC_ITEM_NOSPACEININVENTORY,		//< �κ��丮�� �� ������ ����.
		RC_ITEM_INVALIDSTATEOFPLAYER,	//< �÷��̾� ���°� �̻��ϴ�
		RC_ITEM_INVALIDPOSFORDROPITEM,	//< ����� �������� ��ġ�� �߸��Ǿ���.
		RC_ITEM_UNKNOWNERROR,			//< �˼� ���� ����, Ŭ���̾�Ʈ������ �߸��� ������ Ȥ�� ��ŷ�� ���� ������ ũ��
		RC_ITEM_DBP_ASYNC,				//< DBP�κ��� �񵿱������� �ø��� �߱� ��û ����
		RC_ITEM_COOLTIME_ERROR,			//< ��Ÿ���� ������ �ʾҴ�.

		//< Combine
		RC_ITEM_ITEMCODENOTEQUAL,		//< ������ �ڵ尡 �ٸ���.

		//< use
		RC_ITEM_ISNOTWASTEITEM,					//< �Ҹ� �������� �ƴϴ�.
		RC_ITEM_PORTION_USE_LIMIT,				//< �������ÿ��� ���ǻ�� ���� ������ �ִ�.
		RC_ITEM_CAN_USE_FIELD,					//< �ʵ忡���� ����� �� �ִ�.
		RC_ITEM_INVALID_TYPE,					//< �ش� ����� �̿��� �� �ִ� ������ ������ �ƴϴ�.
		RC_ITEM_FIELD_IS_NULL,
		RC_ITEM_UNABLE_FUNCTION_FOR_CUR_ZONE,	//< ���� ���������� ����� �� ���� ����Դϴ�.
        RC_ITEM_CANNOT_USE,                     // �������� ����� �� �����ϴ�.
		RC_ITEM_CANNOT_MOVE_TO_THE_AREA,		//< �ش���ġ�� �̵��� �� ����.
		RC_ITEM_CANNOT_LEARN_SKILL,				//< ��ų�� ���� �� �� �����ϴ�.
		RC_ITEM_CANNOT_SKILL_ACTION,			//< ��ų�� ���� �� �� �����ϴ�. 816, 
		RC_ITEM_CANNOT_FIND_PLAYER_TO_MOVE,		//< �̵���Ű���� �÷��̾ ã�� �� ����.
		RC_ITEM_NOT_STORE_COORD,				//< ��ǥ�� ����Ǿ� ���� �ʴ�.
		RC_ITEM_ALREADY_STORE_COORD,			//< �̹� ��ǥ�� ����Ǿ� �ִ�.
		RC_ITEM_CANNOT_STORE_COORD_AT_THIS_AREA,//< �ش���ġ������ ��ǥ�� ������ �� ����.	
		RC_ITEM_CANNOT_CREATE_TOTEMNPC,			//< TotemNPC�� ������ �� ����.
#ifdef _NA_006689_20130503_CANNOTUSE_BUFFITEM
        RC_ITEM_CANNOT_USE_ALREADY_APPLY_HIGHRANK_BUFF, //< �̹� ������ũ�� ������ �ɷ��ֱ⶧���� ����� �� ����.
#endif //_NA_006689_20130503_CANNOTUSE_BUFFITEM

		//< sell
		RC_ITEM_INVALIDSHOPLISTID,		//< dwShopListID�� �߸��Ǿ���.
		RC_ITEM_OUTOFSHOPITEMINDEX,		//< shop���� �Ǹ��ϴ� ������ �ε����� �ƴϴ�.
		RC_ITEM_HAVENOTMONEY,			//< ���� ����.

		//< buy
		RC_ITEM_ISNOTEMPTYSLOT,			//< �� ������ ���� �� á��.
		RC_ITEM_HAVENOTSPACE,			//< ������ �����ϴ�.
		RC_ITEM_INVALIDVALUE,			//< �߸��� ���� �Դ�.

		//< pick
		RC_ITEM_OVER_PLAYING_TIME,
		RC_ITEM_DONOT_HAVE_OWNERSHIP,		//< �����ۿ� ���� �������� ����.
		RC_ITEM_UNPICKABLE_TYPE,			//< ���� �� ���� ������ Ÿ���̴�. 
		RC_ITEM_CANT_PICKABLE_BY_LENGTH,	//< ���������� ���� �� ���� �Ÿ���. ��?
        RC_ITEM_DEAD_OWNER,                 //< �������� �������ڰ� �׾� �ִ�.

		//</.....
		//< drop
		RC_ITEM_ERRORDROPMONEY,

		//< enchant ����
		RC_ITEM_ENCHANT_SUCCESS,
		//< enchant ����
		RC_ITEM_ENCHANT_FAILED,	
		RC_ITEM_ENCHANT_DOWN_FAILED,
		RC_ITEM_ENCHANT_CRACK_FAILED,
		RC_ITEM_ENCHANT_HAVENOTMETERIALS,
		RC_ITEM_ENCHANT_INVALID_ITEMLEVEL,			//< ������ ������ ���� ��þƮ �ʿ� ��� ������ ����. (��, ������ ������ �߸��Ǿ���. )
		RC_ITEM_ENCHANT_INVALID_RATE_INDEX,			//< �߸��� ����Ȯ�� �ε��� ���̴�.
		RC_ITEM_ENCHANT_FAILED_AND_ENCHNAT_DOWN,	//< ��þƮ�� �����Ͽ���, ��þƮ ���� �ٿ� �Ǿ����ϴ�.
		RC_ITEM_ENCHANT_FAILED_AND_DESTROY,			//< ��þƮ�� �����Ͽ��� �������� �Ҹ�Ǿ����ϴ�. [0815���� ��� �ȵ�.]
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        RC_ITEM_ENCHANT_FAILED_AND_PREVENT,         //< ��þƮ�� �����Ͽ���, ������ �Ҹ� �� �ٿ��� �����Ǿ����ϴ�.
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

		//< rankup
		RC_ITEM_INVALID_CONDITION,					//< üũ ���ǿ� ���� �ʴ´�.
		RC_ITEM_CANNT_RANKUP_ITEM,
		RC_ITEM_NO_MORE_RANK,
		RC_ITEM_INSUFFICIENT_MONEY,					//< ���� �����ϴ�.		
	#ifdef __JP_000899_20080318_ENCHANT_RANKUP_LOCALIZE
		RC_ITEM_RANKUP_FAILED_AND_DESTROY,			//< ��ũ���� ���� �Ͽ� �������� �Ҹ�Ǿ����ϴ�.
	#endif

		//< socket
		RC_ITEM_FULLSOCKET,				//< ������ �� á��.

		//< extract socket
		RC_ITEM_EXTRACTSOCKET_SUCCESS,
		RC_ITEM_EXTRACTSOCKET_FAILED,

		RC_ITEM_UNUSABLE_FUNCTION,		//< ���� ����� �� ���� ����Դϴ�.
		RC_ITEM_INVALID_VALUE,			//< �߸��� ���̴�.
		RC_ITEM_CANNNOT_DROPSTATE,		//< drop�� �� ���� �����Դϴ�.
		RC_ITEM_NOTEXIST_ITEM,			//< �������� �������� �ʴ´�.

		RC_ITEM_CANNOT_SELL_ITEM,		//< �ǸŰ� �Ұ����� �������Դϴ�.
		RC_ITEM_CANNOT_TRADE_ITEM,		//< �ŷ��� �Ұ����� �������Դϴ�.
		RC_ITEM_CANNOT_DROP_ITEM,		//< ����� �Ұ����� �������Դϴ�.
		RC_ITEM_CANNOT_EXCHANGE_ITEM,	//< ��ȯ�� �� ���� �������Դϴ�.
		RC_ITEM_CANNOT_MOVE_OTHER_SLOT_ITEM,	//< �ٸ� �������� �̵��� �� ���� �������Դϴ�.
		RC_ITEM_CANNOT_DESTROY_ITEM,	//< �ı�(delete_syn)�� �� ���� �������Դϴ�.
		RC_ITEM_CANNOT_PICKUP_OBSERVER,	//< ������¿����� �������� �ֿ� �� ����.
		//< Compose
		RC_ITEM_COMPOSE_SUCCESS,
		RC_ITEM_COMPOSE_FAILED,

		//< Crystalize
		RC_ITEM_CRYSTALIZE_SUCCESS,
		RC_ITEM_CRYSTALIZE_FAILED,

		RC_ITEM_UNABLE_FUNCTION_FOR_CURR_LEVEL,		//< ���� �������� �̿��� �� ���� ����Դϴ�.

		// ������, ����
		RC_ITEM_DONT_NEED_TO_REPAIR,				//< ������ �ʿ� ����
		RC_ITEM_CANNOT_REPAIR_ITEM,					//< ������ �Ұ����� �������Դϴ�.

		RC_ITEM_COMPOSE_INVALID_LOCATION,			//< ������ �� ���� ���Դϴ�.
		RC_ITEM_COMPOSE_LIMIT_CLASS,				//< ������ �� ���� Ŭ�����Դϴ�.

		RC_ITEM_UNABLE_FUNCTION_FOR_CHAOSTATE,		//< ī�����¿����� �̿��� �� ���� ����̴�.
		RC_ITEM_CANNOT_USE_QUEST_ACCEPT_ITEM,		//< ����Ʈ ���� �������� ����� �� �����ϴ�.

		RC_ITEM_CANNOT_RANKUP_ELITE,				//< ����Ʈ ������ ��ũ�� �Ұ�
		RC_ITEM_CANNOT_CRYSTALIZE_ELITE,			//< ����Ʈ ������ ����ȭ �Ұ�
		RC_ITEM_CANNOT_RANKUP_UNIQUE,				//< ����ũ ������ ��ũ�� �Ұ�
		RC_ITEM_CANNOT_CRYSTALIZE_UNIQUE,			//< ����ũ ������ ����ȭ �Ұ�

		RC_ITEM_UNABLE_FUNCTION_FOR_TRANSFORMATION,	//< ���Ż��¿����� �̿��� �� ���� ����̴�.

		RC_ITEM_CANNOT_DELETE_FOR_SHORT_COUNT,		//< ������������ ������ �� ����.
		RC_ITEM_LENGTH_CANNOT_USE_NPC,				//< NPC�� �̿��� �� ���� �Ÿ�.
		RC_ITEM_DBG_WRONG_PACKET,					//< �߸��� ��Ŷ ����	

		RC_ITEM_CANNOT_ADD_EXTRAINVENTORY,			//< ���̻� ����ȭ �κ��丮�� �߰��� �� ����.
		RC_ITEM_CANNOT_DO_THIS_FOR_CHARGEITEM,		//< ����ȭ �����ۿ� ���� �� ����� �̿��� �� �����ϴ�.
		RC_ITEM_DONT_EXIST_TO_REPAIR,				//< ������ �������� �����ϴ�.

		//�Ҹ��� ��
		RC_ITEM_NOT_INVENTORY,						//< �κ��丮�� �ƴ� ���Կ� �����ϴ� �������Դϴ�.
		RC_ITEM_INVALID_SOCKET_NUM,					//< ���Ͽɼ� ������ �߸��� ���ϳѹ� �Դϴ�.
		RC_ITEM_SOCKET_HAS_NOT_OPTION,				//< ���Ͽɼ��� ���� �������� ������ ���� �Ϸ��� �մϴ�.
		RC_ITEM_SOCKET_DIFFERENT_LEVEL,				//< ��,�ϱ޷��� �ٸ� ���� �ɼ��� ������ �� �����ϴ�.		
		RC_ITEM_SOKET_NOT_RUNE_ITEM,				//< ��� �������� '�Ҹ��� ��'�� �ƴϴ�.
        
		RC_ITEM_ISNOT_PARTY_MEMBER,					//< ��Ƽ ����� ���� ����� �� �ִ� ����̴�.

		RC_ITEM_NOT_MIXTURE,						//< ����� ����� �� ���� �������̴�.(Enchant, Rankup ���...)
		
		//�ʱ�ȭ ������
		RC_ITEM_LESS_FOR_USE,						//< �ش� �������� ���ڶ��.
		RC_ITEM_NOT_UNEQUIP_ALL,					//< ��� ��� �������� ���� �����̴�.
		RC_ITEM_NOT_MORE_DECREASE,					//< �� �̻� ������ ������ �� ����.
		RC_ITEM_CANNOT_USE_MULTI_ALL_ITEM,			//< �ý��� �ʱ�ȭ �������� ������ ����� �� ����.

		//���� ��ȯ ������
		RC_ITEM_MONSTER_SUMMON_FAILED,				//< ���� ��ȯ �������� ����� ���������� Ȯ���� ���� ����

		RC_ITEM_WAREHOUSE_NEED_PASSWORD,			//< â�� ��� �ʿ��մϴ�.
		RC_ITEM_INVENTORY_NEED_PASSWORD,			//< �κ��丮 ��� �ʿ��մϴ�.
		RC_ITEM_INVENTORY_INVALID_PASSWORD,			//< �߸��� ����Դϴ�.
		RC_ITEM_INVENTORY_ALREADY_TRANSACTION,		//< �ٸ� ������� ó�����Դϴ�.
		RC_ITEM_INVENTORY_PWD_SETTING_FLOW_VIOLATION, //< ��� �������� �÷ο츦 �����߽��ϴ�.
		RC_ITEM_INVENTORY_INVALID_PACKET,

		RC_ITEM_CANNOT_MOVE_WAREHOUSE_ITEM,			//< â��� �̵��� �� ���� �������Դϴ�.		

		RC_ITEM_CANNOT_RANKDOWN_ELITE,				//< ����Ʈ �������� ��ũ ���� �Ұ�
		RC_ITEM_CANNOT_RANKDOWN_UNIQUE,				//< ����ũ �������� ��ũ ���� �Ұ�
		RC_ITEM_NOT_RANK_DOWN_TO_E_RANK_ITEM,		//< E��ũ �������� ��ũ�� ������ �� �����ϴ�.

		RC_ITEM_CANNOT_EXTEND_CASH_SLOT,			//< ���̻� ����ȭ ���������� �߰��� �� ����.
		RC_ITEM_CANNOT_EXTEND_DATE,					//< �Ⱓ�� ������ �� ���� �������̴�.
		RC_ITEM_CANNOT_RANKUP_LIMITED,				//< ����Ƽ�� �������� ��ũ�� �Ұ�.
		RC_ITEM_CANNOT_REPAIR_MAX_DURA_TWO_OVER,	//< �ִ� ������ 2���Ͽ����� ��� ����
		RC_ITEM_NOT_LIMITED_ITEM,					//< ����Ƽ�� �������� �ƴ�
		RC_ITEM_OVER_REPAIR_COUNT,					//< ���� ���� Ƚ�� ����
		RC_ITEM_CAN_NOT_EQUIP,						//< ����� �� ���� ������
		RC_ITEM_CANNOT_RANKUP_FATE,					//< Fate �������� ��ũ�� �Ұ�.
		RC_ITEM_NOT_FATE_ITEM,						//< Fate ������ �ƴ�.
		RC_ITEM_ALREADY_IDENTYFIED_FATE_ITEM,		//< �̹� ������ Fate ������
		RC_ITEM_WIN_PRIZE_INFINITY_ITEM,			//< NOTE: f110428.5L, removed don't use anymore since 1102, ������ �����ۿ� ��÷��(���������)
		RC_ITEM_NOT_IDENTYFIED_FATE_ITEM,			//< �̹� ������ Fate ������
		RC_ITEM_NOT_EXIST,							//< �������� ���� ���� �ʽ��ϴ�.		

		RC_ITEM_EVENT_SUCCESS,
		RC_ITEM_EVENT_FAILED,
		RC_ITEM_EVENT_ALREADY_ANOTHER_TRANSACTION,		//< [�̺�Ʈ] �̹� �ٸ� ����� �������Դϴ�. ����� �ٽ� �õ��ϼ���.
		RC_ITEM_EVENT_CANNOT_MOVE_TO_INVENTORY,			//< [�̺�Ʈ] Can't Move EventInven->Inven
		RC_ITEM_IMPOSSIBLE_PERIOD_EXTEND,				//<  �Ⱓ ���� �Ұ��� ������
		RC_ITEM_ALREADY_REGISTER_AC_ITEM,				//< �̹� ��ϵ� AC ������

		RC_ITEM_SUMMON_SUMMON_SUCCESS,					// ��ȯ ����
		RC_ITEM_SUMMON_RETURN_SUCCESS,					// ���� ����		
		RC_ITEM_SUMMON_ALREADY_SUMMONED,				// �̹� �ٸ� ���� ��ȯ �Ǿ� �־� ����
		RC_ITEM_SUMMON_NOT_PET_ITEM,					// �� �������� �ƴ�
		RC_ITEM_SUMMON_NOT_SUMMONED_PET,				// ��ȯ�� ���� ���µ� RETURN �Ϸ��� ��
		RC_ITEM_SUMMON_MISMATCH_PET_ITEM,				// ��ȯ�� ���̶� �����Ϸ��� �������� ���� �ٸ�.	
		RC_ITEM_CAN_NOT_SUMMON_FULLNESS_ZERO,			// �������� 0 �̾ ��ȯ �Ұ�
		RC_ITEM_NOT_SUMMONED_PET,						// ��ȯ�� ���� ����

		//��� â��
		RC_ITEM_GUILDWAREHOUSE_USENOT_SLOT_BY_GRADE,    // ��� ����� ���� ����� �� ���� ����
		RC_ITEM_GUILDWAREHOUSE_USENOT_SLOT_BY_CASHITEM, //ĳ������ ��� ����� �� ���� ����
		RC_ITEM_GUILDWAREHOUSE_NOEN_OUT_RIGHT,          //��� ��� �� ������ ����.
		RC_ITEM_GUILDWAREHOUSE_OPEN_USENOT_CASHITEM,	//��� â�� �����ִ� ���¿����� Ȯ�����縦 �̿��Ҽ� ����.
		RC_ITEM_GUILDWAREHOUSE_NONE_USENOT_CASHITEM,	//�Ҽӵ� ��尡 ��� ��� â�� Ȯ�� �������� ����� �� �����ϴ�.
		RC_ITEM_GUILDWAREHOUSE_NONE_USE_RIGHT,		    //��� â�� Ȯ������ ĳ������ ����� ������ ����.
	    //__NA_1247_POSSESSION_ITEM
		RC_ITEM_GUILDWAREHOUSE_UNABLE_POSSESSION_ITEM,	// �ͼӵ� �������� ���â�� �ű� �� ����.
        //__NA_000968_ETHER_EMISSION_DEVICE
		RC_ITEM_ETHER_DEVICE_USE_ONLY_TO_WEAPON,		// ���׸� ������ġ�� ���⿡�� ���� ����
		RC_ITEM_ALREADY_EQUIP_ETHER_DEVICE,				// ���׸� ������ġ�� �̹� ���� �Ǿ� ����
		RC_ITEM_NOT_EQUIP_ETHER_DEVICE,					// ���׸� ������ġ�� ���� �Ǿ����� ����
		RC_ITEM_ALREADY_CHARGE_ETHER_BULLET,			// ���׸� �Ѿ��� �̹� ���� �Ǿ� ����
        // LOTTO
		RC_ITEM_LOTTO_OPEN_SUCCESS,						// ���� ����
		RC_ITEM_LOTTO_OPEN_FAIL,						// ���� ����
		RC_ITEM_LOTTO_IDENTITY_FAIL,					// ���� ����
		RC_ITEM_LOTTO_IDENTITY_SUCCESS,					// ���� ����
		RC_ITEM_LOTTO_ALREADY_IDENTYFIED_LOTTO_ITEM,	// �̹� �����Ǿ��ִ�.
		RC_ITEM_LOTTO_NEED_IDENTYFY_LOTTO_ITEM,			// ������ �ʿ��ϴ�.
		RC_ITEM_LOTTO_NEED_EMPTY_SLOT,					// ������ �����ϴ�.
		RC_ITEM_LOTTO_NOLOTTOITEM,						// �ζ� �������� �ƴϴ�.
		RC_ITEM_LOTTO_INVALID_LOTTOSCRIPT_INDEX,		// ��ũ��Ʈ �ε����� �߸��Ǿ���.
		RC_ITEM_LOTTO_INVALID_STATE,					// �̻��� �����̴�.
		RC_ITEM_LOTTO_NEED_OPEN_STATE,					// �����־�� �Ѵ�.
		RC_ITEM_LOTTO_ALREADY_OPENED_LOTTO_ITEM,		// �̹� �����ִ�.
		RC_ITEM_LOTTO_CONCRETIZE_SUCCESS,				// ��üȭ ����
		RC_ITEM_LOTTO_CONCRETIZE_FAIL,					// ��üȭ ����
		RC_ITEM_PANDORABOX_CLOSE_SUCCESS,				// �ǵ������ �ݱ� ����
		RC_ITEM_PANDORABOX_CLOSE_FAIL,					// �ǵ������ �ݱ� ����

        RC_ITEM_WINDOW_ALREADY_OENDEDSTATE,				// �̹� �����찡 �����ִ�.
		RC_ITEM_NOTEXIST_SUMMON_MONSTERINFO,			// ���� ������ �����ϴ�.
		RC_ITEM_CANNOT_USE_PET_NAME,					// ����� �� ���� �� �̸�(�弳, ���� ��)
		RC_ITEM_CAN_NOT_ACTIVE_ETHER_BULLET,			// ���׸� źȯ�� Ȱ��ȭ �� �� ����
		RC_ITEM_CAN_NOT_INACTIVE_ETHER_BULLET,			// ���׸� źȯ�� ��Ȱ��ȭ �� �� ����
		RC_ITEM_CANT_EQUIP_WEAPON			,			//< ���ݹ��� ������ �� �����ϴ�.
		RC_ITEM_CAN_USE_ONLY_CHAO,						//< 903 ī�� ������ ����� �� �ִ�.(����ī�� ����)
		RC_ITEM_CANNOT_USE_PLAYER_LEVEL,				//< 904 ��� ������ ������ �ƴ� __CN_1299_WASTE_ITEM_LEVEL_LIMITE_CHECK
		RC_ITEM_ALREADY_FIREUP_ITEM_SAME_TYPE,			//< 903 �̹� ���� Ÿ���� �������� Ȱ��ȭ �Ǿ��ִ�.
        //__NA1390_1391_090915_RIDING_SYSTEM_ITEM_PART__
        RC_ITEM_ALREADY_EXIST,                          //< �ش� ���Կ� �������� �̹� �����Ѵ�.
        //RC_ITEM_CAN_NOT_EQUIP <������ ���� - ����� �� ���� ������
        RC_ITEM_ALREADY_TIME_EXPIRED,                   //< �̹� ����� �������Դϴ�.

        // (090919) (���� ������ �ڵ�) __NA_001358_ENCHANT_UNIFICATION
        RC_ITEM_INVALID_ENCHANT_LEVEL,					//< ��ȿ���� ���� ��þƮ ����
        RC_ITEM_ALREADY_USE_CHARWAYPOINT,		        //< �齺�� �������� �̹� ����� �Դϴ�.
        //_NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
        RC_ITEM_ALREADY_EXTRACTED_TO_ETHERIA,           //< �̹� ���׸��ư� ����� ����Դϴ�.
        RC_ITEM_CANT_EXTRACT_TO_ETHERIA,                //< �� �������� ���׸��� ������ �Ұ����մϴ�.
        RC_ITEM_CANT_COMBINED_WITH_ITEM,                //< �ش� �������� ������ �� �����ϴ�.
        RC_ITEM_CANT_COMBINED_WITH_DIFFERENT_ITEM,      //< ���� ������ ������ �� �ֽ��ϴ�.

        // _NA_000096_20100527_CHANGE_CHARACTER_APPEARANCE_SYSTEM
        RC_ITEM_FAIL_TO_CHANGE_CHARACTER_APPEARANCE, // ĳ���� �ܸ� ���� ����
        
        // _NA_000251_20100727_SOCKET_SYSTEM
        RC_ITEM_NOT_UNIDENTIFIED_SOCKET,             // ��Ȯ�� ������ �ƴϴ�.

        // _NA_002253_20100811_CUBE_SYSTEM
        RC_ITEM_SUCCESS_TO_COMPOSE_CUBE, // ť�� �ռ� ����
        RC_ITEM_FAILED_TO_COMPOSE_CUBE, // ť�� �ռ� ����
        RC_ITEM_SUCCESS_TO_DECOMPOSE_CUBE, // ť�� ���� ����
        RC_ITEM_FAILED_TO_DECOMPOSE_CUBE,  // ť�� ���� ����

        // _NA_000587_20100928_DOMINATION_BUFF_ITEM
        RC_ITEM_UNAVAILABLE_TARGET, // ������ ��� �������� ����� �� �����ϴ�.
        RC_ITEM_NOT_FRIEND, // �Ʊ��� �ƴմϴ�.

        // _NA_20101011_HEIM_LOTTO_CASH_TICKET
        // ĳ�� ����� ������� ������� ���� ���� Ȱ��ȭ ���� ����
        RC_ITEM_HEIM_LOTTO_TICKET_MAX_REACHED,          // ������� ���ǿ��� ���� ���� ���� �ʰ�
        RC_ITEM_HEIM_LOTTO_UNAVAILABLE,                 // ��÷ ���� ������ �Ͻ������� ����� ����� �� ����

        // _NA_000251_20100727_SOCKET_SYSTEM
        RC_ITEM_SUCCESS_TO_COMPOSE_ZARD, // ��� �ռ� ����
        RC_ITEM_FAILED_TO_COMPOSE_ZARD, // ��� �ռ� ����

        RC_ITEM_SUCCESS_TO_GET_COMPOSITION_OR_DECOMPOSITION_HISTORIES, // �ֱٿ� ������ �ռ� �Ǵ� ���� ��� ��� ����
        RC_ITEM_FAILED_TO_GET_COMPOSITION_OR_DECOMPOSITION_HISTORIES, // �ֱٿ� ������ �ռ� �Ǵ� ���� ��� ��� ����

        RC_ITEM_SOCKET_FAILED_TO_EXTRACT_BY_RATIO, // Ȯ���� ���� ��� ���⿡ ���������� ���� �õ� ��ü�� ����
        RC_ITEM_SOCKET_FAILED_TO_FILL_BY_GRADE, // ��� ��޺� ���� ���ѿ� �ɷ��� ��� �ڱ� ����

        //_NA_20110303_ADD_TYPE_OF_NPCSHOP
        RC_ITEM_IS_SHORT_HAVE_ITEM,             // ������ Ÿ�� �������� �����ϱ� ���� ���� �������� �����ϴ�.
        RC_ITEM_IS_SHORT_HAVE_ACCUMULATEPOINT,  // ���� ����Ʈ �������� �����ϱ� ���� ���� ����Ʈ�� �����ϴ�.

        RC_ITEM_CANNOT_EQUIPMENT_BETAKEY,       //�ش� ��ŸŰ�� ������ ���� ������ �����Ҽ� ���� �������̴�.
        RC_ITEM_CANNOT_USE_BETAKEY,             //�ش� ��ŸŰ�� ������ ���� ������ ����Ҽ� ���� �������̴�.

        RC_ITEM_BUY_ERROR_OF_REPUTE,                   // ������ �����ؼ� �������� ���� �� �� ����.
        RC_ITEM_BUY_ERROR_OF_FAME,                     // ���� �����ؼ� �������� ���� �� �� ����.

        //_NA004034_20120102_POINT_WALLET_SYSTEM
        RC_ITEM_FAILED_TO_EXCHANGE_CURRENCY_ITEM_BY_LIMIT,  // �ִ������������ ȭ��������� ȭ������Ʈ ȯ�� ����
        RC_ITEM_FAILED_TO_BUY_BY_POINT,                     // ȭ������Ʈ �������� �����۱��� ����

        //_NA004034_20120102_GUILD_POINT_SYSTEM
        RC_ITEM_GUILDCOIN_NOT_GUILD_MEMBER,                    // ������� �ƴ�
        RC_ITEM_GUILDCOIN_NOT_ENOUGH_COIN,                     // ������� ����
        RC_ITEM_GUILDCOIN_DONATION_SUCCESS,                    // ������� ��� ����
        RC_ITEM_GUILDCOIN_DONATION_COUNT_OVER,                 // ��� Ƚ�� �ʰ�        
        RC_ITEM_GUILDCOIN_EXCEED_MAX_LEVEL,                    // �ְ� �����̹Ƿ� ���̻� ��ΰ� �ȵ�
        RC_ITEM_BUY_ERROR_GUILD_LEVEL,                          // ��� ������ �����Ҷ�
        RC_ITEM_BUY_ERROR_GUILD_EXP,                            // ��� ���嵵�� �����Ҷ�

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        RC_ITEM_GUILDFACILITY_NOT_GUILD_MEMBER,            // ��� ����� �ƴϴ�
        RC_ITEM_GUILDFACILITY_NONE_USE_RIGHT,              // ��� ������ ����
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

        // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
        RC_EQUIPMENT_AWAKENING_SUCCESS,                         // ���� ����
        RC_EQUIPMENT_AWAKENING_FAIL,                            // ���� ����
        RC_EQUIPMENT_AWAKENING_EQUIP_EXCEEDED_PERMIT_LEVEL,     // ��� ���� ��� ������ �Ѿ���.
        RC_EQUIPMENT_AWAKENING_EQUIP_MAX_LEVEL,                 // ��� ���� ������ �ְ��� �Դϴ�
        RC_EQUIPMENT_AWAKENING_DOES_NOT_MATCH_ADDITIVE,         // ÷���� �������� ���� �ʴ´�
        RC_EQUIPMENT_AWAKENING_DOES_NOT_MATCH_ADDITIVE_AMOUNT,  // ÷���� �������� ������ ���� �ʴ´�
        RC_EQUIPMENT_AWAKENING_DOES_NOT_MATCH_MATERIAL,         // ��� �������� ���� �ʴ´�
#ifdef _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD
		RC_EQUIPMENT_AWAKENING_DOES_NOT_MATCH_MATERIAL_AMOUNT,  // ��� �������� ������ ���� �ʴ´�
#endif // _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD
        RC_EQUIPMENT_AWAKENING_INVALID_EQUIP_INFO,              // ��� ������ �� �� ����
        RC_EQUIPMENT_AWAKENING_INVALID_MATERIAL_INFO,           // ��� ������ �� �� ����
        RC_EQUIPMENT_AWAKENING_INVALID_AWAKENING_INFO,          // ���� ������ �� �� ����

		RC_EQUIPMENT_EVOLUTION_SUCCESS,                         // ��ȭ ����
		RC_EQUIPMENT_EVOLUTION_FAIL,                            // ��ȭ ����
		RC_EQUIPMENT_EVOLUTION_INVALID_EQUIP_INFO,              // ��� ������ �� �� ����
		RC_EQUIPMENT_EVOLUTION_ITEM_CAN_NOT_EVOLUTION,          // ��ȭ ��ų �� ���� ������ �Դϴ�
		RC_EQUIPMENT_EVOLUTION_DOES_NOT_MATCH_ADDITIVE,         // ÷���� �������� ���� �ʴ´�
		RC_EQUIPMENT_EVOLUTION_DOES_NOT_MATCH_ADDITIVE_AMOUT,   // ÷���� �������� ������ ���� �ʴ´�
		RC_EQUIPMENT_EVOLUTION_DOES_NOT_MATCH_AWAKENING_GRADE,  // ���� ����� ���� �ʴ´�

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
        RC_ITEM_SPA_COSTUME_INVALID_ITEM,                       // ��õƼ���� �ƴϴ�
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM

#ifdef _NA_008404_20150712_MINIGAME_SYSTEM
        RC_ITEM_FAILED_EXIST_SEPARATE_NAK_PACKET,               // ������ NAK ��Ŷ�� �����Ѵ�
#endif // _NA_008404_20150712_MINIGAME_SYSTEM

        //------------------------------------------------------------------------------------------
        // (CHANGES) (f100517.2L) must be last settled
        RC_ITEM_PROCESS_PENDING,                        // ������ ���� ���� ������ ����� ��� ������ �� ���� ó�� ����
        RC_ITEM_UPPERBOUND                              // ������ ��� �ڵ� ���Ѱ�
	}; // eITEM_RESULT

    // ������ ��� ��� ó���� ���� ��ƿ��Ƽ
    typedef ResultUtilTemplate<eITEM_RESULT, 
                               RC_ITEM_FAILED, 
                               RC_ITEM_UPPERBOUND, 
                               RC_ITEM_SUCCESS, 
                               RC_ITEM_FAILED, 
                               BYTE> ItemResultUtil;

	enum eWAREHOUSE_RESULT
	{
		RC_WAREHOUSE_NOT_AVAILABLE_SERVICE,				//< â�� ���񽺸� �̿��� �� ����.
		RC_WAREHOUSE_INSUFFICIENT_MONEY,				//< ���� �����ϴ�.
		RC_WAREHOUSE_INVALID_STATE,						//< â�� ����� �� �ִ� ���°� �ƴϴ�.
		RC_WAREHOUSE_UNABLE_FUNCTION_FOR_CURR_LEVEL,	//< ���� �������� �̿��� �� ���� ����Դϴ�.
		RC_WAREHOUSE_UNAVAILABLE_LENGTH,				//< â�� �̿��� �� ���� �Ÿ� �Դϴ�.

		RC_WAREHOUSE_NEED_PASSWORD,						//< ��� �ʿ��մϴ�.
		RC_WAREHOUSE_INVALID_PASSWORD,					//< �߸��� ����Դϴ�.
		RC_WAREHOUSE_ALREADY_TRANSACTION,				//< �ٸ� ������� ó�����Դϴ�.
		RC_WAREHOUSE_PWD_SETTING_FLOW_VIOLATION,		//< ��� �������� �÷ο츦 �����߽��ϴ�.
		RC_WAREHOUSE_INVALID_PACKET,
	};

	enum eTRADE_RESULT
	{
		RC_TRADE_SUCCESS,
		RC_TRADE_FAILED,
		RC_TRADE_CANCEL,
		RC_TRADE_PLAYER1_HAVENOTSPACE,
		RC_TRADE_PLAYER2_HAVENOTSPACE,
		RC_TRADE_PLAYER1_HAVENOTMONEY,
		RC_TRADE_PLAYER2_HAVENOTMONEY,
		RC_TRADE_COUNTERPARTNER_NOTACCEPT,
		RC_TRADE_COUNTERPARTNER_NOTPROPOSAL,
		RC_TRADE_INVALID_STATE,
		RC_TRADE_COUNTERPARTNER_CANNOT_STATE,		//< ������ �ŷ��� �� ���� �����Դϴ�.
		RC_TRADE_CANNOT_TRADE_ITEM,					//< �ŷ��� �Ұ����� �������Դϴ�.
		RC_TRADE_NOT_AVAILABLE_SERVICE,				//< ���� �̿��� �� ���� �����Դϴ�.
		RC_TRADE_BLOCKED_ITEM,						//< ���� �������Դϴ�.
		RC_TRADE_DIFFERENT_ITEM_INFO,				//< ó�� �ø� �����۰� ���� �������� ������ Ʋ���ϴ�.
		RC_TRADE_UNABLE_FUNCTION_FOR_CHAOSTATE,		//< ī�����¿����� �̿��� �� ���� ����̴�.
		RC_TRADE_LENGTH_CANNOT_TRADE,				//< �ŷ��� �� ���� �Ÿ��̴�.
		RC_TRADE_DIFFERENT_ZONE,					//< ���� �ٸ� Zone�� ��ġ �� �ִ�.
		RC_TRADE_INVENTORY_NEED_PASSWORD,			//< ��� �ʿ��մϴ�.
		RC_TRADE_INTERCEPTION,						//< ������ �ŷ��� �����߽��ϴ�.
		RC_TRADE_MODIFY_CANCEL,						//< ������ �ŷ����� ��û�� ���.
		RC_TRADE_MODIFY_FAIL,						//< MSG_CG_TRADE_MODIFY_RESPONSE_SYN�� ���� ��� �ڵ�� ����ó���� ���� ���� ��
	};
	enum eVENDOR_RESULT
	{
		RC_VENDOR_SUCCESS,
		RC_VENDOR_FAILED,
		RC_VENDOR_INVALID_STATE,					//< ��ȿ���� ���� �÷��̾��� ����
		RC_VENDOR_INVALID_VENDORSLOT,				//< �߸��� ���� ��ġ�� ����
		RC_VENDOR_NOSPACEININVENTORY,				//< BUY : ������� �����ϴ�.
		RC_VENDOR_CANNOT_TRADE_ITEM,				//< �ŷ��� �Ұ����� �������Դϴ�.
		RC_VENDOR_NOT_AVAILABLE_SERVICE,			//< ���� �̿��� �� ���� �����Դϴ�.
		RC_VENDOR_BLOCKED_ITEM,						//< ���� �������Դϴ�.
		RC_VENDOR_INCLUDE_INVALID_ITEM_TO_START,	//< �ŷ��� �� ���� �������� ���ԵǾ� ������ �� �����ϴ�.
		RC_VENDOR_UNABLE_FUNCTION_FOR_CHAOSTATE,	//< ī�����¿����� �̿��� �� ���� ����̴�.
		RC_VENDOR_HAVE_NO_MONEY,					//< ���� �����մϴ�.2006/9/13
		RC_VENDOR_ALREADY_END,						//< �̹� ����� �������� �����Ϸ��� �Ѵ�.
		RC_VENDOR_INVENTORY_NEED_PASSWORD,			//< ��� �ʿ��մϴ�.
		RC_VENDOR_TITLE_BAN_WORD,					//< ���� �̸��� �����ܾ�
        RC_VENDOR_DIFFERENT_ZONE,                   //< ���� �ٸ� Zone�� ��ġ �� �ִ�.
        //------------------------------------------------------------------
        RC_VENDOR_RESULT_COUNTS // upperbound
	};

	enum eVENDOR_LIST_RESULT
	{
		RC_VENDOR_LIST_SUCCESS,							//<NPC�� �̿��� ��������Ʈ �̿� ����
		RC_VENDOR_LIST_FAILED,							//<���� �ּ��� eVENDOR_RESULT ����
		RC_VENDOR_LIST_INVALID_STATE,					
		RC_VENDOR_LIST_UNABLE_FUNCTION_FOR_CHAOSTATE,
		RC_VENDOR_LIST_NOT_AVAILABLE_SERVICE,				
		RC_VENDOR_LIST_NONE_PAGE,						//�������� ����� ����Ʈ�� ����
		RC_VENDOR_LIST_SEARCH_FAILED,
		RC_VENDOR_LIST_REFRENCE_NULL,					//NULL ������ ����
		RC_VENDOR_LIST_ALREADY,							//�̹� ���� ����Ʈ Open
        //------------------------------------------------------------------
        RC_VENDOR_LIST_RESULT_COUNTS // upperbound
	};
	
	enum eSKILL_RESULT
	{
		RC_SKILL_SUCCESS,
		RC_SKILL_FAILED,							//< ��Ÿ������ ����

		RC_SKILL_BASEINFO_NOTEXIST,					//< BaseSkillInfo�� ����.

		RC_SKILL_STATE_WHERE_CANNOT_ATTACK_ENEMY,	//< ���� ������ �� ���� �����̴�.
		RC_SKILL_COOLTIME_ERROR,					//< ���� ��Ÿ���� ������ �ʾҴ�.
		RC_SKILL_HPMP_RUN_SHORT,					//< HP, MP ����
		RC_SKILL_CHAR_CLASS_LIMIT,					//< Ŭ���� ����(�� ��ų�� ����� �� ���� ĳ���� Ÿ���̴�)
		RC_SKILL_WEAPON_LIMIT,						//< ���� ����
		RC_SKILL_SEALING_STATE,						//< ���� ���°� �ɷ��ִ�.
		RC_SKILL_OUT_OF_RANGE,						//< ��ų ��Ÿ����� �����.

		RC_SKILL_REQUIRE_LEVEL_LIMIT,				//< �䱸���� ����
		RC_SKILL_REQUIRE_SKILLSTAT_LIMIT,			//< �䱸���õ� ����
		RC_SKILL_DOES_NOT_HAVE,						//< ��ų�� ������ ���� �ʴ�.
		RC_SKILL_REMAIN_SKILLPOINT_LACK,			//< ���� ��ų����Ʈ�� ��ų������ �ø��⿡�� �����ϴ�.
		RC_SKILL_MAX_LEVEL_LIMIT,					//< �̹� �����̶� ���̻� ��ų����Ʈ�� �ø� �� ����.
		RC_SKILL_ALREADY_EXIST_SKILL,				//< �̹� �����ϴ� ��ų�̴�.

		RC_SKILL_INVALID_STATE,						//< �÷��̾��� ���°� ��ȿ���� �ʽ��ϴ�.
		RC_SKILL_NOTEXIST,							//< ��ų�� �������� �ʽ��ϴ�.
		RC_SKILL_INVLIDPOS,							//< �߸��� ��ġ������ �Խ��ϴ�.

		RC_SKILL_FIGHTING_ENERGY_FULL,				//< ���ⰳ���� �ִ�ġ�Ƿ� ���̻� �ø� �� ����.

		RC_SKILL_POSITION_INVALID,					//< ��ǥ�� ��ȿ���� �ʴ�.

		RC_SKILL_SUMMONED_NOTEXIST,					//< ��ȯü�� �������� �ʴ´�.
		RC_SKILL_TARGET_NOTEXIST,					//< Ÿ���� �������� �ʴ´�.
		RC_SKILL_FIELD_IS_NULL,

		RC_SKILL_ATTACKER_LEVEL_IS_LOW,				//< ������ ������ ���Ƽ� PK�� �� �� ����.
		RC_SKILL_TARGET_LEVEL_IS_LOW,				//< Ÿ�� ������ ���Ƽ� PK�� ���� �� ����.
		RC_SKILL_ISNOT_PK_ZONE,						//< PK�� ������ ������ �ƴϴ�.
		RC_SKILL_ISNOT_PK_SERVER,					//< PK�� ������ ������ �ƴϴ�.
		RC_SKILL_SERVER_STATE_IMPOSSIBLE_PK,		//< PK�� �Ұ����� ���������̴�.
		RC_SKILL_PK_IMPOSSIBLE_TARGET_TYPE,			//< PK�� �Ұ����� Ÿ��Ÿ���̴�.

		RC_SKILL_UNABLE_FUNCTION_FOR_TRANSFORMATION,//< ���Ż��¿����� �̿��� �� ���� ����̴�.

		RC_SKILL_CAN_LEARN_BY_ITEM,					//< �������� ����ؾ� ��ų�� ������ �� �ִ�.

        // (WARNING) (WAVERIX) [35244]~[45248] ���̿� (LST1024)�� ���� ���� ������ �߻�����.
        // ������ �� ���̿� ����� ��������, ������ �ֽñ���... �� �ϴ��� c/s�� revision��ȣ�� ����
        // �� �����Ѵٴ� ���� �Ͽ����� ���� �߻��� ���� ���� ���� ������, ��� ���� ��Ȳ������ ȥ��
        // �ʷ� ���ɼ� �����մϴ�.
		RC_SKILL_PLAYER_TILE_INVLIDPOS,				//< ����� Ÿ���� �������� �ʴ´�.
		RC_SKILL_ISNOT_TROUBLED_TYPE,				//< Ÿ���� ���� ������ �ƴϴ�
		RC_SKILL_TARGET_ISDEAD,						//< Ÿ���� �׾� �ִ�.
		RC_SKILL_SAME_GUILD_CHARACTER,				//< ���� �����̴�.
		RC_SKILL_SAME_PARTY_CHARACTER,				//< ���� ��Ƽ���̴�.
		RC_SKILL_CAN_NOT_STATE_HIDE,				//< ���̵� �Ұ� ���� __NA_0_SHADOW_HIDE_BUG_FIX

        //__NA_000921_20080227_TROUBLED_PARTS               //< Ŭ���̾�Ʈ ��� ���ҽ� ������ ������
        RC_SKILL_ISNOT_CHAO_PART,					//< Ÿ���� ī�� ������ �ƴϴ�	//��빫
        RC_SKILL_ISNOT_TROUBLED_ZONE,				//< ���� ������ ������ �ƴϴ�.	//��빫
        RC_SKILL_IMPOSSIBLE_PACKET_DATA,			//< ��Ŷ �����Ͱ� ���Ŀ� ���� �ʴ´�. //��빫
        //_NA002217_100728_EP2_PERK_SYSTEM_
        RC_SKILL_CANT_FIND_INFO,                    //< �ش� ������ ã�� �� �����ϴ�. (generic)
        RC_SKILL_UNROOT_PERK_REQUIRE_INACTIVE_STATE,//< ��Ȱ�� ���¿��� �մϴ�. (specific)
        
        //_NA_0_20100901_SKILLTREE_SYSTEM
        RC_SKILL_CANT_FIND_UPPER_SKILL,             //< ���� ��ų�� ã���������ϴ�.
        RC_SKILL_DISABLE_SKILL_LINE,                //< �ش� ��ų������ ����Ҽ� ����.

        //_NA_0_20110131_SKILLTREE_SYSTEM_RENEWAL
        RC_SKILL_ISNOT_ENOUGH_UPPER_SKILL_LEVEL,    //< ���� ��ų ������ �����մϴ�.

        RC_SKILL_CANNOT_SUMMON,                     //< ��ȯ�� �� ���� ����̴�.
        RC_SKILL_CANNOT_HEAL,                       //< ȸ����ų�� ����� �� ���� ����̴�.
        RC_SKILL_FP_RUN_SHORT,                      //< Fp�� �����ϴ�   
        RC_SKILL_CANNOT_USE_SPA,                    //< ��õ���� ��ų�� ����� �� �����ϴ�
    };

	enum eQUICK_RESULT
	{
		RC_QUICK_SUCCESS,
		RC_QUICK_FAILED,
		RC_QUICK_INVALIDSTATE,
		RC_QUICK_INVALIDPOS,
		RC_QUICK_INVALIDSLOTIDX,
		RC_QUICK_UNKNOWNERROR,
		RC_QUICK_CANNOT_LINK_CHARGE_INVENTAB,		// �κ��丮 ���� �������� ��â�� ����� �� ����!
	};

	enum eSTYLE_RESULT
	{
		RC_STYLE_SUCCESS,
		RC_STYLE_FAILED,
		RC_STYLE_INVALIDSTATE,
		RC_STYLE_INVALIDPOS,
		RC_STYLE_INVALIDSLOTIDX,
		RC_STYLE_UNKNOWNERROR,
	};

	enum eROOM_RESULT
	{
		RC_ROOM_SUCCESS,
		RC_ROOM_FAILED,
		RC_ROOM_NOTEXISTLOBBY,		
		RC_ROOM_NOTEXISTROOM,
		RC_ROOM_ISNOTMATERPLAYER,				//< ������ �ƴϴ�
		RC_ROOM_BEFOREENTERZONE,				//< ���� �������� �ٽ� ��Ŷ�� ������ ���
		RC_ROOM_INVALIDROOMTYPE,				//< �߸��� �� �����̴�.
		RC_ROOM_INVALIDPREVLOBBY,				//< ������ �� ��ȣ�� �߸��Ǿ���.

		RC_ROOM_INVALID_ROOM_TYPE,				//< ��Ÿ���� �߸��Ǿ���.
		RC_ROOM_ALREADY_EXIST_PLAYER,			//< �̹� �����ϴ� �÷��̾�
		RC_ROOM_ALREADY_DOING_TRANSACTION,		//< �̹� �ٸ� Ʈ������� �������̴�.
		RC_ROOM_CANNOT_JOIN_ROOMTYPE,			//< ��� �� �� ���� ��Ÿ���Դϴ�.
		RC_ROOM_NOTEQUAL_PASSWORD,				//< �н����尡 Ʋ���ϴ�.
		RC_ROOM_FULL_ROOM,						//< ���� �����ο��� �Ѿ����ϴ�.
		RC_ROOM_INVALID_MAPCODE,				//< ���ڵ尡 �߸��Ǿ��ִ�.
		RC_ROOM_INVALID_LIMITLEVEL,				//< �������Ѱ��� �߸��Ǿ��ִ�.
		RC_ROOM_INVALID_LIMITCLASS,				//< Ŭ�������Ѱ��� �߸��Ǿ��ִ�.
		RC_ROOM_INVALID_LIMITCLASS_FOR_ME,		//< �ڽ��� Ŭ�������Ѱ��� ���� �ʴ�.
		RC_ROOM_INVALID_LIMITLEVEL_FOR_ME,		//< �ڽ��� ������ ���� �ʴ� ���� �����̴�.
		RC_ROOM_INVALID_LIMIT_NUMBEROFPLAYER,	//< �÷��̾� �� ���� ���� �߸��Ǿ��ִ�.
		RC_ROOM_INVALID_BONUS_VALUE,			//< ���ʽ� ���� �߸��Ǿ��ִ�.
		RC_ROOM_INVALID_FIELD_VALUE,			//< �ʵ� ���� �߸��Ǿ��ִ�.
		RC_ROOM_INVALID_DIFFICULTY_VALUE,		//< ���̵� ���� �߸��Ǿ��ִ�.
		RC_ROOM_INVALID_LASTROOMTYPE,			//< �����Ϸ��� ��Ÿ�԰� ������ ��Ÿ���� Ʋ���ϴ�.
		RC_ROOM_NOTEXISTVILLAGE,				//< ������ �������� �ʴ´�.
		RC_ROOM_ALREADY_READY_STATE,			//< �̹� ���� �����̴�.
		RC_ROOM_ALREADY_NOT_READY_STATE,		//< �̹� ���� ���°� �ƴϴ�.
		RC_ROOM_IS_NOT_ALL_READY,				//< ������� ������°� �ƴϴ�.
		RC_ROOM_INSUFFICIENT_USER_NUM,			//< �̼��� ������ ����� �����մϴ�.
		RC_ROOM_NOT_CMD_FOR_MASTER,				//< ������ �� �� �ִ� ����� �ƴϴ�.
		RC_ROOM_NOT_CMD_FOR_MEMBER,				//< �汸������ �� �� �ִ� ����� �ƴϴ�.
		RC_ROOM_NOT_EXIST_MEMBER,				//< �������� �濡 �������� �ʴ´�.
		RC_ROOM_NOT_EXIST_ROOM,					//< �� ���ǿ� �´� ���� �������� �ʽ��ϴ�.
		RC_ROOM_CANNOT_CREATE_ROOM_FOR_LIMIT,	//< �� ���� ���� ������ �Ѿ����ϴ�.
		RC_ROOM_INVALID_PLAYER_STATE,			//< �߸��� �÷��̾��� �����̴�.
		RC_ROOM_NEED_CHAR_LEVEL_10_FOR_PVP,		//< PVP 10�����̻��� �ʿ��ϴ�.
		RC_ROOM_NEED_EVEN_NUMBER,				//< PVP �ο� ������ ¦������ �մϴ�.
		RC_ROOM_NOTEXISTHUNTINGROOM,			//< ���ù��� �������� �ʴ´�.
		RC_ROOM_INVALID_TEAM,					//< �߸��� �� ��ȣ�Դϴ�.
		RC_ROOM_INVALID_RULE_VALUE,				//< �߸��� �� ���Դϴ�.
		RC_ROOM_INVALID_MODE_VALUE,				//< �߸��� ��� ���Դϴ�.
		RC_ROOM_INVALID_LIMITUSERNUM,			//< �߸��� �ο� ���� ���Դϴ�.
		RC_ROOM_INSUFFICIENT_MONEY,				//< ���� �����մϴ�.
		RC_ROOM_PREVIOUS_MISSION_CLEAR,			//< �̼��� Ŭ���� �ؾ� �մϴ�.
		RC_ROOM_LIMITMAP_FOR_ME,				//< ������ ���ѵ� ���Դϴ�.
		RC_ROOM_SUCCESS_FOR_GENERAL_JOIN,		//< �濡 ������ �� �ִ�.
		RC_ROOM_SUCCESS_FOR_RECOMMEND_JOIN,		//< ��õ�������� �����Ѵ�.
		RC_ROOM_ISNOT_MASTER_OF_PARTY,			//< ��Ƽ���� �ƴϴ�.
		RC_ROOM_DOING_COMPETITION,				//< ������ �������̶� �� �� ���ų� �κ���� ���� �� ����.
		RC_ROOM_CANNOT_CHANGE_OPTION_ROOMTYPE,	//< �ɼ��� �ٲ� �� ���� ��Ÿ���̴�.
		RC_ROOM_INVALID_CHUNTING_LINK_KEY,		//< ��ũ��ų �������ù��� ����.
		RC_ROOM_CAN_JOIN_CHUNTING_THROUGH_LOBBY,//< �κ� ���ľ߸� �������ÿ� �� �� �ִ�.
		RC_ROOM_MEMBERNUM_OF_PARTY_IS_MANY,		//< ��Ƽ���� �ο����� �������� ��Ƽ�� ������ ���� ������ �Ұ����ϴ�.
		RC_ROOM_NEED_ITEM,						//< �ʿ� �������� �������� �ʰ� �ֽ��ϴ�.
		RC_ROOM_CHUNTING_PLAYTIMES_LIMIT,		//< �Ϸ翡 �÷����� �� �ִ� �������� Ƚ���� �ʰ��߽��ϴ�.
		RC_ROOM_ALREADY_EXIST_LINKED_LOBBY,		//< �̹� �������ÿ� ��ũ�� �κ���� �ֽ��ϴ�.
		RC_ROOM_NEED2_CLEAR_PRE_MISSION,		//< ����̼��� ���ž� �� �� �ֽ��ϴ�.
		RC_ROOM_PREVIOUS_QUEST_CLEAR,			//< ����Ʈ�� ���ž� �� �� �ֽ��ϴ�.
		// 2006�� 6�� 1�� ����� ���� 4:56:08
		RC_ROOM_CHAR_BLOCK,						//< ����� ĳ���Ϳ��� ������ �� �����ϴ�.
		RC_ROOM_PARTY_MEMBER_ACCESS,			//< ��Ƽ������� �ʰ��ؼ� ������ �� �����ϴ�.(�ʵ忡 5�� �̻��� ��Ƽ�� ������ �� ����.)
		RC_ROOM_DONT_EXIST_WAYPOINT,			//< ���� ����Ʈ�� �������� �ʾƼ� �̵� �� �� �����ϴ�.
		RC_ROOM_UNABLE_FUNCTION_FOR_CHAOSTATE,	//< ī�����¿����� �̿��� �� ���� ����̴�.

		RC_ROOM_NOT_HAVE_SUPER_MASTER_ITEM,		//< ���۹��� �������� ������ ���� �ʴ�.
		RC_ROOM_ISNOT_PCBANG_USER,				//< PC�� ������ �ƴϴ�.

		RC_ROOM_CANT_RETURN_TO_TUTORIAL_FIELD_IN_LOBBY,		//Ʃ�丮�� �̼� �κ񿡼��� ���ư� �� ����.
		RC_ROOM_CANT_RETURN_TO_VILLAGE_IN_TUTORIAL_MISSION,	//Ʃ�丮�� �̼ǿ����� ������ ���ư� �� ����.

		RC_ROOM_MISMATCH_LEVEL_IN_PARTY,					//< ������ ���尡���� ������ �����ϴ� ��Ƽ���� �ִ�.
		RC_ROOM_MISMATCH_NUMBER_IN_PARTY,					//< ������ ���尡���� �ο��� �ƴϴ�. ���ų� ���ų�...
		RC_ROOM_CAN_NOT_CREATE_PLACE,						//< �δ��� ������ �� �ִ� ��Ұ� �ƴϴ�.
		RC_ROOM_MISMATCH_SAME_CLASS_IN_PARTY,				//< ������ ���尡���� Ŭ���� ������ �ƴϴ�.
		RC_ROOM_MISMATCH_LEVEL_10_WITH_MASTER,				//< ��Ƽ��� ���� 10�̻� ���� ��

		RC_ROOM_LOBBY_WAITTIME_NOT_EXPIRED,					//< �ٸ� ��Ƽ���� �����⸦ ��ٸ��� ���̴�.
		RC_ROOM_NOT_OPEN_TIME,								//< ���� ����(����)�� �� �ִ� �ð��� �ƴϴ�.
#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
		RC_ROOM_CANNOT_CREATE_ROOM_FOR_NO_GUILD,			//< ������ �ƴ϶� ���PVP ���� ������ �� ����.
		RC_ROOM_SHORT_ENTRANCE_CARD_OF_GUILD_PVP,			//< ������� �����մϴ�.
		RC_ROOM_CANNOT_JOIN_ROOM_FOR_DIFF_GUILD,			//< ���� ��尡 �ƴϾ ������ �� ����.
		RC_ROOM_CANNOT_CREATE_ROOM_FOR_LIMIT_USER,			//< ������(5��)�� ���� �ʾ� ���PVP ���� ������ �� ����.
		RC_ROOM_CANNOT_CHANGE_LIMITUSERNUM_IN_GUILD_PVP,	//< ��� PVP�濡���� �������Ѽ��� ������ �� �����ϴ�.
		RC_ROOM_IS_NOT_GUILD_PVP,							//< ��� PVP���� �ƴϴ�.
#endif
		RC_ROOM_CANNOT_USE_ROOM_TITLE,						//< ����� �� ���� ���̸�
		RC_ROOM_CANNOT_WAYPOINT_PLAYERSTATE,				//< ������ �� ���¿��� ��������Ʈ�� �̿��� �� �����ϴ�.	
		RC_ROOM_EXIST_CHAOSTATE_PARTYMEMBER,				//< ��Ƽ���߿� ī�� ������ ������ �ֽ��ϴ�.
		RC_ROOM_NOT_EXIST_ROOM_BY_FITNESS_LEVEL,			//< �ش� ������ �´� ���� �������� �ʴ´�.
		RC_ROOM_CANNOT_ENTER_BEFORE_REWARD_OR_REFUND,		//< (����/ȯ��) �ޱ� ���� ������ �� �����ϴ�.
		RC_ROOM_NOT_OPEN_TIME_FOR_SSQ,						//< ���� RC_ROOM_NOT_OPEN_TIME�� ��ž���̶󼭸�...
		RC_ROOM_CANNOT_JOIN_SOMEONE_PARTY_MEMBER,			//< ��Ƽ�� �� ������ ��ž�� ���� �Ұ����� �����̴�
		RC_ROOM_NEED_UNEQUIP_WEAPON_FOR_ENTER,		        //< �����Ϸ��� ���ݹ��� ���� ���� ���¿��� �մϴ�.
        //__NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__
        RC_ROOM_CANNOT_CREATE_RELAY_ROOM_BY_PORTAL_ENTERED, //< ��Ż�� ���� ����ÿ��� ����۵��� �ʽ��ϴ�.
        //__NA001334_090420_SERVER_SELECT_FACILITY__
        RC_ROOM_SERVER_SELECT_TRANSACTION_CANCELED,         //< ���� ���� ���μ����� ����Ǿ����ϴ�.
        RC_ROOM_SERVER_SELECT_TRANSACTION_CANT_CANCEL,      //< ���� ���� ���μ����� ������ �� �����ϴ�. (�̹� �� �غ�� ��Ȳ�� ���)

        //_NA001385_20090924_DOMINATION_EVENT_CONTROLL
        RC_ROOM_NOT_OPEN_TIME_FOR_DOMINATION,				//< ���� �������� ������ �� �ִ� �ð��� �ƴϴ�.(��, �� ���� �Ұ� �ð��� ���� �õ�)
        RC_ROOM_MISMATCH_JOIN_GUILD,                        //< ���� �� �� �ִ� ��尡 �ƴϴ�. (��,���� ���ش� ��尡 ����õ�, ���� ���� �Ұ� �ð��� ���� �õ�)
        RC_ROOM_LIMIT_MAX_GUILDMEMBER,                      //< ���� �� �� �ִ� �������� ��á��. ( ���, ���� �ð��뿡 ����õ��� MAX �ο��� ���� ���)         
        RC_ROOM_NOT_INCLUDE_GUILD,                          //��尡 ���� �÷��̴�.

        RC_ROOM_CANNOT_BETAKEY,                             //��ŸŰ�� ���� ����ڴ� ��Ż �̵��Ҽ� ����.

        //_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
        RC_ROOM_ISNOT_JOINMEMBER,                           //< �������� �����Ҽ� �ִ� ����� �ƴϴ�.
        // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
        RC_ROOM_BATTLE_GROUND_RESERVATION,                  //< ���� �������̴�.
    };

	enum eLOGOUT_RESULT
	{
		RC_LOGOUT_NORMAL,						//< ���������� �α׾ƿ� �Ǿ����ϴ�.
		RC_LOGOUT_WRONG_VERSION,				//< ������ ��ġ���� �ʽ��ϴ�.
		RC_LOGOUT_AUTH_REQUEST,					//< ���� �α׾ƿ� ��û���� �α׾ƿ� �Ǿ����ϴ�.
		RC_LOGOUT_WRONG_AUTHKEY,				//< �ùٸ� ���������� ��ġ�� �ʾƼ� �α׾ƿ� �Ǿ����ϴ�.
		RC_LOGOUT_HACKING_USER,					//< ������ �߸��� �����͸� �����ؼ� �α׾ƿ� �Ǿ����ϴ�.
		RC_LOGOUT_DB_ERROR,						//< �����Ϳ� ������ �߻��ؼ� �α׾ƿ� �Ǿ����ϴ�.
		RC_LOGOUT_CHAR_BLOCK,					//< ���� ĳ���Ϳ��� �α׾ƿ� ��ŵ�ϴ�.
		RC_LOGOUT_DUPLICATED_USER,				//< �ߺ�UserGuid�� ���� ������ �̹� �����մϴ�. ������ ����� ���� �ߺ��α���!![����]
	};

	enum eZONE_RESULT
	{
		RC_ZONE_SUCCESS,						//< �Ϲ������� ������ ���� ���̴�. ������ �ٸ� ó���� �����ǹǷ�...
		RC_ZONE_FAILED,							//<	�������� (�������� �ʴ�) ������ ���� ����
		RC_ZONE_ALREADY_IN_ZONE,
		RC_ZONE_DISCONNECTED_DBPROXY,			//< db�����ð� ������ �ִ�.
		RC_ZONE_CANT_FIND_ZONE,					//< �ش� ���� �������� �ʽ��ϴ�.
		RC_ZONE_NOT_MATCHED_MAP_VERSION,		//< �ʹ����� �ٸ��ϴ�.
		RC_ZONE_MAP_PROBLEM,					//< �ش� �ʿ� ������ �־� ������ �� �����ϴ�.
		RC_ZONE_ASSERTION_FLOW_VIOLATION,		//< (�����) Ȯ�� �÷ο� ����
        RC_ZONE_NOT_MATCHED_WMO_VERSION,        //< (�����) wmo ������ Ʋ���� (�ʹ����� �ٸ��ϴ�.) added by _NA000000_100623_CHECK_WMO_CHECKSUM_
	};

	enum eCONNECTION_RESULT
	{
		RC_CHARSELECT_OUTOFRANGEINDEX,		//< ĳ�������ε����� �߸��Ѿ��
		RC_CHARINFO_DUPLICATEDREQ,
		RC_CONNECTION_NOTYETSERVICE,		//< ���� ���� �غ����̶� �����Ŀ� �ٽ� ������3
		RC_CONNECTION_REQUESTFROMGAMESERVER,	//< ���� �������� ������ �ڸ�
		RC_CONNECTION_DONOTEXIST_LINKSERVER,
		RC_CONNECTION_ROOM_ALREADY_DOING_TRANSACTION,
		RC_CHARSELECT_MAPCODE_WRONG,			//< ������ ĳ������ ���ڵ尡 �߸���
		// �� ��Ŷ �÷ο쿡 ����ġ ���� ��Ŷ�� ���ŵǾ����ϴ�.
		// ĳ���� ����â���� �� ������ �߻��ϸ� C/S�� ī�װ�,�������� ����ġ ������ �� �ֽ��ϴ�.
		RC_CONNECTION_UNEXPECTED_PACKET,
		RC_CONNECTION_INVALID_CHAR_STATE,		//< ĳ���� ���°� �̻��ϴ�.
		RC_CONNECTION_DUPLICATED_CHAR_NAME,		//< �ߺ��Ǵ� ĳ���� �̸��� �����Ѵ�.
		RC_CONNECTION_CHAR_BLOCK,				//< ���� ĳ���� �Դϴ�.
		RC_CONNECTION_DEL_CHAR,					//< ������ ĳ���� ��

		//
		// enum eDISCONNECT_RESULT���� �̵��� ����
		//
		RC_DISCONNECT_NORMAL,
		RC_DISCONNECT_UNKNOWN,
		RC_DISCONNECT_MOVE_SYNC_BROKEN,
		RC_DISCONNECT_KBMOVE_SYNC_BROKEN,
		RC_DISCONNECT_ATTACK_SYNC_BROKEN,
		RC_DISCONNECT_TOO_LONG_THRUST_DISTANCE,
		RC_DISCONNECT_TOO_MANY_ATTACK_TARGETS,
		RC_DISCONNECT_TOO_FAST_ATTACK,
		RC_DISCONNECT_INCORRECT_ATTACK_SEQUENCE,
		RC_DISCONNECT_INVALID_ATTACK_SEQUENCE,
		RC_DISCONNECT_INCORRECT_WEAPON_ON_STYLE_ATTACK,
		RC_DISCONNECT_INCORRECT_CLASS_ON_STYLE_ATTACK,
		RC_DISCONNECT_NOT_ACQUIRED_STYLE,
		RC_DISCONNECT_SPEED_HACK,
		RC_DISCONNECT_GM_FORCE_DISCONNECT,
		RC_DISCONNECT_CRYPTOGRAPHY_INCORRECT_SNKEY,
		RC_DISCONNECT_CRYPTOGRAPHY_KEYEXCHANGE_TIMEOUT,
		RC_DISCONNECT_CRYPTOGRAPHY_KEYEXCHANGE_NAK,
		RC_DISCONNECT_INCORRECT_VERSION,				//< ������ Ʋ���ϴ�.
		RC_DISCONNECT_DBP_CONNETION_ERROR,				//< DBP�� �������� ���߽��ϴ�
		RC_DISCONNECT_FROM_GM,							//< GM������ ���� ���� �Ǿ����ϴ�.
		RC_DISCONNECT_DUPLICATE_CHARNAME,				//< ĳ�����̸��� �̹� ���ӿ� �ֽ��ϴ�.(�ߺ�)
		RC_DISCONNECT_REMAIN_USER_CONNECTION,			//< ���ο� ĳ���Ͱ� ���÷����ؼ� �����ϴ�.
		RC_DISCONNECT_REQ_AUTH,							//< �������������� ��û�����ؼ� �����ϴ�.
		RC_DISCONNECT_REQ_MASTER,						//< �����ͼ��������� ��û�����ؼ� �����ϴ�.
		RC_DISCONNECT_SERVER_ERROR,						//< ��������(�ٿ�)�� ���� ������ �����ϴ�.
		RC_DISCONNECT_INCORRECT_AUTH_KEY,				//< �߸��� ����Ű�Դϴ�.
		RC_DISCONNECT_DUPLICATE_LOGIN,					//< �ߺ� �α���
		RC_DISCONNECT_MISSION_REWARD_POLICYMENT,		//< �̼� ���� ��å ���� (������ �÷�)
		RC_DISCONNECT_BILLING_EXPIRED,					//< ���� ����
		RC_DISCONNECT_CRACKED_CLIENT,					//< ũ���� Ŭ���̾�Ʈ ������� Ȯ���� ����
		RC_DISCONNECT_IRREGULAR_CLIENT_CYCLE_TICK,		//< �������� �ֱ⸦ ���� Ŭ���̾�Ʈ(���ǵ��� ���ɼ��� ����)
		RC_DISCONNECT_ARMORKIT_NOT_SERVICED,			//< �ش� Ŭ���̾�Ʈ�� ArmorKit�� ���������� �ʽ��ϴ�.
		RC_DISCONNECT_CANT_RECEIVE_CHAR_LIST_FROM_DBP,	//< ĳ���� ����� ���޾Ƽ� �����ϴ� ���
		RC_DISCONNECT_USER_RELEASE,						//< �������� ������ ��Ű�鼭 ���� ���
		RC_DISCONNECT_OVERLAPPED_CONN_FROM_AUTH,		//< �ߺ� ���ӿ� ���� Auth�κ��� ���� ���� ����
		RC_DISCONNECT_INVALID_AUTH_FROM_AUTH,			//< ���� ������ ���� Auth�κ��� ���� ���� ����
		RC_DISCONNECT_LOGIC_VIOLATION_OVER_LIMIT,
		RC_DISCONNECT_NOT_FOUND_DB_USER,				//< DB ������ ã�� ���߽��ϴ�.
		RC_DISCONNECT_NOT_CONNECTED_DB_CHARACTER,		//< �������� ���� DB ĳ���� �Դϴ�.
		RC_DISCONNECTION_INVALID_CHAR_STATE,			//< ĳ���� ���°� �̻��ϴ�.
        //__NA001334_090420_SERVER_SELECT_FACILITY__
        RC_DISCONNECT_FOR_SERVER_SELECT_TRANSACTION,    //< ���� ���� ��û�� ���� �� ���Ӽ����� ����Ǿ����ϴ�.
        //---(NO-ERROR-MESSAGE, Ŭ��� �˾Ƽ� ���Ź��� ������ ������ ���������� �����Ѵ�.)
        // _NA_20100802_SPEED_HACK_DETECTION
        RC_DISCONNECT_SPEED_HACK_SUSPECTED,             //< ���ǵ��� ����� �ǽɵǾ� ���� ���� ����
	};

	enum ePARTY_RESULT
	{
		RC_PARTY_SUCCESS,

		RC_PARTY_PLAYER_NOTEXIST,			//< �׷� �÷��̾ �������� �ʴ´�.
		RC_PARTY_ISNOT_MASTEROFPARTY,		//< ��Ƽ���� �ƴϴ�.
		RC_PARTY_ISNOT_MEMBEROFPARTY,		//< ��Ƽ���� �ƴϴ�.
		RC_PARTY_PARTY_NOTEXIST,			//< ��Ƽ�� �������� �ʴ´�.
		RC_PARTY_REJECT_INVITEMENT,			//< ������ ��Ƽ�ʴ� ��û�� �����ϴ�.
		RC_PARTY_ISNOT_QUALIFIED_LEVEL,		//< ���������� ���� �ʴ´�.
		RC_PARTY_INVALID_ROOMTYPE,			//< �߸��� �� �����̴�.
		RC_PARTY_CANNOT_CHANGE_MASTER,		//< ��Ƽ���� ������ �� ����.
		RC_PARTY_ALREADY_IS_MEMBEROFPARTY,	//< �̹� ��Ƽ�� �ҼӵǾ� �ִ�.
		RC_PARTY_WASNOT_REQUEST_PARTY,		//< ������ ��Ƽ�� ��û���� �ʾҴ�.
		RC_PARTY_INVALID_DATATYPE,			//< ����Ÿ Ÿ���� ���� �ʴ´�.( ��ȿ���� ���� ���̴�. )
		RC_PARTY_PLAYER_NOTEXIST_TO_FIELD,	//< �÷��̾ �ʵ忡 �������� �ʴ´�.
		RC_PARTY_CANNOT_FIND_TARGET,		//< Ÿ���� ã�� �� ����.
		RC_PARTY_TARGET_ISNOT_ENEMY,		//< Ÿ���� �Ʊ��̴�.
		RC_PARTY_PLAYER_OBSERVER_MODE,		//< �÷��̾ ������� ��Ƽ�� �Ҽӵ��� �ʴ´�.
		RC_PARTY_PARTYMEMBER_NUM_FULL,		//< ��Ƽ�� ���� �ִ�ġ�� ���̻� ��Ƽ���� ���� �� ����.
		RC_PARTY_NEXTMASTER_NOTEXIST,		//< ��Ƽ������ �Ӹ��� ����� �������� �ʴ´�.
		RC_PARTY_INVALID_ZONESTATE,			//< �÷��̾��� �����°� �̻��ϴ�.(���� ã�� ���� ����.)
		RC_PARTY_REJECT_SUMMON,				//< ��ȯ��û�� �����ϴ�.
		RC_PARTY_SKILLINFO_IS_NULL,			//< �׷� ������ �������� �ʴ´�.
		RC_PARTY_COOLTIME_ERROR,			//< ��Ÿ���� ������ �ʾҴ�.
		RC_PARTY_MASTER_NOTEXIST,			//< ��Ƽ���� �������� �ʴ´�.		
		RC_PARTY_UNKNOWN_REASON,			//< ��Ÿ����...
		RC_PARTY_INVALID_PLAYER_STATE,		//< ��밡 ��ȿ���� ���� �����̴�.
		RC_PARTY_GUIDANCE_NOTEXIST,			//< ���̴����� �������� �ʴ´�.
		RC_PARTY_ISNOT_MASTEROFGUIDANCE,	//< ���̴������� �ƴϴ�.
		RC_PARTY_ISNOT_MEMBEROFGUIDANCE,	//< ���̴������� �ƴϴ�.
		RC_PARTY_CANNOT_CREATE_GUIDANCE,	//< ���̴����� ������ �� ����.
		RC_PARTY_CHANGE_ITEM_DISTRIBUTION_FAIL,		//<������ �й� Ÿ�� ���� ����
		RC_PARTY_CANNOT_INVITE_HOSTILITY,
		RC_PARTY_LEAVE_BY_NORMAL_DESTROY,	//��Ƽ Ż�� �������� ��Ƽ ������ ���� �̷����
		RC_PARTY_LEAVE_BY_HOSTILITY_RELATION, //��Ƽ Ż�� ��Ƽ�ȿ� �ִ� �ٸ� ������ ���� ���迡 ���� �̷����
        RC_PARTY_CANNOT_INVITE_NEW_MEMBER,  // < ���ο� ��Ƽ���� ���� �� ���� ���� __NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__
        // _NA_0_20110210_INVITE_PARTY_BUG
        RC_PARTY_BEING_INVITE_REQUEST,      // ��Ƽ �ʴ� ��û ���̴�.
        RC_PARTY_BEING_INVITE_RESPONSE,     // ��Ƽ �ʴ� �޴� ���̴�.
        RC_PARTY_INVITE_RESPONSE_TIME_OVER, // ��Ƽ �ʴ� ���� �ð��� ������.(�ð����� : 8 ��)
        // _NA_20110630_BATTLE_ZONE_RENEWAL
        RC_PARTY_ALREADY_DOING_TRANSACTION, // ���� �̹� Ʈ������ ���̴�.
        RC_PARTY_PORTAL_ENTERANCE_MEMBER,   // ��Ż ���� ���� ����Դϴ�. - _NA003129_QUEST_PORTAL_CONVENIENCE_EXTENTION 
        // _NA_20110630_BATTLE_ZONE_RENEWAL
        RC_PARTY_FAIL_CREATE_PARTY_ROOM,    // ��Ƽ�� ������ ���� : 39
        RC_PARTY_FAIL_DESTROY_PARTY_ROOM,   // ��Ƽ�� ���ſ� ���� : 40
        RC_PARTY_FAIL_DESTROY_PARTY_LIST,   // ��Ƽ �� ����Ʈ ���ſ� ����
        RC_PARTY_FAIL_LEAVE_PARTY_ROOM,     // ��Ƽ �����⿡ ����
        RC_PARTY_FAIL_JOIN_PARTY_ROOM,      // ��Ƽ�� ������ ����
        RC_PARTY_FAIL_START_MISSION,        // ��Ƽ�� �̼� ���� ����
        RC_PARTY_FAIL_RESET_MISSION_CODE,   // �̼� �ڵ� �ʱ�ȭ�� ����
        RC_PARTY_FAIL_CHANGE_ROOM_OPTION,   // ��Ƽ�� �ɼǺ��濡 ����
        RC_PARTY_FAIL_CREATE_ROOM,          // �� ������ ���� �Ͽ���
        RC_PARTY_KICKOUT_PARTY_ROOM,        // ������ ��Ƽ�뿡�� KICK
        RC_PARTY_NOT_EXIST_PARTY,           // ��Ƽ�� ���� ���� ����
        RC_PARTY_NOT_INCORRECT_MAPCODE,     // �߸��� ���ڵ�(�̼��ڵ�) : 50
        RC_PARTY_HAVE_NOT_REQUIRE_ITEM,     // �̼� ������ �ʿ��� ������ ����
        RC_PARTY_NOT_FOUND_PARTYROOM,       // ��Ƽ�� ������ ���� ���� ����
        RC_PARTY_IS_NOT_PARTY_ROOM,         // �� ������ ��Ƽ�� �ƴ�
        RC_PARTY_INVALID_PARTY_ROOM_MAPCODE,// �߸��� ��Ƽ���� ���ڵ� ����
        RC_PARTY_NOT_MISSION_REWARD,        // �̼� ������ �޾ƾ� �Ѵ�
        RC_PARTY_NOT_ALLMEMBER_READY,       // ��� ����� �غ� ���°� �ƴϴ�.
        RC_PARTY_NOT_OPEN_TIME,             // �ش� ���� ���� �ִ� �ð��� �ƴϴ�.
        RC_PARTY_INVALID_ITEM,              // ������ ��� ������ ���� �ʴ´�.
        RC_PARTY_NOT_EXIST_ITEM,            // �ʿ��� �������� ������ ���� �ʴ�.
        RC_PARTY_ROOM_NEED_UNEQUIP_WEAPON,  // �뿡 �����Ҷ� ��� ������ �� ���Ǿ��ִ�. : 60
        RC_PARTY_CANNOT_JOIN_SOMEONE_MEMBER,// �뿡 �������� ���ϴ� ��� ����
        RC_PARTY_NOTEQUAL_PASSWORD,         // �н����尡 �ٸ���
        RC_PARTY_NOT_FOUND_USER,            // ������ ã�� �� ����.
        RC_PARTY_NOT_MATCHING_USER_LEVEL,   // ���� ���ǿ� ���� �ʴ´�.
        RC_PARTY_NOT_CAHNGE_OPTION_PLAYING, // �̼� �÷��� �߿��� ������ �� ����. : 65
        RC_PARTY_PREVIOUS_QUEST_CLEAR,      // ���� ����Ʈ�� ���� Ŭ���� �ؾ� �Ѵ�.
        RC_PARTY_NEED_CLEAR_PRE_MISSION,    // ���� �̼��� ���� Ŭ���� �ؾ� �Ѵ�.
        RC_PARTY_FAIL_CHECK_TERMS,          // ���� �˻翡�� ���� �Ͽ���.
        RC_PARTY_FAIL_MAX_MEMBER_COUNT,     // �ɹ� ���ڰ� �ִ�ġ �� �����Ͽ���.
        RC_PARTY_INVALID_PARTY_STATE,       // ��Ƽ�� ���°� ������ ���̴�.
        RC_PARTY_NOT_FOUND_ROOM,            // ���� ���� ���� �ʴ´�.
        RC_PARTY_DIFFER_TEAM,               // ���� �� ���� �ٸ���.
        RC_PARTY_ALREADY_USE_IN_CHAOS_ZONE, // �̹� ī�������� �̿� ���̴�.
    };

	enum eGM_RESULT
	{
		RC_GM_ROOM_NOT_EXIST,				//< ���� �������� �ʴ´�.
		RC_GM_INVALID_ARGUMENT,				//< �߸��� ���ڰ� �̴�.
		RC_GM_ISNOT_GM,						//< GM�� �ƴϴ�
		RC_GM_NOTEXIST_USER,				//< USER�� �������� �ʴ´�.
		RC_GM_CANNOT_RESURRECTION,			//< ��Ȱ�Ҽ� ����.
		RC_GM_CANNOT_RECOVERY,				//< ȸ���Ҽ� ����.
		RC_GM_PLAYER_NOTEXIST,				//< �׷� �÷��̾ �������� �ʴ´�.
		RC_GM_NOT_KILL_MYSELF,				//< �ڱ� �ڽ��� ���ϼ� ����.
		RC_GM_CURROOM_ISNOT_CHUNTING,		//< ���� ��Ÿ���� ���������� �ƴϴ�.
		RC_GM_MOVEZONE_FAIL,				//< ���̵��� ���� �߽��ϴ�.
		RC_GM_USER_STATE_INVALID,			//< ������ ���°� �̻��ϴ�.
		RC_GM_PLAYER_NOTEXIST_TO_FIELD,	
		RC_GM_PLAYER_NOTEXIST_TO_ZONE,
		RC_GM_CANNOT_MOVE_TO_BATTLEROOM,	//< �������� �̵��� �������� �ʴ´�.
		RC_GM_CMD_SUCCESS,					//< GM��ɾ� ����.
		RC_GM_NOTEXIST_SCRIPT,				//< �ش� ��ũ��Ʈ ������ �������� �ʽ��ϴ�.
		RC_GM_INVALID_VERSION_TYPE,			//< �ش� Ÿ���� ������ �������� �ʽ��ϴ�.
		RC_GM_NOT_EXIST_LIMITED_INDEX,		//< �������� �ʴ� ����Ƽ�� �ε��� �Դϴ�.
		RC_GM_NOT_MATCH_FATE_RATE,			//< ���� �ʴ� ����Ʈ ������ ����Դϴ�.
		RC_GM_CANNOT_FIND_FATE_INDEX,		//< ����Ʈ ������ �ε����� ã�� �� �����ϴ�.
		RC_GM_CMD_FAILED,					//< GM��ɾ� ����.
	};

	enum eBATTLE_RESULT
	{
		RC_BATTLE_SUCCESS,

		RC_BATTLE_PLAYER_NOTEXIST_TO_FIELD,					//< �÷��̾ �ʵ忡 �������� �ʴ´�.
		RC_BATTLE_INVALID_MAINTARGET,						//< ����Ÿ���� ã�� �� ����.
		RC_BATTLE_PLAYER_STATE_WHERE_CANNOT_ATTACK_ENEMY,	//< �����ڰ� ���� ������ �� ���� �����̴�.
		RC_BATTLE_VKR_RELOAD_COUNT_LACK,					//< ��Ű�� �������� ���� 
		RC_BATTLE_TRAGET_STATE_WHERE_CANNOT_ATTACKED,		//< Ÿ���� ���ݹ��� �� ���� �����̴�.
		RC_BATTLE_OUT_OF_RANGE,								//< ����Ÿ���� ���ݻ�Ÿ����� ���
		RC_BATTLE_INVLIDPOS,								//< �߸��� ��ġ������ �Խ��ϴ�.
		RC_BATTLE_THRUST_DIST_OVER,							//< Ŭ�󿡼� ������ �и��� �Ÿ��� �ʹ� ���.(��ŷ ���ɼ�)
		RC_BATTLE_ALREADY_DOING_ACTION,						//< �̹� �������� �����̴�.
		RC_BATTLE_BASEINFO_NOTEXIST,						//< BaseStyleInfo�� ����.
		RC_BATTLE_STYLECODE_WHERE_DONOT_SELECT,				//< �������� ���� ��Ÿ���ڵ�
		RC_BATTLE_CHAR_CLASS_LIMIT,							//< Ŭ���� ����(�� ��Ÿ���� ����� �� ���� ĳ���� Ÿ���̴�)
		RC_BATTLE_WEAPON_LIMIT,								//< ���� ����
		RC_BATTLE_FIELD_IS_NULL,
		RC_BATTLE_ATTACKER_LEVEL_IS_LOW,					//< ������ ������ ���Ƽ� PK�� �� �� ����.
		RC_BATTLE_TARGET_LEVEL_IS_LOW,						//< Ÿ�� ������ ���Ƽ� PK�� ���� �� ����.
		RC_BATTLE_ISNOT_PK_ZONE,							//< PK�� ������ ������ �ƴϴ�.
		RC_BATTLE_ISNOT_PK_SERVER,							//< PK�� ������ ������ �ƴϴ�.
		RC_BATTLE_SERVER_STATE_IMPOSSIBLE_PK,				//< PK�� �Ұ����� ���������̴�.
		RC_BATTLE_PK_IMPOSSIBLE_TARGET_TYPE,				//< PK�� �Ұ����� Ÿ��Ÿ���̴�.
		RC_BATTLE_ALLIENCE_GUILD_CHARACTER,					//< ���� ��� ĳ�����̹Ƿ� PK �Ұ���

		RC_BATTLE_ISNOT_TROUBLED_PART,						//< Ÿ���� ���� ������ �ƴϴ�
		RC_BATTLE_PLAYER_TILE_INVLIDPOS,					//< ����� Ÿ���� �������� �ʴ´�.
		RC_BATTLE_FAILED,									//< ��Ÿ������ ����
		RC_BATTLE_SAME_GUILD_CHARACTER,						//< ���� �����̴�.
		RC_BATTLE_SAME_PARTY_CHARACTER,						//< ���� ��Ƽ���̴�.

        //__NA_000921_20080227_TROUBLED_PARTS               //< Ŭ���̾�Ʈ ��� ���ҽ� ������ ������
        RC_BATTLE_ISNOT_CHAO_PART,							//< ī�� ������ ������ �ƴϴ�.	//��빫
        RC_BATTLE_ISNOT_TROUBLED_ZONE,						//< ���� ������ ������ �ƴϴ�.	//��빫
        RC_BATTLE_IMPOSSIBLE_PACKET_DATA,					//< ��Ŷ �����Ͱ� ���Ŀ� ���� �ʴ´�. //��빫
    };

	enum eCONDITION_RESULT
	{
		RC_CONDITION_SUCCESS,
		RC_CONDITION_ALREADY_SAME_CONDITION,				//< �̹� ���� �����̴�.
		RC_CONDITION_INVALID_CONDITION,						//< �������� �ʴ� �����̴�.
		RC_CONDITION_DOING_ACTION,							//< �ٸ� ������ �������̴�.
		RC_CONDITION_DRAGON_TRANSFORMATION_LIMIT,			//< �巡�� ���� ���¿����� ������ȯ �Ұ�
		RC_CONDITION_FAILED,
	};

	enum ePVP_RESULT
	{
		RC_PVP_SUCCESS,
		RC_PVP_NOT_AT_ZONE,
		RC_PVP_INVALID_USER_STATE,
	};

	enum eSUMMON_RESULT
	{
		RC_SUMMON_SUCCESS,
		RC_SUMMON_INVALID_COMMAND,				//< ������������ Ÿ���� ����̴�.
		RC_SUMMON_SUMMONED_NPC_NOTEXIST,		//< ��ȯü�� �������� �ʴ´�.
		RC_SUMMON_SAME_COMMAND,					//< �̹� ������ ����̴�.
        RC_SUMMON_CANNOT_ATTACK_PLAYER,			//< �ش� �÷��̾ ������ �� ����.
        //_NA001385_20090924_DOMINATION_SKILL
        RC_SUMMON_INVALID_PLAYER,			    //< ������ ã���� ����.(�ý��ۿ���, ��Ʈ�� ó�� �Ұ�)
        RC_SUMMON_INVALID_FIELD,			    //< ������ �ʵ忡 �������� �ʴ´�.(�ý��ۿ���, ��Ʈ�� ó�� �Ұ�)
        RC_SUMMON_INVALID_DOMINATION_FIELD,     //< ������ ������������ �������� �ʴ´�. (�Ϲ� �ʵ忡�� ��ȯ�� ��ɾ� ����)
        RC_SUMMON_INVALID_CRYSTALWARP,          //< ���� ��ȯü�� �������� �ʴ´�. (��ȯ�� ��� ���� ��ȯü�� ������)
        RC_SUMMON_CANNOT_WARP_CMD,              //< 2���� �������� ����� ��ġ ���� �ʾҴµ� ������ ���������� ���� ��.
        RC_SUMMON_FAIL_WARP_CMD = 10,           //< ��ȯü ����� ���� ���� ���࿡ �����ߴ�.
        RC_SUMMON_INVALID_GUILD,                //< ũ����Ż���� ��ȯ��尡 ���� ���� �ʽ��ϴ�. (��ȯ�� ��� ���� ��带 ã���� ����.)
        RC_SUMMON_FAIL_CREATE_SKILL,            //< ũ����Ż���� ��ȯ��ų(����,����) ��뿡 �����Ͽ���. (��ų ��� �������� ���� �߻�, Ŭ�� MSG_CG_SKILL_ACTION_NAK) 
        RC_SUMMON_COOLTIME_ERROR,               //< ũ����Ż���� ��Ÿ���� �����ִ�.             (���ȯ �����̰� ���� ������)
        RC_SUMMON_CRYSTALWARP_COUNTFULL,        //< �̹� 2���� ũ����Ż������ ��ȯ �Ǿ��ִ�.    (3��°�� ũ����Ż ���� ��ȯ �õ���)
        RC_SUMMON_INVALID_SUMMONED,             //< ������ ��ȯ���� �ƴϴ�
        RC_SUMMON_RECALL_MAX,                   //��ȯ �ִ���� �ʰ� �Ͽ���.
	};

	enum eGUILD_RESULT
	{
		RC_GUILD_SUCCESS,
		RC_GUILD_FAILED,
		RC_GUILD_INVALID_STATE,
		RC_GUILD_CANNOT_CREAT_GUILD_STATE,
		RC_GUILD_CANNOT_CREAT_LEVEL_STATE,
		RC_GUILD_CANNOT_CREAT_MONEY_STATE,
		RC_GUILD_CANNOT_CREAT_SAMENAME_STATE,		//< ������ �̸��� ��尡 �ֽ��ϴ�

		RC_GUILD_CANNOT_DESTROY_STATE,
		RC_GUILD_CANNOT_DESTROY_NOT_MASTER_STATE,
		RC_GUILD_CANNOT_DESTROY_EXIST_MEMBER_STATE,

		RC_GUILD_NOT_EXIST_TARGET_STATE,			//< �ʴ��� ����� �������� �ʴ´�.	10
		RC_GUILD_CANNOT_INVITE_STATE,				//< �ʴ��� �� ���� �����̴�.
		RC_GUILD_DONT_HAVE_RIGHTS_STATE,			//< �ʴ��ϴ� ������ ����.
		RC_GUILD_FULL_MEMBER_STATE,					//< �̹� ������ ���� �ο��� ��á��.

		RC_GUILD_NOT_EXIST_HOST_STATE,				//< �ʴ��� ����� �������� �ʴ´�.
		RC_GUILD_CANNOT_WITHDRAW_STATE,
		RC_GUILD_NOT_EXIST_GUILD,					// ��尡 �������� �ʽ��ϴ�.
		RC_GUILD_CANNOT_JOIN_STATE,
		RC_GUILD_CANNOT_CHANGEPOSITION_STATE,
		RC_GUILD_CANNOT_SELECT_GUILD_STATE,			//< ��� ������ ������ �� ���� �����̴�.

		RC_GUILD_SUCCESS_SELECT,					//< �̹� �����ϴ� ��忡 ���� �����Ͽ���.(���������� ���)	20
		RC_GUILD_DISCONNECTED_GUILD_SERVER,
		RC_GUILD_NEED_SELECT,						//< ��尡 ��� select�� �ʿ䰡 �ִ�.(���������� ���)
		RC_GUILD_WAIT_A_WHILE_THEN_TRY_AGAIN,		//< ����� �ٽ� �õ��� �ֽñ� �ٶ��ϴ�.
		RC_GUILD_ALREADY_SELECT_QUERY,				//< �̹� ��� ���� ��û��(���������� ���)

		RC_GUILD_CANNOT_KICK_STATE,
		RC_GUILD_CANNOT_KICK_RIGHT,

		RC_GUILD_CANNOT_CHANGENOTICE_STATE,

		// ��� ����Ʈ ���� Result����
		RC_GUILD_NOT_ENOUGH_UP,						//< UP �����մϴ�.
		RC_GUILD_NOT_ENOUGH_GP,						//< GP �����մϴ�.
		RC_GUILD_CANNOT_RANKUP_MONEY_STATE,			//< �� �����Ͽ� ��ũ�� �Ұ���	30

		RC_GUILD_UNABLE_FUNCTION_FOR_CHAOSTATE,		//< ī�� ĳ���ʹ� �̿��� �� ���� ����̴�
		RC_GUILD_INVALID_PLAYER_STATE,				//< ��밡 ��ȿ���� ���� �����̴�.

		//��� 2�� �����
		RC_GUILD_LOW_GRADE_STATE,					// ��� ���� ���� ��� ����[������]
		RC_GUILD_DONT_RIGHTS_DUTY_STATE,			// ��� ���� ���� ��å �ƴ�[������]
		RC_GUILD_NOT_GUILD_MEMBER,					// ��� ����� �ƴϴ�
		RC_GUILD_CHANGE_NOTICE_LENGTH_OVERFLOW,		// ��� ���� ���̰� �� ���
		RC_GUILD_CURRENT_NONE_MASTER,				// ��� ������ ���� ���� 
		RC_GUILD_INVITE_PLAYER_ALREADY_MEMBER,		// �ʴ��� ����� �̹� �ٸ� ��� �����	38

		RC_GUILD_CREATE_CAMP_FAILED,				// ��� �Ʒô� ���� ����
		RC_GUILD_CAMP_NUM_OVERFLOW,					// ��� �Ʒô� ���� ����

		RC_GUILD_MARK_INVALID_TYPE,					// ��� ��ũ ���Ŀ� ��߳�
		RC_GUILD_MARK_CREATE_FAILED,				// ��� ��ũ ���� ����
		RC_GUILD_MARK_NOT_READY,					// ��� ��ũ ���� �غ���� ����
		RC_GUILD_NOT_EXIST_GUILD_SEAL,				// ��� ���� ����
		RC_GUILD_MARK_ALEADY_EXIST,					//����� ��帶ũ�� ������
		RC_GUILD_MARK_LEVEL_LOW,					//��� ��ũ�� ����ϱ⿡�� ������ ����
		RC_GUILD_CHANGE_MARK_NOT_RIGHT,				// ��� ��ũ ���� ���� ����
		RC_GUILD_CHANGE_MARK_NOT_DUTY_STATE,		// ��� ��ũ ���� �Ұ� ��å
		RC_GUILD_MARK_DELETE_FAILED,								// ��� ��ũ ���� ����

		RC_GUILD_DESTROY_PENALTY_TIME,				// ��� ��ü ���Ƽ Ÿ�� ������
		RC_GUILD_WITHDRAW_PENALTY_TIME,				// ��� Ż�� �г�Ƽ Ÿ�� ������
		RC_GUILD_KICK_PENALTY_TIME,					// ��� ���� �г�Ƽ Ÿ��
		RC_GUILD_RECOVERY_FAILED,					// ��� ���� ����
		RC_GUILD_DELCHK_DESTROY_WAIT,				// ��� ���� �������

		RC_GUILD_NOT_FOUND_GUILD_INFO_FOR_GRADE,			// �ش� ��� ��޿� ��� ������ ���� ���� �ʽ��ϴ�.
		RC_GUILD_MAX_GUILD_RANK,							// ���� ��� ����� �ְ� ����Դϴ�.

		RC_GUILD_HAVE_NOT_RIGHT,							// ��� ������ �����ϴ�.
		RC_GUILD_CANNOT_COMMAND_MYSELF,						// �ڱ� �ڽſ��� ����� ������ �����ϴ�.
		RC_GUILD_CANNOT_CHANGE_DUTY_BECAUSE_SAME_DUTY,		// ���� ��å�� �ٲٷ��� ��å�� ���Ƽ� ��å�� �ٲܼ� �����ϴ�.
		RC_GUILD_CANNOT_CHANGE_DUTY_BECAUSE_FULL_MEMEBER,	// �ش� ������ �ο��� �� ���� ��å ������ �� �� �����ϴ�.
		RC_GUILD_CANNOT_CHANGE_DUTY_BECAUSE_INVALID_DUTY,	// �ùٸ� ��å�� �ƴϿ��� ��å�� ������ �� �����ϴ�.
		RC_GUILD_HAVE_NOT_RIGHT_TARGET_USER,				// Ÿ�������� �ش� ������ �����ϴ�.
		RC_GUILD_INVALID_DELEGATE_RIGHT,					// �̻��� ���� ���Ӱ� �Դϴ�.
		RC_GUILD_PLAYER_ALEADY_DUTY,						// ��� ��å �̹� �����ϴ� �÷��̾�
		RC_GUILD_CHANGE_MEMBER_DUTY_FAILED,					// ��� ��� ��å ���� ����
		RC_GUILD_CHANGE_MEMBER_RIGHT_FAILED,				// ��� ��� ���� ���� ����
		RC_GUILD_ALREADY_EXIST_DUTY,						// �̹� �����ϴ� ��å �Դϴ�.
		RC_GUILD_CANNOT_CHANGE_MASTER_IN_DAY_14,			// �渶�� �������� 14�� ������ �ٽ� ������ �� ����.
		RC_GUILD_CANNOT_CHANGE_DUTY_BY_WAREHOUSE_USEING, //��� â�� ������� ������ ��å�� ������ �� ����.

		RC_GUILD_SELECT_CHAR_NO_GUILD_MEMBER,				// ��� ���� ���� �������� ������ �ƴ�
		RC_GUILD_CREATE_CORPS_FAILED,						// ��� ���� ���� ����
		RC_GUILD_CORPS_NUM_OVERFLOW,						// ��� ���� ���� ����
		RC_GUILD_MEMBER_NOT_HEAD_CORPS,						// ������ ���ܺ���(���ұ���) �Ҽ��� �ƴ�.
		RC_GUILD_MEMBER_NOT_SOLDIER,						// ������ �������� �ƴϴ�.
		RC_GUILD_CORPS_NOT_FOUND,							// ��� ������ �����ϴ�.
		RC_GUILD_ALREADY_GUILD_MEMBER,						// �̹� ��忡 ������ ��� �Դϴ�.
		RC_GUILD_ALREADY_EXIST_COMMANDER,					// �̹� ��忡 �ش� �������� �����մϴ�.
		RC_GUILD_ALREADY_EXIST_TRAINING_LEADER,				// �̹� ��忡 �ش� �Ʒô����� �����մϴ�.
		RC_GUILD_CANT_CHANGE_COMMANDER_BECAUSE_COMMANDER,	// �������� ���������� ��å ���� �� �� �����ϴ�.
		RC_GUILD_CORPS_FULL_MEMBER,							// ������ �ο��� �� á���ϴ�.
		RC_GUILD_FAILED_BY_NOT_CONNECT_SERVER,				// �������� ������ ������ �ش� ��� �۾��� ���� �Ͽ����ϴ�.
		RC_GUILD_INVALID_DUTY_IN_BASE_CORPS,				// ���ұ��ܿ����� ����� �� ���� ��å �Դϴ�.  
		RC_GUILD_NOT_SOLDIER,								// �������� �ƴմϴ�.
		RC_GUILD_CANNOT_CHANGE_CORPS_BECAUSE_SAME_CORPS,	// ���� ���ܳ������� ���ܰ� �̵��� �� �� �����ϴ�.
		RC_GUILD_ALREADY_EXIST_REAR_CAMP,					// �̹� ����ķ���� �����մϴ�.
		RC_GUILD_NOT_EXIST_REAR_CAMP,						// ����ķ���� �������� �ʽ��ϴ�.
		RC_GUILD_CANNOT_LEAVE_MASTER,						// �渶�� Ż�� �Ұ��� �մϴ�.
		RC_GUILD_INVALID_FUNCTION_IN_BASE_CORPS,			// ���ұ��ܿ����� ����� �� ���� ��� �Դϴ�.
		RC_GUILD_EXIST_MEMBER,								// ��忡 ����� �����մϴ�.
		RC_GUILD_ALREADY_EXIST_GUILD,						// �̹� ��尡 �����մϴ�.
		RC_GUILD_CANNOT_LEVELUP,							// �������� �� �� �����ϴ�.
		RC_GUILD_KICK_GUILD_BY_LEVELUP,						// �Ʒú��� ������ 31�� �����Ͽ� ��带 Ż���ŵ�ϴ�.
		RC_GUILD_NOT_CORPS_MEMBER,							// ���ܿ� ���� ����� �ƴմϴ�.
		RC_GUILD_NOT_BASE_CORPS,							// ���ұ����� �ƴմϴ�.
		RC_GUILD_CANT_CHANGE_CAMP_SOLDIER_BY_LEVEL,			// �������� ������ �Ʒú����� ������ �Ұ��� �մϴ�.
		RC_GUILD_CORPS_PENALTY_TIME,						// ��� ���� �г�Ƽ Ÿ�� ������ �Դϴ�.
		RC_GUILD_NOT_JOIN_GUILD,							// ��忡 �������� �ʾҽ��ϴ�.
		RC_GUILD_ALREADY_LOGIN_MEMBER,						// �̹� �α����� ��� �Դϴ�.
		RC_GUILD_OVER_TOTALINFO_REQ_CNT,					// ��� ��Ż���� ��ûȽ���� ���Ѽ�ġ�� �Ѿ����ϴ�.
		RC_GUILD_CANNOT_JOIN_CAMP_SOLDERY_BY_LEVEL,			// ������ ���� �Ʒú����� �����Ҽ� �����ϴ�. ( 16���� �̻��� ĳ���ʹ� �Ʒú����� ������ �� �����ϴ�. )
		RC_GUILD_CANNOT_MOVE_CORPS_BY_CORPS_PENALTY,		// ���� �г�Ƽ�� ���� �г�Ƽ�� �����ɶ� ���� ���� �̵��� �� �� ����.

        RC_GUILD_STRING_OVERFLOW,							// ��� ���ڿ� ���� �����÷ο�

        RC_GUILD_WAREHOUSE_UNABLE_CURR_GRADE,              //��� ����� ��� â�� �̿��� �� ����
		RC_GUILD_WAREHOUSE_ALEADY_USGIN_PLAYER,            //��� â�� ������� ������ ����
		RC_GUILD_WAREHOUSE_PLAYER_DATA_DISCORD,            //������ ���â�� ������ ���� ����� ��� â�� ������ ����ġ
		RC_GUILD_WAREHOUSE_INVENMONEY_LOW,					  //������ �κ��� �ִ� ���� ������ â�� �Ա��Ҽ� ����.
		RC_GUILD_WAREHOUSE_MONEYOUT_NOENRIGHT,			 //������ ��� â�� ���� ����� ������ ����.
		RC_GUILD_WAREHOUSE_INVENMONEY_LIMIT,                  // ���� �κ��� ���� �Ѱ�ġ(42��)�� �ʰ��Ѵ�.
		RC_GUILD_WAREHOUSE_GET_CANNOT_DB,						//  ��� â�� ���� ����
		RC_GUILD_CANNOT_WITHDRAW_BY_WAREHOUSE_USEING,	//��� â�� ������̿��� Ż��/������ �� ���� ����.
		RC_GUILD_RELATION_LIST_ERROR,	                    // ����� ������ ���� ����Ÿ ����
		RC_GUILD_RELATION_ME_FULL,			                // �ڽ��� �ΰ��� �ϴ� ���� ����Ʈ�� �����̴�.
		RC_GUILD_RELATION_OTHER_FULL,                    // ������ ���� ����Ʈ�� �����̴�.
		RC_GUILD_RELATION_CHANGE_RIGHT_NOT,       // ��� ���踦 ������ ������ ����.
		RC_GUILD_RELATION_REMAIN_GUILD,                // ��� ���谡 �����־ ��带 �ػ��� �� ����.
		RC_GUILD_RELATION_ALEADY_SET,                    // ��� ���踦 �������� ���� �̹� �ٸ� ���谡 �����Ǿ� �ִ�.
		RC_GUILD_RELATION_ALLIENCE_TYPE1_FAILED,  // ������ �������� ��尡 ���� ���� ���踦 �ΰ� �ִ� ���� ���Ͱ���� ���� ���踦 ���� �� ����.
		RC_GUILD_RELATION_ALLIENCE_TYPE2_FAILED,  // ������ �������� ��尡 ���� ���� ���踦 �ΰ� �ִ� ���� ���Ͱ���� ���� ���踦 ���� �� ����.
		RC_GUILD_RELATION_HOSTILITY_FAILED,            // ���븦 �������� ��尡 ���� ���� ���踦 �ΰ� �ִ� ���� ���Ͱ���� ���� ���踦 ���� �� ����.
		RC_GUILD_RELATION_BOTH_HOSTILITY_FAILED,  // ��� ���� �ڽ��� ��尡 �Ϲ��� ���� ���� �Ǵ� ���� ���� ���谡 �ƴϱ� ������ �ֹ� ���� ���踦 ���� �� ����.
		RC_GUILD_RELATION_NONE_DELETE_NOT,         // ��� ���� �ƹ� ���踦 �ΰ� ���� �ʱ� ������ ���� ������ �� �� ����.
		RC_GUILD_RELATION_GUILD_DESTROY_WAIT,     // ��� �ػ� ������̶� ��� ���踦 ���� �� ����.
		RC_GUILD_RELATION_GUILD_GRADE_LOW_ME,	// �ڽ��� ��� ����(���)�� ���� ���踦 ���� �� ����.
		RC_GUILD_RELATION_GUILD_GRADE_LOW_OTHER, // ���� ����� ����(���)�� ���� ���踦 ���� �� ����.
		RC_GUILD_RELATION_DELETE_FAILED,				//��� ���� ���� ����
		RC_GUILD_RELATION_DELETE_FAILED_BY_TIME,  //���� ���� ������ ���� ������ ������ �� 1�ð��� ������ ������ �� �ִ�.
        RC_GUILD_RELATION_CHANGE_FAILED_DOMINATION, // ������ �����߿��� �����踦 ������ �� ����. //_NA_006826_20130722_DOMINATION_RENEWAL

        RC_GUILD_NOTICE_CHNAGE_NONE_RIGHT,				// ��� ����� 2������ �ȵǰų� �÷��̾ ���� ���� ������ ��� ��� ������ ������ �� ����.
		RC_GUILD_DESTROY_WAIT_CANNOT_NONE_RIGHT,        //��� �ػ� ������ ����.
		RC_GUILD_DESTROY_WAIT_CANNOT_MEMBER_EXIST,      //��� ����� �����Ͽ� �ػ��� �� ����.
		RC_GUILD_DESTROY_WAIT_CANNOT_RELATION_REMAIN,   //��� ���谡 (�Ϲ��� ������� ����)�����־ �ػ��� �� ����.
		RC_GUILD_DESTROY_WAIT_CANNOT_GUILDWAREHOUSE_ITEM_REMAIN, //��� â�� ������ �Ǵ� ������ �����־ �ػ��� �� ����.
        //__NA000000_20081106_GUILD_NAME_CHANGE
		RC_GUILD_NAME_CHANGE_CANNOT_GRADE,				//���� ���� ������ ����.
		RC_GUILD_NAME_CHANGE_CANNOT_DB,					//���� ���� ���ν��� ���ϰ� fali
		RC_GUILD_NAME_CHANGE_CANNOT_DB_SYSTEM,			//���� ���� ���ν��� ���ϰ� fali �ý��� ����
		RC_GUILD_NAME_CHANGE_CANNOT_DB_SAME_GUILDNAME,	//���� ���� ���ν��� ���ϰ� fali ���� ���� ����
		RC_GUILD_NAME_CHANGE_CANNOT_DB_BLANK_SPACE,		//���� ���� ���ν��� ���ϰ� fali ���� ���� �̴�
		
		RC_GUILD_NAME_BAN_WORD,							//�������� ���� ���� �ܾ��̴�.
		RC_GUILD_DISCONNECTED_DBP_SERVER,				//��� ������ DBP������ ����Ǿ� ���� �ʴ�.

        //_NA001385_20090924_DOMINATION_ETC
        RC_GUILD_CANNOT_WITHDRAW_BY_DOMINATIONFIELD,	//������ �ʿ� ���� �ϴ� ������ Ż��(����) �� �� ����. (�ش� ������ �Ա��� ����,��õ� �ٶ�)
        RC_GUILD_CANNOT_KICK_BY_DOMINATIONFIELD,	    //������ �ʿ� ���� �ϴ� ������ ���� ��ų�� ����.      (�ش� ������ �Ա��� ����,��õ� �ٶ�)
        //
        RC_GUILD_DOMINATION_ACTIONINFO_CANNOT_DESTROY,  //������ ���� ���(���)�� �ػ� ��û �� �� ����.
        RC_GUILD_DOMINATION_INFO_CANNOT_DESTROY,        //������ ���� ���(����,����)�� �ػ� ��û �� �� ����.

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        RC_GUILD_JOIN_REQUEST_FULL_LIST,                //���� ��û�� ����� �ʰ��Ǿ���.
        RC_GUILD_JOIN_REQUEST_ALREADY_EXIST,            //�̹� ���� ��û�� ��Ͽ� �����Ѵ�.
        RC_GUILD_JOIN_REQUEST_NOT_EXIST,                //���� ��û�� ��Ͽ� ���������ʴ´�.
        RC_GUILD_JOIN_REQUEST_GUILD_EXIST,              //��û�ڰ� �̹� ��带 ������ �ִ�.
        RC_GUILD_JOIN_REQUEST_INVALID_GUILD,            //��带 ã�� �� ����.
        RC_GUILD_JOIN_REQUEST_REJECT,                   //���� ��û�� ���� ���ߴ�.
        RC_GUILD_JOIN_REQUEST_OVER_WAIT_TIME,           //���� ��û ��� �ð��� ������.
        RC_GUILD_JOIN_REQUEST_PLAYER_MAX,               //�÷��̾ �� �� �ִ� �ִ� ��û���� �ʰ��Ͽ���.

        RC_GUILD_NOT_EXIST_FACILITY,                    //���������ʴ� �ü��̴�.
        RC_GUILD_NOT_ENOUGH_UPGRADE_ITEM,               //��� �ü� ���׷��̵� �������� �����ϴ�.
        RC_GUILD_NOT_ENOUGH_ACTIVE_ITEM,                //��� �ü� Ȱ��ȭ �������� �����ϴ�.
        RC_GUILD_NOT_EXIST_FACILITY_ACTIVE,             //��� �ü� ��Ƽ�갡 �������� �ʴ´�.
        RC_GUILD_NOT_BE_ACTIVATED_IN_RIDE,              //Ż���� ź���¿����� ��� �ü��� Ȱ��ȭ �� �� ����.

        RC_GUILD_NOT_EXIST_PLAYER,                      //�ش� ��� �÷��̾ �������� �ʴ´�.
        RC_GUILD_FACILITY_CANNOT_ACTIVATED_FLYING,      // ������¿����� ���ü� Ȱ��ȭ �Ұ����ϴ�
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        //-------------------------------------------------
        // Upperbound
        RC_GUILD_RESULT_COUNTS // don't declare after this mark.
	};

	enum eEVENT_RESULT
	{
		RC_EVENT_SUCCESS,
		RC_EVENT_FAILED,
		//////////////////////////////////////////////////////////////////////////
		//	EventInventory����
		//RC_EVENT_ALREADY_ANOTHER_TRANSACTION,		//< �̹� �ٸ� ����� �������Դϴ�. ����� �ٽ� �õ��ϼ���.
		//RC_EVENT_CANNOT_MOVE_TO_INVENTORY,
		//RC_EVENT_FULL_INVENTORY,					//< �κ��丮�� ��á��.
		//RC_EVENT_NOTEXIST_EVENTITEM,				//< �̺�Ʈ�������� �������� �ʴ´�.
		//RC_EVENT_INVALID_ITEMCODE,					//< ��ũ��Ʈ�� �������� �ʴ� �������̴�.

		//////////////////////////////////////////////////////////////////////////
		//	Event Packet Handling
		RC_EVENT_INVALID_EVENT,						// �̺�Ʈ ���� �ƴϴ�
		RC_EVENT_ITEM_NUM_OVERFLOW,					// �̺�Ʈ ������ ��� ���� ������ �Ѿ���
		RC_EVENT_NOTEXISTITEM,						// �̺�Ʈ�� ������ �������� ����
		RC_EVENT_CANT_OBTAIN,						// ���� �������� ���� ���� ���� (���� ����)
		RC_EVENT_INSUFFICIENT_HEIM,					// [���Ӻ���] ����Ϸ��� ���� > ���� ���� 
		RC_EVENT_DONATION_HEIM_LIMIT,				// [���Ӻ���] �ּ� ��� ���� ���� ���� ����
		RC_EVENT_REWARDED_ITEM_NUM_OVERFLOW,		// [�����ۺ���] ���̻� �������� ���� �� ����.
		RC_EVENT_SUCCESS_HEIM_DONATION,				// [���Ӻ���] ��� ���� (������ ���޴� �Ϲ� ����)
		RC_EVENT_ITEM_INVENTORY_LOCKED,				// [�����ۺ���] �κ��� ����־ ������� ���Ѵ�.
		RC_EVENT_CANT_BUFF_GRANTED,					// [�����̺�Ʈ] �̹� ������ �ο��Ǿ� �ִ�
		RC_EVENT_UNQUALIFIED,						// �̺�Ʈ ������ ���� �� �ִ� �ڰ��� ���� ���Ѵ�. ������ ���� �ڵ��...
		RC_EVENT_UNABLE_FUNCTION_FOR_CHAOSTATE,		//< ī���� �̿��� �� ����

		// UPPERBOUND - RC�߰� ���ǽ� �� �տ� �Է��� ��
		RC_EVENT_MAX,
	};

	enum eSYNC_RESULT
	{
		RC_SYNC_SUCCESS,
		RC_SYNC_FAILED,
		RC_SYNC_PLAYER_IS_DEAD,						// �÷��̾ �׾ �̵�����
		RC_SYNC_FIELD_IS_NULL,						// �÷��̾��� �ʵ尪�� NULL�̴�.
		RC_SYNC_ROUTE_SIZE_EXCESS,					// ��� ���� �ִ밪 �ʰ�
		RC_SYNC_PLAYER_IS_UNMOVABLE_STATE,			// �÷��̾ �̵��� �� ���� �����̴�.
		RC_SYNC_SETPATH_ERROR,				
		RC_SYNC_INVALID_MOVESATE,					// �̵����� �̻�(�ȱ�, �޸���)
		RC_SYNC_CUR_POSITION_IS_INVALID,			// Ŭ�� ������ ������ǥ�� ��ȿ���� �ʴ�.
		RC_SYNC_DEST_POSITION_IS_INVALID,			// ������ ��ǥ�� ��ȿ���� �ʴ�.
		RC_SYNC_JUMP_LENGTH_TOO_LONG,				// �����Ÿ��� �ʹ� �ִ�
		RC_SYNC_ANIMATION_DELAY_PROCESSING,			// Ŭ�� Animation Delay ����� ó�� ����(���� ���� ó��) __NA001003_080527_PLAYER_ANIMATION_TIME_CONTROL__
		RC_SYNC_CUR_POSITION_IS_INVALID_NO_HACK,	// Ŭ�� ������ ������ǥ�� ��ȿ���� �ʴ�. ���� ��輱���� Ŭ�� ���� Ʋ�� ��ǥ�� ���� �� �ִ�.
	};

	enum eREWARD_RESULT
	{
		RC_REWARD_SUCCESS,
		RC_REWARD_FAILED,
		RC_REWARD_INSUFFICIENT_SPACE,
		RC_REWARD_FULL_MONEY,
		RC_REWARD_EMPTY,					//< ����
		RC_REWARD_FATIGUE_TIME,				//< �Ƿνð� ������ ������� �� ����.
        RC_REWARD_CANNOT_BETAKEY,           //���� ������ �ִ� ��ŸŰ�� ������ �ʾҵ�.
	};

	enum eQUEST_RESULT
	{
		RC_QUEST_SUCCESS,
		RC_QUEST_FAILED,
		RC_QUEST_INSUFFICIENT_SPACE,
		RC_QUEST_FULL_MONEY,
		RC_QUEST_EMPTY,						//< ����
		RC_QUEST_TOCOMPLETED,				//< ����Ʈ�� �Ϸ���� �ʴ´�.CanComplete()����
		RC_QUEST_CANNOTREWARD_CHILDQ,		//< �ڽ�����Ʈ�� ������� �� ����.
		RC_QUEST_NOT_COMPLETED_STATE,		//< ����Ʈ�� �Ϸ�� ���°� �ƴϴ�.
		RC_QUEST_CAN_NOT_INCOMPLET,			//< ����Ʈ�� Incomplet�� �� ����. CanComplete()����
		RC_QUEST_CANNOTREWARD_FATIGUE,		//< �Ƿνð� ������ ������� �� ����.
		RC_QUEST_CANNOTQUEST_MAXCOUNT,		//< �������� ����Ʈ�� �ִ밪�̾ ���̻� ���� �� ����.
		RC_QUEST_UNABLE_FUNCTION_FOR_CHAOSTATE, //< ī���� �̿��� �� ����
		RC_QUEST_MUST_UNEQUIP_ALL_ITEM,			//< �� ����� __NA_0_STAT_INIT_WHEN_100LEVEL_CHANGEUP
		RC_QUEST_ALREADY_PROGRESS_TIME_QUEST,	//< �̹� �������� Ÿ�� ����Ʈ�� �ִ�. __NA001282_20090206_MODIFY_QUEST_TIME
		RC_QUEST_OVER_REPEAT,						//�ݺ�Ƚ�� �ʰ�
		RC_QUEST_MISMATCH_CHARACTER_LV,				//ĳ���� ���� X
		RC_QUEST_NOT_CHAO,							//ī���ƴ�
		RC_QUEST_MISMATCH_CHARACTER_CLASS,			//ĳ���� Ŭ���� X
		RC_QUEST_LESS_MONEY,						//���� ������
		RC_QUEST_NOT_EXIST_NECESSARY_QUEST,			//�ʿ� ����Ʈ�� ����.
		RC_QUEST_MUST_NOT_EXIST_QUEST,				//����Ʈ�� ����� �Ѵ�.
		RC_QUEST_NOT_EXIST_NECESSARY_ITEM,			//�ʿ� �������� ����.
		RC_QUEST_MUST_NOT_EXIST_ITEM,				//�������� ����� �Ѵ�.
		RC_QUEST_NOT_KILL_NECESSARY_MONSTER,		//�ʿ� ���͸� ���� �ʾҴ�.
		RC_QUEST_NOT_LESS_MISSION_POINT,			//�̼� Ŭ���� ����Ʈ ����.
		RC_QUEST_NOT_CRASH_AREA,					//Ư������ �浹 X
		RC_QUEST_NOT_EMPTY_ITEM_SPACE,				//������ �� ���� ����
        RC_QUEST_DISTANCE_DIFF_TOO_LONG,            //����Ʈ �ο��ڿ� ������û�� ���̰� �ʹ� �ִ�.
        RC_QUEST_INCORRECT_POSITION,                //��ġ ������ �ùٸ��� �ʴ�.
        RC_QUEST_NOT_AREAID,                        //�ش� ����Ʈ�� ���� ����ID�� ���� ���� �ʴ´�.
        RC_QUEST_EXIST,                             //�̹� �����ϴ� ����Ʈ
        RC_QUEST_CANNOT_BETAKEY,                    //����ڰ� ������ ��ŸŰ�δ� �ش� ������ �Ҽ� �����ϴ�.
        RC_QUEST_COMPLETED_DAILY_QUEST,             //�Ϸ�� ��������Ʈ
        RC_QUEST_MAX_DAILY_QUEST_COUNT,             //��������Ʈ ���� ����
        RC_QUEST_PLAYER_BEHAVE_UNEXPECTED_WAITING,  //�÷��̾ ������ ���� �����
        RC_QUEST_CHILD,                             //�ڽ� ����Ʈ�̱� ������ �Ҽ� ����
        RC_QUEST_NOT_PLAYER,                        //�÷��̾� ������ ã�� �� �����ϴ�.
        RC_QUEST_NOT_DAILY,                         //��������Ʈ�� �ƴմϴ�.
        RC_QUEST_NOT_TODAY,                         //���� ������ ����Ʈ�� �����ϴ�.
        RC_QUEST_NOT_FIELD,                         //�ʵ尡 �ƴմϴ�.
        RC_QUEST_LESS_HONOR,                        //���� ������ �����մϴ�.
        RC_QUEST_MAX_GUILD_DAILY_QUEST_COUNT,       //��� ��������Ʈ ���� ����
#ifdef _NA_006607_20130402_ADD_QUESTTYPE
        RC_QUEST_NOT_ENOUGH_MISSION_CLEAR_COUNT,    //�̼� Ŭ����Ƚ���� �����ϴ�.
#endif //_NA_006607_20130402_ADD_QUESTTYPE
#ifdef _NA_007407_20140710_CHALLENGE_DAILY_QUEST
        RC_QUEST_MAX_CHALLENGE_DAILY_QUEST_COUNT,
#endif //_NA_007407_20140710_CHALLENGE_DAILY_QUEST
	};

	enum eAC_RESULT
	{
		RC_AC_SUCCESS,
		RC_AC_INVALD_PARAM,
		RC_AC_INSUFFICIENT_POINT,
		RC_AC_INSUFFICIENT_SPACE,
		RC_AC_ALREADY_REWARD,
		RC_AC_FAILED,
		RC_AC_REWARD_FAIL_FATIGUE,		   // �Ƿνð��̿��� AC������ ������ �����ϴ�.
	};

	enum eCHAR_RESULT
	{
		RC_CHAR_SUCCESS,
		RC_CHAR_FAILED,						//< DB�κ��� �ùٸ� ������ �޾ƿ��� ���߽��ϴ�.
		//RC_CHAR_EXIST_SAME_NAME,			//< ���� ĳ���� �� ������
		//RC_CHAR_SLOT_FULL,				//< ���� Ǯ
		//RC_CHAR_TRANSACTION_ERROR,		//< Ʈ������ ����(��������,��Ÿ����)
		//RC_CHAR_GUILD_MASTER,				//< ���� ĳ���Ͱ� ��� ��������(�����Ұ�)
		//RC_CHAR_DISCONNECTED_DBPROXY,		//< DBP���� ������ ������ �ֽ��ϴ�.(����Ĵٽýõ����ֽñ�ٶ��ϴ�)
		//RC_CHAR_INVALID_STATE,			//< �߸��� ���¿��� ��Ŷ�� �Խ��ϴ�.
		//RC_CHAR_ALREADY_DOING_TRANSACTION,//< �̹� �ٸ� Ʈ������� �������Դϴ�.(��Ŷ�����������ϰų��ٸ���Ȳ��������)
		//RC_CHAR_INCORRECT_SSN,			//< �߸��� �ֹε�Ϲ�ȣ�̴�.
		//RC_CHAR_MINIMUM_CHARACTER,		//< �ּұ��ڼ� ���� ���� : ����4����, �ѱ�2���� �̻�
		//RC_CHAR_CANNOT_CREATE_TO_SERVICE,	//< �ش޿����� ȥ������ �ű�ĳ���� ������ �Ұ����մϴ�.
		//RC_CHAR_DESTROIED,					//< �̹� �ı��� ĳ���� �̴�.

		RC_CHAR_DBUSER_ALREADY_CREATED,		//< DBP�� ������ �̹� ������� �ִ�.
		//RC_CHAR_QUERY_ERROR,				//< DB���� ����
		//RC_CHAR_DBUSER_NOT_EXIST,			//< DBProxy�� DB������ �������� �ʴ´�.
		//RC_CHAR_DBCHAR_ALREADY_CREATED,		//< DBP�� ĳ���Ͱ� �̹� ������� �ִ�.
		//RC_CHAR_DELETING_CHAR_NOT_EXIST,	//< DBP�� ������ ĳ���Ͱ� ����.
	};

	enum eCHAR_CREATE_RESULT
	{
		RC_CHAR_CREATE_SUCCESS,
		RC_CHAR_CREATE_ALREADY_DOING_TRANSACTION,	//< �̹� �ٸ� Ʈ������� �������̴�.
		RC_CHAR_CREATE_INVALID_PARAM,				//< ��Ŷ�� �߸��� ���Դϴ�.
		RC_CHAR_CREATE_MINIMUM_CHARACTER,			//< �ּұ��ڼ� ���� ���� : ����4����, �ѱ�2���� �̻�
		RC_CHAR_CREATE_DISCONNECTED_DBPROXY,		//< DBP�� ������ ���� �ʾҽ��ϴ�.(��õ� ���� �̿�ٶ�)
		RC_CHAR_CREATE_CANNOT_CREATE_TO_SERVICE,	//< �ش޿����� ȥ������ �ű�ĳ���� ������ �Ұ����մϴ�.
		RC_CHAR_CREATE_EXIST_SAME_NAME,				//< ���� ĳ���� �� ������
		RC_CHAR_CREATE_SLOT_FULL,					//< ���� Ǯ
		RC_CHAR_CREATE_TRANSACTION_ERROR,			//< Ʈ������ ����(��������,��Ÿ����)
		RC_CHAR_CREATE_QUERY_ERROR,					//< DB���� ����
		RC_CHAR_CREATE_DBUSER_NOT_EXIST,			//< DBProxy�� DB������ �������� �ʴ´�.
		RC_CHAR_CREATE_DBCHAR_ALREADY_CREATED,		//< DBP�� ĳ���Ͱ� �̹� ������� �ִ�.
        RC_CHAR_CREATE_CHARNAME_BAN_WORD,			//< ���� �ܾ ����� ĳ���͸� 
        RC_CHAR_CREATE_CANNOT_BETAKEY,              //����ڰ� ���� ��ŸŰ�δ� �ش� ĳ���͸� �����Ҽ� �����ϴ�.
	};
	enum eCHAR_DESTROY_RESULT
	{
		RC_CHAR_DESTROY_SUCCESS,
		RC_CHAR_DESTROY_ALREADY_DOING_TRANSACTION,	//< �̹� �ٸ� Ʈ������� �������̴�.
		RC_CHAR_DESTROY_INVALID_PARAM,				//< ��Ŷ�� �߸��� ���Դϴ�.
		RC_CHAR_DESTROY_DESTROIED,					//< �̹� �ı��� ĳ���� �̴�.
		RC_CHAR_DESTROY_INCORRECT_SSN,				//< �߸��� �ֹε�Ϲ�ȣ�̴�.
		RC_CHAR_DESTROY_TRANSACTION_ERROR,			//< Ʈ������ ����(��������,��Ÿ����)
		RC_CHAR_DESTROY_DISCONNECTED_DBPROXY,		//< DBP�� ������ ���� �ʾҽ��ϴ�.(��õ� ���� �̿�ٶ�)
		RC_CHAR_DESTROY_GUILD_MASTER,				//< ���� ĳ���Ͱ� ��� ��������(�����Ұ�)
		RC_CHAR_DESTROY_DBUSER_ALREADY_CREATED,		//< DBP�� ������ �̹� ������� �ִ�.
		RC_CHAR_DESTROY_QUERY_ERROR,				//< DB���� ����
		RC_CHAR_DESTROY_DBUSER_DONOT_EXIST,			//< DBProxy�� DB������ �������� �ʴ´�.
		RC_CHAR_DESTROY_DBCHAR_DONOT_EXIST,			//< ������ ĳ���Ͱ� ����.
		RC_CHAR_DESTROY_FAILED,						//< ĳ���� ���� ����
        RC_CHAR_DESTROY_GROUP_MEMBER,               //< �׷�ɹ��� �����Ͽ� �ɸ��� ������ ���� �Ͽ���.//[20090811][4540]�׷� �ɹ� ����� ���� �Ұ� �޼��� �߰�
	};
	enum eCHAR_RECOVER_RESULT
	{
		RC_CHAR_RECOVER_SUCCESS,
		RC_CHAR_RECOVER_ALREADY_DOING_TRANSACTION,	//< �̹� �ٸ� Ʈ������� �������̴�.
		RC_CHAR_RECOVER_INVALID_PARAM,				//< ��Ŷ�� �߸��� ���Դϴ�.
		RC_CHAR_RECOVER_NOTDESTROYED,				//< �������� ���� ĳ�����̴�.
		RC_CHAR_RECOVER_DISCONNECTED_DBPROXY,		//< DBP�� ������ ���� �ʾҽ��ϴ�.(��õ� ���� �̿�ٶ�)
		RC_CHAR_RECOVER_TRANSACTION_ERROR,			//< Ʈ������ ����(��������,��Ÿ����)
		RC_CHAR_RECOVER_DBUSER_DONOT_EXIST,			//< DBProxy�� DB������ �������� �ʴ´�.
		RC_CHAR_RECOVER_DBCHAR_DONOT_EXIST,			//< ������ ĳ���Ͱ� ����.
	};

	enum eCHAR_IDCHECK_RESULT
	{
		RC_CHAR_IDCHECK_SUCCESS,					
		RC_CHAR_IDCHECK_ALREADY_DOING_TRANSACTION,	//< �ٸ� Ʈ������� �������̴�.
		RC_CHAR_IDCHECK_DISCONNECTED_DBPROXY,		//< DBP�� ������ ���� �ʾҴ�.
		RC_CHAR_IDCHECK_TRANSACTION_ERROR,			//< Ʈ������ ����( ���� ����, ��Ÿ ���� )
		RC_CHAR_IDCHECK_DBUSER_DONOT_EXIST,			//< DBProxy�� DB������ �������� �ʴ´�.
		RC_CHAR_IDCHECK_FAILED,
	};

	enum eCHAT_RESULT
	{
		RC_CHAT_UNKNOWN,
		RC_CHAT_BLOCKED,					// ä�� ���� ����
		RC_CHAT_ALLOWED,					// ä�� ��� ����
		RC_CHAT_SYNC,						// ����ȭ ����(����/�ʵ忡�� �߻��ϸ�, VIEWPORT_CHARSTATE�� �ٽ� ������� ��)
		RC_CHAT_NOCHAR,						// ��� ĳ���Ͱ� ���ų� �������� �ʾ���
		RC_CHAT_ALREADYSHOUT,				// ��ġ�� �ð� ���� ������ �ٽ� �õ�
		RC_CHAT_NOROOM,						// ä�� ä�ù濡 �� ���� ���� ���¿��� ä�� ä�� ���� ��û
		RC_CHAT_ALREADYINROOM,				// �̹� ä�� ä�ù濡 �������
		RC_CHAT_NOT_SUPER_MASTER,			// ���۹����� �ƴϹǷ� �����ġ�� ��ɾ� �ǹ� ����
		RC_CHAT_NOT_DECREE_ITEM,
		RC_GENERAL_SHOUT_LIMIT_LEVEL,		// TW_�Ϲ� ��ġ�� ���� ����
		RC_CHANNEL_CHAT_TITLE_BAN_WORD,		// ä�� ä�� ������ �����ܾ�
		RC_CHAT_USE_BAN_WORD,				// ä�� �� �����ܾ� ���
		RC_CHAT_ANTI_DOBAE,					// ä�� ���� ����.

#ifdef __NA_0_20080515_BUG_FIX_WORLD_SHOUT_LEVEL
		RC_WORLD_SHOUT_LIMIT_LEVEL,		// TW_���� ��ġ�� ���� ����
#endif
		RC_CHAT_BELONG_ZONE_BLOCK, 
        RC_CHAT_DENY_INVITATION, // ä�� �ʴ� �ź� (ä�� ä��)
        RC_CHANNEL_CHAT_BLOCKED, // ä�� ä���� ���ܵ�
	};

	enum eFRIEND_RESULT
	{
		RC_FRIEND_UNKNOWN,
		RC_FRIEND_NOTCONNECTED,				// ���� �� �� ����
		RC_FRIEND_NOTFRIEND,				// ģ�� �ƴ�
		RC_FRIEND_DIFFCHANNEL,				// ä���� �ٸ�
		RC_FRIEND_INVALIDZONE,				// ���� ������ �� Ÿ��(�κ�, ���ù�)�� �ƴ�
        RC_FRIEND_SUCCESS,		            // ...	_NA_0_20100527_ADD_CHAR_RELATION_MEMO	
        RC_FRIEND_FAIL,
        RC_FRIEND_NOTGUILDMEMBER,		    // ��� ģ�� �ƴ�.	
	};

	enum eSTATUS_RESULT
	{
		RC_STATUS_SUCCESS,
		RC_STATUS_UNKNOWN,
		RC_STATUS_ZONE_IS_NULL,
		RC_STATUS_FIELD_IS_NULL,
		RC_STATUS_CHAR_IS_ALIVE,
		RC_STATUS_IS_NOT_SAFEZONE,
		RC_STATUS_TELEPORT_FAIL,
		RC_STATUS_RESURRECTION_FAIL,
		RC_STATUS_MAPINFO_IS_NULL,
		RC_STATUS_TEAM_IS_NULL,
		RC_STATUS_SKILLINFO_IS_NULL,
		RC_STATUS_REQUIRE_LEVEL_LIMIT,
		RC_STATUS_STATE_NOT_EXSITS,				// �׷� ���°� �������� �ʴ´�.
		RC_STATUS_DRAGON_TRANSFORMATION_LIMIT,	// �巡�ﺯ�� ���¿����� ����� �� ���� ���
		RC_STATUS_ALIVE,						// ��� �ִ�.
		RC_STATUS_NOT_FALL_DEAD,				// �����ؼ� ���� ���°� �ƴϴ�.
		//RC_STATUS_INTERCEPTION_FAILED,
		RC_STATUS_TRADE_INTERCEPTION_FAILED,
		RC_STATUS_WHISPER_INTERCEPTION_FAILED,
		RC_STATUS_PLAYER_LEVEL_NOT_UNDER_20,
		RC_STATUS_NOT_EXIST_INIT_POINT,
		RC_STATUS_NOT_UNEQUIP_ALL,
		RC_STATUS_PLAYER_LEVEL_NOT_UNDER_40,	//__NA_1260_CHANGE_LEVEL_FOR_STAT_INIT
        //__NA001390_090915_RIDING_SYSTEM__
        RC_STATUS_CANT_EMOTION_ON_CURRENT_STATE,// ���� ���¿����� �̸���� ���� �� �����ϴ�.
        // _NA_0062_20100318_CHANGE_LEVEL_FOR_STAT_INIT_2
        RC_STATUS_PLAYER_LEVEL_TOO_HIGH_FOR_FREE_STAT_INIT, // ĳ���� ������ ���� ���� ���� �ʱ�ȭ ����
	};

	// ���� ����Ʈ ����
	enum eWP_RESULT
	{
		RC_WP_SUCCESS,

		RC_WP_FAILED,
		RC_WP_CANNOT_LOAD_FROM_DB,				//< DB�κ��� ������ ������ �� �����ϴ�.
		RC_WP_CANNOT_SAVE_FROM_DB,				//< DB�κ��� ������ ������ �� �����ϴ�.
		RC_WP_ALREADY_SAVE_MAPCODE,				//< �̹� ����Ǿ��ִ� MapCode�Դϴ�.
		RC_WP_UNABLE_FUNCTION_FOR_CHAOSTATE,	//< ī�����¿����� �̿��� �� ���� ����̴�.

        // 901, __NA001164_20081024_CHAR_WAY_POINT_SET_DB_FUNC
		RC_WP_CANNOT_DELETE_FROM_DB,			//< DB�κ��� ������ ������ �� �����ϴ�.
		RC_WP_CANNOT_FIND_WP_IDX,				//< ��������Ʈ �ε����� ã���� �����ϴ�.
		RC_WP_CANNOT_FIND_WAYPOINT_ITEM,		//< ��������Ʈ �������� ã���� �����ϴ�.
		RC_WP_UNABLE_DELETE_FOR_DELTIME,		//< �����Ұ��ð��� ���� �־� ��������� ����Ҽ� �����ϴ�.
		RC_WP_UNABLE_FUNCTION_FOR_OVERCOUNT,	//< ���� ������ ������ �Ѿ���.
		RC_WP_UNABLE_FUNCTION_FOR_ZONETYPE,		//< ����, �ʵ忡���� ��� �����ϴ�.
		RC_WP_NOT_YET_LOAD_FROM_DB,				//< DB�� ���� ������ LOAD �Ǿ� ���� �ʴ�.
        RC_WP_UNABLE_FUNCTION_FOR_CUR_ZONE,		//< ��������Ʈ�� �����Ҽ� ���� ��ġ�� �ֽ��ϴ�.        
        RC_WP_PORTAL_FAIL,						//< ��������Ʈ�� �̵��� �����߽��ϴ�.
        //
	};

	// ������ ����Ʈ ����
	enum eREVENGE_RESULT
	{
		RC_REVENGE_SUCCESS,
		RC_REVENGE_FAILED,					// �� �� ���� ����
		RC_REVENGE_NO_POINT,				// ������ ����Ʈ ����
		RC_REVENGE_INVALID_CHAR,			// ������ ����� �ƴ�
		RC_REVENGE_INVALID_SITUATION,		// ������ ������ ��Ȳ�� �ƴ�(�δ��� PK���� ���°� �ƴ�)

		RC_REVENGE_NO_CHAR,					// ����� �������� ����
		RC_REVENGE_REVENGEITEM_NOT_EXSITS,	// ������ ���� �������� �ʴ´�
		RC_REVENGE_CHAR_INVALID_STATE,		// ĳ���Ͱ� �������� ����� �� ���� ������
		RC_REVENGE_INVALID_ROOM,			// ����� ��ȿ�� �濡 �������� ����
		RC_REVENGE_MOVEZONE_FAIL,			// ���̵� ����
		RC_REVENGE_INVALID_ZONE_LEVEL,		// ���� �� �� ���� ����	// __BUG_2598_REVENGE_STONE_MAP_LEVEL_BUG
	};

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
    enum eSUN_RANKING_RESULT
    {
        RC_SUN_RANKING_SUCCESS,                 // ����
        RC_SUN_RANKING_FAILED,                  // ����
        RC_SUN_RANKING_INVALID_RANKING_TYPE,    // �� �� ���� ��ŷ Ÿ��
        RC_SUN_RANKING_SETTLE,                  // ������
    };
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM

	enum eRANKING_RESULT
	{
		RC_RANKING_SUCCESS,
		RC_RANKING_FAILED,						// �� �� ���� ����
		RC_RANKING_INVALID_STATE,				// �߸��� ����
		RC_RANKING_ALREADY_REWARDED,			// �̹� ���� ���� ����
		RC_RANKING_DOING_TRANSACTION,			// Ʈ����� ������, �̹� �ٸ� �۾� Ȥ�� �� �۾��� �������� ( ��Ŷ 2���� ���ɼ�! )
		RC_RANKING_NETWORK_ERROR_FOR_PORTAL,	// ��Ż ������ ������ �ִ�. ��Ʈ��ũ ��ַ� �Ͻ� ��� ����
		RC_RANKING_NETWORK_ERROR_FOR_DBPROXY,	// DBP ������ ������ �ִ�. ��Ʈ��ũ ��ַ� �Ͻ� ��� ����
		RC_RANKING_NOT_SPACE_FOR_REWARD,		// ������ ���� ������ ������ �����ϴ�.

		// ���� ���� ���� ����
		RC_RANKING_INSUFFICEINT_PARTY_MEMBER,	// ��ŷ�� ����ϱ� ���� ��Ƽ���(����5��)�� �����ϴ�.
		RC_RANKING_A_CLASS_OBTAIN_MAJORITY,		// �� Ŭ������ ���ݼ��� ���� �ϸ� �ȵȴ�.
		RC_RANKING_DONT_INCLUDE_MISSION_LEVEL_AREA,// �̼Ƿ��������� ���Ե��� �ʴ� �÷��̾ �����Ѵ�.
		RC_RANKING_INVALID_PARAM,					// �߸��� �����Դϴ�.

		RC_RANKING_DONT_INCLUDE_RANKING,			// ��ŷ�� ���Ե��� �ʾҽ��ϴ�.
		RC_RANKING_INVALID_REWARD_INDEX,			// ���� �ε����� ��ȿ���� �ʽ��ϴ�.
		RC_RANKING_SETTLING_INFO,					// ��ŷ ������, ��ŷ���� ���� ������
		RC_RANKING_DBPROXY_SP_LENGTH_OVERFLOW,		// �̼� ��ŷ SP ���ڿ� ���� �÷ο� �߻�
		RC_RANKING_DBPROXY_SP_TRANS_ERROR,			// �̼� ��ŷ SP Ʈ����� ���� �߻�
		RC_RANKING_UNSATISFY_KILL_MONSTER_NUM,		// �ּ� ���� ų���� �����մϴ�.

		// �̼Ƿ�ŷ ���а���
		RC_RANKING_NOT_FIND_REWARD_USER,			// �̼Ƿ�ŷ�� ������� ������ ã�� ���Ͽ����ϴ�.
		RC_RANKING_NOT_FIND_RANKING_INFO,			// �̼Ƿ�ŷ������ ã�� ���Ͽ����ϴ�.

		RC_RANKING_REWARD_FAIL_FATIGUE,				// �Ƿνð��̿��� �̼Ƿ�ŷ������ ���� ���Ͽ����ϴ�.
        RC_RANKING_NOT_REGIST_INVALID_PERIOD,		//��ȿ�� �Ⱓ�� �ƴ϶� ��ŷ ��� �Ұ�
	};

	enum eMEMO_RESULT
	{
		RC_MEMO_SUCCESS,
		RC_MEMO_FAILED,
		RC_MEMO_WAIT_INFO,						// ���� �ð� �ȿ� ��� ��û�� �� ����
		RC_MEMO_DOING_TRANSACTION,				// �̹� �޸� ���� Ʈ�������
		RC_MEMO_NOUSER,							// �ش� ����� ����
		RC_MEMO_NOMEMO,							// �ش��ϴ� ������ ����
		// 902, __NA001254_20090109_RTTG_FUNC_ADD
		RC_MEMO_NOSERVER,						// �������� (�ش� ���� ����, ���۽���)
		RC_MEMO_WRONG_RECVNAMES,				// �޴� �ɸ����� �߸��Ǿ���.()
		RC_MEMO_DBFAILED,
		//
	};

	enum ePORTAL_RESULT
	{
		RC_PORTAL_FAIL,
		RC_PORTAL_INSTANT_MOVE,					// ����̵�(���� �ʵ�)
		RC_PORTAL_AGENT_RELAY_MOVE,				// Agent �߰� �̵�(�ٸ� �ʵ�, ����)
        // 901, __NA001164_20081024_CHAR_WAY_POINT_PORTAL
        RC_PORTAL_CAN_SUCCESS,				    // ��Ż �̵� ���� ����
        RC_PORTAL_UNABLE_CUR_ZONE,              // ��Ż ��� �Ұ� �� (���� ��)
        RC_PORTAL_UNABLE_DEST_ZONE,              // ��Ż ��� �Ұ� �� (������)
        RC_PORTAL_FIELD_IS_NULL,                // �ʵ尡 ����.
        RC_PORTAL_INVALIDSTATE,                 // ��Ż �Ұ��� ����.
	};

	enum eITEM_DELETE_RESULT
	{
		RC_ITEM_DELETE_REVENGE_STONE,			// �����ǵ� ����
		RC_ITEM_DELETE_PORTAL_SUCCESS,			// ��ǥ�̵� ����			
		RC_ITEM_DELETE_PROTECT_DROP,			// �������
		RC_ITEM_DELETE_EXPIRED_CHARGE_ITEM,		// ��ȿ�Ⱓ�� ���� �������Դϴ�. �����մϴ�.
		RC_ITEM_DELETE_SUCCESS,					// �������� ���� �Ǿ����ϴ�.(��Ÿ����)
		RC_ITEM_DELETE_CLEAR_COORD,				// ��ǥ �ʱ�ȭ ������ ����
		RC_ITEM_DELETE_USE_KEY_SUCCESS,			// �δ� ���� Ű ��� ����
	};

	enum eFATIGUE_REWARD_RESULT					// �Ƿε� ���� result
	{
		RC_FATIGUE_REWARD_ZERO = 0,				// 1�ð� �̸�
		RC_FATIGUE_REWARD_OVER_AN_HOUR,			// 1�ð� �̻� 3�ð� �̸�
		RC_FATIGUE_REWARD_OVER_THREE_HOURS,		// 3�ð� �̻� 5�ð� �̸�
		RC_FATIGUE_REWARD_OVER_FIVE_HOURS,		// 5�ð� �̻�
	};

	enum eSLOT_INSERT_RESULT
	{
		RC_INSERT_SLOT_SUCCESS,
		RC_INSERT_SLOT_FAILED,		
		RC_INSERT_SLOT_AT_POS_OVER,
		RC_INSERT_SLOT_AT_POS_EMPTY,
		RC_INSERT_SLOT_TO_POS_OVER,
		RC_INSERT_SLOT_CAN_NOT_EQUIP,
		RC_INSERT_SLOT_DISABLE_SERVICE,
        // 902, __NA_1247_POSSESSION_ITEM
		RC_INSERT_SLOT_UNABLE_GUILDWAREHOUSE_POSSESSION_ITEM,	// �ͼӵ� �������� ���â�� �ű� �� ����.
	};

	enum ePET_SUMMON_RESULT
	{
		RC_PET_SUMMON_SUMMON_SUCCESS = 0,				// ��ȯ ����
		RC_PET_SUMMON_RETURN_SUCCESS,					// ���� ����		
		RC_PET_SUMMON_RETURN_FAILED,					// ����, ���� �� �Ǵ� Ŭ�� ���� ��
		RC_PET_SUMMON_ALREADY_SUMMONED,					// �̹� �ٸ� ���� ��ȯ �Ǿ� �־� ����
		RC_PET_SUMMON_NOT_PET_ITEM,						// �� �������� �ƴ�
        RC_PET_SUMMON_NOT_SUMMONED_PET,					// ��ȯ�� ���� ���µ� RETURN �Ϸ��� ��
		RC_PET_SUMMON_MISMATCH_PET_ITEM,				// ��ȯ�� ���̶� �����Ϸ��� �������� ���� �ٸ�.
		RC_PET_CAN_NOT_SUMMON_FULLNESS_ZERO,			// �� �������� 0�̾ ��ȯ �Ұ�
		RC_PET_EXPIRED_CHARGE_ITEM,						// �Ⱓ ����� ������
	};

    //__NA001390_090915_RIDING_SYSTEM__
    enum eRIDER_RESULT
    {
        RC_RIDER_SUCCESS                    = 0,    // ����
        RC_RIDER_FAILED                     ,       // ���� (Normal)
        RC_RIDER_UNEXPECTED_FAILED          ,       // ���� (Unexpected)
        RC_RIDER_CANT_FIND_INFO             ,       // ���̴��� ���õ� ������ ã�� �� ����. (Script?
        RC_RIDER_NOT_RIDER_ITEM             ,       // ���̴� ������ �ƴ�
        RC_RIDER_TIME_EXPIRED               ,       // �Ⱓ ����� ������
        RC_RIDER_ALREADY_SUMMONED           ,       // �̹� ��ȯ�Ǿ� �ֽ��ϴ�.
        RC_RIDER_ALREADY_SUMMON_RELEASED    ,       // �̹� ��ȯ �����Ǿ� �ֽ��ϴ�.
        RC_RIDER_MISMATCH_SUMMON_RIDER      ,       // ��ȯ�� ������ ���̴��� �����Ϸ��� ���̴��� Ʋ����.
        RC_RIDER_SUMMON_CASTING             ,       // ��ȯ ĳ���� ���Դϴ�.
        RC_RIDER_SUMMON_RELEASED_BY_HIT     ,       // ����|��ġ�ų� �װų� �ؼ� ��ȯ �����˴ϴ�.

        RC_RIDER_COUNTS                         // Upperbound
    };

    // 902, __NA001283_20090225_COLLECTION_USE
	enum eCOLLECT_RESULT
	{
		RC_COLLECT_SUCCESS = 0,							
		RC_COLLECT_FAIL,								// ä������ �̻��� �ִ�. (�ý��� ����)
		RC_COLLECT_ALREADY_COLLECTED,					// �ش� ä������ �̹� ä�����̰ų� ä�� �Ǿ���. (ä�� �õ���)
		RC_COLLECT_NOTEXIST,							// ä������ ���� ���� �ʴ´�. (object �� �������� �ʰų�, ä��ID�� �������� �ʴ´�.)
		RC_COLLECT_NOTCOLLECTING,						// ä������ ���°� ä������ �ƴϴ�. ����Ҽ� ����.
		RC_COLLECT_FAIL_CANCEL,							// ��ҿ� �����ߴ�.
		RC_COLLECT_FAIL_QCONDISION,						// ����Ʈ ������ ���� �ʴ´�.
		RC_COLLECT_FAIL_ACCONDISION,					// AC������ ���� �ʴ´�.
		RC_COLLECT_FAIL_COUNTCONDISION,					// ���� ������ ä������ ���̻� ä���Ҽ� ����.
		RC_COLLECT_POSITION_INVALID,					// �÷��̾ �����Ҽ� ���� ��ġ�� �ִ�.	
		RC_COLLECT_DISTANCE_INVALID = 10,				// �ִ� ä�� ���� �Ÿ��� �����.
		RC_COLLECT_ALREADY_COLLECTING,					// �÷��̾�� ä���߿� �ִ�.
		RC_COLLECT_FAIL_DB_ADD,							// DB�� ä����ǰ ���� ����
		RC_COLLECT_FAIL_REWORD_EXIT_CHAR,				// ���� ����(ĳ���͸� ã��������)
		RC_COLLECT_FAIL_REWORD_DEAD_CHAR,				// ���� ����(ĳ���Ͱ� �׾��ִ�)
		RC_COLLECT_FAIL_REWORD_WRONG_COLLECTOBJKEY,		// ���� ����(ä����Ű�� ���۽� ������ Ű�� ���� �ʴ�)
		RC_COLLECT_FAIL_REWORD_POS,						// ���� ����(���۽� ��ġ�� ����� ��ġ ���̰� ��������� �ִ� �ִ� �Ÿ��� �Ѿ���)
		RC_COLLECT_FAIL_REWORD_DISTANCE,				// ���� ����(��������� �ִ� ä�������� �ִ� �Ÿ��� �Ѿ���)
		RC_COLLECT_FAIL_REWORD_ITEM,					// ���� ����(��������� ���޿� �����ߴ�)
		RC_COLLECT_CHAR_NOTCOLLECTING,					// �÷��̾�� ä������ �ƴϴ�.	(ä�� ���� ��Ŷ�� ������ �ɸ��Ͱ� ä������ ä������ ������. )
		RC_COLLECT_FAIL_DB_DEL = 20,					// DB�� ä����ǰ ���� ����
        RC_COLLECT_FAIL_OBJECTID_CONDISION,             // ������Ʈ �ı� ������ ���� �ʴ´�.
        RC_COLLECT_FAIL_STAMP_CONDISION,                // ���� ������ ���� �ʴ´�. (��帶���;ƴ�)

        // _NA001385_20090924_DOMINATION_ETC
        RC_COLLECT_FAIL_STAMP_DEAD_CHAR,				// ���� ����(ĳ���Ͱ� �׾��ִ�)

        RC_COLLECT_FAIL_HAVENOTSPACE,
	};

    //���� ������ ��û ��� Ÿ��
    enum eDOMINATION_RESULT
    { 
        DOMINATION_RESULT_FAIL= 0,                  //�ܼ� ����.
        DOMINATION_RESULT_SUCCESS,                  //�ܼ� ����.
        DOMINATION_RESULT_DBERROR,                  //DATA ���� ����,
        DOMINATION_RESULT_NOT_GUILDMASTER,          //��� �����Ͱ� �ƴ϶� ���� �Ͽ���. (���� ����, ���� ���ý�)
        DOMINATION_RESULT_GUILDLEVEL_FAIL,          //��� ������ ��ȿ���� �ʴ�.
        DOMINATION_RESULT_NOT_ENOUGHMONEY,          //���� ������ �����ϴ�. (���� ���ý�)
        DOMINATION_RESULT_NOT_GUILDMEMBER,          //��� �ɹ��� �ƴϴ�.   (���� ����, ���� ���ý�)
        DOMINATION_RESULT_LIMIT_ENOUGHMONEY,        //���� �ݾ��� ���� ���� �⺻ ���� ���Ӻ��� �����ϴ�.
        DOMINATION_RESULT_BEST_ENOUGHMONEY,         //���� �ݾ��� ���� ���� ��� �ְ� ���Ӻ��� �����ϴ�
        DOMINATION_RESULT_DEFFENSEWALL_OVERPOINT,   //�溮�� ���� ����Ʈ�� �� ����Ʈ���� ����. (max����Ʈ(5) ���� ���� ����Ʈ�� ���� ��� )
        DOMINATION_RESULT_USER_LOCATION_FAIL,       //������ ��ġ�� ������ ���� ������ ����Ʈ�� �ƴϴ�. (�Ϲ� �ʵ忡�� ���� ����, ���� ���ý�)
        DOMINATION_RESULT_GUILDSTAT_DESTROYWAIT,    //��尡 ���� ��� ���¿� �ִ�. (��� ������)
        DOMINATION_RESULT_EMPTY_DATA,               //�����Ͱ� �������� �ʴ´�.
        DOMINATION_RESULT_OVERAP_APPLY,             //�������� ���, SUCCESS �ǹ̵� ������ �ִ�.
        DOMINATION_RESULT_LESS_TOKEN,               //���� ��ǥ ���� ����
        DOMINATION_RESULT_CAN_JOIN_ONLYONE_AREA,    //������ ������ �������� �����ϴ�.
        DOMINATION_RESULT_CAN_BETTING_ONLYONE_AREA, //������ ������ �������� �����ϴ�.
        DOMINATION_RESULT_NOT_PLAYER,               //�÷��̾� ��ü�� ����.
        DOMINATION_RESULT_NOT_IDLE_STATE,           //�÷��̾ IDLE ���°� �ƴϴ�.
        DOMINATION_RESULT_NOT_FOUND_AREA,           //���� ��ü�� ã���� ����.
        DOMINATION_RESULT_NOT_FOUND_EVENT,          //������ �̺�Ʈ�� ������ ����.
        DOMINATION_RESULT_NOT_JOIN_GUILD,           //���� ������ ��尡 �ƴϴ�.
        DOMINATION_RESULT_OVERFLOW_MEMBER,          //�����Ҽ� �ִ� ������� �ʰ� �ߴ�.
        DOMINATION_RESULT_AUCTION_LOW_LEVEL,        //��� ������ ���� ������ ������ �Ҽ� ����
        DOMINATION_RESULT_PENALTY_TIME,             // ���� ���� �Ⱓ
    };

#ifdef _NA_1419_20100121_ACCUMULATE_POINT
    enum eACCUMULATE_POINT_RESULT
    {
        RC_ACCUMULATE_POINT_SUCCESS,
        RC_ACCUMULATE_POINT_FAILED,
        RC_ACCUMULATE_POINT_INITIALIZE,             // �⵵�� �ٲ�� ���� �ð� �ʱ�ȭ ��.
        RC_ACCUMULATE_POINT_NOSPACEININVENTORY,     // �κ��丮 ���� ����.
        RC_ACCUMULATE_POINT_HAVENOTPOINT,           // ���� ����Ʈ ����
        RC_ACCUMULATE_POINT_CHANGE_REQUIRE_POINT,   // �ʿ� ���� ����Ʈ �����.
    };
#endif

#ifdef _NA_0_20110216_STATUS_STAT_POINT_SIMULATION
    enum eSTAT_POINT_RESULT
    {
        RC_STAT_POINT_SUCCESS,
        RC_STAT_POINT_FAILED,
        RC_STAT_POINT_IS_NOT_ENOUGH,                // ���� ����Ʈ ����.
        RC_STAT_POINT_DRAGON_CHANGED,               // ���� ���� ����
    };
#endif

#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM

    // ��Ƽ ���� ���
    enum ePARTY_FINDING_RESULT
    {
        PARTY_FINDING_SUCCESS = 0, // �⺻ ����
        PARTY_FINDING_FAIL, // �⺻ ����
        
        PARTY_FINDING_FAIL_SAME_KEY_IS_EXIST, // ����Ϸ��� Ű�� ���� Ű�� �̹� ����
        PARTY_FINDING_FAIL_CANT_ALLOC_POOL, // Ǯ���� �޸𸮸� �Ҵ� ���� ���ؼ� ��ü ������ ����
        PARTY_FINDING_FAIL_CANT_FIND_MASTER, // ��û�� ��Ƽ�� ������ ã�� �� ����
        PARTY_FINDING_FAIL_MASTER_IS_MEMBER, // ��Ƽ ���� ��û�ڰ� �̹� �ٸ� ��Ƽ�� ���� ����
        PARTY_FINDING_FAIL_NOT_VALID_INPUT, // ����ڰ� �Է��� ���� ������ ����
        PARTY_FINDING_FAIL_CANT_FIND_PARTY_FINDING, // ��û�� ��Ƽ ���� ������ ã�� �� ����
        PARTY_FINDING_FAIL_CANT_FIND_PARTY, // ��û�� ��Ƽ ������ ã�� �� ����
        PARTY_FINDING_FAIL_IS_NOT_MASTER, // ��û�ڰ� ��Ƽ���� �ƴ�
        PARTY_FINDING_FAIL_MAX_NUM_OF_MEMBER_IS_TOO_SMALL, // ��û�� �ִ� �ο����� ���� �ο������� ����
        PARTY_FINDING_FAIL_CANT_CANCEL, // ��Ƽ ���� ��Ҹ� �õ��ߴµ� ����
        PARTY_FINDING_FAIL_PARTY_IS_FULL, // ��Ƽ �ο��� ��� ���� ���� ����
        PARTY_FINDING_FAIL_CANT_FIND_JOIN_WANTED_USER, // ��û�� ���� ��û�� ������ ã�� �� ����
        PARTY_FINDING_FAIL_CHANNEL_IS_DIFFERENT, // ���� ��û�ڿ� ��Ƽ���� ä���� ���� �ٸ�
        PARTY_FINDING_FAIL_CANT_FIND_CHANNEL, // ��û���� ä���� ã�� �� ����
        PARTY_FINDING_FAIL_CANT_JOIN_PARTY, // ��Ƽ ������ �õ��ߴµ� ����
        PARTY_FINDING_FAIL_DENY_JOIN_REQUEST, // ��Ƽ���� ��Ƽ ���� ��û�� �ź�
        
        PARTY_FINDING_RESULT_COUNT
    }; // ePARTY_FINDING_RESULT

    // ��Ƽ ���� ��� ó���� ���� ��ƿ��Ƽ
    typedef ResultUtilTemplate<ePARTY_FINDING_RESULT, 
                               PARTY_FINDING_SUCCESS, 
                               PARTY_FINDING_RESULT_COUNT, 
                               PARTY_FINDING_SUCCESS, 
                               PARTY_FINDING_FAIL, 
                               BYTE> PartyFindingResultUtil;

#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM

    // ĳ���� �� ���� ���
    enum eCHANGE_CHARACTER_NAME_RESULT
    {
        RC_CHANGE_CHARACTER_NAME_SUCCESS = 0, // �⺻ ����
        RC_CHANGE_CHARACTER_NAME_ALREADY_USED, // �̹� ���� ��� ����
        RC_CHANGE_CHARACTER_NAME_DELAY_WAIT, // �������� 7���� ��� ���� ����
        RC_CHANGE_CHARACTER_NAME_FAIL, // �⺻ ����
    };

    //_NA_000096_20100527_CHANGE_CHARACTER_APPEARANCE_SYSTEM
    // ĳ���� �ܸ� ���� ���
    enum eCHANGE_CHARACTER_APPEARANCE_RESULT
    {
        RC_CHANGE_CHARACTER_APPEARANCE_SUCCESS = 0, // �⺻ ����
        RC_CHANGE_CHARACTER_APPEARANCE_FAIL, // �⺻ ����        
        RC_CHANGE_CHARACTER_APPEARANCE_COUNT
    };

    // ĳ���� �ܸ� ���� ��� ó���� ���� ��ƿ��Ƽ
    typedef ResultUtilTemplate<eCHANGE_CHARACTER_APPEARANCE_RESULT, 
                               RC_CHANGE_CHARACTER_APPEARANCE_SUCCESS, 
                               RC_CHANGE_CHARACTER_APPEARANCE_COUNT, 
                               RC_CHANGE_CHARACTER_APPEARANCE_SUCCESS, 
                               RC_CHANGE_CHARACTER_APPEARANCE_FAIL, 
                               BYTE> ChangeCharacterAppearanceResultUtil;

    // declared by _NA_0_20110329_GAMEOPTION_SETTING
    enum eGAME_OPTION_RESULT
    {
        RC_GAME_OPTION_SUCCESS = 0,
        RC_GAME_OPTION_FAIL,
        RC_GAME_OPTION_TYPE_FAIL,
        RC_GAME_OPTION_FIND_OPTION_FAIL,
        RC_GAME_OPTION_DB_USER_NOT_FOUND,
        RC_GAME_OPTION_UNLOADED,
        RC_GAME_OPTION_TRANSACTION,
        RC_GAME_OPTION_LOADING_FROM_DB,
        RC_GAME_OPTION_LOAD_FAIL_FROM_DB,
    };

#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
    enum eSHOP_RESULT
    {
        RC_SHOP_SUCCESS = 0,
        RC_SHOP_FAILED,
        RC_SHOP_PROCESS_TRANSACTION,            // �̹� �������� �۾����� ���� �ش� �۾��� ó���Ҽ� ����. (���߿� �ٽ� �õ�)
        RC_SHOP_REQUEST_SHOPSERVER_FAIL,        // ���������� ��û ����.
        RC_SHOP_SERVER_CLOSED,                  // �������� �����ִ�.
        RC_SHOP_ALREADY_EXSIT_USER,             // �̹� ���� �����ִ� �����̴�.
        RC_SHOP_CANNOT_FIND_USER,               // �������� ã�� �� ����.
        RC_SHOP_CANNOT_USE_SHOP,                // ���� �̿��� �� �����ϴ�.
        RC_SHOP_DIFFERENT_SCRIPT_VERSION,       // ��ũ��Ʈ ������ Ʋ��.
        RC_SHOP_INVALID_SCRIPT_VERSION,         // ��ũ��Ʈ ������ ��ȿ�ϴ�.
        RC_SHOP_CHANGED_SCRIPT_VERSION,         // ��Ŭ��Ʈ ������ ���� �����.
        RC_SHOP_STORAGE_PAGE_SIZE_FAIL,         // ��û�� ������ ������ ����� �߸���.
        RC_SHOP_BUY_ITEM_SIZE_FAIL,             // �����ϴ� ������ ����Ʈ ����� �߸���.
        RC_SHOP_GIFT_ITEM_SIZE_FAIL,            // ������ ���� ������ ����Ʈ ����� �߸���.
        RC_SHOP_BUY_ITEM_RELAY_FAIL,            // ���� ������ ����Ʈ ���� ����.
        RC_SHOP_GIFT_ITEM_RELAY_FAIL,           // ���� ������ ����Ʈ ���� ����.
        RC_SHOP_GIFT_RECEIVER_NOT_FOUND,        // ���� �޴� ������ ã�� ������.
        RC_SHOP_INVENTORY_FULL,                 // �κ��丮�� �������� ������ �������� �����ü�����.
        RC_SHOP_NOT_EXIST_CHARACTER_NAME,       // �ش� �̸��� ������ �������� �ʽ��ϴ�.
        RC_SHOP_SHOPSERVER_FAILED,              // �� �������� ���� ���� �Ѱ���.
        RC_SHOP_SHOPSERVER_DB_FAILED,           // Shop Data Base ���� ����.
        RC_SHOP_NOT_ENOUGH_CASH,                // ĳ�� ����
        RC_SHOP_BUY_IMPOSSIBLE,                 // ���� ��ǰ �����ϱ� �Ұ���.
        RC_SHOP_GIFT_IMPOSSIBLE,                // ��ǰ ���� �Ұ���.
        RC_SHOP_ITEM_EMPTY,                     // �ܿ� ���� ����.
        RC_SHOP_SELL_DATE_END,                  // �Ǹ� �Ⱓ ����
        RC_SHOP_SELL_END,                       // �Ǹ� ����
        RC_SHOP_EVENT_ITEM_BUY_IMPOSSIBLE,      // �̺�Ʈ ������ ���� �Ұ�.
        RC_SHOP_EVENT_ITEM_BUY_COUNT_OVER,      // �̺�Ʈ ������ ���� ���� Ƚ�� �ʰ�.
        RC_SHOP_BUY_COOL_TIME_FAIL,             // ��ǰ ���� �� Ÿ�� ���� ����.
        RC_SHOP_CANNOT_GIFT_ONESELF,            // ���� �߽�/�����ڰ� ����.
        RC_SHOP_EVENT_ITEM_GIFT_IMPOSSIBLE,     // �̺�Ʈ ������ ���� �Ұ�.
        RC_SHOP_EVENT_ITEM_GIFT_COUNT_OVER,     // �̺�Ʈ ������ ���� ���� Ƚ�� �ʰ�.
        RC_SHOP_LIMIT_CASH_TO_GIFT,             // �����ϱ� �ѵ� �ʰ�.(1�޿� ������ ���� �ݾ��� �ʰ�)
        RC_SHOP_OUTBOUND_MODULE_ERROR,          // �ƿ� �ٿ�� ��⿡��.
        RC_SHOP_DIFFERENT_CASH_TYPE,            // ĳ�� Ÿ���� ��������.
    };
#endif //_NA_0_20110613_GAME_SHOP_RENEWAL
    
    enum PrimiumServiceResult
    {
        RC_PRIMIUM_SUCCESS = 0,
        RC_PRIMIUM_FAIL,
        RC_PRIMIUM_NOT_SUPPORT_USER,
        RC_PRIMIUM_FAIL_GET_USER_INFO,
        RC_PRIMIUM_INVALID_STATUS_USER,
        RC_PRIMIUM_FAIL_START_USING_STATUS,
        RC_PRIMIUM_NOT_REMAIN_TIME,
        RC_PRIMIUM_NOT_CONNECTED_BILLINGSERVER,
        RC_PRIMIUM_RESPONSE_DATA_IS_NULL,
        RC_PRIMIUM_FAIL_KEEP_USING_STATUS,
    };

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    enum eBATTLE_GROUND_RESULT
    {
        RC_BATTLE_GROUND_RESERVATION_SUCCESS = 0,       // ���� ���� ����
        RC_BATTLE_GROUND_RESERVATION_FAIL,              // ���� ���� �Ұ�
#ifdef _NA_007135_20140402_MODIFY_BATTLE_GROUND_RESERVATION_AVAILABLE_IN_BATTLE_GROUND_CHANNEL
        RC_BATTLE_GROUND_RESERVATION_NOT_AVAILABLE_CHANNEL,  //���� ���� �Ұ��� ä��
#endif
        RC_BATTLE_GROUND_RESERVATION_ALREADY,           // �̹� ������
        RC_BATTLE_GROUND_NOT_INCORRECT_MAPCODE,         // �������� ���� �� �ڵ�
        RC_BATTLE_GROUND_IS_NOT_QUALIFIED_LEVEL,        // ���� ���� ����
        RC_BATTLE_GROUND_IS_NOT_QUALIFIED_ITEM_SCORE,   // ���� ��� ���� ����
        RC_BATTLE_GROUND_IS_NOT_USER,                   // ������ ����
        RC_BATTLE_GROUND_IS_NOT_EXIST_BATTLE_SERVER,    // ��Ʋ������ ���� ���� �ʾ� ���� �Ұ�
        RC_BATTLE_GROUND_EXIST_STATE_FUGITIVE_BUF,      // ������ ����(�г�Ƽ)�� �����Ͽ� ���� �Ұ�.
        RC_BATTLE_GROUND_PLAYER_IS_CHAO_STATE,          // �÷��̾ ī������ �϶� ���� �Ұ�.
        RC_BATTLE_GROUND_ALREADY_EXIST_PARTY,           // �̹� ��Ƽ�� �־� ���� �Ұ�.
        
        RC_BATTLE_GROUND_RESERVATION_CANCEL_SUCCESS,    // ���� ���� ��� ����
        RC_BATTLE_GROUND_RESERVATION_CANCEL_FAIL,       // ���� ���� ��� �Ұ�(�̹� ��ҵ�)
        RC_BATTLE_GROUND_STATE_INCLUDE_MATCHING_GROUPS, // ��Ī �׷쿡 ���Ե� ����(������� �Ұ�)
        RC_BATTLE_GROUND_REWARD_WINDOW_OPEN,            // �̼� ����â�� �����ִ� ����
        RC_BATTLE_GROUND_NONADMISSION_TIME,             // ���� ���� �ð��� �ƴϴ�.
#ifdef _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
        RC_BATTLE_GROUND_INITIATE_VOTE_SUCCESS,                     // ��ǥ���� ����
        RC_BATTLE_GROUND_INITIATE_VOTE_FAIL,                        // ��ǥ���� �Ұ���
        RC_BATTLE_GROUND_INITIATE_VOTE_DO_NOT_SAME_TEAM,            // ���� �� ���� ���� �ƴ϶� ��ǥ ���� �Ұ���
        RC_BATTLE_GROUND_INITIATE_VOTE_DO_NOT_SATISFY_VICTORYPOINT, // ���� ������ �������� �ʾ� ��ǥ���� �Ұ���
        RC_BATTLE_GROUND_INITIATE_VOTE_DO_NOT_SATISFY_MEMBER_COUNT, // ��ǥ ���� ������ �ο����� ���� ���� �ʾ� ��ǥ ���� �Ұ���
        RC_BATTLE_GROUND_INITIATE_VOTE_DO_NOT_SATISFY_LIMIT_TIME,   // ��ǥ ����� �ð��� ��� �����ʾ� ��ǥ ���� �Ұ���
#endif // _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        RC_GOLDRUSH_RESERVATION_SUCCESS,
        RC_GOLDRUSH_RESERVATION_FAIL,              // ��巯�� ���� ���� �Ұ�
        RC_GOLDRUSH_RESERVATION_ALREADY,           // �̹� ��巯�� ���� ������
        RC_GOLDRUSH_IS_NOT_QUALIFIED_LEVEL,        // ��巯�� ���� ����
        RC_GOLDRUSH_IS_NOT_QUALIFIED_ITEM_SCORE,   // ��巯�� ��� ���� ����
        RC_GOLDRUSH_EXIST_STATE_FUGITIVE_BUF,      // ��巯�� ������ ����
        RC_GOLDRUSH_PLAYER_IS_CHAO_STATE,          // ��巯�� ī�� ����
        RC_GOLDRUSH_ALREADY_EXIST_PARTY,           // ��巯�� �̹� ��Ƽ�� �־� ���� �Ұ�.

        RC_GOLDRUSH_RESERVATION_CANCEL_SUCCESS,    // ��巯�� ���� ��� ����
        RC_GOLDRUSH_RESERVATION_CANCEL_FAIL,       // ��巯�� ���� ��� �Ұ�(�̹� ��ҵ�)
        RC_GOLDRUSH_STATE_INCLUDE_MATCHING_GROUPS, // ��巯�� ��Ī �׷쿡 ���Ե� ����(������� �Ұ�)
        RC_GOLDRUSH_REWARD_WINDOW_OPEN,            // ��巯�� �̼� ����â�� �����ִ� ����
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    };
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
    enum eFREE_PVP_RESULT
    {
        RC_FREE_PVP_CREATE_SUCCESS = 0, // ����� ����
        RC_FREE_PVP_CREATE_FAIL, // ����� ����
        RC_FREE_PVP_CREATE_FAIL_MAX_ROOM, // �� ���ѿ� ���� ���� ����
        RC_FREE_PVP_CREATE_FAIL_ZONE, // �ν��Ͻ� �� ���� ����
        RC_FREE_PVP_CREATE_FAIL_TRANSACTION, // �ٸ� Ʈ�������
        RC_FREE_PVP_CREATE_FAIL_WAIT_MOVE_ZONE, // ���� ���� ����� ����.
        RC_FREE_PVP_CREATE_FAIL_INACTIVE, // ���������� ��Ȱ��ȭ.
        RC_FREE_PVP_CREATE_FAIL_ALREADY_BATTLE_GROUND_RESERVATION, // ���� ���� ��
        
        RC_FREE_PVP_JOIN_SUCCESS, // ���� ����.
        RC_FREE_PVP_JOIN_FAIL, // ���� ���� .
        RC_FREE_PVP_JOIN_FAIL_ISNOT_ROOM, // ���� �������� �ʴ´�.
        RC_FREE_PVP_JOIN_FAIL_MAX_USER, // ������ �� á��.
        RC_FREE_PVP_JOIN_FAIL_DIFFER_PASSWORD, // ��й�ȣ ����.
        RC_FREE_PVP_JOIN_FAIL_INACTIVE, // ���������� ��Ȱ��ȭ.
    };
#endif // _NA_006088_20120910_CHAOS_ZONE_FREE_PVP

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    enum eSPA_RESULT
    {
        RC_SPA_USER_ALREADY_DOING_TRANSACTION, //Ʈ����ǹ���
        RC_SPA_INVALID_MAPCODE,                //���ڵ尡 �ùٸ��� �ʽ��ϴ�
        RC_SPA_INVALID_ZONE,                   //Zone����, ���ų� ��õ�� �ƴϴ�
    };
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM

#ifdef _NA_008334_20150608_SONNENSCHEIN
    enum eSONNENSCHEIN_RESULT
    {
        RC_SONNENSCHEIN_SUCCESS = 0,                    //sonnenschein ��������
        RC_SONNENSCHEIN_FAILED,                         // sonnenschein ��������
        RC_SONNENSCHEIN_FAIL_TRANSACTION,               // Ʈ����ǽ���, �ٸ��ൿ��
        RC_SONNENSCHEIN_BATTLEGROUND_RESERVATION_STATE, // ���� ��������Դϴ�
        RC_SONNENSCHEIN_ALEADY_CREATED,                 // sonnenschein�� �̹� �����Ǿ����ϴ�.
        RC_SONNENSCHEIN_ROOM_CREATE_FAILED,              // instance_dungeon��������
        RC_SONNENSCHEIN_ROOM_NO_EXIST,                  // ���� �������� �ʴ´�
        RC_SONNENSCHEIN_ALREADY_ENTERED,                // �̹� ������ �����̴�
        RC_SONNENSCHEIN_STATE_NOT_READY,                // ���� ���� ������ �� ���� �����̴�
        RC_SONNENSCHEIN_STATE_END,                      // �ش� ���� �̹� ����Ǿ��� 
        RC_SONNENSCHEIN_NOTEXIST_ENTERANCE_ITEM,        // ����������� ����
        RC_SONNENSCHEIN_CHECK_ENTRANCE_ITEM_FAILED,     // ��������� üũ ����
        RC_SONNENSCHEIN_NOTEXIST_MISSIONINFO,           // ���̼������� ����
        RC_SONNENSCHEIN_NOT_DOMINATION_CHANNEL          // �ҳٻ����� ������ ä�ο����� �����ϴ�
    };
#endif //_NA_008334_20150608_SONNENSCHEIN

#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
	enum eONEDAYSGUILDMISSION_RESULT
	{
		RC_ONEDAYSGUILDMISSION_SUCCESS = 0,                    // ��������
		RC_ONEDAYSGUILDMISSION_FAILED,                         // ��������
		RC_ONEDAYSGUILDMISSION_FAIL_TRANSACTION,               // Ʈ����ǽ���, �ٸ��ൿ��
		RC_ONEDAYSGUILDMISSION_BATTLEGROUND_RESERVATION_STATE, // ���� ��������Դϴ�
		RC_ONEDAYSGUILDMISSION_ALEADY_CREATED,                 // �̹� �����Ǿ����ϴ�.
		RC_ONEDAYSGUILDMISSION_ROOM_CREATE_FAILED,             // instance_dungeon��������
		RC_ONEDAYSGUILDMISSION_ROOM_NOT_EXIST,                 // ���� �������� �ʴ´�
		RC_ONEDAYSGUILDMISSION_ALREADY_ENTERED,                // �̹� ������ �����̴�
		RC_ONEDAYSGUILDMISSION_STATE_END,                      // �ش� ���� �̹� ����Ǿ��� 
		RC_ONEDAYSGUILDMISSION_NOTEXIST_ENTERANCE_ITEM,        // ����������� ����
		RC_ONEDAYSGUILDMISSION_CHECK_ENTRANCE_ITEM_FAILED,     // ��������� üũ ����
		RC_ONEDAYSGUILDMISSION_NOTEXIST_MISSIONINFO,           // ���̼������� ����
		RC_ONEDAYSGUILDMISSION_NOT_DOMINATION_CHANNEL,         // ���ϱ��̼��� ������ ä�ο����� �����ϴ�
		RC_ONEDAYSGUILDMISSION_NOT_GUILD_USER,				   // ��������� �ƴϴ�
		RC_ONEDAYSGUILDMISSION_OVER_MAX_LIMIT_ENTERED_CNT	   // ����ο��� �Ѿ� ���� �Ұ�
	};
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION

#ifdef _NA_008404_20150712_MINIGAME_SYSTEM
    enum eMINIGAME_RESULT
    {
        RC_MINIGAME_ACTION_SUCCESS,                     //
        RC_MINIGAME_REWARD_SUCCESS,                     //
        //
        RC_MINIGAME_INVAILD_TYPE,                       // �˼����� �̴ϰ��� Ÿ���Դϴ�.
        RC_MINIGAME_NOT_OPEN_TIME,                      // �̴ϰ��� ���� �ð��� �ƴմϴ�.
        //
        RC_MINIGAME_BINGO_INVALID_ITEM_INFO,            // ���� ������ ������ �߸��Ǿ����ϴ�.
        RC_MINIGAME_BINGO_ALREADY_FILLED_NUMBER,        // �̹� ��ϵ� ���� �Դϴ�.
        RC_MINIGAME_BINGO_NOT_EXIST_NUMBER,             // �����Ͻ� �����ǿ� ���� �����Դϴ�.
        RC_MINIGAME_BINGO_REWARD_FAILED,                // ���� ����ޱ⿡ �����Ͽ����ϴ�.
        //
    };
#endif // _NA_008404_20150712_MINIGAME_SYSTEM

}; // namespace RC

#endif // __RESULT_CODE_H__
