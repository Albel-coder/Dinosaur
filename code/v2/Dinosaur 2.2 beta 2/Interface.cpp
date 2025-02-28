#include "Interface.h"

void DrawMap(short ConsoleWidth, short ConsoleHeight, short UpBorderColor, short DownBorderColor, short LeftBorderColor, short RightBorderColor, short TopLeftCoord, short TopUpCoord, short X, short Y)
{
    cPos.X = TopLeftCoord; cPos.Y = TopUpCoord; SetConsoleCursorPosition(hConsole, cPos);
    SetConsoleTextAttribute(hConsole, 7);
    cout << "Current map:";
    cPos.Y++;
    SetConsoleCursorPosition(hConsole, cPos);
    SetConsoleTextAttribute(hConsole, UpBorderColor);
    for (int i = 0; i < X + 2; i++)
    {
        if (i < ConsoleWidth - TopLeftCoord)
            cout << "#";
    }
    cPos.Y = TopUpCoord + 2;
    SetConsoleCursorPosition(hConsole, cPos);
    SetConsoleTextAttribute(hConsole, LeftBorderColor);
    for (int i = 0; i < Y; i++)
    {
        if (i < ConsoleHeight - TopUpCoord)
        {
            cout << "#";
            cPos.Y++;
            SetConsoleCursorPosition(hConsole, cPos);
        }
    }
    SetConsoleTextAttribute(hConsole, DownBorderColor);
    for (int i = 0; i < X + 2; i++)
    {
        if (i < ConsoleWidth - TopLeftCoord)
            cout << "#";
    }
    if (X < ConsoleWidth - TopLeftCoord - 2)
    {
        cPos.X = X + TopLeftCoord + 1; cPos.Y = TopUpCoord + 2;
        SetConsoleCursorPosition(hConsole, cPos);
        SetConsoleTextAttribute(hConsole, RightBorderColor);
        for (int i = 0; i < Y; i++)
        {
            cout << "#";
            cPos.Y++;
            SetConsoleCursorPosition(hConsole, cPos);
        }
    }
}

void ShowValue(short ConsoleHeight)
{
    SetConsoleTextAttribute(hConsole, 15);
    cPos.X = 13; cPos.Y = ConsoleHeight - 2;
    SetConsoleCursorPosition(hConsole, cPos);
    ConS.ConsoleCursorVisible(true, 5);
    cout << Value << "       ";
    unsigned int tmp = Value;
    for (int i = 1; i < 10; i++)
    {
        tmp /= 10;
        if (tmp == 0)
        {
            cPos.X += i; SetConsoleCursorPosition(hConsole, cPos);
            break;
        }
    }
}

void KeysInterface(string str, short& ConsoleWidth, short& ConsoleHeight)
{
    if (SelectedFunction < 5 && SelectedFunction >= 0)
    {
        // ������� ����� ���������� �� �������
        cPos.X = 22; cPos.Y = 2 + SelectedFunction * 2;
        SetConsoleCursorPosition(hConsole, cPos);
        SetConsoleTextAttribute(hConsole, 1);
        Variables[12 + SelectedFunction - 1] = tmp;
        cout << tmp;
        cPos.X = 13; cPos.Y = ConsoleHeight - 2;
        SetConsoleCursorPosition(hConsole, cPos);
        SetConsoleTextAttribute(hConsole, 15);
        cout << tmp;
        return;
    }
    // ���� ������� �� ��������� ������, 
    // ������, �� ������ ����� �� ������� � ����� ������������ ��������������� ��������
    cPos.X = 22;
    cPos.Y = 2;
    SetConsoleTextAttribute(hConsole, 15);
    for (int i = 0; i < 4; i++)
    {
        cPos.Y += 2;
        SetConsoleCursorPosition(hConsole, cPos);
        cout << Variables[i + 12];
    }
    SetConsoleTextAttribute(hConsole, 15);
    cPos.X = 13; cPos.Y = ConsoleHeight - 2;
    SetConsoleCursorPosition(hConsole, cPos);
}

