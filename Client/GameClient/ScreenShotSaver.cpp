/**
 *	@file		ScreenShotSaver.cpp
 *	@brief		ScreenShotSaver class	
 *  @author		Unknown
 *	@version	Unknown
 */

// ScreenShotSaver.cpp

#include "SunClientPrecompiledHeader.h"
#include "ScreenShotSaver.h"
#include "InterfaceManager.h"
#include "uiLoginMan/uiLoginMan.h"
#include "globalfunc.h"

#ifdef _NA_007726_20141015_PROTECTION_SCREENSHOT
  #include "../zlib/zlibstat/zip.h"
  #include "../zlib/zlibstat/iowin32.h"
#endif _NA_007726_20141015_PROTECTION_SCREENSHOT

//------------------------------------------------------------------------------
/**
*/
ScreenShotSaver::ScreenShotSaver( void )
{
	Initialize();
}

//------------------------------------------------------------------------------
/**
*/
ScreenShotSaver::~ScreenShotSaver( void )
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
void ScreenShotSaver::Initialize( void )
{
	memset( m_fileName, 0, sizeof( m_fileName ) );
	memset( m_filePath, 0, sizeof( m_filePath ) );

	m_prepared = false;
}

//------------------------------------------------------------------------------
/**
*/
bool ScreenShotSaver::Prepare( void )
{
	Initialize();

	static int printScreenNo = 0;

	TCHAR dir[INTERFACE_STRING_LENGTH];
	TCHAR serverName[INTERFACE_STRING_LENGTH];

	const int screenShotSaveFolder = 70238;
	g_InterfaceManager.GetInterfaceString( screenShotSaveFolder, dir, INTERFACE_STRING_LENGTH );

	if( FALSE == CreateDirectory( dir, NULL ) )
	{
		//	����( ������ �̹� ������ ��쵵 ���� ó���� )
		_asm nop;
	}

	// ���� ���� ������ �˾ƿ´�.
	ZeroMemory( serverName, INTERFACE_STRING_LENGTH );

	uiLoginMan* loginMan = (uiLoginMan*)g_InterfaceManager.GetUserInterfaceManager( UIMAN_LOGIN );
	if( loginMan )
	{
		AuthSetverData* pServerData = loginMan->GetCurrentServerData();
		if (pServerData)
		{
			StrCopy( serverName, pServerData->m_ServerName.c_str() );
		}
		else
		{
			StrCopy( serverName, _T( "Test" ) );
		}
	}

	// ���� ��¥ �� �ð��� ���ؿ´�.
	SYSTEMTIME sysTime;
    util::TimeSync::GetLocalTime( &sysTime );

	//	������ �̹� �ִ� �� �˻�
	while( printScreenNo < 10000 )
	{
		// Screen(�����-�ú�)-�Ϸù�ȣ[��������]
		if( _tcslen( serverName ) > 0 )
		{
			Sprintf( m_fileName,
				_T("Screen(%02d%02d%02d-%02d%02d)-%03d[%s].jpg"), 
				sysTime.wYear, sysTime.wMonth, sysTime.wDay,
				sysTime.wHour, sysTime.wMinute,
				printScreenNo++, 
				serverName
				);
		}
		else
		{
			Sprintf( m_fileName,
				_T("Screen(%02d%02d%02d-%02d%02d)-%03d.jpg"), 
				sysTime.wYear%99, sysTime.wMonth, sysTime.wDay,
				sysTime.wHour, sysTime.wMinute,
				printScreenNo++
				);
		}

		Sprintf( m_filePath, _T("%s%s"), dir, m_fileName );

		HANDLE hFile = CreateFile( 
			m_filePath,				// Open MYFILE.TXT;
			GENERIC_READ,			// Open for reading
			FILE_SHARE_READ,		// Share for reading
			NULL,					// No security
			OPEN_EXISTING,			// Existing file only
			FILE_ATTRIBUTE_NORMAL,	// Normal file
			NULL );					// No template file

		if( hFile == INVALID_HANDLE_VALUE )
		{
			RenderNoticeMessage( true );
			m_prepared = true;

			return true;
		}
	}

	RenderNoticeMessage( false );

	return false;
}

//------------------------------------------------------------------------------
/**
*/

