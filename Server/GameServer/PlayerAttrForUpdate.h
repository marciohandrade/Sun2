#pragma once

#include "PlayerAttributes.h"

class PlayerAttrForUpdate : public PlayerAttributes
{
public :
    PlayerAttrForUpdate() {}
    virtual ~PlayerAttrForUpdate() {}

    virtual void SetOwner(IPlayerAttributeLinker* linker);
    virtual void UpdateEx();

private:
    Player* player_;
};

