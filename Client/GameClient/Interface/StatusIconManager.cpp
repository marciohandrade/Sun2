#include "SunClientPrecompiledHeader.h"
#include "StatusIconManager.h"
#include "StateInfoParser.h"
#include "GraphicResourceList.h"
#include "uiHeroGaugeMan/uiHeroGaugeMan.h"

#ifdef _NA_000000_20130114_RENEWER_UI
	#include "HeroGaugeDialog2.h"
#else
	#include "HeroGaugeDialog.h"
#endif

#include "uiPartyMan/uiPartyMan.h"
#include "uiTargetMan/uiTargetMan.h"
#include "TargetDialog.h"
#include "uiSummonTargetMan/uiSummonTargetMan.h"
#include "SummonTargetDialog.h"
#include "Character.h"
#include "DllManager.h"
#include <client/3dengine/3drenderer/ElementRender.h>

StatusIconManager::StatusIconManager():
is_initialize_(false),
interfacemanager_(NULL),
drawbase_(NULL),
element_render_(NULL),
state_texture_(INVALID_HANDLE_VALUE),
cash_state_texture_(INVALID_HANDLE_VALUE)
{
}

StatusIconManager::~StatusIconManager()
{
    Release();
}

bool StatusIconManager::Initialize(InterfaceManager* interfacemanager, CDrawBase* drawbase)
{
    interfacemanager_ = interfacemanager;
    drawbase_ = drawbase;
    assert(interfacemanager_);
    assert(drawbase_);
    
    if ((interfacemanager_ == NULL) ||
        (drawbase_ == NULL))
    {
        return false;
    }

    element_render_ = static_cast<CElementRender*>(drawbase_->GetElementRender());
    assert(element_render_);
    if (element_render_ == NULL)
    {
        return false;
    }

    LoadTexture();

    // info container
    status_icon_container_.clear();
    cash_status_icon_container_.clear();


    is_initialize_ = true;
    return true;
}

void StatusIconManager::Release()
{
    // 핸들해제
    if (drawbase_)
    {
        drawbase_->x_pManagerTexture->DestroyTexture(state_texture_);
        drawbase_->x_pManagerTexture->DestroyTexture(cash_state_texture_);
    }    
}

void StatusIconManager::LoadTexture()
{
    if (state_texture_ == INVALID_HANDLE_VALUE)
    {
        RESOURCE_INFO* pInfo = NULL;    
        pInfo = GraphicResourceList::Instance()->GetItemInfo(IMAGACODE_STATE_TEXTURE);
        if (pInfo != NULL)
        {
            state_texture_ = drawbase_->x_pManagerTexture->LoadTexture(pInfo->szFilename, 
                TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);

            St_TextureContainer* pTexture = drawbase_->x_pManagerTexture->GetTextureInfo( state_texture_ );
            if (pTexture)
            {
                frame_texture_rect_.fTopX = 0.0f;
                frame_texture_rect_.fTopY = static_cast<float>(pTexture->m_nHeight - STATUS_ICON_SIZE);
                frame_texture_rect_.fWidth = STATUS_ICON_SIZE;
                frame_texture_rect_.fHeight = STATUS_ICON_SIZE;

                frame_texture_rect2_.fTopX = STATUS_ICON_SIZE;
                frame_texture_rect2_.fTopY = static_cast<float>(pTexture->m_nHeight - STATUS_ICON_SIZE);
                frame_texture_rect2_.fWidth = STATUS_ICON_SIZE;
                frame_texture_rect2_.fHeight = STATUS_ICON_SIZE;
            }
        }
    }
    if (cash_state_texture_ == INVALID_HANDLE_VALUE)
    {
        cash_state_texture_ = drawbase_->x_pManagerTexture->LoadTexture(
            "Data\\Interface\\etc\\Alpha_Out.tga", TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
    }
}

void StatusIconManager::RegisterStatusIconInfo()
{
    status_icon_container_.clear();
    cash_status_icon_container_.clear();
    
    // hero, party, target - character 를 통하여 insert(StateIconInfo);
#ifdef _NA_000000_20130114_RENEWER_UI
	HeroGaugeDialog2 *ui_hero_gauge = (HeroGaugeDialog2 *)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_HEROGAUGE);
#else
	HeroGaugeDialog *ui_hero_gauge = (HeroGaugeDialog *)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_HEROGAUGE);
