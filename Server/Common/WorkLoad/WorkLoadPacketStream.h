#pragma once

#include "WorkLoad.Common.h"

class WorkLoadPacketStream
{
public:
	// (���� �ð�)���� �����Ǵ� ��Ŷ ���۷� üũ���� (���� �ð�)
	static const DWORD c_DefaultUpdateTick = 15;
	static const DWORD c_DefaultLimitTransferSize = 20000;	// BYTE
public:
	WorkLoadPacketStream() :
		m_dwStackedTick(0),
		m_dwStackedTransferSize(0),
		m_dwUpdateTick(c_DefaultUpdateTick),
		m_dwLimitTransferSize(c_DefaultLimitTransferSize)
		{ }

	~WorkLoadPacketStream() { }

	inline VOID			Initialize( const DWORD dwUpdateTick, const DWORD dwLimitTransferSize )
	{
		m_dwUpdateTick = dwUpdateTick;
		m_dwLimitTransferSize = dwLimitTransferSize;
	}

	inline VOID			AddTransferSize( const DWORD dwTransferSize )
	{
		m_dwStackedTransferSize += dwTransferSize;
	}

	// <return>
	inline BOOL			CheckOverflow()
	{
		BOOL bRet = FALSE;
		DWORD dwCurTick = GetTickCount();
		if( m_dwStackedTick+m_dwUpdateTick < dwCurTick )
		{
			bRet = (BOOL)( m_dwStackedTransferSize > m_dwLimitTransferSize );
				
			m_dwStackedTick = dwCurTick;
			m_dwStackedTransferSize = 0;
		}
		return bRet;
	}
public:

private:
	DWORD	m_dwStackedTick;
	DWORD	m_dwStackedTransferSize;
	DWORD	m_dwUpdateTick;
	DWORD	m_dwLimitTransferSize;
};

