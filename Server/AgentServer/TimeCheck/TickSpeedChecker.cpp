#include "StdAfx.h"

#include "TickSpeedChecker.h"

#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>

#include "PacketStruct_CG.h"

#ifdef _NA_20100802_SPEED_HACK_DETECTION

#define FOCUS_LOG_BEGIN(log_level) if (focused_) { \
    SUNLOGEX(GetSettings().GetLog(), CSunLog::c##log_level, CSunLog::cOUTPUT_FILE, CSunLog::cCATE_ABUSE,
#define FOCUS_LOG_END ); }

// ���� ���� ������ ���� �ð� ���� (�� ����)
// �ּ��� (Update() �ð� �ػ� (1��) * NUM_REPETITIONS_PER_ROUND) ���ٴ� ũ����
// ���� ������ ������ �����Ǿ�� �Ѵ�.
const unsigned int TickSpeedChecker::Settings::kMillisecsBetweenRoundStart = 60;
// ������ ����� �ֱ� ������ ������ ����
const size_t TickSpeedChecker::Settings::kNumRecentRounds = 5;

// ���� �Ӱ谪 delta ����ǥ
float TickSpeedChecker::Settings::kThresholds[MAX_ASSESSMENT_GRADE - 1] = { \
    1.1f, // DOUBTFUL
    1.2f, // SUSPECT
    1.4f, // KEY_SUSPECT
    1.8f  // SECURITY_RISK
};

// �ֱ� nȸ�� �������� ���忡�� ����� ��޺� ���� ����
float TickSpeedChecker::Settings::kGradeLimits[MAX_ASSESSMENT_GRADE - 1] = { \
    4, // DOUBTFUL
    3, // SUSPECT
    2, // KEY_SUSPECT
    1  // SECURITY_THREAT
};

// ���� ��� ���ڿ� ǥ��
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
    PickMillisecsToNextRound(); // ���� ��������� �ð� ���� �ʱ�ȭ

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
        // ���� 1ȸ�� �ǳʶڴ�
        if (last_round_start_tick_ == 0) {
            last_round_start_tick_ = current_tick;
            PickMillisecsToNextRound();
            return false;
        }
        last_round_start_tick_ = current_tick;
        PickMillisecsToNextRound();
        if (repetition_ >= 0) {
            Assess(); // ���� ���尡 ������� �ʾ����� ���� ��/����
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

	// ������ ���� ƽ ī��Ʈ�� ������ �ΰ�
	server_tick_[repetition_] = GetTickCount();

	// Ŭ���̾�Ʈ�� ƽ ���� ��Ŷ�� �����Ѵ�.
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
        delta_ratio[i] = 0.0f; // delta ���� ���� �ʱ�ȭ
    }
    // delta ���� ���
    for (i = 0, count = 0; i < kNumDeltaPerRound; ++i)
    {
        if ( (client_tick_[i] == 0) || (client_tick_[i + 1] == 0) ) {
            continue;
        }
        DWORD client_delta = GetDifference(client_tick_[i], client_tick_[i + 1]);
        DWORD server_delta = GetDifference(server_tick_[i], server_tick_[i + 1]);

        if (client_delta > 0x7fffffff)
        {
            // �������� wrap-around�� �ƴ϶� ���� ���� ���� ������ �ణ ������ ���ư� �ִٴ� ���̴�
            // Ŭ���̾�Ʈ �������� ���������� ������ �ƴ϶�� �Ұ����� ���̴�
            // �ְ� ����� ���縦 ���� �� �ִ�

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

        // ���� ������ �Ǵ� delta ������ (Ŭ���̾�Ʈ ƽ ����)/(���� ƽ ����)�̴�
        delta_ratio[i] = (float)client_delta / server_delta;

        FOCUS_LOG_BEGIN(DEV_LOG)
            "[%s][%s] Round #%u assessment: %d-%d %.2f (%u/%u)", 
                user_->GetUserID(), user_->GetCharName(), (num_total_rounds_+1), i, (i+1), delta_ratio[i], client_delta, server_delta
        FOCUS_LOG_END
    }

    // ��� ��꿡�� ������ �ִ�/�ּҰ��� �����Ѵ�
    // ��, �ִ�/�ּҰ��� �� �� �̻� ��Ÿ���ٸ� �������� �ʵ��� �Ѵ�.
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
    // �ִ�/�ּҰ��� �����ϰ� delta ������ 1�� ����� ����Ѵ�
    float sum = 0.0f;
    for (i = 0, count = 0; i < kNumDeltaPerRound; ++i)
    {
        float ratio = delta_ratio[i];
        // ���ӿ��� �ð��� ������ �����ν� ���� �� �ִ� �̵��� ����
        // Ŭ���̾�Ʈ�� ���Ƽ� �����ϴ� ��� client_delta�� 0�� ������� �� �����Ƿ�
        // delta ���� ����(1)�� 90% �̸� ������ ������
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
    // 1�� ��� ���� 33% �̻� �Ѿ�ų� �ص��� ���ܰ���� �ִ�/�ּҰ��� �����ϰ� �ٽ� ����� ���Ѵ�
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
    // ��ȿ�� delta ǥ�� ���� ���忡�� ���� �� �ִ� ���� ���� 50% ������ ���
    if ( count <= (kNumDeltaPerRound / 2) ) {
        // �� ���� �����ʹ� ������ ������� �ʴ´�
        FOCUS_LOG_BEGIN(FULL_LOG)
            "[%s][%s] Round #%u failed: insufficient delta sample %d", 
                user_->GetUserID(), user_->GetCharName(), (num_total_rounds_+1), count
        FOCUS_LOG_END
    }
    else
    {
        // ���� ���
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
    // �˻� ������ ������ ���
    if (grade == SUSPENDED)
    {
        // �˻� ���� �������� ���� ���������� ��� ���� �������� ó��
        float round_success_ratio = (float)num_successful_rounds_ / num_total_rounds_;
        if (round_success_ratio < 0.01f && num_total_rounds_ >= 100)
        {
            FOCUS_LOG_BEGIN(CRITICAL_LOG)
                "[%s][%s] Abnormal round completion rate: succeeded %u / total %u", 
                    user_->GetUserID(), user_->GetCharName(), num_successful_rounds_, num_total_rounds_
            FOCUS_LOG_END

            // �õ��� �� ���� ���� 100ȸ�� �Ѵµ� 1% �̸��� �������� ���δٸ�
            // �ְ� ����� ���縦 ���� �� �ִ�
            TCHAR description[MAX_ADDINFO_SIZE + 1];
            _sntprintf(description, MAX_ADDINFO_SIZE, "Abnormal Round Completion %u/%u", \
                num_successful_rounds_, num_total_rounds_);
            description[MAX_ADDINFO_SIZE] = '\0';
            GAMELOG->LogSpeedHackSuspect(user_, description);

            DisconnectUser();
        }
        return;
    }

    // ������ ���� ���� ���ؿ� ��ġ�� ���Ѵٸ� ������ �����Ѵ�
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

            assessment_.clear(); // ���� ���� ���Ḧ ������� �ʴ� ��츦 ���� �ֱ� �� ��� ��� �ʱ�ȭ

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
    // ���� �ð� ������ -33% ~ +33% ���̷� ���� ���� ���۱����� �ð� ���� ����
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
        enabled_ = true; // �߱��� ���� Ư���� �������� ������ Ȱ��ȭ
    }
#endif

    SUNLOG(eCRITICAL_LOG, "[SpeedHackChecker] GLOBAL:ENABLE = %d", enabled_);
    SUNLOG(eCRITICAL_LOG, "[SpeedHackChecker] GLOBAL:FORCE_DISCONN = %d", force_disconnection_);

    TCHAR buffer[2048+1]; // �о� ���̴� ���� ���ڿ� �ִ� ���̸� 2048�� ����

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