#endif
    if (ui_hero_gauge)
    {
        ui_hero_gauge->UpdateStatusIcon();
    }

    uiPartyMan* ui_party_manager = GET_DIALOG_MANAGER(uiPartyMan, UIMAN_PARTY);
    if (ui_party_manager)
    {
        ui_party_manager->UpdateStatusIcon();
    }

    TargetDialog* target_dialog = GET_CAST_DIALOG(TargetDialog, IM_TARGET_MANAGER::TARGET_DIALOG);
    if (target_dialog)
    {
        target_dialog->UpdateStatusIcon();
    }

    SummonTargetDialog* summon_dialog = 
        GET_CAST_DIALOG(SummonTargetDialog, IM_SUMMON_TARGET_MANAGER::SUMMON_TARGET_DIALOG);
    if (summon_dialog)
    {
        summon_dialog->UpdateStatusIcon();
    }
}

void StatusIconManager::RegisterStatusIconInfo(std::vector<SKILL_EFFECT_IMAGE>& status_icon_container)
{
    typedef std::vector<SKILL_EFFECT_IMAGE>::iterator STATUS_ICON_ITERATOR;
    SKILL_EFFECT_IMAGE* status_icon = NULL;
    STATUS_ICON_ITERATOR status_info_container_itr = status_icon_container.begin();
    STATUS_ICON_ITERATOR status_info_container_end_itr = status_icon_container.end();
    while (status_info_container_itr != status_info_container_end_itr)
    {
        status_icon = &(*status_info_container_itr);
        if (status_icon != NULL)
        {
#ifdef _NA_006819_20130719_STATE_ICON_RENDER
            status_icon_container_.push_back(status_icon);
#else //_NA_006819_20130719_STATE_ICON_RENDER
            if (status_icon->pStateInfo->m_byIconKind == eSTATE_KIND_CASH)
            {            
                cash_status_icon_container_.push_back(status_icon);
            }
            else
            {
                status_icon_container_.push_back(status_icon);
            }
#endif //_NA_006819_20130719_STATE_ICON_RENDER
        }
        ++status_info_container_itr;
    }
}

void StatusIconManager::RenderStatusIcon()
{
    if ((interfacemanager_ == NULL) || (interfacemanager_->GetShowFlag() != TRUE) ||
         (interfacemanager_->IsShowDialog(InterfaceManager::DIALOG_HEROGAUGE) != TRUE))
    {
        return;
    }

    // 일반 상태 아이콘 그리기(캐시 제외 전부)
    if (StatusIconRender())
    {
        element_render_->Flush();
    }

#ifdef _NA_006819_20130719_STATE_ICON_RENDER

#else // _NA_006819_20130719_STATE_ICON_RENDER
    // 캐시 상태 아이콘 그리기
    _RenderIconWithBlend( cash_status_icon_container_, cash_state_texture_);
#endif // _NA_006819_20130719_STATE_ICON_RENDER
}

