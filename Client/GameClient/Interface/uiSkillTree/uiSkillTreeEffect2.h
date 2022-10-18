#pragma once
#ifdef _NA_20111213_SKILLTREE_VISUAL_RENEWAL2
#ifndef _NA_0_20120321_SKILLTREE_SECOND_RENEWAL


#include "DrawTexture.h"



class uiSkillTreeEffect
{
private:
    nDrawTexture::DrawTextureManager m_DrawTextureManager;
    bool is_playing_animateion_;
    int started_animation_time_;   // 현재 상태가 시작된 시간


public:
    void SetPlayAnimation(bool value);
    bool IsPlayingAnimation() { return is_playing_animateion_; }


private:
    void RenderBlocks( RECT dest_rect, int delta_time, int max_teim );
    void RenderChain( RECT dest_rect, int delta_time, int max_teim );
    void RenderTornado( RECT dest_rect, int delta_time, int max_teim );


private:
    HANDLE arrow_texture_;

    int start_tab_;

    int cur_tab_;				// 현재의 단계
    int cur_stat_;				// 단계별 획득 포인트

    RECT dest_rect_;			// 화면상의 좌표

    int sound_handle_;          // 재생중인 효과음

private:
    HANDLE GetArrowTexture() { return arrow_texture_; }

    void OnStartEffectAnimation();
    void OnStopEffectAnimation();

public:
	uiSkillTreeEffect();
    ~uiSkillTreeEffect();

	void Init();
	void Release();

    void RenderOpenEffect( const RECT& dest_rect );
    void RenderMaxLevel( const RECT& dest_rect );

	void SetTotalSkillLevel( int total_level, bool data_clear );


private:
    //////////////////////////////////////////////////////////////////////////////////////////////////
    // 화살표
    class SkillTreeArrowInfo
    {
    private:
        enum eArrowState
        {
            ARROW_STATE_HIDE,
            ARROW_STATE_ANIMATION,
            ARROW_STATE_VISIBLE,
        };

    public:
        SkillTreeArrowInfo()
        {
            arrow_state_ = ARROW_STATE_HIDE;

            arrow_position_ = 0;
            arrow_type_ = 0;

            arrow_skill_level_ = 0;
            arrow_skill_class_code_ = 0;

            animation_start_time_ = 0;
        }

        void SetArrowSkillInfo( int skill_class_code, int skill_level ) { arrow_skill_class_code_ = skill_class_code; arrow_skill_level_ = skill_level; }
        void SetArrowPosition( int pos ) { arrow_position_ = pos; } 
        void SetArrowType( int type ) { arrow_type_ = type; }

        int GetArrowPosition() { return arrow_position_; }
        int GetArrowType() { return arrow_type_; }
        int GetSkillClassCode() { return arrow_skill_class_code_; }
        int GetSkillLeve() { return arrow_skill_level_; }

        void SetVisibleState() { arrow_state_ = ARROW_STATE_VISIBLE; }
        void SetHideState() { arrow_state_ = ARROW_STATE_HIDE; }
        bool IsHideState() { return arrow_state_ == ARROW_STATE_HIDE; }

        void StartAnimation()
        {
            if( arrow_state_ != ARROW_STATE_HIDE )
                return;

            arrow_state_ = ARROW_STATE_ANIMATION;
            animation_start_time_ = GetCurrentTime();
        }

        float GetImageRate()
        {
            if( arrow_state_ == ARROW_STATE_HIDE )
                return 0.f;

            if( arrow_state_ == ARROW_STATE_VISIBLE )
                return 0.7f;

            static DWORD animation_duration = 3000; // 3초

            DWORD current_time = GetCurrentTime();
            if( current_time - animation_start_time_ < animation_duration )
            {
                DWORD delta_time = current_time - animation_start_time_;
                float time = (float)delta_time * WZ_PI / 500.f;
                return cos( time ) * 0.5f + 0.5f;
            }
            else
            {
                arrow_state_ = ARROW_STATE_VISIBLE;
                return 0.7f;
            }

            return 0.f;
        }

