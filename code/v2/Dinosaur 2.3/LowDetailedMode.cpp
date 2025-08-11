#include "LowDetailedMode.h"

void DrawMapBorders(uint8_t ConsoleWidth, uint8_t ConsoleHeight, uint8_t X, uint8_t Y, uint8_t TopLeftCoord, uint8_t TopUpCoord, uint8_t RightBorderColor, uint8_t LeftBorderColor, uint8_t UpBorderColor, uint8_t DownBorderColor)
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

void InputChar()
{
    if (_kbhit())
    {
        char key = _getch();
        if (key == 'w' || key == 'W')
        {
            if (Pos == 0) {
                Jump = true;
            }
        }
    }
}

void JumpingAbility()
{
    if (Jump)
    {
        if (Pos < 5)
        {
            Pos += Multi;
            Multi /= 1.1;
        }
        else Jump = false;
    }
    else
    {
        if (Pos > 0)
        {
            Pos -= Multi;
            Multi *= 1.1;
        }
        else
        {
            Multi = 1;
            Pos = 0;
        }
    }
}

void DrawGameAttributes()
{
    cPos.X = MapWidth - 15; cPos.Y = 2;
    SetConsoleCursorPosition(hConsole, cPos);
    SetConsoleTextAttribute(hConsole, 15);
    if (point % 10) std::cout << "Points: " << point / 10;

    //PLAYER

    cPos.X = 6; cPos.Y = MapHeight - 1 - Pos;
    for (int i = 0; i < 2; i++, cPos.Y++)
    {
        SetConsoleCursorPosition(hConsole, cPos);
        std::cout << "*";
    }
    if (Jump)
        cPos.Y = MapHeight + 1 - Pos;
    else
        cPos.Y = MapHeight - 2 - Pos;
    SetConsoleCursorPosition(hConsole, cPos);
    std::cout << " ";


    /// OBS

    cPos.X = MapWidth - Value - 2; cPos.Y = MapHeight;
    SetConsoleCursorPosition(hConsole, cPos);
    for (int i = 4; i > 0; i--)
    {
        std::cout << "$  ";
        cPos.Y--;
        SetConsoleCursorPosition(hConsole, cPos);
    }

    Sleep(30);

    //// CLEAR

    if (Value > MapWidth - 5)
    {
        cPos.Y = MapHeight;
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
    point++;
}

void Start(uint8_t ConsoleHeight, uint8_t ConsoleWidth)
{
    while (true)
    {
        system("color 0F");
        system("cls");
        srand(time(NULL));

        DrawMapBorders(ConsoleWidth, ConsoleHeight, MapWidth, MapHeight);

        while (true)
        {
            InputChar();
            JumpingAbility();
            DrawGameAttributes();
            if (cPos.X == 6 && Pos < 3)
            {
                break;
            }
        }
        cPos.X = 35; cPos.Y = 5;
        SetConsoleCursorPosition(hConsole, cPos);
        SetConsoleTextAttribute(hConsole, 1);
        std::cout << "GAME OVER!";

        cPos.X = 0; cPos.Y = 15;
        SetConsoleCursorPosition(hConsole, cPos);
        SetConsoleTextAttribute(hConsole, 241);
        std::cout << "Enter 0 to exit application.\n";
        std::cout << "Enter 1 to restart game.\n";
        std::cout << "Enter 2 to go back to menu.\n";

        // ввод
        while (true)
        {
            if (_kbhit())
            {
                Key = _getch();
                if (Key == '0')
                {
                    return;
                }
                if (Key == '1')
                {
                    break;
                }
                if (Key == '2')
                {
                    return;
                }
            }
        }
        system("cls");
    }
}