void NotifySaves(short ConsoleHeight)
{
    SetConsoleTextAttribute(hConsole, 15);
    cPos.X = 0; cPos.Y = ConsoleHeight - 3;
    SetConsoleCursorPosition(hConsole, cPos);
    cout << "Saving...";
    this_thread::sleep_for(chrono::milliseconds(100));
    SetConsoleTextAttribute(hConsole, 15);
    cPos.X = 0; cPos.Y = ConsoleHeight - 3;
    SetConsoleCursorPosition(hConsole, cPos);
    cout << "             ";
}

void ColorInterface(string str, short& ConsoleWidth, short& ConsoleHeight)
{
    if (SelectedFunction != 8)
    {
        if (Value > 99)
            Value /= 10;
        for (int i = 1; i <= 7; i++)
        {
            if (SelectedFunction == i)
            {
                if (Key != 1)
                    Variables[i + 2] = Value;
                if (Variables[i + 2] > 15 || Variables[i + 2] == 0)
                    Variables[i + 2] = 15;
            }
        }
        DrawMap(ConsoleWidth, ConsoleHeight, 
            Variables.GetUpMapBorder(), 
            Variables.GetDownMapBorder(), 
            Variables.GetLeftMapBorder(),
            Variables.GetRightMapBorder(),
            50, 3, 40, 7);
        cPos.X = 50; cPos.Y = 3; 
        SetConsoleCursorPosition(hConsole, cPos); 
        SetConsoleTextAttribute(hConsole, Variables.GetPointsColor());
        cout << "Points:               ";
        for (int i = 0; i < 2; i++)
        {
            cPos.Y = 10 + i; cPos.X = 54;
            SetConsoleCursorPosition(hConsole, cPos); 
            SetConsoleTextAttribute(hConsole, Variables.GetPlayerColor());
            cout << "*";
            cPos.Y = 10 + i; cPos.X = 88; 
            SetConsoleCursorPosition(hConsole, cPos); 
            SetConsoleTextAttribute(hConsole, Variables.GetObstaclesColor());
            cout << "$";
        }

        cPos.X = 30; cPos.Y = 3; 
        SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 7);
        cout << "Color list:";
        ifstream in("Tabs\\ColorList.txt");
        if (in.is_open())
        {
            for (int i = 1; i < 16; i++)
            {
                cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                SetConsoleTextAttribute(hConsole, 15);
                cout << "- ";
                getline(in, str); SetConsoleTextAttribute(hConsole, i);
                cout << str;
                cPos.X = 40; SetConsoleCursorPosition(hConsole, cPos); cPos.X = 30;
                SetConsoleTextAttribute(hConsole, 15);
                cout << " - " << i;
            }
        }
        in.close();
        ShowValue(ConsoleHeight);
    }
    if (SelectedFunction == 8 || Tab::Conversion(7, Points))
    {
        Variables.Save();
        NotifySaves(ConsoleHeight);
        tmp = ENTER;
    }
}

void SpeedInterface(string str, short& ConsoleWidth, short& ConsoleHeight)
{
    if (Value > 9999)
        Value /= 10;
    cPos.X = 20; cPos.Y = 1; 
    SetConsoleCursorPosition(hConsole, cPos);
    cout << "                             ";
    cPos.X = 20; cPos.Y = 4; 
    SetConsoleCursorPosition(hConsole, cPos);
    cout << "               ";
    if (Value == 0)
    {
        Key = '0';
        cPos.X = 20; cPos.Y = 1; 
        SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 7);
        cout << "- go one step back";
    }
    else if (Key != 1)
        Variables.GetSpeed() = Value;
    if (Variables.GetSpeed() > 1000)
        Variables.GetSpeed() = 1000;
    cPos.X = 4; cPos.Y = 7; 
    SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 7);
    cout << "Current speed value:";
    cPos.Y++; 
    SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
    cout << Variables.GetSpeed() << "     ";
    cPos.Y++; 
    SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 4);
    cout << "Enter number to select and change value";
    ShowValue(ConsoleHeight);
}

