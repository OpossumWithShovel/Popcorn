#include "Config.h"

// AsConfig
//------------------------------------------------------------------------------------------------------------
bool AsConfig::Level_Has_Floor = false;
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
const AColor AsConfig::Cornea_Color(AsConfig::BG_Color, AsConfig::White_Color, AsConfig::Global_Scale * 2 / 3);
const AColor AsConfig::Iris_Color(AsConfig::BG_Color, AsConfig::Blue_Color, AsConfig::Global_Scale * 2 / 3);
const AColor AsConfig::Explode_Outline_Color(AsConfig::White_Color);

HWND AsConfig::Hwnd;

const double AsConfig::D_Global_Scale = (double)Global_Scale;
const double AsConfig::Moving_Step_Size = 1.0 / D_Global_Scale;
const double AsConfig::Start_Ball_Y_Pos = 184.0;
const double AsConfig::Max_Ball_Speed = 6.0;
const double AsConfig::Ball_Normal_Speed = 3.0;
const double AsConfig::Ball_Acceleration = 1.001;
const double AsConfig::Min_Ball_Angle = M_PI / 8.0;
const double AsConfig::Ball_Radius = 2.0 - 0.5 / AsConfig::D_Global_Scale;
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
void AsTools::Ellipse_Outline(HDC hdc, RECT &rect, const AColor &color)
{
	color.Select(hdc);
	Arc(hdc, rect.left, rect.top, rect.right - 1, rect.bottom - 1, 0, 0, 0, 0);
}
//------------------------------------------------------------------------------------------------------------
void AsTools::Invalidate_Rect(RECT &rect)
{
	InvalidateRect(AsConfig::Hwnd, &rect, FALSE);
}
//------------------------------------------------------------------------------------------------------------
unsigned char AsTools::Get_Fading_Channel(unsigned char channel, unsigned char bg_channel, int step, int steps_count)
{
	return channel - step * (channel - bg_channel) / (steps_count - 1);
}
//------------------------------------------------------------------------------------------------------------
void AsTools::Get_Fading_Color(const AColor &origin_color, int step, AColor &modified_color, int steps_count)
{
	unsigned char r, g, b;

	r = Get_Fading_Channel(origin_color.R, AsConfig::BG_Color.R, step, steps_count);
	g = Get_Fading_Channel(origin_color.G, AsConfig::BG_Color.G, step, steps_count);
	b = Get_Fading_Channel(origin_color.B, AsConfig::BG_Color.B, step, steps_count);

	modified_color = AColor(r, g, b);
}
//------------------------------------------------------------------------------------------------------------
bool AsTools::Reflect_From_Circle(double next_x_pos, double next_y_pos, double circle_radius, double circle_x, double circle_y, ABall_Object *ball)
{
	double dx, dy;
	double distance, two_radiuses;
	double alpha, beta, gamma;
	double related_ball_direction;

	dx = next_x_pos - circle_x;
	dy = next_y_pos - circle_y;

	distance = sqrt(dy * dy + dx * dx);
	two_radiuses = circle_radius + AsConfig::Ball_Radius;

	if (distance + AsConfig::Moving_Step_Size < two_radiuses)
	{
		beta = atan2(-dy, dx);

		related_ball_direction = ball->Get_Direction();
		related_ball_direction -= beta;

		if (related_ball_direction > 2.0 * M_PI)
			related_ball_direction -= 2.0 * M_PI;
		if (related_ball_direction < 0.0)
			related_ball_direction += 2.0 * M_PI;

		if (related_ball_direction > M_PI_2 && related_ball_direction < M_PI + M_PI_2)
		{
			alpha = beta + M_PI - ball->Get_Direction();
			gamma = beta + alpha;

			ball->Set_Direction(gamma);
			return true;
		}
	}

	return false;
}
//------------------------------------------------------------------------------------------------------------
void AsTools::Throw()
{
	throw 13;
}
//------------------------------------------------------------------------------------------------------------





// AHit_Checker
//------------------------------------------------------------------------------------------------------------
bool AHit_Checker::Check_Hit(double next_x_pos, double next_y_pos)
{
	return false;
}
//------------------------------------------------------------------------------------------------------------
bool AHit_Checker::Check_Hit(RECT &rect)
{
	return false;
}
//------------------------------------------------------------------------------------------------------------
bool AHit_Checker::Hit_Circle_On_Line(double distance_to_line, double ball_center_pos, double line_start_pos, double line_end_pos, double radius, double &x)
{// checking intersection line (that has a start coordinate (line_start_pos) and end coordinate (line_end_pos)) with ball (that has radius)

	double min_x, max_x;

	if (fabs(distance_to_line) > radius)
		return false;

	x = sqrt(radius * radius - distance_to_line * distance_to_line);

	max_x = ball_center_pos + x;
	min_x = ball_center_pos - x;

	if (max_x >= line_start_pos && max_x <= line_end_pos || min_x >= line_start_pos && min_x <= line_end_pos)
		return true;
	else
		return false;
}
//------------------------------------------------------------------------------------------------------------




// AHit_Checker_List
//------------------------------------------------------------------------------------------------------------
AHit_Checker_List::AHit_Checker_List()
	: 	Hit_Checkers_Count(0), Hit_Checkers{}
{}
//------------------------------------------------------------------------------------------------------------
void AHit_Checker_List::Add_Hit_Checker(AHit_Checker *hit_checker)
{
	if (Hit_Checkers_Count >= sizeof(Hit_Checkers) / sizeof(Hit_Checkers[0]) )
	{
		AsTools::Throw();
		return;
	}

	Hit_Checkers[Hit_Checkers_Count++] = hit_checker;
}
//------------------------------------------------------------------------------------------------------------
bool AHit_Checker_List::Check_Hit(double x_pos, double y_pos)
{
	int i;

	for (i = 0; i < Hit_Checkers_Count; i++)
		if (Hit_Checkers[i]->Check_Hit(x_pos, y_pos) )
			return true;

	return false;
}
//------------------------------------------------------------------------------------------------------------
bool AHit_Checker_List::Check_Hit(double x_pos, double y_pos, ABall_Object *ball)
{
	int i;

	for (i = 0; i < Hit_Checkers_Count; i++)
		if (Hit_Checkers[i]->Check_Hit(x_pos, y_pos, ball) )
			return true;

	return false;
}
//------------------------------------------------------------------------------------------------------------
bool AHit_Checker_List::Check_Hit(RECT &rect)
{
	int i;

	for (i = 0; i < Hit_Checkers_Count; i++)
		if (Hit_Checkers[i]->Check_Hit(rect) )
			return true;

	return false;
}
//------------------------------------------------------------------------------------------------------------
