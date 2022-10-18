#ifndef _SOLAR_SLOT_SCLAXLOCK_HPP
#define _SOLAR_SLOT_SCLAXLOCK_HPP
#pragma once

template<typename _TYPE>
class SCLaxLock
{
    _TYPE m_Count;
public:
    SCLaxLock();
    virtual ~SCLaxLock(); 

    BOOL CanLock();
    void EnterLock();
    void LeaveLock();
    BOOL IsLock();

    void Clear();
};

template<typename _TYPE>
SCLaxLock<_TYPE>::SCLaxLock()
{
    m_Count = 0;
}

template<typename _TYPE>
SCLaxLock<_TYPE>::~SCLaxLock()
{
    m_Count = 0;
}

template<typename _TYPE>
BOOL SCLaxLock<_TYPE>::CanLock()
{
    BOOST_STATIC_ASSERT(boost::is_integral<_TYPE>::value != false);
    // (CHANGES) (100115) changes usage 'pow' function
    if (m_Count < static_cast<_TYPE>(-1)) //if(m_Count < ::pow(2, sizeof(_TYPE)*8) - 1)
        return TRUE;

    return FALSE;
}

template<typename _TYPE>
void SCLaxLock<_TYPE>::EnterLock()
{
    BOOST_STATIC_ASSERT(boost::is_integral<_TYPE>::value != false);
    if (m_Count == static_cast<_TYPE>(-1)) //if(m_Count == ::pow(2, sizeof(_TYPE)*8) - 1)
    {
        ASSERT(FALSE);
        return;
    }

    m_Count++;
}

template<typename _TYPE>
void SCLaxLock<_TYPE>::LeaveLock()
{
    if(m_Count == 0)
    {
        ASSERT(FALSE);
        return;
    }

    m_Count--;
}

template<typename _TYPE>
BOOL SCLaxLock<_TYPE>::IsLock()
{
    if(m_Count > 0)
        return TRUE;

    return FALSE;
}

template<typename _TYPE>
void SCLaxLock<_TYPE>::Clear()
{
    m_Count = 0;
}

#endif //_SOLAR_SLOT_SCLAXLOCK_HPP
