#include <iostream>
#include <Windows.h>
#include <thread>
#include <conio.h>
#include <fstream>
#include <string>
#include <locale.h>

using namespace std;

COORD cPos;
COORD Chars;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
enum eDirection { STOP, UP, DOWN };
eDirection dir;

unsigned long long int points = 0;
short x = 40;
short y = 7;
string S = "-1";
unsigned char Digit;
string Str = "-1";
int value = 0;
string T = "-1";
short Up = 15;
short Down = 15;
short Left = 15;
short Right = 15;
short HorizontalIndent = 0;
short VerticalIndent = 0;
short ObstaclesColor = 15;
short PlayerColor = 15;
short PointsColor = 1;
short Speed = 150;


string Digits[10] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };

class Tab
{    
    friend class Interface;

public:

    Tab(string str)
    {
        path = str;
    }

    void Entrance(void (*Select)(
        string path, 
        short ConsoleWidth, 
        short ConsoleHeight), 
        short ConsoleWidth, 
        short ConsoleHeight, 
        short MinConsoleWidth, 
        short MinConsoleHeight, 
        bool Clear, 
        bool ChangeValue)
    {
        // выбираем вкладку / функцию вкладки
        while (true) // значение '-13' будет соответствовать нажатой клавиши enter
        {
            if (Clear)
            {
                system("cls");
                Draw(ConsoleWidth, ConsoleHeight);                
            }     
            Select(path, ConsoleWidth, ConsoleHeight);
            if (Str == "-13")
                break;
            if (ConsoleWidth < MinConsoleWidth || ConsoleHeight < MinConsoleHeight) // проверяем, допустимого ли размера консоль
                CheckConsoleSize(ConsoleWidth, ConsoleHeight, MinConsoleWidth, MinConsoleHeight); // просим пользователя установить консоли размер побольше
            while (true)
            {
                while (true)
                {  
                    GetBufferChars(&Chars); 
                    if (Input() || ConsoleWidth != Chars.X || ConsoleHeight != Chars.Y)
                    {
                        if (ChangeValue)
                        {
                            if (Str != "-8") // если не backspace
                            {
                                value *= 10; unsigned char j = '0';
                                for (int i = 0; i < 10; i++, j++)
                                {
                                    if (j == Digit)
                                    {
                                        value += i;
                                        break;
                                    }
                                }
                            }
                            else
                            {
                                value /= 10;
                            }
                        }
                        break;
                    }
                }
                if (Str == "-13" || ConsoleWidth != Chars.X || ConsoleHeight != Chars.Y) // если нажали enter или изменили размер консоли
                    break;
                if ((S != Str || ChangeValue) && Str != "-13") // если мы что-то ввели кроме enter
                {
                    Select(path, ConsoleWidth, ConsoleHeight); // выбираем вкладку или вводим значения для переменной в какой-то вкладке
                    Str = S;
                }
                this_thread::sleep_for(chrono::milliseconds(20)); // замедляем текущий поток
            }           
            if (Str == "-13")
                break;
            if (!Clear && (ConsoleWidth != Chars.X || ConsoleHeight != Chars.Y)) // если во время ввода переменной у консоли изменили размер
                break;     
            ConsoleWidth = Chars.X;
            ConsoleHeight = Chars.Y;
        }
        Str = "-1"; value = 0;
        cPos.X = 13; cPos.Y = ConsoleHeight - 2; SetConsoleCursorPosition(hConsole, cPos);
        cout << " ";
    }  

    static void DrawMap(short ConsoleWidth, short ConsoleHeight, short RightBorderColor, short LeftBorderColor, short UpBorderColor, short DownBorderColor, short TopLeftCoord, short TopUpCoord, short X, short Y)
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

private:
    string path;

