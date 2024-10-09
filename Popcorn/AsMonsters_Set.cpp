#include "AsMonsters_Set.h"

// AExplosive_Ball
AColor AExplosive_Ball::Fading_Red_Colors[Fading_Steps_Count];
AColor AExplosive_Ball::Fading_Blue_Colors[Fading_Steps_Count];
AColor AExplosive_Ball::Fading_Outline_Colors[Fading_Steps_Count];
//------------------------------------------------------------------------------------------------------------
AExplosive_Ball::AExplosive_Ball()
	: Explosive_Ball_State(EExplosive_Ball_State::Disable), Is_Red(false), X_Pos(0.0), Y_Pos(0.0),
	Start_Explode_Tick(0), Start_Fading_Tick(0), End_Explode_Tick(0),
	Size(0), Max_Size(0), Curr_Rect{}, Curr_Ball_Color(0), Curr_Outline_Color(0)
{
}
//------------------------------------------------------------------------------------------------------------
void AExplosive_Ball::Act()
{
	switch (Explosive_Ball_State)
	{
	case EExplosive_Ball_State::Disable:
		return;

	case EExplosive_Ball_State::Waiting:
		if (AsConfig::Current_Timer_Tick >= Start_Explode_Tick)
		{
			Explosive_Ball_State = EExplosive_Ball_State::Expanding;
		}
		break;

	case EExplosive_Ball_State::Expanding:
		if (AsConfig::Current_Timer_Tick >= Start_Fading_Tick)
			Explosive_Ball_State = EExplosive_Ball_State::Fading;
		else
			Act_Expanding_State();
		break;

	case EExplosive_Ball_State::Fading:
		if (AsConfig::Current_Timer_Tick >= End_Explode_Tick)
			Explosive_Ball_State = EExplosive_Ball_State::Disable;

		Act_Fading_State();
		break;

	default:
		AsTools::Throw();
	}
}
//------------------------------------------------------------------------------------------------------------
void AExplosive_Ball::Clear(HDC hdc, RECT &paint_area)
{
	//!!!
}
//------------------------------------------------------------------------------------------------------------
void AExplosive_Ball::Draw(HDC hdc, RECT &paint_area)
{
	if (Explosive_Ball_State == EExplosive_Ball_State::Disable)
		return;

	if (Curr_Ball_Color != 0 && Curr_Outline_Color != 0)
	{
		AsTools::Ellipse(hdc, Curr_Rect, *Curr_Ball_Color);
		AsTools::Ellipse_Outline(hdc, Curr_Rect, *Curr_Outline_Color);
	}
	else
		AsTools::Throw();
}
//------------------------------------------------------------------------------------------------------------
bool AExplosive_Ball::Is_Finished()
{
	return false;  //!!!
}
//------------------------------------------------------------------------------------------------------------
void AExplosive_Ball::Explode(double x_pos, double y_pos, double max_size, int explode_delay_ticks, bool is_red)
{
	if (explode_delay_ticks > 0.0)
		Explosive_Ball_State = EExplosive_Ball_State::Waiting;
	else
		Explosive_Ball_State = EExplosive_Ball_State::Expanding;

	X_Pos = x_pos;
	Y_Pos = y_pos;
	Max_Size = max_size;
	Size = 0.0;
	Is_Red = is_red;

	if (is_red)
		Curr_Ball_Color = &Fading_Red_Colors[0];
	else
		Curr_Ball_Color = &Fading_Blue_Colors[0];

	Curr_Outline_Color = &Fading_Outline_Colors[0];

	Start_Explode_Tick = explode_delay_ticks + AsConfig::Current_Timer_Tick;
	Start_Fading_Tick = Start_Explode_Tick + Expanding_Timeout;
	End_Explode_Tick = Start_Fading_Tick + Fading_Timeout;
}
//------------------------------------------------------------------------------------------------------------
void AExplosive_Ball::Setup_Colors()
{
	int i;

	for (i = 0; i < Fading_Steps_Count; i++)
	{
		AsTools::Get_Fading_Color(AsConfig::Red_Color, i, Fading_Red_Colors[i], Fading_Steps_Count);
		AsTools::Get_Fading_Color(AsConfig::Blue_Color, i, Fading_Blue_Colors[i], Fading_Steps_Count);
		AsTools::Get_Fading_Color(AsConfig::Explode_Outline_Color, i, Fading_Outline_Colors[i], Fading_Steps_Count);
	}
}
//------------------------------------------------------------------------------------------------------------
void AExplosive_Ball::Act_Expanding_State()
{
	double ratio;

	ratio = (double)(AsConfig::Current_Timer_Tick - Start_Explode_Tick) / (double)Expanding_Timeout;

	Size = Max_Size * ratio;

	Update_Rect();
	AsTools::Invalidate_Rect(Curr_Rect);
}
//------------------------------------------------------------------------------------------------------------
void AExplosive_Ball::Act_Fading_State()
{
	int curr_color_index;
	double ratio;

	ratio = (double)(AsConfig::Current_Timer_Tick - Start_Fading_Tick) / (double)Expanding_Timeout;

	curr_color_index = (int)round( (double)Fading_Steps_Count * ratio);

	if (curr_color_index < 0)
		curr_color_index = 0;
	if (curr_color_index > Fading_Steps_Count - 1)
		curr_color_index = Fading_Steps_Count - 1;

	if (Is_Red)
		Curr_Ball_Color = &Fading_Red_Colors[curr_color_index];
	else
		Curr_Ball_Color = &Fading_Blue_Colors[curr_color_index];

	Curr_Outline_Color = &Fading_Outline_Colors[curr_color_index];

	AsTools::Invalidate_Rect(Curr_Rect);
}
//------------------------------------------------------------------------------------------------------------
void AExplosive_Ball::Update_Rect()
{
	Curr_Rect.left = (int)( (X_Pos - Size / 2.0) * AsConfig::D_Global_Scale);
	Curr_Rect.top = (int)( (Y_Pos - Size / 2.0) * AsConfig::D_Global_Scale);
	Curr_Rect.right = (int)( (X_Pos + Size / 2.0) * AsConfig::D_Global_Scale);
	Curr_Rect.bottom = (int)( (Y_Pos + Size / 2.0) * AsConfig::D_Global_Scale);
}
//------------------------------------------------------------------------------------------------------------





