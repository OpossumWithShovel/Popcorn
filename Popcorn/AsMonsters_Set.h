#pragma once

#include "Border.h"
#include "Monster.h"

enum class EMonster_Set_State: unsigned char
{
	Idle,
	Select_Next_Gate,
	Waitinig_For_Open_Gate,
	Waitinig_For_Close_Gate
};
//------------------------------------------------------------------------------------------------------------
class AsMonsters_Set: public AGame_Objects_Set
{
public:
	AsMonsters_Set();

	virtual void Act();

	void Init(AsBorder *border);
	void Activate(int max_alive_monsters_count);
	void Let_Out(int gate_index);

private:
	virtual bool Get_Next_Obj(int &index, AGame_Object **game_obj);

	EMonster_Set_State Monster_Set_State;

	int Current_Gate;
	int Max_Alive_Monsters_Count;

	AsBorder *Border;  // UNO

	AMonster Monsters[AsConfig::Max_Monsters_Count];
};
//------------------------------------------------------------------------------------------------------------
