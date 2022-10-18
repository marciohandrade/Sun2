#ifndef     __UIDEF_MANAGER_PARTY_H__
#define     __UIDEF_MANAGER_PARTY_H__

struct	IM_PARTY_MANAGER
{
	static const DWORD	PARTY_OPTION;
	static const DWORD	PARTY_MEMBER1;
	static const DWORD	PARTY_MEMBER2;
	static const DWORD	PARTY_MEMBER3;
	static const DWORD	PARTY_MEMBER4;
	static const DWORD	PARTY_MEMBER5;
	static const DWORD	PARTY_MEMBER6;
	static const DWORD	PARTY_MEMBER7;
	static const DWORD	PARTY_MEMBER8;
	static const DWORD	PARTY_MEMBER9;
	static const DWORD	PARTY_LEAVE_POPUP;
	static const DWORD	PARTY_ITEM_SHARE_SYSTEM;
};

const short c_sMAX_PARTY_MEMBER_NUM = MAX_PARTYMEMBER_NUM - 1;

enum
{
    eUIPARTY_SORT_NORMAL=0,
    eUIPARTY_SORT_LEVEL_HIGH_TO_LOW,
    eUIPARTY_SORT_LEVEL_LOW_TO_HIGH,
    eUIPARTY_SORT_CLASS_HIGH_TO_LOW,
    eUIPARTY_SORT_CLASS_LOW_TO_HIGH,
};

// ckTODO: 나중에 제거
// 2006. 2. 16
// 디폴트 그림이 없고해서 기본 그림이 있기전 까진 일단 비어있는 그림으로 사용
const short c_sCHARFACE_DEFAULT = eCHAR_SHADOW;

/**
    파티원 정보
*/

class uicPartyMemberInfoPlus
{
public:
	uicPartyMemberInfoPlus() { Clear(); }
	~uicPartyMemberInfoPlus() {}

	uicPartyMemberInfoPlus(const uicPartyMemberInfoPlus& src);
	uicPartyMemberInfoPlus& operator=(const uicPartyMemberInfoPlus& rhs);
	uicPartyMemberInfoPlus& operator=(const _PARTY_MEMBER_INFO& rhs);
	uicPartyMemberInfoPlus& operator=(const _PARTY_MEMBER_INFO* rhs);

	inline
		void Clear()
	{
		bFill = false;
		bDead = false;
#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
#else
		bUnexpectedTerminate = false;
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
	}

	inline
		bool IsFill() const
	{
		return bFill;
	}

	inline
		const _PARTY_MEMBER_INFO& Get() const
	{
		assert (bFill);
		return memInfo;
	}

	inline
		bool CompKeySimple(DWORD dwKey)
	{
		return (memInfo.m_dwMemberKey == dwKey);
	}

    inline
        DWORD CompareName(const TCHAR* character_name)
    {
        if (strcmp(memInfo.m_szCharName, character_name) == 0)
        {
            return memInfo.m_dwMemberKey;
        }
        return 0;
    }

	inline
		bool CompKey(const uicPartyMemberInfoPlus& memInfoPlus)
	{
		return (memInfo.m_dwMemberKey == memInfoPlus.Get().m_dwMemberKey);
	}

	inline 
		void SetHpRatio(BYTE byHPRatio)
	{
		memInfo.m_byHPRatio = byHPRatio;
	}

	inline
		void SetChaoState(BYTE bState)
	{
		memInfo.m_byChaoState = bState;
	}

	inline 
		void SetMpRatio(BYTE byMPRatio)
	{
		memInfo.m_byMPRatio = byMPRatio;
	}

	inline
		void SetLevel(WORD wLevel)
	{
		memInfo.m_wLevel = wLevel;
	}

#ifdef _DH_BATTLEZONE2_
    inline
        void set_is_ready(bool is_ready)
    {
        memInfo.m_isReady = is_ready;
    }
    inline
        const bool is_ready() const
    {
        return memInfo.m_isReady;
    }
    inline
        void set_map_code(DWORD map_code)
    {
        memInfo.map_code_ = map_code;
    }
    inline
        const DWORD map_code() const
    {
        return memInfo.map_code_;
    }


