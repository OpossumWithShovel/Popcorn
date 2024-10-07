#include "Laser_Beams_Set.h"

// ALaser_Beams_Set
//------------------------------------------------------------------------------------------------------------
void ALaser_Beams_Set::Begin_Movement()
{
	int i;

	for (i = 0; i < AsConfig::Max_Laser_Beams_Count; i++)
		Laser_Beams[i].Begin_Movement();
}
//------------------------------------------------------------------------------------------------------------
void ALaser_Beams_Set::Finish_Movement()
{
	int i;

	for (i = 0; i < AsConfig::Max_Laser_Beams_Count; i++)
		Laser_Beams[i].Finish_Movement();
}
//------------------------------------------------------------------------------------------------------------
void ALaser_Beams_Set::Advance(double max_speed)
{
	int i;

	for (i = 0; i < AsConfig::Max_Laser_Beams_Count; i++)
		Laser_Beams[i].Advance(max_speed);
}
//------------------------------------------------------------------------------------------------------------
double ALaser_Beams_Set::Get_Speed()
{
	int i;
	double max_speed = 0.0;
	double curr_speed;

	for (i = 0; i < AsConfig::Max_Laser_Beams_Count; i++)
	{
		if (Laser_Beams[i].Get_State() != ELaser_Beam_State::Disable)
			continue;

		curr_speed = fabs(Laser_Beams[i].Get_Speed() );

		if (curr_speed > max_speed)
			max_speed = curr_speed;
	}

	return max_speed;
}
//------------------------------------------------------------------------------------------------------------
void ALaser_Beams_Set::Act()
{
	// Заглушка. Этот метод не используется, т.к. у лазеров нет анимации
}
//------------------------------------------------------------------------------------------------------------
void ALaser_Beams_Set::Clear(HDC hdc, RECT &paint_area)
{
	int i;

	for (i = 0; i < AsConfig::Max_Laser_Beams_Count; i++)
		Laser_Beams[i].Clear(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------
void ALaser_Beams_Set::Draw(HDC hdc, RECT &paint_area)
{
	int i;

	for (i = 0; i < AsConfig::Max_Laser_Beams_Count; i++)
		Laser_Beams[i].Draw(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------
bool ALaser_Beams_Set::Is_Finished()
{
	return false;  //!!!
}
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
