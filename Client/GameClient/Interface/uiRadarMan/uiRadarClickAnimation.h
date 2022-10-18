#pragma once

class uiRadarClickAnimation
{
public:
    enum
    {
        kMaxClickAniNum = 11,
        kTimePerFrame = 50,
    };

public:
    uiRadarClickAnimation() {}
    ~uiRadarClickAnimation() {}

    void SetClickPosition(float position_x, float position_y,int display_time);

    float click_position_x() { return click_position_x_; }
    float click_position_y() { return click_position_y_; }
    int display_time() { return display_time_; }
    void set_display_time(DWORD time_value) { display_time_ = time_value; }
    
private:
    // 맵 이미지 사이즈
    float click_position_x_;
    float click_position_y_;
    int display_time_;
};

inline void uiRadarClickAnimation::SetClickPosition(float position_x, float position_y, int display_time)
{
    click_position_x_ = position_x;
    click_position_y_ = position_y;
    display_time_ = display_time;
}