    void Draw(short ConsoleWidth, short ConsoleHeight)
    {
        // выводим элементы вкладки в консоль
        short Color = 15;
        ifstream in("Tabs\\" + path);
        if (in.is_open())
        {
            string str;
            if (Conversion(1, points)) // если пользователь включил демонстрацию пути функции
            {
                in >> str;
                cPos.X = 0; cPos.Y = 0; SetConsoleCursorPosition(hConsole, cPos);
                while (str != "&")
                {
                    in >> Color >> str;
                    SetConsoleTextAttribute(hConsole, Color);
                    cout << " " << str;
                }
            }
            getline(in, str);
            if (Conversion(2, points) && path != "-Menu.txt") // если пользователь включил демонстрацию go back
            {
                cPos.X = 0; cPos.Y = 1; SetConsoleCursorPosition(hConsole, cPos);
                SetConsoleTextAttribute(hConsole, 15);
                cout << " <- Go back\n";
                SetConsoleTextAttribute(hConsole, 4);
                cout << "Enter 0";
            }
            cPos.X = 3; cPos.Y = 4;
            while (!in.eof()) // вывод функции и подсказок по вводу
            {
                in >> Color;
                str = "";
                SetConsoleTextAttribute(hConsole, Color);
                getline(in, str);
                SetConsoleCursorPosition(hConsole, cPos);
                cout << str;
                cPos.Y++;
            }
        }
        in.close();
        // выводим в правом нижнем углу версию программы

        cPos.Y = ConsoleHeight - 1; cPos.X = ConsoleWidth - 13;
        SetConsoleCursorPosition(hConsole, cPos);
        SetConsoleTextAttribute(hConsole, 7);
        cout << "version: 2.2";

        // возвращаем стандартные значения курсора и цвета
        SetConsoleTextAttribute(hConsole, 15);
        cPos.X = 0; cPos.Y = ConsoleHeight - 2;
        SetConsoleCursorPosition(hConsole, cPos);
        cout << "Enter value: ";
    }

    bool Input()
    {
        if (_kbhit())
        {
            char tmp = _getch();
            switch (tmp)
            {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':              
                S = tmp;
                Digit = tmp;
                return true;
                break;
            case 13: // enter
            case 27: // esc
                Str = "-13";
                return true;
                break;
            case 8: // backspace
                Str = "-8";            
                return true;
                break;
            }
        }
    }

    CONSOLE_SCREEN_BUFFER_INFO csbInfo;
    void GetBufferChars(COORD* Chars)
    {
        GetConsoleScreenBufferInfo(hConsole, &csbInfo);
        Chars->X = csbInfo.srWindow.Right - csbInfo.srWindow.Left + 1;
        Chars->Y = csbInfo.srWindow.Bottom - csbInfo.srWindow.Top + 1;
    }

    void CheckConsoleSize(short ConsoleWidth, short ConsoleHeight, short MinConsoleWidth, short MinConsoleHeight)
    {
        // уведомляем пользователя о недостаточном размере консоли
        system("cls");
        cPos.X = 0; cPos.Y = 0; SetConsoleCursorPosition(hConsole, cPos);
        SetConsoleTextAttribute(hConsole, 7);
        cout << "Your current console size is less than acceptable! \n\nEnlarge your console window or set full screen to continue.";
        while (ConsoleWidth < MinConsoleWidth || ConsoleHeight < MinConsoleHeight) // ждем, пока пользователь установит нужный размер консоли
        {
            GetBufferChars(&Chars);
            this_thread::sleep_for(chrono::milliseconds(20));
            ConsoleWidth = Chars.X;
            ConsoleHeight = Chars.Y;
        }
    }   
          
    static void Save()
    {
        ofstream out("Saves.txt");
        if (out.is_open())
        {
            out << x << "\n";
            out << y << "\n";
            out << Right << "\n";
            out << Left << "\n";
            out << Up << "\n";
            out << Down << "\n";
            out << HorizontalIndent << "\n";
            out << VerticalIndent << "\n";
        }
        out.close();
    }

    static void NotifySaves(short ConsoleHeight)
    {
        SetConsoleTextAttribute(hConsole, 15);
        cPos.X = 0; cPos.Y = ConsoleHeight - 3;
        SetConsoleCursorPosition(hConsole, cPos);
        cout << "Saving...";
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    
    static int Conversion(short a, unsigned long long int b)
    {
        for (int i = 0; i < (a - 1); i++)
        {
            b /= 2;
        }
        return (b % 2);
    }
    /*
    Переменная очков хранит до запуска игры информацию о том, какие функции включены
    1) - отображать путь функции
    2) - отображать подсказку go back
    3) - использовать русский язык
    4) - использовать огромные значки
    5) - подсказки в меню
    6) - подсказки во время геймплея
    7) - автоматически сохранять все настройки

    например, если значение очков равно 83 (1010011 - в двоичной)
    поэтому у пользователя будут включены функции 1) 2) 5) 7)
    */  

};

class Interface
{

public:

