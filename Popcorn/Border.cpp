#include "Border.h"

// AsBorder
//------------------------------------------------------------------------------------------------------------
AsBorder::~AsBorder()
{
	int i;

	for (i = 0; i < AsConfig::Gates_Count; i++)
		delete Gates[i];
}
//------------------------------------------------------------------------------------------------------------
AsBorder::AsBorder()
	: Floor_Rect{}
{
	Floor_Rect.left = AsConfig::Level_X_Offset * AsConfig::Global_Scale;
	Floor_Rect.top = AsConfig::Floor_Y_Pos * AsConfig::Global_Scale;
	Floor_Rect.right = AsConfig::Level_Max_X_Offset * AsConfig::Global_Scale;
	Floor_Rect.bottom = AsConfig::Play_Area_Max_Y_Offset * AsConfig::Global_Scale;

	Gates[0] = new AGate(2, 29);
	Gates[1] = new AGate(AsConfig::Level_Max_X_Offset, 29);

	Gates[2] = new AGate(2, 77);
	Gates[3] = new AGate(AsConfig::Level_Max_X_Offset, 77);

	Gates[4] = new AGate(2, 129);
	Gates[5] = new AGate(AsConfig::Level_Max_X_Offset, 129);

	Gates[6] = new AGate(2, 178);
	Gates[7] = new AGate(AsConfig::Level_Max_X_Offset, 178);
}
//------------------------------------------------------------------------------------------------------------
bool AsBorder::Check_Hit(double next_x_pos, double next_y_pos, ABall *ball)
{// Проверяем столкновения с рамками уровня или полом
	bool got_hit = false;

	// 1. С левой или правой рамки
	if (next_x_pos - ball->Radius < Left_Border_X_Offset || next_x_pos + ball->Radius > Right_Border_X_Offset)
	{
		got_hit = true;
		ball->Reflect(false);
	}

	// 2. С верхней рамкой
	if (next_y_pos - ball->Radius < Top_Border_X_Offset)
	{
		got_hit = true;
		ball->Reflect(true);
	}
	else if (next_y_pos + ball->Radius >= AsConfig::Floor_Y_Pos && AsConfig::Level_Has_Floor)
	{
		got_hit = true;
		ball->Reflect(true);
	}
	else if (next_y_pos - ball->Radius > AsConfig::Play_Area_Max_Y_Offset)
	{
		ball->Set_State(EBall_State::Lost);
	}

	return got_hit;
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Begin_Movement()
{
	// Заглушка т.к. этот метод не используется
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Finish_Movement()
{
	// Заглушка т.к. этот метод не используется
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Advance(double max_speed)
{
	// Заглушка т.к. этот метод не используется
}
//------------------------------------------------------------------------------------------------------------
double AsBorder::Get_Speed()
{
	return 0.0;  // Заглушка т.к. этот метод не используется
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Act()
{
	int i;

	for (i = 0; i < AsConfig::Gates_Count; i++)
		Gates[i]->Act();
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Clear(HDC hdc, RECT &paint_area)
{
	int i;
	RECT intersection_rect;

	for (i = 0; i < AsConfig::Gates_Count; i++)
		Gates[i]->Clear(hdc, paint_area);

	if (AsConfig::Level_Has_Floor)
		return;

	if ( ! IntersectRect(&intersection_rect, &paint_area, &Floor_Rect) )
		return;

	AsTools::Rect(hdc, Floor_Rect, AsConfig::BG_Color);
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Draw(HDC hdc, RECT &paint_area)
{
	int i;

	Draw_Bounds(hdc, paint_area);

	if (AsConfig::Level_Has_Floor)
		Draw_Floor(hdc, paint_area);

	for (i = 0; i < AsConfig::Gates_Count; i++)
		Gates[i]->Draw(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------
bool AsBorder::Is_Finished()
{
	return false;  // Заглушка. Этот метод не используется
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Redraw_Floor()
{
	AsTools::Invalidate_Rect(Floor_Rect);
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Open_Gate(int gate_index, bool is_short_open)
{
	if (gate_index < 0 || gate_index >= AsConfig::Gates_Count)
	{
		AsTools::Throw();
		return;
	}

	if (gate_index != AsConfig::Gates_Count - 1 && is_short_open == true)
		AsTools::Throw();

	Gates[gate_index]->Open_Gate(is_short_open);
}
//------------------------------------------------------------------------------------------------------------
bool AsBorder::Is_Gate_Open(int gate_index)
{
	if (gate_index < 0 || gate_index >= AsConfig::Gates_Count)
	{
		AsTools::Throw();
		return false;
	}

	return Gates[gate_index]->Is_Gate_Open();
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Get_Gate_Pos(int gate_index, int &gate_x_pos, int &gate_y_pos)
{
	Gates[gate_index]->Get_Gate_Pos(gate_x_pos, gate_y_pos);
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Draw_Tile(HDC hdc, RECT &paint_area, int x, int y, bool is_top) const
{
	int i;
	int gate_top_y, gate_low_y;
	RECT intersection_rect, tile_rect{};

	tile_rect.left = x * AsConfig::Global_Scale;
	tile_rect.top = y * AsConfig::Global_Scale;
	tile_rect.right = tile_rect.left + Tile_Size * AsConfig::Global_Scale;
	tile_rect.bottom  = tile_rect.top + Tile_Size * AsConfig::Global_Scale;

	if ( ! IntersectRect(&intersection_rect, &paint_area, &tile_rect) )
		return;

	if (! is_top)
	{
		for (i = 0; i < AsConfig::Gates_Count; i++)
		{
			Gates[i]->Get_Y_Area(gate_top_y, gate_low_y);

			if (gate_top_y <= tile_rect.top && gate_low_y >= tile_rect.bottom)
				return;
		}
	}

	AsTools::Rect(hdc, tile_rect, AsConfig::Blue_Color);
	AsTools::Rect(hdc, x + 2, y + 2, 1, 1, AsConfig::BG_Color);

	if (! is_top)
		AsTools::Rect(hdc, x, y, 1, Tile_Size, AsConfig::White_Color);
	else
		AsTools::Rect(hdc, x, y, Tile_Size, 1, AsConfig::White_Color);
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Draw_Bounds(HDC hdc, RECT &paint_area) const
{

	for (int i = 0; i < AsConfig::Play_Area_Max_Y_Offset / Tile_Size + 1; i++)
	{
		Draw_Tile(hdc, paint_area, Left_Border_X_Offset - Tile_Size, 1 + Tile_Size * i, false);  // left bound
		Draw_Tile(hdc, paint_area, Right_Border_X_Offset, 1 + Tile_Size * i, false);  // right bound
	}
	// top bound
	for (int i = 0; i < AsConfig::Level_Max_X_Offset / Tile_Size; i++)
		Draw_Tile(hdc, paint_area, Tile_Size + Tile_Size * i, 0, true);
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Draw_Floor(HDC hdc, RECT &paint_area) const
{
	int i;
	int x, y;
	int line_len, gap_len;
	int stroke_len, strokes_count;
	RECT intersection_rect;

	if ( ! IntersectRect(&intersection_rect, &paint_area, &Floor_Rect) )
		return;

	line_len = 6 * AsConfig::Global_Scale;
	gap_len = 2 * AsConfig::Global_Scale;

	x = Floor_Rect.left + gap_len - 1;
	y = Floor_Rect.top + (Floor_Rect.bottom - Floor_Rect.top) / 2;

	stroke_len = line_len + gap_len;

	strokes_count = (Floor_Rect.right - Floor_Rect.left) / stroke_len;

	for (i = 0; i < strokes_count; i++)
	{
		AsConfig::Symbol_Color.Select_Pen(hdc);

		MoveToEx(hdc, x, y, 0);
		LineTo(hdc, x + line_len - 1, y);

		x += stroke_len;
	}
}
//------------------------------------------------------------------------------------------------------------
