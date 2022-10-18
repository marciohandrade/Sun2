//////////////////////////////////////////////////////////////////////////
//
// 이지연 작업 디파인
//
//////////////////////////////////////////////////////////////////////////

///////////////// [테스트 완료 후 삭제] /////////////////
//#define _KIKI_MODIFY_TOOLTIP					// 2006. 02. 20 / 이지연 / 툴팁 수정
//#define _KIKI_CHAR_POPUP_MENU					// 2006. 03. 07 / 이지연 / 캐릭터 팝업 메뉴
//#define _KIKI_HELP_MENU						// 2006. 03. 13 / 이지연 / 도움말 메뉴
//#define _KIKI_UI_ADD_SOUND_OPTION				// 2006. 03. 28 / 이지연 / 사운드 옵션창 추가
//#define _KIKI_UI_MODIFY_GRAPHIC_OPTION		// 2006. 03. 29 / 이지연 / 그래픽옵션 변경
//#define _KIKI_UI_MODIFY_GAME_OPTION			// 2006. 03. 29 / 이지연 / 게임옵션 변경
//#define _KIKI_UPDATE_WZSOUND_LIB				// 2006. 04. 08 / 이지연 / wzsound lib 에 새로 추가된 기능 적용
//#define _KIKI_MODIFY_HELP_INFO				// 2006. 04. 17 / 이지연 / 도움말 스크립트 변경
//#define _KIKI_ADD_PARTY_MEMBER_STATUS			// 2006. 04. 15 / 이지연 / 파티원 상태 관련된 메시지 받아와 처리 (레벨,체력,죽음)
//#define _KIKI_ADD_TOOLTIP_LIMITVIT			// 2006. 04. 04 / 이지연 / 툴팁에 체력 제한 추가	
//#define _KIKI_ADD_TOOLTIP_LIMITSPR			// 2006. 04. 04 / 이지연 / 툴팁에 정신력 제한 추가
//#define _KIKI_ADD_TOOLTIP_NEXT_GRADE_LIMIT	// 2006. 04. 07 / 이지연 / 인챈트,랭크업 시 다음등급 제한 표시

//#define _KIKI_CHANGE_LOADING_IMAGE_FORMAT		// 2006. 04. 14 / 이지연 / 로딩 이미지 포맷 변경
//#define _KIKI_APPLY_HELP_OPTION				// 2006. 04. 13 / 이지연 / 채팅창 도움말 제거 및 도움말 옵션 변경
//#define _KIKI_VOICE_CHAT_GET_ID				// 2006. 04. 06 / 이지연 / 파티정보에 계정 추가
//#define _KIKI_ADD_MIC_STATUS					// 2006. 04. 24 / 이지연 / 마이크 상태 on/off
//#define _KIKI_PARTY_VOICE_CHAT_MIC_VOL_OFF	// 2006. 04. 28 / 이지연 / 파티음성채팅 시작할때 마이크 볼륨 0으로 세팅
//#define _KIKI_MODIFY_CHAT_MIC_BTN				// 채팅창 마이크 버튼 -> 체크버튼으로 수정
//#define _KIKI_UI_MODIFY_CHAT_DIALOG			// 2006. 03. 31 / 이지연 / 채팅창 UI 수정
//#define _KIKI_MODIFY_CHAT_MENU				// 채팅 메뉴에 외치기, 귓속말 GUI 추가
//#define _KIKI_MODIFY_HP_MIC_BTN				// hp 게이지 창에 마이크 체크버튼 추가
//#define _KIKI_CHECK_READ_HELP					// 2006.04.17 / 이지연 / 도움말 읽었는지 저장

//#define _KIKI_NOPTION_TOOLTIP					// 2006. 03. 20 / 이지연 / 툴팁에 엑스트라 스톤 옵션 설명 추가
//#define _KIKI_BUG_FIX_TARGET_HP				// 2006. 04. 22	/ 이지연 / 타겟창 유저가 같은 파티원일때 HP갱신해주기
//#define _KIKI_MODIFY_BATTLE_ROOM_PASSWORD		// 2006. 04. 25 / 이지연 / 비밀방 생성 및 입장 관련 작업
//#define _KIKI_NEW_WZSOUND						// 2006. 05. 16 // 사운드 라이브러리 변경 
//#define _KIKI_MODIFY_TRADE					// 2006. 05. 03 / 이지연 / 거래 수정
//#define _KIKI_DEL_NORMAL_VOICE_CHAT			// 2006. 04. 27 / 이지연 / 1:1 음성 채팅 막기
//#define _KIKI_UI_EVENT_DIALOG					// 2006. 04. 11 / 이지연 / 이벤트 다이얼로그 작업

