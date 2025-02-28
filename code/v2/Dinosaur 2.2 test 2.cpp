#include <iostream>
#include <Windows.h>
#include <ctime>
#include <thread>
#include <stdio.h>
#include <conio.h>
#include <fstream>
#include <string>
#include <locale.h>
#include <cstdlib>
using namespace std;

bool game_over = false;
// map borders
short Left = 15;
short Right = 0;
short Up = 0;
short Down = 15;
short Obstacles_L = 0; // obstacles length
short Obstacles_H = 0; // obstacles height
short ObstaclesColor = 15;
short PlayerColor = 15;
short PointsIndicatorColor = 1;
short ObsNum = 0; // Obstacles number
unsigned long int points = 0;
unsigned short obstacles = 0;
bool OBS[4][4];
short speed = 900;
string S = "-1";
short number;
short maxspeed = 1;
short exponent = 0;
short pos = 5;
short randtime = 0;
short value = 0;
unsigned short x = 40;
unsigned short y = 7;
short t = 0;
short H_indent = 0; // Horizontal indentation 
short V_indent = 0; // Vertical indentation
short clear = 0;
int Challenge = 0;
short g = 0; // acceleration of gravity
unsigned short amount = 0;

// keys 
char MoveDinoUp1 = 'w';
char MoveDinoUp2 = 'W';
char MoveDinoUp3 = 'ц';
char MoveDinoUp4 = 'Ц';
char MoveDinoDown1 = 's';
char MoveDinoDown2 = 'S';
char MoveDinoDown3 = 'ы';
char MoveDinoDown4 = 'Ы';

COORD cPos;
COORD Chars;
short widht = Chars.X;
short height = Chars.Y;
HANDLE hConsole;
enum eDirection { STOP, UP, DOWN };
eDirection dir;

