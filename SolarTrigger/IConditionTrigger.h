#pragma once
#ifndef SOLARTRIGGER_ICONDITIONTRIGGER_H
#define SOLARTRIGGER_ICONDITIONTRIGGER_H
//==================================================================================================
/// IConditionTrigger class
/**
    @author Kim Min Wook < taiyo@webzen.com >
    @since  2004. 12. 29
    @remark
        - ����� Ʈ���ſ� ���� �߻��� �ֻ��� Ŭ����
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
    virtual void OnInstanciate(); //< �����ڰ� ȣ��ɶ� : Inherited class���� ȣ���ؾ� ��ȿ��!��!
    virtual void OnInit();        //< Ʈ���� Active������ �ѹ� ȣ��
    virtual void OnReset();       //< ������ �ʿ��� ���, satisfied_ = false
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
} //< �����ڰ� ȣ��ɶ� : Inherited class���� ȣ���ؾ� ��ȿ��!��!

inline void IConditionTrigger::OnInit() {
}        //< Ʈ���� Active������ �ѹ� ȣ��

inline void IConditionTrigger::OnReset() {
}       //< ������ �ʿ��� ���, satisfied_ = false

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