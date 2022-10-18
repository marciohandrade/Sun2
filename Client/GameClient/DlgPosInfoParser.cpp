//------------------------------------------------------------------------------
//	
//	(C) 2005 y2jinc
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "DlgPosInfoParser.h"
#include "Application.h"

//----------------------------------------------------------------------------
/**
*/
DlgPosInfoParser::DlgPosInfoParser():	m_pDataTable ( NULL )
{
}


//----------------------------------------------------------------------------
/**
*/
DlgPosInfoParser::~DlgPosInfoParser()
{
	assert( m_pDataTable == NULL);
}


//----------------------------------------------------------------------------
/**
*/
VOID 
DlgPosInfoParser::Release()
{
	Unload();
	delete m_pDataTable;
	m_pDataTable = NULL;
}


//----------------------------------------------------------------------------
/**
*/
VOID 
DlgPosInfoParser::Init(DWORD dwPoolSize)
{
	assert( m_pDataTable == NULL );
	m_pDataTable = new util::SolarHashTable<BASE_DlgPosInfo*>;
	m_pDataTable->Initialize( dwPoolSize );
}

VOID
DlgPosInfoParser::Init( DWORD dwPoolSize, char * pszPackFileName )
{
	Init( dwPoolSize );
	SetPackFileName( pszPackFileName );
}

//----------------------------------------------------------------------------
/**
*/
VOID 
DlgPosInfoParser::Unload()
{
	BASE_DlgPosInfo * pInfo = NULL;
	if( m_pDataTable == NULL )
	{
		return;
	}
	m_pDataTable->SetFirst();
	while( pInfo = m_pDataTable->GetNext() )
	{
		delete pInfo;
	}
	m_pDataTable->RemoveAll();
}


//----------------------------------------------------------------------------
/**
*/
BASE_DlgPosInfo * 
DlgPosInfoParser::GetDlgPosInfo(WzID wzKey)
{
	return m_pDataTable->GetData( wzKey );
}


//----------------------------------------------------------------------------
/**
*/
BOOL			
DlgPosInfoParser::GetDlgPoint(WzID wzID, POINT * OUT pDlgPoint)
{
	if (!pDlgPoint)
		return FALSE;

	if (!this->GetDlgPosInfo( wzID ))
	{
		return FALSE;
	}

	pDlgPoint->x = this->getPosX(wzID);
	pDlgPoint->y = this->getPosY(wzID);

	return TRUE;
}


//----------------------------------------------------------------------------
/**
*/
int						   
DlgPosInfoParser::getPosX(WzID wzKey)
{
	int x = 0;

	BASE_DlgPosInfo * pInfo = GetDlgPosInfo(wzKey);

	if (!pInfo)
	{
		return x;
	}

	RECT rect;
	rect.left	= rect.top = 0;


	POINT pt = g_pApplication->Get2DModeMapping();

	rect.right = pt.x;
	rect.bottom = pt.y;
	
	switch ((HORIZONTAL_ARRANGE)pInfo->m_by_X_ref_Type)
	{
		// 왼쪽을 기준으로 해서 OffSet 만큼 + 한다.
	case HORIZONTAL_LEFT:
		{
            if (g_pApplication->screen_ratio() == SR_4_3)   return pInfo->m_i_offset_X;
            if (g_pApplication->screen_ratio() == SR_16_9)  return pInfo->m_i_offset_Wide_X;
            if (g_pApplication->screen_ratio() == SR_16_10) return pInfo->m_i_offset_Half_Wide_X;
		}
		break;

		// 중앙을 기준으로 해서 OffSet 만큼 +- 한다.
	case HORIZONTAL_MIDDLE:
		{
            if (g_pApplication->screen_ratio() == SR_4_3)   return (rect.right / 2) + pInfo->m_i_offset_X;
            if (g_pApplication->screen_ratio() == SR_16_9)  return (rect.right / 2) + pInfo->m_i_offset_Wide_X;
            if (g_pApplication->screen_ratio() == SR_16_10) return (rect.right / 2) + pInfo->m_i_offset_Half_Wide_X;
		}
		break;

		// 오른쪽을 기준으로 해서 OffSet 만큼 +- 한다.
	case HORIZONTAL_RIGHT:
		{
            if (g_pApplication->screen_ratio() == SR_4_3)   return rect.right - abs(pInfo->m_i_offset_X);
            if (g_pApplication->screen_ratio() == SR_16_9)  return rect.right - abs(pInfo->m_i_offset_Wide_X);
            if (g_pApplication->screen_ratio() == SR_16_10) return rect.right - abs(pInfo->m_i_offset_Half_Wide_X);
		}
		break;

	default:
		{
			assert(0);
		}
		break;
	}

	return x;
}


