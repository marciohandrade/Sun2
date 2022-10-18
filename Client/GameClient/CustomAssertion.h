/**
 *	@file		CustomAssertion.h
 *	@brief		SUN에 특화된 Assertion
 *  @author		Lee Ji Yeon (kiki@webzen.co.kr)
 *	@version	Unknown
 */

#pragma once

#include <Singleton.h>

//---------------------------------------------------------------------------------------
//		Custom Assert
//---------------------------------------------------------------------------------------

#ifdef _DEV_VER

	#ifdef _DEBUG
		#define SUN_ASSERT(expr)			ASSERT(expr)
		#define SUN_ASSERT_MSG(expr, msg)	ASSERT(expr)
        #define ASSERTMSG(expr, msg)	    ASSERT(expr)
	#else //else _DEBUG
		#define SUN_ASSERT(expr)			if (!(expr))	CustomAssertion::Instance()->PushMsg(#expr, __FILE__, __LINE__)
		#define SUN_ASSERT_MSG(expr, msg)	if (!(expr))	CustomAssertion::Instance()->PushMsg(#expr, msg)
		#define ASSERTMSG(expr, msg)        if (!(expr))	CustomAssertion::Instance()->PushMsg(#expr, msg)
	#endif //_DEBUG

	#define MAX_ASSERTMSG_CNT	30
	class CustomAssertion : public Singleton<CustomAssertion> 
	{
	public:
		CustomAssertion()	
		{	
			m_listAssertMsg.clear(); 
			m_bRender = false;	
		}

		~CustomAssertion()	{}

		__inline void ShowAssertMssage(bool bShow)
		{
			m_bRender = bShow;
		}

		__inline void ToggleAssertMssage()
		{
			m_bRender = !m_bRender;
		}


		__inline void PushMsg(const char *pExpr, const char *pFile, DWORD line)
		{
			const char* pFileName = strrchr(pFile, '\\');
			const TCHAR *ptszLog = MakeMsg(_T("[%s] (%d) %s"), pExpr, line, pFileName);

			if(m_listAssertMsg.size() >= MAX_ASSERTMSG_CNT)
			{
				m_listAssertMsg.pop_front();
			}

			m_listAssertMsg.push_back(ptszLog);

		}

		__inline void PushMsg(const char *pExpr, TCHAR *pMsg)
		{
			const TCHAR *ptszLog = MakeMsg(_T("[%s] %s"), pExpr, pMsg);

			if(m_listAssertMsg.size() >= MAX_ASSERTMSG_CNT)
			{
				m_listAssertMsg.pop_front();
			}

			m_listAssertMsg.push_back(ptszLog);
		}


		const TCHAR* MakeMsg(const char* pszParam, ...)
		{
			static TCHAR buf[256] = {0,};
			TCHAR szTemp[256] = {0,};

			va_list va;
			va_start(va, pszParam);
			vsprintf(szTemp, pszParam, (va_list)va);
			va_end(va);


			struct tm * today;
			time_t ltime;
			time(&ltime);
			today = localtime(&ltime);

			_snprintf(buf, 255, _T("[%02d:%02d:%02d] %s"), today->tm_hour, today->tm_min, today->tm_sec, szTemp);

			return buf;
		}

		__inline void RenderAssertMessage()
		{
			if (!g_pSunRenderer)
			{
				return;
			}
			if(m_bRender)
			{
				g_pSunRenderer->x_pManagerTextOut->StartToUseFont( "m216" );
				int x = 10;
				int y = 100;
				int iLine = 0;

				g_pSunRenderer->x_pManagerTextOut->DrawText(_T("<Assert Message for DevVer.>  On/Off Key == F4"), x, y + iLine * 17, WzColor_RGB( 255, 255, 255 ));
				++iLine;

				std::list<util::_tstring>::iterator it, itEnd;
				it = m_listAssertMsg.begin();
				itEnd = m_listAssertMsg.end();

				for ( ; it != itEnd; ++it)
				{
					TCHAR *pStr = const_cast<TCHAR *>((*it).c_str());
					g_pSunRenderer->x_pManagerTextOut->DrawText(pStr, x, y + iLine * 17, WzColor_RGB( 255, 255, 255 ));
					++iLine;
				}
				g_pSunRenderer->x_pManagerTextOut->EndUseFont();
			}
		}


	private:
		std::list<util::_tstring> m_listAssertMsg;
		bool m_bRender;


	};

	

#else //_DEV_VER

	#define SUN_ASSERT(expr)            ((void)0)
	#define SUN_ASSERT_MSG(expr,msg)    ((void)0)

#endif //_DEV_VER







