#include "SunClientPrecompiledHeader.h"
#include "OptionItem.h"

#include "TextureListInfoHandleManager.h"
#include "TextureListInfoParser.h"
#include "Mouse.h"


//////////////////////////////////////////////////////////////////////////////////////////
cOptionComboItem::cOptionComboItem()
{
    m_ItemIndex = 0;
}
void cOptionComboItem::SetNext()
{
    ++m_ItemIndex;
    if( m_ItemIndex >= m_ItemArray.size() )
        m_ItemIndex = 0;
}

void cOptionComboItem::SetPrev()
{
    --m_ItemIndex;
    if( m_ItemIndex >= m_ItemArray.size() )
        m_ItemIndex = m_ItemArray.size()-1;

}

void cOptionComboItem::Render( const RECT& DestRect )
{
	DWORD font_ID = StrToWzID("st11");
	g_pSunRenderer->x_pManagerTextOut->StartToUseFont(font_ID);

    RECT TitleRect = DestRect;
    TitleRect.left += 10;
	TitleRect.top -= 1;

    g_pSunRenderer->x_pManagerTextOut->DrawText( GetTitleText().c_str(), &TitleRect, 0xFFFFFFFF, WzColor_RGBA(0, 0, 0, 0), TP_HLEFT | TP_VCENTER);

    int ItemOffsetX = 261;
    int ItemOffsetY = 2;
    int ItemWidth = 118;
    int ItemHeight = 18;

    RECT ItemRect;
    ItemRect.left = DestRect.left + ItemOffsetX;
    ItemRect.top = DestRect.top + ItemOffsetY;
    ItemRect.right = ItemRect.left + ItemWidth;
    ItemRect.bottom = ItemRect.top + ItemHeight;

    g_pSunRenderer->x_pManagerTextOut->DrawText( m_ItemArray[m_ItemIndex].c_str(), &ItemRect, WzColor_RGBA(255,246,104,255), WzColor_RGBA(0, 0, 0, 0), TP_HCENTER | TP_VCENTER);

	g_pSunRenderer->x_pManagerTextOut->EndUseFont();
}

//////////////////////////////////////////////////////////////////////////////////////////
cOptionGaugeItem::cOptionGaugeItem() 
: m_Range(10)
, m_Position(10) 
, m_iMovePosition(MOVE_NONE)
{
}

void cOptionGaugeItem::SetNext()
{
    ++m_Position;
    if( m_Position >= m_Range )
        m_Position = m_Range;
}

void cOptionGaugeItem::SetPrev()
{
    --m_Position;
    if( m_Position >= m_Range )
        m_Position = 0;

}