// AMonster
const double AMonster::Max_Cornea_Height = 7.0;
const double AMonster::Blink_Timeouts[] = { 0.7, 0.1, 1.0, 0.2, 0.2, 1.5, 0.5 };
const EEye_State AMonster::Blink_States[] =
{
	EEye_State::Closed, EEye_State::Opening, EEye_State::Staring, EEye_State::Closing,
	EEye_State::Opening, EEye_State::Staring, EEye_State::Closing
};
//------------------------------------------------------------------------------------------------------------
AMonster::AMonster()
: Monster_State(EMonster_State::Missing), Eye_State(EEye_State::Closed),
  X_Pos(0.0), Y_Pos(0.0), Emiting_Timer_Tick(0), Start_Blink_Timer_Tick(0), Direction_Change_Timer_Tick(0), Total_Animation_Timeout(0), Cornea_Height(0.0),
	Direction(0.0), Blink_Ticks{}, Monster_Rect{}, Prev_Monster_Rect{}
{
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Begin_Movement()
{
	if (Monster_State == EMonster_State::Missing)
		return;

	Prev_Monster_Rect = Monster_Rect;
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Finish_Movement()
{
	if (Monster_State == EMonster_State::Missing)
		return;

	Redraw_Monster();
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Advance(double max_speed)
{
	if (Monster_State == EMonster_State::Missing)
		return;

	double next_step;

	next_step = Speed / max_speed * AsConfig::Moving_Step_Size;

	X_Pos += next_step * cos(Direction);
	Y_Pos -= next_step * sin(Direction);

	if (Monster_State != EMonster_State::Alive)
		return;

	if (X_Pos < AsConfig::Level_X_Offset)
		X_Pos = AsConfig::Level_X_Offset;
	else if (X_Pos + Monster_Size > AsConfig::Level_Max_X_Offset)
		X_Pos = AsConfig::Level_Max_X_Offset - Monster_Size;

	if (Y_Pos < AsConfig::Level_Y_Offset - 1)
		Y_Pos = AsConfig::Level_Y_Offset - 1;
	else if (Y_Pos + Monster_Size > AsConfig::Play_Area_Max_Y_Offset)
		Y_Pos = AsConfig::Play_Area_Max_Y_Offset - Monster_Size;
}
//------------------------------------------------------------------------------------------------------------
double AMonster::Get_Speed()
{
	return 0.0;  // Заглушка
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Act()
{
	switch (Monster_State)
	{
	case EMonster_State::Missing:
		return;

	case EMonster_State::Emitting:
		if (AsConfig::Current_Timer_Tick >= Emiting_Timer_Tick)
			Monster_State = EMonster_State::Alive;
		// else no break!

	case EMonster_State::Alive:
		Act_Alive();
		break;

	case EMonster_State::Destroing:
		Act_Destroing();
		break;

	default:
		AsTools::Throw();
	}
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Clear(HDC hdc, RECT &paint_area)
{
	RECT intersection_rect;

	if (Monster_State == EMonster_State::Missing)
		return;

	if ( ! IntersectRect(&intersection_rect, &paint_area, &Prev_Monster_Rect) )
		return;

	AsTools::Ellipse(hdc, Prev_Monster_Rect, AsConfig::BG_Color);
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Draw(HDC hdc, RECT &paint_area)
{
	RECT intersection_rect;

	if ( ! IntersectRect(&intersection_rect, &paint_area, &Monster_Rect) )
		return;

	switch (Monster_State)
	{
	case EMonster_State::Missing:
		break;

	case EMonster_State::Emitting:
	case EMonster_State::Alive:
		Draw_Alive(hdc);
		break;

	case EMonster_State::Destroing:
		Draw_Destroing(hdc, paint_area);
		break;

	default:
		AsTools::Throw();
	}
}
//------------------------------------------------------------------------------------------------------------
bool AMonster::Is_Finished()
{
	return false;  // Заглушка
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Let_Out(int gate_x_pos, int gate_y_pos, bool left_gate)
{
	int i;
	int tick_offset;
	double curr_tick = 0;

	Monster_State = EMonster_State::Emitting;

	X_Pos = gate_x_pos;
	Y_Pos = gate_y_pos + 1;
	Speed = (double)(AsTools::Rand(5) + 1) / 10.0;
	
	if (left_gate)
		Direction = 0.0;
	else
	{
		X_Pos -= Monster_Size - AGate::Width;
		Direction = M_PI;
	}

	Redraw_Monster();

	Start_Blink_Timer_Tick = AsConfig::Current_Timer_Tick;

	for (i = 0; i < Blinks_Stages_Count; i++)
	{
		curr_tick += Blink_Timeouts[i];
		tick_offset = (int)( (double)AsConfig::FPS * curr_tick);
		Blink_Ticks[i] = tick_offset;
	}

	Total_Animation_Timeout = tick_offset;

	Emiting_Timer_Tick = AsConfig::Current_Timer_Tick + (int)( (double)AGate::Width / (double)Speed);
}
//------------------------------------------------------------------------------------------------------------
bool AMonster::Is_Active() const
{
	if (Monster_State == EMonster_State::Missing)
		return false;
	else
		return true;
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Destroy()
{
	int i;
	bool is_red;
	int monster_half_size = Monster_Size / 2;
	int monster_x_center = (int)X_Pos + monster_half_size;
	int monster_Y_center = (int)Y_Pos + monster_half_size;
	int x_offset, y_offset;
	int size;
	int delay;
	Monster_State = EMonster_State::Destroing;

	for (i = 0; i < Max_Explosive_Balls_Count; i++)
	{
		is_red = AsTools::Rand(2);
		delay = AsTools::Rand(20);
		x_offset = AsTools::Rand(monster_half_size) - monster_half_size / 2;
		y_offset = AsTools::Rand(monster_half_size) - monster_half_size / 2;

		size = (monster_half_size - (int)sqrt(x_offset * x_offset + y_offset * y_offset) ) * 2;

		Explosive_Balls[i].Explode(monster_x_center + x_offset, monster_Y_center + y_offset, size, delay, is_red);
	}
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Act_Alive()
{
	int i;
	int current_tick_offset, prev_tick;
	double ratio;

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
		Cornea_Height = Max_Cornea_Height - Max_Cornea_Height * ratio;
		break;

	default:
		AsTools::Throw();
	}

	AsTools::Invalidate_Rect(Monster_Rect);

	if (Monster_State == EMonster_State::Emitting)
		return;

	if (AsConfig::Current_Timer_Tick >= Direction_Change_Timer_Tick)
	{
		Direction_Change_Timer_Tick += AsTools::Rand(AsConfig::FPS);

		Direction += (double)(AsTools::Rand(90) - 45) * M_PI / 180.0;
	}
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Act_Destroing()
{
	int i;

	for (i = 0; i < Max_Explosive_Balls_Count; i++)
		Explosive_Balls[i].Act();
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Draw_Alive(HDC hdc)
{
	const int scale = AsConfig::Global_Scale;
	const double d_scale = AsConfig::D_Global_Scale;
	int monster_y_center = (int)( (Y_Pos + (double)Monster_Size / 2.0) * d_scale);
	int cornea_x_offset = 1;
	int iris_x_offset = 4;
	int iris_half_height = 3;
	int pupil_x_offset = 7;
	int pupil_half_height = 1;
	RECT cornea_rect{}, iris_rect{}, pupil_rect{};
	HRGN cornea_rgn;

	// 1. Монстр
	AsTools::Ellipse(hdc, Monster_Rect, AsConfig::Red_Color);

	// 2. Глаз
	// 2.1 Роговица
	cornea_rect.left = Monster_Rect.left + cornea_x_offset * scale;
	cornea_rect.top = monster_y_center - (int)(Cornea_Height / 2.0 * d_scale) - scale;
	cornea_rect.right = Monster_Rect.right - cornea_x_offset * scale;
	cornea_rect.bottom = cornea_rect.top + (int)(Cornea_Height * d_scale);

	AsTools::Ellipse(hdc, cornea_rect, AsConfig::Cornea_Color);

	// 2.2 Радужка
	cornea_rgn = CreateRectRgnIndirect(&cornea_rect);
	SelectClipRgn(hdc, cornea_rgn);

	iris_rect.left = Monster_Rect.left + iris_x_offset * scale;
	iris_rect.top = monster_y_center - iris_half_height * scale - scale;
	iris_rect.right = Monster_Rect.right - iris_x_offset * scale;
	iris_rect.bottom = monster_y_center + iris_half_height * scale;

	AsTools::Ellipse(hdc, iris_rect, AsConfig::Iris_Color);

	// 2.3 Хрусталик
	pupil_rect.left = Monster_Rect.left + pupil_x_offset * scale;
	pupil_rect.top = monster_y_center - pupil_half_height * scale - scale;
	pupil_rect.right = Monster_Rect.right - pupil_x_offset * scale;
	pupil_rect.bottom = monster_y_center + pupil_half_height * scale;

	AsTools::Rect(hdc, pupil_rect, AsConfig::BG_Color);

	SelectClipRgn(hdc, 0);
	DeleteObject(cornea_rgn);

	// 2.4 Обводка
	AsConfig::Cornea_Color.Select_Pen(hdc);
	Arc(hdc, cornea_rect.left, cornea_rect.top, cornea_rect.right, cornea_rect.bottom, 0, 0, 0, 0);
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Draw_Destroing(HDC hdc, RECT &paint_area)
{
	int i;

	for (i = 0; i < Max_Explosive_Balls_Count; i++)
		Explosive_Balls[i].Draw(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Redraw_Monster()
{
	Monster_Rect.left = (int)(X_Pos * AsConfig::D_Global_Scale);
	Monster_Rect.top = (int)(Y_Pos * AsConfig::D_Global_Scale);
	Monster_Rect.right = Monster_Rect.left + Monster_Size * AsConfig::Global_Scale;
	Monster_Rect.bottom = Monster_Rect.top + Monster_Size * AsConfig::Global_Scale;

	AsTools::Invalidate_Rect(Prev_Monster_Rect);
	AsTools::Invalidate_Rect(Monster_Rect);
}
//------------------------------------------------------------------------------------------------------------




// AsMonsters_Set
//------------------------------------------------------------------------------------------------------------
AsMonsters_Set::AsMonsters_Set()
: Monster_Set_State(EMonster_Set_State::Idle), Current_Gate(0),
  Max_Alive_Monsters_Count(0), Border(0)
{
}
//------------------------------------------------------------------------------------------------------------
void AsMonsters_Set::Act()
{
	int i;
	int curr_alive_monsters_count;

	switch (Monster_Set_State)
	{
	case EMonster_Set_State::Idle:
		break;

	case EMonster_Set_State::Select_Next_Gate:
		curr_alive_monsters_count = 0;
		for (i = 0; i < AsConfig::Max_Monsters_Count; i++)
			if (Monsters[i].Is_Active() )
				++curr_alive_monsters_count;

		if(curr_alive_monsters_count < Max_Alive_Monsters_Count)
		{
			Current_Gate = Border->Long_Open_Gate();
			Monster_Set_State = EMonster_Set_State::Waitinig_For_Open_Gate;
		}
		break;

	case EMonster_Set_State::Waitinig_For_Open_Gate:
		if (Border->Is_Gate_Open(Current_Gate) )
		{
			Let_Out(Current_Gate);
			Monster_Set_State = EMonster_Set_State::Waitinig_For_Close_Gate;
		}
		break;

	case EMonster_Set_State::Waitinig_For_Close_Gate:
		if (Border->Is_Gate_Close(Current_Gate) )
			Monster_Set_State = EMonster_Set_State::Select_Next_Gate;
		break;

	default:
		AsTools::Throw();
	}

	AGame_Objects_Set::Act();
}
//------------------------------------------------------------------------------------------------------------
void AsMonsters_Set::Init(AsBorder *border)
{
	if (border == 0)
		AsTools::Throw();

	Border = border;
}
//------------------------------------------------------------------------------------------------------------
void AsMonsters_Set::Activate(int max_alive_monsters_count)
{
	Max_Alive_Monsters_Count = max_alive_monsters_count;
	Monster_Set_State = EMonster_Set_State::Select_Next_Gate;
}
//------------------------------------------------------------------------------------------------------------
void AsMonsters_Set::Let_Out(int gate_index)
{
	int i;
	bool is_gate_left;
	int gate_x_pos, gate_y_pos;
	AMonster *curr_monster = 0;

	for (i = 0; i < AsConfig::Max_Monsters_Count; i++)
	{
		curr_monster = &Monsters[i];

		if ( ! curr_monster->Is_Active() )
			break;
	}

	if (gate_index % 2 == 0)
		is_gate_left = true;
	else
		is_gate_left = false;

	Border->Get_Gate_Pos(gate_index, gate_x_pos, gate_y_pos);

	curr_monster->Let_Out(gate_x_pos, gate_y_pos, is_gate_left);  //!!! moving right
	//curr_monster->Destroy();  //!!! monster
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