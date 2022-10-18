//=======================================================================================================================
/// Solar Assert
/**
	@author
			Kim Min Wook < taiyo@webzen.co.kr >	
	@since
			2005. 4. 11
	@remarks
			- 커스텀한 Assert 창
	@todo
			- 컴파일 타임과 런타임용으로 나눔~!
			- 파일에 기록하거나, OutputDebugString()으로 출력하는 부분 추가
			- 가변인자 처리
*/
//=======================================================================================================================

#ifndef __SOLAR_ASSERT_H__
#define __SOLAR_ASSERT_H__

#include <windows.h>
#include <windowsx.h>

extern char * VSTR( char * s, ...);
extern int _solarAssertBox( const char * pszDesc, int line, char * file, char * date, char * time );
extern int _ignoreMessageBox();

/*
#define SASSERT( exp, desc )	\
		{	\
				static bool bIgnoreToAssertBox = false; \
				static bool bDontAskAgain = true; \
				if( exp==0 && bDontAskAgain ) \
				{ \
					if( 2 == _solarAssertBox( desc, __LINE__, __FILE__, __DATE__, __TIME__ ) ) \
					{ \
						__debugbreak(); \
					} \
					if( !bIgnoreToAssertBox ) \
					{ \
						if( IDNO == _ignoreMessageBox() ) \
						{ \
							bDontAskAgain  = false; \
						} \
					} \
				} \
		}
*/
#define SASSERT( exp, desc ) __noop


#define DASSERT( exp, desc )	\
		{ \
			if( !(exp) ) \
			{ \
				OutputDebugString( desc ); \
			} \
		}

#endif // __SOLAR_ASSERT_H__