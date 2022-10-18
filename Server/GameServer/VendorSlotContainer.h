#pragma once

//=============================================================================================================================
/// VendorSlot class
/**
@author	Kim Min Wook < taiyo@webzen.com >
@since	2004. 12. 29
@remark
- VendorSlot을 조작하는 컨테이너 클래스
@note
- 
@history 
- 
*/
//=============================================================================================================================

#include <scslotcontainer.h>
#include <SafeStringBufferControl.h>

struct ESTABLISHER_VENDOR_TOTAL_INFO;

class VendorSlotContainer : public SCSlotContainer
{
public:		
    VendorSlotContainer();
	virtual ~VendorSlotContainer();

public:
    virtual BOOL IsLocked( POSTYPE AtPos );
    virtual void SetLock( POSTYPE AtPos, BOOL val );
    virtual BOOL IsEmpty( POSTYPE AtPos );
    virtual eSlotType GetSlotType() const					
    { 
        return ST_VENDOR; 
    }

public:
	bool CreateVendor(
        Player* const establisher, 
        const util::StrArrayForVendorTitle& title,
		const ESTABLISHER_VENDOR_TOTAL_INFO& vendor_total_info
	);
	void DestroyVendor();
	bool ModifySlot(const POSTYPE pos, const VENDOR_ITEMSLOT& vendor_slot);
    Player*	GetEstablisher() const
    { 
        if (obj_util::IsValid(established_player_) == false)
        {
            return NULL;
        }
        return established_player_;	
    }
    void AddObserver(Player* const observer);
	void RemoveObserver(const Player* const observer);
    const TCHAR* GetTitle() const
    {
        return vendor_title_;
    }
    void SetTitle(const util::StrArrayForVendorTitle& vendor_title)
    {
        _tcsncpy(vendor_title_, vendor_title, _countof(vendor_title_));
        vendor_title_[_countof(vendor_title_) - 1] = _T('\0');
    }
	void SendPacketToObservers(
        Player* const actor, MSG_BASE_FORWARD* const msg_base, const uint16_t msg_size) const;

#ifdef _NA_0_20091208_VENDOR_ITEM_SEARCH
    virtual void OnInsert(SCSlot& IN rSlotIn);
    virtual void OnDelete(SCSlot& IN rSlotIn);
    BOOL HasItem( SLOTCODE ItemCode );
#endif //_NA_0_20091208_VENDOR_ITEM_SEARCH

private:	
	virtual SCSlot*	CreateSlot();

private:	
    Player*	established_player_;
	PLAYER_HASH	observer_players_;
    util::StrArrayForVendorTitle vendor_title_;

#ifdef _NA_0_20091208_VENDOR_ITEM_SEARCH
    typedef STLX_HASH_MAP<SLOTCODE, INT> SLOTCODE_HASH;
    SLOTCODE_HASH  m_SlotCodeHash;
#endif //_NA_0_20091208_VENDOR_ITEM_SEARCH
};