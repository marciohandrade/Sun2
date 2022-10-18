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
    BYTE m_GroupID; // WorldID 임
    BYTE m_ChannelID;
    BYTE m_CrowdGrade;
    BYTE m_ConnectStatus;   // 0 : Disconnected, 1 : Connected
};


#define MAX_AUTH		64	// 패킷 구조체 내부 버퍼 최대값(현재 Matrix 사용하는 값은 48byte지만 혹시나 해서 64byte 잡았음)
#define MAX_MATRIX		16	// 인증받을 최대 좌표 개수

typedef struct
{
    DWORD   dwType;           // 보안 인증 방법(eSECURE_AUTH_CODE 참조)
    BYTE    byData[MAX_AUTH]; // 유저가 입력할 보안 카드 내의 좌표 인덱스들
} SECURE_INFO, *LP_SECURE_INFO;

typedef struct
{
    WORD    wMatrix[MAX_MATRIX];  // 보안 카드 좌표, 또는 그 좌표의 값들 목록
    BYTE    byFlag[MAX_MATRIX];   // 보안 카드 좌표값의 앞부분, 뒷부분 사용 여부 등의 옵션 조정을 위해 필요한 플래그(현재 사용 안함)
} SC_INFO, *LP_SC_INFO;

#pragma pack(pop)


