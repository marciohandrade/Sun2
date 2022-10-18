#pragma once
//////////////////////////////////////////////////////////////////////////
//		송찬종 작업 디파인 SCJ, naklu
//////////////////////////////////////////////////////////////////////////
//------------------------------- 2015년 -------------------------------//
//#define _NA_008757_20160329_DOMINATION_GRAPHIC_CONTROL            // 2016.03.29 / 송찬종 / 점령전 그래픽 하향조절 기능 복구
//#define _NA_000000_20160315_ETHERWEAPON_SPECIALMOVE               // 2016.03.15 / 송찬종 / 발키리 에테르웨폰도 스페셜무브 추가
//#define _NA_008721_20160308_AUTOMOVE_TO_PORTAL_BUG_FIX            // 2016.03.08 / 송찬종 / 자동이동으로 포탈로 갈 때 플레이어 싱크 안맞는 문제 수정
//#define _NA_000000_20151125_MOUSECLICK_AREA_SELECT_CANCLE         // 2015.11.25 / 송찬종 / 마우스 클릭으로 스킬큐잉이 풀리지 않게 변경
//#define _NA_008546_20151029_SITTING_PACKET_DELAY_POSITION_SYNC_BUG// 2015.10.29 / 송찬종 / 앉기 패킷 SYN와 ACK사이의 딜레이동안 움직이면서 위치 안맞는 현상 수정
//#define _NA_000000_20151015_CALC_PORTAL_OFFSET_PROBLEM_MODIFY     // 2015.10.15 / 송찬종 / 포탈명 출력 위치 조정 중 무한루프 문제 수정
//#define _NA_000000_20150714_MOVEMENT_DISTANCE_CHECK               // 2015.07.14 / 남기봉 / 스피드핵 체크. 이동 가능 거리 체크
//#define _NA_008395_20150710_EVENT_CRYSTAL_UI_TEXT_CONTROL         // 2015.07.10 / 송찬종 / 수정기둥 ui의 텍스트 중 레벨 값을 스크립트에서 읽어와서 출력
//#define _NA_008350_20150617_SPA_NEW_STYLE_MODE_ANIMATION          // 2015.06.17 / 송찬종 / 온천 신규 스타일 모드에 따른 특수 애니메이션 작업
//#define _NA_000000_20150616_SPA_COSTUME_SHOW_HAIR                 // 2015.06.16 / 송찬종 / 온천옷 머리 보일 수 있도록 플래그로 설정하도록 개선
//#define _NA_000000_20150609_CHARACTER_ROTATE_RUNNING_BUG_FIX      // 2015.06.09 / 송찬종 / 회전 이동 시 움직임 버그 수정
//#define _NA_000000_20150604_BUG_FIND_TEST                         // 2015.06.04 / 송찬종 / 버그 찾기용 테스트 코드 (CTRL+F10 으로 퀘스트 네비게이션 위치 초기화)
//#define _NA_008302_20150601_QUEST_COMPLET_SYN_RECAST              // 2015.06.01 / 송찬종 / 퀘스트 완료 실패 시 재요청 기능 추가
//#define _NA_008316_20150604_DEV_VER_SHOW_DAMAGE_INFO                  // 2015.06.04 / 송찬종 / 개발모드에서 데미지 가해량을 표시해주는 작업
//#define _NA_008298_20150519_ALLOW_HEROMOVE_TO_DOWN_STATE          // 2015.05.19 / 송찬종 / 이동중에 다운이 안걸리는 현상 수정(디버프감소시간4초에 의해서 발생)
//#define _NA_008296_20150518_HELLOID_SPIN_CANCLE_BUG_FIX           // 2015.05.18 / 송찬종 / 헬로이드 스핀캔슬 버그 수정
//#define _NA_000000_20150428_ATTENDANCE_CHECK_SYSTEM_SUPERPOSITION_TYPE_MODIFY  // 2015.04.28 / 송찬종 / 출석체크 시스템 누적형 정렬방식 수정 (달력형과 동일하게)
//#define _NA_008252_20150427_AURASKILL_PACKET_PENDING              // 2015.04.27 / 송찬종 / 오오라스킬 패킷 중복처리 문제
//#define _NA_008235_BOSS_MONSTER_CRITICAL_DAMAGE_ANIMATION_IGNORE  // 2015.04.23 / 송찬종 / 보스(캡틴)몬스터 크리티컬 타격 애니메이션 무시
//#define _NA_008226_AC_PARENTMAP_LIST_SKIP                         // 2015.04.15 / 송찬종 / AC 선행과제 리스트 목록이 길어질 때 생략하기 기능 추가
//#define _NA_008196_20150407_GM_REMOVE_STATUS                      // 2015.04.07 / 송찬종 / 삭제 불가능한 상태제거 해주는 GM편의기능 추가
//#define _NA_008185_20150402_COSTUME_ITEM_FOR_NPC                  // 2015.04.02 / 송찬종 / 코스튬 NPC입히기
//#define _NA_008139_20150318_MODIFY_BUFF_ICON_UI                   // 2015.02.10 / 송찬종 / 버프아이콘(Status) UI 수정
//#define _NA_008069_20150210_ADD_SPA_SYSTEM                        // 2015.02.10 / 민경승, 송찬종 / 온천시스템 추가
//#define _NA_008090_20150226_GAMEOPTION_RECVPACKET_DONT_WAIT       // 2015.02.26 / 송찬종 / 게임종료 시 정보열람실 저장완료 패킷을 기다리지 않고 바로 종료
//#define _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM               // 2015.01.13 / 임상수 / 출석 시스템 추가
//#define _NA_007942_20150115_ADD_MAINMENU_DIALOG_ICON              // 2015.01.15 / 송찬종 / 메인메뉴 대륙지도 버튼 추가
//#define _NA_000000_20150114_MODIFY_MISSION_REWARD_DIALOG_SLOT     // 2015.01.14 / 송찬종 / 미션보상창 슬롯 출력,툴팁 수정
//#define _NA_007942_20150113_MODIFY_RUNE_STONE_WAYPOINT_MAXCOUNT   // 2015.01.13 / 송찬종 / 개인 웨이포인트 최대 저장 갯수 증가 (10->15)
//------------------------------- 2014년 -------------------------------//
//#define _NA_007865_20141403_LIGHTMAP_BUILD_EXCLUDE_OBJECTS        // 2014.12.03 / 송찬종 / 라이트맵 빌드 시 오브젝트 관련 제외 & 기능 부활
//#define _NA_007732_20141024_RIDER_MOVESPEED_SYNC                  // 2014.10.24 / 송찬종 / 다른플레이어의 탈것 이속 싱크 마춤
//#define _NA_007726_20141015_PROTECTION_SCREENSHOT                 // 2014.10.15 / 송찬종 / 스크린샷 암호화(압축) 기능
//#define _SCJ_SHOW_NPC_ID                                          // 2014.07.23 / 송찬종 / NPC ID 보여주기
//#define _SCJ_TOGGLE_EDITBOX_CONTROL                               // 2014.07.22 / 송찬종 / 토글 에디트박스 컨트롤 추가
//#define _NA_000000_20140520_MAPLOAD_FAIL_TO_FTP                   // 2014.05.20 / 송찬종 / 맵로드 실패 시 FTP서버로 통보
//#define _NA_007192_20140428_CLIENT_LUA_MACRO_SYSTEM               // 2014.04.28 / 송찬종 / 클라이언트 루아 매크로 시스템
//#define _NA_000000_20140423_SUMMON_BACKGROUND_PICKING             // 2014.04.23 / 송찬종 / 소환수 후선 픽킹
//#define _NA_000000_20140409_ADD_SHADOW_AREA_SKILL_POINTER         // 2014.04.08 / 송찬종 / 섀도우 전용 마법진 추가
//#define _NA_000000_20140408_SkILL_USE_AFTER_AUTOATTACK_OPTION     // 2014.04.08 / 송찬종 / 스킬 사용 후 자동공격 전환 여부 옵션화
//#define _NA_007113_20140324_SHADOW_HPSUCTION_RENEWAL              // 2014.03.24 / 송찬종 / 섀도우 스킬 중 에너지석션 광역화 (보류됨)
//#define _NA_007113_20140324_SHADOW_SKILL_BALLANCE_RENEWAL         // 2014.03.24 / 송찬종 / 섀도우 스킬 밸런스 리뉴얼
//#define _NA_007086_20140318_MONSTERBOOK                           // 2014.03.18 / 정현성,송찬종 / 몬스터도감
//#define _NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX    // 2014.02.10 / 송찬종 / 전혀 다른 퀘스트 창이 열리는 현상 수정
//#define _NA_000000_20140120_SMART_NPC_SYSTEM
//#define _NA_007029_20140120_DOMINATION_RESURRECTION_REFORM        // 2014.01.20 / 송찬종 / 점령전 부활 방식 변경 (죽으면 팝업창)
//#define _NA_007029_20140114_CHAOS_ZONE_ENTER_BLIND                // 2014.01.14 / 송찬종 / 카오스존 입장 활성화/비활성화 추가
//------------------------------- 2013년 -------------------------------//
//#define _NA_006953_20131114_DOMINATION_MANAGER_UI_RENEWAL         // 2013.11.14 / 송찬종 / 점령전 지역관리 리뉴얼
//#define _NA_000000_20130903_CHANNEL_GAGE_INCREASE                 // 2013.09.13 / 송찬종 / 채널 혼잡 게이지를 많아보이게 증가
//#define _NA_000000_20130828_GUILDMARK_SIZE_MODIFY                 // 2013.08.28 / 송찬종 / 길드마크 크기 변경
//#define _TEMP_0068_20130820_DOMINATION_MANAGE_PACKET_CONTROL // 임시로 성문관리 임시패킷 만들어서 베팅리스트에 성문개수를 보여줌
//#define _NA_006826_20130722_DOMINATION_RENEWAL                    // 2013.07.22 / 김진휘, 송찬종 / 피의성전 점령전 개편
//#define _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM             // 2013.07.03 / 김진휘, 송찬종 / 전장 투표 시스템 추가
//#define _NA_006744_20130531_COLLECT_NAK_TO_REMOVE_COLLECT         // 2013.05.31 / naklu / 채집 실패 시, 에러코드에 따라 채집을 비활성화 시킴
//#define _NA_006658_20130422_ADD_ACTION_TRIGGER_CHANGE_OBJECT_HP   // 2013.04.22 / naklu / 트리거 추가 - 오브젝트 HP 변경
//#define _NA_006619_20130422_ADD_ACTION_TRIGGER_CHANGE_RADAR_MAP   // 2013.04.22 / naklu / 트리거 추가 - 레이더맵(미니맵) 변경
//#define _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO        // 2013.04.15 / naklu / 쉴드 표시 (타겟창 ui에 플레이어일 경우 쉴드 표시)

