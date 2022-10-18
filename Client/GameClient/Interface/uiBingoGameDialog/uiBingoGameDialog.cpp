#include "SunClientPrecompiledHeader.h"
#ifdef _NA_008405_20150716_MINIGAME_BINGO
#include "uiBingoGameDialog.h"
#include "GlobalFunc.h"
#include "GameFramework.h"
#include "uiMenuMan/uiMenuMan.h"
#include "MainDialog2.h"
#include "TextureListInfoParser.h"
#include "TextureListInfoHandleManager.h"

const BYTE         FREE_NUMBER = 255;

const int          MAX_NUMBER_COORDINATE	=72;

//------------------------------------------------------------------------------
/**
*/
static const float NumberCoordinate[MAX_NUMBER_COORDINATE] = 
{
#ifdef _CHINA
	//left			   //top			     //right			    //bottom
	0.0f,      0.0f,     18.0f,   22.0f,	// 0
		19.0f,      0.0f,     31.0f,   22.0f,	// 1
		32.0f,      0.0f,     48.0f,   22.0f,	// 2
		49.0f,      0.0f,     65.0f,   22.0f,	// 3
		66.0f,      0.0f,     84.0f,   22.0f,	// 4
		85.0f,      0.0f,    101.0f,  22.0f,	// 5
		102.0f,     0.0f,    118.0f,  22.0f,	// 6
		119.0f,     0.0f,    134.0f,  22.0f,	// 7
		135.0f,     0.0f,    152.0f,  22.0f,	// 8
		153.0f,     0.0f,    169.0f,  22.0f,	// 9
		170.0f,     0.0f,    179.0f,  22.0f,	// !
		180.0f,     0.0f,    249.0f,  22.0f,	// XP
		0.0f,      23.0f,    91.0f,  45.0f,	// Critical
		105.0f,    23.0f,    153.0f, 45.0f,	// Block
		0.0f,      48.0f,    45.0f,  69.0f,	// Miss
		57.0f,     48.0f,    103.0f, 69.0f,  // Reload
		0.0f,      74.0f,    45.0f,  95.0f,	// Double
		57.0f,     74.0f,    103.0f, 95.0f,	// Double
#else
	//left			   //top			     //right			    //bottom
	0.0f,      0.0f,     18.0f,   22.0f,	// 0
		19.0f,      0.0f,     31.0f,   22.0f,	// 1
		32.0f,      0.0f,     48.0f,   22.0f,	// 2
		49.0f,      0.0f,     65.0f,   22.0f,	// 3
		66.0f,      0.0f,     84.0f,   22.0f,	// 4
		85.0f,      0.0f,    101.0f,  22.0f,	// 5
		102.0f,     0.0f,    118.0f,  22.0f,	// 6
		119.0f,     0.0f,    134.0f,  22.0f,	// 7
		135.0f,     0.0f,    152.0f,  22.0f,	// 8
		153.0f,     0.0f,    169.0f,  22.0f,	// 9
		170.0f,     0.0f,    179.0f,  22.0f,	// !
		180.0f,     0.0f,    221.0f,  27.0f,	// XP 	
		0.0f,      28.0f,    112.0f,  48.0f,	// Critical
		114.0f,    28.0f,    195.0f,  48.0f,	// Block
		0.0f,      51.0f,    64.0f,   73.0f,	// Miss
		65.0f,     51.0f,    161.0f,  74.0f,	// Reload	
		0.0f,      76.0f,    89.0f,   95.0f,	// Double	
		93.0f,     75.0f,    137.0f,  95.0f,	// Double	
#endif // _CHINA
};

