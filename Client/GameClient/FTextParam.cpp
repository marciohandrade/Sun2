#include "SunClientPrecompiledHeader.h"
#include "ftextparam.h"

//-------------------------------------------------------------------------------------------
/**
*/
FTextParam::FTextParam()
{
	Clear();
}

//-------------------------------------------------------------------------------------------
/**
*/
FTextParam::FTextParam( const FTextParam & Param)
{
	this->eType	  = Param.eType;
    if (eType == FORMAT_TEXT_PARAM_ICON)
    {
        texture_x = Param.texture_x;
        texture_y = Param.texture_y;
        texture_width = Param.texture_width;
        texture_height = Param.texture_height;
        draw_width = Param.draw_width;
        draw_height = Param.draw_height;
    }
    else
    {
        this->dwColor = Param.dwColor;
        this->wzFont  = Param.wzFont;
        this->eShapeType   = Param.eShapeType;
        this->dwShapeColor = Param.dwShapeColor;
        this->align_text_style = Param.align_text_style;
    }
    this->dwValue = Param.dwValue;
    this->m_StrContents	= Param.m_StrContents;
}

//-------------------------------------------------------------------------------------------
/**
*/
FTextParam::~FTextParam()
{
	Clear();
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL	
FTextParam::IsEmpty() const
{
	return !BOOL(m_StrContents.size());
}


//-------------------------------------------------------------------------------------------
/**
*/
FTextParam & FTextParam::operator= (const FTextParam & Param)
{
	this->eType	  = Param.eType;
    if (eType == FORMAT_TEXT_PARAM_ICON)
    {
        texture_x = Param.texture_x;
        texture_y = Param.texture_y;
        texture_width = Param.texture_width;
        texture_height = Param.texture_height;
        draw_width = Param.draw_width;
        draw_height = Param.draw_height;
    }
	else
    {
        this->dwColor = Param.dwColor;
        this->wzFont  = Param.wzFont;
        this->eShapeType   = Param.eShapeType;
        this->dwShapeColor = Param.dwShapeColor;
        this->align_text_style = Param.align_text_style;
    }

    this->dwValue = Param.dwValue;
    this->m_StrContents	= Param.m_StrContents;

	return *this;
}

//-------------------------------------------------------------------------------------------
/**
*/
FTextParam & FTextParam::operator+ (const FTextParam & Param)
{
	this->eType	  = Param.eType;
    if ((this->eType == FORMAT_TEXT_PARAM_ICON) || 
        (Param.eType == FORMAT_TEXT_PARAM_ICON))
    {
        // no operation
    }
    else
    {
        this->dwColor = Param.dwColor;
        this->wzFont  = Param.wzFont;
        this->m_StrContents	+= Param.m_StrContents;	
        this->eShapeType   = Param.eShapeType;
        this->dwShapeColor = Param.dwShapeColor;
        this->align_text_style = Param.align_text_style;
    }	

	return *this;
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL		 
operator==(const FTextParam & rLParam, const FTextParam & rRParam)
{
    if ((rLParam.eType == FORMAT_TEXT_PARAM_ICON) || 
        (rRParam.eType == FORMAT_TEXT_PARAM_ICON))
    {
        return FALSE;
    }

	return BOOL(rLParam.eType == rRParam.eType && 
		        rLParam.dwColor == rRParam.dwColor && 
		        rLParam.wzFont  == rRParam.wzFont &&
		        rLParam.eShapeType == rRParam.eShapeType &&
		        rLParam.dwShapeColor == rRParam.dwShapeColor &&
                rLParam.align_text_style == rRParam.align_text_style);
}