//------------------------------------------------------------------------------
//  SlotKeyGenerator.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "slotkeygenerator.h"

SlotKeyGenerator g_SlotKeyGenerator;

SlotKeyGenerator::SlotKeyGenerator( void )
//: m_keyGenerator( 0 )
{
	// empty
}

SlotKeyGenerator::~SlotKeyGenerator( void )
{
	//assert( !m_keyGenerator );
}

void SlotKeyGenerator::Init( void )
{
	//assert( !m_keyGenerator );

	//m_keyGenerator = new util::CObjKeyGenerator<DWORD>( 1, 100000 );
	//assert( m_keyGenerator );
    
    current_max_key_ = 1;
    restored_key.clear();
}

void SlotKeyGenerator::Release( void )
{
	//if( m_keyGenerator )
	//{
	//	delete m_keyGenerator;
	//	m_keyGenerator = NULL;
	//}
    restored_key.clear();
}

DWORD SlotKeyGenerator::GetKey( void )
{
	//assert( m_keyGenerator );

	//DWORD key = m_keyGenerator->GetKey();
	//assert( key != 0 );

    if (restored_key.empty() == false)
    {
        DWORD key = restored_key.front();
        restored_key.pop_front();

        return key;
    }
    
    DWORD key = current_max_key_++;
    if (current_max_key_ > 100000) {
        current_max_key_ = 1;
    }
    return key;    
}

void SlotKeyGenerator::Restore( DWORD key )
{
	//assert( m_keyGenerator );

    //assert( key != 0 );

	//m_keyGenerator->RestoreKey( key );
    restored_key.push_back(key);
}
