#pragma once

#define WAVERIX_DEBUG_CODE( code )	code

#include <SSQ/SSQPacketProcS2S.h>

class Player;
class ServerSession;
class GameServerSession;
class WorldServerSession;
class GameInstanceDungeon;

#pragma warning(push)
#pragma warning(disable : 4512)
struct GameSSQCtrlCmd
{
	struct Created {};
	struct Destroy {};

	struct CharacterState {
		Character* const	pCharacter;
		const BOOLEAN		isEnter;
		CharacterState( Character* const _pCharacter, const BOOLEAN _isEnter )
			: pCharacter(_pCharacter), isEnter(_isEnter) {}

		struct Enter;
		struct Leave;
	};

	enum eCharacterEvent { On_Empty, On_Dead, On_Max };
	struct CharacterEvent {
		const eCharacterEvent	event;
		Character* const		pDest;
		Character* const		pSrc;
		CharacterEvent( const eCharacterEvent _event, Character* const _pDest, Character* const _pSrc )
			: event(_event), pDest(_pDest), pSrc(_pSrc) {}

		struct OnDead;
	};

	struct CharacterState::Enter : public CharacterState { Enter( Character* const pCharacter ) : CharacterState( pCharacter, TRUE ) {} };
	struct CharacterState::Leave : public CharacterState { Leave( Character* const pCharacter ) : CharacterState( pCharacter, FALSE ) {} };

	struct CharacterEvent::OnDead : public CharacterEvent { OnDead( Character* const pDest, Character* const pSrc ) : CharacterEvent( On_Dead, pDest, pSrc ) {} };

	struct GMCmd {
		nsSSQ_Util::GM_Spec::StringArray			Arguments;
	};
};
#pragma warning(pop)

struct GameSSQSessionCmd
{
	struct FromClient;
	struct FromAgent;
};

struct GameSSQSessionCmd::FromClient
{
	FromClient( ServerSession* const pSession ) : m_pSession(pSession) {}
	ServerSession*			m_pSession;
};

struct GameSSQSessionCmd::FromAgent
{
	FromAgent( ServerSession* const pSession ) : m_pSession(pSession) {}
	ServerSession*			m_pSession;
};

struct GameSSQControlManagerImpl;

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	<GameWarControlManager>
//
class GameWarControlManager : public IWorkNode
{
public:
	static const DWORD				INTERVALofUPDATE	= 100;	//100ms
public:		GameWarControlManager();
public:
	virtual	~GameWarControlManager();
	static	GameWarControlManager*	Instance() { return ms_pSSQControlManager; }
	//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
	//	<IWorkNode Interface>
public:
	static	WORK_KEY				GetWorkKey() { return ms_WorkKey; }
	static	VOID					SetWorkKey( const WORK_KEY workKey ) { ms_WorkKey = workKey; }
	virtual	BOOL					DoInit();
	virtual	BOOL					DoProcess();
	virtual	BOOL					IsComplete();
	virtual	BOOL					DoRelease();
	// Control Interface
	virtual	VOID					OnMsg( sSIG_MSG* pSIG_MSG );

	//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
	//	<INTERFACEs>
public:		VOID					Update();
			VOID					Serialize();
			BOOL					SendPacket( const MSG_BASE* pMsg, const WORD wSize );
public:
	static	VOID					OnRecv( const MSG_BASE* pMsg, const WORD wSize, const GameSSQSessionCmd::FromClient& pSession );
	static	VOID					OnRecv( const MSG_BASE* pMsg, const WORD wSize, const GameSSQSessionCmd::FromAgent& pSession );

	//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
	//	<INTERFACEs>
public:		BOOLEAN					IsEnableSSQField( const DWORD IDofSSQ );

public:
	static	VOID					OnMsg( const GameSSQCtrlCmd::Created&, GameInstanceDungeon* const pIndun );
	static	VOID					OnMsg( const GameSSQCtrlCmd::Destroy&, GameInstanceDungeon* const pIndun );
	static	VOID					OnMsg( const GameSSQCtrlCmd::CharacterState& rInfo, GameInstanceDungeon* const pIndun );
	static	VOID					OnMsg( const GameSSQCtrlCmd::CharacterEvent& rInfo, GameInstanceDungeon* const pIndun );

	static	BOOLEAN					OnMsg( const GameSSQCtrlCmd::GMCmd& rInfo, Player* const pGmPlayer );

private:	GameSSQControlManagerImpl*	m_pSSQImpl;
private:
	static	WORK_KEY				ms_WorkKey;
	static	GameWarControlManager*	ms_pSSQControlManager;
	friend struct GameSSQControlManagerImpl;
};

