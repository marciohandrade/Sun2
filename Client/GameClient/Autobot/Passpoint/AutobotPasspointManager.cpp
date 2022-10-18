#include "SunClientPrecompiledHeader.h"

#include "AutobotPasspointManager.h"

//------------------------------------------------------------------------------
AutobotPasspointManager::AutobotPasspointManager()
{
    passpoint_list_.clear();
}
//------------------------------------------------------------------------------
AutobotPasspointManager::~AutobotPasspointManager()
{
}
//------------------------------------------------------------------------------
AutobotPasspoint* AutobotPasspointManager::Current()
{
    std::list<AutobotPasspoint>::iterator iter = passpoint_list().begin();
    if (iter != passpoint_list().end())
    {
        return &(*iter);
    }

    return NULL;
}
//------------------------------------------------------------------------------
bool AutobotPasspointManager::Next()
{
    Pop();

    return (passpoint_list().size() > 0);
}
//------------------------------------------------------------------------------
void AutobotPasspointManager::Push(AutobotPasspoint& checkpoint)
{
    AutobotPasspoint* current_point = Current();
    if (current_point != NULL)
    {
        if ((current_point->passpoint_type() == AutobotPasspoint::kPasspoint_Temporarily) || 
            (current_point->passpoint_type() == AutobotPasspoint::kPasspoint_Runaway) || 
            (current_point->passpoint_type() == AutobotPasspoint::kPasspoint_FindFollowPlayer))
        {
            Pop();
        }
    }

    passpoint_list().push_front(checkpoint);
}
//------------------------------------------------------------------------------
void AutobotPasspointManager::Push(AutobotPasspoint::PasspointType passpoint_type, 
                                   DWORD objectkey, 
                                   WzVector& position, 
                                   eActionID action_id)
{
    AutobotPasspoint pass_point(passpoint_type, objectkey, position, action_id);

    Push(pass_point);;
}
//------------------------------------------------------------------------------
void AutobotPasspointManager::Pop()
{
    if (passpoint_list().size() > 0)
    {
        passpoint_list().pop_front();
    }
}
//------------------------------------------------------------------------------
void AutobotPasspointManager::Clear()
{
    passpoint_list().clear();
}


////------------------------------------------------------------------------------
//! 지도 클릭시 3d 좌표로 변환
//bool AutobotPasspointManager::InsertCheckPoint(POINT& pt)
//{
//    RadarFullDialog* radar_dialog = GET_CAST_DIALOG(RadarFullDialog, IM_RADAR_MANAGER::RADAR_DIALOG_FULL);
//    if (radar_dialog == NULL || radar_dialog->IsVisible() == FALSE)
//    {
//        return false;
//    }
//
//    RECT rect;
//    if (radar_dialog->GetPictureSize(rect) == false)
//    {
//        return false;
//    }
//
//    float center_pos_x = (float)rect.left + (float)(rect.right - rect.left) * 0.5f;
//    float center_pos_y = (float)rect.top + (float)(rect.bottom - rect.top) * 0.5f;
//
//    float fposx = pt.x - center_pos_x;
//    float fposy = pt.y - center_pos_y;
//    fposx = fposx + (radar_dialog->GetRadarImageWidth() >> 1);
//    fposy = fposy + (radar_dialog->GetRadarImageHeight() >> 1);
//
//    D3DXMATRIX inverse_matrix;
//    D3DXMatrixInverse(&inverse_matrix, 0, (D3DXMATRIX*)(radar_dialog->GetMapMatrix()));
//
//    float radar_in_x = (fposx / radar_dialog->GetRadarMapWidth() - 0.5f) / 0.5f;
//    float radar_in_y = (fposy / radar_dialog->GetRadarMapHeight() - 0.5f) / -0.5f;
//
//    D3DXVECTOR4 pos_vect4 = D3DXVECTOR4(radar_in_x, radar_in_y, 0, 1.0f );
//    D3DXVec4Transform(&pos_vect4, &pos_vect4, &inverse_matrix);
//
//    WzRay ray;
//    ray.m_wvOrigin.x = pos_vect4.x;
//    ray.m_wvOrigin.y = pos_vect4.z;
//    ray.m_wvOrigin.z = 1000.0f;
//    ray.m_wvDirection.x = 0;
//    ray.m_wvDirection.y = 0;
//    ray.m_wvDirection.z = -1.0f;
//
//    float ft = 0.0f;
//    int iTile = g_pSunTerrain->PickPathTile(&ray, &ft);
//
//    if (iTile < 0)
//    {
//        return false;
//    }
//
//    if (g_pSunTerrain->x_pPathFinder->GetAttribute(iTile) & (PTA_ONLY_JUMP | PTA_NO_WALK))
//    {
//        return false;
//    }
//
//    WzVector pos = ray.m_wvOrigin + ray.m_wvDirection * ft;
//    WzVector hero_pos = g_pHero->GetPosition();
//
//    AutobotPasspoint field_check_point(AutobotPasspoint::kPasspoint_Map, 0, pos, ACTION_INVALID);
//    field_checkpoint_vector().push_back(field_check_point);
//
//    return true;
//}