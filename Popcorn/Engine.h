#pragma once

#include "Ball_Set.h"
#include "Level.h"
#include "Platform.h"
#include "Border.h"

//-----------------------------------------------------------------------------------------------------------
enum class EGame_State: unsigned char
{
	Test_Ball,

	Play_Level,
	Lost_Ball,
	Restart_Level
};
//-----------------------------------------------------------------------------------------------------------
enum class EKey_Type: unsigned char
{
	Left,
	Right,
	Space
};
//-----------------------------------------------------------------------------------------------------------
class AsEngine
{
public:
	AsEngine();

	void Init_Engine(HWND hwnd);
	void Draw_Frame(HDC hdc, RECT &paint_area);
	int On_Key(EKey_Type key_type, bool key_down);
	int On_Timer();

	static const int Timer_ID = WM_USER + 1;

private:
	void Act();
	void Restart_Level();
	void Play_Level();
	void Advance_Movers();
	void On_Falling_Letter(AFalling_Letter *falling_letter);
	void Add_Next_Module(int &index, AGame_Object *game_obj);

	EGame_State Game_State;

	int Life_Count;

	double Rest_Distance;

	AsBall_Set Ball_Set;
	AsPlatform Platform;
	AsLevel Level;
	AsBorder Border;
	ALaser_Beams_Set Laser_Beams_Set;

	AGame_Object *Modules[AsConfig::Max_Modules_Count];

	//AMover *Movers[AsConfig::Max_Movers_Count];
	//AGraphics_Object *Modules[AsConfig::Max_Modules_Count];
};
//-----------------------------------------------------------------------------------------------------------
