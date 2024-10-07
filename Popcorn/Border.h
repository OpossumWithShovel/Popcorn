#pragma once

#include "Ball.h"
#include "Gate.h"

//------------------------------------------------------------------------------------------------------------
class AsBorder: public AHit_Checker, public AGraphics_Object
{
public:
	~AsBorder();
	AsBorder();

	virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall *ball);

	virtual void Act();
	virtual void Clear(HDC hdc, RECT &paint_area);
	virtual void Draw(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

	void Redraw_Floor();
	void Open_Gate(int gate_index, bool is_short_open);
	bool Is_Gate_Open(int gate_index);

private:
	void Draw_Tile(HDC hdc, RECT &paint_area, int x, int y, bool is_top) const;
	void Draw_Bounds(HDC hdc, RECT &paint_area) const;
	void Draw_Floor(HDC hdc, RECT &paint_area) const;

	AGate *Gates[AsConfig::Gates_Count];

	RECT Floor_Rect;

	static const int Tile_Size = 4;
	static const int Left_Border_X_Offset = AsConfig::Level_X_Offset - 1;
	static const int Right_Border_X_Offset = AsConfig::Level_Max_X_Offset + 1;
	static const int Top_Border_X_Offset = AsConfig::Level_Y_Offset - Tile_Size / 2;
};
//------------------------------------------------------------------------------------------------------------
