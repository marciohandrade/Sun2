#include "stdafx.h"
#include "./GmCmdTokenizer.hxx"
#include <GMCmdParser.h>
#include "./GMCmdManager.h"
#include "./Status/StatusManager.h"

#include "PlayerRider.h"
#include "MoveStateControl.h"

namespace nsGM {
;
//==================================================================================================
struct NeedFunctionLogic { static const bool value = false; };

int
CmdTokenizer::Route(Player* player, const GmCmdTokens& gm_strings)
{
    const char* group_string = gm_strings[0].c_str();
    eCmdGroup group = Group::string_to_type(group_string);
    if(group.value == group.none)
        return 1;
    BOOST_STATIC_WARNING(NeedFunctionLogic::value);
    return 1;
/*
    GM_CMD_STRING::const_iterator it = gm_strings.begin();
    ++it;
    GM_CMD_STRING::const_iterator end = gm_strings.end();

    int written = 0;
    char buffer[256];

    // concat string
    for( ; it != end; ++it) {
        const char* token = (*it).c_str();
        written += _snprintf(&buffer[written], _countof(buffer) - written, "%s", token);
    }
    buffer[_countof(buffer) - 1] = '\0';

    // tokenizing
    int number_of_nodes = 0;
    OptionNodes nodes = { 0, };
    char* token = strtok(buffer, "|");
    while(token != 0) {
        if(number_of_nodes >= _countof(nodes))
            return 2;
        nodes[number_of_nodes++].option = token;
        token = strtok(0, "|");
    }

    if(number_of_nodes == 0)
        return 3;

    switch(group.value)
    {
    case group.item:
        assert(0);//function not implementation
        break;
    case group.rider:
        return Rider::Parse(player, gm_strings, nodes, number_of_nodes);
    }
*/
    return 1;
}


//==================================================================================================

eCmdGroup
CmdTokenizer::Group::string_to_type(const char* token)
{
    eCmdGroup group = { group.none };

    size_t len = strlen(token);
    if(len == 0)
        return group;

    if(strncmp("item", token, 5) == 0)
        group.value = group.item;
    else if(strncmp("rider", token, 6) == 0)
        group.value = group.rider;

    return group;
}

const char*
CmdTokenizer::Group::type_to_string(eCmdGroup group)
{
    switch(group.value)
    {
    case group.item:
        return "item";
    case group.rider:
        return "rider";
    }
    return "";
}

int
CmdTokenizer::setup_arg_fail_message(MSG_CG_ETC_DEBUGINFO_CMD* msg, int msg_offset,
                                     const char* append_msg)
{
    msg_offset += _snprintf(&msg->m_pszChatMsg[msg_offset],
                            _countof(msg->m_pszChatMsg) - msg_offset,
                            "|Fail|Msg=%s|", append_msg);

    return msg_offset;
}


//==================================================================================================

eRiderCmd
CmdTokenizer::Rider::string_to_type(const char* token)
{
    eRiderCmd cmd_type = { cmd_type.none };

    size_t len = strlen(token);
    if(len == 0)
        return cmd_type;

    if(strncmp("help", token, 5) == 0)
        cmd_type.value = cmd_type.help;
    else if(strncmp("select", token, 7) == 0)
        cmd_type.value = cmd_type.select;
    else if(strncmp("update", token, 7) == 0)
        cmd_type.value = cmd_type.update;
    else if(strncmp("summon", token, 7) == 0)
        cmd_type.value = cmd_type.summon;
    else if(strncmp("release", token, 8) == 0)
        cmd_type.value = cmd_type.release;
    else if(strncmp("equip", token, 6) == 0)
        cmd_type.value = cmd_type.equip;
    else if(strncmp("unequip", token, 8) == 0)
        cmd_type.value = cmd_type.unequip;
    else if(strncmp("wax", token, 4) == 0)
        cmd_type.value = cmd_type.wax;
    else if(strncmp("date", token, 5) == 0)
        cmd_type.value = cmd_type.date;
    else if(strncmp("where", token, 6) == 0)
        cmd_type.value = cmd_type.where;
    else if(strncmp("from", token, 5) == 0)
        cmd_type.value = cmd_type.from;
    else if(strncmp("to", token, 3) == 0)
        cmd_type.value = cmd_type.to;
    else if(strncmp("speed", token, 6) == 0)
        cmd_type.value = cmd_type.speed;
    else
        cmd_type.value = cmd_type.none;

    return cmd_type;
}

const char*
CmdTokenizer::Rider::type_to_string(eRiderCmd cmd_type)
{
    switch(cmd_type.value)
    {
    case cmd_type.help:
        return "help";
    case cmd_type.select:
        return "select";
    case cmd_type.update:
        return "update";
    case cmd_type.summon:
        return "summon";
    case cmd_type.release:
        return "release";
    case cmd_type.equip:
        return "equip";
    case cmd_type.unequip:
        return "unequip";
    case cmd_type.wax:
        return "wax";
    case cmd_type.date:
        return "date";
    case cmd_type.where:
        return "where";
    case cmd_type.from:
        return "from";
    case cmd_type.to:
        return "to";
    case cmd_type.speed:
        return "speed";
    }
    return "";
}

eRiderCmd
CmdTokenizer::Rider::string_to_parts_type(const char* token)
{
    eRiderCmd cmd_type = { cmd_type.none };

    size_t len = strlen(token);
    if(len == 0)
        return cmd_type;

    if(CmdTokenizer::is_all_numeric(token)) {
        if(strncmp("91", token, 3) == 0)
            cmd_type.value = cmd_type.head;
        else if(strncmp("92", token, 3) == 0)
            cmd_type.value = cmd_type.body;
        else if(strncmp("93", token, 3) == 0)
            cmd_type.value = cmd_type.legs;
    }
    else if(strncmp("head", token, 5) == 0)
        cmd_type.value = cmd_type.head;
    else if(strncmp("body", token, 5) == 0)
        cmd_type.value = cmd_type.body;
    else if(strncmp("legs", token, 5) == 0)
        cmd_type.value = cmd_type.legs;

    return cmd_type;
}

const char*
CmdTokenizer::Rider::parts_type_to_string(eRiderCmd cmd_type)
{
    switch(cmd_type.value)
    {
    case cmd_type.head:
        return "head";
    case cmd_type.body:
        return "body";
    case cmd_type.legs:
        return "legs";
    }
    return "";
}

//==================================================================================================

int
CmdTokenizer::Rider::Parse(Player* player, const GmCmdTokens& gm_strings,
                           OptionNodes& nodes, int number_of_nodes)
{
    char* token = 0;
    char* last_option = nodes[0].option;
    BOOLEAN help = false;

    eRiderCmd cmd_type = { cmd_type.none };
    eRiderCmd query = cmd_type;

    int cmd_index = 0;
    OptionNode* const end = &nodes[number_of_nodes];
    RiderOptionBlock options;
    ZeroMemory(&options, sizeof(options));

    for(OptionNode* it = nodes; it != end; ++it) {
        token = strtok(it->option, "= \t");
        while(token != 0) {
            cmd_type = string_to_type(token);
            if(cmd_type.value == cmd_type.help) {
                help = true;
                ++cmd_index;
                goto process_help;
            }

            last_option = token;
            token = strtok(0, "= \t");
            nodes[cmd_index].value = token ? token : "";
            if(token) //for skip
                token = strtok(0, "= \t");

            //printf("  |option=%s|value=%s|\n",
            //    nodes[cmd_index].option, nodes[cmd_index].value);
            nodes[cmd_index].rider_option = cmd_type;
            ++cmd_index;
        }
    }

    //-----------------------------------------------------------
    cmd_type.value = cmd_type.none;

    for(OptionNode* it = nodes; it != end; ++it) {
        cmd_type = it->rider_option;
        switch(cmd_type.value)
        {
            //case cmd_type.help:
            //  return "help";
        case cmd_type.select:
        case cmd_type.update:
        case cmd_type.summon:
        case cmd_type.release:
        case cmd_type.equip:
        case cmd_type.unequip:
        case cmd_type.wax:
            if(query.value == query.none) {
                query = cmd_type;
                options.nodes[query.value] = *it;
            }
            break;
        case cmd_type.date:
        case cmd_type.where:
        case cmd_type.from:
        case cmd_type.to:
        case cmd_type.speed:
            {
                OptionNode& nodes = options.nodes[cmd_type.value];
                if(nodes.rider_option.value == nodes.rider_option.none) {
                    options.nodes[cmd_type.value] = *it;
                }
            }
            break;
        }
    } //end 'for'

    if(cmd_type.value == cmd_type.none)
        return 1;

    //-----------------------------------------------------------
    return Action(player, gm_strings, query, options);

process_help:
    if(last_option == token) {
        //total help
        printf("root=%s cmd=%s|\n", gm_strings[0].c_str(), token);
    }
    else {
        //option help
        printf("option=%s cmd=%s|\n", last_option, token);
    }

    return 0;
}

int
CmdTokenizer::Rider::Action(Player* player, const GmCmdTokens& gm_strings,
                            const eRiderCmd& query, RiderOptionBlock& options)
{
    BYTE _stream[max(sizeof(MSG_CG_ITEM_RIDER_CONTROL_BRD),
                     sizeof(MSG_CG_ITEM_RIDER_CONTROL_PARTS_BRD))] = { 0, };

    int dbgmsg_len = 0;
    MSG_CG_ETC_DEBUGINFO_CMD msgDBG;
    MSG_CG_ITEM_RIDER_CONTROL_BRD& msgRider =
        reinterpret_cast<MSG_CG_ITEM_RIDER_CONTROL_BRD&>(_stream);
    MSG_CG_ITEM_RIDER_CONTROL_PARTS_BRD& msgParts =
        reinterpret_cast<MSG_CG_ITEM_RIDER_CONTROL_PARTS_BRD&>(_stream);

    RiderControl req_info;
    req_info.Clear();

    //RiderControl& rider_hdr = msgRider.header;
    //RiderControlData& rider_data = msgRider.data;
    //RiderControl& parts_hdr = msgParts.header;
    //RiderControlData& parts_data = msgParts.data;

    WORD player_key = (WORD)player->GetObjectKey();
    BOOLEAN success = false;

    StatusManager* pStatusManager = player->GetStatusManager();
    const BOOLEAN is_summoned = pStatusManager->Status.IsRidingRider();

    IPlayerRiderD& rider_d = player->Rider;

    switch(query.value)
    {
    case query.select:
        {
            const char* option = options.nodes[query.speed].option;
            if(option != 0) {
                float move_speed = player->GetMoveStateControl()->GetMoveSpeed();
                BOOLEAN riding = player->GetStatusManager()->Status.IsRidingRider();
                int riding_speed_ratio = 0;
                if(riding)
                    riding_speed_ratio = player->GetAttr().GetRiderSpeedRatio();
                dbgmsg_len += _snprintf(&msgDBG.m_pszChatMsg[dbgmsg_len],
                                        _countof(msgDBG.m_pszChatMsg) - dbgmsg_len,
                                        "|Speed=%3.4f|Riding=%d|RidingSpeedRatio=%d|",
                                        move_speed, riding, riding_speed_ratio);
            }
        }
        break;
    case query.update:
        break;
    case query.summon:
        {
            if(is_summoned != 0) {
                dbgmsg_len += _snprintf(&msgDBG.m_pszChatMsg[dbgmsg_len],
                                        _countof(msgDBG.m_pszChatMsg) - dbgmsg_len,
                                        "|Fail|Msg=Already summoned|");
                break;
            }

            const OptionNode& arg_where = options.nodes[query.where];
            if((arg_where.option && arg_where.value) == 0) {
                dbgmsg_len = CmdTokenizer::setup_arg_fail_message(&msgDBG, dbgmsg_len,
                    arg_where.option == 0 ? "can't find 'where'" : "can't find where='?'");
                break;
            }
            // 먼저 캐스팅 상태로 전환해야 한다.
            if(rider_d.IsValidState() == 0)
                rider_d.Init(player);

            new (&msgRider) MSG_CG_ITEM_RIDER_CONTROL_BRD;

            int where = atoi(options.nodes[query.where].value);

            PlayerRider& rider = rider_d.Get();
            req_info.request = req_info.eReq_SummonStart;
            req_info.args.casting_start.slot_pos = (POSTYPE)where;
            PlayerRider::eResult result = rider.Route(req_info, &msgRider.header, &msgRider.data);
            //result ...hummm
            rider.ChangeToInstanceCasting();

            req_info.Clear();

            req_info.request = req_info.eReq_Summon;
            req_info.args.summon.slot_pos = (POSTYPE)where;

            result = rider.Route(req_info, &msgRider.header, &msgRider.data);
            if(result == rider.eResult_1toN)
                player->SendPacketAround(&msgRider, msgRider.GetSize(), true);
            else
                player->SendPacket(&msgRider, msgRider.GetSize());

            if(result != rider.eResult_Fail)
                success = true;
        }
        break;
    case query.release:
        {
            if(is_summoned == 0) {
                dbgmsg_len += _snprintf(&msgDBG.m_pszChatMsg[dbgmsg_len],
                                        _countof(msgDBG.m_pszChatMsg) - dbgmsg_len,
                                        "|Fail|Msg=not exist summoned rider|");
                break;
            }

            assert(rider_d.IsValidState() != 0);

            new (&msgRider) MSG_CG_ITEM_RIDER_CONTROL_BRD;

            PlayerRider& rider = rider_d.Get();

            req_info.request = req_info.eReq_SummonRelease;
            req_info.args.summon.slot_pos = rider.GetSummonedRiderItemPos();

            PlayerRider::eResult result = rider.Route(req_info, &msgRider.header, &msgRider.data);
            if(result == rider.eResult_1toN)
                player->SendPacketAround(&msgRider, msgRider.GetSize(), true);
            else
                player->SendPacket(&msgRider, msgRider.GetSize());

            if(result != rider.eResult_Fail)
                success = true;
        }
        break;
    case query.equip:
        {
            if(is_summoned == 0) {
                dbgmsg_len += _snprintf(&msgDBG.m_pszChatMsg[dbgmsg_len],
                    _countof(msgDBG.m_pszChatMsg) - dbgmsg_len,
                    "|Fail|Msg=not exist summoned rider|");
                break;
            }

            BOOLEAN is_valid_state = true;

            const OptionNode& arg_from = options.nodes[query.from];
            if((arg_from.option && arg_from.value) == 0) {
                const char* msg = arg_from.option == 0 ? "can't find 'from'"
                                                       : "can't find from='?'";
                dbgmsg_len += _snprintf(&msgDBG.m_pszChatMsg[dbgmsg_len],
                                        _countof(msgDBG.m_pszChatMsg) - dbgmsg_len,
                                        "|Fail|Msg=%s|", msg);
                is_valid_state = false;
            }

            const OptionNode& arg_to = options.nodes[query.to];
            if((arg_to.option && arg_to.value) == 0) {
                const char* msg = arg_to.option == 0 ? "can't find 'to'"
                                                     : "can't find to='?'";
                dbgmsg_len += _snprintf(&msgDBG.m_pszChatMsg[dbgmsg_len],
                                        _countof(msgDBG.m_pszChatMsg) - dbgmsg_len,
                                        "|Fail|Msg=%s|", msg);
                is_valid_state = false;
            }

            if(is_valid_state == false)
                break;

            ///
            new (&msgRider) MSG_CG_ITEM_RIDER_CONTROL_PARTS_BRD;

            PlayerRider& rider = rider_d.Get();

            req_info.request = req_info.eReq_EquipParts;
            req_info.args.equip.from = (POSTYPE)atoi(arg_from.value);
            req_info.args.equip.to.value = string_to_parts_type(arg_to.value).value;

            PlayerRider::eResult result = rider.Route(req_info, &msgRider.header, &msgRider.data);
            if(result == rider.eResult_1toN)
                player->SendPacketAround(&msgRider, msgRider.GetSize(), true);
            else
                player->SendPacket(&msgRider, msgRider.GetSize());

            if(result != rider.eResult_Fail)
                success = true;

            if(success == 0) {
                dbgmsg_len = CmdTokenizer::setup_arg_fail_message(&msgDBG, dbgmsg_len,
                    "|Fail|Msg=invalid input values|");
                break;
            }
        }
        break;
    case query.unequip:
        {
            BOOLEAN is_valid_state = true;

            const OptionNode& arg_where = options.nodes[query.where];
            if((arg_where.option && arg_where.value) == 0) {
                dbgmsg_len = CmdTokenizer::setup_arg_fail_message(&msgDBG, dbgmsg_len,
                    arg_where.option == 0 ? "can't find 'where'" : "can't find where='?'");
                is_valid_state = false;
            }

            const OptionNode& arg_from = options.nodes[query.from];
            if((arg_from.option && arg_from.value) == 0) {
                dbgmsg_len = CmdTokenizer::setup_arg_fail_message(&msgDBG, dbgmsg_len,
                    arg_from.option == 0 ? "can't find 'from'" : "can't find from='?'");
                is_valid_state = false;
            }

            const OptionNode& arg_to = options.nodes[query.to];
            if((arg_to.option && arg_to.value) == 0) {
                dbgmsg_len = CmdTokenizer::setup_arg_fail_message(&msgDBG, dbgmsg_len,
                    arg_to.option == 0 ? "can't find 'to'" : "can't find to='?'");
                is_valid_state = false;
            }

            if(is_valid_state == false)
                break;

            ///
            new (&msgRider) MSG_CG_ITEM_RIDER_CONTROL_PARTS_BRD;

            PlayerRider& rider = rider_d.Get();

            req_info.request = req_info.eReq_UnEquipParts;
            req_info.args.unequip.from.value = string_to_parts_type(arg_from.value).value;
            req_info.args.unequip.to = (POSTYPE)atoi(arg_to.value);
            req_info.args.unequip.where = (POSTYPE)atoi(arg_where.value);

            PlayerRider::eResult result = rider.Route(req_info, &msgRider.header, &msgRider.data);
            if(result == rider.eResult_1toN)
                player->SendPacketAround(&msgRider, msgRider.GetSize(), true);
            else
                player->SendPacket(&msgRider, msgRider.GetSize());

            if(result != rider.eResult_Fail)
                success = true;

            if(success == 0) {
                dbgmsg_len = CmdTokenizer::setup_arg_fail_message(&msgDBG, dbgmsg_len,
                    "|Fail|Msg=invalid input values|");
                break;
            }
        }
        break;
    case query.wax:
        {
            BOOLEAN is_valid_state = true;

            const OptionNode& arg_from = options.nodes[query.from];
            if((arg_from.option && arg_from.value) == 0) {
                dbgmsg_len = CmdTokenizer::setup_arg_fail_message(&msgDBG, dbgmsg_len,
                    arg_from.option == 0 ? "can't find 'from'" : "can't find from='?'");
                is_valid_state = false;
            }

            const OptionNode& arg_to = options.nodes[query.to];
            if((arg_to.option && arg_to.value) == 0) {
                dbgmsg_len = CmdTokenizer::setup_arg_fail_message(&msgDBG, dbgmsg_len,
                    arg_to.option == 0 ? "can't find 'to'" : "can't find to='?'");
                is_valid_state = false;
            }

            if(is_valid_state == false)
                break;

            ///
            new (&msgRider) MSG_CG_ITEM_RIDER_CONTROL_PARTS_BRD;

            //PlayerRider& rider = rider_d.Get();

            req_info.request = req_info.eReq_WaxGloss;
            req_info.args.wax_gloss.from_wax = (POSTYPE)atoi(arg_from.value);
            req_info.args.wax_gloss.to_parts = (POSTYPE)atoi(arg_to.value);

            PlayerRider::eResult result =
                PlayerRider::WaxGloss(req_info, &msgRider.header, &msgRider.data, player);
            if(result == PlayerRider::eResult_1toN)
                player->SendPacketAround(&msgRider, msgRider.GetSize(), true);
            else
                player->SendPacket(&msgRider, msgRider.GetSize());

            if(result != PlayerRider::eResult_Fail)
                success = true;

            if(success == 0) {
                dbgmsg_len = CmdTokenizer::setup_arg_fail_message(&msgDBG, dbgmsg_len,
                    "|Fail|Msg=invalid input values|");
                break;
            }
        }
        break;
    }

    if(dbgmsg_len != 0) {
        msgDBG.m_byMsgLength = (BYTE)dbgmsg_len;
        player->SendPacket(&msgDBG, msgDBG.GetSize());
    }
    return 0;
}


//==================================================================================================

}; //end of namespace


