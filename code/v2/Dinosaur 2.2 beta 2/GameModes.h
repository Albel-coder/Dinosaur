#pragma once
#include "Interface.h"

class FullDetailedDinosaur
{
public:
	FullDetailedDinosaur(string Quality);

	void Draw();

	~FullDetailedDinosaur();

private:

	short width = 41; short height = 50;
	char* Dino = new char[width * height + 1];	

	string numbers[10] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };
	
	unsigned short Position = 0;
	unsigned long long X = 0;
	char EmptyTile = ' ';
	char Tile = 219;
	int Num = 0;
	string path;

	enum eDirection { STOP, UP, DOWN };
	eDirection dir;
	
	void OutputPoints();

	void DrawLegs();

	void DrawCactus();

	void Jump();

	void DayAndNightSwitch();

	void Input();	
};