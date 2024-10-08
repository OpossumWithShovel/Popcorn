#include "AsMonsters_Set.h"

// AMonster
const double AMonster::Max_Cornea_Height = 10.0;
const double AMonster::Blink_Timeouts[] = { 0.1, 0.5, 1.0, 0.4, 0.4, 0.7, 0.5 };
const EEye_State AMonster::Blink_States[] =
{
	EEye_State::Closed, EEye_State::Opening, EEye_State::Staring, EEye_State::Closing,
	EEye_State::Opening, EEye_State::Staring, EEye_State::Closing
};
//------------------------------------------------------------------------------------------------------------
AMonster::AMonster()
: Is_Active(false), Eye_State(EEye_State::Closed), X_Pos(0), Y_Pos(0), Start_Blink_Timer_Tick(0),
Total_Animation_Timeout(0), Cornea_Height(0.0), Blink_Ticks{}, Monster_Rect{}, Prev_Monster_Rect{}
{
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Begin_Movement()
{
	// Заглушка
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Finish_Movement()
{
	// Заглушка
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Advance(double max_speed)
{
	// Заглушка
}
//------------------------------------------------------------------------------------------------------------
double AMonster::Get_Speed()
{
	return 0.0;  // Заглушка
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Act()
{
	int i;
	int current_tick_offset, prev_tick;
	double ratio;

	if (! Is_Active)
		return;

	current_tick_offset = (AsConfig::Current_Timer_Tick - Start_Blink_Timer_Tick) % Total_Animation_Timeout;

	for (i = 0; i < Blinks_Stages_Count; i++)
	{
		if (current_tick_offset < Blink_Ticks[i])
		{
			Eye_State = Blink_States[i];
			break;
		}
	}
	
	if (i == 0)
		prev_tick = 0;
	else
		prev_tick = Blink_Ticks[i - 1];

	ratio = (double)(current_tick_offset - prev_tick) / (double)(Blink_Ticks[i] - prev_tick);

	switch (Eye_State)
	{
	case EEye_State::Closed:
		Cornea_Height = 0.0;
		break;

	case EEye_State::Opening:
		Cornea_Height = Max_Cornea_Height * ratio;
		break;

	case EEye_State::Staring:
		Cornea_Height = Max_Cornea_Height;
		break;

	case EEye_State::Closing:
		Cornea_Height = 1.0 - Max_Cornea_Height * ratio;
		break;

	default:
		AsTools::Throw();
	}

	AsTools::Invalidate_Rect(Monster_Rect);
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Clear(HDC hdc, RECT &paint_area)
{
	//RECT intersection_rect;

	//if ( ! IntersectRect(&intersection_rect, &paint_area, &Prev_Monster_Rect) )
	//	return;

	//AsTools::Ellipse(hdc, Prev_Monster_Rect, AsConfig::BG_Color);
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Draw(HDC hdc, RECT &paint_area)
{
	const int scale = AsConfig::Global_Scale;
	const double d_scale = AsConfig::D_Global_Scale;
	int monster_y_center = (Y_Pos + Monster_Height / 2) * scale;
	int cornea_x_offset = 1;
	int iris_x_offset = 4;
	int iris_half_height = 3;
	int pupil_x_offset = 7;
	int pupil_half_height = 1;
	RECT intersection_rect;
	RECT cornea_rect{}, iris_rect{}, pupil_rect{};
	HRGN cornea_rgn;

	if (! Is_Active)
		return;

	if ( ! IntersectRect(&intersection_rect, &paint_area, &Monster_Rect) )
		return;

	AsTools::Ellipse(hdc, Monster_Rect, AsConfig::Red_Color);

	if (Eye_State == EEye_State::Closed)
		return;

	cornea_rect.left = Monster_Rect.left + cornea_x_offset * scale;
	cornea_rect.top = monster_y_center - (int)(Cornea_Height / 2.0 * d_scale) - scale;
	cornea_rect.right = Monster_Rect.right - cornea_x_offset * scale;
	cornea_rect.bottom = cornea_rect.top + (int)(Cornea_Height * d_scale);
	
	AsTools::Ellipse(hdc, cornea_rect, AsConfig::Cornea_Color);

	cornea_rgn = CreateRectRgnIndirect(&cornea_rect);
	
	iris_rect.left = Monster_Rect.left + iris_x_offset * scale;
	iris_rect.top = monster_y_center - iris_half_height * scale - scale;
	iris_rect.right = Monster_Rect.right - iris_x_offset * scale;
	iris_rect.bottom = monster_y_center + iris_half_height * scale;
	
	SelectClipRgn(hdc, cornea_rgn);
	AsTools::Ellipse(hdc, iris_rect, AsConfig::Iris_Color);
	
	pupil_rect.left = Monster_Rect.left + pupil_x_offset * scale;
	pupil_rect.top = monster_y_center - pupil_half_height * scale - scale;
	pupil_rect.right = Monster_Rect.right - pupil_x_offset * scale;
	pupil_rect.bottom = monster_y_center + pupil_half_height * scale;
	
	AsTools::Rect(hdc, pupil_rect, AsConfig::BG_Color);

	SelectClipRgn(hdc, 0);

	AsConfig::Cornea_Color.Select_Pen(hdc);
	Arc(hdc, cornea_rect.left, cornea_rect.top, cornea_rect.right, cornea_rect.bottom, 0, 0, 0, 0);

	DeleteObject(cornea_rgn);
}
//------------------------------------------------------------------------------------------------------------
bool AMonster::Is_Finished()
{
	return false;  // Заглушка
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Let_Out(int gate_x_pos, int gate_y_pos)
{
	int i;
	int tick_offset;
	double curr_tick = 0;

	Is_Active = true;

	X_Pos = gate_x_pos + 20;
	Y_Pos = gate_y_pos;
 
	Monster_Rect.left = X_Pos * AsConfig::Global_Scale;
	Monster_Rect.top = Y_Pos * AsConfig::Global_Scale;
	Monster_Rect.right = Monster_Rect.left + Monster_Width * AsConfig::Global_Scale;
	Monster_Rect.bottom = Monster_Rect.top + Monster_Height * AsConfig::Global_Scale;
	
	Redraw();

	Start_Blink_Timer_Tick = AsConfig::Current_Timer_Tick;

	for (i = 0; i < Blinks_Stages_Count; i++)
	{
		curr_tick += Blink_Timeouts[i];
		tick_offset = (int)( (double)AsConfig::FPS * curr_tick);
		Blink_Ticks[i] = tick_offset;
	}

	Total_Animation_Timeout = tick_offset;
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Redraw()
{
	Prev_Monster_Rect = Monster_Rect;

	Monster_Rect.left = X_Pos * AsConfig::Global_Scale;
	Monster_Rect.top = Y_Pos * AsConfig::Global_Scale;
	Monster_Rect.right = Monster_Rect.left + Monster_Width * AsConfig::Global_Scale;
	Monster_Rect.bottom = Monster_Rect.top + Monster_Height * AsConfig::Global_Scale;

	AsTools::Invalidate_Rect(Prev_Monster_Rect);
	AsTools::Invalidate_Rect(Monster_Rect);
}
//------------------------------------------------------------------------------------------------------------




// AsMonsters_Set
//------------------------------------------------------------------------------------------------------------
AsMonsters_Set::AsMonsters_Set()
: Border(0)
{
}
//------------------------------------------------------------------------------------------------------------
void AsMonsters_Set::Init(AsBorder *border)
{
	if (border == 0)
		AsTools::Throw();

	Border = border;
}
//------------------------------------------------------------------------------------------------------------
void AsMonsters_Set::Let_Out(int gate_index)
{
	int i;
	int gate_x_pos, gate_y_pos;
	AMonster *curr_monster = 0;

	for (i = 0; i < AsConfig::Max_Monsters_Count; i++)
	{
		curr_monster = &Monsters[i];

		if (!curr_monster->Is_Active)
		{
			Border->Get_Gate_Pos(gate_index, gate_x_pos, gate_y_pos);

			curr_monster->Let_Out(gate_x_pos, gate_y_pos);
		}
	}
}
//------------------------------------------------------------------------------------------------------------
bool AsMonsters_Set::Get_Next_Obj(int &index, AGame_Object **game_obj)
{
	if (index < 0 || index >= AsConfig::Max_Monsters_Count)
		return false;

	*game_obj = &Monsters[index++];

	return true;
}
//------------------------------------------------------------------------------------------------------------