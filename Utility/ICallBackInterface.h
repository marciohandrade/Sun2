#pragma once

//=======================================================================================================================
/// �ݺ� ó���� ���ϰ� ����ϱ� ���� �߻� Ŭ����
/**
	@author
			Kim Min Wook < taiyo@webzen.com >	
	@since
			2006. 2. 8
	@remarks
			- 
			- 
	@note
			- 
	@history
			- 
*/
//=======================================================================================================================

template< class OBJECT_TYPE >
class ICallBackInterface
{
	typedef OBJECT_TYPE tObject;
	typedef void (OBJECT_TYPE::*Func)();
public:
	explicit ICallBackInterface( const tObject & rT, Func func ):m_pT(&rT),m_cbFunc(func){}
	virtual ~ICallBackInterface(void);

	inline void SetFunc( Func func ) { m_cbFunc = func; }
	inline void operator()() const { m_pT->m_cbFunc(); }
private:
	const tObject *	m_pT;
	Func			m_cbFunc;
};
