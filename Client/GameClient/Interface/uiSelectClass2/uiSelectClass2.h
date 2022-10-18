#pragma once
#if defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
#include "uiBase.h"

class SelectClass2
{
public:
    SelectClass2(void);    
    ~SelectClass2();

public:
    void Init(void);
    void Release(void);    

    // 설정자
public:
    void set_current_class_type(eCHAR_TYPE select_class_type) { current_class_type_ = select_class_type; }
    void set_current_class_code(BYTE selected_class_code) { current_class_code_ = selected_class_code; }

    // 접근자
public:
    BYTE get_selected_class_code(void) { return current_class_code_; }

public:
    void OnExcute(void);
    void select_create_class(eCHAR_TYPE select_class_type, BYTE select_class_code);

    // 멤버 변수
private:
    eCHAR_TYPE current_class_type_;
    BYTE       current_class_code_;
};
#endif // _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES
