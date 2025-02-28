#include "GameModes.h"

void FullDetailedDinosaur::Draw()
{
    Input();
    cPos.X = 190 - (X % 190); cPos.Y = 26;
    if (X % 190 < 188)
    {
        DrawCactus();
    }
    else
    {
        cPos.X = 4;
        for (int i = 0; i < 20; i++)
        {
            cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
            for (int j = 0; j < 20; j++)
            {
                cout << EmptyTile;
            }
        }
        if (dir == STOP)
        {
            cPos.X = 0; cPos.Y = 20; SetConsoleCursorPosition(hConsole, cPos);
            printf(Dino);
            cPos.X = 0; cPos.Y = 0; SetConsoleCursorPosition(hConsole, cPos);
        }
    }
    X += 2;
    if (dir == STOP && Points % 1 == 0)
    {
        DrawLegs();        
    }
    else if (dir != STOP)
    {
        Jump();
    }
    cPos.X = 0; cPos.Y = 42; SetConsoleCursorPosition(hConsole, cPos);
    char tmp = 219;
    for (int i = 0; i < 50; i++)
    {
        cout << tmp;
    }    
    OutputPoints();
    DayAndNightSwitch();
    Points++;
    this_thread::sleep_for(chrono::milliseconds(1));
    cPos.X = 0; cPos.Y = 0; SetConsoleCursorPosition(hConsole, cPos);
}

FullDetailedDinosaur::FullDetailedDinosaur(string Quality)
{
    dir = STOP; 
    Points = 0;
    Dino[width * height] = '\0';
    ifstream in("FullDetailedDinosaur\\dino.txt");
    if (in.is_open())
    {
        char tmp;
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                in >> tmp;
                if (tmp == '0')
                {
                    Dino[j + i * width] = ' ';
                    Dino[j + i * width + 1] = ' '; j++;
                }
                if (tmp == '1')
                {
                    Dino[j + i * width] = 219;
                    Dino[j + i * width + 1] = 219; j++;
                }
                if (tmp == '2')
                {
                    Dino[j + i * width] = '\n';
                }
            }
        }
    }
    in.close();
    cPos.X = 0; cPos.Y = 20; SetConsoleCursorPosition(hConsole, cPos); 
    printf(Dino);
    cPos.X = 0; cPos.Y = 42; SetConsoleCursorPosition(hConsole, cPos);
    short tmp = ConS.GetConsoleWidth();
    for (int i = 0; i < tmp; i++)
    {
        cout << Tile;
    }
    cPos.X = 0; cPos.Y = 0; SetConsoleCursorPosition(hConsole, cPos);
}

void FullDetailedDinosaur::OutputPoints()
{
    short Multi = 1;
    for (int x = 0; x < 5; x++)
    {
        cPos.Y = 3; cPos.X = ConS.GetConsoleWidth() - 20 - 10 * x; 
        SetConsoleCursorPosition(hConsole, cPos);
        ifstream in("FullDetailedDinosaur\\" + numbers[(Points / Multi) % 10] + ".txt");
        if (in.is_open())
        {
            bool tmp = 0; 
            char Tile;
            for (int i = 0; i < 8; i++)
            {
                for (int j = 0; j < 7; j++)
                {
                    in >> tmp;
                    if (tmp)
                        Tile = 219;
                    else
                        Tile = ' ';
                    printf("%c", Tile);
                }
                cPos.Y++; 
                SetConsoleCursorPosition(hConsole, cPos);
            }
        }
        in.close();
        Multi *= 10;
    }
}