void cOptionGaugeItem::Render( const RECT& DestRect )
{
	DWORD font_ID = StrToWzID("st11");
	g_pSunRenderer->x_pManagerTextOut->StartToUseFont(font_ID);
    {
        RECT TitleRect = DestRect;
        TitleRect.left += 10;
        g_pSunRenderer->x_pManagerTextOut->DrawText( GetTitleText().c_str(), &TitleRect, 0xFFFFFFFF, WzColor_RGBA(0, 0, 0, 0), TP_HLEFT | TP_VCENTER);
    }
	g_pSunRenderer->x_pManagerTextOut->EndUseFont();

    int ItemOffsetX = 261;
    int ItemOffsetY = 3;
    int ItemWidth = 118;
    int ItemHeight = 18;

    RECT ItemRect;
    ItemRect.left = DestRect.left + (ItemOffsetX+3);
    ItemRect.top = DestRect.top+ (ItemOffsetY+3);
    //ItemRect.right = ItemRect.left + ItemWidth;
    //ItemRect.bottom = ItemRect.top + ItemHeight;

    BASE_TextureListInfo* tex_info = NULL; 
    HANDLE tex_handle = TextureListInfoHandleManager::Instance()->GetTexture(10009, tex_info);
    if( tex_info != NULL && tex_handle != INVALID_HANDLE_VALUE )
    {
        float DrawWidth = (float)m_Position / (float)m_Range * (float)ItemWidth;
        float SrcWidth = (float)m_Position / (float)m_Range * (float)tex_info->tex_size.right;


		if(SrcWidth >= (float)tex_info->tex_size.right)
		{
			SrcWidth = (float)tex_info->tex_size.right;
		}

		if( DrawWidth >= (float)tex_info->tex_size.right)
		{
			DrawWidth = (float)tex_info->tex_size.right;
		}

        g_pSunRenderer->RenderTexture(tex_handle, 
            (float)ItemRect.left,
            (float)ItemRect.top,
            (float)DrawWidth,
            (float)11,
            (float)tex_info->tex_size.left,
            (float)tex_info->tex_size.top,
            (float)SrcWidth,
            (float)tex_info->tex_size.bottom,
            NULL);

		RECT SubRect;
		SubRect.left = (ItemRect.left + tex_info->tex_size.right/2) - 8;
		SubRect.top = ItemRect.top + 1;
		SubRect.bottom = SubRect.top + tex_info->tex_size.bottom;
		SubRect.right = SubRect.left + tex_info->tex_size.right/2;
		

		DWORD font_ID = StrToWzID("st08");
		g_pSunRenderer->x_pManagerTextOut->StartToUseFont(font_ID);

		float InScale = (float)m_Position/(float)m_Range;
		int   InPercent = (int)(InScale * 100);
		Sprintf(m_strItemPercentGauge,"%d%%",InPercent);
		g_pSunRenderer->x_pManagerTextOut->DrawText( m_strItemPercentGauge, &SubRect, 0xFFFFFFFF, WzColor_RGBA(0, 0, 0, 0), TP_HLEFT | TP_VCENTER , ETS_SHADOW);

		g_pSunRenderer->x_pManagerTextOut->EndUseFont();
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
cOptionCategory::cOptionCategory( const std::string& Title ) 
: m_Title(Title)
, m_Expended(true) 
{

}

void cOptionCategory::Render( const RECT& DestRect )
{
    {
        BASE_TextureListInfo* tex_info = NULL; 
        HANDLE tex_handle = TextureListInfoHandleManager::Instance()->GetTexture(10010, tex_info);
        if( tex_info != NULL && tex_handle != INVALID_HANDLE_VALUE )
        {
            g_pSunRenderer->RenderTexture(tex_handle, 
                (float)DestRect.left+2, 
                (float)DestRect.top-1, 
                (float)(DestRect.right-DestRect.left),
                (float)(DestRect.bottom-DestRect.top),
                (float)tex_info->tex_size.left,
				(float)tex_info->tex_size.top,
                (float)tex_info->tex_size.right,
                (float)tex_info->tex_size.bottom,
                NULL);
        }
    }


    {
        BASE_TextureListInfo* tex_info = NULL; 
        HANDLE tex_handle = TextureListInfoHandleManager::Instance()->GetTexture( IsExpended() == false ? 10002 : 10003, tex_info);
        if( tex_info != NULL && tex_handle != INVALID_HANDLE_VALUE )
        {
            g_pSunRenderer->RenderTexture(tex_handle, 
                (float)DestRect.left+2, 
                (float)DestRect.top+1, 
                (float)20,
                (float)20,
                (float)tex_info->tex_size.left,
                (float)tex_info->tex_size.top,
                (float)tex_info->tex_size.right,
                (float)tex_info->tex_size.bottom,
                NULL);
        }
    }

    {
        RECT dest = DestRect;
        dest.left += 24;
        dest.right -= 24;
		dest.top -= 1;

		DWORD font_ID = StrToWzID("st11");
		g_pSunRenderer->x_pManagerTextOut->StartToUseFont(font_ID);

        g_pSunRenderer->x_pManagerTextOut->DrawText( m_Title.c_str(),
            &dest,
            WzColor_RGBA(217,117,0,255),
            WzColor_RGBA(0, 0, 0, 0),
            TP_HLEFT | TP_VCENTER);

		g_pSunRenderer->x_pManagerTextOut->EndUseFont();
    }
}

cOptionCategory::~cOptionCategory()
{
	for (int i = 0 ; i < (int)m_OptionCategory.size() ; ++i)
	{
		delete m_OptionCategory[i];
		m_OptionCategory[i] = NULL;
	}
	m_OptionCategory.clear();
}

#include "GameOption.h"

namespace nOptionFacade
{
    nOptionType::eOptionTypeIndex GetOptionTypeIndexFromOptionID( DWORD OptionID )
    {
        switch( OptionID )
        {
        case 100:   return nOptionType::MAX;                            //�ػ󵵱���
        case 101:   return nOptionType::MAX;                            //�ػ󵵼���
        case 102:   return nOptionType::TEXTURE_QUALITY;                //�ؽ���ǰ��
        case 103:   return nOptionType::EFFECT_QUALITY;                 //����Ʈǰ��
        case 104:   return nOptionType::LOD_LEVEL;                      //���ι������(LOD)
        case 105:   return nOptionType::GAMMA_VALUE;                    //��⼳��(��������)
        case 106:   return nOptionType::SHOW_SHADOW;                    //�׸���ȿ��
        case 107:   return nOptionType::USE_BLUR;                       //�۷ο�ȿ��
        case 108:   return nOptionType::USE_FOG;                        //�Ȱ�ȿ��
        case 109:   return nOptionType::USE_TILE_BLENDING;              //Ÿ��ȿ��
        case 110:   return nOptionType::USE_MAP_DISTANCE_VIEW;          //����ȿ��
        case 111:   return nOptionType::USE_VOLUME_LIGHT;               //����ȿ��
        case 112:   return nOptionType::CULLING_DISTANCE_VALUE;         //�繰ǥ�ðŸ�(�ø�����)
        case 113:   return nOptionType::USE_MULTITHREAD;                //��Ƽ������
#ifdef _NA_007865_20141403_LIGHTMAP_BUILD_EXCLUDE_OBJECTS
        case 114:   return nOptionType::SHADOW_DISTANCE;                //�׸��ڰŸ�
#endif //_NA_007865_20141403_LIGHTMAP_BUILD_EXCLUDE_OBJECTS

        case 200:   return nOptionType::DAMAGE_TEXT_OUTPUT;             //���ؼ�ġǥ��
        case 201:   return nOptionType::SHOW_NPC_NAME;                  //NPC��
        case 202:   return nOptionType::SHOW_MONSTERINFO;               //��������
        case 203:   return nOptionType::SHOW_PLAYER_NAME;               //�ٸ��÷��̾��̸�
        case 204:   return nOptionType::HIDE_COSTUME;                    //�ڽ�Ƭ
        case 205:   return nOptionType::SHOW_MY_RENDER_INFO;            //�����̵�
        case 206:   return nOptionType::SHOW_PLAYER_GRADE;              //�ٸ��÷��̾���
        case 207:   return nOptionType::USE_HELP;                       //����������
        case 208:   return nOptionType::SHOW_PLAYER_PETNAME;            //�ָ�
        case 209:   return nOptionType::SHOW_VENDOR_TITLE;              //���λ�����
        case 210:   return nOptionType::SHOW_PLAYER_GUILD;              //����
        case 211:   return nOptionType::NOTIFY_DROP_ITEM;               //���������5�ʰ�ǥ��
        case 212:   return nOptionType::NOTIFY_PICKUP_ITEM;             //��������۾˸�
        case 213:   return nOptionType::USE_PLAY_MOVIE;                 //�����׵�����
        case 214:   return nOptionType::SHOW_PLAYERS;                   //�ٸ��÷��̾�ĳ����
        case 215:   return nOptionType::SHOW_VENDERS;                   //���λ���ĳ����
        case 216:   return nOptionType::SHOW_WING;                      //���󿡼��������̱�
        case 217:   return nOptionType::USE_AUTO_MOVE;                  //���ݽ��ڵ��̵�
        case 218:   return nOptionType::USE_AUTO_TARGET;                //�ڵ�Ÿ��
        case 219:   return nOptionType::USE_FORWARD_TARGET;             //����켱Ÿ��
        case 220:   return nOptionType::USE_FIXED_TARGET;               //�����켱Ÿ��
        case 221:   return nOptionType::USE_SKILLQUEUEING;              //��ųť��
#ifdef _YJ_PK_OPTION_001120
        case 222:   return nOptionType::USE_PK_WITH_CONTROL_KEY;        //�׻�PKŰ����
#endif //_YJ_PK_OPTION_001120
        case 223:   return nOptionType::MOUSE_SENS_VALUE;               //���콺��������
        case 224:   return nOptionType::MOUSE_VIEW_SPEED_VALUE;         //������ȯ�ӵ�
        case 225:   return nOptionType::MOUSE_VIEW_SENS_VALUE;          //���콺�����ӵ�
        case 226:   return nOptionType::CAMERA_VIEW_HEIGHT_OFFSET;      //ĳ���ͽ�������
        case 227:   return nOptionType::USE_CHANGE_EYE_MOUSE;           //���콺��/�����
        case 228:   return nOptionType::USE_MOVE_TO_CAMERA;             //ī�޶���������̵�
        case 229:   return nOptionType::USE_SPRING_CAMERA;              //������ī�޶�ȸ��
        case 230:   return nOptionType::USE_EXTENSION_CAMERA_DISTANCE;  //ī�޶�Ÿ���Ȯ��
        case 231:   return nOptionType::USE_BLOCK_TRADE;                //�ŷ�����
        case 232:   return nOptionType::USE_BLOCK_WHISPER;              //�ӼӸ�����
        case 233:   return nOptionType::PICKING_FRIEND;                 //�ٸ��÷��̾������
        case 234:   return nOptionType::USE_BLOCK_CHANNEL_INVITE;       //ä���ʴ�����
        case 235:   return nOptionType::USE_FIXED_UI_POSITION;          //�������̽�ũ�����
#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
		case 236:   return nOptionType::HEIM_TRADINGVOLUME;        //���� �ŷ��� �˸�
#endif // _NA_008678_20160203_ADD_HEIM_NOTIFICATION_OPTION

        case 300:   return nOptionType::BGM_VOLUME;                     //�����
        case 301:   return nOptionType::AMBIENT_VOLUME;                 //ȯ��ȿ����
        case 302:   return nOptionType::EFFECT_VOLUME;                  //�ý���ȿ����
        case 303:   return nOptionType::USE_SYSTEM_VOICE;               //�������
        case 304:   return nOptionType::USE_NPC_VOICE;                  //NPC����
        case 305:   return nOptionType::SYSTEM_VOLUME;                  //�ý��ۺ���
        case 306:   return nOptionType::SPEAKER_VOLUME;                 //����Ŀ����
#ifdef _NA_000000_20140408_SkILL_USE_AFTER_AUTOATTACK_OPTION
        //case 405:   return nOptionType::SKILL_USE_AFTER_AUTOATTACK;     //��ų ��� �� �ڵ�����
#endif //_NA_000000_20140408_SkILL_USE_AFTER_AUTOATTACK_OPTION
        }

        return nOptionType::MAX;
    }


    void RestoreOptionValue( cOptionItem* pItem )
    {
        CGameOption* pGameOption = CGameOption::Instance();

        if( pItem->GetOptionID() == 101 )
        {
            // �ػ� ���
            pItem->SetItemValue( pGameOption->GetGameOption().GetScreenIndex() );
        }
        else if( pItem->GetOptionID() == 100 )
        {
            // �ػ� ����
            pItem->SetItemValue( pGameOption->GetGameOption().GetScreenMode() );
        }
        else
        {
            if( pItem->GetOptionTypeIndex() == nOptionType::GAMMA_VALUE || 
                pItem->GetOptionTypeIndex() == nOptionType::CULLING_DISTANCE_VALUE )
            {
                pItem->SetItemValue( (DWORD)pGameOption->GetGameOption().GetOptionValueFloat( pItem->GetOptionTypeIndex() ) );
            }
            else if( pItem->GetOptionTypeIndex() != nOptionType::MAX )
            {
                pItem->SetItemValue( pGameOption->GetGameOption().GetOptionValue( pItem->GetOptionTypeIndex() ) );
            }
            else
            {
                pItem = pItem;
            }
        }
    }

    void SetOptionValue( cOptionItem* pItem )
    {
        CGameOption* pGameOption = CGameOption::Instance();
        
        if( pItem->GetOptionID() == 101 )
        {
            pGameOption->SetScreenIndex( pItem->GetItemValue() );
        }
        else if( pItem->GetOptionID() == 100 )
        {
            pGameOption->SetScreenMode( (OptionParam::OptionScreenMode)pItem->GetItemValue() );
        }
        else
        {
            if( pItem->GetOptionTypeIndex() == nOptionType::GAMMA_VALUE || 
                pItem->GetOptionTypeIndex() == nOptionType::CULLING_DISTANCE_VALUE )
            {
                pGameOption->GetGameOption().SetOptionValue( pItem->GetOptionTypeIndex(), pItem->GetItemValue() * 100 );
            }
            else if( pItem->GetOptionTypeIndex() != nOptionType::MAX )
            {
                pGameOption->GetGameOption().SetOptionValue( pItem->GetOptionTypeIndex(), pItem->GetItemValue() );
            }
            
            else
            {
                pItem = pItem;
            }
        }

    }


    int GetCurrentOption( DWORD OptionID )
    {
        return 0;
    }


};

