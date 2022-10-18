#include "SunClientPrecompiledHeader.h"

#include "RiderContainer.h"	
#include "Character.h"
#include "ObjectManager.h"
#include "SlotKeyGenerator.h"
#include "Rider.h"
#include "InterfaceManager.h"
#include "GlobalFunc.h"
#include "SCItemSlot.h"
#include "Hero.h"
#ifdef _NA_007732_20141024_RIDER_MOVESPEED_SYNC
  #include "RidingInfoParser.h"
  #include "SetItemOptionInfoParser.h"
#endif //_NA_007732_20141024_RIDER_MOVESPEED_SYNC
//----------------------------------------------------------------------------
/** 
*/
WzID RiderContainer::m_wzId[RIDER_MAX] = 
{ 
    StrToWzID("B001"),//RIDER_SLOT_01(BODY)
    StrToWzID("B002"),//RIDER_SLOT_02(BOOTS)
    StrToWzID("B003"),//RIDER_SLOT_03(HELM)
    StrToWzID("BT00"),//RIDER_BTN_LEFT 
    StrToWzID("BT01"),//RIDER_BTN_RIGHT
    StrToWzID("B999"),//RIDER_BTN_EXIT
    StrToWzID("T999"),//RIDER_BTN_TEXT
    StrToWzID("PI00"),//RIDER_SLOT_VIEW
};					


RiderContainer::RiderContainer()
{
	m_dwOwnerKey = 0;
    Init(MAX_RIDER_SLOT, SI_RIDER);

#ifdef _NA_007732_20141024_RIDER_MOVESPEED_SYNC
    rider_speed_ = 0;
    dwRiderCode = 0;
#endif //_NA_007732_20141024_RIDER_MOVESPEED_SYNC
}

