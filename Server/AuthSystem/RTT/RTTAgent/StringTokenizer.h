#pragma  once

#include "String.h"

#define  MaxDelimiterCount		64

class StringTokenizer
{
public:
			StringTokenizer();
			StringTokenizer( LPCTSTR breakup, LPCTSTR delimiter );
	virtual	~StringTokenizer();

			void		setBreakupString( LPCTSTR breakup, LPCTSTR delimiter );
			String		nextToken();

	inline	bool		hasMoreTokens()			{	return ( nextDelimiterIndex < delimiterIndexCount - 1 );	}

private:
			void		breakupParsing();
			void		offsetNextDelimiter();


			String		breakupString;
			String		delimiterString;

			int			nextDelimiterIndex;
			int			delimiterIndex[MaxDelimiterCount];
			int			delimiterIndexCount;
};