#ifdef _NA_007726_20141015_PROTECTION_SCREENSHOT
uLong filetime(
               const char *f,                /* name of file to get info on */
               tm_zip *tmzip,             /* return value: access, modific. and creation times */
               uLong *dt              /* dostime */
               )
{
    int ret = 0;
    {
        FILETIME ftLocal;
        HANDLE hFind;
        WIN32_FIND_DATA  ff32;

        hFind = FindFirstFile(f,&ff32);
        if (hFind != INVALID_HANDLE_VALUE)
        {
            FileTimeToLocalFileTime(&(ff32.ftLastWriteTime),&ftLocal);
            FileTimeToDosDateTime(&ftLocal,((LPWORD)dt)+1,((LPWORD)dt)+0);
            FindClose(hFind);
            ret = 1;
        }
    }
    return ret;
}
bool ScreenShotSaver::Save( void )
{
    const TCHAR* zip_password = _T("WebzeN1@#4");

    if( m_prepared )
    {
        m_prepared = false;

        assert( g_pSunRenderer );
        bool result = (g_pSunRenderer->SaveScreen( m_filePath ) == TRUE);

        if (result == true)
        {
            TCHAR temp_image_name[MAX_PATH] = {0,};  // �ӽ÷� �����Ǵ� ����� �̹���
            TCHAR new_file_name[MAX_PATH] = {0,};   // ������ zip����

            // �ӽ� ���� ����
            ;{
                TCHAR buffer_string[MAX_PATH] = {0,};
                TCHAR* token = _T(".jpg");

                StrnCopy(buffer_string, m_filePath, MAX_PATH-1);
                TCHAR* file_name = _tcstok(buffer_string, token);
                Sprintf(new_file_name, "%s%s", file_name, _T("[lock].zip"));

                StrnCopy(buffer_string, m_fileName, MAX_PATH-1);
                file_name = _tcstok(buffer_string, token);
                Sprintf(temp_image_name, "%s%s", file_name, _T("[lock].jpg"));
                
                CopyFile(m_filePath, temp_image_name, false);
            };

            // crc
            unsigned long CRC = crc32(0L, Z_NULL, 0);
            ;{
                FILE* pfile = fopen(temp_image_name,"rb");
                if (pfile == NULL)
                {
                    return false;
                }

                size_t nRead;
                unsigned char Buffer[10000];

                while((nRead = fread(Buffer, 1, sizeof(Buffer), pfile)) > 0)
                {
                    CRC = crc32(CRC, Buffer, nRead);
                }
                fclose(pfile);
            };

            // ����
            zlib_filefunc_def file_func;
            fill_win32_filefunc(&file_func);
            zipFile zip_file = zipOpen2(new_file_name, 0, NULL, &file_func);

            if (zip_file == NULL)
            {
                return false;
            }

            TCHAR buffer[4086];
            int err = ZIP_OK;
            size_t size_read;
            zip_fileinfo file_info = {0,};

            filetime(temp_image_name, &file_info.tmz_date, &file_info.dosDate);

            err = zipOpenNewFileInZip3(zip_file, temp_image_name,&file_info,
                NULL,0,NULL,0,NULL /* comment*/,
                Z_DEFLATED,
                Z_DEFAULT_COMPRESSION, 0,
                /* -MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY, */
                -MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY,
                zip_password, CRC);

            if (err != ZIP_OK)
            {
                zipClose(zip_file, NULL);
                DeleteFile(temp_image_name);
                return false;
            }

            FILE* file_pointer = fopen(temp_image_name,"rb");
            if (file_pointer == NULL)
            {
                zipCloseFileInZip(zip_file);
                zipClose(zip_file, NULL);
                DeleteFile(temp_image_name);
                return false;
            }

            do
            {
                err = ZIP_OK;
                size_read = fread(buffer, 1, sizeof(buffer), file_pointer);
                if (size_read < sizeof(buffer))
                {
                    if (feof(file_pointer) == 0)
                    {
                        err = ZIP_ERRNO;
                    }
                }

                if (size_read > 0)
                {
                    err = zipWriteInFileInZip(zip_file, buffer, size_read);
                }
            } while ((err == ZIP_OK) && (size_read > 0));

            fclose(file_pointer);
            zipCloseFileInZip(zip_file);
            zipClose(zip_file, NULL);

            // �ӽ� ���� ����
            DeleteFile(temp_image_name);
        }

        return result;
    }

    return false;
}
#else //_NA_007726_20141015_PROTECTION_SCREENSHOT
bool ScreenShotSaver::Save( void )
{
    if( m_prepared )
    {
        m_prepared = false;

        assert( g_pSunRenderer );
        return ( g_pSunRenderer->SaveScreen( m_filePath ) == TRUE );
    }

    return false;
}
#endif //_NA_007726_20141015_PROTECTION_SCREENSHOT


