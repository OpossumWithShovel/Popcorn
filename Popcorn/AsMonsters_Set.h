#pragma once

#include "Border.h"
#include "Monster.h"

enum class EMonster_Set_State: unsigned char
{
	Idle,
	Select_Next_Gate,
	Waitinig_For_Open_Gate,
	Waitinig_For_Close_Gate,
	Waiting_For_Destroy_All
};
//------------------------------------------------------------------------------------------------------------
class AsMonsters_Set: public AHit_Checker, public AGame_Objects_Set
{
public:
	AsMonsters_Set();

	virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall_Object *ball);
	virtual bool Check_Hit(double next_x_pos, double next_y_pos);
	virtual bool Check_Hit(RECT &rect);

	virtual void Act();

	void Init(AsBorder *border);
	void Activate(int max_alive_monsters_count);
	void Let_Out(int gate_index);
	void Destroy_All();

private:
	virtual bool Get_Next_Obj(int &index, AGame_Object **game_obj);

	EMonster_Set_State Monster_Set_State;

	int Current_Gate;
	int Max_Alive_Monsters_Count;

	AsBorder *Border;  // UNO

	AMonster Monsters[AsConfig::Max_Monsters_Count];
};
//------------------------------------------------------------------------------------------------------------
