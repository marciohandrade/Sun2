#ifndef _SOLAR_SLOT_SCITEMSLOT_H
#define _SOLAR_SLOT_SCITEMSLOT_H
#pragma once

//==================================================================================================
/// ������ ���� Ŭ����
/**
    @author Kim Min Wook < taiyo@webzen.co.kr >
    @since  2004. 12. 29
    @remark
    - �ϳ��� �������� ��Ÿ���� Ŭ����
    @note
    - Dura�� ���� WithDura�϶� ������, �ƴҶ��� �����̴�. �׷��Ƿ� 1���̻�
    - ITEMSTREAM�� byDura�ʵ�� ������� �ʴ´�.
    @history 
    - 2006/1/9 : SCRankInfo, SCSocketInfo�ɹ��� ���� �ɼ� ���� �и�
    - 2006/1/11 : ��ũ,��þƮ �����Լ��� ȣ���Ҷ� ó���ϴ� ������� ����
    - 2006/1/13 : �������� ���� ���� ó�� + GetMaxNum() �߰�
    - 2006/1/17 : ExistEmptySocket()�Լ� �߰�
    - 2006/2/8 : 
    1. IsBlocked()�Լ� �߰� : ������ ���Կ� �̿����� ���� üũ (m_Set == 15�̸�, �̿���������)
    2. void CopyOut(RENDER_ITEMSTREAMEX& OUT ItemStream) �Լ� �߰�
    - 2006/2/13 : RENDER_ITEMSTREAM�� EX���� �Լ��� �߰�
    - 2006/3/17 : SCNOption ���� �߰�
    - 2006/4/25 : DropLV => WORD->DWORD�� ����
    - 2006/5/16 : OPT_ITEMSTREAMEX���� ó��
    - 2006/5/17 : ������ ���� ó��
        BOOL DecreasingDura(DWORD param1, DWORD param2);
        param1 = ���� ���� ������ or ����
        param2 = �ǰ��� ���� or ���� ���� ���� ������
        return TRUE = Dura�� �������� ���
        return FALSE = Dura�� �������� �ʾ��� ���
    - 2006/5/23 : IsLocked(), IsBlocked() ����ó��
    - 2007/1/19 : SetFixedDateTime(int d) �߰� : �ð���(d)�� �������� �ʰ� m_DateTime�� �ٷ� ���� ����
    - 09.09.06 | waverix | code rearrangement

    @usage
    - �������� ���������� ���� : IsOverlap()
    - �������� �����ϴ� ���������� ���� : WithDura()
    - OnEquip() �� ��ȯ�� �ǹ� : eCHARGE_SUB_FIXED_AMOUNT_EXŸ���϶�, ó�� ������ ��� TRUE��ȯ, �׿� FALSE��ȯ
    - [����ȭ ���� ������ ����]

    SCItemSlot slot;
    switch(slot.GetItemInfo()->m_byChargeSubType)
    {
    case eCHARGE_SUB_FIXED_AMOUNT:
        {
        // �ð��� ����(�ʴ���)
        // if it will be 30�� ���� ������
        slot.SetDateTime(30*24*60*60);
        // ��¥�� ���
        SYSTEMTIME systime;
        YYYYMMDDHHMMSSToSYSTEMTIME(slot.GetDateTime(), systime);
        //or
        if(!slot.GetDateTimeToSYSTEMTIME(systime))
        {
            // �������� ��� ��¥�� ����(�����)
        }
        // systime�� ������ ���� �ȴ�.
        //......
        }break;
    case eCHARGE_SUB_FIXED_AMOUNT_EX:
        {
            // �ð� ����(�ʴ���)
            // �ٸ� �ʴ����� ���ڴ� ���õ�!(�ð������� ����)
            // if it will be 30�ð� �̿� ������
            slot.SetDateTime(30*60*60);

            // ��¥�� ���
            if(slot.IsProgressTime())
            {
                // 20061107240023
                SYSTEMTIME systime;
                YYYYMMDDHHMMSSToSYSTEMTIME(slot.GetDateTime(), systime);
                //or
                if(!slot.GetDateTimeToSYSTEMTIME(systime))
                {
                // �������� ��� ��¥�� ����(�����)
                }
                // systime�� ������ ���� �ȴ�.
                //.....
            }
            else
            {
                // ����Ҽ��ִ½ð�
                int remain_hours = slot.GetDateTime()/(60*60)
                //....
            }
        }break;
    case eCHARGE_SUB_FIXED_QUANTITY:
        {
            // ��� �ð� �Է�(�ʴ����� �Է�)
            // if it will be 30�ð� �̿� ������
            slot.SetDateTime(30*60*60);

            // ���� ��� �ð�
            int remain_hours = slot.GetDateTime()/(60*60)
            //....

        }break;
    case eCHARGE_SUB_COORDINATE:
        {
            // ��ǥ ����
            slot.SetMapCode(10001);
            slot.SetX(13);
            slot.SetY(145);
            slot.SetZ(21);
            // ��ǥ �̿�
            slot.GetMapCode();
            slot.GetX();
            slot.GetY();
            slot.GetZ();
        }break;
    }

*/
//==================================================================================================