    inline
        void SetTeam(BYTE is_team)
    {
        memInfo.m_selecteTeam = is_team;
    }

    inline
        const BYTE GetTeam() const
    {
        return memInfo.m_selecteTeam;
    }
#endif //_DH_BATTLEZONE2_

	inline
		void SetDead(bool bDeadFlag)
	{
		bDead = bDeadFlag;
		if(bDead) 
		{
			SetHpRatio(0);
		}
		else 
		{
			SetHpRatio(100);
		}
	}

	inline
		bool IsDead() const
	{
		return bDead;
	}

#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
    inline bool IsConnected() const {return memInfo.m_isConnected;}
    inline void SetIsConnected(bool val) {memInfo.m_isConnected = val;}
#else
    inline bool IsUnexpectedTerminate() const {return bUnexpectedTerminate;}
    inline void SetUnexpectedTerminate(bool val) { bUnexpectedTerminate = val;}
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS

#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
    inline 
        void SetSDRatio(BYTE bySDRatio)
    {
        memInfo.m_bySDRatio = bySDRatio;
    }
#endif // _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO

private:
	_PARTY_MEMBER_INFO	memInfo;
	bool                bFill;
	bool				bDead;
#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
#else
	bool				bUnexpectedTerminate;
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
};

//------------------------------------------------------------------------------
/**
*/
inline
uicPartyMemberInfoPlus::uicPartyMemberInfoPlus(const uicPartyMemberInfoPlus& src)
{
	*this = src;
}

//------------------------------------------------------------------------------
/**
*/
inline
uicPartyMemberInfoPlus&
uicPartyMemberInfoPlus::operator=(const uicPartyMemberInfoPlus& rhs)
{
	this->bFill = true;
	this->bDead = rhs.bDead;
#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
#else
	this->bUnexpectedTerminate = rhs.bUnexpectedTerminate;
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS

	memcpy(&memInfo, &(rhs.Get()), sizeof(_PARTY_MEMBER_INFO));

	return *this;
}

//------------------------------------------------------------------------------
/**
*/
inline
uicPartyMemberInfoPlus&
uicPartyMemberInfoPlus::operator=(const _PARTY_MEMBER_INFO& rhs)
{
	this->bFill = true;
	memcpy(&memInfo, &rhs, sizeof(_PARTY_MEMBER_INFO));

	return *this;
}

//------------------------------------------------------------------------------
/**
*/
inline
uicPartyMemberInfoPlus&
uicPartyMemberInfoPlus::operator=(const _PARTY_MEMBER_INFO* rhs)
{
	this->bFill = true;
	memcpy(&memInfo, rhs, sizeof(_PARTY_MEMBER_INFO));

	return *this;
}


//typedef MSG_CG_PARTY_NAVI_INFO_BRD::Node		NAVINODE;
struct NAVINODE 
{
	DWORD		objectKey;
	FIELDCODE	fieldCode;
	float		posX, posY;
    float       posZ; // Z좌표

	NAVINODE()
	{
		objectKey = 0;
		fieldCode = 0;
		posX = 0.0f;
		posY = 0.0f;
        posZ = 0.0f;
	}
	NAVINODE(MSG_CG_PARTY_NAVI_INFO_BRD::Node& rNode)
	{
		objectKey = DWORD(rNode.objectKey);
		fieldCode = rNode.fieldCode;
		posX = float(rNode.posX);
		posY = float(rNode.posY);
        posZ = float(rNode.posZ);
	}
	NAVINODE& operator=(MSG_CG_PARTY_NAVI_INFO_BRD::Node& rNode)
	{
		objectKey = DWORD(rNode.objectKey);
		fieldCode = rNode.fieldCode;
		posX = float(rNode.posX);
		posY = float(rNode.posY);
        posZ = float(rNode.posZ);

		return *this;
	}
};


class PartyNaviInfo
{
	typedef std::map<DWORD, NAVINODE>				MAP_NAVINODE;
	typedef std::map<DWORD, NAVINODE>::iterator		MAP_NAVINODE_ITR;
	typedef std::pair<DWORD, NAVINODE>				PAIR_NAVINODE;
	typedef std::vector<DWORD>						VEC_OBJKEY;