void ObstaclesEditorInterface(string str, short& ConsoleWidth, short& ConsoleHeight)
{        
    unsigned short obstacles = 0;
    unsigned short var = 0;
    
    switch (tmp)
    {
    case 'w':
    case 'W':
    case '�':
    case '�':
    case KEY_UP:
        if (Obstacles_H > 0) { Obstacles_H--; }
        break;
    case 's':
    case 'S':
    case '�':
    case '�':
    case KEY_DOWN:
        if (Obstacles_H < 3) { Obstacles_H++; }
        break;
    case 'a':
    case 'A':
    case '�':
    case '�':
    case KEY_LEFT:
        if (Obstacles_L > 0) { Obstacles_L--; }
        break;
    case 'd':
    case 'D':
    case '�':
    case '�':
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
        Key = '0';
        break;
    case '1':
        Key = '1';
        break;
    case '!':
        Key = '2';
        break;
    }

    ConS.ConsoleCursorVisible(false, 100);

    cPos.X = 0; cPos.Y = 2;
    SetConsoleCursorPosition(hConsole, cPos);
    SetConsoleTextAttribute(hConsole, 15);
    cout << "Number of obstacles in the list: "; 
    SetConsoleTextAttribute(hConsole, 1); 
    cout << ObsNum << "/1000\n"; 
    SetConsoleTextAttribute(hConsole, 15);
    cout << "Use 'w', 'a', 's', 'd' or arrows to select; enter '/' to print $ in the current cell; enter '*' to clear current cell; enter '1' to save obstacle; enter '!' to clear list of obstacles\n\n";
    cPos.X = 50; cPos.Y = 6;
    SetConsoleCursorPosition(hConsole, cPos);
    cout << "######\n";
    for (int i = 0; i < 4; i++)
    {
        cPos.X = 50; cPos.Y = i + 7;
        SetConsoleCursorPosition(hConsole, cPos);
        cout << "#";
        for (int j = 0; j < 4; j++)
        {
            if (Obstacles_H == i && Obstacles_L == j)
                SetConsoleTextAttribute(hConsole, 40);
            if (OBS[i][j] == 1)
                cout << "$";
            else
                cout << " ";
            SetConsoleTextAttribute(hConsole, 15);
        }
        cout << "#";
    }
    cPos.X = 50; cPos.Y = 11;
    SetConsoleCursorPosition(hConsole, cPos);
    cout << "######\n";
    if (tmp == '2')
    {
        ObsNum = 0;
        ofstream out;
        out.open("Obstacles.txt");
        out.close();
        Key = 1;
    }
    if (tmp == '1')
    {
        var = 1; 
        obstacles = 0;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                obstacles += (OBS[i][j] * var);
                var *= 2;
            }
        }
        var = 1;
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
            if (Key == '0')
            { 
                Key = 1;
                ObsNum == 1000; 
            }
            if (Key == '1')
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

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                OBS[i][j] = 0;
            }
        }
    }
}

void RandomGeneratorInterface(string str, short& ConsoleWidth, short& ConsoleHeight)
{
    
}

void ObstaclesListInterface(string str, short& ConsoleWidth, short& ConsoleHeight)
{
    
}

void HorizontalAlignmentInterface(string str, short& ConsoleWidth, short& ConsoleHeight)
{
    if (Value > 9999)
        Value /= 10;
    ConS.ConsoleCursorVisible(false, 100);
    DrawMap(ConsoleWidth, ConsoleHeight, 
        0, 0, 0, 0, 
        Variables.GetHorizontalAlignment(),
        3, 
        Variables.GetMapWidth(),
        Variables.GetMapHeight());
    cPos.X = Variables.GetHorizontalAlignment(); cPos.Y = 3; SetConsoleCursorPosition(hConsole, cPos);
    cout << "                  ";
    if (Key != 1 && Value > 0)
        Variables.GetHorizontalAlignment() = Value;
    if (Variables.GetHorizontalAlignment() > ConsoleWidth - Variables.GetHorizontalAlignment() - 3)
        Variables.GetHorizontalAlignment() = ConsoleWidth - Variables.GetHorizontalAlignment() - 3;
    DrawMap(ConsoleWidth, ConsoleHeight, 
        15, 15, 15, 15, 
        Variables.GetHorizontalAlignment(), 
        3, 
        Variables.GetMapWidth(),
        Variables.GetMapHeight());
    ShowValue(ConsoleHeight);
}

