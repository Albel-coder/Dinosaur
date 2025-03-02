#pragma once
#include <Windows.h>

struct Console
{
	unsigned char MinConsoleWidth, MinConsoleHeight;

	Console();

	void GetBuffer();

	short& GetConsoleWidth();

	short& GetConsoleHeight();

	bool IsChanged();

	bool Acceptable();

	void ConsoleCursorVisible(bool show, short size);

private:

	short ConsoleWidth, ConsoleHeight, CharsX, CharsY;

	CONSOLE_SCREEN_BUFFER_INFO csbInfo;
};