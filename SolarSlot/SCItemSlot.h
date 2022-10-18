#ifndef _SOLAR_SLOT_SCITEMSLOT_H
#define _SOLAR_SLOT_SCITEMSLOT_H
#pragma once

//==================================================================================================
/// 아이템 슬롯 클래스
/**
    @author Kim Min Wook < taiyo@webzen.co.kr >
    @since  2004. 12. 29
    @remark
    - 하나의 아이템을 나타내는 클래스
    @note
    - Dura의 값은 WithDura일때 내구도, 아닐때는 개수이다. 그러므로 1개이상
    - ITEMSTREAM의 byDura필드는 사용하지 않는다.
    @history 
    - 2006/1/9 : SCRankInfo, SCSocketInfo맴버로 따로 옵션 정보 분리
    - 2006/1/11 : 랭크,인첸트 레벨함수를 호출할때 처리하는 방식으로 변경
    - 2006/1/13 : 내구도와 개수 별개 처리 + GetMaxNum() 추가
    - 2006/1/17 : ExistEmptySocket()함수 추가
    - 2006/2/8 : 
    1. IsBlocked()함수 추가 : 아이템 슬롯에 이용정지 여부 체크 (m_Set == 15이면, 이용정지상태)
    2. void CopyOut(RENDER_ITEMSTREAMEX& OUT ItemStream) 함수 추가
    - 2006/2/13 : RENDER_ITEMSTREAM와 EX관련 함수들 추가
    - 2006/3/17 : SCNOption 정보 추가
    - 2006/4/25 : DropLV => WORD->DWORD로 수정
    - 2006/5/16 : OPT_ITEMSTREAMEX관련 처리
    - 2006/5/17 : 내구도 관련 처리
        BOOL DecreasingDura(DWORD param1, DWORD param2);
        param1 = 공격 성공 데미지 or 방어력
        param2 = 피격자 방어력 or 공격 받은 물리 데미지
        return TRUE = Dura가 감소했을 경우
        return FALSE = Dura가 감소하지 않았을 경우
    - 2006/5/23 : IsLocked(), IsBlocked() 구분처리
    - 2007/1/19 : SetFixedDateTime(int d) 추가 : 시간값(d)을 가공하지 않고 m_DateTime에 바로 값을 셋팅
    - 09.09.06 | waverix | code rearrangement

    @usage
    - 겹쳐지는 아이템인지 여부 : IsOverlap()
    - 내구도가 존재하는 아이템인지 여부 : WithDura()
    - OnEquip() 의 반환값 의미 : eCHARGE_SUB_FIXED_AMOUNT_EX타입일때, 처음 장착한 경우 TRUE반환, 그외 FALSE반환
    - [유료화 관련 아이템 사용법]

    SCItemSlot slot;
    switch(slot.GetItemInfo()->m_byChargeSubType)
    {
    case eCHARGE_SUB_FIXED_AMOUNT:
        {
        // 시간을 셋팅(초단위)
        // if it will be 30일 기한 아이템
        slot.SetDateTime(30*24*60*60);
        // 날짜를 사용
        SYSTEMTIME systime;
        YYYYMMDDHHMMSSToSYSTEMTIME(slot.GetDateTime(), systime);
        //or
        if(!slot.GetDateTimeToSYSTEMTIME(systime))
        {
            // 아이템이 사용 날짜가 지남(만료됨)
        }
        // systime를 가져다 쓰면 된다.
        //......
        }break;
    case eCHARGE_SUB_FIXED_AMOUNT_EX:
        {
            // 시간 셋팅(초단위)
            // 다만 초단위의 숫자는 무시됨!(시간단위로 계산됨)
            // if it will be 30시간 이용 아이템
            slot.SetDateTime(30*60*60);

            // 날짜를 사용
            if(slot.IsProgressTime())
            {
                // 20061107240023
                SYSTEMTIME systime;
                YYYYMMDDHHMMSSToSYSTEMTIME(slot.GetDateTime(), systime);
                //or
                if(!slot.GetDateTimeToSYSTEMTIME(systime))
                {
                // 아이템이 사용 날짜가 지남(만료됨)
                }
                // systime를 가져다 쓰면 된다.
                //.....
            }
            else
            {
                // 사용할수있는시간
                int remain_hours = slot.GetDateTime()/(60*60)
                //....
            }
        }break;
    case eCHARGE_SUB_FIXED_QUANTITY:
        {
            // 사용 시간 입력(초단위로 입력)
            // if it will be 30시간 이용 아이템
            slot.SetDateTime(30*60*60);

            // 남은 사용 시간
            int remain_hours = slot.GetDateTime()/(60*60)
            //....

        }break;
    case eCHARGE_SUB_COORDINATE:
        {
            // 좌표 설정
            slot.SetMapCode(10001);
            slot.SetX(13);
            slot.SetY(145);
            slot.SetZ(21);
            // 좌표 이용
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
    enum { FIXED_AMOUNT_EXPIRE_WAIT_TIME = 7 * 24 * 60 * 60 }; //< 7일 대기후 삭제
public:
    class EditModeTransaction;

    SCItemSlot(void);
    virtual ~SCItemSlot(void);

    inline SCItemSlot(const SCItemSlot& r);
    inline SCItemSlot& operator=(const SCItemSlot& r);

    //SCItemSlot(const ITEMSTREAM& item_stream); //< 옵션을 제외한 정보만을 셋팅
    SCItemSlot(const ITEMSTREAMEX& item_stream); //< 옵션을 포함한 정보를 셋팅
    //SCItemSlot(const RENDER_ITEMSTREAM& item_stream);
    SCItemSlot(const RENDER_ITEMSTREAMEX& item_stream);
    SCItemSlot(const OPT_ITEMSTREAMEX& item_stream);
    void Copy(const ITEMSTREAMEX& IN item_stream);
    //void Copy(const ITEMSTREAM& IN item_stream);
    //void Copy(const RENDER_ITEMSTREAM& IN item_stream); //< 서버에서구현안함
    void Copy(const RENDER_ITEMSTREAMEX& IN item_stream); //< 서버에서구현안함
    void Copy(const OPT_ITEMSTREAMEX& IN item_stream);
    void CopyOut(ITEMSTREAMEX& OUT item_stream) const;
    //void CopyOut(ITEMSTREAM& OUT item_stream) const; //< 옵션 제외
    void CopyOut(RENDER_ITEMSTREAMEX& OUT item_stream) const; //< 시리얼 제외
    //void CopyOut(RENDER_ITEMSTREAM& OUT item_stream) const; //< 시리얼+옵션 제외
    void CopyOut(OPT_ITEMSTREAMEX& OUT item_stream) const; //< 시리얼+옵션수치 제외

    virtual void Copy(BitStream& IN stream, eSLOT_SERIALIZE eType);
    virtual void CopyOut(BitStream& OUT stream, eSLOT_SERIALIZE eType);

    virtual void Copy(BitStream& IN stream);
    virtual void CopyOut(BitStream& OUT stream);

    virtual void Copy_Render(BitStream& IN stream);
    virtual void CopyOut_Render(BitStream& OUT stream);

private:
    // 내부적으로 사용할 셋팅 함수
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
    // (WAVERIX) (NOTE) 아이템 설정시 최초에 설정해야하는 코드 : 코드가 정해져야 다음 정책이 세워진다.
    // 전제: SCItemSlot instance가 완료되어 있어야 한다. (즉, 초기화 되어 있어야 한다.)
    // 예외: Copy(from server stream, db stream)의 경우라면, SetCode는 내부에서 호출된다.
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
    bool IsUserBlocked() const; // Game issue에 의해 락을 걸어야 하는 경우, ex) 펫/라이더 소환 상태 + wings spreading
    bool IsPetLocked() const; // UserBlock사용 중이다. = 'IsUserBlocked' || 'IsRiderSummoned'
    // f100909.1L
    void SetOptionTag(const KindOfItemTag& tag);
    const KindOfItemTag& GetOptionTag() const;
    // 개수
    DURATYPE GetMaxNum() const;

    //----------------------------------------------------------------------------------------------
    // 아이템 복사(for stat change) - serial key not issued, need serial key 
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
    // 내구도 관련

    // pseudo
    // if 내구도로 사용하는가?
    //   (true)
    // elif 개수로 사용하는가?
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
    // 겹쳐지는 아이테인지여부 체크
    bool IsOverlap() const;
    static bool IsOverlap(SLOTCODE code);

    // 계산식이 필요한 ITEMINFO 함수 추가 예정
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

    //(lst1024)(080918)(2447)인챈트시 내구도를 다시 셋팅 해준다.
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
    // Divine Option (old comment) Set 정보
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
    // 좌표
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
    // 날짜
    bool IsProgressTime() const;
    void SetProgressTime(bool bFlag);

    DWORD64 GetDateTime() const;
    BOOL GetDateTimeToSYSTEMTIME(SYSTEMTIME& systime OUT);
    void SetDateTime(DWORD64 d/*second*/);
    void SetFixedDateTime(DWORD64 d); //< 주의! : 타입에 따라 다른 형식의 값을 입력해야함
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
    // 해당 등급의 쟈드가 삽입 가능한지 검사한다.
    bool IsEnableFillSocketByGrade(const eSOCKET_LEVEL zard_grade) const;

    //----------------------------------------------------------------------------------------------
    // NOption 정보
    //  - (NOTE) ExtraStone Option이란다...
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

    // (WARNING) 외부에서 제어하는 시점은 2군데, { drop, gmcmd } 이것이 설정된 이후로
    // 시리얼 키 인증이 되면 사용이 가능하다.
    // (NOTE) 추가 1개소 (헤르메시즘의 성서) 이것은 이미 인증된 아이템에 대해서 값을 변경하는 것.
    // 다른 Set함수는 사용하지 말 것. 대체 인터페이스 제공(CopyFromSrcForStatChange)
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
    // (WARNING) 클라이언트에서 Refresh하는 사례가 발견되어 상수화하지 못했다.
    SCLottoInfo* GetLottoInfo() const;