void VerticalAlignmentInterface(string str, short& ConsoleWidth, short& ConsoleHeight)
{
    if (Value > 9999)
        Value /= 10;
    ConS.ConsoleCursorVisible(false, 100);
    DrawMap(ConsoleWidth, ConsoleHeight, 
        0, 0, 0, 0, 30, 
        Variables.GetVerticalAlignment(),
        Variables.GetMapWidth(),
        Variables.GetMapHeight());
    cPos.X = 30; cPos.Y = Variables.GetVerticalAlignment(); SetConsoleCursorPosition(hConsole, cPos);
    cout << "                  ";
    if (Key != 1)
        Variables.GetVerticalAlignment() = Value;
    if (Variables.GetVerticalAlignment() > ConsoleHeight - Variables.GetVerticalAlignment() - 3)
        Variables.GetVerticalAlignment() = ConsoleHeight - Variables.GetVerticalAlignment() - 3;
    DrawMap(ConsoleWidth, ConsoleHeight, 
        15, 15, 15, 15, 30, 
        Variables.GetVerticalAlignment(), 
        Variables.GetMapWidth(),
        Variables.GetMapHeight());
    ShowValue(ConsoleHeight);
}

void MapSizeInterface(string str, short& ConsoleWidth, short& ConsoleHeight)
{
    if (SelectedFunction == 1 || SelectedFunction == 2)
    {
        if (Value > 9999)
            Value /= 10;
        ConS.ConsoleCursorVisible(false, 100);
        DrawMap(ConsoleWidth, ConsoleHeight, 
            0, 0, 0, 0, 
            20, 
            3, 
            Variables.GetMapWidth(),
            Variables.GetMapHeight());
        if (SelectedFunction == 1)
        {
            if (Key != 1)
                Variables.GetMapWidth() = Value;
            if (Variables.GetMapWidth() > ConsoleWidth - 23)
                Variables.GetMapWidth() = ConsoleWidth - 23;
            DrawMap(ConsoleWidth, ConsoleHeight, 
                1, 1, 15, 15, 
                20, 
                3, 
                Variables.GetMapWidth(),
                Variables.GetMapHeight());
        }
        if (SelectedFunction == 2)
        {
            if (Key != 1)
                Variables.GetMapHeight() = Value;
            if (Variables.GetMapHeight() > ConsoleHeight - 10)
                Variables.GetMapHeight() = ConsoleHeight - 10;
            DrawMap(ConsoleWidth, ConsoleHeight, 
                15, 15, 1, 1, 
                20, 
                3, 
                Variables.GetMapWidth(),
                Variables.GetMapHeight());
        }        
        ShowValue(ConsoleHeight);
        return;
    }
    if (SelectedFunction == 3)
    {
        Variables.Save();
        NotifySaves(ConsoleHeight);
        tmp = ENTER;
    }
    DrawMap(ConsoleWidth, ConsoleHeight, 
        15, 15, 15, 15, 
        20, 
        3, 
        Variables.GetMapWidth(),
        Variables.GetMapHeight());
}

void MapBorderInterface(string str, short& ConsoleWidth, short& ConsoleHeight)
{
    for (int i = 1; i <= 4; i++)
    {
        if (SelectedFunction == i)
        {
            if (Variables[i + 2] != 0)
                Variables[i + 2] = 0;
            else
                Variables[i + 2] = 15;
        }
    }
    if (SelectedFunction == 5)
    {
        Variables.Save();
        NotifySaves(ConsoleHeight);
        tmp = ENTER;
    }
    DrawMap(ConsoleWidth, ConsoleHeight, 
        Variables.GetUpMapBorder(),
        Variables.GetDownMapBorder(),
        Variables.GetLeftMapBorder(),
        Variables.GetRightMapBorder(),
        20, 3, 40, 7);
}

void QuickSetupInterface(string str, short& ConsoleWidth, short& ConsoleHeight)
{

}