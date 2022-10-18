#ifndef NETWORK_MODULE_NETBASE_H
#define NETWORK_MODULE_NETBASE_H

//==================================================================================================
/// 패킷 헤더
#pragma pack(push, 1)

struct PACKET_HEADER
{
    WORD size; // 순수 패킷의 크기    [size][msg=byte*size] 포멧
};

//  로컬 연산용
union uPACKET_HEADER
{
    WORD WSIZE; // 순수 패킷의 크기    [size][msg=byte*size] 포멧
    DWORD DWSIZE; // 스택 연산용 (오버플로 검출)
};

#define SIZE_PACKET_HEADER() sizeof(WORD)

//==================================================================================================
//  서버/클라이언트 프로토콜 기본 헤더 in SUN
struct MSG_PROTO
{
    DWORD m_dwKey;
};

struct MSG_BASE : public MSG_PROTO
{
    BYTE  m_byCategory; // 카테고리
    BYTE  m_byProtocol; // 카테고리 아래의 타입
};

#pragma pack(pop)
//
//==================================================================================================
//#define NETMSG_SIZE_WITH_HDRSZ(packet_header) \
//  ((INT)(sizeof(packet_header) + packet_header.size))

#endif //NETWORK_MODULE_NETBASE_H