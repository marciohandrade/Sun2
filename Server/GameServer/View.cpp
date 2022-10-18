#include "StdAfx.h"
/*
#include ".\view.h"
#include ".\Player.h"
#include ".\Sector.h"
#include ".\GameZone.h"
#include "Player_Grid_Operator.h"
#include "ServerOptionParserEx.h"
#include <iterator>

View::__player_set View::m_setCurrSector;
View::__player_set View::m_setPrevSector;
View::__player_set View::m_setEnterSector;
View::__player_set View::m_setLeaveSector;

View::View(void)
{
    m_MaxViewObj = c_MAX_DEFAULT_VIEW_OBJ_NUM;
    m_bUsingViewport = FALSE;

    m_pPreView = new view_hash;
    m_pCurView = new view_hash;
}

View::~View(void)
{
    SAFE_DELETE(m_pPreView);
    SAFE_DELETE(m_pCurView);
}

void
View::InitView(Player* pOwner)
{
    SERVER_COMMON_OPTION& stServerOptionInfo = ServerOptionParserEx::Instance()->GetServerOption();
    m_MaxViewObj = stServerOptionInfo.m_wVIEWPORT_MAX_USER_COUNT;
    m_ViewUpdateTimer.SetTimer(stServerOptionInfo.m_WVIEWPORT_UPDATE_TIME);

    m_pOwner = pOwner;
    m_pPreView->clear();
    m_pCurView->clear();
    m_ReferedList.clear();
    m_bUsingViewport = FALSE;
    m_sortViewList.clear();
    m_dwLastSendTick = GetTickCount();
    m_dwSentSize = 0;
}

void
View::ReleaseView()
{
    m_pOwner = NULL;
    m_MaxViewObj = c_MAX_DEFAULT_VIEW_OBJ_NUM;
    m_ViewUpdateTimer.Clear();
    m_pPreView->clear();
    m_pCurView->clear();
    m_ReferedList.clear();
    m_bUsingViewport = FALSE;
    m_sortViewList.clear();
}

void
View::EnterField(BOOL bUsingViewport)
{
    m_bUsingViewport = bUsingViewport;
}

void
View::LeaveField()
{
    if(!IsUsingViewPort()) return;

    // 내가 참조로 하던 유저들(나의 CurView 유저들)로부터 나의 존재를 삭제한다.
    // 내가 등장할 때 MyCurView를 무조건 갱신하듯이(상대방 ReferedPlayer에 나를 삽입) 떠날 때도 CurView를 갱신해줘야 상대방의 ReferedPlayer에서 나를 삭제할 수 있다.
    view_hash_it it = m_pCurView->begin(), end = m_pCurView->end();
    for(; it != end ; ++it)
    {
        Player* pAround = it->second;

        pAround->m_pViewPort->PopReferedPlayer(m_pOwner->GetObjectKey());
    }

    // 나는 떠나지만 주변 유저들의 CurView에는 여전히 Player 정보가 남아 있다.
    // 지금 갱신을 하지 않는다면 자칫 다른 유저가 해당 Player의 포인터를 차지해 버릴 수도 있다.
    view_hash_it it2 = m_ReferedList.begin(), end2 = m_ReferedList.end();
    for(; it2 != end2 ; ++it2)
    {
        Player* pReferedPlayer = it2->second;

        pReferedPlayer->m_pViewPort->LeaveField(m_pOwner);
    }
    m_pPreView->clear();
    m_pCurView->clear();
    m_ReferedList.clear();
    m_sortViewList.clear();
}

void
View::LeaveField(Player* pLeavedPlayer)
{
    // 내 뷰포트에서 나간 유저 정보를 나에게 알린다.
    m_setLeaveSector.clear();
    m_setLeaveSector.insert(m_setLeaveSector.end(), pLeavedPlayer);
    LeaveNeighbor(FALSE);

    // 내 CurView에서 삭제한다.
    m_pCurView->erase(pLeavedPlayer->GetObjectKey());
}

void
View::UpdateView()
{
    Field* pField = m_pOwner->GetField();
    if(!pField) return;

    view_hash* pTmp = m_pPreView;
    m_pPreView = m_pCurView;
    m_pCurView = pTmp;
    m_pCurView->clear();
    m_sortViewList.clear();
    // 주변 9개 섹터의 유저정보를 갱신한다.
    const SECTOR_IDX sector_index = m_pOwner->GetSectorIndex();
    SectorGrid3x3 OUT grid;
    if(pField->SectorGridRef.GetAroundSectors(sector_index, &grid))
    { // Center Processing
        Sector* const pSector = pField->GetSector(sector_index);
        grid.Grid[ SectorGrid3x3::CENTER ] = INVALID_SECTOR_INDEX; // Center 제거

        _Update_User(pSector);
    };

    // 주변 섹터만 처리한다. Center는 앞 로직에서 제거되었음.
    FOREACH_ARRAY(const SECTOR_IDX sector_idx, grid.Grid, SectorGrid3x3::GridArray)
    {
        if(INVALID_SECTOR_INDEX == sector_idx)
            continue;

        Sector* const pSector = pField->GetSector(sector_idx);
        _Update_User(pSector);
    }

    _MakeCurView();

    // 신규로 들어온 유저와 나간 유저를 갱신한다.
    _UpdateSector();
}


// 거리가 먼 순서대로 정렬한다.
bool
view_comparison(SORT_VIEW_DATA Left, SORT_VIEW_DATA Right)
{
    if(Left.m_bReferedPlayer != Right.m_bReferedPlayer)
        return Left.m_bReferedPlayer == true;

    float fLeftDist = Left.m_pPlayer->GetDist2(Left.m_pOwner);
    float fpRightDist = Right.m_pPlayer->GetDist2(Right.m_pOwner);

    return fLeftDist < fpRightDist;
}

void
View::__Update_User(OBJECT_PAIR rPair)
{
    Player* pAround = (Player*)rPair.second;
    if(pAround == m_pOwner)
        return;

    static SORT_VIEW_DATA sortData;

    view_hash_it it = m_ReferedList.find(pAround->GetObjectKey());
    bool bReferedPlayer = it == m_ReferedList.end() ? false : true;
    sortData.Init(m_pOwner, pAround, bReferedPlayer);

    m_sortViewList.push_back(sortData);
}

void
View::_Update_User(Sector* pSector)
{
    if(!pSector)
        return;

    pSector->ForeachAllPlayer(&View::__Update_User, this);
}


void
View::_MakeCurView()
{
    // m_pCurView에 넣어야 할 대상이 m_MaxViewObj 보다 크면 정렬한 후 최대 개수만큼만 m_pCurView에 넣는다.
    if(m_sortViewList.size() > m_MaxViewObj)
    {
        m_sortViewList.sort(view_comparison);
    }

    sortview_list::iterator it = m_sortViewList.begin(), end = m_sortViewList.end();
    for(int Count = 0; it != end ; ++it, Count++)
    {
        SORT_VIEW_DATA& sortData = *it;

        // 최대인원을 넘어가고 ReferedPlayer가 아니라면 짤라낸다.
        if(Count >= m_MaxViewObj && !sortData.m_bReferedPlayer)
        {
            break;
        }

        m_pCurView->insert(view_hash::value_type(sortData.m_pPlayer->GetObjectKey(), sortData.m_pPlayer));
    }
}

void
View::_UpdateSector()
{
    m_setPrevSector.clear();
    m_setCurrSector.clear();

    view_hash_it it = m_pPreView->begin(), end = m_pPreView->end();
    while(it != end)
    {
        m_setPrevSector.insert(m_setPrevSector.end(), it->second);
        ++it;
    }

    it = m_pCurView->begin(), end = m_pCurView->end();
    while(it != end)
    {
        m_setCurrSector.insert(m_setCurrSector.end(), it->second);
        ++it;
    }

    m_setEnterSector.clear();
    set_difference( m_setCurrSector.begin(),
        m_setCurrSector.end(),
        m_setPrevSector.begin(),
        m_setPrevSector.end(),
        inserter(m_setEnterSector, m_setEnterSector.end()));


    m_setLeaveSector.clear();
    set_difference( m_setPrevSector.begin(),
        m_setPrevSector.end(),
        m_setCurrSector.begin(),
        m_setCurrSector.end(),
        inserter(m_setLeaveSector, m_setLeaveSector.end()));

}

void
View::Update()
{
    if(!IsUsingViewPort())
        return;
    Field* pField = m_pOwner->GetField();
    if(!pField)
        return;
    if(!m_ViewUpdateTimer.IsExpired())
        return;

    // m_CurView를 갱신한다.
    UpdateView();

    // 나의 뷰포트로 들어온 플레이어의 정보를 나에게 알린다.
    EnterNeighbor();

    // 나의 뷰포트에서 나간 플레이어의 정보를 삭제하라고 나에게 알린다.
    LeaveNeighbor(TRUE);

    DWORD UpdateTime = c_VIEWPORT_UPDATE_DELAY_TIME;
    UpdateTime = max(c_VIEWPORT_UPDATE_DELAY_TIME, ServerOptionParserEx::Instance()->GetServerOption().m_WVIEWPORT_UPDATE_TIME);

    const DWORD number_of_users_in_sector = (DWORD)m_sortViewList.size();

    if(m_pOwner->GetGameZonePtr()->GetZoneType() == eZONETYPE_VILLAGE)
    {
        if(number_of_users_in_sector >= 100)     UpdateTime += 2000;
        else if(number_of_users_in_sector >= 50) UpdateTime += 1000;
    }
    else
    {
        if(number_of_users_in_sector >= 100)     UpdateTime += 3000;
        else if(number_of_users_in_sector >= 70) UpdateTime += 2000;
        else if(number_of_users_in_sector >= 40) UpdateTime += 1000;
    }
    m_ViewUpdateTimer.SetTimer(UpdateTime);
}

void
View::EnterNeighbor()
{
    if(!IsUsingViewPort())
        return;
    if(m_setEnterSector.empty())
        return;

    // 주변 플레이어 각종 렌더 정보를 받는다.
    // Addin - 주변 플레이어 이동 정보를 받고 뷰포트로 들어온 유저의 m_ReferedList를 갱신한다.
    m_pOwner->OnSendAllPlayersRenderInfoAllKinds(this);
}

void
View::LeaveNeighbor(BOOL bPopReferedPlayer)
{
    if(!IsUsingViewPort())
        return;
    if(m_setLeaveSector.empty())
        return;

    // %주의% 섹터를 벗어난 Player 정보는 유효하지만 서버를 떠난 Player 정보는 유효하지 않다.
    // 기존 Player가 떠나고 다른 유저가 해당 Player 포인터를 받게된다면 꼬인다.

    // 나의 뷰포트에서 나간 플레이어의 정보를 삭제하라고 나에게 알린다.
    // Addin - 뷰포트에서 나간 유저의 m_ReferedList를 갱신한다.
    m_pOwner->OnSendAllPlayersLeaveInfo(this, bPopReferedPlayer);
}


*/
