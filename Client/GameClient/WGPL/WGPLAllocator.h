#pragma once

namespace WGPL
{
    //! ��ŷ ������ �ݹ�
    typedef void(*fn_CallbackWGPL)(void);
    extern fn_CallbackWGPL _Callback_WGPL;

	void* Alloc(size_t size);
	void Free(void* p, size_t size);
    void SetCallbackWGPL(fn_CallbackWGPL fn);
}
