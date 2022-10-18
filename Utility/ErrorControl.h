#ifndef __ERROR_CONTROL_H
#define __ERROR_CONTROL_H
#pragma once

//
//
//==================================================================================================
//  (FlowControl)
//  (WAVERIX) (071103)
//  @history
//      0x0100 - ErrorControl
//      0x0110 - Change ErrorControl->FlowControl
//      0x0120 - Update StateControl, Assert Handler
//      0x0130 - Trim unused StateControl,
//                  Change 'FCCheckFail -> IfFailThenBreak, IfFailThen' compare routine
//      0x0140 - fit in coding style and changes condition block
class FlowControl
{
public:
    static const DWORD Version     = 0x0140;

    static const bool  FAIL        = false;
    static const bool  SUCCESS     = true;
    static const DWORD LOGIC_BLOCK = 0x01110000;
    static const DWORD ERROR_BLOCK = 0x00001111;

    FlowControl() : CurrentState(current_state_),
                    current_state_(SUCCESS),
                    error_code_(0),
                    error_string_(0)
    {}

    inline BOOLEAN IsSuccess() const       { return current_state_ != FAIL; }
    inline BOOLEAN IsErrorOccured() const  { return current_state_ == FAIL; }
    inline DWORD   GetErrorCode() const    { return error_code_; }
    inline const char* GetErrorString() const  { return error_string_; }

    //----------------------------------------------------------------------------------------------
    //  (Interface #1)
    //  (Description)
    //      expect_true - ���� �ܰ�� �Ѿ�� ���ؼ��� True���� �ϴ� ����
    //  (Format)
    //      IfFailThenBreak(condition, error_code);
    //  (Pseudo)
    //      if(expect_true == false) {
    //          { Set Error code & Error string }
    //          break;
    //      }
    //      Next step;
    //
    //  (Side-Effect)
    //      errorCode, errorString - condition == false�� ��� �����Ǵ� ��
#pragma warning(push)
#pragma warning(disable : 4706)
    __forceinline BOOLEAN __IsFailFc(bool expect_true,
                                     DWORD if_fail_then_fail_reason_code,
                                     const char* fail_string = 0)
    {
        if(current_state_ && !(current_state_ = expect_true))
        {
            error_code_ = if_fail_then_fail_reason_code;
            error_string_ = fail_string;
        }

        return !current_state_;
    }
#pragma warning(pop)

