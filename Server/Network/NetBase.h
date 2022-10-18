#ifndef NETWORK_MODULE_NETBASE_H
#define NETWORK_MODULE_NETBASE_H

//==================================================================================================
/// ��Ŷ ���
#pragma pack(push, 1)

struct PACKET_HEADER
{
    WORD size; // ���� ��Ŷ�� ũ��    [size][msg=byte*size] ����
};

//  ���� �����
union uPACKET_HEADER
{
    WORD WSIZE; // ���� ��Ŷ�� ũ��    [size][msg=byte*size] ����
    DWORD DWSIZE; // ���� ����� (�����÷� ����)
};

#define SIZE_PACKET_HEADER() sizeof(WORD)

//==================================================================================================
//  ����/Ŭ���̾�Ʈ �������� �⺻ ��� in SUN
struct MSG_PROTO
{
    DWORD m_dwKey;
};

struct MSG_BASE : public MSG_PROTO
{
    BYTE  m_byCategory; // ī�װ�
    BYTE  m_byProtocol; // ī�װ� �Ʒ��� Ÿ��
};

#pragma pack(pop)
//
//==================================================================================================
//#define NETMSG_SIZE_WITH_HDRSZ(packet_header) \
//  ((INT)(sizeof(packet_header) + packet_header.size))

#endif //NETWORK_MODULE_NETBASE_H