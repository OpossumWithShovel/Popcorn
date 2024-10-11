#include "AsMonsters_Set.h"

// AsMonsters_Set
//------------------------------------------------------------------------------------------------------------
AsMonsters_Set::AsMonsters_Set()
: Monster_Set_State(EMonster_Set_State::Idle), Current_Gate(0),
  Max_Alive_Monsters_Count(0), Border(0)
{
	memset(Monsters, 0, sizeof (AMonster *) * AsConfig::Max_Monsters_Count);
}
//------------------------------------------------------------------------------------------------------------
bool AsMonsters_Set::Check_Hit(double next_x_pos, double next_y_pos, ABall_Object *ball)
{
	int i;

	AMonster *curr_monster;

	for (i = 0; i < AsConfig::Max_Monsters_Count; i++)
	{
		curr_monster = Monsters[i];

		if (curr_monster != 0)
			if (curr_monster->Check_Hit(next_x_pos, next_y_pos, ball) )
				return true;
	}

	return false;
}
//------------------------------------------------------------------------------------------------------------
bool AsMonsters_Set::Check_Hit(double next_x_pos, double next_y_pos)
{
	int i;

	AMonster *curr_monster;

	for (i = 0; i < AsConfig::Max_Monsters_Count; i++)
	{
		curr_monster = Monsters[i];

		if (curr_monster != 0)
			if (curr_monster->Check_Hit(next_x_pos, next_y_pos) )
				return true;
	}

	return false;
}
//------------------------------------------------------------------------------------------------------------
bool AsMonsters_Set::Check_Hit(RECT &rect)
{
	int i;

	AMonster *curr_monster;

	for (i = 0; i < AsConfig::Max_Monsters_Count; i++)
	{
		curr_monster = Monsters[i];

		if (curr_monster != 0)
			if (curr_monster->Check_Hit(rect) )
				return true;
	}

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
			if (Monsters[i] != 0 && ! Monsters[i]->Is_Finished() )
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

	if (Monster_Set_State != EMonster_Set_State::Idle)
	{
		for (i = 0; i < AsConfig::Max_Monsters_Count; i++)
			if (Monsters[i] != 0 && Monsters[i]->Is_Finished() )
			{
				delete Monsters[i];
				Monsters[i] = 0;
			}
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
		curr_monster = Monsters[i];

		if (curr_monster == 0)
		{
			if (AsTools::Rand(2) == 0)
				curr_monster = new AMonster_Eye();
			else
				curr_monster = new AMonster_Comet();

			Monsters[i] = curr_monster;
			break;
		}
	}

	if (gate_index % 2 == 0)
		is_gate_left = true;
	else
		is_gate_left = false;

	Border->Get_Gate_Pos(gate_index, gate_x_pos, gate_y_pos);

	curr_monster->Let_Out(gate_x_pos, gate_y_pos, is_gate_left);
}
//------------------------------------------------------------------------------------------------------------
void AsMonsters_Set::Destroy_All()
{
	int i;

	for (i = 0; i < AsConfig::Max_Monsters_Count; i++)
		if (Monsters[i] != 0)
			Monsters[i]->Destroy();

	Monster_Set_State = EMonster_Set_State::Idle;
}
//------------------------------------------------------------------------------------------------------------
bool AsMonsters_Set::Get_Next_Obj(int &index, AGame_Object **game_obj)
{
	AMonster *curr_monster = 0;

	if (index < 0 || index >= AsConfig::Max_Monsters_Count)
		return false;

	while (index < AsConfig::Max_Monsters_Count)
	{
		curr_monster = Monsters[index++];

		if (curr_monster != 0)
		{
			*game_obj = curr_monster;
			return true;
		}
	}

	return false;
}
//------------------------------------------------------------------------------------------------------------