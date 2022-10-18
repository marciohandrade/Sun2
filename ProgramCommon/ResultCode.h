#ifndef __RESULT_CODE_H__
#define __RESULT_CODE_H__



namespace RC
{
    // 결과 처리를 돕는 유틸리티 템플릿
    template <typename ENUM_TYPE, // 나열자 형식
              ENUM_TYPE begin_value, // 시작값
              ENUM_TYPE end_value, // 종료값 (*STL 반복자의 end와 의미가 같음을 유의할 것!)
              ENUM_TYPE success_value, // 성공값
              ENUM_TYPE default_value, // 기본값
              typename VALUE_TYPE> // 실제 사용할 값 형식
    struct ResultUtilTemplate
    {
        typedef VALUE_TYPE ValueType; // 실제 사용할 값 형식

        // 나열자 값을 실제 사용할 값의 형식으로 변환한다.
        inline static ValueType EnumToValue(const int enum_value)
        {
            // 값이 유효하지 않으면
            if (enum_value < begin_value && enum_value >= end_value)
            {
                // 로그에 기록하고 기본값을 반환한다.
                MessageOut(eCRITICAL_LOG, 
                           _T("|ResultUtil|Value's boundary is invalid.|begin:%d|end:%d|value:%d|"), 
                           begin_value, 
                           end_value, 
                           enum_value);
                return GetDefaultValue();
            }
            return static_cast<ValueType>(enum_value);
        }

        // 기본값을 얻는다.
        inline static ValueType GetDefaultValue()
        {
            BOOST_STATIC_ASSERT(default_value >= begin_value && default_value < end_value);
            return static_cast<ValueType>(default_value);
        }

        // 결과가 성공 했는지 검사한다.
        inline static bool IsSuccess(const int result_value)
        {
            return (result_value == success_value);
        }

        // 결과가 실패 했는지 검사한다.
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
		RC_ITEM_EMPTYSLOT,				//< 특정위치의 슬롯이 비어있다.
		RC_ITEM_NOTEXISTITEMATFIELD,	//< 필드에 아이템이 존재하지 않는다.
		RC_ITEM_NOINFO,					//< Code에 해당하는 아이템 정보가 존재하지 않는다.
		RC_ITEM_NOSPACEININVENTORY,		//< 인벤토리에 빈 공간이 없다.
		RC_ITEM_INVALIDSTATEOFPLAYER,	//< 플레이어 상태가 이상하다
		RC_ITEM_INVALIDPOSFORDROPITEM,	//< 드랍할 아이템의 위치가 잘못되었다.
		RC_ITEM_UNKNOWNERROR,			//< 알수 없는 에러, 클라이언트에서의 잘못된 데이터 혹은 해킹에 의해 데이터 크랙
		RC_ITEM_DBP_ASYNC,				//< DBP로부터 비동기적으로 시리얼 발급 요청 상태
		RC_ITEM_COOLTIME_ERROR,			//< 쿨타임이 끝나지 않았다.

		//< Combine
		RC_ITEM_ITEMCODENOTEQUAL,		//< 아이템 코드가 다르다.

		//< use
		RC_ITEM_ISNOTWASTEITEM,					//< 소모성 아이템이 아니다.
		RC_ITEM_PORTION_USE_LIMIT,				//< 경쟁헌팅에서 포션사용 개수 제한이 있다.
		RC_ITEM_CAN_USE_FIELD,					//< 필드에서만 사용할 수 있다.
		RC_ITEM_INVALID_TYPE,					//< 해당 기능을 이용할 수 있는 아이템 종류가 아니다.
		RC_ITEM_FIELD_IS_NULL,
		RC_ITEM_UNABLE_FUNCTION_FOR_CUR_ZONE,	//< 현재 지역에서는 사용할 수 없는 기능입니다.
        RC_ITEM_CANNOT_USE,                     // 아이템을 사용할 수 없습니다.
		RC_ITEM_CANNOT_MOVE_TO_THE_AREA,		//< 해당위치로 이동할 수 없다.
		RC_ITEM_CANNOT_LEARN_SKILL,				//< 스킬을 습득 할 수 없습니다.
		RC_ITEM_CANNOT_SKILL_ACTION,			//< 스킬을 시전 할 수 없습니다. 816, 
		RC_ITEM_CANNOT_FIND_PLAYER_TO_MOVE,		//< 이동시키려는 플레이어를 찾을 수 없다.
		RC_ITEM_NOT_STORE_COORD,				//< 좌표가 저장되어 있지 않다.
		RC_ITEM_ALREADY_STORE_COORD,			//< 이미 좌표가 저장되어 있다.
		RC_ITEM_CANNOT_STORE_COORD_AT_THIS_AREA,//< 해당위치에서는 좌표를 저장할 수 없다.	
		RC_ITEM_CANNOT_CREATE_TOTEMNPC,			//< TotemNPC를 생성할 수 없다.
#ifdef _NA_006689_20130503_CANNOTUSE_BUFFITEM
        RC_ITEM_CANNOT_USE_ALREADY_APPLY_HIGHRANK_BUFF, //< 이미 상위랭크의 버프가 걸려있기때문에 사용할 수 없다.
#endif //_NA_006689_20130503_CANNOTUSE_BUFFITEM

		//< sell
		RC_ITEM_INVALIDSHOPLISTID,		//< dwShopListID가 잘못되었다.
		RC_ITEM_OUTOFSHOPITEMINDEX,		//< shop에서 판매하는 아이템 인덱스가 아니다.
		RC_ITEM_HAVENOTMONEY,			//< 돈이 없다.

		//< buy
		RC_ITEM_ISNOTEMPTYSLOT,			//< 빈 슬롯이 없다 꽉 찼다.
		RC_ITEM_HAVENOTSPACE,			//< 공간이 부족하다.
		RC_ITEM_INVALIDVALUE,			//< 잘못된 값이 왔다.

		//< pick
		RC_ITEM_OVER_PLAYING_TIME,
		RC_ITEM_DONOT_HAVE_OWNERSHIP,		//< 아이템에 대한 소유권이 없다.
		RC_ITEM_UNPICKABLE_TYPE,			//< 주을 수 없는 아이템 타입이다. 
		RC_ITEM_CANT_PICKABLE_BY_LENGTH,	//< 정상적으로 주을 수 없는 거리다. 핵?
        RC_ITEM_DEAD_OWNER,                 //< 아이템의 소유권자가 죽어 있다.

		//</.....
		//< drop
		RC_ITEM_ERRORDROPMONEY,

		//< enchant 성공
		RC_ITEM_ENCHANT_SUCCESS,
		//< enchant 실패
		RC_ITEM_ENCHANT_FAILED,	
		RC_ITEM_ENCHANT_DOWN_FAILED,
		RC_ITEM_ENCHANT_CRACK_FAILED,
		RC_ITEM_ENCHANT_HAVENOTMETERIALS,
		RC_ITEM_ENCHANT_INVALID_ITEMLEVEL,			//< 아이템 레벨에 대한 인첸트 필요 재료 정보가 없다. (즉, 아이템 레벨이 잘못되었다. )
		RC_ITEM_ENCHANT_INVALID_RATE_INDEX,			//< 잘못된 성공확률 인덱스 값이다.
		RC_ITEM_ENCHANT_FAILED_AND_ENCHNAT_DOWN,	//< 인첸트가 실패하였고, 인첸트 값이 다운 되었습니다.
		RC_ITEM_ENCHANT_FAILED_AND_DESTROY,			//< 인첸트가 실패하였고 아이템이 소멸되었습니다. [0815에서 사용 안됨.]
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        RC_ITEM_ENCHANT_FAILED_AND_PREVENT,         //< 인첸트가 실패하였고, 아이템 소멸 및 다운이 방지되었습니다.
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

		//< rankup
		RC_ITEM_INVALID_CONDITION,					//< 체크 조건에 맞지 않는다.
		RC_ITEM_CANNT_RANKUP_ITEM,
		RC_ITEM_NO_MORE_RANK,
		RC_ITEM_INSUFFICIENT_MONEY,					//< 돈이 부족하다.		
	#ifdef __JP_000899_20080318_ENCHANT_RANKUP_LOCALIZE
		RC_ITEM_RANKUP_FAILED_AND_DESTROY,			//< 랭크업이 실패 하여 아이템이 소멸되었습니다.
	#endif

		//< socket
		RC_ITEM_FULLSOCKET,				//< 소켓이 꽉 찼다.

		//< extract socket
		RC_ITEM_EXTRACTSOCKET_SUCCESS,
		RC_ITEM_EXTRACTSOCKET_FAILED,

		RC_ITEM_UNUSABLE_FUNCTION,		//< 현재 사용할 수 없는 기능입니다.
		RC_ITEM_INVALID_VALUE,			//< 잘못된 값이다.
		RC_ITEM_CANNNOT_DROPSTATE,		//< drop할 수 없는 상태입니다.
		RC_ITEM_NOTEXIST_ITEM,			//< 아이템이 존재하지 않는다.

		RC_ITEM_CANNOT_SELL_ITEM,		//< 판매가 불가능한 아이템입니다.
		RC_ITEM_CANNOT_TRADE_ITEM,		//< 거래가 불가능한 아이템입니다.
		RC_ITEM_CANNOT_DROP_ITEM,		//< 드랍이 불가능한 아이템입니다.
		RC_ITEM_CANNOT_EXCHANGE_ITEM,	//< 교환할 수 없는 아이템입니다.
		RC_ITEM_CANNOT_MOVE_OTHER_SLOT_ITEM,	//< 다른 슬롯으로 이동할 수 없는 아이템입니다.
		RC_ITEM_CANNOT_DESTROY_ITEM,	//< 파괴(delete_syn)할 수 없는 아이템입니다.
		RC_ITEM_CANNOT_PICKUP_OBSERVER,	//< 투명상태에서는 아이템을 주울 수 없다.
		//< Compose
		RC_ITEM_COMPOSE_SUCCESS,
		RC_ITEM_COMPOSE_FAILED,

		//< Crystalize
		RC_ITEM_CRYSTALIZE_SUCCESS,
		RC_ITEM_CRYSTALIZE_FAILED,

		RC_ITEM_UNABLE_FUNCTION_FOR_CURR_LEVEL,		//< 현재 레벨에서 이용할 수 없는 기능입니다.

		// 내구도, 수리
		RC_ITEM_DONT_NEED_TO_REPAIR,				//< 수리할 필요 없음
		RC_ITEM_CANNOT_REPAIR_ITEM,					//< 수리가 불가능한 아이템입니다.

		RC_ITEM_COMPOSE_INVALID_LOCATION,			//< 조합할 수 없는 맵입니다.
		RC_ITEM_COMPOSE_LIMIT_CLASS,				//< 조합할 수 없는 클래스입니다.

		RC_ITEM_UNABLE_FUNCTION_FOR_CHAOSTATE,		//< 카오상태에서는 이용할 수 없는 기능이다.
		RC_ITEM_CANNOT_USE_QUEST_ACCEPT_ITEM,		//< 퀘스트 수락 아이템을 사용할 수 없습니다.

		RC_ITEM_CANNOT_RANKUP_ELITE,				//< 엘리트 아이템 랭크업 불가
		RC_ITEM_CANNOT_CRYSTALIZE_ELITE,			//< 엘리트 아이템 결정화 불가
		RC_ITEM_CANNOT_RANKUP_UNIQUE,				//< 유니크 아이템 랭크업 불가
		RC_ITEM_CANNOT_CRYSTALIZE_UNIQUE,			//< 유니크 아이템 결정화 불가

		RC_ITEM_UNABLE_FUNCTION_FOR_TRANSFORMATION,	//< 변신상태에서는 이용할 수 없는 기능이다.

		RC_ITEM_CANNOT_DELETE_FOR_SHORT_COUNT,		//< 개수부족으로 삭제할 수 없다.
		RC_ITEM_LENGTH_CANNOT_USE_NPC,				//< NPC를 이용할 수 없는 거리.
		RC_ITEM_DBG_WRONG_PACKET,					//< 잘못된 패킷 수신	

		RC_ITEM_CANNOT_ADD_EXTRAINVENTORY,			//< 더이상 유료화 인벤토리를 추가할 수 없다.
		RC_ITEM_CANNOT_DO_THIS_FOR_CHARGEITEM,		//< 유료화 아이템에 대해 이 기능을 이용할 수 없습니다.
		RC_ITEM_DONT_EXIST_TO_REPAIR,				//< 수리할 아이템이 없습니다.

		//소멸의 룬
		RC_ITEM_NOT_INVENTORY,						//< 인벤토리가 아닌 슬롯에 존재하는 아이템입니다.
		RC_ITEM_INVALID_SOCKET_NUM,					//< 소켓옵션 삭제시 잘못된 소켓넘버 입니다.
		RC_ITEM_SOCKET_HAS_NOT_OPTION,				//< 소켓옵션이 없는 아이템의 소켓을 삭제 하려고 합니다.
		RC_ITEM_SOCKET_DIFFERENT_LEVEL,				//< 상,하급룬이 다른 소켓 옵션을 삭제할 수 없습니다.		
		RC_ITEM_SOKET_NOT_RUNE_ITEM,				//< 사용 아이템이 '소멸의 룬'이 아니다.
        
		RC_ITEM_ISNOT_PARTY_MEMBER,					//< 파티 멤버일 때만 사용할 수 있는 기능이다.

		RC_ITEM_NOT_MIXTURE,						//< 기능을 사용할 수 없는 아이템이다.(Enchant, Rankup 등등...)
		
		//초기화 아이템
		RC_ITEM_LESS_FOR_USE,						//< 해당 아이템이 모자라다.
		RC_ITEM_NOT_UNEQUIP_ALL,					//< 장비를 모두 해제하지 않은 상태이다.
		RC_ITEM_NOT_MORE_DECREASE,					//< 더 이상 스탯을 감소할 수 없다.
		RC_ITEM_CANNOT_USE_MULTI_ALL_ITEM,			//< 올스텟 초기화 아이템은 복수로 사용할 수 없다.

		//몬스터 소환 아이템
		RC_ITEM_MONSTER_SUMMON_FAILED,				//< 몬스터 소환 아이템을 사용은 성공했지만 확률로 인해 실패

		RC_ITEM_WAREHOUSE_NEED_PASSWORD,			//< 창고 비번 필요합니다.
		RC_ITEM_INVENTORY_NEED_PASSWORD,			//< 인벤토리 비번 필요합니다.
		RC_ITEM_INVENTORY_INVALID_PASSWORD,			//< 잘못된 비번입니다.
		RC_ITEM_INVENTORY_ALREADY_TRANSACTION,		//< 다른 비번관련 처리중입니다.
		RC_ITEM_INVENTORY_PWD_SETTING_FLOW_VIOLATION, //< 비번 설정관련 플로우를 위반했습니다.
		RC_ITEM_INVENTORY_INVALID_PACKET,

		RC_ITEM_CANNOT_MOVE_WAREHOUSE_ITEM,			//< 창고로 이동할 수 없는 아이템입니다.		

		RC_ITEM_CANNOT_RANKDOWN_ELITE,				//< 엘리트 아이템은 랭크 하향 불가
		RC_ITEM_CANNOT_RANKDOWN_UNIQUE,				//< 유니크 아이템은 랭크 하향 불가
		RC_ITEM_NOT_RANK_DOWN_TO_E_RANK_ITEM,		//< E랭크 아이템은 랭크를 하향할 수 없습니다.

		RC_ITEM_CANNOT_EXTEND_CASH_SLOT,			//< 더이상 유료화 장착슬롯을 추가할 수 없다.
		RC_ITEM_CANNOT_EXTEND_DATE,					//< 기간을 연장할 수 없는 아이템이다.
		RC_ITEM_CANNOT_RANKUP_LIMITED,				//< 리미티드 아이템은 랭크업 불가.
		RC_ITEM_CANNOT_REPAIR_MAX_DURA_TWO_OVER,	//< 최대 내구도 2이하에서만 사용 가능
		RC_ITEM_NOT_LIMITED_ITEM,					//< 리미티드 아이템이 아님
		RC_ITEM_OVER_REPAIR_COUNT,					//< 수리 가능 횟수 넘음
		RC_ITEM_CAN_NOT_EQUIP,						//< 장비할 수 없는 아이템
		RC_ITEM_CANNOT_RANKUP_FATE,					//< Fate 아이템은 랭크업 불가.
		RC_ITEM_NOT_FATE_ITEM,						//< Fate 아이템 아님.
		RC_ITEM_ALREADY_IDENTYFIED_FATE_ITEM,		//< 이미 감정된 Fate 아이템
		RC_ITEM_WIN_PRIZE_INFINITY_ITEM,			//< NOTE: f110428.5L, removed don't use anymore since 1102, 무제한 아이템에 당첨됨(서버만사용)
		RC_ITEM_NOT_IDENTYFIED_FATE_ITEM,			//< 이미 감정된 Fate 아이템
		RC_ITEM_NOT_EXIST,							//< 아이템이 존재 하지 않습니다.		

		RC_ITEM_EVENT_SUCCESS,
		RC_ITEM_EVENT_FAILED,
		RC_ITEM_EVENT_ALREADY_ANOTHER_TRANSACTION,		//< [이벤트] 이미 다른 명령이 수행중입니다. 잠시후 다시 시도하세요.
		RC_ITEM_EVENT_CANNOT_MOVE_TO_INVENTORY,			//< [이벤트] Can't Move EventInven->Inven
		RC_ITEM_IMPOSSIBLE_PERIOD_EXTEND,				//<  기간 연장 불가능 아이템
		RC_ITEM_ALREADY_REGISTER_AC_ITEM,				//< 이미 등록된 AC 아이템

		RC_ITEM_SUMMON_SUMMON_SUCCESS,					// 소환 성공
		RC_ITEM_SUMMON_RETURN_SUCCESS,					// 해제 성공		
		RC_ITEM_SUMMON_ALREADY_SUMMONED,				// 이미 다른 펫이 소환 되어 있어 실패
		RC_ITEM_SUMMON_NOT_PET_ITEM,					// 펫 아이템이 아님
		RC_ITEM_SUMMON_NOT_SUMMONED_PET,				// 소환된 펫이 없는데 RETURN 하려고 함
		RC_ITEM_SUMMON_MISMATCH_PET_ITEM,				// 소환된 펫이랑 해제하려는 아이템이 서로 다름.	
		RC_ITEM_CAN_NOT_SUMMON_FULLNESS_ZERO,			// 포만감이 0 이어서 소환 불가
		RC_ITEM_NOT_SUMMONED_PET,						// 소환된 펫이 없음

		//길드 창고
		RC_ITEM_GUILDWAREHOUSE_USENOT_SLOT_BY_GRADE,    // 길드 등급이 낮아 사용할 수 없는 슬롯
		RC_ITEM_GUILDWAREHOUSE_USENOT_SLOT_BY_CASHITEM, //캐쉬템이 없어서 사용할 수 없는 슬롯
		RC_ITEM_GUILDWAREHOUSE_NOEN_OUT_RIGHT,          //길드 출고를 할 권한이 없다.
		RC_ITEM_GUILDWAREHOUSE_OPEN_USENOT_CASHITEM,	//길드 창고가 열려있는 상태에서는 확장자재를 이용할수 없다.
		RC_ITEM_GUILDWAREHOUSE_NONE_USENOT_CASHITEM,	//소속된 길드가 없어서 길드 창고 확장 아이템을 사용할 수 없습니다.
		RC_ITEM_GUILDWAREHOUSE_NONE_USE_RIGHT,		    //길드 창고 확장자재 캐쉬템을 사용할 권한이 없다.
	    //__NA_1247_POSSESSION_ITEM
		RC_ITEM_GUILDWAREHOUSE_UNABLE_POSSESSION_ITEM,	// 귀속된 아이템은 길드창고에 옮길 수 없다.
        //__NA_000968_ETHER_EMISSION_DEVICE
		RC_ITEM_ETHER_DEVICE_USE_ONLY_TO_WEAPON,		// 에테르 방출장치는 무기에만 장착 가능
		RC_ITEM_ALREADY_EQUIP_ETHER_DEVICE,				// 에테르 방출장치가 이미 장착 되어 있음
		RC_ITEM_NOT_EQUIP_ETHER_DEVICE,					// 에테르 방출장치가 장착 되어있지 않음
		RC_ITEM_ALREADY_CHARGE_ETHER_BULLET,			// 에테르 총알이 이미 장전 되어 있음
        // LOTTO
		RC_ITEM_LOTTO_OPEN_SUCCESS,						// 오픈 성공
		RC_ITEM_LOTTO_OPEN_FAIL,						// 오픈 실패
		RC_ITEM_LOTTO_IDENTITY_FAIL,					// 인증 실패
		RC_ITEM_LOTTO_IDENTITY_SUCCESS,					// 인증 성공
		RC_ITEM_LOTTO_ALREADY_IDENTYFIED_LOTTO_ITEM,	// 이미 인증되어있다.
		RC_ITEM_LOTTO_NEED_IDENTYFY_LOTTO_ITEM,			// 인증이 필요하다.
		RC_ITEM_LOTTO_NEED_EMPTY_SLOT,					// 슬롯이 부족하다.
		RC_ITEM_LOTTO_NOLOTTOITEM,						// 로또 아이템이 아니다.
		RC_ITEM_LOTTO_INVALID_LOTTOSCRIPT_INDEX,		// 스크립트 인덱스가 잘못되었다.
		RC_ITEM_LOTTO_INVALID_STATE,					// 이상한 상태이다.
		RC_ITEM_LOTTO_NEED_OPEN_STATE,					// 열려있어야 한다.
		RC_ITEM_LOTTO_ALREADY_OPENED_LOTTO_ITEM,		// 이미 열려있다.
		RC_ITEM_LOTTO_CONCRETIZE_SUCCESS,				// 실체화 성공
		RC_ITEM_LOTTO_CONCRETIZE_FAIL,					// 실체화 실패
		RC_ITEM_PANDORABOX_CLOSE_SUCCESS,				// 판도라상자 닫기 성공
		RC_ITEM_PANDORABOX_CLOSE_FAIL,					// 판도라상자 닫기 실패