#include "SCSlotHeader.h"
#include "SCSlotStruct.h"
#include "ItemOptionHeader.h"

#include "ISlotUpdater.h"

//--------------------------------------------------------------------------------------------------
//  SubOptions
#include "SCSlot.h"
#include "SCItemFuncBranch.h"

#include "SCRankInfo.h"

#ifdef _NA_0_20100727_SOCKET_SYSTEM_CONVERT
#include "SCSocketOptionInfo.h"
#else
#include "SCSocketInfo.h"
#endif // _NA_0_20100727_SOCKET_SYSTEM_CONVERT

#include "SCNOption.h"
#include "SCLimitedInfo.h"
#include "SCFateInfo.h"
#include "SCPetInfo.h"
#include "SCLottoInfo.h"
#include "SCLaxLock.hpp"
#include "SCWayPointInfo.h"
#include "SCRiderInfo.h"
#ifdef _NA_003740_20111122_RANDOM_ITEM
#include "SCRandomInfo.h"
#endif // _NA_003740_20111122_RANDOM_ITEM

//==================================================================================================
//
//

class CSunOnlineItem;
//==================================================================================================
//
class SCItemSlot : public SCSlot
{
    enum { FIXED_AMOUNT_EXPIRE_WAIT_TIME = 7 * 24 * 60 * 60 }; //< 7�� ����� ����
public:
    class EditModeTransaction;

    SCItemSlot(void);
    virtual ~SCItemSlot(void);

    inline SCItemSlot(const SCItemSlot& r);
    inline SCItemSlot& operator=(const SCItemSlot& r);

    //SCItemSlot(const ITEMSTREAM& item_stream); //< �ɼ��� ������ �������� ����
    SCItemSlot(const ITEMSTREAMEX& item_stream); //< �ɼ��� ������ ������ ����
    //SCItemSlot(const RENDER_ITEMSTREAM& item_stream);
    SCItemSlot(const RENDER_ITEMSTREAMEX& item_stream);
    SCItemSlot(const OPT_ITEMSTREAMEX& item_stream);
    void Copy(const ITEMSTREAMEX& IN item_stream);
    //void Copy(const ITEMSTREAM& IN item_stream);
    //void Copy(const RENDER_ITEMSTREAM& IN item_stream); //< ����������������
    void Copy(const RENDER_ITEMSTREAMEX& IN item_stream); //< ����������������
    void Copy(const OPT_ITEMSTREAMEX& IN item_stream);
    void CopyOut(ITEMSTREAMEX& OUT item_stream) const;
    //void CopyOut(ITEMSTREAM& OUT item_stream) const; //< �ɼ� ����
    void CopyOut(RENDER_ITEMSTREAMEX& OUT item_stream) const; //< �ø��� ����
    //void CopyOut(RENDER_ITEMSTREAM& OUT item_stream) const; //< �ø���+�ɼ� ����
    void CopyOut(OPT_ITEMSTREAMEX& OUT item_stream) const; //< �ø���+�ɼǼ�ġ ����

    virtual void Copy(BitStream& IN stream, eSLOT_SERIALIZE eType);
    virtual void CopyOut(BitStream& OUT stream, eSLOT_SERIALIZE eType);

    virtual void Copy(BitStream& IN stream);
    virtual void CopyOut(BitStream& OUT stream);

    virtual void Copy_Render(BitStream& IN stream);
    virtual void CopyOut_Render(BitStream& OUT stream);

private:
    // ���������� ����� ���� �Լ�
    void CopyPart(const ITEMPART& IN rPart);
    void CopyPart(const RENDER_ITEMPART& IN rPart);
    void CopyPart(const OPT_ITEMPART& IN rPart);
    void CopyOutPart(ITEMPART& OUT rPart) const;
    void CopyOutPart(RENDER_ITEMPART& OUT rPart) const;
    void CopyOutPart(OPT_ITEMPART& OUT rPart) const;
    //
    //void CopyPart(const OPTIONPART_VER5& IN rPart);
    //void CopyOutPart(OPTIONPART_VER5& OUT rPart) const;

    void CopyPart(const RENDER_OPTIONPART& IN rPart);
    void CopyOutPart(RENDER_OPTIONPART& OUT rPart) const;

    void CopyPart(const OPTIONPART& IN rPart); // (090911) (ADD-IN)
    void CopyOutPart(OPTIONPART& OUT rPart) const; // (090911) (ADD-IN)
public:

    virtual void Copy(SCSlot& IN slot);
    virtual void Clear();

