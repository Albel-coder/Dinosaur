#include "Console.h"

Console::Console()
{
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    UpdateSize();
}

HANDLE Console::GetConsoleHandle() noexcept
{
    return hConsole;
}

void Console::SetMinimumSize(short MinWidth, short MinHeight) noexcept
{
    MinWidth_ = MinWidth;
    MinHeight_ = MinHeight;
}

void Console::UpdateSize() noexcept
{
    CONSOLE_SCREEN_BUFFER_INFO csbInfo;
    if (GetConsoleScreenBufferInfo(GetConsoleHandle(), &csbInfo))
    {
        CurrentWidth = csbInfo.srWindow.Right - csbInfo.srWindow.Left + 1;
        CurrentHeight = csbInfo.srWindow.Bottom - csbInfo.srWindow.Top + 1;
    }
}

bool Console::IsSizeChanged() noexcept
{
    const short LastWidth = CurrentWidth;
    const short LastHeight = CurrentHeight;

    UpdateSize();

    return (LastWidth != CurrentWidth || LastHeight != CurrentHeight);
}

bool Console::IsSizeAcceptable() const noexcept
{
    return CurrentWidth >= MinWidth_ && CurrentHeight >= MinHeight_;
}

void Console::SetCursorVisible(bool show, short size) noexcept
{
    if (!hConsole || hConsole == INVALID_HANDLE_VALUE) return;

    CONSOLE_CURSOR_INFO cursorInfo;
    if (GetConsoleCursorInfo(hConsole, &cursorInfo))
    {
        cursorInfo.bVisible = show;
        cursorInfo.dwSize = size;
        SetConsoleCursorInfo(GetConsoleHandle(), &cursorInfo);
    }
}

short Console::GetWidth() const noexcept
{
    return CurrentWidth;
}

short Console::GetHeight() const noexcept
{
    return CurrentHeight;
}