        RC_ITEM_WINDOW_ALREADY_OENDEDSTATE,				// 이미 윈도우가 열려있다.
		RC_ITEM_NOTEXIST_SUMMON_MONSTERINFO,			// 서먼 정보가 없습니다.
		RC_ITEM_CANNOT_USE_PET_NAME,					// 사용할 수 없는 펫 이름(욕설, 음란 등)
		RC_ITEM_CAN_NOT_ACTIVE_ETHER_BULLET,			// 에테르 탄환을 활성화 할 수 없다
		RC_ITEM_CAN_NOT_INACTIVE_ETHER_BULLET,			// 에테르 탄환을 비활성화 할 수 없다
		RC_ITEM_CANT_EQUIP_WEAPON			,			//< 공격무기 장착할 수 없습니다.
		RC_ITEM_CAN_USE_ONLY_CHAO,						//< 903 카오 유저만 사용할 수 있다.(프리카오 포함)
		RC_ITEM_CANNOT_USE_PLAYER_LEVEL,				//< 904 사용 가능한 레벨이 아님 __CN_1299_WASTE_ITEM_LEVEL_LIMITE_CHECK
		RC_ITEM_ALREADY_FIREUP_ITEM_SAME_TYPE,			//< 903 이미 같은 타입의 아이템이 활성화 되어있다.
        //__NA1390_1391_090915_RIDING_SYSTEM_ITEM_PART__
        RC_ITEM_ALREADY_EXIST,                          //< 해당 슬롯에 아이템이 이미 존재한다.
        //RC_ITEM_CAN_NOT_EQUIP <위에서 선언 - 장비할 수 없는 아이템
        RC_ITEM_ALREADY_TIME_EXPIRED,                   //< 이미 만료된 아이템입니다.

        // (090919) (현재 미적용 코드) __NA_001358_ENCHANT_UNIFICATION
        RC_ITEM_INVALID_ENCHANT_LEVEL,					//< 유효하지 않은 인첸트 레벨
        RC_ITEM_ALREADY_USE_CHARWAYPOINT,		        //< 룬스톤 아이템이 이미 사용중 입니다.
        //_NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
        RC_ITEM_ALREADY_EXTRACTED_TO_ETHERIA,           //< 이미 에테리아가 추출된 장비입니다.
        RC_ITEM_CANT_EXTRACT_TO_ETHERIA,                //< 이 아이템은 에테리아 추출이 불가능합니다.
        RC_ITEM_CANT_COMBINED_WITH_ITEM,                //< 해당 아이템은 결합할 수 없습니다.
        RC_ITEM_CANT_COMBINED_WITH_DIFFERENT_ITEM,      //< 같은 종류만 결합할 수 있습니다.

        // _NA_000096_20100527_CHANGE_CHARACTER_APPEARANCE_SYSTEM
        RC_ITEM_FAIL_TO_CHANGE_CHARACTER_APPEARANCE, // 캐릭터 외모 변경 실패
        
        // _NA_000251_20100727_SOCKET_SYSTEM
        RC_ITEM_NOT_UNIDENTIFIED_SOCKET,             // 미확인 소켓이 아니다.

        // _NA_002253_20100811_CUBE_SYSTEM
        RC_ITEM_SUCCESS_TO_COMPOSE_CUBE, // 큐브 합성 성공
        RC_ITEM_FAILED_TO_COMPOSE_CUBE, // 큐브 합성 실패
        RC_ITEM_SUCCESS_TO_DECOMPOSE_CUBE, // 큐브 분해 성공
        RC_ITEM_FAILED_TO_DECOMPOSE_CUBE,  // 큐브 분해 실패

        // _NA_000587_20100928_DOMINATION_BUFF_ITEM
        RC_ITEM_UNAVAILABLE_TARGET, // 지정된 대상에 아이템을 사용할 수 없습니다.
        RC_ITEM_NOT_FRIEND, // 아군이 아닙니다.

        // _NA_20101011_HEIM_LOTTO_CASH_TICKET
        // 캐시 응모권 사용으로 하임행운 응모 용지 활성화 실패 사유
        RC_ITEM_HEIM_LOTTO_TICKET_MAX_REACHED,          // 하임행운 복권용지 보유 개수 제한 초과
        RC_ITEM_HEIM_LOTTO_UNAVAILABLE,                 // 추첨 등의 이유로 일시적으로 기능을 사용할 수 없음

        // _NA_000251_20100727_SOCKET_SYSTEM
        RC_ITEM_SUCCESS_TO_COMPOSE_ZARD, // 쟈드 합성 성공
        RC_ITEM_FAILED_TO_COMPOSE_ZARD, // 쟈드 합성 실패

        RC_ITEM_SUCCESS_TO_GET_COMPOSITION_OR_DECOMPOSITION_HISTORIES, // 최근에 성공한 합성 또는 분해 목록 얻기 성공
        RC_ITEM_FAILED_TO_GET_COMPOSITION_OR_DECOMPOSITION_HISTORIES, // 최근에 성공한 합성 또는 분해 목록 얻기 실패

        RC_ITEM_SOCKET_FAILED_TO_EXTRACT_BY_RATIO, // 확률로 인해 쟈드 추출에 실패했지만 추출 시도 자체는 성공
        RC_ITEM_SOCKET_FAILED_TO_FILL_BY_GRADE, // 쟈드 등급별 개수 제한에 걸려서 쟈드 박기 실패

        //_NA_20110303_ADD_TYPE_OF_NPCSHOP
        RC_ITEM_IS_SHORT_HAVE_ITEM,             // 아이템 타입 상점에서 구입하기 위한 소지 아이템이 부족하다.
        RC_ITEM_IS_SHORT_HAVE_ACCUMULATEPOINT,  // 적립 포인트 상점에서 구입하기 위한 소지 포인트가 부족하다.

        RC_ITEM_CANNOT_EQUIPMENT_BETAKEY,       //해당 배타키를 가지지 못한 유저는 장착할수 없는 아이템이다.
        RC_ITEM_CANNOT_USE_BETAKEY,             //해당 배타키를 가지지 못한 유저는 사용할수 없는 아이템이다.

        RC_ITEM_BUY_ERROR_OF_REPUTE,                   // 평판이 부족해서 아이템을 구매 할 수 없다.
        RC_ITEM_BUY_ERROR_OF_FAME,                     // 명성이 부족해서 아이템을 구매 할 수 없다.

        //_NA004034_20120102_POINT_WALLET_SYSTEM
        RC_ITEM_FAILED_TO_EXCHANGE_CURRENCY_ITEM_BY_LIMIT,  // 최대소지제한으로 화폐아이템의 화폐포인트 환전 실패
        RC_ITEM_FAILED_TO_BUY_BY_POINT,                     // 화폐포인트 부족으로 아이템구입 실패

        //_NA004034_20120102_GUILD_POINT_SYSTEM
        RC_ITEM_GUILDCOIN_NOT_GUILD_MEMBER,                    // 길드멤버가 아님
        RC_ITEM_GUILDCOIN_NOT_ENOUGH_COIN,                     // 길드코인 부족
        RC_ITEM_GUILDCOIN_DONATION_SUCCESS,                    // 길드코인 기부 성공
        RC_ITEM_GUILDCOIN_DONATION_COUNT_OVER,                 // 기부 횟수 초과        
        RC_ITEM_GUILDCOIN_EXCEED_MAX_LEVEL,                    // 최고 레벨이므로 더이상 기부가 안됨
        RC_ITEM_BUY_ERROR_GUILD_LEVEL,                          // 길드 레벨이 부족할때
        RC_ITEM_BUY_ERROR_GUILD_EXP,                            // 길드 공헌도가 부족할때

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        RC_ITEM_GUILDFACILITY_NOT_GUILD_MEMBER,            // 길드 멤버가 아니다
        RC_ITEM_GUILDFACILITY_NONE_USE_RIGHT,              // 사용 권한이 없다
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

        // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
        RC_EQUIPMENT_AWAKENING_SUCCESS,                         // 각성 성공
        RC_EQUIPMENT_AWAKENING_FAIL,                            // 각성 실패
        RC_EQUIPMENT_AWAKENING_EQUIP_EXCEEDED_PERMIT_LEVEL,     // 장비 각성 허용 레벨을 넘었다.
        RC_EQUIPMENT_AWAKENING_EQUIP_MAX_LEVEL,                 // 장비 각성 레벨이 최고레벨 입니다
        RC_EQUIPMENT_AWAKENING_DOES_NOT_MATCH_ADDITIVE,         // 첨가제 아이템이 맞지 않는다
        RC_EQUIPMENT_AWAKENING_DOES_NOT_MATCH_ADDITIVE_AMOUNT,  // 첨가제 아이템의 수량이 맞지 않는다
        RC_EQUIPMENT_AWAKENING_DOES_NOT_MATCH_MATERIAL,         // 재료 아이템이 맞지 않는다
#ifdef _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD
		RC_EQUIPMENT_AWAKENING_DOES_NOT_MATCH_MATERIAL_AMOUNT,  // 재료 아이템의 수량이 맞지 않는다
#endif // _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD
        RC_EQUIPMENT_AWAKENING_INVALID_EQUIP_INFO,              // 장비 정보를 알 수 없다
        RC_EQUIPMENT_AWAKENING_INVALID_MATERIAL_INFO,           // 재료 정보를 알 수 없다
        RC_EQUIPMENT_AWAKENING_INVALID_AWAKENING_INFO,          // 각성 정보를 알 수 없다

		RC_EQUIPMENT_EVOLUTION_SUCCESS,                         // 진화 성공
		RC_EQUIPMENT_EVOLUTION_FAIL,                            // 진화 실패
		RC_EQUIPMENT_EVOLUTION_INVALID_EQUIP_INFO,              // 장비 정보를 알 수 없다
		RC_EQUIPMENT_EVOLUTION_ITEM_CAN_NOT_EVOLUTION,          // 진화 시킬 수 없는 아이템 입니다
		RC_EQUIPMENT_EVOLUTION_DOES_NOT_MATCH_ADDITIVE,         // 첨가제 아이템이 맞지 않는다
		RC_EQUIPMENT_EVOLUTION_DOES_NOT_MATCH_ADDITIVE_AMOUT,   // 첨가제 아이템의 수량이 맞지 않는다
		RC_EQUIPMENT_EVOLUTION_DOES_NOT_MATCH_AWAKENING_GRADE,  // 각성 등급이 맞지 않는다

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
        RC_ITEM_SPA_COSTUME_INVALID_ITEM,                       // 온천티켓이 아니다
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM

#ifdef _NA_008404_20150712_MINIGAME_SYSTEM
        RC_ITEM_FAILED_EXIST_SEPARATE_NAK_PACKET,               // 별도의 NAK 패킷이 존재한다
#endif // _NA_008404_20150712_MINIGAME_SYSTEM

        //------------------------------------------------------------------------------------------
        // (CHANGES) (f100517.2L) must be last settled
        RC_ITEM_PROCESS_PENDING,                        // 서버간 연동 등의 이유로 결과를 즉시 판정할 수 없어 처리 보류
        RC_ITEM_UPPERBOUND                              // 아이템 결과 코드 상한값
	}; // eITEM_RESULT

    // 아이템 사용 결과 처리를 돕는 유틸리티
    typedef ResultUtilTemplate<eITEM_RESULT, 
                               RC_ITEM_FAILED, 
                               RC_ITEM_UPPERBOUND, 
                               RC_ITEM_SUCCESS, 
                               RC_ITEM_FAILED, 
                               BYTE> ItemResultUtil;

	enum eWAREHOUSE_RESULT
	{
		RC_WAREHOUSE_NOT_AVAILABLE_SERVICE,				//< 창고 서비스를 이용할 수 없다.
		RC_WAREHOUSE_INSUFFICIENT_MONEY,				//< 돈이 부족하다.
		RC_WAREHOUSE_INVALID_STATE,						//< 창고를 사용할 수 있는 상태가 아니다.
		RC_WAREHOUSE_UNABLE_FUNCTION_FOR_CURR_LEVEL,	//< 현재 레벨에서 이용할 수 없는 기능입니다.
		RC_WAREHOUSE_UNAVAILABLE_LENGTH,				//< 창고를 이용할 수 없는 거리 입니다.

		RC_WAREHOUSE_NEED_PASSWORD,						//< 비번 필요합니다.
		RC_WAREHOUSE_INVALID_PASSWORD,					//< 잘못된 비번입니다.
		RC_WAREHOUSE_ALREADY_TRANSACTION,				//< 다른 비번관련 처리중입니다.
		RC_WAREHOUSE_PWD_SETTING_FLOW_VIOLATION,		//< 비번 설정관련 플로우를 위반했습니다.
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
		RC_TRADE_COUNTERPARTNER_CANNOT_STATE,		//< 상대방이 거래할 수 없는 상태입니다.
		RC_TRADE_CANNOT_TRADE_ITEM,					//< 거래가 불가능한 아이템입니다.
		RC_TRADE_NOT_AVAILABLE_SERVICE,				//< 현재 이용할 수 없는 서비스입니다.
		RC_TRADE_BLOCKED_ITEM,						//< 블럭된 아이템입니다.
		RC_TRADE_DIFFERENT_ITEM_INFO,				//< 처음 올린 아이템과 현재 아이템의 정보가 틀립니다.
		RC_TRADE_UNABLE_FUNCTION_FOR_CHAOSTATE,		//< 카오상태에서는 이용할 수 없는 기능이다.
		RC_TRADE_LENGTH_CANNOT_TRADE,				//< 거래할 수 없는 거리이다.
		RC_TRADE_DIFFERENT_ZONE,					//< 서로 다른 Zone에 위치 해 있다.
		RC_TRADE_INVENTORY_NEED_PASSWORD,			//< 비번 필요합니다.
		RC_TRADE_INTERCEPTION,						//< 상대방이 거래를 차단했습니다.
		RC_TRADE_MODIFY_CANCEL,						//< 상대방이 거래변경 요청을 취소.
		RC_TRADE_MODIFY_FAIL,						//< MSG_CG_TRADE_MODIFY_RESPONSE_SYN에 대한 결과 코드로 응답처리에 대한 실패 시
	};
	enum eVENDOR_RESULT
	{
		RC_VENDOR_SUCCESS,
		RC_VENDOR_FAILED,
		RC_VENDOR_INVALID_STATE,					//< 유효하지 않은 플레이어의 상태
		RC_VENDOR_INVALID_VENDORSLOT,				//< 잘못된 상점 위치의 정보
		RC_VENDOR_NOSPACEININVENTORY,				//< BUY : 빈공간이 부족하다.
		RC_VENDOR_CANNOT_TRADE_ITEM,				//< 거래가 불가능한 아이템입니다.
		RC_VENDOR_NOT_AVAILABLE_SERVICE,			//< 현재 이용할 수 없는 서비스입니다.
		RC_VENDOR_BLOCKED_ITEM,						//< 블럭된 아이템입니다.
		RC_VENDOR_INCLUDE_INVALID_ITEM_TO_START,	//< 거래할 수 없는 아이템이 포함되어 시작할 수 없습니다.
		RC_VENDOR_UNABLE_FUNCTION_FOR_CHAOSTATE,	//< 카오상태에서는 이용할 수 없는 기능이다.
		RC_VENDOR_HAVE_NO_MONEY,					//< 돈이 부족합니다.2006/9/13
		RC_VENDOR_ALREADY_END,						//< 이미 종료된 상점에서 구입하려고 한다.
		RC_VENDOR_INVENTORY_NEED_PASSWORD,			//< 비번 필요합니다.
		RC_VENDOR_TITLE_BAN_WORD,					//< 상점 이름이 금지단어
        RC_VENDOR_DIFFERENT_ZONE,                   //< 서로 다른 Zone에 위치 해 있다.
        //------------------------------------------------------------------
        RC_VENDOR_RESULT_COUNTS // upperbound
	};

	enum eVENDOR_LIST_RESULT
	{
		RC_VENDOR_LIST_SUCCESS,							//<NPC를 이용한 상점리스트 이용 성공
		RC_VENDOR_LIST_FAILED,							//<없는 주석은 eVENDOR_RESULT 참조
		RC_VENDOR_LIST_INVALID_STATE,					
		RC_VENDOR_LIST_UNABLE_FUNCTION_FOR_CHAOSTATE,
		RC_VENDOR_LIST_NOT_AVAILABLE_SERVICE,				
		RC_VENDOR_LIST_NONE_PAGE,						//페이지에 출력한 리스트가 없음
		RC_VENDOR_LIST_SEARCH_FAILED,
		RC_VENDOR_LIST_REFRENCE_NULL,					//NULL 페이지 참조
		RC_VENDOR_LIST_ALREADY,							//이미 상점 리스트 Open
        //------------------------------------------------------------------
        RC_VENDOR_LIST_RESULT_COUNTS // upperbound
	};
	
	enum eSKILL_RESULT
	{
		RC_SKILL_SUCCESS,
		RC_SKILL_FAILED,							//< 기타원인의 실패

		RC_SKILL_BASEINFO_NOTEXIST,					//< BaseSkillInfo가 없다.

		RC_SKILL_STATE_WHERE_CANNOT_ATTACK_ENEMY,	//< 적을 공격할 수 없는 상태이다.
		RC_SKILL_COOLTIME_ERROR,					//< 아직 쿨타임이 끝나지 않았다.
		RC_SKILL_HPMP_RUN_SHORT,					//< HP, MP 부족
		RC_SKILL_CHAR_CLASS_LIMIT,					//< 클래스 제한(그 스킬을 사용할 수 없는 캐릭터 타입이다)
		RC_SKILL_WEAPON_LIMIT,						//< 무기 제한
		RC_SKILL_SEALING_STATE,						//< 봉인 상태가 걸려있다.
		RC_SKILL_OUT_OF_RANGE,						//< 스킬 사거리에서 벗어난다.

		RC_SKILL_REQUIRE_LEVEL_LIMIT,				//< 요구레벨 제한
		RC_SKILL_REQUIRE_SKILLSTAT_LIMIT,			//< 요구숙련도 제한
		RC_SKILL_DOES_NOT_HAVE,						//< 스킬을 가지고 있지 않다.
		RC_SKILL_REMAIN_SKILLPOINT_LACK,			//< 남은 스킬포인트가 스킬레벨을 올리기에는 부족하다.
		RC_SKILL_MAX_LEVEL_LIMIT,					//< 이미 만렙이라서 더이상 스킬포인트를 올릴 수 없다.
		RC_SKILL_ALREADY_EXIST_SKILL,				//< 이미 존재하는 스킬이다.

		RC_SKILL_INVALID_STATE,						//< 플레이어의 상태가 유효하지 않습니다.
		RC_SKILL_NOTEXIST,							//< 스킬이 존재하지 않습니다.
		RC_SKILL_INVLIDPOS,							//< 잘못된 위치정보가 왔습니다.

		RC_SKILL_FIGHTING_ENERGY_FULL,				//< 투기개수가 최대치므로 더이상 올릴 수 없다.

		RC_SKILL_POSITION_INVALID,					//< 좌표가 유효하지 않다.

