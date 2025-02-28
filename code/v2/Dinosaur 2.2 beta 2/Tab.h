#pragma once
#include <iostream>
#include <Windows.h>
#include <thread>
#include <conio.h>
#include <fstream>
#include <string>

#include "Console.h"
#include "GameFeatures.h"

//#define DOWNLOAD_NOT_RELEASED
//#define DEBUG

/* Клавиши */
#define ENTER 13
#define ESC 27
#define BACKSPACE 8
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

extern char Key;
extern char tmp;
extern char ModifiedInterface;
extern short SelectedFunction;
extern unsigned long long Points;
extern unsigned int Value;
extern Console ConS;
extern COORD cPos;
extern HANDLE hConsole;
extern GameFeatures Variables;

using namespace std;

class Tab
{
	friend struct Interface;
		
public:
	
	Tab(string str);

	void Entrance(void (*DrawInfo)(string Path, short& ConsoleWidth, short& ConsoleHeight));
	
#ifdef DOWNLOAD_NOT_RELEASED
	bool CreateFiles();
#endif // DOWNLOAD_NOT_RELEASED

	static int Conversion(short a, unsigned long long b);

	string GetPath();
	
private:

	bool Select(bool ChangeValue, void (*DrawInfo)(string Path, short& ConsoleWidth, short& ConsoleHeight));

	void Draw(short& ConsoleWidth, short& ConsoleHeight);

	void WaitForValidSize();
		
	string Path;
	
	bool Input(char& tmp);		
};

void ClearAndShowInfo(string Path, short& ConsoleWidth, short& ConsoleHeight);