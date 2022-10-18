//----------------------------------------------------------------------------
/** UserSlotSet
	- UserSlot 을 관리하는 클레스

	@remark
		- 상태 전이 경로

						
		  Block		   +-> Ready 
			|          |  | 	
		  Open -> Fill ->-+ <Ban>
		   |              |
		   +--------<-----+
*/
#ifndef USERSLOTSET
#define USERSLOTSET

class UserSlot;

#define INVALID_USER_SLOT_INDEX		-1

class UserSlotSet
{
public:
	enum eRT_USERSLOT_SET
	{
		SUCCESS = 1,
		FULL_USER_SLOT,
		EXIST_ALEADY_USER_SLOT,
	};

	UserSlotSet(void);
	virtual ~UserSlotSet(void);

	void		Initialize(DWORD dwInitSlotNum);
	void		Release();

	UserSlotSet::eRT_USERSLOT_SET 
				AddUser(UserSlot & EnterUserSlot);

	void		SetBlockIndex(int index);
	void		SetOpenIndex(int index);
	void		SetReadyIndex(int index);
	void		SetReadyCancleIndex(int index);

	void		SetTeamIndex(int index, int iTeam);

	BOOL		LeaveUser(DWORD dwPlayerKey);
#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
    BOOL		LeaveUser(char* character_name);
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
	void		FlushUsers();
	void		ReleaseUserAll();

	UserSlot *	GetUserByIndex(int index);
	UserSlot *	GetUserByKey(DWORD dwPlayerKey );
	int			GetUserIndexFromKey(DWORD dwPlayerKey );
	DWORD		GetMaxUser() const;
	DWORD		GetNumUser() const;
	DWORD		GetNonBlockNum() const;
	DWORD		GetBlockNum() const;

	BOOL		IsExistEmptySlot() const;

protected:
	int			findUserIndex(DWORD dwPlayerKey);
	int			getEmptySlot();
	UserSlot *	getUserSlot(DWORD dwPlayerKey);

private:
	UserSlot *	m_pUserSlotArr;	
	DWORD	m_dwMaxUserNum;
	DWORD	m_dwCurUserNum;
};

#endif
// USERSLOTSET