//------------------------------- 2012년 -------------------------------//
//#define _NA_000000_20121113_DRAW_FTEXT_CLIPPING_TEXT        // 2012.11.13 / naklu / FTEXT 출력 시 밑의 잘리는 영역 그리기
//#define _NA_006222_20121031_EVENT_WINDOW_DISTRIBUTE_ENABLE_TEXT   // 2012.10.31 / naklu / uiEventWindow 에 이벤트 중일 때와 아닐 때 대사를 나눔
//#define _NA_006221_20121031_SPRITSLUG_SKILL_MODIFY          // 2012.10.31 / naklu / 드래곤나이트 스피릿슬러그 스킬의 발동조건을 섀도우 다크브레이크와 동일하게 변경
//#define _NA_000000_121015_LEAVE_CANCEL_TO_REVIVE_RE_CASTING // 2012.10.15 / naklu / 떠나기 캐스팅 취소 시 다시 부활 캐스팅하기
//#define _NA_004923_120618_SKILL_QUEUE_MOVE_CANCEL           // 2012.06.18 / naklu / 이동 시 스킬 큐잉 해제 (활성화된 마법진 제외)
//#define _NA_004921_20120628_ADD_QUEST_NPC_EMOTICON_FOR_QUEST_TYPE // 퀘스트 타입(일일,길드,반복)에 따른 NPC이모티콘(머리위 두루마리) 추가
//#define _NA_004756_120504_SHOW_SSQ_PLAYER_NAME              // 2012.05.04 / naklu / 타락한 사원에서 플레이어 이름 보이기 GM명령어 추가
//#define _SCJ_120104_SORT_QUEST_VIEW_PROGRESS                // 2012.01.04 / naklu / 퀘스트 미리보기 정렬하기