    virtual void SetSlotType(eSlotType type);
    virtual eSlotType GetSlotType() const;
    virtual void SetSerial(DBSERIAL serial);
    virtual DBSERIAL GetSerial() const;
    // (WAVERIX) (NOTE) ������ ������ ���ʿ� �����ؾ��ϴ� �ڵ� : �ڵ尡 �������� ���� ��å�� ��������.
    // ����: SCItemSlot instance�� �Ϸ�Ǿ� �־�� �Ѵ�. (��, �ʱ�ȭ �Ǿ� �־�� �Ѵ�.)
    // ����: Copy(from server stream, db stream)�� �����, SetCode�� ���ο��� ȣ��ȴ�.
    virtual void SetCode(SLOTCODE code);
    virtual SLOTCODE GetCode() const;
    SLOTCODE GetItemCode() const; // NOTE: f110422.3L, add default accessability to easy acquisition
    SLOTCODE GetImageCode() const; //_NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_

    BYTE GetType() const;
    void SetType(BYTE type);
    DBWLDSERIAL GetWorldSerial() const;

    bool IsContainValidItem() const; // (WAVERIX) (CHANGES) support for client, see a .inl

    //BASE_ITEMINFO* GetItemInfo();
    const BASE_ITEMINFO* GetItemInfo() const;
    const BASE_ITEMINFO* GetImageItemInfo() const;
    const nsSlot::ItemRuleInfo& GetItemRuleInfo() const;

    void SetUserBlocked(BOOL val);
    void SetBlocked(BOOL val);
    bool IsLocked() const;
    void SetLock(bool val);

    bool IsBlocked() const;
    bool IsUserBlocked() const; // Game issue�� ���� ���� �ɾ�� �ϴ� ���, ex) ��/���̴� ��ȯ ���� + wings spreading
    bool IsPetLocked() const; // UserBlock��� ���̴�. = 'IsUserBlocked' || 'IsRiderSummoned'
    // f100909.1L
    void SetOptionTag(const KindOfItemTag& tag);
    const KindOfItemTag& GetOptionTag() const;
    // ����
    DURATYPE GetMaxNum() const;

    //----------------------------------------------------------------------------------------------
    // ������ ����(for stat change) - serial key not issued, need serial key 
    // (090929) (ADD-IN)
    bool CopyFromSrcForStatChange(SLOTCODE to_change_item_code, const SCItemSlot& src);
    // f100910.1L, (for inheritable tag option) related "f100909.1L' and "(Get/Set)OptionTag()"
    // WARNING: this method is not support a stat change operation.
    // NOTE: usage case = { make a clone item, making item from a lottery item, and that. }
    bool CopyFromInheritableOption(const SCItemSlot& src);
    // CHANGES: f110327.1L, changes charge_sub_type access field
    // to support various making item with timer
    // accetable = { sDefault -> sTime, sEquip -> sEquipTime, sEquipEtc -> sEquipEtcTime }
    bool ChangeItemToUsingTimer();
    //----------------------------------------------------------------------------------------------
    // ������ ����

    // pseudo
    // if �������� ����ϴ°�?
    //   (true)
    // elif ������ ����ϴ°�?
    //   (false)
    bool WithDura() const;

    BYTE GetDura() const;
    void SetDura(DURATYPE dura);
    BYTE GetMaxDura() const;
    BOOL DecreasingDura(eDecDuraType eDecType);
    BOOL IsNeedToRepair();
    BOOL StoreDamageNDecreaseDura(const DWORD Damage);

    MONEY GetPriceForRepair() const;

public: 
    // �������� �������������� üũ
    bool IsOverlap() const;
    static bool IsOverlap(SLOTCODE code);

    // ������ �ʿ��� ITEMINFO �Լ� �߰� ����
    MONEY GetPriceForBuy() const;
    MONEY GetPriceForSell() const;
    MONEY GetPriceForUpGradeFare() const;
private:
    LEVELTYPE GetLimitLevel() const; // (UNUSED) ??
    //DWORD GetPhyDef() const;    // (UNUSED) ??
    DWORD GetDropLevel() const; 
public:
    //----------------------------------------------------------------------------------------------
    // Enchant Option
    void SetEnchant(BYTE enchant);

    //(lst1024)(080918)(2447)��æƮ�� �������� �ٽ� ���� ���ش�.
    void SetEnchantUpdateDura(BYTE enchant);
    BYTE GetEnchant() const;
    eENCHANTLEVEL GetEnchantLV() const;

#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
    //----------------------------------------------------------------------------------------------
    // Awakening Option
    void SetAwakening(BYTE awakening);
    BYTE GetAwakening() const;
    bool EnableAwakening() const;
#endif // _NA_008124_20150313_AWAKENING_SYSTEM

    //----------------------------------------------------------------------------------------------
    // Divine Option (old comment) Set ����
    bool IsDivine() const;

    void SetSet(BYTE set);  // Divine...
    bool GetSet() const;

