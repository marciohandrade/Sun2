#pragma once
#ifndef GAMESERVER_SKILLMANAGER_ROUTER_H
#define GAMESERVER_SKILLMANAGER_ROUTER_H

//==================================================================================================
#define SKILL_SYSTEM_VERSION_ROUTING_INL
  //#ifdef _NA002217_100820_EP2_SKILL_SYSTEM_VERSION_
  //  #include "./SkillSystem/SkillManagerEP2.inl"
  //#else
    #include "./SkillSystem/SkillManagerEx.inl"
  //#endif
#undef SKILL_SYSTEM_VERSION_ROUTING_INL
//==================================================================================================

#endif //GAMESERVER_SKILLMANAGER_ROUTER_H
