#pragma once

#include "Laser_Beam.h"

//------------------------------------------------------------------------------------------------------------
class ALaser_Beams_Set: public AGame_Objects_Set
{
public:
	ALaser_Beams_Set();

	void Fire(double left_gun_x_pos, double right_gun_x_pos);
	void Disable_All();

private:
	virtual bool Get_Next_Obj(int &index, AGame_Object **game_obj);

	std::vector<ALaser_Beam> Laser_Beams;
};
//------------------------------------------------------------------------------------------------------------