		RC_SKILL_SUMMONED_NOTEXIST,					//< 소환체가 존재하지 않는다.
		RC_SKILL_TARGET_NOTEXIST,					//< 타겟이 존재하지 않는다.
		RC_SKILL_FIELD_IS_NULL,

		RC_SKILL_ATTACKER_LEVEL_IS_LOW,				//< 공격자 레벨이 낮아서 PK를 할 수 없다.
		RC_SKILL_TARGET_LEVEL_IS_LOW,				//< 타겟 레벨이 낮아서 PK를 당할 수 없다.
		RC_SKILL_ISNOT_PK_ZONE,						//< PK가 가능한 지역이 아니다.
		RC_SKILL_ISNOT_PK_SERVER,					//< PK가 가능한 서버가 아니다.
		RC_SKILL_SERVER_STATE_IMPOSSIBLE_PK,		//< PK가 불가능한 서버상태이다.
		RC_SKILL_PK_IMPOSSIBLE_TARGET_TYPE,			//< PK가 불가능한 타겟타입이다.

		RC_SKILL_UNABLE_FUNCTION_FOR_TRANSFORMATION,//< 변신상태에서는 이용할 수 없는 기능이다.

		RC_SKILL_CAN_LEARN_BY_ITEM,					//< 아이템을 사용해야 스킬을 습득할 수 있다.

        // (WARNING) (WAVERIX) [35244]~[45248] 사이에 (LST1024)에 의해 값의 변경이 발생했음.
        // 다행히 그 사이에 빌드는 없었지만, 주의해 주시구랴... 뭐 일단은 c/s간 revision번호를 맞춰
        // 서 빌드한다는 조건 하에서는 문제 발생은 하지 않을 수도 있지만, 평소 개발 상황에서는 혼란
        // 초래 가능성 존재합니다.
		RC_SKILL_PLAYER_TILE_INVLIDPOS,				//< 대상의 타일이 존재하지 않는다.
		RC_SKILL_ISNOT_TROUBLED_TYPE,				//< 타일이 분쟁 지역이 아니다
		RC_SKILL_TARGET_ISDEAD,						//< 타겟이 죽어 있다.
		RC_SKILL_SAME_GUILD_CHARACTER,				//< 같은 길드원이다.
		RC_SKILL_SAME_PARTY_CHARACTER,				//< 같은 파티원이다.
		RC_SKILL_CAN_NOT_STATE_HIDE,				//< 하이드 불가 상태 __NA_0_SHADOW_HIDE_BUG_FIX

        //__NA_000921_20080227_TROUBLED_PARTS               //< 클라이언트 사용 안할시 삭제될 데이터
        RC_SKILL_ISNOT_CHAO_PART,					//< 타일이 카오 지역이 아니다	//사용무
        RC_SKILL_ISNOT_TROUBLED_ZONE,				//< 분쟁 가능한 지역이 아니다.	//사용무
        RC_SKILL_IMPOSSIBLE_PACKET_DATA,			//< 패킷 데이터가 형식에 맞지 않는다. //사용무
        //_NA002217_100728_EP2_PERK_SYSTEM_
        RC_SKILL_CANT_FIND_INFO,                    //< 해당 정보를 찾을 수 없습니다. (generic)
        RC_SKILL_UNROOT_PERK_REQUIRE_INACTIVE_STATE,//< 비활성 상태여야 합니다. (specific)
        
        //_NA_0_20100901_SKILLTREE_SYSTEM
        RC_SKILL_CANT_FIND_UPPER_SKILL,             //< 상위 스킬을 찾을수없습니다.
        RC_SKILL_DISABLE_SKILL_LINE,                //< 해당 스킬라인은 사용할수 없다.

        //_NA_0_20110131_SKILLTREE_SYSTEM_RENEWAL
        RC_SKILL_ISNOT_ENOUGH_UPPER_SKILL_LEVEL,    //< 상위 스킬 레벨이 부족합니다.

        RC_SKILL_CANNOT_SUMMON,                     //< 소환할 수 없는 대상이다.
        RC_SKILL_CANNOT_HEAL,                       //< 회복스킬을 사용할 수 없는 대상이다.
        RC_SKILL_FP_RUN_SHORT,                      //< Fp가 부족하다   
        RC_SKILL_CANNOT_USE_SPA,                    //< 온천에서 스킬을 사용할 수 없습니다
    };

	enum eQUICK_RESULT
	{
		RC_QUICK_SUCCESS,
		RC_QUICK_FAILED,
		RC_QUICK_INVALIDSTATE,
		RC_QUICK_INVALIDPOS,
		RC_QUICK_INVALIDSLOTIDX,
		RC_QUICK_UNKNOWNERROR,
		RC_QUICK_CANNOT_LINK_CHARGE_INVENTAB,		// 인벤토리 탭의 아이템은 퀵창에 등록할 수 없다!
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
		RC_ROOM_ISNOTMATERPLAYER,				//< 방장이 아니다
		RC_ROOM_BEFOREENTERZONE,				//< 존에 들어가기전에 다시 패킷을 보내는 경우
		RC_ROOM_INVALIDROOMTYPE,				//< 잘못된 방 상태이다.
		RC_ROOM_INVALIDPREVLOBBY,				//< 이전의 방 번호가 잘못되었다.

		RC_ROOM_INVALID_ROOM_TYPE,				//< 룸타입이 잘못되었다.
		RC_ROOM_ALREADY_EXIST_PLAYER,			//< 이미 존재하는 플레이어
		RC_ROOM_ALREADY_DOING_TRANSACTION,		//< 이미 다른 트랜잭션이 진행중이다.
		RC_ROOM_CANNOT_JOIN_ROOMTYPE,			//< 들어 갈 수 없는 방타입입니다.
		RC_ROOM_NOTEQUAL_PASSWORD,				//< 패스워드가 틀립니다.
		RC_ROOM_FULL_ROOM,						//< 룸의 제한인원이 넘었습니다.
		RC_ROOM_INVALID_MAPCODE,				//< 맵코드가 잘못되어있다.
		RC_ROOM_INVALID_LIMITLEVEL,				//< 레벨제한값이 잘못되어있다.
		RC_ROOM_INVALID_LIMITCLASS,				//< 클래스제한값이 잘못되어있다.
		RC_ROOM_INVALID_LIMITCLASS_FOR_ME,		//< 자신의 클래스제한값과 맞지 않다.
		RC_ROOM_INVALID_LIMITLEVEL_FOR_ME,		//< 자신의 레벨과 맞지 않는 레벨 제한이다.
		RC_ROOM_INVALID_LIMIT_NUMBEROFPLAYER,	//< 플레이어 수 제한 값이 잘못되어있다.
		RC_ROOM_INVALID_BONUS_VALUE,			//< 보너스 값이 잘못되어있다.
		RC_ROOM_INVALID_FIELD_VALUE,			//< 필드 값이 잘못되어있다.
		RC_ROOM_INVALID_DIFFICULTY_VALUE,		//< 난이도 값이 잘못되어있다.
		RC_ROOM_INVALID_LASTROOMTYPE,			//< 생성하려는 룸타입과 현재의 룹타입이 틀립니다.
		RC_ROOM_NOTEXISTVILLAGE,				//< 마을이 존재하지 않는다.
		RC_ROOM_ALREADY_READY_STATE,			//< 이미 레디 상태이다.
		RC_ROOM_ALREADY_NOT_READY_STATE,		//< 이미 레디 상태가 아니다.
		RC_ROOM_IS_NOT_ALL_READY,				//< 모든사람이 레디상태가 아니다.
		RC_ROOM_INSUFFICIENT_USER_NUM,			//< 미션을 수행할 사람이 부족합니다.
		RC_ROOM_NOT_CMD_FOR_MASTER,				//< 방장이 할 수 있는 명령이 아니다.
		RC_ROOM_NOT_CMD_FOR_MEMBER,				//< 방구성원이 할 수 있는 명령이 아니다.
		RC_ROOM_NOT_EXIST_MEMBER,				//< 구성원이 방에 존재하지 않는다.
		RC_ROOM_NOT_EXIST_ROOM,					//< 내 조건에 맞는 방이 존재하지 않습니다.
		RC_ROOM_CANNOT_CREATE_ROOM_FOR_LIMIT,	//< 방 생성 제한 개수가 넘었습니다.
		RC_ROOM_INVALID_PLAYER_STATE,			//< 잘못된 플레이어의 상태이다.
		RC_ROOM_NEED_CHAR_LEVEL_10_FOR_PVP,		//< PVP 10레벨이상이 필요하다.
		RC_ROOM_NEED_EVEN_NUMBER,				//< PVP 인원 제한이 짝수여야 합니다.
		RC_ROOM_NOTEXISTHUNTINGROOM,			//< 헌팅방이 존재하지 않는다.
		RC_ROOM_INVALID_TEAM,					//< 잘못된 팀 번호입니다.
		RC_ROOM_INVALID_RULE_VALUE,				//< 잘못된 룰 값입니다.
		RC_ROOM_INVALID_MODE_VALUE,				//< 잘못된 모드 값입니다.
		RC_ROOM_INVALID_LIMITUSERNUM,			//< 잘못된 인원 제한 값입니다.
		RC_ROOM_INSUFFICIENT_MONEY,				//< 돈이 부족합니다.
		RC_ROOM_PREVIOUS_MISSION_CLEAR,			//< 미션을 클리어 해야 합니다.
		RC_ROOM_LIMITMAP_FOR_ME,				//< 나에게 제한된 맵입니다.
		RC_ROOM_SUCCESS_FOR_GENERAL_JOIN,		//< 방에 참가할 수 있다.
		RC_ROOM_SUCCESS_FOR_RECOMMEND_JOIN,		//< 추천레벨방이 존재한다.
		RC_ROOM_ISNOT_MASTER_OF_PARTY,			//< 파티장이 아니다.
		RC_ROOM_DOING_COMPETITION,				//< 경쟁이 진행중이라서 들어갈 수 없거나 로비방을 만들 수 없다.
		RC_ROOM_CANNOT_CHANGE_OPTION_ROOMTYPE,	//< 옵션을 바꿀 수 없는 룸타입이다.
		RC_ROOM_INVALID_CHUNTING_LINK_KEY,		//< 링크시킬 경쟁헌팅방이 없다.
		RC_ROOM_CAN_JOIN_CHUNTING_THROUGH_LOBBY,//< 로비를 거쳐야만 경쟁헌팅에 들어갈 수 있다.
		RC_ROOM_MEMBERNUM_OF_PARTY_IS_MANY,		//< 파티원의 인원수가 진행중인 파티원 수보다 많아 참가가 불가능하다.
		RC_ROOM_NEED_ITEM,						//< 필요 아이템을 소지하지 않고 있습니다.
		RC_ROOM_CHUNTING_PLAYTIMES_LIMIT,		//< 하루에 플레이할 수 있는 경쟁헌팅 횟수가 초과했습니다.
		RC_ROOM_ALREADY_EXIST_LINKED_LOBBY,		//< 이미 경쟁헌팅에 링크된 로비방이 있습니다.
		RC_ROOM_NEED2_CLEAR_PRE_MISSION,		//< 선행미션을 깨셔야 들어갈 수 있습니다.
		RC_ROOM_PREVIOUS_QUEST_CLEAR,			//< 퀘스트를 깨셔야 들어갈 수 있습니다.
		// 2006년 6월 1일 목요일 오후 4:56:08
		RC_ROOM_CHAR_BLOCK,						//< 블락된 캐릭터여서 입장할 수 없습니다.
		RC_ROOM_PARTY_MEMBER_ACCESS,			//< 파티멤버수가 초과해서 입장할 수 없습니다.(필드에 5명 이상의 파티는 입장할 수 없다.)
		RC_ROOM_DONT_EXIST_WAYPOINT,			//< 웨이 포인트에 존재하지 않아서 이동 할 수 없습니다.
		RC_ROOM_UNABLE_FUNCTION_FOR_CHAOSTATE,	//< 카오상태에서는 이용할 수 없는 기능이다.

		RC_ROOM_NOT_HAVE_SUPER_MASTER_ITEM,		//< 슈퍼방장 아이템을 가지고 있지 않다.
		RC_ROOM_ISNOT_PCBANG_USER,				//< PC방 유저가 아니다.

		RC_ROOM_CANT_RETURN_TO_TUTORIAL_FIELD_IN_LOBBY,		//튜토리얼 미션 로비에서는 돌아갈 수 없다.
		RC_ROOM_CANT_RETURN_TO_VILLAGE_IN_TUTORIAL_MISSION,	//튜토리얼 미션에서는 마을로 돌아갈 수 없다.

		RC_ROOM_MISMATCH_LEVEL_IN_PARTY,					//< 던전에 입장가능한 레벨이 부합하는 파티원이 있다.
		RC_ROOM_MISMATCH_NUMBER_IN_PARTY,					//< 던전에 입장가능한 인원이 아니다. 많거나 적거나...
		RC_ROOM_CAN_NOT_CREATE_PLACE,						//< 인던을 생성할 수 있는 장소가 아니다.
		RC_ROOM_MISMATCH_SAME_CLASS_IN_PARTY,				//< 던전에 입장가능한 클래스 조합이 아니다.
		RC_ROOM_MISMATCH_LEVEL_10_WITH_MASTER,				//< 파티장과 레벨 10이상 차이 남

		RC_ROOM_LOBBY_WAITTIME_NOT_EXPIRED,					//< 다른 파티원이 들어오기를 기다리는 중이다.
		RC_ROOM_NOT_OPEN_TIME,								//< 방을 생성(입장)할 수 있는 시간이 아니다.
#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
		RC_ROOM_CANNOT_CREATE_ROOM_FOR_NO_GUILD,			//< 길드원이 아니라서 길드PVP 방을 생성할 수 없다.
		RC_ROOM_SHORT_ENTRANCE_CARD_OF_GUILD_PVP,			//< 입장권이 부족합니다.
		RC_ROOM_CANNOT_JOIN_ROOM_FOR_DIFF_GUILD,			//< 같은 길드가 아니어서 입장할 수 없다.
		RC_ROOM_CANNOT_CREATE_ROOM_FOR_LIMIT_USER,			//< 유저수(5명)가 맞지 않아 길드PVP 방을 생성할 수 없다.
		RC_ROOM_CANNOT_CHANGE_LIMITUSERNUM_IN_GUILD_PVP,	//< 길드 PVP방에서는 유저제한수를 변경할 수 없습니다.
		RC_ROOM_IS_NOT_GUILD_PVP,							//< 길드 PVP방이 아니다.
#endif
		RC_ROOM_CANNOT_USE_ROOM_TITLE,						//< 사용할 수 없는 방이름
		RC_ROOM_CANNOT_WAYPOINT_PLAYERSTATE,				//< 상점을 연 상태에서 웨이포인트를 이용할 수 없습니다.	
		RC_ROOM_EXIST_CHAOSTATE_PARTYMEMBER,				//< 파티원중에 카오 상태의 유저가 있습니다.
		RC_ROOM_NOT_EXIST_ROOM_BY_FITNESS_LEVEL,			//< 해당 레벨에 맞는 방이 존재하지 않는다.
		RC_ROOM_CANNOT_ENTER_BEFORE_REWARD_OR_REFUND,		//< (보상/환불) 받기 전에 입장할 수 없습니다.
		RC_ROOM_NOT_OPEN_TIME_FOR_SSQ,						//< 기존 RC_ROOM_NOT_OPEN_TIME은 저탑용이라서리...
		RC_ROOM_CANNOT_JOIN_SOMEONE_PARTY_MEMBER,			//< 파티원 중 누군가 저탑에 입장 불가능한 상태이다
		RC_ROOM_NEED_UNEQUIP_WEAPON_FOR_ENTER,		        //< 입장하려면 공격무기 장착 해제 상태여야 합니다.
        //__NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__
        RC_ROOM_CANNOT_CREATE_RELAY_ROOM_BY_PORTAL_ENTERED, //< 포탈을 통한 입장시에는 재시작되지 않습니다.
        //__NA001334_090420_SERVER_SELECT_FACILITY__
        RC_ROOM_SERVER_SELECT_TRANSACTION_CANCELED,         //< 서버 선택 프로세스가 종료되었습니다.
        RC_ROOM_SERVER_SELECT_TRANSACTION_CANT_CANCEL,      //< 서버 선택 프로세스를 종료할 수 없습니다. (이미 다 준비된 상황인 경우)

        //_NA001385_20090924_DOMINATION_EVENT_CONTROLL
        RC_ROOM_NOT_OPEN_TIME_FOR_DOMINATION,				//< 지역 점령전에 입장할 수 있는 시간이 아니다.(공, 수 입장 불가 시간에 입장 시도)
        RC_ROOM_MISMATCH_JOIN_GUILD,                        //< 입장 할 수 있는 길드가 아니다. (공,수에 비해당 길드가 입장시도, 수비 입장 불가 시간에 입장 시도)
        RC_ROOM_LIMIT_MAX_GUILDMEMBER,                      //< 입장 할 수 있는 길드원수가 다찼다. ( 대기, 교전 시간대에 입장시도시 MAX 인원이 다찬 경우)         
        RC_ROOM_NOT_INCLUDE_GUILD,                          //길드가 없는 플레이다.

        RC_ROOM_CANNOT_BETAKEY,                             //배타키가 없는 사용자는 포탈 이동할수 없다.

        //_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
        RC_ROOM_ISNOT_JOINMEMBER,                           //< 점령전에 참여할수 있는 멤버가 아니다.
        // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
        RC_ROOM_BATTLE_GROUND_RESERVATION,                  //< 전장 예약중이다.
    };

	enum eLOGOUT_RESULT
	{
		RC_LOGOUT_NORMAL,						//< 정상적으로 로그아웃 되었습니다.
		RC_LOGOUT_WRONG_VERSION,				//< 버전이 일치하지 않습니다.
		RC_LOGOUT_AUTH_REQUEST,					//< 강제 로그아웃 요청으로 로그아웃 되었습니다.
		RC_LOGOUT_WRONG_AUTHKEY,				//< 올바른 인증절차를 거치지 않아서 로그아웃 되었습니다.
		RC_LOGOUT_HACKING_USER,					//< 서버로 잘못된 데이터를 전송해서 로그아웃 되었습니다.
		RC_LOGOUT_DB_ERROR,						//< 데이터에 오류가 발생해서 로그아웃 되었습니다.
		RC_LOGOUT_CHAR_BLOCK,					//< 블럭된 캐릭터여서 로그아웃 시킵니다.
		RC_LOGOUT_DUPLICATED_USER,				//< 중복UserGuid를 가진 유저가 이미 존재합니다. 인증을 통과해 버린 중복로그인!![버그]
	};

	enum eZONE_RESULT
	{
		RC_ZONE_SUCCESS,						//< 일반적으로 보내지 않을 것이다. 성공시 다른 처리로 이전되므로...
		RC_ZONE_FAILED,							//<	내부적인 (공개되지 않는) 이유로 입장 실패
		RC_ZONE_ALREADY_IN_ZONE,
		RC_ZONE_DISCONNECTED_DBPROXY,			//< db프락시가 끊어져 있다.
		RC_ZONE_CANT_FIND_ZONE,					//< 해당 방이 존재하지 않습니다.
		RC_ZONE_NOT_MATCHED_MAP_VERSION,		//< 맵버전이 다릅니다.
		RC_ZONE_MAP_PROBLEM,					//< 해당 맵에 문제가 있어 입장할 수 없습니다.
		RC_ZONE_ASSERTION_FLOW_VIOLATION,		//< (비공개) 확정 플로우 위반
        RC_ZONE_NOT_MATCHED_WMO_VERSION,        //< (비공개) wmo 버전이 틀려용 (맵버전이 다릅니다.) added by _NA000000_100623_CHECK_WMO_CHECKSUM_
	};

	enum eCONNECTION_RESULT
	{
		RC_CHARSELECT_OUTOFRANGEINDEX,		//< 캐릭선택인덱스가 잘못넘어옴
		RC_CHARINFO_DUPLICATEDREQ,
		RC_CONNECTION_NOTYETSERVICE,		//< 아직 서비스 준비중이라서 조금후에 다시 접속하3
		RC_CONNECTION_REQUESTFROMGAMESERVER,	//< 게임 서버에서 강제로 자름
		RC_CONNECTION_DONOTEXIST_LINKSERVER,
		RC_CONNECTION_ROOM_ALREADY_DOING_TRANSACTION,
		RC_CHARSELECT_MAPCODE_WRONG,			//< 선택한 캐릭터의 맵코드가 잘못됨
		// 현 패킷 플로우에 예기치 못한 패킷이 수신되었습니다.
		// 캐릭터 선택창에서 이 에러가 발생하면 C/S간 카테고리,프로토콜 불일치 문제일 수 있습니다.
		RC_CONNECTION_UNEXPECTED_PACKET,
		RC_CONNECTION_INVALID_CHAR_STATE,		//< 캐릭터 상태가 이상하다.
		RC_CONNECTION_DUPLICATED_CHAR_NAME,		//< 중복되는 캐릭터 이름이 존재한다.
		RC_CONNECTION_CHAR_BLOCK,				//< 블럭된 캐릭터 입니다.
		RC_CONNECTION_DEL_CHAR,					//< 삭제된 캐릭터 임

