#ifndef __MOUSE_H__
#define __MOUSE_H__


//=============================================================================================================================
///  시스템 마우스 이벤트 처리 클래스
/**
	@author	Kim Min Wook < taiyo@webzen.co.kr >
	@since	2004. 1. 14
	@note
		- 매번 winproc() 호출시 마우스 이벤트 갱신
	@todo
		- 중요 이벤트 캡쳐 혹은 버퍼링이 필요
		- direct mouse buffered mode를 사용하는 방법도 고려.
		- 메세지 후킹에 대비해야 함! ( 오토 마우스 )
*/
//=============================================================================================================================


#pragma once

#include <Singleton.h>


struct sMOUSEINFO
{
	int nX;
	int nY;
	int nEvent;
	int nKey;
};


#define MAX_QUESIZE 16

enum eMouseEvent
{
	eME_LDOWN,
	eME_RDOWN,
	eME_MDOWN,
	eME_LUP,
	eME_RUP,
	eME_MUP,
	eME_LDBL,
	eME_RDBL,
	eME_MDBL,
	eME_COUNT,
	eME_NONE,
};


class Mouse  : public util::Singleton<Mouse>
{
public:
	Mouse();
	~Mouse();

	VOID			Init( HWND hWnd );
	VOID			Release();

	VOID			MouseMessageProc( HWND hWnd, DWORD wmsg, WPARAM wParam, LPARAM lParam );

	VOID			GetNextMouseEvent(  HWND hWnd, sMOUSEINFO * pMouseInfo );
	BOOL			IsEmptyMouseEvent() { return m_nTail == m_nHead ? TRUE : FALSE; }

	VOID			SetPoint(POINT & pt);
	VOID			GetPoint();

	POINT           ConvertPoint(POINT pt);

	POINT			CheckMousePos();


	void			SetScreenRes( float fWidth, float fHeight ) { m_fScreenWidthRes = fWidth; m_fScreenHeightRes = fHeight; }


	void			SetMoveLevel(int iLevel);
	int				GetMoveLevel();
	void			SetMoveRange(int min , int max);
	sScope			GetMoveRange();
	int				GetMoveRangeMax();
	int				GetMoveRangeMin();


	inline int		GetMouseX()				{ return m_nMouseX;					}
	inline int		GetMouseY()				{ return m_nMouseY;					}
    inline POINT    GetMousePoint()         { POINT pt; pt.x = m_nMouseX; pt.y = m_nMouseY; return pt; }      
	inline BOOL		LButtonDown()			{ return m_bMouseEvent[eME_LDOWN];	}
	inline BOOL		RButtonDown()			{ return m_bMouseEvent[eME_RDOWN];	}
	inline BOOL		MButtonDown()			{ return m_bMouseEvent[eME_MDOWN];	}
	inline BOOL		LButtonUp()				{ return m_bMouseEvent[eME_LUP];	}
	inline BOOL		RButtonUp()				{ return m_bMouseEvent[eME_RUP];	}
	inline BOOL		MButtonUp()				{ return m_bMouseEvent[eME_MUP];	}
	inline BOOL		LButtonDoubleClick()	{ return m_bMouseEvent[eME_LDBL];	}
	inline BOOL		RButtonDoubleClick()	{ return m_bMouseEvent[eME_RDBL];	}
	inline BOOL		MButtonDoubleClick()	{ return m_bMouseEvent[eME_MDBL];	}
	inline int		GetMouseEventX()		{ return m_nMouseEventX;			}
	inline int		GetMouseEventY()		{ return m_nMouseEventY;			}
	inline int		GetMouseAxisX()			{ return m_nMouseAxisX;				}
	inline int		GetMouseAxisY()			{ return m_nMouseAxisY;				}
	inline BOOL		LButtonPressed()		{ return m_bLBtnPress;				}
	inline BOOL		RButtonPressed()		{ return m_bRBtnPress;				}
	inline BOOL		MButtonPressed()		{ return m_bMBtnPress;				}
	inline BOOL		LButtonDrag()			{ return m_bLBtnDrag;				}
	inline BOOL		RButtonDrag()			{ return m_bRBtnDrag;				}
	inline BOOL		MButtonDrag()			{ return m_bMBtnDrag;				}
	inline int		GetWheel()				{ return m_nWheel;					}

	inline BOOL		CtrlPressed()			{ return m_nCombineKey & MK_CONTROL;}
	inline BOOL		ShiftPressed()			{ return m_nCombineKey & MK_SHIFT;	}

protected:

	VOID			RecordMouseEvent( HWND hWnd, int nEvent, WPARAM wParam, LPARAM lParam );
	VOID			ParseCurMouseEvent( sMOUSEINFO* pMouseInfo );
	BOOL			PushMouseInfo( sMOUSEINFO* pIn );
	BOOL			PopMouseInfo( sMOUSEINFO* pOut );

	void			SetSystemMoveSpeed(int iLevel);
	int				GetSystemMoveSpeed();
	int				GetUserDefaultMoveLevel(int value);


private:

	float							m_fScreenWidthRes;
	float							m_fScreenHeightRes;

	sMOUSEINFO*						m_pMouseInfo;
	sMOUSEINFO						m_CurMouseInfo;
	int								m_nHead;
	int								m_nTail;
	int								m_nQueSize;



	BOOL							m_bMouseEvent[eME_COUNT];

	BOOL							m_bLBtnPress;
	BOOL							m_bRBtnPress;
	BOOL							m_bMBtnPress;
	BOOL							m_bLBtnDrag;
	BOOL							m_bRBtnDrag;
	BOOL							m_bMBtnDrag;

	int								m_nMouseX;
	int								m_nMouseY;
	int								m_nMouseOldX;
	int								m_nMouseOldY;
	int								m_nMouseEventX;
	int								m_nMouseEventY;

	int								m_nMouseAxisX;
	int								m_nMouseAxisY;

	int								m_nWheelMsg;
	int								m_nWheel;

	int								m_nCombineKey;
	BOOL							m_bInited;
	POINT							m_Point;

	int								m_iMoveSpeed;
	int								m_iMoveLevel;
	int								m_iBackupMoveSpeed;
	sScope							m_MoveScrollRange;

};


#endif // __MOUSE_H__
