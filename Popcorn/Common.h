#pragma once

#include <Windows.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include <string>
#include <vector>

//------------------------------------------------------------------------------------------------------------
enum class EBall_State: unsigned char
{
	Disabled,

	Normal,
	Lost,
	On_Platform,
	On_Glue_Platform,
	On_Parashute,
	Off_Parashute,
	Teleporting_Stage_1,
	Teleporting_Stage_2
};
//------------------------------------------------------------------------------------------------------------
class ABall_Object
{
public:
	virtual double Get_Direction() const = 0;
	virtual void Set_Direction(double new_direction) = 0;
	virtual EBall_State Get_State() const = 0;
	virtual void Set_State(EBall_State new_state, double x_pos = 0, double y_pos = 0) = 0;
	virtual void Reflect(bool from_horizontal) = 0;
	virtual bool Is_Move_Up() const = 0;
	virtual bool Is_Move_Left() const = 0;
	virtual void Set_On_Parashute(int brick_y, int brick_x) = 0;
	virtual void Get_Center(double &x_pos, double &y_pos) const = 0;
	virtual void Draw_Teleporting(HDC hdc, int step) const = 0; 
};
//------------------------------------------------------------------------------------------------------------
class AMover
{
public:
	virtual ~AMover();
	AMover();

	virtual void Begin_Movement() = 0;
	virtual void Finish_Movement() = 0;
	virtual void Advance(double max_speed) = 0;
	virtual double Get_Speed() = 0;

	double Speed;
};
//------------------------------------------------------------------------------------------------------------
class AGraphics_Object
{
public:
	virtual ~AGraphics_Object();

	virtual void Act() = 0;
	virtual void Clear(HDC hdc, RECT &paint_area) = 0;
	virtual void Draw(HDC hdc, RECT &paint_area) = 0;
	virtual bool Is_Finished() = 0;
};
//------------------------------------------------------------------------------------------------------------
class AGame_Object: public AMover, public AGraphics_Object
{
};
//------------------------------------------------------------------------------------------------------------
class AGame_Objects_Set: public AGame_Object
{
public:
	virtual void Begin_Movement();
	virtual void Finish_Movement();
	virtual void Advance(double max_speed);
	virtual double Get_Speed();

	virtual void Act();
	virtual void Clear(HDC hdc, RECT &paint_area);
	virtual void Draw(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

protected:
	virtual bool Get_Next_Obj(int &index, AGame_Object **game_obj) = 0;
};
//-----------------------------------------------------------------------------------------------------------
class AString
{
public:
	AString();
	AString(const wchar_t *str);

	void Append(int value);
	const wchar_t *Get_Content();
	int Get_Lenght();

private:
	std::wstring Content;
};
//------------------------------------------------------------------------------------------------------------