#pragma once
#include "Tab.h"

extern short ObsNum;
extern short Obstacles_L;
extern short Obstacles_H;
extern bool OBS[4][4];

void DrawMap(short ConsoleWidth, short ConsoleHeight, short UpBorderColor, short DownBorderColor, short LeftBorderColor, short RightBorderColor, short TopLeftCoord, short TopUpCoord, short X, short Y);

void NotifySaves(short ConsoleHeight);

void KeysInterface(string str, short& ConsoleWidth, short& ConsoleHeight);

void ColorInterface(string str, short& ConsoleWidth, short& ConsoleHeight);

void SpeedInterface(string str, short& ConsoleWidth, short& ConsoleHeight);

void ObstaclesEditorInterface(string str, short& ConsoleWidth, short& ConsoleHeight);

void RandomGeneratorInterface(string str, short& ConsoleWidth, short& ConsoleHeight);

void ObstaclesListInterface(string str, short& ConsoleWidth, short& ConsoleHeight);

void HorizontalAlignmentInterface(string str, short& ConsoleWidth, short& ConsoleHeight);

void VerticalAlignmentInterface(string str, short& ConsoleWidth, short& ConsoleHeight);

void MapSizeInterface(string str, short& ConsoleWidth, short& ConsoleHeight);

void MapBorderInterface(string str, short& ConsoleWidth, short& ConsoleHeight);

void QuickSetupInterface(string str, short& ConsoleWidth, short& ConsoleHeight);