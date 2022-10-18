
#include "DBEncryptFileWriter.h"
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
	CHAR	pID_Dev[MAX_ID_SIZE+1];
	CHAR	pPwd_Dev[MAX_PWD_SIZE+1];
	CHAR	pID_Company[MAX_ID_SIZE+1];
	CHAR	pPwd_Company[MAX_PWD_SIZE+1];
	CHAR	pID_Live[MAX_ID_SIZE+1];
	CHAR	pPwd_Live[MAX_PWD_SIZE+1];
	CHAR	pID_TestLive[MAX_ID_SIZE+1];
	CHAR	pPwd_TestLive[MAX_PWD_SIZE+1];
};

static VOID	Usage();
static BOOL	ParseArgument( SArgument& rArg, CHAR* argv[] );

int main(int argc, CHAR* argv[])
{
	SArgument rArg;
	if( !(argc > 4) || !ParseArgument( rArg, argv ) )
	{
		Usage();
		return -1;
	}

	DBEncryptFileWriter* pDBWriter = new DBEncryptFileWriter();

	if( !pDBWriter->WriteStart( rArg.pFileName, rArg._tKey ) )
	{
		printf( "Can't start encrypt\n" );
		return -1;
	}

	//
	if( !pDBWriter->SetIdentification( (BYTE*)rArg.pID_Dev ) )
	{
		printf( "Can't encrypt\n" );
		return -1;
	}

	if( !pDBWriter->SetPassword( (BYTE*)rArg.pPwd_Dev ) )
	{
		printf( "Can't encrypt\n" );
		return -1;
	}

	//
	if( !pDBWriter->SetIdentification( (BYTE*)rArg.pID_Company ) )
	{
		printf( "Can't encrypt\n" );
		return -1;
	}

	if( !pDBWriter->SetPassword( (BYTE*)rArg.pPwd_Company ) )
	{
		printf( "Can't encrypt\n" );
		return -1;
	}

	//
	if( !pDBWriter->SetIdentification( (BYTE*)rArg.pID_Live ) )
	{
		printf( "Can't encrypt\n" );
		return -1;
	}

	if( !pDBWriter->SetPassword( (BYTE*)rArg.pPwd_Live ) )
	{
		printf( "Can't encrypt\n" );
		return -1;
	}

	//
	if( !pDBWriter->SetIdentification( (BYTE*)rArg.pID_TestLive ) )
	{
		printf( "Can't encrypt\n" );
		return -1;
	}

	if( !pDBWriter->SetPassword( (BYTE*)rArg.pPwd_TestLive ) )
	{
		printf( "Can't encrypt\n" );
		return -1;
	}

	pDBWriter->WriteEnd();
	printf( "SUCCESS! created file %s\n", rArg.pFileName );


	return 0;
}

static BOOL	ParseArgument( SArgument& rArg, CHAR* argv[] )
{
	CHAR* pFileName = argv[1];
	CHAR* pKey = argv[2];
	CHAR* pID_Dev = argv[3];
	CHAR* pPwd_Dev = argv[4];
	CHAR* pID_Company = argv[5];
	CHAR* pPwd_Company = argv[6];
	CHAR* pID_Live = argv[7];
	CHAR* pPwd_Live = argv[8];
	CHAR* pID_TestLive = argv[9];
	CHAR* pPwd_TestLive = argv[10];

	if( strlen(pKey) < 1 || strlen(pFileName) < 1 )
		return FALSE;

	int len;

	len = (int)strlen(pID_Dev);
	if( !len || !(len<16) )
		return FALSE;
	len = (int)strlen(pPwd_Dev);
	if( !len || !(len<16) )
		return FALSE;

	len = (int)strlen(pID_Company);
	if( !len || !(len<16) )
		return FALSE;
	len = (int)strlen(pPwd_Company);
	if( !len || !(len<16) )
		return FALSE;

	len = (int)strlen(pID_Live);
	if( !len || !(len<16) )
		return FALSE;
	len = (int)strlen(pPwd_Live);
	if( !len || !(len<16) )
		return FALSE;

	len = (int)strlen(pID_TestLive);
	if( !len || !(len<16) )
		return FALSE;
	len = (int)strlen(pPwd_TestLive);
	if( !len || !(len<16) )
		return FALSE;

	//printf( "%s\t%s\n", pKey, pFileName );

	rArg.pFileName = pFileName;
	memcpy( rArg._tKey, pKey, 4 );

	memcpy( rArg.pID_Dev, pID_Dev, MAX_ID_SIZE );
	rArg.pID_Dev[MAX_ID_SIZE] = '\0';
	memcpy( rArg.pPwd_Dev, pPwd_Dev, MAX_PWD_SIZE );
	rArg.pPwd_Dev[MAX_PWD_SIZE] = '\0';

	memcpy( rArg.pID_Company, pID_Company, MAX_ID_SIZE );
	rArg.pID_Company[MAX_ID_SIZE] = '\0';
	memcpy( rArg.pPwd_Company, pPwd_Company, MAX_PWD_SIZE );
	rArg.pPwd_Company[MAX_PWD_SIZE] = '\0';

	memcpy( rArg.pID_Live, pID_Live, MAX_ID_SIZE );
	rArg.pID_Live[MAX_ID_SIZE] = '\0';
	memcpy( rArg.pPwd_Live, pPwd_Live, MAX_PWD_SIZE );
	rArg.pPwd_Live[MAX_PWD_SIZE] = '\0';

	memcpy( rArg.pID_TestLive, pID_TestLive, MAX_ID_SIZE );
	rArg.pID_TestLive[MAX_ID_SIZE] = '\0';
	memcpy( rArg.pPwd_TestLive, pPwd_TestLive, MAX_PWD_SIZE );
	rArg.pPwd_TestLive[MAX_PWD_SIZE] = '\0';

	return TRUE;
}

static VOID	Usage()
{
	printf( "encoder.exe [FileName] [KEY] "
			"[DevID:16] [DevPwd:16] "
			"[CompanyID:16] [CompanyPwd:16] "
			"[LiveID:16] [LivePwd:16] "
			"[TestLiveID:16] [TestLivePwd:16] \n" );
}

