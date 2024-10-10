#include "Laser_Beams_Set.h"

// ALaser_Beams_Set
//------------------------------------------------------------------------------------------------------------
void ALaser_Beams_Set::Fire(double left_gun_x_pos, double right_gun_x_pos)
{
	int i;
	ALaser_Beam *curr_beam = 0;
	ALaser_Beam *left_beam = 0, *right_beam = 0;

	for (i = 0; i < AsConfig::Max_Laser_Beams_Count; i++)
	{
		curr_beam = &Laser_Beams[i];

		if (curr_beam->Get_State() != ELaser_Beam_State::Disable)
			continue;

		if (left_beam == 0)
			left_beam = curr_beam;
		else if (right_beam == 0)
		{
			right_beam = curr_beam;
			break;
		}
	}

	if (left_beam != 0)
		left_beam->Set_On_Gun(left_gun_x_pos);
	if (right_beam != 0)
		right_beam->Set_On_Gun(right_gun_x_pos);

	if (left_beam == 0 || right_beam == 0)
		AsTools::Throw();
}
//------------------------------------------------------------------------------------------------------------
void ALaser_Beams_Set::Disable_All()
{
	int i;

	for (i = 0; i < AsConfig::Max_Laser_Beams_Count; i++)
		Laser_Beams[i].Disable();
}
//------------------------------------------------------------------------------------------------------------
bool ALaser_Beams_Set::Get_Next_Obj(int &index, AGame_Object **game_obj)
{
	if (index < 0 || index >= AsConfig::Max_Laser_Beams_Count)
		return false;

	*game_obj = &Laser_Beams[index++];
	
	return true;
}
//------------------------------------------------------------------------------------------------------------
