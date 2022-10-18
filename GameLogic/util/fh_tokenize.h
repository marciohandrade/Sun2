/*****************************************************
fallenhobbit@gmail.com

fh_strtok(char* str, char* toks, int ln)

str is she string to be tokenized 

toks is the string of tokens 

ln is the length of str
*****************************************************/
#ifndef FH_TOKENIZE
#define FH_TOKENIZE
#include <windows.h>

//first pass, str should be the string 
//second and subsequent passes, pass NULL for str
char* fh_strtok(char* str, char* toks, int ln)
{
	static char* curpos; //static to keep track of data between calls
	static int trkindx;
	int curindx=0;
	int tkln=lstrlen(toks);
	try
	{
		if(str)
		{
			curpos=0;
			trkindx=0;
		}
		if(trkindx>=ln)//if we have loved over the whole ...ok watching to much scifi
		{
			return 0;
		}
		if(str)
		{
			curpos=str;//marks each token with a NULL
			for(int i=0; i<ln; i++)
			{
				for(int j=0;j<tkln;j++)
				{
					if(str[i]==toks[j])
						str[i]=0;
				}
			}
			return curpos;
		}
		while(1)
		{
			if(curpos[curindx]==0)//now we just loop over the string 
			{						//the static vars save each last 
				curpos+=curindx;	//place we were at so each new call gets the next 
				trkindx+=curindx;	//token
				++trkindx;
				if(trkindx<ln)
				{
 					return ++curpos;
				}
				else
				{
					return 0;
				}
			}
			++curindx;
			if(curindx==ln)
				break;
		}
	}catch(...){return (char*)-1;} //0n error either -1 or zero is returned
}

WCHAR* fh_strtokW(WCHAR* str, WCHAR* toks, int ln)
{
	static WCHAR* curpos; //static to keep track of data between calls
	static int trkindx;
	int curindx=0;
	int tkln=lstrlenW(toks);
	try
	{
		if(str)
		{
			curpos=0;
			trkindx=0;
		}
		if(trkindx>=ln)//if we have loved over the whole ...ok watching to much scifi
		{
			return 0;
		}
		if(str)
		{
			curpos=str;//marks each token with a NULL
			for(int i=0; i<ln; i++)
			{
				for(int j=0;j<tkln;j++)
				{
					if(str[i]==toks[j])
						str[i]=0;
				}
			}
			return curpos;
		}
		while(1)
		{
			if(curpos[curindx]==0)//now we just loop over the string 
			{						//the static vars save each last 
				curpos+=curindx;	//place we were at so each new call gets the next 
				trkindx+=curindx;	//token
				++trkindx;
				if(trkindx<ln)
				{
 					return ++curpos;
				}
				else
				{
					return 0;
				}
			}
			++curindx;
			if(curindx==ln)
				break;
		}
	}catch(...){return (WCHAR*)-1;} //0n error either -1 or zero is returned
}

#endif