#pragma once

#include "Falling_Letter.h"

//-----------------------------------------------------------------------------------------------------------
class AsInfo_Panel: public AGame_Object
{
public:
	~AsInfo_Panel();
	AsInfo_Panel();

	virtual void Begin_Movement();
	virtual void Finish_Movement();
	virtual void Advance(double max_speed);
	virtual double Get_Speed();

	virtual void Act();
	virtual void Clear(HDC hdc, RECT &paint_area);
	virtual void Draw(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

	void Init();

private:
	void Choose_Font();
	void Show_Extra_Lives(HDC hdc);
	void Draw_Extra_Life(HDC hdc, int x_pos, int y_pos);
	void Draw_Score_Panel(HDC hdc);
	void Draw_String(HDC hdc, RECT &str_rect, HFONT &font, AString &str, const AColor &color);
	void Draw_Rect_With_Shadow(HDC hdc, RECT &panel_rect, const AColor &panel_color);
	void Get_Shadow_Rect(RECT &src_rect, RECT &dst_rect, int distance_to_shadow);

	AColor *Shadow_Color, *Highlight_Color;
	AFalling_Letter Letter_F, Letter_M, Letter_Plus;

	AString Player_Name;

	HFONT Logo_Pop_Font, Logo_Corn_Font, Player_Name_Font; 

	static const int Shadow_X_Offset = 5 * AsConfig::Global_Scale;
	static const int Shadow_Y_Offset = 5 * AsConfig::Global_Scale;
	static const int Score_X_Pos = 208;
	static const int Score_Y_Pos = 108;
	static const int Score_Width = 110;
	static const int Score_Heigth = 90;
};
//-----------------------------------------------------------------------------------------------------------
