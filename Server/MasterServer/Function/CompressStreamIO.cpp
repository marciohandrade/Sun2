#include "stdafx.h"
#include "CompressStreamIO.h"

#include <zlib.h>

DWORD	CompressStreamIO::_CalcCompressingSafeBufferSize( const DWORD sourceLen )
{
	//Upon entry, destLen is the total size of the destination buffer, 
	//which must be at least 0.1% larger than sourceLen plus 12 bytes.
	const DWORD calcedLen = (DWORD)(1.001f*(sourceLen+12)) + 1;
	return calcedLen;
}

BOOL	CompressStreamIO::ResourceAlloc( const CHAR* filename, STREAM_INFO_NODE& OUT rInfo )
{
	BYTE* pSTREAM = NULL;

	FlowControl flow;
	FLOWCONTROL_START()
    {
		// start LOGIC_BLOCK
		IfFailThenBreak(filename, CompressStreamIO::QR_ERR_FILE_CONTROL);

		HANDLE hFILE = INVALID_HANDLE_VALUE;
		{
			OFSTRUCT pOF[1];
			hFILE = (HANDLE)(ULONGLONG)(HFILE)OpenFile( filename, pOF, OF_READ );
		};
		IfFailThenBreak(INVALID_HANDLE_VALUE != hFILE, CompressStreamIO::QR_ERR_FILE_CONTROL);

		const DWORD fileSz = GetFileSize( hFILE, NULL );
		const DWORD allocedSz = (DWORD)(fileSz + 10);

		pSTREAM = new BYTE[allocedSz];
		ZeroMemory( pSTREAM, allocedSz );
		DWORD readedSz;
		ReadFile( hFILE, pSTREAM, allocedSz, &readedSz, NULL );
		CloseHandle( hFILE );

		IfFailThenBreak(fileSz == readedSz, CompressStreamIO::QR_ERR_CANT_LOAD_FILE);

		rInfo.STREAM = pSTREAM;
		rInfo.SIZE = readedSz;

		return TRUE;
    }
    FLOWCONTROL_END;

	ZeroMemory( &rInfo, sizeof(rInfo) );

	switch( flow.GetErrorCode() )
	{
	case CompressStreamIO::QR_ERR_CANT_LOAD_FILE:
		delete [] pSTREAM;
	case CompressStreamIO::QR_ERR_FILE_CONTROL:
	default:
		rInfo.STREAM = 0;
		rInfo.SIZE = 0;
		break;
	}
	//MessageOut( eCRITICAL_LOG, _T("%s : ERROR [%s][%u]"), flow.GetErrorString(), flow.GetErrorCode() );

	return FALSE;
}

BOOL	CompressStreamIO::ResourceFree( const STREAM_INFO_NODE& IN rInfo )
{
	if( rInfo.STREAM && rInfo.SIZE )
#ifdef NDEBUG
		delete [] rInfo.STREAM;
#else
	{
		//UINT iRange = static_cast<UINT>(rInfo.SIZE);
		//if( !IsBadWritePtr( rInfo.STREAM, (UINT_PTR)&iRange ) )
			delete [] rInfo.STREAM;
	}
#endif

	return TRUE;
}