RiderContainer::~RiderContainer()
{
	Release();
}
//-------------------------------------------------------------------------------------------
/** 스트림을 통해서 슬롯을 재구축한다
    
    라이더아이템(SCItemSlot)요하나에 변수식의로 3개의 라이더파츠가 포함되어있다.
    요아이템을 넣어서 파츠정보를 끄집어 내야된다.
*/
DWORD RiderContainer::CopyItem(SCItemSlot& RiderSlot)
{
    //파츠변경시 기존 컨테이너를 전부 날린다.
    Clear();

#ifdef _NA_007732_20141024_RIDER_MOVESPEED_SYNC
    // 헤더로 이동
#else
    DWORD dwRiderCode = 0;
#endif //_NA_007732_20141024_RIDER_MOVESPEED_SYNC

    Player *pPlayer = (Player *)g_ObjectManager.GetObject(m_dwOwnerKey);

    if(!pPlayer)
    {
        return 0;
    }

    if(RiderSlot.IsRiderItem())
    {
        dwRiderCode = RiderSlot.GetItemInfo()->m_wSkillCode;

        //eRIDER_PARTS_POS::StartIndex(91)부터 라이더Pos이다
        for(int i=0;i<MAX_RIDER_SLOT;++i)
        {
            SCRiderPartsData parts_data;
            memset(&parts_data,0,sizeof(parts_data));

            eRIDER_PARTS_POS Position; 
            Position.value = Position.StartIndex+i;

            SCItemSlot ItemSlot;
            
            if(RiderSlot.GetRiderPartsItem(Position,&ItemSlot))
            {
                if(ItemSlot.IsContainValidItem())
                {
                    this->InsertSlot( i, ItemSlot);
                }
                
            }
            else
            {
                //assert(!"파츠아이템생성실패");
                _asm nop;
            }
        }
    }
    else
    {
        assert(!"라이더 아이템이 아닙니다.");
    }
    
    return dwRiderCode;
   
}
//-------------------------------------------------------------------------------------------
/** 외형업데이트를해준다.
*/
void RiderContainer::RefreshApperance()
{
    Player *pPlayer =(Player *)g_ObjectManager.GetObject(m_dwOwnerKey);

    if(pPlayer)
    {
        for(int i=0;i<MAX_RIDER_SLOT;++i)
        {
            SCItemSlot &ItemSlot =(SCItemSlot &)GetSlot(i);

            const BASE_ITEMINFO *pItemInfo = ItemSlot.GetItemInfo();

           //슬롯에 먼가가 들어있네
           if(pItemInfo && pItemInfo->IsRiderPartsItem())
           {
               pPlayer->AttachRiderPart(i,ItemSlot.GetItemInfo()->m_Code);
               pPlayer->RiderSubtype(i,ItemSlot.GetEnchant());
           }
           else
           {
              pPlayer->DetachRiderPart(i);
           }
        }
    }
}
//-------------------------------------------------------------------------------------------
/**
*/
void RiderContainer::Clear()
{
    for ( POSTYPE pos = 0; pos < MAX_RIDER_SLOT; ++pos )
    {
        if ( !IsEmpty(pos) )
        {
            DeleteSlot(pos, NULL);
        }
    }
}
//-------------------------------------------------------------------------------------------
/**
*/
void
RiderContainer::Release()
{
	Clear();
}
//----------------------------------------------------------------------------
/** 슬롯삽입
*/
RC::eSLOT_INSERT_RESULT 
RiderContainer::InsertSlot(POSTYPE AtPos, SCSlot& rSlot)
{
    //slot_safe_code:안전한 슬롯체크 구문(삽입)
    if (ValidPos(AtPos) == false)
    {
        return RC::RC_INSERT_SLOT_FAILED;
    }

    // 슬롯에 넣음
    RC::eSLOT_INSERT_RESULT eInsertSlotResult = SCSlotContainer::InsertSlot( AtPos, rSlot );
    SCSlot &refSlot  = GetSlot(AtPos);
    refSlot.SetSerial(g_SlotKeyGenerator.GetKey());
    return eInsertSlotResult;
}
//----------------------------------------------------------------------------
/** 슬롯제거
*/
BOOL 
RiderContainer::DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut )
{
    //slot_safe_code:안전한 슬롯체크 구문(삭제)
    if ((ValidPos(AtPos) == false) ||
        IsEmpty(AtPos))
    {
        return FALSE;
    }

    SCItemSlot & rItemSlot = (SCItemSlot & )GetSlot(AtPos);
    g_SlotKeyGenerator.Restore((DWORD)rItemSlot.GetSerial());
    SCSlotContainer::DeleteSlot(AtPos, pSlotOut );
    return TRUE;

}

