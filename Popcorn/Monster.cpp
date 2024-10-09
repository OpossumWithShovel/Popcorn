#include "Monster.h"


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
