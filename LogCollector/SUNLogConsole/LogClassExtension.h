
class ActionDataEx : public ActionData
{
public:

	ActionDataEx(){};
	~ActionDataEx(){};

	int m_nServerCode;
	int m_nChannelCode;

};

class AdminDataEx : public AdminData
{
public:

	AdminDataEx()
	{

	};
	~AdminDataEx(){};

	int m_nServerCode;
	int m_nChannelCode;
};

class ExpDataEx : public ExpData
{
public:

	ExpDataEx()
	{
	};
	~ExpDataEx(){};

	int m_nServerCode;
	int m_nChannelCode;
};

class InventoryDataEx : public InventoryData
{
public:


	InventoryDataEx()
	{

	};
	~InventoryDataEx(){};

	int m_nServerCode;
	int m_nChannelCode;
};

class ItemDataEx : public ItemData
{
public:

	ItemDataEx()
	{
	

	};
	~ItemDataEx(){};

	int m_nServerCode;
	int m_nChannelCode;
};

class MoneyDataEx : public MoneyData
{
public:





	MoneyDataEx()
	{

	};
	~MoneyDataEx(){};

	int m_nServerCode;
	int m_nChannelCode;
};


class SessionDataEx : public SessionData
{
public:
	


	SessionDataEx()
	{
	
	};
	~SessionDataEx(){};

	int m_nServerCode;
	int m_nChannelCode;
};

class SkillDataEx : public SkillData
{
public:


	SkillDataEx()
	{

	};
	~SkillDataEx(){};

	int m_nServerCode;
	int m_nChannelCode;
};

class SnapShotDataEx : public SnapShotData
{
public:
	



	SnapShotDataEx()
	{


	};
	~SnapShotDataEx(){};

	int m_nServerCode;
	int m_nChannelCode;
};

class WareHouseDataEx : public WareHouseData
{
public:



	WareHouseDataEx()
	{
	
	};
	~WareHouseDataEx(){};

	int m_nServerCode;
	int m_nChannelCode;
};

class ChatDataEx : public ChatData
{
public:


	ChatDataEx()
	{
	
	};
	~ChatDataEx(){};

	int m_nServerCode;
	int m_nChannelCode;
};

#ifdef _GUILD_LOG
class GuildLogDataEx : public GuildData
{
public:
	GuildLogDataEx()
	{

	};
	~GuildLogDataEx(){};

	int m_nServerCode;
	int m_nChannelCode;
};
#endif //_GUILD_LOG


#ifdef _POINT_LOG
class AccumulatePointDataEx : public AccumulatePointData
{
public:
	AccumulatePointDataEx()
	{

	};
	~AccumulatePointDataEx(){};

	int m_nServerCode;
	int m_nChannelCode;
};

#endif //_POINT_LOG