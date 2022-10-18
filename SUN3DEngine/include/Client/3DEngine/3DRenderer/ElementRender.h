//------------------------------------------------------------------------------
//! created : 2010/04/19
//! author : by i4u4me
//! remark : 2D ��������Ʈ�� �ؽ��� ������ ���Ƽ� ������
//! ����� : 2d ���帵�� �ϱ� ���� �⺻�������Ǿ� �־�� ��
//!             Render()���� �������� ��������Ʈ�� ������ �ְ�
//!             Flush() �Լ����� ������ �׸�
//! �� : 
//        CDrawBase::Set2DMode(TRUE);
//        CDrawBase::SetZBufferWrite( FALSE );
//        CDrawBase::SetZBufferTest( FALSE );	
//        CDrawBase::SetAlphaBlendMode(AB_ALPHA);	
//          CElementRender::Render()
//          CElementRender::Flush()
//        //!����Ǿ��� renderstate restore
//        CDrawBase::EndRender()
//------------------------------------------------------------------------------

#pragma once

#include "typedef.h"
#include <client/3dengine/common/wzuvvert.h>
#include <client/3dengine/common/wzcolor.h>
#include <programcommon/wzsingleton.h>

class CDrawBase;

//------------------------------------------------------------------------------
class CElementRender
{
public:
	CElementRender(void);
    ~CElementRender(void);

    void Cleanup();
    void Init(CDrawBase* renderer, int vertex_count);
    bool is_init() { return (vertices_count_ > 0); }
    virtual void Render(HANDLE texture_handle, const St_FRect& texture_rect, const St_FRect& target_rect, WzColor& color);
    virtual void Flush();

    int     vertices_count() const { return vertices_count_; }
    void    set_vertices_count(int& val) { vertices_count_ = val; }
    int     current_write_no() const { return current_write_no_; }
    void    current_write_no_up() { ++current_write_no_; }
    void    set_current_write_no(int val) { current_write_no_ = val; }
    HANDLE  current_texture_handle() const { return current_texture_handle_; }
    void    set_current_texture_handle(HANDLE val) { current_texture_handle_ = val; }

private:
    CDrawBase* renderer_;
    HANDLE  current_texture_handle_;
    int     current_write_no_;

    int     vertices_count_;
    BYTE*   vertices_buffer_; //! (sizeof(WzVector) + sizeof(WzUVVert) + sizeof(WzColor)) * vertices_count_

};