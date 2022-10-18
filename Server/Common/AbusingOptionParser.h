#pragma once

#include <Singleton.h>
#include <SolarFileReader.h>

struct ABUSING_OPTION_INFO
{
    char m_szCategoryName[64];
    char m_szViolationName[64];
    BYTE m_AbuseType;           // ��������, ��������, ���, �۾���, �ߺ��ø���, ũ����
    BYTE m_AbuseSubType;        // ���� �������� Ÿ��
    BYTE m_bDisconnect;         // �ѹ� ���ݽ� Disconnect ����
    BYTE m_Level;               // Level1, Level2, Level3(ũ��Ƽ�� ����), Level4(��������), Level5(���)
    WORD m_MilestoneCount;      // ���� ���� ����Ƚ��
    WORD m_IncAbusingPoint;     // ���ݿ� ���� ���¡ ����
    WORD m_MaxAbusingPoint;     // �ش����ݿ� ���� �ִ� ���¡ ����
};

class AbusingOptionParser : public util::Singleton<AbusingOptionParser>, public IParser
{
public:
    AbusingOptionParser();
    ~AbusingOptionParser();

public:
    // (CHANGES)(20100311)(hukim) Init(), Release() �������̵� �޼��� ȥ���� ���� ���� virtual Ű���� �߰�
    // (CHANGES)(20100311)(hukim) GetAbusingOptionInfo() ���� ������ ����Ű�� �ʱ� ������ const �߰�

    virtual VOID Init(DWORD pool_size);
    virtual VOID Release(); 
    virtual BOOL LoadScript(eSCRIPT_CODE script_code, BOOL is_reload);

    ABUSING_OPTION_INFO* GetAbusingOptionInfo(BYTE abuse_sub_type) const;

private:
    // (CHANGES)(20100311)(hukim) LoadOptionInfo() Load()��� �̸��� �޼��尡 �θ� Ŭ������ �̹� ����
    // �ϱ� ������ �����ε��� ���� ȥ���� ���� ���� _Load()���� LoadOptionInfo()�� �̸� ����

    BOOL LoadOptionInfo(BOOL is_reload);

private:
    // (CHANGES)(20100311)(hukim) ABUSING_OPTION_TABLE_IT�� ���Ǵ� ���� ���� ������ �ּ� ó��
    // (CHANGES)(20100311)(hukim) m_pszFileName�� ���Ǵ� ���� ���� ������ �ּ� ó��

    typedef STLX_MAP<BYTE, ABUSING_OPTION_INFO*> ABUSING_OPTION_TABLE;
    ABUSING_OPTION_TABLE abusing_option_table_;

    //typedef ABUSING_OPTION_TABLE::iterator ABUSING_OPTION_TABLE_IT;
    //char m_pszFileName[MAX_PATH];

private:
    // (CHANGES)(20100309)(hukim) �̱��� ��ü�̹Ƿ� ���簡 �Ͼ�� �ȵǱ� ������ __DISABLE_COPY() �߰�

    __DISABLE_COPY(AbusingOptionParser);
};