//------------------------------------------------------------------------------
/**
*/
void ScreenShotSaver::RenderNoticeMessage( bool prepared )
{
	TCHAR message[INTERFACE_STRING_LENGTH] = {0, };

	if( prepared )
	{
		// ä��â�� �����ߴٴ� �޽��� ���
		TCHAR buf[INTERFACE_STRING_LENGTH];

		const int screenShotSaveMessage = 70237;
		g_InterfaceManager.GetInterfaceString( screenShotSaveMessage, buf, INTERFACE_STRING_LENGTH );

		Sprintf( message, buf, m_fileName );
	}
	else
	{
		const int screenShotSaveError = 70236;
		g_InterfaceManager.GetInterfaceString( screenShotSaveError, message, INTERFACE_STRING_LENGTH );
	}

	if( _tcslen( message ) > 0 )
	{
		GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_CHAT, message );
	}
}

//------------------------------------------------------------------------------
/**
	���� �ڵ�
*/
void ScreenShotSaver::SaveNow( void )
{
	static int siPrintScreenNum = 0;

	TCHAR	szMessage[INTERFACE_STRING_LENGTH], 
		szFileName[INTERFACE_STRING_LENGTH], 
		szDir[INTERFACE_STRING_LENGTH], 
		szString[INTERFACE_STRING_LENGTH],
		szServerName[INTERFACE_STRING_LENGTH];

	const int	c_iScreenShotSaveFolder = 70238;
	g_InterfaceManager.GetInterfaceString( c_iScreenShotSaveFolder, szDir, INTERFACE_STRING_LENGTH );

	if( FALSE == CreateDirectory( szDir, NULL ) )
	{
		//	����( ������ �̹� ������ ��쵵 ���� ó���� )
		_asm nop;
	}

	// ���� ���� ������ �˾ƿ´�.
	ZeroMemory(szServerName, INTERFACE_STRING_LENGTH);

	uiLoginMan * pLoginMan = (uiLoginMan *)
		g_InterfaceManager.GetUserInterfaceManager(UIMAN_LOGIN);
	if ( pLoginMan )
	{
		AuthSetverData* pServerData = pLoginMan->GetCurrentServerData();
		if (pServerData)
		{
			StrCopy( szServerName, pServerData->m_ServerName.c_str() );
		}
		else
		{
			_stprintf(szServerName,_T("Test"));			
		}
	}

	// ���� ��¥ �� �ð��� ���ؿ´�.

	SYSTEMTIME	SysTime;
		
	util::TimeSync::GetLocalTime( &SysTime );

	//	������ �̹� �ִ� �� �˻�
	while( true )
	{
		if( siPrintScreenNum < 10000 )
		{
			// Screen(�����-�ú�)-�Ϸù�ȣ[��������]
			if ( _tcslen( szServerName ) )
			{
				Sprintf( szFileName,
					_T("Screen(%02d%02d%02d-%02d%02d)-%03d[%s].jpg"), 
					SysTime.wYear, SysTime.wMonth, SysTime.wDay,
					SysTime.wHour, SysTime.wMinute,
					siPrintScreenNum++, 
					szServerName
					);
			}
			else
			{
				Sprintf( szFileName,
					_T("Screen(%02d%02d%02d-%02d%02d)-%03d.jpg"), 
					SysTime.wYear%99, SysTime.wMonth, SysTime.wDay,
					SysTime.wHour, SysTime.wMinute,
					siPrintScreenNum++
					);
			}

			Sprintf( szMessage, _T("%s%s"), szDir, szFileName );

			HANDLE hFile = CreateFile(	szMessage,   // Open MYFILE.TXT;
				GENERIC_READ,           // Open for reading
				FILE_SHARE_READ,        // Share for reading
				NULL,                   // No security
				OPEN_EXISTING,          // Existing file only
				FILE_ATTRIBUTE_NORMAL,  // Normal file
				NULL);                  // No template file

			if (hFile == INVALID_HANDLE_VALUE)
			{
				//	������ ����! �׷� �� �̸����� ����!
				g_pSunRenderer->SaveScreen( szMessage );

				//	ä��â�� �����ߴٴ� �޽��� ���
				const int	c_iScreenShotSaveMessage = 70237;
				g_InterfaceManager.GetInterfaceString( c_iScreenShotSaveMessage, szString, INTERFACE_STRING_LENGTH );
				Sprintf( szMessage, szString, szFileName );
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
				break;
			}
		}
		else
		{
			//	��~ ��ũ���� ���� ������ 10000�� ��á��. ��¼��?
			const int	c_iScreenShotSaveError = 70236;
			g_InterfaceManager.GetInterfaceString( c_iScreenShotSaveError, szMessage, INTERFACE_STRING_LENGTH );
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
			break;
		}
	}
}