    static void ClearAndShowInfo(string path, short ConsoleWidth, short ConsoleHeight)
    {
        // выводим информацию о функции    
        unsigned char i = '0'; string str;
        ifstream in("Tabs\\-Info" + path);
        if (in.is_open())
        {
            if (path != "-Menu.txt" && !Tab::Conversion(2, points))
            {
                in >> cPos.X >> cPos.Y;
                getline(in, str);
            }
            while (!in.eof())
            {
                in >> cPos.X >> cPos.Y;
                getline(in, str);
                if (i == Digit)
                {
                    SetConsoleCursorPosition(hConsole, cPos);
                    SetConsoleTextAttribute(hConsole, 7);
                    cout << str;
                    in >> cPos.X;
                    SetConsoleCursorPosition(hConsole, cPos);
                    SetConsoleTextAttribute(hConsole, 1);
                    getline(in, str);
                    cout << str;
                }
                else
                {
                    SetConsoleCursorPosition(hConsole, cPos);
                    SetConsoleTextAttribute(hConsole, 0);
                    cout << str;
                    in >> cPos.X;
                    SetConsoleCursorPosition(hConsole, cPos);
                    SetConsoleTextAttribute(hConsole, 15);
                    getline(in, str);
                    cout << str;
                }
                i++;
            }
        }
        in.close();
        cPos.X = 13; cPos.Y = ConsoleHeight - 2;
        SetConsoleCursorPosition(hConsole, cPos);
        SetConsoleTextAttribute(hConsole, 15);
        cout << Digit;
    }

    static void MapSize(string str, short ConsoleWidth, short ConsoleHeight)
    {
        if (T != "3")
        {
            if (value > 9999)
                value /= 10;
            ConsoleCursorVisible(false, 100);
            Tab::DrawMap(ConsoleWidth, ConsoleHeight, 0, 0, 0, 0, 20, 3, x, y);            
            if (T == "1")
            {
                if (S != "-1")
                    x = value;
                if (x > ConsoleWidth - 23)
                    x = ConsoleWidth - 23;
                Tab::DrawMap(ConsoleWidth, ConsoleHeight, 15, 15, 1, 1, 20, 3, x, y);
            }
            if (T == "2")
            {
                if (S != "-1")
                    y = value;
                if (y > ConsoleHeight - 10)
                    y = ConsoleHeight - 10;
                Tab::DrawMap(ConsoleWidth, ConsoleHeight, 1, 1, 15, 15, 20, 3, x, y);
            }
            ShowValue(ConsoleHeight);
        }        
        if (T == "3" || Tab::Conversion(7, points))
        {
            Tab::Save();
            Tab::NotifySaves(ConsoleHeight);
        }
    }

    static void MapBorder(string str, short ConsoleWidth, short ConsoleHeight)
    {
        if (S == "1")
        {
            if (Right != 0)
                Right = 0;
            else
                Right = 15;
        }
        if (S == "2")
        {
            if (Left != 0)
                Left = 0;
            else
                Left = 15;
        }
        if (S == "3")
        {
            if (Up != 0)
                Up = 0;
            else
                Up = 15;
        }
        if (S == "4")
        {
            if (Down != 0)
                Down = 0;
            else
                Down = 15;
        }
        if (S != "5")
            Tab::DrawMap(ConsoleWidth, ConsoleHeight, Right, Left, Up, Down, 20, 3, 40, 7);
        if (S == "5" || Tab::Conversion(7, points))
            Tab::Save();
    }

    static void PreviewMap(string str, short ConsoleWidth, short ConsoleHeight)
    {
        Tab::DrawMap(ConsoleWidth, ConsoleHeight, 15, 15, 15, 15, 20, 3, x, y);
    }

    static void AlignmentSave(short ConsoleHeight)
    {
        Tab::Save();
        Tab::NotifySaves(ConsoleHeight);
    }

