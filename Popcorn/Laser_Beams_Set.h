#pragma once

#include "Laser_Beam.h"

//------------------------------------------------------------------------------------------------------------
class ALaser_Beams_Set: public AMover, public AGraphics_Object
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

	void Fire(double left_gun_x_pos, double right_gun_x_pos);

	ALaser_Beam Laser_Beams[AsConfig::Max_Laser_Beams_Count];
};
//------------------------------------------------------------------------------------------------------------