//#define _KIKI_UI_VOICE_CHAT					// 2006. 03. 20 / 이지연 / 음성채팅 컨트롤 윈도우
//#ifdef _KIKI_UI_VOICE_CHAT
//	#define _KIKI_VOICE_CHAT					// 2006. 02. 17 / 이지연 / 음성채팅 모듈 추가
//#endif
//#define _KIKI_VOICE_CHAT_RUN_MODIFY			// 2006. 04. 13 / 이지연 / WzVoiceClient 실행 시점 변경
//#define _KIKI_MODIFY_VOICE_CHAT				// 음성채팅 기능 수정
//#define _KIKI_UI_ANNOUNCE_WINDOW				// 2006. 05. 19 / 화면 하단 알림창

//#define _KIKI_KEY_BLOCK						// 2006. 04. 19 / 이지연 / Alt+F4 막기 (나머지 키 처리는 나중에...)
//#define _KIKI_ADD_OPTION_LIST					// 2006. 06. 14 / 옵션 리스트 스크립트로 읽어와서 처리 ( OptionList.txt )
//#define _KIKI_APPLY_TRADE_SELL_TYPE			// 2006. 04. 14 / 이지연 / 거래 가능여부 체크후 메시지 보여주기
//#define _KIKI_ADD_CHUNTING_MODE				// 2006. 04. 29 / 이지연 / 경쟁헌팅 모드 새로 추가 (타임어택,데스매치)
//#define _KIKI_UI_CHUNTING_WIN_DIALOG			// 2006. 05. 01 / 이지연 / 경쟁헌팅 승리 윈도우 추가
//#define _KIKI_UI_CHUNTING_DOING				// 2006. 05. 02 / 이지연 / 킬포인트 게이지 바 추가
//#define _KIKI_SORT_MAP_LIST					// 경쟁헌팅, 미션 맵 리스트 권장레벨 순으로 SORT
//#define _KIKI_CHANGE_SURVIVAL_DEAD_MSG		// 2006. 05. 18 // 서바이벌  모드에서 사망시 제자리 부활 버튼 삭제
//#define _KIKI_TIMEATTACK_OTHER_TEAM_COLOR		// 2006. 05. 19 / 타임어택모드에서 다른 파티 색깔 구분할것

//#define _KIKI_MODIFY_FRIEND					// 2006. 07. 06 / 친구대화 개선작업
//#define _KIKI_ADD_FRIEND_INFO					// 2006. 08. 04 / 친구 정보보기 기능 추가
//#define _KIKI_MODIFY_SKILL_DESC				// 2006. 06. 09 / 스킬 설명 수정: 표시방식이 TYPE_PERCENTAGE 인 경우, 소숫점 자리까지 출력
//#define _KIKI_ADD_KICK_GUILD_MEMBER			// 2006. 06. 14 / 길드원 제명
//#define _KIKI_ADD_GUILD_NOTICE				// 2006. 06. 20 / 길드 공지 명령어 추가: 사용예) /길드공지 공지내용
//#define _KIKI_ADD_DEFAULT_BATTLEROOM_NAME		// 2006. 06. 21 / 배틀존 방 생성시 방이름 디폴트 세팅
//#define _KIKI_ADD_DBLCLICK_JOINROOM			// 2006. 06. 28 / 배틀존 리스트 더블 클릭하면 바로 조인 할 수 있다.
//#define _KIKI_MODIFY_EVENT_MAN				// 2006. 08. 01 / 이벤트중인 버튼만 Show
//#define _KIKI_MODIFY_PARTY_MEMBER_KICK		// 2006. 09. 04 / 파티장의 파티원 강제 탈퇴 기능 개선 (파티창 x컨트롤 HIDE / 강퇴시 확인창 출력)
//#define _KIKI_NOT_LEAVE_ON_READY_BUTTON		// 2006. 09. 07	/ 레디중엔 방나가기 버튼 HIDE
//#define _KIKI_DISPLAY_CHUNTING_TYPE			// 2006. 09. 13	/ 배틀존 리스트창에 경쟁헌팅 방 타입 표시
//#define _KIKI_BUG_FIX_HIDE_CHATDIALOG			// 2006. 09. 25 / 배틀로비에서 시스템 다이얼로그 출력 이후에 채팅 다이얼로그 사라지는 현상 수정
//#define _KIKI_ADD_WAYPOINT					// 2006. 06. 26 / 웨이포인트 추가
//#define _KIKI_ADD_MISSION_RANK				// 2006. 08. 14 / 미션 랭킹
//#define _KIKI_READ_PACKET_BEFORE_CLOSE_SOCKET	// 2007.02.02	/ 서버와 연결이 끊어졌을때, 바로 소켓을 닫지 않고 버퍼에 남아있는 패킷을 다 처리한 후 끊어주도록 처리
//#define _KIKI_MODIFY_PVP_CONFIG				// 2006. 10. 20 / SubBattleInfoDialog -> uiBattleZoneReadyPVPConfig 로 변경
//#define _KIKI_MODIFY_BATTLE_ZONE				// 2006. 10. 20 / 배틀존 리팩토링
//#define _KIKI_SOUND_BUG_FIX_20345				// 2007.10.10 / 20345, 20349 버그 수정 / NPC 클릭 하여 창 열고, ESC로 창 닫기를 빠르게 여러번 반복하면
                                                // NPC 음성이 무한 반복 되는 버그 수정

