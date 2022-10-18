//------------------------------------------------------------------------------
/**
Work Define ( 작업시 디파인 )

@author
    유재영 (y2jinc@webzen.co.kr)
@since

@remarks
    _MOD : Modify 수정
    _ADD : 추가 사항
    _DEL : 삭제 될 사항.
*/

//////////////////////////////////////////////////////////////////////////
//삭제 예정 
//#define _REMOVE_UI_SUSPEND_FOCUS		    // 2006. 5. 17 / 유재영 / 포커스여 안녕~~~
//
//#define _ADD_CHECK_OVERLAP_EXCUTE		    // 2006. 5. 19 / 유재영 / 중복 실행 체크.
//#define _ADD_ATTACK_JUMP                    // 2006. 6. 26 / 유재영 / 공격 점프 ( 몬스터 )
//#define _TRY_SEND_LOGOUT_PACKET             // 2006. 8.  7 / 유재영 / 종료시 로그아웃 패킷 보냄.
//#define _ADD_CANNOTMOVE_WHILE_SITTING       // 2006. 8. 25 / 유재영 / 앉기 동작중에 이동 안되게 함.
//#define _DONT_CHECK_TRADE_ITEM				// 2006. 9. 13 / 유재영 / 아이템 거래 타입에 따른 체크 클라이언트는 안하게 함.(서버와 합의함(김민욱대리님))
//#define _ADD_ITEM_WASTE_BASKET				// 2006. 9. 12 / 유재영 / 인벤토리에 아이템 휴지통 관련 작업.
//
//#define _MOD_PREVIEW_ACCONQUEST_REWARD_INFO	// 2006. 9. 25 / 유재영 / 에어리어 컨퀘스트 보상정보 미리보기 기능 넣기.
//#define _ADD_LUCKY_MONSTER_RUNAWAY			// 2006. 8. 30 / 유재영 / 럭키 몬스터가 특정 시간마다 도망치며 사라질때 연출
//#define _ADD_FIND_ROOM_BY_NUMBER			// 2007. 2. 12 / 유재영 / 방 번호로 방 찾게함.( 기존엔 기존에 받아놓은 리스트에서만 찾았으나 서버에 패킷을 보내서 방을 찾게 함 )
//#define _MOD_POPUP_DAMAGE_INFO				// 2006.11. 29 / 유재영 / 데미지 표시 부분 수정( 색상, 크기, 모양 )
//
//#define _ADD_ELITE_ITEM_EQUIP_CAP		// 2007. 1.  5 / 유재영 / 엘리트 아이템 헬맷 장착시 커스터마이즈 작업.
//#define _MOD_CREATE_MISSION_METHOD			// 2006.12. 21 / 유재영 / 미션맵생성시 메인 미션없이도 생성되게

//#define _ADD_ELITE_ITEM						// 2006. 9. 29 / 유재영 / 엘리트 아이템 추가에 따른 작업.
//#define _ADD_UNIQUE_ITEM					// 2006.10. 16 / 유재영 / 유니크 아이템 추가에 따른 작업.






//////////////////////////////////////////////////////////////////////////
// 작업 진행 중 
//#define _MAP_OPTIMIZATION_OUTSIDE_FRUSTUM	// 2007. 1. 30 / 유재영 / 프러스텀 컬링을 해서 비주얼 리스트를 얻어왔는데 리스트를 순회하면서 또 컬링 검사 하는 부분 제거.
//#define _MOD_CHAGNE_DELETE_HERO				// 2007. 2. 22 / 유재영 / 히어로 삭제 방법 수정.( 기존엔 씬전환시 무조건 삭제했던걸 해당 히어로 생성시에만 삭제하게 함 ).

//////////////////////////////////////////////////////////////////////////
// 사용되지 않는 디파인
//#define _TEST_SYNCRONIZE					// 2006. 9. 25 / 유재영 / 파인드 패스 프로그래스를 일정 값으로 픽스해서 동기화 문제를 해결.

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// 지워진 디파인
//#define _STYLE_QUICK_AUTO_REGISTER	    // 스타일 퀵 창에 스타일 슬롯 자동 등록
//#define _ALL_CHAT_TO_WORLD			    // 무조건 월드 서버로 채팅 케시지를 보낸다.
//#define _NPC_PORTAL					    // 마을간 이동 처리
//#define _LIMIT_CREATE_BATTLE			    // 베틀 생성시 스크립트를 참조하여 생성가능 한멥만 선택가능.
//#define _EVENT_INVENTORY
//#define _GUILD_CHAT
//#define _DELAY_LEAVE_FIELD			    // 2006. 2. 15 / 유재영 / 필드에서 마을로 돌아갈때 딜레이를 줌.
//#define _EQUIP_LIMIT_OPTIONS			    // 2006. 2. 16 / 유재영 / 아이템 탈/장착시 아이템 옵션도 조건에 + 된다.
//#define _MOD_DEAD_PROCESS				    // 2006. 2. 20 / 유재영 / 죽었을때 표시 내용 및 처리 변경.
//#define _MOD_SYS_MSG_DLG				    // 2006. 2. 20 / 유재영 / SystemVerfy, SystemConfrim 표시 부분 수정
//#define SERVER_BASED_HP_MULTIPLIER
//// 조합 관련
//#define _ENCHANT_TEST					    // 새로 작성된 인첸트
//#define _RANKUP_TEST					    // 새로 작성된 Rankup
//#define _ADD_NEW_ITEM_COMPOSITE			// 2006. 3. 16 / 유재영 / 새로 바뀐 아이템 조합 추가
//#if defined( _ADD_NEW_ITEM_COMPOSITE )
//#	define _ADD_ITEM_COMPOSITE_COMPOSITE	// / 유재영 / 조합
//#	define _ADD_ITEM_COMPOSITE_CRYSTALIZE	// / 유재영 / 결정화 
//#	define _EXTRASTONE_USE					// / 유재영 / 엑스트라 스톤 사용
//#	define _MOD_ITEM_EXTRACTSOCKET			// / 유재영 / 옵션 제거
//#endif
//#define _ADD_ITEM_COMPOSITE_FORWARD_INFO	// 2006. 3. 28 / 유재영 / NPC 별로 조합 할 부분 결정.
//#define _CHANGE_MAP_LOAD_METHOD		    // 2006. 4. 27 / 유재영 / 맵로딩 방식 변경.
//#define _ADD_MAP_LOAD_OPTION			    // 2006. 6.  2 / 유재영 / 맵로딩시 옵션 분리.

//#define _MOD_SCREEN_SHOT				    // 2006. 4.  7 / 유재영 / 스크린 샷 파일명 관련 변경 사항 
//#define _ADD_EQUIP_SPECIAL_ACSSESORY	    // 2006. 5.  7 / 유재영 / 특수 악세사리 장착 
//#define _ADD_LOCK_ITEM_COMPOSITE		    // 2006. 5.  9 / 유재영 / 아이조합 락킹 관련.
//#define _CHANGE_GAME_INFO_LOAD			// 2006. 5. 16 / 유재영 / 게임 기본 정보 로딩 하는 부분 수정(위치 수정)
//////////////////////////////////////////////////////////////////////////