void FullDetailedDinosaur::DrawLegs()
{
    if (Points % 2 == 0)
    {
        cPos.X = 10; cPos.Y = 38; SetConsoleCursorPosition(hConsole, cPos);
        ifstream in("FullDetailedDinosaur\\dinoleg1.txt");
        if (in.is_open())
        {
            bool tmp = 0; char tile;
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 7; j++)
                {
                    in >> tmp;
                    if (tmp)
                        tile = 219;
                    else
                        tile = ' ';
                    printf("%c", tile); printf("%c", tile);
                }
                cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
            }
        }
        in.close();
    }
    else
    {
        cPos.X = 10; cPos.Y = 38; SetConsoleCursorPosition(hConsole, cPos);
        ifstream in("FullDetailedDinosaur\\dinoleg2.txt");
        if (in.is_open())
        {
            bool tmp = 0; char tile;
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 7; j++)
                {
                    in >> tmp;
                    if (tmp)
                        tile = 219;
                    else
                        tile = ' ';
                    printf("%c", tile); printf("%c", tile);
                }
                cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
            }
        }
        in.close();
    }
}

void FullDetailedDinosaur::DrawCactus()
{
    short width; short height;
    srand(time(NULL));    
    if (X % 190 == 0)
    {
        if (X % 190 == 0)
        {
            Num = 0;
        }
        if (X % 380 == 0)
        {
            Num = 1;
        }
        if (X % 570 == 0)
        {
            Num = 2;
        }
        else
        {
            Num = 3;
        }
        switch (Num)
        {
        case 0:
            path = "0";
            break;
        case 1:
            path = "1";
            break;
        case 2:
            path = "2";
            break;
        case 3:
            path = "3";
            break;
        case 4:
            path = "4";
            break;
        case 5:
            path = "5";
            break;
        case 6:
            path = "6";
            break;
        case 7:
            path = "7";
            break;
        case 8:
            path = "8";
            break;
        case 9:
            path = "9";
            break;
        case 10:
            path = "10";
            break;
        }
    }    
    ifstream in("FullDetailedDinosaur\\C" + path + ".txt");
    if (in.is_open())
    {
        char tmp; char ch = ' ';
        in >> width >> height;
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                in >> tmp;
                if (tmp == '0')
                {
                    ch = ' ';
                    printf("%c", ch);
                    printf("%c", ch);
                }
                if (tmp == '1')
                {
                    ch = 219;
                    printf("%c", ch);
                    printf("%c", ch);
                }
                if (tmp == '2')
                {
                    cPos.Y++;
                    SetConsoleCursorPosition(hConsole, cPos);
                }
            }
        }
    }
    in.close();
}

void FullDetailedDinosaur::Jump()
{
    if (dir == DOWN)
    {
        if (Position >= 2 && Position <= 16)
        {
            cPos.X = 0; cPos.Y = 19 - Position; SetConsoleCursorPosition(hConsole, cPos);
            for (int i = 0; i < 4; i++)
            {
                cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                for (int j = 0; j < 40; j++)
                {
                    cout << EmptyTile;
                }
            }
        }
        if (Position > 0)
            Position -= 2;
        else
        {
            cPos.X = 0; cPos.Y = 38; SetConsoleCursorPosition(hConsole, cPos);
            dir = STOP;
        }
    }
    if (dir == UP && Position <= 16)
    {
        cPos.X = 0; cPos.Y = 20 - Position; SetConsoleCursorPosition(hConsole, cPos);
        printf(Dino);
        cPos.X = 0; cPos.Y = 0; SetConsoleCursorPosition(hConsole, cPos);
    }
    else if (dir == DOWN && Position < 16)
    {
        cPos.X = 0; cPos.Y = 20 - Position; SetConsoleCursorPosition(hConsole, cPos);
        printf(Dino);
        cPos.X = 0; cPos.Y = 0; SetConsoleCursorPosition(hConsole, cPos);
    }
    if (dir == UP)
    {
        if (Position <= 54)
            Position += 2;
        else
        {
            dir = DOWN; Position = 16;
        }
    }
}