		//
		// enum eDISCONNECT_RESULT에서 이동된 것임
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
		RC_DISCONNECT_INCORRECT_VERSION,				//< 버전이 틀립니다.
		RC_DISCONNECT_DBP_CONNETION_ERROR,				//< DBP에 연결하지 못했습니다
		RC_DISCONNECT_FROM_GM,							//< GM에의해 강제 종료 되었습니다.
		RC_DISCONNECT_DUPLICATE_CHARNAME,				//< 캐릭터이름이 이미 게임에 있습니다.(중복)
		RC_DISCONNECT_REMAIN_USER_CONNECTION,			//< 새로운 캐릭터가 들어올려고해서 끊습니다.
		RC_DISCONNECT_REQ_AUTH,							//< 인증서버에서의 요청에의해서 끊습니다.
		RC_DISCONNECT_REQ_MASTER,						//< 마스터서버에서의 요청에의해서 끊습니다.
		RC_DISCONNECT_SERVER_ERROR,						//< 서버오류(다운)에 의해 접속을 끊습니다.
		RC_DISCONNECT_INCORRECT_AUTH_KEY,				//< 잘못된 인증키입니다.
		RC_DISCONNECT_DUPLICATE_LOGIN,					//< 중복 로그인
		RC_DISCONNECT_MISSION_REWARD_POLICYMENT,		//< 미션 보상 정책 위반 (비정규 플로)
		RC_DISCONNECT_BILLING_EXPIRED,					//< 빌링 만료
		RC_DISCONNECT_CRACKED_CLIENT,					//< 크랙된 클라이언트 사용중일 확률이 높음
		RC_DISCONNECT_IRREGULAR_CLIENT_CYCLE_TICK,		//< 비정상적 주기를 가진 클라이언트(스피드핵 가능성도 있음)
		RC_DISCONNECT_ARMORKIT_NOT_SERVICED,			//< 해당 클라이언트는 ArmorKit이 동작중이지 않습니다.
		RC_DISCONNECT_CANT_RECEIVE_CHAR_LIST_FROM_DBP,	//< 캐릭터 목록을 못받아서 종료하는 경우
		RC_DISCONNECT_USER_RELEASE,						//< 유저세션 릴리즈 시키면서 끊는 경우
		RC_DISCONNECT_OVERLAPPED_CONN_FROM_AUTH,		//< 중복 접속에 의한 Auth로부터 강제 접속 종료
		RC_DISCONNECT_INVALID_AUTH_FROM_AUTH,			//< 인증 오류에 의한 Auth로부터 강제 접속 종료
		RC_DISCONNECT_LOGIC_VIOLATION_OVER_LIMIT,
		RC_DISCONNECT_NOT_FOUND_DB_USER,				//< DB 유저를 찾지 못했습니다.
		RC_DISCONNECT_NOT_CONNECTED_DB_CHARACTER,		//< 접속하지 않은 DB 캐릭터 입니다.
		RC_DISCONNECTION_INVALID_CHAR_STATE,			//< 캐릭터 상태가 이상하다.
        //__NA001334_090420_SERVER_SELECT_FACILITY__
        RC_DISCONNECT_FOR_SERVER_SELECT_TRANSACTION,    //< 서버 선택 요청에 의해 현 게임서버와 종료되었습니다.
        //---(NO-ERROR-MESSAGE, 클라는 알아서 수신받은 정보를 가지고 인증서버에 접속한다.)
        // _NA_20100802_SPEED_HACK_DETECTION
        RC_DISCONNECT_SPEED_HACK_SUSPECTED,             //< 스피드핵 사용이 의심되어 강제 접속 종료
	};

	enum ePARTY_RESULT
	{
		RC_PARTY_SUCCESS,

		RC_PARTY_PLAYER_NOTEXIST,			//< 그런 플레이어가 존재하지 않는다.
		RC_PARTY_ISNOT_MASTEROFPARTY,		//< 파티장이 아니다.
		RC_PARTY_ISNOT_MEMBEROFPARTY,		//< 파티원이 아니다.
		RC_PARTY_PARTY_NOTEXIST,			//< 파티가 존재하지 않는다.
		RC_PARTY_REJECT_INVITEMENT,			//< 상대방이 파티초대 요청을 거절하다.
		RC_PARTY_ISNOT_QUALIFIED_LEVEL,		//< 레벨조건이 맞지 않는다.
		RC_PARTY_INVALID_ROOMTYPE,			//< 잘못된 방 상태이다.
		RC_PARTY_CANNOT_CHANGE_MASTER,		//< 파티장을 변경할 수 없다.
		RC_PARTY_ALREADY_IS_MEMBEROFPARTY,	//< 이미 파티에 소속되어 있다.
		RC_PARTY_WASNOT_REQUEST_PARTY,		//< 상대방이 파티를 요청하지 않았다.
		RC_PARTY_INVALID_DATATYPE,			//< 데이타 타입이 맞지 않는다.( 유효하지 않은 값이다. )
		RC_PARTY_PLAYER_NOTEXIST_TO_FIELD,	//< 플레이어가 필드에 존재하지 않는다.
		RC_PARTY_CANNOT_FIND_TARGET,		//< 타겟을 찾을 수 없다.
		RC_PARTY_TARGET_ISNOT_ENEMY,		//< 타겟이 아군이다.
		RC_PARTY_PLAYER_OBSERVER_MODE,		//< 플레이어가 투명모드라서 파티에 소속되지 않는다.
		RC_PARTY_PARTYMEMBER_NUM_FULL,		//< 파티원 수가 최대치라서 더이상 파티원을 받을 수 없다.
		RC_PARTY_NEXTMASTER_NOTEXIST,		//< 파티장으로 임명할 대상이 존재하지 않는다.
		RC_PARTY_INVALID_ZONESTATE,			//< 플레이어의 존상태가 이상하다.(존을 찾을 수가 없다.)
		RC_PARTY_REJECT_SUMMON,				//< 소환요청을 거절하다.
		RC_PARTY_SKILLINFO_IS_NULL,			//< 그런 정보가 존재하지 않는다.
		RC_PARTY_COOLTIME_ERROR,			//< 쿨타임이 끝나지 않았다.
		RC_PARTY_MASTER_NOTEXIST,			//< 파티장이 존재하지 않는다.		
		RC_PARTY_UNKNOWN_REASON,			//< 기타이유...
		RC_PARTY_INVALID_PLAYER_STATE,		//< 상대가 유효하지 않은 상태이다.
		RC_PARTY_GUIDANCE_NOTEXIST,			//< 가이던스가 존재하지 않는다.
		RC_PARTY_ISNOT_MASTEROFGUIDANCE,	//< 가이던스장이 아니다.
		RC_PARTY_ISNOT_MEMBEROFGUIDANCE,	//< 가이던스원이 아니다.
		RC_PARTY_CANNOT_CREATE_GUIDANCE,	//< 가이던스를 생성할 수 없다.
		RC_PARTY_CHANGE_ITEM_DISTRIBUTION_FAIL,		//<아이템 분배 타입 변경 실패
		RC_PARTY_CANNOT_INVITE_HOSTILITY,
		RC_PARTY_LEAVE_BY_NORMAL_DESTROY,	//파티 탈퇴가 정상적인 파티 해제에 의해 이루어짐
		RC_PARTY_LEAVE_BY_HOSTILITY_RELATION, //파티 탈퇴가 파티안에 있는 다른 길드원과 적대 관계에 의해 이루어짐
        RC_PARTY_CANNOT_INVITE_NEW_MEMBER,  // < 새로운 파티원을 받을 수 없는 상태 __NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__
        // _NA_0_20110210_INVITE_PARTY_BUG
        RC_PARTY_BEING_INVITE_REQUEST,      // 파티 초대 요청 중이다.
        RC_PARTY_BEING_INVITE_RESPONSE,     // 파티 초대 받는 중이다.
        RC_PARTY_INVITE_RESPONSE_TIME_OVER, // 파티 초대 응답 시간이 지났다.(시간제한 : 8 초)
        // _NA_20110630_BATTLE_ZONE_RENEWAL
        RC_PARTY_ALREADY_DOING_TRANSACTION, // 유저 이미 트렌젝션 중이다.
        RC_PARTY_PORTAL_ENTERANCE_MEMBER,   // 포탈 진입 예정 멤버입니다. - _NA003129_QUEST_PORTAL_CONVENIENCE_EXTENTION 
        // _NA_20110630_BATTLE_ZONE_RENEWAL
        RC_PARTY_FAIL_CREATE_PARTY_ROOM,    // 파티룸 생성에 실패 : 39
        RC_PARTY_FAIL_DESTROY_PARTY_ROOM,   // 파티룸 제거에 실패 : 40
        RC_PARTY_FAIL_DESTROY_PARTY_LIST,   // 파티 룸 리스트 제거에 실패
        RC_PARTY_FAIL_LEAVE_PARTY_ROOM,     // 파티 나가기에 실패
        RC_PARTY_FAIL_JOIN_PARTY_ROOM,      // 파티룸 참여에 실패
        RC_PARTY_FAIL_START_MISSION,        // 파티룸 미션 시작 실패
        RC_PARTY_FAIL_RESET_MISSION_CODE,   // 미션 코드 초기화에 실패
        RC_PARTY_FAIL_CHANGE_ROOM_OPTION,   // 파티룸 옵션변경에 실패
        RC_PARTY_FAIL_CREATE_ROOM,          // 룸 생성에 실패 하였다
        RC_PARTY_KICKOUT_PARTY_ROOM,        // 강제로 파티룸에서 KICK
        RC_PARTY_NOT_EXIST_PARTY,           // 파티가 존재 하지 않음
        RC_PARTY_NOT_INCORRECT_MAPCODE,     // 잘못된 맵코드(미션코드) : 50
        RC_PARTY_HAVE_NOT_REQUIRE_ITEM,     // 미션 생성에 필요한 아이템 없음
        RC_PARTY_NOT_FOUND_PARTYROOM,       // 파티룸 정보가 존재 하지 않음
        RC_PARTY_IS_NOT_PARTY_ROOM,         // 룸 형식의 파티가 아님
        RC_PARTY_INVALID_PARTY_ROOM_MAPCODE,// 잘못된 파티룸의 맵코드 정보
        RC_PARTY_NOT_MISSION_REWARD,        // 미션 보상을 받아야 한다
        RC_PARTY_NOT_ALLMEMBER_READY,       // 모든 멤버가 준비 상태가 아니다.
        RC_PARTY_NOT_OPEN_TIME,             // 해당 룸을 열수 있는 시간이 아니다.
        RC_PARTY_INVALID_ITEM,              // 아이템 사용 조건이 맞지 않는다.
        RC_PARTY_NOT_EXIST_ITEM,            // 필요한 아이템을 가지고 있지 않다.
        RC_PARTY_ROOM_NEED_UNEQUIP_WEAPON,  // 룸에 입장할때 장비 정보가 잘 못되어있다. : 60
        RC_PARTY_CANNOT_JOIN_SOMEONE_MEMBER,// 룸에 입장하지 못하는 멤버 존재
        RC_PARTY_NOTEQUAL_PASSWORD,         // 패스워드가 다르다
        RC_PARTY_NOT_FOUND_USER,            // 유저를 찾을 수 없다.
        RC_PARTY_NOT_MATCHING_USER_LEVEL,   // 레벨 조건에 맞지 않는다.
        RC_PARTY_NOT_CAHNGE_OPTION_PLAYING, // 미션 플레이 중에는 변경할 수 없다. : 65
        RC_PARTY_PREVIOUS_QUEST_CLEAR,      // 이전 퀘스트을 먼저 클리어 해야 한다.
        RC_PARTY_NEED_CLEAR_PRE_MISSION,    // 이전 미션을 먼저 클리어 해야 한다.
        RC_PARTY_FAIL_CHECK_TERMS,          // 조건 검사에서 실패 하였다.
        RC_PARTY_FAIL_MAX_MEMBER_COUNT,     // 맴버 숫자가 최대치 에 도달하였다.
        RC_PARTY_INVALID_PARTY_STATE,       // 파티의 상태가 비정상 적이다.
        RC_PARTY_NOT_FOUND_ROOM,            // 방이 존재 하지 않는다.
        RC_PARTY_DIFFER_TEAM,               // 전장 내 팀이 다르다.
        RC_PARTY_ALREADY_USE_IN_CHAOS_ZONE, // 이미 카오스존을 이용 중이다.
    };

	enum eGM_RESULT
	{
		RC_GM_ROOM_NOT_EXIST,				//< 방이 존재하지 않는다.
		RC_GM_INVALID_ARGUMENT,				//< 잘못된 인자값 이다.
		RC_GM_ISNOT_GM,						//< GM이 아니다
		RC_GM_NOTEXIST_USER,				//< USER가 존재하지 않는다.
		RC_GM_CANNOT_RESURRECTION,			//< 부활할수 없다.
		RC_GM_CANNOT_RECOVERY,				//< 회복할수 없다.
		RC_GM_PLAYER_NOTEXIST,				//< 그런 플레이어가 존재하지 않는다.
		RC_GM_NOT_KILL_MYSELF,				//< 자기 자신을 죽일수 없다.
		RC_GM_CURROOM_ISNOT_CHUNTING,		//< 현재 방타입이 경쟁헌팅이 아니다.
		RC_GM_MOVEZONE_FAIL,				//< 존이동에 실패 했습니다.
		RC_GM_USER_STATE_INVALID,			//< 유저의 상태가 이상하다.
		RC_GM_PLAYER_NOTEXIST_TO_FIELD,	
		RC_GM_PLAYER_NOTEXIST_TO_ZONE,
		RC_GM_CANNOT_MOVE_TO_BATTLEROOM,	//< 방으로의 이동은 지원하지 않는다.
		RC_GM_CMD_SUCCESS,					//< GM명령어 성공.
		RC_GM_NOTEXIST_SCRIPT,				//< 해당 스크립트 파일이 존재하지 않습니다.
		RC_GM_INVALID_VERSION_TYPE,			//< 해당 타입의 버전이 존재하지 않습니다.
		RC_GM_NOT_EXIST_LIMITED_INDEX,		//< 존재하지 않는 리미티드 인덱스 입니다.
		RC_GM_NOT_MATCH_FATE_RATE,			//< 맞지 않는 페이트 아이템 등급입니다.
		RC_GM_CANNOT_FIND_FATE_INDEX,		//< 페이트 아이템 인덱스를 찾을 수 없습니다.
		RC_GM_CMD_FAILED,					//< GM명령어 실폐.
	};

	enum eBATTLE_RESULT
	{
		RC_BATTLE_SUCCESS,

		RC_BATTLE_PLAYER_NOTEXIST_TO_FIELD,					//< 플레이어가 필드에 존재하지 않는다.
		RC_BATTLE_INVALID_MAINTARGET,						//< 메인타겟을 찾을 수 없다.
		RC_BATTLE_PLAYER_STATE_WHERE_CANNOT_ATTACK_ENEMY,	//< 공격자가 적을 공격할 수 없는 상태이다.
		RC_BATTLE_VKR_RELOAD_COUNT_LACK,					//< 발키리 장전개수 부족 
		RC_BATTLE_TRAGET_STATE_WHERE_CANNOT_ATTACKED,		//< 타겟이 공격받을 수 없는 상태이다.
		RC_BATTLE_OUT_OF_RANGE,								//< 메인타겟이 공격사거리에서 벗어남
		RC_BATTLE_INVLIDPOS,								//< 잘못된 위치정보가 왔습니다.
		RC_BATTLE_THRUST_DIST_OVER,							//< 클라에서 보내는 밀리는 거리가 너무 길다.(해킹 가능성)
		RC_BATTLE_ALREADY_DOING_ACTION,						//< 이미 공격중인 상태이다.
		RC_BATTLE_BASEINFO_NOTEXIST,						//< BaseStyleInfo가 없다.
		RC_BATTLE_STYLECODE_WHERE_DONOT_SELECT,				//< 선택하지 않은 스타일코드
		RC_BATTLE_CHAR_CLASS_LIMIT,							//< 클래스 제한(그 스타일을 사용할 수 없는 캐릭터 타입이다)
		RC_BATTLE_WEAPON_LIMIT,								//< 무기 제한
		RC_BATTLE_FIELD_IS_NULL,
		RC_BATTLE_ATTACKER_LEVEL_IS_LOW,					//< 공격자 레벨이 낮아서 PK를 할 수 없다.
		RC_BATTLE_TARGET_LEVEL_IS_LOW,						//< 타겟 레벨이 낮아서 PK를 당할 수 없다.
		RC_BATTLE_ISNOT_PK_ZONE,							//< PK가 가능한 지역이 아니다.
		RC_BATTLE_ISNOT_PK_SERVER,							//< PK가 가능한 서버가 아니다.
		RC_BATTLE_SERVER_STATE_IMPOSSIBLE_PK,				//< PK가 불가능한 서버상태이다.
		RC_BATTLE_PK_IMPOSSIBLE_TARGET_TYPE,				//< PK가 불가능한 타겟타입이다.
		RC_BATTLE_ALLIENCE_GUILD_CHARACTER,					//< 동맹 길드 캐릭터이므로 PK 불가능

		RC_BATTLE_ISNOT_TROUBLED_PART,						//< 타일이 분쟁 지역이 아니다
		RC_BATTLE_PLAYER_TILE_INVLIDPOS,					//< 대상의 타일이 존재하지 않는다.
		RC_BATTLE_FAILED,									//< 기타원인의 실패
		RC_BATTLE_SAME_GUILD_CHARACTER,						//< 같은 길드원이다.
		RC_BATTLE_SAME_PARTY_CHARACTER,						//< 같은 파티원이다.

        //__NA_000921_20080227_TROUBLED_PARTS               //< 클라이언트 사용 안할시 삭제될 데이터
        RC_BATTLE_ISNOT_CHAO_PART,							//< 카오 가능한 지역이 아니다.	//사용무
        RC_BATTLE_ISNOT_TROUBLED_ZONE,						//< 분쟁 가능한 지역이 아니다.	//사용무
        RC_BATTLE_IMPOSSIBLE_PACKET_DATA,					//< 패킷 데이터가 형식에 맞지 않는다. //사용무
    };

	enum eCONDITION_RESULT
	{
		RC_CONDITION_SUCCESS,
		RC_CONDITION_ALREADY_SAME_CONDITION,				//< 이미 같은 상태이다.
		RC_CONDITION_INVALID_CONDITION,						//< 존재하지 않는 상태이다.
		RC_CONDITION_DOING_ACTION,							//< 다른 동작을 진행중이다.
		RC_CONDITION_DRAGON_TRANSFORMATION_LIMIT,			//< 드래곤 변신 상태에서는 상태전환 불가
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
		RC_SUMMON_INVALID_COMMAND,				//< 존재하지않은 타입의 명령이다.
		RC_SUMMON_SUMMONED_NPC_NOTEXIST,		//< 소환체가 존재하지 않는다.
		RC_SUMMON_SAME_COMMAND,					//< 이미 내려진 명령이다.
        RC_SUMMON_CANNOT_ATTACK_PLAYER,			//< 해당 플레이어를 공격할 수 없다.
        //_NA001385_20090924_DOMINATION_SKILL
        RC_SUMMON_INVALID_PLAYER,			    //< 유저를 찾을수 없다.(시스템에러, 스트링 처리 불가)
        RC_SUMMON_INVALID_FIELD,			    //< 유저가 필드에 존재하지 않는다.(시스템에러, 스트링 처리 불가)
        RC_SUMMON_INVALID_DOMINATION_FIELD,     //< 유저가 지역점령전에 존재하지 않는다. (일반 필드에서 소환물 명령어 사용시)
        RC_SUMMON_INVALID_CRYSTALWARP,          //< 워프 소환체가 존재하지 않는다. (소환물 명령 사용시 소환체가 없을시)
        RC_SUMMON_CANNOT_WARP_CMD,              //< 2개의 수정워프 기둥이 설치 되지 않았는데 나머지 수정으로의 진입 시.
        RC_SUMMON_FAIL_WARP_CMD = 10,           //< 소환체 명령중 워프 로직 수행에 실패했다.
        RC_SUMMON_INVALID_GUILD,                //< 크리스탈워프 소환길드가 존재 하지 않습니다. (소환물 명령 사용시 길드를 찾을수 없다.)
        RC_SUMMON_FAIL_CREATE_SKILL,            //< 크리스탈워프 소환스킬(생성,해제) 사용에 실패하였다. (스킬 사용 로직에서 에러 발생, 클라에 MSG_CG_SKILL_ACTION_NAK) 
        RC_SUMMON_COOLTIME_ERROR,               //< 크리스탈워프 쿨타임이 남아있다.             (재소환 딜레이가 남아 있을시)
        RC_SUMMON_CRYSTALWARP_COUNTFULL,        //< 이미 2개의 크리스탈워프가 소환 되어있다.    (3개째의 크리스탈 워프 소환 시도시)
        RC_SUMMON_INVALID_SUMMONED,             //< 적합한 소환수가 아니다
        RC_SUMMON_RECALL_MAX,                   //소환 최대수를 초과 하였다.
	};

