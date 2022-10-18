/*!	
 *	\file		TextDisaplyManager.h
 *	\brief		시간이 지나면 페이드아웃 텍스트를 관리하는 메니져
 *	\author		김경표
 *	\todo		제네릭하게 바꿀 필요 있음.
 *	\version	2009/02/02  김경표 (lammy@webzen.co.kr) - doxygen 주석 추가
 *	\version	2008/12/12  김경표 (lammy@webzen.co.kr) - first release
 */

#ifndef __TEXT_DISPLAY_MANAGER_H__
#define __TEXT_DISPLAY_MANAGER_H__


#include <boost/shared_ptr.hpp>

/*!	
 *	\class		Text_Member	class
 *	\brief		텍스트 데이터
 *	\author		김경표
 */

class Text_Member
{
public:
	/*!	
	 *	생성자	
	 *  \param[in]	str		출력할 텍스트
	 */
	Text_Member( std::string str );
	/*!	
	*	생성자	
	*  \param[in]	str		출력할 텍스트
	*  \param[in]	Color	텍스트 색상
	*/
	Text_Member( std::string str, WzColor Color );
	/// 소멸자
	~Text_Member() {}
protected:
	/// 기본생성자 블럭
	Text_Member() {}

public:
	/*!	
	 *	계산 함수
	 *  \param[in]	dwTick	시간틱
	 */
	BOOL	Process( DWORD dwTick );

	///	디스플레이 시간 세팅	
	void	SetDisplayTime( DWORD time ) { m_dwDisplayTime = time; }
	///	알파값을 리턴
	BYTE    GetAlpha() const { return m_Alpha; }
	///	스트링을 리턴
	char *  GetString() { return const_cast<char*>( m_str.c_str() ); }
	/// 스트링 길이 리턴
	size_t  GetStringSize() { return m_str.size(); }

	WzColor	GetColor() { return m_Color; }

private:
	/// 문자를 저장할 스트링
	std::string		m_str;
	///	디스플레이 시간
	DWORD			m_dwDisplayTime;
	/// 사라질 시간
	DWORD			m_dwDisappearTime;
	///	계산중인지를 체크
	BOOL			m_bProcess;
	///	사라질 것인지를 체크
	BOOL			m_bDisappear;
	///	스트링 알파값
	BYTE			m_Alpha;
	/// 텍스트 색상
	WzColor			m_Color;
};

typedef boost::shared_ptr<Text_Member>	TEXT_MEMBER_SHARED_PTR_TYPE;

/*!	
 *	\class		TextDisplayManager class
 *	\brief		텍스트 데이터를 관리하는 메니져
 *	\author		김경표
 */
class TextDisplayManager
{
	/// 텍스트를 관리하는 리스트
	typedef	std::list<TEXT_MEMBER_SHARED_PTR_TYPE >				TEXT_MEMBER_LIST_TYPE;
	typedef	std::list<TEXT_MEMBER_SHARED_PTR_TYPE >::iterator	TEXT_MEMBER_LIST_ITR_TYPE;
public:
    enum
    {
        DEFAULT_HEIGHT_POS = 62,
    };

public:
    /// 생성자
    TextDisplayManager();
    /*!	
    *	생성자	
    *  \param[in]   iHeightPosition 출력할 텍스트
    *  \param[in]	FontID          텍스트 폰트
    */
    TextDisplayManager( int iHeightPosition, WzID FontID );
	///	계산 함수
	BOOL	Process( DWORD dwTick );
	///	텍스트를 렌더링 한다.
	BOOL	Render();
	///	데이터를 인서트 한다.
	BOOL	Insert( TEXT_MEMBER_SHARED_PTR_TYPE data );

private:
	///	데이터 관리 리스트
	TEXT_MEMBER_LIST_TYPE	m_TextMemberList;
    ///	텍스트가 출력되는 높이값
    int                     m_iHeightPosition;
    /// 텍스트 폰트
    WzID			        m_FontID;
};


#endif