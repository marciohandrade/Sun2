
#include "DBDecryptFileReader.h"
#include <stdio.h>

#ifdef _DEBUG
#	pragma comment(lib, "TheMOLA_d.lib" )
#else
#	pragma comment(lib, "TheMOLA.lib" )
#endif


struct SArgument
{
	CHAR*	pFileName;
	union
	{
		DWORD	dwKey;
		CHAR	_tKey[4];
	};
	CHAR	pID[MAX_ID_SIZE+1];
	CHAR	pPwd[MAX_PWD_SIZE+1];
};

static VOID	Usage();
static BOOL	ParseArgument( SArgument& rArg, CHAR* argv[] );

int main(int argc, CHAR* argv[])
{
	SArgument rArg;
	if( !(argc > 2) || !ParseArgument( rArg, argv ) )
	{
		Usage();
		return -1;
	}

	DBDecryptFileReader* pDBReader = new DBDecryptFileReader();

	if( !pDBReader->ReadStart( rArg.pFileName, rArg._tKey ) )
	{
		printf( "Can't start encrypt\n" );
		return -1;
	}
/*
	if( !pDBReader->GetIdentification( (BYTE*)rArg.pID ) )
	{
		printf( "Can't encrypt\n" );
		return -1;
	}

	printf( "ID %s\n", rArg.pID );

	if( !pDBReader->GetPassword( (BYTE*)rArg.pPwd ) )
	{
		printf( "Can't encrypt\n" );
		return -1;
	}

	printf( "Pwd %s\n" , rArg.pPwd );
*/
	for( int i=1 ; i<(4+1) ; ++i )
	{
		printf( "ID: %s\n", pDBReader->GetIdentification(i) );
		printf( "PWD: %s\n", pDBReader->GetPassword(i) );
	}

	pDBReader->ReadEnd();

	delete pDBReader;

	printf( "SUCCESS! read file %s\n", rArg.pFileName );

	return 0;
}

static BOOL	ParseArgument( SArgument& rArg, CHAR* argv[] )
{
	CHAR* pFileName = argv[1];
	CHAR* pKey = argv[2];

	if( strlen(pKey) < 1 || strlen(pFileName) < 1 )
		return FALSE;

	//printf( "%s\t%s\n", pKey, pFileName );

	rArg.pFileName = pFileName;
	memcpy( rArg._tKey, pKey, 4 );

	return TRUE;
}

static VOID	Usage()
{
	printf( "encoder.exe [FileName] [KEY]\n" );
}

