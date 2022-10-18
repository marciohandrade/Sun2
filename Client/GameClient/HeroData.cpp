#include "SunClientPrecompiledHeader.h"
#include "HeroData.h"
#include "Hero.h"
#include <../ProgramCommon/ItemInfoParser.h>
#include <../ProgramCommon/SkillInfoParser.h>

#include "CooltimeManager.h"

cHeroData::cHeroData()
{
    memset(&m_HeroInfo, 0, sizeof(m_HeroInfo) );
}

cHeroData::~cHeroData()
{
}

void cHeroData::Initialize()
{
}

void cHeroData::Release()
{
    SAFE_DELETE_ARRAY(m_HeroInfo.m_pItemStream);
}

void cHeroData::SetSkillTotalInfo( const SKILL_TOTAL_INFO* pSkillTotalInfo )
{
    memcpy( &m_HeroInfo.m_SkillTotalInfo, pSkillTotalInfo, pSkillTotalInfo->GetSize() );
}

void cHeroData::SetQuickTotalInfo( const QUICK_TOTAL_INFO* pQuickTotalInfo )
{
    memcpy( &m_HeroInfo.m_QuickTotalInfo, pQuickTotalInfo, pQuickTotalInfo->GetSize() );
}

void cHeroData::SetEquipItemInfo( const EQUIP_ITEM_INFO* pEquipItemInfo )
{
    memcpy( &m_HeroInfo.m_EquiptItemInfo, pEquipItemInfo, pEquipItemInfo->GetSize() );
}

void cHeroData::SetItemStream( const BYTE* pStreamBuffer, int StreamLength )
{
    SAFE_DELETE_ARRAY( m_HeroInfo.m_pItemStream );

    m_HeroInfo.m_pItemStream = new BYTE[MSG_CG_CHARINFO_CHAR_ITEM_CMD::_MAX_BINARY_SIZE];
    memcpy( m_HeroInfo.m_pItemStream, pStreamBuffer, StreamLength );

    m_HeroInfo.m_wStreamLength = StreamLength;
}

