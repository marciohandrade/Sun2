//------------------------------------------------------------------------------
//  uiBaseMan_net.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiBaseMan.h"

//------------------------------------------------------------------------------
/**
*/
void uiBaseMan::InitPKBooleans( void )
{
    m_vPKBool.clear();
}

//------------------------------------------------------------------------------
/**
*/
int uiBaseMan::TimeOutPKBooleans( void )
{
	int size = (int)m_vPKBool.size();

    for( int i = 1; i < size; ++i )
    {
        if( m_vPKBool[i].timeOut() )
        {
            m_vPKBool[i].clear();
            return i;
        }
    }

    return 0;
}

//------------------------------------------------------------------------------
/**
*/
void uiBaseMan::HandlerPKBooleans( void )
{
	TimeOutPKBooleans();
}

//------------------------------------------------------------------------------
/**
*/
void uiBaseMan::SetPKBool( int eIdx, DWORD dwTimeOut )
{
	int size = (int)m_vPKBool.size();
	assert( eIdx > 0 && eIdx < size );

	if( eIdx > 0 && eIdx < size )
	{
		m_vPKBool[eIdx].setWorking( dwTimeOut );
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiBaseMan::UnsetPKBool( int eIdx )
{
	int size = (int)m_vPKBool.size();
	assert( eIdx > 0 && eIdx < size );

	if( eIdx > 0 && eIdx < size )
	{
		m_vPKBool[eIdx].clear();
	}
}

//------------------------------------------------------------------------------
/**
*/
bool uiBaseMan::GetPKBool( int eIdx )
{
	int size = (int)m_vPKBool.size();
	assert( eIdx > 0 && eIdx < size );

	if( eIdx > 0 && eIdx < size )
	{
		return m_vPKBool[eIdx].getWorking();
	}

	return false;
}

//-------------------------------------------------------------------------------------------
/**
*/
bool uiBaseMan::IsPKPending( void )
{
	int size = (int)m_vPKBool.size();

	for( int i = 1; i < size; ++i )
	{
		if( !SafePKBool( i ) )
		{
			return true;
		}
	}

	return false;
}
