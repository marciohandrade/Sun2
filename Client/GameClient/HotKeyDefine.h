#pragma once

struct HOT_KEY_CODE
{
    union
    {
        struct
        {
            WORD key_code_  : 13;
            WORD shift_key_ : 1;
            WORD ctrl_key_  : 1;
            WORD alt_key_   : 1;
        };
        WORD hot_key_code_;
    };

    HOT_KEY_CODE()
        :hot_key_code_(0)
    {
    }

    HOT_KEY_CODE(WORD key_code, BYTE shift_key, BYTE ctrl_key, BYTE alt_key)
        :hot_key_code_(0)
    {
        key_code_ = key_code;
        shift_key_ = shift_key;
        ctrl_key_ = ctrl_key;
        alt_key_ = alt_key;
    }

    HOT_KEY_CODE(WORD key_code)
        :hot_key_code_(key_code)
    {
    }
};

struct KEY_BIND_PAIR
{
    enum KeyPairIndex
    {
        kMainHotKey = 0,
        kSubHotKey,

        kMaxHotKey,
    };

    HOT_KEY_CODE hot_key_code[kMaxHotKey];

    KEY_BIND_PAIR()
    {
    }

    KEY_BIND_PAIR(HOT_KEY_CODE main_hot_key, HOT_KEY_CODE sub_hot_key)
    {
        hot_key_code[kMainHotKey].hot_key_code_ = main_hot_key.hot_key_code_;
        hot_key_code[kSubHotKey].hot_key_code_ = sub_hot_key.hot_key_code_;
    }

    void SetHotKeyCode(KeyPairIndex pair_index, HOT_KEY_CODE key_code)
    {
        if (pair_index != kMaxHotKey)
        {
            hot_key_code[pair_index] = key_code;
        }
    }
    void SetHotKeyCode(KeyPairIndex pair_index, WORD key_code)
    {
        if (pair_index != kMaxHotKey)
        {
            hot_key_code[pair_index].hot_key_code_ = key_code;
        }
    }
    
    WORD GetHotKeyCode(KeyPairIndex pair_index)
    {
        if (pair_index != kMaxHotKey)
        {
            return hot_key_code[pair_index].hot_key_code_;
        }

        return 0;
    }
};

enum HOT_KEY_FUNCTION_INDEX
{
    kNone_Function_Index = 0,
    // 캐릭터 이동/조종 관련
    kMove_Forward = 1,
    kMove_Backward,
    kMove_LeftSide,
    kMove_RightSide,
    kTurn_Left,
    kTurn_Right,
    kJump,
    kAutoRun,
    kSitStand,
    kPickUp_Item,
    kWingSpread,

    // 시스템 단축키
    kChat_OpenNSend = 101,
    kScreenShot,
    kScreenShotMode,
    kHide_InterfaceAll,
    
    kReply_Whisper, // 삭제?
    kShow_DropItemName, // 삭제?
    
    // 대상선택 및 상호작용
    kSelect_TargetEnemy = 201,
    kNext_TargetEnemy,
    kPrev_TargetEnemy,
    kSelect_Hero,
    kSelect_PartyMember1,
    kSelect_PartyMember2,
    kSelect_PartyMember3,
    kSelect_PartyMember4,
    kSelect_PartyMember5,
    kSelect_PartyMember6,
    kSelect_PartyMember7,
    kSelect_PartyMember8,
    kSelect_PartyMember9,
    kAttack_TargetEnemy,
    kForceAttack_Target, // 삭제?

    // 퀵슬롯 관련
    kExpand_QuickSlotBar = 301,
    kExchange_QuickSlotBar,
    kUse_QuickSlot1,
    kUse_QuickSlot2,
    kUse_QuickSlot3,
    kUse_QuickSlot4,
    kUse_QuickSlot5,
    kUse_QuickSlot6,
    kUse_QuickSlot7,
    kUse_QuickSlot8,
    kUse_QuickSlot9,
    kUse_QuickSlot10,
    kUse_QuickSlot11,
    kUse_QuickSlot12,
    kUse_QuickSlot13,
    kUse_QuickSlot14,
    kUse_QuickSlot15,
    kUse_QuickSlot16,
    kUse_QuickSlot17,
    kUse_QuickSlot18,
    kUse_QuickSlot19,
    kUse_QuickSlot20,
    // 추가적으로 지원될 퀵슬롯 세팅
    kUse_QuickSlot21,
    kUse_QuickSlot22,
    kUse_QuickSlot23,
    kUse_QuickSlot24,
    kUse_QuickSlot25,
    kUse_QuickSlot26,
    kUse_QuickSlot27,
    kUse_QuickSlot28,
    kUse_QuickSlot29,
    kUse_QuickSlot30,
    kUse_QuickSlot31,
    kUse_QuickSlot32,
    kUse_QuickSlot33,
    kUse_QuickSlot34,
    kUse_QuickSlot35,
    kUse_QuickSlot36,
    kUse_QuickSlot37,
    kUse_QuickSlot38,
    kUse_QuickSlot39,
    kUse_QuickSlot40,

