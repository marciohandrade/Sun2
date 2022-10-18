//----------------------------------------------------------------------------
/**
@desc : 상대적인 윈도우 위치 저장해놓은 스크립트를 관리
@author : 유재영 (y2jinc@webzen.co.kr)
@remark :
*/

#pragma once

#include <SolarHashTable.h>
#include <Singleton.h>
#include <SolarFileReader.h>

#define WZID_LENGTH 5


enum HORIZONTAL_ARRANGE
{
	HORIZONTAL_LEFT = 1,
	HORIZONTAL_RIGHT,
	HORIZONTAL_MIDDLE,
};

enum VERTICAL_ARRANGE
{
	VERTICAL_TOP = 1,
	VERTICAL_BOTTOM,
	VERTICAL_MIDDLE,
};

#define MAX_DLGFILENAME_LENGTH	0xff

struct BASE_DlgPosInfo 
{
     char     m_szDlgWzID[WZID_LENGTH];
     char     m_szdlgFileName[MAX_DLGFILENAME_LENGTH];
     BYTE     m_by_X_ref_Type;
     int      m_i_offset_X;
	 int      m_i_offset_Wide_X;
     BYTE     m_by_Y_ref_Type;
     int	  m_i_offset_Y;
	 int	  m_i_offset_Wide_Y;

     //! 2010.12.21 / i4u4me / 애니메이션 기능 (시작 위치들)
     //! _YMS_DIALOG_ANIMATION_ 
     bool  is_ani_;
     DWORD ani_until_millisecond_;
     int   ani_start_x_; 
     int   ani_start_y_;
     int   ani_wide_start_x_;
     int   ani_wide_start_y_;
     float ani_start_alpha_;
     float ani_speed_ratio_;

     int    m_i_offset_Half_Wide_X;
     int    m_i_offset_Half_Wide_Y;
     int    ani_half_wide_start_x_;
     int    ani_half_wide_start_y_;

     bool  is_ani() const { return is_ani_; }
     float ani_start_alpha() const { return ani_start_alpha_; }
     float ani_speed_ratio() const { return ani_speed_ratio_; }
     DWORD ani_until_millisecond() const { return ani_until_millisecond_; }
};


class DlgPosInfoParser	: public Singleton<DlgPosInfoParser>, public IParser
{
public:
   DlgPosInfoParser();
   ~DlgPosInfoParser();


   VOID			        Init(DWORD dwPoolSize);
   VOID			        Init(DWORD dwPoolSize, char* pszPackFileName);
   VOID                 Release();

   virtual	BOOL        LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);

   BOOL			        GetDlgPoint(WzID wzID, POINT* OUT pDlgPoint);
   BASE_DlgPosInfo*     GetDlgPosInfo(WzID wzKey);

   bool			        GetAniStartPos(WzID wzID, OUT POINT* out_pos);

protected:
   int			        getPosX(WzID wzKey);
   int			        getPosY(WzID wzKey);

   int			        GetAniStartPosX(BASE_DlgPosInfo* info, RECT& window_rect);
   int			        GetAniStartPosY(BASE_DlgPosInfo* info, RECT& window_rec);


private:
	VOID		        Unload();
	util::SolarHashTable<BASE_DlgPosInfo*>*     m_pDataTable;
};
