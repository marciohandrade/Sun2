/*!
 *	\file		HackShield.h
 *	\brief		Hackshield wrapper 파일
 *	\author		김경표( lammy@webzen.co.kr )
 *	\version	2009/02/11	doxygen 커맨트 작성
 */
#ifdef	__NA000000_070913_HACKSHIELD_INSTALLATION__
#ifndef	__HACK_SHIELD_H__
#define __HACK_SHIELD_H__

#define HSUPCHK_INCLUDE_PATH( v )	 <HShield/##v/HSUpChk.h>

#include <windows.h>
#include HSUPCHK_INCLUDE_PATH( HACKSHIELD_VERSION )

#if (HACKSHIELD_VERSION >= 5040501) && (WZENGINEVER >= 300)
    #define _HACKSHIELD_CHECK_TIMER_API_FUNCTION_   // 함수포인터 변조 체크
#endif //(HACKSHIELD_VERSION >= 5040501) && (WZENGINEVER >= 300)

class HackShield : public Singleton<HackShield>
{
    enum
    {
        kMinute_Wait_Time = 60 * 1000,
    };
public:
    HackShield();
    ~HackShield();

    /*!
    *	핵쉴드 관련 모듈을 초기화
    *	\return	성공	TRUE
    *	\return 실패	FALSE
    */ 
    BOOL HS_Init();
    /*!
    *	핵쉴드 서비스를 시작
    *	\return	성공	TRUE
    *	\return 실패	FALSE
    */ 
    BOOL HS_StartService();
    /*!
    *	핵쉴드 서비스를 종료
    *	\return	성공	TRUE
    *	\return 실패	FALSE
    */ 
    BOOL HS_StopService();
    /*!
    *	모듈을 릴리즈 한다.
    *	\return	성공	TRUE
    *	\return 실패	FALSE
    */ 
    BOOL HS_UnInit();
    /*!
    *   호환성모드인지 체크 함수 - 호환성 모드로 돌아가면 안됨
    *   \return 호환성모드      TRUE
    *   \return 호환성모드아님  FALSE
    */ 
    BOOL CheckCompatibilityMode();
    /*!
    *	핵쉴드용 메시지 박스. 엔진이 로드되기전과 후 지원
    *	\param[in]	szMessage	메시지내용
    */
    static void HSMessageBox( LPCTSTR szMessage );
    static int __stdcall HS_CallbackProc(long lCode, long lParamSize, void * pParam);
    static LONG WINAPI Game_UnhandledExceptionHandler(struct _EXCEPTION_POINTERS* lpTopLevelExcptionFilter);
    static LRESULT CALLBACK MessageBoxProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

private:
    TCHAR title_[MAX_PATH];
    DWORD check_variable_crc_;

    static TCHAR error_message_[MAX_PATH];
};

#endif //__HACK_SHIELD_H__
#endif //__NA000000_070913_HACKSHIELD_INSTALLATION__