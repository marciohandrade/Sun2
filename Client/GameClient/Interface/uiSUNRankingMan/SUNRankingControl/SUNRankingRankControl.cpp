#include "SunClientPrecompiledHeader.h"
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM
#include "SUNRankingRankControl.h"

const int          MAX_NUMBER_COORDINATE	=72;

HANDLE SUNRankingRankControl::number_texture_handle_ = INVALID_HANDLE_VALUE;

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

SUNRankingRankControl::SUNRankingRankControl(void)
{
}

SUNRankingRankControl::~SUNRankingRankControl(void)
{
	Release();
}

void SUNRankingRankControl::Release()
{
	parent_slot_ = NULL;

	if ( number_texture_handle_ != INVALID_HANDLE_VALUE) 
	{
		if(g_pSunRenderer && g_pSunRenderer->x_pManagerTexture)
		{
			g_pSunRenderer->x_pManagerTexture->DestroyTexture( number_texture_handle_ );
			number_texture_handle_ = INVALID_HANDLE_VALUE;
		}
	}
}

void SUNRankingRankControl::Init( SUNRankingSlot* parent_slot, CControlWZ* total_control_ptr )
{
	parent_slot_ = parent_slot;
	total_control_ptr_ = total_control_ptr;

	if ( number_texture_handle_ == INVALID_HANDLE_VALUE )
	{
		number_texture_handle_ = g_pSunRenderer->x_pManagerTexture->LoadTexture("Data\\Interface\\etc\\number.tga",TLS_ORIGINAL_SIZE | TLS_NO_MULTITHREAD);
	}
}

void SUNRankingRankControl::Update()
{

}

void SUNRankingRankControl::Render()
{
	if ( is_show_() == false )
		return;

	if ( parent_slot_->get_rank_() == 0 )
	{
		return;
	}

	RECT parent_rect = parent_slot_->get_control_rect_();
	RECT origin_parent_rect = parent_rect;

	static int left = 14;
	static int top = 13;
	static int right = 34;
	static int bottom = 25;

	parent_rect.left += left;
	parent_rect.top += top;
	parent_rect.right = right;
	parent_rect.bottom -= bottom;

	RECT total_rect = total_control_ptr_->GetSizeRect();
	RECT result_rect = parent_rect;
	bool is_bottom = false;

	if (parent_slot_->get_index_() != 0 && parent_rect.top + parent_rect.bottom > total_rect.bottom)
	{
		is_bottom = true;
		LONG differ = parent_rect.top + parent_rect.bottom - total_rect.bottom;
		differ = parent_rect.bottom - differ;
		result_rect.bottom = differ;
	}

	WzColor my_color[4];
	my_color[0] = WzColor_RGBA(255, 0, 0, 255);
	my_color[1] = WzColor_RGBA(255, 0, 0, 255);
	my_color[2] = WzColor_RGBA(255, 0, 0, 255);
	my_color[3] = WzColor_RGBA(255, 0, 0, 255);
	WzColor color[4];
	color[0] = WzColor_RGBA(255, 255, 255, 255);
	color[1] = WzColor_RGBA(255, 255, 255, 255);
	color[2] = WzColor_RGBA(255, 255, 255, 255);
	color[3] = WzColor_RGBA(255, 255, 255, 255);

	BYTE digitarray[30];
	int number_count = 0;
	number_count = MakeDigitArray(parent_slot_->get_rank_(), digitarray);
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

		if ( is_bottom == true )
		{
			float percent_result = static_cast<float>(parent_rect.bottom - result_rect.bottom);
			percent_result = percent_result / (float)(parent_rect.bottom);
			float top_gap = 0.0f;
			float bottom_gap = 0.0f;
			top_gap = NumberCoordinate[digitarray[i] * 4 +1] * percent_result;
			bottom_gap = (NumberCoordinate[digitarray[i] * 4 +3] - NumberCoordinate[digitarray[i] * 4 +1]) * percent_result;
			if (number_count == 1)
			{
				g_pSunRenderer->RenderTexture(number_texture_handle_,
					(float)result_rect.left + (result_rect.right / 2) + 4 - (result_rect.right / 4) - 2,
					(float)result_rect.top,
					(float)result_rect.right / 2 - 4,
					(float)result_rect.bottom,
					NumberCoordinate[digitarray[i] * 4],
					NumberCoordinate[digitarray[i] * 4 +1],
					NumberCoordinate[digitarray[i] * 4 +2] - NumberCoordinate[digitarray[i] * 4],
					NumberCoordinate[digitarray[i] * 4 +3] - NumberCoordinate[digitarray[i] * 4 +1] - bottom_gap,
					parent_slot_->get_index_() == 0 ? my_color : color
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
					NumberCoordinate[digitarray[i] * 4 +3] - NumberCoordinate[digitarray[i] * 4 +1] - bottom_gap,
					parent_slot_->get_index_() == 0 ? my_color : color
					);
			}
		}
		else
		{
			if (number_count == 1)
			{
				g_pSunRenderer->RenderTexture(number_texture_handle_,
					(float)parent_rect.left + (parent_rect.right / 2) + 4 - (parent_rect.right / 4) - 2,
					(float)parent_rect.top,
					(float)parent_rect.right / 2 - 4,
					(float)parent_rect.bottom,
					NumberCoordinate[digitarray[i] * 4],
					NumberCoordinate[digitarray[i] * 4 +1],
					NumberCoordinate[digitarray[i] * 4 +2] - NumberCoordinate[digitarray[i] * 4],
					NumberCoordinate[digitarray[i] * 4 +3] - NumberCoordinate[digitarray[i] * 4 +1],
					parent_slot_->get_index_() == 0 ? my_color : color
					);
			}
			else
			{
				g_pSunRenderer->RenderTexture(number_texture_handle_,
					(float)parent_rect.left + ((parent_rect.right / number_count) * i),
					(float)parent_rect.top,
					(float)parent_rect.right / number_count,
					(float)parent_rect.bottom,
					NumberCoordinate[digitarray[i] * 4],
					NumberCoordinate[digitarray[i] * 4 +1],
					NumberCoordinate[digitarray[i] * 4 +2] - NumberCoordinate[digitarray[i] * 4],
					NumberCoordinate[digitarray[i] * 4 +3] - NumberCoordinate[digitarray[i] * 4 +1],
					parent_slot_->get_index_() == 0 ? my_color : color
					);
			}
		}		
	}
}

int SUNRankingRankControl::MakeDigitArray(int value, BYTE* digitarray )
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

#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM
