#include "stdafx.h"
#include "./SCSkillSlot.h"

#include <limits.h>
#include <SkillInfoParser.h>

//==================================================================================================
// (CHANGES) (f100307.3) add const null stream
BOOST_STATIC_ASSERT(sizeof(SkillScriptInfo) >= sizeof(BASE_STYLEINFO));
static const char kEmptyStream_static[sizeof(SkillScriptInfo)] = { 0, };

const SkillScriptInfo& SCSkillSlot::Null_SkillInfo = \
    *reinterpret_cast<const SkillScriptInfo*>(kEmptyStream_static);

//--------------------------------------------------------------------------------------------------

SCSkillSlot::SCSkillSlot(void)
    : root_skill_info_(NULL)
{
    Clear();
}

SCSkillSlot::~SCSkillSlot(void)
{
}

void
SCSkillSlot::SetCode(SLOTCODE code)
{ 
    //ASSERT(code <= USHRT_MAX); (f100307.2) removes meaningless assertion checker
    // (NOTE) must be partial initialization, \
    // because custom coding have a precede action previous 'SetCode' call
    ZeroMemory(&skill_stream_, sizeof(skill_stream_));
    root_skill_info_ = NULL;
    // set
    skill_stream_.Part.wCode = code; 
    _SetSkillInfo();
}

void
SCSkillSlot::_SetSkillInfo()
{
    SLOTCODE skill_code = GetCode();
    assert(skill_code != 0);
    root_skill_info_ = SkillInfoParser::Instance()->GetInfo(skill_code);
    if (FlowControl::FCAssert(root_skill_info_ != NULL) == false)
    {
    #ifdef _SERVER
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|can't find skill info(%d)|"),
               __FUNCTION__, skill_code);
    #endif
        // desired occurred fault if routine has invalid access
        root_skill_info_ = &Null_SkillInfo;
        return;
    }
#if _DEBUG
    // add checker
    char is_skill = root_skill_info_->IsSkill();
    char is_style = root_skill_info_->IsStyle();
    ASSERT((is_skill ^ is_style) == 1);
#endif
}

bool
SCSkillSlot::IsStyle() const
{
    if (IsContainValidInfo()) {
        return root_skill_info_->IsStyle();
    }
    return false;
}

bool
SCSkillSlot::IsSKill() const
{
    if (IsContainValidInfo()) {
        return root_skill_info_->IsSkill();
    }
    return false;
}

const BASE_STYLEINFO*
SCSkillSlot::GetStyleInfo() const
{
    if (FlowControl::FCAssert(IsContainValidInfo()))
    {
        if (root_skill_info_->IsStyle()) {
            return static_cast<const BASE_STYLEINFO*>(root_skill_info_);
        }
    }
    return NULL;
}

const SkillScriptInfo*
SCSkillSlot::GetSkillInfo() const
{
    if (FlowControl::FCAssert(IsContainValidInfo()))
    {
        if (root_skill_info_->IsSkill()) {
            return static_cast<const SkillScriptInfo*>(root_skill_info_);
        }
    }
    return NULL;
}