	enum eGUILD_RESULT
	{
		RC_GUILD_SUCCESS,
		RC_GUILD_FAILED,
		RC_GUILD_INVALID_STATE,
		RC_GUILD_CANNOT_CREAT_GUILD_STATE,
		RC_GUILD_CANNOT_CREAT_LEVEL_STATE,
		RC_GUILD_CANNOT_CREAT_MONEY_STATE,
		RC_GUILD_CANNOT_CREAT_SAMENAME_STATE,		//< 동일한 이름의 길드가 있습니다

		RC_GUILD_CANNOT_DESTROY_STATE,
		RC_GUILD_CANNOT_DESTROY_NOT_MASTER_STATE,
		RC_GUILD_CANNOT_DESTROY_EXIST_MEMBER_STATE,

		RC_GUILD_NOT_EXIST_TARGET_STATE,			//< 초대한 대상이 존재하지 않는다.	10
		RC_GUILD_CANNOT_INVITE_STATE,				//< 초대할 수 없는 상태이다.
		RC_GUILD_DONT_HAVE_RIGHTS_STATE,			//< 초대하는 권한이 없다.
		RC_GUILD_FULL_MEMBER_STATE,					//< 이미 길드원의 제한 인원이 꽉찼다.

		RC_GUILD_NOT_EXIST_HOST_STATE,				//< 초대한 사람이 존재하지 않는다.
		RC_GUILD_CANNOT_WITHDRAW_STATE,
		RC_GUILD_NOT_EXIST_GUILD,					// 길드가 존재하지 않습니다.
		RC_GUILD_CANNOT_JOIN_STATE,
		RC_GUILD_CANNOT_CHANGEPOSITION_STATE,
		RC_GUILD_CANNOT_SELECT_GUILD_STATE,			//< 길드 정보를 가져올 수 없는 상태이다.

		RC_GUILD_SUCCESS_SELECT,					//< 이미 존재하는 길드에 대해 성공하였다.(서버에서만 사용)	20
		RC_GUILD_DISCONNECTED_GUILD_SERVER,
		RC_GUILD_NEED_SELECT,						//< 길드가 없어서 select할 필요가 있다.(서버에서만 사용)
		RC_GUILD_WAIT_A_WHILE_THEN_TRY_AGAIN,		//< 잠시후 다시 시도해 주시기 바랍니다.
		RC_GUILD_ALREADY_SELECT_QUERY,				//< 이미 길드 정보 요청함(서버에서만 사용)

		RC_GUILD_CANNOT_KICK_STATE,
		RC_GUILD_CANNOT_KICK_RIGHT,

		RC_GUILD_CANNOT_CHANGENOTICE_STATE,

		// 길드 포인트 관련 Result값들
		RC_GUILD_NOT_ENOUGH_UP,						//< UP 부족합니다.
		RC_GUILD_NOT_ENOUGH_GP,						//< GP 부족합니다.
		RC_GUILD_CANNOT_RANKUP_MONEY_STATE,			//< 돈 부족하여 랭크업 불가능	30

		RC_GUILD_UNABLE_FUNCTION_FOR_CHAOSTATE,		//< 카오 캐릭터는 이용할 수 없는 기능이다
		RC_GUILD_INVALID_PLAYER_STATE,				//< 상대가 유효하지 않은 상태이다.

		//길드 2차 결과값
		RC_GUILD_LOW_GRADE_STATE,					// 길드 권한 수정 등급 낮음[서버용]
		RC_GUILD_DONT_RIGHTS_DUTY_STATE,			// 길드 권한 수정 직책 아님[서버용]
		RC_GUILD_NOT_GUILD_MEMBER,					// 길드 멤버가 아니다
		RC_GUILD_CHANGE_NOTICE_LENGTH_OVERFLOW,		// 길드 공지 길이가 넘 길다
		RC_GUILD_CURRENT_NONE_MASTER,				// 길드 마스터 존재 안함 
		RC_GUILD_INVITE_PLAYER_ALREADY_MEMBER,		// 초대한 대상이 이미 다른 길드 멤버임	38

		RC_GUILD_CREATE_CAMP_FAILED,				// 길드 훈련단 생성 실패
		RC_GUILD_CAMP_NUM_OVERFLOW,					// 길드 훈련단 갯수 넘음

		RC_GUILD_MARK_INVALID_TYPE,					// 길드 마크 형식에 어긋남
		RC_GUILD_MARK_CREATE_FAILED,				// 길드 마크 생성 실패
		RC_GUILD_MARK_NOT_READY,					// 길드 마크 전송 준비되지 않음
		RC_GUILD_NOT_EXIST_GUILD_SEAL,				// 길드 인장 없음
		RC_GUILD_MARK_ALEADY_EXIST,					//등록한 길드마크가 존재함
		RC_GUILD_MARK_LEVEL_LOW,					//길드 마크를 사용하기에는 레벨이 낮음
		RC_GUILD_CHANGE_MARK_NOT_RIGHT,				// 길드 마크 수정 권한 없음
		RC_GUILD_CHANGE_MARK_NOT_DUTY_STATE,		// 길드 마크 권한 불가 직책
		RC_GUILD_MARK_DELETE_FAILED,								// 길드 마크 삭제 실패

		RC_GUILD_DESTROY_PENALTY_TIME,				// 길드 해체 페널티 타임 실행중
		RC_GUILD_WITHDRAW_PENALTY_TIME,				// 길드 탈퇴 패널티 타임 실행중
		RC_GUILD_KICK_PENALTY_TIME,					// 길드 강퇴 패널티 타임
		RC_GUILD_RECOVERY_FAILED,					// 길드 복구 실패
		RC_GUILD_DELCHK_DESTROY_WAIT,				// 길드 삭제 대기중임

		RC_GUILD_NOT_FOUND_GUILD_INFO_FOR_GRADE,			// 해당 길드 등급에 길드 정보가 존재 하지 않습니다.
		RC_GUILD_MAX_GUILD_RANK,							// 현재 길드 등급이 최고 등급입니다.

		RC_GUILD_HAVE_NOT_RIGHT,							// 길드 권한이 없습니다.
		RC_GUILD_CANNOT_COMMAND_MYSELF,						// 자기 자신에게 명령을 내릴수 없습니다.
		RC_GUILD_CANNOT_CHANGE_DUTY_BECAUSE_SAME_DUTY,		// 현재 직책과 바꾸려는 직책이 같아서 직책을 바꿀수 없습니다.
		RC_GUILD_CANNOT_CHANGE_DUTY_BECAUSE_FULL_MEMEBER,	// 해당 직잭의 인원이 꽉 차서 직책 변경을 할 수 없습니다.
		RC_GUILD_CANNOT_CHANGE_DUTY_BECAUSE_INVALID_DUTY,	// 올바른 직책이 아니여서 직책을 변경할 수 없습니다.
		RC_GUILD_HAVE_NOT_RIGHT_TARGET_USER,				// 타켓유저에 해당 권한이 없습니다.
		RC_GUILD_INVALID_DELEGATE_RIGHT,					// 이상한 권한 위임값 입니다.
		RC_GUILD_PLAYER_ALEADY_DUTY,						// 길드 직책 이미 존재하는 플레이어
		RC_GUILD_CHANGE_MEMBER_DUTY_FAILED,					// 길드 멤버 직책 변경 실패
		RC_GUILD_CHANGE_MEMBER_RIGHT_FAILED,				// 길드 멤버 권한 변경 실패
		RC_GUILD_ALREADY_EXIST_DUTY,						// 이미 존재하는 직책 입니다.
		RC_GUILD_CANNOT_CHANGE_MASTER_IN_DAY_14,			// 길마를 변경한지 14일 내에는 다시 변경할 수 없다.
		RC_GUILD_CANNOT_CHANGE_DUTY_BY_WAREHOUSE_USEING, //길드 창고를 사용중인 유저는 직책을 변경할 수 없다.

		RC_GUILD_SELECT_CHAR_NO_GUILD_MEMBER,				// 길드 군단 생성 군단장이 길드원이 아님
		RC_GUILD_CREATE_CORPS_FAILED,						// 길드 군단 생성 실패
		RC_GUILD_CORPS_NUM_OVERFLOW,						// 길드 군단 갯수 넘음
		RC_GUILD_MEMBER_NOT_HEAD_CORPS,						// 길드원이 군단본부(직할군단) 소속이 아님.
		RC_GUILD_MEMBER_NOT_SOLDIER,						// 길드원이 정예군이 아니다.
		RC_GUILD_CORPS_NOT_FOUND,							// 길드 군단이 없습니다.
		RC_GUILD_ALREADY_GUILD_MEMBER,						// 이미 길드에 가입한 멤버 입니다.
		RC_GUILD_ALREADY_EXIST_COMMANDER,					// 이미 길드에 해당 군단장이 존재합니다.
		RC_GUILD_ALREADY_EXIST_TRAINING_LEADER,				// 이미 길드에 해당 훈련대장이 존재합니다.
		RC_GUILD_CANT_CHANGE_COMMANDER_BECAUSE_COMMANDER,	// 군단장은 군단장으로 직책 변경 할 수 없습니다.
		RC_GUILD_CORPS_FULL_MEMBER,							// 군단의 인원이 꽉 찼습니다.
		RC_GUILD_FAILED_BY_NOT_CONNECT_SERVER,				// 서버와의 연결이 끊어져 해당 길드 작업이 실패 하였습니다.
		RC_GUILD_INVALID_DUTY_IN_BASE_CORPS,				// 직할군단에서는 사용할 수 없는 직책 입니다.  
		RC_GUILD_NOT_SOLDIER,								// 정예병이 아닙니다.
		RC_GUILD_CANNOT_CHANGE_CORPS_BECAUSE_SAME_CORPS,	// 같은 군단내에서는 군단간 이동을 할 수 없습니다.
		RC_GUILD_ALREADY_EXIST_REAR_CAMP,					// 이미 육성캠프가 존재합니다.
		RC_GUILD_NOT_EXIST_REAR_CAMP,						// 육성캠프가 존재하지 않습니다.
		RC_GUILD_CANNOT_LEAVE_MASTER,						// 길마는 탈퇴가 불가능 합니다.
		RC_GUILD_INVALID_FUNCTION_IN_BASE_CORPS,			// 직할군단에서는 사용할 수 없는 기능 입니다.
		RC_GUILD_EXIST_MEMBER,								// 길드에 멤버가 존재합니다.
		RC_GUILD_ALREADY_EXIST_GUILD,						// 이미 길드가 존재합니다.
		RC_GUILD_CANNOT_LEVELUP,							// 레벨업을 할 수 없습니다.
		RC_GUILD_KICK_GUILD_BY_LEVELUP,						// 훈련병이 레벨업 31에 도달하여 길드를 탈퇴시킵니다.
		RC_GUILD_NOT_CORPS_MEMBER,							// 군단에 속한 멤버가 아닙니다.
		RC_GUILD_NOT_BASE_CORPS,							// 직할군단이 아닙니다.
		RC_GUILD_CANT_CHANGE_CAMP_SOLDIER_BY_LEVEL,			// 레벨제한 때문에 훈련병으로 변경이 불가능 합니다.
		RC_GUILD_CORPS_PENALTY_TIME,						// 길드 군단 패널티 타임 적용중 입니다.
		RC_GUILD_NOT_JOIN_GUILD,							// 길드에 가입하지 않았습니다.
		RC_GUILD_ALREADY_LOGIN_MEMBER,						// 이미 로그인한 멤버 입니다.
		RC_GUILD_OVER_TOTALINFO_REQ_CNT,					// 길드 토탈인포 요청횟수가 제한수치를 넘었습니다.
		RC_GUILD_CANNOT_JOIN_CAMP_SOLDERY_BY_LEVEL,			// 레벨에 의해 훈련병으로 가입할수 없습니다. ( 16레벨 이상의 캐릭터는 훈련병으로 가입할 수 없습니다. )
		RC_GUILD_CANNOT_MOVE_CORPS_BY_CORPS_PENALTY,		// 군단 패널티로 인해 패널티가 해제될때 까지 군단 이동을 할 수 없다.

        RC_GUILD_STRING_OVERFLOW,							// 길드 문자열 관련 오버플로우

        RC_GUILD_WAREHOUSE_UNABLE_CURR_GRADE,              //길드 등급이 길드 창고를 이용할 수 없음
		RC_GUILD_WAREHOUSE_ALEADY_USGIN_PLAYER,            //길드 창고를 사용중인 유저가 있음
		RC_GUILD_WAREHOUSE_PLAYER_DATA_DISCORD,            //유저의 길드창고 정보와 실제 길드의 길드 창고 정보가 불일치
		RC_GUILD_WAREHOUSE_INVENMONEY_LOW,					  //유저의 인벤에 있는 돈이 부족해 창고에 입금할수 없다.
		RC_GUILD_WAREHOUSE_MONEYOUT_NOENRIGHT,			 //유저의 길드 창고 돈을 출금할 권한이 없다.
		RC_GUILD_WAREHOUSE_INVENMONEY_LIMIT,                  // 유저 인벤의 돈이 한계치(42억)을 초과한다.
		RC_GUILD_WAREHOUSE_GET_CANNOT_DB,						//  길드 창고 열기 실패
		RC_GUILD_CANNOT_WITHDRAW_BY_WAREHOUSE_USEING,	//길드 창고 사용중이여서 탈퇴/제명을 할 수가 없다.
		RC_GUILD_RELATION_LIST_ERROR,	                    // 길드의 길드관계 정보 데이타 오류
		RC_GUILD_RELATION_ME_FULL,			                // 자신의 맺고자 하는 관계 리스트가 만땅이다.
		RC_GUILD_RELATION_OTHER_FULL,                    // 상대방의 관계 리스트가 만땅이다.
		RC_GUILD_RELATION_CHANGE_RIGHT_NOT,       // 길드 관계를 설정할 권한이 없다.
		RC_GUILD_RELATION_REMAIN_GUILD,                // 길드 관계가 남아있어서 길드를 해산할 수 없다.
		RC_GUILD_RELATION_ALEADY_SET,                    // 길드 관계를 맺으려는 길드와 이미 다른 관계가 성립되어 있다.
		RC_GUILD_RELATION_ALLIENCE_TYPE1_FAILED,  // 동맹을 맺으려는 길드가 나와 적대 관계를 맺고 있는 길드와 동맹관계라 동맹 관계를 맺을 수 없다.
		RC_GUILD_RELATION_ALLIENCE_TYPE2_FAILED,  // 동맹을 맺으려는 길드가 나와 적대 관계를 맺고 있는 길드와 동맹관계라 동맹 관계를 맺을 수 없다.
		RC_GUILD_RELATION_HOSTILITY_FAILED,            // 적대를 맺으려는 길드가 나와 동맹 관계를 맺고 있는 길드와 동맹관계라 적대 관계를 맺을 수 없다.
		RC_GUILD_RELATION_BOTH_HOSTILITY_FAILED,  // 상대 길드와 자신의 길드가 일방적 적대 상태 또는 적대 선포 관계가 아니기 때문에 쌍방 적대 관계를 맺을 수 없다.
		RC_GUILD_RELATION_NONE_DELETE_NOT,         // 상대 길드와 아무 관계를 맺고 있지 않기 때문에 관계 삭제를 할 수 없다.
		RC_GUILD_RELATION_GUILD_DESTROY_WAIT,     // 길드 해산 대기중이라 길드 관계를 맺을 수 없다.
		RC_GUILD_RELATION_GUILD_GRADE_LOW_ME,	// 자신의 길드 레벨(등급)이 낮아 관계를 맺을 수 없다.
		RC_GUILD_RELATION_GUILD_GRADE_LOW_OTHER, // 상대방 길드의 레벨(등급)이 낮아 관계를 맺을 수 없다.
		RC_GUILD_RELATION_DELETE_FAILED,				//길드 관계 삭제 실패
		RC_GUILD_RELATION_DELETE_FAILED_BY_TIME,  //적대 선포 삭제시 적대 선포를 선언한 후 1시간이 지나야 삭제할 수 있다.
        RC_GUILD_RELATION_CHANGE_FAILED_DOMINATION, // 점령전 진행중에는 길드관계를 변경할 수 없다. //_NA_006826_20130722_DOMINATION_RENEWAL

        RC_GUILD_NOTICE_CHNAGE_NONE_RIGHT,				// 길드 등급이 2레벨이 안되거나 플레이어가 공지 변경 권한이 없어서 길드 공지를 수정할 수 없다.
		RC_GUILD_DESTROY_WAIT_CANNOT_NONE_RIGHT,        //길드 해산 권한이 없다.
		RC_GUILD_DESTROY_WAIT_CANNOT_MEMBER_EXIST,      //길드 멤버가 존재하여 해산할 수 없다.
		RC_GUILD_DESTROY_WAIT_CANNOT_RELATION_REMAIN,   //길드 관계가 (일방적 적대관계 제외)남아있어서 해산할 수 없다.
		RC_GUILD_DESTROY_WAIT_CANNOT_GUILDWAREHOUSE_ITEM_REMAIN, //길드 창고에 아이템 또는 하임이 남아있어서 해산할 수 없다.
        //__NA000000_20081106_GUILD_NAME_CHANGE
		RC_GUILD_NAME_CHANGE_CANNOT_GRADE,				//길드명 변경 권한이 없다.
		RC_GUILD_NAME_CHANGE_CANNOT_DB,					//길드명 변경 프로시져 리턴값 fali
		RC_GUILD_NAME_CHANGE_CANNOT_DB_SYSTEM,			//길드명 변경 프로시져 리턴값 fali 시스템 오류
		RC_GUILD_NAME_CHANGE_CANNOT_DB_SAME_GUILDNAME,	//길드명 변경 프로시져 리턴값 fali 동일 길드명 존재
		RC_GUILD_NAME_CHANGE_CANNOT_DB_BLANK_SPACE,		//길드명 변경 프로시져 리턴값 fali 길드명 공백 이다
		
		RC_GUILD_NAME_BAN_WORD,							//길드명으로 쓸수 없는 단어이다.
		RC_GUILD_DISCONNECTED_DBP_SERVER,				//길드 서버가 DBP서버와 연결되어 있지 않다.

        //_NA001385_20090924_DOMINATION_ETC
        RC_GUILD_CANNOT_WITHDRAW_BY_DOMINATIONFIELD,	//점령전 맵에 존재 하는 유저라 탈퇴(자퇴) 할 수 없다. (해당 유저는 입구로 강퇴,재시도 바람)
        RC_GUILD_CANNOT_KICK_BY_DOMINATIONFIELD,	    //점령전 맵에 존재 하는 유저라 제명 시킬수 없다.      (해당 유저는 입구로 강퇴,재시도 바람)
        //
        RC_GUILD_DOMINATION_ACTIONINFO_CANNOT_DESTROY,  //점령전 참여 길드(경매)는 해산 신청 할 수 없다.
        RC_GUILD_DOMINATION_INFO_CANNOT_DESTROY,        //점령전 참여 길드(공격,수비)는 해산 신청 할 수 없다.

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        RC_GUILD_JOIN_REQUEST_FULL_LIST,                //가입 요청자 목록이 초과되었다.
        RC_GUILD_JOIN_REQUEST_ALREADY_EXIST,            //이미 가입 요청자 목록에 존재한다.
        RC_GUILD_JOIN_REQUEST_NOT_EXIST,                //가입 요청자 목록에 존재하지않는다.
        RC_GUILD_JOIN_REQUEST_GUILD_EXIST,              //요청자가 이미 길드를 가지고 있다.
        RC_GUILD_JOIN_REQUEST_INVALID_GUILD,            //길드를 찾을 수 없다.
        RC_GUILD_JOIN_REQUEST_REJECT,                   //가입 요청이 거절 당했다.
        RC_GUILD_JOIN_REQUEST_OVER_WAIT_TIME,           //가입 요청 대기 시간이 지났다.
        RC_GUILD_JOIN_REQUEST_PLAYER_MAX,               //플레이어가 할 수 있는 최대 요청수를 초과하였다.