//------------------------------- 2011년 -------------------------------//
//#define _SCJ_111202_UI_RENEWAL                              // 2011.12.02 / naklu / UI 리뉴얼 작업 (퀘스트, ac, 캐릭터, 캐시샵창)
//#define _SCJ_CHECK_SEND_GAMEOPTION_AUTOHUNTING_PACKET       // 2011.11.10 / naklu / 내용변경 있을 때에만 자동사냥 옵션 저장    
//#define _SCJ_ADD_FORMAT_TEXT_PARAM_QUEST_START_NPC          // 2011.11.04 / naklu / FTEXT 타입 추가 - 퀘스트시작(완료) NPC이름
//#define _SCJ_ACSYSTEM_OVERLAP_PARENT                        // 2011.10.05 / naklu / AC과제 목록을 선행과제로 뭉치기
//#define _SCJ_SHOW_ONES_SPECIALACCESSORY_APPEARANCE          // 2011.09.26 / naklu / 특수 악세서리 외형 통합 (특악 외형의 복수 삭제)
//#define _SCJ_EXP_EVENT_ENABLE_EFFECT                        // 2011.09.23 / naklu / 경험치 이벤트 알리미 기능
//#define _NA_003033_20110831_DICECAST_ITEM_ROUTING           // 2011.08.31 / 정현성, 송찬종 / 주사위 시스템
//#define _SCJ_MAKE_DOUBLE_QUOTATION_MARKS                    // 2011.08.22 / naklu / 메세지의 따옴표에 따옴표 하나 더 붙이기
//#define _SCJ_REMOVE_GENDER_CREATE_SCEANE                    // 2011.06.29 / naklu / 일본 1102에 성별 추가 된 캐릭터 생성 막기
//#define _NA_002935_20110704_ITEM_SCORE                      // 2011.07.04 / 김현웅, 송찬종 / 장비 평가 점수 표시
//#define _SCJ_MODIFY_OPTIONLIST_PARSER_VALUETYPE_FILED_TWO   // 2011.06.22 / naklu / OptionList.txt의 ValueType2 필드가 추가되고 값이 1인 옵션은 음수가 이로운 옵션
//#define _SCJ_ADD_SNS_SHORTCUT                               // 2011.06.22 / naklu / SNS 바로가기 버튼 추가
//#define _SCJ_REMOVE_KEY_HELP_WINDOW                         // 2011.06.20 / naklu / ToTalGuide 이전에 사용하던 키 도움말 창을 사용안함 
//#define _SCJ_REMOVE_ANNOUNCE_WINDOW                         // 2011.06.20 / naklu / (디파인 적용 보류) ToTalGuide가 추가되면서 기존에 사용하던 이그니스 도움말 창을 사용안함
//#define _SCJ_CHANGE_SKILL_COOLTIME_COLOR                    // 2011.06.07 / naklu / 스킬 쿨타임 색상 변경
//#deinfe _SCJ_MODIFY_OPTIONLIST_PARSER_SUFFIX_FILED          // 2011.05.23 / naklu / OptionList.txt의 RatioValue 필드를 ValueType로 변경 작업 (접미사의 예외 경우 설정)
//#define _SCJ_SSQ_DIRECT_ENTER                               // 2011.05.20 / naklu / 타락한사원 입장권 사용 시 바로 포탈 열기
//#define _SCJ_ADD_TYPE_LAYER_FOR_ITEM                        // 2011.04.11 / naklu / 아이템 타입에 따른 레이어 추가
//#define _SCJ_SETITEM_TOOLTIP_REFORM                         // 2011.04.05 / naklu / (보류중) 세트아이템 툴팁 출력 개선
//#define _SCJ_THREE_LEVELUP_PER_EFFECT                       // 2011.04.01 / naklu / 3레벨 마다 레벨업 이펙트 추가/변경
//#define _SCJ_TEXT_COLOR_RENEWAL                             // 2011.03.07 / naklu / 채팅 텍스트 글씨에 테두리 속성 주기
//#define _SCJ_GM_CMD_CREATE_ITEM_TO                          // 2011.03.02 / naklu / 아이템 생성을 여러개를 가능하도록 하기
//#define _SCJ_SHOW_ITEM_ID                                   // 2011.02.01 / naklu / 아이템 툴팁에 아이템 ID 보여주기      
//#define _SCJ_PK_POPUP_DIALOG                                // 2011.01.04 / naklu / 서버 입장 시 PK관련 안내 팝업창 뛰움 
//#define _SCJ_LONG_TEXT_REDUCING                             // 2011.01.06 / naklu / 범위를 넘어가는 긴 길이의 문자열 줄이기