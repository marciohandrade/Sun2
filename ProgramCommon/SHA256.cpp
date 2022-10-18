#include "stdafx.h"
#include <shlwapi.h>
#include <comdef.h>
#include <CRTDBG.H>
#include <atlconv.h>
#include <wchar.h>
#include <strsafe.h>

#include "./SHA256.h"
#include "./OSVersion.h"


#pragma comment(lib,"shlwapi.lib")



namespace Crypt
{

class SHA256_Module
{
public:
    SHA256_Module()
    :handle_(NULL)
    ,SHA256_Crypt(NULL)
    ,SHA256_Compare(NULL)
    {
        memset(last_error_, 0, _countof(last_error_));
    }

    ~SHA256_Module()
    {
        if (handle_)
        {
            FreeLibrary(handle_);
            handle_ = NULL;
        }
    }

    bool Load()
    {
        memset(last_error_, 0, _countof(last_error_));

        if (handle_ != NULL)
            return true;

        bool os_error_flag = false;
        Greenmaru::COSVersion osv;
        if( osv.IsWindowsXp() && 3 <= osv.ServicePack() )
        {
            os_error_flag = true;
        }
        else
        {
            if( osv.IsWindows2003() || osv.IsWindowsVistaOrLater() )
            {
                os_error_flag = true;
            }
        }

        if (os_error_flag == false)
        {
            strcpy(last_error_, "Support OS is XP Service pack 3, VISTA, WINDOWS 7 or later and Server 2003, Server 2008 or later");
            return false;
        }

#ifdef _X64_VERIFY_
    #ifdef _DEBUG
            handle_ = LoadLibrary("WZMsgDigest_x64d.dll");
    #else
            handle_ = LoadLibrary("WZMsgDigest_x64.dll");
    #endif
#else
    #ifdef _DEBUG
            handle_ = LoadLibrary("WZMsgDigest_x86d.dll");
    #else
            handle_ = LoadLibrary("WZMsgDigest_x86.dll");
    #endif
#endif

        if (handle_ == NULL)
        {
            strcpy(last_error_, "Fail to LoadLibrary(WZMsgDigest_XXX.dll)");
            return false;
        }


        SHA256_Crypt	= (lpfnDigest)GetProcAddress(handle_, "XP_SHA2_CryptKeyEx");
        SHA256_Compare	= (lpfnCompare)GetProcAddress(handle_, "XP_SHA2_CompareEx");

        if(SHA256_Crypt == NULL || SHA256_Compare == NULL)
        {
            FreeLibrary(handle_);
            handle_ = NULL;
            strcpy(last_error_, "Fail to GetProcAddress(XP_SHA2_CryptKeyEx, XP_SHA2_CompareEx)");
            return false;
        }

        return true;
    }

    const char* GetLastError() const
    {
        return last_error_;
    }

    typedef const char* (*lpfnDigest)(BSTR, BSTR);
    typedef bool (*lpfnCompare)(BSTR, BSTR, BSTR);

    lpfnDigest SHA256_Crypt;
    lpfnCompare SHA256_Compare;

private:
    HMODULE     handle_;
    char        last_error_[256];
};

SHA256_Module g_sha256_module_;





SHA256::SHA256()
{
}

SHA256::~SHA256()
{
}

const char* SHA256::GetLastError()
{
    return g_sha256_module_.GetLastError();
}

const char* SHA256::CreateKey(const char* input1, const char* input2)
{
    if (g_sha256_module_.Load() == false)
        return NULL;

    if (input1 == NULL || input2 == NULL)
        return NULL;

    if (strlen(input1) == 0 || strlen(input2) == 0)
        return NULL;

    USES_CONVERSION;

    BSTR first_in = ::SysAllocString(A2W(input1));
    BSTR second_in = ::SysAllocString(A2W(input2));

    bool result_flag = false;
    const char* result = g_sha256_module_.SHA256_Crypt(first_in, second_in);
    if (result && strlen(result) != 0)
    {
        strncpy(key_string_, result, 65);
        result_flag = true;
    }
    else
    {
        memset(key_string_, 0, 65);
    }

    ::SysFreeString(first_in);
    ::SysFreeString(second_in);

    return result_flag ? key_string_ : NULL;
}


bool SHA256::Compare(const char* input1, const char* input2, const char* key_string)
{
    if (g_sha256_module_.Load() == false)
        return NULL;

    if (input1 == NULL || input2 == NULL || key_string == NULL)
        return NULL;

    if (strlen(input1) == 0 || strlen(input2) == 0 || strlen(key_string) == 0)
        return NULL;

    USES_CONVERSION;

    BSTR first_in = ::SysAllocString(A2W(input1));
    BSTR second_in = ::SysAllocString(A2W(input2));
    BSTR digest_in = ::SysAllocString(A2W(key_string));

    bool result_flag = g_sha256_module_.SHA256_Compare(first_in, digest_in, second_in);

    ::SysFreeString(first_in);
    ::SysFreeString(second_in);
    ::SysFreeString(digest_in);

    return result_flag;
}


}