        RC_GUILD_NOT_EXIST_FACILITY,                    //존재하지않는 시설이다.
        RC_GUILD_NOT_ENOUGH_UPGRADE_ITEM,               //길드 시설 업그레이드 아이템이 부족하다.
        RC_GUILD_NOT_ENOUGH_ACTIVE_ITEM,                //길드 시설 활성화 아이템이 부족하다.
        RC_GUILD_NOT_EXIST_FACILITY_ACTIVE,             //길드 시설 액티브가 존재하지 않는다.
        RC_GUILD_NOT_BE_ACTIVATED_IN_RIDE,              //탈것을 탄상태에서는 길드 시설을 활성화 할 수 없다.

        RC_GUILD_NOT_EXIST_PLAYER,                      //해당 길드 플레이어가 존재하지 않는다.
        RC_GUILD_FACILITY_CANNOT_ACTIVATED_FLYING,      // 비행상태에서는 길드시설 활성화 불가능하다
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
		//	EventInventory관련
		//RC_EVENT_ALREADY_ANOTHER_TRANSACTION,		//< 이미 다른 명령이 수행중입니다. 잠시후 다시 시도하세요.
		//RC_EVENT_CANNOT_MOVE_TO_INVENTORY,
		//RC_EVENT_FULL_INVENTORY,					//< 인벤토리가 꽉찼다.
		//RC_EVENT_NOTEXIST_EVENTITEM,				//< 이벤트아이템이 존재하지 않는다.
		//RC_EVENT_INVALID_ITEMCODE,					//< 스크립트에 존재하지 않는 아이템이다.

		//////////////////////////////////////////////////////////////////////////
		//	Event Packet Handling
		RC_EVENT_INVALID_EVENT,						// 이벤트 중이 아니다
		RC_EVENT_ITEM_NUM_OVERFLOW,					// 이벤트 아이템 등록 제한 개수를 넘었다
		RC_EVENT_NOTEXISTITEM,						// 이벤트에 응모할 아이템이 없다
		RC_EVENT_CANT_OBTAIN,						// 보상 아이템을 넣을 수가 없다 (지급 실패)
		RC_EVENT_INSUFFICIENT_HEIM,					// [하임보상] 기부하려는 하임 > 보유 하임 
		RC_EVENT_DONATION_HEIM_LIMIT,				// [하임보상] 최소 기부 하임 제한 만족 못함
		RC_EVENT_REWARDED_ITEM_NUM_OVERFLOW,		// [아이템보상] 더이상 아이템을 받을 수 없다.
		RC_EVENT_SUCCESS_HEIM_DONATION,				// [하임보상] 기부 성공 (보상은 못받는 일반 성공)
		RC_EVENT_ITEM_INVENTORY_LOCKED,				// [아이템보상] 인벤이 잠겨있어서 보상받지 못한다.
		RC_EVENT_CANT_BUFF_GRANTED,					// [버프이벤트] 이미 버프가 부여되어 있다
		RC_EVENT_UNQUALIFIED,						// 이벤트 보상을 받을 수 있는 자격이 되지 못한다. 사유는 별개 코드로...
		RC_EVENT_UNABLE_FUNCTION_FOR_CHAOSTATE,		//< 카오는 이용할 수 없음

		// UPPERBOUND - RC추가 정의시 이 앞에 입력할 것
		RC_EVENT_MAX,
	};

	enum eSYNC_RESULT
	{
		RC_SYNC_SUCCESS,
		RC_SYNC_FAILED,
		RC_SYNC_PLAYER_IS_DEAD,						// 플레이어가 죽어서 이동실패
		RC_SYNC_FIELD_IS_NULL,						// 플레이어의 필드값이 NULL이다.
		RC_SYNC_ROUTE_SIZE_EXCESS,					// 경로 개수 최대값 초과
		RC_SYNC_PLAYER_IS_UNMOVABLE_STATE,			// 플레이어가 이동할 수 없는 상태이다.
		RC_SYNC_SETPATH_ERROR,				
		RC_SYNC_INVALID_MOVESATE,					// 이동상태 이상(걷기, 달리기)
		RC_SYNC_CUR_POSITION_IS_INVALID,			// 클라가 보내준 현재좌표가 유효하지 않다.
		RC_SYNC_DEST_POSITION_IS_INVALID,			// 목적지 좌표가 유효하지 않다.
		RC_SYNC_JUMP_LENGTH_TOO_LONG,				// 점프거리가 너무 멀다
		RC_SYNC_ANIMATION_DELAY_PROCESSING,			// 클라 Animation Delay 고려한 처리 목적(서버 내부 처리) __NA001003_080527_PLAYER_ANIMATION_TIME_CONTROL__
		RC_SYNC_CUR_POSITION_IS_INVALID_NO_HACK,	// 클라가 보내준 현재좌표가 유효하지 않다. 맵의 경계선에서 클라가 조금 틀린 좌표를 보낼 수 있다.
	};

	enum eREWARD_RESULT
	{
		RC_REWARD_SUCCESS,
		RC_REWARD_FAILED,
		RC_REWARD_INSUFFICIENT_SPACE,
		RC_REWARD_FULL_MONEY,
		RC_REWARD_EMPTY,					//< 없다
		RC_REWARD_FATIGUE_TIME,				//< 피로시간 때문에 보상받을 수 없다.
        RC_REWARD_CANNOT_BETAKEY,           //보상 받을수 있는 배타키를 가지지 않았따.
	};

	enum eQUEST_RESULT
	{
		RC_QUEST_SUCCESS,
		RC_QUEST_FAILED,
		RC_QUEST_INSUFFICIENT_SPACE,
		RC_QUEST_FULL_MONEY,
		RC_QUEST_EMPTY,						//< 없다
		RC_QUEST_TOCOMPLETED,				//< 퀘스트가 완료되지 않는다.CanComplete()실패
		RC_QUEST_CANNOTREWARD_CHILDQ,		//< 자식퀘스트는 보상받을 수 없다.
		RC_QUEST_NOT_COMPLETED_STATE,		//< 퀘스트가 완료된 상태가 아니다.
		RC_QUEST_CAN_NOT_INCOMPLET,			//< 퀘스트를 Incomplet할 수 없다. CanComplete()성공
		RC_QUEST_CANNOTREWARD_FATIGUE,		//< 피로시간 때문에 보상받을 수 없다.
		RC_QUEST_CANNOTQUEST_MAXCOUNT,		//< 수행중인 퀘스트가 최대값이어서 더이상 받을 수 없다.
		RC_QUEST_UNABLE_FUNCTION_FOR_CHAOSTATE, //< 카오는 이용할 수 없음
		RC_QUEST_MUST_UNEQUIP_ALL_ITEM,			//< 옷 벗어라 __NA_0_STAT_INIT_WHEN_100LEVEL_CHANGEUP
		RC_QUEST_ALREADY_PROGRESS_TIME_QUEST,	//< 이미 진행중인 타임 퀘스트가 있다. __NA001282_20090206_MODIFY_QUEST_TIME
		RC_QUEST_OVER_REPEAT,						//반복횟수 초과
		RC_QUEST_MISMATCH_CHARACTER_LV,				//캐릭터 레벨 X
		RC_QUEST_NOT_CHAO,							//카오아님
		RC_QUEST_MISMATCH_CHARACTER_CLASS,			//캐릭터 클래스 X
		RC_QUEST_LESS_MONEY,						//돈이 모지람
		RC_QUEST_NOT_EXIST_NECESSARY_QUEST,			//필요 퀘스트가 없다.
		RC_QUEST_MUST_NOT_EXIST_QUEST,				//퀘스트가 없어야 한다.
		RC_QUEST_NOT_EXIST_NECESSARY_ITEM,			//필요 아이템이 없다.
		RC_QUEST_MUST_NOT_EXIST_ITEM,				//아이템이 없어야 한다.
		RC_QUEST_NOT_KILL_NECESSARY_MONSTER,		//필요 몬스터를 잡지 않았다.
		RC_QUEST_NOT_LESS_MISSION_POINT,			//미션 클리어 포인트 부족.
		RC_QUEST_NOT_CRASH_AREA,					//특정지역 충돌 X
		RC_QUEST_NOT_EMPTY_ITEM_SPACE,				//아이템 빈 공간 없음
        RC_QUEST_DISTANCE_DIFF_TOO_LONG,            //퀘스트 부여자와 수락요청자 사이가 너무 멀다.
        RC_QUEST_INCORRECT_POSITION,                //위치 정보가 올바르지 않다.
        RC_QUEST_NOT_AREAID,                        //해당 퀘스트에 동일 영역ID가 존재 하지 않는다.
        RC_QUEST_EXIST,                             //이미 존재하는 퀘스트
        RC_QUEST_CANNOT_BETAKEY,                    //사용자가 가지는 베타키로는 해당 퀘스를 할수 없습니다.
        RC_QUEST_COMPLETED_DAILY_QUEST,             //완료된 일일퀘스트
        RC_QUEST_MAX_DAILY_QUEST_COUNT,             //일일퀘스트 개수 제한
        RC_QUEST_PLAYER_BEHAVE_UNEXPECTED_WAITING,  //플레이어가 비정상 종료 대기중
        RC_QUEST_CHILD,                             //자식 퀘스트이기 때문에 할수 없음
        RC_QUEST_NOT_PLAYER,                        //플레이어 정보를 찾을 수 없습니다.
        RC_QUEST_NOT_DAILY,                         //일일퀘스트가 아닙니다.
        RC_QUEST_NOT_TODAY,                         //오늘 가능한 퀘스트가 없습니다.
        RC_QUEST_NOT_FIELD,                         //필드가 아닙니다.
        RC_QUEST_LESS_HONOR,                        //명예나 평판이 부족합니다.
        RC_QUEST_MAX_GUILD_DAILY_QUEST_COUNT,       //길드 일일퀘스트 개수 제한
#ifdef _NA_006607_20130402_ADD_QUESTTYPE
        RC_QUEST_NOT_ENOUGH_MISSION_CLEAR_COUNT,    //미션 클리어횟수가 부족하다.
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
		RC_AC_REWARD_FAIL_FATIGUE,		   // 피로시간이여서 AC보상을 받을수 없습니다.
	};

	enum eCHAR_RESULT
	{
		RC_CHAR_SUCCESS,
		RC_CHAR_FAILED,						//< DB로부터 올바른 정보를 받아오지 못했습니다.
		//RC_CHAR_EXIST_SAME_NAME,			//< 동일 캐릭터 명 존재함
		//RC_CHAR_SLOT_FULL,				//< 슬롯 풀
		//RC_CHAR_TRANSACTION_ERROR,		//< 트랜젝션 오류(쿼리오류,기타오류)
		//RC_CHAR_GUILD_MASTER,				//< 삭제 캐릭터가 길드 마스터임(삭제불가)
		//RC_CHAR_DISCONNECTED_DBPROXY,		//< DBP와의 연결이 끊어져 있습니다.(잠시후다시시도해주시기바랍니다)
		//RC_CHAR_INVALID_STATE,			//< 잘못된 상태에서 패킷이 왔습니다.
		//RC_CHAR_ALREADY_DOING_TRANSACTION,//< 이미 다른 트랜잭션이 진행중입니다.(패킷여러번전송하거나다른상황이진행중)
		//RC_CHAR_INCORRECT_SSN,			//< 잘못된 주민등록번호이다.
		//RC_CHAR_MINIMUM_CHARACTER,		//< 최소글자수 제한 오류 : 영문4글자, 한글2글자 이상
		//RC_CHAR_CANNOT_CREATE_TO_SERVICE,	//< 해달월드의 혼잡으로 신규캐릭터 생성이 불가능합니다.
		//RC_CHAR_DESTROIED,					//< 이미 파괴된 캐릭터 이다.

		RC_CHAR_DBUSER_ALREADY_CREATED,		//< DBP의 유저가 이미 만들어져 있다.
		//RC_CHAR_QUERY_ERROR,				//< DB쿼리 실패
		//RC_CHAR_DBUSER_NOT_EXIST,			//< DBProxy의 DB유저가 존재하지 않는다.
		//RC_CHAR_DBCHAR_ALREADY_CREATED,		//< DBP의 캐릭터가 이미 만들어져 있다.
		//RC_CHAR_DELETING_CHAR_NOT_EXIST,	//< DBP의 삭제할 캐릭터가 없다.
	};

	enum eCHAR_CREATE_RESULT
	{
		RC_CHAR_CREATE_SUCCESS,
		RC_CHAR_CREATE_ALREADY_DOING_TRANSACTION,	//< 이미 다른 트랜잭션을 진행중이다.
		RC_CHAR_CREATE_INVALID_PARAM,				//< 패킷이 잘못된 값입니다.
		RC_CHAR_CREATE_MINIMUM_CHARACTER,			//< 최소글자수 제한 오류 : 영문4글자, 한글2글자 이상
		RC_CHAR_CREATE_DISCONNECTED_DBPROXY,		//< DBP가 연결이 되지 않았습니다.(잠시뒤 서비스 이용바람)
		RC_CHAR_CREATE_CANNOT_CREATE_TO_SERVICE,	//< 해달월드의 혼잡으로 신규캐릭터 생성이 불가능합니다.
		RC_CHAR_CREATE_EXIST_SAME_NAME,				//< 동일 캐릭터 명 존재함
		RC_CHAR_CREATE_SLOT_FULL,					//< 슬롯 풀
		RC_CHAR_CREATE_TRANSACTION_ERROR,			//< 트랜젝션 오류(쿼리오류,기타오류)
		RC_CHAR_CREATE_QUERY_ERROR,					//< DB쿼리 실패
		RC_CHAR_CREATE_DBUSER_NOT_EXIST,			//< DBProxy의 DB유저가 존재하지 않는다.
		RC_CHAR_CREATE_DBCHAR_ALREADY_CREATED,		//< DBP의 캐릭터가 이미 만들어져 있다.
        RC_CHAR_CREATE_CHARNAME_BAN_WORD,			//< 금지 단어를 사용한 캐릭터명 
        RC_CHAR_CREATE_CANNOT_BETAKEY,              //사용자가 가진 베타키로는 해당 캐릭터를 생성할수 없습니다.
	};
	enum eCHAR_DESTROY_RESULT
	{
		RC_CHAR_DESTROY_SUCCESS,
		RC_CHAR_DESTROY_ALREADY_DOING_TRANSACTION,	//< 이미 다른 트랜잭션을 진행중이다.
		RC_CHAR_DESTROY_INVALID_PARAM,				//< 패킷이 잘못된 값입니다.
		RC_CHAR_DESTROY_DESTROIED,					//< 이미 파괴된 캐릭터 이다.
		RC_CHAR_DESTROY_INCORRECT_SSN,				//< 잘못된 주민등록번호이다.
		RC_CHAR_DESTROY_TRANSACTION_ERROR,			//< 트랜젝션 오류(쿼리오류,기타오류)
		RC_CHAR_DESTROY_DISCONNECTED_DBPROXY,		//< DBP가 연결이 되지 않았습니다.(잠시뒤 서비스 이용바람)
		RC_CHAR_DESTROY_GUILD_MASTER,				//< 삭제 캐릭터가 길드 마스터임(삭제불가)
		RC_CHAR_DESTROY_DBUSER_ALREADY_CREATED,		//< DBP의 유저가 이미 만들어져 있다.
		RC_CHAR_DESTROY_QUERY_ERROR,				//< DB쿼리 실패
		RC_CHAR_DESTROY_DBUSER_DONOT_EXIST,			//< DBProxy의 DB유저가 존재하지 않는다.
		RC_CHAR_DESTROY_DBCHAR_DONOT_EXIST,			//< 삭제할 캐릭터가 없다.
		RC_CHAR_DESTROY_FAILED,						//< 캐릭터 삭제 실패
        RC_CHAR_DESTROY_GROUP_MEMBER,               //< 그룹맴버가 존재하여 케릭터 삭제에 실패 하였다.//[20090811][4540]그룹 맴버 존재시 삭제 불가 메세지 추가
	};
	enum eCHAR_RECOVER_RESULT
	{
		RC_CHAR_RECOVER_SUCCESS,
		RC_CHAR_RECOVER_ALREADY_DOING_TRANSACTION,	//< 이미 다른 트랜잭션을 진행중이다.
		RC_CHAR_RECOVER_INVALID_PARAM,				//< 패킷이 잘못된 값입니다.
		RC_CHAR_RECOVER_NOTDESTROYED,				//< 삭제되지 않은 캐릭터이다.
		RC_CHAR_RECOVER_DISCONNECTED_DBPROXY,		//< DBP가 연결이 되지 않았습니다.(잠시뒤 서비스 이용바람)
		RC_CHAR_RECOVER_TRANSACTION_ERROR,			//< 트랜젝션 오류(쿼리오류,기타오류)
		RC_CHAR_RECOVER_DBUSER_DONOT_EXIST,			//< DBProxy의 DB유저가 존재하지 않는다.
		RC_CHAR_RECOVER_DBCHAR_DONOT_EXIST,			//< 복구할 캐릭터가 없다.
	};

	enum eCHAR_IDCHECK_RESULT
	{
		RC_CHAR_IDCHECK_SUCCESS,					
		RC_CHAR_IDCHECK_ALREADY_DOING_TRANSACTION,	//< 다른 트랜잭션을 진행중이다.
		RC_CHAR_IDCHECK_DISCONNECTED_DBPROXY,		//< DBP가 연결이 되지 않았다.
		RC_CHAR_IDCHECK_TRANSACTION_ERROR,			//< 트랜젝션 오류( 쿼리 오류, 기타 오류 )
		RC_CHAR_IDCHECK_DBUSER_DONOT_EXIST,			//< DBProxy의 DB유저가 존재하지 않는다.
		RC_CHAR_IDCHECK_FAILED,
	};

	enum eCHAT_RESULT
	{
		RC_CHAT_UNKNOWN,
		RC_CHAT_BLOCKED,					// 채팅 금지 상태
		RC_CHAT_ALLOWED,					// 채팅 허용 상태
		RC_CHAT_SYNC,						// 동기화 오류(마을/필드에서 발생하며, VIEWPORT_CHARSTATE를 다시 보내줘야 함)
		RC_CHAT_NOCHAR,						// 상대 캐릭터가 없거나 접속하지 않았음
		RC_CHAT_ALREADYSHOUT,				// 외치기 시간 간격 이전에 다시 시도
		RC_CHAT_NOROOM,						// 채널 채팅방에 들어가 있지 않은 상태에서 채널 채팅 관련 요청
		RC_CHAT_ALREADYINROOM,				// 이미 채널 채팅방에 들어있음
		RC_CHAT_NOT_SUPER_MASTER,			// 슈퍼방장이 아니므로 월드외치가 명령어 의미 없음
		RC_CHAT_NOT_DECREE_ITEM,
		RC_GENERAL_SHOUT_LIMIT_LEVEL,		// TW_일반 외치기 레벨 제한
		RC_CHANNEL_CHAT_TITLE_BAN_WORD,		// 채널 채팅 제목이 금지단어
		RC_CHAT_USE_BAN_WORD,				// 채팅 중 금지단어 사용
		RC_CHAT_ANTI_DOBAE,					// 채팅 도배 방지.

#ifdef __NA_0_20080515_BUG_FIX_WORLD_SHOUT_LEVEL
		RC_WORLD_SHOUT_LIMIT_LEVEL,		// TW_월드 외치기 레벨 제한
#endif
		RC_CHAT_BELONG_ZONE_BLOCK, 
        RC_CHAT_DENY_INVITATION, // 채팅 초대 거부 (채널 채팅)
        RC_CHANNEL_CHAT_BLOCKED, // 채널 채팅이 차단됨
	};

