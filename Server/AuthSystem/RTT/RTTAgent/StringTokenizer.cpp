#include "stdafx.h"

#pragma warning(push)
#pragma warning(disable:4702)

#include <vector>
#include <algorithm>
#include <functional>

#pragma warning(pop)

#include "StringTokenizer.h"


StringTokenizer::StringTokenizer()
	: nextDelimiterIndex( 0 ), delimiterIndexCount( 0 )
{
	memset(&delimiterIndex[0], 0, sizeof(int) * MaxDelimiterCount);
}

StringTokenizer::StringTokenizer( LPCTSTR breakup, LPCTSTR delimiter )
	: breakupString( breakup ), delimiterString( delimiter ),
	nextDelimiterIndex( 0 ), delimiterIndexCount( 0 )
{
	memset(&delimiterIndex[0], 0, sizeof(int) * MaxDelimiterCount);
	breakupParsing();
}

StringTokenizer::~StringTokenizer()
{
}

void StringTokenizer::setBreakupString( LPCTSTR breakup, LPCTSTR delimiter )
{
	breakupString		= breakup;
	delimiterString		= delimiter;
	nextDelimiterIndex	= 0;
	delimiterIndexCount	= 0;

	breakupParsing();
}

void StringTokenizer::breakupParsing()
{
	std::vector	< int >				container;
	int								offset = 0;

	//  Make delimiter index list and Sorting.
	int delimiterCount	= delimiterString.length();
	for(int i = 0 ; i < delimiterCount ; i++ )
	{
		offset = 0;
		while( offset != -1 )
		{
			offset = breakupString.find( delimiterString[i], offset );
			container.push_back( offset );

			if( offset != -1 )
				offset++;
		}
	}

	std::sort( container.begin(), container.end(), std::less< int >() );

	//  Make sorted delimiter index.
	::memset( delimiterIndex, 0, sizeof( int ) * MaxDelimiterCount );

	nextDelimiterIndex	= 0;
	delimiterIndexCount	= 1;
	delimiterIndex[0]	= -1;
	for(std::vector	< int >::iterator iter = container.begin() ; iter != container.end() ; iter++ )
	{
		if( *iter == -1 )
			continue;

		delimiterIndex[delimiterIndexCount] = *iter;
		delimiterIndexCount++;
	}

	if( delimiterIndex[delimiterIndexCount - 1] < breakupString.length() - 1 )
		delimiterIndex[delimiterIndexCount++] = breakupString.length();


	offsetNextDelimiter();
}

void StringTokenizer::offsetNextDelimiter()
{
	//  Offset array index of nextDelimiterIndex
	for(int i = nextDelimiterIndex ; i < delimiterIndexCount - 1 ; i++ )
	{
		if( delimiterIndex[i + 1] - delimiterIndex[i] == 1 )
			nextDelimiterIndex++;
		else
			break;
	}
}

String StringTokenizer::nextToken()
{
	String	token( breakupString );

	if( hasMoreTokens() == false )
		return _T( "" );

	int startIndex	= delimiterIndex[nextDelimiterIndex] + 1;
	int endIndex	= delimiterIndex[nextDelimiterIndex + 1];
	nextDelimiterIndex++;

	if( startIndex == endIndex )
		return nextToken();

	int length = token.left( endIndex ).length();
	token.right( length - startIndex );

	offsetNextDelimiter();

	return token;
}