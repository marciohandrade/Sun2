#include "SunClientPrecompiledHeader.h"
#include "SkillEventEffectInfo.h"


cSkillEventEffectInfoData::cSkillEventEffectInfoData()
: m_EventType( EVENT_TYPE_MAX )
, m_PositionType( POSITION_TYPE_MAX )
, m_ScaleType( SCALE_TYPE_NONE )
, m_PositionData( eSKILLEFFECT_POS_NONE, 0 )
, m_StaticRotate(FALSE)
, m_CameraViberationType(Camera::VIBE_TYPE_NONE)
, m_CameraViberationPower(0.0f)
, m_ActiveDelayTime(0)
{
}

cSkillEventEffectInfo::cSkillEventEffectInfo()
{
}

cSkillEventEffectInfoData* cSkillEventEffectInfo::AddData( const std::string& EventType, const std::string& PositionType, const std::string& ScaleType, LPCTSTR EventCode )
{
    cSkillEventEffectInfoData* data = new cSkillEventEffectInfoData();

    if( EventType == "START" )
        data->SetEventType( cSkillEventEffectInfoData::EVENT_TYPE_START );
    else if( EventType == "END" )
        data->SetEventType( cSkillEventEffectInfoData::EVENT_TYPE_END );
    else if( EventType == "BUFF" )
        data->SetEventType( cSkillEventEffectInfoData::EVENT_TYPE_BUFF );
    else if( EventType == "DAMAGE" )
        data->SetEventType( cSkillEventEffectInfoData::EVENT_TYPE_DAMAGE );
    else if( EventType == "ANIMATION_EVENT" )
        data->SetEventType( cSkillEventEffectInfoData::EVENT_TYPE_ANIMATION_EVENT );
    else if( EventType == "FIELD" )
        data->SetEventType( cSkillEventEffectInfoData::EVENT_TYPE_FIELD );
    else if( EventType.find( "EVENT_" ) == 0 )
    {
        std::string temp = EventType.c_str() + strlen("EVENT_");
        int number = atoi(temp.c_str());
        data->SetEventType( cSkillEventEffectInfoData::EVENT_TYPE_ANIMATION_EVENT );
        data->SetHitEventNumber( number );
    }

    if( PositionType == "TARGET_ARROW" )
        data->SetPositionType( cSkillEventEffectInfoData::POSITION_TYPE_TARGET_ARROW );
    else if( PositionType == "TARGET_POSITION" )
        data->SetPositionType( cSkillEventEffectInfoData::POSITION_TYPE_TARGET_POSITION );
    else if( PositionType == "USER_POSITION" )
        data->SetPositionType( cSkillEventEffectInfoData::POSITION_TYPE_USER_POSITION );
    else if( PositionType == "USER_ARROW" )
        data->SetPositionType( cSkillEventEffectInfoData::POSITION_TYPE_USER_ARROW_POSITION );
    else if( PositionType == "POS_HEAD" )
    {
        data->SetPositionType( cSkillEventEffectInfoData::POSITION_TYPE_CUSTOM );
        data->SetPositionData( cSkillPositionData( eSKILLEFFECT_POS_HEAD, 0 ) );
    }
    else if( PositionType == "POS_BODY" )
    {
        data->SetPositionType( cSkillEventEffectInfoData::POSITION_TYPE_CUSTOM );
        data->SetPositionData( cSkillPositionData( eSKILLEFFECT_POS_BODY, 0 ) );
    }
    else if( PositionType == "POS_FOOT" )
    {
        data->SetPositionType( cSkillEventEffectInfoData::POSITION_TYPE_CUSTOM );
        data->SetPositionData( cSkillPositionData( eSKILLEFFECT_POS_FOOT, 0 ) );
    }
    else if( PositionType == "POS_RHAND" )
    {
        data->SetPositionType( cSkillEventEffectInfoData::POSITION_TYPE_CUSTOM );
        data->SetPositionData( cSkillPositionData( eSKILLEFFECT_POS_RHAND, 0 ) );
    }
    else if( PositionType == "POS_LHAND" )
    {
        data->SetPositionType( cSkillEventEffectInfoData::POSITION_TYPE_CUSTOM );
        data->SetPositionData( cSkillPositionData( eSKILLEFFECT_POS_LHAND, 0 ) );
    }
    else if( PositionType == "POS_WEAPON" )
    {
        data->SetPositionType( cSkillEventEffectInfoData::POSITION_TYPE_CUSTOM );
        data->SetPositionData( cSkillPositionData( eSKILLEFFECT_POS_WEAPON, 0 ) );
    }
    else if( PositionType == "POS_LRHAND" )
    {
        data->SetPositionType( cSkillEventEffectInfoData::POSITION_TYPE_CUSTOM );
        data->SetPositionData( cSkillPositionData( eSKILLEFFECT_POS_HEAD, 0 ) );
    }
    else if( PositionType == "POS_BUST" )
    {
        data->SetPositionType( cSkillEventEffectInfoData::POSITION_TYPE_CUSTOM );
        data->SetPositionData( cSkillPositionData( eSKILLEFFECT_POS_BUST, 0 ) );
    }
    else if( PositionType == "POS_BOTTOM" )
    {
        data->SetPositionType( cSkillEventEffectInfoData::POSITION_TYPE_CUSTOM );
        data->SetPositionData( cSkillPositionData( eSKILLEFFECT_POS_BOTTOM, 0 ) );
    }
    else if( PositionType == "POS_LAND" )
    {
        data->SetPositionType( cSkillEventEffectInfoData::POSITION_TYPE_CUSTOM );
        data->SetPositionData( cSkillPositionData( eSKILLEFFECT_ONLY_POS, 0 ) );
    }
    else if( PositionType == "POS_TARGET" )
    {
        data->SetPositionType( cSkillEventEffectInfoData::POSITION_TYPE_CUSTOM );
        data->SetPositionData( cSkillPositionData( eSKILLEFFECT_TARGET_POS, 0 ) );
    }
    else if( PositionType.find( "POS_BONE_" ) == 0 )
    {
        std::string temp = PositionType.c_str() + strlen("POS_BONE_");
        int number = atoi(temp.c_str());
        data->SetPositionType( cSkillEventEffectInfoData::POSITION_TYPE_CUSTOM );
        data->SetPositionData( cSkillPositionData( eSKILLEFFECT_POS_BONE_INDEX, number ) );
    }

    if( ScaleType == "USER" )
        data->SetScaleType( cSkillEventEffectInfoData::SCALE_TYPE_USER );
    else if( ScaleType == "TARGET" )
        data->SetScaleType( cSkillEventEffectInfoData::SCALE_TYPE_TARGET );


    if( PositionType.find( "CAMERA_V_" ) == 0 )
    {
        std::string temp = PositionType.c_str() + strlen("CAMERA_V_");
        int number = atoi(temp.c_str());
        data->SetCameraViberationType( Camera::VIBE_TYPE_VERTICAL );
        data->SetCameraViberationPower( (float)(number) * 0.01f );
        data->SetActionType( cSkillEventEffectInfoData::ACTION_TYPE_SHAKE_CAMERA );
    }
    else if( PositionType.find( "CAMERA_H_" ) == 0 )
    {
        std::string temp = PositionType.c_str() + strlen("CAMERA_H_");
        int number = atoi(temp.c_str());
        data->SetCameraViberationType( Camera::VIBE_TYPE_HORISONTAL );
        data->SetCameraViberationPower( (float)(number) * 0.01f );
        data->SetActionType( cSkillEventEffectInfoData::ACTION_TYPE_SHAKE_CAMERA );
    }
    else if( PositionType.find( "CAMERA_A_" ) == 0 )
    {
        std::string temp = PositionType.c_str() + strlen("CAMERA_A_");
        int number = atoi(temp.c_str());
        data->SetCameraViberationType( Camera::VIBE_TYPE_ALL );
        data->SetCameraViberationPower( (float)(number) * 0.01f );
        data->SetActionType( cSkillEventEffectInfoData::ACTION_TYPE_SHAKE_CAMERA );
    }

    if( EventCode != "NULL")
        data->SetActionType( cSkillEventEffectInfoData::ACTION_TYPE_CREATEEFFECT );
    data->SetEventCode( StrToWzID(EventCode) );

    m_DataArray.push_back( data );

    return data;
}

cSkillEventEffectInfo::~cSkillEventEffectInfo()
{
	//메모리릭 정리.
	for (int i = 0 ; i < (int)m_DataArray.size() ; ++i)
	{
		delete m_DataArray[i];
		m_DataArray[i] = NULL;
	}

	m_DataArray.clear();
}



