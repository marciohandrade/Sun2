========================================================================
                        CryptIni 프로젝트 개요
========================================================================

목적: CryptIni는 간단하게 암호화된 INI를 사용할 수 있는 방법을 보여줍니다

참고:
	샘플 코드를 참조하십시요
	
	
제공 API:
	
	모든 API는 실패시 -1, 성공시 0 혹은 0이상의 값을 리턴합니다
	
	;	inifile 을 decrypt함수를 이용하여 복호화하여 읽어 들임
	
	int	LoadFromFile( LPCTSTR inifile, INI_CRYPT decrypt );
		inifile - Ini파일 절대 경로
		decrypt - 로드하면서 복호화할 함수포인터
	
	
	;	inifile 을 encrypt함수를 이용하여 암호화하여 저장 함	
	
	int	SaveToFile( LPCTSTR inifile, INI_CRYPT encrypt );
		inifile - Ini파일 절대 경로
		decrypt - 로드하면서 암호화할 함수포인터		
		
	
	;	section 에 해당하는 key 의 값(스트링)을 읽어서 pStr에 저장 함
	
	int	getString( LPCTSTR section, LPCTSTR key, LPTSTR pStr, DWORD slen );
		section - 읽어올 섹션명
		key		- 읽어올 키이름
		pStr	- 읽은후 저장될 내용값 버퍼
		strsz	- 저장될 버퍼의 길이(주의: 문자열길이로 지정함)
		
	
	;	section 에 해당하는 key 의 값(정수)을 읽어서 pInt에 저장 함

	int	getInteger( LPCTSTR section, LPCTSTR key, LONG *pInt );
		section	- 읽어올 섹션명
		key		- 읽어올 키이름
		pInt	- 저장될 정수값 버퍼의 포인터
		

	;	section 에 key 와 pStr(스트링)쌍으로 저장 함

	int	putString( LPCTSTR section, LPCTSTR key, LPCTSTR pStr );
		section - 저장할 섹션명
		key		- 저장할 키이름
		pStr	- 저장할 내용값 버퍼


	;	section 에 key 와 value(정수)쌍으로 저장 함
	
	int	putInteger( LPCTSTR section, LPCTSTR key, LONG value );
		section	- 저장될 섹션명
		key		- 저장될 키이름
		value	- 저장될 정수값의 버퍼
	
	
주의:
	1. Ini의 각 줄의 컬럼길이는 MAX_LINE을 초과할 수 없습니다
	2. 버퍼오버플로우및 언더플로우가 발생하지 않도록 주의하여서 사용하십시요
	3. MemoryIni클래스는 Thread-Safe하지 않습니다
	4. getString()의 마지막인자 slen는 버퍼크기가 아닌 문자열개수로 지정해야 함
	5. 암호화알고리즘은 꼭 커스터마이징 하여 사용할 것
	
