#pragma once

#pragma pack(push,1)

struct GAME_VERSION
{
    BYTE m_ClassA, m_ClassB, m_ClassC, m_ClassD;

    inline BOOL IsValid()
    {
        if(m_ClassA == 0 && m_ClassB == 0 && m_ClassC == 0 && m_ClassD == 0)
            return FALSE;

        return TRUE;
    }
};

struct GROUP_DATA
{
    char m_szGroupName[MAX_GROUP_NAME_LEN+1];
    BYTE m_GroupID;
    BYTE m_LimitedAge;
    BYTE m_ConnectStatus;
};

struct SERVER_DATA
{
    char m_szChannelName[MAX_SERVER_NAME_LEN+1];
    BYTE m_GroupID; // WorldID ��
    BYTE m_ChannelID;
    BYTE m_CrowdGrade;
    BYTE m_ConnectStatus;   // 0 : Disconnected, 1 : Connected
};


#define MAX_AUTH		64	// ��Ŷ ����ü ���� ���� �ִ밪(���� Matrix ����ϴ� ���� 48byte���� Ȥ�ó� �ؼ� 64byte �����)
#define MAX_MATRIX		16	// �������� �ִ� ��ǥ ����

typedef struct
{
    DWORD   dwType;           // ���� ���� ���(eSECURE_AUTH_CODE ����)
    BYTE    byData[MAX_AUTH]; // ������ �Է��� ���� ī�� ���� ��ǥ �ε�����
} SECURE_INFO, *LP_SECURE_INFO;

typedef struct
{
    WORD    wMatrix[MAX_MATRIX];  // ���� ī�� ��ǥ, �Ǵ� �� ��ǥ�� ���� ���
    BYTE    byFlag[MAX_MATRIX];   // ���� ī�� ��ǥ���� �պκ�, �޺κ� ��� ���� ���� �ɼ� ������ ���� �ʿ��� �÷���(���� ��� ����)
} SC_INFO, *LP_SC_INFO;

#pragma pack(pop)


