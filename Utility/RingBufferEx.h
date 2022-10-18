#pragma once

namespace util
{
//=============================================================================================================================
/// RingBufferEx class
/**
	@author	Kim Min Wook < taiyo@webzen.com >
	@since	2004. 12. 29
	@remark
		- 기존의 링버퍼(RingBuffer)의 다른 기능의 링버퍼
	@note
		- 
	@history 
		- 
*/
//=============================================================================================================================
//
// Socket으로 data를 주고받기 전에 저장해두는 buffer( circular queue )
//
// m_Head==m_Tail : empty
// m_Head==m_Tail+1 : full ( m_Head > m_Tail )
// m_Head==0, m_Tail==m_BufferSize-1 : full ( m_Head < m_Tail )
// (!) m_Tail 자리는 항상 empty이다.
//
class RingBufferEx
{
public :
	RingBufferEx(int bufferSize = 0);
	virtual ~RingBufferEx();

	void	Init( int bufferSize );
	void	Release();
	void	Recycle();
	
	int		Write( const BYTE * pBuffer, int len );	// 내부 buffer에 쓴다.
	int		Read( BYTE * pBuffer, int len );			// 내부 buffer에서 읽어온다.
	int		Peek( BYTE * pBuffer, int len );			// Read인데 head, tail은 변화없다.
	int		Skip( int len );						// head, tail만 변화

	bool	IsEmpty() const			{ return m_Head == m_Tail; }
	bool	IsFull() const			{ return GetFreeSize()==0; }
	int		GetBufferSize() const	{ return m_BufferSize; }
	int		GetDataSize() const		{ return m_Head <= m_Tail? m_Tail-m_Head : m_BufferSize-m_Head+m_Tail; }
	int		GetFreeSize() const		{ return GetBufferSize() - GetDataSize() - 1; }	// -1 for tail position

	BYTE *	GetContigousPtr(OUT int & size) const;
protected :
	BYTE *		m_pBuffer;
	int			m_BufferSize;
	int			m_Head;
	int			m_Tail;
};

}