//------------------------------------------------------------------------------
/**
*/
WzID uiBingoGameDialog::wz_id_[kControl_Size] = 
{
	StrToWzID("P003"), 
	StrToWzID("P004"), 
	StrToWzID("P007"), 
	StrToWzID("P010"), 
	StrToWzID("P011"), 
	StrToWzID("P016"), 
	StrToWzID("P015"), 
	StrToWzID("P014"), 
	StrToWzID("P013"), 
	StrToWzID("P012"), 
	StrToWzID("P021"), 
	StrToWzID("P020"), 
	StrToWzID("P019"), 
	StrToWzID("P018"), 
	StrToWzID("P017"), 
	StrToWzID("P026"), 
	StrToWzID("P025"), 
	StrToWzID("P024"), 
	StrToWzID("P023"), 
	StrToWzID("P022"), 
	StrToWzID("P031"), 
	StrToWzID("P030"), 
	StrToWzID("P029"), 
	StrToWzID("P028"), 
	StrToWzID("P027"), 

	StrToWzID("S000"), 

	StrToWzID("BT01"), 

	StrToWzID("C000"), 

	StrToWzID("B999"), 
};

//------------------------------------------------------------------------------
/**
*/
uiBingoGameDialog::uiBingoGameDialog(InterfaceManager* ui_manager) : 
mini_game_manager_(NULL), 
number_texture_handle_(INVALID_HANDLE_VALUE), 
is_enable_(false) 
{
}

//------------------------------------------------------------------------------
/**
*/
uiBingoGameDialog::~uiBingoGameDialog(void)
{
}

