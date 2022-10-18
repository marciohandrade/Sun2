// Singleton.h: interface for the Singleton class.
//
//////////////////////////////////////////////////////////////////////

#ifndef SINGLETON_H_
#define SINGLETON_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <assert.h>
#include <windows.h>

namespace util {

// 싱글톤 해제 방법
struct SingletonRelease
{
    enum Value
    {
        kManual = 1, // 수동
        kAuto = 2 // 자동
    };
};

// 수동 해제 버전 (기본값)
template <typename T, SingletonRelease::Value kRelease = SingletonRelease::kManual>
class Singleton
{
protected:
    Singleton()
    {		
    }
    ~Singleton()
    {
    }

public:
    static T* Instance()
    {
        if (instance_ == NULL)
        {
            instance_ = new T;
        }
        return instance_;
    }

    static void DestroyInstance()
    {
        if (instance_ != NULL) 
        {		
            delete instance_;
            instance_ = NULL;
        }
    }

private:
    static T* instance_;
};

// 자동 해제 버전
template <typename T>
class Singleton<T, SingletonRelease::kAuto>
{
protected:
    Singleton()
    {		
    }
    ~Singleton()
    {
    }

public:
    static T* Instance()
    {
        static T instance;
        return &instance;
    }
};

template<typename T, SingletonRelease::Value kRelease> 
T* Singleton< T, kRelease >::instance_ = NULL;

} // util

#endif // SINGLETON_H_