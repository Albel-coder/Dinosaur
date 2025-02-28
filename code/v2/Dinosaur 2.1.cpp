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
bool d = 0; // gaps in the map
short speed = 900;
string S = "-1";
short number;
short maxspeed = 1;
short exponent = 0;
short pos = 5;
short randtime = 0;
short value = 0;
short x = 40;
short y = 7;
short t = 0;
short H_indent = 0; // Horizontal indentation 
short V_indent = 0; // Vertical indentation

COORD cPos;
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
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 77
#define KEY_RIGHT 75
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
        case 'ö':
        case 'Ö':
        case KEY_UP:
            dir = UP;
            break;
        case 's':
        case 'S':
        case 'û':
        case 'Û':
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
        case 'ö':
        case 'Ö':
        case KEY_UP:
            if (t > 1)
            {
                t--;
            }
            break;
        case 's':
        case 'S':
        case 'û':
        case 'Û':
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
        case 'ö':
        case 'Ö':
        case KEY_UP:
            if (Obstacles_H > 0) { Obstacles_H--; }
            break;
        case 's':
        case 'S':
        case 'û':
        case 'Û':
        case KEY_DOWN:
            if (Obstacles_H < 3) { Obstacles_H++; }
            break;
        case 'a':
        case 'A':
        case 'ô':
        case 'Ô':
        case KEY_LEFT:
            if (Obstacles_L > 0) { Obstacles_L--; }
            break;
        case 'd':
        case 'D':
        case 'â':
        case 'Â':
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
            cPos.X = x - obs2[2 * a + 1] + 10 * a + 1 + H_indent; cPos.Y = obs2[2 * a] + 3 + V_indent; SetConsoleTextAttribute(hConsole, ObstaclesColor); SetConsoleCursorPosition(hConsole, cPos); cout << "$";
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
            cPos.X = x - obs2[2 * a + 1] + 10 * a + 1 + H_indent; cPos.Y = obs2[2 * a] + 3 + V_indent; SetConsoleCursorPosition(hConsole, cPos); cout << " ";
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
    cPos.X = 5 + H_indent; cPos.Y = pos + 3 + V_indent; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, PlayerColor); cout << "*"; cPos.X = 5 + H_indent; cPos.Y = pos + 4 + V_indent; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, PlayerColor); cout << "*";
    for (int i = 0; i < (n / 2); i++)
    {
        if (obs2[2 * i + 1] > 13 * i)
        {
            unsigned short k = Obstacles2[i];
            for (int a = 0; a < 4; a++)
            {
                for (int b = 0; b < 4; b++)
                {
                    if (k % 2 == 1) {
                        if (obs2[2 * i + 1] > 13 * i + b) {
                            cPos.X = x - obs2[2 * i + 1] + 13 * i + b + 1 + H_indent; cPos.Y = obs2[2 * i] + 2 + a + V_indent;
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
            }
        }
    }
    this_thread::sleep_for(chrono::milliseconds(1000 - speed + 1));
    for (int i = 0; i < (n / 2); i++)
    {
        if (obs2[2 * i + 1] >= 13 * i + 4)
        {
            for (int j = 0; j < 4; j++)
            {
                cPos.X = x - obs2[2 * i + 1] + 13 * i + 1 + H_indent; cPos.Y = obs2[2 * i] + 2 + j + V_indent; SetConsoleCursorPosition(hConsole, cPos); cout << "    ";
            }
        }
    }
    cPos.X = 5 + H_indent; cPos.Y = pos + 3 + V_indent; SetConsoleCursorPosition(hConsole, cPos); cout << " "; cPos.X = 5 + H_indent; cPos.Y = pos + 4 + V_indent; SetConsoleCursorPosition(hConsole, cPos); cout << " ";
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
    for (int i = 0; i < (n / 2); i++)
    {
        obs2[2 * i + 1]++;
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
int main()
{
    SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_FULLSCREEN_MODE, 0);
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
            ifstream in("Obstacles.txt");
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
            SetConsoleTextAttribute(hConsole, 15);
            //start
            for (int i = 0; i < 95; i++) { cout << " "; }
            cout << " ##  ###  #  ##  ###\n"; for (int i = 0; i < 95; i++) { cout << " "; }
            cout << "#     #  # # # #  #\n"; for (int i = 0; i < 95; i++) { cout << " "; }
            cout << " ##   #  ### ##   #\n"; for (int i = 0; i < 95; i++) { cout << " "; }
            cout << "   #  #  # # # #  #\n"; for (int i = 0; i < 95; i++) { cout << " "; }
            cout << " ##   #  # # # #  # \n"; for (int i = 0; i < 102; i++) { cout << " "; }
            SetConsoleTextAttribute(hConsole, 4);
            cout << "Enter 0\n";
            SetConsoleTextAttribute(hConsole, 15);
            //settings
            for (int i = 0; i < 90; i++) { cout << " "; }
            cout << " ##   ## ### ### # #  # ###  ## \n"; for (int i = 0; i < 90; i++) { cout << " "; }
            cout << "#    #    #   #    ## # #   #   \n"; for (int i = 0; i < 90; i++) { cout << " "; }
            cout << " ##   ##  #   #  # # ## # #  ## \n"; for (int i = 0; i < 90; i++) { cout << " "; }
            cout << "   # #    #   #  # #  # # #    #\n"; for (int i = 0; i < 90; i++) { cout << " "; }
            cout << " ##   ##  #   #  # #  # ###  ## \n"; for (int i = 0; i < 102; i++) { cout << " "; }
            SetConsoleTextAttribute(hConsole, 4);
            cout << "Enter 1" << endl;
            SetConsoleTextAttribute(hConsole, 15);
            //exit
            for (int i = 0; i < 95; i++) { cout << " "; }
            cout << "  ##  #   #  #  ###\n"; for (int i = 0; i < 95; i++) { cout << " "; }
            cout << " #     # #       # \n"; for (int i = 0; i < 95; i++) { cout << " "; }
            cout << "  ##    #    #   # \n"; for (int i = 0; i < 95; i++) { cout << " "; }
            cout << " #     # #   #   # \n"; for (int i = 0; i < 95; i++) { cout << " "; }
            cout << "  ##  #   #  #   # \n"; for (int i = 0; i < 102; i++) { cout << " "; }
            SetConsoleTextAttribute(hConsole, 4);
            cout << "Enter 2\n";
            SetConsoleTextAttribute(hConsole, 15);
            while (S != "0" && S != "1")
            {
                cin >> S; if (S == "2") { break; }
            }
            if (S == "0") { break; } if (S == "2") { break; }
            // Settings
            if (S == "1")
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
                        cout << "Settings\n";
                        SetConsoleTextAttribute(hConsole, 15);
                    }
                    if (Convertion(2, points) == 1) { SetConsoleTextAttribute(hConsole, 15); cout << "<- Go back\n"; SetConsoleTextAttribute(hConsole, 4); cout << "Enter 0\n"; }
                    SetConsoleTextAttribute(hConsole, 15);
                    //map size
                    cout << "\n # #   #  ##\n";
                    cout << "# # # # # # # \n";
                    cout << "# # # ### ##  \n";
                    cout << "#   # # # #   \n";
                    cout << "#   # # # #   \n";
                    SetConsoleTextAttribute(hConsole, 4);
                    cout << "Enter 1\n";
                    SetConsoleTextAttribute(hConsole, 15);
                    //color setting
                    cout << " ##   #   #   #   ## \n";
                    cout << "#    # #  #  # #  # #\n";
                    cout << "#    # #  #  # #  ## \n";
                    cout << "#    # #  #  # #  # #\n";
                    cout << " ##   #   #   #   # #\n";
                    SetConsoleTextAttribute(hConsole, 4);
                    cout << "Enter 2\n";
                    SetConsoleTextAttribute(hConsole, 15);
                    //speed settings
                    cout << " ##   ##    ##   ##  ## \n";
                    cout << "#     # #  #    #    # #\n";
                    cout << " ##   ##    ##   ##  # #\n";
                    cout << "   #  #    #    #    # #\n";
                    cout << " ##   #     ##   ##  ## \n";
                    SetConsoleTextAttribute(hConsole, 4);
                    cout << "Enter 3\n";
                    SetConsoleTextAttribute(hConsole, 15);
                    //settings up obstacles
                    cout << " #   ##    ##  ###   #    ##  #   ##   ## \n";
                    cout << "# #  # #  #     #   # #  #    #  #    #   \n";
                    cout << "# #  ##    ##   #   ###  #    #   ##   ## \n";
                    cout << "# #  # #     #  #   # #  #    #  #       #\n";
                    cout << " #   ##    ##   #   # #   ##  #   ##   ## \n";
                    SetConsoleTextAttribute(hConsole, 4);
                    cout << "Enter 4\n";
                    SetConsoleTextAttribute(hConsole, 15);
                    // Save
                    cout << " ##    #   # #   ##\n";
                    cout << "#     # #  # #  #  \n";
                    cout << " ##   ###  # #   ##\n";
                    cout << "   #  # #  # #  #  \n";
                    cout << " ##   # #   #    ##\n";
                    SetConsoleTextAttribute(hConsole, 4);
                    cout << "Enter 5\n";
                    SetConsoleTextAttribute(hConsole, 15);
                    // Other
                    cout << " #   ###  # #   ##  ## \n";
                    cout << "# #   #   # #  #    # #\n";
                    cout << "# #   #   ###   ##  ## \n";
                    cout << "# #   #   # #  #    # #\n";
                    cout << " #    #   # #   ##  # #\n";
                    SetConsoleTextAttribute(hConsole, 4);
                    cout << "Enter 6\n";
                    SetConsoleTextAttribute(hConsole, 15);
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
                    }
                    // Go back to menu
                    if (S == "0") { break; }
                    // Map
                    if (S == "1")
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
                                    if (Convertion(1, points) == 1)
                                    {
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << " > ";
                                        SetConsoleTextAttribute(hConsole, 1);
                                        cout << "Settings";
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
                                    ShowMap();
                                    this_thread::sleep_for(chrono::milliseconds(200));
                                    try
                                    {
                                        number = CheckInput("Length: \n");
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
                                        number = CheckInput("Height: \n");
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
                                    if (Convertion(1, points) == 1)
                                    {
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << " > ";
                                        SetConsoleTextAttribute(hConsole, 1);
                                        cout << "Settings";
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
                                    if (Left == 15) { SetConsoleTextAttribute(hConsole, 2); cout << "ON\n"; SetConsoleTextAttribute(hConsole, 4); }
                                    if (Left == 0) { SetConsoleTextAttribute(hConsole, 4); cout << "OFF\n"; }
                                    cout << "Enter 1\n";
                                    SetConsoleTextAttribute(hConsole, 15);
                                    cout << "Right border: ";
                                    if (Right == 15) { SetConsoleTextAttribute(hConsole, 2); cout << "ON\n"; SetConsoleTextAttribute(hConsole, 4); }
                                    if (Right == 0) { SetConsoleTextAttribute(hConsole, 4); cout << "OFF\n"; }
                                    cout << "Enter 2\n";
                                    SetConsoleTextAttribute(hConsole, 15);
                                    cout << "Down border: ";
                                    if (Down == 15) { SetConsoleTextAttribute(hConsole, 2); cout << "ON\n"; SetConsoleTextAttribute(hConsole, 4); }
                                    if (Down == 0) { SetConsoleTextAttribute(hConsole, 4); cout << "OFF\n"; }
                                    cout << "Enter 3\n";
                                    SetConsoleTextAttribute(hConsole, 15);
                                    cout << "Up border: ";
                                    if (Up == 15) { SetConsoleTextAttribute(hConsole, 2); cout << "ON\n"; SetConsoleTextAttribute(hConsole, 4); }
                                    if (Up == 0) { SetConsoleTextAttribute(hConsole, 4); cout << "OFF\n"; }
                                    cout << "Enter 4\n";
                                    SetConsoleTextAttribute(hConsole, 15);
                                    cout << "Map:\n";
                                    if (Up == 15) { for (int u = 0; u < 40; u++) { cout << "#"; } cout << "\n"; }
                                    else { cout << "\n"; }
                                    for (int i = 0; i < 7; i++)
                                    {
                                        if (Left == 15) { cout << "#"; }
                                        else { cout << " "; }
                                        for (int j = 0; j < 38; j++)
                                        {
                                            cout << " ";
                                            if (i == 5 || i == 6)
                                            {
                                                cout << "   *"; for (int r = 0; r < 32; r++) { cout << " "; } cout << "$"; break;
                                            }
                                        }
                                        if (Right == 15) { cout << "#"; }
                                        cout << "\n";
                                    }
                                    if (Down == 15) { for (int i = 0; i < 40; i++) { cout << "#"; } cout << "\n"; }
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
                                    if (Convertion(1, points) == 1)
                                    {
                                        SetConsoleTextAttribute(hConsole, 15);
                                        cout << " > ";
                                        SetConsoleTextAttribute(hConsole, 1);
                                        cout << "Settings";
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
                                    cin >> S; if (S == "0") { break; }
                                    // Horizontal
                                    if (S == "1")
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
                                            cout << "Enter map offset to the right:\n";
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
                                            if (Convertion(1, points) == 1)
                                            {
                                                SetConsoleTextAttribute(hConsole, 15);
                                                cout << " > ";
                                                SetConsoleTextAttribute(hConsole, 1);
                                                cout << "Settings";
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
                                            cout << "Enter map offset to the down:\n";
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
                    if (S == "2")
                    {
                        S = "-1"; short k = 0;
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
                                cout << "Color\n";
                                SetConsoleTextAttribute(hConsole, 15);
                            }
                            else { cout << "\n"; }
                            if (Convertion(1, points) == 1) { SetConsoleTextAttribute(hConsole, 15); cout << "<- Go back\n"; SetConsoleTextAttribute(hConsole, 4); cout << "Enter 0\n"; }
                            else { cout << "\n\n"; }
                            SetConsoleTextAttribute(hConsole, 15); if (S == "1" && Left != 0) { SetConsoleTextAttribute(hConsole, 1); } cout << "\nLeft map border"; SetConsoleTextAttribute(hConsole, 15);
                            if (k == 10) { for (int i = 0; i < 10; i++) { cout << " "; } SetConsoleTextAttribute(hConsole, 15); cout << "- "; SetConsoleTextAttribute(hConsole, 1); cout << "Blue"; SetConsoleTextAttribute(hConsole, 15); cout << " - 1"; }
                            SetConsoleTextAttribute(hConsole, 4); cout << "\nEnter 1";
                            if (k == 10) { for (int i = 0; i < 18; i++) { cout << " "; } SetConsoleTextAttribute(hConsole, 15); cout << "- "; SetConsoleTextAttribute(hConsole, 2); cout << "Green"; SetConsoleTextAttribute(hConsole, 15); cout << " - 2"; }
                            SetConsoleTextAttribute(hConsole, 15); if (S == "2" && Right != 0) { SetConsoleTextAttribute(hConsole, 1); } cout << "\nRight map border"; SetConsoleTextAttribute(hConsole, 15);
                            if (k == 10) { for (int i = 0; i < 9; i++) { cout << " "; } SetConsoleTextAttribute(hConsole, 15); cout << "- "; SetConsoleTextAttribute(hConsole, 3); cout << "Cyal"; SetConsoleTextAttribute(hConsole, 15); cout << " - 3"; }
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
                            if (k == 10) { for (int i = 0; i < 16; i++) { cout << " "; } SetConsoleTextAttribute(hConsole, 15); cout << "- "; SetConsoleTextAttribute(hConsole, 11); cout << "Lightcyal"; SetConsoleTextAttribute(hConsole, 15); cout << " - 11"; }
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
                    if (S == "3")
                    {
                        short k = 0;
                        while (S != "0")
                        {
                            S = "-1"; system("cls");
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
                    if (S == "4")
                    {
                        short t = 0; short k = 0;
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
                                                    cPos.Y = 15 + i + k; cPos.X = 10 * b;
                                                    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos);
                                                    SetConsoleTextAttribute(hConsole, 4);
                                                    cout << (u + 1) / 2 << ")";
                                                    SetConsoleTextAttribute(hConsole, 15);
                                                }
                                                cPos.Y = 16 + i + k; cPos.X = 10 * b;
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
                                        cout << "Obstacle list is empty";
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
                    // Saving
                    if (S == "5")
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
                        cout << "Saving...\n";
                        this_thread::sleep_for(chrono::milliseconds(1000));
                        S = "-3";
                    }
                    // Other
                    if (S == "6")
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
                            cout << "Display an errors: ";
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
                            cout << "Reset settings\n";
                            SetConsoleTextAttribute(hConsole, 4);
                            cout << "Enter 6\n";
                            SetConsoleTextAttribute(hConsole, 15);
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
                            if (S == "6")
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
                } S = "-1";
            }
        }
        if (S == "2") { break; }
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
        HANDLE hStdout;
        hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
        pos = y - 3;
        // Map Borders
        SetConsoleTextAttribute(hConsole, Up); cPos.X = H_indent; cPos.Y = 1 + V_indent; SetConsoleCursorPosition(hConsole, cPos); for (int i = 0; i < x + 2; i++) { cout << "#"; }
        SetConsoleTextAttribute(hConsole, Down); cPos.X = H_indent; cPos.Y = y + 2 + V_indent; SetConsoleCursorPosition(hConsole, cPos); for (int i = 0; i < x + 2; i++) { cout << "#"; }
        SetConsoleTextAttribute(hConsole, Left); cPos.X = H_indent; for (int i = 0; i < y + 1; i++) { cPos.Y = i + 1 + V_indent; SetConsoleCursorPosition(hConsole, cPos); cout << "#"; }
        SetConsoleTextAttribute(hConsole, Right); cPos.X = x + 1 + H_indent; for (int i = 0; i < y + 1; i++) { cPos.Y = i + 1 + V_indent; SetConsoleCursorPosition(hConsole, cPos); cout << "#"; }
        unsigned short* Obstacles = new unsigned short[size / 2];
        srand(time(0));
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
        in.close();     // close file
    }
}