    //----------------------------------------------------------------------------------------------
    // Rank Option
    // get current item rank information
    eRANK GetRank() const;
    int GetRankAttrIndex(eRANK rank) const;
    eATTR_TYPE GetRankAttr(eRANK rank) const;
    int GetRankAttrValue(eRANK rank) const;
    const sRANK_OPTION* GetRankAttrDesc(eRANK rank) const;
    eRANK_LEVEL GetRankLevel() const;
    //
#if SUN_CODE_BACKUP // an unused sections at this point in this time
    // item rank up based on current rank
    // prev_rank == result_rank -> Fail (highest, etc...)
    // prev_rank != result_rank -> Success & update rank (assert: prev_rank + 1 == result_rank)
    eRANK Rankup(const sRANK_OPTION* rank_option);
    // item rank down based on current rank
    // prev_rank == result_rank -> Fail (lowest, etc...)
    // prev_rank != result_rank -> Success & update rank (assert: prev_rank - 1 == result_rank)
    eRANK Rankdown();
#endif
#if DEPRECATE_CODE_DELETE_RESERVED //<- delete reserved section
    // old version corresponding interfaces
    /*DEPRECATE_RESERVED*/ void SetRank(eRANK rank);
    /*DEPRECATE_RESERVED*/ void SetRankAttr(eRANK rank, const sRANK_OPTION* pRankOption);
    /*DEPRECATE_RESERVED*/ void DelRankAttr(eRANK rank);
#endif //WAVERIX_DELETE_RESERVED<- delete reserved section

    //----------------------------------------------------------------------------------------------
    // ��ǥ
    bool IsCoordinateItem() const;
    WORD GetMapCode() const;
    short GetX() const;
    short GetY() const;
    short GetZ() const;

    void SetMapCode(WORD v);
    void SetX(short v);
    void SetY(short v);
    void SetZ(short v);

    //----------------------------------------------------------------------------------------------
    // ��¥
    bool IsProgressTime() const;
    void SetProgressTime(bool bFlag);

    DWORD64 GetDateTime() const;
    BOOL GetDateTimeToSYSTEMTIME(SYSTEMTIME& systime OUT);
    void SetDateTime(DWORD64 d/*second*/);
    void SetFixedDateTime(DWORD64 d); //< ����! : Ÿ�Կ� ���� �ٸ� ������ ���� �Է��ؾ���
    bool IsExpiredWaitTime() const;
    void SetExpiredWaitTime(bool bFlag);

    bool IsExpiredTime() const;
    void SetExpiredTime(bool bFlag);

    bool StartQuantity();
    void EndQuantity();

    //----------------------------------------------------------------------------------------------
    // Socket Option information
    BYTE GetSocketNum() const;
    void SetSocketNum(BYTE num);

#ifdef _NA_0_20100727_SOCKET_SYSTEM_CONVERT
    eSOCKET_IDENTIFY_TYPE GetSocketIdentifyType() const;
    void SetSocketIdentifyType(const eSOCKET_IDENTIFY_TYPE socket_identify_type);
    void SetSocketOption(eSOCKET sock, SOCKETINDEX index);
    const sSOCKET_OPTION* GetSocketOption(eSOCKET sock) const;
    void CopySocketInfo(const SCSocketOptionInfo& sock_info);
    const SCSocketOptionInfo& GetSocketInfo();
#else
    void SetSocketAttr(eSOCKET sock, eSOCKET_LEVEL level, const sSOCKET_OPTION* socket_option);
    eATTR_TYPE GetSocketAttr(eSOCKET sock) const;
    int GetSocketAttrValue(eSOCKET sock) const;
    eSOCKET_LEVEL GetSocketAttrLevel(eSOCKET sock) const;
    int GetSocketAttrIndex(eSOCKET sock) const;
    const sSOCKET_OPTION* GetSocketAttrDesc(eSOCKET sock) const;
    void CopySocketInfo(const SCSocketInfo& sock_info);
    const SCSocketInfo& GetSocketInfo();
#endif // _NA_0_20100727_SOCKET_SYSTEM_CONVERT

    bool ExistEmptySocket(eSOCKET& OUT EmptySocket) const;    
    // �ش� ����� ��尡 ���� �������� �˻��Ѵ�.
    bool IsEnableFillSocketByGrade(const eSOCKET_LEVEL zard_grade) const;

    //----------------------------------------------------------------------------------------------
    // NOption ����
    //  - (NOTE) ExtraStone Option�̶���...
    eATTR_TYPE GetNOptionAttr() const;
    int GetNOptionAttrValue() const;
    int GetNOptionAttrIndex() const;
    const sEXTRASTONE_OPT_INFO* GetNOptionAttrDesc() const;
    int GetNOptionValueIndex() const;
    void SetNOptionAttr(int iAttrValueIndex);

public:
    //----------------------------------------------------------------------------------------------
    // Limited Item Option
    bool IsLimited() const;

    // (WARNING) �ܺο��� �����ϴ� ������ 2����, { drop, gmcmd } �̰��� ������ ���ķ�
    // �ø��� Ű ������ �Ǹ� ����� �����ϴ�.
    // (NOTE) �߰� 1���� (�츣�޽����� ����) �̰��� �̹� ������ �����ۿ� ���ؼ� ���� �����ϴ� ��.
    // �ٸ� Set�Լ��� ������� �� ��. ��ü �������̽� ����(CopyFromSrcForStatChange)
    void SetLimitedIndex(WORD limited_index);

