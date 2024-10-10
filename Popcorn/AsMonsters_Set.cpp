#include "AsMonsters_Set.h"

// AsMonsters_Set
//------------------------------------------------------------------------------------------------------------
AsMonsters_Set::AsMonsters_Set()
: Monster_Set_State(EMonster_Set_State::Idle), Current_Gate(0),
  Max_Alive_Monsters_Count(0), Border(0)
{
}
//------------------------------------------------------------------------------------------------------------
bool AsMonsters_Set::Check_Hit(double next_x_pos, double next_y_pos, ABall_Object *ball)
{
	int i;

	for (i = 0; i < AsConfig::Max_Monsters_Count; i++)
		if (Monsters[i].Is_Active() )
			if (Monsters[i].Check_Hit(next_x_pos, next_y_pos, ball) )
				return true;

	return false;
}
//------------------------------------------------------------------------------------------------------------
bool AsMonsters_Set::Check_Hit(double next_x_pos, double next_y_pos)
{
	int i;

	for (i = 0; i < AsConfig::Max_Monsters_Count; i++)
		if (Monsters[i].Is_Active() )
			if (Monsters[i].Check_Hit(next_x_pos, next_y_pos) )
				return true;

	return false;
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