//----------------------------------------------------------------------------
/**
*/
int						   
DlgPosInfoParser::getPosY(WzID wzKey)
{
	int y = 0;

	BASE_DlgPosInfo * pInfo = GetDlgPosInfo(wzKey);
	if (!pInfo)
	{
		return y;
	}

	POINT pt = g_pApplication->Get2DModeMapping();

	RECT rect;

	rect.left = rect.right = 0;	
	rect.right = pt.x;
	rect.bottom = pt.y;
	
	switch ((VERTICAL_ARRANGE)pInfo->m_by_Y_ref_Type)
	{
		//상단을 기준으로 해서 떨어진 Offset을 적용.
	case VERTICAL_TOP:
		{
            if (g_pApplication->screen_ratio() == SR_4_3)   return abs(pInfo->m_i_offset_Y);
            if (g_pApplication->screen_ratio() == SR_16_9)  return abs(pInfo->m_i_offset_Wide_Y);
            if (g_pApplication->screen_ratio() == SR_16_10) return abs(pInfo->m_i_offset_Half_Wide_Y);
		}
		break;

		//중단을 기준으로 해서 떨어진 Offset을 +- 적용.
	case VERTICAL_MIDDLE:
		{
            if (g_pApplication->screen_ratio() == SR_4_3)   return (rect.bottom / 2) + pInfo->m_i_offset_Y;
            if (g_pApplication->screen_ratio() == SR_16_9)  return (rect.bottom / 2) + pInfo->m_i_offset_Wide_Y;
            if (g_pApplication->screen_ratio() == SR_16_10) return (rect.bottom / 2) + pInfo->m_i_offset_Half_Wide_Y;
		}
		break;

		//하단을 기준으로 해서 떨어진 Offset을 - 적용.
	case VERTICAL_BOTTOM:
		{
            if (g_pApplication->screen_ratio() == SR_4_3)   return rect.bottom - abs(pInfo->m_i_offset_Y);
            if (g_pApplication->screen_ratio() == SR_16_9)  return rect.bottom - abs(pInfo->m_i_offset_Wide_Y);
            if (g_pApplication->screen_ratio() == SR_16_10) return rect.bottom - abs(pInfo->m_i_offset_Half_Wide_Y);
		}
		break;

	default:
		{
			assert(0);
		}
		break;
	}	
	
	return y;
}

//------------------------------------------------------------------------------ 
bool	DlgPosInfoParser::GetAniStartPos(WzID wzID, OUT POINT* out_pos)
{
    BASE_DlgPosInfo* info = GetDlgPosInfo(wzID);
    if (info == NULL)
    {
        return false;
    }
    if (info->is_ani() == false)
    {
        return false;
    }
    

    POINT pt = g_pApplication->Get2DModeMapping();

    RECT rect;
    rect.left = rect.top = 0;
    rect.right = pt.x;
    rect.bottom = pt.y;

    out_pos->x = GetAniStartPosX(info, rect);
    out_pos->y = GetAniStartPosY(info, rect);

    return true;
}

