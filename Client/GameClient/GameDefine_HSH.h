#pragma once
//////////////////////////////////////////////////////////////////////////
//		홍성현 작업 디파인 HSH, hsh2303
//////////////////////////////////////////////////////////////////////////
///*($Delete_[x/C]$)*/#define _NA_006539_20130319_ITEM_COMPOSIT_CERTAIN                                   // 2013.03.19 / 홍성현 / 제작 성공률표시를 86%~100를 매우 높음에서 100% 일 때 '확실'로 뜨도록 분리 수정 작업-1302 빌드 때 나가야 함.
///*($Delete_[x/C]$)*/#define _NA_006600_20130329_ADD_CHAOS_SCORE_KILL_ABOUT                              // 2013.03.29 / 홍성현 / 자피르 격전지에서 상대편 플레이어 제거 시 추가 득점 추가
///*($Delete_[x/C]$)*/#define _NA_006627_20130411_ADD_QUEST_TYPE                                          // 2013.04.11 / 홍성현 / 퀘스트 수행 타입 추가<1302로 나가야 된다.>
///*($Delete_[x/C]$)*/#define _NA_006639_20130415_DEATH_AND_NOT_RELIEVE_RIDER                             // 2013.04.15 / 홍성현 / 탈것을 탄 상태에서 사망 시 탈것 소환과 날기가 해제되지 않는 현상 버그 수정
///*($Delete_[x/C]$)*/#define _NA_006640_20130415_VIEW_INCREASE_HEAL                                      // 2013.04.15 / 홍성현 / 발키리 소환수 "생명의 정령"의 힐 스킬에 소환사의 스킬 공격력 추가 힐량이 적용되지 않는 현상 수정
///*($Delete_[x/C]$)*/#define _NA_006647_20130419_TOOLTIP_VIEW_GENDER_COLOR                               // 2013.04.19 / 홍성현 / 여성 발키리 캐릭터가 "여성 엘리멘탈리스트용 호피무늬 의상"에 마우스 오버시 사용 가능 클래스 출력 부분이 흰색으로 출력되는 현상
///*($Delete_[x/C]$)*/#define _NA_006657_20130422_BUG_NOT_TRANS_MONSTER                                   // 2013.04.22 / 홍성현 / 바람의 시련의 보스 칼버스가 죽은 후 드래곤으로 부활하지 않는 버그
///*($Delete_[x/C]$)*/#define _NA_006665_20130424_ADD_CHAOS_ASSIST_TYPE                                   // 2013.04.24 / 홍성현 / 전장 어시스트 항목 추가 작업(1302에 나가야 함)(_NA_006633_20130415_ADD_BATTLEGROUND_ASSIST와 통합)
///*($Delete_[x/C]$)*/#define _NA_006687_20130502_SETTING_FIGHTINGENERGY_INIT_BUG                         // 2013.05.02 / 홍성현 / 버서커의 검기충전이 다른 캐릭터에서도 버프 아이콘으로 나타나는 현상 수정
///*($Delete_[x/C]$)*/#define _NA_006700_20130506_BUG_SHADOW_SKILL_NOT_USE_DARKBREAK                      // 2013.05.06 / 홍성현 / 섀도우의 "다크브레이크" 스킬이 사용되지 않는 현상 수정(실질적으로 사용하지 않음)
///*($Delete_[x/C]$)*/#define _NA_006701_20130506_BUG_DRAGONNIGHT_SKILL_NOT_USE_SPIRITSLUG                // 2013.05.06 / 홍성현 / 드래곤나이트의 "스피릿슬러그" 스킬이 사용되지 않는 현상 수정(실질적으로 사용하지 않음)
///*($Delete_[x/C]$)*/#define _NA_006708_20130509_BUG_ROOM_STRING_LIMIT                                   // 2013.05.09 / 홍성현 / [배틀존&카오스존] 방을 생성할 때 방제목을 한글 15자 영문/숫자 30자로제한 수정 작업
//#define _JP_006736_20130524_CAN_TUTORIAL_FIELD_JOIN_BATTLEZONE_AND_CHAOSZONE        // 2013.05.24 / 홍성현 / 튜토리얼 필드에서도 배틀존에 참가 할 수 있도록 개선
//#define _JP_006740_20130527_LIMIT_EVENT_TUTORIAL_FIELD_WAYPOINT_VIEW_MIN_MAX_LEVEL  // 2013.05.27 / 홍성현 / 이벤트 기간 중 엘소드 마을로 가는 웨이 포인트 레벨 표시를 최소~최대 레벨 표시로 개선
///*($Delete_[x/C]$)*/#define _NA_006772_20130614_BUG_ENERGY_SUCTION_CONTINUE_ANIMATION                   // 2013.06.14 / 홍성현 / 섀도우 클래스 커스 계열 3행의 "에너지 석션" 스킬을 사용하다 캐릭터가 사망한 상태에서 스킬 애니메이션이 지속되는 현상 수정 작업
///*($Delete_[x/C]$)*/#define _NA_006781_20130627_BUG_SHOW_OTHERPLAYERNAME_OF_GAMEOPTION                  // 2013.06.27 / 홍성현 / 옵션 게임 탭에서 "다른 플레이어 이름" 기능을 Off하면 내 캐릭터의 이름도 출력되지 않는 현상 수정 작업
//#define _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM                              // 2013.07.04 / 홍성현 / 랭킹 시스템 개편
//#define _NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR                        // 2013.08.19 / 홍성현 / 랭킹 시스템 개편 Minor
///*($Delete_[x/C]$)*/#define _NA_006886_20130910_BUG_POINT_ACCUMULATE_IMAGE                              // 2013.09.10 / 홍성현 / 접속 포인트 보상 받기 중 엘리트 2차 재료가 보상으로 받아지는 경우 해당 아이템의 아이콘이 1초마다 각 클래스의 재료 아이템으로 바뀌어 출력되는 현상 수정 작업
//#define _NA_006928_20131022_HELMET_UI_BUG                                           // 2013.10.22 / 홍성현 / 캐릭터가 처음 접속했을 때 헬멧 보이기 안보이기 UI가 맞지 않는 현상 수정
//#define _NA_006930_20131023_ACCUMULATE_PAUSE_AND_CLOSE_ADD_FUNC                     // 2013.10.23 / 홍성현 / 접속포인트 보상 받는 도중 ESC키를 통해 정지시키고 dialog를 닫는 기능 추가
//#define _RU_007007_20131223_POSSIBLE_ENTER_CHRISTMAS_MAP                            // 2013.12.23 / 홍성현 / 꼬꼬마 눈사람 맵 입장가능하도록 하는 작업
//#define _NA_007019_20140107_PERK_CHANGE_SYSTEM                                      // 2014.01.07 / 홍성현 / 커스텀 패시브 그룹으로 변경하는 작업
///*($Delete_[x/C]$)*/#define _NA_007080_20140227_DEACTIVATION_APPEARANCE_UPDATE_AND_ATTACKRANGE_UPDATE_BUG // 2014.02.27 / 홍성현 / 장비가 비활성화 되었을 때(내구도 0이나 스텟이 안맞을 때)갱신이 제대로 되지 않는 버그, 펀치상태인데도 원거리 공격이 가능한 버그
//#define _NA_007127_20140326_OPEN_INVENTORY_FRAME_DROP_ADJUST                        // 2014.03.14 / 홍성현 / 인벤토리를 열었을 때 느려지는 현상 수정
//#define _NA_007115_20140313_MONDSCHIEN_RENEWAL                                      // 2014.03.13 / 홍성현 / 몬트샤인 리뉴얼
//#define _NA_007114_20140313_CHAOSZONE_GOLDRUSH                                      // 2014.03.13 / 홍성현 / 골드러쉬
//#define _NA_000000_20140519_GAMEGUARD_LOG // 게임가드에서 작동되는 로그로 확인 할 수 있게 하는 작업
///*($Delete_[x/C]$)*/#define _NA_000000_20140724_GUILD_EXTENTION_ACTIVE_MARK // 길드 시설에서 액티브마크를 보이게 하는 작업
///*($Delete_[x/C]$)*/#define _NA_007450_20140805_CRYSTALIZATION_RENEWAL // 결정화 리뉴얼
//#define _NA_007450_20140805_CRYSTALIZATION_RENEWAL_NPC_SELECT_HEIGHT_ADJUST // 결정화 리뉴얼 관련해서 NPC셀렉트에서 결정화 버튼을 없앴을 때 사이즈 조절해주는 작업
//#define _NA_007619_20140916_PROCESS_CHANGE_AIRBLOW_DOWN // 2014.09.16 / 홍성현 / 에어블로우 다운 처리방식변경
//#define _NA_007665_20140930_CASHSHOP_COSTUME_PREVIEW_BUG // 2014.09.30 / 홍성현 / 캐시샵 코스튬 미리보기 기능에서 성별의 애매함으로 인하여 성별 선택기능을 보이지 않는다.
//#define _NA_007863_20141203_VOLUME_LIGHT_SCATTERING // 2014.12.03 / 홍성현 / 볼륨 라이트가 제대로 작동하지 않던 현상 수정
//#define _NA_007924_20150105_MINIMAP_PORTAL_STRING_OUTRANGE_BUG // 2015.01.05 / 홍성현 / 포탈 텍스트가 미니맵 UI 위에 겹쳐져 출려되는 현상 수정
//#define _NA_007935_20150109_GM_COMMAND_EXP_EVENT // 2015.01.09 / 홍성현 // GM 명령어 추가 개발(GM명령어로 경험치 이벤트 만들기)
//#define _NA_007889_20141215_GM_COMMAND_CHARINFO // 2014.12.15 / 홍성현 // GM 명령어 추가 개발(캐릭터 정보를 보여 줄 수 있도록 한다.)
//#define _NA_000000_20150130_ITEMSHOP_NOT_SLOTTEXT_ALL_FILL // 2015.01.29 / 홍성현 / 아이템 샵에서 슬롯의 설명 텍스트를 줄이지 않고 표시
//#define _NA_008016_20150202_OLD_RANKING_SYSTEM // 2015.02.02 / 홍성현 / 기존 랭킹 시스템 디파인 설정
//#define _NA_008016_20150203_SUN_RANKING_SYSTEM // 2015.02.03 / 홍성현 / SUN 랭킹 시스템
//#define _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE // 2015.02.03 / 홍성현 / SUN 랭킹 등급 시스템
//#define _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE // 2015.02.03 / 홍성현 / SUN 랭킹 시스템 동상
//#define _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM // 2015.03.11 / 홍성현 / 장비 각성 시스템
//#define _NA_008121_20150318_EQUIPMENT_EVOLUTION_SYSTEM // 2015.03.18 / 홍성현 / 장비 진화 시스템
//#define _NA_008168_20150330_IMPROVED_REGISTRATION_OF_NEW_SHORTCUTS // 2015.03.30 / 홍성현 / 신규 단축키 등록 개선
//#define _NA_008016_20150331_SUN_RANKING_SYSTEM_SETTLE_DATE_VIEW // 2015.03.31 / 홍성현 / SUN 랭킹 시스템 정산일 보여주는 작업
//#define _NA_008243_20150422_DOMINATION_SCENE_OPEN_NPC_STORE // 2015.04.22 / 홍성현 / 점령전 씬에서 NPC상점을 열 수 있도록 수정
//#define _NA_008283_20150511_NOTIFICATIONS_SYSTEM // 2015.05.11 / 홍성현, 임상수 / 알림 시스템
//#define _NA_000000_20150616_ALWAYS_NOT_USE_FIXED_UI_POSITION // 2015.06.16 / 홍성현 / 인터페이스 고정 사용 안함
//#define _NA_008364_20150623_CHANGE_TEXT_PATH // 2015.06.23 / 홍성현 / TEXT 경로 변경
//#define _NA_000000_20150422_SPEED_HACK // Speed Hack 재현
//#define _NA_008405_20150716_MINIGAME_BINGO // 2015.07.16 / 홍성현 / 미니게임 빙고
//#define _NA_008472_20150903_IMPROVED_BALANCE // 2015.09.03 / 홍성현 / 밸런스 개선
//#define _NA_008502_20150922_BATTLEZONE_LOCK_ICON_BUG // 2015.09.22 / 홍성현 / 배틀존의 락 아이콘이 제대로 표시되지 않는 현상 수정 작업
//#define _NA_008536_20151023_ELITE4TH_ADD // 2015.10.23 / 홍성현 / 엘리트4차 무기/방어구 추가 작업
//#define _NA_008536_20151023_ELITE4TH_ADD // 2015.10.23 / 홍성현 / 엘리트4차 무기/방어구 추가 작업
//#define _NA_000000_20151026_TRIGGER_TIMER_SCENE_TYPE_REMOVE // 2015.10.26 / 홍성현 / 트리거 타이머 SCENE TYPE을 없앤다
//#define _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD // 2015.10.30 / 홍성현 / 각성에 재료 개수를 추가하는 작업
//#define _NA_008559_20151103_QUEST_AUTO_ACCEPT_PAGE_FUNC_ADD // 2015.11.03 / 홍성현 / 퀘스트 자동 수락 페이지 기능 추가
//#define _NA_008571_20151116_MODIFY_VENDOR_SEARCH // 2015.11.16 / 홍성현 / 개인상점 검색 시, 마을제한 제거
//#define _NA_008633_20160104_ADD_DECORATECOSTUME // 2016.01.04 / 홍성현 / 데코 코스튬 추가
//#define _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD // 2015.10.30 / 홍성현 / 각성에 재료 개수를 추가하는 작업
//#define _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION // 2016.02.03 / 홍성현 / 하임 거래량 알림 옵션 추가
//#define _NA_000000_20160308_ATTACK_RATE_RISE_BUG // 2016.03.08 / 홍성현 / 공격속도 상승 버그 수정
//#define _NA_008732_20160315_WITCHBLADE_BALANCE_ADJUST // 2016.03.15 / 홍성현 / 위치블레이드 밸런싱 조정 작업.
//#define _NA_008740_20160321_MAGICIAN_STAFF_CREATE_PROJECTILE // 2016.03.21 / 홍성현 / 엘리멘탈리스트 지팡이 일반공격 발사체를 생성하도록 작업.
//#define _NA_008783_20160418_DEFENCEMOTION_CANCEL_BUG // 2016.04.18 / 홍성현 / 디펜스모션이 풀리는 버그 수정