    //----------------------------------------------------------------------------------------------
    //  (INTERFACE #2)
    //  (Description)
    //      ������ Error Control ó���� �����ϱ� ����.
    //      FCAssert�� ���, ���� FlowControl�̴�. ���� ��� Release��忡�� Assert�ε���
    //      Assert�� �ƴ� ��Ȳ�� �߻����� ���, ����� �����ϴ�.
    //  (Sample)
    //      if(FALSE == flow.FCAssert(failCase)) {
    //          SetFailFc(failCase, error_code);
    //          break;
    //      }
    __forceinline void SetFailFc(DWORD fail_reason_code, char* fail_string = NULL)
    {
        current_state_ = FAIL;
        error_code_ = fail_reason_code;
        error_string_ = fail_string;
    }

#if defined(_DEBUG) || defined(_SERVER) // ����� ���ڿ� ���� ���� ����
    #define IfFailThen(expect_true, if_fail_then_fail_reason_code)          \
        if (flow.CurrentState == FlowControl::FAIL || \
            flow.__IsFailFc(!!(expect_true), if_fail_then_fail_reason_code, ###expect_true))

    #define IfFailThenBreak(expect_true, if_fail_then_fail_reason_code)    \
        if (flow.CurrentState == FlowControl::FAIL || \
            flow.__IsFailFc(!!(expect_true), if_fail_then_fail_reason_code, ###expect_true)) break

    #define SetFailFc(expect_true_but_fail_state, fail_reason_code)      \
        flow.SetFailFc(fail_reason_code, #expect_true_but_fail_state)
#else
    #define IfFailThen(expect_true, if_fail_then_fail_reason_code)          \
        if (flow.CurrentState == FlowControl::FAIL || \
            flow.__IsFailFc(!!(expect_true), if_fail_then_fail_reason_code))

    #define IfFailThenBreak(expect_true, if_fail_then_fail_reason_code)    \
        if (flow.CurrentState == FlowControl::FAIL || \
            flow.__IsFailFc(!!(expect_true), if_fail_then_fail_reason_code)) break

    #define SetFailFc(expect_true_but_fail_state, fail_reason_code)      \
        flow.SetFailFc(fail_reason_code)
#endif

    //////////////////////////////////////////////////////////////////////////
    //  (INTERFACE #3)
    //  (Description)
    //      �Ϲ������� _ASSERT�� ���� ���� release��忡���� ������ �ʴ´�.
    //      ������ _ASSERT�ӿ��� _ASSERT�� �ƴ� ��Ȳ�� �߻��Ѵ�. ���� �����鿡 ������ �߻��Կ���
    //      ������� �ȵǴ� ������ �߻��Ѵ�. Release������ redirect�� �ڵ鸵�� ���� ���� ������ ������ �� �ֵ��� ����
    //      FlowControl::RegisterAssertionHandler ����ϸ� �ȴ�.
    //  (Sample)
    //      if(FALSE == flow.FCAssert(failCase))    {   <- �⺻ �ڵ鷯�� �ܼ� ���, redirect�����ν� ���� �α��� ����
    //          // error control
    //      }
#if _MSC_VER <= 1310
  #if defined(_DEBUG)
    inline static BOOLEAN FCAssert(BOOLEAN must_be_true) {
        return must_be_true != false;
    }
    //  (INTERFACE #3.1.1)
    #define FCAssert(must_be_true)  \
        FCAssert(( (!!(must_be_true)) || (_assert(###must_be_true, __FILE__, __LINE__), 0) ))

  #else
    inline static BOOLEAN FCAssert(char* msg, char* file_name, DWORD line_number) {
        ms_AssertionHandler(msg, file_name, line_number);
        return false;
    }
    inline static BOOLEAN _Interceptor(BOOLEAN intercept) {
        return intercept;
    }
    //  (INTERFACE #3.1.2)
   #ifdef _SUNGAME_VER //client side���� ����ϴ� ��ũ�� (����������)
    #define FCAssert(must_be_true)  \
        _Interceptor(!!(must_be_true))
   #else //!defined(_SUNGAME_VER) //client side���� ����ϴ� ��ũ�� (������������ ������ ��� ������ �ǹ�)
    #define FCAssert(must_be_true)  \
        _Interceptor( !!(must_be_true) || FlowControl::FCAssert(###must_be_true, __FILE__, __LINE__) )
   #endif
  #endif
#else // to support VS2008
  #if defined(_DEBUG)
    inline static BOOLEAN FCAssert(BOOLEAN must_be_true) {
        return must_be_true != false;
    }
    //  (INTERFACE #3.1.1)
    #define FCAssert(must_be_true)  \
        FCAssert(( (!!(must_be_true)) || (_wassert(_CRT_WIDE(#must_be_true), _CRT_WIDE(__FILE__), __LINE__), 0) ))
  #else // to support VS2008
    inline static BOOLEAN FCAssert(char* msg, char* file_name, DWORD line_number) {
        ms_AssertionHandler(msg, file_name, line_number);
        return false;
    }
    inline static BOOLEAN _Interceptor(BOOLEAN intercept) {
        return intercept;
    }
    //  (INTERFACE #3.1.2)
   #ifdef _SUNGAME_VER //client side���� ����ϴ� ��ũ�� (����������)
    #define FCAssert(must_be_true)  \
        _Interceptor(!!(must_be_true))
   #else //!defined(_SUNGAME_VER) //client side���� ����ϴ� ��ũ�� (������������ ������ ��� ������ �ǹ�)
    #define FCAssert(must_be_true)  \
        _Interceptor( !!(must_be_true) || FlowControl::FCAssert(###must_be_true, __FILE__, __LINE__) )
   #endif
  #endif
#endif //

    //////////////////////////////////////////////////////////////////////////
    //  <ASSERTION HANDLER REGISTRATION INTERFACE #1>
    typedef void (*AssertionHandler_t)(char* msg, char* file_name, DWORD line_number);
    static void RegisterAssertionHandler(AssertionHandler_t funcAssertionHandler)
    { funcAssertionHandler && (ms_AssertionHandler = funcAssertionHandler); }
    //////////////////////////////////////////////////////////////////////////
    //  <FIELDs>
    const bool& CurrentState;
private:
    bool  current_state_;
    DWORD error_code_;
    const char* error_string_;
    static AssertionHandler_t ms_AssertionHandler; // can't change name to support library dependency
    __DISABLE_COPY(FlowControl);
};

#define FLOWCONTROL_START()     switch(FlowControl::LOGIC_BLOCK) { case FlowControl::LOGIC_BLOCK:
#define FLOWCONTROL_END         }

#define ERRORCONTROL_START(flow)  switch(flow.GetErrorCode())
#define ERRORCONTROL_END        

//
//
//==================================================================================================
//
//

class FlowControl2 
{
public:
	FlowControl2() 
		: is_error_(false)
		, error_code_(0) {}

public:
    void Init() { is_error_ = false, error_code_ = 0; }

	void SetErrorCode(int  n) { SetError(); error_code_ = n; }
	void GetErrorCode(int& n) { n = (int)error_code_; }

	void SetErrorCode(DWORD  dw) { SetError(); error_code_ = dw; }
	void GetErrorCode(DWORD& dw) { dw = (DWORD)error_code_; }

	bool IsError()	{ return is_error_; }
	void SetError() { is_error_ = true; }

private:	
	bool	is_error_;
	__int64 error_code_;
};


#define _S_TRY_SWITCH		FLOWCONTROL_START() 

#define S_TRY				FlowControl2 f; _S_TRY_SWITCH
#define S_TRY_				f.Init();       _S_TRY_SWITCH

#define S_CATCH0()			break; } if(f.IsError())
#define S_CATCH(type, e)	break; } type e; f.GetErrorCode(e); if(f.IsError())

#define S_THROW0()			{ f.SetError();			break; }
#define S_THROW(e)			{ f.SetErrorCode(e);	break; }


#endif //__ERROR_CONTROL_H