    private:
        eArrowState arrow_state_;       // 화살표의 상태 (비표시, 블랜딩, 표시)
        int arrow_position_;            // 화살표의 위치
        int arrow_type_;                // 어떤 모양의 화살표 인가

        int arrow_skill_level_;         // 스킬의 레벨
        int arrow_skill_class_code_;    // 스킬의 클래스코드

        DWORD animation_start_time_;    // 애니메이션 시작 시간
    };

private:
    std::vector<SkillTreeArrowInfo> arrow_list_;

    void InitArrowResource();
    void RenderArrows();

public:
    void AddArrowInfo( int arrow_position, int arrow_type, int skill_class_code, int skill_level );
    void SetAnimationArrow( int skill_class_code, int skill_level );
    void SetVisibleArrow( int skill_class_code, int skill_level );
    void ResetArrow();



private:
    //////////////////////////////////////////////////////////////////////////////////////////////////
    // 잠김표시
    class SkillTreeLockInfo
    {
    private:
        enum eLockState
        {
            LOCK_STATE_VISIBLE,
            LOCK_STATE_ANIMATION,
            LOCK_STATE_HIDE,
        };

    public:
        SkillTreeLockInfo()
        {
            state_ = LOCK_STATE_VISIBLE;
            position_ = 0;
            skill_class_ = 0;
            fateout_begin_time_ = 0;
        }

        void SetPosition( int position ) { position_ = position; }
        void SetSkillClass( int skill_class ) { skill_class_ = skill_class; }


        int GetPosition() const { return position_; }
        int GetSkillClass() const { return skill_class_; }

        void BeginFadeout() 
        {
            if( state_ == LOCK_STATE_VISIBLE )
            {
                state_ = LOCK_STATE_ANIMATION;
                fateout_begin_time_ = GetCurrentTime();
            }
        }

        void SetVisibleState() { state_ = LOCK_STATE_VISIBLE; }
        void SetHideState() { state_ = LOCK_STATE_HIDE; }

        float GetImageRate() 
        {
            if( state_ == LOCK_STATE_VISIBLE )
            {
                return 1.f;
            }
            else if( state_ == LOCK_STATE_HIDE )
            {
                return 0.f;
            }

            static DWORD fateout_duration = 1000;  // 1초에 사라락 사라진다

            //if( fateout_begin_time_ == 0 )
            //{
            //    // 잠금 표시
            //    return 1.0f;
            //}


            DWORD current_time = GetCurrentTime();

            DWORD delta_time = current_time - fateout_begin_time_;
            if( delta_time >= fateout_duration )
            {
                // 페이드 아웃 종료
                state_ = LOCK_STATE_HIDE;
                return 0.0f;
            }

            return (float)(fateout_duration - delta_time) / (float)fateout_duration;
        }

    private:
        eLockState state_;
        int position_;                  // 위치
        int skill_class_;               // 스킬 식별을 위한 클래스
        DWORD fateout_begin_time_;
    };

private:
    std::vector<SkillTreeLockInfo> lock_info_array_;

public:
    void RenderLockImage();

    void AddLockInfo( int position, int skill_class )
    {
        SkillTreeLockInfo info;
        info.SetPosition( position );
        info.SetSkillClass( skill_class );
        lock_info_array_.push_back( info );
    }

    void BegionFadeout( int required_skill_class )
    {
        for( size_t i=0;i!=lock_info_array_.size();++i)
        {
            if( lock_info_array_.at(i).GetSkillClass() == required_skill_class )
            {
                lock_info_array_[i].BeginFadeout();
            }
        }
    }

    void ResetLock();

    void HideLock( int skill_class )
    {
        for( size_t i=0;i<lock_info_array_.size();++i)
        {
            if( lock_info_array_[i].GetSkillClass() == skill_class )
            {
                lock_info_array_[i].SetHideState();
            }
        }
    }
};

#endif
#endif //_NA_20111213_SKILLTREE_VISUAL_RENEWAL2
