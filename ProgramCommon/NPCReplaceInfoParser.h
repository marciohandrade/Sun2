#ifndef _NPC_REPLACE_INFO_PARSER_H
#define _NPC_REPLACE_INFO_PARSER_H

#ifdef _NA_003649_20111115_BATTLE_ZONE_HARDMODE

#include <Singleton.h>
#include <SolarFileReader.h>
#include <ScriptCode.IParser.h>

class NPCReplaceInfoParser : public util::Singleton<NPCReplaceInfoParser>, public IParser
{
public:
    NPCReplaceInfoParser();
    virtual ~NPCReplaceInfoParser();

    virtual	BOOL LoadScript(eSCRIPT_CODE script_code, BOOL is_reload);

    DWORD GetReplaceNPCCode(const MAPCODE mapcode, const MONSTERCODE monstercode) const;

private:
    typedef DWORD ReplaceNPCInfoKey;
    ReplaceNPCInfoKey MakePairKey(const MAPCODE mapcode, const MONSTERCODE monstercode) const
    {
        return (ReplaceNPCInfoKey)(monstercode + (mapcode << (sizeof(MONSTERCODE)*8)));
    }

    BOOL _Load(BOOL is_reload);
    void Release();

    typedef STLX_HASH_MAP<ReplaceNPCInfoKey, MONSTERCODE> NPCReplaceInfoMap;
    NPCReplaceInfoMap npc_replaceinfo_map_;
};

#endif //_NA_003649_20111115_BATTLE_ZONE_HARDMODE
#endif //_NPC_REPLACE_INFO_PARSER_H