#pragma once

#include "Border.h"

enum class EExplosive_Ball_State: unsigned char
{
	Disable,
	Waiting,
	Expanding,
	Fading
};
//------------------------------------------------------------------------------------------------------------
class AExplosive_Ball: public AGraphics_Object
{
public:
	AExplosive_Ball();

	virtual void Act();
	virtual void Clear(HDC hdc, RECT &paint_area);
	virtual void Draw(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

	void Explode(double x_pos, double y_pos, double max_size, int explode_delay_ticks, bool is_red);

	static void Setup_Colors();

private:
	void Act_Expanding_State();
	void Act_Fading_State();
	void Update_Rect();

	EExplosive_Ball_State Explosive_Ball_State;

	bool Is_Red;

	int Start_Explode_Tick, Start_Fading_Tick, End_Explode_Tick;

	double X_Pos, Y_Pos;
	double Size, Max_Size;

	RECT Curr_Rect;

	AColor *Curr_Ball_Color;
	AColor *Curr_Outline_Color;

	static const int Expanding_Timeout = AsConfig::FPS / 2;
	static const int Fading_Timeout = AsConfig::FPS / 2;
	static const int Fading_Steps_Count = Fading_Timeout;

	static AColor Fading_Red_Colors[Fading_Steps_Count];
	static AColor Fading_Blue_Colors[Fading_Steps_Count];
	static AColor Fading_Outline_Colors[Fading_Steps_Count];
};
//------------------------------------------------------------------------------------------------------------
enum class EEye_State: unsigned char
{
	Closed,
	Opening,
	Staring,
	Closing
};
//------------------------------------------------------------------------------------------------------------
enum class EMonster_State: unsigned char
{
	Missing,
	Alive,
	Destroing
};
//------------------------------------------------------------------------------------------------------------
class AMonster: public AGame_Object
{
public:
	AMonster();

	virtual void Begin_Movement();
	virtual void Finish_Movement();
	virtual void Advance(double max_speed);
	virtual double Get_Speed();

	virtual void Act();
	virtual void Clear(HDC hdc, RECT &paint_area);
	virtual void Draw(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

	void Activate(int gate_x_pos, int gate_y_pos);
	bool Is_Active() const;
	void Destroy();

private:
	void Act_Alive();
	void Act_Destroing();
	void Draw_Alive(HDC hdc);
	void Draw_Destroing(HDC hdc, RECT &paint_area);

	EMonster_State Monster_State;
	EEye_State Eye_State;

	int X_Pos, Y_Pos;
	int Start_Blink_Timer_Tick;
	int Total_Animation_Timeout;

	double Cornea_Height;

	RECT Monster_Rect;

	static const int Monster_Size = 16;
	static const int Blinks_Stages_Count = 7;
	static const int Max_Explosive_Balls_Count = 20;
	int Blink_Ticks[Blinks_Stages_Count];

	AExplosive_Ball Explosive_Balls[Max_Explosive_Balls_Count];

	static const double Max_Cornea_Height;

	static const double Blink_Timeouts[Blinks_Stages_Count];
	static const EEye_State Blink_States[Blinks_Stages_Count];
};
//------------------------------------------------------------------------------------------------------------
class AsMonsters_Set: public AsGame_Objects_Set
{
public:
	AsMonsters_Set();

	void Init(AsBorder *border);
	void Let_Out(int gate_index);

private:
	virtual bool Get_Next_Obj(int &index, AGame_Object **game_obj);

	AsBorder *Border;  // UNO

	AMonster Monsters[AsConfig::Max_Monsters_Count];
};
//------------------------------------------------------------------------------------------------------------
