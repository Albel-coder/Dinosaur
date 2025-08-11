#pragma once
#include <iostream>
#include <windows.h>
#include <vector>
#include <fstream>
#include <conio.h>
#include <string>
#include <memory>

extern HANDLE hConsole;
extern COORD cPos;
extern std::vector<std::vector<std::string>> Cactus;
extern std::string DinoLeg1Sprite;
extern std::string DinoLeg2Sprite;
extern std::string DinoSprite;

class ScoreDisplay
{
	std::vector<std::vector<std::string>> DigitSprites = std::vector<std::vector<std::string>>(10);

public:
	ScoreDisplay();

	void DrawScore(uint64_t Score, uint8_t ConsoleWidth);
};

class DayAndNightSwitch
{
public:
	void Switch(uint64_t Score);
};

class Ground
{
	uint8_t GroundCoords[2][30];

public:
	Ground(uint8_t ConsoleWidth);

	void DrawGround(uint64_t Score, uint8_t ConsoleWidth, uint8_t ConsoleHeight);
};

class Skyline
{
	std::string Line;
public:
	Skyline(uint8_t ConsoleWidth);

	void DrawSkyline(uint8_t ConsoleHeight);
};

void LoadSprites();

void DrawObs(int X, int ConsoleHeight, std::vector<std::string> Sprite);

void JumpingDino(int Y);

void DrawRunningDino(uint64_t points, uint8_t ConsoleHeight);