#ifdef _NA_0_20100727_SOCKET_SYSTEM_CONVERT
    WORD GetExtendData() const;
    void SetExtendData(WORD v);

    bool IsZardItem() const; // 쟈드 아이템인지 검사한다.
    bool IsZardAdditiveItem() const; // 쟈드 합성 첨가제 아이템인지 검사한다.
    // 소켓 생성이 가능한 아이템일 경우 소켓을 생성한다.
    bool CreateSocketItem(const BYTE number_of_socket, const bool is_recreate); 
#endif // _NA_0_20100727_SOCKET_SYSTEM_CONVERT
#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
    BYTE GetDiceCastType();
    void SetDiceCastType(BYTE _type);
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING

    // 랜덤화 가능한 아이템인지 검사한다.
    bool EnableRandomize() const;
#ifdef _NA_003740_20111122_RANDOM_ITEM
    // 랜덤화 가능한 아이템일 경우 랜덤으로 아이템 능력치를 다시 설정한다.
    bool Randomize(const RandomValueTypeGrade::Value value_type_grade);

    // 랜덤화된 옵션의 등급을 얻는다.
    RandomValueGrade::Value GetRandomOptionGrade(const uint8_t index) const;

    // 랜덤화된 옵션의 문자열 표현을 얻는다.
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
    bool FireUp_DateTime();  // frame단위로 변경할 필요가 있다.
    void ClearWayPointItem();   // for GM command
    bool IsWayPointItem() const;
    //bool IsProgressTime() const;
    bool CanWayPointDelete() const;
    void ResetWayPointDelete();

    //----------------------------------------------------------------------------------------------
    //  Possession Info
    void SetIdentifyPossession(BOOLEAN value);
    bool GetIdentifyPossession() const;
    // (WARNING) 해당 아이템이 귀속 가능한지의 여부이며, 아이템의 귀속 여부가 아님.
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
    // (returns) 추출된 장비 위치가 기한 만료 상태라면 반영되지 않은 결과를 반환한다.
    // Attribute 계산에 필요.
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
    // (returns) 추출된 장비 위치를 기록한 결과를 반환한다. 'CanUnEquipRiderParts' 선행 검사 필요
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

    //버전 관리를 위한 스트림 변환
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

    // (WARNING) 아이템 타입별(ref:SerializeItemOption으로 구별되는)로 공유정보 사용하는 사례가 있다.
    // 이것이 사용 중일 경우, 다른 공유 정보 사용하는 타입으로의 전환 또는 사용은 허용되지 않는다.
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
        BYTE item_slot_point_holder_start_[1]; // (WARNING) 반드시 데이터 필드 처음에 위치시켜야 한다.
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
    const BASE_ITEMINFO* item_info_;     // 이 포인터 설정 여부에 따라 슬롯 자체의 활성 상태를 표기할 수 있다.
    const BASE_ITEMINFO* image_item_info_;
    const nsSlot::ItemRuleInfo item_rule_info_;

    BYTE item_slot_point_holder_options_[1]; // (MARK) 아이템 슬롯 고유 옵션들 시작 위치
    ISlotUpdaterD        slot_updater_;
    //----------------------------------------------------------------------------------------------
    //  Lock Information
    nsSlot::SCItemLockInfo lock_info_;
    SCLaxLock<BYTE> m_LaxLock; //res.SCItemLockInfo //어차피 이동 못하므로 Copy에 구현 안함

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
    // (WARNING) 반드시 데이터 필드 마지막에 위치시켜야 한다.
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
    // 게임 로직 COM 클래스에서 SCItemSlot의 private 멤버 함수들을 사용할 수 있도록
    // 임시 조치이며, 게임 로직 인터페이스가 정리되는 대로 이 friend 관계는 삭제 예정
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