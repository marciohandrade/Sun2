#pragma once


#if defined(KISSAPI_EXPORTS)
#define	KISSAPI		__declspec(dllexport)
#else
#define	KISSAPI		__declspec(dllimport)
#endif


// Const Define
enum eKISSApiConsts
{
	KAC_NUM_QUEUE_SIZE = 512,		// ���� Queue�� ũ��
	KAC_STR_QUEUE_SIZE = 512,		// ���� Queue�� ũ��
	KAC_MAX_STR_LEN = 512,			// ���ڿ� �ִ� ����
	KAC_HEART_BEAT = 1,				// ���񽺰� ������� �ǹ��ϴ� �ĺ���
	KAC_CONC_USER_COUNT = 2			// ���� ����� ����� ��(����) �ĺ��� (Concurrent User Count)
};

const float MEANINGLESS_VALUE = -99999.0f;


enum eNotifyInfoType
{
	KAC_NOTI_TYPE_NUMERIC,
	KAC_NOTI_TYPE_STRING,
};


// Notify info structures
#pragma pack(push, 1)

typedef struct _Notify_Info_Num
{
	SYSTEMTIME	registered_time;					// ��Ͻð�(local time��)
	int			activity_id;
	DWORD		port_no;
	float		max_val;
	float		min_val;
	float		val;								// MEANINGLESS_VALUE �� �ǹ̾��� ����.
}
NOTIFY_INFO_NUM, *LPNOTIFY_INFO_NUM;

typedef struct _Notify_Info_StrA
{
	SYSTEMTIME	registered_time;					// ��Ͻð�(local time��)
	int			activity_id;
	char		content[KAC_MAX_STR_LEN + 1];
}
NOTIFY_INFO_STRA, *LPNOTIFY_INFO_STRA;

typedef struct _Notify_Info_StrW
{
	SYSTEMTIME	registered_time;					// ��Ͻð�(local time��)
	int			activity_id;
	wchar_t		content[KAC_MAX_STR_LEN + 1];
}
NOTIFY_INFO_STRW, *LPNOTIFY_INFO_STRW;

#pragma pack(pop)


typedef void	(*LPFN_GET_NOTIFY_INFO)(const int _nType, void* _pData);


typedef	UINT	(*LPFN_OPEN)();
typedef	void	(*LPFN_CLOSE)();
typedef void	(*LPFN_RESET)();

typedef	bool	(*LPFN_IS_OPENED)();

typedef	DWORD	(*LPFN_NOTIFY_NUMBER)(const LPNOTIFY_INFO_NUM _lpInfo);
typedef	DWORD	(*LPFN_NOTIFY_STRINGA)(const LPNOTIFY_INFO_STRA _lpInfo);
typedef	DWORD	(*LPFN_NOTIFY_STRINGW)(const LPNOTIFY_INFO_STRW _lpInfo);

typedef	DWORD	(*LPFN_GET_NUMBER)(LPNOTIFY_INFO_NUM _lpInfo);
typedef	DWORD	(*LPFN_GET_STRINGW)(LPNOTIFY_INFO_STRW _lpInfo);

typedef struct _Main_Handle
{
	LPFN_OPEN							lpfnOpen;
	LPFN_CLOSE							lpfnClose;
	LPFN_RESET							lpfnReset;

	LPFN_IS_OPENED						lpfnIsOpened;

	LPFN_NOTIFY_NUMBER					lpfnNotifyNumber;
	LPFN_NOTIFY_STRINGA					lpfnNotifyStringA;
	LPFN_NOTIFY_STRINGW					lpfnNotifyStringW;

	LPFN_GET_NUMBER						lpfnGetNumber;
	LPFN_GET_STRINGW					lpfnGetStringW;
}
MAIN_HANDLE, *LPMAIN_HANDLE;


typedef LPMAIN_HANDLE	(WINAPI *LPFN_GET_HANDLE)();


#ifdef _cplusplus
extern "C"
{
#endif	// _cplusplus


	KISSAPI	LPMAIN_HANDLE GetHandle();


#ifdef _cplusplus
}
#endif	// _cplusplus