    static void Horizontal(string str, short ConsoleWidth, short ConsoleHeight)
    {
        if (value > 9999)
            value /= 10;
        ConsoleCursorVisible(false, 100);
        Tab::DrawMap(ConsoleWidth, ConsoleHeight, 0, 0, 0, 0, HorizontalIndent, 3, x, y);
        cPos.X = HorizontalIndent; cPos.Y = 3; SetConsoleCursorPosition(hConsole, cPos);
        cout << "                  ";        
        if (S != "-1")
            HorizontalIndent = value;
        if (HorizontalIndent > ConsoleWidth - x - 3)
            HorizontalIndent = ConsoleWidth - x - 3;
        Tab::DrawMap(ConsoleWidth, ConsoleHeight, 15, 15, 15, 15, HorizontalIndent, 3, x, y);
        ShowValue(ConsoleHeight);
    }

    static void Vertical(string str, short ConsoleWidth, short ConsoleHeight)
    {
        if (value > 9999)
            value /= 10;
        ConsoleCursorVisible(false, 100);
        Tab::DrawMap(ConsoleWidth, ConsoleHeight, 0, 0, 0, 0, 30, VerticalIndent, x, y);
        cPos.X = 30; cPos.Y = VerticalIndent; SetConsoleCursorPosition(hConsole, cPos);
        cout << "                  ";        
        if (S != "-1")
            VerticalIndent = value;
        if (VerticalIndent > ConsoleHeight - y - 3)
            VerticalIndent = ConsoleHeight - y - 3;
        Tab::DrawMap(ConsoleWidth, ConsoleHeight, 15, 15, 15, 15, 30, VerticalIndent, x, y);
        ShowValue(ConsoleHeight);
    }

    static void ColoredMap(string str, short ConsoleWidth, short ConsoleHeight)
    {
        if (T != "8")
        {
            if (value > 99)
                value /= 10;            
            if (T == "1")
            {
                if (S != "-1")
                    Right = value;
                if (Right > 15 || Right == 0)
                    Right = 15;
            }
            if (T == "2")
            {
                if (S != "-1")
                    Left = value;
                if (Left > 15 || Left == 0)
                    Left = 15;
            }
            if (T == "3")
            {
                if (S != "-1")
                    Up = value;
                if (Up > 15 || Up == 0)
                    Up = 15;
            }
            if (T == "4")
            {
                if (S != "-1")
                    Down = value;
                if (Down > 15 || Down == 0)
                    Down = 15;
            }
            if (T == "5")
            {
                if (S != "-1")
                    ObstaclesColor = value;
                if (ObstaclesColor > 15 || ObstaclesColor == 0)
                    ObstaclesColor = 15;
            }
            if (T == "6")
            {
                if (S != "-1")
                    PlayerColor = value;
                if (PlayerColor > 15 || PlayerColor == 0)
                    PlayerColor = 15;
            }
            if (T == "7")
            {
                if (S != "-1")
                    PointsColor = value;
                if (PointsColor > 15 || PointsColor == 0)
                    PointsColor = 15;
            }
            Tab::DrawMap(ConsoleWidth, ConsoleHeight, Right, Left, Up, Down, 50, 3, 40, 7);
            cPos.X = 50; cPos.Y = 3; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, PointsColor);
            cout << "Points:               ";
            for (int i = 0; i < 2; i++)
            {
                cPos.Y = 10 + i; cPos.X = 54; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, PlayerColor);
                cout << "*";
                cPos.Y = 10 + i; cPos.X = 88; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, ObstaclesColor);
                cout << "$";
            }

            cPos.X = 30; cPos.Y = 3; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 7);
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
        if (T == "8" || Tab::Conversion(7, points))
        {
            Tab::Save();
            Tab::NotifySaves(ConsoleHeight);
        }
    }

    static void ChangeSpeed(string str, short ConsoleWidth, short ConsoleHeight)
    {
        if (value > 9999)
            value /= 10;
        cPos.X = 20; cPos.Y = 1; SetConsoleCursorPosition(hConsole, cPos);
        cout << "                             ";
        cPos.X = 20; cPos.Y = 4; SetConsoleCursorPosition(hConsole, cPos);
        cout << "               ";
        if (value == 0)
        {
            S = "0";
            cPos.X = 20; cPos.Y = 1; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 7);
            cout << "- go one step back";
        }
        else if (S != "-1")
            Speed = value;
        if (Speed > 1000)
            Speed = 1000;
        cPos.X = 4; cPos.Y = 7; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 7);
        cout << "Current speed value:";
        cPos.Y++; SetConsoleCursorPosition(hConsole, cPos); SetConsoleTextAttribute(hConsole, 15);
        cout << Speed << "     ";
        ShowValue(ConsoleHeight);
    }
    
    static void SaveSpeed(short ConsoleHeight)
    {
        Tab::Save();
        Tab::NotifySaves(ConsoleHeight);
    }

    static void DrawQuickSetup(string str, short ConsoleWidth, short ConsoleHeight)
    {

    }