	enum {
		INTERPOLATON_DISTANCE		= 10,
		NONINTERPOLATON_DISTANCE	= 250,
	};

	// 메소드
public:
	PartyNaviInfo()		{}
	~PartyNaviInfo()	{}

	void Clear()
	{
		m_mapNaviNode_New.clear();
		m_mapNaviNode_Old.clear();
	}

	void Recv_NaviInfo(MSG_CG_PARTY_NAVI_INFO_BRD* pMsg)
	{
		if ( MSG_CG_PARTY_NAVI_INFO_BRD::OPTION_PARTY_NAVI == pMsg->m_CodeType )
		{
			MAP_NAVINODE_ITR itr;
			MAP_NAVINODE_ITR itrEnd = m_mapNaviNode_New.end();

			for(int index = 0; index < pMsg->m_NumberOfNodes; ++index )
			{
				itr = m_mapNaviNode_New.find( pMsg->m_Nodes[index].objectKey );
				if ( itr != itrEnd )
				{
					itr->second = pMsg->m_Nodes[index];
				}
				else
				{
					NAVINODE NaviNode = NAVINODE(pMsg->m_Nodes[index]);
					m_mapNaviNode_New.insert(PAIR_NAVINODE(NaviNode.objectKey, NaviNode));
					m_mapNaviNode_Old.insert(PAIR_NAVINODE(NaviNode.objectKey, NaviNode));
				}
			}
		}
	}

	bool SetNaviInfo(NAVINODE& naviInfo)
	{
		MAP_NAVINODE_ITR itr = m_mapNaviNode_New.find(naviInfo.objectKey);
		MAP_NAVINODE_ITR itrEnd = m_mapNaviNode_New.end();

		if ( itr != itrEnd )
		{
			(itr->second) = naviInfo;
			return true;
		}
		return false;
	}
	bool GetNaviInfo(DWORD dwObjKey, NAVINODE * pNode)
	{
		if ( pNode )
		{
			MAP_NAVINODE_ITR itr = m_mapNaviNode_Old.find(dwObjKey);
			MAP_NAVINODE_ITR itrEnd = m_mapNaviNode_Old.end();

			if ( itr != itrEnd )
			{
				(*pNode) = itr->second;
				return true;
			}
		}
		return false;
	}
	bool RemoveNaviInfo(DWORD dwObjKey)
	{
		m_mapNaviNode_Old.erase(dwObjKey);
		m_mapNaviNode_New.erase(dwObjKey);
		return true;
	}

	void Process(DWORD dwTick)
	{		
		MAP_NAVINODE_ITR itr = m_mapNaviNode_Old.begin();
		MAP_NAVINODE_ITR itrEnd = m_mapNaviNode_Old.end();

		while( itr != itrEnd)
		{
			MAP_NAVINODE_ITR itrNew = m_mapNaviNode_New.find(itr->first);
			
			WzVector vecTo = {itrNew->second.posX, itrNew->second.posY, itrNew->second.posZ};
			WzVector vecFrom = {itr->second.posX, itr->second.posY, itr->second.posZ};

			WzVector direction = vecTo - vecFrom;

			if ( VectorLength2(&direction) < 0.1f || VectorLength2(&direction) > NONINTERPOLATON_DISTANCE )
			{
				itr->second.posX = itrNew->second.posX;
				itr->second.posY = itrNew->second.posY;
                itr->second.posZ = itrNew->second.posZ;
			}
			else
			{
				VectorNormalize(&direction, &direction);
				itr->second.posX += (direction.x * INTERPOLATON_DISTANCE * dwTick /1000.0f);
				itr->second.posY += (direction.y * INTERPOLATON_DISTANCE * dwTick /1000.0f);
                itr->second.posZ += (direction.z * INTERPOLATON_DISTANCE * dwTick /1000.0f);
			}	
			itr->second.fieldCode = itrNew->second.fieldCode;

			++itr;
		}
	}

private:		

	// 데이터
private:	
	MAP_NAVINODE		m_mapNaviNode_New;
	MAP_NAVINODE		m_mapNaviNode_Old;	
};

//------------------------------------------------------------------------------
#endif  // __UIDEF_MANAGER_PARTY_H__
