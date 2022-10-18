========================================================================
    동적 연결 라이브러리 : SolarQuest 프로젝트 개요
========================================================================


{{남은 할일}}

2006.3.27
1. QuestCondition_Concrete의 구현 : 개별 게임 객체마다 가져야 하는 변수 선언
2. 


{{클라이언트와 서버의 구현의 차이점}}
* 클라이언트
- QuestManager의 Event()함수를 이벤트가 일어났을 때마다 호출해줘야 한다.
  ( 아이템insert,delete,update, hero LV, 퀘스트 완료, 시간체크, 돈체크 등등)
- QuestManager_Concrete클래스의 OnEvent()함수를 overloading하여 Quest의 상태 변경 이벤트를 catch한다.
- 성공인지, 실패인지에 따라 서버로 QUEST_FAILED_SYN, QUEST_COMPLETE_SYN를 보낸다.
- FAILED_ACK를 받으면 -> 인터페이스에 실패를 표시한다.
- COMPLETE_ACK를 받으면 -> 인터페이스에 완료 표시를 해준다.
- NAK이면 에러 메세지 출력
- 플레이어가 퀘스트를 포기하면 QUEST_ABANDON_SYN를 보낸다.
- 플레이어가 퀘스트를 보상받기하면 QUEST_REWARD_SYN를 보낸다.

* 서버
- 플레이어가 마을에 들어 왔을 때, DB로부터 Quest정보를 받고, QuestStreamManager에 넣어준다.
- QuestStreamManager의 값을 이용해 진행중인 퀘스트를 QuestManager에 넣어준다, 그리고 클라이언트에 Quest정보를 준다.
- 클라이언트에서 QUEST_ACCEPT_SYN 패킷이 왔을 경우, QuestManager의 CanAccept()를 호출한여 ACK/NAK를 보낸다.
- QuestManager의 Event()함수는 몬스터 킬만 처리한다. 몬스터 킬시 클라이언트로 QCode에 해당하는 몬스터 킬수를 보내준다.(QUEST_MONSTER_KILL_CMD)
- QUEST_ABANDON_SYN가왔을 때 QuestManager에서 진행중인 Quest를 삭제한다.
- QUEST_REWARD_SYN가 왔을 때 QuestManager에서 Quest를 제거해주고, 완료 퀘스트로 저장한다.(QuestStreamManager에 저장)
  Quest정보를 읽어서 보상스크립트에서 보상을 준다.
- 