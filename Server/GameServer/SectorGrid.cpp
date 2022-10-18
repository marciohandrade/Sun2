#include "stdafx.h"

#include <PacketControl/WarMessages_Sync.h>

#include "SectorGrid.h"
#include "Sector.h"
#include "GameZone.h"
#include "GameField.h"


namespace nsGrid {
;

//==================================================================================================
//	<SectorGridShared>
//
bool
SectorGridShared::GetAroundSectors(const SECTOR_IDX coordinates,
                                   SectorGrid3x3* const OUT pOutGrid) const
{
    memset(pOutGrid->Grid, INVALID_SECTOR_INDEX, sizeof(pOutGrid->Grid));
    if ((coordinates == INVALID_SECTOR_INDEX) || !(coordinates < m_SectorCount)) {
        return false;
    };
    const DWORD length_x = m_Length[LEN_X];		// for load to GPR
    const DWORD indexX__ = coordinates % length_x;
    const DWORD __indexY = coordinates / length_x;

    const bool lowerboundX = !indexX__;
    const bool lowerboundY = !__indexY;
    const bool upperboundX = (indexX__ + 1) == m_Length[LEN_X];
    const bool upperboundY = (__indexY + 1) == m_Length[LEN_Y];

    //const DWORD indexX__
    const DWORD indexXm1 = indexX__ - 1;
    const DWORD indexXp1 = indexX__ + 1;

    const DWORD indexY__ = __indexY * length_x;
    const DWORD indexYm1 = indexY__ - length_x;
    const DWORD indexYp1 = indexY__ + length_x;

    SECTOR_IDX* pOffset = (pOutGrid->Grid);

#define GRID_ARRAY_CODE(index, cond, x, y)  \
    if (!(cond)) { *pOffset = (x) + (y); }; \
    ++pOffset

    GRID_ARRAY_CODE(0+0, (lowerboundX || lowerboundY), indexXm1, indexYm1);
    GRID_ARRAY_CODE(0+1, (               lowerboundY), indexX__, indexYm1);
    GRID_ARRAY_CODE(0+2, (upperboundX || lowerboundY), indexXp1, indexYm1);
    GRID_ARRAY_CODE(3+0, (lowerboundX               ), indexXm1, indexY__);
    GRID_ARRAY_CODE(3+1, (0                         ), indexX__, indexY__);
    GRID_ARRAY_CODE(3+2, (upperboundX               ), indexXp1, indexY__);
    GRID_ARRAY_CODE(6+0, (lowerboundX || upperboundY), indexXm1, indexYp1);
    GRID_ARRAY_CODE(6+1, (               upperboundY), indexX__, indexYp1);
    GRID_ARRAY_CODE(6+2, (upperboundX || upperboundY), indexXp1, indexYp1);

#undef GRID_ARRAY_CODE

    return true;
};

bool
SectorGridShared::ChangeSector(const SECTOR_IDX prevSectorXY, const SECTOR_IDX newSectorXY,
                               SectorGrid3x3_ChangeSector& OUT info) const
{
    memset(&info, INVALID_SECTOR_INDEX, sizeof(info));
    const bool bNoChange = prevSectorXY == newSectorXY;
    if (bNoChange) {
        return false;
    }
    const bool bValidPrevS   = (prevSectorXY < m_SectorCount);
    const bool bValidNewS    = (newSectorXY < m_SectorCount);

    //  Prev | New      Leave | Curr  | Enter
    //   X      X    ->   X       X       X
    //   X      O    ->   X       O       O    (C==E)
    //   O      X    ->   O       X       X    (Leave에서 해당 정보 참조 정보 모두 제거)
    //   O      O    ->   O       O       O    (C=N, E=N-P, L=P-N)

    //	(C = N, E = N - P, L = P - N) 
    //	E = N - (P∩N), L = P - (P∩N)

    if (bValidPrevS == false && bValidNewS == false) {
        return false;
    }
    if (bValidPrevS == false && bValidNewS == true)
    {
        GetAroundSectors(newSectorXY, &info.CurrentSectors);
        info.EnterSectors = info.CurrentSectors;
        return true;
    }
    if (bValidPrevS == true && bValidNewS == false)
    {
        GetAroundSectors(prevSectorXY, &info.LeaveSectors);
        return true;
    }

    //
    //if ((bValidPrevS==true) && (bValidNewS==true))
    {
        SectorGrid3x3& currentSectors = info.CurrentSectors;
        SectorGrid3x3& leaveSectors   = info.LeaveSectors;
        SectorGrid3x3& enterSectors   = info.EnterSectors;

        GetAroundSectors(prevSectorXY, &leaveSectors);
        GetAroundSectors(newSectorXY, &currentSectors);
        enterSectors = currentSectors;

        SectorGrid3x3 tmpSectors = leaveSectors;

        typedef SectorGrid3x3::GridArray    GA;
        //  calculation set_difference  L = P-N
        FOREACH_ARRAY(SECTOR_IDX& rPrev, leaveSectors.Grid, GA)
        {
            if (INVALID_SECTOR_INDEX == rPrev) {
                continue;
            }
            FOREACH_ARRAY(SECTOR_IDX& rNew, currentSectors.Grid, GA)
            {
                if (INVALID_SECTOR_INDEX == rNew) {
                    continue;
                }
                if (rPrev == rNew)
                {
                    rPrev = INVALID_SECTOR_INDEX;
                    break;
                }
            }
        }

        //  calculation set_difference  E = N-P
        FOREACH_ARRAY(SECTOR_IDX& rNew, enterSectors.Grid, GA)
        {
            if (INVALID_SECTOR_INDEX == rNew) {
                continue;
            }
            FOREACH_ARRAY(SECTOR_IDX& rPrev, tmpSectors.Grid, GA)
            {
                if (INVALID_SECTOR_INDEX == rPrev) {
                    continue;
                }
                if (rNew == rPrev)
                {
                    rNew = INVALID_SECTOR_INDEX;
                    break;
                }
            }
        }
    }

    return true;
}


void
SectorGridShared::Build(const SectorGridShared& rSrc)
{   //  이것은 FieldInfo가 가지는 기본 섹터 정보를 복사하기 위함...
    // SectorGridInfo를 복사한다고 생각하자.  이것은 실행 중에 동작하는 함수다.

    m_SectorXYdist = rSrc.m_SectorXYdist; //.GetSectorXYDistance();
    m_SectorCount  = rSrc.SectorCount;
    CopyMemory(m_Boundaries, rSrc.m_Boundaries, sizeof(m_Boundaries));
    CopyMemory(m_Length, rSrc.m_Length, sizeof(m_Length));
}



void
SectorGridShared::Build(const BvAABB& rSrc, const DWORD sectorDist /*meter*/)
{
    m_SectorXYdist = sectorDist;
    // input X - boundary
    {
        m_Boundaries[SG_X_MIN] = (DWORD)min(rSrc.m_wvMin.x, rSrc.m_wvMax.x);
        m_Boundaries[SG_Y_MIN] = (DWORD)min(rSrc.m_wvMin.y, rSrc.m_wvMax.y);
        m_Boundaries[SG_Z_MIN] = (DWORD)min(rSrc.m_wvMin.z, rSrc.m_wvMax.z);
    };

    DWORD& rLenX = m_Length[LEN_X];
    DWORD& rLenY = m_Length[LEN_Y];
    DWORD& rLenZ = m_Length[LEN_Z];
    {
        rLenX = (DWORD)abs(rSrc.m_wvMax.x - rSrc.m_wvMin.x);
        rLenY = (DWORD)abs(rSrc.m_wvMax.y - rSrc.m_wvMin.y);
        rLenZ = (DWORD)abs(rSrc.m_wvMax.z - rSrc.m_wvMin.z);
    };
    {
        m_Boundaries[SG_X_MAX] = (DWORD)rLenX;
        m_Boundaries[SG_Y_MAX] = (DWORD)rLenY;
        m_Boundaries[SG_Z_MAX] = (DWORD)rLenZ;
    };
    {
        rLenX = (DWORD)(rLenX / m_SectorXYdist) + !!(rLenX % m_SectorXYdist);
        rLenY = (DWORD)(rLenY / m_SectorXYdist) + !!(rLenY % m_SectorXYdist);
        rLenZ = (DWORD)(rLenZ / m_SectorXYdist) + !!(rLenZ % m_SectorXYdist);
    };
    {
        m_SectorCount = rLenX * rLenY;
    };
}


//==================================================================================================
// SYNC_MSG_LINKEDLIST
//  NOTE: f110112.5L, fix to byte alignment to enhance system architecture address calculator.
#pragma warning(push, 4)

inline SYNC_MSG_LINKEDLIST*
SYNC_MSG_LINKEDLIST::SerializeTo(WAR_MSG_HDR* const IN header,
        const WAR_TAG_V* war_msg_in_buffer, uint16_t war_msg_in_buffer_size) const
{
#ifdef BUF_ALIGN_PTR
    #error "this macro should be used as local declaration only"
#endif
#define BUF_ALIGN_PTR(__size)    (((__size) + ((sizeof(void*)) - 1)) & (~((sizeof(void*)) - 1)))

    WAR_MSG_HDR::MSG_SIZE_T header_offset = header->size;
    ;                       header_offset = BUF_ALIGN_PTR(header_offset);
    //const uint16_t size = GetSize();
    WAR_MSG_HDR::MSG_SIZE_T offset = sizeof(*this) + header_offset;

    this_type* const dest = reinterpret_cast<this_type*>(
        reinterpret_cast<uint8_t*>(header + 1) + header_offset);
    ;{
        util::LList::Init(dest);
        dest->war_msg = war_msg_in_buffer;
        dest->war_msg_size = war_msg_in_buffer_size;
    #ifdef _WIN64
        BOOST_STATIC_ASSERT(sizeof(SYNC_MSG_LINKEDLIST) == 32);
    #else
        BOOST_STATIC_ASSERT(sizeof(SYNC_MSG_LINKEDLIST) == 16);
    #endif
    };

    header->size = offset;

    return dest;

#undef BUF_ALIGN_PTR
};

//==================================================================================================
//  <SectorGrid>
//

void
SectorGrid::Init()
{
    // 너무 큰 맵을 경계한다.
    if (!FlowControl::FCAssert(!m_pSectorNodes && m_SectorCount && (m_SectorCount<60000))) {
        return;
    }
    const SIZE_T alloc_size_array = (m_SectorCount+1) * sizeof(Node);
    m_pSectorNodes = (Node*)TAllocMalloc(alloc_size_array);
    ZeroMemory(m_pSectorNodes, alloc_size_array);

    Node* pIt = &m_pSectorNodes[0];
    //Node** pItEnd = &m_pSectors[m_SectorCount];
    for (DWORD sector_index = 0; sector_index != m_SectorCount; ++sector_index, ++pIt)
    {
        Sector* const pSector = TAllocNew(Sector);
        pSector->Init(sector_index);

        Node& rNode = *pIt;
        rNode.pSector = pSector;
        util::LList::Init(&rNode.m_SectorRoot);
    }

    //LList::Init(&m_FieldRoot);
    //m_Sectors.reserve(m_SectorCount);
    //for (DWORD sector_index=0 ; sector_index!=m_SectorCount ; ++sector_index)
    //{
    //  Sector* const pSector = TAllocNew(Sector);
    //  pSector->Init(sector_index);
    //  m_Sectors.push_back(pSector);
    //}
}

void
SectorGrid::Release()
{
    if (!m_pSectorNodes) {
        return;
    }
    Node* pIt    = &m_pSectorNodes[0];
    Node* pItEnd = &m_pSectorNodes[m_SectorCount];
    for (; pIt != pItEnd; ++pIt)
    {
        Node& rNode = *pIt;
        Sector* const pSector = rNode.pSector;
        pSector->Release();
        TAllocDelete(Sector, pSector);
        //  pNode->m_Root <- 이것은 초기화 하지 않는다. 필드에 걸린 로드밸런스를 위한 메시지스트림 버퍼는 알아서 초기화 될 것이다.
    }
    TAllocFree(m_pSectorNodes);
    m_pSectorNodes = NULL;

    //LList::Init(&m_FieldRoot);

    //if (m_Sectors.empty())
    //  return;
    //FOREACH_CONTAINER(SECTORS::value_type& rNode, m_Sectors, SECTORS)
    //{
    //  Sector* const pSector = rNode;
    //  pSector->Release();
    //  TAllocDelete(Sector, pSector);
    //}
    //m_Sectors.clear();
}

// enhanced changed by _NA001605_EP2_CHANGE_SKILL_BROADCASTER_
Sector* SectorGrid::AddSectorMsgNode(WAR_MSG_HDR* const header, const SECTOR_IDX coordinates,
    const WAR_TAG_V* war_msg_in_buffer, uint16_t war_msg_size) const
{
    // NOTE: f110112.5L, fix to byte alignment to enhance system architecture address calculator.
    if ((m_pSectorNodes && coordinates < m_SectorCount) == false) {
        return NULL;
    }

    Node& node = m_pSectorNodes[coordinates];
    Sector* const sector = node.pSector;
    if (sector == NULL) {
        return NULL;
    };

    ;{
        // add message to linked list of a sector
        SYNC_MSG_LINKEDLIST* linked_list = NULL;
        // it is safe to call this null object.
        linked_list = linked_list->SerializeTo(header, war_msg_in_buffer, war_msg_size);
        util::LList::AddPrev(linked_list, &node.m_SectorRoot);
    };

    return sector;
}

static void
HighLoadBalanceAppendMsg(MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN& rMsgBRD,
                         SYNC_MSG_LINKEDLIST& rLList, WAR_MSG_HDR* const pHeader)
{
    using namespace nsSync;

    const DWORD FLAG_SINGLE_MSG_SIZE    = 8000;
    const WORD INVALID_MSG_SIZE         = 65000;

    DWORD total_sum = 0;

    SYNC_MSG_LINKEDLIST* pLIt = rLList.next;
    SYNC_MSG_LINKEDLIST* pLItEnd = &rLList;

    //for (; pLIt!=pLItEnd ; pLIt=static_cast<SYNC_MSG_LINKEDLIST*>(pLIt->next))
    // (080904) (REMOVED) - for Packet Size Control
    while (pLIt != pLItEnd)
    {
        WAR_TAG_V* const pSubMsg = const_cast<WAR_TAG_V*>(pLIt->war_msg);
        uint16_t msg_size = pLIt->war_msg_size;
//--------------------------------------------------------------------------------------------------
// Debugging facility
#define WAR_MESSAGE_TOKEN_DEBUGGING     (0)
//--------------------------------------------------------------------------------------------------
    #if WAR_MESSAGE_TOKEN_DEBUGGING == 0
        pHeader->size += msg_size;
        rMsgBRD.AppendMsg(reinterpret_cast<BYTE*>(pSubMsg), msg_size);
    #endif
//--------------------------------------------------------------------------------------------------

#if WAR_MESSAGE_TOKEN_DEBUGGING
        //////////////////////////////////////////////////////////////////////////
#define WAR_MSG_SYNC_CASE_CODE(_subprotocol) \
    case _subprotocol: {                     \
        MSGSUB_##_subprotocol& rMsg = static_cast<MSGSUB_##_subprotocol&>(*pSubMsg); \
        msg_size = rMsg.GetSize();           \
        pHeader->size += msg_size;           \
        rMsgBRD.AppendMsg(reinterpret_cast<BYTE*>(pSubMsg), msg_size); \
                       } break
        //////////////////////////////////////////////////////////////////////////

        switch(pSubMsg->_TAG)
        {
        //case SYNC_MONSTER_ENTER_BRD               :
        //  {
        //      MSGSUB_SYNC_MONSTER_ENTER_BRD& rMsg = static_cast<MSGSUB_SYNC_MONSTER_ENTER_BRD&>(*pSubMsg);
        //      const WORD msg_size = rMsg.GetSize();
        //      rMsgBRD.AppendMsg(reinterpret_cast<BYTE*>(pSubMsg), msg_size);
        //      pHeader->size += msg_size;
        //  }
        //  break;
        ////WAR_MSG_SYNC_CASE_CODE( SYNC_MONSTER_ENTER_BRD          );
        //WAR_MSG_SYNC_CASE_CODE(   SYNC_MONSTER_DIR_ENTER_BRD      );
        //WAR_MSG_SYNC_CASE_CODE(   SYNC_MONSTER_LEAVE_BRD          );
        //WAR_MSG_SYNC_CASE_CODE(   SYNC_SUMMONED_ENTER_BRD         );
        //WAR_MSG_SYNC_CASE_CODE(   SYNC_MAPNPC_ENTER_BRD           );
        //WAR_MSG_SYNC_CASE_CODE(   SYNC_FIELDITEM_ENTER_BRD        );
        //WAR_MSG_SYNC_CASE_CODE(   SYNC_FIELDITEM_LEAVE_BRD        );
        //WAR_MSG_SYNC_CASE_CODE(   SYNC_BOSS_MONSTER_ENTER_BRD     );
        //WAR_MSG_SYNC_CASE_CODE(   SYNC_NPC_JUMPRESULT_BRD         );
        //WAR_MSG_SYNC_CASE_CODE(   SYNC_LUCKY_MONSTER_RUNAWY_BRD   );
        //WAR_MSG_SYNC_CASE_CODE(   SYNC_TOTEMNPC_ENTER_BRD         );

        //////////////////////////////////////////////////////////////////////////
        //  eSYNC_POS

        //WAR_MSG_SYNC_CASE_CODE(   SYNC_MOVE_NAK                   );
        WAR_MSG_SYNC_CASE_CODE( SYNC_MOVE_BRD                   );
        WAR_MSG_SYNC_CASE_CODE( SYNC_MOVE_THRUST_BRD            );
        WAR_MSG_SYNC_CASE_CODE( SYNC_TARGET_MOVE_BRD            );
        WAR_MSG_SYNC_CASE_CODE( SYNC_KBMOVE_BRD                 );
        WAR_MSG_SYNC_CASE_CODE( SYNC_NPC_JUMP_BRD               );
        WAR_MSG_SYNC_CASE_CODE( SYNC_STOP_BRD                   );
        //WAR_MSG_SYNC_CASE_CODE(   SYNC_NPC_STOP_BRD               );
        //WAR_MSG_SYNC_CASE_CODE(   SYNC_FORCED_WARP_BRD            );
        WAR_MSG_SYNC_CASE_CODE( SYNC_FAST_KBMOVE_BRD            );
        //WAR_MSG_SYNC_CASE_CODE(   SYNC_JUMP_ACK                   );
        //WAR_MSG_SYNC_CASE_CODE(   SYNC_JUMP_NAK                   );
        WAR_MSG_SYNC_CASE_CODE( SYNC_JUMP_BRD                   );
        //WAR_MSG_SYNC_CASE_CODE(   SYNC_JUMP_STOP_NAK              );
        WAR_MSG_SYNC_CASE_CODE( SYNC_CHARACTER_DEAD_BRD);
        WAR_MSG_SYNC_CASE_CODE( SYNC_CHARACTER_RESURRECTION_BRD);
        //__NA_001290_20090525_SHIELD_SYSTEM
        WAR_MSG_SYNC_CASE_CODE( SYNC_STATUS_ADD_BRD);
        WAR_MSG_SYNC_CASE_CODE( SYNC_STATUS_REMOVE_BRD);
        WAR_MSG_SYNC_CASE_CODE( SYNC_REGENID_BRD);  // _NA_0_20100817_HELLON_LAST_TRIGGER
        //_NA000000_SPEED_HACK_PREVENTION_SUPPORT
        WAR_MSG_SYNC_CASE_CODE(SYNC_SERVER_TIMER_SYNCHRONIZE_BRD);
        //------------------------------------------------------------------------------------------
        // The Episode II
        WAR_MSG_SYNC_CASE_CODE(BATTLE_MONSTER_ATTACK_BRD);
        WAR_MSG_SYNC_CASE_CODE(BATTLE_MONSTER_JUMP_ATTACK_BRD);
        //
        WAR_MSG_SYNC_CASE_CODE(BATTLE_PLAYER_ATTACK_BRD);
        WAR_MSG_SYNC_CASE_CODE(BATTLE_PLAYER_AREA_ATTACK_BRD);
        //
        WAR_MSG_SYNC_CASE_CODE(STYLE_PLAYER_ATTACK_BRD);
        WAR_MSG_SYNC_CASE_CODE(STYLE_PLAYER_ATTACK_RESULT_BRD);
        //
        WAR_MSG_SYNC_CASE_CODE(SKILL_ACTION_INSTANT_RESULT_BRD);
        WAR_MSG_SYNC_CASE_CODE(SKILL_ACTION_DELAY_START_BRD);
        WAR_MSG_SYNC_CASE_CODE(SKILL_ACTION_DELAY_RESULT_BRD);
        //
        WAR_MSG_SYNC_CASE_CODE(SKILL_CASTING_CANCEL_BRD);

        WAR_MSG_SYNC_CASE_CODE(SKILL_PERIODIC_DAMAGE_BRD);
        WAR_MSG_SYNC_CASE_CODE(SKILL_REFLECT_DAMAGE_BRD);
        WAR_MSG_SYNC_CASE_CODE(SKILL_EFFECT_BOMB_BRD);
        WAR_MSG_SYNC_CASE_CODE(SKILL_SELF_DESTRUCTION_BRD);
        WAR_MSG_SYNC_CASE_CODE(SKILL_TRANSPARENT_BRD);
        WAR_MSG_SYNC_CASE_CODE(SKILL_SUCTION_BRD);
        WAR_MSG_SYNC_CASE_CODE(SKILL_VITAL_SUCTION_BRD);
        //
        //------------------------------------------------------------------------------------------
        //
        default:
            SUNLOG(eCRITICAL_LOG, "Unreadable sync sub-protocol %u", pSubMsg->_TAG);
            break;
        };
#endif //
        {   //  Linked List Control - (080904)(REVISION) - for Packet Size Control
            pLIt = util::LList::Delete(pLIt);

            if (INVALID_MSG_SIZE == msg_size) {
                return;
            }
            total_sum += msg_size;
            if (FLAG_SINGLE_MSG_SIZE < total_sum) {
                return;
            }
        };
    }
}


void SectorGrid::Update(GameField* const game_field) const
{
    if (m_pSectorNodes == NULL) {
        return;
    };
    GameZone* const game_zone = game_field->GetGameZone();
    // NOTE: f110531.4L, set to default 'ENABLE_FIELD_HIGH_LOAD_BALANCING'
    // CHANGES: f110531.4L, remove the broadcast upper boundary limitation using 1toN broadcaster
    // for region events SYNC messages
    MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN& msg_brd =
        MSG_ONEtoN_BROADCASTER::InstanceOfBase();
    MSG_CG_SYNC_COMPOSED_STREAM_CMD dummy_war_header;

    Node* node_it = &m_pSectorNodes[0];
    const Node* const node_end = &m_pSectorNodes[m_SectorCount];
    for (; node_it != node_end; ++node_it)
    {
        Node& sector_node = *node_it;

        SYNC_MSG_LINKEDLIST& llist_root = sector_node.m_SectorRoot;
        if (util::LList::IsEmpty(&llist_root)) {
            continue;
        }

        Sector* const sector = sector_node.pSector;
        if (sector->GetPlayerNum() == 0) // list초기화 필요하다.
        {
            util::LList::Init(&llist_root);
            continue;
        }
        //------------------------------------------------------------------------------------------
        do
        {   //  DONE: 이 부분에 대한 루핑처리가 가능하도록 변경하면 더 큰 버퍼 할당이 가능해진다.
            //  recycling msg_brd
            dummy_war_header.m_Header->size = 0;
            msg_brd.Init(&dummy_war_header, sizeof(dummy_war_header));

            MSG_CG_SYNC_COMPOSED_STREAM_CMD* war_header =
                static_cast<MSG_CG_SYNC_COMPOSED_STREAM_CMD*>(msg_brd.GetBrdPacketPtr());
            HighLoadBalanceAppendMsg(msg_brd, llist_root, war_header->m_Header);
            //LList::Init(&llist_root); (080904)(REMOVED) - for Packet Size Control

            uint16_t* const user_it = msg_brd.GetUsersStartPtr();

            ns_object::SearchNodeConstIterator it = sector->GetPlayerRoot();
            const ns_object::SearchNodeConstIterator end = sector->GetPlayerEnd();

            while (const ns_object::SearchNode::LinkNode* link_node = it.ClimbToNext(end))
            {
                Player* player = link_node->select.player_node.player;
                if (obj_util::IsValid(player) == false)
                {
                    continue;
                }
                const DWORD object_key = player->GetObjectKey();
                DWORD bound_check = msg_brd.AddUser(user_it, object_key);
                if (bound_check == msg_brd.MAX_USER_COUNT) {
                    break;
                }
            }

            //if (msg_brd.m_byUserCount) <- user는 반드시 존재한다.
            g_pGameServer->SendToServer(AGENT_SERVER, &msg_brd, msg_brd.GetSendingSize());

        } while (util::LList::IsEmpty(&llist_root) == false);
    };//end 'foreach sector in sectors'
}


//==================================================================================================
//	<SectorGridVR>
//

inline bool
SectorGridVR::CheckAllowMinMaxBoundary(const SECTOR_IDX coordinates, const SectorGridShared& rSrc,
                                       const Boundaries* const IN pB) const
{
    const DWORD dist        = rSrc.m_SectorXYdist;
    const DWORD indexX_len  = (coordinates % rSrc.m_Length[LEN_X]) * dist;
    const DWORD indexY_len  = (coordinates / rSrc.m_Length[LEN_X]) * dist;

    Boundaries b;
    b.minX = indexX_len ;
    b.maxX = indexX_len + (dist);
    b.minY = indexY_len ;
    b.maxY = indexY_len + (dist);

    if ((b.maxX < pB->minX || pB->maxX < b.minX) ||
        (b.maxY < pB->minY || pB->maxY < b.minY))
    {
        return false;
    }
    return true;
}

inline void
SectorGridVR::Get3x3MinMaxBoundary(const SECTOR_IDX coordinates, const SectorGridVR& rSrc,
                                   Boundaries* const OUT pOut) const
{
    const DWORD dist        = rSrc.m_SectorXYdist;
    const DWORD indexX_len  = (coordinates % rSrc.m_Length[LEN_X]) * dist;
    const DWORD indexY_len  = (coordinates / rSrc.m_Length[LEN_X]) * dist;

    pOut->minX = indexX_len - (dist);
    pOut->maxX = indexX_len + (dist << 1);
    pOut->minY = indexY_len - (dist);
    pOut->maxY = indexY_len + (dist << 1);
}


bool
SectorGridVR::GetAroundSectors(const WzVector* IN pPos, SectorGrid3x3* const OUT pOutGrid,
                               ViewBoxCubic* const OUT pOutBoundary) const
{
    // no check ASSERT(m_pProjectionGrid);  //로직단에서 제어하기로 한다.
    const SECTOR_IDX sector_index = m_pProjectionGrid->GetSectorIndex(pPos);
    if (!m_pProjectionGrid->GetAroundSectors(sector_index, pOutGrid)) {
        return false;
    }
    const SECTOR_VRIDX sector_index_VR = GetSectorIndex(pPos);
    Boundaries boundary;
    Get3x3MinMaxBoundary(sector_index_VR, (const SectorGridVR&)*this, &boundary);

    //  해상도가 같으면 더 볼 필요 없다.
    if (m_SectorXYdist != m_pProjectionGrid->m_SectorXYdist)
    {
        typedef SectorGrid3x3::GridArray    GA;
        SectorGrid3x3& rGrid = *pOutGrid;
        FOREACH_ARRAY(SECTOR_IDX& rNode, rGrid.Grid, GA)
        {
            if (INVALID_SECTOR_INDEX == rNode) {
                continue;
            }
            if (!CheckAllowMinMaxBoundary(rNode, *m_pProjectionGrid, &boundary)) {
                rNode = INVALID_SECTOR_INDEX;
            }
        }
    }

    if (pOutBoundary)
    {
        BvAABB& aabb   = pOutBoundary->Cubic;
        aabb.m_wvMin.x = (float)(int)(boundary.minX + m_Boundaries[SG_X_MIN]);
        aabb.m_wvMin.y = (float)(int)(boundary.minY + m_Boundaries[SG_Y_MIN]);
        aabb.m_wvMin.z = -5.f;
        aabb.m_wvMax.x = (float)(int)(boundary.maxX + m_Boundaries[SG_X_MIN]);
        aabb.m_wvMax.y = (float)(int)(boundary.maxY + m_Boundaries[SG_Y_MIN]);
        aabb.m_wvMax.z =  5.f;
    }

    return true;
}

}; // end namespace nsGrid

//==================================================================================================