    WORD GetLimitedIndex();
    int GetDamageRatio() const;
    int GetLimitValueRatio() const;
    DURATYPE GetLimitedDuraMax() const;
    DURATYPE GetLimitedDuraMMax() const;
    BYTE GetLimitedRepairCount() const;

    // called state:
    //  on Recv Serial key, called by 'ItemManager::AddSerialCode'
    void CalLimited();
    void RepairMaxDura();
    BOOL DecreasingMaxDura();
    //-----------------------------------------------------------------------------------
private:
    void SetDamageRatio(int value);         // guide - don't use, delete reserved
    void SetLimitValueRatio(int value);     // guide - don't use, delete reserved
    void SetLimitedDuraMax(WORD value);     // guide - don't use, delete reserved
    void SetLimitedDuraMMax(WORD value);    // guide - don't use, delete reserved
    void SetLimitedRepairCount(BYTE value); // guide - don't use, delete reserved
    bool _CanChangeToLimited() const;
    bool _ChangeToLimitedType(); // must be true, pre-check '_CanChangeToLimited()' + a
                                    // ref: SCItemSlot::CalLimited()
public:
    //----------------------------------------------------------------------------------------------
    // (FateItem)
    bool IsFateItem() const; 
    bool CalFateInfo();
    bool CalFateDateTime(DWORD& OUT date_time);

    WORD GetFateAtkDefRatio() const;
    bool GetFateIdentify() const;
    DURATYPE GetFateDuraMax() const;
    WORD GetFatePeriod() const;

    void SetFateAtkDefRatio(WORD value);
    void SetFateIdentify(BOOL identify);
    void SetFateDuraMax(WORD value);
    void SetFatePeriod(WORD period);

    //----------------------------------------------------------------------------------------------
    // (ISlotUpdater)
    void RegisterISlotUpdater(ISlotUpdater* pISlotUpdater);
    void UnRegisterISlotUpdater();

    bool IsPetItem() const;
    /*const*/ SCPetInfo* GetPetInfo() const;

    bool IsEquipEtherDevice() const;
    void SetEquipEtherDevice(BOOL bValue);
    BYTE GetEtherBulletIndex() const;
    void SetEtherBulletIndex(BYTE byIndex);
    SCLaxLock<BYTE>* GetLaxLock();
    //----------------------------------------------------------------------------------------------
    // Lotto Info
    bool IsLottoItem() const;
    bool IsPandoraOpen() const;
    // (WARNING) Ŭ���̾�Ʈ���� Refresh�ϴ� ��ʰ� �߰ߵǾ� ���ȭ���� ���ߴ�.
    SCLottoInfo* GetLottoInfo() const;

#ifdef _NA_0_20100727_SOCKET_SYSTEM_CONVERT
    WORD GetExtendData() const;
    void SetExtendData(WORD v);

    bool IsZardItem() const; // ��� ���������� �˻��Ѵ�.
    bool IsZardAdditiveItem() const; // ��� �ռ� ÷���� ���������� �˻��Ѵ�.
    // ���� ������ ������ �������� ��� ������ �����Ѵ�.
    bool CreateSocketItem(const BYTE number_of_socket, const bool is_recreate); 
#endif // _NA_0_20100727_SOCKET_SYSTEM_CONVERT
#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
    BYTE GetDiceCastType();
    void SetDiceCastType(BYTE _type);
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING

    // ����ȭ ������ ���������� �˻��Ѵ�.
    bool EnableRandomize() const;
#ifdef _NA_003740_20111122_RANDOM_ITEM
    // ����ȭ ������ �������� ��� �������� ������ �ɷ�ġ�� �ٽ� �����Ѵ�.
    bool Randomize(const RandomValueTypeGrade::Value value_type_grade);

    // ����ȭ�� �ɼ��� ����� ��´�.
    RandomValueGrade::Value GetRandomOptionGrade(const uint8_t index) const;

    // ����ȭ�� �ɼ��� ���ڿ� ǥ���� ��´�.
    template < size_t kConvStrBufSize >
    void RandomOptionToString(
        const RandomValueTypeGrade::Value value_type_grade, 
        TCHAR OUT conv_str[]
    ) const {
        random_info_.OptionToString<kConvStrBufSize>(value_type_grade, conv_str);
    }

#endif // _NA_003740_20111122_RANDOM_ITEM
    
    //----------------------------------------------------------------------------------------------
    //  WayPoint Info
    bool FireUp_DateTime();  // frame������ ������ �ʿ䰡 �ִ�.
    void ClearWayPointItem();   // for GM command
    bool IsWayPointItem() const;
    //bool IsProgressTime() const;
    bool CanWayPointDelete() const;
    void ResetWayPointDelete();

