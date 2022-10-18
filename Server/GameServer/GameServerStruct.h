#pragma once
#ifndef GAMESERVER_STRUCT_H
#define GAMESERVER_STRUCT_H

//==================================================================================================

class ServerSession;

class TradeSlotContainer;
class VendorSlotContainer;
class CWarehouseSlotContainer;
class EventInventorySlotContainer;

class SlotManager;
class QuickManager;
class IQuickStyleManager;
class MissionManager;
class ItemManager;
class QuestManager_Concrete;
class FatigueManager;
class AchievementManager;
class MissionRecordManager;
class PostManager; //_NA_20100420_CHANGE_SYSTEM_MEMO_LOGIC
class TriggerManagerEx;

class MovementChecker;
class PVPInfo;
class CompetitionInfo;
class WayPoint;
class CharWayPoint;
class ChaoState;
class View;
class VendorPage;
#ifdef _NA_008012_20150130_RANKING_SYSTEM
class RankingSystemCtl;
#endif // _NA_008012_20150130_RANKING_SYSTEM
class CGuildWarehouseSlotContainer;

class CollectedList;
class AccumulatePoint; //_NA_1419_20100121_ACCUMULATE_POINT
class PlayerOperator;

struct SkillInfo;
class SCSkillSlot;
class SkillSlotManager;
class SkillScriptInfo;

//==================================================================================================
// game object hierarchy

class Object;
class NonCharacter;     class Item;
                        class Money;
                        class Collection;
                        class MapObject;

class Character;        class Player;
                        class NPC;          class Monster;      class FriendMonster;
                                                                class SSQMonster;
                                                                class LuckyMonster;
                                                                class SyncMerchant;
                                            class Summoned;
                                            class MapNpc;
                                            class CrystalWarp;
                                            class Merchant;
                                            class TotemNpc;


//==================================================================================================
//  <Player_NestedInfo.hxx>
//  -   Player Internal Function Classes
//
// hierarchy of a game server namespaces
namespace util { namespace internal {}; };
namespace Generic {};
namespace ns_functor {};
namespace nsSlot {};
namespace nsSkill {};
namespace nsSSQ {};
namespace nsSSQ_Util {};
namespace nsSync {};
namespace ns_trigger {};
namespace nsAI {};
namespace nsGrid {}; // exported
namespace nsGM {};
namespace ns_object {};

//--------------------------------------------------------------------------------------------------

namespace util {
;

}; //end of namespace

//--------------------------------------------------------------------------------------------------

namespace util { namespace internal {
;

}}; //end of namespace

//--------------------------------------------------------------------------------------------------

namespace Generic {
;

}; //end of namespace

//--------------------------------------------------------------------------------------------------

namespace ns_functor {
;
}; //end of namespace

//--------------------------------------------------------------------------------------------------

namespace nsSlot {
;

class EtheriaImpl;
struct EtheriaControlOption;

}; //end of namespace

//--------------------------------------------------------------------------------------------------

namespace nsSkill {
;

class PerkManager; //_NA002217_100728_EP2_PERK_SYSTEM_

};

//--------------------------------------------------------------------------------------------------

namespace nsSSQ {
;
}; //end of namespace

//--------------------------------------------------------------------------------------------------

namespace nsSSQ_Util {
;
}; //end of namespace

//--------------------------------------------------------------------------------------------------

namespace nsSync {
;

class CSSyncTuner;

}; //end of namespace

//--------------------------------------------------------------------------------------------------

namespace ns_trigger {
;

class TriggerPlayerLinkInGame;
class TriggerPlayerLinkInGameSlots;
// _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER_BASE
class TriggerNpcLinkInGame;
class TriggerNpcLinkInGameSlots;
class TriggerNpcLinkMatchTable;

}; //end of namespace

//--------------------------------------------------------------------------------------------------

namespace nsAI {
;
}; //end of namespace

//--------------------------------------------------------------------------------------------------

namespace nsGrid {
;
}; //end of namespace

//--------------------------------------------------------------------------------------------------

namespace nsGM {
;
}; //end of namespace

//--------------------------------------------------------------------------------------------------

namespace ns_object {
;

class SPECIAL_RENDER_EFFECT;
// TODO: f110315.2L, class PlayerRider;
class PlayerWings;

}; //end of namespace

//==================================================================================================


#endif //GAMESERVER_STRUCT_H