//----------------------------------------------------------------------------
/** 로비에서 보여줄 기본 유저 슬롯 정보.
*/

#ifndef USERSLOT_H
#define USERSLOT_H

class UserSlot
{
public:
	enum eSLOT_STATE
	{
		OPENED = 0,
		FILL,
		BLOCK,
		READY,
	};

	UserSlot(void);
	virtual ~UserSlot(void);

	void			Clear();

	void			SetSlotState(UserSlot::eSLOT_STATE eState);
	void			SetPlayerKey(DWORD dwPlayerKey);
	void			SetTeam(int iTeam);
    void            SetBattleZonePlayerKey(DWORD val) { m_dwBattleZonePlayerKey = val; }

	UserSlot::eSLOT_STATE   GetSlotState() const;
	DWORD   GetPlayerKey() const;
	int     GetTeam() const;
    DWORD   GetBattleZonePlayerKey() { return m_dwBattleZonePlayerKey; }

#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
    inline void SetPlayerName(const char* character_name)
    {
        StrnCopy(m_szCharName, character_name, MAX_CHARNAME_LENGTH);
    }
    char* GetPlayerName(){return m_szCharName;}
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS

private:
	eSLOT_STATE		m_eSlotState;
    DWORD			m_dwPlayerKey;
    DWORD			m_dwBattleZonePlayerKey;
	int				m_iTeam;
#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
    char            m_szCharName[MAX_CHARNAME_LENGTH + 1]; // 캐릭터 ID
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
};

#endif 
// end of USERSLOT_H