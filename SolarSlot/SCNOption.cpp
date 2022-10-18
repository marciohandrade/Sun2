#include "StdAfx.h"
#include ".\scnoption.h"
#include <BitStream.hpp>

//==================================================================================================
//

void
SCNOption::SetNOptionAttr(WORD item_type, LEVELTYPE item_level, int iAttrValueIndex)
{
    const sEXTRASTONE_OPT_INFO* n_option =
        ExtraStoneOptionInfoParser::Instance()->GetExtraStoneOptionInfo(item_type);

    if(n_option != 0 &&
       (n_option->m_ItemLV == 0 || n_option->m_ItemLV == item_level))
    {
        SetNOptionAttr(iAttrValueIndex, n_option);
    }
    else
    {
        Clear();
    }
}

void
SCNOption::SetNOptionAttr(int iAttrValueIndex, const sEXTRASTONE_OPT_INFO* n_option)
{
    Clear();

    if(iAttrValueIndex != 0 &&  // <- 이것 정상인 값인가? n_option->m_dwValue의 표현값은 0,1,2,3이다.
       n_option != 0 &&         // (BUG-FIX) sizeof -> _countof
       n_option->m_wOptionIndex < _countof(g_ITEM_OPTION) &&
       iAttrValueIndex < _countof(n_option->m_dwValue))
    {
        attr_index_ = n_option->m_wOptionIndex;
        attr_type_  = g_ITEM_OPTION[n_option->m_wOptionIndex];
        attr_value_index_ = iAttrValueIndex;
        attr_value_ = n_option->m_dwValue[iAttrValueIndex];
        n_option_ = n_option;
    }
}
//
//==================================================================================================

