#pragma once
#ifndef GAMESERVER_PASSIVE_SKILL_H
#define GAMESERVER_PASSIVE_SKILL_H

//==================================================================================================
#define SKILL_SYSTEM_VERSION_ROUTING_INL
  //#ifdef _NA002217_100820_EP2_SKILL_SYSTEM_VERSION_
  //  #include "./SkillSystem/SkillEP2.inl"
  //#else
    #include "./SkillSystem/PassiveSkill.inl"
  //#endif
#undef SKILL_SYSTEM_VERSION_ROUTING_INL
//==================================================================================================

#endif //GAMESERVER_PASSIVE_SKILL_H