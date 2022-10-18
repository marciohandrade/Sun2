
#pragma once
//------------------------------------------------------------------------------
/**

    라이더 컨테이너(Base: Player)

    @author		광수

    @since		

    @remarks	BASE라이더 컨테이너 Player마다 라이더 컨테이너를가진다.

*/

#include <SCItemSlotContainer.h>

class CControlWZ;

#define MAX_RIDER_SLOT	3
#define LIMIT_SUBTYPE   5

class RiderContainer : public SCItemSlotContainer
{
public:
    enum eRIDER_POS
    {
        RIDER_SLOT_01=0,	//BODY
        RIDER_SLOT_02,		//BOOTS
        RIDER_SLOT_03,		//HELMET
        RIDER_BTN_LEFT, 
        RIDER_BTN_RIGHT, 
        RIDER_BTN_EXIT, 
        RIDER_BTN_TEXT, 
        RIDER_SLOT_VIEW,    //VIEW
        RIDER_MAX,
    };


public:
	RiderContainer();
	virtual ~RiderContainer();

	void					Release();

    void					Clear();

    DWORD CopyItem(SCItemSlot& RiderSlot);

    virtual void            ReloadRider(SCItemSlot &RiderSlot){/*Only Hero*/};

    virtual void			RefreshApperance();

    void                    SetOwnerKey(DWORD dwKey){m_dwOwnerKey = dwKey;};

    virtual RC::eSLOT_INSERT_RESULT		InsertSlot( POSTYPE AtPos, SCSlot & IN rSlot );

	virtual BOOL						DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut );

#ifdef _NA_007732_20141024_RIDER_MOVESPEED_SYNC
    int get_rider_speed() { return rider_speed_; }
    void RefreshRiderSpeed();   // 탈것 이동속도 갱신
#endif //_NA_007732_20141024_RIDER_MOVESPEED_SYNC

protected:
	virtual CControlWZ *	getControl( POSTYPE /*AtPos*/ ){ return NULL; }

	static WzID				m_wzId[RIDER_MAX];

	DWORD                   m_dwOwnerKey;

#ifdef _NA_007732_20141024_RIDER_MOVESPEED_SYNC
    int rider_speed_;
    DWORD dwRiderCode;
#endif //_NA_007732_20141024_RIDER_MOVESPEED_SYNC
};
