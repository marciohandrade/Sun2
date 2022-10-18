#pragma once

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<External Includes>
//
//#pragma message("     _Adding library: zlib.lib (Please read License)" ) 
#pragma comment(lib, "../../lib/"VC_DIR_REDIRECT"/zlib.lib")

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<CompressStreamIO>
//
class CompressStreamIO
{
public:
	enum eQUERY_RESULT {
		QR_ERR_DEFAULT			= 0,
		QR_ERR_SUCCESS			= 1,
		QR_ERR_FILE_CONTROL		,
		QR_ERR_CANT_LOAD_FILE	,
		QR_ERR_BUFFER_PTR		,
		QR_ERR_BUFFER_SIZE		,
		QR_ERR_COMPRESS			,
		QR_ERR_UNCOMPRESS		,
	};
	struct sSTREAM_INFO
	{
		sSTREAM_INFO() : STREAM(0), SIZE(0) {}

		BYTE*		STREAM;
		DWORD		SIZE;
	};
	typedef sSTREAM_INFO					STREAM_INFO_NODE;

public:		//	<ResourceAlloc>
	//		filename : file name for load
	//		rInfo.STREAM : loaded file stream
	//		rInfo.SIZE : loaded file stream size
public:		static BOOL						ResourceAlloc( const CHAR* filename, STREAM_INFO_NODE& OUT rInfo );
			//	<ResourceFree>
			//		if( rINFO.STREAM && rINFO.SIZE ) delete memory
			static BOOL						ResourceFree( const STREAM_INFO_NODE& IN rInfo );
			//	<Compress>
			//		rPlainInfo.STREAM		:[IN] source plain stream
			//		rPlainInfo.SIZE			:[IN] source plain stream size
			//		rCompressInfo.STREAM	:[OUT] dest compressed stream ptr
			//		rCompressInfo.SIZE		:[OUT] dest compressed stream size (fix)
			static BOOL						Compress( const sSTREAM_INFO& rPlainInfo, sSTREAM_INFO& OUT rCompressInfo );
			//	<Compress>
			//		rCompressInfo.STREAM	:[IN] source compressed stream ptr
			//		rCompressInfo.SIZE		:[IN] source compressed stream size (fix)
			//		rPlainInfo.STREAM		:[OUT] dest plain stream (NULL)
			//		rPlainInfo.SIZE			:[IN:OUT] dest plain stream size
			//			(값이 있어야 한다. plain stream으로 복원시 할당될 크기,
			//			내부적으로 null-terminate고려 크기 재 할당 후 복원)
			static BOOL						Uncompress( const sSTREAM_INFO& rCompressInfo, sSTREAM_INFO& INOUT rPlainInfo );
private:	inline static DWORD				_CalcCompressingSafeBufferSize( const DWORD sourceLen );
};

