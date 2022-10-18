#include "stdafx.h"
#include ".\SCQuickStyleSlot.h"

SCQuickStyleSlot::SCQuickStyleSlot(void)
{
}

SCQuickStyleSlot::~SCQuickStyleSlot(void)
{
}

// moved to header
/*
SCQuickStyleSlot::SCQuickStyleSlot(const STYLESTREAM& stream)
{
    Copy(stream);
}

void
SCQuickStyleSlot::Copy(const STYLESTREAM& IN stream)
{
    CopyMemory(&style_stream_, &stream, sizeof(style_stream_));
}

void
SCQuickStyleSlot::CopyOut(STYLESTREAM& OUT stream)
{
    CopyMemory(&stream, &style_stream_, sizeof(stream));
}

void
SCQuickStyleSlot::Clear()
{
    SCSlot::Clear();
    ZeroMemory(&style_stream_, sizeof(style_stream_));
}

void
SCQuickStyleSlot::Copy(SCSlot& IN slot)
{
    SCSlot::Copy(slot);
    SCQuickStyleSlot& style_slot = (SCQuickStyleSlot &)slot;
    CopyMemory(&style_stream_, &style_slot.style_stream_, sizeof(style_stream_));
}
*/
