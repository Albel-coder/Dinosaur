#include <ctime>
#include <iostream>
#include <thread>
#include <conio.h>
#include <string>
#include <Windows.h>
using namespace std;
bool game_over = false;
bool d = 0; // gaps in the map
int long points = 0;
short speed = 80;
string S = "-1";
short pos = 5;
short randtime = 0;
short x = 100;
short y = 10;
HANDLE hConsole;
enum eDirection { STOP, UP, DOWN };
eDirection dir;
void Input()
{
    if (_kbhit())
    {
        switch (_getch())
        {
        case 'w':
        case 'W':
            dir = UP;
            break;
        case 's':
        case 'S':
            dir = DOWN;
            break;
        }
    }
}
void Logic()
{
    switch (dir)
    {
    case UP:
        if (pos > 0)
        {
            pos--;
        }
        dir = STOP;
        break;
    case DOWN:
        if (pos < y - 2)
        {
            pos++;
        }
        dir = STOP;
        break;
    }
}
int main()
{
    SendMessage(GetConsoleWindow(), WM_SYSKEYDOWN, VK_RETURN, 0x20000000);
    short size = 6;
    while (S != "-2")
    {
        S = "-1";
        while (S != "0" && S != "1")
        {
            if (d == 1) { S = "0"; break; }
            system("cls"); game_over = false; points = 0; dir = STOP;
            hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole, 15);
            //start
            for (int i = 0; i < 95; i++) { cout << " "; }
            cout << " ##  ###  #  ##  ###\n";
            for (int i = 0; i < 95; i++) { cout << " "; }
            cout << "#     #  # # # #  #\n";
            for (int i = 0; i < 95; i++) { cout << " "; }
            cout << " ##   #  ### ##   #\n";
            for (int i = 0; i < 95; i++) { cout << " "; }
            cout << "   #  #  # # # #  #\n";
            for (int i = 0; i < 95; i++) { cout << " "; }
            cout << " ##   #  # # # #  # \n";
            for (int i = 0; i < 102; i++) { cout << " "; }
            SetConsoleTextAttribute(hConsole, 4);
            cout << "Enter 0\n";
            SetConsoleTextAttribute(hConsole, 15);
            //exit
            for (int i = 0; i < 95; i++) { cout << " "; }
            cout << "  ##  #   #  #  ###\n";
            for (int i = 0; i < 95; i++) { cout << " "; }
            cout << " #     # #       # \n";
            for (int i = 0; i < 95; i++) { cout << " "; }
            cout << "  ##    #    #   # \n";
            for (int i = 0; i < 95; i++) { cout << " "; }
            cout << " #     # #   #   # \n";
            for (int i = 0; i < 95; i++) { cout << " "; }
            cout << "  ##  #   #  #   # \n";
            for (int i = 0; i < 102; i++) { cout << " "; }
            SetConsoleTextAttribute(hConsole, 4);
            cout << "Enter 1\n";
            SetConsoleTextAttribute(hConsole, 15);
            while (S != "0" && S != "1")
            {
                cin >> S;
            }
        }
        if (S == "1")
        {
            break;
        }
        system("cls");
        if (S == "0")
        {
            size = (x - 10 - (x % 10)) / 5;
            int* obs = new int[size]; //obstacle
            for (int i = 0; i < size; i++)
            {
                obs[i] = 0;
            }
            for (int a = 0; a < (size / 2); a++)
            {
                obs[2 * a] = rand() % (y - 2);
            }
            SetConsoleTextAttribute(hConsole, 15);
            SetConsoleOutputCP(1251);
            while (game_over == false)
            {
                SetConsoleTextAttribute(hConsole, 1);
                cout << "Points: " << points << "\n";
                SetConsoleTextAttribute(hConsole, 15);
                for (int i = 0; i < x + 1; i++) { cout << "#"; } cout << "\n"; points++;
                for (int a = 0; a < (size / 2); a++)
                {
                    obs[2 * a + 1]++;
                }
                Input();
                Logic();
                for (int i = 0; i < y; i++)
                {
                    cout << "#";
                    for (int j = 0; j < x - 1; j++)
                    {
                        // player
                        if ((i == pos && j == 3) || (i == pos + 1 && j == 3))
                        {
                            cout << "*"; d++;
                        }
                        // move obstacles
                        for (int a = 0; a < (size / 2); a++)
                        {
                            if ((i == obs[2 * a] || i == obs[2 * a] + 1) && j == x - obs[2 * a + 1] + 10 * a)
                            {
                                cout << "$"; d++;
                                if ((j == 4 || j == 3) && (obs[2 * a] == pos + 1 || obs[2 * a] == pos || obs[2 * a] == pos - 1))
                                {
                                    game_over = true; break;
                                }
                            }
                        }
                        if (d == 0) { cout << " "; } d = 0;
                    }
                    cout << "#\n";
                }
                for (int i = 0; i < x + 1; i++) { cout << "#"; }
                //random
                for (int a = 0; a < (size / 2); a++)
                {
                    if (obs[2 * a + 1] == x + 10 * a + 2)
                    {
                        obs[2 * a + 1] = 0;
                        srand(time(0));
                        if ((randtime % ((5 + a) + obs[2 * a] % (5 + a))) == 0)
                        {
                            obs[2 * a] = rand() % (y - 1);
                            randtime = 0;
                        }
                    }
                }
                this_thread::sleep_for(chrono::milliseconds(speed)); printf("\x1b[H");
            }
            delete[] obs; d = 0;
        }
        system("cls");
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
        for (int i = 0; i < 28; i++) { cout << " "; }
        cout << "Points: " << points - 1 << "\n";
        SetConsoleTextAttribute(hConsole, 4);
        cout << "Enter 0 to continue\n";
        cout << "Enter 1 to exit\n";
        SetConsoleTextAttribute(hConsole, 15);
        while (true)
        {
            cin >> S;
            if (S == "0") { d++; game_over = false; points = 0; break; }
            if (S == "1") { break; }
        }
        if (S == "1")
        {
            break;
        }
    }
}