void cHeroData::SetHeroInfo( const CLIENT_CHARACTER_PART& CharacterPart, const PLAYERINFO_FOR_PACKET& PlayerInfo )
{
    m_BadgeItemData.Reset();
    m_PCBangItemData.Reset();
    
    // CLIENT_CHARACTER_PART
    m_HeroInfo.m_CharInfo.m_byClassCode		    = CharacterPart.m_byClass;
    m_HeroInfo.m_CharInfo.gender		        = CharacterPart.gender;

    m_HeroInfo.m_CharInfo.m_byFace                  = CharacterPart.m_byFace;
    m_HeroInfo.m_CharInfo.m_byHair                  = CharacterPart.m_byHair;
    m_HeroInfo.m_CharInfo.m_byHeight                = CharacterPart.m_byHeight;
    m_HeroInfo.m_CharInfo.m_bySlot				    = CharacterPart.m_bySlot;
    StrnCopy(m_HeroInfo.m_CharInfo.m_tszCharName, CharacterPart.m_tszCharName, MAX_CHARNAME_LENGTH);
    m_HeroInfo.m_CharInfo.m_tszCharName[_countof(m_HeroInfo.m_CharInfo.m_tszCharName) - 1] = '\0';
    m_HeroInfo.m_CharInfo.m_iRegion			        = CharacterPart.m_iRegion;
    m_HeroInfo.m_CharInfo.m_LV					    = CharacterPart.m_LV;
    m_HeroInfo.m_CharInfo.m_ChangeOfClassStep.Set(CharacterPart.m_ChangeOfClassStep);

    // BASE_PLAYERINFO
    m_HeroInfo.m_CharInfo.m_i64Exp				    = PlayerInfo.m_i64Exp;
    m_HeroInfo.m_CharInfo.m_iMaxHP				    = PlayerInfo.m_MaxHP;
    m_HeroInfo.m_CharInfo.m_iHP				        = PlayerInfo.m_HP;
    m_HeroInfo.m_CharInfo.m_iMaxMP				    = PlayerInfo.m_MaxMP;
    m_HeroInfo.m_CharInfo.m_iMP				        = PlayerInfo.m_MP;
    m_HeroInfo.m_CharInfo.m_Money				    = PlayerInfo.m_Money;
    m_HeroInfo.m_CharInfo.m_iRemainStat		        = PlayerInfo.m_dwRemainStat;
    m_HeroInfo.m_CharInfo.m_iRemainSkill		    = PlayerInfo.m_dwRemainSkill;
    m_HeroInfo.m_CharInfo.m_iSelectStyleCode	    = PlayerInfo.m_SelectStyleCode;
    m_HeroInfo.m_CharInfo.m_byPKState			    = PlayerInfo.m_byPKState;
    m_HeroInfo.m_CharInfo.m_byCharState		        = PlayerInfo.m_byCharState;
    StrnCopy(m_HeroInfo.m_CharInfo.m_tszTitleID,PlayerInfo.m_tszTitleID, MAX_TITLEID_LENGTH);
    m_HeroInfo.m_CharInfo.m_iInventoryLock		    = PlayerInfo.m_iInventoryLock;
    m_HeroInfo.m_CharInfo.m_sStrength			    = PlayerInfo.m_sStrength;
    m_HeroInfo.m_CharInfo.m_sDexterity			    = PlayerInfo.m_sDexterity;
    m_HeroInfo.m_CharInfo.m_sVitality			    = PlayerInfo.m_sVitality;
    m_HeroInfo.m_CharInfo.m_sInteligence		    = PlayerInfo.m_sInteligence;
    m_HeroInfo.m_CharInfo.m_sSpirit			        = PlayerInfo.m_sSpirit;
    m_HeroInfo.m_CharInfo.m_sSkillStat1		        = PlayerInfo.m_sSkillStat1;
    m_HeroInfo.m_CharInfo.m_sSkillStat2		        = PlayerInfo.m_sSkillStat2;
    m_HeroInfo.m_CharInfo.m_Money				    = PlayerInfo.m_Money;
    m_HeroInfo.m_CharInfo.m_bWhisperFlag		    = PlayerInfo.m_bWhisperFlag;
    m_HeroInfo.m_CharInfo.m_bTradeFlag			    = PlayerInfo.m_bTradeFlag;
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    m_HeroInfo.m_CharInfo.honor_title_              = PlayerInfo.m_wHonorTitle;
#endif //_NA_003027_20111013_HONOR_SYSTEM
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    m_HeroInfo.m_CharInfo.cur_FP_                   = PlayerInfo.cur_FP;
    m_HeroInfo.m_CharInfo.max_FP_                   = PlayerInfo.max_FP;
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

    m_HeroInfo.m_CharInfo.m_PlayLimitedTime         = PlayerInfo.m_PlayLimitedTime;

    StrnCopy(m_HeroInfo.m_CharInfo.m_tszGuildName, PlayerInfo.m_tszGuildName, MAX_GUILDNAME_LENGTH );

    m_HeroInfo.m_CharInfo.m_eGuildDuty			    = (eGUILD_DUTY)PlayerInfo.m_GuildDuty;
    m_HeroInfo.m_CharInfo.m_GuildGuid			    = PlayerInfo.m_GuildGuid;
    m_HeroInfo.m_GuildRenderInfo.m_GuildDuty = m_HeroInfo.m_CharInfo.m_eGuildDuty;
    m_HeroInfo.m_GuildRenderInfo.m_GuildGuid = m_HeroInfo.m_CharInfo.m_GuildGuid;
    m_HeroInfo.m_CharInfo.m_UserPoint			    = PlayerInfo.m_UserPoint;

    

    m_HeroInfo.m_byGMGrade                          = PlayerInfo.m_byGMGrade;
#ifdef _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
    m_HeroInfo.m_bHideHelmet                        = PlayerInfo.VisibleOptionFlag.m_Helmet;
    m_HeroInfo.m_bHideWing                          = PlayerInfo.VisibleOptionFlag.m_Wing;
#else
    m_HeroInfo.m_bHideHelmet                        = PlayerInfo.m_byInvisibleOptFlag;
#endif//_NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
#ifdef _NA_006683_20130502_EQUIPMENTS_VISIBLE_COSTUME
    m_HeroInfo.m_bHideCostume                       = PlayerInfo.VisibleOptionFlag.m_Costume;
#endif
    m_HeroInfo.m_byPCBang                           = PlayerInfo.m_byPCBangUser;
    m_HeroInfo.m_byBlockPCBang                      = PlayerInfo.m_byPCRoomSts;

    m_HeroInfo.m_CharInfo.m_ChaoState = PlayerInfo.m_ChaoState;
    m_HeroInfo.m_CharInfo.m_ExtraInventoryTabCount  = PlayerInfo.m_ExtraInventoryTabCount;	
    m_HeroInfo.m_CharInfo.m_ExtraEquipCashSlotCount = PlayerInfo.m_ExtraEquipCashSlotCount;

    m_HeroInfo.m_CharInfo.m_bSummonedPet			= PlayerInfo.m_bSummonedPet;
    m_HeroInfo.m_CharInfo.m_bMAXIntimacy			= PlayerInfo.m_bMAXIntimacy;
    m_HeroInfo.m_CharInfo.m_PetItemPos				= PlayerInfo.m_PetItemPos;
    m_HeroInfo.m_CharInfo.m_bMAXOptionRemainTime	= PlayerInfo.m_bMAXOptionRemainTime;

    m_HeroInfo.m_CharInfo.m_bActiveEtherDevice		= PlayerInfo.m_bActiveEtherDevice;
    m_HeroInfo.m_CharInfo.m_byEtherBulletPos		= PlayerInfo.m_byEtherBulletPos;
    m_HeroInfo.m_CharInfo.m_iMaxSD					= PlayerInfo.m_MaxSD;
    m_HeroInfo.m_CharInfo.m_iSD					    = PlayerInfo.m_SD;

#ifdef _PCBANG_POLICY_CHANGE_POST_EXPIRED
    m_HeroInfo.m_bPCBangStatus = 0;
#endif //_PCBANG_POLICY_CHANGE_POST_EXPIRED

    m_HeroInfo.m_CharInfo.m_byFreeInitCount         = PlayerInfo.m_byFreeInitCount;

#ifdef _NA_004035_20120227_COSTUME_ITEM
    m_HeroInfo.m_CharInfo.is_equipped_costume_      = CharacterPart.is_equipped_costume;
    m_HeroInfo.m_CharInfo.costume_item_pos_         = CharacterPart.costume_item_pos;
    m_HeroInfo.m_CharInfo.costume_item_code_        = CharacterPart.costume_item_code;

    if( m_HeroInfo.m_CharInfo.is_equipped_costume_ == false )
    {
        m_HeroInfo.m_CharInfo.costume_item_pos_         = INVALID_SLOT_POS;
        m_HeroInfo.m_CharInfo.costume_item_code_        = 0;
    }
#endif//_NA_004035_20120227_COSTUME_ITEM
#ifdef _NA_008633_20160104_ADD_DECORATECOSTUME
	m_HeroInfo.m_CharInfo.is_equipped_decorate_costume_ = CharacterPart.is_equipped_decorate_costume;
	m_HeroInfo.m_CharInfo.decorate_costume_item_pos_    = CharacterPart.decorate_costume_item_pos;
	m_HeroInfo.m_CharInfo.decorate_costume_item_code_   = CharacterPart.decorate_costume_item_code;

	if ( m_HeroInfo.m_CharInfo.is_equipped_decorate_costume_ == false )
	{
		m_HeroInfo.m_CharInfo.decorate_costume_item_pos_  = INVALID_SLOT_POS;
		m_HeroInfo.m_CharInfo.decorate_costume_item_code_ = 0;
	}
#endif // _NA_008633_20160104_ADD_DECORATECOSTUME
    m_HeroInfo.m_FightEnergy = 0;
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE
	m_HeroInfo.client_player_ranking_grade_info_.ranking_type = eSUN_RANKING_TYPE_END;
	m_HeroInfo.client_player_ranking_grade_info_.grade = eSUN_RANKING_GRADE_NONE;
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    m_HeroInfo.m_spa_item_code = 0;
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
}

