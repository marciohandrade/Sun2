#pragma once

#include "ObjKeyGenerator.h"

class SlotKeyGenerator
{
public:
	SlotKeyGenerator( void );
	~SlotKeyGenerator( void );

	void Init( void );
	void Release( void );

	DWORD GetKey( void );
	void Restore( DWORD key );

private:
	//util::CObjKeyGenerator<DWORD>* m_keyGenerator;
    DWORD current_max_key_;
    std::deque<DWORD> restored_key;
};

extern SlotKeyGenerator	g_SlotKeyGenerator;