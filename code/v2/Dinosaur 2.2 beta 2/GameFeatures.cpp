#include "GameFeatures.h"

GameFeatures::GameFeatures()
{
	ifstream in("Saves.txt");
	if (in.is_open())
	{
		for (int i = 0; i < 16; i++)
		{
			in >> GameVariables[i];
		}

		in.close();
	}
	else
	{
		GameVariables[0] = 40;
		GameVariables[1] = 7;
		GameVariables[2] = 15;
		GameVariables[3] = 15;
		GameVariables[4] = 15;
		GameVariables[5] = 15;
		GameVariables[6] = 15;
		GameVariables[7] = 15;
		GameVariables[8] = 15;
		GameVariables[9] = 0;
		GameVariables[10] = 0;
		GameVariables[11] = 0;
		GameVariables[12] = 'w';
		GameVariables[13] = 'a';
		GameVariables[14] = 's';
		GameVariables[15] = 'd';
	}
}

unsigned char& GameFeatures::operator[](short Index)
{
	return GameVariables[Index];
}

void GameFeatures::Save()
{
	ofstream out("Saves.txt");
	if (out.is_open())
	{
		for (int i = 0; i < 16; i++)
		{
			out << GameVariables[i] << "\n";
		}
		out.close();
	}
}

GameFeatures::~GameFeatures()
{
	Save();
}

unsigned char& GameFeatures::GetMapWidth()
{
	return GameVariables[0];
}

unsigned char& GameFeatures::GetMapHeight()
{
	return GameVariables[1];
}

unsigned char& GameFeatures::GetUpMapBorder()
{
	return GameVariables[2];
}

unsigned char& GameFeatures::GetDownMapBorder()
{
	return GameVariables[3];
}

unsigned char& GameFeatures::GetLeftMapBorder()
{
	return GameVariables[4];
}

unsigned char& GameFeatures::GetRightMapBorder()
{
	return GameVariables[5];
}

unsigned char& GameFeatures::GetObstaclesColor()
{
	return GameVariables[6];
}

unsigned char& GameFeatures::GetPlayerColor()
{
	return GameVariables[7];
}

unsigned char& GameFeatures::GetPointsColor()
{
	return GameVariables[8];
}

unsigned char& GameFeatures::GetHorizontalAlignment()
{
	return GameVariables[9];
}

unsigned char& GameFeatures::GetVerticalAlignment()
{
	return GameVariables[10];
}

unsigned char& GameFeatures::GetSpeed()
{
	return GameVariables[11];
}

unsigned char& GameFeatures::GetMoveUp()
{
	return GameVariables[12];
}

unsigned char& GameFeatures::GetMoveDown()
{
	return GameVariables[13];
}

unsigned char& GameFeatures::GetMoveLeft()
{
	return GameVariables[14];
}

unsigned char& GameFeatures::GetMoveRight()
{
	return GameVariables[15];
}