    // 소환수 관련
    kSelect_MySummonMonster = 401,
    kUse_SummonSkill1,
    kUse_SummonSkill2,
    kUse_SummonSkill3,
    kUse_SummonSkill4,
    kUse_SummonSkill5,
    kUse_SummonSkill6,
    kUse_SummonSkill7,
    kOrderAttack_SummonMonster,
    kOrderFollow_SummonMonster,
    kAttackMode_SummonMonster,
    kDefenceMode_SummonMonster,
    kRelease_SummonMonster,
    kOrderNonPKAttack_SummonMonster,

    // 인터페이스 관련
    kOpen_CharacterStatus_UI = 501,
    kOpen_Inventory_UI,
    kModeChange_ExtendInventory_UI,
    kOpen_Skill_UI,
    kOpen_Quest_UI,
    kOpen_Guild_UI,
    kOpen_Community_UI,
    kOpen_AreaConquest_UI,
    kOpen_PetStatus_UI,
    kOpen_SystemMenu_N_Cancel,
    kOpen_SystemMenu_UI,
    kOpen_ContinentMap_UI,
    kOpen_BattleScore_UI,
    kModeChange_Radar_UI,
    kModeChange_FullRadar_UI,
    kOpen_CashShop_UI,
    kOpen_GuideInfomation_UI,
    kOpen_MissionObject_UI,
    kOpen_BattleZone_UI,
    kOpen_ChaosZone_UI,
    kOpen_personal_Warp_UI,
    kOpen_MonsterBook_UI,
    kOpen_Attendance_UI,
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM
	kOpen_SUN_Ranking_UI,
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM
#ifdef _NA_008405_20150716_MINIGAME_BINGO
	kOpen_MINI_Game_UI, 
#endif // _NA_008405_20150716_MINIGAME_BINGO

    // 시점 관련
    kZoom_In = 601,
    kZoom_Out,
    kRotate_Left_Camera,
    kRotate_Right_Camera,

    // 할당된 번호는 변경되면 안됨. [9/7/2011 iforall]
    // 해당 기능 번호는 2byte 공간에 저장됨(~65535까지)
    kKeyFunctionMax = 0xFFFF, // 여기까지 서버에 저장
    
    // 아래는 개발/디버그 관련
    //...

};

typedef STLX_MAP<HOT_KEY_FUNCTION_INDEX, KEY_BIND_PAIR> KEY_BIND_PAIR_MAP;
typedef STLX_MAP<WORD, HOT_KEY_FUNCTION_INDEX> HOT_KEY_CODE_ASSOCIATION_MAP;

struct KEY_BIND_INFO
{
    KEY_BIND_PAIR_MAP key_bind_pair_map_;
    HOT_KEY_CODE_ASSOCIATION_MAP hot_key_code_association_map_;

    KEY_BIND_INFO()
    {
        key_bind_pair_map_.clear();
        hot_key_code_association_map_.clear();
    }

    void SetKeyBindInfo(HOT_KEY_FUNCTION_INDEX key_function_index, KEY_BIND_PAIR& key_bind_pair);

    KEY_BIND_PAIR_MAP& key_bind_pair_map() { return key_bind_pair_map_; }
    HOT_KEY_CODE_ASSOCIATION_MAP& hot_key_code_association_map() { return hot_key_code_association_map_; }
};

inline void KEY_BIND_INFO::SetKeyBindInfo(HOT_KEY_FUNCTION_INDEX key_function_index, 
                                          KEY_BIND_PAIR& key_bind_pair)
{
    WORD hot_key_code = 0;
    KEY_BIND_PAIR_MAP::iterator found_itr = key_bind_pair_map().find(key_function_index);
    if (found_itr != key_bind_pair_map().end())
    {
        // 기존에 있는 경우
        KEY_BIND_PAIR temp_key_bind_pair = found_itr->second;

        hot_key_code = temp_key_bind_pair.GetHotKeyCode(KEY_BIND_PAIR::kMainHotKey);
        if (hot_key_code != 0)
        {
            hot_key_code_association_map().erase(hot_key_code);
        }
        hot_key_code = temp_key_bind_pair.GetHotKeyCode(KEY_BIND_PAIR::kSubHotKey);
        if (hot_key_code != 0)
        {
            hot_key_code_association_map().erase(hot_key_code);
        }

        found_itr->second = key_bind_pair;
    }
    else
    {
        // 기존에 없는 경우
        key_bind_pair_map().insert(
            KEY_BIND_PAIR_MAP::value_type(key_function_index, key_bind_pair));
    }

    // 유효성 체크
    hot_key_code = key_bind_pair.GetHotKeyCode(KEY_BIND_PAIR::kMainHotKey);
    if (hot_key_code != 0)
    {
        hot_key_code_association_map().insert(
            HOT_KEY_CODE_ASSOCIATION_MAP::value_type(hot_key_code, key_function_index));
    }

    hot_key_code = key_bind_pair.GetHotKeyCode(KEY_BIND_PAIR::kSubHotKey);
    if (hot_key_code != 0)
    {
        hot_key_code_association_map().insert(
            HOT_KEY_CODE_ASSOCIATION_MAP::value_type(hot_key_code, key_function_index));
    }
}
