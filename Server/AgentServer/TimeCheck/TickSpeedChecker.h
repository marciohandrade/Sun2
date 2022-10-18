#ifndef SERVER_AGENTSERVER_TIMECHECK_TICKSPEEDCHECKER_H_
#define SERVER_AGENTSERVER_TIMECHECK_TICKSPEEDCHECKER_H_

#pragma once

#ifdef _NA_20100802_SPEED_HACK_DETECTION

#include <deque>
#include <hash_set>

// 스피드핵 검사 클래스
class TickSpeedChecker
{
public:
    TickSpeedChecker();
    ~TickSpeedChecker();

    // 주어진 User 객체 참조와 함께 이 TickSpeedChecker 객체를 초기화한다.
	void Init(User* user);
    // 연관된 사용자 계정의 검사 제외/주시 설정을 갱신한다.
    void SetUserID();

    // 검사가 가능한 상태인지의 여부를 설정한다.
	// SecurePackNode4User::EnableHeartbeats() 함수에서 호출된다.
	void SetEnabled(bool flag) {
        enabled_ = flag;
    }
	// 검사가 가능한 상태인지의 여부를 리턴한다.
    bool IsEnabled() const {
        return (!excepted_ && enabled_);
    }

	// 라운드 활성화 여부를 검사하고 활성화되어 있지 않으면 조건에 따라 라운드를 활성화한다.
	bool TryActivate(DWORD current_tick);

    // 실행 루프에서 주기적으로 호출되는 작업 루틴.
	void Update();

	// 클라이언트 응답 수신시 호출되는 작업 루틴.
    void HandleClientResponse(DWORD round, BYTE index, DWORD client_tick, DWORD server_tick2);

    // 설정 파일로부터 전역 설정을 읽어 들인다.
    static void LoadSettings(const TCHAR* filename) {
        GetSettings().Load(filename);
    }

    // 전역적으로 검사가 활성화되어 있는지의 여부를 리턴한다.
    static bool IsGloballyEnabled() {
        return GetSettings().IsEnabled();
    }

    // 전용 로그 파일을 생성한다.
    static void CreateLogFile(const TCHAR* path) {
        GetSettings().CreateLog(path);
    }

private:
    // 평가 판정 등급
    enum AssessmentGrade
    {
        SUSPENDED = -1,         // 판정 유보
        GOOD = 0,               // 양호
        DOUBTFUL,               // 가벼운 의심
        SUSPECT,                // 의심
        KEY_SUSPECT,            // 유력
        SECURITY_RISK,          // 보안 위협
        MAX_ASSESSMENT_GRADE
    };

    // 라운드 검사 결과를 평가하고 그에 따른 후속 조치를 수행한다.
    void Assess();
    // 평가 결과에 따른 후속 조치를 수행한다. Assess() 내부에서 호출된다.
    void Dispose(AssessmentGrade grade, float average_delta_ratio);

	// 라운드 검사 정보를 초기화한다.
	void Reset();

    // 다음 라운드 시작까지의 시간(밀리초 단위)을 결정한다.
    // 범위가 제한된 랜덤 값에 의존한다.
    void PickMillisecsToNextRound();

    // 연관된 사용자를 강제 접속 종료 처리한다.
    void DisconnectUser();

    // 최근 판정 등급 기록에서 주어진 등급 이상의 등급 개수를 리턴한다.
    size_t CountGrades(int grade);

    // 주어진 delta 비율 값을 판정 임계값과 비교해 판정 등급을 리턴한다.
    static AssessmentGrade GetAssessmentGrade(float delta_ratio);

	// 32비트 틱 카운트 from과 to의 차를 리턴한다.
	inline static DWORD GetDifference(DWORD from, DWORD to)
	{
		if (from <= to) {
			return (to - from);
        }
		return ((DWORD)0xffffffff - from) + to; // wrap-around
	}
    // [min, max] 범위의 float 랜덤 값을 리턴한다.
    inline static float frand(float min, float max)
    {
        return min + ( (max - min) * ((float)rand()/RAND_MAX) );
    }

private:
    // 한 라운드 당 반복수
    static const unsigned int kNumRepetitionsPerRound = 10;
    // 한 라운드 당 얻을 수 있는 delta 수는 틱 카운트 표본 수에서 1을 뺀 값이다
    static const unsigned int kNumDeltaPerRound = kNumRepetitionsPerRound - 1;