    //----------------------------------------------------------------------------------------------
    //  Possession Info
    void SetIdentifyPossession(BOOLEAN value);
    bool GetIdentifyPossession() const;
    // (WARNING) �ش� �������� �ͼ� ���������� �����̸�, �������� �ͼ� ���ΰ� �ƴ�.
    bool IsPossessionItem() const;
    // __NA_0_POSSESSION_ITEM_DROP = {
    void SetDestroyItem(BOOL value);
    bool IsDestroyItem() const;
    //_NA002823_WINGS_EQUIPMENTS_EXTENTION
    bool IsWingItem() const;
    // };
    bool IsRiderItem() const;
    bool IsRiderSummoned() const; // is rider summoned? UserBlock(t/f)
    nsSlot::RiderUpdateBits RiderSummonStatus(BOOLEAN is_summoned);
    // for server support interface with ProgramCommon
    // (returns) if exist then Acquire 'SCItemSlot|Parts Item'
    //                        { exist | empty (empty item, need check IsRiderPartsItem) }
    //           else     then return 0 { index out of range | is not rider_item }
    bool GetRiderPartsInfo(eRIDER_PARTS_POS pos, SCRiderPartsData* OUT parts_data) const;
    // for Client support interface
    // (returns) if exist then Acquire 'SCItemSlot|Parts Item'
    //                        { exist | empty (empty item, need check IsRiderPartsItem) }
    //           else     then return 0 { index out of range | is not rider_item }
    bool GetRiderPartsItem(eRIDER_PARTS_POS pos, SCItemSlot* OUT parts_item) const;
    // for server support interface
    nsSlot::RiderUpdateBits GetRiderAttrChangedInfo() const;
    RC::eITEM_RESULT CanEquipRiderParts(const SCItemSlot& rider_part_item);
    // for server support interface
    // sequence
    //  1) check deletable inventory slot
    //  2) CanEquipRiderParts
    //  3) delete from inventory slot
    //  4) insert parts in rider (EquipRiderParts) -> change None->Active|Inactive
    //  5) check modification (need update attribute(+)?)
    //  6) update attribute (+)
    bool EquipRiderParts(const SCItemSlot& rider_part_item);
    // for server support interface
    // (returns) ����� ��� ��ġ�� ���� ���� ���¶�� �ݿ����� ���� ����� ��ȯ�Ѵ�.
    // Attribute ��꿡 �ʿ�.
    bool CanUnEquipRiderParts(eRIDER_PARTS_POS pos, nsSlot::RiderUpdateBits* update_bits);
    // for server support interface
    // sequence
    //  1) check insertable inventory slot
    //  2) update attribute (-)
    //  3) delete parts from rider (UnEquipRiderParts)-> get SCItemSlot
    //  4) insert slot
    //  5) serial code issue (lock->unlock) -> change (in)Active -> (in)Active
    // int pos : eRIDER_PARTS_POS::eValue { 91, 92, 93 }
    //
    // (returns) ����� ��� ��ġ�� ����� ����� ��ȯ�Ѵ�. 'CanUnEquipRiderParts' ���� �˻� �ʿ�
    bool UnEquipRiderParts(eRIDER_PARTS_POS pos, SCItemSlot* rider_part_item);
    //bool UpdateRiderItem(ISlotUpdater);
    //
    // (f100727.1L) Shared interface for the Etheria control
    eEtheriaHistory GetEtheriaHistory() const;
    //----------------------------------------------------------------------------------------------
    // checkers for item facilities
    // (CHANGES) (f100510.1L) redirected interfaces to support the contents of an etheria
    static bool CanEquip(const BASE_ITEMINFO* item_info);
    // (CHANGES) (f100510.1L) redirected interfaces to support the contents of an etheria
    static bool CanUseMixture(const BASE_ITEMINFO* item_info, eMIXTURE_FLAG flag);
    // (CHANGES) (f100510.1L)                             whether the 'is_blocked' value is blocked
    static bool CanTradeSell(const BASE_ITEMINFO* item_info, eITEM_TRADESELL_TYPE is_blocked);
    // (CHANGES) (f100510.1L) added interfaces to support the contents of an etheria
    bool CanEquip() const;
    // (CHANGES) (f100510.1L) added interfaces to support the contents of an etheria
    bool CanUseMixture(eMIXTURE_FLAG flag) const;
    // (CHANGES) (f100510.1L) added interfaces to support the contents of an etheria,
    //                        whether the 'is_blocked' value is blocked
    bool CanTradeSell(eITEM_TRADESELL_TYPE is_blocked) const;
    // the result is fails,
    // if mount(by rider) items or pet items is changed to the shell or the etheria
    bool CanSummonStatus() const;
    //----------------------------------------------------------------------------------------------
    bool IsEtheriaItem() const;
    bool IsShellItem() const;
    //{ _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
    bool CanExtractToEtheria(RC::eITEM_RESULT* result) const;
    // this check routine should be called by the shell item.
    bool CanCombineWithEtheria(RC::eITEM_RESULT* result) const;
    // this check routine should be called by the etheria item.
    bool CanCombineWithShell(RC::eITEM_RESULT* result) const;
    // it is operated internally. an item object with options is changed to a default item.
    bool ChangeToShellItem();
    // it is operated internally. an item object with options is changed to an etheria item which
    // is equal to the base item except an image link and an uncorrectable mode (is_editable == 0)
    bool ChangeToEtheriaItem();
    // this -|-> the shell item
    //       |-> the etheria item (from making_item)
    RC::eITEM_RESULT ExtractToEtheria(SCItemSlot* making_item);
    // this(the shell item) -|-> the combination item
    // the etheria item     -|-> post action=delete
    // the etheria item should be cleared, no control in this slot to logic control
    RC::eITEM_RESULT CombineWithEtheria(const SCItemSlot& etheria_item);
    //}
    virtual BOOL OnEquip();
    virtual void OnUnEquip();

