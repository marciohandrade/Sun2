/*!	
 *	\file		TextDisaplyManager.h
 *	\brief		�ð��� ������ ���̵�ƿ� �ؽ�Ʈ�� �����ϴ� �޴���
 *	\author		���ǥ
 *	\todo		���׸��ϰ� �ٲ� �ʿ� ����.
 *	\version	2009/02/02  ���ǥ (lammy@webzen.co.kr) - doxygen �ּ� �߰�
 *	\version	2008/12/12  ���ǥ (lammy@webzen.co.kr) - first release
 */

#ifndef __TEXT_DISPLAY_MANAGER_H__
#define __TEXT_DISPLAY_MANAGER_H__


#include <boost/shared_ptr.hpp>

/*!	
 *	\class		Text_Member	class
 *	\brief		�ؽ�Ʈ ������
 *	\author		���ǥ
 */

class Text_Member
{
public:
	/*!	
	 *	������	
	 *  \param[in]	str		����� �ؽ�Ʈ
	 */
	Text_Member( std::string str );
	/*!	
	*	������	
	*  \param[in]	str		����� �ؽ�Ʈ
	*  \param[in]	Color	�ؽ�Ʈ ����
	*/
	Text_Member( std::string str, WzColor Color );
	/// �Ҹ���
	~Text_Member() {}
protected:
	/// �⺻������ ��
	Text_Member() {}

public:
	/*!	
	 *	��� �Լ�
	 *  \param[in]	dwTick	�ð�ƽ
	 */
	BOOL	Process( DWORD dwTick );

	///	���÷��� �ð� ����	
	void	SetDisplayTime( DWORD time ) { m_dwDisplayTime = time; }
	///	���İ��� ����
	BYTE    GetAlpha() const { return m_Alpha; }
	///	��Ʈ���� ����
	char *  GetString() { return const_cast<char*>( m_str.c_str() ); }
	/// ��Ʈ�� ���� ����
	size_t  GetStringSize() { return m_str.size(); }

	WzColor	GetColor() { return m_Color; }

private:
	/// ���ڸ� ������ ��Ʈ��
	std::string		m_str;
	///	���÷��� �ð�
	DWORD			m_dwDisplayTime;
	/// ����� �ð�
	DWORD			m_dwDisappearTime;
	///	����������� üũ
	BOOL			m_bProcess;
	///	����� �������� üũ
	BOOL			m_bDisappear;
	///	��Ʈ�� ���İ�
	BYTE			m_Alpha;
	/// �ؽ�Ʈ ����
	WzColor			m_Color;
};

typedef boost::shared_ptr<Text_Member>	TEXT_MEMBER_SHARED_PTR_TYPE;

/*!	
 *	\class		TextDisplayManager class
 *	\brief		�ؽ�Ʈ �����͸� �����ϴ� �޴���
 *	\author		���ǥ
 */
class TextDisplayManager
{
	/// �ؽ�Ʈ�� �����ϴ� ����Ʈ
	typedef	std::list<TEXT_MEMBER_SHARED_PTR_TYPE >				TEXT_MEMBER_LIST_TYPE;
	typedef	std::list<TEXT_MEMBER_SHARED_PTR_TYPE >::iterator	TEXT_MEMBER_LIST_ITR_TYPE;
public:
    enum
    {
        DEFAULT_HEIGHT_POS = 62,
    };

public:
    /// ������
    TextDisplayManager();
    /*!	
    *	������	
    *  \param[in]   iHeightPosition ����� �ؽ�Ʈ
    *  \param[in]	FontID          �ؽ�Ʈ ��Ʈ
    */
    TextDisplayManager( int iHeightPosition, WzID FontID );
	///	��� �Լ�
	BOOL	Process( DWORD dwTick );
	///	�ؽ�Ʈ�� ������ �Ѵ�.
	BOOL	Render();
	///	�����͸� �μ�Ʈ �Ѵ�.
	BOOL	Insert( TEXT_MEMBER_SHARED_PTR_TYPE data );

private:
	///	������ ���� ����Ʈ
	TEXT_MEMBER_LIST_TYPE	m_TextMemberList;
    ///	�ؽ�Ʈ�� ��µǴ� ���̰�
    int                     m_iHeightPosition;
    /// �ؽ�Ʈ ��Ʈ
    WzID			        m_FontID;
};


#endif