#ifdef _NA_007732_20141024_RIDER_MOVESPEED_SYNC
void RiderContainer::RefreshRiderSpeed() // 탈것 이동속도 갱신
{
    rider_speed_ = 100;  // 최종 결과
    int base_speed = 0;  // 기본 이속
    int parts_speed = 0; // 파츠 이속
    int setitem_increase = 0; // 풀세트옵션 증가량

    // 기본 이속
    const sRiderInfo* rider_info = RidingInfoParser::Instance()->GetInfo(dwRiderCode);
    if (rider_info)
    {
        base_speed = rider_info->set_speed_;
    }

    // 파츠 이속
    Player *player = static_cast<Player*>(g_ObjectManager.GetObject(m_dwOwnerKey));
    if (player == NULL)
    {
        return;
    }
    int is_pcroom_player = !!(player->GetPCBangMode());

    WORD setitem_group = 0;
    BYTE setitem_count = 1;
    for (int i = 0; i < MAX_RIDER_SLOT; ++i)
    {
        SCItemSlot& item_slot = (SCItemSlot&)GetSlot(i);
        const BASE_ITEMINFO* item_info = item_slot.GetItemInfo();

        //슬롯에 먼가가 들어있네
        if (item_info && item_info->IsRiderPartsItem())
        {
            int loop_count = is_pcroom_player ? 2 : 1;
            for (int loop = 0; loop < loop_count; ++loop)
            {
                const BASE_ITEMINFO::OptionIndexes* _option_indexs = \
                    (loop == 0) ? &item_info->m_wOptionIndex : &item_info->m_wPCOptionIndex;
                const BASE_ITEMINFO::OptionTypes* _option_types = \
                    (loop == 0) ? &item_info->m_byOptionType : &item_info->m_byPCOptionType;
                const BASE_ITEMINFO::OptionValues* _option_values = \
                    (loop == 0) ? &item_info->m_iOptionValue : &item_info->m_iPCOptionValue;

                const BASE_ITEMINFO::OptionIndexes& option_indexs = *_option_indexs;
                const BASE_ITEMINFO::OptionTypes& option_types = *_option_types;
                const BASE_ITEMINFO::OptionValues& option_values  = *_option_values;

                for (int i = 0; i < BASE_ITEMINFO::_MAX_OPTION_NUM; ++i)
                {
                    if ((option_indexs[i] == 0) ||
                        (option_values[i] == 0) ||
                        (option_indexs[i] >= _countof(g_ITEM_OPTION)))
                    {
                        continue;
                    }

                    const eATTR_TYPE attr_type = g_ITEM_OPTION[option_indexs[i]];
                    if (attr_type != eATTR_MOVE_SPEED)
                    {
                        continue;
                    }
                    if (option_types[i] != 1) // 고정수치값이 아니면 일단 패스
                    {
                        continue;
                    }

                    parts_speed += option_values[i];
                }
            }

            //세트 아이템체크 (MAX_RIDER_SLOT가 3일 경우에만 로직이 맞음)
            SUN_ASSERT(MAX_RIDER_SLOT==3);
            for (int j = i+1; j < MAX_RIDER_SLOT; ++j)
            {
                SCItemSlot& item_slot2 = (SCItemSlot&)GetSlot(j);

                const BASE_ITEMINFO* item_info2 = item_slot2.GetItemInfo();

                //슬롯에 먼가가 들어있네
                if (item_info2 && item_info2->IsRiderPartsItem())
                {
                    if ((item_info->set_group_code == item_info2->set_group_code) &&
                        (1 == item_info->m_bySetOptionType) &&
                        (1 == item_info2->m_bySetOptionType))
                    {
                        // 같은 세트아이템이면
                        ++setitem_count;
                        setitem_group = item_info->set_group_code;
                    }
                }
            }
        }//if (item_info && item_info->IsRiderPartsItem())


        
    }

    //세트옵션 적용 (라이더 세트옵션은 풀세트 옵션만 적용. 그런가보네요?)
    if (setitem_group != 0)
    {
        SETITEM_OPTION_INFO* option_info = SetItemOptionInfoParser::Instance()->Find(setitem_group);
        if (option_info && (option_info->m_byActiveFullOptionNum <= setitem_count))
        {
            for (int i = 0; i < MAX_SETITEM_FULL_OPTION; ++i)
            {
                if ((option_info->m_FullOption[i].m_byOptionKind == 0) ||
                    (option_info->m_FullOption[i].m_nOption == 0) ||
                    (option_info->m_FullOption[i].m_byOptionKind >= _countof(g_ITEM_OPTION)))
                {
                    continue;
                }

                const eATTR_TYPE attr_type = g_ITEM_OPTION[option_info->m_FullOption[i].m_byOptionKind];
                if (attr_type != eATTR_MOVE_SPEED)
                {
                    continue;
                }
                if (option_info->m_FullOption[i].m_byUseType != 1) // 고정수치값이 아니면 일단 패스
                {
                    continue;
                }

                setitem_increase += option_info->m_FullOption[i].m_nOption;
            }
        }
    }

    // 최종 결과
    rider_speed_ = base_speed + parts_speed + setitem_increase;
}
#endif //_NA_007732_20141024_RIDER_MOVESPEED_SYNC