	enum eFRIEND_RESULT
	{
		RC_FRIEND_UNKNOWN,
		RC_FRIEND_NOTCONNECTED,				// 접속 안 돼 있음
		RC_FRIEND_NOTFRIEND,				// 친구 아님
		RC_FRIEND_DIFFCHANNEL,				// 채널이 다름
		RC_FRIEND_INVALIDZONE,				// 조인 가능한 방 타입(로비, 헌팅방)이 아님
        RC_FRIEND_SUCCESS,		            // ...	_NA_0_20100527_ADD_CHAR_RELATION_MEMO	
        RC_FRIEND_FAIL,
        RC_FRIEND_NOTGUILDMEMBER,		    // 길드 친구 아님.	
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
		RC_STATUS_STATE_NOT_EXSITS,				// 그런 상태가 존재하지 않는다.
		RC_STATUS_DRAGON_TRANSFORMATION_LIMIT,	// 드래곤변신 상태에서는 사용할 수 없는 기능
		RC_STATUS_ALIVE,						// 살아 있다.
		RC_STATUS_NOT_FALL_DEAD,				// 낙사해서 죽은 상태가 아니다.
		//RC_STATUS_INTERCEPTION_FAILED,
		RC_STATUS_TRADE_INTERCEPTION_FAILED,
		RC_STATUS_WHISPER_INTERCEPTION_FAILED,
		RC_STATUS_PLAYER_LEVEL_NOT_UNDER_20,
		RC_STATUS_NOT_EXIST_INIT_POINT,
		RC_STATUS_NOT_UNEQUIP_ALL,
		RC_STATUS_PLAYER_LEVEL_NOT_UNDER_40,	//__NA_1260_CHANGE_LEVEL_FOR_STAT_INIT
        //__NA001390_090915_RIDING_SYSTEM__
        RC_STATUS_CANT_EMOTION_ON_CURRENT_STATE,// 현재 상태에서는 이모션을 취할 수 없습니다.
        // _NA_0062_20100318_CHANGE_LEVEL_FOR_STAT_INIT_2
        RC_STATUS_PLAYER_LEVEL_TOO_HIGH_FOR_FREE_STAT_INIT, // 캐릭터 레벨이 높아 무료 스텟 초기화 실패
	};

	// 웨이 포인트 관련
	enum eWP_RESULT
	{
		RC_WP_SUCCESS,

		RC_WP_FAILED,
		RC_WP_CANNOT_LOAD_FROM_DB,				//< DB로부터 정보를 가져올 수 없습니다.
		RC_WP_CANNOT_SAVE_FROM_DB,				//< DB로부터 정보를 저장할 수 없습니다.
		RC_WP_ALREADY_SAVE_MAPCODE,				//< 이미 저장되어있는 MapCode입니다.
		RC_WP_UNABLE_FUNCTION_FOR_CHAOSTATE,	//< 카오상태에서는 이용할 수 없는 기능이다.

        // 901, __NA001164_20081024_CHAR_WAY_POINT_SET_DB_FUNC
		RC_WP_CANNOT_DELETE_FROM_DB,			//< DB로부터 정보를 삭제할 수 없습니다.
		RC_WP_CANNOT_FIND_WP_IDX,				//< 웨이포인트 인덱스를 찾을수 없습니다.
		RC_WP_CANNOT_FIND_WAYPOINT_ITEM,		//< 웨이포인트 아이템을 찾을수 없습니다.
		RC_WP_UNABLE_DELETE_FOR_DELTIME,		//< 삭제불가시간이 남아 있어 삭제기능을 사용할수 없습니다.
		RC_WP_UNABLE_FUNCTION_FOR_OVERCOUNT,	//< 저장 가능한 갯수를 넘었다.
		RC_WP_UNABLE_FUNCTION_FOR_ZONETYPE,		//< 마을, 필드에서만 사용 가능하다.
		RC_WP_NOT_YET_LOAD_FROM_DB,				//< DB로 부터 정보가 LOAD 되어 있지 않다.
        RC_WP_UNABLE_FUNCTION_FOR_CUR_ZONE,		//< 웨이포인트로 저장할수 없는 위치에 있습니다.        
        RC_WP_PORTAL_FAIL,						//< 웨이포인트로 이동에 실패했습니다.
        //
	};

	// 리벤지 리스트 관련
	enum eREVENGE_RESULT
	{
		RC_REVENGE_SUCCESS,
		RC_REVENGE_FAILED,					// 알 수 없는 에러
		RC_REVENGE_NO_POINT,				// 리벤지 포인트 부족
		RC_REVENGE_INVALID_CHAR,			// 리벤지 대상이 아님
		RC_REVENGE_INVALID_SITUATION,		// 리벤지 가능한 상황이 아님(부당한 PK당한 상태가 아님)

		RC_REVENGE_NO_CHAR,					// 대상이 존재하지 않음
		RC_REVENGE_REVENGEITEM_NOT_EXSITS,	// 복수의 돌이 존재하지 않는다
		RC_REVENGE_CHAR_INVALID_STATE,		// 캐릭터가 아이템을 사용할 수 없는 상태임
		RC_REVENGE_INVALID_ROOM,			// 대상이 유효한 방에 존재하지 않음
		RC_REVENGE_MOVEZONE_FAIL,			// 존이동 실패
		RC_REVENGE_INVALID_ZONE_LEVEL,		// 존에 갈 수 없는 레벨	// __BUG_2598_REVENGE_STONE_MAP_LEVEL_BUG
	};

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
    enum eSUN_RANKING_RESULT
    {
        RC_SUN_RANKING_SUCCESS,                 // 성공
        RC_SUN_RANKING_FAILED,                  // 실패
        RC_SUN_RANKING_INVALID_RANKING_TYPE,    // 알 수 없는 랭킹 타입
        RC_SUN_RANKING_SETTLE,                  // 정산중
    };
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM

	enum eRANKING_RESULT
	{
		RC_RANKING_SUCCESS,
		RC_RANKING_FAILED,						// 알 수 없는 에러
		RC_RANKING_INVALID_STATE,				// 잘못된 상태
		RC_RANKING_ALREADY_REWARDED,			// 이미 보상 받은 상태
		RC_RANKING_DOING_TRANSACTION,			// 트랜잭션 진행중, 이미 다른 작업 혹은 그 작업이 진행중임 ( 패킷 2번일 가능성! )
		RC_RANKING_NETWORK_ERROR_FOR_PORTAL,	// 포탈 서버가 끊어져 있다. 네트워크 장애로 일시 기능 중지
		RC_RANKING_NETWORK_ERROR_FOR_DBPROXY,	// DBP 서버가 끊어져 있다. 네트워크 장애로 일시 기능 중지
		RC_RANKING_NOT_SPACE_FOR_REWARD,		// 보상을 받을 아이템 공간이 부족하다.

		// 제한 조건 관련 에러
		RC_RANKING_INSUFFICEINT_PARTY_MEMBER,	// 랭킹을 등록하기 위한 파티멤버(현재5명)가 부족하다.
		RC_RANKING_A_CLASS_OBTAIN_MAJORITY,		// 한 클래스가 과반수를 차지 하면 안된다.
		RC_RANKING_DONT_INCLUDE_MISSION_LEVEL_AREA,// 미션레벨구간에 포함되지 않는 플레이어가 존재한다.
		RC_RANKING_INVALID_PARAM,					// 잘못된 인자입니다.

		RC_RANKING_DONT_INCLUDE_RANKING,			// 랭킹에 포함되지 않았습니다.
		RC_RANKING_INVALID_REWARD_INDEX,			// 보상 인덱스가 유효하지 않습니다.
		RC_RANKING_SETTLING_INFO,					// 랭킹 산정중, 랭킹관련 서비스 중지중
		RC_RANKING_DBPROXY_SP_LENGTH_OVERFLOW,		// 미션 랭킹 SP 문자열 오버 플로우 발생
		RC_RANKING_DBPROXY_SP_TRANS_ERROR,			// 미션 랭킹 SP 트랜잭션 오류 발생
		RC_RANKING_UNSATISFY_KILL_MONSTER_NUM,		// 최소 몬스터 킬수가 부족합니다.

		// 미션랭킹 실패관련
		RC_RANKING_NOT_FIND_REWARD_USER,			// 미션랭킹를 보상받을 유저를 찾지 못하였습니다.
		RC_RANKING_NOT_FIND_RANKING_INFO,			// 미션랭킹정보를 찾지 못하였습니다.

		RC_RANKING_REWARD_FAIL_FATIGUE,				// 피로시간이여서 미션랭킹보상을 받지 못하였습니다.
        RC_RANKING_NOT_REGIST_INVALID_PERIOD,		//유효한 기간이 아니라 래킹 등록 불가
	};

	enum eMEMO_RESULT
	{
		RC_MEMO_SUCCESS,
		RC_MEMO_FAILED,
		RC_MEMO_WAIT_INFO,						// 일정 시간 안에 계속 요청할 수 없음
		RC_MEMO_DOING_TRANSACTION,				// 이미 메모 관련 트랜잭션중
		RC_MEMO_NOUSER,							// 해당 사용자 없음
		RC_MEMO_NOMEMO,							// 해당하는 쪽지가 없음
		// 902, __NA001254_20090109_RTTG_FUNC_ADD
		RC_MEMO_NOSERVER,						// 서버에러 (해당 서버 없음, 전송실폐)
		RC_MEMO_WRONG_RECVNAMES,				// 받는 케릭명이 잘못되었음.()
		RC_MEMO_DBFAILED,
		//
	};

	enum ePORTAL_RESULT
	{
		RC_PORTAL_FAIL,
		RC_PORTAL_INSTANT_MOVE,					// 즉시이동(같은 필드)
		RC_PORTAL_AGENT_RELAY_MOVE,				// Agent 중개 이동(다른 필드, 마을)
        // 901, __NA001164_20081024_CHAR_WAY_POINT_PORTAL
        RC_PORTAL_CAN_SUCCESS,				    // 포탈 이동 가능 상태
        RC_PORTAL_UNABLE_CUR_ZONE,              // 포탈 사용 불가 존 (현재 존)
        RC_PORTAL_UNABLE_DEST_ZONE,              // 포탈 사용 불가 존 (목적지)
        RC_PORTAL_FIELD_IS_NULL,                // 필드가 없다.
        RC_PORTAL_INVALIDSTATE,                 // 포탈 불가능 상태.
	};

	enum eITEM_DELETE_RESULT
	{
		RC_ITEM_DELETE_REVENGE_STONE,			// 복수의돌 삭제
		RC_ITEM_DELETE_PORTAL_SUCCESS,			// 좌표이동 성공			
		RC_ITEM_DELETE_PROTECT_DROP,			// 드랍방지
		RC_ITEM_DELETE_EXPIRED_CHARGE_ITEM,		// 유효기간이 지난 아이템입니다. 삭제합니다.
		RC_ITEM_DELETE_SUCCESS,					// 아이템이 삭제 되었습니다.(기타이유)
		RC_ITEM_DELETE_CLEAR_COORD,				// 좌표 초기화 아이템 삭제
		RC_ITEM_DELETE_USE_KEY_SUCCESS,			// 인던 입장 키 사용 성공
	};

	enum eFATIGUE_REWARD_RESULT					// 피로도 관련 result
	{
		RC_FATIGUE_REWARD_ZERO = 0,				// 1시간 미만
		RC_FATIGUE_REWARD_OVER_AN_HOUR,			// 1시간 이상 3시간 미만
		RC_FATIGUE_REWARD_OVER_THREE_HOURS,		// 3시간 이상 5시간 미만
		RC_FATIGUE_REWARD_OVER_FIVE_HOURS,		// 5시간 이상
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
		RC_INSERT_SLOT_UNABLE_GUILDWAREHOUSE_POSSESSION_ITEM,	// 귀속된 아이템은 길드창고에 옮길 수 없다.
	};

	enum ePET_SUMMON_RESULT
	{
		RC_PET_SUMMON_SUMMON_SUCCESS = 0,				// 소환 성공
		RC_PET_SUMMON_RETURN_SUCCESS,					// 해제 성공		
		RC_PET_SUMMON_RETURN_FAILED,					// 실패, 원인 모름 또는 클라 몰라도 됨
		RC_PET_SUMMON_ALREADY_SUMMONED,					// 이미 다른 펫이 소환 되어 있어 실패
		RC_PET_SUMMON_NOT_PET_ITEM,						// 펫 아이템이 아님
        RC_PET_SUMMON_NOT_SUMMONED_PET,					// 소환된 펫이 없는데 RETURN 하려고 함
		RC_PET_SUMMON_MISMATCH_PET_ITEM,				// 소환된 펫이랑 해제하려는 아이템이 서로 다름.
		RC_PET_CAN_NOT_SUMMON_FULLNESS_ZERO,			// 펫 포만감이 0이어서 소환 불가
		RC_PET_EXPIRED_CHARGE_ITEM,						// 기간 만료된 아이템
	};

    //__NA001390_090915_RIDING_SYSTEM__
    enum eRIDER_RESULT
    {
        RC_RIDER_SUCCESS                    = 0,    // 성공
        RC_RIDER_FAILED                     ,       // 실패 (Normal)
        RC_RIDER_UNEXPECTED_FAILED          ,       // 실패 (Unexpected)
        RC_RIDER_CANT_FIND_INFO             ,       // 라이더에 관련된 정보를 찾을 수 없다. (Script?
        RC_RIDER_NOT_RIDER_ITEM             ,       // 라이더 아이템 아님
        RC_RIDER_TIME_EXPIRED               ,       // 기간 만료된 아이템
        RC_RIDER_ALREADY_SUMMONED           ,       // 이미 소환되어 있습니다.
        RC_RIDER_ALREADY_SUMMON_RELEASED    ,       // 이미 소환 해제되어 있습니다.
        RC_RIDER_MISMATCH_SUMMON_RIDER      ,       // 소환된 상태의 라이더와 해제하려는 라이더가 틀리다.
        RC_RIDER_SUMMON_CASTING             ,       // 소환 캐스팅 중입니다.
        RC_RIDER_SUMMON_RELEASED_BY_HIT     ,       // 성공|다치거나 죽거나 해서 소환 해제됩니다.

        RC_RIDER_COUNTS                         // Upperbound
    };

    // 902, __NA001283_20090225_COLLECTION_USE
	enum eCOLLECT_RESULT
	{
		RC_COLLECT_SUCCESS = 0,							
		RC_COLLECT_FAIL,								// 채집물에 이상이 있다. (시스템 오류)
		RC_COLLECT_ALREADY_COLLECTED,					// 해당 채집물은 이미 채집중이거나 채집 되었다. (채집 시도시)
		RC_COLLECT_NOTEXIST,							// 채집물이 존재 하지 않는다. (object 가 존재하지 않거나, 채집ID가 존재하지 않는다.)
		RC_COLLECT_NOTCOLLECTING,						// 채집물의 상태가 채집중이 아니다. 취소할수 없다.
		RC_COLLECT_FAIL_CANCEL,							// 취소에 실패했다.
		RC_COLLECT_FAIL_QCONDISION,						// 퀘스트 조건이 맞지 않는다.
		RC_COLLECT_FAIL_ACCONDISION,					// AC조건이 맞지 않는다.
		RC_COLLECT_FAIL_COUNTCONDISION,					// 같은 종류의 채집물을 더이상 채집할수 없다.
		RC_COLLECT_POSITION_INVALID,					// 플레이어가 존재할수 없는 위치에 있다.	
		RC_COLLECT_DISTANCE_INVALID = 10,				// 최대 채집 가능 거리를 벗어났다.
		RC_COLLECT_ALREADY_COLLECTING,					// 플레이어는 채집중에 있다.
		RC_COLLECT_FAIL_DB_ADD,							// DB에 채집물품 저장 실패
		RC_COLLECT_FAIL_REWORD_EXIT_CHAR,				// 보상 실패(캐릭터를 찾을수없다)
		RC_COLLECT_FAIL_REWORD_DEAD_CHAR,				// 보상 실패(캐릭터가 죽어있다)
		RC_COLLECT_FAIL_REWORD_WRONG_COLLECTOBJKEY,		// 보상 실패(채집물키가 시작시 설정된 키와 같지 않다)
		RC_COLLECT_FAIL_REWORD_POS,						// 보상 실패(시작시 위치와 보상시 위치 차이가 보상받을수 있는 최대 거리를 넘었다)
		RC_COLLECT_FAIL_REWORD_DISTANCE,				// 보상 실패(보상받을수 있는 채집물과의 최대 거리를 넘었다)
		RC_COLLECT_FAIL_REWORD_ITEM,					// 보상 실패(보상아이템 지급에 실패했다)
		RC_COLLECT_CHAR_NOTCOLLECTING,					// 플레이어는 채집중이 아니다.	(채집 실패 패킷을 받을시 케릭터가 채집중인 채집물이 없을때. )
		RC_COLLECT_FAIL_DB_DEL = 20,					// DB에 채집물품 삭제 실패
        RC_COLLECT_FAIL_OBJECTID_CONDISION,             // 오브젝트 파괴 조건이 맞지 않는다.
        RC_COLLECT_FAIL_STAMP_CONDISION,                // 각인 조건이 맞지 않는다. (길드마스터아님)

        // _NA001385_20090924_DOMINATION_ETC
        RC_COLLECT_FAIL_STAMP_DEAD_CHAR,				// 각인 실패(캐릭터가 죽어있다)

        RC_COLLECT_FAIL_HAVENOTSPACE,
	};

    //지역 점령전 요청 결과 타입
    enum eDOMINATION_RESULT
    { 
        DOMINATION_RESULT_FAIL= 0,                  //단순 실패.
        DOMINATION_RESULT_SUCCESS,                  //단순 성공.
        DOMINATION_RESULT_DBERROR,                  //DATA 저장 실패,
        DOMINATION_RESULT_NOT_GUILDMASTER,          //길드 마스터가 아니라 실패 하였다. (하임 배팅, 성문 셋팅시)
        DOMINATION_RESULT_GUILDLEVEL_FAIL,          //길드 레벨이 유효하지 않다.
        DOMINATION_RESULT_NOT_ENOUGHMONEY,          //소유 하임이 부족하다. (하임 배팅시)
        DOMINATION_RESULT_NOT_GUILDMEMBER,          //길드 맴버가 아니다.   (하임 배팅, 성문 셋팅시)
        DOMINATION_RESULT_LIMIT_ENOUGHMONEY,        //배팅 금액이 지역 점령 기본 배팅 하임보다 부족하다.
        DOMINATION_RESULT_BEST_ENOUGHMONEY,         //배팅 금액이 지역 점령 경매 최고 하임보다 부족하다
        DOMINATION_RESULT_DEFFENSEWALL_OVERPOINT,   //방벽에 쓰일 포인트가 총 포인트보다 많다. (max포인트(5) 보다 많은 포인트가 쓰인 경우 )
        DOMINATION_RESULT_USER_LOCATION_FAIL,       //유저가 위치한 지역이 지역 점령전 포인트가 아니다. (일반 필드에서 하임 배팅, 성문 셋팅시)
        DOMINATION_RESULT_GUILDSTAT_DESTROYWAIT,    //길드가 삭제 대기 상태에 있다. (경매 참여시)
        DOMINATION_RESULT_EMPTY_DATA,               //데이터가 존재하지 않는다.
        DOMINATION_RESULT_OVERAP_APPLY,             //재입찰의 경우, SUCCESS 의미도 가지고 있다.
        DOMINATION_RESULT_LESS_TOKEN,               //왕의 증표 갯수 부족
        DOMINATION_RESULT_CAN_JOIN_ONLYONE_AREA,    //점령전 참여는 한지역만 가능하다.
        DOMINATION_RESULT_CAN_BETTING_ONLYONE_AREA, //점령전 입찰은 한지역만 가능하다.
        DOMINATION_RESULT_NOT_PLAYER,               //플레이어 객체가 없다.
        DOMINATION_RESULT_NOT_IDLE_STATE,           //플레이어가 IDLE 상태가 아니다.
        DOMINATION_RESULT_NOT_FOUND_AREA,           //지역 객체를 찾을수 없다.
        DOMINATION_RESULT_NOT_FOUND_EVENT,          //점령전 이벤트를 얻을수 없다.
        DOMINATION_RESULT_NOT_JOIN_GUILD,           //참여 가능한 길드가 아니다.
        DOMINATION_RESULT_OVERFLOW_MEMBER,          //참여할수 있는 멤버수를 초과 했다.
        DOMINATION_RESULT_AUCTION_LOW_LEVEL,        //길드 레벨이 낮아 점령전 입찰을 할수 없음
        DOMINATION_RESULT_PENALTY_TIME,             // 입찰 제한 기간
    };

#ifdef _NA_1419_20100121_ACCUMULATE_POINT
    enum eACCUMULATE_POINT_RESULT
    {
        RC_ACCUMULATE_POINT_SUCCESS,
        RC_ACCUMULATE_POINT_FAILED,
        RC_ACCUMULATE_POINT_INITIALIZE,             // 년도가 바뀌어 적립 시간 초기화 됨.
        RC_ACCUMULATE_POINT_NOSPACEININVENTORY,     // 인벤토리 공간 없음.
        RC_ACCUMULATE_POINT_HAVENOTPOINT,           // 적립 포인트 부족
        RC_ACCUMULATE_POINT_CHANGE_REQUIRE_POINT,   // 필요 적립 포인트 변경됨.
    };
#endif

#ifdef _NA_0_20110216_STATUS_STAT_POINT_SIMULATION
    enum eSTAT_POINT_RESULT
    {
        RC_STAT_POINT_SUCCESS,
        RC_STAT_POINT_FAILED,
        RC_STAT_POINT_IS_NOT_ENOUGH,                // 스텟 포인트 부족.
        RC_STAT_POINT_DRAGON_CHANGED,               // 드라곤 변신 상태
    };
#endif

#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM

