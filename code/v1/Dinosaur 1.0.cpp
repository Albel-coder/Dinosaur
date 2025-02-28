#include <ctime>
#include <iostream>
#include <stdlib.h>
#include <thread>
#include <stdio.h>
#include <conio.h>
#include <Windows.h>
using namespace std;
int points = 0;
int S = -1;
int p = 0;
int q = 0;
int a = 0;
int t = 0;
int F = 0;
int k = 0;
int pos = 5;
int rand_pos = 4;
unsigned char randtime = 0;
int x = 7;
int y = 40;
HANDLE hConsole;
enum eDirection { STOP, UP, DOWN };
eDirection dir;
void Setup()
{
    dir = STOP;
}
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
        if (pos < 5)
        {
            pos++;
        }
        dir = STOP;
        break;
    }
}
void Draw()
{
    system("cls");
    cout << "#";
    for (int i = 0; i < 30; i++)
    {
        cout << " ";
    }
    SetConsoleTextAttribute(hConsole, 1);
    cout << "Points: " << points << endl;
    SetConsoleTextAttribute(hConsole, 15);
    for (int i = 0; i < x; i++)
    {
        cout << "#";
        for (int j = 0; j < y; j++)
        {
            if ((i == pos && j == 3) || (i == pos + 1 && j == 3))
            {
                cout << "*";
            }
            else
            {
                cout << " ";
            }
            if ((i == rand_pos) && j == y - k - 2)
            {
                cout << "$";
                if (j == 3 && (rand_pos == pos + 1 || rand_pos == pos || rand_pos - 1 == pos))
                {
                    F++;
                }
            }
            if ((i == rand_pos + 1) && j == y - k - 2)
            {
                cout << "$";
                if (j == 3 && rand_pos + 1 == pos)
                {
                    F++;
                }
            }
            if ((i == t) && p > 12 && j == y - p + 11)
            {
                cout << "$";
                if (j == 3 && (t == pos + 1 || t == pos || t - 1 == pos))
                {
                    F++;
                }
            }
            if ((i == t + 1) && p > 12 && j == y - p + 11)
            {
                cout << "$";
                if (j == 3 && t + 1 == pos)
                {
                    F++;
                }
            }
            if ((i == a) && q > 22 && j == y - q + 21)
            {
                cout << "$";
                if (j == 3 && (a == pos || a + 1 == pos || a - 1 == pos))
                {
                    F++;
                }
            }
            if ((i == a + 1) && q > 22 && j == y - q + 21)
            {
                cout << "$";
                if (j == 3 && a + 1 == pos)
                {
                    F++;
                }
            }
        }
        cout << endl;
    }
    for (int j = 0; j < y; j++)
    {
        cout << "#";
    }
    k++;
    p++;
    q++;
    points++;
    if (k == y - 1)
    {
        k = 0;
        srand(time(0));
        if ((randtime) % ((6) + rand_pos % (6)) == 0)
        {
            rand_pos = rand() % (x - 2);
            randtime = 0;
        }
    }
    if (p == y + 11)
    {
        p = 0;
        srand(time(0));
        if ((randtime) % ((7) + t % (7)) == 0)
        {
            t = rand() % (x - 2);
            randtime = 0;
        }
    }
    if (q == y + 21)
    {
        q = 0;
        srand(time(0));
        if ((randtime) % ((8) + a % (8)) == 0)
        {
            a = rand() % (x - 2);
            randtime = 0;
        }
    }
    this_thread::sleep_for(chrono::milliseconds(150));
}
int main(int argc, const char* argv[])
{
    while (points != 1)
    {
        while (S != 1 && S != 0)
        {
            system("cls"); F = 0; points = 0;
            hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            for (int i = 0; i < 53; i++)
            {
                cout << " ";
            }
            cout << " ##  ###  #  ##  ###" << endl;
            for (int i = 0; i < 53; i++)
            {
                cout << " ";
            }
            cout << "#     #  # # # #  #" << endl;
            for (int i = 0; i < 53; i++)
            {
                cout << " ";
            }
            cout << " ##   #  ### ##   #" << endl;
            for (int i = 0; i < 53; i++)
            {
                cout << " ";
            }
            cout << "   #  #  # # # #  #" << endl;
            for (int i = 0; i < 53; i++)
            {
                cout << " ";
            }
            cout << " ##   #  # # # #  # " << endl;
            for (int i = 0; i < 60; i++)
            {
                cout << " ";
            }
            SetConsoleTextAttribute(hConsole, 4);
            cout << "Enter 1" << endl;
            SetConsoleTextAttribute(hConsole, 15);
            for (int i = 0; i < 47; i++)
            {
                cout << " ";
            }
            cout << "  ##   ##    ##   #   ##    ##\n";
            for (int i = 0; i < 48; i++)
            {
                cout << " ";
            }
            cout << "#    #     #    # #  # #  #  \n";
            for (int i = 0; i < 48; i++)
            {
                cout << " ";
            }
            cout << " ##   ##   #    ###  ##    ##\n";
            for (int i = 0; i < 48; i++)
            {
                cout << " ";
            }
            cout << "#       #  #    # #  #    #  \n";
            for (int i = 0; i < 48; i++)
            {
                cout << " ";
            }
            cout << " ##   ##    ##  # #  #     ##\n";
            for (int i = 0; i < 60; i++)
            {
                cout << " ";
            }
            SetConsoleTextAttribute(hConsole, 4);
            cout << "Enter 0" << endl;
            SetConsoleTextAttribute(hConsole, 15);
            while (!(cin >> S))
            {
                cin.clear();
                cin.ignore(cin.rdbuf()->in_avail());
                S = -1;
            }
        }
        if (S == 0)
        {
            break;
        }
        system("cls");
        while (F != 1)
        {
            Draw();
            Input();
            Logic();
        }
        system("cls");
        cout << "   ###     #      #   #    #####        ##   #     #  #####  ### " << endl;
        cout << "  #   #   # #    # # # #  #            #  #  #     # #      #   #" << endl;
        cout << " #        # #    # # # #  #           #    # #     # #      #   #" << endl;
        cout << " #       #   #   #  #  #  #           #    # #     # #      #  # " << endl;
        cout << " #  ##   #   #   #     #  #####       #    #  #   #  #####  # #  " << endl;
        cout << " #    #  #####  #       # #           #    #  #   #  #      ##   " << endl;
        cout << " #    # #     # #       # #           #    #   # #   #      # #  " << endl;
        cout << "  #  #  #     # #       # #            #  #    # #   #      #  # " << endl;
        cout << "   ##   #     # #       #  #####        ##      #    #####  #   #" << endl;
        SetConsoleTextAttribute(hConsole, 1);
        for (int i = 0; i < 28; i++)
        {
            cout << " ";
        }
        cout << "Points: " << points - 1 << endl;
        SetConsoleTextAttribute(hConsole, 15);
        cout << "Enter something to escape\n";
        cin >> points;
        break;
    }
}
