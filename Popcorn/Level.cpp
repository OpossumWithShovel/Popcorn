#include "Level.h"

//------------------------------------------------------------------------------------------------------------
char AsLevel::Test_Level[AsConfig::Level_Height][AsConfig::Level_Width] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 5,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
//------------------------------------------------------------------------------------------------------------
char AsLevel::Level_01[AsConfig::Level_Height][AsConfig::Level_Width] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
//------------------------------------------------------------------------------------------------------------

// AsLevel
AsLevel *AsLevel::Level = 0;
char AsLevel::Current_Level[AsConfig::Level_Height][AsConfig::Level_Width] = {};
//------------------------------------------------------------------------------------------------------------
AsLevel::~AsLevel()
{
	Cansel_All_Activity();

	delete[] Teleport_Bricks_Pos;
}
//------------------------------------------------------------------------------------------------------------
AsLevel::AsLevel()
	: Parashute_Brick_Color(AsConfig::Red_Color, AsConfig::Blue_Color, AsConfig::Global_Scale), Advertisment(0), Need_To_Cansel_All(false),
	Current_Brick_Left_X(0.0), Current_Brick_Right_X(0.0), Current_Brick_Top_Y(0.0), Current_Brick_Low_Y(0.0),
	Level_Rect{}, Active_Bricks_Count(0), Falling_Letters_Count(0), Teleport_Bricks_Count(0), Teleport_Bricks_Pos(0), Active_Bricks{}, Falling_Letters{}
{
	Level = this;
}
//------------------------------------------------------------------------------------------------------------
bool AsLevel::Check_Hit(double next_x_pos, double next_y_pos, ABall *ball)
{
	int i, j;
	double min_ball_x, max_ball_x;
	double min_ball_y, max_ball_y;
	int min_level_x, max_level_x;
	int min_level_y, max_level_y;
	bool got_horizontal_hit, got_vertical_hit;
	double horizontal_reflection_pos, vertical_reflection_pos;

	if (next_y_pos - ball->Radius > AsConfig::Bricks_Area_Y_Offset + (AsConfig::Level_Height - 1) * AsConfig::Cell_Height + AsConfig::Brick_Height)
		return false;

	min_ball_x = next_x_pos - ball->Radius;
	max_ball_x = next_x_pos + ball->Radius;
	min_ball_y = next_y_pos - ball->Radius;
	max_ball_y = next_y_pos + ball->Radius;

	min_level_x = (int)( (min_ball_x - AsConfig::Bricks_Area_X_Offset) / (double)AsConfig::Cell_Width);
	max_level_x = (int)( (max_ball_x - AsConfig::Bricks_Area_X_Offset) / (double)AsConfig::Cell_Width);
	min_level_y = (int)( (min_ball_y - AsConfig::Bricks_Area_Y_Offset) / (double)AsConfig::Cell_Height);
	max_level_y = (int)( (max_ball_y - AsConfig::Bricks_Area_Y_Offset) / (double)AsConfig::Cell_Height);

	if (min_level_x < 0)
		min_level_x = 0;

	if (min_level_y < 0)
		min_level_y = 0;

	if (max_level_x >= AsConfig::Level_Width)
		max_level_x = AsConfig::Level_Width - 1;

	if (max_level_y >= AsConfig::Level_Height)
		max_level_y = AsConfig::Level_Height - 1;

	for (i = max_level_y; i >= min_level_y; i--)
	{
		Current_Brick_Top_Y = AsConfig::Bricks_Area_Y_Offset + i * AsConfig::Cell_Height;
		Current_Brick_Low_Y = Current_Brick_Top_Y + AsConfig::Brick_Height;

		for (j = min_level_x; j <= max_level_x; j++)
		{
			if (Current_Level[i][j] == 0)
				continue;

			Current_Brick_Left_X = AsConfig::Bricks_Area_X_Offset + j * AsConfig::Cell_Width;
			Current_Brick_Right_X = Current_Brick_Left_X + AsConfig::Brick_Width;

			got_vertical_hit = Check_Vertical_Hit(next_x_pos, next_y_pos, j, i, vertical_reflection_pos, ball);
			got_horizontal_hit = Check_Horizontal_Hit(next_x_pos, next_y_pos, j, i, horizontal_reflection_pos, ball);

			if (got_vertical_hit && got_horizontal_hit)
			{
				if (On_Hit(i, j, ball, true) )
				{
					if (vertical_reflection_pos < horizontal_reflection_pos)
						ball->Reflect(true);
					else
						ball->Reflect(false);
				}

				return true;
			}
			else if (got_vertical_hit)
			{
				if (On_Hit(i, j, ball, true) )
					ball->Reflect(true);

				return true;
			}
			else if (got_horizontal_hit)
			{
				if (On_Hit(i, j, ball, false) )
					ball->Reflect(false);
				return true;
			}
		}
	}

	return false;
}
//------------------------------------------------------------------------------------------------------------
bool AsLevel::Check_Hit(double next_x_pos, double next_y_pos)
{
	int level_index_x, level_index_y;

	level_index_x = (int)( (next_x_pos - AsConfig::Bricks_Area_X_Offset) / (double)AsConfig::Cell_Width);
	level_index_y = (int)( (next_y_pos - AsConfig::Bricks_Area_Y_Offset) / (double)AsConfig::Cell_Height);

	if (level_index_x < 0 || level_index_x >= AsConfig::Level_Width)
		return false;

	if (level_index_y < 0 || level_index_y >= AsConfig::Level_Height)
		return false;

	if (Current_Level[level_index_y][level_index_x] == 0)
		return false;

	On_Hit(level_index_y, level_index_x);

	return true;
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Begin_Movement()
{
	// Заглушка т.к. этот метод не используется
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Finish_Movement()
{
	// Заглушка т.к. этот метод не используется
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Advance(double max_speed)
{
	// Заглушка т.к. этот метод не используется
}
//------------------------------------------------------------------------------------------------------------
double AsLevel::Get_Speed()
{
	return 0.0;  // Заглушка т.к. этот метод не используется
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Act()
{
	Act_Object( (AGraphics_Object **)&Active_Bricks, AsConfig::Max_Active_Bricks_Count, Active_Bricks_Count);
	Act_Object( (AGraphics_Object **)&Falling_Letters, AsConfig::Max_Falling_Letters_count, Falling_Letters_Count);

	if (Advertisment != 0)
		Advertisment->Act();
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Clear(HDC hdc, RECT &paint_area)
{
	Clear_Object(hdc, paint_area, (AGraphics_Object**)&Falling_Letters, AsConfig::Max_Falling_Letters_count);

	if (Advertisment != 0)
		Advertisment->Clear(hdc, paint_area);

	if (Need_To_Cansel_All)
	{
		Cansel_All_Activity();
		Need_To_Cansel_All = false;
	}
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Draw(HDC hdc, RECT &paint_area)
{
	int i, j;
	RECT intersection_rect, brick_rect{};

	Clear(hdc, paint_area);

	if (Advertisment != 0)
		Advertisment->Draw(hdc, paint_area);

	if (IntersectRect(&intersection_rect, &paint_area, &Level_Rect) )
	{
		for (i = 0; i < AsConfig::Level_Height; i++)
			for (j = 0; j < AsConfig::Level_Width; j++)
			{
				brick_rect.left = (AsConfig::Bricks_Area_X_Offset + AsConfig::Cell_Width * j) * AsConfig::Global_Scale;
				brick_rect.top = (AsConfig::Bricks_Area_Y_Offset + AsConfig::Cell_Height * i) * AsConfig::Global_Scale;
				brick_rect.right = brick_rect.left + AsConfig::Brick_Width * AsConfig::Global_Scale;
				brick_rect.bottom = brick_rect.top + AsConfig::Brick_Height * AsConfig::Global_Scale;

				if (IntersectRect(&intersection_rect, &paint_area, &brick_rect))
					Draw_Brick(hdc, brick_rect, j, i);

			}

		Draw_Object(hdc, paint_area, (AGraphics_Object**)&Active_Bricks, AsConfig::Max_Active_Bricks_Count);
	}

	Draw_Object(hdc, paint_area, (AGraphics_Object**)&Falling_Letters, AsConfig::Max_Falling_Letters_count);
}
//------------------------------------------------------------------------------------------------------------
bool AsLevel::Is_Finished()
{
	return false;  // Заглушка. Этот метод не используется
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Init()
{
	Level_Rect.left = AsConfig::Bricks_Area_X_Offset * AsConfig::Global_Scale;
	Level_Rect.top = AsConfig::Bricks_Area_Y_Offset * AsConfig::Global_Scale;
	Level_Rect.right = Level_Rect.left + AsConfig::Cell_Width * AsConfig::Level_Width * AsConfig::Global_Scale;
	Level_Rect.bottom = Level_Rect.top + AsConfig::Cell_Height * AsConfig::Level_Height * AsConfig::Global_Scale;

	memset(Current_Level, 0, sizeof(Current_Level) );
	memset(Active_Bricks, 0, sizeof(Active_Bricks) );
	memset(Falling_Letters, 0, sizeof(Falling_Letters) );
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Set_Current_Level(char level[AsConfig::Level_Height][AsConfig::Level_Width])
{
	int i, j;
	int index = 0;
	Teleport_Bricks_Count = 0;
	EBrick_Type brick_type;

	memcpy(Current_Level, level, sizeof(Current_Level) );

	for (i = 0; i < AsConfig::Level_Height; i++)
	{
		for (j = 0; j < AsConfig::Level_Width; j++)
		{
			brick_type = (EBrick_Type)Current_Level[i][j];

			if (brick_type == EBrick_Type::Teleport)
				++Teleport_Bricks_Count;
		}
	}

	delete[] Teleport_Bricks_Pos;
	Teleport_Bricks_Pos = 0;

	if (Teleport_Bricks_Count != 0)
	{
		if (Teleport_Bricks_Count == 1)
			AsTools::Throw();

		Teleport_Bricks_Pos = new SPoint[Teleport_Bricks_Count];

		for (i = 0; i < AsConfig::Level_Height; i++)
		{
			for (j = 0; j < AsConfig::Level_Width; j++)
			{
				brick_type = (EBrick_Type)Current_Level[i][j];

				if (brick_type == EBrick_Type::Teleport)
				{
					Teleport_Bricks_Pos[index].X = j;
					Teleport_Bricks_Pos[index].Y = i;
					++index;
				}
			}
		}
	}

	//Advertisment = new AAdvertisment(6, 9, 2, 3); //!!!
}
//------------------------------------------------------------------------------------------------------------
bool AsLevel::Get_Falling_Letter(int &index, AFalling_Letter **falling_letter) const
{
	AFalling_Letter *current_letter;

	if (Falling_Letters_Count == 0)
		return false;

	if (index < 0 || index > AsConfig::Max_Falling_Letters_count)
		return false;

	while (index < AsConfig::Max_Falling_Letters_count)
	{
		current_letter = Falling_Letters[index++];

		if (current_letter != 0)
		{
			*falling_letter = current_letter;
			return true;
		}
	}

	return false;
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Stop()
{
	Need_To_Cansel_All = true;
}
//------------------------------------------------------------------------------------------------------------
bool AsLevel::Has_Brick_At(int level_x, int level_y)
{
	EBrick_Type brick_type;

	if (level_x < 0 || level_x > AsConfig::Level_Width - 1)
		return false;

	if (level_y < 0 || level_y > AsConfig::Level_Height - 1)
		return false;

	brick_type = (EBrick_Type)Level->Current_Level[level_y][level_x];

	if (brick_type == EBrick_Type::None)
		return false;
	else
		return true;
}
//------------------------------------------------------------------------------------------------------------
bool AsLevel::On_Hit(int brick_y, int brick_x, ABall *ball, bool vertical_hit)
{
	bool can_reflect = true;
	EBrick_Type brick_type;

	brick_type = (EBrick_Type)Current_Level[brick_y][brick_x];

	if (brick_type == EBrick_Type::Parashute)
	{
		ball->Set_On_Parashute(brick_y, brick_x);
		Current_Level[brick_y][brick_x] = (char)EBrick_Type::None;
		can_reflect = false;
	}
	else if (Add_Falling_Letter(brick_y, brick_x, brick_type) )
		Current_Level[brick_y][brick_x] = (char)EBrick_Type::None;
	else
		can_reflect = Create_Active_Brick(brick_y, brick_x, brick_type, ball, vertical_hit);

	Redraw_Brick(brick_y, brick_x);

	return can_reflect;
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::On_Hit(int level_y, int level_x)
{
	EBrick_Type brick_type;

	brick_type = (EBrick_Type)Current_Level[level_y][level_x];

	if (brick_type == EBrick_Type::Parashute)
		return;
	else if (Add_Falling_Letter(level_y, level_x, brick_type) )
		Current_Level[level_y][level_x] = (char)EBrick_Type::None;
	else
		Create_Active_Brick(level_y, level_x, brick_type, 0, 0);

	Redraw_Brick(level_y, level_x);
}
//------------------------------------------------------------------------------------------------------------
bool AsLevel::Create_Active_Brick(int brick_y, int brick_x, EBrick_Type brick_type, ABall *ball, bool vertical_hit)
{
	AActive_Brick *active_brick = 0;

	if (Active_Bricks_Count >= AsConfig::Max_Active_Bricks_Count)
		return true;

	switch (brick_type)
	{
	case EBrick_Type::None:
		return false;

	case EBrick_Type::Invisible:
		return true;

	case EBrick_Type::Red:
	case EBrick_Type::Blue:
		active_brick = new AActive_Brick_Fading(brick_type, brick_y, brick_x);
		Current_Level[brick_y][brick_x] = (char)EBrick_Type::None;
		break;

	case EBrick_Type::Unbreakable:
		active_brick = new AActive_Brick_Unbreakable(brick_y, brick_x);
		break;

	case EBrick_Type::Multihit_1:
		active_brick = new AActive_Brick_Multihit(brick_y, brick_x);
		Current_Level[brick_y][brick_x] = (char)EBrick_Type::None;
		break;

	case EBrick_Type::Multihit_2:
	case EBrick_Type::Multihit_3:
	case EBrick_Type::Multihit_4:
		Current_Level[brick_y][brick_x] = (int)brick_type - 1;
		return true;

	case EBrick_Type::Teleport:
		if (ball != 0)
			Add_Active_Brick_Teleport(brick_x, brick_y, ball, vertical_hit);
		return false;

	case EBrick_Type::Ad:
		active_brick = new AActive_Brick_Ad(brick_y, brick_x, Advertisment);
		Current_Level[brick_y][brick_x] = (char)EBrick_Type::Invisible;
		break;

	default:
		AsTools::Throw();
	}

	if (active_brick != 0)
		Add_New_Active_Brick(active_brick);

	return true;
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Add_Active_Brick_Teleport(int source_x, int source_y, ABall *ball, bool vertical_hit)
{
	int i;
	int dest_x, dest_y;
	double prev_x_pos, prev_y_pos;
	double post_x_pos, post_y_pos;
	bool got_direction = false;
	EDirection_Type direction;
	AActive_Brick_Teleport *sourse_teleport, *destination_teleport;

	ball->Get_Center(prev_x_pos, prev_y_pos);

	destination_teleport = Select_Destination_Teleport(source_y, source_x);
	sourse_teleport = new AActive_Brick_Teleport(source_y, source_x, ball, destination_teleport);

	destination_teleport->Get_Level_Pos(dest_x, dest_y);
	ball->Get_Center(post_x_pos, post_y_pos);

	if (vertical_hit)
	{
		if (prev_y_pos < post_y_pos)
			direction = EDirection_Type::Down;
		else
			direction = EDirection_Type::Up;
	}
	else
	{
		if (prev_x_pos < post_x_pos)
			direction = EDirection_Type::Right;
		else
			direction = EDirection_Type::Left;
	}

	for (i = 0; i < 4; i++)
	{
		switch (direction)
		{
		case EDirection_Type::Left:
			if (dest_x > 0 && Current_Level[dest_y][dest_x - 1] == 0)
				got_direction = true;
			break;

		case EDirection_Type::Right:
			if (dest_x < AsConfig::Level_Width - 1 && Current_Level[dest_y][dest_x + 1] == 0)
				got_direction = true;
			break;

		case EDirection_Type::Up:
			if (dest_y > 0 && Current_Level[dest_y - 1][dest_x] == 0)
				got_direction = true;
			break;

		case EDirection_Type::Down:
			if (dest_y < AsConfig::Level_Height - 1 && Current_Level[dest_y + 1][dest_x] == 0)
				got_direction = true;
			break;

		default:
			AsTools::Throw();
		}

		if (got_direction)
			break;

		direction = (EDirection_Type)( (int)direction - 1);
		ball->Set_Direction(ball->Get_Direction() + M_PI_2);

		if ( (int)direction < 0)
			direction = EDirection_Type::Down;
	}

	destination_teleport->Release_Direction = direction;

	Add_New_Active_Brick(sourse_teleport);
	Add_New_Active_Brick(destination_teleport);
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Add_New_Active_Brick(AActive_Brick *active_brick)
{
	int i;

	for (i = 0; i < AsConfig::Max_Active_Bricks_Count; i++)
	{
		if (Active_Bricks[i] == 0)
		{
			Active_Bricks[i] = active_brick;
			++Active_Bricks_Count;
			break;
		}
	}
}
//------------------------------------------------------------------------------------------------------------
bool AsLevel::Add_Falling_Letter(int brick_y, int brick_x, EBrick_Type brick_type)
{
	int i;
	AFalling_Letter *falling_letter;
	ELetter_Type letter_type;
	int letter_x, letter_y;

	if (Falling_Letters_Count >= AsConfig::Max_Falling_Letters_count)
		return false;

	if (brick_type == EBrick_Type::Red || brick_type == EBrick_Type::Blue)
	{
		if (AsTools::Rand(AsConfig::Hits_Per_Letter) == 0)
		{
			for (i = 0; i < AsConfig::Max_Falling_Letters_count; i++)
			{
				if (Falling_Letters[i] == 0)
				{
					letter_x = brick_x * AsConfig::Cell_Width + AsConfig::Bricks_Area_X_Offset;
					letter_y = brick_y * AsConfig::Cell_Height + AsConfig::Bricks_Area_Y_Offset;

					letter_type = AFalling_Letter::Get_Random_Letter_Type();
					switch (AsTools::Rand(3) )
					{
					case 0:
						letter_type = ELetter_Type::G;
						break;
					case 1:
						letter_type = ELetter_Type::L;
						break;
					case 2:
						letter_type = ELetter_Type::W;
						break;
					}

					letter_type = ELetter_Type::L;

					falling_letter = new AFalling_Letter(brick_type, letter_type, letter_x, letter_y);
					Falling_Letters[i] = falling_letter;
					++Falling_Letters_Count;
					break;
				}

			}

			return true;
		}
	}

	return false;
}
//------------------------------------------------------------------------------------------------------------
AActive_Brick_Teleport *AsLevel::Select_Destination_Teleport(int source_y, int source_x)
{
	int dest_index;

	if (Teleport_Bricks_Count < 2)
	{
		AsTools::Throw();
		return 0;
	}

	dest_index = AsTools::Rand(Teleport_Bricks_Count);

	if (Teleport_Bricks_Pos[dest_index].Y == source_y && Teleport_Bricks_Pos[dest_index].X == source_x)
	{
		++dest_index;

		if (dest_index >= Teleport_Bricks_Count)
			dest_index = 0;
	}


	return new AActive_Brick_Teleport(Teleport_Bricks_Pos[dest_index].Y, Teleport_Bricks_Pos[dest_index].X, 0, 0);
}
//------------------------------------------------------------------------------------------------------------
bool AsLevel::Check_Vertical_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, double &reflection_pos, ABall *ball)
{
	double direction = ball->Get_Direction();

	if (ball->Is_Move_Up())
	{
		if (Hit_Circle_On_Line(next_y_pos - Current_Brick_Low_Y, next_x_pos, Current_Brick_Left_X, Current_Brick_Right_X, ball->Radius, reflection_pos))
		{
			if (level_y < AsConfig::Level_Height - 1 && Current_Level[level_y + 1][level_x] == 0 /*&& level_x != AsConfig::Level_Width*/ ||
				level_y == AsConfig::Level_Height - 1)
				return true;
			else
				return false;
		}
	}
	else
	{
		if (Hit_Circle_On_Line(next_y_pos - Current_Brick_Top_Y, next_x_pos, Current_Brick_Left_X, Current_Brick_Right_X, ball->Radius, reflection_pos) )
		{
			if (level_y > 0 && level_y < AsConfig::Level_Height && Current_Level[level_y - 1][level_x] == 0)
				return true;
			else
				return false;
		}
	}

	return false;
}
//------------------------------------------------------------------------------------------------------------
bool AsLevel::Check_Horizontal_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, double &reflection_pos, ABall *ball)
{
	double direction = ball->Get_Direction();

	if (ball->Is_Move_Left())
	{
		if (Hit_Circle_On_Line(next_x_pos - Current_Brick_Right_X, next_y_pos, Current_Brick_Top_Y, Current_Brick_Low_Y, ball->Radius, reflection_pos))
		{
			if (level_x > 0 && level_x < AsConfig::Level_Width - 1 && Current_Level[level_y][level_x + 1] == 0/* && level_x != 0*/)
				return true;
			else
				return false;
		}
	}
	else
	{
		if (Hit_Circle_On_Line(Current_Brick_Left_X - next_x_pos, next_y_pos, Current_Brick_Top_Y, Current_Brick_Low_Y, ball->Radius, reflection_pos) )
		{
			if (level_x > 0 && level_x < AsConfig::Level_Width && Current_Level[level_y][level_x - 1] == 0)
				return true;
			else
				return false;
		}
	}

	return false;
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Draw_Brick(HDC hdc, RECT &brick_rect,int level_x, int level_y)
{
	EBrick_Type brick_type = (EBrick_Type)Current_Level[level_y][level_x];

	switch (brick_type)
	{
	case EBrick_Type::None:
		break;

	case EBrick_Type::Red:
	case EBrick_Type::Blue:
		AActive_Brick_Fading::Draw_In_Level(hdc, brick_rect, brick_type);
		break;

	case EBrick_Type::Unbreakable:
		AActive_Brick_Unbreakable::Draw_In_Level(hdc, brick_rect);
		break;

	case EBrick_Type::Multihit_1:
	case EBrick_Type::Multihit_2:
	case EBrick_Type::Multihit_3:
	case EBrick_Type::Multihit_4:
		AActive_Brick_Multihit::Draw_In_Level(hdc, brick_type, brick_rect);
		break;

	case EBrick_Type::Parashute:
		Draw_Parashute_Brick_In_Level(hdc, brick_rect);
		break;

	case EBrick_Type::Teleport:
		AActive_Brick_Teleport::Draw_In_Level(hdc, brick_rect);
		break;

	case EBrick_Type::Ad:
		AActive_Brick_Ad::Draw_In_Level(hdc, brick_rect);
		break;

	case EBrick_Type::Invisible:
		break;

	default:
		AsTools::Throw();
	}
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Draw_Parashute_Brick_In_Level(HDC hdc, RECT &brick_rect)
{
	Draw_Parashute_Segment(hdc, brick_rect, 0, 4);
	Draw_Parashute_Segment(hdc, brick_rect, 4, 6);
	Draw_Parashute_Segment(hdc, brick_rect, 10, 4);
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Draw_Parashute_Segment(HDC hdc, RECT &brick_rect, int x_offset, int segment_width)
{
	int scale = AsConfig::Global_Scale;
	RECT segment_rect{};

	Parashute_Brick_Color.Select(hdc);

	segment_rect.left = brick_rect.left + x_offset * scale + 1;
	segment_rect.top = brick_rect.top + 1;
	segment_rect.right = segment_rect.left + segment_width * scale + 1;
	segment_rect.bottom = segment_rect.top + 3 * scale;

	AsTools::Round_Rect(hdc, segment_rect);

	segment_rect.top += 3 * scale + 1;
	segment_rect.bottom = segment_rect.top + 3 * scale;

	AsTools::Round_Rect(hdc, segment_rect);
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Redraw_Brick(int brick_y, int brick_x)
{
	RECT brick_rect{};

	brick_rect.left = (AsConfig::Bricks_Area_X_Offset + brick_x * AsConfig::Cell_Width) * AsConfig::Global_Scale;
	brick_rect.top = (AsConfig::Bricks_Area_Y_Offset + brick_y * AsConfig::Cell_Height) * AsConfig::Global_Scale;
	brick_rect.right = brick_rect.left + AsConfig::Brick_Width * AsConfig::Global_Scale;
	brick_rect.bottom = brick_rect.top + AsConfig::Brick_Height * AsConfig::Global_Scale;

	AsTools::Invalidate_Rect(brick_rect);
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Act_Object(AGraphics_Object **object, const int max_objects_count, int &current_obj_count)
{
	int i;

	for (i = 0; i < max_objects_count; i++)
	{
		if (object[i] != 0)
		{
			object[i]->Act();

			if (object[i]->Is_Finished() )
			{
				delete object[i];
				object[i] = 0;
				--current_obj_count;
			}
		}
	}
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Draw_Object(HDC hdc, RECT &paint_area, AGraphics_Object **object, int objects_count)
{
	int i;

	for (i = 0; i < objects_count; i++)
	{
		if (object[i] != 0)
			object[i]->Draw(hdc, paint_area);
	}
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Clear_Object(HDC hdc, RECT &paint_area, AGraphics_Object **object, int objects_count)
{
	int i;

	for (i = 0; i < objects_count; i++)
	{
		if (object[i] != 0)
			object[i]->Clear(hdc, paint_area);
	}
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Delete_Object(AGraphics_Object **object, const int max_objects_count, int &current_obj_count)
{
	int i;

	for (i = 0; i < max_objects_count; i++)
	{
		if (object[i] != 0)
		{
			delete object[i];
			object[i] = 0;
		}
	}

	current_obj_count = 0;
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Cansel_All_Activity()
{	
	Delete_Object( (AGraphics_Object **)&Active_Bricks, AsConfig::Max_Active_Bricks_Count, Active_Bricks_Count);
	Delete_Object( (AGraphics_Object **)&Falling_Letters, AsConfig::Max_Falling_Letters_count, Falling_Letters_Count);
}
//------------------------------------------------------------------------------------------------------------
