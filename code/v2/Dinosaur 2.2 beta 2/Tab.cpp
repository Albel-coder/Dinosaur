#include "Tab.h"

Tab::Tab(string str)
{
    Path = str;
    tmp = ' ';
    Key = 1;
    SelectedFunction = 1000;
    cPos.X = 0;
    cPos.Y = 0;
    Value = 0;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}

void Tab::Entrance(void (*DrawInfo)(string Path, short& ConsoleWidth, short& ConsoleHeight))
{
    while (true)
    {        
        //  Разделим работу со вкладкой на две задачи: 
        //  1) выбрать функцию 
        //  2) если выбрали не вкладку, то вводим значение для переменной
        while (tmp != ENTER) // Выбор функции
        {            
            if (ConS.Acceptable()) // проверяем, подходящего ли размера консоль
            {
                Key = ' ';
                system("cls");
                // рисуем элементы вкладки, являющиеся пересечением множеств всех вкладок
                Draw(ConS.GetConsoleWidth(), ConS.GetConsoleHeight());                 
                DrawInfo(Path, ConS.GetConsoleWidth(), ConS.GetConsoleHeight());
                ConS.GetBuffer();
                // пользователь выбирает функцию или вкладку
                if (!Select(false, ClearAndShowInfo) || ModifiedInterface == '1')
                {
                    break;
                }                                          
                // возвращаем начальные значения
                cPos.X = 13;
                cPos.Y = ConS.GetConsoleHeight() - 2;
                SetConsoleCursorPosition(hConsole, cPos);
                cout << " ";
            }
            else
            {
                WaitForValidSize();
            }
        }
        // если пользователь вошел во вкладку, прерываем функцию
        if (DrawInfo == ClearAndShowInfo)
        {
            if (tmp == ENTER)
            {
                return;
            }
            else
            {
                continue;
            }
        }        
        if (Key == '0')
        {
            return;
        }
        unsigned char ch = '0';
        for (int i = 0; i < 100; i++)
        {
            if (ch == Key)
            {
                SelectedFunction = i;
                break;
            }
            ch++;
        }        
                
        tmp = ' ';
        Key = 1;
        DrawInfo(Path, ConS.GetConsoleWidth(), ConS.GetConsoleHeight());

        while (tmp != ENTER) // ввод значения
        {
            if (ConS.Acceptable())
            {
                Key = ' ';
                ConS.GetBuffer();
                if (!Select(true, DrawInfo)) // вводим значения для переменной соответствующей функции
                {
                    break;
                }              
                // возвращаем начальные значения
                Value = 0;
                cPos.X = 13;
                cPos.Y = ConS.GetConsoleHeight() - 2;
                SetConsoleCursorPosition(hConsole, cPos);
                cout << " ";
            }
            else
            {
                WaitForValidSize();
            }
        }
        tmp = ' ';
        Key = 1;
        SelectedFunction = 1000;
        cPos.X = 0;
        cPos.Y = 0;
        Value = 0;
        if (ModifiedInterface == '1')
        {
            break;
        }
    }
}

