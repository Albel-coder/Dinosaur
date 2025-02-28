#pragma once
#include <fstream>

using namespace std;

struct GameFeatures
{
	GameFeatures();

	unsigned char GameVariables[16];

	unsigned char& operator[](short Index);	

	void Save();

	~GameFeatures();

	unsigned char& GetMoveUp();

	unsigned char& GetMoveDown();

	unsigned char& GetMoveLeft();

	unsigned char& GetMoveRight();

	unsigned char& GetMapWidth();

	unsigned char& GetMapHeight();

	unsigned char& GetUpMapBorder();

	unsigned char& GetDownMapBorder();

	unsigned char& GetLeftMapBorder();

	unsigned char& GetRightMapBorder();

	unsigned char& GetObstaclesColor();

	unsigned char& GetPlayerColor();

	unsigned char& GetPointsColor();

	unsigned char& GetHorizontalAlignment();

	unsigned char& GetVerticalAlignment();

	unsigned char& GetSpeed();	
};