//------------------------------------------------------------------------------ 
int DlgPosInfoParser::GetAniStartPosX(BASE_DlgPosInfo* info, RECT& window_rect)
{
    if (info == NULL)
    {
        return 0;
    }

    bool is_wide = (g_pApplication->IsWideModeFlag() == TRUE);
    int offset = (is_wide) ? info->ani_wide_start_x_ : info->ani_start_x_;

    if (g_pApplication->screen_ratio() == SR_4_3)   offset = info->ani_start_x_;
    if (g_pApplication->screen_ratio() == SR_16_9)  offset = info->ani_wide_start_x_;
    if (g_pApplication->screen_ratio() == SR_16_10) offset = info->ani_half_wide_start_x_;

    if (info->m_by_X_ref_Type == HORIZONTAL_LEFT)
    {
        return offset;
    }
    else if (info->m_by_X_ref_Type == HORIZONTAL_MIDDLE)
    {
        return (window_rect.right / 2) + offset;
    }
    else if (info->m_by_X_ref_Type == HORIZONTAL_RIGHT)
    {
        return (window_rect.right - abs(offset));
    }
	
	return 0;
}
//------------------------------------------------------------------------------ 
int DlgPosInfoParser::GetAniStartPosY(BASE_DlgPosInfo* info, RECT& window_rect)
{
    if (info == NULL)
    {
        return 0;
    }

    bool is_wide = (g_pApplication->IsWideModeFlag() == TRUE);
    int offset = (is_wide) ? info->ani_wide_start_y_ : info->ani_start_y_;

    if (g_pApplication->screen_ratio() == SR_4_3)   offset = info->ani_start_y_;
    if (g_pApplication->screen_ratio() == SR_16_9)  offset = info->ani_wide_start_y_;
    if (g_pApplication->screen_ratio() == SR_16_10) offset = info->ani_half_wide_start_y_;

    if (info->m_by_Y_ref_Type == VERTICAL_TOP)
    {
        return offset;
    }
    else if (info->m_by_Y_ref_Type == VERTICAL_MIDDLE)
    {
        return (window_rect.bottom / 2) + offset;
    }
    else if (info->m_by_Y_ref_Type == VERTICAL_BOTTOM)
    {
        return (window_rect.bottom - abs(offset));
    }

    return 0;
}


#pragma warning ( push )
#pragma warning ( disable : 4244)

BOOL DlgPosInfoParser::LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload)
{
	WzID wzID;
	int nRowSize = GetRowSize();

	for (int nRow = 0; nRow < nRowSize; ++nRow)
	{
		BASE_DlgPosInfo* pInfo = new BASE_DlgPosInfo;
        ZeroMemory(pInfo, sizeof(BASE_DlgPosInfo));
		
		//w_Dlg_ID
		//strncpy(pInfo->m_szDlgWzID, GetDataString("w_Dlg_ID", nRow), WZID_LENGTH);
        Sprintf(pInfo->m_szDlgWzID, "%04d", GetDataInt("w_Dlg_ID", nRow));

		wzID = StrToWzID(pInfo->m_szDlgWzID);

		//s_Idlg_Name
		strncpy(pInfo->m_szdlgFileName, GetDataString("s_Idlg_Name", nRow), MAX_DLGFILENAME_LENGTH);
		
		//b_X_ref_Type
		pInfo->m_by_X_ref_Type		= GetDataBYTE("b_X_ref_Type",nRow);

		//w_offset_X
		pInfo->m_i_offset_X			= GetDataInt("w_offset_X",nRow);
		
		//Wide_w_offset_X
		pInfo->m_i_offset_Wide_X	= GetDataInt("Wide_w_offset_X",nRow);
		
		//b_Y_ref_Type
		pInfo->m_by_Y_ref_Type		= GetDataBYTE("b_Y_ref_Type",nRow);
		
		//w_Offset_Y
		pInfo->m_i_offset_Y			= GetDataInt("w_Offset_Y",nRow);
		
		//Wide_w_offset_Y
		pInfo->m_i_offset_Wide_Y	= GetDataInt("Wide_w_offset_Y",nRow);

        //! 2010.12.21 / i4u4me / 애니메이션 
        pInfo->is_ani_ = (GetDataInt("Ani", nRow) != 0);
        pInfo->ani_until_millisecond_ = GetDataDWORD("Until_Millisecond", nRow);
        pInfo->ani_start_x_ = GetDataInt("Start_X", nRow);
        pInfo->ani_wide_start_x_ = GetDataInt("Wide_Start_X", nRow);
        pInfo->ani_start_y_ = GetDataInt("Start_Y", nRow);
        pInfo->ani_wide_start_y_ = GetDataInt("Wide_Start_Y", nRow);
        pInfo->ani_start_alpha_ = GetDataFloat("Ani_Start_Alpha", nRow);
        pInfo->ani_speed_ratio_ = GetDataFloat("Ani_Speed_Ratio", nRow);

        pInfo->m_i_offset_Half_Wide_X = GetDataInt("Half_Wide_w_offset_X",nRow);
        pInfo->m_i_offset_Half_Wide_Y = GetDataInt("Half_Wide_w_offset_Y",nRow);
        pInfo->ani_half_wide_start_x_ = GetDataInt("Half_Wide_Start_X",nRow);
        pInfo->ani_half_wide_start_y_ = GetDataInt("Half_Wide_Start_Y",nRow);

		assert(NULL == m_pDataTable->GetData(wzID));
		m_pDataTable->Add(pInfo, wzID);
	}
	return TRUE;
}



#pragma warning ( pop )
