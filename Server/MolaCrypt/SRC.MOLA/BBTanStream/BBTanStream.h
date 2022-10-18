#pragma once

union uRND_CONVKEY
{
	DWORD		KEYBLOCK;
	BYTE		RNDBLOCK[4];
	WORD		WORDBLOCK[2];
};

#define DWORD2uRND( val )		(*(uRND_CONVKEY*)&(val))

typedef BOOL	(*ConvertBBTanHandler)( uRND_CONVKEY& rKEY, BYTE* pBYTE, WORD wRemain, BYTE& rbyLinkedNo );

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朽
// <BBTanStream>
class BBTanStream
{
	typedef STLX_MAP<BYTE, ConvertBBTanHandler>	BBTAN_MAP;
	typedef BBTAN_MAP::iterator				BBTAN_MAP_IT;

public:		BBTanStream();
			~BBTanStream();

public:		VOID	Init();
			VOID	Init_T1();
			VOID	Init_T2();
			VOID	Encode( uRND_CONVKEY& rKEY, BYTE* pBYTE, WORD wMSG_BASE, BYTE& rbyLinkedNo );
			VOID	Decode( uRND_CONVKEY& rKEY, BYTE* pBYTE, WORD wMSG_BASE, BYTE& rbyLinkedNo );
			VOID	Release();
private:	VOID	Converter( BBTAN_MAP& rStarter, uRND_CONVKEY& rKEY, BYTE* pBYTE, WORD wMSG_BASE, BYTE& rbyLinkedNo );

//private:	uRND_CONVKEY	m_SenderKEY;
//			uRND_CONVKEY	m_ReceiverKEY;
private:	BBTAN_MAP		m_Sender;
			BBTAN_MAP		m_Receiver;
};


#include "BBTanStream.InterHdr.h"

#include "BBTanStream.RndList.T1.hxx"
#include "BBTanStream.RndList.T2.hxx"

//#if SERVER_CODE

#define BBTAN_STREAM_TYPE1NAME	"BBTanStream.T1.cpp"
#define BBTAN_STREAM_TYPE2NAME	"BBTanStream.T2.cpp"

//#else
//
//#define BBTAN_STREAM_TYPE1NAME	"BBTanStream.T2.cpp"
//#define BBTAN_STREAM_TYPE2NAME	"BBTanStream.T1.cpp"
//
//#endif //

#define ARRAY4_RNDIDX( val )	((val)%4)