	User* user_;                                    // 연결된 User 객체 참조
    bool  enabled_;                                 // 검사 가능 여부 (Heartbeat 활성화 상태)
    bool  excepted_;                                // 연결된 사용자가 검사 제외 대상인가
    bool  focused_;                                 // 연결된 사용자가 검사 주시 대상인가
	DWORD last_round_start_tick_;                   // 최근 라운드 시작 틱 카운트
    DWORD millisecs_to_next_round_;                 // 다음 라운드 시작까지의 틱 카운트

	int   repetition_;                              // 라운드 내 현재 반복수
	DWORD server_tick_[kNumRepetitionsPerRound];
	DWORD client_tick_[kNumRepetitionsPerRound];

    std::deque<AssessmentGrade> assessment_;        // 최근 n회의 판정 등급 기록

    size_t num_total_rounds_;                       // 검사 시도한 라운드의 총 수
    size_t num_successful_rounds_;                  // 검사 성공한 (일정 수 이상의 유효 표본을 확보한) 라운드의 총 수

    // 스피드핵 검사 관련 전역 설정 유틸리티 클래스
    class Settings
    {
    public:
        Settings();
        ~Settings();

        // 주어진 전용 설정 파일로부터 설정을 읽어 들인다.
        void Load(const TCHAR* filename);

        // 전역적으로 검사가 활성화되어 있는지의 여부를 리턴한다.
        bool IsEnabled() const {
            return enabled_;
        }
        // 최고 등급의 제재 수단으로 강제 접속 종료를 사용할 것인지의 여부를 리턴한다.
        bool UsingForcedDisconnection() const {
            return force_disconnection_;
        }
        // 최고 등급의 제재 수단으로 강제 접속 종료를 사용할 것인지의 여부를 설정한다.
        void UseForcedDisconnection(bool flag) {
            force_disconnection_ = flag;
        }

        // 주어진 계정이 검사 제외 대상인가를 판단한다.
        bool IsException(const char* user_id) const {
            return (except_users_.find(user_id) != except_users_.end());
        }
        // 주어진 계정이 검사 주시 대상인가를 판단한다.
        bool IsFocus(const char* user_id) const {
            return (focus_users_.find(user_id) != focus_users_.end());
        }

        // 전용 로그 파일을 생성한다.
        void CreateLog(const TCHAR* path);

        // 전용 로그를 얻는다.
        CSunLog* GetLog() {
            return log_;
        }

        // 라운드 시작 사이의 기준 시간 간격 (초 단위)
        static const unsigned int kMillisecsBetweenRoundStart;
        // 판정에 사용할 최근 성공한 라운드의 개수
        static const size_t kNumRecentRounds;

        // 판정 임계값 delta 비율표
        static float kThresholds[MAX_ASSESSMENT_GRADE - 1];
        // 최근 n회의 성공적인 라운드에서 허용할 등급별 개수 상한
        static float kGradeLimits[MAX_ASSESSMENT_GRADE - 1];
        // 판정 등급 문자열 표현
        static const char* kGradeString[MAX_ASSESSMENT_GRADE];

    private:
        bool enabled_;              // 전역 활성화 여부
        bool force_disconnection_;  // 위반 탐지시 강제 접속 종료 사용 여부

        stdext::hash_set<std::string> except_users_;    // 검사 제외 대상 UserID 집합
        stdext::hash_set<std::string> focus_users_;     // 검사 주시 대상 UserID 집합

        CSunLog* log_; // 전용 로그
    };

    // 전역 설정 객체 참조를 리턴한다.
    static Settings& GetSettings() {
        static Settings settings;
        return settings;
    }

    friend class TickSpeedCheckerTest_EffectiveForcedDisconnection_Test;
    friend class TickSpeedCheckerTest_IneffectiveForcedDisconnection_Test;
};

#endif // #ifdef _NA_20100802_SPEED_HACK_DETECTION

#endif // #ifndef SERVER_AGENTSERVER_TIMECHECK_TICKSPEEDCHECKER_H_
