#include <iostream>
#include <thread>
#include <conio.h>
#include "Console.h"

COORD cPos;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

Console console;

using namespace std;

float Position = 0;
float Multi = 0;
uint64_t Points = 0;
uint8_t X = 80;
uint8_t Y = 10;
bool Jump = false;
bool GameOver = false;

static void DrawMapBorders(uint8_t ConsoleWidth, uint8_t ConsoleHeight, uint8_t X, uint8_t Y, uint8_t TopLeftCoord = 0, uint8_t TopUpCoord = 0, uint8_t RightBorderColor = 15, uint8_t LeftBorderColor = 15, uint8_t UpBorderColor = 15, uint8_t DownBorderColor = 15)
{    
    if (X > ConsoleWidth - TopLeftCoord - 2) X = ConsoleWidth - TopLeftCoord - 1;
    else
    {
        cPos.X = X + TopLeftCoord + 1; cPos.Y = TopUpCoord + 1;
        SetConsoleTextAttribute(hConsole, RightBorderColor);
        for (int i = 0; i < Y + 1; i++, cPos.Y++)
        {
            SetConsoleCursorPosition(hConsole, cPos);
            std::cout << "#";
        }
    }
    cPos.X = TopLeftCoord; cPos.Y = TopUpCoord;
    SetConsoleCursorPosition(hConsole, cPos);
    SetConsoleTextAttribute(hConsole, UpBorderColor);
    for (int i = 0; i < X + 2; i++)
    {
        std::cout << "#";
    }
    cPos.Y = TopUpCoord + 1;
    SetConsoleTextAttribute(hConsole, LeftBorderColor);
    if (Y > ConsoleHeight - TopUpCoord) Y = ConsoleHeight - TopUpCoord - 1;
    for (int i = 0; i < Y; i++, cPos.Y++)
    {
        SetConsoleCursorPosition(hConsole, cPos);
        std::cout << "#";
    }
    SetConsoleCursorPosition(hConsole, cPos);
    SetConsoleTextAttribute(hConsole, DownBorderColor);    
    for (int i = 0; i < X + 1; i++)
    {
        std::cout << "#";
    }
}

void Input()
{
    if (_kbhit())
    {
        char key = _getch();
        switch (key)
        {
        case 'w':
            if (Position == 0) Jump = true;
            break;
        }
    }
}

static inline bool Conversion(uint16_t a, uint64_t b)
{
    for (int i = 0; i < a - 1; i++)
        b /= 2;
    return b % 2;
}

void JumpingAbility()
{
    if (Jump)
    {
        if (Position < 5)
        {
            Position += Multi;
            Multi /= 1.1;
        }
        else Jump = false;
    }
    else
    {
        if (Position > 0)
        {
            Position -= Multi;
            Multi *= 1.1;
        }
        else
        {
            Multi = 1;
            Position = 0;
        }
    }
}

uint8_t Value = 0;

void DrawGameAttributes()
{
    cPos.X = 65; cPos.Y = 2;
    SetConsoleCursorPosition(hConsole, cPos);
    SetConsoleTextAttribute(hConsole, 15);
    if (Points % 10) std::cout << "Points: " << Points / 10;

    //PLAYER

    cPos.X = 6; cPos.Y = 9 - Position;
    for (int i = 0; i < 2; i++, cPos.Y++)
    {
        SetConsoleCursorPosition(hConsole, cPos);
        std::cout << "*";
    }
    if (Jump)
        cPos.Y = 11 - Position;
    else
        cPos.Y = 8 - Position;
    SetConsoleCursorPosition(hConsole, cPos);
    std::cout << " ";


    /// OBS

    cPos.X = X - Value - 2; cPos.Y = 10;
    SetConsoleCursorPosition(hConsole, cPos);
    for (int i = 4; i > 0; i--)
    {
        std::cout << "$  ";
        cPos.Y--;
        SetConsoleCursorPosition(hConsole, cPos);
    }
    if (cPos.X == 6 && Position < 3)
        GameOver = true;     

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    //// CLEAR

    if (Value > X - 5)
    {
        cPos.Y = 10;
        SetConsoleCursorPosition(hConsole, cPos);
        for (int i = 4; i > 0; i--)
        {
            std::cout << " ";
            cPos.Y--;
            SetConsoleCursorPosition(hConsole, cPos);
        }
        Value = 0;
    }

    Value += 2;
    Points++;
}

int main()
{    
    console.ConsoleCursorVisible(false, 100);
    srand(time(NULL));

    DrawMapBorders(console.GetConsoleWidth(), console.GetConsoleWidth(), X, Y);

    while (!GameOver)
    {
        Input();
        JumpingAbility();
        DrawGameAttributes();
    }
    cPos.X = 35; cPos.Y = 5;
    SetConsoleCursorPosition(hConsole, cPos);
    SetConsoleTextAttribute(hConsole, 1);
    cout << "GAME OVER!";

    cin >> Points;
    return 0;
}