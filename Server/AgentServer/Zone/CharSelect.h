#pragma once
//=============================================================================================================================
/// CharSelect 클래스
/**
	@author	Kim Min Wook < taiyo@webzen.com >
	@since	2005. 12. 8
	@remark
		- 캐릭터 선택창에 대한 존 정보 관리
	@note
		- 
	@history 
		- (WAVERIX) (090114) Change 'ZoneInterface::GetMapCode()' To abstract
	@usage
		- 
*/
//=============================================================================================================================

#include "zoneinterface.h"

class CharSelect : public ZoneInterface
{
	_DECLARE_POOL(CharSelect);
public:
    // NOTE: f110622.2L, when all character list received from the database proxy,
    // the related timer will start using the interval.
    static const DWORD kHeIsHereNotificationInterval = 60 * 1000;
	CharSelect(void);
	virtual ~CharSelect(void);

	VOID Create( KEYTYPE key );
	inline virtual eZONETYPE GetType() { return eZONETYPE_CHARSELECT; }
    // (WAVERIX) (090114) Change 'ZoneInterface::GetMapCode()' To abstract
    //virtual	MAPCODE GetMapCode() { return 11000; } // non-change hardcode value
    // NOTE: f110622.2L, ZoneInterface interface
    virtual BOOL Process();
    // NOTE: f110622.2L
    void StartHeIsHereNotificationTimer();
protected:

	virtual VOID On_create();
	virtual VOID On_destroy();
	virtual VOID On_readyuser( User * pUser );
	virtual VOID On_enteruser( User * pUser );
	virtual VOID On_leaveuser( User * pUser );
    //
private:
    util::ITimerBase notification_timer_updater_;
};
