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
int t = 12;
int F = 0;
int k = 0;
int r = 22;
int m = 70;
int a = 0;
int rand_pos = 4;
unsigned char randtime = 0;
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
		}
	}
}
void Logic()
{
	switch (dir)
	{
	case UP:
		k = 7;
		dir = STOP;
		break;
	}
}
void Draw()
{
	this_thread::sleep_for(chrono::milliseconds(150));
	system("cls");
	cout << "#";
	for (int i = 0; i < 90; i++)
	{
		cout << " ";
	}
	SetConsoleTextAttribute(hConsole, 1);
	cout << "Points : " << points << "\n";
	SetConsoleTextAttribute(hConsole, 15);
	for (int i = 0; i < t; i++)
	{
		cout << "#\n";
	}
	for (int i = 0; i < 5; i++)
	{
		cout << "#";
		for (int j = 0; j < 12; j++)
		{
			cout << " ";
		}
		if (i == 0)
		{
			cout << " ******** \n";
		}
		if (i == 1)
		{
			cout << "** *******\n";
		}
		if (i == 2)
		{
			cout << "**********\n";
		}
		if (i == 3)
		{
			cout << "**** \n";
		}
		if (i == 4)
		{
			cout << "******* \n";
		}
	}
	//
	cout << "#*"; for (int i = 0; i < 10; i++) { cout << " "; } cout << "**** ";
	if (t == 0 || t == 6 || m < 0) { cout << "\n"; }
	if (m != 0 && t == 12)
	{
		for (int i = 0; i < m; i++) { cout << " "; } cout << "$\n";
	}
	//
	cout << "#*"; for (int i = 0; i < 8; i++) { cout << " "; } cout << "***** ";
	if (t == 0 || t == 6 || m < 0) { cout << "\n"; }
	if (m != 0 && t == 12)
	{
		for (int i = 0; i < m; i++) { cout << " "; } cout << "$$ $\n";
	}
	//
	cout << "#** ********* ";
	if (t == 0 || t == 6 || m < 0) { cout << "\n"; }
	if (m != 0 && t == 12)
	{
		for (int i = 0; i < m; i++) { cout << " "; } cout << "$$ $$\n";
	}
	//
	cout << "#*** ******** * ";
	if (t == 0 || t == 6 || m < 0) { cout << "\n"; }
	if (m != 0 && t == 12)
	{
		for (int i = 0; i < m; i++) { cout << " "; } cout << "$$ $$ $\n";
	}
	//
	cout << "# *************"; for (int i = 0; i < 8; i++) { cout << " "; }
	if (t == 6 || t == 0 || m < 0) { cout << "\n"; }
	if (m != 0 && t > 7)
	{
		for (int i = 0; i < m; i++) { cout << " "; } cout << "$$$$$$ $$\n";
	}
	//
	cout << "# ***********"; for (int i = 0; i < 9; i++) { cout << " "; }
	if (t == 6 || t == 0 || m < 0) { cout << "\n"; }
	if (m != 0 && t > 7)
	{
		for (int i = 0; i < m; i++) { cout << " "; } cout << " $$$$ $$\n";
	}
	//
	cout << "# *******"; for (int i = 0; i < 11; i++) { cout << " "; }
	if (t == 0 || m < 0) { cout << "\n"; }
	if (m != 0 && t > 7)
	{
		for (int i = 0; i < m; i++) { cout << " "; } cout << " $$$$$$\n";
	}
	if (m != 0 && t == 6)
	{
		for (int i = 0; i < m; i++) { cout << " "; } cout << "$\n";
	}
	//
	cout << "# *** **"; for (int i = 0; i < 11; i++) { cout << " "; }
	if (t == 0 || m < 0) { cout << "\n"; }
	if (m != 0 && t > 7)
	{
		for (int i = 0; i < m; i++) { cout << " "; } cout << " $$$\n";
	}
	if (t == 6 && m != 0)
	{
		for (int i = 0; i < m; i++) { cout << " "; } cout << "$$ $\n";
	}
	//
	if (p % 2 == 0 && q > 0)
	{
		cout << "# ** **"; for (int i = 0; i < 10; i++) { cout << " "; }
		if (t == 0 || m < 0) { cout << "\n"; }
		if (m != 0 && t > 7)
		{
			for (int i = 0; i < m; i++) { cout << " "; } cout << " $$\n";
		}
		if (m != 0 && t == 6)
		{
			for (int i = 0; i < m; i++) { cout << " "; } cout << "$$ $$\n";
		}
		//
		cout << "# *"; for (int i = 0; i < 16; i++) { cout << " "; }
		if (t == 0 || m < 0) { cout << "\n"; }
		if (m != 0 && t > 7)
		{
			for (int i = 0; i < m; i++) { cout << " "; } cout << " $$\n";
		}
		if (m != 0 && t == 6)
		{
			for (int i = 0; i < m; i++) { cout << " "; } cout << "$$ $$ $\n";
		}
		//
		cout << "# **"; for (int i = 0; i < 15; i++) { cout << " "; }
		if (t == 0 && m < 0) { cout << "\n"; }
		if (m != 0 && t > 7)
		{
			for (int i = 0; i < m; i++) { cout << " "; } cout << " $$\n";
		}
		if (m != 0 && t == 6)
		{
			for (int i = 0; i < m; i++) { cout << " "; } cout << "$$$$$$ $$\n";
		}
		if (t == 0) { cout << "\n"; }
	}
	if (p % 2 == 1)
	{
		cout << "# ** *"; for (int i = 0; i < 11; i++) { cout << " "; }
		if (t == 0 || m < 0) { cout << "\n"; }
		if (m != 0 && t > 7)
		{
			for (int i = 0; i < m; i++) { cout << " "; } cout << " $$\n";
		}
		if (t == 6 && m != 0)
		{
			for (int i = 0; i < m; i++) { cout << " "; } cout << "$$ $$\n";
		}
		//
		cout << "# ** *"; for (int i = 0; i < 11; i++) { cout << " "; }
		if (t == 0 || m < 0) { cout << "\n"; }
		if (m != 0 && t > 7)
		{
			for (int i = 0; i < m; i++) { cout << " "; } cout << " $$\n";
		}
		if (t == 6 && m != 0)
		{
			for (int i = 0; i < m; i++) { cout << " "; } cout << "$$ $$ $\n";
		}
		//
		cout << "# **"; for (int i = 0; i < 10; i++) { cout << " "; }
		if (t == 0 || m < 0) { cout << "\n"; }
		if (m != 0 && t > 7)
		{
			for (int i = 0; i < m; i++) { cout << " "; } cout << " $$\n";
		}
		if (t == 6 && m != 0)
		{
			for (int i = 0; i < m; i++) { cout << " "; } cout << "$$$$$$ $$\n";
		}
	}
	if (q == 0)
	{
		cout << "# ** *"; for (int i = 0; i < 11; i++) { cout << " "; }
		if (m != 0 && t > 7)
		{
			for (int i = 0; i < m; i++) { cout << " "; } cout << " $$\n";
		}
		//
		cout << "# * *"; for (int i = 0; i < 11; i++) { cout << " "; }
		if (t == 6 && m != 0)
		{
			for (int i = 0; i < m; i++) { cout << " "; } cout << "$$ $$\n";
		}
		if (t == 0 || m < 0) { cout << "\n"; }
		if (m != 0 && t > 7)
		{
			for (int i = 0; i < m; i++) { cout << " "; } cout << " $$\n";
		}
		//
		cout << "# ** **"; for (int i = 0; i < 10; i++) { cout << " "; }
		if (t == 6 && m != 0)
		{
			for (int i = 0; i < m; i++) { cout << " "; } cout << "$$ $$ $\n";
		}
		if (t == 0 || m < 0) { cout << "\n"; }
		if (m != 0 && t > 7)
		{
			for (int i = 0; i < m; i++) { cout << " "; } cout << " $$\n";
		}
		if (t == 6 && m != 0)
		{
			for (int i = 0; i < m; i++) { cout << " "; } cout << "$$$$$$ $$\n";
		}
		q++;
	}
	if (t == 0)
	{
		cout << "#\n";
		cout << "#";
		for (int j = 0; j < r; j++) { cout << " "; } for (int i = 0; i < m; i++) { cout << " "; } cout << "$\n";
		cout << "#";
		for (int j = 0; j < r; j++) { cout << " "; } for (int i = 0; i < m; i++) { cout << " "; } cout << "$$ $\n";
		cout << "#";
		for (int j = 0; j < r; j++) { cout << " "; } for (int i = 0; i < m; i++) { cout << " "; } cout << "$$ $$\n";
		cout << "#";
		for (int j = 0; j < r; j++) { cout << " "; } for (int i = 0; i < m; i++) { cout << " "; } cout << "$$ $$ $\n";
		cout << "#";
		for (int j = 0; j < r; j++) { cout << " "; } for (int i = 0; i < m; i++) { cout << " "; } cout << "$$$$$$ $$\n";
	}
	if (t == 6 || t == 0)
	{
		cout << "#";
		for (int j = 0; j < r; j++) { cout << " "; } for (int i = 0; i < m; i++) { cout << " "; } cout << " $$$$ $$\n";
		cout << "#";
		for (int j = 0; j < r; j++) { cout << " "; } for (int i = 0; i < m; i++) { cout << " "; } cout << " $$$$$$\n";
		cout << "#";
		for (int j = 0; j < r; j++) { cout << " "; } for (int i = 0; i < m; i++) { cout << " "; } cout << " $$$\n";
		for (int j = 0; j < 3; j++)
		{
			cout << "#";
			for (int j = 0; j < r; j++) { cout << " "; } for (int i = 0; i < m; i++) { cout << " "; } cout << " $$\n";
		}
	}
	for (int j = 0; j < 103; j++) { cout << "#"; }
	if (k > 5 && k < 8)
	{
		t -= 6;
	}
	if (k > 0 && k < 3)
	{
		t += 6;
	}
	if (k < 0)
	{
		p++;
	}
	k--;
	if (m < 1 && t > 5)
	{
		F++;
	}
	m -= 5;
	if (m < 1)
	{
		r -= 5;
	}
	if (m == -15)
	{
		m = 70;
		r = 22;
	}
	points++;
	randtime++;
}
int main(int argc, const char* argv[])
{
	while (points != 1)
	{
		while (S != 1 && S != 0)
		{
			points = 0; p = 0; q = 0; t = 12; k = 0; r = 22; m = 70; a = 0;
			rand_pos = 4;
			randtime = 0;
			system("cls"); F = 0;
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
		S = -1;
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
		for (int i = 0; i < 28; i++)
		{
			cout << " ";
		}
		SetConsoleTextAttribute(hConsole, 1);
		cout << "Points: " << points - 1 << endl;
		SetConsoleTextAttribute(hConsole, 15);
		points = 3;
		cout << "To escape enter 1" << endl << "Enter 2 to restart" << endl;
		while (points != 2)
		{
			while (!(cin >> points))
			{
				cin.clear();
				cin.ignore(cin.rdbuf()->in_avail());
			}
			if (points == 1 || points == 2) { break; }
		}
		if (points == 1) { break; }
	}
}