void Tab::Draw(short& ConsoleWidth, short& ConsoleHeight)
{
    // выводим элементы вкладки в консоль
    short Color = 15;
    ifstream in("Tabs\\" + Path + ".txt");
    if (in.is_open())
    {
        string str;
        if (Conversion(1, 3)) // если пользователь включил демонстрацию пути функции
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
        if (Conversion(2, 3) && Path != "Menu") // если пользователь включил демонстрацию go back
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

#ifdef DEBUG
    cPos.X = ConsoleWidth - 10; cPos.Y = 0;
    SetConsoleTextAttribute(hConsole, 7);

    for (int i = 0; i < 12; i++)
    {
        SetConsoleCursorPosition(hConsole, cPos);
        cout << "[" << i << "] " << int(Variables[i]);
        cPos.Y++;
    }
    for (int i = 12; i < 16; i++)
    {
        SetConsoleCursorPosition(hConsole, cPos);
        cout << "[" << i << "] " << Variables[i];
        cPos.Y++;
    }
#endif // DEBUG

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

int Tab::Conversion(short a, unsigned long long b)
{
    for (int i = 0; i < (a - 1); i++)
    {
        b /= 2;
    }
    return (b % 2);

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
}

string Tab::GetPath()
{
    return Path;
}

bool Tab::Select(bool ChangeValue, void (*DrawInfo)(string Path, short& ConsoleWidth, short& ConsoleHeight))
{
    while (true)
    {
        while (true) // пока пользователь не ввел что-то или не изменил размер консоли
        {
            ConS.GetBuffer();

            if (Input(Key))
            {
                break;
            }
            else if (ConS.IsChanged())
            {
                return false;
            }
            this_thread::sleep_for(chrono::milliseconds(20));
        }
        if (tmp == ENTER || tmp == DELETE || tmp == ESC)
        {            
            return true;
        }        
        // если пользователь ввел не тоже самое (кроме ввода значений в переменную)
        else if (Key != tmp || DrawInfo != ClearAndShowInfo)
        {
            if (ChangeValue)
            {
                if (tmp != BACKSPACE) 
                {
                    unsigned char Ch = '0';
                    for (int i = 0; i < 10; i++)
                    {
                        if (Ch == tmp)
                        {
                            Value *= 10;
                            Value += i;
                            break;
                        }
                        Ch++;
                    }
                }
                else
                {
                    Value /= 10;
                }
            }            
            // выводим информацию о выбранной вкладке или текущего значения переменной
            DrawInfo(Path, ConS.GetConsoleWidth(), ConS.GetConsoleHeight());
            Key = tmp;
        }
    }

    return true;
}

void Tab::WaitForValidSize()
{
    // уведомляем пользователя о недостаточном размере консоли
    system("cls");
    cPos.X = 0; cPos.Y = 0; 
    SetConsoleCursorPosition(hConsole, cPos);
    SetConsoleTextAttribute(hConsole, 7);
    cout << "Your current console size is less than acceptable! \n\nEnlarge your console window or set full screen to continue.";
    while (!ConS.Acceptable()) // ждем, пока пользователь установит нужный размер консоли
    {
        ConS.GetBuffer();
        this_thread::sleep_for(chrono::milliseconds(20));
    }
}

bool Tab::Input(char& Key)
{
    if (_kbhit())
    {
        char ch = _getch();
        switch (ch)
        {
        case ENTER:
            tmp = 13;
            return true;
        case ESC:
            tmp = 27;
            return true;
        case DELETE:
            tmp = 8;
            return true;
        default:
            tmp = ch;
            return true;
        }
    }
    
    return false;
}

#ifdef DOWNLOAD_NOT_RELEASED

bool Tab::CreateFiles()
{
    bool Result = true;
    CreateDirectory(L"Tab", NULL);

    // создаем области видимости, чтобы убивать в нужный момент элементы ofstream
    {
        ofstream out("Tab\\Menu.txt");
        if (out.is_open())
        {
            out << "&\n";
            out << "15 Start\n";
            out << "4 Enter 0\n";
            out << "15 Settings\n";
            out << "4 Enter 1\n";
            out << "15 Level editor\n";
            out << "4 Enter 2\n";
            out << "15 About program\n";
            out << "4 Enter 3\n";
            out << "15 Exit\n";
            out << "4 Enter 4\n";
        }
        else
        {
            Result = false;
        }
        out.close();
    }

    {
        ofstream out("Tab\\InfoMenu.txt");
        if (out.is_open())
        {
            out << "20 4 - Choose and start level\n";
            out << "3 Start\n";
            out << "20 6 - Change game parameters as you want\n";
            out << "3 Settings\n";
            out << "20 8 - Create and edit levels\n";
            out << "3 Level editor\n";
            out << "20 10 - Find out more about the application\n";
            out << "3 About program\n";
        }
        else
        {
            Result = false;
        }
    }

    {
        ofstream out("Tab\\AboutProgram.txt");
        if (out.is_open())
        {
            out << "@ 15 > 1 About 1 program 0 &\n";
            out << "15 This program was originally conceived as a computer game,\n";
            out << "15 created based on the google dinosaur game in a rare style in console.\n";
            out << "15 In fact, the application received full functionality\n";
            out << "15 of this idea only in the current version. \n";
            out << "15 And besides this, the program received \n";
            out << "15 many other interesting features in the new update 2.2.\n";
        }
        else
        {
            Result = false;
        }
        out.close();
    }

    {
        ofstream out("Tab\\InfoAboutProgram.txt");
        if (out.is_open())
        {

        }
        else
        {
            Result = false;
        }
        out.close();
    }

    {
        ofstream out("Tab\\Settings.txt");
        if (out.is_open())
        {
            out << "@ 15 > 1 Settings 0 &\n";
            out << "15 Game options\n";
            out << "Enter 1\n";
            out << "15 Accessibility\n";
            out << "Enter 2\n";
            out << "15 Screen\n";
            out << "Enter 3\n";
            out << "15 Quality\n";
            out << "Enter 4\n";
            out << "15 Control\n";
            out << "Enter 5\n";
            out << "15 Keys\n";
            out << "Enter 6\n";
            out << "15 Other\n";
            out << "Enter 7\n";
        }
        else
        {
            Result = false;
        }
        out.close();
    }

    {
        ofstream out("Tab\\InfoSettings.txt");
        if (out.is_open())
        {
            out << "12 1 - go one step back\n";
            out << "0 <- Go back\n";
            out << "20 4 - Change games variables\n";
            out << "3 Game options\n";
            out << "20 6 - Size of text\n";
            out << "3 Accessibility\n";
            out << "20 8 - Screen settings\n";
            out << "3 Screen\n";
            out << "20 10 - Change quality in full detailed dino mode\n";
            out << "3 Quality\n";
            out << "20 12 - Control settings\n";
            out << "3 Control\n";
            out << "20 14 - Keys settings\n";
            out << "Keys\n";
            out << "20 16 - Change other game settings\n";
            out << "Other\n";
        }
        else
        {
            Result = false;
        }
        out.close();
    }

    {
        ofstream out("Tab\\GameOptions.txt");
        if (out.is_open())
        {
            out << "@ 15 > 1 Settings 15 > 1 Game 1 Options 0 &\n";
            out << "15 Quick setup\n";
            out << "4 Enter 1\n";
            out << "15 Map\n";
            out << "4 Enter 2\n";
            out << "15 Color\n";
            out << "4 Enter 3\n";
            out << "15 Speed\n";
            out << "4 Enter 4\n";
            out << "15 Obstacles\n";
            out << "4 Enter 5\n";
        }
        else
        {
            Result = false;
        }
        out.close();
    }

    {
        ofstream out("Tab\\InfoGameOptions.txt");
        if (out.is_open())
        {
            out << "12 1 - go one step back\n";
            out << "0 <- Go back\n";
            out << "20 4 - Change game values in one tab\n";
            out << "3 Quick setup\n";
            out << "20 6 - Install and change map settings\n";
            out << "3 Map\n";
            out << "20 8 - Set the color of objects as you want\n";
            out << "3 Color\n";
            out << "20 10 - Initial speed values\n";
            out << "3 Speed\n";
            out << "20 12 - Create and generate your own obstacles\n";
            out << "3 Obstacles\n";
        }
        else
        {
            Result = false;
        }
        out.close();
    }

    {
        ofstream out("Tab\\Map.txt");
        if (out.is_open())
        {
            out << "@ 15 > 1 Settings 15 > 1 Game 1 Options 15 > 1 Map 0 &\n";
            out << "15 Size\n";
            out << "4 Enter 1\n";
            out << "15 Border\n";
            out << "4 Enter 2\n";
            out << "15 Alignment\n";
            out << "4 Enter 3\n";
        }
        else
        {
            Result = false;
        }
        out.close();
    }

    {
        ofstream out("Tab\\InfoMap.txt");
        if (out.is_open())
        {
            out << "12 1 - go one step back\n";
            out << "0 <- Go back\n";
            out << "20 4 - change map size\n";
            out << "3 Size\n";
            out << "20 6 - turn on or off map borders\n";
            out << "3 Border\n";
            out << "20 8 - change map alignment\n";
            out << "3 Alignment\n";
        }
        else
        {
            Result = false;
        }
        out.close();
    }

    {
        ofstream out("Tab\\MapSize.txt");
        if (out.is_open())
        {
            out << "! 15 > 1 Settings 15 > 1 Game 1 Options 15 > 1 Map 15 > 1 Size 0 &\n";
            out << "15 Width\n";
            out << "4 Enter 1\n";
            out << "15 Height\n";
            out << "4 Enter 2\n";
            out << "15 Save\n";
            out << "4 Enter 3\n";
        }
        else
        {
            Result = false;
        }
        out.close();
    }

    {
        ofstream out("Tab\\InfoMapSize.txt");
        if (out.is_open())
        {
            out << "12 1 - go one step back\n";
            out << "0 <- Go back\n";
            out << "10 4 <\n";
            out << "3 Width\n";
            out << "10 6 <\n";
            out << "3 Height\n";
            out << "10 8 <\n";
            out << "3 Save\n";
        }
        else
        {
            Result = false;
        }
        out.close();
    }

    {
        ofstream out("Tab\\MapBorder.txt");
        if (out.is_open())
        {
            out << "? 15 > 1 Settings 15 > 1 Game 1 Options 15 > 1 Map 15 > 1 Border 0 &\n";
            out << "15 Right border\n";
            out << "4 Enter 1\n";
            out << "15 Left border\n";
            out << "4 Enter 2\n";
            out << "15 Up border\n";
            out << "4 Enter 3\n";
            out << "15 Down border\n";
            out << "4 Enter 4\n";
            out << "15 Save\n";
            out << "4 Enter 5\n";
        }
        else
        {
            Result = false;
        }
        out.close();
    }

    {
        ofstream out("Tab\\InfoMapBorder.txt");
        if (out.is_open())
        {
            out << "12 1 - go one step back\n";
            out << "0 <- Go back\n";
            out << "10 4 <\n";
            out << "3 Right border\n";
            out << "10 6 <\n";
            out << "3 Left border\n";
            out << "10 8 <\n";
            out << "3 Up border\n";
            out << "10 10 <\n";
            out << "3 Down border\n";
            out << "10 12 <\n";
            out << "3 Save\n";
        }
        else
        {
            Result = false;
        }
        out.close();
    }

    {
        ofstream out("Tab\\MapAlignment.txt");
        if (out.is_open())
        {
            out << "! 15 > 1 Settings 15 > 1 Game 1 Options 15 > 1 Map 15 > 1 Alignment 0 &\n";
            out << "15 Horizontal\n";
            out << "4 Enter 1\n";
            out << "15 Vertical\n";
            out << "4 Enter 2\n";
            out << "15 Save\n";
            out << "4 Enter 3\n";
        }
        else
        {
            Result = false;
        }
        out.close();
    }

    {
        ofstream out("Tab\\InfoMapAlignment.txt");
        if (out.is_open())
        {
            out << "12 1 - go one step back\n";
            out << "0 <- Go back\n";
            out << "10 4 <\n";
            out << "3 Horizontal\n";
            out << "10 6 <\n";
            out << "3 Vertical\n";
            out << "10 8 <\n";
            out << "3 Save\n";
        }
        else
        {
            Result = false;
        }
        out.close();
    }

    {
        ofstream out("Tab\\Color.txt");
        if (out.is_open())
        {
            out << "? 15 > 1 Settings 15 > 1 Game 1 Options 15 > 1 Color 0 &\n";
            out << "15 Right border\n";
            out << "4 Enter 1\n";
            out << "15 Left border\n";
            out << "4 Enter 2\n";
            out << "15 Up border\n";
            out << "4 Enter 3\n";
            out << "15 Down border\n";
            out << "4 Enter 4\n";
            out << "15 Obstacles color\n";
            out << "4 Enter 5\n";
            out << "15 Player color\n";
            out << "4 Enter 6\n";
            out << "15 Points color\n";
            out << "4 Enter 7\n";
            out << "15 Save\n";
            out << "4 Enter 8\n";
        }
        else
        {
            Result = false;
        }
        out.close();
    }

    {
        ofstream out("Tab\\InfoColor.txt");
        if (out.is_open())
        {
            out << "12 1 - go one step back\n";
            out << "0 <- Go back\n";
            out << "10 4 <\n";
            out << "3 Right border\n";
            out << "10 6 <\n";
            out << "3 Left border\n";
            out << "10 8 <\n";
            out << "3 Up border\n";
            out << "10 10 <\n";
            out << "3 Down border\n";
            out << "10 12 <\n";
            out << "3 Obstacles color\n";
            out << "10 14 <\n";
            out << "3 Player color\n";
            out << "10 16 <\n";
            out << "3 Points color\n";
            out << "10 18 <\n";
            out << "3 Save\n";
        }
        else
        {
            Result = false;
        }
        out.close();
    }

    {
        ofstream out("Tab\\ColorList.txt");
        if (out.is_open())
        {
            out << "Blue\n";
            out << "Green\n";
            out << "Cyan\n";
            out << "Red\n";
            out << "Magenta\n";
            out << "Brown\n";
            out << "Lightgray\n";
            out << "Darkgray\n";
            out << "Lightblue\n";
            out << "Lightgreen\n";
            out << "Lightcyan\n";
            out << "Lightred\n";
            out << "Lightmagenta\n";
            out << "Yellow\n";
            out << "White\n";
        }
        else
        {
            Result = false;
        }
        out.close();
    }

    {
        ofstream out("Tab\\Speed.txt");
        if (out.is_open())
        {
            out << "! 15 > 1 Settings 15 > 1 Game 1 Options 15 > 1 Speed 0 &\n";
        }
        else
        {
            Result = false;
        }
        out.close();
    }

    return Result; 
}

#endif // DOWNLOAD_NOT_RELEASED

void ClearAndShowInfo(string Path, short& ConsoleWidth, short& ConsoleHeight)
{
    // выводим информацию о функции    
    unsigned char i = '0';
    ifstream in("Tabs\\Info" + Path + ".txt");
    if (in.is_open())
    {
        // переменная Path нам больше не нужна
        // вместо того чтобы создавать новую переменную 
        // можем использовать Path, например, для записи в нее данных из файла :)
        while (!in.eof())
        {
            in >> cPos.X >> cPos.Y;
            getline(in, Path);
            if (i == tmp) // выводим новую информацию о выбранной функции
            {
                SetConsoleCursorPosition(hConsole, cPos);
                SetConsoleTextAttribute(hConsole, 7);
                cout << Path;
                in >> cPos.X;
                SetConsoleCursorPosition(hConsole, cPos);
                SetConsoleTextAttribute(hConsole, 1);
                getline(in, Path);
                cout << Path;
            }
            else if (i == Key) // очищаем старую информацию о выбранной функции
            {
                SetConsoleCursorPosition(hConsole, cPos);
                SetConsoleTextAttribute(hConsole, 0);
                cout << Path;
                in >> cPos.X;
                SetConsoleCursorPosition(hConsole, cPos);
                SetConsoleTextAttribute(hConsole, 15);
                getline(in, Path);
                cout << Path;
            }
            else // пропускаем все остальное
            {
                getline(in, Path);
            }
            i++;
        }
    }
    in.close();
    cPos.X = 13; cPos.Y = ConsoleHeight - 2;
    SetConsoleCursorPosition(hConsole, cPos);
    SetConsoleTextAttribute(hConsole, 15);
    cout << tmp;
}