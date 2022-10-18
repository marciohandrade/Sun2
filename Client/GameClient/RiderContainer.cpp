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
/** ��Ʈ���� ���ؼ� ������ �籸���Ѵ�
    
    ���̴�������(SCItemSlot)���ϳ��� �������Ƿ� 3���� ���̴������� ���ԵǾ��ִ�.
    ��������� �־ ���������� ������ ���ߵȴ�.
*/
DWORD RiderContainer::CopyItem(SCItemSlot& RiderSlot)
{
    //��������� ���� �����̳ʸ� ���� ������.
    Clear();

#ifdef _NA_007732_20141024_RIDER_MOVESPEED_SYNC
    // ����� �̵�
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

        //eRIDER_PARTS_POS::StartIndex(91)���� ���̴�Pos�̴�
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
                //assert(!"���������ۻ�������");
                _asm nop;
            }
        }
    }
    else
    {
        assert(!"���̴� �������� �ƴմϴ�.");
    }
    
    return dwRiderCode;
   
}
//-------------------------------------------------------------------------------------------
/** ����������Ʈ�����ش�.
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

           //���Կ� �հ��� ����ֳ�
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
/** ���Ի���
*/
RC::eSLOT_INSERT_RESULT 
RiderContainer::InsertSlot(POSTYPE AtPos, SCSlot& rSlot)
{
    //slot_safe_code:������ ����üũ ����(����)
    if (ValidPos(AtPos) == false)
    {
        return RC::RC_INSERT_SLOT_FAILED;
    }

    // ���Կ� ����
    RC::eSLOT_INSERT_RESULT eInsertSlotResult = SCSlotContainer::InsertSlot( AtPos, rSlot );
    SCSlot &refSlot  = GetSlot(AtPos);
    refSlot.SetSerial(g_SlotKeyGenerator.GetKey());
    return eInsertSlotResult;
}
//----------------------------------------------------------------------------
/** ��������
*/
BOOL 
RiderContainer::DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut )
{
    //slot_safe_code:������ ����üũ ����(����)
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
void RiderContainer::RefreshRiderSpeed() // Ż�� �̵��ӵ� ����
{
    rider_speed_ = 100;  // ���� ���
    int base_speed = 0;  // �⺻ �̼�
    int parts_speed = 0; // ���� �̼�
    int setitem_increase = 0; // Ǯ��Ʈ�ɼ� ������

    // �⺻ �̼�
    const sRiderInfo* rider_info = RidingInfoParser::Instance()->GetInfo(dwRiderCode);
    if (rider_info)
    {
        base_speed = rider_info->set_speed_;
    }

    // ���� �̼�
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

        //���Կ� �հ��� ����ֳ�
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
                    if (option_types[i] != 1) // ������ġ���� �ƴϸ� �ϴ� �н�
                    {
                        continue;
                    }

                    parts_speed += option_values[i];
                }
            }

            //��Ʈ ������üũ (MAX_RIDER_SLOT�� 3�� ��쿡�� ������ ����)
            SUN_ASSERT(MAX_RIDER_SLOT==3);
            for (int j = i+1; j < MAX_RIDER_SLOT; ++j)
            {
                SCItemSlot& item_slot2 = (SCItemSlot&)GetSlot(j);

                const BASE_ITEMINFO* item_info2 = item_slot2.GetItemInfo();

                //���Կ� �հ��� ����ֳ�
                if (item_info2 && item_info2->IsRiderPartsItem())
                {
                    if ((item_info->set_group_code == item_info2->set_group_code) &&
                        (1 == item_info->m_bySetOptionType) &&
                        (1 == item_info2->m_bySetOptionType))
                    {
                        // ���� ��Ʈ�������̸�
                        ++setitem_count;
                        setitem_group = item_info->set_group_code;
                    }
                }
            }
        }//if (item_info && item_info->IsRiderPartsItem())


        
    }

    //��Ʈ�ɼ� ���� (���̴� ��Ʈ�ɼ��� Ǯ��Ʈ �ɼǸ� ����. �׷������׿�?)
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
                if (option_info->m_FullOption[i].m_byUseType != 1) // ������ġ���� �ƴϸ� �ϴ� �н�
                {
                    continue;
                }

                setitem_increase += option_info->m_FullOption[i].m_nOption;
            }
        }
    }

    // ���� ���
    rider_speed_ = base_speed + parts_speed + setitem_increase;
}
#endif //_NA_007732_20141024_RIDER_MOVESPEED_SYNC
