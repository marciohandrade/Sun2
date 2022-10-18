
#ifndef		_PROTOCOLAGENT_H
#define		_PROTOCOLAGENT_H


/*

class CSceneGame;
class CPacketGenerator;


class ProtocolAgent
{
public:
	CProtocolAgentForSceneGame(void);
	~CProtocolAgentForSceneGame(void);
	void Clear( void);

	BOOL Create( CSceneGame *pSceneGame);
	void Destroy( void);


	/// @name 패킷 해석
public:
	void TranslatePacket( BYTE *pbyMsg);

protected:
	void TranslatePacket_Connection( MSG_BASE *pMsg);
	void TranslatePacket_CharInfo( MSG_BASE *pMsg);
	void TranslatePacket_Sync( MSG_BASE *pMsg);
	void TranslatePacket_Conversation( MSG_BASE *pMsg);
	void TranslatePacket_Other( MSG_BASE *pMsg);

	/// @name 패킷 보내기
protected:
	BOOL SendPacket( void *pPacket, int iSize);
	BOOL SendPacket( CPacketGenerator *ppg);
public:
	void SendPacket_RegisterOnServer( void);
	void SendPacket_SelectChar_Req( int iSlot);
	void SendPacket_Walk( CPathExplorer *ppeExp);
};

*/

#endif	//	_PROTOCOLAGENT_H

