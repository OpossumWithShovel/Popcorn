#include "Config.h"

// AsConfig
//------------------------------------------------------------------------------------------------------------
bool AsConfig::Level_Has_Floor = true;
int AsConfig::Current_Timer_Tick = 0;

const AColor AsConfig::BG_Color(0, 0, 0);
const AColor AsConfig::Red_Color(255, 85, 255);
const AColor AsConfig::Blue_Color(85, 255, 255);
const AColor AsConfig::White_Color(255, 255, 255);
const AColor AsConfig::Symbol_Color(AsConfig::White_Color, AsConfig::Global_Scale);
const AColor AsConfig::Teleport_Color(AsConfig::Blue_Color, AsConfig::BG_Color, AsConfig::Global_Scale);
const AColor AsConfig::Red_Highlight(AsConfig::Red_Color, 3 * AsConfig::Global_Scale);
const AColor AsConfig::Blue_Highlight(AsConfig::Blue_Color, AsConfig::Global_Scale - 1);
const AColor AsConfig::Laser_Color(AsConfig::White_Color, AsConfig::Global_Scale);
const AColor AsConfig::Gate_Highlight_Color(AsConfig::White_Color, AsConfig::Global_Scale);

HWND AsConfig::Hwnd;

const double AsConfig::D_Global_Scale = Global_Scale;
const double AsConfig::Moving_Step_Size = 1.0 / D_Global_Scale;
const double AsConfig::Start_Ball_Y_Pos = 184.0;
const double AsConfig::Max_Ball_Speed = 6.0;
const double AsConfig::Ball_Normal_Speed = 3.0;
const double AsConfig::Ball_Acceleration = 1.001;
const double AsConfig::Min_Ball_Angle = M_PI / 8.0;
//------------------------------------------------------------------------------------------------------------




// AsTools
//------------------------------------------------------------------------------------------------------------
int AsTools::Rand(int range)
{// Возвращает псевдослучайное число в диапазоне == range [0, .. range - 1]

	return rand() * range / RAND_MAX;
}
//------------------------------------------------------------------------------------------------------------
void AsTools::Round_Rect(HDC hdc, RECT &rect, int rounding_value)
{
	const int scale = AsConfig::Global_Scale;

	RoundRect(hdc, rect.left, rect.top, rect.right - 1, rect.bottom - 1, rounding_value * scale, rounding_value * scale);
}
//------------------------------------------------------------------------------------------------------------
void AsTools::Round_Rect(HDC hdc, int x, int y, int width, int height, const AColor &color, int rounding_value)
{
	const int scale = AsConfig::Global_Scale;

	color.Select(hdc);
	RoundRect(hdc, x * scale, y * scale, (x + width) * scale - 1, (y + height) * scale - 1, rounding_value * scale, rounding_value * scale);
}
//------------------------------------------------------------------------------------------------------------
void AsTools::Rect(HDC hdc, RECT &rect, const AColor &color)
{
	color.Select(hdc);
	Rectangle(hdc, rect.left, rect.top, rect.right - 1, rect.bottom - 1);
}
//------------------------------------------------------------------------------------------------------------
void AsTools::Rect(HDC hdc, int x, int y, int width, int height, const AColor &color)
{
	const int scale = AsConfig::Global_Scale;

	color.Select(hdc);
	Rectangle(hdc, x * scale, y * scale, (x + width) * scale - 1, (y + height) * scale - 1);
}
//------------------------------------------------------------------------------------------------------------
void AsTools::Ellipse(HDC hdc, RECT &rect, const AColor &color)
{
	color.Select(hdc);
	::Ellipse(hdc, rect.left, rect.top, rect.right - 1, rect.bottom - 1);
}
//------------------------------------------------------------------------------------------------------------
void AsTools::Ellipse(HDC hdc, int x, int y, int width, int height, const AColor &color)
{
	const int scale = AsConfig::Global_Scale;

	color.Select(hdc);
	::Ellipse(hdc, x * scale, y * scale, (x + width) * scale - 1, (y + height) * scale - 1);
}
//------------------------------------------------------------------------------------------------------------
void AsTools::Invalidate_Rect(RECT &rect)
{
	InvalidateRect(AsConfig::Hwnd, &rect, FALSE);
}
//------------------------------------------------------------------------------------------------------------
void AsTools::Throw()
{
	throw 13;
}
//------------------------------------------------------------------------------------------------------------
