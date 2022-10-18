#pragma once

#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
#include "Player.h"

struct StatueGuildInfo
{
	TCHAR guild_name[MAX_GUILDNAME_LENGTH+1];
	int guild_mark_pattern_idx;
	int guild_mark_background_idx;

	StatueGuildInfo()
	{
		ZeroMemory(guild_name, MAX_GUILDNAME_LENGTH+1);
		guild_mark_pattern_idx = 0;
		guild_mark_background_idx = 0;
	}
};

class StatuePlayer : public Player
{
public:
	StatuePlayer(void);
	virtual ~StatuePlayer(void);

	virtual BOOL        Create( DWORD object_key, DWORD id = 0 );
	virtual void        Destroy( void );
	virtual BOOL        Process( DWORD tick );
	void                RenderInfo();
	virtual BOOL        Render();
	virtual void        AnimateUnit();

	virtual WzID        GetCurrentIdleAnimation();
	virtual WzID        GetCurrentMoveAnimation();

	// 순수가상함수재정의:StatuePlayer에서는 안씀
	virtual WzID        GetCurrentAttackAnimation() { return 0; }
	virtual WzID        GetCurrentDeathAnimation() { return 0; }
	virtual WzID        GetCurrentDamageAnimation(BOOL right, BOOL critical) { return 0; }
	virtual char        GetWeaponAnimationCode();
	virtual void     SetExp(EXPTYPE exp) {}
	virtual EXPTYPE     GetExp() { return 0; }
	virtual EXPTYPE        GetNextExp() { return 0; }
	virtual void        SetLevel(LEVELTYPE LV) {}
	virtual LEVELTYPE   GetLevel() { return 0; }
	virtual DWORD       GetMaxHP() { return 0; }
	virtual DWORD       GetMaxMP() { return 0; }

	virtual void        SetClass(eCHAR_TYPE character_class);

protected:
	virtual void        InitContainer();
	virtual void        ReleaseContainer();

public:
	bool                RefreshApperBronzePlayer();
	EquipmentContainer* get_proto_equip_container_() { return proto_equip_container_; }
	EquipmentContainer* get_statue_equip_container_() { return statue_equip_container_; }

	void                set_equip_container_(EquipmentContainer* equip_container);
//	    virtual int                 GetItemScore() { return m_ItemScore; }

public:
	void                set_stand_animation_id_(std::string animation_id);
	WzID                get_stand_animation_id_() const;
	void                set_statue_guild_info(const StatueGuildInfo& statue_guild_info) { statue_guild_info_ = statue_guild_info; }
	StatueGuildInfo     get_statue_guild_info() const { return statue_guild_info_; }

private:
	WzID                stand_animation_id_;
	EquipmentContainer* proto_equip_container_;
	EquipmentContainer* statue_equip_container_;
	StatueGuildInfo     statue_guild_info_;
	//int                 m_ItemScore;
};

inline WzID StatuePlayer::get_stand_animation_id_() const
{
	if (stand_animation_id_ == 0)
	{
		return StrToWzID("N205");
	}

	return stand_animation_id_;
}

inline void StatuePlayer::set_stand_animation_id_(std::string animation_id)
{
	stand_animation_id_ = StrToWzID(animation_id.c_str());
}

inline WzID StatuePlayer::GetCurrentIdleAnimation()
{
	WzID animationID = StrToWzID("N205");
	return animationID;
}

inline WzID StatuePlayer::GetCurrentMoveAnimation()
{
	WzID animationID = StrToWzID("N205");
	return animationID;
}
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
