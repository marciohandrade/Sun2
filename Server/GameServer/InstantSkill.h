#pragma once
#ifndef GAMESERVER_INSTANT_SKILL_ROUTER_H
#define GAMESERVER_INSTANT_SKILL_ROUTER_H

//==================================================================================================
#define SKILL_SYSTEM_VERSION_ROUTING_INL
  //#ifdef _NA002217_100820_EP2_SKILL_SYSTEM_VERSION_
  //  #include "./SkillSystem/SkillEP2.inl"
  //#else
    #include "./SkillSystem/InstantSkill.inl"
  //#endif
#undef SKILL_SYSTEM_VERSION_ROUTING_INL
//==================================================================================================

#endif //GAMESERVER_INSTANT_SKILL_ROUTER_H