int CheckInput(const char* msg)
{
    string tmp;
    int value;
    while (true)
    {
        cout << msg;
        cin >> tmp;
        try
        {
            value = stoi(tmp);
            cin.ignore((numeric_limits<unsigned char>::max)(), '\n');
            return value;
        }
        catch (invalid_argument)
        {
            cin.clear();
            cin.ignore(cin.rdbuf()->in_avail());
            unsigned long int c = points;
            for (int i = 0; i < 4; i++)
            {
                c /= 2;
            }
            if (c % 2 == 1)
            {
                cout << "Error\n";
            }
        }
        catch (out_of_range)
        {
            cin.ignore((numeric_limits<unsigned char>::max)(), '\n');
        }
    }
}
int Convertion(short a, unsigned long int b)
{
    for (int i = 0; i < (a - 1); i++)
    {
        b /= 2;
    }
    return (b % 2);
}
void ConsoleCursorVisible(bool show, short size)
{
    CONSOLE_CURSOR_INFO structCursorInfo;
    GetConsoleCursorInfo(hConsole, &structCursorInfo);
    structCursorInfo.bVisible = show;
    structCursorInfo.dwSize = size;
    SetConsoleCursorInfo(hConsole, &structCursorInfo);
}
void ShowMap()
{
    SetConsoleTextAttribute(hConsole, Up);
    for (int i = 0; i < (x + 2); i++) { cout << "#"; } cout << "\n";
    for (int i = 0; i < y; i++)
    {
        SetConsoleTextAttribute(hConsole, Left);
        cout << "#";
        SetConsoleTextAttribute(hConsole, 15);
        for (int j = 0; j < x; j++)
        {
            if ((i == y - 2 || i == y - 1) && (j == 3))
            {
                SetConsoleTextAttribute(hConsole, PlayerColor); cout << "*"; j++;
            }
            cout << " ";
        }
        SetConsoleTextAttribute(hConsole, Right);
        cout << "#\n";
    }
    SetConsoleTextAttribute(hConsole, Down);
    for (int i = 0; i < (x + 2); i++) { cout << "#"; } cout << "\n";
}
CONSOLE_SCREEN_BUFFER_INFO csbInfo;
void GetBufferChars(COORD* Chars)
{
    GetLargestConsoleWindowSize(hConsole);
    Chars->X = csbInfo.srWindow.Right - csbInfo.srWindow.Left + 1;
    Chars->Y = csbInfo.srWindow.Bottom - csbInfo.srWindow.Top + 1;
}
void cls1()
{
    system("cls");
}
void cls2()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    SMALL_RECT scrollRect;
    COORD scrollTarget;
    CHAR_INFO fill;
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
    {
        return;
    }
    scrollRect.Left = 0;
    scrollRect.Top = 0;
    scrollRect.Right = csbi.dwSize.X;
    scrollRect.Bottom = csbi.dwSize.Y;
    scrollTarget.X = 0;
    scrollTarget.Y = (SHORT)(0 - csbi.dwSize.Y);
    fill.Char.UnicodeChar = TEXT(' ');
    fill.Attributes = csbi.wAttributes;
    ScrollConsoleScreenBuffer(hConsole, &scrollRect, NULL, scrollTarget, &fill);
    csbi.dwCursorPosition.X = 0;
    csbi.dwCursorPosition.Y = 0;
    SetConsoleCursorPosition(hConsole, csbi.dwCursorPosition);
}
void cls3()
{
    COORD coordScreen = { 0, 0 };
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
    {
        return;
    }
    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    if (!FillConsoleOutputCharacter(hConsole,
        (TCHAR)' ',
        dwConSize,
        coordScreen,
        &cCharsWritten))
    {
        return;
    }
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
    {
        return;
    }
    if (!FillConsoleOutputAttribute(hConsole,
        csbi.wAttributes,
        dwConSize,
        coordScreen,
        &cCharsWritten))
    {
        return;
    }
    SetConsoleCursorPosition(hConsole, coordScreen);
}
void cls4()
{
    printf("\x1b[H");
    for (int i = 0; i < 50; i++)
    {
        for (int j = 0; j < 200; j++)
        {
            cout << " ";
        }
        cout << "\n";
    }
    printf("\x1b[H");
}
void ChallengeComplete(short victorypoints)
{
    short CongratsColor[100]; S = "-1";
    for (int i = 0; i < 100; i++)
    {
        if (i % 3 == 0) { CongratsColor[i] = 1; }
        if (i % 3 == 1) { CongratsColor[i] = 3; }
        if (i % 3 == 2) { CongratsColor[i] = 9; }
    } points = 0;
    while (true)
    {
        system("cls");
        // C
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 0 + 70; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[0]); if ((i < 2 || i > 6) && i != 10) { cout << " "; } else { cout << "#"; } }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 1 + 70; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[1]); if (i == 1 || i == 7 || i == 10) { cout << "#"; } else { cout << " "; } }
        for (int j = 0; j < 4; j++) {
            for (int i = 0; i < 11; i++) {
                cPos.Y = i; cPos.X = 2 + 70 + j; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[2 + j]); if (i == 0 || i == 8 || i == 10) { cout << "#"; }
                else { cout << " "; }
            }
        }
        // O                                                    
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 8 + 70; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[6]); if ((i < 2 || i > 6) && i != 10) { cout << " "; } else { cout << "#"; } }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 9 + 70; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[7]); if (i == 1 || i == 7 || i == 10) { cout << "#"; } else { cout << " "; } }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 10 + 70; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[8]); if (i == 0 || i == 8 || i == 10) { cout << "#"; } else { cout << " "; } }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 11 + 70; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[9]); if (i == 0 || i == 8 || i == 10) { cout << "#"; } else { cout << " "; } }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 12 + 70; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[10]); if (i == 1 || i == 7 || i == 10) { cout << "#"; } else { cout << " "; } }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 13 + 70; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[11]); if ((i < 2 || i > 6) && i != 10) { cout << " "; } else { cout << "#"; } }
        // N
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 16 + 70; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[11]); if (i != 9) { cout << "#"; } else { cout << " "; } }
        for (int j = 0; j < 5; j++) {
            for (int i = 0; i < 11; i++) {
                cPos.Y = i; cPos.X = 17 + 70 + j; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[12 + j]); if (i == 1 + j || i == 10) { cout << "#"; }
                else { cout << " "; }
            }
        }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 22 + 70; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[17]); if (i != 9) { cout << "#"; } else { cout << " "; } }
        // G
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 25 + 70; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[18]); if ((i < 2 || i > 6) && i != 10) { cout << " "; } else { cout << "#"; } }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 26 + 70; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[19]); if (i == 1 || i == 7 || i == 10) { cout << "#"; } else { cout << " "; } }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 27 + 70; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[20]); if (i == 0 || i == 8 || i == 10) { cout << "#"; } else { cout << " "; } }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 28 + 70; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[21]); if (i == 0 || i == 4 || i == 8 || i == 10) { cout << "#"; } else { cout << " "; } }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 29 + 70; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[22]); if (i == 0 || i == 4 || i == 7 || i == 10) { cout << "#"; } else { cout << " "; } }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 30 + 70; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[23]); if (i == 1 || i == 5 || i == 6 || i == 10) { cout << "#"; } else { cout << " "; } }
        // R
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 33 + 70; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[24]); if (i > 0 && i != 9) { cout << "#"; } else { cout << " "; } }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 34 + 70; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[25]); if (i == 0 || i == 5 || i == 10) { cout << "#"; } else { cout << " "; } }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 35 + 70; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[26]); if (i == 0 || i == 4 || i == 6 || i == 10) { cout << "#"; } else { cout << " "; } }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 36 + 70; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[27]); if (i == 0 || i == 3 || i == 7 || i == 10) { cout << "#"; } else { cout << " "; } }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 37 + 70; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[28]); if (i == 1 || i == 2 || i == 8 || i == 10) { cout << "#"; } else { cout << " "; } }
        // A
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 40 + 70; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[29]); if (i > 5 && i != 9) { cout << "#"; } else { cout << " "; } }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 41 + 70; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[30]); if ((i > 2 && i < 6) || i == 10) { cout << "#"; } else { cout << " "; } }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 42 + 70; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[31]); if (i == 1 || i == 2 || i == 5 || i == 10) { cout << "#"; } else { cout << " "; } }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 43 + 70; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[32]); if (i == 0 || i == 5 || i == 10) { cout << "#"; } else { cout << " "; } }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 44 + 70; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[33]); if (i == 1 || i == 2 || i == 5 || i == 10) { cout << "#"; } else { cout << " "; } }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 45 + 70; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[34]); if ((i > 2 && i < 6) || i == 10) { cout << "#"; } else { cout << " "; } }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 46 + 70; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[35]); if ((i > 5) && i != 9) { cout << "#"; } else { cout << " "; } }
        // T
        for (int j = 0; j < 7; j++)
        {
            for (int i = 0; i < 11; i++)
            {
                if (i == 9) { break; }
                cPos.Y = i; cPos.X = 49 + j + 70; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[36 + j]); cout << "#";
                if (j != 3) { break; }
            }
            cPos.Y = 10; cPos.X = 49 + j + 70; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[36 + j]); cout << "#";
        }
        // S
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 58 + 70; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[43]); if (i == 2 || i == 10) { cout << "#"; } else { cout << " "; } }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 59 + 70; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[44]); if (i == 1 || i == 3 || i == 10) { cout << "#"; } else { cout << " "; } }
        for (int j = 0; j < 4; j++) {
            for (int i = 0; i < 11; i++) {
                for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 60 + 70 + j; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[45 + j]); if (i == 0 || i == 4 || i == 8 || i == 10) { cout << "#"; } else { cout << " "; } }
            }
        }        
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 64 + 70; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[49]); if (i == 0 || i == 5 || i == 7 || i == 10) { cout << "#"; } else { cout << " "; } }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 65 + 70; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[50]); if (i == 6) { cout << "#"; } else { cout << " "; } } SetConsoleTextAttribute(hConsole, 15);
        if (points < victorypoints) { SetConsoleTextAttribute(hConsole, 15); cPos.Y = 12; cPos.X = 66 + 31; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); cout << "Points: " << points; } if (points > victorypoints) { break; }
        if (points % 20 == 0)
        {
            for (int i = 0; i < 51; i++)
            {
                if (CongratsColor[i] == 1) { CongratsColor[i] = 27; }
                CongratsColor[i] /= 3;
            }
        }
        points += 10;
        this_thread::sleep_for(chrono::milliseconds(20));
        if (points > 100)
        {
            if (S == "1") { break; }
            cPos.X = 0; cPos.Y = 15; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15); cout << "Enter 0 to skip";
            if (_kbhit())
            {
                switch (_getch())
                {
                case '0':
                    S = "1";
                    break;
                }
            }
        }
    }
    system("cls");
    while (true)
    {
        system("cls");
        // C
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 0 + 30; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[0]); if ((i < 2 || i > 6) && i != 10) { cout << " "; } else { cout << "#"; } }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 1 + 30; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[1]); if (i == 1 || i == 7 || i == 10) { cout << "#"; } else { cout << " "; } }
        for (int j = 0; j < 4; j++) {
            for (int i = 0; i < 11; i++) {
                cPos.Y = i; cPos.X = 2 + j + 30; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[2 + j]); if (i == 0 || i == 8 || i == 10) { cout << "#"; }
                else { cout << " "; }
            }
        }
        // H
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 8 + 30; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[6]); if (i != 9) { cout << "#"; } else { cout << " "; } }
        for (int j = 0; j < 3; j++) {
            for (int i = 0; i < 11; i++) {
                cPos.Y = i; cPos.X = 9 + j + 30; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[7 + j]); if (i == 4 || i == 10) { cout << "#"; }
                else { cout << " "; }
            }
        }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 12 + 30; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[10]); if (i != 9) { cout << "#"; } else { cout << " "; } }
        // A
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 15 + 30; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[11]); if (i > 5 && i != 9) { cout << "#"; } else { cout << " "; } }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 16 + 30; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[12]); if ((i > 2 && i < 6) || i == 10) { cout << "#"; } else { cout << " "; } }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 17 + 30; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[13]); if (i == 1 || i == 2 || i == 5 || i == 10) { cout << "#"; } else { cout << " "; } }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 18 + 30; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[14]); if (i == 0 || i == 5 || i == 10) { cout << "#"; } else { cout << " "; } }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 19 + 30; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[15]); if (i == 1 || i == 2 || i == 5 || i == 10) { cout << "#"; } else { cout << " "; } }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 20 + 30; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[16]); if ((i > 2 && i < 6) || i == 10) { cout << "#"; } else { cout << " "; } }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 21 + 30; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[17]); if ((i > 5) && i != 9) { cout << "#"; } else { cout << " "; } }
        // L
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 24 + 30; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[18]); if (i != 9) { cout << "#"; } else { cout << " "; } }
        for (int i = 0; i < 4; i++) { cPos.Y = 8; cPos.X = 25 + i + 30; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[19 + i]); cout << "#"; cPos.Y = 10; cPos.X = 25 + i + 30; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[19 + i]); cout << "#"; }
        // L
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 31 + 30; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[23]); if (i != 9) { cout << "#"; } else { cout << " "; } }
        for (int i = 0; i < 4; i++) { cPos.Y = 8; cPos.X = 32 + i + 30; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[24 + i]); cout << "#"; cPos.Y = 10; cPos.X = 32 + i + 30; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[24 + i]); cout << "#"; }
        // E
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 38 + 30; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[29]); if (i == 0 || i == 8 || i == 9) { cout << " "; } else { cout << "#"; } }
        for (int j = 0; j < 4; j++) {
            for (int i = 0; i < 11; i++) {
                cPos.Y = i; cPos.X = 39 + j + 30; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[30 + j]); if (i == 0 || i == 4 || i == 8 || i == 10) { cout << "#"; }
                else { cout << " "; }
            }
        }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 43 + 30; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[34]); if (i == 0 || i == 8 || i == 10) { cout << "#"; } else { cout << " "; } }
        // N
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 46 + 30; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[35]); if (i != 9) { cout << "#"; } else { cout << " "; } }
        for (int j = 0; j < 5; j++) {
            for (int i = 0; i < 11; i++) {
                cPos.Y = i; cPos.X = 47 + j + 30; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[36 + j]); if (i == j + 1 || i == 10) { cout << "#"; }
                else { cout << " "; }
            }
        }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 52 + 30; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[41]); if (i != 9) { cout << "#"; } else { cout << " "; } }
        // G
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 55 + 30; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[44]); if ((i < 2 || i > 6) && i != 10) { cout << " "; } else { cout << "#"; } }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 56 + 30; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[45]); if (i == 1 || i == 7 || i == 10) { cout << "#"; } else { cout << " "; } }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 57 + 30; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[46]); if (i == 0 || i == 8 || i == 10) { cout << "#"; } else { cout << " "; } }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 58 + 30; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[47]); if (i == 0 || i == 4 || i == 8 || i == 10) { cout << "#"; } else { cout << " "; } }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 59 + 30; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[48]); if (i == 0 || i == 4 || i == 7 || i == 10) { cout << "#"; } else { cout << " "; } }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 60 + 30; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[49]); if (i == 1 || i == 5 || i == 6 || i == 10) { cout << "#"; } else { cout << " "; } }
        // E
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 63 + 30; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[50]); if (i == 0 || i == 8 || i == 9) { cout << " "; } else { cout << "#"; } }
        for (int j = 0; j < 4; j++) {
            for (int i = 0; i < 11; i++) {
                cPos.Y = i; cPos.X = 64 + j + 30; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[51 + j]); if (i == 0 || i == 4 || i == 8 || i == 10) { cout << "#"; }
                else { cout << " "; }
            }
        }
        // C
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 72 + 34; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[51]); if ((i < 2 || i > 6) && i != 10) { cout << " "; } else { cout << "#"; } }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 73 + 34; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[52]); if (i == 1 || i == 7 || i == 10) { cout << "#"; } else { cout << " "; } }
        for (int j = 0; j < 4; j++) {
            for (int i = 0; i < 11; i++) {
                cPos.Y = i; cPos.X = j + 74 + 34; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[j + 53]); if (i == 0 || i == 8 || i == 10) { cout << "#"; }
                else { cout << " "; }
            }
        }
        // O
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 80 + 34; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[57]); if ((i < 2 || i > 6) && i != 10) { cout << " "; } else { cout << "#"; } }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 81 + 34; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[58]); if (i == 1 || i == 7 || i == 10) { cout << "#"; } else { cout << " "; } }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 82 + 34; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[59]); if (i == 0 || i == 8 || i == 10) { cout << "#"; } else { cout << " "; } }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 83 + 34; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[60]); if (i == 0 || i == 8 || i == 10) { cout << "#"; } else { cout << " "; } }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 84 + 34; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[61]); if (i == 1 || i == 7 || i == 10) { cout << "#"; } else { cout << " "; } }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 85 + 34; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[62]); if ((i < 2 || i > 6) && i != 10) { cout << " "; } else { cout << "#"; } }
        // M
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 88 + 34; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[63]); if (i < 5 || i == 9) { cout << " "; } else { cout << "#"; } }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 89 + 34; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[64]); if ((i > 0 && i < 5) || i == 10) { cout << "#"; } else { cout << " "; } }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 90 + 34; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[65]); if (i == 0 || i == 10) { cout << "#"; } else { cout << " "; } }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 91 + 34; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[66]); if (i == 1 || i == 2 || i == 10) { cout << "#"; } else { cout << " "; } }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 92 + 34; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[67]); if (i == 3 || i == 10) { cout << "#"; } else { cout << " "; } }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 93 + 34; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[68]); if (i == 1 || i == 2 || i == 10) { cout << "#"; } else { cout << " "; } }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 94 + 34; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[69]); if (i == 0 || i == 10) { cout << "#"; } else { cout << " "; } }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 95 + 34; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[70]); if ((i > 0 && i < 5) || i == 10) { cout << "#"; } else { cout << " "; } }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 96 + 34; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[71]); if (i < 5 || i == 9) { cout << " "; } else { cout << "#"; } }
        // P
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 99 + 34; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[72]); if (i == 0 || i == 9) { cout << " "; } else { cout << "#"; } }
        for (int j = 0; j < 4; j++) {
            for (int i = 0; i < 11; i++) {
                cPos.Y = i; cPos.X = j + 100 + 34; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[j + 73]); if (i == 0 || i == 4 || i == 10) { cout << "#"; }
                else { cout << " "; }
            }
        }
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 104 + 34; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[77]); if (i == 1 || i == 2 || i == 3 || i == 10) { cout << "#"; } else { cout << " "; } }
        // L
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 107 + 34; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[78]); if (i != 9) { cout << "#"; } else { cout << " "; } }
        for (int i = 0; i < 4; i++) { cPos.Y = 8; cPos.X = 108 + i + 34; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[79 + i]); cout << "#"; cPos.Y = 10; cPos.X = 108 + i + 34; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[79 + i]); cout << "#"; }
        // E
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 114 + 34; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[73]); if (i == 0 || i == 8 || i == 9) { cout << " "; } else { cout << "#"; } }
        for (int j = 0; j < 4; j++) {
            for (int i = 0; i < 11; i++) {
                cPos.Y = i; cPos.X = 115 + j + 34; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[74 + j]); if (i == 0 || i == 4 || i == 8 || i == 10) { cout << "#"; }
                else { cout << " "; }
            }
        }
        // T
        for (int j = 0; j < 7; j++)
        {
            for (int i = 0; i < 11; i++)
            {
                if (i == 9) { break; }
                cPos.Y = i; cPos.X = 121 + j + 34; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[75 + j]); cout << "#";
                if (j != 3) { break; }
            }
            cPos.Y = 10; cPos.X = 121 + j + 34; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[75 + j]); cout << "#";
        }
        // E
        for (int i = 0; i < 11; i++) { cPos.Y = i; cPos.X = 130 + 34; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[82]); if (i == 0 || i == 8 || i == 9) { cout << " "; } else { cout << "#"; } }
        for (int j = 0; j < 4; j++) {
            for (int i = 0; i < 11; i++) {
                cPos.Y = i; cPos.X = 131 + j + 34; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, CongratsColor[83 + j]); if (i == 0 || i == 4 || i == 8 || i == 10) { cout << "#"; }
                else { cout << " "; }
            }
        } SetConsoleTextAttribute(hConsole, 15);
        if (points % 100 > 50) { SetConsoleTextAttribute(hConsole, 0); } if (points % 100 > 0 && points % 100 < 50) { SetConsoleTextAttribute(hConsole, 15); }  cPos.Y = 12; cPos.X = 66 + 31; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); cout << "Points: " << victorypoints;
        if (points % 30 == 0)
        {
            for (int i = 0; i < 100; i++)
            {
                if (CongratsColor[i] == 1) { CongratsColor[i] = 27; }
                CongratsColor[i] /= 3;
            }
        }
        if (points == 2 * victorypoints) { break; }
        points += 10;
        this_thread::sleep_for(chrono::milliseconds(20));
        if (points > 100)
        {
            if (S == "1") { S = "-1"; break; }
            cPos.X = 0; cPos.Y = 15; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15); cout << "Enter 0 to skip";
            if (_kbhit())
            {
                switch (_getch())
                {
                case '0':
                    S = "1";
                    break;
                }
            }
        }
    }
}
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
void Input()
{
    if (_kbhit())
    {
        char str;
        str = _getch();
        switch (str)
        {
        case 'w':
        case 'W':
        case 'ц':
        case 'Ц':
        case KEY_UP:
            dir = UP;
            break;
        case 's':
        case 'S':
        case 'ы':
        case 'Ы':
        case KEY_DOWN:
            dir = DOWN;
            break;
        }
    }
}
void Select()
{
    if (_kbhit())
    {
        char str;
        str = _getch();
        switch (str)
        {
        case 'w':
        case 'W':
        case 'ц':
        case 'Ц':
        case KEY_UP:
            if (t > 1)
            {
                t--;
            }
            break;
        case 's':
        case 'S':
        case 'ы':
        case 'Ы':
        case KEY_DOWN:
            if (t < 3)
            {
                t++;
            }
            break;
        case '0':
            S = "0";
            break;
        case '=':
            S = "=";
            break;
        }
    }
}
void ObsInput()
{
    if (_kbhit())
    {
        char str;
        str = _getch();
        switch (str)
        {
        case 'w':
        case 'W':
        case 'ц':
        case 'Ц':
        case KEY_UP:
            if (Obstacles_H > 0) { Obstacles_H--; }
            break;
        case 's':
        case 'S':
        case 'ы':
        case 'Ы':
        case KEY_DOWN:
            if (Obstacles_H < 3) { Obstacles_H++; }
            break;
        case 'a':
        case 'A':
        case 'ф':
        case 'Ф':
        case KEY_LEFT:
            if (Obstacles_L > 0) { Obstacles_L--; }
            break;
        case 'd':
        case 'D':
        case 'в':
        case 'В':
        case KEY_RIGHT:
            if (Obstacles_L < 3) { Obstacles_L++; }
            break;
        case '/':
            OBS[Obstacles_H][Obstacles_L] = 1;
            break;
        case '*':
            OBS[Obstacles_H][Obstacles_L] = 0;
            break;
        case '0':
            S = "0";
            break;
        case '1':
            S = "1";
            break;
        case '!':
            S = "2";
            break;
        }
    }
}
void Logic()
{
    switch (dir)
    {
    case UP:
        if (pos >= 0)
        {
            pos--;
        }
        dir = STOP;
        break;
    case DOWN:
        if (pos < y - 3)
        {
            pos++;
        }
        dir = STOP;
        break;
    }
}
void Draw(short obs2[], short n)
{
    cPos.X = H_indent; cPos.Y = V_indent; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, PointsIndicatorColor); cout << "Points: " << points; for (int a = 0; a < 15; a++) { cout << " "; }
    SetConsoleTextAttribute(hConsole, 15);
    cPos.X = 5 + H_indent; cPos.Y = pos + 3 + V_indent; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, PlayerColor); cout << "*"; cPos.X = 5 + H_indent; cPos.Y = pos + 4 + V_indent; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, PlayerColor); cout << "*";
    for (int a = 0; a < (n / 2); a++)
    {
        if (obs2[2 * a + 1] > 10 * a)
        {
            cPos.X = x - obs2[2 * a + 1] + 10 * a + 1 + H_indent; cPos.Y = obs2[2 * a] + 2 + V_indent; SetConsoleTextAttribute(hConsole, ObstaclesColor); SetConsoleCursorPosition(hConsole, cPos); cout << "$";
            cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); cout << "$";
            if (x - obs2[2 * a + 1] + 10 * a == 5 && (obs2[2 * a] + 2 == pos + 2 || obs2[2 * a] + 2 == pos + 3 || obs2[2 * a] + 2 == pos + 4)) {
                game_over = true; break;
            }
        }
    }
    this_thread::sleep_for(chrono::milliseconds(1000 - speed + 1));
    cPos.X = 5 + H_indent; cPos.Y = pos + 3 + V_indent; SetConsoleCursorPosition(hConsole, cPos); cout << " "; cPos.X = 5 + H_indent; cPos.Y = pos + 4 + V_indent; SetConsoleCursorPosition(hConsole, cPos); cout << " ";
    for (int a = 0; a < (n / 2); a++)
    {
        if (obs2[2 * a + 1] > 10 * a)
        {
            cPos.X = x - obs2[2 * a + 1] + 10 * a + 1 + H_indent; cPos.Y = obs2[2 * a] + 2 + V_indent; SetConsoleCursorPosition(hConsole, cPos); cout << " ";
            cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); cout << " ";
        }
    }
    //random
    for (int a = 0; a < (n / 2); a++)
    {
        if (obs2[2 * a + 1] == x + 10 * a)
        {
            obs2[2 * a + 1] = 0;
            srand(time(0));
            if ((randtime % ((5 + a) + obs2[2 * a] % (5 + a))) == 0)
            {
                obs2[2 * a] = rand() % (y - 1);
                randtime = 0;
            }
        }
    }
    points++;
    for (int a = 0; a < (n / 2); a++)
    {
        obs2[2 * a + 1]++;
    }
    if (exponent > 0 && points % 50 == 0 && maxspeed > speed)
    {
        speed += exponent; if (speed > 1000) { speed = 1000; }
    }
    if (exponent < 0 && points % 50 == 0 && maxspeed < speed)
    {
        speed += exponent; if (speed < 1) { speed = 1; }
    }
}
void DrawObs(short obs2[], unsigned short Obstacles2[], short n)
{
    cPos.X = H_indent; cPos.Y = V_indent; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, PointsIndicatorColor); cout << "Points: " << points; for (int i = 0; i < 15; i++) { cout << " "; }
    SetConsoleTextAttribute(hConsole, 15);
    cPos.X = 5 + H_indent; cPos.Y = pos + 3 + V_indent; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, PlayerColor); cout << "*"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, PlayerColor); cout << "*";
    for (int i = 0; i < (n / 2); i++)
    {
        if (obs2[2 * i + 1] > 13 * i)
        {
            unsigned short k = Obstacles2[i]; cPos.X = x - obs2[2 * i + 1] + 13 * i + H_indent; cPos.Y = obs2[2 * i] + 1 + V_indent;
            for (int a = 0; a < 4; a++)
            {
                cPos.Y++;
                for (int b = 0; b < 4; b++)
                {
                    cPos.X++;
                    if (k % 2 == 1) {
                        if (obs2[2 * i + 1] > 13 * i + b) {
                            // collision
                            if ((x - obs2[2 * i + 1] + 13 * i + b == 4) && (obs2[2 * i] + a == pos + 1 || obs2[2 * i] + a == pos + 2))
                            {
                                game_over = true;
                            }
                            SetConsoleTextAttribute(hConsole, ObstaclesColor); SetConsoleCursorPosition(hConsole, cPos); cout << "$";
                        }
                    }
                    k /= 2; if (k == 0) { break; }
                }
                cPos.X -= 4;
            }
        }
    }
    this_thread::sleep_for(chrono::milliseconds(1000 - speed + 1));
    for (int i = 0; i < (n / 2); i++)
    {
        if (obs2[2 * i + 1] >= 13 * i + 4)
        {
            cPos.X = x - obs2[2 * i + 1] + 13 * i + 1 + H_indent; cPos.Y = obs2[2 * i] + 1 + V_indent;
            for (int j = 0; j < 4; j++)
            {
                cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); cout << "    ";
            }
        }
    }
    cPos.X = 5 + H_indent; cPos.Y = pos + 3 + V_indent; SetConsoleCursorPosition(hConsole, cPos); cout << " "; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); cout << " ";
    //random
    srand(time(0));
    for (int i = 0; i < (n / 2); i++)
    {
        if (obs2[2 * i + 1] + 1 == x + 13 * i)
        {
            obs2[2 * i + 1] = 13 * i;
            if ((randtime % ((5 + i) + obs2[2 * i] % (5 + i))) == 0)
            {
                obs2[2 * i] = rand() % (y - 3);
                randtime = 0;
            }
            ifstream in("Obstacles.txt"); // open file .txt
            if (in.is_open())
            {
                short a = rand() % ObsNum;
                for (int b = 0; b <= a; b++)
                {
                    in >> obstacles;
                }
                Obstacles2[i] = obstacles;
            }
            in.close();     // close file
        }
    }
    points++;
    if (exponent > 0 && points % 50 == 0 && maxspeed > speed)
    {
        speed += exponent; if (speed > 1000) { speed = 1000; }
    }
    if (exponent < 0 && points % 50 == 0 && maxspeed < speed)
    {
        speed += exponent; if (speed < 1) { speed = 1; }
    }
    for (int i = 0; i < (n / 2); i++)
    {
        obs2[2 * i + 1]++;
    }
}
int DisplayConfirmSaveAsMessageBox()
{
    int msgboxID = MessageBox(
        NULL,
        L"We recommend looking at the mechanics of creating levels.\n",
        L"",
        MB_ICONEXCLAMATION | MB_YESNO
    );

    if (msgboxID == IDYES)
    {
        S = "*";
    }

    return msgboxID;
}
int main()
{
    // MessageBox(NULL, L"Ошибка получения дескриптора", L"Ошибка", MB_OK || MB_ICONERROR);
    // MessageBox(NULL, L"Just kidding)", L")", MB_OK);   
    // string name = "First Level";
    // CreateDirectoryA(name.c_str(), NULL);    
    HWND window_header = GetConsoleWindow();
    SetWindowPos(window_header, HWND_TOP, 0, 0, 300, 300, NULL);
    HANDLE out_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD maxWindow = GetLargestConsoleWindowSize(out_handle);
    SMALL_RECT srctWindow = { 0, 0, maxWindow.X - 1, maxWindow.Y - 1 }; maxWindow.Y = 90000000;
    SMALL_RECT minWindow = { 0, 0, 0, 0 };
    SetConsoleWindowInfo(out_handle, false, &minWindow);
    SetConsoleScreenBufferSize(out_handle, maxWindow);
    SetConsoleWindowInfo(out_handle, true, &srctWindow);
    HWND hWnd = GetConsoleWindow();
    HMENU hMenu = GetSystemMenu(hWnd, false);
    int c = GetMenuItemCount(hMenu);
    DeleteMenu(hMenu, c - 1, MF_BYPOSITION);
    RemoveMenu(hMenu, SC_SIZE, MF_BYCOMMAND | MF_REMOVE);
    RemoveMenu(hMenu, SC_MAXIMIZE, MF_BYCOMMAND | MF_REMOVE);
    DrawMenuBar(hWnd);
    // SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_FULLSCREEN_MODE, 0);
    maxWindow = GetLargestConsoleWindowSize(out_handle);
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    short size = 6;  points = 26;
    ifstream in("Saves.txt"); // open file .txt
    if (in.is_open())
    {
        in >> x >> y;
        in >> points;
        in >> Left >> Right >> Down >> Up;
        in >> PlayerColor;
        in >> ObstaclesColor;
        in >> PointsIndicatorColor;
        in >> speed;
        in >> maxspeed;
        in >> exponent;
        in >> H_indent;
        in >> V_indent;
    }
    in.close();     // close file    
    if (x < 40) { x = 40; } if (x > 209) { x = 209; } if (y < 7) { y = 7; } if (y > 40) { y = 40; }
    if (Left < 0) { Left = 0; } if (Left > 15) { Left = 15; } if (Right < 0) { Right = 0; } if (Right > 15) { Right = 15; }
    if (Down < 0) { Down = 0; } if (Down > 15) { Down = 15; } if (Up < 0) { Up = 15; } if (Up > 15) { Up = 15; }
    if (PlayerColor < 0) { PlayerColor = 0; } if (PlayerColor > 15) { PlayerColor = 15; } if (ObstaclesColor < 0) { ObstaclesColor = 0; } if (ObstaclesColor > 15) { ObstaclesColor = 15; }
    if (PointsIndicatorColor < 0) { PointsIndicatorColor = 0; } if (PointsIndicatorColor > 15) { PointsIndicatorColor = 15; } if (speed < 1) { speed = 1; } if (speed > 1000) { speed = 1000; }
    if (exponent < -100 || exponent > 100) { exponent = 0; } if (maxspeed < 1) { maxspeed = 1; } if (maxspeed > 1000) { maxspeed = 1000; }
    if ((exponent > 0 && speed > maxspeed) || (exponent < 0 && speed < maxspeed)) { maxspeed = 0; exponent = 0; } if (points < 0) { points = 26; }
    while (S != "2")
    {
        while (S != "0" && S != "2")
        {
            ObsNum = 0;
            ifstream in("obstacles.txt");
            if (in.is_open())
            {
                unsigned short m = 0;
                while (!in.eof())
                {
                    if (!(in >> m)) { break; }
                    ObsNum++;
                }
            }
            in.close();
            S = "-1";
            system("cls"); game_over = false; dir = STOP;
            hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            if (Convertion(6, points) != 1)
            {
                SetConsoleTextAttribute(hConsole, 15);
                //start
                cPos.Y = 0; cPos.X = (maxWindow.X - 20) / 2; SetConsoleCursorPosition(hConsole, cPos);
                cout << " ##  ###  #  ##  ###"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                cout << "#     #  # # # #  # "; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                cout << " ##   #  ### ##   # "; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                cout << "   #  #  # # # #  # "; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                cout << " ##   #  # # # #  # "; cPos.Y++; cPos.X = (maxWindow.X - 7) / 2; SetConsoleCursorPosition(hConsole, cPos);
                SetConsoleTextAttribute(hConsole, 4);
                cout << "Enter 0";
                SetConsoleTextAttribute(hConsole, 15);
                //settings
                cPos.Y++; cPos.X = (maxWindow.X - 32) / 2; SetConsoleCursorPosition(hConsole, cPos);
                cout << " ##   ## ### ### # #  # ###  ## "; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                cout << "#    #    #   #    ## # #   #   "; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                cout << " ##   ##  #   #  # # ## # #  ## "; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                cout << "   # #    #   #  # #  # # #    #"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                cout << " ##   ##  #   #  # #  # ###  ## "; cPos.Y++; cPos.X = (maxWindow.X - 7) / 2; SetConsoleCursorPosition(hConsole, cPos);
                SetConsoleTextAttribute(hConsole, 4);
                cout << "Enter 1" << endl;
                SetConsoleTextAttribute(hConsole, 15);
                // level editor
                cPos.Y++; cPos.X = (maxWindow.X - 55) / 2; SetConsoleCursorPosition(hConsole, cPos);
                cout << "#     ##  # #   ##  #        ##  ##   #  ###   #   ## "; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                cout << "#    #    # #  #    #       #    # #      #   # #  # #"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                cout << "#     ##  # #   ##  #        ##  # #  #   #   # #  ## "; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                cout << "#    #    # #  #    #       #    # #  #   #   # #  # #"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                cout << "###   ##   #    ##  ###      ##  ##   #   #    #   # #"; cPos.Y++; cPos.X = (maxWindow.X - 7) / 2; SetConsoleCursorPosition(hConsole, cPos);
                SetConsoleTextAttribute(hConsole, 4);
                cout << "Enter 2";
                SetConsoleTextAttribute(hConsole, 15);
                //exit
                cPos.Y++; cPos.X = (maxWindow.X - 18) / 2; SetConsoleCursorPosition(hConsole, cPos);
                cout << " ##  #   #  #  ###"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                cout << "#     # #       # "; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                cout << " ##    #    #   # "; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                cout << "#     # #   #   # "; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                cout << " ##  #   #  #   # "; cPos.Y++; cPos.X = (maxWindow.X - 7) / 2; SetConsoleCursorPosition(hConsole, cPos);
                SetConsoleTextAttribute(hConsole, 4);
                cout << "Enter 3";
                SetConsoleTextAttribute(hConsole, 15);
                cPos.X = 0; cPos.Y = maxWindow.Y - 5; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
            }
            else
            {
                SetConsoleTextAttribute(hConsole, 15);
                //start
                for (int i = 0; i < (maxWindow.X - 23) / 2; i++) { cout << " "; }
                cout << " ##  ###   #   ##   ###\n"; for (int i = 0; i < (maxWindow.X - 23) / 2; i++) { cout << " "; }
                cout << "#     #   # #  # #   # \n"; for (int i = 0; i < (maxWindow.X - 23) / 2; i++) { cout << " "; }
                cout << "#     #   ###  ##    # \n"; for (int i = 0; i < (maxWindow.X - 23) / 2; i++) { cout << " "; }
                cout << "#     #   # #  #     # \n"; for (int i = 0; i < (maxWindow.X - 23) / 2; i++) { cout << " "; }
                cout << " ##   #   # #  #     # \n"; for (int i = 0; i < (maxWindow.X - 7) / 2; i++) { cout << " "; }
                SetConsoleTextAttribute(hConsole, 4);
                cout << "Введите 0\n";
                SetConsoleTextAttribute(hConsole, 15);
                //settings
                for (int i = 0; i < (maxWindow.X - 47) / 2; i++) { cout << " "; }
                cout << "# #   #    ##  ###  ##    #   # # #  #  #  #  #\n"; for (int i = 0; i < (maxWindow.X - 47) / 2; i++) { cout << " "; }
                cout << "# #  # #  #     #   # #  # #  ##  #  # #   # ##\n"; for (int i = 0; i < (maxWindow.X - 47) / 2; i++) { cout << " "; }
                cout << "###  ###  #     #   ##   # #  # # #  ##    ## #\n"; for (int i = 0; i < (maxWindow.X - 47) / 2; i++) { cout << " "; }
                cout << "# #  # #  #     #   #    # #  #  ##  # #   #  #\n"; for (int i = 0; i < (maxWindow.X - 47) / 2; i++) { cout << " "; }
                cout << "# #  # #   ##   #   #     #   #   #  #  #  #  #\n"; for (int i = 0; i < (maxWindow.X - 7) / 2; i++) { cout << " "; }
                SetConsoleTextAttribute(hConsole, 4);
                cout << "Введите 1" << endl;
                SetConsoleTextAttribute(hConsole, 15);
                //exit
                for (int i = 0; i < (maxWindow.X - 29) / 2; i++) { cout << " "; }
                cout << "##   #   #  #   #   #     #  \n"; for (int i = 0; i < (maxWindow.X - 29) / 2; i++) { cout << " "; }
                cout << "# #  #   #   # #   # #   # # \n"; for (int i = 0; i < (maxWindow.X - 29) / 2; i++) { cout << " "; }
                cout << "##   ##  #    #    # #   # # \n"; for (int i = 0; i < (maxWindow.X - 29) / 2; i++) { cout << " "; }
                cout << "# #  # # #   # #   # #  #####\n"; for (int i = 0; i < (maxWindow.X - 29) / 2; i++) { cout << " "; }
                cout << "##   ##  #  #   #   #   #   #\n"; for (int i = 0; i < (maxWindow.X - 7) / 2; i++) { cout << " "; }
                SetConsoleTextAttribute(hConsole, 4);
                cout << "Введите 2\n";
                SetConsoleTextAttribute(hConsole, 15);
            }
            while (true)
            {
                cin >> S; if (S == "0") { break; } if (S == "1") { break; } if (S == "2") { break; } if (S == "3") { break; }
            }
            if (S == "0") { break; } if (S == "3") { break; }
            // Settings
            if (S == "1")
            {
                S = "-1";
                while (true)
                {
                    system("cls");
                    if (Convertion(1, points) == 1)
                    {
                        SetConsoleTextAttribute(hConsole, 15);
                        cout << " > ";
                        SetConsoleTextAttribute(hConsole, 1);
                        cout << "Settings\n";
                        SetConsoleTextAttribute(hConsole, 15);
                    }
                    if (Convertion(2, points) == 1) { SetConsoleTextAttribute(hConsole, 15); cout << "<- Go back\n"; SetConsoleTextAttribute(hConsole, 4); cout << "Enter 0\n"; }
                    SetConsoleTextAttribute(hConsole, 15); cout << "\n";
                    cout << " ##   #    # #    ##      #   ##   ###  #   #   #  #   ##\n";
                    cout << "#    # #  # # #  #       # #  # #   #      # #  ## #  #  \n";
                    cout << "# #  ###  # # #   ##     # #  ##    #   #  # #  # ##   ##\n";
                    cout << "# #  # #  #   #  #       # #  #     #   #  # #  #  #     #\n";
                    cout << "##   # #  #   #   ##      #   #     #   #   #   #  #   ##\n";
                    SetConsoleTextAttribute(hConsole, 4); cout << "Enter 1\n";
                    SetConsoleTextAttribute(hConsole, 15);
                    cout << " ##   #   #  #  ###  ##    #   #\n";
                    cout << "#    # #  ## #   #   # #  # #  #\n";
                    cout << "#    # #  # ##   #   ##   # #  #\n";
                    cout << "#    # #  #  #   #   # #  # #  #\n";
                    cout << " ##   #   #  #   #   # #   #   #\n";
                    SetConsoleTextAttribute(hConsole, 4); cout << "Enter 2\n";
                    SetConsoleTextAttribute(hConsole, 15);
                    cout << "##   #   ##   ##   #   #   # #\n";
                    cout << "# #     #     # #  #  # #  # #\n";
                    cout << "# #  #   ##   ##   #  ###   # \n";
                    cout << "# #  #     #  #    #  # #   # \n";
                    cout << "##   #   ##   #    #  # #   # \n";
                    SetConsoleTextAttribute(hConsole, 4); cout << "Enter 3\n";
                    SetConsoleTextAttribute(hConsole, 15);
                    cout << "#  #  #  ###   # \n";
                    cout << "   ## #  #    # #\n";
                    cout << "#  # ##  ##   # #\n";
                    cout << "#  #  #  #    # #\n";
                    cout << "#  #  #  #     # \n";
                    SetConsoleTextAttribute(hConsole, 4); cout << "Enter 4\n"; SetConsoleTextAttribute(hConsole, 15);
                    while (true)
                    {
                        cin >> S; if (S == "0") { break; } if (S == "1") { break; } if (S == "2") { break; } if (S == "3") { break; } if (S == "4") { break; }
                    }
                    if (S == "0") { break; }
                    // Options
                    if (S == "1")
                    {
                        while (S != "0")
                        {
                            system("cls");
                            if (Convertion(1, points) == 1)
                            {
                                SetConsoleTextAttribute(hConsole, 15);
                                cout << " > ";
                                SetConsoleTextAttribute(hConsole, 1);
                                cout << "Settings";
                                SetConsoleTextAttribute(hConsole, 15);
                                cout << " > ";
                                SetConsoleTextAttribute(hConsole, 1);
                                cout << "Game options\n";
                                SetConsoleTextAttribute(hConsole, 15);
                            }
                            if (Convertion(2, points) == 1) { SetConsoleTextAttribute(hConsole, 15); cout << "<- Go back\n"; SetConsoleTextAttribute(hConsole, 4); cout << "Enter 0\n"; }
                            SetConsoleTextAttribute(hConsole, 15);
                            if (Convertion(6, points) != 1)
                            {
                                SetConsoleTextAttribute(hConsole, 15); cPos.X = 10; cPos.Y = 3; SetConsoleCursorPosition(hConsole, cPos);
                                cout << " ##  # #  #   ##  #  #     ##    ##  ###  # #  ## "; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                                cout << "# #  # #     #    # #     #     #     #   # #  # #"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                                cout << " ##  # #  #  #    ##       ##    ##   #   # #  ## "; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                                cout << "  #  # #  #  #    # #        #  #     #   # #  #  "; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                                cout << "  #   #   #   ##  #  #     ##    ##   #    #   #  ";
                                SetConsoleTextAttribute(hConsole, 4); cPos.X += 20; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); cout << "Enter 1";
                                SetConsoleTextAttribute(hConsole, 15); cPos.X = 70; cPos.Y = 3; SetConsoleCursorPosition(hConsole, cPos);
                                cout << " ##   #   #   #  ###  ##    #   #"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                                cout << "#    # #  ##  #   #   # #  # #  #"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                                cout << "#    # #  # # #   #   ##   # #  #"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                                cout << "#    # #  #  ##   #   # #  # #  #"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                                cout << " ##   #   #   #   #   # #   #   #";
                                SetConsoleTextAttribute(hConsole, 4); cPos.X += 15; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); cout << "Enter 8";
                                SetConsoleTextAttribute(hConsole, 15); cPos.X = 115; cPos.Y = 3; SetConsoleCursorPosition(hConsole, cPos);
                                cout << " ##    #   # #   ##"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                                cout << "#     # #  # #  #  "; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                                cout << " ##   ###  # #   ##"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                                cout << "   #  # #  # #  #  "; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                                cout << " ##   # #   #    ##";
                                SetConsoleTextAttribute(hConsole, 4); cPos.X += 10; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); cout << "Enter 15";
                                // first
                                SetConsoleTextAttribute(hConsole, 15); cPos.X = 0; cPos.Y = 10; SetConsoleCursorPosition(hConsole, cPos);
                                cout << "#   #   #   ## \n";
                                cout << "## ##  # #  # #\n";
                                cout << "# # #  ###  ## \n";
                                cout << "#   #  # #  #  \n";
                                cout << "#   #  # #  #  \n";
                                SetConsoleTextAttribute(hConsole, 4);
                                cout << "Enter 2\n";
                                SetConsoleTextAttribute(hConsole, 15); cPos.X = 0; cPos.Y = 16; SetConsoleCursorPosition(hConsole, cPos);
                                cout << " ##   #   #   #   ## \n";
                                cout << "#    # #  #  # #  # #\n";
                                cout << "#    # #  #  # #  ## \n";
                                cout << "#    # #  #  # #  # #\n";
                                cout << " ##   #   #   #   # #\n";
                                SetConsoleTextAttribute(hConsole, 4);
                                cout << "Enter 3\n";
                                SetConsoleTextAttribute(hConsole, 15); cPos.X = 0; cPos.Y = 22; SetConsoleCursorPosition(hConsole, cPos);
                                cout << " ##   ##    ##   ##  ## \n";
                                cout << "#     # #  #    #    # #\n";
                                cout << " ##   ##    ##   ##  # #\n";
                                cout << "   #  #    #    #    # #\n";
                                cout << " ##   #     ##   ##  ## \n";
                                SetConsoleTextAttribute(hConsole, 4);
                                cout << "Enter 4\n";
                                SetConsoleTextAttribute(hConsole, 15); cPos.X = 0; cPos.Y = 28; SetConsoleCursorPosition(hConsole, cPos);
                                cout << " #   ##    ##   ###   #    ##  #   ##   ## \n";
                                cout << "# #  # #  #      #   # #  #    #  #    #   \n";
                                cout << "# #  ##    ##    #   ###  #    #   ##   ## \n";
                                cout << "# #  # #     #   #   # #  #    #  #       #\n";
                                cout << " #   ##    ##    #   # #   ##  #   ##   ## \n";
                                SetConsoleTextAttribute(hConsole, 4);
                                cout << "Enter 5\n";
                                SetConsoleTextAttribute(hConsole, 15); cPos.X = 0; cPos.Y = 34; SetConsoleCursorPosition(hConsole, cPos);
                                cout << " ##  # #   #   #  #  ###  # #\n";
                                cout << "# #  # #  # #  #      #   # #\n";
                                cout << " ##  # #  ###  #  #   #    #\n";
                                cout << "  #  # #  # #  #  #   #    #\n";
                                cout << "  #   #   # #  #  #   #    #\n";
                                SetConsoleTextAttribute(hConsole, 4);
                                cout << "Enter 6\n";
                                SetConsoleTextAttribute(hConsole, 15); cPos.X = 0; cPos.Y = 40; SetConsoleCursorPosition(hConsole, cPos);
                                cout << " #   ###  # #   ##  ## \n";
                                cout << "# #   #   # #  #    # #\n";
                                cout << "# #   #   ###   ##  ## \n";
                                cout << "# #   #   # #  #    # #\n";
                                cout << " #    #   # #   ##  # #\n";
                                SetConsoleTextAttribute(hConsole, 4);
                                cout << "Enter 7\n";
                                // second
                                SetConsoleTextAttribute(hConsole, 15); cPos.X = 50; cPos.Y = 10; SetConsoleCursorPosition(hConsole, cPos);
                                cout << " ##  #   ##   #   ## "; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                                cout << "#    #  #    # #  # #"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                                cout << "#    #   ##  ###  ## "; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                                cout << "#    #  #    # #  # #"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                                cout << " ##  #   ##  # #  # #"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                                SetConsoleTextAttribute(hConsole, 4); cout << "Enter 9";
                                SetConsoleTextAttribute(hConsole, 15); cPos.X = 50; cPos.Y = 16; SetConsoleCursorPosition(hConsole, cPos);
                                cout << " ##  ##    #   # #  #  ###  # #"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                                cout << "#    # #  # #  # #      #   # #"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                                cout << "# #  ##   ###  # #  #   #    # "; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                                cout << "# #  # #  # #  # #  #   #    # "; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                                cout << "##   # #  # #   #   #   #    # "; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                                SetConsoleTextAttribute(hConsole, 4); cout << "Enter 10";
                            }
                            else
                            {

                            }
                            // if player saves setting
                            if (S == "-3")
                            {
                                cout << "\nNow after re-entering the application, all settings will remain unchanged.\n";
                            }
                            while (S != "0")
                            {
                                cin >> S;
                                if (S == "1") { break; }
                                if (S == "2") { break; }
                                if (S == "3") { break; }
                                if (S == "4") { break; }
                                if (S == "5") { break; }
                                if (S == "6") { break; }
                                if (S == "7") { break; }
                                if (S == "8") { break; }
                                if (S == "9") { break; }
                                if (S == "10") { break; }
                                if (S == "11") { break; }
                                if (S == "12") { break; }
                            }
                            // Go back to menu
                            if (S == "0") { break; }
                            // Map
                            if (S == "2")
                            {
                                S = "-1";
                                while (S != "0")
                                {
                                    system("cls");
                                    if (Convertion(6, points) != 1)
                                    {
                                        if (Convertion(1, points) == 1)
                                        {
                                            SetConsoleTextAttribute(hConsole, 15);
                                            cout << " > ";
                                            SetConsoleTextAttribute(hConsole, 1);
                                            cout << "Settings";
                                            SetConsoleTextAttribute(hConsole, 15);
                                            cout << " > ";
                                            SetConsoleTextAttribute(hConsole, 1);
                                            cout << "Game options";
                                            SetConsoleTextAttribute(hConsole, 15);
                                            cout << " > ";
                                            SetConsoleTextAttribute(hConsole, 1);
                                            cout << "Map\n";
                                            SetConsoleTextAttribute(hConsole, 15);
                                        }
                                        if (Convertion(2, points) == 1) { SetConsoleTextAttribute(hConsole, 15); cout << "<- Go back\n"; SetConsoleTextAttribute(hConsole, 4); cout << "Enter 0\n"; }
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << "\n ##   #  ###   ##\n";
                                        cout << "#          #  #  \n";
                                        cout << " ##   #   #    ##\n";
                                        cout << "   #  #  #    #  \n";
                                        cout << " ##   #  ###   ##\n";
                                        SetConsoleTextAttribute(hConsole, 4);
                                        cout << "Enter 1\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << "##    #   ##   ##    ##  ## \n";
                                        cout << "# #  # #  # #  # #  #    # #\n";
                                        cout << "##   # #  ##   # #   ##  ## \n";
                                        cout << "# #  # #  # #  # #  #    # #\n";
                                        cout << "##    #   # #  ##    ##  # #\n";
                                        SetConsoleTextAttribute(hConsole, 4);
                                        cout << "Enter 2\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << " #   #  #   ##  #  #  #   #   ##  #  #  ###\n";
                                        cout << "# #  #     #    ## #  ## ##  #    ## #   # \n";
                                        cout << "###  #  #  # #  # ##  # # #   ##  # ##   # \n";
                                        cout << "# #  #  #  # #  #  #  #   #  #    #  #   # \n";
                                        cout << "# #  #  #  ##   #  #  #   #   ##  #  #   # \n";
                                        SetConsoleTextAttribute(hConsole, 4);
                                        cout << "Enter 3\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                    }
                                    else
                                    {
                                        if (Convertion(1, points) == 1)
                                        {
                                            SetConsoleTextAttribute(hConsole, 15);
                                            cout << " > ";
                                            SetConsoleTextAttribute(hConsole, 1);
                                            cout << "Настройки";
                                            SetConsoleTextAttribute(hConsole, 15);
                                            cout << " > ";
                                            SetConsoleTextAttribute(hConsole, 1);
                                            cout << "Карта\n";
                                            SetConsoleTextAttribute(hConsole, 15);
                                        }
                                        if (Convertion(2, points) == 1) { SetConsoleTextAttribute(hConsole, 15); cout << "<- Вернуться\n"; SetConsoleTextAttribute(hConsole, 4); cout << "Введите 0\n"; }
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << "\n##    #   ##   #   #   ##  ## \n";
                                        cout << "# #  # #    #  ## ##  #    # #\n";
                                        cout << "##   ###  ##   # # #   ##  ## \n";
                                        cout << "#    # #    #  #   #  #    #  \n";
                                        cout << "#    # #  ##   #   #   ##  #  \n";
                                        SetConsoleTextAttribute(hConsole, 4);
                                        cout << "Введите 1\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << "###  ##    #   # #  #  #  # #   #   #\n";
                                        cout << "#    # #  # #  # #  # ##  # #   #   #\n";
                                        cout << "#    ##   ###  ###  ## #  # #   ##  #\n";
                                        cout << "#    #    # #  # #  #  #  ###   # # #\n";
                                        cout << "#    #    # #  # #  #  #     #  ##  #\n";
                                        SetConsoleTextAttribute(hConsole, 4);
                                        cout << "Введите 2\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << "##   #   #  ##    #   ##   # #  #  #  ##    #   # #  #  #   ##\n";
                                        cout << "# #  #   #  # #  # #  # #  # #  # ##  # #  # #  # #  # ##  #  \n";
                                        cout << "##   ##  #  ##   ###  ##   ###  ## #  ##   ###  ###  ## #   ##\n";
                                        cout << "# #  # # #  #    # #  # #  # #  #  #  # #  # #  # #  #  #  #  \n";
                                        cout << "##   ##  #  #    # #  ##   # #  #  #  ##   # #  # #  #  #   ##\n";
                                        SetConsoleTextAttribute(hConsole, 4);
                                        cout << "Введите 3\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                    }
                                    while (S != "0")
                                    {
                                        cin >> S;
                                        if (S == "1") { break; }
                                        if (S == "2") { break; }
                                        if (S == "3") { break; }
                                    }
                                    if (S == "0") { break; }
                                    // size
                                    if (S == "1")
                                    {
                                        while (true)
                                        {
                                            system("cls");
                                            if (Convertion(6, points) != 1)
                                            {
                                                if (Convertion(1, points) == 1)
                                                {
                                                    SetConsoleTextAttribute(hConsole, 15);
                                                    cout << " > ";
                                                    SetConsoleTextAttribute(hConsole, 1);
                                                    cout << "Settings";
                                                    SetConsoleTextAttribute(hConsole, 15);
                                                    cout << " > ";
                                                    SetConsoleTextAttribute(hConsole, 1);
                                                    cout << "Game options";
                                                    SetConsoleTextAttribute(hConsole, 15);
                                                    cout << " > ";
                                                    SetConsoleTextAttribute(hConsole, 1);
                                                    cout << "Map";
                                                    SetConsoleTextAttribute(hConsole, 15);
                                                    cout << " > ";
                                                    SetConsoleTextAttribute(hConsole, 1);
                                                    cout << "Size\n";
                                                    SetConsoleTextAttribute(hConsole, 15);
                                                }
                                                if (Convertion(2, points) == 1) { SetConsoleTextAttribute(hConsole, 15); cout << "<- Go back\n"; SetConsoleTextAttribute(hConsole, 4); cout << "Enter 0\n"; }
                                                SetConsoleTextAttribute(hConsole, 15);
                                                cout << "\nEnter the length and height of the game map through enter: \n";
                                                cout << "Length: " << x << "\n";
                                                cout << "Height: " << y << "\n" << "\n";
                                                cout << "Current map size: \n";
                                            }
                                            else
                                            {
                                                if (Convertion(1, points) == 1)
                                                {
                                                    SetConsoleTextAttribute(hConsole, 15);
                                                    cout << " > ";
                                                    SetConsoleTextAttribute(hConsole, 1);
                                                    cout << "Настройки";
                                                    SetConsoleTextAttribute(hConsole, 15);
                                                    cout << " > ";
                                                    SetConsoleTextAttribute(hConsole, 1);
                                                    cout << "Карта";
                                                    SetConsoleTextAttribute(hConsole, 15);
                                                    cout << " > ";
                                                    SetConsoleTextAttribute(hConsole, 1);
                                                    cout << "Размер\n";
                                                    SetConsoleTextAttribute(hConsole, 15);
                                                }
                                                if (Convertion(2, points) == 1) { SetConsoleTextAttribute(hConsole, 15); cout << "<- Вернуться\n"; SetConsoleTextAttribute(hConsole, 4); cout << "Введите 0\n"; }
                                                SetConsoleTextAttribute(hConsole, 15);
                                                cout << "\nВведите длину и высоту игровой карты через Enter: \n";
                                                cout << "Текущая длина: " << x << "\n";
                                                cout << "Текущая высота: " << y << "\n" << "\n";
                                                cout << "Текущий размер карты: \n";
                                            }
                                            ShowMap();
                                            this_thread::sleep_for(chrono::milliseconds(200));
                                            try
                                            {
                                                if (Convertion(6, points) != 1)
                                                {
                                                    number = CheckInput("Length: \n");
                                                }
                                                else
                                                {
                                                    number = CheckInput("Длина: \n");
                                                }
                                                if (number == 0)
                                                {
                                                    break;
                                                }
                                                x = number;
                                                if (x > 209)
                                                {
                                                    x = 209;
                                                }
                                                if (x < 40)
                                                {
                                                    x = 40;
                                                }
                                            }
                                            catch (char const* ex)
                                            {
                                                cout << ex << "\n";
                                            }
                                            try
                                            {
                                                if (Convertion(6, points) != 1)
                                                {
                                                    number = CheckInput("Height : \n");
                                                }
                                                else
                                                {
                                                    number = CheckInput("Высота: \n");
                                                }
                                                if (number == 0)
                                                {
                                                    break;
                                                }
                                                y = number;
                                                if (y > 40)
                                                {
                                                    y = 40;
                                                }
                                                if (y < 7)
                                                {
                                                    y = 7;
                                                }
                                            }
                                            catch (char const* ex)
                                            {
                                                cout << ex << "\n";
                                            }
                                        }
                                    }
                                    // border
                                    if (S == "2")
                                    {
                                        while (S != "0")
                                        {
                                            S = "-1";
                                            system("cls");
                                            if (Convertion(6, points) != 1)
                                            {
                                                if (Convertion(1, points) == 1)
                                                {
                                                    SetConsoleTextAttribute(hConsole, 15);
                                                    cout << " > ";
                                                    SetConsoleTextAttribute(hConsole, 1);
                                                    cout << "Settings";
                                                    SetConsoleTextAttribute(hConsole, 15);
                                                    cout << " > ";
                                                    SetConsoleTextAttribute(hConsole, 1);
                                                    cout << "Game options";
                                                    SetConsoleTextAttribute(hConsole, 15);
                                                    cout << " > ";
                                                    SetConsoleTextAttribute(hConsole, 1);
                                                    cout << "Map";
                                                    SetConsoleTextAttribute(hConsole, 15);
                                                    cout << " > ";
                                                    SetConsoleTextAttribute(hConsole, 1);
                                                    cout << "Border\n";
                                                    SetConsoleTextAttribute(hConsole, 15);
                                                }
                                                if (Convertion(2, points) == 1) { SetConsoleTextAttribute(hConsole, 15); cout << "<- Go back\n"; SetConsoleTextAttribute(hConsole, 4); cout << "Enter 0\n"; }
                                                SetConsoleTextAttribute(hConsole, 15);
                                                cout << "\nLeft border: ";
                                                if (Left == 0) { SetConsoleTextAttribute(hConsole, 4); cout << "OFF\n"; }
                                                else { SetConsoleTextAttribute(hConsole, 2); cout << "ON\n"; SetConsoleTextAttribute(hConsole, 4); }
                                                cout << "Enter 1\n";
                                                SetConsoleTextAttribute(hConsole, 15);
                                                cout << "Right border: ";
                                                if (Right == 0) { SetConsoleTextAttribute(hConsole, 4); cout << "OFF\n"; }
                                                else { SetConsoleTextAttribute(hConsole, 2); cout << "ON\n"; SetConsoleTextAttribute(hConsole, 4); }
                                                cout << "Enter 2\n";
                                                SetConsoleTextAttribute(hConsole, 15);
                                                cout << "Down border: ";
                                                if (Down == 0) { SetConsoleTextAttribute(hConsole, 4); cout << "OFF\n"; }
                                                else { SetConsoleTextAttribute(hConsole, 2); cout << "ON\n"; SetConsoleTextAttribute(hConsole, 4); }
                                                cout << "Enter 3\n";
                                                SetConsoleTextAttribute(hConsole, 15);
                                                cout << "Up border: ";
                                                if (Up == 0) { SetConsoleTextAttribute(hConsole, 4); cout << "OFF\n"; }
                                                else { SetConsoleTextAttribute(hConsole, 2); cout << "ON\n"; SetConsoleTextAttribute(hConsole, 4); }
                                                cout << "Enter 4\n";
                                                SetConsoleTextAttribute(hConsole, 15);
                                                cout << "Map:\n";
                                            }
                                            else
                                            {
                                                if (Convertion(1, points) == 1)
                                                {
                                                    SetConsoleTextAttribute(hConsole, 15);
                                                    cout << " > ";
                                                    SetConsoleTextAttribute(hConsole, 1);
                                                    cout << "Настройки";
                                                    SetConsoleTextAttribute(hConsole, 15);
                                                    cout << " > ";
                                                    SetConsoleTextAttribute(hConsole, 1);
                                                    cout << "Карта";
                                                    SetConsoleTextAttribute(hConsole, 15);
                                                    cout << " > ";
                                                    SetConsoleTextAttribute(hConsole, 1);
                                                    cout << "Границы\n";
                                                    SetConsoleTextAttribute(hConsole, 15);
                                                }
                                                if (Convertion(2, points) == 1) { SetConsoleTextAttribute(hConsole, 15); cout << "<- Вернуться\n"; SetConsoleTextAttribute(hConsole, 4); cout << "Введите 0\n"; }
                                                SetConsoleTextAttribute(hConsole, 15);
                                                cout << "\nЛевая граница: ";
                                                if (Left == 0) { SetConsoleTextAttribute(hConsole, 4); cout << "ВЫКЛ\n"; }
                                                else { SetConsoleTextAttribute(hConsole, 2); cout << "ВКЛ\n"; SetConsoleTextAttribute(hConsole, 4); }
                                                cout << "Введите 1\n";
                                                SetConsoleTextAttribute(hConsole, 15);
                                                cout << "Правая граница: ";
                                                if (Right == 0) { SetConsoleTextAttribute(hConsole, 4); cout << "ВЫКЛ\n"; }
                                                else { SetConsoleTextAttribute(hConsole, 2); cout << "ВКЛ\n"; SetConsoleTextAttribute(hConsole, 4); }
                                                cout << "Введите 2\n";
                                                SetConsoleTextAttribute(hConsole, 15);
                                                cout << "Нижняя граница: ";
                                                if (Down == 0) { SetConsoleTextAttribute(hConsole, 4); cout << "ВЫКЛ\n"; }
                                                else { SetConsoleTextAttribute(hConsole, 2); cout << "ВКЛ\n"; SetConsoleTextAttribute(hConsole, 4); }
                                                cout << "Введите 3\n";
                                                SetConsoleTextAttribute(hConsole, 15);
                                                cout << "Верхняя граница: ";
                                                if (Up == 0) { SetConsoleTextAttribute(hConsole, 4); cout << "ВЫКЛ\n"; }
                                                else { SetConsoleTextAttribute(hConsole, 2); cout << "ВКЛ\n"; SetConsoleTextAttribute(hConsole, 4); }
                                                cout << "Введите 4\n";
                                                SetConsoleTextAttribute(hConsole, 15);
                                                cout << "Текущие границы карты:\n";
                                            }
                                            if (Up != 0) { SetConsoleTextAttribute(hConsole, Up); for (int u = 0; u < 40; u++) { cout << "#"; } cout << "\n"; }
                                            else { cout << "\n"; }
                                            for (int i = 0; i < 7; i++)
                                            {
                                                if (Left != 0) { SetConsoleTextAttribute(hConsole, Left); cout << "#"; }
                                                else { cout << " "; }
                                                for (int j = 0; j < 38; j++)
                                                {
                                                    cout << " ";
                                                    if (i == 5 || i == 6)
                                                    {
                                                        cout << "   *"; for (int r = 0; r < 32; r++) { cout << " "; } cout << "$"; break;
                                                    }
                                                }
                                                if (Right != 0) { SetConsoleTextAttribute(hConsole, Right); cout << "#"; }
                                                cout << "\n";
                                            }
                                            if (Down != 0) { SetConsoleTextAttribute(hConsole, Down); for (int i = 0; i < 40; i++) { cout << "#"; } cout << "\n"; }
                                            while (S != "0")
                                            {
                                                cin >> S;
                                                if (S == "1") { break; } if (S == "2") { break; } if (S == "3") { break; } if (S == "4") { break; }
                                            }
                                            if (S == "1" && Left == 0) { Left = 15; S = "-1"; }
                                            if (S == "1" && Left == 15) { Left = 0; S = "-1"; }
                                            if (S == "2" && Right == 0) { Right = 15; S = "-1"; }
                                            if (S == "2" && Right == 15) { Right = 0; S = "-1"; }
                                            if (S == "3" && Down == 0) { Down = 15; S = "-1"; }
                                            if (S == "3" && Down == 15) { Down = 0; S = "-1"; }
                                            if (S == "4" && Up == 0) { Up = 15; S = "-1"; }
                                            if (S == "4" && Up == 15) { Up = 0; S = "-1"; }
                                        }
                                        S = "-1";
                                    }
                                    // alignment
                                    if (S == "3")
                                    {
                                        while (S != "0")
                                        {
                                            system("cls");
                                            if (Convertion(6, points) != 1)
                                            {
                                                if (Convertion(1, points) == 1)
                                                {
                                                    SetConsoleTextAttribute(hConsole, 15);
                                                    cout << " > ";
                                                    SetConsoleTextAttribute(hConsole, 1);
                                                    cout << "Settings";
                                                    SetConsoleTextAttribute(hConsole, 15);
                                                    cout << " > ";
                                                    SetConsoleTextAttribute(hConsole, 1);
                                                    cout << "Game options";
                                                    SetConsoleTextAttribute(hConsole, 15);
                                                    cout << " > ";
                                                    SetConsoleTextAttribute(hConsole, 1);
                                                    cout << "Map";
                                                    SetConsoleTextAttribute(hConsole, 15);
                                                    cout << " > ";
                                                    SetConsoleTextAttribute(hConsole, 1);
                                                    cout << "Alignment\n";
                                                    SetConsoleTextAttribute(hConsole, 15);
                                                }
                                                if (Convertion(2, points) == 1) { SetConsoleTextAttribute(hConsole, 15); cout << "<- Go back\n"; SetConsoleTextAttribute(hConsole, 4); cout << "Enter 0\n"; }
                                                SetConsoleTextAttribute(hConsole, 15);
                                                cout << "\n# #   #   ##   #  ###   #   #  #  ###   #   #\n";
                                                cout << "# #  # #  # #       #  # #  ## #   #   # #  #\n";
                                                cout << "###  # #  ##   #   #   # #  # ##   #   ###  #\n";
                                                cout << "# #  # #  # #  #  #    # #  #  #   #   # #  #\n";
                                                cout << "# #   #   # #  #  ###   #   #  #   #   # #  #\n";
                                                SetConsoleTextAttribute(hConsole, 4);
                                                cout << "Enter 1\n";
                                                SetConsoleTextAttribute(hConsole, 15);
                                                cout << "# #   ##  ##   ###  #   ##   #   #\n";
                                                cout << "# #  #    # #   #      #    # #  #\n";
                                                cout << "# #   ##  ##    #   #  #    ###  #\n";
                                                cout << "# #  #    # #   #   #  #    # #  #\n";
                                                cout << " #    ##  # #   #   #   ##  # #  #\n";
                                                SetConsoleTextAttribute(hConsole, 4);
                                                cout << "Enter 2\n";
                                                SetConsoleTextAttribute(hConsole, 15);
                                            }
                                            else
                                            {
                                                if (Convertion(1, points) == 1)
                                                {
                                                    SetConsoleTextAttribute(hConsole, 15);
                                                    cout << " > ";
                                                    SetConsoleTextAttribute(hConsole, 1);
                                                    cout << "Настройки";
                                                    SetConsoleTextAttribute(hConsole, 15);
                                                    cout << " > ";
                                                    SetConsoleTextAttribute(hConsole, 1);
                                                    cout << "Карта";
                                                    SetConsoleTextAttribute(hConsole, 15);
                                                    cout << " > ";
                                                    SetConsoleTextAttribute(hConsole, 1);
                                                    cout << "Смещение\n";
                                                    SetConsoleTextAttribute(hConsole, 15);
                                                }
                                                if (Convertion(2, points) == 1) { SetConsoleTextAttribute(hConsole, 15); cout << "<- Вернуться\n"; SetConsoleTextAttribute(hConsole, 4); cout << "Введите 0\n"; }
                                                SetConsoleTextAttribute(hConsole, 15);
                                                cout << "###   #   ##   #  #  ##    #   # #  ###   #    #   #    # #   #    ##\n";
                                                cout << "#    # #  # #  # ##    #  # #  # #   #   # #  # #  #    # #  # #  #  \n";
                                                cout << "#    # #  ##   ## #  ##   # #  ###   #   ###  # #  ##   ###  # #   ##\n";
                                                cout << "#    # #  #    #  #    #  # #  # #   #   # #  # #  # #  # #  # #  #  \n";
                                                cout << "#     #   #    #  #  ##    #   # #   #   # #  # #  ##   # #   #    ##\n";
                                                SetConsoleTextAttribute(hConsole, 4);
                                                cout << "Введите 1\n";
                                                SetConsoleTextAttribute(hConsole, 15);
                                                cout << "##    ##  ##   ###  #  #  #  #   #    #   #    # #   #    ##\n";
                                                cout << "# #  #    # #   #   # ##  # #   # #  # #  #    # #  # #  #  \n";
                                                cout << "##    ##  ##    #   ## #  ##    ###  # #  ##   ###  # #   ##\n";
                                                cout << "# #  #    #     #   #  #  # #   # #  # #  # #  # #  # #  #  \n";
                                                cout << "##    ##  #     #   #  #  #  #  # #  # #  ##   # #   #    ##\n";
                                                SetConsoleTextAttribute(hConsole, 4);
                                                cout << "Введите 2\n";
                                                SetConsoleTextAttribute(hConsole, 15);
                                            }
                                            while (true)
                                            {
                                                cin >> S; if (S == "0") { break; } if (S == "1") { break; } if (S == "2") { break; }
                                            }
                                            // Horizontal
                                            if (S == "1")
                                            {
                                                while (S != "0")
                                                {
                                                    S = "-1";
                                                    system("cls");
                                                    if (Convertion(6, points) != 1)
                                                    {
                                                        if (Convertion(1, points) == 1)
                                                        {
                                                            SetConsoleTextAttribute(hConsole, 15);
                                                            cout << " > ";
                                                            SetConsoleTextAttribute(hConsole, 1);
                                                            cout << "Settings";
                                                            SetConsoleTextAttribute(hConsole, 15);
                                                            cout << " > ";
                                                            SetConsoleTextAttribute(hConsole, 1);
                                                            cout << "Game options";
                                                            SetConsoleTextAttribute(hConsole, 15);
                                                            cout << " > ";
                                                            SetConsoleTextAttribute(hConsole, 1);
                                                            cout << "Map";
                                                            SetConsoleTextAttribute(hConsole, 15);
                                                            cout << " > ";
                                                            SetConsoleTextAttribute(hConsole, 1);
                                                            cout << "Alignment";
                                                            SetConsoleTextAttribute(hConsole, 15);
                                                            cout << " > ";
                                                            SetConsoleTextAttribute(hConsole, 1);
                                                            cout << "Horizontal\n";
                                                        }
                                                        if (Convertion(2, points) == 1) { SetConsoleTextAttribute(hConsole, 15); cout << "<- Go back\n"; SetConsoleTextAttribute(hConsole, 4); cout << "Enter 0\n"; }
                                                        SetConsoleTextAttribute(hConsole, 15);
                                                        cout << "Enter '-1' to place the map in the middle\n";
                                                        cout << "Enter '-2' to place the map to the right\n";
                                                        cout << "Enter '-3' to place the map to the left\n";
                                                    }
                                                    else
                                                    {
                                                        if (Convertion(1, points) == 1)
                                                        {
                                                            SetConsoleTextAttribute(hConsole, 15);
                                                            cout << " > ";
                                                            SetConsoleTextAttribute(hConsole, 1);
                                                            cout << "Настройки";
                                                            SetConsoleTextAttribute(hConsole, 15);
                                                            cout << " > ";
                                                            SetConsoleTextAttribute(hConsole, 1);
                                                            cout << "Карта";
                                                            SetConsoleTextAttribute(hConsole, 15);
                                                            cout << " > ";
                                                            SetConsoleTextAttribute(hConsole, 1);
                                                            cout << "Смещение";
                                                            SetConsoleTextAttribute(hConsole, 15);
                                                            cout << " > ";
                                                            SetConsoleTextAttribute(hConsole, 1);
                                                            cout << "Горизонтальное\n";
                                                        }
                                                        if (Convertion(2, points) == 1) { SetConsoleTextAttribute(hConsole, 15); cout << "<- Вернуться\n"; SetConsoleTextAttribute(hConsole, 4); cout << "Введите 0\n"; }
                                                        SetConsoleTextAttribute(hConsole, 15);
                                                        cout << "Введите '-1', чтобы поместить карту в центр\n";
                                                        cout << "Введите '-2', чтобы поместить карту вправо\n";
                                                        cout << "Введите '-3', чтобы поместить карту влево\n";
                                                    }
                                                    for (int i = 0; i < V_indent - 7; i++) { cout << "\n"; }
                                                    for (int i = 0; i < H_indent; i++) { cout << " "; }
                                                    SetConsoleTextAttribute(hConsole, Up);
                                                    for (int i = 0; i < (x + 2); i++) { cout << "#"; } cout << "\n";
                                                    for (int i = 0; i < y; i++)
                                                    {
                                                        for (int s = 0; s < H_indent; s++) { cout << " "; }
                                                        SetConsoleTextAttribute(hConsole, Left);
                                                        cout << "#";
                                                        SetConsoleTextAttribute(hConsole, 15);
                                                        for (int j = 0; j < x; j++)
                                                        {
                                                            if ((i == y - 2 || i == y - 1) && (j == 3))
                                                            {
                                                                SetConsoleTextAttribute(hConsole, PlayerColor); cout << "*"; j++;
                                                            }
                                                            cout << " ";
                                                        }
                                                        SetConsoleTextAttribute(hConsole, Right);
                                                        cout << "#\n";
                                                    }
                                                    for (int i = 0; i < H_indent; i++) { cout << " "; }
                                                    SetConsoleTextAttribute(hConsole, Down);
                                                    for (int i = 0; i < (x + 2); i++) { cout << "#"; } cout << "\n";
                                                    if (Convertion(6, points) != 1)
                                                    {
                                                        cout << "Enter map offset to the right:\n";
                                                    }
                                                    else
                                                    {
                                                        cout << "Введите смещение карты вправо:\n";
                                                    }
                                                    number = 1;
                                                    number = CheckInput(" "); if (number == 0) { break; }
                                                    else { H_indent = number; } if (H_indent == -3) { H_indent = 0; } if (H_indent == -2) { H_indent = 209 - x; } if (H_indent == -1) { H_indent = (209 - x) / 2; }
                                                    if (H_indent < 0) { H_indent = 0; } if (H_indent > (209 - x)) { H_indent = 209 - x; }
                                                }
                                            }
                                            // Vertical
                                            if (S == "2")
                                            {
                                                while (S != "0")
                                                {
                                                    S = "-1";
                                                    system("cls");
                                                    if (Convertion(6, points) != 1)
                                                    {
                                                        if (Convertion(1, points) == 1)
                                                        {
                                                            SetConsoleTextAttribute(hConsole, 15);
                                                            cout << " > ";
                                                            SetConsoleTextAttribute(hConsole, 1);
                                                            cout << "Settings";
                                                            SetConsoleTextAttribute(hConsole, 15);
                                                            cout << " > ";
                                                            SetConsoleTextAttribute(hConsole, 1);
                                                            cout << "Game options";
                                                            SetConsoleTextAttribute(hConsole, 15);
                                                            cout << " > ";
                                                            SetConsoleTextAttribute(hConsole, 1);
                                                            cout << "Map";
                                                            SetConsoleTextAttribute(hConsole, 15);
                                                            cout << " > ";
                                                            SetConsoleTextAttribute(hConsole, 1);
                                                            cout << "Alignment";
                                                            SetConsoleTextAttribute(hConsole, 15);
                                                            cout << " > ";
                                                            SetConsoleTextAttribute(hConsole, 1);
                                                            cout << "Vertical\n";
                                                        }
                                                        if (Convertion(2, points) == 1) { SetConsoleTextAttribute(hConsole, 15); cout << "<- Go back\n"; SetConsoleTextAttribute(hConsole, 4); cout << "Enter 0\n"; }
                                                        SetConsoleTextAttribute(hConsole, 15);
                                                        cout << "Enter '-1' to place the map in the middle\n";
                                                        cout << "Enter '-2' to place the map to the up\n";
                                                        cout << "Enter '-3' to place the map to the down\n";
                                                    }
                                                    else
                                                    {
                                                        if (Convertion(1, points) == 1)
                                                        {
                                                            SetConsoleTextAttribute(hConsole, 15);
                                                            cout << " > ";
                                                            SetConsoleTextAttribute(hConsole, 1);
                                                            cout << "Настройки";
                                                            SetConsoleTextAttribute(hConsole, 15);
                                                            cout << " > ";
                                                            SetConsoleTextAttribute(hConsole, 1);
                                                            cout << "Карта";
                                                            SetConsoleTextAttribute(hConsole, 15);
                                                            cout << " > ";
                                                            SetConsoleTextAttribute(hConsole, 1);
                                                            cout << "Смещение";
                                                            SetConsoleTextAttribute(hConsole, 15);
                                                            cout << " > ";
                                                            SetConsoleTextAttribute(hConsole, 1);
                                                            cout << "Вертикальное\n";
                                                        }
                                                        if (Convertion(2, points) == 1) { SetConsoleTextAttribute(hConsole, 15); cout << "<- Вернуться\n"; SetConsoleTextAttribute(hConsole, 4); cout << "Введите 0\n"; }
                                                        SetConsoleTextAttribute(hConsole, 15);
                                                        cout << "Введите '-1', чтобы поместить карту в центр\n";
                                                        cout << "Введите '-2', чтобы поместить карту вверх\n";
                                                        cout << "Введите '-3', чтобы поместить карту вниз\n";
                                                    }
                                                    for (int i = 0; i < V_indent - 7; i++) { cout << "\n"; }
                                                    for (int i = 0; i < H_indent; i++) { cout << " "; }
                                                    SetConsoleTextAttribute(hConsole, Up);
                                                    for (int i = 0; i < (x + 2); i++) { cout << "#"; } cout << "\n";
                                                    for (int i = 0; i < y; i++)
                                                    {
                                                        for (int s = 0; s < H_indent; s++) { cout << " "; }
                                                        SetConsoleTextAttribute(hConsole, Left);
                                                        cout << "#";
                                                        SetConsoleTextAttribute(hConsole, 15);
                                                        for (int j = 0; j < x; j++)
                                                        {
                                                            if ((i == y - 2 || i == y - 1) && (j == 3))
                                                            {
                                                                SetConsoleTextAttribute(hConsole, PlayerColor); cout << "*"; j++;
                                                            }
                                                            cout << " ";
                                                        }
                                                        SetConsoleTextAttribute(hConsole, Right);
                                                        cout << "#\n";
                                                    }
                                                    for (int i = 0; i < H_indent; i++) { cout << " "; }
                                                    SetConsoleTextAttribute(hConsole, Down);
                                                    for (int i = 0; i < (x + 2); i++) { cout << "#"; } cout << "\n";
                                                    if (Convertion(6, points) != 1)
                                                    {
                                                        cout << "Enter map offset to the down:\n";
                                                    }
                                                    else
                                                    {
                                                        cout << "Введите смещение карты вниз:\n";
                                                    }
                                                    number = 1;
                                                    number = CheckInput(" "); if (number == 0) { break; }
                                                    else { V_indent = number; } if (V_indent == -1) { V_indent = (43 - y) / 2; } if (V_indent == -2) { V_indent = 0; } if (V_indent == -3) { V_indent = 43 - y; }
                                                    if (V_indent < 0) { V_indent = 0; } if (V_indent > 43 - y) { V_indent = 43 - y; }
                                                }
                                            }
                                        }
                                        number = 0; S = "-1";
                                    }
                                    S = "-1";
                                }
                                S = "-1";
                            }
                            // Color setting
                            if (S == "3")
                            {
                                S = "-1"; short k = 0;
                                while (S != "0")
                                {
                                    system("cls");
                                    if (Convertion(6, points) != 1)
                                    {
                                        if (Convertion(1, points) == 1)
                                        {
                                            SetConsoleTextAttribute(hConsole, 15);
                                            cout << " > ";
                                            SetConsoleTextAttribute(hConsole, 1);
                                            cout << "Settings";
                                            SetConsoleTextAttribute(hConsole, 15);
                                            cout << " > ";
                                            SetConsoleTextAttribute(hConsole, 1);
                                            cout << "Color\n";
                                            SetConsoleTextAttribute(hConsole, 15);
                                        }
                                        else { cout << "\n"; }
                                        if (Convertion(2, points) == 1) { SetConsoleTextAttribute(hConsole, 15); cout << "<- Go back\n"; SetConsoleTextAttribute(hConsole, 4); cout << "Enter 0\n"; }
                                        else { cout << "\n\n"; }
                                        SetConsoleTextAttribute(hConsole, 15); if (S == "1" && Left != 0) { SetConsoleTextAttribute(hConsole, 1); } cout << "\nLeft map border"; SetConsoleTextAttribute(hConsole, 15);
                                        if (k == 10) { for (int i = 0; i < 10; i++) { cout << " "; } SetConsoleTextAttribute(hConsole, 15); cout << "- "; SetConsoleTextAttribute(hConsole, 1); cout << "Blue"; SetConsoleTextAttribute(hConsole, 15); cout << " - 1"; }
                                        SetConsoleTextAttribute(hConsole, 4); cout << "\nEnter 1";
                                        if (k == 10) { for (int i = 0; i < 18; i++) { cout << " "; } SetConsoleTextAttribute(hConsole, 15); cout << "- "; SetConsoleTextAttribute(hConsole, 2); cout << "Green"; SetConsoleTextAttribute(hConsole, 15); cout << " - 2"; }
                                        SetConsoleTextAttribute(hConsole, 15); if (S == "2" && Right != 0) { SetConsoleTextAttribute(hConsole, 1); } cout << "\nRight map border"; SetConsoleTextAttribute(hConsole, 15);
                                        if (k == 10) { for (int i = 0; i < 9; i++) { cout << " "; } SetConsoleTextAttribute(hConsole, 15); cout << "- "; SetConsoleTextAttribute(hConsole, 3); cout << "Cyan"; SetConsoleTextAttribute(hConsole, 15); cout << " - 3"; }
                                        SetConsoleTextAttribute(hConsole, 4); cout << "\nEnter 2";
                                        if (k == 10) { for (int i = 0; i < 18; i++) { cout << " "; } SetConsoleTextAttribute(hConsole, 15); cout << "- "; SetConsoleTextAttribute(hConsole, 4); cout << "Red"; SetConsoleTextAttribute(hConsole, 15); cout << " - 4"; }
                                        SetConsoleTextAttribute(hConsole, 15); if (S == "3" && Down != 0) { SetConsoleTextAttribute(hConsole, 1); } cout << "\nDown map border"; SetConsoleTextAttribute(hConsole, 15);
                                        if (k == 10) { for (int i = 0; i < 10; i++) { cout << " "; } SetConsoleTextAttribute(hConsole, 15); cout << "- "; SetConsoleTextAttribute(hConsole, 5); cout << "Magenta"; SetConsoleTextAttribute(hConsole, 15); cout << " - 5"; }
                                        SetConsoleTextAttribute(hConsole, 4); cout << "\nEnter 3";
                                        if (k == 10) { for (int i = 0; i < 18; i++) { cout << " "; } SetConsoleTextAttribute(hConsole, 15); cout << "- "; SetConsoleTextAttribute(hConsole, 6); cout << "Brown"; SetConsoleTextAttribute(hConsole, 15); cout << " - 6"; }
                                        SetConsoleTextAttribute(hConsole, 15); if (S == "4" && Up != 0) { SetConsoleTextAttribute(hConsole, 1); } cout << "\nUp map border"; SetConsoleTextAttribute(hConsole, 4);
                                        if (k == 10) { for (int i = 0; i < 12; i++) { cout << " "; } SetConsoleTextAttribute(hConsole, 15); cout << "- "; SetConsoleTextAttribute(hConsole, 7); cout << "Lightgray"; SetConsoleTextAttribute(hConsole, 15); cout << " - 7"; }
                                        SetConsoleTextAttribute(hConsole, 4); cout << "\nEnter 4";
                                        if (k == 10) { for (int i = 0; i < 18; i++) { cout << " "; } SetConsoleTextAttribute(hConsole, 15); cout << "- "; SetConsoleTextAttribute(hConsole, 8); cout << "Darkgray"; SetConsoleTextAttribute(hConsole, 15); cout << " - 8"; }
                                        SetConsoleTextAttribute(hConsole, 15); if (S == "5") { SetConsoleTextAttribute(hConsole, 1); } cout << "\nPlayer"; SetConsoleTextAttribute(hConsole, 4);
                                        if (k == 10) { for (int i = 0; i < 19; i++) { cout << " "; } SetConsoleTextAttribute(hConsole, 15); cout << "- "; SetConsoleTextAttribute(hConsole, 9); cout << "Lightblue"; SetConsoleTextAttribute(hConsole, 15); cout << " - 9"; }
                                        SetConsoleTextAttribute(hConsole, 4); cout << "\nEnter 5";
                                        if (k == 10) { for (int i = 0; i < 18; i++) { cout << " "; } SetConsoleTextAttribute(hConsole, 15); cout << "- "; SetConsoleTextAttribute(hConsole, 10); cout << "Lightgreen"; SetConsoleTextAttribute(hConsole, 15); cout << " - 10"; }
                                        SetConsoleTextAttribute(hConsole, 15); if (S == "6") { SetConsoleTextAttribute(hConsole, 1); }cout << "\nObstacles"; SetConsoleTextAttribute(hConsole, 4);
                                        if (k == 10) { for (int i = 0; i < 16; i++) { cout << " "; } SetConsoleTextAttribute(hConsole, 15); cout << "- "; SetConsoleTextAttribute(hConsole, 11); cout << "Lightcyan"; SetConsoleTextAttribute(hConsole, 15); cout << " - 11"; }
                                        SetConsoleTextAttribute(hConsole, 4); cout << "\nEnter 6";
                                        if (k == 10) { for (int i = 0; i < 18; i++) { cout << " "; } SetConsoleTextAttribute(hConsole, 15); cout << "- "; SetConsoleTextAttribute(hConsole, 12); cout << "Lightred"; SetConsoleTextAttribute(hConsole, 15); cout << " - 12"; }
                                        SetConsoleTextAttribute(hConsole, 15); if (S == "7") { SetConsoleTextAttribute(hConsole, 1); } cout << "\nScore indicator"; SetConsoleTextAttribute(hConsole, 4);
                                        if (k == 10) { for (int i = 0; i < 10; i++) { cout << " "; } SetConsoleTextAttribute(hConsole, 15); cout << "- "; SetConsoleTextAttribute(hConsole, 13); cout << "Lightmagenta"; SetConsoleTextAttribute(hConsole, 15); cout << " - 13"; }
                                        SetConsoleTextAttribute(hConsole, 4); cout << "\nEnter 7";
                                        if (k == 10) { for (int i = 0; i < 18; i++) { cout << " "; } SetConsoleTextAttribute(hConsole, 15); cout << "- "; SetConsoleTextAttribute(hConsole, 14); cout << "Yellow"; SetConsoleTextAttribute(hConsole, 15); cout << " - 14"; }
                                        SetConsoleTextAttribute(hConsole, 15); cout << "\nColor list"; SetConsoleTextAttribute(hConsole, 4);
                                        if (k == 10) { for (int i = 0; i < 15; i++) { cout << " "; } SetConsoleTextAttribute(hConsole, 15); cout << "- White - 15"; } SetConsoleTextAttribute(hConsole, 4);
                                        cout << "\nEnter 8\n\n";
                                        SetConsoleTextAttribute(hConsole, PointsIndicatorColor); cout << "Points: \n";
                                        ShowMap();
                                        cPos.Y = y + 21; cPos.X = x - 4; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, ObstaclesColor); cout << "$";
                                        cPos.Y = y + 22; cPos.X = x - 4; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, ObstaclesColor); cout << "$"; cPos.Y = y + 25; cPos.X = 0; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, 15);
                                        if (S == "1") { if (Left != 0) { cout << "Enter the number of color\n"; Left = CheckInput("\n"); if (Left <= 0 || Left > 15) { Left = 15; } S = "-9"; } else { cout << "\nYou cannot change the color of a removed map border\n"; } }
                                        if (S == "2") { if (Right != 0) { cout << "Enter the number of color\n"; Right = CheckInput("\n"); if (Right <= 0 || Right > 15) { Right = 15; } S = "-9"; } else { cout << "\nYou cannot change the color of a removed map border\n"; } }
                                        if (S == "3") { if (Down != 0) { cout << "Enter the number of color\n"; Down = CheckInput("\n"); if (Down <= 0 || Down > 15) { Down = 15; } S = "-9"; } else { cout << "\nYou cannot change the color of a removed map border\n"; } }
                                        if (S == "4") { if (Up != 0) { cout << "Enter the number of color\n"; Up = CheckInput("\n"); if (Up <= 0 || Up > 15) { Up = 15; } S = "-9"; } else { cout << "\nYou cannot change the color of a removed map border\n"; } }
                                        if (S == "5") { cout << "Enter the number of color\n"; PlayerColor = CheckInput("\n"); if (PlayerColor <= 0 || PlayerColor > 15) { PlayerColor = 15; } S = "-9"; }
                                        if (S == "6") { cout << "Enter the number of color\n"; ObstaclesColor = CheckInput("\n"); if (ObstaclesColor <= 0 || ObstaclesColor > 15) { ObstaclesColor = 15; } S = "-9"; }
                                        if (S == "7") { cout << "Enter the number of color\n"; PointsIndicatorColor = CheckInput("\n"); if (PointsIndicatorColor <= 0 || PointsIndicatorColor > 15) { PointsIndicatorColor = 15; } S = "-9"; }
                                    }
                                    else
                                    {
                                        if (Convertion(1, points) == 1)
                                        {
                                            SetConsoleTextAttribute(hConsole, 15);
                                            cout << " > ";
                                            SetConsoleTextAttribute(hConsole, 1);
                                            cout << "Настройки";
                                            SetConsoleTextAttribute(hConsole, 15);
                                            cout << " > ";
                                            SetConsoleTextAttribute(hConsole, 1);
                                            cout << "Цвет\n";
                                            SetConsoleTextAttribute(hConsole, 15);
                                        }
                                        else { cout << "\n"; }
                                        if (Convertion(2, points) == 1) { SetConsoleTextAttribute(hConsole, 15); cout << "<- Вернуться\n"; SetConsoleTextAttribute(hConsole, 4); cout << "Введите 0\n"; }
                                        else { cout << "\n\n"; }
                                        SetConsoleTextAttribute(hConsole, 15); if (S == "1" && Left != 0) { SetConsoleTextAttribute(hConsole, 1); } cout << "\nЛевая граница карты"; SetConsoleTextAttribute(hConsole, 15);
                                        if (k == 10) { for (int i = 0; i < 10; i++) { cout << " "; } SetConsoleTextAttribute(hConsole, 15); cout << "- "; SetConsoleTextAttribute(hConsole, 1); cout << "Синий"; SetConsoleTextAttribute(hConsole, 15); cout << " - 1"; }
                                        SetConsoleTextAttribute(hConsole, 4); cout << "\nВведите 1";
                                        if (k == 10) { for (int i = 0; i < 18; i++) { cout << " "; } SetConsoleTextAttribute(hConsole, 15); cout << "- "; SetConsoleTextAttribute(hConsole, 2); cout << "Зеленый"; SetConsoleTextAttribute(hConsole, 15); cout << " - 2"; }
                                        SetConsoleTextAttribute(hConsole, 15); if (S == "2" && Right != 0) { SetConsoleTextAttribute(hConsole, 1); } cout << "\nПравая граница карты"; SetConsoleTextAttribute(hConsole, 15);
                                        if (k == 10) { for (int i = 0; i < 9; i++) { cout << " "; } SetConsoleTextAttribute(hConsole, 15); cout << "- "; SetConsoleTextAttribute(hConsole, 3); cout << "Голубой"; SetConsoleTextAttribute(hConsole, 15); cout << " - 3"; }
                                        SetConsoleTextAttribute(hConsole, 4); cout << "\nВведите 2";
                                        if (k == 10) { for (int i = 0; i < 18; i++) { cout << " "; } SetConsoleTextAttribute(hConsole, 15); cout << "- "; SetConsoleTextAttribute(hConsole, 4); cout << "Красный"; SetConsoleTextAttribute(hConsole, 15); cout << " - 4"; }
                                        SetConsoleTextAttribute(hConsole, 15); if (S == "3" && Down != 0) { SetConsoleTextAttribute(hConsole, 1); } cout << "\nНижняя граница карты"; SetConsoleTextAttribute(hConsole, 15);
                                        if (k == 10) { for (int i = 0; i < 10; i++) { cout << " "; } SetConsoleTextAttribute(hConsole, 15); cout << "- "; SetConsoleTextAttribute(hConsole, 5); cout << "Пурпурный"; SetConsoleTextAttribute(hConsole, 15); cout << " - 5"; }
                                        SetConsoleTextAttribute(hConsole, 4); cout << "\nВведите 3";
                                        if (k == 10) { for (int i = 0; i < 18; i++) { cout << " "; } SetConsoleTextAttribute(hConsole, 15); cout << "- "; SetConsoleTextAttribute(hConsole, 6); cout << "Коричневый"; SetConsoleTextAttribute(hConsole, 15); cout << " - 6"; }
                                        SetConsoleTextAttribute(hConsole, 15); if (S == "4" && Up != 0) { SetConsoleTextAttribute(hConsole, 1); } cout << "\nВерхняя граница карты"; SetConsoleTextAttribute(hConsole, 4);
                                        if (k == 10) { for (int i = 0; i < 12; i++) { cout << " "; } SetConsoleTextAttribute(hConsole, 15); cout << "- "; SetConsoleTextAttribute(hConsole, 7); cout << "Светло-серый"; SetConsoleTextAttribute(hConsole, 15); cout << " - 7"; }
                                        SetConsoleTextAttribute(hConsole, 4); cout << "\nВведите 4";
                                        if (k == 10) { for (int i = 0; i < 18; i++) { cout << " "; } SetConsoleTextAttribute(hConsole, 15); cout << "- "; SetConsoleTextAttribute(hConsole, 8); cout << "Темно-серый"; SetConsoleTextAttribute(hConsole, 15); cout << " - 8"; }
                                        SetConsoleTextAttribute(hConsole, 15); if (S == "5") { SetConsoleTextAttribute(hConsole, 1); } cout << "\nигрока"; SetConsoleTextAttribute(hConsole, 4);
                                        if (k == 10) { for (int i = 0; i < 19; i++) { cout << " "; } SetConsoleTextAttribute(hConsole, 15); cout << "- "; SetConsoleTextAttribute(hConsole, 9); cout << "Светло-синий"; SetConsoleTextAttribute(hConsole, 15); cout << " - 9"; }
                                        SetConsoleTextAttribute(hConsole, 4); cout << "\nВведите 5";
                                        if (k == 10) { for (int i = 0; i < 18; i++) { cout << " "; } SetConsoleTextAttribute(hConsole, 15); cout << "- "; SetConsoleTextAttribute(hConsole, 10); cout << "Светло-зеленый"; SetConsoleTextAttribute(hConsole, 15); cout << " - 10"; }
                                        SetConsoleTextAttribute(hConsole, 15); if (S == "6") { SetConsoleTextAttribute(hConsole, 1); }cout << "\nПрепятствия"; SetConsoleTextAttribute(hConsole, 4);
                                        if (k == 10) { for (int i = 0; i < 16; i++) { cout << " "; } SetConsoleTextAttribute(hConsole, 15); cout << "- "; SetConsoleTextAttribute(hConsole, 11); cout << "Светло-голубой"; SetConsoleTextAttribute(hConsole, 15); cout << " - 11"; }
                                        SetConsoleTextAttribute(hConsole, 4); cout << "\nВведите 6";
                                        if (k == 10) { for (int i = 0; i < 18; i++) { cout << " "; } SetConsoleTextAttribute(hConsole, 15); cout << "- "; SetConsoleTextAttribute(hConsole, 12); cout << "Светло-красный"; SetConsoleTextAttribute(hConsole, 15); cout << " - 12"; }
                                        SetConsoleTextAttribute(hConsole, 15); if (S == "7") { SetConsoleTextAttribute(hConsole, 1); } cout << "\nИндикатор очков"; SetConsoleTextAttribute(hConsole, 4);
                                        if (k == 10) { for (int i = 0; i < 10; i++) { cout << " "; } SetConsoleTextAttribute(hConsole, 15); cout << "- "; SetConsoleTextAttribute(hConsole, 13); cout << "Светло-пурпурный"; SetConsoleTextAttribute(hConsole, 15); cout << " - 13"; }
                                        SetConsoleTextAttribute(hConsole, 4); cout << "\nВведите 7";
                                        if (k == 10) { for (int i = 0; i < 18; i++) { cout << " "; } SetConsoleTextAttribute(hConsole, 15); cout << "- "; SetConsoleTextAttribute(hConsole, 14); cout << "Желтый"; SetConsoleTextAttribute(hConsole, 15); cout << " - 14"; }
                                        SetConsoleTextAttribute(hConsole, 15); cout << "\nСписок цветов"; SetConsoleTextAttribute(hConsole, 4);
                                        if (k == 10) { for (int i = 0; i < 15; i++) { cout << " "; } SetConsoleTextAttribute(hConsole, 15); cout << "- Белый - 15"; } SetConsoleTextAttribute(hConsole, 4);
                                        cout << "\nВведите 8\n\n";
                                        SetConsoleTextAttribute(hConsole, PointsIndicatorColor); cout << "Очки: \n";
                                        ShowMap();
                                        cPos.Y = y + 21; cPos.X = x - 4; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, ObstaclesColor); cout << "$";
                                        cPos.Y = y + 22; cPos.X = x - 4; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, ObstaclesColor); cout << "$"; cPos.Y = y + 25; cPos.X = 0; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos); SetConsoleTextAttribute(hConsole, 15);
                                        if (S == "1") { if (Left != 0) { cout << "Введите номер цвета\n"; Left = CheckInput("\n"); if (Left <= 0 || Left > 15) { Left = 15; } S = "-9"; } else { cout << "\nВы не можете изменить цвет выключенного препятствия\n"; } }
                                        if (S == "2") { if (Right != 0) { cout << "Введите номер цвета\n"; Right = CheckInput("\n"); if (Right <= 0 || Right > 15) { Right = 15; } S = "-9"; } else { cout << "\nВы не можете изменить цвет выключенного препятствия\n"; } }
                                        if (S == "3") { if (Down != 0) { cout << "Введите номер цвета\n"; Down = CheckInput("\n"); if (Down <= 0 || Down > 15) { Down = 15; } S = "-9"; } else { cout << "\nВы не можете изменить цвет выключенного препятствия\n"; } }
                                        if (S == "4") { if (Up != 0) { cout << "Введите номер цвета\n"; Up = CheckInput("\n"); if (Up <= 0 || Up > 15) { Up = 15; } S = "-9"; } else { cout << "\nВы не можете изменить цвет выключенного препятствия\n"; } }
                                        if (S == "5") { cout << "Введите номер цвета\n"; PlayerColor = CheckInput("\n"); if (PlayerColor <= 0 || PlayerColor > 15) { PlayerColor = 15; } S = "-9"; }
                                        if (S == "6") { cout << "Введите номер цвета\n"; ObstaclesColor = CheckInput("\n"); if (ObstaclesColor <= 0 || ObstaclesColor > 15) { ObstaclesColor = 15; } S = "-9"; }
                                        if (S == "7") { cout << "Введите номер цвета\n"; PointsIndicatorColor = CheckInput("\n"); if (PointsIndicatorColor <= 0 || PointsIndicatorColor > 15) { PointsIndicatorColor = 15; } S = "-9"; }
                                    }
                                    while (S != "0")
                                    {
                                        if (S == "-9") { S = "-1"; break; }
                                        cin >> S; if (S == "0") { break; }
                                        if (S == "1") { break; } if (S == "2") { break; } if (S == "3") { break; } if (S == "4") { break; }
                                        if (S == "5") { break; } if (S == "6") { break; } if (S == "7") { break; } if (S == "8") { if (k == 0) { k = 10; } else { k = 0; } break; }
                                        S = "-1";
                                    }
                                    if (S == "0") { break; }
                                }
                                S = "-1"; k = 0;
                            }
                            // Speed settings
                            if (S == "4")
                            {
                                short k = 0;
                                while (S != "0")
                                {
                                    S = "-1"; system("cls");
                                    if (Convertion(6, points) != 1)
                                    {
                                        if (Convertion(1, points) == 1)
                                        {
                                            SetConsoleTextAttribute(hConsole, 15);
                                            cout << " > ";
                                            SetConsoleTextAttribute(hConsole, 1);
                                            cout << "Settings";
                                            SetConsoleTextAttribute(hConsole, 15);
                                            cout << " > ";
                                            SetConsoleTextAttribute(hConsole, 1);
                                            cout << "Speed\n";
                                            SetConsoleTextAttribute(hConsole, 15);
                                        }
                                        if (Convertion(2, points) == 1) { SetConsoleTextAttribute(hConsole, 15); cout << "<- Go back\n"; SetConsoleTextAttribute(hConsole, 4); cout << "Enter 0\n"; }
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << "\n ##   #   #  #   ##  ###   #   #  #  ###     ##   ##    ##   ##  ## \n";
                                        cout << "#    # #  ## #  #     #   # #  ## #   #     #     # #  #    #    # #\n";
                                        cout << "#    # #  # ##   ##   #   ###  # ##   #      ##   ##    ##   ##  # #\n";
                                        cout << "#    # #  #  #     #  #   # #  #  #   #        #  #    #    #    # #\n";
                                        cout << " ##   #   #  #   ##   #   # #  #  #   #      ##   #     ##   ##  ## \n";
                                        SetConsoleTextAttribute(hConsole, 4);
                                        cout << "Enter 1\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << "#  #  #   ##  ##    ##   #    ##   #  #  #   ##     ##   ##    ##   ##  ## \n";
                                        cout << "   ## #  #    # #  #    # #  #        ## #  #      #     # #  #    #    # #\n";
                                        cout << "#  # ##  #    ##    ##  ###   ##   #  # ##  # #     ##   ##    ##   ##  # #\n";
                                        cout << "#  #  #  #    # #  #    # #     #  #  #  #  # #       #  #    #    #    # #\n";
                                        cout << "#  #  #   ##  # #   ##  # #   ##   #  #  #  ###     ##   #     ##   ##  ## \n";
                                        SetConsoleTextAttribute(hConsole, 4);
                                        cout << "Enter 2\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << "###   #   #  #  #  #  #   ##     ##   ##    ##   ##  ## \n";
                                        cout << "#    # #  #  #     ## #  #      #     # #  #    #    # #\n";
                                        cout << "##   ###  #  #  #  # ##  # #     ##   ##    ##   ##  # #\n";
                                        cout << "#    # #  #  #  #  #  #  # #       #  #    #    #    # #\n";
                                        cout << "#    # #  #  #  #  #  #  ###     ##   #     ##   ##  ## \n";
                                        SetConsoleTextAttribute(hConsole, 4);
                                        cout << "Enter 3\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                    }
                                    else
                                    {
                                        if (Convertion(1, points) == 1)
                                        {
                                            SetConsoleTextAttribute(hConsole, 15);
                                            cout << " > ";
                                            SetConsoleTextAttribute(hConsole, 1);
                                            cout << "Настройка";
                                            SetConsoleTextAttribute(hConsole, 15);
                                            cout << " > ";
                                            SetConsoleTextAttribute(hConsole, 1);
                                            cout << "Скорость\n";
                                            SetConsoleTextAttribute(hConsole, 15);
                                        }
                                        if (Convertion(2, points) == 1) { SetConsoleTextAttribute(hConsole, 15); cout << "<- Вернуться\n"; SetConsoleTextAttribute(hConsole, 4); cout << "Введите 0\n"; }
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << "###   #    ##  ###   #    ##  # #  # #   #    ##      ##  #  #   #   ##    #    ##  ###  #  \n";
                                        cout << "# #  # #  #     #   # #  # #  # #  # #  # #  # #     #    # #   # #  # #  # #  #     #   #  \n";
                                        cout << "# #  # #  #     #   # #   ##  ###  ###  ###   ##     #    ##    # #  ##   # #  #     #   ## \n";
                                        cout << "# #  # #  #     #   # #  # #  # #  # #  # #  # #     #    # #   # #  #    # #  #     #   # #\n";
                                        cout << "# #   #    ##   #    #   # #  # #  # #  # #  # #      ##  #  #   #   #     #    ##   #   ## \n";
                                        SetConsoleTextAttribute(hConsole, 4);
                                        cout << "Введите 1\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << "##    #   ##   ##    #    ##  ###   #   #  #   # # #    #    ##      ##  #  #   #   ##    #    ##  ###  #  \n";
                                        cout << "# #  # #    #  # #  # #  #     #   # #  # # #  # # #   # #  # #     #    # #   # #  # #  # #  #     #   #  \n";
                                        cout << "##   # #  ##   ##   ###  #     #   ###  ### #  # # #   ###   ##     #    ##    # #  ##   # #  #     #   ## \n";
                                        cout << "# #  # #    #  #    # #  #     #   # #  # # #  # # #   # #  # #     #    # #   # #  #    # #  #     #   # #\n";
                                        cout << "##    #   ##   #    # #   ##   #   # #  #  #    # # #  # #  # #      ##  #  #   #   #     #    ##   #   ## \n";
                                        SetConsoleTextAttribute(hConsole, 4);
                                        cout << "Введите 2\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << "#   #  ###  #   #  ##    #   #  #   # # #    #    ##      ##  #  #   #   ##    #    ##  ###  #  \n";
                                        cout << " # #   #    #   #  # #  # #  # # #  # # #   # #  # #     #    # #   # #  # #  # #  #     #   #  \n";
                                        cout << "  #    ##   ##  #  ##   ###  ### #  # # #   ###   ##     #    ##    # #  ##   # #  #     #   ## \n";
                                        cout << " #     # #  # # #  # #  # #  # # #  # # #   # #  # #     #    # #   # #  #    # #  #     #   # #\n";
                                        cout << "#      ##   ##  #  ##   # #  #  #    # # #  # #  # #      ##  #  #   #   #     #    ##   #   ## \n";
                                        SetConsoleTextAttribute(hConsole, 4);
                                        cout << "Введите 3\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                    }
                                    while (S != "0")
                                    {
                                        cin >> S;
                                        if (S == "1") { break; }
                                        if (S == "2") { break; }
                                        if (S == "3") { break; }
                                    }
                                    if (S == "0")
                                    {
                                        S = "-1";
                                        break;
                                    }
                                    // constant speed
                                    if (S == "1")
                                    {
                                        S = "-1";
                                        system("cls");
                                        while (S != "0")
                                        {
                                            system("cls");
                                            if (Convertion(6, points) != 1)
                                            {
                                                if (Convertion(1, points) == 1)
                                                {
                                                    SetConsoleTextAttribute(hConsole, 15);
                                                    cout << " > ";
                                                    SetConsoleTextAttribute(hConsole, 1);
                                                    cout << "Settings";
                                                    SetConsoleTextAttribute(hConsole, 15);
                                                    cout << " > ";
                                                    SetConsoleTextAttribute(hConsole, 1);
                                                    cout << "Speed";
                                                    SetConsoleTextAttribute(hConsole, 15);
                                                    cout << " > ";
                                                    SetConsoleTextAttribute(hConsole, 1);
                                                    cout << "Constant speed\n";
                                                    SetConsoleTextAttribute(hConsole, 15);
                                                }
                                                if (Convertion(2, points) == 1) { SetConsoleTextAttribute(hConsole, 15); cout << "<- Go back\n"; SetConsoleTextAttribute(hConsole, 4); cout << "Enter 0\n"; }
                                                SetConsoleTextAttribute(hConsole, 15);
                                                cout << "Speed: " << speed << "\n";
                                                if (S != "1")
                                                {
                                                    try
                                                    {
                                                        number = CheckInput("\nEnter speed (max speed value - 1000):\n"); if (number <= 0) { break; } if (number < 1001) { speed = number; }
                                                    }
                                                    catch (char const* ex)
                                                    {
                                                        cout << ex << "\n";
                                                    }
                                                }
                                            }
                                            else
                                            {
                                                if (Convertion(1, points) == 1)
                                                {
                                                    SetConsoleTextAttribute(hConsole, 15);
                                                    cout << " > ";
                                                    SetConsoleTextAttribute(hConsole, 1);
                                                    cout << "Настройки";
                                                    SetConsoleTextAttribute(hConsole, 15);
                                                    cout << " > ";
                                                    SetConsoleTextAttribute(hConsole, 1);
                                                    cout << "Скорость";
                                                    SetConsoleTextAttribute(hConsole, 15);
                                                    cout << " > ";
                                                    SetConsoleTextAttribute(hConsole, 1);
                                                    cout << "Постоянная скорость\n";
                                                    SetConsoleTextAttribute(hConsole, 15);
                                                }
                                                if (Convertion(2, points) == 1) { SetConsoleTextAttribute(hConsole, 15); cout << "<- Вернуться\n"; SetConsoleTextAttribute(hConsole, 4); cout << "Введите 0\n"; }
                                                SetConsoleTextAttribute(hConsole, 15);
                                                cout << "Текущая скорость: " << speed << "\n";
                                                if (S != "1")
                                                {
                                                    try
                                                    {
                                                        number = CheckInput("\nВведите скорость (максимальное значение скорости - 1000):\n"); if (number <= 0) { break; } if (number < 1001) { speed = number; }
                                                    }
                                                    catch (char const* ex)
                                                    {
                                                        cout << ex << "\n";
                                                    }
                                                }
                                            }
                                        }
                                        S = "-1";
                                    }
                                    // increasing and falling speed
                                    if (S == "2" || S == "3")
                                    {
                                        t = 1;
                                        system("cls");
                                        if (S == "2") { k = 2; } if (S == "3") { k = 3; }
                                        while (S != "0")
                                        {
                                            system("cls");
                                            if (Convertion(1, points) == 1)
                                            {
                                                SetConsoleTextAttribute(hConsole, 15);
                                                cout << " > ";
                                                SetConsoleTextAttribute(hConsole, 1);
                                                cout << "Settings";
                                                SetConsoleTextAttribute(hConsole, 15);
                                                cout << " > ";
                                                SetConsoleTextAttribute(hConsole, 1);
                                                cout << "Speed";
                                                SetConsoleTextAttribute(hConsole, 15);
                                                cout << " > ";
                                                SetConsoleTextAttribute(hConsole, 1);
                                                if (k == 2) { cout << "Increasing speed\n"; SetConsoleTextAttribute(hConsole, 15); } if (k == 3) { cout << "Falling speed\n"; SetConsoleTextAttribute(hConsole, 15); }
                                            }
                                            if (Convertion(2, points) == 1) { SetConsoleTextAttribute(hConsole, 15); cout << "<- Go back\n"; SetConsoleTextAttribute(hConsole, 4); cout << "Enter 0\n"; }
                                            SetConsoleTextAttribute(hConsole, 15);
                                            cout << "\nUse 'w' and 's' or arrows to select, then press '=' and enter values.\n" << "\n";
                                            if (t == 1)
                                            {
                                                SetConsoleTextAttribute(hConsole, 2);
                                            }
                                            cout << "Current speed: " << speed << "\n" << "\n";
                                            SetConsoleTextAttribute(hConsole, 15);
                                            if (t == 2)
                                            {
                                                SetConsoleTextAttribute(hConsole, 2);
                                            }
                                            cout << "Exponential change: " << exponent << "\n" << "\n";
                                            SetConsoleTextAttribute(hConsole, 15);
                                            if (t == 3)
                                            {
                                                SetConsoleTextAttribute(hConsole, 2);
                                            }
                                            cout << "Speed limit: " << maxspeed << "\n" << "\n";
                                            SetConsoleTextAttribute(hConsole, 15);
                                            Select();
                                            this_thread::sleep_for(chrono::milliseconds(200));
                                            if (S == "=")
                                            {
                                                number = CheckInput("\n");
                                            }
                                            if (S == "0")
                                            {
                                                break;
                                            }
                                            if (t == 1 && S == "=")
                                            {
                                                if (k == 2)
                                                {
                                                    if (number > maxspeed)
                                                    {
                                                        cout << "Current speed must be LESS than speed limit.";
                                                        this_thread::sleep_for(chrono::milliseconds(2000));
                                                    }
                                                    else
                                                    {
                                                        if (number > 1000) { speed = 1000; }
                                                        else if (number > 0) { speed = number; }
                                                        system("cls");
                                                    }
                                                }
                                                if (k == 3)
                                                {
                                                    if (number < maxspeed)
                                                    {
                                                        cout << "Current speed must be MORE than speed limit.";
                                                        this_thread::sleep_for(chrono::milliseconds(2000));
                                                    }
                                                    else
                                                    {
                                                        if (number > 1000) { speed = 1000; }
                                                        else if (number > 0) { speed = number; }
                                                        system("cls");
                                                    }
                                                }
                                            }
                                            if (t == 2 && S == "=")
                                            {
                                                if (k == 2)
                                                {
                                                    if (speed > maxspeed)
                                                    {
                                                        cout << "Current speed must be LESS than speed limit.";
                                                        this_thread::sleep_for(chrono::milliseconds(2000));
                                                    }
                                                    else
                                                    {
                                                        if (number > 100 || number < 0) { exponent = 100; }
                                                        else if (number >= 0) { exponent = number; }
                                                    }
                                                }
                                                if (k == 3)
                                                {
                                                    if (speed < maxspeed)
                                                    {
                                                        cout << "Current speed must be MORE than speed limit.";
                                                        this_thread::sleep_for(chrono::milliseconds(2000));
                                                    }
                                                    else
                                                    {
                                                        if (number > 100 || number < 0) { exponent = -100; }
                                                        else if (number >= 0) { exponent = number * -1; }
                                                    }
                                                }
                                                system("cls");
                                            }
                                            if (t == 3 && S == "=")
                                            {
                                                if (k == 2)
                                                {
                                                    if (speed < number)
                                                    {
                                                        if (number > 1000) { maxspeed = 1000; }
                                                        else { maxspeed = number; }
                                                    }
                                                    else
                                                    {
                                                        cout << "Max speed must be MORE than current speed.";
                                                        this_thread::sleep_for(chrono::milliseconds(2000));
                                                    }
                                                }
                                                if (k == 3)
                                                {
                                                    if (speed > number)
                                                    {
                                                        if (number > 1000) { maxspeed = 1000; }
                                                        else { maxspeed = number; }
                                                    }
                                                    else
                                                    {
                                                        cout << "Max speed must be LESS than current speed.";
                                                        this_thread::sleep_for(chrono::milliseconds(2000));
                                                    }
                                                }
                                                system("cls");
                                            }
                                            if (k == 2 && exponent < 0 && speed < maxspeed) { exponent *= -1; }
                                            if (k == 3 && exponent > 0 && speed > maxspeed) { exponent *= -1; }
                                            S = "-1";
                                        }
                                    } S = "-1";
                                }
                            }
                            // Obstacles
                            if (S == "5")
                            {
                                short t = 0; short k = 0;
                                while (S != "0")
                                {
                                    S = "-1";
                                    system("cls");
                                    if (Convertion(6, points) != 1)
                                    {
                                        if (Convertion(1, points) == 1)
                                        {
                                            SetConsoleTextAttribute(hConsole, 15);
                                            cout << " > ";
                                            SetConsoleTextAttribute(hConsole, 1);
                                            cout << "Settings";
                                            SetConsoleTextAttribute(hConsole, 15);
                                            cout << " > ";
                                            SetConsoleTextAttribute(hConsole, 1);
                                            cout << "Obstacles\n";
                                            SetConsoleTextAttribute(hConsole, 15);
                                        }
                                        if (Convertion(2, points) == 1) { SetConsoleTextAttribute(hConsole, 15); cout << "<- Go back\n"; SetConsoleTextAttribute(hConsole, 4); cout << "Enter 0\n"; }
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << "\n ##  ##   #  ###   #   ## \n";
                                        cout << "#    # #      #   # #  # #\n";
                                        cout << " ##  # #  #   #   # #  ## \n";
                                        cout << "#    # #  #   #   # #  # #\n";
                                        cout << " ##  ##   #   #    #   # #\n";
                                        SetConsoleTextAttribute(hConsole, 4);
                                        cout << "Enter 1\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << "##    #   #  #  ##    #   #   #      ##   ##  #  #   ##  ##    #   ###   #   ## \n";
                                        cout << "# #  # #  ## #  # #  # #  ## ##     #    #    ## #  #    # #  # #   #   # #  # #\n";
                                        cout << "##   ###  # ##  # #  # #  # # #     # #   ##  # ##   ##  ##   ###   #   # #  ## \n";
                                        cout << "# #  # #  #  #  # #  # #  #   #     # #  #    #  #  #    # #  # #   #   # #  # #\n";
                                        cout << "# #  # #  #  #  ##    #   #   #     ##    ##  #  #   ##  # #  # #   #    #   # #\n";
                                        SetConsoleTextAttribute(hConsole, 4);
                                        cout << "Enter 2\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << "#  #   ##   ###\n";
                                        cout << "#     #      # \n";
                                        cout << "#  #   ##    # \n";
                                        cout << "#  #     #   # \n";
                                        cout << "#  #   ##    # \n";
                                        SetConsoleTextAttribute(hConsole, 4);
                                        cout << "Enter 3\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                    }
                                    else
                                    {
                                        if (Convertion(1, points) == 1)
                                        {
                                            SetConsoleTextAttribute(hConsole, 15);
                                            cout << " > ";
                                            SetConsoleTextAttribute(hConsole, 1);
                                            cout << "Настройки";
                                            SetConsoleTextAttribute(hConsole, 15);
                                            cout << " > ";
                                            SetConsoleTextAttribute(hConsole, 1);
                                            cout << "Препятствия\n";
                                            SetConsoleTextAttribute(hConsole, 15);
                                        }
                                        if (Convertion(2, points) == 1) { SetConsoleTextAttribute(hConsole, 15); cout << "<- Ввернуться\n"; SetConsoleTextAttribute(hConsole, 4); cout << "Введите 0\n"; }
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << "\n";
                                        cout << "\n";
                                        cout << "\n";
                                        cout << "\n";
                                        cout << "\n";
                                        SetConsoleTextAttribute(hConsole, 4);
                                        cout << "Введите 1\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << "\n";
                                        cout << "\n";
                                        cout << "\n";
                                        cout << "\n";
                                        cout << "\n";
                                        SetConsoleTextAttribute(hConsole, 4);
                                        cout << "Введите 2\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << "\n";
                                        cout << "\n";
                                        cout << "\n";
                                        cout << "\n";
                                        cout << "\n";
                                        SetConsoleTextAttribute(hConsole, 4);
                                        cout << "Введите 3\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                    }
                                    while (true)
                                    {
                                        cin >> S;
                                        if (S == "0") { break; } if (S == "1") { break; } if (S == "2") { break; } if (S == "3") { break; }
                                    }
                                    if (S == "0") { S = "-1"; break; }
                                    // Editor
                                    if (S == "1")
                                    {
                                        for (int i = 0; i < 4; i++)
                                        {
                                            for (int j = 0; j < 4; j++)
                                            {
                                                OBS[i][j] = 0;
                                            }
                                        }
                                        Obstacles_H = 0; Obstacles_L = 0; S = "-1";
                                        system("cls");
                                        while (S != "0")
                                        {
                                            this_thread::sleep_for(chrono::milliseconds(50));
                                            printf("\x1b[H");
                                            if (Convertion(1, points) == 1)
                                            {
                                                SetConsoleTextAttribute(hConsole, 15);
                                                cout << " > ";
                                                SetConsoleTextAttribute(hConsole, 1);
                                                cout << "Settings";
                                                SetConsoleTextAttribute(hConsole, 15);
                                                cout << " > ";
                                                SetConsoleTextAttribute(hConsole, 1);
                                                cout << "Obstacles";
                                                SetConsoleTextAttribute(hConsole, 15);
                                                cout << " > ";
                                                SetConsoleTextAttribute(hConsole, 1);
                                                cout << "Editor\n";
                                                SetConsoleTextAttribute(hConsole, 15);
                                            }
                                            if (Convertion(2, points) == 1) { SetConsoleTextAttribute(hConsole, 15); cout << "<- Go back\n"; SetConsoleTextAttribute(hConsole, 4); cout << "Enter 0\n"; } cout << "\n";
                                            ObsInput(); if (S == "0") { break; }
                                            SetConsoleTextAttribute(hConsole, 15);
                                            cout << "Number of obstacles in the list: "; SetConsoleTextAttribute(hConsole, 1); cout << ObsNum << "/1000\n"; SetConsoleTextAttribute(hConsole, 15);
                                            cout << "Use 'w', 'a', 's', 'd' or arrows to select; enter '/' to print $ in the current cell; enter '*' to clear current cell; enter '1' to save obstacle; enter '!' to clear list of obstacles\n\n";
                                            for (int i = 0; i < 50; i++) { cout << " "; }
                                            cout << "######\n";
                                            for (int i = 0; i < 4; i++)
                                            {
                                                for (int u = 0; u < 50; u++) { cout << " "; }
                                                cout << "#";
                                                for (int j = 0; j < 4; j++)
                                                {
                                                    if (Obstacles_H == i && Obstacles_L == j) { SetConsoleTextAttribute(hConsole, 40); }
                                                    if (OBS[i][j] == 1) { cout << "$"; }
                                                    else { cout << " "; }
                                                    SetConsoleTextAttribute(hConsole, 15);
                                                }
                                                cout << "#\n";
                                            }
                                            for (int i = 0; i < 50; i++) { cout << " "; }
                                            cout << "######\n";
                                            if (S == "2")
                                            {
                                                ObsNum = 0;
                                                ofstream out;
                                                out.open("Obstacles.txt");
                                                out.close();
                                                S = "-1";
                                            }
                                            if (S == "1")
                                            {
                                                t = 1; obstacles = 0;
                                                for (int i = 0; i < 4; i++)
                                                {
                                                    for (int j = 0; j < 4; j++)
                                                    {
                                                        obstacles += (OBS[i][j] * t);
                                                        t *= 2;
                                                    }
                                                }
                                                t = 1;
                                                if (obstacles > 0 && obstacles < 65536)
                                                {
                                                    ObsNum++;
                                                    if (ObsNum <= 1000)
                                                    {
                                                        cout << "Saving...";
                                                        if (ObsNum == 1)
                                                        {
                                                            ofstream out;
                                                            out.open("Obstacles.txt");
                                                            if (out.is_open())
                                                            {
                                                                out << obstacles << " ";
                                                            }
                                                            out.close();
                                                        }
                                                        else
                                                        {
                                                            ofstream out;
                                                            out.open("Obstacles.txt", ofstream::app);
                                                            if (out.is_open())
                                                            {
                                                                out << obstacles << " ";
                                                            }
                                                            out.close();
                                                        }
                                                    }
                                                }
                                                if (ObsNum > 1000)
                                                {
                                                    SetConsoleTextAttribute(hConsole, 15);
                                                    cout << "\nThe number of obstacles is maximum";
                                                    cout << "\nClear list of obstacles";
                                                    SetConsoleTextAttribute(hConsole, 4);
                                                    cout << "\nEnter 1\n";
                                                    SetConsoleTextAttribute(hConsole, 15);
                                                    while (true)
                                                    {
                                                        cin >> S; if (S == "0") { break; } if (S == "1") { break; }
                                                    }
                                                    if (S == "0") { S = "-1"; ObsNum == 1000; break; }
                                                    if (S == "1")
                                                    {
                                                        ofstream out;
                                                        out.open("Obstacles.txt");
                                                        if (out.is_open())
                                                        {
                                                            out << obstacles << " ";
                                                        }
                                                        out.close();
                                                        ObsNum = 1;
                                                    }
                                                }
                                                this_thread::sleep_for(chrono::milliseconds(200));
                                                system("cls"); S = "-1"; obstacles = 0;
                                                for (int i = 0; i < 4; i++)
                                                {
                                                    for (int j = 0; j < 4; j++)
                                                    {
                                                        OBS[i][j] = 0;
                                                    }
                                                }
                                            }
                                        } S = "-1";
                                    }
                                    // Random generate
                                    if (S == "2")
                                    {
                                        system("cls"); S = "-1";
                                        while (S != "0")
                                        {
                                            system("cls");
                                            if (Convertion(1, points) == 1)
                                            {
                                                SetConsoleTextAttribute(hConsole, 15);
                                                cout << " > ";
                                                SetConsoleTextAttribute(hConsole, 1);
                                                cout << "Settings";
                                                SetConsoleTextAttribute(hConsole, 15);
                                                cout << " > ";
                                                SetConsoleTextAttribute(hConsole, 1);
                                                cout << "Obstacles";
                                                SetConsoleTextAttribute(hConsole, 15);
                                                cout << " > ";
                                                SetConsoleTextAttribute(hConsole, 1);
                                                cout << "Random generator\n";
                                                SetConsoleTextAttribute(hConsole, 15);
                                            }
                                            if (Convertion(2, points) == 1) { SetConsoleTextAttribute(hConsole, 15); cout << "<- Go back\n"; SetConsoleTextAttribute(hConsole, 4); cout << "Enter 0\n"; } cout << "\n";
                                            SetConsoleTextAttribute(hConsole, 15);
                                            cout << "Number of obstacles in the list: "; SetConsoleTextAttribute(hConsole, 1); cout << ObsNum << "/1000\n"; SetConsoleTextAttribute(hConsole, 15);
                                            cout << "Enter 1 to resaving to save obstacles by removing old ones. Enter 2 to add obstacles to list\n";
                                            while (true)
                                            {
                                                cin >> S; if (S == "0") { break; } if (S == "1") { break; } if (S == "2") { break; }
                                            }
                                            if (S == "0") { break; }
                                            cout << "Enter the number of different obstacles you want to generate\n";
                                            number = CheckInput(" "); if (number < 1) { number = 1; }
                                            system("cls");
                                            if (Convertion(1, points) == 1)
                                            {
                                                SetConsoleTextAttribute(hConsole, 15);
                                                cout << " > ";
                                                SetConsoleTextAttribute(hConsole, 1);
                                                cout << "Settings";
                                                SetConsoleTextAttribute(hConsole, 15);
                                                cout << " > ";
                                                SetConsoleTextAttribute(hConsole, 1);
                                                cout << "Obstacles";
                                                SetConsoleTextAttribute(hConsole, 15);
                                                cout << " > ";
                                                SetConsoleTextAttribute(hConsole, 1);
                                                cout << "Random generator\n";
                                                SetConsoleTextAttribute(hConsole, 15);
                                            }
                                            if (Convertion(2, points) == 1) { SetConsoleTextAttribute(hConsole, 15); cout << "<- Go back\n"; SetConsoleTextAttribute(hConsole, 4); cout << "Enter 0\n"; } cout << "\n";
                                            SetConsoleTextAttribute(hConsole, 15);
                                            cout << "Number of obstacles in the list: "; SetConsoleTextAttribute(hConsole, 1); cout << ObsNum << "/1000\n"; SetConsoleTextAttribute(hConsole, 15);
                                            if (ObsNum + number <= 1000)
                                            {
                                                ObsNum += number;
                                                unsigned short* Obstacles = new unsigned short[number];
                                                for (int i = 0; i < number; i++)
                                                {
                                                    Obstacles[i] = rand() % (65536);
                                                }
                                                k = 0;
                                                unsigned short b = 0;
                                                unsigned short c = 0;
                                                for (int u = 0; u < number; u++)
                                                {
                                                    c = Obstacles[u];
                                                    for (int i = 0; i < 4; i++)
                                                    {
                                                        for (int j = 0; j < 4; j++)
                                                        {
                                                            OBS[i][j] = (c % 2);
                                                            c /= 2;
                                                        }
                                                    }
                                                    for (int i = 0; i < 4; i++)
                                                    {
                                                        cPos.Y = 5 + i + k; cPos.X = 10 * b;
                                                        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos);
                                                        if (i == 0)
                                                        {
                                                            SetConsoleTextAttribute(hConsole, 4);
                                                            cout << u + 1 << ")\n";
                                                            SetConsoleTextAttribute(hConsole, 15);
                                                        }
                                                        for (int j = 0; j < 4; j++)
                                                        {
                                                            if (OBS[i][j] == 1) { cout << "$"; }
                                                            else { cout << " "; }
                                                        }
                                                    }
                                                    b++;
                                                    if ((u + 1) % 21 == 0)
                                                    {
                                                        k += 6;
                                                        b = 0;
                                                    }
                                                }
                                                SetConsoleTextAttribute(hConsole, 15);
                                                cout << "\n\nEnter 0 to go back and save current random obstacles. Enter 1 to regenerate\n";
                                                string F = "0";
                                                while (true)
                                                {
                                                    cin >> F; if (F == "0") { break; } if (F == "1") { ObsNum -= number; break; }
                                                }
                                                if (F == "0")
                                                {
                                                    if (S == "1")
                                                    {
                                                        ObsNum = 0;
                                                        ofstream out;
                                                        out.open("Obstacles.txt");
                                                        if (out.is_open())
                                                        {
                                                            for (int i = 0; i < number; i++)
                                                            {
                                                                out << Obstacles[i] << " ";
                                                                ObsNum++;
                                                            }
                                                        }
                                                        out.close();
                                                    }
                                                    if (S == "2")
                                                    {
                                                        ofstream out;
                                                        out.open("Obstacles.txt", ofstream::app);
                                                        if (out.is_open())
                                                        {
                                                            for (int i = 0; i < number; i++)
                                                            {
                                                                out << Obstacles[i] << " ";
                                                            }
                                                        }
                                                        out.close();
                                                    }
                                                    S = "-1";
                                                    break;
                                                }
                                                delete[] Obstacles;
                                            } S = "-1";
                                        }
                                    }
                                    // List
                                    if (S == "3")
                                    {
                                        system("cls"); S = "-1";
                                        while (S != "0")
                                        {
                                            system("cls");
                                            if (Convertion(1, points) == 1)
                                            {
                                                SetConsoleTextAttribute(hConsole, 15);
                                                cout << " > ";
                                                SetConsoleTextAttribute(hConsole, 1);
                                                cout << "Settings";
                                                SetConsoleTextAttribute(hConsole, 15);
                                                cout << " > ";
                                                SetConsoleTextAttribute(hConsole, 1);
                                                cout << "Obstacles";
                                                SetConsoleTextAttribute(hConsole, 15);
                                                cout << " > ";
                                                SetConsoleTextAttribute(hConsole, 1);
                                                cout << "List\n";
                                                SetConsoleTextAttribute(hConsole, 15);
                                            }
                                            if (Convertion(2, points) == 1) { SetConsoleTextAttribute(hConsole, 15); cout << "<- Go back\n"; SetConsoleTextAttribute(hConsole, 4); cout << "Enter 0"; }
                                            SetConsoleTextAttribute(hConsole, 15);
                                            cout << "\nShow complete list of all obstacles\n";
                                            SetConsoleTextAttribute(hConsole, 4); cout << "Enter 1\n";
                                            SetConsoleTextAttribute(hConsole, 15);
                                            cout << "Show all current saved obstacles in list\n";
                                            SetConsoleTextAttribute(hConsole, 4); cout << "Enter 2\n";
                                            obstacles = 0; unsigned short b = 0; unsigned short k = 0; unsigned short c = 0; SetConsoleTextAttribute(hConsole, 15);
                                            // full list
                                            if (S == "1")
                                            {
                                                for (int u = 1; u < 65536; u++)
                                                {
                                                    if (u % 2 == 1) { c = u; }
                                                    else { u++; c = u; }
                                                    for (int i = 0; i < 4; i++)
                                                    {
                                                        for (int j = 0; j < 4; j++)
                                                        {
                                                            OBS[i][j] = 0;
                                                        }
                                                    }
                                                    for (int i = 0; i < 4; i++)
                                                    {
                                                        for (int j = 0; j < 4; j++)
                                                        {
                                                            OBS[i][j] = (c % 2);
                                                            c /= 2;
                                                        }
                                                    }
                                                    for (int i = 0; i < 4; i++)
                                                    {
                                                        if (i == 0)
                                                        {
                                                            cPos.Y = 25 + i + k; cPos.X = 10 * b;
                                                            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos);
                                                            SetConsoleTextAttribute(hConsole, 4);
                                                            cout << (u + 1) / 2 << ")";
                                                            SetConsoleTextAttribute(hConsole, 15);
                                                        }
                                                        cPos.Y = 26 + i + k; cPos.X = 10 * b;
                                                        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos);
                                                        for (int j = 0; j < 4; j++)
                                                        {
                                                            if (OBS[i][j] == 1) { cout << "$"; }
                                                            else { cout << " "; }
                                                        }
                                                    }
                                                    b++;
                                                    if ((u + 1) % 21 == 0)
                                                    {
                                                        k += 5;
                                                        b = 0;
                                                    }
                                                }
                                                cout << "\n";
                                            }
                                            if (S == "2" && ObsNum > 0)
                                            {
                                                k = 0;
                                                ifstream in("Obstacles.txt");
                                                if (in.is_open())
                                                {
                                                    for (int u = 0; u < ObsNum; u++)
                                                    {
                                                        in >> c;
                                                        for (int i = 0; i < 4; i++)
                                                        {
                                                            for (int j = 0; j < 4; j++)
                                                            {
                                                                OBS[i][j] = (c % 2);
                                                                c /= 2;
                                                            }
                                                        }
                                                        for (int i = 0; i < 4; i++)
                                                        {
                                                            cPos.Y = 10 + i + k; cPos.X = 10 * b;
                                                            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos);
                                                            if (i == 0)
                                                            {
                                                                SetConsoleTextAttribute(hConsole, 4);
                                                                cout << u + 1 << ")\n";
                                                                SetConsoleTextAttribute(hConsole, 15);
                                                            }
                                                            for (int j = 0; j < 4; j++)
                                                            {
                                                                if (OBS[i][j] == 1) { cout << "$"; }
                                                                else { cout << " "; }
                                                            }
                                                        }
                                                        b++;
                                                        if ((u + 1) % 21 == 0)
                                                        {
                                                            k += 6;
                                                            b = 0;
                                                        }
                                                    }
                                                }
                                                in.close();
                                            }
                                            else if (ObsNum == 0)
                                            {
                                                SetConsoleTextAttribute(hConsole, 15);
                                                cout << "Obstacle list is empty\n";
                                            }
                                            while (true)
                                            {
                                                cin >> S; if (S == "0") { break; } if (S == "1") { break; } if (S == "2") { break; }
                                            }
                                            if (S == "0") { break; }
                                        }
                                    }
                                } S = "-1";
                            }
                            // Quality
                            if (S == "5")
                            {
                                while (S != "0")
                                {
                                    S = "-1";
                                    system("cls");
                                    if (Convertion(1, points) == 1)
                                    {
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << " > ";
                                        SetConsoleTextAttribute(hConsole, 1);
                                        cout << "Settings";
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << " > ";
                                        SetConsoleTextAttribute(hConsole, 1);
                                        cout << "Quality\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                    }
                                    if (Convertion(2, points) == 1) { SetConsoleTextAttribute(hConsole, 15); cout << "<- Go back\n"; SetConsoleTextAttribute(hConsole, 4); cout << "Enter 0\n"; }
                                    SetConsoleTextAttribute(hConsole, 15);
                                    cout << "\nComing soon)\n";
                                    while (true)
                                    {
                                        cin >> S; if (S == "0") { break; }
                                    }
                                } S = "-1";
                            }
                            // Saving
                            if (S == "11")
                            {
                                ofstream out;
                                out.open("Saves.txt");
                                if (out.is_open())
                                {
                                    out << x << " ";
                                    out << y << "\n";
                                    out << points << "\n";
                                    out << Left << " " << Right << " " << Down << " " << Up << "\n";
                                    out << PlayerColor << "\n";
                                    out << ObstaclesColor << "\n";
                                    out << PointsIndicatorColor << "\n";
                                    out << speed << "\n";
                                    out << maxspeed << "\n";
                                    out << exponent << "\n";
                                    out << H_indent << "\n";
                                    out << V_indent << "\n";
                                }
                                out.close();
                                if (Convertion(6, points) != 1)
                                {
                                    cout << "\n\nSaving...";
                                }
                                else
                                {
                                    cout << "\n\nСохраняем...";
                                }
                                this_thread::sleep_for(chrono::milliseconds(1000));
                                S = "-3";
                            }
                            // Other
                            if (S == "7")
                            {
                                while (S != "0")
                                {
                                    system("cls");
                                    if (Convertion(6, points) != 1)
                                    {
                                        if (Convertion(1, points) == 1)
                                        {
                                            SetConsoleTextAttribute(hConsole, 15);
                                            cout << " > ";
                                            SetConsoleTextAttribute(hConsole, 1);
                                            cout << "Settings";
                                            SetConsoleTextAttribute(hConsole, 15);
                                            cout << " > ";
                                            SetConsoleTextAttribute(hConsole, 1);
                                            cout << "Other\n";
                                            SetConsoleTextAttribute(hConsole, 15);
                                        }
                                        if (Convertion(2, points) == 1) { SetConsoleTextAttribute(hConsole, 15); cout << "<- Go back\n"; SetConsoleTextAttribute(hConsole, 4); cout << "Enter 0\n"; }
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << "\nFunction path: ";
                                        if (Convertion(1, points) != 1)
                                        {
                                            SetConsoleTextAttribute(hConsole, 4);
                                            cout << "OFF\n";
                                        }
                                        else
                                        {
                                            SetConsoleTextAttribute(hConsole, 2);
                                            cout << "ON\n";
                                        }
                                        SetConsoleTextAttribute(hConsole, 4);
                                        cout << "Enter 1\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << "Show help '<- Go back': ";
                                        if (Convertion(2, points) != 1)
                                        {
                                            SetConsoleTextAttribute(hConsole, 4);
                                            cout << "OFF\n";
                                        }
                                        else
                                        {
                                            SetConsoleTextAttribute(hConsole, 2);
                                            cout << "ON\n";
                                        }
                                        SetConsoleTextAttribute(hConsole, 4);
                                        cout << "Enter 2\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << "Autosave: ";
                                        if (Convertion(3, points) != 1)
                                        {
                                            SetConsoleTextAttribute(hConsole, 4);
                                            cout << "OFF\n";
                                        }
                                        else
                                        {
                                            SetConsoleTextAttribute(hConsole, 2);
                                            cout << "ON\n";
                                        }
                                        SetConsoleTextAttribute(hConsole, 4);
                                        cout << "Enter 3\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << "Help info while playing: ";
                                        SetConsoleTextAttribute(hConsole, 15);
                                        if (Convertion(4, points) != 1)
                                        {
                                            SetConsoleTextAttribute(hConsole, 4);
                                            cout << "OFF\n";
                                        }
                                        else
                                        {
                                            SetConsoleTextAttribute(hConsole, 2);
                                            cout << "ON\n";
                                        }
                                        SetConsoleTextAttribute(hConsole, 4);
                                        cout << "Enter 4\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << "Display errors: ";
                                        if (Convertion(5, points) != 1)
                                        {
                                            SetConsoleTextAttribute(hConsole, 4);
                                            cout << "OFF\n";
                                        }
                                        else
                                        {
                                            SetConsoleTextAttribute(hConsole, 2);
                                            cout << "ON\n";
                                        }
                                        SetConsoleTextAttribute(hConsole, 4);
                                        cout << "Enter 5\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << "Language: EN\n";
                                        SetConsoleTextAttribute(hConsole, 4);
                                        cout << "Enter 6\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << "Reset settings\n";
                                        SetConsoleTextAttribute(hConsole, 4);
                                        cout << "Enter 7\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                    }
                                    else
                                    {
                                        if (Convertion(1, points) == 1)
                                        {
                                            SetConsoleTextAttribute(hConsole, 15);
                                            cout << " > ";
                                            SetConsoleTextAttribute(hConsole, 1);
                                            cout << "Настройки";
                                            SetConsoleTextAttribute(hConsole, 15);
                                            cout << " > ";
                                            SetConsoleTextAttribute(hConsole, 1);
                                            cout << "Другое\n";
                                            SetConsoleTextAttribute(hConsole, 15);
                                        }
                                        if (Convertion(2, points) == 1) { SetConsoleTextAttribute(hConsole, 15); cout << "<- Вернуться\n"; SetConsoleTextAttribute(hConsole, 4); cout << "Введите 0\n"; }
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << "\nПуть к функции: ";
                                        if (Convertion(1, points) != 1)
                                        {
                                            SetConsoleTextAttribute(hConsole, 4);
                                            cout << "ВЫКЛ\n";
                                        }
                                        else
                                        {
                                            SetConsoleTextAttribute(hConsole, 2);
                                            cout << "ВКЛ\n";
                                        }
                                        SetConsoleTextAttribute(hConsole, 4);
                                        cout << "Введите 1\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << "Показывать подсказку '<- Вернуться': ";
                                        if (Convertion(2, points) != 1)
                                        {
                                            SetConsoleTextAttribute(hConsole, 4);
                                            cout << "ВЫКЛ\n";
                                        }
                                        else
                                        {
                                            SetConsoleTextAttribute(hConsole, 2);
                                            cout << "ВКЛ\n";
                                        }
                                        SetConsoleTextAttribute(hConsole, 4);
                                        cout << "Введите 2\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << "Автосохранение: ";
                                        if (Convertion(3, points) != 1)
                                        {
                                            SetConsoleTextAttribute(hConsole, 4);
                                            cout << "ВЫКЛ\n";
                                        }
                                        else
                                        {
                                            SetConsoleTextAttribute(hConsole, 2);
                                            cout << "ВКЛ\n";
                                        }
                                        SetConsoleTextAttribute(hConsole, 4);
                                        cout << "Введите 3\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << "Подсказки во время игры: ";
                                        SetConsoleTextAttribute(hConsole, 15);
                                        if (Convertion(4, points) != 1)
                                        {
                                            SetConsoleTextAttribute(hConsole, 4);
                                            cout << "ВЫКЛ\n";
                                        }
                                        else
                                        {
                                            SetConsoleTextAttribute(hConsole, 2);
                                            cout << "ВКЛ\n";
                                        }
                                        SetConsoleTextAttribute(hConsole, 4);
                                        cout << "Введите 4\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << "Отображать ошибки: ";
                                        if (Convertion(5, points) != 1)
                                        {
                                            SetConsoleTextAttribute(hConsole, 4);
                                            cout << "ВЫКЛ\n";
                                        }
                                        else
                                        {
                                            SetConsoleTextAttribute(hConsole, 2);
                                            cout << "ВКЛ\n";
                                        }
                                        SetConsoleTextAttribute(hConsole, 4);
                                        cout << "Введите 5\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << "Язык: RU\n";
                                        SetConsoleTextAttribute(hConsole, 4);
                                        cout << "Введите 6\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << "Сбросить настройки\n";
                                        SetConsoleTextAttribute(hConsole, 4);
                                        cout << "Введите 7\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                    }
                                    while (S != "0")
                                    {
                                        cin >> S;
                                        if (S == "1") { break; } if (S == "2") { break; } if (S == "3") { break; } if (S == "4") { break; } if (S == "5") { break; } if (S == "6") { break; }
                                    }
                                    if (S == "1" && Convertion(1, points) != 1) { points += 1; S = "-1"; }
                                    if (S == "1" && Convertion(1, points) == 1) { points -= 1; S = "-1"; }
                                    if (S == "2" && Convertion(2, points) != 1) { points += 2; S = "-1"; }
                                    if (S == "2" && Convertion(2, points) == 1) { points -= 2; S = "-1"; }
                                    if (S == "3" && Convertion(3, points) != 1) { points += 4; S = "-1"; SetConsoleTextAttribute(hConsole, 15); cout << "\nBefore each start, all settings will be saved.\n"; this_thread::sleep_for(chrono::milliseconds(3000)); }
                                    if (S == "3" && Convertion(3, points) == 1) { points -= 4; S = "-1"; }
                                    if (S == "4" && Convertion(4, points) != 1) { points += 8; S = "-1"; }
                                    if (S == "4" && Convertion(4, points) == 1) { points -= 8; S = "-1"; }
                                    if (S == "5" && Convertion(5, points) != 1) { points += 16; S = "-1"; }
                                    if (S == "5" && Convertion(5, points) == 1) { points -= 16; S = "-1"; }
                                    if (S == "6" && Convertion(6, points) != 1) { points += 32; S = "-1"; }
                                    if (S == "6" && Convertion(6, points) == 1) { points -= 32; S = "-1"; }
                                    if (S == "7")
                                    {
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << "\nAre you sure you want to reset all settings?\n\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << "Confirm\n";
                                        SetConsoleTextAttribute(hConsole, 4);
                                        cout << "Enter 0\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << "Reject\n";
                                        SetConsoleTextAttribute(hConsole, 4);
                                        cout << "Enter 1\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                        while (true)
                                        {
                                            cin >> S; if (S == "0") { break; } if (S == "1") { break; }
                                        }
                                        if (S == "0")
                                        {
                                            Left = 15; Right = 0; Up = 0; Down = 15; ObstaclesColor = 15; PlayerColor = 15; PointsIndicatorColor = 1; points = 26; speed = 900; maxspeed = 1; exponent = 0; pos = 5; x = 40; y = 7; H_indent = 0; V_indent = 0;
                                            ofstream out;
                                            out.open("Saves.txt");
                                            if (out.is_open())
                                            {
                                                out << x << " ";
                                                out << y << "\n";
                                                out << points << "\n";
                                                out << Left << " " << Right << " " << Down << " " << Up << "\n";
                                                out << ObsNum << "\n";
                                                out << PlayerColor << "\n";
                                                out << ObstaclesColor << "\n";
                                                out << PointsIndicatorColor << "\n";
                                                out << speed << "\n";
                                                out << maxspeed << "\n";
                                                out << exponent << "\n";
                                            }
                                            out.close();
                                        }
                                        S = "-1";
                                    }
                                }
                                S = "-1";
                            }
                            // Gravity
                            if (S == "11")
                            {
                                S = "-1";
                                while (S != "0")
                                {
                                    system("cls");
                                    if (Convertion(1, points) == 1)
                                    {
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << " > ";
                                        SetConsoleTextAttribute(hConsole, 1);
                                        cout << "Settings";
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << " > ";
                                        SetConsoleTextAttribute(hConsole, 1);
                                        cout << "Gravity\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                    }
                                    if (Convertion(2, points) == 1) { SetConsoleTextAttribute(hConsole, 15); cout << "<- Go back\n"; SetConsoleTextAttribute(hConsole, 4); cout << "Enter 0\n"; }
                                    SetConsoleTextAttribute(hConsole, 15);
                                    cout << "\nGravity: ";
                                    if (Convertion(9, points) != 1)
                                    {
                                        SetConsoleTextAttribute(hConsole, 4);
                                        cout << "OFF\n";
                                    }
                                    else
                                    {
                                        SetConsoleTextAttribute(hConsole, 2);
                                        cout << "ON\n";
                                    }
                                    SetConsoleTextAttribute(hConsole, 4);
                                    cout << "Enter 1";
                                    SetConsoleTextAttribute(hConsole, 15);
                                    cout << "\nFlip Gravity: ";
                                    if (Convertion(10, points) != 1)
                                    {
                                        SetConsoleTextAttribute(hConsole, 4);
                                        cout << "OFF\n";
                                    }
                                    else
                                    {
                                        SetConsoleTextAttribute(hConsole, 2);
                                        cout << "ON\n";
                                    }
                                    SetConsoleTextAttribute(hConsole, 4);
                                    cout << "Enter 2\n";
                                    SetConsoleTextAttribute(hConsole, 15);
                                    cout << "Gravity value: " << g;
                                    SetConsoleTextAttribute(hConsole, 4);
                                    cout << "Enter 3\n";
                                    SetConsoleTextAttribute(hConsole, 15);
                                    while (true)
                                    {
                                        cin >> S; if (S == "0") { break; } if (S == "1") { break; } if (S == "2") { break; } if (S == "3") { break; }
                                    }
                                    if (S == "0") { S = "-1"; break; }
                                    if (S == "1")
                                    {
                                        if (Convertion(9, points) == 0) { points += 256; }
                                        if (Convertion(9, points) == 1) { points -= 256; }
                                    }
                                    if (S == "2")
                                    {
                                        if (Convertion(10, points) == 0) { points += 512; }
                                        if (Convertion(10, points) == 1) { points -= 512; }
                                    }
                                    if (S == "3")
                                    {
                                        number = 0;
                                        number = CheckInput("\nEnter gravity value\n");
                                        if (number < 5) { number = 5; }
                                        if (number > x - 2) { number = x - 2; }
                                        g = number;
                                        number = 0;
                                    }
                                }
                            }
                            // Challenges
                            if (S == "13")
                            {
                                system("cls");
                                S = "-1";
                                while (S != "0")
                                {
                                    system("cls");
                                    if (Convertion(1, points) == 1)
                                    {
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << " > ";
                                        SetConsoleTextAttribute(hConsole, 1);
                                        cout << "Settings";
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << " > ";
                                        SetConsoleTextAttribute(hConsole, 1);
                                        cout << "Challenges\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                    }
                                    if (Convertion(2, points) == 1) { SetConsoleTextAttribute(hConsole, 15); cout << "<- Go back\n"; SetConsoleTextAttribute(hConsole, 4); cout << "Enter 0\n"; }
                                    if (Convertion(8, points) == 1)
                                    {
                                        SetConsoleTextAttribute(hConsole, 1);
                                        cout << "\nLess few seconds to avoid                              S is for speed\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << "##########                                            ################\n";
                                        cout << "#    $   #                                            #        $     #\n";
                                        cout << "#    $   #                                            #        $     #\n";
                                        cout << "#$       #                                            #    $         #\n";
                                        cout << "#$       #                                            #    $         #\n";
                                        cout << "#        #                                            #$            $#\n";
                                        cout << "#   $    #                                            #$            $#\n";
                                        cout << "#   $    #                                            #              #\n";
                                        cout << "#        #                                            #      $       #\n";
                                        cout << "#  *    $#                                            #  *   $       #\n";
                                        cout << "#  *    $#                                            #  *           #\n";
                                        cout << "##########                                            ################\n";
                                        SetConsoleTextAttribute(hConsole, 4);
                                        cout << "Enter 1                                                    Enter 4\n";
                                        SetConsoleTextAttribute(hConsole, 1);
                                        cout << "Invisi obstacles                                     Falling obstacles\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << "################################               ##############################\n";
                                        cout << "#               $              #               #   $                  $     #\n";
                                        cout << "#               $              #               #   $                  $     #\n";
                                        cout << "#                              #               #              $             #\n";
                                        cout << "#                              #               #              $             #\n";
                                        cout << "#         $                    #               #                            #\n";
                                        cout << "#         $                    #               #         $                  #\n";
                                        cout << "#                              #               #         $                  #\n";
                                        cout << "#                         $    #               #               $            #\n";
                                        cout << "#  *           $          $    #               #   *           $            #\n";
                                        cout << "#  *           $               #               #   *                        #\n";
                                        cout << "################################               ##############################\n";
                                        SetConsoleTextAttribute(hConsole, 4);
                                        cout << "Enter 2                                                  Enter 5\n";
                                        SetConsoleTextAttribute(hConsole, 1);
                                        cout << "Invisi obstacles 2                                       Dual mode\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << "################################                 #########################\n";
                                        cout << "#                        $     #                 #             $         #\n";
                                        cout << "#     $         $        $     #                 #   *         $         #\n";
                                        cout << "#     $         $        $     #                 #   *                   #\n";
                                        cout << "#     $         $        $     #                 #$                  $   #\n";
                                        cout << "#     $         $              #                 #$        $         $   #\n";
                                        cout << "#                              #                 #         $             #\n";
                                        cout << "#                              #                 #########################\n";
                                        cout << "#         $                    #                 #   $                   #\n";
                                        cout << "#         $               $    #                 #   $          $        #\n";
                                        cout << "#         $    $          $    #                 #              $        #\n";
                                        cout << "#         $    $          $    #                 #  *                   $#\n";
                                        cout << "#  *           $          $    #                 #  *        $          $#\n";
                                        cout << "#  *           $               #                 #           $           #\n";
                                        cout << "################################                 #########################\n";
                                        SetConsoleTextAttribute(hConsole, 4);
                                        cout << "Enter 3                                                   Enter 6\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                    }
                                    if (Convertion(8, points) == 0)
                                    {
                                        SetConsoleTextAttribute(hConsole, 1);
                                        cout << "\nLess few seconds\nto avoid           Reverse gravity\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << "#$                  #   *   $      \n";
                                        cout << "#$                  #   *   $      \n";
                                        cout << "#  $                #            $ \n";
                                        cout << "#  $                #            $ \n";
                                        cout << "#                   #              \n";
                                        cout << "# *                 #              \n";
                                        cout << "# *                 #              \n";
                                        cout << "######              ###############\n";
                                        SetConsoleTextAttribute(hConsole, 4);
                                        cout << "   Enter 1             Enter 5\n\n";
                                        SetConsoleTextAttribute(hConsole, 1);
                                        cout << "Invisi obstacles         Dual\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << "#             "; cout << "      #             $\n";
                                        cout << "#             "; cout << "      #   *         $\n";
                                        cout << "#     "; SetConsoleTextAttribute(hConsole, 7); cout << "$             "; SetConsoleTextAttribute(hConsole, 15); cout << "#   *\n";
                                        cout << "#     "; SetConsoleTextAttribute(hConsole, 7); cout << "$             "; SetConsoleTextAttribute(hConsole, 15); cout << "#        \n";
                                        cout << "#             "; cout << "      ###############\n";
                                        cout << "#   *     "; SetConsoleTextAttribute(hConsole, 7); cout << "$         "; SetConsoleTextAttribute(hConsole, 15); cout << "#        $\n";
                                        cout << "#   *     "; SetConsoleTextAttribute(hConsole, 7); cout << "$         "; SetConsoleTextAttribute(hConsole, 15); cout << "#   *    $\n";
                                        cout << "###############"; cout << "     #   *\n";
                                        SetConsoleTextAttribute(hConsole, 4);
                                        cout << "    Enter 2              Enter 6\n\n";
                                        SetConsoleTextAttribute(hConsole, 1);
                                        cout << "Invisi obstacles 2  Symmetrical dual\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << "#    "; cout << "               #            $\n";
                                        cout << "#    "; cout << "               #   *        $\n";
                                        cout << "#   "; SetConsoleTextAttribute(hConsole, 1); cout << "$               "; SetConsoleTextAttribute(hConsole, 15); cout << "#   *\n";
                                        cout << "#   "; SetConsoleTextAttribute(hConsole, 7); cout << "$               "; SetConsoleTextAttribute(hConsole, 15); cout << "#        \n";
                                        cout << "#     "; cout << "              #\n";
                                        cout << "#   *         "; SetConsoleTextAttribute(hConsole, 7); cout << "$     "; SetConsoleTextAttribute(hConsole, 15); cout << "#        $\n";
                                        cout << "#   *         "; SetConsoleTextAttribute(hConsole, 2); cout << "$     "; SetConsoleTextAttribute(hConsole, 15); cout << "#   *    $\n";
                                        cout << "###############"; cout << "     #   *\n";
                                        SetConsoleTextAttribute(hConsole, 4);
                                        cout << "    Enter 3           Enter 7\n\n";
                                        SetConsoleTextAttribute(hConsole, 1);
                                        cout << "Flappy saur        Inversed map          Portals\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << "#      $     $     #             #    #              \n";
                                        cout << "#      $     $     #     $$      #    #          |   \n";
                                        cout << "#            $     #             #    #          | $ \n";
                                        cout << "#  *         $     #        $$   #    # $          $ \n";
                                        cout << "#  *   $           # $$          #    # $            \n";
                                        cout << "#      $           #             #    #   *   |      \n";
                                        cout << "#      $     $     #     **      #    #   *   |      \n";
                                        cout << "###############    ###############    ###############\n";
                                        SetConsoleTextAttribute(hConsole, 4);
                                        cout << "    Enter 4         Enter 8             Enter 12\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                    }
                                    SetConsoleTextAttribute(hConsole, 15);
                                    cout << "Large icons: ";
                                    if (Convertion(8, points) == 1) { SetConsoleTextAttribute(hConsole, 2); cout << "ON\n"; }
                                    if (Convertion(8, points) == 0) { SetConsoleTextAttribute(hConsole, 4); cout << "OFF\n"; }
                                    SetConsoleTextAttribute(hConsole, 15);
                                    cout << "Enter '-2'\n";
                                    while (S != "0")
                                    {
                                        cin >> S;
                                        if (S == "1") { break; } if (S == "2") { break; } if (S == "3") { break; } if (S == "4") { break; } if (S == "5") { break; } if (S == "6") { break; } if (S == "-2") { break; }
                                    }
                                    if (S == "-2" && Convertion(8, points) == 1)
                                    {
                                        points -= 128; S = "-1";
                                    }
                                    else if (S == "-2")
                                    {
                                        points += 128; S = "-1";
                                    }
                                }
                                // Less few seconds to avoid
                                if (S == "1")
                                {
                                    system("cls");
                                    S = "-1";
                                    while (S != "0")
                                    {
                                        if (Convertion(1, points) == 1)
                                        {
                                            SetConsoleTextAttribute(hConsole, 15);
                                            cout << " > ";
                                            SetConsoleTextAttribute(hConsole, 1);
                                            cout << "Settings";
                                            SetConsoleTextAttribute(hConsole, 15);
                                            cout << " > ";
                                            SetConsoleTextAttribute(hConsole, 1);
                                            cout << "Challenges";
                                            SetConsoleTextAttribute(hConsole, 15);
                                            cout << " > ";
                                            SetConsoleTextAttribute(hConsole, 1);
                                            cout << "Less few seconds to avoid\n";
                                            SetConsoleTextAttribute(hConsole, 15);
                                        }
                                        if (Convertion(2, points) == 1) { SetConsoleTextAttribute(hConsole, 15); cout << "<- Go back\n"; SetConsoleTextAttribute(hConsole, 4); cout << "Enter 0\n"; }
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << "############"; cout << "     The speed settings are standard, gravity does not affect the player,\n";
                                        cout << "#          #"; cout << "     the interval between obstacle spawns is very small, map height is 15\n";
                                        cout << "#    $     #"; cout << "     the length of the map is as short as possible.\n";
                                        cout << "#    $     #"; cout << "     Collect"; SetConsoleTextAttribute(hConsole, 1); cout << " 1000 points "; SetConsoleTextAttribute(hConsole, 15); cout << "to complete the challenge.\n";
                                        cout << "#          #\n";
                                        cout << "#          #\n";
                                        cout << "#$         #\n";
                                        cout << "#$         #\n";
                                        cout << "#   $      #\n";
                                        cout << "#   $      #\n";
                                        cout << "#          #\n";
                                        cout << "#          #\n";
                                        cout << "#  *  $    #\n";
                                        cout << "#  *  $    #\n";
                                        cout << "############\n";
                                        SetConsoleTextAttribute(hConsole, 4);
                                        cout << "\nEnter 1 to start\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                        while (true)
                                        {
                                            cin >> S; if (S == "0") { break; } if (S == "1") { break; }
                                        }
                                        if (S == "0") { S = "-1"; break; }
                                        if (S == "1")
                                        {
                                            system("cls");
                                            {
                                                size = 20; points = 0; x = 13; y = 15; speed = 900;
                                                short* obs = new short[size]; //obstacles
                                                for (int i = 0; i < size; i++)
                                                {
                                                    obs[i] = 0;
                                                }
                                                for (int a = 0; a < (size / 2); a++)
                                                {
                                                    obs[2 * a] = rand() % (y - 2);
                                                }
                                                SetConsoleTextAttribute(hConsole, Up); cPos.X = H_indent; cPos.Y = 1 + V_indent; SetConsoleCursorPosition(hConsole, cPos); for (int i = 0; i < x + 2; i++) { cout << "#"; }
                                                SetConsoleTextAttribute(hConsole, Down); cPos.X = H_indent; cPos.Y = y + 3 + V_indent; SetConsoleCursorPosition(hConsole, cPos); for (int i = 0; i < x + 2; i++) { cout << "#"; }
                                                SetConsoleTextAttribute(hConsole, Left); cPos.X = H_indent; for (int i = 0; i < y + 3; i++) { cPos.Y = i + 1 + V_indent; SetConsoleCursorPosition(hConsole, cPos); cout << "#"; }
                                                SetConsoleTextAttribute(hConsole, Right); cPos.X = x + 1 + H_indent; for (int i = 0; i < y + 3; i++) { cPos.Y = i + 1 + V_indent; SetConsoleCursorPosition(hConsole, cPos); cout << "#"; }
                                                while (game_over == false)
                                                {
                                                    Input();
                                                    Logic();
                                                    Draw(obs, size);
                                                    if (points == 1000) { break; }
                                                }
                                                game_over = false; system("cls"); delete[] obs;
                                                if (points == 1000)
                                                {
                                                    ChallengeComplete(1000);
                                                    system("cls");
                                                    if (Convertion(1, Challenge) != 1) { Challenge += 1; }
                                                    if (Convertion(1, Challenge) == 1)
                                                    {
                                                        SetConsoleTextAttribute(hConsole, 15);
                                                        for (int i = 0; i < 90; i++) { cout << " "; }
                                                        cout << " ##   #   #   #   ## \n"; for (int i = 0; i < 90; i++) { cout << " "; }
                                                        cout << "#    # #  #  # #  # #\n"; for (int i = 0; i < 90; i++) { cout << " "; }
                                                        cout << "#    # #  #  # #  ## \n"; for (int i = 0; i < 90; i++) { cout << " "; }
                                                        cout << "#    # #  #  # #  # #\n"; for (int i = 0; i < 90; i++) { cout << " "; }
                                                        cout << " ##   #   #   #   # #\n"; for (int i = 0; i < 90; i++) { cout << " "; }
                                                        cout << "      Unlocked!";
                                                        this_thread::sleep_for(chrono::milliseconds(2000));
                                                        system("cls");
                                                    }
                                                    obstacles = 1;
                                                    ofstream out;
                                                    out.open("Challenge.txt");
                                                    if (out.is_open())
                                                    {
                                                        srand(time(NULL));
                                                        int j = 0;
                                                        for (int i = 0; i < 1000; i++)
                                                        {
                                                            j = rand() % 1000000;
                                                            if (i < 35 && i % 5 == 0)
                                                            {
                                                                if (j % 10 == 0) { j++; } if (j % 10 > 5) { j--; }
                                                                obstacles *= (j % 10);
                                                            }
                                                            if (i == 75) { Challenge += obstacles; out << Challenge << "\n"; }
                                                            else { out << j << "\n"; }
                                                        }
                                                        Challenge -= obstacles;
                                                    }
                                                    out.close();
                                                    obstacles = 0;
                                                }
                                                system("cls");
                                                ifstream in("Saves.txt"); // open file .txt
                                                if (in.is_open())
                                                {
                                                    in >> x >> y;
                                                    in >> points;
                                                    in >> Left >> Right >> Down >> Up;
                                                    in >> ObsNum;
                                                }
                                                in.close();     // close file
                                            }
                                        }
                                    } S = "-1";
                                }
                                // Invisi Obstacles
                                if (S == "2")
                                {
                                    system("cls"); S = "-1";
                                    while (S != "0")
                                    {
                                        if (Convertion(1, points) == 1)
                                        {
                                            SetConsoleTextAttribute(hConsole, 15);
                                            cout << " > ";
                                            SetConsoleTextAttribute(hConsole, 1);
                                            cout << "Settings";
                                            SetConsoleTextAttribute(hConsole, 15);
                                            cout << " > ";
                                            SetConsoleTextAttribute(hConsole, 1);
                                            cout << "Challenges";
                                            SetConsoleTextAttribute(hConsole, 15);
                                            cout << " > ";
                                            SetConsoleTextAttribute(hConsole, 1);
                                            cout << "Invisi Obstacles\n";
                                            SetConsoleTextAttribute(hConsole, 15);
                                        }
                                        if (Convertion(2, points) == 1) { SetConsoleTextAttribute(hConsole, 15); cout << "<- Go back\n"; SetConsoleTextAttribute(hConsole, 4); cout << "Enter 0\n"; }
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << "################################"; cout << "     The speed settings are standard, gravity does not affect the player,\n";
                                        cout << "#                              #"; cout << "     map height is 15, map length is max, after reaching 200 points,\n";
                                        cout << "#     $                  $     #"; cout << "     obstacles will become invisible and will be shown for a moment every next 100 points.\n";
                                        cout << "#     $                  $     #"; cout << "     Collect"; SetConsoleTextAttribute(hConsole, 1); cout << " 2000 points "; SetConsoleTextAttribute(hConsole, 15); cout << "to complete the challenge.\n";
                                        cout << "#               $              #\n";
                                        cout << "#               $              #\n";
                                        cout << "#                              #\n";
                                        cout << "#                              #\n";
                                        cout << "#         $                    #\n";
                                        cout << "#         $                    #\n";
                                        cout << "#                              #\n";
                                        cout << "#                         $    #\n";
                                        cout << "#  *           $          $    #\n";
                                        cout << "#  *           $               #\n";
                                        cout << "################################\n";
                                        SetConsoleTextAttribute(hConsole, 4);
                                        cout << "\nEnter 1 to start\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                        while (true)
                                        {
                                            cin >> S; if (S == "0") { break; } if (S == "1") { break; }
                                        }
                                        if (S == "0") { S = "-1"; break; }
                                        if (S == "1")
                                        {
                                            system("cls");
                                            points = 0; x = 209; y = 15; speed = 900;
                                            size = (x - 10 - (x % 10)) / 5;
                                            short* obs = new short[size]; //obstacles
                                            for (int i = 0; i < size; i++)
                                            {
                                                obs[i] = 0;
                                            }
                                            for (int a = 0; a < (size / 2); a++)
                                            {
                                                obs[2 * a] = rand() % (y);
                                            }
                                            SetConsoleTextAttribute(hConsole, Up); cPos.X = H_indent; cPos.Y = 1 + V_indent; SetConsoleCursorPosition(hConsole, cPos); for (int i = 0; i < x + 2; i++) { cout << "#"; }
                                            SetConsoleTextAttribute(hConsole, Down); cPos.X = H_indent; cPos.Y = y + 3 + V_indent; SetConsoleCursorPosition(hConsole, cPos); for (int i = 0; i < x + 2; i++) { cout << "#"; }
                                            SetConsoleTextAttribute(hConsole, Left); cPos.X = H_indent; for (int i = 0; i < y + 3; i++) { cPos.Y = i + 1 + V_indent; SetConsoleCursorPosition(hConsole, cPos); cout << "#"; }
                                            SetConsoleTextAttribute(hConsole, Right); cPos.X = x + 1 + H_indent; for (int i = 0; i < y + 3; i++) { cPos.Y = i + 1 + V_indent; SetConsoleCursorPosition(hConsole, cPos); cout << "#"; }
                                            while (game_over == false)
                                            {
                                                Input();
                                                Logic();
                                                Draw(obs, size);
                                                if (points > 200 && points % 100 > 10) { ObstaclesColor = 0; }
                                                else { ObstaclesColor = 15; }
                                                if (points == 2000) { break; }
                                            }
                                            game_over = false; system("cls"); delete[] obs;
                                            if (points == 2000)
                                            {
                                                ChallengeComplete(2000);
                                                system("cls");
                                                if (Convertion(2, Challenge) != 1) { Challenge += 2; }
                                                if (Convertion(2, Challenge) == 1)
                                                {
                                                    SetConsoleTextAttribute(hConsole, 15);
                                                    for (int i = 0; i < 90; i++) { cout << " "; }
                                                    cout << "Obstacles in color settings\n"; for (int i = 0; i < 90; i++) { cout << " "; }
                                                    cout << "      Unlocked!";
                                                    this_thread::sleep_for(chrono::milliseconds(2000));
                                                    system("cls");
                                                }
                                                obstacles = 1;
                                                ofstream out;
                                                out.open("Challenge.txt");
                                                if (out.is_open())
                                                {
                                                    srand(time(NULL));
                                                    int j = 0;
                                                    for (int i = 0; i < 1000; i++)
                                                    {
                                                        j = rand() % 1000000;
                                                        if (i < 35 && i % 5 == 0)
                                                        {
                                                            if (j % 10 == 0) { j++; } if (j % 10 > 5) { j--; }
                                                            obstacles *= (j % 10);
                                                        }
                                                        if (i == 75) { Challenge += obstacles; out << Challenge << "\n"; }
                                                        else { out << j << "\n"; }
                                                    }
                                                    Challenge -= obstacles;
                                                }
                                                out.close();
                                                obstacles = 0;
                                            }
                                            system("cls");
                                            ifstream in("Saves.txt"); // open file .txt
                                            if (in.is_open())
                                            {
                                                in >> x >> y;
                                                in >> points;
                                                in >> Left >> Right >> Down >> Up;
                                                in >> ObsNum;
                                            }
                                            in.close();     // close file
                                        }
                                    } S = "-1";
                                }
                                // Invisi Obstacles 2
                                if (S == "3")
                                {
                                    system("cls"); S = "-1";
                                    while (S != "0")
                                    {
                                        if (Convertion(1, points) == 1)
                                        {
                                            SetConsoleTextAttribute(hConsole, 15);
                                            cout << " > ";
                                            SetConsoleTextAttribute(hConsole, 1);
                                            cout << "Settings";
                                            SetConsoleTextAttribute(hConsole, 15);
                                            cout << " > ";
                                            SetConsoleTextAttribute(hConsole, 1);
                                            cout << "Challenges";
                                            SetConsoleTextAttribute(hConsole, 15);
                                            cout << " > ";
                                            SetConsoleTextAttribute(hConsole, 1);
                                            cout << "Invisi Obstacles 2\n";
                                            SetConsoleTextAttribute(hConsole, 15);
                                        }
                                        if (Convertion(2, points) == 1) { SetConsoleTextAttribute(hConsole, 15); cout << "<- Go back\n"; SetConsoleTextAttribute(hConsole, 4); cout << "Enter 0\n"; }
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << "################################"; cout << "     The speed value is 930, gravity does not affect the player,\n";
                                        cout << "#                        "; SetConsoleTextAttribute(hConsole, 1); cout << "$"; SetConsoleTextAttribute(hConsole, 15); cout << "     #"; cout << "     map height is 15, map length is max. Obstacles are not fully visible: \n";
                                        cout << "#     $         $        $     #"; cout << "     obstacles with a blue visible part have 3 more cells on the bottom,\n";
                                        cout << "#     $         $        $     #"; cout << "     and green ones have 3 more cells on top.\n";
                                        cout << "#     $         $        $     #"; cout << "     Collect"; SetConsoleTextAttribute(hConsole, 1); cout << " 2000 points "; SetConsoleTextAttribute(hConsole, 15); cout << "to complete the challenge.\n";
                                        cout << "#     "; SetConsoleTextAttribute(hConsole, 2); cout << "$         $"; SetConsoleTextAttribute(hConsole, 15); cout << "              #\n";
                                        cout << "#                              #\n";
                                        cout << "#                              #\n";
                                        cout << "#         "; SetConsoleTextAttribute(hConsole, 1); cout << "$"; SetConsoleTextAttribute(hConsole, 15); cout << "                    #\n";
                                        cout << "#         $               "; SetConsoleTextAttribute(hConsole, 1); cout << "$"; SetConsoleTextAttribute(hConsole, 15); cout << "    #\n";
                                        cout << "#         $    $          $    #\n";
                                        cout << "#         $    $          $    #\n";
                                        cout << "#  *           $          $    #\n";
                                        cout << "#  *           "; SetConsoleTextAttribute(hConsole, 2); cout << "$"; SetConsoleTextAttribute(hConsole, 15); cout << "               #\n";
                                        cout << "################################\n";
                                        SetConsoleTextAttribute(hConsole, 4);
                                        cout << "\nEnter 1 to start\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                        while (true)
                                        {
                                            cin >> S; if (S == "0") { break; } if (S == "1") { break; }
                                        }
                                        if (S == "0") { S = "-1"; break; }
                                        if (S == "1")
                                        {
                                            system("cls");
                                            x = 209; y = 15; speed = 930;
                                            size = (x - 10 - (x % 10)) / 5;
                                            short* obs = new short[size]; //obstacles
                                            for (int i = 0; i < size; i++)
                                            {
                                                obs[i] = 0;
                                            }
                                            for (int a = 0; a < (size / 2); a++)
                                            {
                                                obs[2 * a] = rand() % (13);
                                            }
                                            SetConsoleTextAttribute(hConsole, Up); cPos.X = H_indent; cPos.Y = 1 + V_indent; SetConsoleCursorPosition(hConsole, cPos); for (int i = 0; i < x + 2; i++) { cout << "#"; }
                                            SetConsoleTextAttribute(hConsole, Down); cPos.X = H_indent; cPos.Y = y + 3 + V_indent; SetConsoleCursorPosition(hConsole, cPos); for (int i = 0; i < x + 2; i++) { cout << "#"; }
                                            SetConsoleTextAttribute(hConsole, Left); cPos.X = H_indent; for (int i = 0; i < y + 3; i++) { cPos.Y = i + 1 + V_indent; SetConsoleCursorPosition(hConsole, cPos); cout << "#"; }
                                            SetConsoleTextAttribute(hConsole, Right); cPos.X = x + 1 + H_indent; for (int i = 0; i < y + 3; i++) { cPos.Y = i + 1 + V_indent; SetConsoleCursorPosition(hConsole, cPos); cout << "#"; }
                                            short* FirstColor = new short[size / 2];
                                            short* SecondColor = new short[size / 2];
                                            for (int i = 0; i < (size / 2); i++)
                                            {
                                                if (i % 3 == 0 || points % 2 == 9)
                                                {
                                                    FirstColor[i] = 1;
                                                    SecondColor[i] = 0;
                                                }
                                                else
                                                {
                                                    FirstColor[i] = 0;
                                                    SecondColor[i] = 2;
                                                }
                                            }
                                            points = 0;
                                            while (game_over == false)
                                            {
                                                Input();
                                                Logic();
                                                cPos.X = 0; cPos.Y = 0; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 1); cout << "Points: " << points; for (int a = 0; a < 15; a++) { cout << " "; }
                                                SetConsoleTextAttribute(hConsole, 15);
                                                cPos.X = 5; cPos.Y = pos + 3; SetConsoleCursorPosition(hConsole, cPos); cout << "*"; cPos.X = 5; cPos.Y = pos + 4; SetConsoleCursorPosition(hConsole, cPos); cout << "*";
                                                for (int a = 0; a < (size / 2); a++)
                                                {
                                                    if (obs[2 * a + 1] > 10 * a)
                                                    {
                                                        if (FirstColor[a] == 1)
                                                        {
                                                            cPos.X = x - obs[2 * a + 1] + 10 * a + 1; cPos.Y = obs[2 * a] + 2; SetConsoleTextAttribute(hConsole, FirstColor[a]); SetConsoleCursorPosition(hConsole, cPos); cout << "$";
                                                            cPos.X = x - obs[2 * a + 1] + 10 * a + 1; cPos.Y = obs[2 * a] + 3; SetConsoleTextAttribute(hConsole, SecondColor[a]); SetConsoleCursorPosition(hConsole, cPos); cout << "$";
                                                            cPos.X = x - obs[2 * a + 1] + 10 * a + 1; cPos.Y = obs[2 * a] + 4; SetConsoleTextAttribute(hConsole, SecondColor[a]); SetConsoleCursorPosition(hConsole, cPos); cout << "$";
                                                            cPos.X = x - obs[2 * a + 1] + 10 * a + 1; cPos.Y = obs[2 * a] + 5; SetConsoleTextAttribute(hConsole, SecondColor[a]); SetConsoleCursorPosition(hConsole, cPos); cout << "$";
                                                        }
                                                        if (SecondColor[a] == 2)
                                                        {
                                                            cPos.X = x - obs[2 * a + 1] + 10 * a + 1; cPos.Y = obs[2 * a] + 2; SetConsoleTextAttribute(hConsole, FirstColor[a]); SetConsoleCursorPosition(hConsole, cPos); cout << "$";
                                                            cPos.X = x - obs[2 * a + 1] + 10 * a + 1; cPos.Y = obs[2 * a] + 3; SetConsoleTextAttribute(hConsole, FirstColor[a]); SetConsoleCursorPosition(hConsole, cPos); cout << "$";
                                                            cPos.X = x - obs[2 * a + 1] + 10 * a + 1; cPos.Y = obs[2 * a] + 4; SetConsoleTextAttribute(hConsole, FirstColor[a]); SetConsoleCursorPosition(hConsole, cPos); cout << "$";
                                                            cPos.X = x - obs[2 * a + 1] + 10 * a + 1; cPos.Y = obs[2 * a] + 5; SetConsoleTextAttribute(hConsole, SecondColor[a]); SetConsoleCursorPosition(hConsole, cPos); cout << "$";
                                                        }
                                                        if (x - obs[2 * a + 1] + 10 * a == 5 && (obs[2 * a] + 2 == pos + 2 || obs[2 * a] + 2 == pos + 3 || obs[2 * a] + 2 == pos + 4))
                                                        {
                                                            game_over = true; break;
                                                        }
                                                    }
                                                }
                                                this_thread::sleep_for(chrono::milliseconds(1000 - speed + 1));
                                                cPos.X = 5; cPos.Y = pos + 3; SetConsoleCursorPosition(hConsole, cPos); cout << " "; cPos.X = 5; cPos.Y = pos + 4; SetConsoleCursorPosition(hConsole, cPos); cout << " ";
                                                for (int a = 0; a < (size / 2); a++)
                                                {
                                                    if (obs[2 * a + 1] > 10 * a)
                                                    {
                                                        cPos.X = x - obs[2 * a + 1] + 10 * a + 1; cPos.Y = obs[2 * a] + 2; SetConsoleCursorPosition(hConsole, cPos); cout << " ";
                                                        cPos.X = x - obs[2 * a + 1] + 10 * a + 1; cPos.Y = obs[2 * a] + 3; SetConsoleCursorPosition(hConsole, cPos); cout << " ";
                                                        cPos.X = x - obs[2 * a + 1] + 10 * a + 1; cPos.Y = obs[2 * a] + 4; SetConsoleCursorPosition(hConsole, cPos); cout << " ";
                                                        cPos.X = x - obs[2 * a + 1] + 10 * a + 1; cPos.Y = obs[2 * a] + 5; SetConsoleCursorPosition(hConsole, cPos); cout << " ";
                                                    }
                                                }
                                                //random
                                                for (int a = 0; a < (size / 2); a++)
                                                {
                                                    if (obs[2 * a + 1] == x + 10 * a)
                                                    {
                                                        obs[2 * a + 1] = 0;
                                                        if (points % 3 == 0 || a % 10 == 9)
                                                        {
                                                            FirstColor[a] = 1;
                                                            SecondColor[a] = 0;
                                                        }
                                                        else
                                                        {
                                                            FirstColor[a] = 0;
                                                            SecondColor[a] = 2;
                                                        }
                                                        srand(time(0));
                                                        if ((randtime % ((5 + a) + obs[2 * a] % (5 + a))) == 0)
                                                        {
                                                            obs[2 * a] = rand() % 13;
                                                            randtime = 0;
                                                        }
                                                    }
                                                }
                                                points++; if (points == 2000) { break; }
                                                for (int a = 0; a < (size / 2); a++)
                                                {
                                                    obs[2 * a + 1]++;
                                                }
                                            }
                                            delete[] FirstColor; delete[] SecondColor; delete[] obs;
                                            game_over = false; system("cls");
                                            if (points == 2000)
                                            {
                                                ChallengeComplete(2000);
                                                system("cls");
                                                if (Convertion(3, Challenge) != 1) { Challenge += 4; }
                                                if (Convertion(3, Challenge) == 1)
                                                {
                                                    SetConsoleTextAttribute(hConsole, 15);
                                                    for (int i = 0; i < 90; i++) { cout << " "; }
                                                    cout << "Obstacles settings\n"; for (int i = 0; i < 90; i++) { cout << " "; }
                                                    cout << "     Unlocked!";
                                                    this_thread::sleep_for(chrono::milliseconds(2000));
                                                    system("cls");
                                                }
                                                obstacles = 1;
                                                ofstream out;
                                                out.open("Challenge.txt");
                                                if (out.is_open())
                                                {
                                                    srand(time(NULL));
                                                    int j = 0;
                                                    for (int i = 0; i < 1000; i++)
                                                    {
                                                        j = rand() % 1000000;
                                                        if (i < 35 && i % 5 == 0)
                                                        {
                                                            if (j % 10 == 0) { j++; } if (j % 10 > 5) { j--; }
                                                            obstacles *= (j % 10);
                                                        }
                                                        if (i == 75) { Challenge += obstacles; out << Challenge << "\n"; }
                                                        else { out << j << "\n"; }
                                                    }
                                                    Challenge -= obstacles;
                                                }
                                                out.close();
                                                obstacles = 0;
                                            }
                                            system("cls");
                                            ifstream in("Saves.txt"); // open file .txt
                                            if (in.is_open())
                                            {
                                                in >> x >> y;
                                                in >> points;
                                                in >> Left >> Right >> Down >> Up;
                                                in >> ObsNum;
                                            }
                                            in.close();     // close file
                                        }
                                    } S = "-1";
                                }
                                // S is for speed
                                if (S == "4")
                                {
                                    system("cls"); S = "-1";
                                    while (S != "0")
                                    {
                                        if (Convertion(1, points) == 1)
                                        {
                                            SetConsoleTextAttribute(hConsole, 15);
                                            cout << " > ";
                                            SetConsoleTextAttribute(hConsole, 1);
                                            cout << "Settings";
                                            SetConsoleTextAttribute(hConsole, 15);
                                            cout << " > ";
                                            SetConsoleTextAttribute(hConsole, 1);
                                            cout << "Challenges";
                                            SetConsoleTextAttribute(hConsole, 15);
                                            cout << " > ";
                                            SetConsoleTextAttribute(hConsole, 1);
                                            cout << "S is for speed\n";
                                            SetConsoleTextAttribute(hConsole, 15);
                                        }
                                        if (Convertion(2, points) == 1) { SetConsoleTextAttribute(hConsole, 15); cout << "<- Go back\n"; SetConsoleTextAttribute(hConsole, 4); cout << "Enter 0\n"; }
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << "################################"; cout << "     The speed value is max, map height is 10, map length is max\n";
                                        cout << "#                              #"; cout << "     all other settings are standard\n";
                                        cout << "#     $                  $     #"; cout << "     Collect"; SetConsoleTextAttribute(hConsole, 1); cout << " 5000 points "; SetConsoleTextAttribute(hConsole, 15); cout << "to complete the challenge.\n";
                                        cout << "#     $                  $     #\n";
                                        cout << "#               $              #\n";
                                        cout << "#               $              #\n";
                                        cout << "#                              #\n";
                                        cout << "#                              #\n";
                                        cout << "#         $                    #\n";
                                        cout << "#         $                    #\n";
                                        cout << "#                              #\n";
                                        cout << "#                         $    #\n";
                                        cout << "#  *           $          $    #\n";
                                        cout << "#  *           $               #\n";
                                        cout << "################################\n";
                                        SetConsoleTextAttribute(hConsole, 4);
                                        cout << "\nEnter 1 to start\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                        while (true)
                                        {
                                            cin >> S; if (S == "0") { break; } if (S == "1") { break; }
                                        }
                                        if (S == "0") { S = "-1"; break; }
                                        if (S == "1")
                                        {
                                            system("cls");
                                            points = 0; x = 209; y = 10; speed = 1000;
                                            size = (x - 10 - (x % 10)) / 5;
                                            short* obs = new short[size]; //obstacles
                                            for (int i = 0; i < size; i++)
                                            {
                                                obs[i] = 0;
                                            }
                                            for (int a = 0; a < (size / 2); a++)
                                            {
                                                obs[2 * a] = rand() % (y);
                                            }
                                            SetConsoleTextAttribute(hConsole, Up); cPos.X = H_indent; cPos.Y = 1 + V_indent; SetConsoleCursorPosition(hConsole, cPos); for (int i = 0; i < x + 2; i++) { cout << "#"; }
                                            SetConsoleTextAttribute(hConsole, Down); cPos.X = H_indent; cPos.Y = y + 3 + V_indent; SetConsoleCursorPosition(hConsole, cPos); for (int i = 0; i < x + 2; i++) { cout << "#"; }
                                            SetConsoleTextAttribute(hConsole, Left); cPos.X = H_indent; for (int i = 0; i < y + 3; i++) { cPos.Y = i + 1 + V_indent; SetConsoleCursorPosition(hConsole, cPos); cout << "#"; }
                                            SetConsoleTextAttribute(hConsole, Right); cPos.X = x + 1 + H_indent; for (int i = 0; i < y + 3; i++) { cPos.Y = i + 1 + V_indent; SetConsoleCursorPosition(hConsole, cPos); cout << "#"; }
                                            while (game_over == false)
                                            {
                                                Input();
                                                Logic();
                                                Draw(obs, size);
                                                if (points == 5000) { break; }
                                            }
                                            game_over = false; system("cls"); delete[] obs;
                                            if (points == 5000)
                                            {
                                                ChallengeComplete(5000);
                                                system("cls");
                                                if (Convertion(4, Challenge) != 1) { Challenge += 8; }
                                                if (Convertion(4, Challenge) == 1)
                                                {
                                                    system("cls");
                                                }
                                                obstacles = 1;
                                                ofstream out;
                                                out.open("Challenge.txt");
                                                if (out.is_open())
                                                {
                                                    srand(time(NULL));
                                                    int j = 0;
                                                    for (int i = 0; i < 1000; i++)
                                                    {
                                                        j = rand() % 1000000;
                                                        if (i < 35 && i % 5 == 0)
                                                        {
                                                            if (j % 10 == 0) { j++; } if (j % 10 > 5) { j--; }
                                                            obstacles *= (j % 10);
                                                        }
                                                        if (i == 75) { Challenge += obstacles; out << Challenge << "\n"; }
                                                        else { out << j << "\n"; }
                                                    }
                                                    Challenge -= obstacles;
                                                }
                                                out.close();
                                                obstacles = 0;
                                            }
                                            system("cls");
                                            ifstream in("Saves.txt"); // open file .txt
                                            if (in.is_open())
                                            {
                                                in >> x >> y;
                                                in >> points;
                                                in >> Left >> Right >> Down >> Up;
                                                in >> ObsNum;
                                            }
                                            in.close();     // close file
                                        }
                                    } S = "-1";
                                }
                                // falling obstacles
                                if (S == "5")
                                {
                                    system("cls"); S = "-1";
                                    while (S != "0")
                                    {
                                        if (Convertion(1, points) == 1)
                                        {
                                            SetConsoleTextAttribute(hConsole, 15);
                                            cout << " > ";
                                            SetConsoleTextAttribute(hConsole, 1);
                                            cout << "Settings";
                                            SetConsoleTextAttribute(hConsole, 15);
                                            cout << " > ";
                                            SetConsoleTextAttribute(hConsole, 1);
                                            cout << "Challenges";
                                            SetConsoleTextAttribute(hConsole, 15);
                                            cout << " > ";
                                            SetConsoleTextAttribute(hConsole, 1);
                                            cout << "Falling obstacles\n";
                                            SetConsoleTextAttribute(hConsole, 15);
                                        }
                                        if (Convertion(2, points) == 1) { SetConsoleTextAttribute(hConsole, 15); cout << "<- Go back\n"; SetConsoleTextAttribute(hConsole, 4); cout << "Enter 0\n"; }
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << "################################"; cout << "     Gravity does not affect the player,\n";
                                        cout << "#                              #"; cout << "     map height is 10, map length is max, speed value is 920.\n";
                                        cout << "#     $                  $     #"; cout << "     Obstacles move one cell in the vertical plane each time.\n";
                                        cout << "#     $                  $     #"; cout << "     Collect"; SetConsoleTextAttribute(hConsole, 1); cout << " 2000 points "; SetConsoleTextAttribute(hConsole, 15); cout << "to complete the challenge.\n";
                                        cout << "#               $              #\n";
                                        cout << "#               $              #\n";
                                        cout << "#                              #\n";
                                        cout << "#                              #\n";
                                        cout << "#         $                    #\n";
                                        cout << "#         $                    #\n";
                                        cout << "#                              #\n";
                                        cout << "#                         $    #\n";
                                        cout << "#  *           $          $    #\n";
                                        cout << "#  *           $               #\n";
                                        cout << "################################\n";
                                        SetConsoleTextAttribute(hConsole, 4);
                                        cout << "\nEnter 1 to start\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                        while (true)
                                        {
                                            cin >> S; if (S == "0") { break; } if (S == "1") { break; }
                                        }
                                        if (S == "0") { S = "-1"; break; }
                                        if (S == "1")
                                        {
                                            system("cls");
                                            points = 0; x = 209; y = 20; speed = 920;
                                            size = (x - 10 - (x % 10)) / 5;
                                            short* obs = new short[size]; //obstacles
                                            for (int i = 0; i < size; i++)
                                            {
                                                obs[i] = 0;
                                            }
                                            for (int a = 0; a < (size / 2); a++)
                                            {
                                                obs[2 * a] = rand() % (y);
                                            }
                                            SetConsoleTextAttribute(hConsole, Up); cPos.X = H_indent; cPos.Y = 1 + V_indent; SetConsoleCursorPosition(hConsole, cPos); for (int i = 0; i < x + 2; i++) { cout << "#"; }
                                            SetConsoleTextAttribute(hConsole, Down); cPos.X = H_indent; cPos.Y = y + 3 + V_indent; SetConsoleCursorPosition(hConsole, cPos); for (int i = 0; i < x + 2; i++) { cout << "#"; }
                                            SetConsoleTextAttribute(hConsole, Left); cPos.X = H_indent; for (int i = 0; i < y + 3; i++) { cPos.Y = i + 1 + V_indent; SetConsoleCursorPosition(hConsole, cPos); cout << "#"; }
                                            SetConsoleTextAttribute(hConsole, Right); cPos.X = x + 1 + H_indent; for (int i = 0; i < y + 3; i++) { cPos.Y = i + 1 + V_indent; SetConsoleCursorPosition(hConsole, cPos); cout << "#"; }
                                            int f = 0;
                                            while (game_over == false)
                                            {
                                                Input();
                                                Logic();
                                                cPos.X = 0; cPos.Y = 0; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 1); cout << "Points: " << points; for (int a = 0; a < 15; a++) { cout << " "; }
                                                SetConsoleTextAttribute(hConsole, 15);
                                                cPos.X = 5; cPos.Y = pos + 3; SetConsoleCursorPosition(hConsole, cPos); cout << "*"; cPos.X = 5; cPos.Y = pos + 4; SetConsoleCursorPosition(hConsole, cPos); cout << "*";
                                                for (int a = 0; a < (size / 2); a++)
                                                {
                                                    if (obs[2 * a + 1] > 10 * a)
                                                    {
                                                        cPos.X = x - obs[2 * a + 1] + 10 * a + 1; cPos.Y = obs[2 * a] + 2; SetConsoleTextAttribute(hConsole, ObstaclesColor); SetConsoleCursorPosition(hConsole, cPos); cout << "$";
                                                        cPos.X = x - obs[2 * a + 1] + 10 * a + 1; cPos.Y = obs[2 * a] + 3; SetConsoleTextAttribute(hConsole, ObstaclesColor); SetConsoleCursorPosition(hConsole, cPos); cout << "$";
                                                        if (x - obs[2 * a + 1] + 10 * a == 5 && (obs[2 * a] + 2 == pos + 2 || obs[2 * a] + 2 == pos + 3 || obs[2 * a] + 2 == pos + 4)) {
                                                            game_over = true; break;
                                                        }
                                                    }
                                                }
                                                this_thread::sleep_for(chrono::milliseconds(1000 - speed + 1));
                                                cPos.X = 5; cPos.Y = pos + 3; SetConsoleCursorPosition(hConsole, cPos); cout << " "; cPos.X = 5; cPos.Y = pos + 4; SetConsoleCursorPosition(hConsole, cPos); cout << " ";
                                                for (int a = 0; a < (size / 2); a++)
                                                {
                                                    if (obs[2 * a + 1] > 10 * a)
                                                    {
                                                        cPos.X = x - obs[2 * a + 1] + 10 * a + 1; cPos.Y = obs[2 * a] + 2; SetConsoleCursorPosition(hConsole, cPos); cout << " ";
                                                        cPos.X = x - obs[2 * a + 1] + 10 * a + 1; cPos.Y = obs[2 * a] + 3; SetConsoleCursorPosition(hConsole, cPos); cout << " ";
                                                    }
                                                }
                                                //random
                                                for (int a = 0; a < (size / 2); a++)
                                                {
                                                    if (obs[2 * a + 1] == x + 10 * a)
                                                    {
                                                        obs[2 * a + 1] = 0;
                                                        srand(time(0));
                                                        if ((randtime % ((5 + a) + obs[2 * a] % (5 + a))) == 0)
                                                        {
                                                            obs[2 * a] = rand() % (y - 1) + 1;
                                                            randtime = 0;
                                                        }
                                                    }
                                                }
                                                points++;
                                                for (int a = 0; a < (size / 2); a++)
                                                {
                                                    obs[2 * a + 1]++;
                                                    if (obs[2 * a] < y - 2)
                                                    {
                                                        obs[2 * a]++;
                                                    }
                                                    else
                                                    {
                                                        obs[2 * a] = 0;
                                                    }
                                                }
                                                if (points == 2000) { break; }
                                            }
                                            game_over = false; system("cls"); delete[] obs;
                                            if (points == 2000)
                                            {
                                                ChallengeComplete(2000);
                                                system("cls");
                                                if (Convertion(5, Challenge) != 1) { Challenge += 16; }
                                                if (Convertion(5, Challenge) == 1)
                                                {
                                                    system("cls");
                                                }
                                                obstacles = 1;
                                                ofstream out;
                                                out.open("Challenge.txt");
                                                if (out.is_open())
                                                {
                                                    srand(time(NULL));
                                                    int j = 0;
                                                    for (int i = 0; i < 1000; i++)
                                                    {
                                                        j = rand() % 1000000;
                                                        if (i < 35 && i % 5 == 0)
                                                        {
                                                            if (j % 10 == 0) { j++; } if (j % 10 > 5) { j--; }
                                                            obstacles *= (j % 10);
                                                        }
                                                        if (i == 75) { Challenge += obstacles; out << Challenge << "\n"; }
                                                        else { out << j << "\n"; }
                                                    }
                                                    Challenge -= obstacles;
                                                }
                                                out.close();
                                                obstacles = 0;
                                            }
                                            system("cls");
                                            ifstream in("Saves.txt"); // open file .txt
                                            if (in.is_open())
                                            {
                                                in >> x >> y;
                                                in >> points;
                                                in >> Left >> Right >> Down >> Up;
                                                in >> ObsNum;
                                            }
                                            in.close();     // close file
                                        }
                                    } S = "-1";
                                }
                                // Dual
                                if (S == "6")
                                {
                                    system("cls"); S = "-1";
                                    while (S != "0")
                                    {
                                        if (Convertion(1, points) == 1)
                                        {
                                            SetConsoleTextAttribute(hConsole, 15);
                                            cout << " > ";
                                            SetConsoleTextAttribute(hConsole, 1);
                                            cout << "Settings";
                                            SetConsoleTextAttribute(hConsole, 15);
                                            cout << " > ";
                                            SetConsoleTextAttribute(hConsole, 1);
                                            cout << "Challenges";
                                            SetConsoleTextAttribute(hConsole, 15);
                                            cout << " > ";
                                            SetConsoleTextAttribute(hConsole, 1);
                                            cout << "Dual\n";
                                            SetConsoleTextAttribute(hConsole, 15);
                                        }
                                        if (Convertion(2, points) == 1) { SetConsoleTextAttribute(hConsole, 15); cout << "<- Go back\n"; SetConsoleTextAttribute(hConsole, 4); cout << "Enter 0\n"; }
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << "################################";  cout << "     Maps heights is 15, maps lengths is max, speed value is 900.\n";
                                        cout << "#                              #";  cout << "     Hold '4' to move the top player up, hold '1' to move the top player down.\n";
                                        cout << "# *   $                  $     #";  cout << "     Hold '5' to move the bottom player down, hold '2' to move the bottom player down.\n";
                                        cout << "# *   $                  $     #";  cout << "     The game is over if at least one player collides with an obstacle\n";
                                        cout << "#               $              #\n";
                                        cout << "#               $              #\n";
                                        cout << "#                              #\n";
                                        cout << "################################\n";
                                        cout << "#         $                    #\n";
                                        cout << "#         $                    #\n";
                                        cout << "#                              #\n";
                                        cout << "#                         $    #\n";
                                        cout << "#  *           $          $    #\n";
                                        cout << "#  *           $               #\n";
                                        cout << "################################\n";
                                        SetConsoleTextAttribute(hConsole, 4);
                                        cout << "\nEnter 1 to start\n";
                                        SetConsoleTextAttribute(hConsole, 15);
                                        while (true)
                                        {
                                            cin >> S; if (S == "0") { break; } if (S == "1") { break; }
                                        }
                                        if (S == "0") { S = "-1"; break; }
                                        if (S == "1")
                                        {
                                            system("cls");
                                            points = 0; x = 100; y = 15; speed = 900;
                                            size = (x - 10 - (x % 10)) / 5;
                                            short* obs = new short[size]; //obstacles
                                            for (int i = 0; i < size; i++)
                                            {
                                                obs[i] = 0;
                                            }
                                            for (int a = 0; a < (size / 2); a++)
                                            {
                                                obs[2 * a] = rand() % (y);
                                            }
                                            short* obs2 = new short[size]; //obstacles
                                            for (int i = 0; i < size; i++)
                                            {
                                                obs2[i] = 0;
                                            }
                                            for (int a = 0; a < (size / 2); a++)
                                            {
                                                obs2[2 * a] = rand() % (y);
                                            }
                                            SetConsoleTextAttribute(hConsole, 15); cPos.X = H_indent; cPos.Y = 1 + V_indent; SetConsoleCursorPosition(hConsole, cPos); for (int i = 0; i < x + 2; i++) { cout << "#"; }
                                            SetConsoleTextAttribute(hConsole, 15); cPos.X = H_indent; cPos.Y = y + 3 + V_indent; SetConsoleCursorPosition(hConsole, cPos); for (int i = 0; i < x + 2; i++) { cout << "#"; }
                                            SetConsoleTextAttribute(hConsole, 15); cPos.X = H_indent; for (int i = 0; i < y + 3; i++) { cPos.Y = i + 1 + V_indent; SetConsoleCursorPosition(hConsole, cPos); cout << "#"; }
                                            SetConsoleTextAttribute(hConsole, 15); cPos.X = x + 1 + H_indent; for (int i = 0; i < y + 3; i++) { cPos.Y = i + 1 + V_indent; SetConsoleCursorPosition(hConsole, cPos); cout << "#"; }
                                            SetConsoleTextAttribute(hConsole, 15); cPos.X = H_indent; cPos.Y = 2 * y + 4 + V_indent; SetConsoleCursorPosition(hConsole, cPos); for (int i = 0; i < x + 2; i++) { cout << "#"; }
                                            SetConsoleTextAttribute(hConsole, 15); cPos.X = H_indent; for (int i = 0; i < y + 3; i++) { cPos.Y = i + 2 + y + V_indent; SetConsoleCursorPosition(hConsole, cPos); cout << "#"; }
                                            SetConsoleTextAttribute(hConsole, 15); cPos.X = x + 1 + H_indent; for (int i = 0; i < y + 3; i++) { cPos.Y = i + 2 + y + V_indent; SetConsoleCursorPosition(hConsole, cPos); cout << "#"; }
                                            short pos2 = 20; short randtime2 = 0;
                                            while (game_over == false)
                                            {
                                                if (_kbhit())
                                                {
                                                    switch (_getch())
                                                    {
                                                    case '1':
                                                        if (pos < y - 2) { pos++; }
                                                        break;
                                                    case '4':
                                                        if (pos >= 0) { pos--; }
                                                        break;
                                                    case '2':
                                                        if (pos2 < 2 * (y - 2)) { pos2++; }
                                                        break;
                                                    case '5':
                                                        if (pos2 >= y + 2) { pos2--; }
                                                        break;
                                                    }
                                                }
                                                cPos.X = 0; cPos.Y = 0; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 1); cout << "Points: " << points; for (int a = 0; a < 15; a++) { cout << " "; }
                                                SetConsoleTextAttribute(hConsole, 15);
                                                cPos.X = 5; cPos.Y = pos + 3; SetConsoleCursorPosition(hConsole, cPos); cout << "*"; cPos.X = 5; cPos.Y = pos + 4; SetConsoleCursorPosition(hConsole, cPos); cout << "*";
                                                for (int a = 0; a < (size / 2); a++)
                                                {
                                                    if (obs2[2 * a + 1] > 10 * a)
                                                    {
                                                        cPos.X = x - obs[2 * a + 1] + 10 * a + 1; cPos.Y = obs[2 * a] + 2; SetConsoleTextAttribute(hConsole, ObstaclesColor); SetConsoleCursorPosition(hConsole, cPos); cout << "$";
                                                        cPos.X = x - obs[2 * a + 1] + 10 * a + 1; cPos.Y = obs[2 * a] + 3; SetConsoleTextAttribute(hConsole, ObstaclesColor); SetConsoleCursorPosition(hConsole, cPos); cout << "$";
                                                        if (x - obs[2 * a + 1] + 10 * a == 5 && (obs[2 * a] + 2 == pos + 2 || obs[2 * a] + 2 == pos + 3 || obs[2 * a] + 2 == pos + 4)) {
                                                            game_over = true; break;
                                                        }
                                                    }
                                                }
                                                cPos.X = 5; cPos.Y = pos2 + 3; SetConsoleCursorPosition(hConsole, cPos); cout << "*"; cPos.X = 5; cPos.Y = pos2 + 4; SetConsoleCursorPosition(hConsole, cPos); cout << "*";
                                                for (int a = 0; a < (size / 2); a++)
                                                {
                                                    if (obs2[2 * a + 1] > 10 * a)
                                                    {
                                                        cPos.X = x - obs2[2 * a + 1] + 10 * a + 1; cPos.Y = obs2[2 * a] + 3 + y; SetConsoleTextAttribute(hConsole, ObstaclesColor); SetConsoleCursorPosition(hConsole, cPos); cout << "$";
                                                        cPos.X = x - obs2[2 * a + 1] + 10 * a + 1; cPos.Y = obs2[2 * a] + 4 + y; SetConsoleTextAttribute(hConsole, ObstaclesColor); SetConsoleCursorPosition(hConsole, cPos); cout << "$";
                                                        if (x - obs2[2 * a + 1] + 10 * a == 5 && (obs2[2 * a] + 2 == pos2 + 2 || obs2[2 * a] + 2 == pos2 + 3 || obs2[2 * a] + 2 == pos2 + 4))
                                                        {
                                                            game_over = true; break;
                                                        }
                                                    }
                                                }
                                                this_thread::sleep_for(chrono::milliseconds(1000 - speed + 1));
                                                cPos.X = 5; cPos.Y = pos + 3; SetConsoleCursorPosition(hConsole, cPos); cout << " "; cPos.X = 5; cPos.Y = pos + 4; SetConsoleCursorPosition(hConsole, cPos); cout << " ";
                                                for (int a = 0; a < (size / 2); a++)
                                                {
                                                    if (obs[2 * a + 1] > 10 * a)
                                                    {
                                                        cPos.X = x - obs[2 * a + 1] + 10 * a + 1; cPos.Y = obs[2 * a] + 2; SetConsoleCursorPosition(hConsole, cPos); cout << " ";
                                                        cPos.X = x - obs[2 * a + 1] + 10 * a + 1; cPos.Y = obs[2 * a] + 3; SetConsoleCursorPosition(hConsole, cPos); cout << " ";
                                                    }
                                                }
                                                cPos.X = 5; cPos.Y = pos2 + 3; SetConsoleCursorPosition(hConsole, cPos); cout << " "; cPos.X = 5; cPos.Y = pos2 + 4; SetConsoleCursorPosition(hConsole, cPos); cout << " ";
                                                for (int a = 0; a < (size / 2); a++)
                                                {
                                                    if (obs[2 * a + 1] > 10 * a)
                                                    {
                                                        cPos.X = x - obs2[2 * a + 1] + 10 * a + 1; cPos.Y = obs2[2 * a] + 3 + y; SetConsoleCursorPosition(hConsole, cPos); cout << " ";
                                                        cPos.X = x - obs2[2 * a + 1] + 10 * a + 1; cPos.Y = obs2[2 * a] + 4 + y; SetConsoleCursorPosition(hConsole, cPos); cout << " ";
                                                    }
                                                }
                                                //random
                                                for (int a = 0; a < (size / 2); a++)
                                                {
                                                    if (obs[2 * a + 1] == x + 10 * a)
                                                    {
                                                        obs[2 * a + 1] = 0;
                                                        srand(time(0));
                                                        if ((randtime % ((5 + a) + obs[2 * a] % (5 + a))) == 0)
                                                        {
                                                            obs[2 * a] = rand() % (y);
                                                            randtime = 0;
                                                        }
                                                    }
                                                }
                                                for (int a = 0; a < (size / 2); a++)
                                                {
                                                    if (obs2[2 * a + 1] == x + 10 * a)
                                                    {
                                                        obs2[2 * a + 1] = 0;
                                                        srand(time(0));
                                                        if ((randtime2 % ((5 + a) + obs2[2 * a] % (5 + a))) == 0)
                                                        {
                                                            obs2[2 * a] = (rand() % (y)) + y + 1;
                                                            randtime2 = 0;
                                                        }
                                                    }
                                                }
                                                points++;
                                                for (int a = 0; a < (size / 2); a++)
                                                {
                                                    obs[2 * a + 1]++;
                                                }
                                                for (int a = 0; a < (size / 2); a++)
                                                {
                                                    obs2[2 * a + 1]++;
                                                }
                                                if (points == 1000) { break; }
                                            }
                                            game_over = false; system("cls"); delete[] obs; delete[] obs2;
                                            if (points == 1000)
                                            {
                                                ChallengeComplete(1000);
                                                system("cls");
                                                if (Convertion(4, Challenge) != 1) { Challenge += 8; }
                                                if (Convertion(4, Challenge) == 1)
                                                {
                                                    system("cls");
                                                }
                                                obstacles = 1;
                                                ofstream out;
                                                out.open("Challenge.txt");
                                                if (out.is_open())
                                                {
                                                    srand(time(NULL));
                                                    int j = 0;
                                                    for (int i = 0; i < 1000; i++)
                                                    {
                                                        j = rand() % 1000000;
                                                        if (i < 35 && i % 5 == 0)
                                                        {
                                                            if (j % 10 == 0) { j++; } if (j % 10 > 5) { j--; }
                                                            obstacles *= (j % 10);
                                                        }
                                                        if (i == 75) { Challenge += obstacles; out << Challenge << "\n"; }
                                                        else { out << j << "\n"; }
                                                    }
                                                    Challenge -= obstacles;
                                                }
                                                out.close();
                                                obstacles = 0;
                                            }
                                            system("cls");
                                            ifstream in("Saves.txt"); // open file .txt
                                            if (in.is_open())
                                            {
                                                in >> x >> y;
                                                in >> points;
                                                in >> Left >> Right >> Down >> Up;
                                                in >> ObsNum;
                                            }
                                            in.close();     // close file
                                        }
                                    } S = "-1";
                                }
                            }
                        } S = "-1";
                    }
                    // Control
                    if (S == "2")
                    {
                        S = "-1";
                        while (true)
                        {
                            system("cls");
                            if (Convertion(1, points) == 1)
                            {
                                SetConsoleTextAttribute(hConsole, 15);
                                cout << " > ";
                                SetConsoleTextAttribute(hConsole, 1);
                                cout << "Settings";
                                SetConsoleTextAttribute(hConsole, 15);
                                cout << " > ";
                                SetConsoleTextAttribute(hConsole, 1);
                                cout << "Control\n";
                                SetConsoleTextAttribute(hConsole, 15);
                            }
                            if (Convertion(2, points) == 1) { SetConsoleTextAttribute(hConsole, 15); cout << "<- Go back\n"; SetConsoleTextAttribute(hConsole, 4); cout << "Enter 0\n"; }
                            SetConsoleTextAttribute(hConsole, 15);
                            cPos.X = 3; cPos.Y = 4; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 7);
                            cout << "Game:"; cPos.Y += 2; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                            cout << "Move dino up"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 2); cout << "Enter 1"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                            cout << "Move dino down"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 2); cout << "Enter 2"; cPos.Y += 2; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 7);
                            cout << "Obstacles editor:"; cPos.Y += 2; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                            cout << "Move cursor up"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 2); cout << "Enter 3"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                            cout << "Move cursor down"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 2); cout << "Enter 4"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                            cout << "Move cursor right"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 2); cout << "Enter 5"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                            cout << "Move cursor left"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 2); cout << "Enter 6"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                            cout << "Print $"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 2); cout << "Enter 7"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                            cout << "Clear current cell"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 2); cout << "Enter 8"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                            cout << "Save obstacle"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 2); cout << "Enter 9"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                            cout << "Clear list of obstacles"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 2); cout << "Enter 10"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                            // keys
                            cPos.X = 30; cPos.Y = 6; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                            cout << "<<   " << MoveDinoUp1 << "   >>"; cPos.Y += 2; SetConsoleCursorPosition(hConsole, cPos);
                            cout << "<<   " << MoveDinoDown1 << "   >>"; cPos.Y += 5; SetConsoleCursorPosition(hConsole, cPos);
                            cout << "<<   w   >>"; cPos.Y += 2; SetConsoleCursorPosition(hConsole, cPos);
                            cout << "<<   s   >>"; cPos.Y += 2; SetConsoleCursorPosition(hConsole, cPos);
                            cout << "<<   d   >>"; cPos.Y += 2; SetConsoleCursorPosition(hConsole, cPos);
                            cout << "<<   a   >>"; cPos.Y += 2; SetConsoleCursorPosition(hConsole, cPos);
                            cout << "<<   /   >>"; cPos.Y += 2; SetConsoleCursorPosition(hConsole, cPos);
                            cout << "<<   *   >>"; cPos.Y += 2; SetConsoleCursorPosition(hConsole, cPos);
                            cout << "<<   1   >>"; cPos.Y += 2; SetConsoleCursorPosition(hConsole, cPos);
                            cout << "<<   !   >>"; cPos.Y += 2; SetConsoleCursorPosition(hConsole, cPos);
                            while (true)
                            {
                                cPos.X = 0; cPos.Y = maxWindow.Y - 5; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                                cin >> S; if (S == "0") { break; }
                                if (S == "1")
                                {
                                    cPos.X = 3; cPos.Y = 6; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 1);
                                    cout << "Move dino up";
                                    cPos.X = 0; cPos.Y = maxWindow.Y - 5; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                                    cout << "Enter key:\n"; cin >> MoveDinoUp1; break;
                                }
                                if (S == "2")
                                {
                                    cPos.X = 3; cPos.Y = 8; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 1);
                                    cout << "Move dino down";
                                    cPos.X = 0; cPos.Y = maxWindow.Y - 5; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                                    cout << "Enter key:\n"; cin >> MoveDinoDown1; break;
                                }
                            }
                            if (S == "0") { break; }
                        }
                    }
                    // Display
                    if (S == "3")
                    {
                        S = "-1";
                        while (true)
                        {
                            system("cls");
                            if (Convertion(1, points) == 1)
                            {
                                SetConsoleTextAttribute(hConsole, 15);
                                cout << " > ";
                                SetConsoleTextAttribute(hConsole, 1);
                                cout << "Settings";
                                SetConsoleTextAttribute(hConsole, 15);
                                cout << " > ";
                                SetConsoleTextAttribute(hConsole, 1);
                                cout << "Display\n";
                                SetConsoleTextAttribute(hConsole, 15);
                            }
                            if (Convertion(2, points) == 1) { SetConsoleTextAttribute(hConsole, 15); cout << "<- Go back\n"; SetConsoleTextAttribute(hConsole, 4); cout << "Enter 0\n"; }
                            SetConsoleTextAttribute(hConsole, 15);
                            cPos.X = 0; cPos.Y = 4; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 7);
                            cout << "Choose function, then enter 1 to switch value or enter 0 to exit function"; cPos.X += 5; cPos.Y += 2; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                            cout << "Function path"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 2); cout << "Enter 1";
                            cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                            cout << "Go back"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 2); cout << "Enter 2";
                            cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                            cout << "Help info"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 2); cout << "Enter 3";
                            cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                            cout << "Extra info"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 2); cout << "Enter 4";
                            cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                            cout << "Clear"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 2); cout << "Enter 5";
                            cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                            cout << "Language"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 2); cout << "Enter 6";
                            cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                            // value
                            cPos.X = 20; cPos.Y = 6; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                            cout << ">>   "; if (Convertion(1, points) == 1) { SetConsoleTextAttribute(hConsole, 2); cout << "ON "; }
                            else { SetConsoleTextAttribute(hConsole, 4); cout << "OFF"; } SetConsoleTextAttribute(hConsole, 15); cout << "  <<"; cPos.Y += 2; SetConsoleCursorPosition(hConsole, cPos);
                            cout << ">>   "; if (Convertion(2, points) == 1) { SetConsoleTextAttribute(hConsole, 2); cout << "ON "; }
                            else { SetConsoleTextAttribute(hConsole, 4); cout << "OFF"; } SetConsoleTextAttribute(hConsole, 15); cout << "  <<"; cPos.Y += 2; SetConsoleCursorPosition(hConsole, cPos);
                            cout << ">>   "; if (Convertion(4, points) == 1) { SetConsoleTextAttribute(hConsole, 2); cout << "ON "; }
                            else { SetConsoleTextAttribute(hConsole, 4); cout << "OFF"; } SetConsoleTextAttribute(hConsole, 15); cout << "  <<"; cPos.Y += 2; SetConsoleCursorPosition(hConsole, cPos);
                            cout << ">>   "; if (Convertion(7, points) == 1) { SetConsoleTextAttribute(hConsole, 2); cout << "ON "; }
                            else { SetConsoleTextAttribute(hConsole, 4); cout << "OFF"; } SetConsoleTextAttribute(hConsole, 15); cout << "  <<"; cPos.Y += 2; SetConsoleCursorPosition(hConsole, cPos);
                            cout << ">>  "; if (clear == 0) { cout << "cls1"; } if (clear == 1) { cout << "cls2"; } if (clear == 2) { cout << "cls3"; } if (clear == 3) { cout << "cls4"; } cout << "  <<"; cPos.Y += 2; SetConsoleCursorPosition(hConsole, cPos);
                            cout << ">>   "; if (Convertion(6, points) != 1) { cout << "EN"; }
                            else { cout << "RU"; } cout << "   <<"; cPos.Y += 2; SetConsoleCursorPosition(hConsole, cPos);
                            if (maxWindow.Y > 40)
                            {
                                cPos.X = 5; cPos.Y += 2; SetConsoleCursorPosition(hConsole, cPos);
                                SetConsoleTextAttribute(hConsole, 7); cout << "Color:"; SetConsoleTextAttribute(hConsole, 15); cPos.Y += 2; SetConsoleCursorPosition(hConsole, cPos);
                                cout << "Name"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 4);
                                cout << "Enter 7"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                                cout << "Input"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 4);
                                cout << "Enter 8"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                                cout << "Help"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 4);
                                cout << "Enter 9";
                            }
                            else
                            {

                            }
                            cPos.X = 0; cPos.Y = maxWindow.Y - 5; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                            while (true)
                            {
                                cin >> S; if (S == "0") { break; }
                                // function path
                                if (S == "1")
                                {
                                    cPos.X = 5; cPos.Y = 6; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 1);
                                    cout << "Function path";
                                    cPos.X = 35; cPos.Y = 6; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 1);
                                    cout << "Shows function path";
                                    while (true)
                                    {
                                        cPos.X = 0; cPos.Y = maxWindow.Y - 5; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                                        cin >> S; if (S == "0") { break; }
                                        if (S == "1")
                                        {
                                            if (Convertion(1, points) != 1)
                                            {
                                                points += 1;
                                                cPos.X = 20; cPos.Y = 6; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                                                cout << ">>   "; SetConsoleTextAttribute(hConsole, 2); cout << "ON"; SetConsoleTextAttribute(hConsole, 15); cout << "   <<";
                                            }
                                            else
                                            {
                                                points -= 1;
                                                cPos.X = 20; cPos.Y = 6; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                                                cout << ">>   "; SetConsoleTextAttribute(hConsole, 4); cout << "OFF"; SetConsoleTextAttribute(hConsole, 15); cout << "  <<";
                                            }
                                        }
                                    }
                                    S = "-1";
                                    break;
                                }
                                // Go back
                                if (S == "2")
                                {
                                    cPos.X = 5; cPos.Y = 8; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 1);
                                    cout << "Go back";
                                    cPos.X = 35; cPos.Y = 8; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 1);
                                    cout << "Shows help '<- Go back'";
                                    while (true)
                                    {
                                        cPos.X = 0; cPos.Y = maxWindow.Y - 5; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                                        cin >> S; if (S == "0") { break; }
                                        if (S == "1")
                                        {
                                            if (Convertion(2, points) != 1)
                                            {
                                                points += 2;
                                                cPos.X = 20; cPos.Y = 8; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                                                cout << ">>   "; SetConsoleTextAttribute(hConsole, 2); cout << "ON"; SetConsoleTextAttribute(hConsole, 15); cout << "   <<";
                                            }
                                            else
                                            {
                                                points -= 2;
                                                cPos.X = 20; cPos.Y = 8; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                                                cout << ">>   "; SetConsoleTextAttribute(hConsole, 4); cout << "OFF"; SetConsoleTextAttribute(hConsole, 15); cout << "  <<";
                                            }
                                        }
                                    }
                                    break;
                                }
                                // help info
                                if (S == "3")
                                {
                                    cPos.X = 5; cPos.Y = 10; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 1);
                                    cout << "Help info";
                                    cPos.X = 35; cPos.Y = 10; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 1);
                                    cout << "Shows help info in settings and while playing";
                                    while (true)
                                    {
                                        cPos.X = 0; cPos.Y = maxWindow.Y - 5; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                                        cin >> S; if (S == "0") { break; }
                                        if (S == "1")
                                        {
                                            if (Convertion(4, points) != 1)
                                            {
                                                points += 8;
                                                cPos.X = 20; cPos.Y = 10; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                                                cout << ">>   "; SetConsoleTextAttribute(hConsole, 2); cout << "ON"; SetConsoleTextAttribute(hConsole, 15); cout << "   <<";
                                            }
                                            else
                                            {
                                                points -= 8;
                                                cPos.X = 20; cPos.Y = 10; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                                                cout << ">>   "; SetConsoleTextAttribute(hConsole, 4); cout << "OFF"; SetConsoleTextAttribute(hConsole, 15); cout << "  <<";
                                            }
                                        }
                                    }
                                    break;
                                }
                                // Extra info
                                if (S == "4")
                                {
                                    cPos.X = 5; cPos.Y = 12; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 1);
                                    cout << "Extra info";
                                    cPos.X = 35; cPos.Y = 12; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 1);
                                    cout << "Reveals hidden information during gameplay";
                                    while (true)
                                    {
                                        cPos.X = 0; cPos.Y = maxWindow.Y - 5; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                                        cin >> S; if (S == "0") { break; }
                                        if (S == "1")
                                        {
                                            if (Convertion(7, points) != 1)
                                            {
                                                points += 64;
                                                cPos.X = 20; cPos.Y = 12; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                                                cout << ">>   "; SetConsoleTextAttribute(hConsole, 2); cout << "ON"; SetConsoleTextAttribute(hConsole, 15); cout << "   <<";
                                            }
                                            else
                                            {
                                                points -= 64;
                                                cPos.X = 20; cPos.Y = 12; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                                                cout << ">>   "; SetConsoleTextAttribute(hConsole, 4); cout << "OFF"; SetConsoleTextAttribute(hConsole, 15); cout << "  <<";
                                            }
                                        }
                                    }
                                    break;
                                }
                                // clear
                                if (S == "5")
                                {
                                    cPos.X = 5; cPos.Y = 14; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 1);
                                    cout << "Clear";
                                    while (true)
                                    {
                                        if (clear == 0)
                                        {
                                            cPos.X = 20; cPos.Y = 14; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                                            cout << ">>  "; SetConsoleTextAttribute(hConsole, 15); cout << "cls1"; SetConsoleTextAttribute(hConsole, 15); cout << "  <<";
                                            cPos.X = 35; cPos.Y = 14; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 1);
                                            cout << "Using system clear function                                             ";
                                        }
                                        if (clear == 1)
                                        {
                                            cPos.X = 20; cPos.Y = 14; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                                            cout << ">>  "; SetConsoleTextAttribute(hConsole, 15); cout << "cls2"; SetConsoleTextAttribute(hConsole, 15); cout << "  <<";
                                            cPos.X = 35; cPos.Y = 14; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 1);
                                            cout << "Using the FillConsoleOutputCharacter and FillConsoleOutputAttribute functions";
                                        }
                                        if (clear == 2)
                                        {
                                            cPos.X = 20; cPos.Y = 14; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                                            cout << ">>  "; SetConsoleTextAttribute(hConsole, 15); cout << "cls3"; SetConsoleTextAttribute(hConsole, 15); cout << "  <<";
                                            cPos.X = 35; cPos.Y = 14; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 1);
                                            cout << "Fill console with spaces                                              ";
                                        }
                                        if (clear == 3)
                                        {
                                            cPos.X = 20; cPos.Y = 14; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                                            cout << ">>  "; SetConsoleTextAttribute(hConsole, 15); cout << "cls4"; SetConsoleTextAttribute(hConsole, 15); cout << "  <<";
                                            cPos.X = 35; cPos.Y = 14; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 1);
                                            cout << "Scroll the screen contents and set the fill for the detected space                         ";
                                        }
                                        cPos.X = 0; cPos.Y = maxWindow.Y - 5; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                                        cin >> S; if (S == "0") { break; }
                                        if (S == "1")
                                        {
                                            clear = (clear + 1) % 4;
                                        }
                                    }
                                    break;
                                }
                                // language
                                if (S == "6")
                                {
                                    cPos.X = 5; cPos.Y = 16; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 1);
                                    cout << "Language";
                                    cPos.X = 35; cPos.Y = 16; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 1);
                                    cout << "Language in application";
                                    while (true)
                                    {
                                        cPos.X = 0; cPos.Y = maxWindow.Y - 5; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                                        cin >> S; if (S == "0") { break; }
                                        if (S == "1")
                                        {
                                            if (Convertion(6, points) != 1)
                                            {
                                                points += 32;
                                                cPos.X = 20; cPos.Y = 16; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                                                cout << ">>   "; SetConsoleTextAttribute(hConsole, 15); cout << "RU"; SetConsoleTextAttribute(hConsole, 15); cout << "   <<";
                                            }
                                            else
                                            {
                                                points -= 32;
                                                cPos.X = 20; cPos.Y = 16; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                                                cout << ">>   "; SetConsoleTextAttribute(hConsole, 15); cout << "EN"; SetConsoleTextAttribute(hConsole, 15); cout << "   <<";
                                            }
                                        }
                                    }
                                    break;
                                }
                            }
                        }
                    }
                    // Info
                    if (S == "4")
                    {

                    }
                } S = "-1";
            }
            // Level editor
            if (S == "2")
            {
                S = "-1";
                system("cls");
                while (true)
                {
                    if (Convertion(1, points) == 1)
                    {
                        SetConsoleTextAttribute(hConsole, 15);
                        cout << " > ";
                        SetConsoleTextAttribute(hConsole, 1);
                        cout << "Level editor\n";
                        SetConsoleTextAttribute(hConsole, 15);
                    }
                    if (Convertion(2, points) == 1) { SetConsoleTextAttribute(hConsole, 15); cout << "<- Go back\n"; SetConsoleTextAttribute(hConsole, 4); cout << "Enter 0\n"; }
                    SetConsoleTextAttribute(hConsole, 15);
                    // create
                    cPos.Y = 5; cPos.X = 0; SetConsoleCursorPosition(hConsole, cPos);
                    cout << " ##  ##    ##   #   ###   ##"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                    cout << "#    # #  #    # #   #   #  "; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                    cout << "#    ##    ##  ###   #    ##"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                    cout << "#    # #  #    # #   #   #  "; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                    cout << " ##  # #   ##  # #   #    ##"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                    SetConsoleTextAttribute(hConsole, 4);
                    cout << "Enter 1";
                    SetConsoleTextAttribute(hConsole, 15);
                    // load
                    cPos.Y = 11; cPos.X = 0; SetConsoleCursorPosition(hConsole, cPos);
                    cout << "#   #    #   ## "; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                    cout << "#  # #  # #  # #"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                    cout << "#  # #  ###  # #"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                    cout << "#  # #  # #  # #"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                    cout << "#   #   # #  ## "; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                    SetConsoleTextAttribute(hConsole, 4);
                    cout << "Enter 2";
                    SetConsoleTextAttribute(hConsole, 15);
                    cPos.Y = maxWindow.Y - 5; cPos.X = 0; SetConsoleCursorPosition(hConsole, cPos);
                    while (true)
                    {
                        cin >> S; if (S == "0") { break; } if (S == "1") { break; } if (S == "2") { break; }
                    }
                    if (S == "0") { S = "-1"; break; }
                    // editor
                    if (S == "1")
                    {
                        S = "-1"; unsigned int value = 0;
                        unsigned long long int Level_Leght, Level_Pos = 0;
                        short Up2, Down2, Right2, Left2;
                        float Percent = 0; unsigned short multi = 1;
                        unsigned short y2 = 0;
                        unsigned long long int x2 = 0;
                        system("cls");
                        // DisplayConfirmSaveAsMessageBox();
                        Up2 = Down2 = Right2 = Left2 = 15;
                        S = "-2"; Level_Leght = x;
                        while (true)
                        {
                            if (S == "*")
                            {
                                system("cls");
                                if (Convertion(1, points) == 1)
                                {
                                    SetConsoleTextAttribute(hConsole, 15);
                                    cout << " > ";
                                    SetConsoleTextAttribute(hConsole, 1);
                                    cout << "Level editor";
                                    SetConsoleTextAttribute(hConsole, 15);
                                    cout << " > ";
                                    SetConsoleTextAttribute(hConsole, 1);
                                    cout << "Create";
                                    SetConsoleTextAttribute(hConsole, 15);
                                    cout << " > ";
                                    SetConsoleTextAttribute(hConsole, 1);
                                    cout << "Help\n";
                                    SetConsoleTextAttribute(hConsole, 15);
                                }
                                if (Convertion(2, points) == 1) { SetConsoleTextAttribute(hConsole, 15); cout << "<- Go back\n"; SetConsoleTextAttribute(hConsole, 4); cout << "Enter 0\n"; }
                                SetConsoleTextAttribute(hConsole, 15);
                                while (true)
                                {
                                    cin >> S; if (S == "0") { S = "-1"; break; }
                                }
                            }
                            if (Convertion(1, points) == 1)
                            {
                                SetConsoleTextAttribute(hConsole, 15);
                                cout << " > ";
                                SetConsoleTextAttribute(hConsole, 1);
                                cout << "Level editor";
                                SetConsoleTextAttribute(hConsole, 15);
                                cout << " > ";
                                SetConsoleTextAttribute(hConsole, 1);
                                cout << "Create\n";
                                SetConsoleTextAttribute(hConsole, 15);
                            }
                            if (Convertion(2, points) == 1) { SetConsoleTextAttribute(hConsole, 15); cout << "<- Go back\n"; SetConsoleTextAttribute(hConsole, 4); cout << "Enter 0\n"; }
                            SetConsoleTextAttribute(hConsole, 15); Percent = (Level_Pos + 0.0) / (Level_Leght) * 100;
                            cPos.X = 0; cPos.Y = 4; SetConsoleCursorPosition(hConsole, cPos);
                            cout << Percent << " %      "; cPos.X = 10; SetConsoleCursorPosition(hConsole, cPos);  SetConsoleTextAttribute(hConsole, 7);
                            for (int i = 0; i < 10; i++) { SetConsoleTextAttribute(hConsole, 15); if (i + 1 <= (Percent / 10)) { SetConsoleTextAttribute(hConsole, 2); } cout << "*"; }
                            if ((Level_Leght / x) * x == Level_Leght || (Level_Leght / x) * x - 1 >= Level_Pos)
                            {
                                SetConsoleTextAttribute(hConsole, Right2); cPos.X = (Level_Leght % x) + 1; for (int i = 0; i < y + 1; i++) { cPos.Y = i + 6; SetConsoleCursorPosition(hConsole, cPos); cout << " "; }
                                SetConsoleTextAttribute(hConsole, Up2); cPos.X = 0; cPos.Y = 6; SetConsoleCursorPosition(hConsole, cPos); for (int i = 0; i < x + 2; i++) { cout << "#"; }
                                SetConsoleTextAttribute(hConsole, Down2); cPos.X = 0; cPos.Y = y + 7; SetConsoleCursorPosition(hConsole, cPos); for (int i = 0; i < x + 2; i++) { cout << "#"; }
                                SetConsoleTextAttribute(hConsole, Left2); cPos.X = 0; for (int i = 0; i < y + 1; i++) { cPos.Y = i + 6; SetConsoleCursorPosition(hConsole, cPos); cout << "#"; }
                                SetConsoleTextAttribute(hConsole, Right2); cPos.X = x + 1; for (int i = 0; i < y + 1; i++) { cPos.Y = i + 6; SetConsoleCursorPosition(hConsole, cPos); cout << "#"; }
                            }
                            else if ((Level_Leght / x) * x - 1 <= Level_Pos)
                            {
                                SetConsoleTextAttribute(hConsole, Up2); cPos.X = 0; cPos.Y = 6; SetConsoleCursorPosition(hConsole, cPos); for (int i = 0; i < x + 2; i++) { cout << " "; }
                                SetConsoleTextAttribute(hConsole, Down2); cPos.X = 0; cPos.Y = y + 7; SetConsoleCursorPosition(hConsole, cPos); for (int i = 0; i < x + 2; i++) { cout << " "; }
                                SetConsoleTextAttribute(hConsole, Left2); cPos.X = 0; for (int i = 0; i < y + 1; i++) { cPos.Y = i + 6; SetConsoleCursorPosition(hConsole, cPos); cout << " "; }
                                SetConsoleTextAttribute(hConsole, Right2); cPos.X = x + 1; for (int i = 0; i < y + 1; i++) { cPos.Y = i + 6; SetConsoleCursorPosition(hConsole, cPos); cout << " "; }
                                SetConsoleTextAttribute(hConsole, Up2); cPos.X = 0; cPos.Y = 6; SetConsoleCursorPosition(hConsole, cPos); for (int i = 0; i < 1 + (Level_Leght % x); i++) { cout << "#"; }
                                SetConsoleTextAttribute(hConsole, Down2); cPos.X = 0; cPos.Y = y + 7; SetConsoleCursorPosition(hConsole, cPos); for (int i = 0; i < 2 + (Level_Leght % x); i++) { cout << "#"; }
                                SetConsoleTextAttribute(hConsole, Left2); cPos.X = 0; for (int i = 0; i < y + 1; i++) { cPos.Y = i + 6; SetConsoleCursorPosition(hConsole, cPos); cout << "#"; }
                                SetConsoleTextAttribute(hConsole, Right2); cPos.X = (Level_Leght % x) + 1; for (int i = 0; i < y + 1; i++) { cPos.Y = i + 6; SetConsoleCursorPosition(hConsole, cPos); cout << "#"; }
                            }
                            //cPos.X = 150; cPos.Y = 20; SetConsoleCursorPosition(hConsole, cPos); cout << (Level_Leght / x) * x << "     " << Level_Pos << "    ";
                            cPos.X = 0; cPos.Y = y + 10; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15); for (int i = 0; i < 100; i++) { cout << "="; }
                            cPos.X = 0; cPos.Y += 2; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 7);
                            cout << "Map size:"; cPos.Y++; SetConsoleTextAttribute(hConsole, 15); SetConsoleCursorPosition(hConsole, cPos);
                            cout << "Length     Height"; cPos.Y++; SetConsoleTextAttribute(hConsole, 4); SetConsoleCursorPosition(hConsole, cPos);
                            cout << "Enter 1    Enter 2"; cPos.Y -= 2; cPos.X += 20; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 7);
                            cout << "Color:"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 4);
                            cout << "Enter 3"; cPos.Y--; cPos.X += 10; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 7);
                            cout << "Initial speed:"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 4);
                            cout << "Enter 4"; cPos.Y--; cPos.X += 18; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 7);
                            cout << "Gamemode:"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 4);
                            cout << "Enter 5"; cPos.Y--; cPos.X += 10; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 7);
                            cout << "Points system:"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 4);
                            cout << "Enter 6"; cPos.Y--; cPos.X += 20; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 7);
                            cout << "Obstacles:"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 4);
                            cout << "Enter 7"; cPos.Y += 3; cPos.X = 0; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 7);
                            cout << "Level length:"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                            cout << "length: " << Level_Leght << "                   "; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 4);
                            cout << "Enter 8"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                            cout << "Multiplier: " << multi << "                 "; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 4);
                            cout << "Enter 9";
                            for (int i = 0; i < 2; i++)
                            {
                                cPos.X = 1 + Level_Pos % (x);
                                cPos.Y = 6 + i * (y + 1); SetConsoleCursorPosition(hConsole, cPos);
                                SetConsoleTextAttribute(hConsole, 4); cout << "#"; cPos.X--;
                                SetConsoleCursorPosition(hConsole, cPos);
                                SetConsoleTextAttribute(hConsole, Up); cout << "#";
                            }
                            cPos.Y = maxWindow.Y - 3; cPos.X = 0; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                            if (S == "-2" || S == "-3")
                            {
                                while (true)
                                {
                                    if (_kbhit())
                                    {
                                        char str;
                                        str = _getch();
                                        switch (str)
                                        {
                                        case '1':
                                            S = "1"; break;
                                        case '2':
                                            S = "2"; break;
                                        case '*':
                                            S = "3"; break;
                                        case '7':
                                            S = "7"; break;
                                        case '8':
                                            S = "8"; break;
                                        case '9':
                                            S = "9"; break;
                                        case 75:
                                        case 'a':
                                        case 'A':
                                        case 'ф':
                                        case 'Ф':
                                            S = "-3";
                                            if (Level_Pos > multi) { Level_Pos -= multi; }
                                            else { Level_Pos = 0; }
                                            break;
                                        case 77:
                                        case 'd':
                                        case 'D':
                                        case 'в':
                                        case 'В':
                                            S = "-3";
                                            Level_Pos += multi; if (Level_Pos > Level_Leght) { Level_Pos = Level_Leght; }
                                            break;
                                        }
                                        if (S != "-1") { break; }
                                    }
                                }
                            }
                            cPos.Y = maxWindow.Y - 3; cPos.X = 0; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 1);
                            if (S == "1") { cout << "Length"; Up2 = Down2 = 1; }
                            if (S == "2") { cout << "Height"; Left2 = Right2 = 1; }
                            if (S == "1" || S == "2" || S == "8" || S == "9")
                            {
                                cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 4);
                                cout << "Enter value:";
                                cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                            }
                            if (amount != 0) { cout << value; }
                            if (S != "7")
                            {
                                while (true)
                                {
                                    if (S == "-3") { break; }
                                    if (_kbhit())
                                    {
                                        char str;
                                        str = _getch();
                                        switch (str)
                                        {
                                        case '0':
                                            if (amount < 6) { value *= 10; cout << "0"; amount++; }
                                            break;
                                        case '1':
                                            if (amount < 6) { value *= 10; value += 1; cout << "1"; amount++; }
                                            break;
                                        case '2':
                                            if (amount < 6) { value *= 10; value += 2; cout << "2"; amount++; }
                                            break;
                                        case '3':
                                            if (amount < 6) { value *= 10; value += 3; cout << "3"; amount++; }
                                            break;
                                        case '4':
                                            if (amount < 6) { value *= 10; value += 4; cout << "4"; amount++; }
                                            break;
                                        case '5':
                                            if (amount < 6) { value *= 10; value += 5; cout << "5"; amount++; }
                                            break;
                                        case '6':
                                            if (amount < 6) { value *= 10; value += 6; cout << "6"; amount++; }
                                            break;
                                        case '7':
                                            if (amount < 6) { value *= 10; value += 7; cout << "7"; amount++; }
                                            break;
                                        case '8':
                                            if (amount < 6) { value *= 10; value += 8; cout << "8"; amount++; }
                                            break;
                                        case '9':
                                            if (amount < 6) { value *= 10; value += 9; cout << "9"; amount++; }
                                            break;
                                        case 8: // backspace
                                            if (amount <= 6) { value /= 10; cPos.X = amount - 1; SetConsoleCursorPosition(hConsole, cPos); cout << " "; cPos.X--; SetConsoleCursorPosition(hConsole, cPos); }
                                            if (amount > 0) { amount--; }
                                            break;
                                        case 13: // enter
                                        case 27: // esc
                                            amount = 0; if (value > 1000000) { value = 999999; } S = "-2"; break;
                                        }
                                        if (value < 1000000 && amount < 6) { break; }
                                    }
                                }
                            }
                            if (S == "1")
                            {
                                SetConsoleTextAttribute(hConsole, Up2); cPos.X = 0; cPos.Y = 6; SetConsoleCursorPosition(hConsole, cPos); for (int i = 0; i < x + 2; i++) { cout << " "; }
                                SetConsoleTextAttribute(hConsole, Down2); cPos.X = 0; cPos.Y = y + 7; SetConsoleCursorPosition(hConsole, cPos); for (int i = 0; i < x + 2; i++) { cout << " "; }
                                SetConsoleTextAttribute(hConsole, Left2); cPos.X = 0; for (int i = 0; i < y + 1; i++) { cPos.Y = i + 6; SetConsoleCursorPosition(hConsole, cPos); cout << " "; }
                                SetConsoleTextAttribute(hConsole, Right2); cPos.X = x + 1; for (int i = 0; i < y + 1; i++) { cPos.Y = i + 6; SetConsoleCursorPosition(hConsole, cPos); cout << " "; }
                                x = value; if (x > maxWindow.X - 4) { x = maxWindow.X - 4; }
                                if (Level_Leght < x) { Level_Leght = x; }
                            }
                            if (S == "2")
                            {
                                SetConsoleTextAttribute(hConsole, Up2); cPos.X = 0; cPos.Y = 6; SetConsoleCursorPosition(hConsole, cPos); for (int i = 0; i < x + 2; i++) { cout << " "; }
                                SetConsoleTextAttribute(hConsole, Down2); cPos.X = 0; cPos.Y = y + 7; SetConsoleCursorPosition(hConsole, cPos); for (int i = 0; i < x + 2; i++) { cout << " "; }
                                SetConsoleTextAttribute(hConsole, Left2); cPos.X = 0; for (int i = 0; i < y + 1; i++) { cPos.Y = i + 6; SetConsoleCursorPosition(hConsole, cPos); cout << " "; }
                                SetConsoleTextAttribute(hConsole, Right2); cPos.X = x + 1; for (int i = 0; i < y + 1; i++) { cPos.Y = i + 6; SetConsoleCursorPosition(hConsole, cPos); cout << " "; }
                                cPos.X = 0; cPos.Y = y + 10; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                                for (int i = 0; i < 15; i++)
                                {
                                    for (int j = 0; j < 100; j++)
                                    {
                                        cout << " ";
                                    }
                                    cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                                }
                                y = value; if (y > maxWindow.Y - 25) { y = maxWindow.Y - 25; }
                            }
                            // obstacles
                            if (S == "7")
                            {
                                cPos.Y = 12 + y; cPos.X = 0; SetConsoleCursorPosition(hConsole, cPos);
                                for (int i = 0; i < 12; i++)
                                {
                                    for (int j = 0; j < 150; j++)
                                    {
                                        cout << " ";
                                    }
                                    cPos.Y++;
                                }
                                if ((Level_Leght / x) * x == Level_Leght || (Level_Leght / x) * x - 1 >= Level_Pos)
                                {
                                    SetConsoleTextAttribute(hConsole, Right2); cPos.X = (Level_Leght % x) + 1; for (int i = 0; i < y + 1; i++) { cPos.Y = i + 6; SetConsoleCursorPosition(hConsole, cPos); cout << " "; }
                                    SetConsoleTextAttribute(hConsole, Up2); cPos.X = 0; cPos.Y = 6; SetConsoleCursorPosition(hConsole, cPos); for (int i = 0; i < x + 2; i++) { cout << "#"; }
                                    SetConsoleTextAttribute(hConsole, Down2); cPos.X = 0; cPos.Y = y + 7; SetConsoleCursorPosition(hConsole, cPos); for (int i = 0; i < x + 2; i++) { cout << "#"; }
                                    SetConsoleTextAttribute(hConsole, Left2); cPos.X = 0; for (int i = 0; i < y + 1; i++) { cPos.Y = i + 6; SetConsoleCursorPosition(hConsole, cPos); cout << "#"; }
                                    SetConsoleTextAttribute(hConsole, Right2); cPos.X = x + 1; for (int i = 0; i < y + 1; i++) { cPos.Y = i + 6; SetConsoleCursorPosition(hConsole, cPos); cout << "#"; }
                                }
                                else if ((Level_Leght / x) * x - 1 <= Level_Pos)
                                {
                                    SetConsoleTextAttribute(hConsole, Up2); cPos.X = 0; cPos.Y = 6; SetConsoleCursorPosition(hConsole, cPos); for (int i = 0; i < x + 2; i++) { cout << " "; }
                                    SetConsoleTextAttribute(hConsole, Down2); cPos.X = 0; cPos.Y = y + 7; SetConsoleCursorPosition(hConsole, cPos); for (int i = 0; i < x + 2; i++) { cout << " "; }
                                    SetConsoleTextAttribute(hConsole, Left2); cPos.X = 0; for (int i = 0; i < y + 1; i++) { cPos.Y = i + 6; SetConsoleCursorPosition(hConsole, cPos); cout << " "; }
                                    SetConsoleTextAttribute(hConsole, Right2); cPos.X = x + 1; for (int i = 0; i < y + 1; i++) { cPos.Y = i + 6; SetConsoleCursorPosition(hConsole, cPos); cout << " "; }
                                    SetConsoleTextAttribute(hConsole, Up2); cPos.X = 0; cPos.Y = 6; SetConsoleCursorPosition(hConsole, cPos); for (int i = 0; i < 1 + (Level_Leght % x); i++) { cout << "#"; }
                                    SetConsoleTextAttribute(hConsole, Down2); cPos.X = 0; cPos.Y = y + 7; SetConsoleCursorPosition(hConsole, cPos); for (int i = 0; i < 2 + (Level_Leght % x); i++) { cout << "#"; }
                                    SetConsoleTextAttribute(hConsole, Left2); cPos.X = 0; for (int i = 0; i < y + 1; i++) { cPos.Y = i + 6; SetConsoleCursorPosition(hConsole, cPos); cout << "#"; }
                                    SetConsoleTextAttribute(hConsole, Right2); cPos.X = (Level_Leght % x) + 1; for (int i = 0; i < y + 1; i++) { cPos.Y = i + 6; SetConsoleCursorPosition(hConsole, cPos); cout << "#"; }
                                }
                                cPos.X = 0; cPos.Y = 4; SetConsoleCursorPosition(hConsole, cPos);  SetConsoleTextAttribute(hConsole, 15);
                                cout << Percent << " %      "; cPos.X = 10; SetConsoleCursorPosition(hConsole, cPos);  SetConsoleTextAttribute(hConsole, 7);
                                for (int i = 0; i < 10; i++) { SetConsoleTextAttribute(hConsole, 15); if (i + 1 <= (Percent / 10)) { SetConsoleTextAttribute(hConsole, 2); } cout << "*"; }
                                cPos.X = 0; cPos.Y = 12 + y; SetConsoleCursorPosition(hConsole, cPos);
                                cout << "Obstacles list:"; cPos.Y++; SetConsoleTextAttribute(hConsole, 15); SetConsoleCursorPosition(hConsole, cPos);
                                cout << "Enter 1"; cPos.Y -= 2; cPos.X += 20; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 7);
                                cout << "All obstacles"; cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 4);
                                cout << "Enter 2"; cPos.Y--; cPos.X += 10; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 7);
                                S = "-1"; cPos.X = 0; cPos.Y = maxWindow.Y - 3; SetConsoleCursorPosition(hConsole, cPos);
                                while (true)
                                {
                                    if (S == "1" || S == "2") { break; }
                                    if (_kbhit())
                                    {
                                        char str;
                                        str = _getch();
                                        switch (str)
                                        {
                                        case '1':
                                            S = "1"; break;
                                        case '2':
                                            S = "2"; break;
                                        }
                                    }
                                }
                                //
                                if (S == "1")
                                {
                                    if (ObsNum == 0)
                                    {
                                        cPos.Y -= 2; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                                        cout << "The list of obstacles is empty";
                                    }
                                    else
                                    {
                                        unsigned short a = 0;
                                        unsigned short c = 0; unsigned short b = 0;
                                        unsigned short k = 0; value = 0;
                                        while (true)
                                        {
                                            cPos.Y -= 4; SetConsoleCursorPosition(hConsole, cPos);
                                            ifstream in("Obstacles.txt");
                                            if (in.is_open())
                                            {
                                                b = 0;
                                                for (int i = 0; i < k; i++) { int s; in >> s; }
                                                for (int u = 0; u < maxWindow.X / 10; u++)
                                                {
                                                    in >> c; if (u + 1 + k > ObsNum) { S = "123"; }
                                                    for (int i = 0; i < 4; i++)
                                                    {
                                                        for (int j = 0; j < 4; j++)
                                                        {
                                                            OBS[i][j] = (c % 2);
                                                            c /= 2;
                                                        }
                                                    }
                                                    if (S == "123")
                                                    {
                                                        for (int i = 0; i < 4; i++)
                                                        {
                                                            for (int j = 0; j < 4; j++)
                                                            {
                                                                OBS[i][j] = 0;
                                                            }
                                                        }
                                                    }
                                                    cPos.Y = 20 + y; cPos.X = 10 * b;
                                                    SetConsoleCursorPosition(hConsole, cPos);
                                                    SetConsoleTextAttribute(hConsole, 4);
                                                    if (S != "123") { cout << u + 1 + k << ")\n"; }
                                                    for (int i = 0; i < 4; i++)
                                                    {
                                                        cPos.Y = 21 + y + i; cPos.X = 10 * b;
                                                        SetConsoleCursorPosition(hConsole, cPos);
                                                        SetConsoleTextAttribute(hConsole, 15);
                                                        if (u + k + 1 == a) { SetConsoleTextAttribute(hConsole, 1); }
                                                        for (int j = 0; j < 4; j++)
                                                        {
                                                            if (OBS[i][j] == 1) { cout << "$"; }
                                                            else { cout << " "; }
                                                        }
                                                    }
                                                    b++;
                                                }
                                            }
                                            in.close();
                                            if ((Level_Leght / x) * x == Level_Leght || (Level_Leght / x) * x - 1 >= Level_Pos)
                                            {
                                                SetConsoleTextAttribute(hConsole, Right2); cPos.X = (Level_Leght % x) + 1; for (int i = 0; i < y + 1; i++) { cPos.Y = i + 6; SetConsoleCursorPosition(hConsole, cPos); cout << " "; }
                                                SetConsoleTextAttribute(hConsole, Up2); cPos.X = 0; cPos.Y = 6; SetConsoleCursorPosition(hConsole, cPos); for (int i = 0; i < x + 2; i++) { cout << "#"; }
                                                SetConsoleTextAttribute(hConsole, Down2); cPos.X = 0; cPos.Y = y + 7; SetConsoleCursorPosition(hConsole, cPos); for (int i = 0; i < x + 2; i++) { cout << "#"; }
                                                SetConsoleTextAttribute(hConsole, Left2); cPos.X = 0; for (int i = 0; i < y + 1; i++) { cPos.Y = i + 6; SetConsoleCursorPosition(hConsole, cPos); cout << "#"; }
                                                SetConsoleTextAttribute(hConsole, Right2); cPos.X = x + 1; for (int i = 0; i < y + 1; i++) { cPos.Y = i + 6; SetConsoleCursorPosition(hConsole, cPos); cout << "#"; }
                                            }
                                            else if ((Level_Leght / x) * x - 1 <= Level_Pos)
                                            {
                                                SetConsoleTextAttribute(hConsole, Up2); cPos.X = 0; cPos.Y = 6; SetConsoleCursorPosition(hConsole, cPos); for (int i = 0; i < x + 2; i++) { cout << " "; }
                                                SetConsoleTextAttribute(hConsole, Down2); cPos.X = 0; cPos.Y = y + 7; SetConsoleCursorPosition(hConsole, cPos); for (int i = 0; i < x + 2; i++) { cout << " "; }
                                                SetConsoleTextAttribute(hConsole, Left2); cPos.X = 0; for (int i = 0; i < y + 1; i++) { cPos.Y = i + 6; SetConsoleCursorPosition(hConsole, cPos); cout << " "; }
                                                SetConsoleTextAttribute(hConsole, Right2); cPos.X = x + 1; for (int i = 0; i < y + 1; i++) { cPos.Y = i + 6; SetConsoleCursorPosition(hConsole, cPos); cout << " "; }
                                                SetConsoleTextAttribute(hConsole, Up2); cPos.X = 0; cPos.Y = 6; SetConsoleCursorPosition(hConsole, cPos); for (int i = 0; i < 1 + (Level_Leght % x); i++) { cout << "#"; }
                                                SetConsoleTextAttribute(hConsole, Down2); cPos.X = 0; cPos.Y = y + 7; SetConsoleCursorPosition(hConsole, cPos); for (int i = 0; i < 2 + (Level_Leght % x); i++) { cout << "#"; }
                                                SetConsoleTextAttribute(hConsole, Left2); cPos.X = 0; for (int i = 0; i < y + 1; i++) { cPos.Y = i + 6; SetConsoleCursorPosition(hConsole, cPos); cout << "#"; }
                                                SetConsoleTextAttribute(hConsole, Right2); cPos.X = (Level_Leght % x) + 1; for (int i = 0; i < y + 1; i++) { cPos.Y = i + 6; SetConsoleCursorPosition(hConsole, cPos); cout << "#"; }
                                            }
                                            b = 0;
                                            cPos.X = 0; cPos.Y = maxWindow.Y - 3; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                                            // for (int i = 0; i < 10; i++) { cout << " "; } cPos.X -= 10; SetConsoleCursorPosition(hConsole, cPos);
                                            if (amount != 0) { cout << value; }
                                            while (true)
                                            {
                                                if (S == "-3") { break; }
                                                if (_kbhit())
                                                {
                                                    char Str;
                                                    Str = _getch();
                                                    switch (Str)
                                                    {
                                                    case '0':
                                                        if (amount < 6) { value *= 10; cout << "0"; amount++; }
                                                        break;
                                                    case '1':
                                                        if (amount < 6) { value *= 10; value += 1; cout << "1"; amount++; }
                                                        break;
                                                    case '2':
                                                        if (amount < 6) { value *= 10; value += 2; cout << "2"; amount++; }
                                                        break;
                                                    case '3':
                                                        if (amount < 6) { value *= 10; value += 3; cout << "3"; amount++; }
                                                        break;
                                                    case '4':
                                                        if (amount < 6) { value *= 10; value += 4; cout << "4"; amount++; }
                                                        break;
                                                    case '5':
                                                        if (amount < 6) { value *= 10; value += 5; cout << "5"; amount++; }
                                                        break;
                                                    case '6':
                                                        if (amount < 6) { value *= 10; value += 6; cout << "6"; amount++; }
                                                        break;
                                                    case '7':
                                                        if (amount < 6) { value *= 10; value += 7; cout << "7"; amount++; }
                                                        break;
                                                    case '8':
                                                        if (amount < 6) { value *= 10; value += 8; cout << "8"; amount++; }
                                                        break;
                                                    case '9':
                                                        if (amount < 6) { value *= 10; value += 9; cout << "9"; amount++; }
                                                        break;
                                                    case 'a':
                                                    case 'A':
                                                    case 'ф':
                                                    case 'Ф':
                                                        S = "-3";
                                                        if (k > multi) { k -= multi; }
                                                        else { k = 0; }
                                                        break;
                                                    case 'd':
                                                    case 'D':
                                                    case 'в':
                                                    case 'В':
                                                        S = "-3";
                                                        k += multi; if (k >= ObsNum) { k = ObsNum - 1; }
                                                        break;
                                                    case 8: // backspace
                                                        if (amount <= 6) { value /= 10; cPos.X = amount - 1; SetConsoleCursorPosition(hConsole, cPos); cout << " "; cPos.X--; SetConsoleCursorPosition(hConsole, cPos); }
                                                        if (amount > 0) { amount--; }
                                                        break;
                                                    case 13: // enter
                                                    case 27: // esc
                                                        amount = 0; if (value > 1000000) { value = 999999; } S = "-2"; break;
                                                    }
                                                    if (value < 1000000 && amount < 6) { break; }
                                                }
                                            }
                                            a = value; if (a > ObsNum) { a = ObsNum; }
                                            if (k + maxWindow.X / 10 < a) { k = a - 1; }
                                            if (k > a && S != "-3")
                                            {
                                                if (a > 0) { k = a - 1; }
                                                else { k = a; }
                                            }
                                            cPos.Y = 20 + y; cPos.X = 0; SetConsoleCursorPosition(hConsole, cPos);
                                            for (int i = 0; i < maxWindow.X; i++) { cout << " "; }
                                            if (S == "-2")
                                            {
                                                ifstream in("Obstacles.txt");
                                                if (in.is_open())
                                                {
                                                    for (int i = 0; i < a; i++)
                                                    {
                                                        in >> c;
                                                    }
                                                }
                                                in.close();
                                                b = c;
                                                for (int i = 0; i < 4; i++)
                                                {
                                                    for (int j = 0; j < 4; j++)
                                                    {
                                                        OBS[i][j] = (c % 2);
                                                        c /= 2;
                                                    }
                                                }
                                                cPos.Y = 3 + y; cPos.X = 1 + (Level_Pos % (x - 3));
                                                SetConsoleCursorPosition(hConsole, cPos);
                                                SetConsoleTextAttribute(hConsole, 1);
                                                for (int i = 0; i < 4; i++)
                                                {
                                                    for (int j = 0; j < 4; j++)
                                                    {
                                                        if (OBS[i][j] == 1) { cout << "$"; }
                                                        else { cout << " "; }
                                                    }
                                                    cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                                                }
                                                S = "-1"; x2 = Level_Pos; y2 = y;
                                                while (true)
                                                {
                                                    // save obs
                                                    if (S == "0")
                                                    {
                                                        t = 1; obstacles = 0;
                                                        for (int i = 0; i < 4; i++)
                                                        {
                                                            for (int j = 0; j < 4; j++)
                                                            {
                                                                obstacles += (OBS[i][j] * t);
                                                                t *= 2;
                                                            }
                                                        }
                                                        t = 1;
                                                        ofstream out("-obstacles-.txt", ofstream::app);
                                                        if (out.is_open())
                                                        {
                                                            out << " " << obstacles << " " << x2 << " " << y2;
                                                        }
                                                        in.close(); S = "-1";
                                                        break;
                                                    }
                                                    // init
                                                    unsigned long long int x3 = x2;
                                                    unsigned short y3 = y2;
                                                    unsigned long long int x4 = 0;
                                                    unsigned short y4 = 0;
                                                    // input
                                                    while (true)
                                                    {
                                                        if (_kbhit())
                                                        {
                                                            char str;
                                                            str = _getch();
                                                            switch (str)
                                                            {
                                                            case 75:
                                                            case 'a':
                                                            case 'A':
                                                            case 'ф':
                                                            case 'Ф':
                                                                S = "-4";
                                                                if (Level_Pos >= multi) { Level_Pos -= multi; x2 = Level_Pos; }
                                                                else { k = 0; }
                                                                break;
                                                            case 77:
                                                            case 'd':
                                                            case 'D':
                                                            case 'в':
                                                            case 'В':
                                                                S = "-5";
                                                                Level_Pos += multi; if (Level_Pos >= Level_Leght - 4) { Level_Pos = Level_Leght - 4; } x2 = Level_Pos;
                                                                break;
                                                            case 72:
                                                            case 'w':
                                                            case 'W':
                                                            case 'ц':
                                                            case 'Ц':
                                                                S = "-6";
                                                                if (y2 > 4) { y2--; }
                                                                break;
                                                            case 80:
                                                            case 's':
                                                            case 'S':
                                                            case 'ы':
                                                            case 'Ы':
                                                                S = "-7";
                                                                if (y2 < y) { y2++; }
                                                                break;
                                                            case 13:
                                                            case 27:
                                                                S = "0";
                                                                break;
                                                            }
                                                        }
                                                        if (S == "0" || S == "-3" || S == "-4" || S == "-5" || S == "-6" || S == "-7") { break; }
                                                    }
                                                    if ((Level_Pos / (Level_Leght + 0.0)) > 0.9) { Level_Pos -= multi; x2 = Level_Pos; }
                                                    if (Level_Pos / (Level_Leght + 0.0) == 0.9)
                                                    {
                                                        cPos.Y = 7;
                                                        if ((Level_Leght / x) * x == Level_Leght)
                                                        {
                                                            cPos.X = x - 3;
                                                        }
                                                        else
                                                        {
                                                            cPos.X = Level_Leght - (Level_Leght / x) * x - 3;
                                                        }
                                                        SetConsoleCursorPosition(hConsole, cPos);
                                                        for (int i = 0; i < y; i++)
                                                        {
                                                            for (int j = 0; j < 4; j++)
                                                            {
                                                                cout << " ";
                                                            }
                                                            cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                                                        }
                                                    }
                                                    SetConsoleTextAttribute(hConsole, Up2); cPos.X = 0; cPos.Y = 6; SetConsoleCursorPosition(hConsole, cPos); for (int i = 0; i < x + 2; i++) { cout << " "; }
                                                    SetConsoleTextAttribute(hConsole, Down2); cPos.X = 0; cPos.Y = y + 7; SetConsoleCursorPosition(hConsole, cPos); for (int i = 0; i < x + 2; i++) { cout << " "; }
                                                    SetConsoleTextAttribute(hConsole, Left2); cPos.X = 0; for (int i = 0; i < y + 1; i++) { cPos.Y = i + 6; SetConsoleCursorPosition(hConsole, cPos); cout << " "; }
                                                    SetConsoleTextAttribute(hConsole, Right2); cPos.X = x + 1; for (int i = 0; i < y + 1; i++) { cPos.Y = i + 6; SetConsoleCursorPosition(hConsole, cPos); cout << " "; }
                                                    if ((Level_Leght / x) * x == Level_Leght || (Level_Leght / x) * x - 7 >= Level_Pos)
                                                    {
                                                        SetConsoleTextAttribute(hConsole, Right2); cPos.X = (Level_Leght % x) + 1; for (int i = 0; i < y + 1; i++) { cPos.Y = i + 6; SetConsoleCursorPosition(hConsole, cPos); cout << " "; }
                                                        SetConsoleTextAttribute(hConsole, Up2); cPos.X = 0; cPos.Y = 6; SetConsoleCursorPosition(hConsole, cPos); for (int i = 0; i < x + 2; i++) { cout << "#"; }
                                                        SetConsoleTextAttribute(hConsole, Down2); cPos.X = 0; cPos.Y = y + 7; SetConsoleCursorPosition(hConsole, cPos); for (int i = 0; i < x + 2; i++) { cout << "#"; }
                                                        SetConsoleTextAttribute(hConsole, Left2); cPos.X = 0; for (int i = 0; i < y + 1; i++) { cPos.Y = i + 6; SetConsoleCursorPosition(hConsole, cPos); cout << "#"; }
                                                        SetConsoleTextAttribute(hConsole, Right2); cPos.X = x + 1; for (int i = 0; i < y + 1; i++) { cPos.Y = i + 6; SetConsoleCursorPosition(hConsole, cPos); cout << "#"; }
                                                    }
                                                    else if ((Level_Leght / x) * x - 7 <= Level_Pos)
                                                    {
                                                        SetConsoleTextAttribute(hConsole, Up2); cPos.X = 0; cPos.Y = 6; SetConsoleCursorPosition(hConsole, cPos); for (int i = 0; i < 1 + (Level_Leght % x); i++) { cout << "#"; }
                                                        SetConsoleTextAttribute(hConsole, Down2); cPos.X = 0; cPos.Y = y + 7; SetConsoleCursorPosition(hConsole, cPos); for (int i = 0; i < 2 + (Level_Leght % x); i++) { cout << "#"; }
                                                        SetConsoleTextAttribute(hConsole, Left2); cPos.X = 0; for (int i = 0; i < y + 1; i++) { cPos.Y = i + 6; SetConsoleCursorPosition(hConsole, cPos); cout << "#"; }
                                                        SetConsoleTextAttribute(hConsole, Right2); cPos.X = (Level_Leght % x) + 1; for (int i = 0; i < y + 1; i++) { cPos.Y = i + 6; SetConsoleCursorPosition(hConsole, cPos); cout << "#"; }
                                                    }
                                                    // draw level with obs
                                                    if (S == "-4" || S == "-5" || S == "-6" || S == "-7")
                                                    {
                                                        // clear
                                                        ifstream i("-obstacles-.txt");
                                                        if (i.is_open())
                                                        {
                                                            while (!i.eof())
                                                            {
                                                                i >> obstacles >> x4 >> y4;
                                                                //cPos.X = 150; cPos.Y = 10; SetConsoleCursorPosition(hConsole, cPos); cout << x4 << "    " << y4 << "    ";
                                                                //cPos.X = 150; cPos.Y = 11; SetConsoleCursorPosition(hConsole, cPos); cout << 1 + (x4) % (x - 3) << "    " << 3 + y4 % (y + 3) << "    ";
                                                                cPos.Y = 3 + y4 % (y + 3); cPos.X = 1 + (x4) % (x - 3);
                                                                SetConsoleCursorPosition(hConsole, cPos);
                                                                SetConsoleTextAttribute(hConsole, 1);
                                                                for (int i = 0; i < 4; i++)
                                                                {
                                                                    for (int j = 0; j < 4; j++)
                                                                    {
                                                                        cout << " ";
                                                                    }
                                                                    cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                                                                }
                                                            }
                                                        }
                                                        i.close();
                                                        // draw obs
                                                        ifstream in("-obstacles-.txt");
                                                        if (in.is_open())
                                                        {
                                                            while (!in.eof())
                                                            {
                                                                in >> obstacles >> x4 >> y4;
                                                                if (((Level_Pos - (Level_Pos / (x - 4))) / (x - 4)) == ((x4 - (x4 / (x - 4))) / (x - 4)))
                                                                {
                                                                    for (int i = 0; i < 4; i++)
                                                                    {
                                                                        for (int j = 0; j < 4; j++)
                                                                        {
                                                                            OBS[i][j] = (obstacles % 2);
                                                                            obstacles /= 2;
                                                                        }
                                                                    }
                                                                    cPos.Y = 3 + y4 % (y + 3); cPos.X = 1 + (x4) % (x - 3);
                                                                    SetConsoleCursorPosition(hConsole, cPos);
                                                                    SetConsoleTextAttribute(hConsole, 1);
                                                                    for (int i = 0; i < 4; i++)
                                                                    {
                                                                        for (int j = 0; j < 4; j++)
                                                                        {
                                                                            if (OBS[i][j] == 1) { cout << "$"; }
                                                                            else { cout << " "; }
                                                                        }
                                                                        cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                                                                    }
                                                                }
                                                            }
                                                        }
                                                        in.close();
                                                        // move obs
                                                        cPos.X = 150; cPos.Y = 20; SetConsoleCursorPosition(hConsole, cPos); cout << Level_Pos << "    " << x4 << "    ";
                                                        cPos.X = 150; cPos.Y = 21; SetConsoleCursorPosition(hConsole, cPos); cout << (x4 + 3 * (Level_Pos + 0.0) / x) / (x + 0.0) << "             ";
                                                        cPos.X = 150; cPos.Y = 22; SetConsoleCursorPosition(hConsole, cPos); cout << (Level_Pos + 3 * (Level_Pos + 0.0) / x) / (x + 0.0) << "           ";
                                                        unsigned short t = b;
                                                        for (int i = 0; i < 4; i++)
                                                        {
                                                            for (int j = 0; j < 4; j++)
                                                            {
                                                                OBS[i][j] = (t % 2);
                                                                t /= 2;
                                                            }
                                                        }
                                                        SetConsoleTextAttribute(hConsole, 15); Percent = (Level_Pos + 0.0) / (Level_Leght) * 100 + 10;
                                                        cPos.X = 0; cPos.Y = 4; SetConsoleCursorPosition(hConsole, cPos);
                                                        cout << Percent << " %      "; cPos.X = 10; SetConsoleCursorPosition(hConsole, cPos);  SetConsoleTextAttribute(hConsole, 7);
                                                        for (int i = 0; i < 10; i++) { SetConsoleTextAttribute(hConsole, 15); if (i + 1 <= (Percent / 10)) { SetConsoleTextAttribute(hConsole, 2); } cout << "*"; }
                                                        cPos.Y = 3 + y3 % (y + 3); cPos.X = 1 + x3 % (x - 3);
                                                        SetConsoleCursorPosition(hConsole, cPos);
                                                        for (int i = 0; i < 4; i++)
                                                        {
                                                            for (int j = 0; j < 4; j++)
                                                            {
                                                                cout << " ";
                                                            }
                                                            cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                                                        }
                                                        cPos.Y = 3 + y2 % (y + 3); cPos.X = 1 + ((Level_Pos) % (x - 3));
                                                        SetConsoleCursorPosition(hConsole, cPos);
                                                        SetConsoleTextAttribute(hConsole, 1);
                                                        for (int i = 0; i < 4; i++)
                                                        {
                                                            for (int j = 0; j < 4; j++)
                                                            {
                                                                if (OBS[i][j] == 1) { cout << "$"; }
                                                                else { cout << " "; }
                                                            }
                                                            cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                                                        }
                                                        for (int i = 0; i < 2; i++)
                                                        {
                                                            cPos.X = 4 + Level_Pos % (x - 3);
                                                            cPos.Y = 6 + i * (y + 1); SetConsoleCursorPosition(hConsole, cPos);
                                                            SetConsoleTextAttribute(hConsole, 4); cout << "#"; if (S == "-5") { cPos.X--; } if (S == "-4") { cPos.X++; }
                                                            SetConsoleCursorPosition(hConsole, cPos);
                                                            SetConsoleTextAttribute(hConsole, Up); cout << "#";
                                                            if ((Level_Leght / x) * x == Level_Leght || (Level_Leght / x) * x - 7 >= Level_Pos)
                                                            {
                                                                cPos.X = i * (x + 1); cPos.Y = y2 + 4; SetConsoleCursorPosition(hConsole, cPos);
                                                                SetConsoleTextAttribute(hConsole, 4); cout << "#"; if (S == "-7") { cPos.Y--; } if (S == "-6") { cPos.Y++; }
                                                                SetConsoleCursorPosition(hConsole, cPos);
                                                                SetConsoleTextAttribute(hConsole, Left); cout << "#";
                                                            }
                                                            else if ((Level_Leght / x) * x - 7 <= Level_Pos)
                                                            {
                                                                cPos.X = i * (1 + Level_Leght - (Level_Leght / x) * x); cPos.Y = y2 + 4; SetConsoleCursorPosition(hConsole, cPos);
                                                                SetConsoleTextAttribute(hConsole, 4); cout << "#"; if (S == "-7") { cPos.Y--; } if (S == "-6") { cPos.Y++; }
                                                                SetConsoleCursorPosition(hConsole, cPos);
                                                                SetConsoleTextAttribute(hConsole, Left); cout << "#";
                                                            }
                                                        }
                                                    }
                                                    if (S != "0") { S = "-1"; }
                                                }
                                            }
                                            S = "-1";
                                        }
                                    }
                                }
                            }
                            if (S == "8")
                            {
                                if (value != 0) { Level_Leght = value; }
                            }
                            if (S == "9")
                            {
                                multi = value;
                            }
                            if (S == "-2")
                            {
                                Left2 = Right2 = Down2 = Up2 = 15;
                                cPos.Y = maxWindow.Y - 5; cPos.X = 0; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                                for (int i = 0; i < 8; i++)
                                {
                                    for (int j = 0; j < 20; j++)
                                    {
                                        cout << " ";
                                    }
                                    cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                                }
                                value = 0;
                            }
                            cPos.X = 0; cPos.Y = 0; SetConsoleCursorPosition(hConsole, cPos);
                        }
                    }
                    // load
                    if (S == "2")
                    {

                    }
                }
            }
        }
        if (S == "3") { break; }
        if (Convertion(3, points) == 1)
        {
            ofstream out;
            out.open("Saves.txt");
            if (out.is_open())
            {
                out << x << " ";
                out << y << "\n";
                out << points << "\n";
                out << Left << " " << Right << " " << Down << " " << Up << "\n";
                out << PlayerColor << "\n";
                out << ObstaclesColor << "\n";
                out << PointsIndicatorColor << "\n";
                out << speed << "\n";
                out << maxspeed << "\n";
                out << exponent << "\n";
                out << H_indent << "\n";
                out << V_indent << "\n";
            }
            out.close();
        }
        system("cls");
        size = (x - 10 - (x % 10)) / 5;
        short* obs = new short[size]; //obstacle
        for (int i = 0; i < size; i++)
        {
            obs[i] = 0;
        }
        srand(time(0));
        for (int a = 0; a < (size / 2); a++)
        {
            obs[2 * a] = rand() % (y - 1);
        }
        pos = y - 3;
        // Map Borders
        SetConsoleTextAttribute(hConsole, Up); cPos.X = H_indent; cPos.Y = 1 + V_indent; SetConsoleCursorPosition(hConsole, cPos); for (int i = 0; i < x + 2; i++) { cout << "#"; }
        SetConsoleTextAttribute(hConsole, Down); cPos.X = H_indent; cPos.Y = y + 2 + V_indent; SetConsoleCursorPosition(hConsole, cPos); for (int i = 0; i < x + 2; i++) { cout << "#"; }
        SetConsoleTextAttribute(hConsole, Left); cPos.X = H_indent; for (int i = 0; i < y + 1; i++) { cPos.Y = i + 1 + V_indent; SetConsoleCursorPosition(hConsole, cPos); cout << "#"; }
        SetConsoleTextAttribute(hConsole, Right); cPos.X = x + 1 + H_indent; for (int i = 0; i < y + 1; i++) { cPos.Y = i + 1 + V_indent; SetConsoleCursorPosition(hConsole, cPos); cout << "#"; }
        unsigned short* Obstacles = new unsigned short[size / 2];
        srand(time(NULL));
        if (ObsNum != 0)
        {
            for (int i = 0; i < (size / 2); i++)
            {
                for (int j = 0; j < ObsNum + 1; j++)
                {
                    short a = (rand() % ObsNum) + 1;
                    ifstream in("Obstacles.txt"); // open file .txt
                    if (in.is_open())
                    {
                        for (int b = 0; b < a; b++)
                        {
                            in >> obstacles;
                        }
                    }
                    in.close();
                    if ((size / 2) > i + j) { Obstacles[i + j] = obstacles; }
                    else { break; }
                }
                i += ObsNum;
            }
        }
        game_over = false; number = 0; ConsoleCursorVisible(false, 100);
        if (ObsNum > 0 && ObsNum < 1001)
        {
            for (int a = 0; a < (size / 2); a++)
            {
                obs[2 * a] = rand() % (y - 5);
            }
            if (Convertion(4, points) != 1)
            {
                points = 0;
                while (game_over == false)
                {
                    Input();
                    Logic();
                    DrawObs(obs, Obstacles, size);
                }
            }
            else
            {
                points = 0;
                while (game_over == false)
                {
                    Input();
                    Logic();
                    DrawObs(obs, Obstacles, size);
                    if (number <= 20)
                    {
                        cPos.X = 0; cPos.Y = y + 4 + V_indent; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                        if (pos == y - 3 && number == 0)
                        {
                            cout << "Use 'w', 's' or arrows to move UP and DOWN.";
                        }
                        else
                        {
                            cout << "Go around $ and score points.               ";
                            number++;
                        }
                    }
                    else
                    {
                        cPos.X = 0; cPos.Y = y + 4 + V_indent; SetConsoleCursorPosition(hConsole, cPos); for (int i = 0; i < 40; i++) { cout << " "; }
                    }
                }
            }
        }
        else
        {
            if (Convertion(4, points) != 1)
            {
                points = 0;
                while (game_over == false)
                {
                    Input();
                    Logic();
                    Draw(obs, size);
                }
            }
            else
            {
                points = 0;
                while (game_over == false)
                {
                    Input();
                    Logic();
                    Draw(obs, size);
                    if (number <= 20)
                    {
                        cPos.X = 0; cPos.Y = y + 4 + V_indent; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
                        if (pos == y - 3 && number == 0)
                        {
                            cout << "Use 'w', 's' or arrows to move UP and DOWN.";
                        }
                        else
                        {
                            cout << "Go around $ and score points.              ";
                            number++;
                        }
                    }
                    else
                    {
                        cPos.X = 0; cPos.Y = y + 4 + V_indent; SetConsoleCursorPosition(hConsole, cPos); for (int i = 0; i < 40; i++) { cout << " "; }
                    }
                }
            }
        }
        delete[] obs; delete[] Obstacles; ConsoleCursorVisible(true, 5);
        system("cls"); SetConsoleTextAttribute(hConsole, 15);
        cout << "   ###     #      #   #    #####        ##   #     #  #####  ### \n";
        cout << "  #   #   # #    # # # #  #            #  #  #     # #      #   #\n";
        cout << " #        # #    # # # #  #           #    # #     # #      #   #\n";
        cout << " #       #   #   #  #  #  #           #    # #     # #      #  # \n";
        cout << " #  ##   #   #   #     #  #####       #    #  #   #  #####  # #  \n";
        cout << " #    #  #####  #       # #           #    #  #   #  #      ##   \n";
        cout << " #    # #     # #       # #           #    #   # #   #      # #  \n";
        cout << "  #  #  #     # #       # #            #  #    # #   #      #  # \n";
        cout << "   ##   #     # #       #  #####        ##      #    #####  #   #\n";
        SetConsoleTextAttribute(hConsole, 1);
        for (int i = 0; i < 28; i++)
        {
            cout << " ";
        }
        cout << "Points: " << points - 1 << "\n";
        SetConsoleTextAttribute(hConsole, 15);
        cout << "Enter 0 to continue\n";
        cout << "Enter 1 to go back to menu\n";
        cout << "Enter 2 to exit\n";
        S = "-1";
        while (S != "2")
        {
            cin >> S;
            if (S == "0") { game_over = false; break; }
            if (S == "1") { break; }
        }
        if (S == "2")
        {
            break;
        }
        ifstream in("Saves.txt"); // open file .txt
        if (in.is_open())
        {
            in >> x >> y;
            in >> points;
        }
        in.close();     // close file MessageBox(NULL, L"Ошибка получения дескриптора", L"Ошибка", MB_OK || MB_ICONERROR);
    }
}