//#define _KIKI_MODIFY_EXE_CONFIG					// 2007.12.27 / 실행파일 환경 수정 (PACK 버전과 UNPACK버전의 호환) : System.wpk 파일이 없으면 Login.ini를 읽어와서 로그인
//																	 General.ini의 USE_PACK_FILE 이 1이면 Resource.wpk, Sound.wpk 로딩
//																	 General.ini의 USE_PACK_FILE 이 0이면 Unpack 리소스를 로딩

//#define _KIKI_DELETE_NOT_USE_PARAMETER			// 2007.12.28 / General.ini 에서 사용하지 않는 파라미터 제거

//#define _KIKI_MODIFY_MISSION_MSG				// 2006. 09. 14 / 미션 연출 (미션에서 사용되는 시스템 메시지를 확장하여 연출적인 부분 강화)
												// Message.txt 스크립트 수정(메시지 타입 분리 및 미션전용 아이템 정보 UI 추가)

//#define _KIKI_MOD_SKILLTEXT						// 2007. 01. 25 / 스킬 텍스트 수정
//#define _KIKI_LOADING_TIME_TEST					// 2007.03.26	/ 게임실행시 커맨드옵션으로 -loading 을 붙이면, log\LoadingTime.log 파일에 로딩시간 로그를 남김.

//#define _KIKI_NEW_VOICE_CHAT					// 2006. 07. 26	/ 음성채팅 새 모듈로 교체
//#define _KIKI_MOD_VOICE_CHAT_MIC				// 2006. 09. 26 / 음성채팅 마이크 기능 비활성화 상태일 경우, 다른 사람의 목소리도 듣지 못하게 수정


//#define _KIKI_MOD_STREAM_SIZE_FOR_SERIALIZE_CLIENT_LOAD		// 2008.04.29	/ 이지연 / eSLOT_SERIALIZE_CLIENT_LOAD 시 스트림 사이즈가 0인 경우(인벤에 아무것도 없을때) 문제수정
															// 서버의 경우와 마찬가지로 MAX_INVENTORY_SLOT_NUM+MAX_TEMPINVENTORY_SLOT_NUM 를 이용해 사이즈 계산하도록 수정


//#define _KIKI_REFACTORING_DEBUG_COMMAND_MANAGER_PARSE_COMMAND		// 2009.04.29~2009.04.30   / DebugCommandManager::ParseCommand 리팩토링
//#define _KIKI_OUTPUTDBGSTR_PACKET_TRAFFIC		// 2009.3.23	/  패킷 전송량 디버그 메시지 출력 옵션 추가
///////////////// [완료] //////////////////////////////////

#if defined(_DEBUG) || defined(_GAMECLIENT_VER) 
    #define _KIKI_AUTOTEST
#endif


//#define _NA_20120109_KIKI_ENABLE_MULTI_CLIENT   // 2012.01.09 / 이지연 / 멀티 클라이언트 허용 
                                                // 현재는 GENERAL.INI의 SPECIAL_MODE가 eSPECIAL_MODE_NONE 일 때, 멀티 클라이언트가 블럭되고,(유저 배포 버전. Sungame.exe, Sungame_Dump.exe)
                                                // SPECIAL_MODE가 1보다 크거나 같으면, 멀티 클라이언트가 허용되고 있다.(개발 버전. GameClient.exe)
                                                //
                                                // SPECIAL_MODE 값은 개발 테스트를 위해서만 1보다 큰 값을 부여하므로,
                                                // SPECIAL_MODE 값과 상관없이 멀티 클라이언트 허용이 가능하도록  하는 디파인을 추가했음.


///////////////// [리팩토링] //////////////////////////////////
#define _KIKI_DELETE_PROJECTILEID_POWER_WAVE_   // 2010.02.01 / 이지연 / PROJECTILEID_POWER_WAVE 관련 코드 제거 (사용하지 않음)


///////////////// [진행중] //////////////////////////////////



///////////////// [테스트 중] //////////////////////////////////


///////////////// [디버깅용 디파인] //////////////////////////////////




///////////////// [GLog - 개발취소] //////////////////////////////////
//#define _KIKI_ADD_G_LOG_MENU					// 2006. 04. 13 / 이지연 / 캐릭 팝업 창에 G-로그 메뉴 추가
//----------------------------------
//07.04.09 / 윤영주 / 주석처리.
//#define _KIKI_UI_WEB_DIALOG						// 2006. 04. 13 / 이지연 / 웹 다이얼로그 UI 추가
//----------------------------------
//#define _KIKI_WEB_AUTH							// 2006. 04. 19 / 이지연 / 로그인시 웹 인증 요청            
//#define _KIKI_G_LOG_PROTOCOL					// 2006. 04. 19 / 이지연 / G-LOG용 아이디 얻어오기
