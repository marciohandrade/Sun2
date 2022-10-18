#include "stdafx.h"
#include "./SCQuickSlot.h"

__EMPTY_FILE_TAG();

// moved to header
/*

SCQuickSlot::SCQuickSlot(void)
{
}

SCQuickSlot::~SCQuickSlot(void)
{
}

SCQuickSlot::SCQuickSlot(const QUICKSTREAM& quick_stream)
{
    quick_stream_ = quick_stream;
}

void
SCQuickSlot::Copy(const QUICKSTREAM& IN quick_stream)
{
    quick_stream_ = quick_stream;
}

void
SCQuickSlot::CopyOut(QUICKSTREAM& OUT quick_stream)
{
    quick_stream = quick_stream_;
}

void
SCQuickSlot::Clear()
{
    SCSlot::Clear();
    ZeroMemory(&quick_stream_, sizeof(quick_stream_));
}

void
SCQuickSlot::Copy(SCSlot& IN slot)
{
    SCSlot::Copy(slot);
    SCQuickSlot& quick_slot = static_cast<SCQuickSlot&>(slot);
    quick_stream_ = quick_slot.quick_stream_;
}
*/