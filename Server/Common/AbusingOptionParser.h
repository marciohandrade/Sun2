#pragma once

#include <Singleton.h>
#include <SolarFileReader.h>

struct ABUSING_OPTION_INFO
{
    char m_szCategoryName[64];
    char m_szViolationName[64];
    BYTE m_AbuseType;           // 로직위반, 강제종료, 블락, 작업장, 중복시리얼, 크래쉬
    BYTE m_AbuseSubType;        // 세부 로직위반 타입
    BYTE m_bDisconnect;         // 한번 위반시 Disconnect 여부
    BYTE m_Level;               // Level1, Level2, Level3(크리티컬 위반), Level4(강제종료), Level5(블락)
    WORD m_MilestoneCount;      // 보고를 위한 위반횟수
    WORD m_IncAbusingPoint;     // 위반에 따른 어뷰징 점수
    WORD m_MaxAbusingPoint;     // 해당위반에 따른 최대 어뷰징 점수
};

class AbusingOptionParser : public util::Singleton<AbusingOptionParser>, public IParser
{
public:
    AbusingOptionParser();
    ~AbusingOptionParser();

public:
    // (CHANGES)(20100311)(hukim) Init(), Release() 오버라이딩 메서드 혼동을 막기 위해 virtual 키워드 추가
    // (CHANGES)(20100311)(hukim) GetAbusingOptionInfo() 내부 변경을 일으키지 않기 때문에 const 추가

    virtual VOID Init(DWORD pool_size);
    virtual VOID Release(); 
    virtual BOOL LoadScript(eSCRIPT_CODE script_code, BOOL is_reload);

    ABUSING_OPTION_INFO* GetAbusingOptionInfo(BYTE abuse_sub_type) const;

private:
    // (CHANGES)(20100311)(hukim) LoadOptionInfo() Load()라는 이름의 메서드가 부모 클래스에 이미 존재
    // 하기 때문에 오버로딩에 대한 혼동을 막기 위해 _Load()에서 LoadOptionInfo()로 이름 변경

    BOOL LoadOptionInfo(BOOL is_reload);

private:
    // (CHANGES)(20100311)(hukim) ABUSING_OPTION_TABLE_IT는 사용되는 곳이 없기 때문에 주석 처리
    // (CHANGES)(20100311)(hukim) m_pszFileName는 사용되는 곳이 없기 때문에 주석 처리

    typedef STLX_MAP<BYTE, ABUSING_OPTION_INFO*> ABUSING_OPTION_TABLE;
    ABUSING_OPTION_TABLE abusing_option_table_;

    //typedef ABUSING_OPTION_TABLE::iterator ABUSING_OPTION_TABLE_IT;
    //char m_pszFileName[MAX_PATH];

private:
    // (CHANGES)(20100309)(hukim) 싱글톤 객체이므로 복사가 일어나선 안되기 때문에 __DISABLE_COPY() 추가

    __DISABLE_COPY(AbusingOptionParser);
};
