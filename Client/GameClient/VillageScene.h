//=======================================================================================================================
/** 마을씬
	@author
			유재영
	@since
			
	@remarks
	 - NPC 생성하는 루틴 추가

*/

#ifndef		_VillageScene_H
#define		_VillageScene_H

class SceneBase;

#include "battlescene.h"

//////////////////////////////////////////////////////////////////////////
// 임시

class Item;

//

typedef stdext::hash_map<DWORD, Item *> ITEM_OBJ_HASH;
typedef std::pair<DWORD, Item *>		ITEM_OBJ_PAIR;
typedef ITEM_OBJ_HASH::iterator	ITEM_OBJ_ITR;

//////////////////////////////////////////////////////////////////////////

class VillageScene : public BattleScene
{
public:
	VillageScene();
	~VillageScene();

	void Activate( void );					//	활성화 되었을 때
	BOOL Init( void );						//	초기화, 로딩
	BOOL Process( DWORD dwTick );			//	메인 계산
	BOOL Render( DWORD dwTick );			//	메인 랜더링
	void Release( void );					//	종료

public:
	void ProcessKeyInput_GM();

	void SetRqstFieldMoveMapCode(MAPCODE MapCode)	{ m_RqstMoveMapCode = MapCode; }
	MAPCODE GetRqstFieldMoveMapCode()				{ return m_RqstMoveMapCode; }

	

public:
	//////////////////////////////////////////////////////////////////////////
	// packetproc
	//////////////////////////////////////////////////////////////////////////	
	virtual void OnRecvCG_ZONE( MSG_BASE * pMsg );
    virtual void OnRecvCG_PARTY( MSG_BASE * pMsg );

	virtual void ParsePacket( MSG_BASE * pMsg );	

	virtual BOOL  SceneWndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam , SI_MESSAGE *stMessage);

	virtual void OnConnectWorldServer();


	virtual RELATIONKIND::TYPE GetRelationKindOfCharacter( Character * pObject, BOOL bForceEnemy  = FALSE );

private:
	MAPCODE		m_RqstMoveMapCode;

};


extern	VillageScene	g_VillageScene;

extern const char RoomResultCode[][INTERFACE_STRING_LENGTH];



#endif	//	_VillageScene_H



