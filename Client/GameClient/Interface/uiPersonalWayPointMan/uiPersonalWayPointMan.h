#pragma once
//------------------------------------------------------------------------------
//		작업자	:	이태준
//		uiPrivateWayPointMan class
//		일본 개인 웨이포인트 매니저
//------------------------------------------------------------------------------

#include "uiBaseMan.h"

#include "boost/shared_ptr.hpp"

class uiPersonalWayPointWindow;
class SCItemSlot;

struct IM_PERSONAL_WAYPOINT_MANAGER 
{
	static const DWORD WAYPOINT_DIALOG;
};

class uiPersonalWayPointMan : public uiBaseMan
{
	/*
	- 처음 창을 열때 서버로부터 웨이포인트 전체 정보를 가져온다
			- 웨이포인트 슬롯 데이터 저장
			- 정보 수신 true
			- 웨이포인트 데이터 정렬 및 창 상태 설정( 아이템 유무, 사용기간에 따른 비활성화 )
			- 창 디스플레이

	- 이후 창을 열때 정보 수신 true 이면 바로 디스플레이
			- 정보 수신 false 이고 수신 대기 중이면 대기
			- 이용시간이 지나거나(아이템 무) 배틀존일 경우 모든 버튼 비활성화
					{// 마을과 필드인지 체크
						if (GameFramework::GetCurrentScene() && 
						(SCENE_TYPE_VILLAGE != GameFramework::GetCurrentScene()->GetType() &&
						SCENE_TYPE_FIELD != GameFramework::GetCurrentScene()->GetType() ) )
					}
			- 삭제 시간제한 시  삭제버튼 비활성화
	
	- 웨이포인트 저장
			- 웨이포인트 목록이 10개 일 경우 -> 10개 제한 메시지 출력
			- 서버에 웨이포인트 세이브 송신
	
	- 웨이포인트 삭제
			- 웨이포인트 목록이 0개 일 경우 -> 무시
			- 삭제 시간제한 - 시간제한 메시지 출력
			- 서버에 웨이포인트 삭제 송신

	- 이동
	- 결과코드

	//////////////////////////////////////////////////////////////////////////

	데이터 수신 플래그 ( 다이얼로그에 포함 )
	웨이포인트 정보 테이블( 다이얼로그에 포함 )

	*/
public:
    uiPersonalWayPointMan(InterfaceManager *pUIMan);

public:
	virtual void			OnInitialize();

	void					InitWayPoint();
    
public:
    virtual void	        NetworkProc( MSG_BASE * pMsg );
	void					_processErrorCode(DWORD dwErrorCode);
	void					SEND_CG_CHAR_WAYPOINT_INFO_LOAD_SYN();

	void					ShowWayPointWindow(bool bShow);

	SCItemSlot*				GetActiveWaypointItem();

#ifdef _NA_000000_20121210_REFACTORY_GAME_USER
    static void HotKeyTogglePersonalwarpDialog();
#endif   //_NA_000000_20121210_REFACTORY_GAME_USER

private:
    uiPersonalWayPointWindow*					m_pUIWayPoint;
};

