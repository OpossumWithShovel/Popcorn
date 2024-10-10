#include "Engine.h"

// AsEngine
//------------------------------------------------------------------------------------------------------------
AsEngine::AsEngine()
	: Game_State(EGame_State::Lost_Ball), Life_Count(5), Rest_Distance(0.0), Modules{}
{
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Init_Engine(HWND hwnd)
{
	int index = 0;

	SYSTEMTIME sys_time;
	FILETIME file_time;

	GetSystemTime(&sys_time);
	SystemTimeToFileTime(&sys_time, &file_time);

	srand(file_time.dwLowDateTime);

	AsConfig::Hwnd = hwnd;

	AFalling_Letter::Init();

	AActive_Brick_Fading::Setup_Colors();
	AExplosive_Ball::Setup_Colors();

	Level.Init();
	Platform.Init(&Ball_Set, &Laser_Beams_Set);
	Monsters_Set.Init(&Border);

	ABall::Hit_Checker_List.Add_Hit_Checker(&Level);
	ABall::Hit_Checker_List.Add_Hit_Checker(&Border);
	ABall::Hit_Checker_List.Add_Hit_Checker(&Platform);
	ABall::Hit_Checker_List.Add_Hit_Checker(&Monsters_Set);

	ALaser_Beam::Hit_Checker_List.Add_Hit_Checker(&Level);
	ALaser_Beam::Hit_Checker_List.Add_Hit_Checker(&Monsters_Set);

	Level.Set_Current_Level(AsLevel::Test_Level);

	//Ball.Set_State(EBall_State::On_Platform, (double)(Platform.X_Pos + Platform.Width / 2) );
	//Platform.Set_State(EPlatform_State::Laser);

	Platform.Redraw();

	SetTimer(AsConfig::Hwnd, Timer_ID, 1000 / AsConfig::FPS, 0);

	memset(Modules, 0, sizeof(Modules) );

	Add_Next_Module(index, &Level);
	Add_Next_Module(index, &Border);
	Add_Next_Module(index, &Platform);
	Add_Next_Module(index, &Ball_Set);
	Add_Next_Module(index, &Laser_Beams_Set);
	Add_Next_Module(index, &Monsters_Set);

	//Modules[0] = &Level;
	//Modules[1] = &Border;
	//Modules[2] = &Platform;
	//Modules[3] = &Ball_Set;
	//Modules[4] = &Laser_Beams_Set;
}
//-------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Frame(HDC hdc, RECT &paint_area)
{
	int i;

	SetGraphicsMode(hdc, GM_ADVANCED);

	for (i = 0; i < AsConfig::Max_Modules_Count; i++)
		if (Modules[i] != 0)
			Modules[i]->Clear(hdc, paint_area);

	for (i = 0; i < AsConfig::Max_Modules_Count; i++)
		if (Modules[i] != 0)
			Modules[i]->Draw(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------
int AsEngine::On_Key(EKey_Type key_type, bool key_down)
{
	switch (key_type)
	{
	case EKey_Type::Left:
		Platform.Move(true, key_down);
		break;


	case EKey_Type::Right:
		Platform.Move(false, key_down);
		break;


	case EKey_Type::Space:
		Platform.On_Space_Key(key_down);
		break;


	default:
		break;
	}

	return 0;
}
//------------------------------------------------------------------------------------------------------------
int AsEngine::On_Timer()
{
	++AsConfig::Current_Timer_Tick;

	switch (Game_State)
	{
	case EGame_State::Test_Ball:
		Ball_Set.Set_For_Test();
		Game_State = EGame_State::Play_Level;
		break;


	case EGame_State::Play_Level:
		Play_Level();
		break;


	case EGame_State::Lost_Ball:
		if (Platform.Has_State(EPlatform_Substate_Regular::Missing) )
			Restart_Level();
		break;


	case EGame_State::Restart_Level:
		if (Platform.Has_State(EPlatform_Substate_Regular::Ready) )
		{
			Ball_Set.Set_On_Platform(Platform.Get_Middle_Pos());
			Game_State = EGame_State::Play_Level;
			Monsters_Set.Activate(7);
		}
		break;


	default:
		break;
	}

	Act();

	//if (AsConfig::Current_Timer_Tick % 10 == 0)

	return 0;
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Act()
{
	int i;
	int index = 0;
	AFalling_Letter *falling_letter;

	for (i = 0; i < AsConfig::Max_Modules_Count; i++)
		if (Modules[i] != 0)
			Modules[i]->Act();

	while (Level.Get_Falling_Letter(index, &falling_letter))
	{
		if (Platform.Across_By(falling_letter) )
			On_Falling_Letter(falling_letter);
	}

	if (Game_State == EGame_State::Restart_Level)
		if (Border.Is_Gate_Open(7) )
			Platform.Set_State(EPlatform_State::Rolling);
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Play_Level()
{
	Advance_Movers();

	if (Ball_Set.Are_All_Balls_Lost() )
	{			
		Game_State = EGame_State::Lost_Ball;
		Level.Stop();
		Platform.Set_State(EPlatform_State::Meltdown);
	}
	else
		Ball_Set.Accelerate();

	if (Ball_Set.Is_Test_Finished() )
		Game_State = EGame_State::Test_Ball;
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Restart_Level()
{
	Game_State = EGame_State::Restart_Level;
	Border.Open_Gate(7, true);
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Advance_Movers()
{
	int i;
	double max_speed = 0.0;
	double curr_speed;

	for (i = 0; i < AsConfig::Max_Movers_Count; i++)
	{
		if (Modules[i] != 0)
		{
			Modules[i]->Begin_Movement();

			curr_speed = fabs(Modules[i]->Get_Speed() );

			if (curr_speed > max_speed)
				max_speed = curr_speed;
		}
	}

	Rest_Distance += max_speed;

	while (Rest_Distance > 0)
	{
		for (i = 0; i < AsConfig::Max_Movers_Count; i++)
		{
			if (Modules[i] != 0)
			{
				Modules[i]->Advance(max_speed);

				if (Modules[i] == &Platform && Platform.Get_State() == EPlatform_State::Glue) 
					if ( ! (Platform.Get_Moving_State() == EPlatform_Moving_State::Stop || Platform.Get_Moving_State() == EPlatform_Moving_State::Stopping) )
						Ball_Set.Forced_Advance(Platform.Get_X_Offset() );
			}
		}

		Rest_Distance -= AsConfig::Moving_Step_Size;
	}

	for (i = 0; i < AsConfig::Max_Movers_Count; i++)
		if (Modules[i] != 0)
			Modules[i]->Finish_Movement();
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::On_Falling_Letter(AFalling_Letter *falling_letter)
{
	switch (falling_letter->Letter_Type)
	{
	case ELetter_Type::C: // cansel action of letters
		Platform.Set_State(EPlatform_Substate_Regular::Normal);
		break;

	case ELetter_Type::I: // inversion
		Ball_Set.Inverse_Direction_Balls();
		Platform.Set_State(EPlatform_Substate_Regular::Normal);
		break;

	case ELetter_Type::V: // velosity
		Ball_Set.Reset_Speed();
		Platform.Set_State(EPlatform_Substate_Regular::Normal);
		break;

		//case ELetter_Type::M: // monsters

	case ELetter_Type::Plus: // life
		if (Life_Count < AsConfig::Max_Life_Count)
			++Life_Count;

		Platform.Set_State(EPlatform_Substate_Regular::Normal);
		break;

	case ELetter_Type::G: // glue
		Platform.Set_State(EPlatform_State::Glue);
		break;

	case ELetter_Type::W: // width
		Platform.Set_State(EPlatform_State::Expanding);
		break;

	case ELetter_Type::F: // floor
		AsConfig::Level_Has_Floor = true;
		Platform.Set_State(EPlatform_Substate_Regular::Normal);
		Border.Redraw_Floor();
		break;

	case ELetter_Type::L: // laser
		Platform.Set_State(EPlatform_State::Laser);
		break;

	case ELetter_Type::T: // three balls
		Platform.Set_State(EPlatform_Substate_Regular::Normal);
		Ball_Set.Triple_Balls();
		break;

		//case ELetter_Type::N: // next level

	default:
		AsTools::Throw();
	}

	falling_letter->Destroy();
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Add_Next_Module(int &index, AGame_Object *game_obj)
{
	if (index >= 0 && index < AsConfig::Max_Laser_Beams_Count)
		Modules[index++] = game_obj;
	else
		AsTools::Throw();
}
//------------------------------------------------------------------------------------------------------------
