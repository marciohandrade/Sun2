// wzcriticalsection.h

#ifndef _PROGRAMCOMMON_WZCRITICALSECTION_H_
#define _PROGRAMCOMMON_WZCRITICALSECTION_H_

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500 
#endif

#include "wztypedef.h"

//------------------------------------------------------------------------------
/**
    @class CWzCriticalSection
*/
class CWzCriticalSection
{
public:
    CWzCriticalSection( void )
    {
        InitializeCriticalSection( &m_csObjet );
    }

    ~CWzCriticalSection( void )
    {
        EnterCriticalSection( &m_csObjet );
        DeleteCriticalSection( &m_csObjet );
    }

    void Enter( void )
    {
        EnterCriticalSection( &m_csObjet );
    }

    void Leave( void )
    {
        LeaveCriticalSection( &m_csObjet );
    }

private:
    CRITICAL_SECTION m_csObjet;
};

//------------------------------------------------------------------------------
/**
    @class CWzSync
*/
class CWzSync
{
public:
    CWzSync( CWzCriticalSection* wcs )
    : m_wcs( wcs ) 
    { 
        WzAssert( m_wcs );

        if( m_wcs )
        {
            m_wcs->Enter(); 
        }
    }

    ~CWzSync( void )
    {
        if( m_wcs )
        {
            m_wcs->Leave();
        }
    }

private:
    CWzCriticalSection* m_wcs;
};

//------------------------------------------------------------------------------
/**
*/
#define WzDeclare_Sync( name )  CWzCriticalSection x_pcs##name
#define WzSync_This( name )     CWzSync sy##name( &x_pcs##name )

#endif // _PROGRAMCOMMON_WZCRITICALSECTION_H_