    virtual BOOL Update(BOOL is_playing_in_pcroom);

    //���� ������ ���� ��Ʈ�� ��ȯ
    // (f100803.4L) change the type of a result stream to ITEMSTREAMEX
    static BOOL ConvertStream(int IN from_version, BYTE* IN from_stream,
                              int& OUT to_version, ITEMSTREAMEX* OUT to_stream);
    static BOOL _ConvertStream(int latest_version, int IN from_version, BYTE* IN from_stream, int& OUT to_version, BYTE* OUT to_stream);

    static BOOL UnderVersion3StreamConvertTo3(int IN from_version, BYTE* IN from_stream, int& OUT to_version, BYTE* OUT to_stream);
    static BOOL Version3StreamConvertTo4(BYTE* IN from_stream, int& OUT to_version, BYTE* OUT to_stream);
    static BOOL Version4StreamConvertTo5(BYTE* IN from_stream, int& OUT to_version, BYTE* OUT to_stream);
    static BOOL Version5StreamConvertTo6(BYTE* IN from_stream, int& OUT to_version, BYTE* OUT to_stream);
    static BOOL Version6StreamConvertTo7(BYTE* IN from_stream, int& OUT to_version, BYTE* OUT to_stream);
//#ifdef _NA000109_100427_SOLAR_SLOT_UPDATE_TO_VER8_
//    static BOOL VersionXStreamConvertToX(BYTE* IN from_stream, int& OUT to_version, BYTE* OUT to_stream);
//#endif
#ifdef _NA000000_110218_RANK_REPORTER_IN_DBUNIT
    // NOTE: f110221.1L, result of one item convert if it has version upgrade
    static void ClearConvertResult();
    static bool GetConvertResult(eRANK* rank);
#endif
    //==============================================================================================
private: //private methods
    static bool _InitializeBinding(const SCItemSlot* first);
    static void _DestroyBinding(const SCItemSlot* deleting_obj);
    static nsSlot::SerializeItemOption* _GetBindingBlock();
    // (f100803.3L) change a binder allocator to support multiple versions and types
    static bool _SerializeToItemStream(OPTIONPART* OUT serialized_stream, int query_version);
    static bool _SerializeFromItemStream(SLOTCODE item_code, const OPTIONPART* serialized_stream);
    static int _SerializeUsedBits(const ItemParseType::ValueType parse_type);
//#ifdef _NA002237_100802_EP2_SOCKET_SYSTEM_DB_
    // (f100803.4L) detach a convert version with binder from an old version converter.
    static bool _ConvertStreamWithBinder(int start_version, ITEMSTREAMEX* start_stream,
                                         int* result_latest_version,
                                         ITEMSTREAMEX* result_latest_stream);
//#endif
    //{ _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
    // inline function in a code file.
    // reset rules based on the value of a lock_info.history_.
    bool ChangeImageItemRule(eEtheriaHistory::value_type history);
    // change an image link which is shapes for an icon, a render image, and etc.
    // will be a implicit call about the ChangeImageItemRule
    // if the value of a history is not eEtheriaHistory::eHistory::None;
    bool ChangeImageItem(SLOTCODE image_item_code, eEtheriaHistory::value_type history);
    //}
    bool _SetItemInfo();
    bool _UpdateTime();

    // (WARNING) ������ Ÿ�Ժ�(ref:SerializeItemOption���� �����Ǵ�)�� �������� ����ϴ� ��ʰ� �ִ�.
    // �̰��� ��� ���� ���, �ٸ� ���� ���� ����ϴ� Ÿ�������� ��ȯ �Ǵ� ����� ������ �ʴ´�.
    bool _CanReuseSharedBlock() const;

#ifdef _NA_003740_20111122_RANDOM_ITEM
    SCRandomInfo random_info_;
#endif // _NA_003740_20111122_RANDOM_ITEM
    eSlotType    SlotType_info_;

    //==============================================================================================
    //  data fields
    //----------------------------------------------------------------------------------------------
    //  Start Data Field
    //----------------------------------------------------------------------------------------------
    union {
        BYTE item_slot_point_holder_start_[1]; // (WARNING) �ݵ�� ������ �ʵ� ó���� ��ġ���Ѿ� �Ѵ�.
    };

