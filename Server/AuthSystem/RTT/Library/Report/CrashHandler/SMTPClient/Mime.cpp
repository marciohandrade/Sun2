#include <stdio.h>
#include <Windows.h>
#include "Mime.h"

Mime::Mime()
{
	strcpy( m_defaultBoundary, CreateMimeBoundary() );
}

Mime::~Mime()
{
}

char* Mime::CreateMimeBoundary()
{
	static int	count = 0;

	sprintf( m_boundry, "== Webzen_1_Next_%d ==", ++count );

	return m_boundry;
}

char* Mime::GetContentType( const char* extension )
{
	strcpy( m_contentType, "application/octet-stream" );

	if( extension == 0 )
		return m_contentType;

	if( stricmp( extension, "GIF" ) == 0 || stricmp( extension, "JPG" ) == 0 ||
		stricmp( extension, "JPEG" ) == 0 || stricmp( extension, "BMP" ) == 0 ||
		stricmp( extension, "TIF" ) == 0 || stricmp( extension, "TIFF" ) == 0 )
		sprintf( m_contentType, "image/%s", extension );
	else if( stricmp( extension, "TXT" ) == 0 )
		strcpy( m_contentType, "text/plain" );
	else if( stricmp( extension, "HTM" ) == 0 || stricmp( extension, "HTML" ) == 0 )
		strcpy( m_contentType, "text/html" );
	else if( stricmp( extension, "MPG" ) == 0 || stricmp( extension, "MPEG" ) == 0 ||
			stricmp( extension, "MPE" ) == 0 )
		strcpy( m_contentType, "video/mpeg" );
	else if( stricmp( extension, "WAV" ) == 0 )
		strcpy( m_contentType, "audio/x-wav" );
	else if( stricmp( extension, "RM" ) == 0 || stricmp( extension, "RAM" ) == 0 ||
			stricmp( extension, "RA" ) == 0 )
		strcpy( m_contentType, "audio/x-pn-realaudio" );
	else if( stricmp( extension, "AVI" ) == 0 )
		strcpy( m_contentType, "audio/x-msvideo" );
	else if( stricmp( extension, "SND" ) == 0 )
		strcpy( m_contentType, "audio/basic" );
	else if( stricmp( extension, "MOV" ) == 0 )
		strcpy( m_contentType, "audio/quicktime" );
	else if( stricmp( extension, "MP3" ) == 0 || stricmp( extension, "MP2" ) == 0 )
		strcpy( m_contentType, "audio/mpeg" );
	else if( stricmp( extension, "ZIP" ) == 0 )
		strcpy( m_contentType, "application/x-zip-compressed" );
	else if( stricmp( extension, "PDF" ) == 0 )
		strcpy( m_contentType, "application/pdf" );
	else if( stricmp( extension, "RTF" ) == 0 )
		strcpy( m_contentType, "application/rtf" );
	else if( stricmp( extension, "GZ" ) == 0 )
		strcpy( m_contentType, "application/x-gunzip" );
	else if( stricmp( extension, "XLS" ) == 0 )
		strcpy( m_contentType, "application/vnd.ms-excel" );
	else if( stricmp( extension, "DOC" ) == 0 )
		strcpy( m_contentType, "application/msword" );
	else if( stricmp( extension, "PPT" ) == 0 )
		strcpy( m_contentType, "application/vnd.ms-powerpoint" );
		
	return m_contentType;
}