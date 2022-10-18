#pragma once

namespace util
{
//=============================================================================================================================
/// RingBufferEx class
/**
	@author	Kim Min Wook < taiyo@webzen.com >
	@since	2004. 12. 29
	@remark
		- ������ ������(RingBuffer)�� �ٸ� ����� ������
	@note
		- 
	@history 
		- 
*/
//=============================================================================================================================
//
// Socket���� data�� �ְ�ޱ� ���� �����صδ� buffer( circular queue )
//
// m_Head==m_Tail : empty
// m_Head==m_Tail+1 : full ( m_Head > m_Tail )
// m_Head==0, m_Tail==m_BufferSize-1 : full ( m_Head < m_Tail )
// (!) m_Tail �ڸ��� �׻� empty�̴�.
//
class RingBufferEx
{
public :
	RingBufferEx(int bufferSize = 0);
	virtual ~RingBufferEx();

	void	Init( int bufferSize );
	void	Release();
	void	Recycle();
	
	int		Write( const BYTE * pBuffer, int len );	// ���� buffer�� ����.
	int		Read( BYTE * pBuffer, int len );			// ���� buffer���� �о�´�.
	int		Peek( BYTE * pBuffer, int len );			// Read�ε� head, tail�� ��ȭ����.
	int		Skip( int len );						// head, tail�� ��ȭ

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
