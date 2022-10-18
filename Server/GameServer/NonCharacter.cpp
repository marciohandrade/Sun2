#include "stdafx.h"
#include "NonCharacter.h"

//==================================================================================================

NonCharacter::NonCharacter()
{
    SetObjectType(NONCHARACTER_OBJECT);
    // CHANGES: f110307.2L
    ;{
        ns_object::SearchReferences ref_option = { 0, };
        _RegisterSearchReference(ref_option);
    };
    Object::_SetPosVector(&m_wzVec);
    SetPos(&kNullVector);
}

