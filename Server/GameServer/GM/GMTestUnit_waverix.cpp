#include "stdafx.h"
#include "GMCmdManager.h"
//
//
// Internal utilities
#include "./GMHandler_InterUtils.hxx"

namespace nsGM {
;

static bool TestUnit4waverix(DWORD player_key, const GmCmdTokens& cmd_tokens)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* player = gm_manager->GetGMPlayer(player_key);
    if (player == NULL) {
        return false;
    }
    DISPMSG("My identification is waverix, goo! goo! goo!\n");
    return true;
}

}; //end of namespace 'nsGM'

bool GMCmdManager::RegisterTestUnit_waverix(const HandlerNode** node_array,
                                            size_t* result_array_count)
{
    using namespace nsGM;
    bool success = false;

#define GMHANDLER_MAPPING(activated, korean, english, handler) \
    { activated, eGM_GRADE_MAX, 0xFFFFFFFF, 0xFFFFFFFF, handler, korean, english }

    static HandlerNode const_node_array[] =
    {
        GMHANDLER_MAPPING(true, "������", "waverix", &TestUnit4waverix),
    }; //

    ModifyGmHandlerNode(const_node_array, _countof(const_node_array));
    *node_array = const_node_array;
    *result_array_count = _countof(const_node_array);
    success = true;
    //
    return success;
}