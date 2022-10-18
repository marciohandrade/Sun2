#ifndef _SOLAR_SLOT_SCSOCKETOPTIONINFO_INL
#error "Can't direct include header file"
#endif

//--------------------------------------------------------------------------------------------------
//
#ifdef _NA_0_20100727_SOCKET_SYSTEM_CONVERT

inline void SCSocketOptionInfo::Clear() 
{
    ZeroMemory(this, sizeof(*this));
}

inline void SCSocketOptionInfo::SetInfo(const nsSlot::SerializeItemOption::sEquip& src, 
                                        const BASE_ITEMINFO* item_info) 
{
    _SetInfo(src.SocketIdentifyType, src.NumberOfSockets, src.SocketOption, item_info);
}

inline void SCSocketOptionInfo::SetInfo(const nsSlot::SerializeItemOption::sEquipTime& src, 
                                        const BASE_ITEMINFO* item_info)
{
    _SetInfo(src.SocketIdentifyType, src.NumberOfSockets, src.SocketOption, item_info);
}

//inline void SCSocketOptionInfo::SetInfo(const nsSlot::SerializeItemOption::sEquipEtc& src, 
//                                        const BASE_ITEMINFO* item_info)
//{
//    _SetInfo(src.SocketIdentifyType, src.NumberOfSockets, src.SocketOption, item_info);
//}
//
//inline void SCSocketOptionInfo::SetInfo(const nsSlot::SerializeItemOption::sEquipEtcTime& src, 
//                                        const BASE_ITEMINFO* item_info) 
//{
//    _SetInfo(src.SocketIdentifyType, src.NumberOfSockets, src.SocketOption, item_info);
//}

inline void SCSocketOptionInfo::SetInfo(const nsSlot::SerializeItemOption::sLimited& src, 
                                        const BASE_ITEMINFO* item_info) 
{
    _SetInfo(src.SocketIdentifyType, src.NumberOfSockets, src.SocketOption, item_info);
}

inline void SCSocketOptionInfo::SetInfo(const nsSlot::SerializeItemOption::sFate& src, 
                                        const BASE_ITEMINFO* item_info)
{
    _SetInfo(src.SocketIdentifyType, src.NumberOfSockets, src.SocketOption, item_info);
}

inline void SCSocketOptionInfo::GetInfo(nsSlot::SerializeItemOption::sEquip* dest)
{
    _GetInfo(&dest->SocketIdentifyType, &dest->NumberOfSockets, dest->SocketOption);
}

inline void SCSocketOptionInfo::GetInfo(nsSlot::SerializeItemOption::sEquipTime* dest)
{
    _GetInfo(&dest->SocketIdentifyType, &dest->NumberOfSockets, dest->SocketOption);
}

//inline void SCSocketOptionInfo::GetInfo(nsSlot::SerializeItemOption::sEquipEtc* dest) 
//{
//    _GetInfo(&dest->SocketIdentifyType, &dest->NumberOfSockets, dest->SocketOption);
//}
//
//inline void SCSocketOptionInfo::GetInfo(nsSlot::SerializeItemOption::sEquipEtcTime* dest) 
//{
//    _GetInfo(&dest->SocketIdentifyType, &dest->NumberOfSockets, dest->SocketOption);
//}

inline void SCSocketOptionInfo::GetInfo(nsSlot::SerializeItemOption::sLimited* dest) 
{
    _GetInfo(&dest->SocketIdentifyType, &dest->NumberOfSockets, dest->SocketOption);
}

inline void SCSocketOptionInfo::GetInfo(nsSlot::SerializeItemOption::sFate* dest) 
{
    _GetInfo(&dest->SocketIdentifyType, &dest->NumberOfSockets, dest->SocketOption);
}

inline const sSOCKET_OPTION* SCSocketOptionInfo::GetSocketOption(eSOCKET sock) const 
{
    if(FlowControl::FCAssert(sock < SOCKET_MAX && sock < number_of_sockets_)) 
        return socket_attributes_[sock].second;

    return NULL;
}

#endif //_NA_0_20100727_SOCKET_SYSTEM_CONVERT
