#pragma once

class uiRadarMapData
{
public:
    uiRadarMapData()
    {
        Clear();
    }
    ~uiRadarMapData() {}

    float map_image_width() { return map_image_width_; }
    void set_map_image_width(float image_width) { map_image_width_ = image_width; }
    float map_image_height() { return map_image_height_; }
    void set_map_image_height(float image_height) { map_image_height_ = image_height; }

    WzMatrix& transform_matrix() { return transform_matrix_; }
    void set_transform_matrix(WzMatrix* matrix) { transform_matrix_ = *matrix; }

    void CalculateTransformPosition(WzVector hero_position);
    WzVector transform_position() { return transform_position_; }

    void Clear();

private:
    // 맵 이미지 사이즈
    float map_image_width_;
    float map_image_height_;

    // 좌표 변환 메트릭스
    WzMatrix transform_matrix_;

    WzVector transform_position_;
};

inline void uiRadarMapData::Clear()
{
    map_image_width_ = 960.0f;
    map_image_height_ = 720.0f;

    MatrixIdentity(&transform_matrix_);
}

inline void uiRadarMapData::CalculateTransformPosition(WzVector hero_position)
{
    D3DXVECTOR4 transfirm_position(hero_position.x, hero_position.z, hero_position.y, 1.0f);
    D3DXVec4Transform(&transfirm_position, &transfirm_position, (D3DXMATRIX*)(&transform_matrix()));

    transform_position_.x = (((transfirm_position.x * 0.5f) + 0.5f) * map_image_width());
    transform_position_.y = (((transfirm_position.y * (-0.5f)) + 0.5f) * map_image_height());
}
