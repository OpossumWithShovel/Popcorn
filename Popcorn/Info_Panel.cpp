#include "Info_Panel.h"

// AsInfo_Panel
//-----------------------------------------------------------------------------------------------------------
AsInfo_Panel::~AsInfo_Panel()
{
	delete Shadow_Color;
	delete Highlight_Color;

	if (Logo_Pop_Font != 0)
		DeleteObject(Logo_Pop_Font);

	if (Logo_Corn_Font != 0)
		DeleteObject(Logo_Corn_Font);

	if (Player_Name_Font != 0)
		DeleteObject(Player_Name_Font);
}
//-----------------------------------------------------------------------------------------------------------
AsInfo_Panel::AsInfo_Panel()
	: Shadow_Color(0), Highlight_Color(0), Logo_Pop_Font(0), Logo_Corn_Font(0), Player_Name_Font(0),
	Letter_F(EBrick_Type::Blue, ELetter_Type::F, 216, 155),
	Letter_M(EBrick_Type::Blue, ELetter_Type::M, 296, 155),
	Letter_Plus(EBrick_Type::Blue, ELetter_Type::Plus, 256, 155), Player_Name(L"COMPUTER")
{
	Letter_F.Show_Full_Size();
	Letter_M.Show_Full_Size();
	Letter_Plus.Show_Full_Size();
}
//-----------------------------------------------------------------------------------------------------------
void AsInfo_Panel::Begin_Movement()
{
	//  Заглушка. Этот метод не используется
}
//-----------------------------------------------------------------------------------------------------------
void AsInfo_Panel::Finish_Movement()
{
	//  Заглушка. Этот метод не используется
}
//-----------------------------------------------------------------------------------------------------------
void AsInfo_Panel::Advance(double max_speed)
{
	//  Заглушка. Этот метод не используется
}
//-----------------------------------------------------------------------------------------------------------
double AsInfo_Panel::Get_Speed()
{
	return 0.0;  //  Заглушка. Этот метод не используется
}
//-----------------------------------------------------------------------------------------------------------
void AsInfo_Panel::Act()
{
	//  Заглушка. Этот метод не используется
}
//-----------------------------------------------------------------------------------------------------------
void AsInfo_Panel::Clear(HDC hdc, RECT &paint_area)
{
	//!!! Надо сделать
}
//-----------------------------------------------------------------------------------------------------------
void AsInfo_Panel::Draw(HDC hdc, RECT &paint_area)
{
	const int scale = AsConfig::Global_Scale;
	int logo_x_pos = 223 * scale;
	int logo_y_pos = 3 * scale;
	const wchar_t *pop_str = L"POP";
	const wchar_t *corn_str = L"CORN";

	// 1. Логотип
	AsTools::Rect(hdc, 212, 7, 102, 99, AsConfig::Blue_Color);

	SetBkMode(hdc, TRANSPARENT);

	// 1.1 Тень
	SetTextColor(hdc, AsConfig::BG_Color.Get_RGB() );

	SelectObject(hdc, Logo_Pop_Font);
	TextOut(hdc, logo_x_pos + Shadow_X_Offset - 8 * scale, logo_y_pos + Shadow_Y_Offset, pop_str, (int)wcslen(pop_str) );
	SelectObject(hdc, Logo_Corn_Font);
	TextOut(hdc, logo_x_pos - 13 * scale + Shadow_X_Offset, logo_y_pos + 44 * scale + Shadow_Y_Offset, corn_str, (int)wcslen(corn_str) );

	// 1.2 Сами символы
	SetTextColor(hdc, AsConfig::Red_Color.Get_RGB() );

	SelectObject(hdc, Logo_Pop_Font);
	TextOut(hdc, logo_x_pos - 8 * scale, logo_y_pos, pop_str, (int)wcslen(pop_str) );
	SelectObject(hdc, Logo_Corn_Font);
	TextOut(hdc, logo_x_pos - 13 * scale, logo_y_pos + 44 * scale, corn_str, (int)wcslen(corn_str) );

	// 2. Таблица счета
	Draw_Score_Panel(hdc);

	// 3. Индикаторы
	// 3.1 Пол
	Letter_F.Draw(hdc, paint_area);
	AsTools::Round_Rect(hdc, Score_X_Pos + 9, Score_Y_Pos + 56, 13, 30, AsConfig::Floor_Indicator_Color);

	// 3.2 Жизни
	Letter_Plus.Draw(hdc, paint_area);
	AsTools::Round_Rect(hdc, Score_X_Pos + 28, Score_Y_Pos + 56, 55, 30, AsConfig::Floor_Indicator_Color);

	Show_Extra_Lives(hdc);

	// 3.3 Монстр
	Letter_M.Draw(hdc, paint_area);
	AsTools::Round_Rect(hdc, Score_X_Pos + 89, Score_Y_Pos + 56, 13, 30, AsConfig::Floor_Indicator_Color);
}
//-----------------------------------------------------------------------------------------------------------
bool AsInfo_Panel::Is_Finished()
{
	return false;  // Заглушка. Этот метод не используется
}
//-----------------------------------------------------------------------------------------------------------
void AsInfo_Panel::Init()
{
	LOGFONT log_font{}; 

	log_font.lfHeight = -128;
	log_font.lfWeight = 900;
	log_font.lfOutPrecision = 3;
	log_font.lfClipPrecision = 2;
	log_font.lfQuality = 1;
	log_font.lfPitchAndFamily = 34;

	wcscpy_s(log_font.lfFaceName, L"Arial Black");

	Logo_Pop_Font = CreateFontIndirect(&log_font); 

	log_font.lfHeight = -96;
	Logo_Corn_Font = CreateFontIndirect(&log_font);

	log_font.lfHeight = -48;
	log_font.lfWeight = 700;
	log_font.lfOutPrecision = 3;
	log_font.lfClipPrecision = 2;
	log_font.lfQuality = 1;
	log_font.lfPitchAndFamily = 49;

	wcscpy_s(log_font.lfFaceName, L"Consolas");

	log_font.lfHeight = -44;
	Player_Name_Font = CreateFontIndirect(&log_font); 

	Shadow_Color = new AColor(AsConfig::BG_Color, AsConfig::Global_Scale);
	Highlight_Color = new AColor(AsConfig::White_Color, AsConfig::Global_Scale);
}
//-----------------------------------------------------------------------------------------------------------
void AsInfo_Panel::Choose_Font()
{
	CHOOSEFONT cf{}; 
	LOGFONT lf{};

	cf.lStructSize = sizeof(CHOOSEFONT); 
	cf.lpLogFont = &lf; 
	cf.Flags = CF_SCREENFONTS; 
	cf.nFontType = SCREEN_FONTTYPE; 

	ChooseFont(&cf); 
}
//-----------------------------------------------------------------------------------------------------------
void AsInfo_Panel::Show_Extra_Lives(HDC hdc)
{
	int i, j;

	int lives_to_draw = AsConfig::Extra_Lives_Count;

	for (j = 0; j < 3; j++)
		for (i = 0; i < 4; i++)
		{
			if (lives_to_draw <= 0)
				break;

			Draw_Extra_Life(hdc, 27 + 6 + j * 16, 58 + i * 7);
			--lives_to_draw;
		}
}
//-----------------------------------------------------------------------------------------------------------
void AsInfo_Panel::Draw_Extra_Life(HDC hdc, int x_pos, int y_pos)
{
	const int scale = AsConfig::Global_Scale;
	RECT rect;

	rect.left = (Score_X_Pos + x_pos) * scale + 1;
	rect.top = (Score_Y_Pos + y_pos) * scale;
	rect.right = rect.left + 4 * scale;
	rect.bottom = rect.top + 5 * scale;

	AsTools::Ellipse(hdc, rect, AsConfig::Red_Color);

	rect.left += 8 * scale;
	rect.right += 8 * scale;

	AsTools::Ellipse(hdc, rect, AsConfig::Red_Color);

	rect.left = (Score_X_Pos + x_pos + 2) * scale + 1;
	rect.top = (Score_Y_Pos + y_pos + 1) * scale;
	rect.right = rect.left + 8 * scale;
	rect.bottom = rect.top + 3 * scale;

	AsConfig::Blue_Color.Select(hdc);
	AsTools::Round_Rect(hdc, rect);
}
//------------------------------------------------------------------------------------------------------------
void AsInfo_Panel::Draw_Score_Panel(HDC hdc)
{
	const int scale = AsConfig::Global_Scale;
	int distance_to_shadow = 4 * scale;
	RECT panel_rect{};
	AString score_str( L"SCORE:");

	panel_rect.left = (Score_X_Pos + 2) * scale;
	panel_rect.top = (Score_Y_Pos + 2) * scale;
	panel_rect.right = panel_rect.left + (Score_Width - 4) * scale;
	panel_rect.bottom = panel_rect.top + (Score_Heigth - 4) * scale;

	// 2.1 Панель
	AsTools::Rect(hdc, Score_X_Pos, Score_Y_Pos, Score_Width, 2, AsConfig::Red_Color);
	AsTools::Rect(hdc, Score_X_Pos, Score_Y_Pos, 2, Score_Heigth, AsConfig::Red_Color);
	AsTools::Rect(hdc, Score_X_Pos, Score_Y_Pos + Score_Heigth - 2, Score_Width, 2, AsConfig::Red_Color);
	AsTools::Rect(hdc, Score_X_Pos + Score_Width - 2, Score_Y_Pos, 2, Score_Heigth, AsConfig::Red_Color);

	// 2.2 Синяя панель
	Draw_Rect_With_Shadow(hdc, panel_rect, AsConfig::Blue_Color);

	// 2.3 Панель с именем игрока
	RECT name_rect{}, name_shadow_rect{};

	name_rect.left = (Score_X_Pos + 5) * scale;
	name_rect.top = (Score_Y_Pos + 5) * scale;
	name_rect.right = name_rect.left + (Score_Width - 2 * 5) * scale;
	name_rect.bottom = name_rect.top + 16 * scale;

	Get_Shadow_Rect(name_rect, name_shadow_rect, distance_to_shadow);

	Draw_Rect_With_Shadow(hdc, name_rect, AsConfig::Red_Color);

	Draw_String(hdc, name_shadow_rect, Player_Name_Font, Player_Name, AsConfig::BG_Color);
	Draw_String(hdc, name_rect, Player_Name_Font, Player_Name, AsConfig::Blue_Color);

	// 2.4 Панель счета
	RECT score_rect{}, score_shadow_rect{};

	score_rect.left = name_rect.left;
	score_rect.top = name_rect.top + 22 * scale;
	score_rect.right = name_rect.right;
	score_rect.bottom = name_rect.bottom + 22 * scale;

	Get_Shadow_Rect(score_rect, score_shadow_rect, distance_to_shadow);

	Draw_Rect_With_Shadow(hdc, score_rect, AsConfig::Red_Color);

	score_str.Append(AsConfig::Score);

	Draw_String(hdc, score_shadow_rect, Player_Name_Font, score_str, AsConfig::BG_Color);
	Draw_String(hdc, score_rect, Player_Name_Font, score_str, AsConfig::White_Color);

}
//-----------------------------------------------------------------------------------------------------------
void AsInfo_Panel::Draw_String(HDC hdc, RECT &str_rect, HFONT &font, AString &str, const AColor &color)
{
	int str_x_pos, str_y_pos;
	SIZE str_size{};

	SelectObject(hdc, font);
	SetTextColor(hdc, color.Get_RGB() );

	GetTextExtentPoint32(hdc, str.Get_Content(), str.Get_Lenght(), &str_size);

	str_x_pos = str_rect.left + (str_rect.right - str_rect.left) / 2 - str_size.cx / 2;
	str_y_pos = str_rect.top + (str_rect.bottom - str_rect.top) / 2 - str_size.cy / 2;

	TextOut(hdc, str_x_pos, str_y_pos, str.Get_Content(), str.Get_Lenght() );
}
//-----------------------------------------------------------------------------------------------------------
void AsInfo_Panel::Draw_Rect_With_Shadow(HDC hdc, RECT &panel_rect, const AColor &panel_color)
{
	AsTools::Rect(hdc, panel_rect, panel_color);

	Shadow_Color->Select_Pen(hdc);
	MoveToEx(hdc, panel_rect.right, panel_rect.top, 0);
	LineTo(hdc, panel_rect.right, panel_rect.bottom);
	LineTo(hdc, panel_rect.left, panel_rect.bottom);

	Highlight_Color->Select_Pen(hdc);
	LineTo(hdc, panel_rect.left, panel_rect.top);
	LineTo(hdc, panel_rect.right, panel_rect.top);

}
//-----------------------------------------------------------------------------------------------------------
void AsInfo_Panel::Get_Shadow_Rect(RECT &src_rect, RECT &dst_rect, int distance_to_shadow)
{
	dst_rect.left = src_rect.left + Shadow_X_Offset - distance_to_shadow;
	dst_rect.top = src_rect.top + Shadow_Y_Offset - distance_to_shadow;
	dst_rect.right = src_rect.right + Shadow_X_Offset - distance_to_shadow;
	dst_rect.bottom = src_rect.bottom + Shadow_Y_Offset - distance_to_shadow;
}
//-----------------------------------------------------------------------------------------------------------