    //----------------------------------------------------------------------------------------------
    //  Default item serializing structure (server internal) (client temporary)
    union {
        struct
        {
            DWORD64 m_dwSerial      : 40;
            DWORD64 m_byType        : 8;
            DWORD64 m_byReserved2   : 8;
            DWORD64 m_byReserved1   : 8;
        };
        DWORD64 m_WorldSerial;
    };

    //----------------------------------------------------------------------------------------------
    SLOTCODE item_code_;
    SLOTCODE image_item_code_; //_NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
    DURATYPE duration_;
    //
    //----------------------------------------------------------------------------------------------
    //
    const BASE_ITEMINFO* item_info_;     // �� ������ ���� ���ο� ���� ���� ��ü�� Ȱ�� ���¸� ǥ���� �� �ִ�.
    const BASE_ITEMINFO* image_item_info_;
    const nsSlot::ItemRuleInfo item_rule_info_;

    BYTE item_slot_point_holder_options_[1]; // (MARK) ������ ���� ���� �ɼǵ� ���� ��ġ
    ISlotUpdaterD        slot_updater_;
    //----------------------------------------------------------------------------------------------
    //  Lock Information
    nsSlot::SCItemLockInfo lock_info_;
    SCLaxLock<BYTE> m_LaxLock; //res.SCItemLockInfo //������ �̵� ���ϹǷ� Copy�� ���� ����

    //----------------------------------------------------------------------------------------------
    // Sharing Part - separated block
    nsSlot::SCEquipInfo equip_info_;
    /*const */SCNOption n_option_info_;
    /*const */SCRankInfo rank_info_;    
#ifdef _NA_0_20100727_SOCKET_SYSTEM_CONVERT
    /*const */SCSocketOptionInfo socket_info_;
#else 
    /*const */SCSocketInfo socket_info_;
#endif // _NA_0_20100727_SOCKET_SYSTEM_CONVERT
#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
    BYTE dicecast_type_;
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING
    //----------------------------------------------------------------------------------------------
    // Sharing Part - address union block
    struct SharedBlock {
        union {
            SCLimitedInfo   limited_info_;
            SCFateInfo      fate_info_;
            SCPetInfo       pet_info_;
            SCWayPointInfo  waypoint_info_;
            SCLottoInfo     lotto_info_;
            nsSlot::SCCoordinateInfo coordinate_info_;
            SCRiderInfo      rider_info_;
            SCRiderPartsInfo rider_parts_info_;
            // _NA_0_20100727_SOCKET_SYSTEM_CONVERT
            nsSlot::SCDefaultEtcInfo default_ect_info_;  
        };
    } shared_;

    nsSlot::SCTimeInfo time_info_;

    //----------------------------------------------------------------------------------------------
    //  Item Options Data Block
    //OPTIONPART backup_; //removed f100909.2L

    // delete reserved
    //WORD m_MapCode; //shared_.coordinate_info_
    //short m_X;      //shared_.coordinate_info_
    //short m_Y;      //shared_.coordinate_info_
    //short m_Z;      //shared_.coordinate_info_

    // ?
    DWORD m_DamageStoreForDura; //?

    //----------------------------------------------------------------------------------------------
    //  ItemSlot Control Field
    // (WARNING) �ݵ�� ������ �ʵ� �������� ��ġ���Ѿ� �Ѵ�.
    BYTE item_slot_point_holder_last_[1];
    //
    //----------------------------------------------------------------------------------------------
    static const SCItemSlot Null_Slot;
    static const BASE_ITEMINFO Null_ItemInfo;
    //----------------------------------------------------------------------------------------------
    // end of data field section
    //----------------------------------------------------------------------------------------------
    friend class SCRiderInfo;
    friend class SCRiderPartsInfo;
    friend class EditModeTransaction;
    // ���� ���� COM Ŭ�������� SCItemSlot�� private ��� �Լ����� ����� �� �ֵ���
    // �ӽ� ��ġ�̸�, ���� ���� �������̽��� �����Ǵ� ��� �� friend ����� ���� ����
    friend class CCSunOnline;
    friend class CSunOnlineItem;
}; //end of class 'SCItemSlot'

//--------------------------------------------------------------------------------------------------
// the change mode class to support the GameLogic library is supported can modify options
class SCItemSlot::EditModeTransaction
{
public:
    EditModeTransaction(SCItemSlot* item_slot);
    ~EditModeTransaction();
private:
    const bool first_value_; // to support toggle
    SCItemSlot* const item_slot_;
    __DISABLE_COPY(EditModeTransaction);
};
//--------------------------------------------------------------------------------------------------

class CSVendorItem : public SCItemSlot
{
public:
	virtual eSlotType GetSlotType() const;
};

// inline function implementation
#define _SOLAR_SLOT_SCITEMSLOT_INL
#include "./SCItemSlot.inl"
#undef _SOLAR_SLOT_SCITEMSLOT_INL

//--------------------------------------------------------------------------------------------------

//==================================================================================================
//==================================================================================================
//==================================================================================================

#endif //_SOLAR_SLOT_SCITEMSLOT_H