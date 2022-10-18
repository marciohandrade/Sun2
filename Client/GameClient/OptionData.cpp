#include "SunClientPrecompiledHeader.h"
#include "OptionData.h"

#include "ChatMsg.h"


namespace nOptionType
{
    std::vector<std::string> m_OptionSaveNameArray;
    std::vector<int> m_DefaultOptionValueArray;

    void AddOptionData( LPCTSTR pName, int DefaultValue )
    {
        m_OptionSaveNameArray.push_back( pName );
        m_DefaultOptionValueArray.push_back( DefaultValue );
    }

    LanguageType GetDefaultLanguageType()
    {
        LanguageType language_type = kLanguageGlobal;

#if defined(__KR) || defined(_KOREA)
        language_type = kLanguageKorea;
#elif defined ( _CHINA )
        language_type = kLanguageChina;
        if (PROGRAMPARAM->nation_type() != kLanguageInvalid)
        {
            language_type = PROGRAMPARAM->nation_type();
        }
#elif defined( _JAPAN )
        language_type = kLanguageJapan;
#elif defined( _GERMANY )
        language_type = kLanguageGermany;
#elif defined( _RUSSIA )
        language_type = kLanguageRussia;
#else   //defined( _GLOBAL )  
        language_type = kLanguageGlobal;
#endif

        return language_type;

    }


