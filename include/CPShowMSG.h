// <Prototypes>
#define ERR_CPSHOW_FAULT		(-1)
#define ERR_CPSHOW_DIFFVERSION	(-2)

// <CPShow_Init>
//   �ʱ�ȭ ��ƾ, �־��� ���� ���� �ùٸ� ������ ���� ���Ǵ��� Ȯ���Ѵ�.
// <returns>
//   ERR_CPSHOW_FAULT : fault (null, etc...)
//   ERR_CPSHOW_DIFFVERSION : different version
typedef INT		(*CPShow_Init)( BYTE verMAJOR, BYTE verMIDDLE, DWORD oddNOcate, DWORD oddNOproto );
// <CPShow_Query>
//   �־��� ī�װ��� ���������� ���� �ش� �޽����� ���������� ��Ÿ���� ���ڿ��� ��ȯ�Ѵ�.
// <returns>
//   NULL : ���� �����̴�.
//   !NULL : �ش� �޽��� "MSG_xxx"
typedef const CHAR*	(*CPShow_Query)( BYTE byCategory, BYTE byProtocol, DWORD* dwOffset );

struct CPShowFNTable
{
	CPShow_Init		fnInit;
	CPShow_Query	fnQuery;
};

const CPShowFNTable*	GetCPShowHandler();

