#pragma once

#include "Laser_Beam.h"

//------------------------------------------------------------------------------------------------------------
class ALaser_Beams_Set: public AsGame_Objects_Set
{
public:
	void Fire(double left_gun_x_pos, double right_gun_x_pos);

private:
	virtual bool Get_Next_Obj(int &index, AGame_Object **game_obj);

	ALaser_Beam Laser_Beams[AsConfig::Max_Laser_Beams_Count];
};
//------------------------------------------------------------------------------------------------------------
