#pragma once

#include "Ball.h"

//-----------------------------------------------------------------------------------------------------------
class AsBall_Set: public AsGame_Objects_Set
{
public:
	//virtual void Begin_Movement();
	//virtual void Finish_Movement();
	//virtual void Advance(double max_speed);
	//virtual double Get_Speed();

	virtual void Act();
	//virtual void Clear(HDC hdc, RECT &paint_area);
	//virtual void Draw(HDC hdc, RECT &paint_area);
	//virtual bool Is_Finished();

	void Set_On_Platform(double platform_x_pos);
	void Release_From_Platform(double platform_x_pos);
	bool Release_From_Glue_Platform();
	bool Are_All_Balls_Lost();
	void Set_For_Test();
	bool Is_Test_Finished();
	void Triple_Balls();
	void Inverse_Direction_Balls();
	void Accelerate();
	void Reset_Speed();
	void Forced_Advance(double offset);

private:
	virtual bool Get_Next_Obj(int &index, AGame_Object **game_obj);

	ABall Balls[AsConfig::Max_Balls_Count];
};
//-----------------------------------------------------------------------------------------------------------
