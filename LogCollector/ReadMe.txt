2010-07-28 플랫폼기술팀으로부터 업무 이관

================= 로그 콜렉터 간단한 설명 ================= 

ProcessSnapShot: 주기적으로 월드당 하나의 SUNLogConsole 를 실행한다. 
SUNLogConsole: 게임 서버의 로그파일을 네트웍드라이브를 이용하여 읽은후, 로그를 DB 에 작성한다.

ProcessSnapShot 는 상시로 실행되어 있는 상태이며, SUNLogConsole는 로그 변환 작업(파일->DB)을 마치면 종료한다.

================= 데이타 복구 방법 ================= 

% 추후 업뎃~


================= ProcessSnapShot 콘솔 명령어 ================= 
q: 종료

c: 해당 로그파일에서 로그 종류의 개수를 파악한다. 
	c C:\SUN\SERVER\FIELD\SUN_FIELD__Date_20110628.wzl(로그파일) C:\log.txt(결과물저장파일)
	콘솔창에서 아래와 같이 실행해도 같은 결과를 얻을수 있다.
	SUNLogConsole_d.exe -logfile C:\SUN\SERVER\FIELD\SUN_FIELD__Date_20110628.wzl -txtfile C:\log.txt