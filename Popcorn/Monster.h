#pragma once

#include "Gate.h"
#include "Explosive_Ball.h"
#include "Level.h"

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
	Emitting,
	Alive,
	Destroing
};
//------------------------------------------------------------------------------------------------------------
class AMonster: public AHit_Checker, public AGame_Object
{
public:
	AMonster();
	
	virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall_Object *ball);
	virtual bool Check_Hit(double next_x_pos, double next_y_pos);

	virtual void Begin_Movement();
	virtual void Finish_Movement();
	virtual void Advance(double max_speed);
	virtual double Get_Speed();

	virtual void Act();
	virtual void Clear(HDC hdc, RECT &paint_area);
	virtual void Draw(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

	void Let_Out(int gate_x_pos, int gate_y_pos, bool moving_right);
	bool Is_Active() const;
	void Destroy();

private:
	void Act_Alive();
	void Act_Destroing();
	void Draw_Alive(HDC hdc);
	void Draw_Destroing(HDC hdc, RECT &paint_area);
	void Get_Monster_Rect(double x_pos, double y_pos, RECT &rect);
	void Redraw_Monster();

	EMonster_State Monster_State;
	EEye_State Eye_State;

	double X_Pos, Y_Pos;
	int Emiting_Timer_Tick;
	int Start_Blink_Timer_Tick;
	int Total_Animation_Timeout;
	int Direction_Change_Timer_Tick;

	double Cornea_Height;
	double Direction;

	RECT Monster_Rect, Prev_Monster_Rect;

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
