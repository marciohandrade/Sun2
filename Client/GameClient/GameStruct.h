/**
 *	@file		GameStruct.h
 *	@brief		클라이언트에서만 사용하는 struct 들을 모아 놓는곳.
 *  @author		Unknown
 *	@version	Unknown
 */

#ifndef     __GAMESTRUCT_H__
#define     __GAMESTRUCT_H__
////------------------------------------------------------------------------------
///**
//    클라이언트에서만 사용하는 struct 들을 모아 놓는곳.
//
//    @author
//        유재영, 방창규
//    @since
//        [09/24/2005] WordWrapper 클래스 추가
//        [09/14/2005] 게임 서버 목록 파서와 관련된 구조체들 추가
//        [09/06/2005] 툴팁 정보 구조체 추가
//        [07/14/2005]
//    @remarks
//*/

//------------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 컬러 테이블 관련
//-----------------------------------------------------------------------------
struct sColorInfo
{
	BYTE	R;
	BYTE	G;
	BYTE	B;
	BYTE	A;
};


// 소트 관련 스트럭트
struct SortQuestListForGroupOps
{
	// 코드순으로 오름차순.
	bool operator()( CODETYPE a, CODETYPE b ) const
	{
		return a < b;
	}
};

//-----------------------------------------------------------------------------
//		Float Rect	-	내부 자료형이 long 이 아닌 float
//-----------------------------------------------------------------------------
typedef struct tagFRECT
{
	float    left;
	float    top;
	float    right;
	float    bottom;
} FRECT, *PFRECT;



#include "OptionData.h"


//----------------------------------------------------------------------------
/** 마우스 오버 관련 
*/

class SCSlot;

struct MouseOverObject
{
	MouseOverObject() : eMouseOverType(eMOUSE_OVER_MAX) 
	{}
	~MouseOverObject() {}

	eMOUSE_OVER_TYPE eMouseOverType;
	
	union 
	{
		SCSlot *	pSlot;
		int			iSkillIdx;
		void *		pETCObject;
	};

};


//------------------------------------------------------------------------------
/**
    @class cTooltipInfo

    툴팁 정보에 대한 클래스

    (C) 2005 ckbang
*/
class cTooltipInfo
{
public:
	cTooltipInfo()  :
	  m_Color( 0x00000000 ),
	  m_ShadowColor( 0x00000000 ),
	  m_fontID( 0 ),
	  m_iTextYInterval( 0 ),
	  m_dwExtraStyle( 0 ),
	  m_dwIconCode( 0 ),
	  m_eSpecialToolTip( eSPECIAL_TOOLTIP_NONE )
	{
		memset( m_szSlotInfoMessage, 0, sizeof( m_szSlotInfoMessage ) );
	}
    typedef struct _image_info
    {
        DWORD image_texture_number;
        DWORD image_frame_number;
        float width;
        float height;
        float x;
        float y;
    }IMAGE_INFO;
    cTooltipInfo(const cTooltipInfo& pTooltipInfo);

    TCHAR       m_szSlotInfoMessage[MAX_SLOTINFO_LENGTH];
    WzColor     m_Color;
    WzColor     m_ShadowColor;
    WzID        m_fontID;   // WzID
    int         m_iTextYInterval;

	DWORD		m_dwExtraStyle;

	DWORD		m_dwIconCode;
	DWORD       m_eSpecialToolTip;
    IMAGE_INFO  image_infomation;
    cTooltipInfo& operator=(const cTooltipInfo& rhs);
};

//------------------------------------------------------------------------------
/**
*/
inline
cTooltipInfo::cTooltipInfo(const cTooltipInfo& src) :
    m_Color(src.m_Color),
    m_ShadowColor(src.m_ShadowColor),
    m_fontID(src.m_fontID),
    m_iTextYInterval(src.m_iTextYInterval),
	m_dwExtraStyle( src.m_dwExtraStyle ),
	m_dwIconCode( src.m_dwIconCode ),
	m_eSpecialToolTip( src.m_eSpecialToolTip )
{
	StrCopy(this->m_szSlotInfoMessage, src.m_szSlotInfoMessage);
}

//------------------------------------------------------------------------------
/**
*/
inline
cTooltipInfo&
cTooltipInfo::operator=(const cTooltipInfo& rhs)
{
    StrCopy (this->m_szSlotInfoMessage, rhs.m_szSlotInfoMessage);
    this->m_Color = rhs.m_Color;
    this->m_ShadowColor = rhs.m_ShadowColor;
    this->m_fontID = rhs.m_fontID;
    this->m_iTextYInterval = rhs.m_iTextYInterval;
	this->m_dwExtraStyle = rhs.m_dwExtraStyle;
	m_dwIconCode = rhs.m_dwIconCode;
	m_eSpecialToolTip = rhs.m_eSpecialToolTip;

    return *this;
}

//------------------------------------------------------------------------------
/**
    @class Wrapped

    - 구분하여 적립하는 문자열 클래스
    - 개행당 하나의 클래스로 관리
*/
class Wrapped
{
public:
    /** 생성자
        @param string  목적 문자열
        @param size    채워넣을 문자 수
    */
    Wrapped(const char *string, int size)
    {
		const char *string2 = string;

		if (size > 0) {
			if (string2[0] == ' ')
			{
				string2++;
				size--;
			}
		}

		if (size > 0) {
			pstr = new char [size];
			memcpy ((char *)pstr, string2, size);
			pstr[size - 1] = 0; // 메모리 복사 후 쓰레기 값 제거
		}
		else
		{
			pstr = NULL;
		}

      
    }
    /** 파괴자
        @note 생성시 할당했던, 문자열 메모리 해제!!
    */
    ~Wrapped()
    {
		if (pstr) {
			delete [] pstr;
			pstr = NULL;
		}
    }

    /// 문자열 반환
    const char *    Get();

private:
    /// 적립 문자열 포인터
    char *pstr;
};

//------------------------------------------------------------------------------
/**
*/
inline
const char *
Wrapped::Get ()
{
    return pstr;
}

//------------------------------------------------------------------------------
#endif      // __GAMESTRUCT_H__
