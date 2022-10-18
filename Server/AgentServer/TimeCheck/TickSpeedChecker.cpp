#include "StdAfx.h"

#include "TickSpeedChecker.h"

#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>

#include "PacketStruct_CG.h"

#ifdef _NA_20100802_SPEED_HACK_DETECTION

#define FOCUS_LOG_BEGIN(log_level) if (focused_) { \
    SUNLOGEX(GetSettings().GetLog(), CSunLog::c##log_level, CSunLog::cOUTPUT_FILE, CSunLog::cCATE_ABUSE,
#define FOCUS_LOG_END ); }

// 라운드 시작 사이의 기준 시간 간격 (초 단위)
// 최소한 (Update() 시간 해상도 (1초) * NUM_REPETITIONS_PER_ROUND) 보다는 크도록
// 랜덤 범위를 감안해 설정되어야 한다.
const unsigned int TickSpeedChecker::Settings::kMillisecsBetweenRoundStart = 60;
// 판정에 사용할 최근 성공한 라운드의 개수
const size_t TickSpeedChecker::Settings::kNumRecentRounds = 5;

// 판정 임계값 delta 비율표
float TickSpeedChecker::Settings::kThresholds[MAX_ASSESSMENT_GRADE - 1] = { \
    1.1f, // DOUBTFUL
    1.2f, // SUSPECT
    1.4f, // KEY_SUSPECT
    1.8f  // SECURITY_RISK
};

// 최근 n회의 성공적인 라운드에서 허용할 등급별 개수 상한
float TickSpeedChecker::Settings::kGradeLimits[MAX_ASSESSMENT_GRADE - 1] = { \
    4, // DOUBTFUL
    3, // SUSPECT
    2, // KEY_SUSPECT
    1  // SECURITY_THREAT
};

// 판정 등급 문자열 표현
const char* TickSpeedChecker::Settings::kGradeString[MAX_ASSESSMENT_GRADE] = { \
    "GOOD",
    "DOUBTFUL",
    "SUSPECT",
    "KEY_SUSPECT",
    "SECURITY_RISK"
};

TickSpeedChecker::TickSpeedChecker()
	: user_(NULL)
	, enabled_(false)
    , excepted_(false)
    , focused_(false)
{
}

TickSpeedChecker::~TickSpeedChecker()
{
}

void TickSpeedChecker::Init(User* user)
{
	user_ = user;
	ASSERT(user_ != NULL);

    last_round_start_tick_ = 0;
    PickMillisecsToNextRound(); // 다음 라운드까지의 시간 간격 초기화

    num_total_rounds_ = 0;
    num_successful_rounds_ = 0;
    assessment_.clear();

	Reset();
}

void TickSpeedChecker::SetUserID()
{
    const TCHAR* user_id = user_->GetUserID();
    excepted_ = GetSettings().IsException(user_id);
    focused_ = GetSettings().IsFocus(user_id);

    const TCHAR* status = _T("Normal");
    if (excepted_) {
        status = _T("Excepted");
    }
    else if (focused_) {
        status = _T("Focused");
    }

    SUNLOG(eCRITICAL_LOG, "[TickSpeedChecker] Activating on user account '%s' : %s", user_id, status);
}

bool TickSpeedChecker::TryActivate(DWORD current_tick)
{
    if ( GetDifference(last_round_start_tick_, current_tick) > millisecs_to_next_round_ )
    {
        // 최초 1회는 건너뛴다
        if (last_round_start_tick_ == 0) {
            last_round_start_tick_ = current_tick;
            PickMillisecsToNextRound();
            return false;
        }
        last_round_start_tick_ = current_tick;
        PickMillisecsToNextRound();
        if (repetition_ >= 0) {
            Assess(); // 이전 라운드가 종료되지 않았으면 강제 평가/종료
        }
        repetition_ = 0;

        FOCUS_LOG_BEGIN(FULL_LOG)
            "[%s][%s] Round #%u started: next round scheduled after %u msecs", 
                user_->GetUserID(), user_->GetCharName(), (num_total_rounds_+1), millisecs_to_next_round_
        FOCUS_LOG_END
        return true;
    }
    return (repetition_ >= 0);
}

void TickSpeedChecker::Update()
{
    if (!IsEnabled()) {
        return;
    }
	ASSERT(repetition_ >= 0);
	if (repetition_ < 0 || repetition_ >= kNumRepetitionsPerRound) {
		return;
	}

	// 서버의 현재 틱 카운트를 저장해 두고
	server_tick_[repetition_] = GetTickCount();

	// 클라이언트로 틱 질의 패킷을 전송한다.
	MSG_CG_CONNECTION_TICK_QUERY_SYN syn;
    syn.round = static_cast<DWORD>(num_total_rounds_ + 1);
	syn.index = static_cast<BYTE>(repetition_++);
	user_->SendPacket(&syn, sizeof(syn));

    FOCUS_LOG_BEGIN(DEV_LOG)
        "[%s][%s] Round #%u repetition %d: probing on server tick %u", 
            user_->GetUserID(), user_->GetCharName(), (num_total_rounds_+1), (repetition_-1), server_tick_[repetition_-1]
    FOCUS_LOG_END
}

void TickSpeedChecker::HandleClientResponse(DWORD round, BYTE index, DWORD client_tick, DWORD server_tick2)
{
    if ( round != static_cast<DWORD>(num_total_rounds_ + 1) )
    {
        FOCUS_LOG_BEGIN(FULL_LOG)
            "[%s][%s] Abnormal client round number: %u (expected %u)", 
                user_->GetUserID(), user_->GetCharName(), round, (num_total_rounds_+1)
        FOCUS_LOG_END
        return;
    }
	ASSERT(index < kNumRepetitionsPerRound);
	if (index >= kNumRepetitionsPerRound)
    {
        FOCUS_LOG_BEGIN(FULL_LOG)
            "[%s][%s] Abnormal client repetition index: %d", 
                user_->GetUserID(), user_->GetCharName(), index
        FOCUS_LOG_END
		return;
	}

	DWORD server_tick = server_tick_[index];
	client_tick_[index] = client_tick;
	DWORD rtt = GetDifference(server_tick, server_tick2); // rtt = server_tick2 - server_tick
	server_tick_[index] = server_tick + (rtt/2);

    FOCUS_LOG_BEGIN(DEV_LOG)
        "[%s][%s] Round #%u repetition %d: client tick %u on server tick %u (sTm=%u)", 
            user_->GetUserID(), user_->GetCharName(), (num_total_rounds_+1), index, client_tick, server_tick2, server_tick_[index]
    FOCUS_LOG_END

    if (index == kNumDeltaPerRound) {
        Assess();
    }
}

void TickSpeedChecker::Assess()
{
    int i, count;
    float delta_ratio[kNumDeltaPerRound];

    for (i = 0; i < kNumDeltaPerRound; ++i) {
        delta_ratio[i] = 0.0f; // delta 비율 공간 초기화
    }
    // delta 비율 계산
    for (i = 0, count = 0; i < kNumDeltaPerRound; ++i)
    {
        if ( (client_tick_[i] == 0) || (client_tick_[i + 1] == 0) ) {
            continue;
        }
        DWORD client_delta = GetDifference(client_tick_[i], client_tick_[i + 1]);
        DWORD server_delta = GetDifference(server_tick_[i], server_tick_[i + 1]);

        if (client_delta > 0x7fffffff)
        {
            // 정상적인 wrap-around가 아니라 다음 값이 이전 값보다 약간 앞으로 돌아가 있다는 것이다
            // 클라이언트 측에서의 비정상적인 조작이 아니라면 불가능한 값이다
            // 최고 등급의 제재를 가할 수 있다

            FOCUS_LOG_BEGIN(CRITICAL_LOG)
                "[%s][%s] Abnormal client tick: [%d]=%u, [%d]=%u", 
                    user_->GetUserID(), user_->GetCharName(), i, client_tick_[i], i + 1, client_tick_[i + 1]
            FOCUS_LOG_END

            TCHAR description[MAX_ADDINFO_SIZE + 1];
            _sntprintf(description, MAX_ADDINFO_SIZE, "Abnormal Client Tick [%d]=%u, [%d]=%u", \
                i, client_tick_[i], i + 1, client_tick_[i + 1]);
            description[MAX_ADDINFO_SIZE] = '\0';
            GAMELOG->LogSpeedHackSuspect(user_, description);

            DisconnectUser();
            return;
        }

        // 판정 기준이 되는 delta 비율은 (클라이언트 틱 차이)/(서버 틱 차이)이다
        delta_ratio[i] = (float)client_delta / server_delta;

        FOCUS_LOG_BEGIN(DEV_LOG)
            "[%s][%s] Round #%u assessment: %d-%d %.2f (%u/%u)", 
                user_->GetUserID(), user_->GetCharName(), (num_total_rounds_+1), i, (i+1), delta_ratio[i], client_delta, server_delta
        FOCUS_LOG_END
    }

    // 평균 계산에서 배제할 최대/최소값을 결정한다
    // 단, 최대/최소값이 두 번 이상 나타난다면 배제하지 않도록 한다.
    float max_ratio = 0.0f;
    float min_ratio = 1000000.0f;
    for (i = 0, count = 0; i < kNumDeltaPerRound; ++i)
    {
        float ratio = delta_ratio[i];
        if (ratio < 0.9f) {
            continue;
        }
        if (ratio >= max_ratio) {
            max_ratio = (ratio != max_ratio ? ratio : 1000000.0f);
        }
        if (ratio <= min_ratio) {
            min_ratio = (ratio != min_ratio ? ratio : 0.0f);
        }
    }
    // 최대/최소값을 제외하고 delta 비율의 1차 평균을 계산한다
    float sum = 0.0f;
    for (i = 0, count = 0; i < kNumDeltaPerRound; ++i)
    {
        float ratio = delta_ratio[i];
        // 게임에서 시간을 느리게 함으로써 얻을 수 있는 이득이 없고
        // 클라이언트가 몰아서 응답하는 경우 client_delta가 0에 가까와질 수 있으므로
        // delta 비율 정상값(1)의 90% 미만 값들은 버린다
        if (ratio < 0.9f) {
            continue;
        }
        if (ratio <= min_ratio || max_ratio <= ratio) {
            continue;
        }
        sum += ratio;
        ++count;
    }

    float average = sum / count;
    float lower_bound = average * 0.67f;
    float upper_bound = average * 1.33f;
    // 1차 평균 값을 33% 이상 넘어서거나 밑도는 예외값들과 최대/최소값을 제외하고 다시 평균을 구한다
    sum = 0.0f;
    for (i = 0, count = 0; i < kNumDeltaPerRound; ++i)
    {
        float ratio = delta_ratio[i];
        if (ratio < 0.9f) {
            continue;
        }
        if (ratio <= min_ratio || max_ratio <= ratio) {
            continue;
        }
        if (ratio < lower_bound || upper_bound < ratio) {
            continue;
        }
        sum += ratio;
        ++count;
    }

    average = 0.0f;
    AssessmentGrade grade = SUSPENDED;
    // 유효한 delta 표본 수가 라운드에서 얻을 수 있는 정상 값의 50% 이하인 경우
    if ( count <= (kNumDeltaPerRound / 2) ) {
        // 이 라운드 데이터는 판정에 사용하지 않는다
        FOCUS_LOG_BEGIN(FULL_LOG)
            "[%s][%s] Round #%u failed: insufficient delta sample %d", 
                user_->GetUserID(), user_->GetCharName(), (num_total_rounds_+1), count
        FOCUS_LOG_END
    }
    else
    {
        // 최종 평균
        average = sum / count;

        grade = GetAssessmentGrade(average);

        assessment_.push_back(grade);
        while (assessment_.size() > Settings::kNumRecentRounds) {
            assessment_.pop_front();
        }

        FOCUS_LOG_BEGIN(FULL_LOG)
            "[%s][%s] Round #%u succeeded: %s (average delta ratio %.2f)", 
                user_->GetUserID(), user_->GetCharName(), (num_total_rounds_+1), Settings::kGradeString[grade], average
        FOCUS_LOG_END

        ++num_successful_rounds_;
    }

    ++num_total_rounds_;

    Dispose(grade, average);
    Reset();
}

void TickSpeedChecker::Dispose(AssessmentGrade grade, float average_delta_ratio)
{
    // 검사 실패한 라운드의 경우
    if (grade == SUSPENDED)
    {
        // 검사 라운드 성공율이 극히 비정상적인 경우 보안 위협으로 처리
        float round_success_ratio = (float)num_successful_rounds_ / num_total_rounds_;
        if (round_success_ratio < 0.01f && num_total_rounds_ >= 100)
        {
            FOCUS_LOG_BEGIN(CRITICAL_LOG)
                "[%s][%s] Abnormal round completion rate: succeeded %u / total %u", 
                    user_->GetUserID(), user_->GetCharName(), num_successful_rounds_, num_total_rounds_
            FOCUS_LOG_END

            // 시도된 총 라운드 수가 100회가 넘는데 1% 미만의 성공율을 보인다면
            // 최고 등급의 제재를 가할 수 있다
            TCHAR description[MAX_ADDINFO_SIZE + 1];
            _sntprintf(description, MAX_ADDINFO_SIZE, "Abnormal Round Completion %u/%u", \
                num_successful_rounds_, num_total_rounds_);
            description[MAX_ADDINFO_SIZE] = '\0';
            GAMELOG->LogSpeedHackSuspect(user_, description);

            DisconnectUser();
        }
        return;
    }

    // 성공한 라운드 수가 기준에 미치지 못한다면 판정을 유보한다
    if (assessment_.size() < Settings::kNumRecentRounds) {
        return;
    }

    for (int i = SECURITY_RISK; i > GOOD; --i)
    {
        size_t n = CountGrades(i);
        if (n >= Settings::kGradeLimits[i - 1])
        {
            FOCUS_LOG_BEGIN(CRITICAL_LOG)
                "[%s][%s] Speed hack suspected: %s", 
                    user_->GetUserID(), user_->GetCharName(), Settings::kGradeString[i]
            FOCUS_LOG_END

            TCHAR description[MAX_ADDINFO_SIZE + 1];
            _sntprintf(description, MAX_ADDINFO_SIZE, "%u %s last_d_ratio=%.2f", \
                n, Settings::kGradeString[i], average_delta_ratio);
            description[MAX_ADDINFO_SIZE] = '\0';
            GAMELOG->LogSpeedHackSuspect(user_, description);

            assessment_.clear(); // 강제 접속 종료를 사용하지 않는 경우를 위해 최근 평가 등급 기록 초기화

            DisconnectUser();
            return;
        }
    }
}

void TickSpeedChecker::Reset()
{
	repetition_ = -1;
	for (int i = 0; i < kNumRepetitionsPerRound; ++i)
    {
		server_tick_[i] = 0;
		client_tick_[i] = 0;
	}
}

void TickSpeedChecker::PickMillisecsToNextRound()
{
    // 기준 시간 간격의 -33% ~ +33% 사이로 다음 라운드 시작까지의 시간 간격 결정
    millisecs_to_next_round_ = static_cast<DWORD>( \
        (Settings::kMillisecsBetweenRoundStart * 1000) * frand(0.67f, 1.33f) );
}

void TickSpeedChecker::DisconnectUser()
{
    if (GetSettings().UsingForcedDisconnection() == false) {
        return;
    }

    GAMELOG->LogSpeedHackSuspectDisconnection(user_);

    MSG_CG_CONNECTION_DISCONNECT_CMD cmd;
    cmd.m_dwErrorCode = RC::RC_CONNECTION_REQUESTFROMGAMESERVER;
    user_->SendPacket(&cmd, sizeof(cmd));

    user_->SetDisconnectCode(RC::RC_DISCONNECT_SPEED_HACK_SUSPECTED);
    user_->DisconnectUser();
}

struct GTE
{
    GTE(int value) : value_(value) {}
    bool operator()(int value) {
        return (value >= value_);
    }
private:
    int value_;
};

size_t TickSpeedChecker::CountGrades(int grade)
{
    return count_if(assessment_.begin(), assessment_.end(), GTE(grade));
}

TickSpeedChecker::AssessmentGrade TickSpeedChecker::GetAssessmentGrade(float delta_ratio)
{
    for (int i = SECURITY_RISK; i > GOOD; --i) {
        if (delta_ratio >= Settings::kThresholds[i - 1]) {
            return static_cast<AssessmentGrade>(i);
        }
    }
    return GOOD;
}

TickSpeedChecker::Settings::Settings()
    : enabled_(false)
    , force_disconnection_(false)
    , log_(NULL)
{
}

TickSpeedChecker::Settings::~Settings()
{
    SAFE_DELETE(log_)
}

void TickSpeedChecker::Settings::Load(const TCHAR* filename)
{
    enabled_ = (GetPrivateProfileInt(_T("GLOBAL"), _T("ENABLE"), 0, filename) != 0 ? true : false);
    force_disconnection_ = (GetPrivateProfileInt(_T("GLOBAL"), _T("FORCE_DISCONN"), 0, filename) != 0 ? true : false);

#ifdef _CHINA
    if (GetPrivateProfileInt(_T("GLOBAL"), _T("ENABLE"), UINT_MAX, filename) == UINT_MAX) {
        enabled_ = true; // 중국에 한해 특별히 지정되지 않으면 활성화
    }
#endif

    SUNLOG(eCRITICAL_LOG, "[SpeedHackChecker] GLOBAL:ENABLE = %d", enabled_);
    SUNLOG(eCRITICAL_LOG, "[SpeedHackChecker] GLOBAL:FORCE_DISCONN = %d", force_disconnection_);

    TCHAR buffer[2048+1]; // 읽어 들이는 설정 문자열 최대 길이를 2048로 제한

    DWORD result = GetPrivateProfileString(_T("USERS"), _T("EXCEPT"), _T(""), buffer, _countof(buffer), filename);
    buffer[_countof(buffer) - 1] = '\0';
    if (result > 0 && _tcslen(buffer) > 0)
    {
        SUNLOG(eCRITICAL_LOG, "[SpeedHackChecker] USERS:EXCEPT = %s", buffer);
        char* token = strtok(buffer, ",");
        while (token != 0)
        {
            except_users_.insert(token);
            token = strtok(NULL, ",");
        }
    }

    result = GetPrivateProfileString(_T("USERS"), _T("FOCUS"), _T(""), buffer, _countof(buffer), filename);
    buffer[_countof(buffer) - 1] = '\0';
    if (result > 0 && _tcslen(buffer) > 0)
    {
        SUNLOG(eCRITICAL_LOG, "[SpeedHackChecker] USERS:FOCUS = %s", buffer);
        char* token = strtok(buffer, ",");
        while (token != 0)
        {
            focus_users_.insert(token);
            token = strtok(NULL, ",");
        }
    }
}

void TickSpeedChecker::Settings::CreateLog(const TCHAR* path)
{
    if (log_ != NULL) {
        return;
    }
    log_ = new CSunLog();
    log_->Init(OUTPUT_BOTH, eDEV_LOG, path, "Agent.TickSpeedChecker");
}

// EOF TickSpeedChecker.cpp

#endif // #ifdef _NA_20100802_SPEED_HACK_DETECTION
