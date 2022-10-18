#pragma once

namespace WGPL
{
    //! 해킹 감지시 콜백
    typedef void(*fn_CallbackWGPL)(void);
    extern fn_CallbackWGPL _Callback_WGPL;

	void* Alloc(size_t size);
	void Free(void* p, size_t size);
    void SetCallbackWGPL(fn_CallbackWGPL fn);
}
