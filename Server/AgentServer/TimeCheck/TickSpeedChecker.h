#ifndef SERVER_AGENTSERVER_TIMECHECK_TICKSPEEDCHECKER_H_
#define SERVER_AGENTSERVER_TIMECHECK_TICKSPEEDCHECKER_H_

#pragma once

#ifdef _NA_20100802_SPEED_HACK_DETECTION

#include <deque>
#include <hash_set>

// ���ǵ��� �˻� Ŭ����
class TickSpeedChecker
{
public:
    TickSpeedChecker();
    ~TickSpeedChecker();

    // �־��� User ��ü ������ �Բ� �� TickSpeedChecker ��ü�� �ʱ�ȭ�Ѵ�.
	void Init(User* user);
    // ������ ����� ������ �˻� ����/�ֽ� ������ �����Ѵ�.
    void SetUserID();

    // �˻簡 ������ ���������� ���θ� �����Ѵ�.
	// SecurePackNode4User::EnableHeartbeats() �Լ����� ȣ��ȴ�.
	void SetEnabled(bool flag) {
        enabled_ = flag;
    }
	// �˻簡 ������ ���������� ���θ� �����Ѵ�.
    bool IsEnabled() const {
        return (!excepted_ && enabled_);
    }

	// ���� Ȱ��ȭ ���θ� �˻��ϰ� Ȱ��ȭ�Ǿ� ���� ������ ���ǿ� ���� ���带 Ȱ��ȭ�Ѵ�.
	bool TryActivate(DWORD current_tick);

    // ���� �������� �ֱ������� ȣ��Ǵ� �۾� ��ƾ.
	void Update();

	// Ŭ���̾�Ʈ ���� ���Ž� ȣ��Ǵ� �۾� ��ƾ.
    void HandleClientResponse(DWORD round, BYTE index, DWORD client_tick, DWORD server_tick2);

    // ���� ���Ϸκ��� ���� ������ �о� ���δ�.
    static void LoadSettings(const TCHAR* filename) {
        GetSettings().Load(filename);
    }

    // ���������� �˻簡 Ȱ��ȭ�Ǿ� �ִ����� ���θ� �����Ѵ�.
    static bool IsGloballyEnabled() {
        return GetSettings().IsEnabled();
    }

    // ���� �α� ������ �����Ѵ�.
    static void CreateLogFile(const TCHAR* path) {
        GetSettings().CreateLog(path);
    }

private:
    // �� ���� ���
    enum AssessmentGrade
    {
        SUSPENDED = -1,         // ���� ����
        GOOD = 0,               // ��ȣ
        DOUBTFUL,               // ������ �ǽ�
        SUSPECT,                // �ǽ�
        KEY_SUSPECT,            // ����
        SECURITY_RISK,          // ���� ����
        MAX_ASSESSMENT_GRADE
    };

    // ���� �˻� ����� ���ϰ� �׿� ���� �ļ� ��ġ�� �����Ѵ�.
    void Assess();
    // �� ����� ���� �ļ� ��ġ�� �����Ѵ�. Assess() ���ο��� ȣ��ȴ�.
    void Dispose(AssessmentGrade grade, float average_delta_ratio);

	// ���� �˻� ������ �ʱ�ȭ�Ѵ�.
	void Reset();

    // ���� ���� ���۱����� �ð�(�и��� ����)�� �����Ѵ�.
    // ������ ���ѵ� ���� ���� �����Ѵ�.
    void PickMillisecsToNextRound();

    // ������ ����ڸ� ���� ���� ���� ó���Ѵ�.
    void DisconnectUser();

    // �ֱ� ���� ��� ��Ͽ��� �־��� ��� �̻��� ��� ������ �����Ѵ�.
    size_t CountGrades(int grade);

    // �־��� delta ���� ���� ���� �Ӱ谪�� ���� ���� ����� �����Ѵ�.
    static AssessmentGrade GetAssessmentGrade(float delta_ratio);

	// 32��Ʈ ƽ ī��Ʈ from�� to�� ���� �����Ѵ�.
	inline static DWORD GetDifference(DWORD from, DWORD to)
	{
		if (from <= to) {
			return (to - from);
        }
		return ((DWORD)0xffffffff - from) + to; // wrap-around
	}
    // [min, max] ������ float ���� ���� �����Ѵ�.
    inline static float frand(float min, float max)
    {
        return min + ( (max - min) * ((float)rand()/RAND_MAX) );
    }

private:
    // �� ���� �� �ݺ���
    static const unsigned int kNumRepetitionsPerRound = 10;
    // �� ���� �� ���� �� �ִ� delta ���� ƽ ī��Ʈ ǥ�� ������ 1�� �� ���̴�
    static const unsigned int kNumDeltaPerRound = kNumRepetitionsPerRound - 1;

