#ifndef _SOLAR_SLOT_SCQUICKSTYLESLOTCONTAINER_H
#define _SOLAR_SLOT_SCQUICKSTYLESLOTCONTAINER_H
#pragma once

//==================================================================================================
/// SCQuickStyleSlotContainer class
/**
    @author Kim Min Wook < taiyo@webzen.co.kr >
    @since  2004. 12. 29
    @remark
    - 퀵 스타일 슬롯을 조작하는 컨테이너 클래스
    @note
    - 
    @history 
    - 09.09.06|waverix|code rearrangement
    - 10.03.10|waverix|refactoring for enhancement of the functionality of slot container
                      |enforce integrity check, to solve client crash fault problem
*/
//==================================================================================================

#include "./SCSlotStruct.h"
#include "./SCSlotContainer.h"

class SCQuickStyleSlotContainer : public SCSlotContainer
{
public:
    SCQuickStyleSlotContainer(void);
    virtual ~SCQuickStyleSlotContainer(void);

    virtual eSlotType GetSlotType() const { return ST_STYLE_QUICK; }
    virtual SCSlot* CreateSlot();

    void InsertSlot(POSTYPE at_pos, STYLESTREAM& stream);
    virtual BOOL IsEmpty(POSTYPE at_pos);
};

#endif //_SOLAR_SLOT_SCQUICKSTYLESLOTCONTAINER_H
