#ifndef __HSUSERUTILDLL_H__
#define __HSUSERUTILDLL_H__

#ifdef HSUSERUTILDLL_EXPORTS
#define HSUSERUTILDLL_API __declspec(dllexport)
#else
#define HSUSERUTILDLL_API __declspec(dllimport)
#endif


// error codes
#define HSUSERUTIL_ERR_CREATE_USER_FAILED				0x01
#define HSUSERUTIL_ERR_SET_FOLDER_PERMISSION_FAILED		0x02
#define HSUSERUTIL_ERR_DELETE_USER_FAILED				0x03
#define HSUSERUTIL_ERR_ENABLE_HSADMIN_RIGHTS			0x04
#define HSUSERUTIL_ERR_HSUSER_ACCOUNT_NOT_EXIST			0x05

// exported functions

#ifdef __cplusplus
extern "C" {
#endif



	
/*
	@param
		void

	@retval
		DWORD : ERROR_SUCCESS = ����
				HSUSERUTIL_ERR_CREATE_USER_FAILED = ����

	@remarks
		�ش� �Լ� ȣ���� ���������� �Ϸ�Ǿ�� �Ϲ� ����� ���������� ������ 
		�����ϰ� ���� ��ŷ ��� ����� ������ �� �ֽ��ϴ�.
		�� ��, �ش� �Լ��� ������ �������� ���� ���α׷�, Install-Shield, ���� ��ó���� ���α׷����� 
		ȣ���մϴ�.
*/
HSUSERUTILDLL_API  
DWORD __stdcall HsUserUtil_ExecUserCreate();

/*
	@param
		LPTSTR : ������ �ο��� ���

	@retval
		DWORD : ERROR_SUCCESS = ����
				HSUSERUTIL_ERR_SET_FOLDER_PERMISSION_FAILED = ����
	@remarks
		���� ���α׷��� NTFS ������ ��ġ�� ��쿡�� �ش� ������ ���ؼ� �Ϲ� ����� �������δ� 
		���� ���� ������ ���� ������ ���������� �������� ���� �� �ֽ��ϴ�. 
		���� ��� ���� ��⿡ ���� ������Ʈ ���α׷��� ����Ǿ ������ ��ġ�� ������ 
		�ֽ� ������ ���� ���ϴ� ��Ȳ�� �߻��Ѵٵ���, ���� ���� �߿� ���� �����͸� 
		�����ϴµ� ������ ���� �ֽ��ϴ�. 

		�Ϲ� ����� ������ ���Ͽ� ���� ��ġ ������ ���� ���� ������ �ο��Ϸ��� 
		�ش� �Լ��� ��������ν� NTFS ���� ������ �ο��� �� �ֽ��ϴ�
*/
HSUSERUTILDLL_API 
DWORD __stdcall HsUserUtil_ExecSetFolderPermissionA(LPSTR szInstallDirA);

HSUSERUTILDLL_API 
DWORD __stdcall HsUserUtil_ExecSetFolderPermissionW(LPWSTR szInstallDirW);
/*
	@param
		void

	@retval
		DWORD : ERROR_SUCCESS = ����
				HSUSERUTIL_ERR_DELETE_USER_FAILED = ����
	@remarks
		�Ϲ� ����� �������� ���������� ���ӹ� �ٽ��� ���ȱ���� �����ϱ� ���� �����
		���� ������ �����ϸ�, ���� ���Žÿ� �ش��Լ��� ȣ���ؾ� �մϴ�.
*/
HSUSERUTILDLL_API 
DWORD __stdcall HsUserUtil_ExecUserDelete();

/*
	@param
		void

	@retval
		DWORD : ERROR_SUCCESS = ADMIN �����̰ų�, ������ ������ �����ϴ� ���.
				HSUSERUTIL_ERR_ENABLE_HSADMIN_RIGHTS = ����
	@remarks
		���� ������ �ٽ��带 ������ �� �ִ� ������ ������ �ִ��� Ȯ���ϴ� �Լ��Դϴ�. 
*/
HSUSERUTILDLL_API 
DWORD __stdcall HsUserUtil_IsEnableHSAdminRights();


/*
	@param
		void

	@retval
		DWORD : ERROR_SUCCESS = �ٽ��� ������ ������ �����ϴ� ���.
				HSUSERUTIL_ERR_HSUSER_ACCOUNT_NOT_EXIST = �ٽ��� ������ ������ �������� �ʴ� ���.
				�׿��� ���ϰ� = ����.
	@remarks
		���� OS �� �ٽ��� ������ ������ �����ϴ��� Ȯ���ϴ� �Լ��Դϴ�.
*/
HSUSERUTILDLL_API 
DWORD	__stdcall HsUserUtil_CheckHSShadowAccount( );


/*
	@param
		void

	@retval
		BOOL : TRUE  = ADMIN ������ ������  �ִ� ���.
			   FALSE = ���� ������ ������ ���� ���� ���.
	@remarks
	   ���� �α��ε� ������ ���� ������ ������ �ִ��� Ȯ���ϴ� �Լ��Դϴ�.
		
*/
HSUSERUTILDLL_API 
BOOL __stdcall HsUserUtil_IsAdmin();



#ifdef __cplusplus
}
#endif


#endif