BOOL	CompressStreamIO::Compress( const sSTREAM_INFO& rPlainInfo, sSTREAM_INFO& OUT rCompressInfo )
{
	const INT		COMPRESS_LEVEL = Z_BEST_COMPRESSION;
	BYTE* pDestPtr = NULL;

	FlowControl flow;
	FLOWCONTROL_START()
    {
		// start LOGIC_BLOCK
		IfFailThenBreak(rPlainInfo.SIZE, CompressStreamIO::QR_ERR_BUFFER_SIZE);

		DWORD destSz = _CalcCompressingSafeBufferSize( rPlainInfo.SIZE );
		pDestPtr = new BYTE[destSz];

//#ifdef _DEBUG
//		printf( "compress compressed %p : %u\tplaing %p : %u\n",
//			pDestPtr, destSz, rPlainInfo.STREAM, rPlainInfo.SIZE );
//#endif
		INT err = compress2( pDestPtr, &destSz, rPlainInfo.STREAM, rPlainInfo.SIZE, COMPRESS_LEVEL);
//#ifdef _DEBUG
//		printf( "compress compressed %p : %u\tplaing %p : %u\n",
//			pDestPtr, destSz, rPlainInfo.STREAM, rPlainInfo.SIZE );
//		printf( "result %u\n", err );
//
//		UINT iRange = static_cast<UINT>(destSz);
//		if( IsBadWritePtr( pDestPtr, (UINT_PTR)&iRange ) )
//			printf( "boot~\n" );
//		//HANDLE hFILE = INVALID_HANDLE_VALUE;
//		//{
//		//	OFSTRUCT pOF[1];
//		//	HANDLE hFILE = (HANDLE)(ULONGLONG)(HFILE)OpenFile( "X:\\SUN.SOLARAUTH\\Server\\_bin\\ouputscript_src.txt", pOF, OF_READWRITE|OF_CREATE );
//		//	DWORD dwWritten;
//		//	WriteFile( hFILE, pDestPtr, destSz, &dwWritten, NULL );
//		//	FlushFileBuffers( hFILE );
//		//	CloseHandle( hFILE );
//		//};
//#endif

		IfFailThenBreak(Z_OK == err, CompressStreamIO::QR_ERR_COMPRESS);

		const DWORD PADDING_SZ = 10;
		const DWORD compressedSz = destSz+PADDING_SZ;
		rCompressInfo.STREAM = new BYTE[compressedSz];
		rCompressInfo.SIZE = destSz;
		CopyMemory( rCompressInfo.STREAM, pDestPtr, sizeof(BYTE)*destSz );
		ZeroMemory( rCompressInfo.STREAM+destSz, PADDING_SZ );

		delete[] pDestPtr;

		return TRUE;
    }
    FLOWCONTROL_END;

	switch( flow.GetErrorCode() )
	{
	case CompressStreamIO::QR_ERR_COMPRESS:
	case CompressStreamIO::QR_ERR_BUFFER_SIZE:
	default:
		rCompressInfo.STREAM = 0;
		rCompressInfo.SIZE = 0;
		delete [] pDestPtr;
		break;
	}
	//MessageOut( eCRITICAL_LOG, _T("%s : ERROR [%s][%u]"), flow.GetErrorString(), flow.GetErrorCode() );

	return FALSE;
}

BOOL	CompressStreamIO::Uncompress( const sSTREAM_INFO& rCompressInfo, sSTREAM_INFO& INOUT rPlainInfo )
{
	const DWORD		PADDING_SIZE = 10;

	FlowControl flow;
	FLOWCONTROL_START()
    {
		// start LOGIC_BLOCK
		IfFailThenBreak(rCompressInfo.STREAM, CompressStreamIO::QR_ERR_BUFFER_PTR);

		IfFailThenBreak(rCompressInfo.SIZE && rPlainInfo.SIZE, CompressStreamIO::QR_ERR_BUFFER_SIZE);

		const DWORD alloced_size = rPlainInfo.SIZE+PADDING_SIZE;
		rPlainInfo.STREAM = new BYTE[alloced_size];
		ZeroMemory( rPlainInfo.STREAM, sizeof(BYTE)*alloced_size );

//#ifdef _DEBUG
//		printf( "Uncompress plain %p : %u\tcompressed %p : %u\n",
//			rPlainInfo.STREAM, rPlainInfo.SIZE, rCompressInfo.STREAM, rCompressInfo.SIZE );
//		//HANDLE hFILE = INVALID_HANDLE_VALUE;
//		//{
//		//	OFSTRUCT pOF[1];
//		//	HANDLE hFILE = (HANDLE)(ULONGLONG)(HFILE)OpenFile( "X:\\SUN.SOLARAUTH\\Server\\_bin\\ouputscript.txt", pOF, OF_READWRITE|OF_CREATE );
//		//	DWORD dwWritten;
//		//	WriteFile( hFILE, rCompressInfo.STREAM, rCompressInfo.SIZE, &dwWritten, NULL );
//		//	FlushFileBuffers( hFILE );
//		//	CloseHandle( hFILE );
//		//};
//#endif
		INT err = uncompress( rPlainInfo.STREAM, &rPlainInfo.SIZE, rCompressInfo.STREAM, rCompressInfo.SIZE);
//#ifdef _DEBUG
//		printf( "Uncompress plain %p : %u\tcompressed %p : %u\n",
//			rPlainInfo.STREAM, rPlainInfo.SIZE, rCompressInfo.STREAM, rCompressInfo.SIZE );
//		printf( "result %u\n", err );
//#endif

		IfFailThenBreak(Z_OK == err, CompressStreamIO::QR_ERR_UNCOMPRESS);

		return TRUE;
    }
    FLOWCONTROL_END;

	switch( flow.GetErrorCode() )
	{
	case CompressStreamIO::QR_ERR_BUFFER_PTR: case CompressStreamIO::QR_ERR_BUFFER_SIZE:
		break;
	case CompressStreamIO::QR_ERR_UNCOMPRESS:
		delete [] rPlainInfo.STREAM;
	default:
		break;
	}
	//MessageOut( eCRITICAL_LOG, _T("%s : ERROR [%s][%u]"), flow.GetErrorString(), flow.GetErrorCode() );

	return FALSE;
}


