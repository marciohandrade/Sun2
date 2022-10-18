#pragma once
#ifndef SOLARTRIGGER_ICONDITIONTRIGGER_H
#define SOLARTRIGGER_ICONDITIONTRIGGER_H
//==================================================================================================
/// IConditionTrigger class
/**
    @author Kim Min Wook < taiyo@webzen.com >
    @since  2004. 12. 29
    @remark
        - 컨디션 트리거에 대한 추상형 최상위 클래스
    @note
        - 
    @history 
        - 
*/
//==================================================================================================
struct TRIGGER_MSG;
class BaseTrigger;
class TriggerManager;

class IConditionTrigger
{
public:
    IConditionTrigger();
    virtual ~IConditionTrigger();

    void Init();

    virtual BOOL IsSatisfied();
    virtual void OnInstanciate(); //< 생성자가 호출될때 : Inherited class에서 호출해야 유효함!必!
    virtual void OnInit();        //< 트리거 Active시점에 한번 호출
    virtual void OnReset();       //< 리셋이 필요한 경우, satisfied_ = false
    virtual BOOL NeedUpdate();
    bool OnMsg(TRIGGER_MSG* trigger_msg);

    virtual WORD TriggerType() = 0;
    void Clear();

protected:
    virtual void onMsg(TRIGGER_MSG* trigger_msg);
    void setSatisfied(BOOL val);
    //
    bool satisfied_;
};


inline IConditionTrigger::IConditionTrigger()
    : satisfied_(false)
{
}

inline IConditionTrigger::~IConditionTrigger()
{
}

inline void IConditionTrigger::Init() {
}

inline BOOL IConditionTrigger::IsSatisfied() {
    return satisfied_;
}

inline void IConditionTrigger::OnInstanciate() {
} //< 생성자가 호출될때 : Inherited class에서 호출해야 유효함!必!

inline void IConditionTrigger::OnInit() {
}        //< 트리거 Active시점에 한번 호출

inline void IConditionTrigger::OnReset() {
}       //< 리셋이 필요한 경우, satisfied_ = false

inline BOOL IConditionTrigger::NeedUpdate() {
    return false;
}

inline void IConditionTrigger::Clear() {
    satisfied_ = false;
}

inline void IConditionTrigger::onMsg(TRIGGER_MSG* trigger_msg)
{
    __UNUSED(trigger_msg);
}

inline void IConditionTrigger::setSatisfied(BOOL val)
{
    satisfied_ = (val != false);
}


#endif //SOLARTRIGGER_ICONDITIONTRIGGER_H