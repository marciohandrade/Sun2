#pragma once
#ifndef SOLARTRIGGER_IACTIONTRIGGER_H
#define SOLARTRIGGER_IACTIONTRIGGER_H
//==================================================================================================
/// IActionTrigger class
/**
    @author Kim Min Wook < taiyo@webzen.com >
    @since  2004. 12. 29
    @remark
        - 액션 트리거에 대한 추상형 최상위 클래스
    @note
        - 
    @history 
        - 
*/
//==================================================================================================


class BaseTrigger;
class TriggerManager;

class IActionTrigger
{
public:
    IActionTrigger() {}
    virtual ~IActionTrigger() {}

    void Init();

    virtual void OnInit() {}
    virtual void OnAction() = 0;
    virtual WORD TriggerType() = 0;
    virtual BOOL OnActionUpdate() { return false; }
};

#endif //SOLARTRIGGER_IACTIONTRIGGER_H