private:

    static void ConsoleCursorVisible(bool show, short size)
    {
        CONSOLE_CURSOR_INFO structCursorInfo;
        GetConsoleCursorInfo(hConsole, &structCursorInfo);
        structCursorInfo.bVisible = show;
        structCursorInfo.dwSize = size;
        SetConsoleCursorInfo(hConsole, &structCursorInfo);
    }

    static void ShowValue(short ConsoleHeight)
    {
        SetConsoleTextAttribute(hConsole, 15);
        cPos.X = 13; cPos.Y = ConsoleHeight - 2;
        SetConsoleCursorPosition(hConsole, cPos);
        ConsoleCursorVisible(true, 5);
        cout << value << "       ";
        int tmp = value;
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
};

CONSOLE_SCREEN_BUFFER_INFO csbInfo;
void GetBufferChars(COORD* Chars)
{
    GetConsoleScreenBufferInfo(hConsole, &csbInfo);
    Chars->X = csbInfo.srWindow.Right - csbInfo.srWindow.Left + 1;
    Chars->Y = csbInfo.srWindow.Bottom - csbInfo.srWindow.Top + 1;
}

int main()
{
    setlocale(LC_ALL, "Russian"); // задаем языковой стандарт на текущую кодовую страницу
    GetBufferChars(&Chars); // получаем размер консоли

    points = 3;

    short ConsoleWidth = Chars.X;
    short ConsoleHeight = Chars.Y;
    short MinConsoleWidth = ConsoleWidth;
    short MinConsoleHeight = ConsoleHeight;  
      
    while (true)
    {
        if (S == "-1")
        {
            if (S == "4") { break; } // если пользователь выходит из программы
            while (true) 
            {
                /* обратим внимание на то, что если бы мы не добавили здесь и дальше в программе
                дополнительные области видимости, то тогда 
                мы бы хранили в памяти информацию об объектах класса, которые уже никак не используем*/
                if (S == "-1") 
                {
                    // входим во вкладку меню
                    MinConsoleWidth = 65; MinConsoleHeight = 20;
                    Tab Menu("-Menu.txt");
                    Menu.Entrance(Interface::ClearAndShowInfo, ConsoleWidth, ConsoleHeight, MinConsoleWidth, MinConsoleHeight, true, false);
                    // будем записывать значение true, если хотим создать вкладку в пустой консоли
                    // и false если хотим создавать поверх предыдущей вкладки
                }
                if (S == "0") { break; } // если пользователь выбрал start
                if (S == "1") // если пользователь входит во вкладку настроек
                {
                    S = "-1"; Digit = ' ';
                    while (true) 
                    {
                        if (S == "-1") 
                        {
                            // входим во вкладку настроек
                            MinConsoleWidth = 65; MinConsoleHeight = 20;
                            Tab Settings("-Settings.txt");
                            Settings.Entrance(Interface::ClearAndShowInfo, ConsoleWidth, ConsoleHeight, MinConsoleWidth, MinConsoleHeight, true, false);
                        }
                        if (S == "0") { break; } // если пользователь переходит на шаг назад
                        if (S == "1") 
                        { // если пользователь вошел в игровые параметры
                            S = "-1"; Digit = ' ';
                            while (true) 
                            {
                                if (S == "-1") 
                                {
                                    // входим во вкладку игровых параметров
                                    MinConsoleWidth = 65; MinConsoleHeight = 20;
                                    Tab GameOptions("-GameOptions.txt");
                                    GameOptions.Entrance(Interface::ClearAndShowInfo, ConsoleWidth, ConsoleHeight, MinConsoleWidth, MinConsoleHeight, true, false);
                                }
                                if (S == "0") { break; } // переходим на шаг назад
                                if (S == "1") 
                                { 
                                    S = "-1"; Digit = ' ';
                                    // входим во вкладку быстрой настройки
                                    Tab QuickSetup("QuickSetup.txt");
                                    while (S != "0")
                                    {
                                        QuickSetup.Entrance(Interface::DrawQuickSetup, ConsoleWidth, ConsoleHeight, MinConsoleWidth, MinConsoleHeight, true, true);
                                    }
                                }
                                if (S == "2") 
                                {
                                    S = "-1"; Digit = ' ';
                                    while (true) 
                                    {
                                        if (S == "-1") 
                                        {
                                            // входим во вкладку карты
                                            MinConsoleWidth = 70; MinConsoleHeight = 20;
                                            Tab Map("-Map.txt");
                                            Map.Entrance(Interface::ClearAndShowInfo, ConsoleWidth, ConsoleHeight, MinConsoleWidth, MinConsoleHeight, true, false);
                                        }
                                        if (S == "0") { break; } // переходим на шаг назад
                                        if (S == "1")
                                        {
                                            S = "-1"; Digit = ' ';
                                            // входим во вкладку размера игровой карты
                                            MinConsoleWidth = 70; MinConsoleHeight = 20;
                                            Tab Size("MapSize.txt");                                              
                                            while (S != "0")
                                            {                  
                                                Str = "-13";
                                                Size.Entrance(Interface::PreviewMap, ConsoleWidth, ConsoleHeight, MinConsoleWidth, MinConsoleHeight, true, false);
                                                Size.Entrance(Interface::ClearAndShowInfo, ConsoleWidth, ConsoleHeight, MinConsoleWidth, MinConsoleHeight, false, false);
                                                T = S; if (S == "0") { break; }
                                                if (S == "-1" || ConsoleWidth != Chars.X || ConsoleHeight != Chars.Y) { ConsoleWidth = Chars.X; ConsoleHeight = Chars.Y; continue; } 
                                                S = "-1"; if (T == "3") { Str = "-13"; }
                                                Size.Entrance(Interface::MapSize, ConsoleWidth, ConsoleHeight, MinConsoleWidth, MinConsoleHeight, false, true);
                                                S = "-1"; Digit = ' '; T = "-1";
                                            }
                                        }
                                        if (S == "2")
                                        { 
                                            // входим во вкладку отображения границ карты
                                            S = "-1"; Digit = ' ';
                                            MinConsoleWidth = 70; MinConsoleHeight = 10;
                                            Tab Border("MapBorder.txt");
                                            while (S != "0")
                                            {                                           
                                                Str = "-13";
                                                Border.Entrance(Interface::MapBorder, ConsoleWidth, ConsoleHeight, MinConsoleWidth, MinConsoleHeight, true, false);
                                                S = "-1"; Digit = ' ';
                                                Border.Entrance(Interface::ClearAndShowInfo, ConsoleWidth, ConsoleHeight, MinConsoleWidth, MinConsoleHeight, false, false);
                                                ConsoleWidth = Chars.X; ConsoleHeight = Chars.Y;
                                            }
                                        }
                                        if (S == "3")
                                        { 
                                            while (S != "0")
                                            {
                                                S = "-1"; Digit = ' ';
                                                // входим во вкладку выравнивания карты
                                                MinConsoleWidth = 70; MinConsoleHeight = 10;
                                                Tab Alignment("MapAlignment.txt");
                                                Alignment.Entrance(Interface::ClearAndShowInfo, ConsoleWidth, ConsoleHeight, MinConsoleWidth, MinConsoleHeight, true, false);
                                                if (S == "0") { break; }
                                                if (S == "1")
                                                {
                                                    S = "-1";
                                                    Tab AlignmentHorizontal(" ");
                                                    while (true)
                                                    {
                                                        AlignmentHorizontal.Entrance(Interface::Horizontal, ConsoleWidth, ConsoleHeight, MinConsoleWidth, MinConsoleHeight, true, true);
                                                        if (ConsoleWidth == Chars.X && ConsoleHeight == Chars.Y)
                                                            break;
                                                    }
                                                    S = "-1"; Digit = ' '; ConsoleWidth = Chars.X; ConsoleHeight = Chars.Y;
                                                }
                                                if (S == "2")
                                                {
                                                    S = "-1";
                                                    Tab AlignmentVertical(" ");
                                                    while (true)
                                                    {
                                                        AlignmentVertical.Entrance(Interface::Vertical, ConsoleWidth, ConsoleHeight, MinConsoleWidth, MinConsoleHeight, true, true);
                                                        if (ConsoleWidth == Chars.X && ConsoleHeight == Chars.Y)
                                                            break;
                                                    }
                                                    S = "-1"; Digit = ' '; ConsoleWidth = Chars.X; ConsoleHeight = Chars.Y;
                                                }
                                                if (S == "3")
                                                {
                                                    Interface::AlignmentSave(ConsoleHeight);
                                                }
                                            }
                                        }
                                        S = "-1"; Digit = ' ';
                                    }
                                }
                                if (S == "3") 
                                { 
                                    S = "-1"; Digit = ' ';
                                    // входим во вкладку выбора цвета
                                    MinConsoleWidth = 100; MinConsoleHeight = 25;
                                    Tab Color("Color.txt");
                                    while (S != "0")
                                    {
                                        Str = "-13";
                                        Color.Entrance(Interface::ColoredMap, ConsoleWidth, ConsoleHeight, MinConsoleWidth, MinConsoleHeight, true, false);
                                        Color.Entrance(Interface::ClearAndShowInfo, ConsoleWidth, ConsoleHeight, MinConsoleWidth, MinConsoleHeight, false, false);
                                        T = S; if (S == "0") { break; }
                                        if (S == "-1" || ConsoleWidth != Chars.X || ConsoleHeight != Chars.Y) { ConsoleWidth = Chars.X; ConsoleHeight = Chars.Y; continue; }
                                        S = "-1"; if (T == "3") { Str = "-13"; }
                                        Color.Entrance(Interface::ColoredMap, ConsoleWidth, ConsoleHeight, MinConsoleWidth, MinConsoleHeight, false, true);
                                        S = "-1"; Digit = ' '; T = "-1";
                                    }
                                }
                                if (S == "4") 
                                {
                                    S = "-1"; Digit = ' ';
                                    // входим во вкладку выбора скорости
                                    MinConsoleWidth = 70; MinConsoleHeight = 15;
                                    Tab Speed("Speed.txt"); value = 1;
                                    while (S != "0")
                                    {
                                        Speed.Entrance(Interface::ChangeSpeed, ConsoleWidth, ConsoleHeight, MinConsoleWidth, MinConsoleHeight, true, true);
                                    }
                                    Interface::SaveSpeed(ConsoleHeight); this_thread::sleep_for(chrono::milliseconds(500));
                                }
                                if (S == "5") 
                                { 
                                    S = "-1"; Digit = ' ';
                                    while (true)
                                    {
                                        if (S == "-1")
                                        {
                                            // входим во вкладку препятствий
                                            MinConsoleWidth = 70; MinConsoleHeight = 20;
                                            Tab Obstacles("Obstacles.txt");
                                            Obstacles.Entrance(Interface::ClearAndShowInfo, ConsoleWidth, ConsoleHeight, MinConsoleWidth, MinConsoleHeight, true, false);
                                        }
                                        if (S == "0") { break; } // переходим на шан назад
                                        if (S == "1") // editor
                                        {

                                        }
                                        if (S == "2") // random generator
                                        {

                                        }
                                        if (S == "3") // list
                                        {

                                        }
                                        S = "-1"; Digit = ' ';
                                    }
                                }
                                S = "-1"; Digit = ' ';
                            }
                        }
                        S = "-1"; Digit = ' ';
                    }
                }
                if (S == "2")
                {

                }
                if (S == "3")
                {
                    S = "-1"; Digit = ' ';
                    while (true)
                    {
                        if (S == "-1")
                        {
                            // входим во вкладку о программе
                            MinConsoleWidth = 80; MinConsoleHeight = 20;
                            Tab AboutProgram("-AboutProgram.txt");
                            AboutProgram.Entrance(Interface::ClearAndShowInfo, ConsoleWidth, ConsoleHeight, MinConsoleWidth, MinConsoleHeight, true, false);
                        }
                        if (S == "0") { break; } // если пользователь переходит на шаг назад
                        S = "-1"; Digit = ' ';
                    }
                }
                if (S == "4") { break; } // если пользователь выходит из программы
                S = "-1"; Digit = ' ';
            }
            // страт выбранного уровня
        }
        if (S == "4") { break; }
    }
    return 0;
}