bool StatusIconManager::StatusIconRender()
{
    DWORD vertices_index = 0;
    BASE_STATEINFO* base_state_info = NULL;
    SKILL_EFFECT_IMAGE* status_icon = NULL;
    WzColor vertex_color;
    SI_FRECT draw_rect, texture_rect;
    STATUS_ICON_CONTAINER::iterator info_container_itr = status_icon_container_.begin();
    STATUS_ICON_CONTAINER::iterator info_container_end_itr = status_icon_container_.end();
    while (info_container_itr != info_container_end_itr)
    {
        status_icon = *info_container_itr;
        base_state_info = status_icon->pStateInfo;

        if (status_icon != NULL && base_state_info != NULL)
        {
            if (status_icon->pSkillEffect)
            {
                int remain_time = status_icon->pSkillEffect->iRemainTime;
                if ((remain_time != 0) &&
                    (remain_time < SKILL_EFFECT_BLINK_TIME))
                {
                    float alpha_rate = 
                        cos(static_cast<float>(remain_time - SKILL_EFFECT_BLINK_TIME) * 0.005f) * 0.5f + 0.5f;
                    alpha_rate = max(0.0f,min(1.0f,alpha_rate));

                    BYTE alpha = (BYTE)(255.0f * alpha_rate);
                    alpha = min(255,alpha);

                    status_icon->m_byAlpha = alpha;
                }
            }
            vertex_color = WzColor_RGBA(255, 255, 255, status_icon->m_byAlpha);

            RECT& tempRect = status_icon->rcDrawRect;
            draw_rect.fTopX = static_cast<float>(tempRect.left);
            draw_rect.fTopY = static_cast<float>(tempRect.top);
            draw_rect.fWidth = static_cast<float>(STATUS_ICON_SIZE);
            draw_rect.fHeight = static_cast<float>(STATUS_ICON_SIZE);

            texture_rect.fTopX = 
                static_cast<float>((base_state_info->m_byIconColumn - 1)*(STATUS_ICON_SIZE));
            texture_rect.fTopY = 
                static_cast<float>((base_state_info->m_byIconRow - 1)*(STATUS_ICON_SIZE));
            texture_rect.fWidth = static_cast<float>(STATUS_ICON_SIZE);
            texture_rect.fHeight = static_cast<float>(STATUS_ICON_SIZE);

            element_render_->Render(state_texture_, texture_rect, draw_rect, vertex_color);

            if( status_icon->pStateInfo->m_byIconKind == eSTATE_KIND_CASH )
            {
                element_render_->Render(state_texture_, frame_texture_rect_, draw_rect, vertex_color);
            }
            else if( status_icon->pStateInfo->m_byIconKind == eSTATE_KIND_SKILL )
            {
                element_render_->Render(state_texture_, frame_texture_rect2_, draw_rect, vertex_color);
            }

        }
        
        ++info_container_itr;
    }
    return true;
}


