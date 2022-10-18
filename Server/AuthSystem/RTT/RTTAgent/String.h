#pragma  once

#include <Windows.h>
#include <TChar.h>
#include <string>

class String
{
public:
			String();
			String( const String& source );
			String( LPTSTR str );
			String( LPCTSTR str );
			String( int n, bool hex = false );
	virtual ~String();

	static	void		format( LPTSTR dest, LPCTSTR buffer, ... );
			void		format( LPCTSTR buffer, ... );

			String&		left( int count );
			String&		right( int count );
			String&		makeUpper();
			String&		makeLower();


	inline	int			find( TCHAR delimiter, int offset = 0 );
	inline	int			find( LPCTSTR delimiter, int offset = 0 );

	inline	TCHAR		getAt( int index )					{	return stringBuffer[index];						}
	inline	void		setAt( int index, TCHAR ch )		{	stringBuffer[index] = ch;						}

	inline	void		empty()								{	stringBuffer.erase();							}
	inline	bool		isEmpty()							{	return stringBuffer.empty();					}

	inline	int			length()							{	return ( int )stringBuffer.length();			}
	inline	LPTSTR		getBuffer()							{	return ( LPTSTR )stringBuffer.data();			}

	inline	int			compareTo( String& str )			{	return ( stringBuffer ==  str.stringBuffer ); 	}
	inline	int			compareTo( LPTSTR str )				{	return ( stringBuffer == str );					}
	inline	int			compareTo( LPCTSTR str )			{	return ( stringBuffer == str );					}
	inline				operator	LPTSTR() const			{	return ( LPTSTR )stringBuffer.data();			}


			TCHAR		operator [] ( int index ) const		{	return stringBuffer[index];						}

	const	String&		operator = ( String& str );
	const	String&		operator = ( LPTSTR str );
	const	String&		operator = ( LPCTSTR str );

	const	String&		operator += ( String& str );
	const	String&		operator += ( LPTSTR str );
	const	String&		operator += ( LPCTSTR str );


	friend	bool		operator == ( String& str1, String& str2 );
	friend	bool		operator == ( String& str1, LPTSTR str2 );
	friend	bool		operator == ( String& str1, LPCTSTR str2 );
	friend	bool		operator == ( LPTSTR str1, String& str2 );
	friend	bool		operator == ( LPCTSTR str1, String& str2 );

	friend	bool		operator != ( String& str1, String& str2 );
	friend	bool		operator != ( String& str1, LPTSTR str2 );
	friend	bool		operator != ( String& str1, LPCTSTR str2 );
	friend	bool		operator != ( LPTSTR str1, String& str2 );
	friend	bool		operator != ( LPCTSTR str1, String& str2 );

	friend	String		operator + ( String& ls, LPTSTR rs );
	friend	String		operator + ( String& ls, LPCTSTR rs );
	friend	String		operator + ( LPTSTR ls, String& rs );
	friend	String		operator + ( LPCTSTR ls, String& rs );
	friend	String		operator + ( String& ls, String& rs );

private:
			std::string	stringBuffer;
};

inline	int String::find( TCHAR delimiter, int offset )
{
	return ( int )stringBuffer.find( delimiter, offset );
}

inline	int String::find( LPCTSTR delimiter, int offset )
{
	return ( int )stringBuffer.find( delimiter, offset, _tcslen( delimiter ) );
}