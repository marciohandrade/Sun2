// <Prototypes>
#define ERR_CPSHOW_FAULT		(-1)
#define ERR_CPSHOW_DIFFVERSION	(-2)

// <CPShow_Init>
//   초기화 루틴, 주어진 값을 통해 올바른 버전에 의해 사용되는지 확인한다.
// <returns>
//   ERR_CPSHOW_FAULT : fault (null, etc...)
//   ERR_CPSHOW_DIFFVERSION : different version
typedef INT		(*CPShow_Init)( BYTE verMAJOR, BYTE verMIDDLE, DWORD oddNOcate, DWORD oddNOproto );
// <CPShow_Query>
//   주어진 카테고리와 프로토콜을 통해 해당 메시지가 무엇인지를 나타내는 문자열을 반환한다.
// <returns>
//   NULL : 없는 정보이다.
//   !NULL : 해당 메시지 "MSG_xxx"
typedef const CHAR*	(*CPShow_Query)( BYTE byCategory, BYTE byProtocol, DWORD* dwOffset );

struct CPShowFNTable
{
	CPShow_Init		fnInit;
	CPShow_Query	fnQuery;
};

const CPShowFNTable*	GetCPShowHandler();