    // 파티 모집 결과
    enum ePARTY_FINDING_RESULT
    {
        PARTY_FINDING_SUCCESS = 0, // 기본 성공
        PARTY_FINDING_FAIL, // 기본 실패
        
        PARTY_FINDING_FAIL_SAME_KEY_IS_EXIST, // 등록하려는 키와 같은 키가 이미 존재
        PARTY_FINDING_FAIL_CANT_ALLOC_POOL, // 풀에서 메모리를 할당 받지 못해서 객체 생성에 실패
        PARTY_FINDING_FAIL_CANT_FIND_MASTER, // 요청한 파티장 정보를 찾을 수 없음
        PARTY_FINDING_FAIL_MASTER_IS_MEMBER, // 파티 모집 요청자가 이미 다른 파티에 속해 있음
        PARTY_FINDING_FAIL_NOT_VALID_INPUT, // 사용자가 입력한 정보 검증에 실패
        PARTY_FINDING_FAIL_CANT_FIND_PARTY_FINDING, // 요청한 파티 모집 정보를 찾을 수 없음
        PARTY_FINDING_FAIL_CANT_FIND_PARTY, // 요청한 파티 정보를 찾을 수 없음
        PARTY_FINDING_FAIL_IS_NOT_MASTER, // 요청자가 파티장이 아님
        PARTY_FINDING_FAIL_MAX_NUM_OF_MEMBER_IS_TOO_SMALL, // 요청한 최대 인원수가 현재 인원수보다 적음
        PARTY_FINDING_FAIL_CANT_CANCEL, // 파티 모집 취소를 시도했는데 실패
        PARTY_FINDING_FAIL_PARTY_IS_FULL, // 파티 인원이 모두 차서 참가 실패
        PARTY_FINDING_FAIL_CANT_FIND_JOIN_WANTED_USER, // 요청한 참가 신청자 정보를 찾을 수 없음
        PARTY_FINDING_FAIL_CHANNEL_IS_DIFFERENT, // 참가 신청자와 파티장의 채널이 서로 다름
        PARTY_FINDING_FAIL_CANT_FIND_CHANNEL, // 요청자의 채널을 찾을 수 없음
        PARTY_FINDING_FAIL_CANT_JOIN_PARTY, // 파티 참가를 시도했는데 실패
        PARTY_FINDING_FAIL_DENY_JOIN_REQUEST, // 파티장이 파티 참가 신청을 거부
        
        PARTY_FINDING_RESULT_COUNT
    }; // ePARTY_FINDING_RESULT

    // 파티 모집 결과 처리를 돕는 유틸리티
    typedef ResultUtilTemplate<ePARTY_FINDING_RESULT, 
                               PARTY_FINDING_SUCCESS, 
                               PARTY_FINDING_RESULT_COUNT, 
                               PARTY_FINDING_SUCCESS, 
                               PARTY_FINDING_FAIL, 
                               BYTE> PartyFindingResultUtil;

#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM

    // 캐릭터 명 변경 결과
    enum eCHANGE_CHARACTER_NAME_RESULT
    {
        RC_CHANGE_CHARACTER_NAME_SUCCESS = 0, // 기본 성공
        RC_CHANGE_CHARACTER_NAME_ALREADY_USED, // 이미 변경 대기 상태
        RC_CHANGE_CHARACTER_NAME_DELAY_WAIT, // 변경한지 7일이 경과 되지 않음
        RC_CHANGE_CHARACTER_NAME_FAIL, // 기본 실패
    };

    //_NA_000096_20100527_CHANGE_CHARACTER_APPEARANCE_SYSTEM
    // 캐릭터 외모 변경 결과
    enum eCHANGE_CHARACTER_APPEARANCE_RESULT
    {
        RC_CHANGE_CHARACTER_APPEARANCE_SUCCESS = 0, // 기본 성공
        RC_CHANGE_CHARACTER_APPEARANCE_FAIL, // 기본 실패        
        RC_CHANGE_CHARACTER_APPEARANCE_COUNT
    };

    // 캐릭터 외모 변경 결과 처리를 돕는 유틸리티
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
        RC_SHOP_PROCESS_TRANSACTION,            // 이미 진행중인 작업으로 인해 해당 작업을 처리할수 없다. (나중에 다시 시도)
        RC_SHOP_REQUEST_SHOPSERVER_FAIL,        // 샵서버로의 요청 실패.
        RC_SHOP_SERVER_CLOSED,                  // 샵서버가 닫혀있다.
        RC_SHOP_ALREADY_EXSIT_USER,             // 이미 샵에 들어와있는 유저이다.
        RC_SHOP_CANNOT_FIND_USER,               // 샵유저를 찾을 수 없다.
        RC_SHOP_CANNOT_USE_SHOP,                // 샵을 이용할 수 없습니다.
        RC_SHOP_DIFFERENT_SCRIPT_VERSION,       // 스크립트 버젼이 틀림.
        RC_SHOP_INVALID_SCRIPT_VERSION,         // 스크립트 버젼이 무효하다.
        RC_SHOP_CHANGED_SCRIPT_VERSION,         // 스클립트 버젼이 새로 변경됨.
        RC_SHOP_STORAGE_PAGE_SIZE_FAIL,         // 요청한 보관함 페이지 사이즈가 잘못됨.
        RC_SHOP_BUY_ITEM_SIZE_FAIL,             // 구입하는 아이템 리스트 사이즈가 잘못됨.
        RC_SHOP_GIFT_ITEM_SIZE_FAIL,            // 보내는 선물 아이템 리스트 사이즈가 잘못됨.
        RC_SHOP_BUY_ITEM_RELAY_FAIL,            // 구입 아이템 리스트 구성 실패.
        RC_SHOP_GIFT_ITEM_RELAY_FAIL,           // 선물 아이템 리스트 구성 실패.
        RC_SHOP_GIFT_RECEIVER_NOT_FOUND,        // 선물 받는 유저를 찾을 수없다.
        RC_SHOP_INVENTORY_FULL,                 // 인벤토리가 가득차서 보관함 아이템을 가져올수없다.
        RC_SHOP_NOT_EXIST_CHARACTER_NAME,       // 해당 이름의 유저가 존재하지 않습니다.
        RC_SHOP_SHOPSERVER_FAILED,              // 샵 서버에서 실패 값을 넘겨줌.
        RC_SHOP_SHOPSERVER_DB_FAILED,           // Shop Data Base 접근 에러.
        RC_SHOP_NOT_ENOUGH_CASH,                // 캐시 부족
        RC_SHOP_BUY_IMPOSSIBLE,                 // 나의 상품 구매하기 불가능.
        RC_SHOP_GIFT_IMPOSSIBLE,                // 상품 선물 불가능.
        RC_SHOP_ITEM_EMPTY,                     // 잔여 수량 부족.
        RC_SHOP_SELL_DATE_END,                  // 판매 기간 종료
        RC_SHOP_SELL_END,                       // 판매 종료
        RC_SHOP_EVENT_ITEM_BUY_IMPOSSIBLE,      // 이벤트 아이템 구매 불가.
        RC_SHOP_EVENT_ITEM_BUY_COUNT_OVER,      // 이벤트 아이템 구매 가능 횟수 초가.
        RC_SHOP_BUY_COOL_TIME_FAIL,             // 상품 구매 쿨 타임 제한 오류.
        RC_SHOP_CANNOT_GIFT_ONESELF,            // 선물 발신/수신자가 동일.
        RC_SHOP_EVENT_ITEM_GIFT_IMPOSSIBLE,     // 이벤트 아이템 선물 불가.
        RC_SHOP_EVENT_ITEM_GIFT_COUNT_OVER,     // 이벤트 아이템 선물 가능 횟수 초가.
        RC_SHOP_LIMIT_CASH_TO_GIFT,             // 선물하기 한도 초과.(1달에 가능한 선물 금액을 초과)
        RC_SHOP_OUTBOUND_MODULE_ERROR,          // 아웃 바운드 모듈에러.
        RC_SHOP_DIFFERENT_CASH_TYPE,            // 캐시 타입이 맞지않음.
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
        RC_BATTLE_GROUND_RESERVATION_SUCCESS = 0,       // 전장 예약 성공
        RC_BATTLE_GROUND_RESERVATION_FAIL,              // 전장 예약 불가
#ifdef _NA_007135_20140402_MODIFY_BATTLE_GROUND_RESERVATION_AVAILABLE_IN_BATTLE_GROUND_CHANNEL
        RC_BATTLE_GROUND_RESERVATION_NOT_AVAILABLE_CHANNEL,  //전장 예약 불가능 채널
#endif
        RC_BATTLE_GROUND_RESERVATION_ALREADY,           // 이미 예약중
        RC_BATTLE_GROUND_NOT_INCORRECT_MAPCODE,         // 존재하지 않은 맵 코드
        RC_BATTLE_GROUND_IS_NOT_QUALIFIED_LEVEL,        // 전장 레벨 제한
        RC_BATTLE_GROUND_IS_NOT_QUALIFIED_ITEM_SCORE,   // 전장 장비 평점 제한
        RC_BATTLE_GROUND_IS_NOT_USER,                   // 유저가 없음
        RC_BATTLE_GROUND_IS_NOT_EXIST_BATTLE_SERVER,    // 배틀서버가 존재 하지 않아 예약 불가
        RC_BATTLE_GROUND_EXIST_STATE_FUGITIVE_BUF,      // 도망자 버프(패널티)가 존재하여 예약 불가.
        RC_BATTLE_GROUND_PLAYER_IS_CHAO_STATE,          // 플레이어가 카오상태 일때 예약 불가.
        RC_BATTLE_GROUND_ALREADY_EXIST_PARTY,           // 이미 파티가 있어 예약 불가.
        
        RC_BATTLE_GROUND_RESERVATION_CANCEL_SUCCESS,    // 전장 예약 취소 성공
        RC_BATTLE_GROUND_RESERVATION_CANCEL_FAIL,       // 정장 예약 취소 불가(이미 취소됨)
        RC_BATTLE_GROUND_STATE_INCLUDE_MATCHING_GROUPS, // 매칭 그룹에 포함된 상태(예약취소 불가)
        RC_BATTLE_GROUND_REWARD_WINDOW_OPEN,            // 미션 보상창이 열려있는 상태
        RC_BATTLE_GROUND_NONADMISSION_TIME,             // 입장 가능 시간이 아니다.
#ifdef _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
        RC_BATTLE_GROUND_INITIATE_VOTE_SUCCESS,                     // 투표발의 성공
        RC_BATTLE_GROUND_INITIATE_VOTE_FAIL,                        // 투표발의 불가능
        RC_BATTLE_GROUND_INITIATE_VOTE_DO_NOT_SAME_TEAM,            // 전장 내 같은 팀이 아니라서 투표 발의 불가능
        RC_BATTLE_GROUND_INITIATE_VOTE_DO_NOT_SATISFY_VICTORYPOINT, // 전장 승점이 만족하지 않아 투표발의 불가능
        RC_BATTLE_GROUND_INITIATE_VOTE_DO_NOT_SATISFY_MEMBER_COUNT, // 투표 발의 가능한 인원수가 만족 하지 않아 투표 발의 불가능
        RC_BATTLE_GROUND_INITIATE_VOTE_DO_NOT_SATISFY_LIMIT_TIME,   // 투표 재발의 시간이 경과 되지않아 투표 발의 불가능
#endif // _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        RC_GOLDRUSH_RESERVATION_SUCCESS,
        RC_GOLDRUSH_RESERVATION_FAIL,              // 골드러시 입장 예약 불가
        RC_GOLDRUSH_RESERVATION_ALREADY,           // 이미 골드러시 입장 예약중
        RC_GOLDRUSH_IS_NOT_QUALIFIED_LEVEL,        // 골드러시 레벨 제한
        RC_GOLDRUSH_IS_NOT_QUALIFIED_ITEM_SCORE,   // 골드러시 장비 평점 제한
        RC_GOLDRUSH_EXIST_STATE_FUGITIVE_BUF,      // 골드러시 도망자 상태
        RC_GOLDRUSH_PLAYER_IS_CHAO_STATE,          // 골드러시 카오 상태
        RC_GOLDRUSH_ALREADY_EXIST_PARTY,           // 골드러시 이미 파티가 있어 예약 불가.

        RC_GOLDRUSH_RESERVATION_CANCEL_SUCCESS,    // 골드러시 예약 취소 성공
        RC_GOLDRUSH_RESERVATION_CANCEL_FAIL,       // 골드러시 예약 취소 불가(이미 취소됨)
        RC_GOLDRUSH_STATE_INCLUDE_MATCHING_GROUPS, // 골드러시 매칭 그룹에 포함된 상태(예약취소 불가)
        RC_GOLDRUSH_REWARD_WINDOW_OPEN,            // 골드러시 미션 보상창이 열려있는 상태
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    };
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
    enum eFREE_PVP_RESULT
    {
        RC_FREE_PVP_CREATE_SUCCESS = 0, // 방생성 성공
        RC_FREE_PVP_CREATE_FAIL, // 방생성 실패
        RC_FREE_PVP_CREATE_FAIL_MAX_ROOM, // 방 제한에 의한 생성 실패
        RC_FREE_PVP_CREATE_FAIL_ZONE, // 인스턴스 존 생성 실패
        RC_FREE_PVP_CREATE_FAIL_TRANSACTION, // 다른 트랜잭션중
        RC_FREE_PVP_CREATE_FAIL_WAIT_MOVE_ZONE, // 던전 입장 대기중 상태.
        RC_FREE_PVP_CREATE_FAIL_INACTIVE, // 자유전투실 비활성화.
        RC_FREE_PVP_CREATE_FAIL_ALREADY_BATTLE_GROUND_RESERVATION, // 전장 예약 중
        
        RC_FREE_PVP_JOIN_SUCCESS, // 입장 성공.
        RC_FREE_PVP_JOIN_FAIL, // 입장 실패 .
        RC_FREE_PVP_JOIN_FAIL_ISNOT_ROOM, // 방이 존재하지 않는다.
        RC_FREE_PVP_JOIN_FAIL_MAX_USER, // 정원이 다 찼다.
        RC_FREE_PVP_JOIN_FAIL_DIFFER_PASSWORD, // 비밀번호 오류.
        RC_FREE_PVP_JOIN_FAIL_INACTIVE, // 자유전투실 비활성화.
    };
#endif // _NA_006088_20120910_CHAOS_ZONE_FREE_PVP

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    enum eSPA_RESULT
    {
        RC_SPA_USER_ALREADY_DOING_TRANSACTION, //트랜잭션문제
        RC_SPA_INVALID_MAPCODE,                //맵코드가 올바르지 않습니다
        RC_SPA_INVALID_ZONE,                   //Zone문제, 없거나 온천이 아니다
    };
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM

#ifdef _NA_008334_20150608_SONNENSCHEIN
    enum eSONNENSCHEIN_RESULT
    {
        RC_SONNENSCHEIN_SUCCESS = 0,                    //sonnenschein 생성성공
        RC_SONNENSCHEIN_FAILED,                         // sonnenschein 생성실패
        RC_SONNENSCHEIN_FAIL_TRANSACTION,               // 트랜잭션실패, 다른행동중
        RC_SONNENSCHEIN_BATTLEGROUND_RESERVATION_STATE, // 전장 예약상태입니다
        RC_SONNENSCHEIN_ALEADY_CREATED,                 // sonnenschein이 이미 생성되었습니다.
        RC_SONNENSCHEIN_ROOM_CREATE_FAILED,              // instance_dungeon생성실패
        RC_SONNENSCHEIN_ROOM_NO_EXIST,                  // 방이 존재하지 않는다
        RC_SONNENSCHEIN_ALREADY_ENTERED,                // 이미 입장한 상태이다
        RC_SONNENSCHEIN_STATE_NOT_READY,                // 아직 방을 생성할 수 없는 상태이다
        RC_SONNENSCHEIN_STATE_END,                      // 해당 룸은 이미 종료되었다 
        RC_SONNENSCHEIN_NOTEXIST_ENTERANCE_ITEM,        // 입장아이템이 없다
        RC_SONNENSCHEIN_CHECK_ENTRANCE_ITEM_FAILED,     // 입장아이템 체크 실패
        RC_SONNENSCHEIN_NOTEXIST_MISSIONINFO,           // 길드미션정보가 없다
        RC_SONNENSCHEIN_NOT_DOMINATION_CHANNEL          // 소넨샤인은 점령전 채널에서만 가능하다
    };
#endif //_NA_008334_20150608_SONNENSCHEIN

#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
	enum eONEDAYSGUILDMISSION_RESULT
	{
		RC_ONEDAYSGUILDMISSION_SUCCESS = 0,                    // 생성성공
		RC_ONEDAYSGUILDMISSION_FAILED,                         // 생성실패
		RC_ONEDAYSGUILDMISSION_FAIL_TRANSACTION,               // 트랜잭션실패, 다른행동중
		RC_ONEDAYSGUILDMISSION_BATTLEGROUND_RESERVATION_STATE, // 전장 예약상태입니다
		RC_ONEDAYSGUILDMISSION_ALEADY_CREATED,                 // 이미 생성되었습니다.
		RC_ONEDAYSGUILDMISSION_ROOM_CREATE_FAILED,             // instance_dungeon생성실패
		RC_ONEDAYSGUILDMISSION_ROOM_NOT_EXIST,                 // 방이 존재하지 않는다
		RC_ONEDAYSGUILDMISSION_ALREADY_ENTERED,                // 이미 입장한 상태이다
		RC_ONEDAYSGUILDMISSION_STATE_END,                      // 해당 룸은 이미 종료되었다 
		RC_ONEDAYSGUILDMISSION_NOTEXIST_ENTERANCE_ITEM,        // 입장아이템이 없다
		RC_ONEDAYSGUILDMISSION_CHECK_ENTRANCE_ITEM_FAILED,     // 입장아이템 체크 실패
		RC_ONEDAYSGUILDMISSION_NOTEXIST_MISSIONINFO,           // 길드미션정보가 없다
		RC_ONEDAYSGUILDMISSION_NOT_DOMINATION_CHANNEL,         // 일일길드미션은 점령전 채널에서만 가능하다
		RC_ONEDAYSGUILDMISSION_NOT_GUILD_USER,				   // 길드유저가 아니다
		RC_ONEDAYSGUILDMISSION_OVER_MAX_LIMIT_ENTERED_CNT	   // 허용인원을 넘어 입장 불가
	};
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION

#ifdef _NA_008404_20150712_MINIGAME_SYSTEM
    enum eMINIGAME_RESULT
    {
        RC_MINIGAME_ACTION_SUCCESS,                     //
        RC_MINIGAME_REWARD_SUCCESS,                     //
        //
        RC_MINIGAME_INVAILD_TYPE,                       // 알수없는 미니게임 타입입니다.
        RC_MINIGAME_NOT_OPEN_TIME,                      // 미니게임 오픈 시간이 아닙니다.
        //
        RC_MINIGAME_BINGO_INVALID_ITEM_INFO,            // 빙고 아이템 정보가 잘못되었습니다.
        RC_MINIGAME_BINGO_ALREADY_FILLED_NUMBER,        // 이미 등록된 숫자 입니다.
        RC_MINIGAME_BINGO_NOT_EXIST_NUMBER,             // 보유하신 빙고판에 없는 숫자입니다.
        RC_MINIGAME_BINGO_REWARD_FAILED,                // 빙고 보상받기에 실패하였습니다.
        //
    };
#endif // _NA_008404_20150712_MINIGAME_SYSTEM

}; // namespace RC

#endif // __RESULT_CODE_H__
