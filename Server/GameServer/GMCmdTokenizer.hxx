#ifndef __GAMESERVER_GMCMD_TOKENIZER_HXX
#define __GAMESERVER_GMCMD_TOKENIZER_HXX

#include <GMCmdParser.h>

//==================================================================================================

namespace nsGM {
;
//--------------------------------------------------------------------------------------------------
struct eCmdGroup
{
    enum {
        none  = 0,
        item  ,
        rider ,
        counts
    };
    BYTE value;
};

//--------------------------------------------------------------------------------------------------
struct eItemCmd
{
    enum eCmds {
        none   = 0,
        help   ,
        code   ,
        num    ,
        divine ,
        rank   ,
        socket ,
        extra  , //extra option
        limited,
        lucky,
        duration,
        attack,
        defense,
        period,
        counts
    };
    BYTE value;
};

//--------------------------------------------------------------------------------------------------
struct eRiderCmd
{
    enum eCmds {
        none    = 0,
        help    ,
        select  ,
        update  ,
        summon  ,
        release ,
        equip   ,
        unequip ,
        wax     ,
        date    ,
        where   ,
        from    ,
        to      ,
        speed   ,
        counts
    };

    enum eParts {
        head    = 91,
        body    ,
        legs    ,
    };
    BYTE value;
};

//==================================================================================================

struct OptionNode
{
    union {
        eItemCmd item_option;
        eRiderCmd rider_option;
    };
    char* option;
    char* value;
};

typedef OptionNode OptionNodes[30];

//==================================================================================================

struct RiderOptionBlock : eRiderCmd
{
    OptionNode nodes[counts];
};

//==================================================================================================
struct CmdTokenizer
{
    static BOOLEAN is_all_numeric(const char* token);
    static int setup_arg_fail_message(MSG_CG_ETC_DEBUGINFO_CMD* msg, int msg_offset,
                                      const char* append_msg);
    struct Group {
        static eCmdGroup string_to_type(const char* token);
        static const char* type_to_string(eCmdGroup group);
    };

    struct Rider {
        static eRiderCmd string_to_type(const char* token);
        static const char* type_to_string(eRiderCmd group);

        static eRiderCmd string_to_parts_type(const char* token);
        static const char* parts_type_to_string(eRiderCmd group);

        static int Parse(Player* player, const GmCmdTokens& gm_strings,
                         OptionNodes& nodes, int number_of_nodes);
        static int Action(Player* player, const GmCmdTokens& gm_strings,
                          const eRiderCmd& query, RiderOptionBlock& options);
    };

    static int Route(Player* player, const GmCmdTokens& gm_strings);
};

inline BOOLEAN
CmdTokenizer::is_all_numeric(const char* token) {
    for(const char* it = token; *it; ++it) {
        char chr = *it;
        if(('0' <= chr && chr <= '9') == 0)
            return false;
    }
    return true;
}

}; //end of namespace

#endif //__GAMESERVER_GMCMD_TOKENIZER_HXX
