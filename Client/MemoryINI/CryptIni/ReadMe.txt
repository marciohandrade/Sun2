========================================================================
                        CryptIni ������Ʈ ����
========================================================================

����: CryptIni�� �����ϰ� ��ȣȭ�� INI�� ����� �� �ִ� ����� �����ݴϴ�

����:
	���� �ڵ带 �����Ͻʽÿ�
	
	
���� API:
	
	��� API�� ���н� -1, ������ 0 Ȥ�� 0�̻��� ���� �����մϴ�
	
	;	inifile �� decrypt�Լ��� �̿��Ͽ� ��ȣȭ�Ͽ� �о� ����
	
	int	LoadFromFile( LPCTSTR inifile, INI_CRYPT decrypt );
		inifile - Ini���� ���� ���
		decrypt - �ε��ϸ鼭 ��ȣȭ�� �Լ�������
	
	
	;	inifile �� encrypt�Լ��� �̿��Ͽ� ��ȣȭ�Ͽ� ���� ��	
	
	int	SaveToFile( LPCTSTR inifile, INI_CRYPT encrypt );
		inifile - Ini���� ���� ���
		decrypt - �ε��ϸ鼭 ��ȣȭ�� �Լ�������		
		
	
	;	section �� �ش��ϴ� key �� ��(��Ʈ��)�� �о pStr�� ���� ��
	
	int	getString( LPCTSTR section, LPCTSTR key, LPTSTR pStr, DWORD slen );
		section - �о�� ���Ǹ�
		key		- �о�� Ű�̸�
		pStr	- ������ ����� ���밪 ����
		strsz	- ����� ������ ����(����: ���ڿ����̷� ������)
		
	
	;	section �� �ش��ϴ� key �� ��(����)�� �о pInt�� ���� ��

	int	getInteger( LPCTSTR section, LPCTSTR key, LONG *pInt );
		section	- �о�� ���Ǹ�
		key		- �о�� Ű�̸�
		pInt	- ����� ������ ������ ������
		

	;	section �� key �� pStr(��Ʈ��)������ ���� ��

	int	putString( LPCTSTR section, LPCTSTR key, LPCTSTR pStr );
		section - ������ ���Ǹ�
		key		- ������ Ű�̸�
		pStr	- ������ ���밪 ����


	;	section �� key �� value(����)������ ���� ��
	
	int	putInteger( LPCTSTR section, LPCTSTR key, LONG value );
		section	- ����� ���Ǹ�
		key		- ����� Ű�̸�
		value	- ����� �������� ����
	
	
����:
	1. Ini�� �� ���� �÷����̴� MAX_LINE�� �ʰ��� �� �����ϴ�
	2. ���ۿ����÷ο�� ����÷ο찡 �߻����� �ʵ��� �����Ͽ��� ����Ͻʽÿ�
	3. MemoryIniŬ������ Thread-Safe���� �ʽ��ϴ�
	4. getString()�� ���������� slen�� ����ũ�Ⱑ �ƴ� ���ڿ������� �����ؾ� ��
	5. ��ȣȭ�˰����� �� Ŀ���͸���¡ �Ͽ� ����� ��
	
