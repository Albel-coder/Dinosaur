#pragma once
#include "DetailedMode.h"

extern float Pos;
extern float Multi;
extern uint64_t point;
extern uint8_t Value;
extern bool Jump;
extern char Key;
extern uint8_t MapWidth;
extern uint8_t MapHeight;

static void DrawMapBorders(uint8_t ConsoleWidth, 
	uint8_t ConsoleHeight, 
	uint8_t X, 
	uint8_t Y, 
	uint8_t TopLeftCoord = 0, 
	uint8_t TopUpCoord = 0, 
	uint8_t RightBorderColor = 15,
	uint8_t LeftBorderColor = 15, 
	uint8_t UpBorderColor = 15, 
	uint8_t DownBorderColor = 15);

void InputChar();

void JumpingAbility();

void DrawGameAttributes();

void Start(uint8_t ConsoleHeight, uint8_t ConsoleWidth);