/*!
 *	\file		HackShield.h
 *	\brief		Hackshield wrapper ����
 *	\author		���ǥ( lammy@webzen.co.kr )
 *	\version	2009/02/11	doxygen Ŀ��Ʈ �ۼ�
 */
#ifdef	__NA000000_070913_HACKSHIELD_INSTALLATION__
#ifndef	__HACK_SHIELD_H__
#define __HACK_SHIELD_H__

#define HSUPCHK_INCLUDE_PATH( v )	 <HShield/##v/HSUpChk.h>

#include <windows.h>
#include HSUPCHK_INCLUDE_PATH( HACKSHIELD_VERSION )

#if (HACKSHIELD_VERSION >= 5040501) && (WZENGINEVER >= 300)
    #define _HACKSHIELD_CHECK_TIMER_API_FUNCTION_   // �Լ������� ���� üũ
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
    *	�ٽ��� ���� ����� �ʱ�ȭ
    *	\return	����	TRUE
    *	\return ����	FALSE
    */ 
    BOOL HS_Init();
    /*!
    *	�ٽ��� ���񽺸� ����
    *	\return	����	TRUE
    *	\return ����	FALSE
    */ 
    BOOL HS_StartService();
    /*!
    *	�ٽ��� ���񽺸� ����
    *	\return	����	TRUE
    *	\return ����	FALSE
    */ 
    BOOL HS_StopService();
    /*!
    *	����� ������ �Ѵ�.
    *	\return	����	TRUE
    *	\return ����	FALSE
    */ 
    BOOL HS_UnInit();
    /*!
    *   ȣȯ��������� üũ �Լ� - ȣȯ�� ���� ���ư��� �ȵ�
    *   \return ȣȯ�����      TRUE
    *   \return ȣȯ�����ƴ�  FALSE
    */ 
    BOOL CheckCompatibilityMode();
    /*!
    *	�ٽ���� �޽��� �ڽ�. ������ �ε�Ǳ����� �� ����
    *	\param[in]	szMessage	�޽�������
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