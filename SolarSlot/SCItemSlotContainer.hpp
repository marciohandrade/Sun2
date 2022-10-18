#ifndef __SOLAR_SLOT_SCITEMSLOT_CONTAINER_HPP__
    #error "Can't direct include this header file"
#endif

template< class STREAM_TYPE >
void
SCItemSlotContainer::InsertSlotUsingStream(POSTYPE at_pos, STREAM_TYPE& stream)
{
    if (stream.Part.IsValid() == false) {
        return;
    }
    SCSlot* slot = GetValidSlotPtr(NULL, at_pos);
    if (slot == NULL) {
        return;
    }
    if (GetEmptySlotNum() == 0) {
        return;
    }
    if (IsEmpty(at_pos) == false) {
        return;
    }
    //
    ++m_nSlotNum;
    //ASSERT(m_nSlotNum <= GetMaxSlotNum()); pre-checked before '++m_nSlotNum', assertion logic
    SCItemSlot* item_slot = static_cast<SCItemSlot*>(slot);
    item_slot->Copy(stream);
    item_slot->SetPos(at_pos);
    item_slot->SetSlotIdx(GetSlotIdx());
    item_slot->OnInsert();
    //
    this->OnInsert(*item_slot);
}

template< class STREAM_TYPE >
void
SCItemSlotContainer::SerializeItemStream(POSTYPE pos, STREAM_TYPE* IN item_stream,
                                         eSERIALIZE serialize_type)
{
    if (serialize_type == SERIALIZE_LOAD)
    {
        SCSlot* slot = GetValidSlotPtr(NULL, pos);
        if (!FlowControl::FCAssert(slot != NULL)) {
            ZeroMemory(item_stream, sizeof(*item_stream)); // no ctor stream structure
            return;
        }
        // with empty slot
        SCItemSlot* item_slot = static_cast<SCItemSlot*>(slot);
        item_slot->CopyOut(*item_stream);
    }
    else if (serialize_type == SERIALIZE_STORE)
    {
        InsertSlotUsingStream(pos, *item_stream);
    }
    // impossible unreached point
    ASSERT(serialize_type == SERIALIZE_LOAD || serialize_type == SERIALIZE_STORE);
}

template< class STREAM_TYPE >
void
SCItemSlotContainer::SerializeItemStreamAll(STREAM_TYPE* INOUT item_stream, WORD stream_size,
                                            eSERIALIZE serialize_type)
{
    BOOST_STATIC_ASSERT(sizeof(item_stream[0]) == sizeof(ITEMSTREAMEX));
    const POSTYPE upperbound = GetMaxSlotNum();
    if (serialize_type == SERIALIZE_LOAD)
    {
        if (!FlowControl::FCAssert(IsBadWritePtr(item_stream, stream_size) == false &&
                                   (stream_size / sizeof(ITEMSTREAMEX) >= upperbound)))
        {
            __debugbreak();
        }
        ZeroMemory(item_stream, stream_size);
        for (POSTYPE pos = 0; pos < upperbound; ++pos)
        {
            if (IsEmpty(pos) == false) {
                SerializeItemStream(pos, &item_stream[pos], serialize_type);
            }
        }
        return;
    }
    else if (serialize_type == SERIALIZE_STORE)
    {
        if (!FlowControl::FCAssert(IsBadReadPtr(item_stream, stream_size) == false)) {
            __debugbreak();
        }
        for (POSTYPE pos = 0; pos < upperbound; ++pos) {
            SerializeItemStream(pos, &item_stream[pos], serialize_type);
        }
        return;
    }
    // impossible unreached point
    ASSERT(serialize_type == SERIALIZE_LOAD || serialize_type == SERIALIZE_STORE);
}