//------------------------------------------------------------------------------
/**
*/
void uiBingoGameDialog::Release()
{
	if ( number_texture_handle_ != INVALID_HANDLE_VALUE) 
	{
		if(g_pSunRenderer && g_pSunRenderer->x_pManagerTexture)
		{
			g_pSunRenderer->x_pManagerTexture->DestroyTexture( number_texture_handle_ );
			number_texture_handle_ = INVALID_HANDLE_VALUE;
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiBingoGameDialog::Init( CDrawBase* draw_base )
{
	uiBase::Init(draw_base);

	RegisterControlPosition(StrToWzID("P003"), kPicture_0);
	RegisterControlPosition(StrToWzID("P004"), kPicture_1);
	RegisterControlPosition(StrToWzID("P007"), kPicture_2);
	RegisterControlPosition(StrToWzID("P010"), kPicture_3);
	RegisterControlPosition(StrToWzID("P011"), kPicture_4);
	RegisterControlPosition(StrToWzID("P016"), kPicture_5);
	RegisterControlPosition(StrToWzID("P015"), kPicture_6);
	RegisterControlPosition(StrToWzID("P014"), kPicture_7);
	RegisterControlPosition(StrToWzID("P013"), kPicture_8);
	RegisterControlPosition(StrToWzID("P012"), kPicture_9);
	RegisterControlPosition(StrToWzID("P021"), kPicture_10);
	RegisterControlPosition(StrToWzID("P020"), kPicture_11);
	RegisterControlPosition(StrToWzID("P019"), kPicture_12);
	RegisterControlPosition(StrToWzID("P018"), kPicture_13);
	RegisterControlPosition(StrToWzID("P017"), kPicture_14);
	RegisterControlPosition(StrToWzID("P026"), kPicture_15);
	RegisterControlPosition(StrToWzID("P025"), kPicture_16);
	RegisterControlPosition(StrToWzID("P024"), kPicture_17);
	RegisterControlPosition(StrToWzID("P023"), kPicture_18);
	RegisterControlPosition(StrToWzID("P022"), kPicture_19);
	RegisterControlPosition(StrToWzID("P031"), kPicture_20);
	RegisterControlPosition(StrToWzID("P030"), kPicture_21);
	RegisterControlPosition(StrToWzID("P029"), kPicture_22);
	RegisterControlPosition(StrToWzID("P028"), kPicture_23);
	RegisterControlPosition(StrToWzID("P027"), kPicture_24);
	RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000);
	RegisterControlPosition(StrToWzID("BT01"), kControl_Button_BT01);
	RegisterControlPosition(StrToWzID("C000"), kControl_CheckButton_C000);
	RegisterControlPosition(StrToWzID("B999"), kControl_Button_B999);

	mini_game_manager_ = GET_DIALOG_MANAGER(uiMiniGameMan, UIMAN_MINIGAME);

	for ( int i = 6575; i <= 6604; ++i )
	{
		TextureListInfoHandleManager::Instance()->LoadIcon(i);
	}

	if ( number_texture_handle_ == INVALID_HANDLE_VALUE )
	{
		number_texture_handle_ = g_pSunRenderer->x_pManagerTexture->LoadTexture("Data\\Interface\\etc\\number.tga",TLS_ORIGINAL_SIZE | TLS_NO_MULTITHREAD);
	}

	review_check_button_ = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ_byPosition(kControl_CheckButton_C000));
	assert(review_check_button_);

	if ( review_check_button_ )
	{
		review_check_button_->SetCheckState(false);
	}

	ResetBingoCheck();
}

//------------------------------------------------------------------------------
/**
*/
void uiBingoGameDialog::OnShowWindow( BOOL is_show )
{
	if ( is_show )
	{
		keyMsg msg;
		ZeroMemory(&msg, sizeof(msg));
		msg.dwType = eDoType_Bingo_Dialog;
		msg.wParam = IM_MINIGAME_MANAGER::BINGOGAME_DIALOG;
		msg.DoSomething = GlobalFunc::DoShowWindowEx;
		g_KeyQueueManager.PushBack(msg);
	}
	else
	{
		g_KeyQueueManager.DeleteMsg(eDoType_Bingo_Dialog);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiBingoGameDialog::MessageProc( SI_MESSAGE* message )
{
	if ( IsLock() )
		return;

	if ( GetMan() == NULL )
		return;

	switch ( message->eResultMsg )
	{
	case RT_MSG_LBUTTONCLICK:
		{
			OnLButtonClick(message);
		}
		break;

	default:
		SolarDialog::MessageProc(message);
		break;
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiBingoGameDialog::OnUpdateSolarDialog()
{

}

//------------------------------------------------------------------------------
/**
*/
void uiBingoGameDialog::OnRenderSolarDialog()
{
	if ( bingo_map_.empty() == true )
	{
		return;
	}

	for ( POSTYPE i = kPicture_0; i <= kPicture_24; ++i )
	{
		std::map<BINGO_VALUE, BingoData>::iterator iter;

		for ( iter = bingo_map_.begin(); iter != bingo_map_.end(); ++iter )
		{
			if ( (iter->second).pos == i )
			{
				CCtrlPictureWZ* control = static_cast<CCtrlPictureWZ*>(GetControlWZ_byPosition(i));
				if ( control )
				{
					RECT control_rect = control->GetSizeRect();

					if ( (iter->second).check == true )
					{						
						BASE_TextureListInfo* tex_info = NULL;
						// 6575

						if ( (iter->second).value == FREE_NUMBER )
						{
							HANDLE tex_handle = TextureListInfoHandleManager::Instance()->GetTexture(6600, tex_info);
							g_pSunRenderer->RenderTexture(tex_handle, 
								(float)control_rect.left, 
								(float)control_rect.top, 
								(float)control_rect.right - control_rect.left, 
								(float)control_rect.bottom - control_rect.top, 
								(float)tex_info->tex_size.left, 
								(float)tex_info->tex_size.top, 
								(float)tex_info->tex_size.right, 
								(float)tex_info->tex_size.bottom
								);
						}
						else
						{
							HANDLE tex_handle = TextureListInfoHandleManager::Instance()->GetTexture((iter->second).value - 1 + 6575, tex_info);
							g_pSunRenderer->RenderTexture(tex_handle, 
								(float)control_rect.left, 
								(float)control_rect.top, 
								(float)control_rect.right - control_rect.left, 
								(float)control_rect.bottom - control_rect.top, 
								(float)tex_info->tex_size.left, 
								(float)tex_info->tex_size.top, 
								(float)tex_info->tex_size.right, 
								(float)tex_info->tex_size.bottom
								);
						}
					}
					else
					{
						WzColor color[4];
						color[0] = WzColor_RGBA(255, 255, 255, 255);
						color[1] = WzColor_RGBA(255, 255, 255, 255);
						color[2] = WzColor_RGBA(255, 255, 255, 255);
						color[3] = WzColor_RGBA(255, 255, 255, 255);

						BYTE digitarray[30];
						int number_count = 0;
						number_count = MakeDigitArray((iter->second).value, digitarray);
						for (int i = 0; i < number_count; ++i)
						{
							if ( number_count >= 30 )
							{
								break;
							}

							// 유효성 체크
							if ( (digitarray[i] * 4 >= MAX_NUMBER_COORDINATE) || (digitarray[i] * 4 + 1 >= MAX_NUMBER_COORDINATE) || 
								(digitarray[i] * 4 + 2 >= MAX_NUMBER_COORDINATE) || (digitarray[i] * 4 + 3 >= MAX_NUMBER_COORDINATE) )
							{
								continue;
							}

							static int gap = 10;
							RECT result_rect = control_rect;
							result_rect.left = result_rect.left + gap;
							result_rect.top  = result_rect.top + gap;
							result_rect.right = control_rect.right - control_rect.left;
							result_rect.bottom = control_rect.bottom - control_rect.top;
							result_rect.right = result_rect.right - gap;
							result_rect.bottom = result_rect.bottom - gap;

							static int offset = 4;
							result_rect.left -= offset;
							result_rect.top -= offset;

							if ( number_count == 1 )
							{
								g_pSunRenderer->RenderTexture(number_texture_handle_, 
									(float)result_rect.left + (result_rect.right / 2) + 4 - (result_rect.right / 4) - 2,  
									(float)result_rect.top, 
									(float)result_rect.right / 2 - 4, 
									(float)result_rect.bottom, 
									NumberCoordinate[digitarray[i] * 4],
									NumberCoordinate[digitarray[i] * 4 +1],
									NumberCoordinate[digitarray[i] * 4 +2] - NumberCoordinate[digitarray[i] * 4],
									NumberCoordinate[digitarray[i] * 4 +3] - NumberCoordinate[digitarray[i] * 4 +1],
									color
									);
							}
							else
							{
								g_pSunRenderer->RenderTexture(number_texture_handle_, 
									(float)result_rect.left + ((result_rect.right / number_count) * i), 
									(float)result_rect.top, 
									(float)result_rect.right / number_count,  
									(float)result_rect.bottom, 
									NumberCoordinate[digitarray[i] * 4],
									NumberCoordinate[digitarray[i] * 4 +1],
									NumberCoordinate[digitarray[i] * 4 +2] - NumberCoordinate[digitarray[i] * 4],
									NumberCoordinate[digitarray[i] * 4 +3] - NumberCoordinate[digitarray[i] * 4 +1], 
									color 
									);
							}
						}
					}
					break;
				}
			}
		}
	}

	DrawBingoLine();
}

//------------------------------------------------------------------------------
/**
*/
bool uiBingoGameDialog::OnLButtonClick( SI_MESSAGE* message )
{
	DWORD control_position = GetControlPosition_byID( message->dwCtrlID );
	switch ( control_position )
	{
	case kControl_Button_BT01: // 보상 받기
		{
			MSG_CG_CHARINFO_GET_MINIGAME_BINGO_REWARD_PACKET msg;
			GlobalFunc::SendPacket(CI_GAMESERVERIDX, &msg, sizeof(msg));
		}
		break;

	case kControl_Button_B999:
		{
			ShowInterface( FALSE );
		}
		break;
	}

	return true;
}

//------------------------------------------------------------------------------
/**
*/
int uiBingoGameDialog::MakeDigitArray( int value, BYTE* digitarray )
{
	int iDigitcount = 0;
	// 0이어도 한번은 쓰도록해주자
	while (value >= 0)
	{
		BYTE number = (BYTE)(value % 10);
		digitarray[iDigitcount] = number;
		value /= 10;
		iDigitcount++;
		if (value == 0) break;
		assert(iDigitcount < 30);
	}

	// 뒤집자~
	int startindex = 0;
	int backindex = iDigitcount - 1;

	while(startindex < backindex)
	{
		digitarray[startindex] ^= digitarray[backindex];
		digitarray[backindex] ^= digitarray[startindex];
		digitarray[startindex] ^= digitarray[backindex];

		startindex++;
		backindex--;
	}
	return iDigitcount;
}

//------------------------------------------------------------------------------
/**
*/
void uiBingoGameDialog::ResetBingoCheck()
{
	for ( BYTE i = 0; i < 12; ++i )
	{
		bingo_check_[i] = false;
	}

	bingo_map_.clear();

	TCHAR format_string[INTERFACE_STRING_LENGTH] = {0, };
	TCHAR result_string[INTERFACE_STRING_LENGTH] = {0, };
	TCHAR number_string[INTERFACE_STRING_LENGTH] = {0, };

	// 41853 : 빙고 %s줄 완성
	g_InterfaceManager.GetInterfaceString(41853, format_string);

	_itoa(0, number_string, 10);

	Snprintf(result_string, INTERFACE_STRING_LENGTH-1, format_string, number_string);

	CCtrlStaticWZ* control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S000));

	if ( control )
	{
		control->SetTextWZ(result_string);
	}

	CCtrlButtonWZ* button_control = static_cast<CCtrlButtonWZ*>(GetControlWZ_byPosition(kControl_Button_BT01));

	if ( button_control )
	{
		button_control->ShowWindowWZ(WZ_HIDE);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiBingoGameDialog::BingoCheck()
{
	int line = 0;
	for ( BYTE i = 0; i < 25; i += 5 )
	{
		if ( bingo_map_[i].check == true && 
			 bingo_map_[i+1].check == true && 
			 bingo_map_[i+2].check == true && 
			 bingo_map_[i+3].check == true && 
			 bingo_map_[i+4].check == true )
		{
			bingo_check_[line] = true;
		}

		++line;
	}

	for ( BYTE i = 0; i < 5; ++i )
	{
		if ( bingo_map_[i].check == true && 
			 bingo_map_[i + 5].check == true && 
			 bingo_map_[i + 10].check == true && 
			 bingo_map_[i + 15].check == true && 
			 bingo_map_[i + 20].check == true )
		{
			bingo_check_[line] = true;
		}

		++line;
	}

	if ( bingo_map_[0].check == true && 
		 bingo_map_[6].check == true && 
		 bingo_map_[12].check == true && 
		 bingo_map_[18].check == true && 
		 bingo_map_[24].check == true )
	{
		bingo_check_[line] = true;
	}

	++line;

	if ( bingo_map_[4].check == true && 
		 bingo_map_[8].check == true && 
		 bingo_map_[12].check == true && 
		 bingo_map_[16].check == true && 
		 bingo_map_[20].check == true ) 
	{
		bingo_check_[line] = true;
	}

	int total_line = 0;
	for ( BYTE i = 0; i < 12; ++i )
	{
		if ( bingo_check_[i] == true )
		{
			++total_line;
		}
	}

	TCHAR format_string[INTERFACE_STRING_LENGTH] = {0, };
	TCHAR result_string[INTERFACE_STRING_LENGTH] = {0, };
	TCHAR number_string[INTERFACE_STRING_LENGTH] = {0, };

	// 41853 : 빙고 %s줄 완성
	g_InterfaceManager.GetInterfaceString(41853, format_string);

	_itoa(total_line, number_string, 10);

	Snprintf(result_string, INTERFACE_STRING_LENGTH-1, format_string, number_string);

	CCtrlStaticWZ* control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S000));

	if ( control )
	{
		control->SetTextWZ(result_string);
	}

	if ( total_line > 0 )
	{
		CCtrlButtonWZ* button_control = static_cast<CCtrlButtonWZ*>(GetControlWZ_byPosition(kControl_Button_BT01));

		if ( button_control )
		{
			button_control->ShowWindowWZ(WZ_SHOW);
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiBingoGameDialog::DrawBingoLine()
{
	// 가로
	for ( BYTE i = 0; i < 5; ++i )
	{
		if ( bingo_check_[i] == true )
		{
			CCtrlPictureWZ* control = static_cast<CCtrlPictureWZ*>(GetControlWZ_byPosition(i * 5));
			CCtrlPictureWZ* second_control = static_cast<CCtrlPictureWZ*>(GetControlWZ_byPosition(i * 5 + 4));
			if ( control && second_control )
			{
				RECT control_rect = control->GetSizeRect();
				RECT second_control_rect = second_control->GetSizeRect();

				RECT result_rect;
				result_rect.left = control_rect.left;
				result_rect.top  = control_rect.top;
				result_rect.right = second_control_rect.right - control_rect.left;
				result_rect.bottom = second_control_rect.bottom - control_rect.top;

				BASE_TextureListInfo* tex_info = NULL;

				HANDLE tex_handle = TextureListInfoHandleManager::Instance()->GetTexture(6601, tex_info);

				result_rect.top = (control_rect.top + (result_rect.bottom / 2)) - (tex_info->tex_size.bottom / 2);

				g_pSunRenderer->RenderTexture(tex_handle, 
					(float)result_rect.left, 
					(float)result_rect.top, 
					(float)result_rect.right, 
					(float)tex_info->tex_size.bottom, 
					(float)tex_info->tex_size.left, 
					(float)tex_info->tex_size.top, 
					(float)tex_info->tex_size.right, 
					(float)tex_info->tex_size.bottom
					);
			}
		}
	}

	// 세로
	for ( BYTE i = 5; i < 10; ++i )
	{
		if ( bingo_check_[i] == true )
		{
			CCtrlPictureWZ* control = static_cast<CCtrlPictureWZ*>(GetControlWZ_byPosition(i - 5));
			CCtrlPictureWZ* second_control = static_cast<CCtrlPictureWZ*>(GetControlWZ_byPosition(i - 5 + 20));
			if ( control && second_control )
			{
				RECT control_rect = control->GetSizeRect();
				RECT second_control_rect = second_control->GetSizeRect();

				RECT result_rect;

				result_rect.left = control_rect.left;
				result_rect.top  = control_rect.top;
				result_rect.right = control_rect.right - control_rect.left;
				result_rect.bottom = second_control_rect.bottom - control_rect.top;

				BASE_TextureListInfo* tex_info = NULL;

				HANDLE tex_handle = TextureListInfoHandleManager::Instance()->GetTexture(6604, tex_info);

				result_rect.left = control_rect.left + ((control_rect.right - control_rect.left) / 2) - (tex_info->tex_size.right / 2);

				g_pSunRenderer->RenderTexture(tex_handle, 
					(float)result_rect.left, 
					(float)result_rect.top, 
					(float)tex_info->tex_size.right, 
					(float)result_rect.bottom,  
					(float)tex_info->tex_size.left, 
					(float)tex_info->tex_size.top, 
					(float)tex_info->tex_size.right, 
					(float)tex_info->tex_size.bottom
					);
			}
		}
	}

	// 좌측부터 우측 대각선
	if ( bingo_check_[10] == true )
	{
		CCtrlPictureWZ* control = static_cast<CCtrlPictureWZ*>(GetControlWZ_byPosition(0));
		CCtrlPictureWZ* second_control = static_cast<CCtrlPictureWZ*>(GetControlWZ_byPosition(24));
		if ( control && second_control )
		{
			RECT control_rect = control->GetSizeRect();
			RECT second_control_rect = second_control->GetSizeRect();

			RECT result_rect;

			result_rect.left   = control_rect.left;
			result_rect.top    = control_rect.top;
			result_rect.right  = second_control_rect.right - control_rect.left;
			result_rect.bottom = second_control_rect.bottom - control_rect.top;

			BASE_TextureListInfo* tex_info = NULL;

			HANDLE tex_handle = TextureListInfoHandleManager::Instance()->GetTexture(6603, tex_info);

			g_pSunRenderer->RenderTexture(tex_handle, 
				(float)result_rect.left, 
				(float)result_rect.top, 
				(float)result_rect.right, 
				(float)result_rect.bottom,  
				(float)tex_info->tex_size.left, 
				(float)tex_info->tex_size.top, 
				(float)tex_info->tex_size.right, 
				(float)tex_info->tex_size.bottom
				);
		}
	}

	// 우측부터 좌측 대각선
	if ( bingo_check_[11] == true )
	{
		CCtrlPictureWZ* control = static_cast<CCtrlPictureWZ*>(GetControlWZ_byPosition(0));
		CCtrlPictureWZ* second_control = static_cast<CCtrlPictureWZ*>(GetControlWZ_byPosition(24));
		if ( control && second_control )
		{
			RECT control_rect = control->GetSizeRect();
			RECT second_control_rect = second_control->GetSizeRect();

			RECT result_rect;

			result_rect.left   = control_rect.left;
			result_rect.top    = control_rect.top;
			result_rect.right  = second_control_rect.right - control_rect.left;
			result_rect.bottom = second_control_rect.bottom - control_rect.top;

			BASE_TextureListInfo* tex_info = NULL;

			HANDLE tex_handle = TextureListInfoHandleManager::Instance()->GetTexture(6602, tex_info);

			g_pSunRenderer->RenderTexture(tex_handle, 
				(float)result_rect.left, 
				(float)result_rect.top, 
				(float)result_rect.right, 
				(float)result_rect.bottom,  
				(float)tex_info->tex_size.left, 
				(float)tex_info->tex_size.top, 
				(float)tex_info->tex_size.right, 
				(float)tex_info->tex_size.bottom
				);
		}
	}
}

void uiBingoGameDialog::BingoGameInfoSetting( MiniGameBingoInfos& minigame_bingo_infos )
{
	ResetBingoCheck();

	for ( int i = 0; i < MINIGAME_BINGO_LENGTH; ++i )
	{
		BingoData bingo_data;

		bingo_data.pos = i;

		if ( minigame_bingo_infos.m_bingo_infos[i].bingo_state == eMINIGAME_BINGO_STATE_FREE )
		{
			bingo_data.value = FREE_NUMBER;
		}
		else
		{
			bingo_data.value = minigame_bingo_infos.m_bingo_infos[i].bingo_number;
		}

		if ( minigame_bingo_infos.m_bingo_infos[i].bingo_state == eMINIGAME_BINGO_STATE_FILLED || 
			 minigame_bingo_infos.m_bingo_infos[i].bingo_state == eMINIGAME_BINGO_STATE_FREE )
		{
			bingo_data.check = true;
		}

		bingo_map_.insert(make_pair(i, bingo_data));
	}

	BingoCheck();

	if ( IsVisible() == FALSE && review_check_button_->GetCheckState() == false )
	{
		ShowInterface(TRUE);
	}

	MainDialog2* main_dialog = GET_CAST_DIALOG(MainDialog2, IM_MENU_MANAGER::MAIN_DLG_2);
	if (main_dialog)
	{
		main_dialog->MiniGameToDisAbleButton(true);
	}

	set_is_enable(true);
}

#endif // _NA_008405_20150716_MINIGAME_BINGO