    void MakeOptionNameArray()
    {
        // eOptionTypeIndex 와 반드시 순서가 일치해야한다

        // 1
        //AddOptionData( _T("ScreenIndex"); // m_OptionParam.iScreenIndex );
#ifdef _JAPAN
        AddOptionData( "TextureQuality", 0); // m_OptionParam.iTextureQuality );
#else
        AddOptionData( "TextureQuality", 1); // m_OptionParam.iTextureQuality );
#endif
        AddOptionData( "LOD", 0); // m_OptionParam.LOD );
        AddOptionData( "NormalMapQuality", 0); // m_OptionParam.iNormalMapQuality );
        AddOptionData( "ShadowQuality", 0); // m_OptionParam.iShadowQuality );
        //AddOptionData( "ScreenMode"); // m_OptionParam.screen_mode );

        // 2. 이펙트 관련
#ifdef _RUSSIA
        AddOptionData( "BlurOption", 0); // m_OptionParam.bBlurState );
#else
        AddOptionData( "BlurState", 1); // m_OptionParam.bBlurState );
#endif //_RUSSIA
        AddOptionData( "FogState", 1); // m_OptionParam.bFogState );
        AddOptionData( "MapSpecular", 1); // m_OptionParam.bMapDistantView );
        AddOptionData( "SystemVoice", 1); // m_OptionParam.bSystemVoice );
        AddOptionData( "NPCVoice", 1); // m_OptionParam.bNPCVoice );
        AddOptionData( "TileBlend", 1); // m_OptionParam.bTileBlending );
        AddOptionData( "VolumeLight", 0); // m_OptionParam.volume_light);

        //AddOptionData( "CameraViewPos"); // m_OptionParam.camera_view_pos);

        // 3.사운드 음량
        AddOptionData( "SystemVolume", 100); // m_OptionParam.fSystemVolume );
        AddOptionData( "SpeakerVolume", 100); // m_OptionParam.fSpeakerVolume );
        AddOptionData( "BGMVolume", 100); // m_OptionParam.fBGMVolume );
        AddOptionData( "AmbientEffectVolume", 100); // m_OptionParam.fAEffVolume );
        AddOptionData( "EffectVolume", 100); // m_OptionParam.fEffVolume );

        // 4. 게임 옵션
        AddOptionData("EffectQuality", 0); // m_OptionParam.iEffectQuality);
        AddOptionData("DamageOutput", 1); // m_OptionParam.iDamageOutput);

        // 5
        AddOptionData("ShowPlayerName", 1); // m_OptionParam.bShowPlayerName);
        AddOptionData("ShowPlayerGrade", 1); // m_OptionParam.bShowGrade);
        AddOptionData("ShowPlayerGuild", 1); // m_OptionParam.bShowGuild);
        AddOptionData("ShowPetName", 1); // m_OptionParam.bShowPetName);

        // 6
#ifdef __CN_ADD_HEAD_EFFECT_FLAG
        AddOptionData( "ShowHeadEffect", 1); // m_OptionParam.bShowHeadEffect);
#endif//__CN_ADD_HEAD_EFFECT_FLAG(INVEN악세사리이펙트버튼)
        AddOptionData( "ShowNPCName", 1); // m_OptionParam.bShowNPCName );
        AddOptionData( "ShowHelm", 1); // m_OptionParam.bShowHelm );
#ifdef _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
        AddOptionData( "ShowWing", 1); // m_OptionParam.bShowWing);
#endif//_NA_000000_20120420_EQUIPMENTS_VISIBLE_WING

        // 7
        AddOptionData( "AutoMove", 1); // m_OptionParam.bAutoMove);
        AddOptionData( "ShowHelp", 1); // m_OptionParam.bHelp );
        AddOptionData( "SkillQueueing", 1); // m_OptionParam.bSkillQueueing );
        AddOptionData( "FixUI", 0); //m_OptionParam.bFixUI);
        AddOptionData( "AutoTarget", 1); // m_OptionParam.bAutoTarget );
        AddOptionData( "ForwardTarget", 0); // m_OptionParam.bForwardTarget);
        AddOptionData( "FixTarget", 0); // m_OptionParam.bFixTarget );
        AddOptionData( "ChangeEyeMouse", 0); //m_OptionParam.bChangeEyeMouse);
        AddOptionData( "MoveToCamera", 1); // m_OptionParam.bMoveToCamera);
        AddOptionData( "SpringCamera", 0); //m_OptionParam.bSpringCamera);
        AddOptionData( "BlockWhisper", 0); //m_OptionParam.bSpringCamera);
        AddOptionData( "BlockTrade", 0); //m_OptionParam.bSpringCamera);
        AddOptionData( "ChannelAccept", 0); //m_OptionParam.bSpringCamera);

        // 8
#ifdef _YJ_PK_OPTION_001120
        AddOptionData( "PKWithoutCtrlKey", 0); // m_OptionParam.bPKWithoutCtrlKey);
#endif //_YJ_PK_OPTION_001120
        AddOptionData( "PickUpItemNotify", 1); // m_OptionParam.bPickUpItemNotify );
        AddOptionData( "NotifyDropItem", 0); // m_OptionParam.bNotifyDropItem );

        // 9
        AddOptionData( "ShowPlayers", 1); // m_OptionParam.bShowPlayers );
        AddOptionData( "ShowVendors", 1); // m_OptionParam.bShowVendors );
        AddOptionData("DisplayMonsterInfo", 1); // m_OptionParam.bDisplayMonsterInfo);
        AddOptionData("ShowMyRnederInfo", 1); // m_OptionParam.bShowMyRenderInfo);

        // 10
        AddOptionData( "MouseSens", 3); // m_OptionParam.iMouseSens );
        AddOptionData( "MouseViewSens", 3); // m_OptionParam.iMouseViewSens );
        AddOptionData( "MouseViewSpeed", 3); // m_OptionParam.iMouseViewSpeed );

        // 11
        AddOptionData( "ShadowON", 1); // m_OptionParam.bShadowON );
        AddOptionData( "GammaRate", 25500); // m_OptionParam.fGamma );
        AddOptionData( "CameraExtensionLen", 1); // , m_OptionParam.bCameraDistanceExtension );

        // 12
        AddOptionData( "MoviePlay", 1); // , m_OptionParam.bMoviePlay );
        AddOptionData( "MultiLoding", 1); // , m_OptionParam.bMultiThread );

        AddOptionData( "MarketText", 1); // m_OptionParam.bMarketTextDraw );
        AddOptionData( "CullLength", 100); // m_OptionParam.fCullLength);

        // 13
        int	DefaultChatFilter = (CHAT_FILTER_TYPE_GENERAL |CHAT_FILTER_TYPE_PARTY|
            CHAT_FILTER_TYPE_GUILD |CHAT_FILTER_TYPE_CHANNEL| CHAT_FILTER_TYPE_WHISPER ) ;
        DefaultChatFilter |= CHAT_FILTER_TYPE_SHOUT;

        AddOptionData("NoramlChat", DefaultChatFilter); // ,  m_OptionParam.m_ChatFilter_Normal);
        AddOptionData("PartyChat", DefaultChatFilter); // ,   m_OptionParam.m_ChatFilter_Party);
        AddOptionData("GuildChat", DefaultChatFilter); // ,   m_OptionParam.m_ChatFilter_Guild);
        AddOptionData("ChannelChat", DefaultChatFilter); // , m_OptionParam.m_ChatFilter_Channel);
        AddOptionData("SystemChat", CHAT_FILTER_TYPE_SYSTEM | CHAT_FILTER_TYPE_WHISPER); // ,  m_OptionParam.m_ChatFilter_System);
#ifdef	_K2P_ADD_WISPER_WINDOW
        AddOptionData("WisperChat", CHAT_FILTER_TYPE_WHISPER); // ,  m_OptionParam.m_ChatFilter_Wsiper);
#endif // _K2P_ADD_WISPER_WINDOW

        // 14
        AddOptionData("LanguageType", (int)GetDefaultLanguageType() ); // m_OptionParam.m_LanguageType);

        AddOptionData("PetGaugeShowType", 0); // m_OptionParam.petgauge_showtype);

        AddOptionData( "QuickSlotSize", 2); // m_OptionParam.quickslot_size );
        AddOptionData( "QuickSlotIndex", 0); // m_OptionParam.quickslot_index );

        //AddOptionData("ChannelAccept"); // m_OptionParam.channel_accept);

        // 15
#ifdef _DH_BATTLEZONE2_
        AddOptionData("PickingFriend", 1); // m_OptionParam.picking_friend);
#endif //_DH_BATTLEZONE2_

        AddOptionData("CameraHeightOffset", 0);

#ifdef _NA_006683_20130502_EQUIPMENTS_VISIBLE_COSTUME
        AddOptionData("HideCostume", 1);
#endif

#ifdef _NA_000000_20140408_SkILL_USE_AFTER_AUTOATTACK_OPTION
        //AddOptionData("SkillUseAfterAutoAttack", 1);    // m_OptionParam.GetOptionValue( nOptionType::SKILL_USE_AFTER_AUTOATTACK )
#endif //_NA_000000_20140408_SkILL_USE_AFTER_AUTOATTACK_OPTION

#ifdef _NA_007865_20141403_LIGHTMAP_BUILD_EXCLUDE_OBJECTS
        AddOptionData("ShaowDistance", 30);
#endif //_NA_007865_20141403_LIGHTMAP_BUILD_EXCLUDE_OBJECTS

#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
		AddOptionData("HeimTradingvolume", 1);
#endif // _NA_008678_20160203_ADD_HEIM_NOTIFICATION_OPTION

        // -d-ffasl;kdfl;asjf;aljfka
        assert( m_OptionSaveNameArray.size() == MAX );
    }