void StatusIconManager::_RenderIconWithBlend( const STATUS_ICON_CONTAINER& IconContainer, HANDLE Texture )
{
    DWORD vertices_index = 0;
    BASE_STATEINFO* base_state_info = NULL;
    SKILL_EFFECT_IMAGE* status_icon = NULL;
    WzColor vertex_color[4];
    SI_FRECT draw_rect, texture_rect;
    STATUS_ICON_CONTAINER::const_iterator info_container_itr = IconContainer.begin();
    STATUS_ICON_CONTAINER::const_iterator info_container_end_itr = IconContainer.end();
    while (info_container_itr != info_container_end_itr)
    {
        status_icon = *info_container_itr;
        base_state_info = status_icon->pStateInfo;

        if (status_icon != NULL && base_state_info != NULL)
        {
            if (status_icon->pSkillEffect)
            {
                int remain_time = status_icon->pSkillEffect->iRemainTime;
                if ((remain_time != 0) &&
                    (remain_time < SKILL_EFFECT_BLINK_TIME))
                {
                    float alpha_rate = 
                        cos(static_cast<float>(remain_time - SKILL_EFFECT_BLINK_TIME) * 0.005f) * 0.5f + 0.5f;
                    alpha_rate = max(0.0f,min(1.0f,alpha_rate));

                    BYTE alpha = (BYTE)(255.0f * alpha_rate);
                    alpha = min(255,alpha);

                    status_icon->m_byAlpha = alpha;
                }
            }
            for (int index_of_color = 0; index_of_color < 4; ++index_of_color)
            {
                vertex_color[index_of_color] = WzColor_RGBA(255, 255, 255, status_icon->m_byAlpha);
            }

            RECT& tempRect = status_icon->rcDrawRect;
            draw_rect.fTopX = static_cast<float>(tempRect.left) + 0.5f;
            draw_rect.fTopY = static_cast<float>(tempRect.top) + 0.5f;
            draw_rect.fWidth = static_cast<float>(STATUS_ICON_SIZE);
            draw_rect.fHeight = static_cast<float>(STATUS_ICON_SIZE);

            texture_rect.fTopX = 
                static_cast<float>((base_state_info->m_byIconColumn - 1)*(STATUS_ICON_SIZE)) + 0.5f;
            texture_rect.fTopY = 
                static_cast<float>((base_state_info->m_byIconRow - 1)*(STATUS_ICON_SIZE)) + 0.5f;
            texture_rect.fWidth = static_cast<float>(STATUS_ICON_SIZE);
            texture_rect.fHeight = static_cast<float>(STATUS_ICON_SIZE);

            drawbase_->SetAlphaBlendMode(AB_ALPHA);

            drawbase_->x_pManagerTexture->SetColorArg1(0,CManagerTexture::ETARG_TEXTURE);				
            drawbase_->x_pManagerTexture->SetAlphaArg1(0,CManagerTexture::ETARG_TEXTURE);
            drawbase_->x_pManagerTexture->SetColorBlendingOp(0,CManagerTexture::ETOP_SELECTARG1);
            drawbase_->x_pManagerTexture->SetAlphaBlendingOp(0,CManagerTexture::ETOP_SELECTARG1);

            drawbase_->x_pManagerTexture->SetColorArg1(1,CManagerTexture::ETARG_CURRENT);				
            drawbase_->x_pManagerTexture->SetAlphaArg1(1,CManagerTexture::ETARG_CURRENT);
            drawbase_->x_pManagerTexture->SetColorBlendingOp(1,CManagerTexture::ETOP_SELECTARG1);
            drawbase_->x_pManagerTexture->SetAlphaBlendingOp(1,CManagerTexture::ETOP_MODULATE);

            drawbase_->x_pManagerTexture->SetAlphaArg2(1,CManagerTexture::ETARG_TEXTURE);

            drawbase_->RenderTextureBlending(state_texture_,
                Texture,
                draw_rect.fTopX, 
                draw_rect.fTopY, 
                draw_rect.fWidth,
                draw_rect.fHeight,
                texture_rect.fTopX,
                texture_rect.fTopY,
                texture_rect.fWidth,
                texture_rect.fHeight,
                0.0f,
                0.0f,
                static_cast<float>(STATUS_ICON_SIZE + 2),
                static_cast<float>(STATUS_ICON_SIZE + 2),
                vertex_color);

            drawbase_->x_pManagerTexture->SetColorBlendingOp(0);
            drawbase_->x_pManagerTexture->SetColorBlendingOp(1);
            drawbase_->x_pManagerTexture->SetColorArg1(0);
            drawbase_->x_pManagerTexture->SetColorArg1(1);				

            drawbase_->x_pManagerTexture->SetAlphaBlendingOp(0);
            drawbase_->x_pManagerTexture->SetAlphaBlendingOp(1);
            drawbase_->x_pManagerTexture->SetAlphaArg1(0);
            drawbase_->x_pManagerTexture->SetAlphaArg1(1);
            drawbase_->x_pManagerTexture->SetAlphaArg2(1);

            for (int index_of_color = 0; index_of_color < 4; ++index_of_color)
            {
                vertex_color[index_of_color] = 
                    WzColor_RGBA(255, 255, 255, (255 - Alpha_WzColor(vertex_color[index_of_color])));
            }

            drawbase_->RenderTexture(Texture, 
                draw_rect.fTopX, 
                draw_rect.fTopY, 
                draw_rect.fWidth,
                draw_rect.fHeight,
                static_cast<float>(STATUS_ICON_SIZE + 2),
                0.0f,
                static_cast<float>(STATUS_ICON_SIZE + 2),
                static_cast<float>(STATUS_ICON_SIZE + 2),
                vertex_color);

        }
        ++info_container_itr;
    }
}

