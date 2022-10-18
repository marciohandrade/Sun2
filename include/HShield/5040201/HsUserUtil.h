/*********************************************************************************
		Administartors�׷� ������ ���� �������� ������� - HsUserUtil.h
					HsUserUtil V1.0 for Windows 2000, XP, 2003
							(��) ��ö�� ������

	��  �� :
	��  �� : Administartors�׷� ������ ���� �������� �� �������� ����
*********************************************************************************/

#ifndef _HSUSERUTIL_H
#define _HSUSERUTIL_H

//ERROR CODE 
#define	HSUSERUTIL_ERR_OK						0x00000000		//����
#define	HSUSERUTIL_ERR_UNKNOWN					0x0005A001		//�˼����� ����
#define	HSUSERUTIL_ERR_NOT_ADMIN				0x0005A002		//Administrator ������ �ƴ�
#define	HSUSERUTIL_ERR_NOT_NT					0x0005A003		//NT�迭�� �ƴ�
#define	HSUSERUTIL_ERR_LOADDLL_FAIL				0x0005A004		//DLL���� �ε� ����
#define	HSUSERUTIL_ERR_DELSHADOWACNT_FAIL		0x0005A005		//��������� ���ϻ��� ����
#define	HSUSERUTIL_ERR_DELHIDEIDINFO_FAIL		0x0005A006		//�α׿½� �������� ���߱�� ���� �������� ����
#define	HSUSERUTIL_ERR_DELSHADOWACNTINFO_FAIL	0x0005A007		//������������� ��������
#define	HSUSERUTIL_ERR_ADDSHADOWACNT_FAIL		0x0005A008		//��������� ��������
#define	HSUSERUTIL_ERR_SHADOWACNT_NOT_EXIST		0x0005A009		//��������� �������� ����


#define	HSUSERUTIL_ERR_SETFLDRPERMISSION_FAIL	0x0005A10A		//���������ֱ� ����
#define	HSUSERUTIL_ERR_GETGROUPSID_FAIL			0x0005A10B		//�׷� SID ��� ����
#define	HSUSERUTIL_ERR_GETSECINFO_FAIL			0x0005A10C		//SD�� DACL���� ������
#define	HSUSERUTIL_ERR_ADDNEWACE_FAIL			0x0005A10D		//�� ACE ���� ����
#define	HSUSERUTIL_ERR_ADDNEWDACL_FAIL			0x0005A10E		//�� DACL ���� ����
#define	HSUSERUTIL_ERR_COPYOLDDACL_FAIL			0x0005A10F		//�� DACL�� ����DACL ���� ����
#define	HSUSERUTIL_ERR_ADDNEWACETONEWDACL_FAIL	0x0005A110		//�� DACL�� �� ACE �߰� ����
#define	HSUSERUTIL_ERR_GETVOLUMEINFO_FAIL		0x0005A111		//����̺� VOLUME INFO��� ����.



/*
	@param
		void

	@retval
		DWORD : ERROR_SUCCESS = ����
				�׿��� ���ϰ� = ����

	@remarks
		�Ϲ� ����� �������� ���������� ���ӹ� �ٽ��� ���ȱ���� �����ϱ� ���� �ٽ��� ������
		�����մϴ�.

		�ش� �Լ� ȣ���� ���������� �Ϸ�Ǿ�� �Ϲ� ����� ���������� ������ 
		�����ϰ� ���� ��ŷ ��� ����� ������ �� �ֽ��ϴ�.
		�� ��, �ش� �Լ��� ������ �������� ���� ���α׷�, Install-Shield, ���� ��ó���� ���α׷����� 
		ȣ���մϴ�.
*/
DWORD   __stdcall _AhnHsUserUtil_CreateUser ( );


/*
	@param
		LPTSTR : ������ �ο��� ���

	@retval
		DWORD : ERROR_SUCCESS = ����
				�׿��� ���ϰ� = ����
	@remarks
		���� ���α׷��� NTFS ������ ��ġ�� ��쿡�� �ش� ������ ���ؼ� �Ϲ� ����� �������δ� 
		���� ���� ������ ���� ������ ���������� �������� ���� �� �ֽ��ϴ�. 
		���� ��� ���� ��⿡ ���� ������Ʈ ���α׷��� ����Ǿ ������ ��ġ�� ������ 
		�ֽ� ������ ���� ���ϴ� ��Ȳ�� �߻��Ѵٵ���, ���� ���� �߿� ���� �����͸� 
		�����ϴµ� ������ ���� �ֽ��ϴ�. 

		�Ϲ� ����� ������ ���Ͽ� ���� ��ġ ������ ���� ���� ������ �ο��Ϸ��� 
		�ش� �Լ��� ��������ν� NTFS ���� ������ �ο��� �� �ֽ��ϴ�
*/
DWORD	__stdcall _AhnHsUserUtil_SetFolderPermission(LPTSTR szPath);



/*
	@param
		void

	@retval
		DWORD : ERROR_SUCCESS = ����
				�׿��� ���ϰ� = ����
	@remarks
		�Ϲ� ����� �������� ���������� ���ӹ� �ٽ��� ���ȱ���� �����ϱ� ���� �����
		���� ������ �����ϸ�, ���� ���Žÿ� �ش��Լ��� ȣ���ؾ� �մϴ�.
*/
DWORD	__stdcall _AhnHsUserUtil_DeleteUser ( );


/*
	@param
		void

	@retval
		DWORD : ERROR_SUCCESS = ADMIN �����̰ų�, ������ ������ �����ϴ� ���.
				�׿��� ���ϰ� = ����
	@remarks
		���� �α��ε� ������ �ٽ��带 ������ �� �ִ� ������ ������ �ִ��� Ȯ���ϴ� �Լ��Դϴ�. 
*/
DWORD	__stdcall _AhnHsUserUtil_IsEnableHSAdminRights ( );


/*
	@param
		void

	@retval
		DWORD : ERROR_SUCCESS = �ٽ��� ������ ������ �����ϴ� ���.
				HSUSERUTIL_ERR_SHADOWACNT_NOT_EXIST = �ٽ��� ������ ������ �������� �ʴ� ���.
				�׿��� ���ϰ� = ����.
	@remarks
		���� OS �� �ٽ��� ������ ������ �����ϴ��� Ȯ���ϴ� �Լ��Դϴ�.
*/
DWORD	__stdcall _AhnHsUserUtil_CheckHSShadowAccount( );


/*
	@param
		void

	@retval
		BOOL : TRUE  = ADMIN ������ ������  �ִ� ���.
			   FALSE = ���� ������ ������ ���� ���� ���.
	@remarks
	   ���� �α��ε� ������ ���� ������ ������ �ִ��� Ȯ���ϴ� �Լ��Դϴ�.
		
*/
BOOL	__stdcall _AhnHSUserUtil_IsAdmin ();




#endif //_HSUSERUTIL_H
