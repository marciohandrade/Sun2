#include "stdafx.h"
#include "String.h"

String::String()
{
}

String::String( const String& source )
{
	stringBuffer = source.stringBuffer;
}

String::String( LPTSTR str )
{
	stringBuffer = str;
}

String::String( LPCTSTR str )
{
	stringBuffer = str;
}

String::String( int n, bool hex )
{
	TCHAR temp[1024] = {0, };

	if( hex == true )	_stprintf( temp, _T( "%x" ), n );
	else				_stprintf( temp, _T( "%d" ), n );

	stringBuffer += temp;
}

String::~String()
{
}

void String::format( LPTSTR dest, LPCTSTR buffer, ... )
{
	va_list	args;

	va_start( args, buffer );
	_vstprintf( dest, buffer, args );
	va_end( args );
}

void String::format( LPCTSTR buffer, ... )
{
	TCHAR	temp[4096] = {0, };
	va_list	args;

	va_start( args, buffer );
	_vstprintf( temp, buffer, args );
	va_end( args );

	stringBuffer = temp;
}

String& String::left( int count )
{
	stringBuffer.resize( count );

	return *this;
}

String& String::right( int count )
{
	if( stringBuffer.length() - count <= 0 )
		return *this;

	stringBuffer = std::string( stringBuffer, stringBuffer.length() - count );

	return *this;
}

String& String::makeUpper()
{
	TCHAR temp[4096] = {0, };

	_tcscpy( temp, stringBuffer.data() );
	_tcsupr( temp );

	stringBuffer = temp;

	return *this;
}

String& String::makeLower()
{
	TCHAR temp[4096] = {0, };

	_tcscpy( temp, stringBuffer.data() );
	_tcslwr( temp );

	stringBuffer = temp;

	return *this;
}

/////////////////////////////////////////////////////////////////
/////////////////////////  Operator =   /////////////////////////
const String& String::operator = ( String& str )
{
	stringBuffer = str.stringBuffer;

	return *this;
}

const String& String::operator = ( LPTSTR str )
{
	stringBuffer = str;

	return *this;
}

const String& String::operator = ( LPCTSTR str )
{
	stringBuffer = str;

	return *this;
}

/////////////////////////////////////////////////////////////////
/////////////////////////  Operator +=  /////////////////////////

const String& String::operator += ( String& str )
{
	stringBuffer += str.stringBuffer;

	return *this;
}

const String& String::operator += ( LPTSTR str )
{
	stringBuffer += str;

	return *this;
}

const String& String::operator += ( LPCTSTR str )
{
	stringBuffer += str;

	return *this;
}

/////////////////////////////////////////////////////////////////
/////////////////////////  Operator ==  /////////////////////////

bool operator == ( String& str1, String& str2 )
{
	return ( str1.stringBuffer == str2.stringBuffer );
}

bool operator == ( String& str1, LPTSTR str2 )
{
	return ( str1.stringBuffer == str2 );
}

bool operator == ( String& str1, LPCTSTR str2 )
{
	return ( str1.stringBuffer == str2 );
}

bool operator == ( LPTSTR str1, String& str2 )
{
	return ( str2.stringBuffer == str1 );
}

bool operator == ( LPCTSTR str1, String& str2 )
{
	return ( str2.stringBuffer == str1 );
}


/////////////////////////////////////////////////////////////////
/////////////////////////  Operator !=  /////////////////////////

bool operator != ( String& str1, String& str2 )
{
	return ( str1.stringBuffer != str2.stringBuffer );
}

bool operator != ( String& str1, LPTSTR str2 )
{
	return ( str1.stringBuffer != str2 );
}

bool operator != ( String& str1, LPCTSTR str2 )
{
	return ( str1.stringBuffer != str2 );
}

bool operator != ( LPTSTR str1, String& str2 )
{
	return ( str2.stringBuffer != str1 );
}

bool operator != ( LPCTSTR str1, String& str2 )
{
	return ( str2.stringBuffer != str1 );
}

/////////////////////////////////////////////////////////////////
/////////////////////////  Operator +   /////////////////////////
String operator + ( String& ls, String& rs )
{
	String value( ls );

	value += rs;

	return value;
}

String operator + ( String& ls, LPTSTR rs )
{
	String value( ls );

	value += rs;

	return value;
}

String operator + ( String& ls, LPCTSTR rs )
{
	String value( ls );

	value += rs;

	return value;
}

String operator + ( LPTSTR ls, String& rs )
{
	String value( ls );

	value += rs;

	return value;
}

String operator + ( LPCTSTR ls, String& rs )
{
	String value( ls );

	value += rs;

	return value;
}