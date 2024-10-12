#pragma once

#include "Falling_Letter.h"

//-----------------------------------------------------------------------------------------------------------
enum class EScore_Event_Type: unsigned char
{
	Hit_Brick,
	Hit_Monster,
	Catch_Letter
};
//-----------------------------------------------------------------------------------------------------------
class AIndicator: public AGraphics_Object
{
public:
	AIndicator(int x_pos, int y_pos);

	virtual void Act();
	virtual void Clear(HDC hdc, RECT &paint_area);
	virtual void Draw(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

	void Restart();

private:
	int X_Pos, Y_Pos;
	int Indicator_Timer_Tick;

	RECT Indicator_Rect;

	static const int Indicator_Width = 13;
	static const int Indicator_Height = 30;
	static const int Inner_Width = Indicator_Width - 2;
	static const int Inner_Height = Indicator_Height - 2;
	static const int Indicator_Timeout = 5 * AsConfig::FPS;
};
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

	static void Update_Score(EScore_Event_Type event_type);

	AIndicator Floor_Indicator, Monster_Indicator;

	static int Extra_Lives_Count;

private:
	void Choose_Font();
	void Show_Extra_Lives(HDC hdc);
	void Draw_Logo(HDC hdc, RECT &paint_area);
	void Draw_Extra_Life(HDC hdc, int x_pos, int y_pos);
	void Draw_Indicators_Panel(HDC hdc, RECT &paint_area);
	void Draw_Player_Name_Panel(HDC hdc, RECT &paint_area);
	void Draw_Score_Panel(HDC hdc, RECT &paint_area);
	void Draw_String(HDC hdc, RECT &str_rect, HFONT &font, AString &str, const AColor &color);
	void Draw_Rect_With_Shadow(HDC hdc, RECT &panel_rect, const AColor &panel_color);
	void Get_Shadow_Rect(RECT &src_rect, RECT &dst_rect, int distance_to_shadow);

	AColor *Shadow_Color, *Highlight_Color;
	AFalling_Letter Letter_F, Letter_M, Letter_Plus;

	static int Score;

	RECT Logo_Rect;
	RECT Indicators_Panel_Rect, Name_Rect;
	static RECT Score_Rect;

	AString Player_Name;

	HFONT Logo_Pop_Font, Logo_Corn_Font, Player_Name_Font; 

	static const int Shadow_X_Offset = 5 * AsConfig::Global_Scale;
	static const int Shadow_Y_Offset = 5 * AsConfig::Global_Scale;
	static const int Indicators_Panel_X_Pos = 208;
	static const int Indicators_Panel_Y_Pos = 108;
	static const int Indicators_Panel_Width = 110;
	static const int Indicators_Panel_Heigth = 90;
	static const int Logo_X_Pos = 223;
	static const int Logo_Y_Pos = 3;
};
//-----------------------------------------------------------------------------------------------------------