    LPCTSTR GetOptionTypeName( eOptionTypeIndex TypeIndex )
    {
        if( m_OptionSaveNameArray.empty() )
        {
            MakeOptionNameArray();
        }

        return m_OptionSaveNameArray[TypeIndex].c_str();
    }

    int GetDefaultOptionValue( eOptionTypeIndex TypeIndex )
    {
        if( m_OptionSaveNameArray.empty() )
        {
            MakeOptionNameArray();
        }

        return m_DefaultOptionValueArray[TypeIndex];
    }
}

#include "GameParameter.h"
void OptionParam::Save(CGameParameter* pParameter )
{
    for( size_t i=0;i!=nOptionType::MAX;++i)
    {
        nOptionType::eOptionTypeIndex TypeIndex = (nOptionType::eOptionTypeIndex)i;
        LPCTSTR pKeyName = nOptionType::GetOptionTypeName(TypeIndex);

        switch( i )
        {
        case nOptionType::GAMMA_VALUE:
        case nOptionType::SYSTEM_VOLUME:
        case nOptionType::SPEAKER_VOLUME:
        case nOptionType::BGM_VOLUME:
        case nOptionType::AMBIENT_VOLUME:
        case nOptionType::EFFECT_VOLUME:
        case nOptionType::CULLING_DISTANCE_VALUE:
            {
                float v = (float)m_OptionValue[i] * 0.01f;
                pParameter->WriteFloat( pKeyName, v, NULL );
                break;
            }
        default:
            {
                pParameter->WriteInt( pKeyName, m_OptionValue[i], NULL );
                break;
            }
        }
    }
}

void OptionParam::Load(CGameParameter* pParameter )
{
    for( size_t i=0;i!=nOptionType::MAX;++i)
    {
        nOptionType::eOptionTypeIndex TypeIndex = (nOptionType::eOptionTypeIndex)i;
        LPCTSTR pKeyName = nOptionType::GetOptionTypeName(TypeIndex);

        switch( i )
        {
        case nOptionType::SYSTEM_VOLUME:
        case nOptionType::SPEAKER_VOLUME:
        case nOptionType::BGM_VOLUME:
        case nOptionType::AMBIENT_VOLUME:
        case nOptionType::EFFECT_VOLUME:
        case nOptionType::CULLING_DISTANCE_VALUE:
        case nOptionType::GAMMA_VALUE:
            {
                float v = pParameter->ReadFloat( pKeyName, 9898.0f, NULL );
                if( v == 9898.0f )
                {
                    m_OptionValue[i] = nOptionType::GetDefaultOptionValue(TypeIndex);
                }
                else
                {
                    m_OptionValue[i] = (int)(v * 100.0f);
                }

                break;
            }
        default:
            {
                m_OptionValue[i] = pParameter->ReadInt( pKeyName, 9898, NULL );
                if( m_OptionValue[i] == 9898 )
                    m_OptionValue[i] = nOptionType::GetDefaultOptionValue(TypeIndex);
                break;
            }
        }
    }
}

