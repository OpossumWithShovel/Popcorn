#pragma once

#include "Border.h"

//------------------------------------------------------------------------------------------------------------
enum class EEye_State: unsigned char
{
	Closed,
	Opening,
	Staring,
	Closing
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

	void Let_Out(int gate_x_pos, int gate_y_pos);

	bool Is_Active;
private:
	void Redraw();

	EEye_State Eye_State;

	int X_Pos, Y_Pos;
	int Start_Blink_Timer_Tick;
	int Total_Animation_Timeout;

	double Cornea_Height;

	RECT Monster_Rect, Prev_Monster_Rect;

	static const int Blinks_Stages_Count = 7;
	int Blink_Ticks[Blinks_Stages_Count];

	static const double Max_Cornea_Height;

	static const int Monster_Width = 16;
	static const int Monster_Height = 16;
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
