#include "Console.h"

void Console::GetBuffer()
{
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbInfo);
	ConsoleWidth = csbInfo.srWindow.Right - csbInfo.srWindow.Left + 1;
	ConsoleHeight = csbInfo.srWindow.Bottom - csbInfo.srWindow.Top + 1;
}

Console::Console()
{
	GetBuffer();
	CharsX = ConsoleWidth;
	CharsY = ConsoleHeight;
}

bool Console::IsChanged()
{
	if (ConsoleWidth != CharsX || ConsoleHeight != CharsY)
	{
		CharsX = ConsoleWidth;
		CharsY = ConsoleHeight;
		return true;
	}
	else
	{
		return false;
	}
}

bool Console::Acceptable()
{
	return (ConsoleWidth < MinConsoleWidth || ConsoleHeight < MinConsoleHeight) ? false : true;
}

void Console::ConsoleCursorVisible(bool show, short size)
{
	CONSOLE_CURSOR_INFO structCursorInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &structCursorInfo);
	structCursorInfo.bVisible = show;
	structCursorInfo.dwSize = size;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &structCursorInfo);
}

short& Console::GetConsoleWidth()
{
	return ConsoleWidth;
}

short& Console::GetConsoleHeight()
{
	return ConsoleHeight;
}