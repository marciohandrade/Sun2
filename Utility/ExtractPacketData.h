#pragma once

#ifdef _NA_0_20080612_WORLDSERVER_REFACTORING

template <typename T>
inline void PacketSetConstantForward(PBYTE& packetBuff, WORD& packetBuffLen, const T& value)
{
    *(T*)packetBuff = value;
    packetBuff += sizeof(T);
    packetBuffLen += sizeof(T);
}

inline void PacketSetStrForward(PBYTE& packetBuff, WORD& packetBuffLen, const char* value)
{   
    WORD valueLen = (value) ? static_cast<WORD>(strlen(value)): 0;
    *(WORD*)&packetBuff[0] = valueLen;
    if(valueLen > 0)
        strncpy((char*)&packetBuff[sizeof(WORD)], value, valueLen);
    packetBuff += (valueLen + sizeof(WORD));
    packetBuffLen += (valueLen + sizeof(WORD));
}

inline void PacketSetShortStrForward(PBYTE& packetBuff, WORD& packetBuffLen, const char* value)
{   
    BYTE valueLen = (value) ? static_cast<BYTE>(strlen(value)): 0;
    packetBuff[0] = valueLen;
    if(valueLen > 0)
        strncpy((char*)&packetBuff[1], value, valueLen);
    packetBuff += (valueLen + sizeof(BYTE));
    packetBuffLen += (valueLen + sizeof(BYTE));
}

inline void PacketSetConstStrForward(PBYTE& packetBuff, WORD& packetBuffLen, const char* value, BYTE valueLen)
{   
	//packetBuff[0] = valueLen;
	//if(valueLen > 0)
	//	strncpy((char*)&packetBuff[1], value, valueLen);
	strncpy((char*)&packetBuff[0], value, valueLen);
	packetBuff += valueLen;
	packetBuffLen += valueLen;
	//packetBuff += (valueLen + sizeof(BYTE));
	//packetBuffLen += (valueLen + sizeof(BYTE));
}

template <typename T>
inline void PacketGetConstantForward(T& dest, PBYTE& packet)
{
    dest = *(T*)packet;
    packet += sizeof(T);
}

inline bool PacketGetStrForward(char* dest, PBYTE& packet, WORD& writtenLen, WORD limitLen)
{
    //size_t len = std::numeric_limits<WORD>::max();
	size_t len = 0;
    len = *(WORD*)&packet[0];
    writtenLen = (WORD)len;
    if(len <= limitLen)
    {
        if(len > 0)
            strncpy(dest, (const char*)&packet[sizeof(WORD)], len);
        dest[len] = '\0';        
        packet += (writtenLen + sizeof(WORD));
        return true;
    }        
    return false;
}

inline bool PacketGetShortStrForward(char* dest, PBYTE& packet, WORD& writtenLen, WORD limitLen)
{
    size_t len = 0;/* = std::numeric_limits<BYTE>::max();*/
    if(packet[0] > 0)
    {
        len = static_cast<size_t>(packet[0]) - sizeof(BYTE);
        writtenLen = static_cast<WORD>(len);
        if(len <= limitLen)
        {
            if(len > 0)
                strncpy(dest, (const char*)&packet[sizeof(BYTE)], len);
            dest[len] = '\0';        
            packet += (writtenLen + sizeof(BYTE));
            return true;
        }        
    } 
    return false;
}

template<typename T>
bool Between(const T value, T rangeStart, T rangeEnd)
{
    //if(rangeStart > rangeEnd)
    //{
    //    std::swap(rangeStart, rangeEnd);
    //}
	return (rangeStart <= value) && (rangeEnd >= value);
}

#endif //#ifdef _NA_0_20080612_WORLDSERVER_REFACTORING