//-------------------------------------------------------------------------------------------
/** 
	FTextParam	
	@Author : y2jinc ( y2inc@webzen.co.kr )
	@Desc  : 
	@Since : 2006. 3. 13.
	
	@Function
	@TODO
*/

#pragma once

#include "FTextParser.h"


class FTextParam
{
public:
	FTextParam();	 
	FTextParam( const FTextParam & Param);
	~FTextParam();

	void	Clear() 
	{
		this->eType		= FORMAT_TEXT_PARAM_TEXT;
		this->dwValue	= 0;
		this->dwColor	= RGBA(255,255,255,255);
		this->wzFont	= StrToWzID("mn12"); 
		this->m_StrContents.clear();
		this->eShapeType   = ETS_BASE;
		this->dwShapeColor = 0xffffffff;
        this->align_text_style = TP_HLEFT;
	}

	BOOL	IsEmpty() const;

	FTextParam & operator= (const FTextParam & Param);
	FTextParam & operator+ (const FTextParam & Param);
       
public:
	eFORMAT_TEXT_PARAM	eType; // (eFORMAT_TEXT_PARAM)
    // param values
    DWORD				dwValue;
    SUN_STRING			m_StrContents;

    union
    {
        struct  
        {
            // Value of Format Text Type(eFORMAT_TEXT_TYPE)
	        DWORD				dwColor;	// rgba
	        WzID				wzFont;
	        ENUM_TEXT_SHAPE		eShapeType;
	        DWORD				dwShapeColor;
            ENUM_DRAWTEXTSTYLE	align_text_style;
        };

        struct
        {
            DWORD texture_x;
            DWORD texture_y;
            DWORD texture_width;
            DWORD texture_height;
            DWORD draw_width;
            DWORD draw_height;
        };
    };
};

BOOL operator== (const FTextParam & rLParam, const FTextParam & rRParam);