void FullDetailedDinosaur::DayAndNightSwitch()
{
    if (Points % 300 > 155 && Points % 300 < 157)
    {
        system("color 7F");
        SetConsoleTextAttribute(hConsole, 112);
        if (dir == STOP)
        {
            cPos.X = 0; cPos.Y = 20; SetConsoleCursorPosition(hConsole, cPos);
            printf(Dino); 
            cPos.X = 0; cPos.Y = 0; SetConsoleCursorPosition(hConsole, cPos);
        }
        cPos.X = 0; cPos.Y = 38; SetConsoleCursorPosition(hConsole, cPos);
        for (int i = 0; i < 40; i++)
        {
            cout << EmptyTile;
        }
    }
    if (Points % 300 > 157 && Points % 300 < 159)
    {
        system("color 8F");
        SetConsoleTextAttribute(hConsole, 135);
        if (dir == STOP)
        {
            cPos.X = 0; cPos.Y = 20; SetConsoleCursorPosition(hConsole, cPos);
            printf(Dino); 
            cPos.X = 0; cPos.Y = 0; SetConsoleCursorPosition(hConsole, cPos);
        }
        cPos.X = 0; cPos.Y = 38; SetConsoleCursorPosition(hConsole, cPos);
        for (int i = 0; i < 40; i++)
        {
            cout << EmptyTile;
        }
    }
    if (Points % 300 == 160)
    {
        system("color 0F");
        SetConsoleTextAttribute(hConsole, 271);
        if (dir == STOP)
        {
            cPos.X = 0; cPos.Y = 20; SetConsoleCursorPosition(hConsole, cPos);
            printf(Dino);
            cPos.X = 0; cPos.Y = 0; SetConsoleCursorPosition(hConsole, cPos);
        }
        cPos.X = 0; cPos.Y = 38; SetConsoleCursorPosition(hConsole, cPos);
        for (int i = 0; i < 40; i++)
        {
            cout << EmptyTile;
        }
    }
    if (Points % 300 > 294 && Points % 300 < 296)
    {
        system("color 8F");
        SetConsoleTextAttribute(hConsole, 135);
        if (dir == STOP)
        {
            cPos.X = 0; cPos.Y = 20; SetConsoleCursorPosition(hConsole, cPos);
            printf(Dino); 
            cPos.X = 0; cPos.Y = 0; SetConsoleCursorPosition(hConsole, cPos);
        }
        cPos.X = 0; cPos.Y = 38; SetConsoleCursorPosition(hConsole, cPos);
        for (int i = 0; i < 40; i++)
        {
            cout << EmptyTile;
        }
    }
    if (Points % 300 > 296 && Points % 300 < 298)
    {
        system("color 7F");
        SetConsoleTextAttribute(hConsole, 112);
        if (dir == STOP)
        {
            cPos.X = 0; cPos.Y = 20; SetConsoleCursorPosition(hConsole, cPos);
            printf(Dino); 
            cPos.X = 0; cPos.Y = 0; SetConsoleCursorPosition(hConsole, cPos);
        }
        cPos.X = 0; cPos.Y = 38; SetConsoleCursorPosition(hConsole, cPos);
        for (int i = 0; i < 40; i++)
        {
            cout << EmptyTile;
        }
    }
    if (Points % 300 == 299)
    {
        system("color F0");
        SetConsoleTextAttribute(hConsole, 240);
        if (dir == STOP)
        {
            cPos.X = 0; cPos.Y = 20; SetConsoleCursorPosition(hConsole, cPos);
            printf(Dino); 
            cPos.X = 0; cPos.Y = 0; SetConsoleCursorPosition(hConsole, cPos);
        }
        cPos.X = 0; cPos.Y = 38; SetConsoleCursorPosition(hConsole, cPos);
        for (int i = 0; i < 40; i++)
        {
            cout << EmptyTile;
        }
    }
    cPos.X = 0; cPos.Y = 0; SetConsoleCursorPosition(hConsole, cPos);
}

void FullDetailedDinosaur::Input()
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
        case 72:
            dir = UP;
            break;
        }
    }
}

FullDetailedDinosaur::~FullDetailedDinosaur()
{
    delete[] Dino;
}