void cHeroData::SaveHero( Hero* pHero )
{
    m_HeroInfo.m_dwKey		= pHero->GetObjectKey();
    m_HeroInfo.m_MakeType	= pHero->GetPlayerType();
    memcpy(&m_HeroInfo.m_CharInfo, pHero->GetCharInfo(), sizeof(m_HeroInfo.m_CharInfo));

    pHero->GetEquipItemTotalInfo(m_HeroInfo.m_EquiptItemInfo);
    pHero->GetInventoryTotalInfo(m_HeroInfo.m_pItemStream, m_HeroInfo.m_wStreamLength);	
    pHero->GetQuickTotalInfo(m_HeroInfo.m_QuickTotalInfo);	

    m_HeroInfo.m_byGMGrade     = pHero->GetGMGrade();
    m_HeroInfo.m_bHideHelmet   = pHero->IsHideHelmet();
#ifdef _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
    m_HeroInfo.m_bHideWing     = pHero->IsHideWing();
#endif//_NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
    m_HeroInfo.m_byPCBang      = pHero->Player::GetPCBangMode();
    m_HeroInfo.m_bObserverMode = pHero->IsObserverMode();
#ifdef _NA_006683_20130502_EQUIPMENTS_VISIBLE_COSTUME
    m_HeroInfo.m_bHideCostume = pHero->isHideCostume();
#endif

    //===============================
    //	펫관련 정보 저장
    //===============================
    m_HeroInfo.m_CharInfo.m_bSummonedPet =  pHero->IsPetReturn();
    m_HeroInfo.m_CharInfo.m_PetItemPos	  =	 pHero->GetPetItemPos();


    //===============================
    //	에테르 탄환정보 저장
    //===============================
    //에테르5차개선안
    m_HeroInfo.m_CharInfo.m_bActiveEtherDevice = pHero->GetEthreInfo().byEtherIndex;
    m_HeroInfo.m_CharInfo.m_byEtherBulletPos   = pHero->GetEthreInfo().byEtherChargePos;

    pHero->SetEtherItemLock(TRUE,FALSE);


    memcpy(&m_HeroInfo.m_GuildRenderInfo, &pHero->GetGuildRenderInfo(), sizeof(PLAYER_GUILD_RENDER_INFO));
    STLX_VECTOR<MAPCODE>& domination_state_info = pHero->GetDominationStateInfo();
    BYTE size_of_info = domination_state_info.size();
    m_HeroInfo.m_GuildRenderInfo.m_stateTotalInfo.m_Count = size_of_info;
    for (BYTE index = 0; index < size_of_info; ++index)
    {
        m_HeroInfo.m_GuildRenderInfo.m_stateTotalInfo.m_Slot[index].m_wDominationCode = domination_state_info[index];
    }

#ifdef _PCBANG_POLICY_CHANGE_POST_EXPIRED
    m_HeroInfo.m_bPCBangStatus = pHero->GetPCRoomStatus();
#endif //_PCBANG_POLICY_CHANGE_POST_EXPIRED

    m_HeroInfo.m_FightEnergy = pHero->GetFightingEnergy();

    // 2012.07.03 naklu / 용변신 상태에서 맵 이동한 경우 스타일이 초기화 되지 않던 문제 수정
    // SaveHero() 중 용변신 상태면 용변신 이전 스타일로 저장함
    if (m_HeroInfo.m_CharInfo.m_byClassCode == eCHAR_DRAGON)
    {
        if ((m_HeroInfo.m_CharInfo.m_iSelectStyleCode == eSTYLE_DRAGON_TRANSFORM1) ||
            (m_HeroInfo.m_CharInfo.m_iSelectStyleCode == eSTYLE_DRAGON_TRANSFORM2))
        {
            m_HeroInfo.m_CharInfo.m_iSelectStyleCode = m_HeroInfo.m_OldAttackStyle;
        }
    }

#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE
	pHero->ClearClientPlayerRankingGradeInfo();
	CLIENT_PLAYER_RANKING_GRADE_INFO player_ranking_grade_info;
	player_ranking_grade_info.ranking_type = m_HeroInfo.client_player_ranking_grade_info_.ranking_type;
	player_ranking_grade_info.grade = m_HeroInfo.client_player_ranking_grade_info_.grade;
	pHero->SetClientPlayerRankingGradeInfo(&player_ranking_grade_info);
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE

#ifdef _NA_004035_20120227_COSTUME_ITEM
    m_HeroInfo.m_spa_item_code = pHero->GetSpaItemCode();
#endif //_NA_004035_20120227_COSTUME_ITEM

}

//------------------------------------------------------------------------------ 
void cHeroData::ReleaseGuildInfo()
{
    ZeroMemory(&m_HeroInfo.m_GuildRenderInfo, sizeof(GUILD_RENDER_INFO));
}

#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE
void cHeroData::SetClientPlayerRankingGrade( const CLIENT_PLAYER_RANKING_GRADE_INFO& client_player_ranking_grade_info )
{
	CopyMemory(&m_HeroInfo.client_player_ranking_grade_info_, &client_player_ranking_grade_info, sizeof(CLIENT_PLAYER_RANKING_GRADE_INFO));
}
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE