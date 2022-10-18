#ifndef __READSCRIPT_H__
#define __READSCRIPT_H__

#include <stdio.h>

static enum SMDToken 
{
	NAME, 
	NUMBER, 
	END, 
	COMMAND = '#',
	LP = '{',
	RP = '}',
	COMMA = ',',
	SEMICOLON = ';',
	SMD_ERROR
};
/*
static FILE     *SMDFile;
static float    TokenNumber;
static char     TokenString[100];
static SMDToken CurrentToken;

static SMDToken GetToken()
{
	char ch;
	TokenString[0] = '\0';
	do
	{
		if ( (ch =(char) fgetc(SMDFile)) == EOF) return END;
		if (ch=='/' && (ch =(char) fgetc(SMDFile) )=='/')	
		{
			while( (ch = (char) fgetc( SMDFile)) != '\n' );
		}
	} while(  isspace(ch) );
	
	char *p, TempString[100];
	switch(ch)
	{	
	case '#':
		return CurrentToken = COMMAND;
	case ';':
		return CurrentToken = SEMICOLON;
	case ',':
		return CurrentToken = COMMA;
	case '{':
		return CurrentToken = LP;
	case '}':
		return CurrentToken = RP;
	case '0':	case '1':	case '2':	case '3':	case '4':
	case '5':	case '6':	case '7':	case '8':	case '9':
	case '.':	case '-':
		ungetc(ch,SMDFile);
		p = TempString;
		while ( (  (ch = getc(SMDFile) ) !=EOF) && (ch=='.' || isdigit(ch) || ch=='-') )
			*p++ = ch;
		*p = 0;
		TokenNumber = (float)atof(TempString);
		//			sscanf(TempString," %f ",&TokenNumber);
		return CurrentToken = NUMBER;
	case '"':
		p = TokenString;
		while ( (  (ch = getc(SMDFile) ) !=EOF) && (ch!='"'))// || isalnum(ch)) )
			*p++ = ch;
		if (ch!='"')
			ungetc(ch,SMDFile);
		*p = 0;
		return CurrentToken = NAME;
	default:
		if (isalpha(ch))	
		{
			p = TokenString;
			*p++ = ch;
			while ( (  (ch = getc(SMDFile) ) !=EOF) && (ch=='.' || ch=='_' || isalnum(ch)) )
				*p++ = ch;
			ungetc(ch,SMDFile);
			*p = 0;
			return CurrentToken = NAME;
		}
		return CurrentToken = SMD_ERROR;
	}
}*/


static HANDLE SMDFile;
static float    TokenNumber;
static char     TokenString[100];
static SMDToken CurrentToken;

static char getChar(HANDLE hFile)
{
	char ch;
	DWORD dwRead;
	BOOL bResult = ReadFile(hFile, &ch, 1, &dwRead, NULL);

	if(bResult && (dwRead == 0) )
	{
		return EOF;
	}
	
	return ch;
}

static int ungetChar(char ch, HANDLE hFile)
{
	SetFilePointer(hFile, -1, NULL, FILE_CURRENT);
	return 0;
}


static SMDToken GetToken()
{
	char ch;
	TokenString[0] = '\0';
	do
	{
		if ( (ch =(char) getChar(SMDFile)) == EOF) return END;
		if (ch=='/' && (ch =(char) getChar(SMDFile) )=='/')	
		{
			while( (ch = (char) getChar( SMDFile)) != '\n' );
		}
	} while(  isspace(ch) );
	
	char *p, TempString[100];
	switch(ch)
	{	
	case '#':
		return CurrentToken = COMMAND;
	case ';':
		return CurrentToken = SEMICOLON;
	case ',':
		return CurrentToken = COMMA;
	case '{':
		return CurrentToken = LP;
	case '}':
		return CurrentToken = RP;
	case '0':	case '1':	case '2':	case '3':	case '4':
	case '5':	case '6':	case '7':	case '8':	case '9':
	case '.':	case '-':
		ungetChar(ch,SMDFile);
		p = TempString;
		while ( (  (ch = getChar(SMDFile) ) !=EOF) && (ch=='.' || isdigit(ch) || ch=='-') )
			*p++ = ch;
		*p = 0;
		TokenNumber = (float)atof(TempString);
		//			sscanf(TempString," %f ",&TokenNumber);
		return CurrentToken = NUMBER;
	case '"':
		p = TokenString;
		while ( (  (ch = getChar(SMDFile) ) !=EOF) && (ch!='"'))// || isalnum(ch)) )
			*p++ = ch;
		if (ch!='"')
			ungetChar(ch,SMDFile);
		*p = 0;
		return CurrentToken = NAME;
	default:
		if (isalpha(ch))	
		{
			p = TokenString;
			*p++ = ch;
			while ( (  (ch = getChar(SMDFile) ) !=EOF) && (ch=='.' || ch=='_' || isalnum(ch)) )
				*p++ = ch;
			ungetChar(ch,SMDFile);
			*p = 0;
			return CurrentToken = NAME;
		}
		return CurrentToken = SMD_ERROR;
	}
}

#endif