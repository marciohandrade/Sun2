/**
 *	@file		GameStruct.h
 *	@brief		Ŭ���̾�Ʈ������ ����ϴ� struct ���� ��� ���°�.
 *  @author		Unknown
 *	@version	Unknown
 */

#ifndef     __GAMESTRUCT_H__
#define     __GAMESTRUCT_H__
////------------------------------------------------------------------------------
///**
//    Ŭ���̾�Ʈ������ ����ϴ� struct ���� ��� ���°�.
//
//    @author
//        ���翵, ��â��
//    @since
//        [09/24/2005] WordWrapper Ŭ���� �߰�
//        [09/14/2005] ���� ���� ��� �ļ��� ���õ� ����ü�� �߰�
//        [09/06/2005] ���� ���� ����ü �߰�
//        [07/14/2005]
//    @remarks
//*/

//------------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// �÷� ���̺� ����
//-----------------------------------------------------------------------------
struct sColorInfo
{
	BYTE	R;
	BYTE	G;
	BYTE	B;
	BYTE	A;
};


// ��Ʈ ���� ��Ʈ��Ʈ
struct SortQuestListForGroupOps
{
	// �ڵ������ ��������.
	bool operator()( CODETYPE a, CODETYPE b ) const
	{
		return a < b;
	}
};

//-----------------------------------------------------------------------------
//		Float Rect	-	���� �ڷ����� long �� �ƴ� float
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
/** ���콺 ���� ���� 
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

    ���� ������ ���� Ŭ����

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

    - �����Ͽ� �����ϴ� ���ڿ� Ŭ����
    - ����� �ϳ��� Ŭ������ ����
*/
class Wrapped
{
public:
    /** ������
        @param string  ���� ���ڿ�
        @param size    ä������ ���� ��
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
			pstr[size - 1] = 0; // �޸� ���� �� ������ �� ����
		}
		else
		{
			pstr = NULL;
		}

      
    }
    /** �ı���
        @note ������ �Ҵ��ߴ�, ���ڿ� �޸� ����!!
    */
    ~Wrapped()
    {
		if (pstr) {
			delete [] pstr;
			pstr = NULL;
		}
    }

    /// ���ڿ� ��ȯ
    const char *    Get();

private:
    /// ���� ���ڿ� ������
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