	User* user_;                                    // ����� User ��ü ����
    bool  enabled_;                                 // �˻� ���� ���� (Heartbeat Ȱ��ȭ ����)
    bool  excepted_;                                // ����� ����ڰ� �˻� ���� ����ΰ�
    bool  focused_;                                 // ����� ����ڰ� �˻� �ֽ� ����ΰ�
	DWORD last_round_start_tick_;                   // �ֱ� ���� ���� ƽ ī��Ʈ
    DWORD millisecs_to_next_round_;                 // ���� ���� ���۱����� ƽ ī��Ʈ

	int   repetition_;                              // ���� �� ���� �ݺ���
	DWORD server_tick_[kNumRepetitionsPerRound];
	DWORD client_tick_[kNumRepetitionsPerRound];

    std::deque<AssessmentGrade> assessment_;        // �ֱ� nȸ�� ���� ��� ���

    size_t num_total_rounds_;                       // �˻� �õ��� ������ �� ��
    size_t num_successful_rounds_;                  // �˻� ������ (���� �� �̻��� ��ȿ ǥ���� Ȯ����) ������ �� ��

    // ���ǵ��� �˻� ���� ���� ���� ��ƿ��Ƽ Ŭ����
    class Settings
    {
    public:
        Settings();
        ~Settings();

        // �־��� ���� ���� ���Ϸκ��� ������ �о� ���δ�.
        void Load(const TCHAR* filename);

        // ���������� �˻簡 Ȱ��ȭ�Ǿ� �ִ����� ���θ� �����Ѵ�.
        bool IsEnabled() const {
            return enabled_;
        }
        // �ְ� ����� ���� �������� ���� ���� ���Ḧ ����� �������� ���θ� �����Ѵ�.
        bool UsingForcedDisconnection() const {
            return force_disconnection_;
        }
        // �ְ� ����� ���� �������� ���� ���� ���Ḧ ����� �������� ���θ� �����Ѵ�.
        void UseForcedDisconnection(bool flag) {
            force_disconnection_ = flag;
        }

        // �־��� ������ �˻� ���� ����ΰ��� �Ǵ��Ѵ�.
        bool IsException(const char* user_id) const {
            return (except_users_.find(user_id) != except_users_.end());
        }
        // �־��� ������ �˻� �ֽ� ����ΰ��� �Ǵ��Ѵ�.
        bool IsFocus(const char* user_id) const {
            return (focus_users_.find(user_id) != focus_users_.end());
        }

        // ���� �α� ������ �����Ѵ�.
        void CreateLog(const TCHAR* path);

        // ���� �α׸� ��´�.
        CSunLog* GetLog() {
            return log_;
        }

        // ���� ���� ������ ���� �ð� ���� (�� ����)
        static const unsigned int kMillisecsBetweenRoundStart;
        // ������ ����� �ֱ� ������ ������ ����
        static const size_t kNumRecentRounds;

        // ���� �Ӱ谪 delta ����ǥ
        static float kThresholds[MAX_ASSESSMENT_GRADE - 1];
        // �ֱ� nȸ�� �������� ���忡�� ����� ��޺� ���� ����
        static float kGradeLimits[MAX_ASSESSMENT_GRADE - 1];
        // ���� ��� ���ڿ� ǥ��
        static const char* kGradeString[MAX_ASSESSMENT_GRADE];

    private:
        bool enabled_;              // ���� Ȱ��ȭ ����
        bool force_disconnection_;  // ���� Ž���� ���� ���� ���� ��� ����

        stdext::hash_set<std::string> except_users_;    // �˻� ���� ��� UserID ����
        stdext::hash_set<std::string> focus_users_;     // �˻� �ֽ� ��� UserID ����

        CSunLog* log_; // ���� �α�
    };

    // ���� ���� ��ü ������ �����Ѵ�.
    static Settings& GetSettings() {
        static Settings settings;
        return settings;
    }

    friend class TickSpeedCheckerTest_EffectiveForcedDisconnection_Test;
    friend class TickSpeedCheckerTest_IneffectiveForcedDisconnection_Test;
};

#endif // #ifdef _NA_20100802_SPEED_HACK_DETECTION

#endif // #ifndef SERVER_AGENTSERVER_TIMECHECK_TICKSPEEDCHECKER_H_
