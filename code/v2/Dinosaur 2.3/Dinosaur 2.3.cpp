#include <iostream>
#include <windows.h>
#include <ctime>
#include <thread>
#include <stdio.h>
#include <conio.h>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include "Console.h"
#include "LowDetailedMode.h"
#include "Download.h"

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
COORD cPos;

char Key = ' ';
uint8_t JumpState = 0;
int JumpHeight = 0;
std::vector<std::vector<std::string>> Cactus(4);
std::string DinoLeg1Sprite;
std::string DinoLeg2Sprite;
std::string DinoSprite;
bool Settings[4];
float Pos = 0;
float Multi = 0;
uint64_t point = 0;
uint8_t Value = 0;
bool Jump = false;
bool DetailedGameMode = true;
bool CheckUpdate = false;
bool IsShow = false;
bool InternetConnection = true;

bool ShowDownloadStatus = false;
int InputStyle = 1;

void DrawMenuWithSelection(int selectedIndex, short ConsoleHeight, short ConsoleWidth)
{
    cPos.X = 0; cPos.Y = 0; SetConsoleCursorPosition(hConsole, cPos);

    const std::string menuItems[5] = { "Start", "Settings", "Input style", "About program", "Exit" };
    const std::string descriptions[5] = 
    {
        " - start the game with selected parameters",
        " - change game parameters as you want",
        " - select the appropriate input type",
        " - find more about the application",
        " - exit the application"
    };
    const char digits[5] = { '0','1','2','3', '4' };
    const int LINE_LENGTH = 50; // Фиксированная длина строки

    std::cout << "\n\n"; // Отступы сверху

    // Отрисовка пунктов меню
    for (int i = 0; i < 5; i++) 
    {
        // Вывод основного текста пункта
        if (i == selectedIndex) 
        {
            SetConsoleTextAttribute(hConsole, 1); // Синий для выбранного
            std::string line = "  " + menuItems[i] + descriptions[i];
            std::cout << line;
            // Дополнение пробелами до фиксированной длины
            if (line.length() < LINE_LENGTH) 
            {
                std::cout << std::string(LINE_LENGTH - line.length(), ' ');
            }
        }
        else 
        {
            SetConsoleTextAttribute(hConsole, 15); // Белый для остальных
            std::string line = "  " + menuItems[i];
            std::cout << line;
            // Дополнение пробелами
            if (line.length() < LINE_LENGTH) 
            {
                std::cout << std::string(LINE_LENGTH - line.length(), ' ');
            }
        }
        std::cout << "\n";

        // Вывод команды для выбора
        SetConsoleTextAttribute(hConsole, 4); // Красный
        std::cout << "  Enter " << digits[i] << "\n";        
    }
    std::cout << "\n\n\n";
    SetConsoleTextAttribute(hConsole, 1);
    std::cout << "Enter value: ";
    SetConsoleTextAttribute(hConsole, 15);
    std::cout << Key;    

    cPos.X = ConsoleWidth - 15; cPos.Y = ConsoleHeight - 10;
    SetConsoleCursorPosition(hConsole, cPos);
    SetConsoleTextAttribute(hConsole, 7); // Стандартный цвет
    std::cout << "version 2.3";
}

void DrawInputStyleWithSelection(int selectedIndex, short ConsoleHeight, short ConsoleWidth)
{
    cPos.X = 0; cPos.Y = 0; SetConsoleCursorPosition(hConsole, cPos);

    const std::string menuItems[3] = { "Go back to menu", "Step by step master", "CLI" };
    const std::string descriptions[3] =
    {
        " - go one step back to menu tab",
        " - set up all parameters step by step",
        " - interact with the system by entering a command into the text line",
    };
    const char digits[3] = { '0','1','2' };
    const int LINE_LENGTH = 80; // Фиксированная длина строки

    std::cout << "\n\n"; // Отступы сверху

    // Отрисовка пунктов меню
    for (int i = 0; i < 3; i++)
    {
        // Вывод основного текста пункта
        if (i == selectedIndex)
        {
            SetConsoleTextAttribute(hConsole, 1); // Синий для выбранного
            std::string line = "  " + menuItems[i] + descriptions[i];
            std::cout << line;
            // Дополнение пробелами до фиксированной длины
            if (line.length() < LINE_LENGTH)
            {
                std::cout << std::string(LINE_LENGTH - line.length(), ' ');
            }
        }
        else
        {
            SetConsoleTextAttribute(hConsole, 15); // Белый для остальных
            std::string line = "  " + menuItems[i];
            std::cout << line;
            // Дополнение пробелами
            if (line.length() < LINE_LENGTH)
            {
                std::cout << std::string(LINE_LENGTH - line.length(), ' ');
            }
        }
        std::cout << "\n";

        // Вывод команды для выбора
        SetConsoleTextAttribute(hConsole, 4); // Красный
        std::cout << "  Enter " << digits[i] << "\n";
    }
    std::cout << "\n\n\n";
    SetConsoleTextAttribute(hConsole, 8);
    if (InputStyle == 0)
    {
        std::cout << "Current system: step by step master      ";
    }
    if (InputStyle == 1)
    {
        std::cout << "Current system: CLI                     ";
    }
    std::cout << "\n\n\n";
    SetConsoleTextAttribute(hConsole, 1);
    std::cout << "Enter value: ";
    SetConsoleTextAttribute(hConsole, 15);
    std::cout << Key;

    cPos.X = ConsoleWidth - 15; cPos.Y = ConsoleHeight - 10;
    SetConsoleCursorPosition(hConsole, cPos);
    SetConsoleTextAttribute(hConsole, 7); // Стандартный цвет
    std::cout << "version 2.3";
}

void ShowCheckUpdateStatus(bool IsVersionNewest, std::string Version)
{
    cPos.X = 0; cPos.Y = 15; SetConsoleCursorPosition(hConsole, cPos);
    std::cout << "\n\n\n";
    SetConsoleTextAttribute(hConsole, 8);
    if (!CheckUpdate)
    {
        std::cout << "Checking latest version from github...";
    }
    else
    {
        if (InternetConnection)
        {
            if (IsVersionNewest)
            {
                SetConsoleTextAttribute(hConsole, 2);
                std::cout << "Your version is newest!                  \n";
                std::cout << "                                         \n";
                std::cout << "                                         \n";
                std::cout << "                                         \n";
            }
            else
            {
                SetConsoleTextAttribute(hConsole, 4);
                std::cout << "Your version is old!                  \n\n";
                SetConsoleTextAttribute(hConsole, 7);
                std::cout << "Download the newest " << Version << " version from github:\n";
                std::cout << "https://github.com/Albel-coder/Dinosaur";
            }
        }
        else
        {
            SetConsoleTextAttribute(hConsole, 4);
            std::cout << "Error with checking new version!                  \n\n";
            SetConsoleTextAttribute(hConsole, 7);
            std::cout << "Please check your internet connection";
        }
    }
    IsShow = true;
}

void DrawAboutProgramWithSelection(short ConsoleHeight, short ConsoleWidth)
{
    system("cls");
    cPos.X = 0; cPos.Y = 0; SetConsoleCursorPosition(hConsole, cPos);
    SetConsoleTextAttribute(hConsole, 7);
    std::cout << "This program was originally conceived as a computer game,\n";
    std::cout << "created based on the google dinosaur game in a rare style in console.\n";
    std::cout << "In fact, the application received full functionality\n";
    std::cout << "of this idea only in the current version.\n";
    std::cout << "And besides this, the program received\n";
    std::cout << "many other interesting features in the new update 2.3.\n\n\n";
    std::cout << "Now you can go to the settings tab\n";
    std::cout << "set all the necessary parameters for a comfortable game\n";
    std::cout << "and start playing by clicking on the appropriate tab\n";
    std::cout << "See you in the next updates ;)\n\n\n";
    std::cout << "To continue press any key\n";

    cPos.X = ConsoleWidth - 15; cPos.Y = ConsoleHeight - 10;
    SetConsoleCursorPosition(hConsole, cPos);
    SetConsoleTextAttribute(hConsole, 7);
    std::cout << "version 2.3";

    while (true)
    {
        if (_kbhit())
        {
            break;
        }
    }
    system("cls");
    Key = ' ';
}

bool InputKey() 
{
    if (_kbhit()) 
    {
        Key = _getch();
        if (Key == 'w' || Key == 'W')
        {
            JumpState = 1;
            return true;
        }
    }
    return false;
}

bool Input()
{
    if (_kbhit())
    {
        Key = _getch();
        return true;
    }
    return false;
}

void SetF11Mode(Console& Cons)
{
    INPUT inputs[2] = {};

    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = VK_F11;

    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = VK_F11;
    inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;

    SendInput(2, inputs, sizeof(INPUT));

    Sleep(50);

    Cons.UpdateSize();
}

void DisableFullscreenToggle() 
{
    HWND hwnd = GetConsoleWindow();
    if (hwnd) 
    {
        HMENU hMenu = GetSystemMenu(hwnd, FALSE);
        if (hMenu) 
        {
            DeleteMenu(hMenu, SC_MAXIMIZE, MF_BYCOMMAND);
            DeleteMenu(hMenu, SC_SIZE, MF_BYCOMMAND);
            DeleteMenu(hMenu, SC_RESTORE, MF_BYCOMMAND);
        }

        // Отключаем обработку Alt+Enter
        DWORD mode;
        GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &mode);
        SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), mode & ~ENABLE_EXTENDED_FLAGS);
    }
}

uint8_t DayAndNightState = 0;
uint8_t MapWidth = 80;
uint8_t MapHeight = 10;
bool ShowScore = true;
bool DrawGroundState = true;
bool DrawSkylineState = true;

class DetailedMode
{
    bool Settings[4];

public:
    DetailedMode(bool Settings[4])
    {
        for (int i = 0; i < 4; i++)
        {
            this->Settings[i] = Settings[i];
        }
    }

    void StartDetailedMode(Console& console)
    {        
        std::unique_ptr<DayAndNightSwitch> p1;
        std::unique_ptr<ScoreDisplay> p2;
        std::unique_ptr<Ground> p3;
        std::unique_ptr<Skyline> p4;

        if (!Settings[0]) {
            p1 = std::make_unique<DayAndNightSwitch>();            
        }
        if (Settings[1]) {
            p2 = std::make_unique<ScoreDisplay>();            
        }
        if (Settings[2]) {
            p3 = std::make_unique<Ground>(console.GetWidth());            
        }
        if (Settings[3]) {
            p4 = std::make_unique<Skyline>(console.GetWidth());
        }

        LoadSprites(); // загружаем спрайты


        while (true)
        {
            system("cls"); // очищаем консоль
            if (DayAndNightState == 0 || DayAndNightState == 1)
            {
                system("color F0");
            }
            if (DayAndNightState == 2)
            {
                system("color 0F");
            }
            // устанавливаем начальное значения для высоты прыжка
            JumpHeight = console.GetHeight() - 29;
            uint64_t Points = 0; // инициализируем очки
            // рисуем динозавра
            cPos.X = 0; cPos.Y = JumpHeight;
            SetConsoleCursorPosition(hConsole, cPos);
            std::cout << DinoSprite;

            int CX = 0; // координата кактуса

            srand(time(0));
            // номер кактуса
            uint8_t CactusNumber = 0;
            CactusNumber = rand() % 4;

            while (true)
            {
                if (p1) p1->Switch(Points);
                if (p2) p2->DrawScore(Points, console.GetWidth());
                if (p3) p3->DrawGround(Points, console.GetWidth(), console.GetHeight());
                if (p4) p4->DrawSkyline(console.GetHeight());

                // ввод, вывод - бегущий / статичный в прыжке динозавр
                if (InputKey() || JumpState)
                {
                    if (JumpState == 1)
                    {
                        if (JumpHeight > 0)
                        {
                            JumpHeight--;
                            cPos.X = 0; cPos.Y = JumpHeight;
                            SetConsoleCursorPosition(hConsole, cPos);
                            std::cout << DinoSprite;
                        }
                        else
                        {
                            JumpState = 2;
                        }
                    }
                    else
                    {
                        if (JumpState > 10)
                        {
                            if (JumpHeight < console.GetHeight() - 29)
                            {
                                JumpHeight++;
                                cPos.X = 0; cPos.Y = JumpHeight;
                                SetConsoleCursorPosition(hConsole, cPos);
                                std::cout << DinoSprite;
                            }
                            else
                            {
                                JumpState = 0;
                            }
                        }
                        else
                        {
                            JumpState++;
                        }
                    }
                }
                else
                {
                    DrawRunningDino(Points, console.GetHeight());
                }

                // рисуем кактус
                CX = (console.GetWidth() - 9) - (Points * 2) % (console.GetWidth() - 9) - 9;
                DrawObs(CX, console.GetHeight(), Cactus[CactusNumber]);

                // увеличивание очков
                Points++;
                if (Points % ((console.GetWidth() - 9) / 2) == 0)
                {
                    CactusNumber = rand() % 4;
                }

                // возвращаем полноэкранный режим, если пользователь его изменил
                if (GetAsyncKeyState(VK_F11) & 0x8000)
                {
                    SetF11Mode(console);
                }

                // определение конца игры
                if (CX < 25 && CX > 2 && JumpHeight > 15)
                {
                    break;
                }

                // замедление текущего потока для облегчения вычислений процессора
                std::this_thread::sleep_for(std::chrono::milliseconds(4));
            }

            // информация о конце игры
            system("color F0");
            cPos.X = 22; cPos.Y = 2 + JumpHeight; SetConsoleCursorPosition(hConsole, cPos);
            SetConsoleTextAttribute(hConsole, 240);
            std::cout << "      ";
            cPos.Y++;
            SetConsoleCursorPosition(hConsole, cPos);
            std::cout << "  " << std::string(2, 219) << "  ";
            cPos.Y++;
            SetConsoleCursorPosition(hConsole, cPos);
            std::cout << "      ";
            cPos.X = (console.GetWidth() - 68) / 2; cPos.Y = console.GetHeight() / 2 - 2;
            SetConsoleCursorPosition(hConsole, cPos);

            std::ifstream in("Resources\\GameOver.txt");
            if (in.is_open())
            {
                short tmp = ' ';
                for (int i = 0; i < 5; i++)
                {
                    for (int j = 0; j < 33; j++)
                    {
                        in >> tmp;
                        if (tmp == 0)
                        {
                            std::cout << "  ";
                        }
                        if (tmp == 1)
                        {
                            std::cout << std::string(2, 219);
                        }
                        if (tmp == 2)
                        {
                            std::cout << "\t\t";
                        }
                    }
                    cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
                }
            }
            in.close();

            // подстазки для ввода
            cPos.Y++; SetConsoleCursorPosition(hConsole, cPos);
            SetConsoleTextAttribute(hConsole, 241);
            std::cout << "Enter 0 to exit application. Enter 1 to restart game. Enter 2 to go back to menu.";

            // ввод
            while (true)
            {
                if (Input())
                {
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
        }
    }
};

void OptionsMasterSettings(Console& console)
{
    SetConsoleTextAttribute(hConsole, 8);
    std::cout << "---Welcome to settings master---\n\n";
    SetConsoleTextAttribute(hConsole, 15);
    std::cout << "Enter game mode: full detailed or low detailed\n";
    SetConsoleTextAttribute(hConsole, 7);
    std::cout << "(use 'full_detailed_mode'/'FULL_DETAILED_MODE'/'fdm'/'FDM'/'true'/'TRUE'/'1' to set full detailed mode)\n";
    std::cout << "(use 'low_detailed_mode'/'LOW_DETAILED_MODE'/'ldm'/'LDM'/'false'/'FALSE'/'0' to set low detailed mode)\n";
    std::string Line;
    SetConsoleTextAttribute(hConsole, 15);
    while (true)
    {
        std::cin >> Line;
        std::string normalized;
        for (char c : Line)
        {
            normalized += std::tolower(static_cast<unsigned char>(c));
        }

        if (normalized == "full_detailed_mode" ||
            normalized == "fdm" ||
            normalized == "true" ||
            normalized == "1")
        {
            DetailedGameMode = true;
            SetConsoleTextAttribute(hConsole, 2);
            std::cout << "Set detailed game mode\n";
            break;
        }
        if (normalized == "low_detailed_mode" ||
            normalized == "ldm" ||
            normalized == "false" ||
            normalized == "0")
        {
            DetailedGameMode = false;
            SetConsoleTextAttribute(hConsole, 2);
            std::cout << "Set low detailed game mode\n";
            break;
        }
        std::cout << "Invalid input, try again\n";
    }

    if (DetailedGameMode)
    {
        std::cout << "Do you want to change day and night in detailed mode (reduces performance when turned on):\n";
        SetConsoleTextAttribute(hConsole, 7);
        std::cout << "(Use 'day_and_night_switch'/'DAY_AND_NIGHT_SWITCH'/'switch'/'SWITCH'/'0' to ON day and night switch)\n";
        std::cout << "(Use 'always_day'/'ALWAYS_DAY'/'day'/'DAY'/'1' to set day)\n";
        std::cout << "(Use 'always_night'/'ALWAYS_NIGHT'/'night'/'NIGHT'/'2' to set night)\n";
        SetConsoleTextAttribute(hConsole, 15);

        while (true)
        {
            std::cin >> Line;

            std::string normalized;
            for (char c : Line)
            {
                normalized += std::tolower(static_cast<unsigned char>(c));
            }

            if (normalized == "day_and_night_switch" ||
                normalized == "switch" ||
                normalized == "0")
            {
                DayAndNightState = 0;
                SetConsoleTextAttribute(hConsole, 2);
                std::cout << "switch day and night option at detailed mode\n";
                break;
            }
            if (normalized == "always_day" ||
                normalized == "day" ||
                normalized == "1")
            {
                DayAndNightState = 1;
                SetConsoleTextAttribute(hConsole, 2);
                std::cout << "set day option at detailed mode\n";
                break;
            }            
            if (normalized == "always_night" ||
                normalized == "night" ||
                normalized == "2")
            {
                DayAndNightState = 2;
                SetConsoleTextAttribute(hConsole, 2);
                std::cout << "set night at detailed mode\n";
                break;
            }

            std::cout << "Invalid input, try again\n";
        }

        std::cout << "Do you want to show score in detailed mode (reduces performance when turned on):\n";
        SetConsoleTextAttribute(hConsole, 7);
        std::cout << "(Use 'show_score'/'SHOW_SCORE'/'show'/'SHOW'/'score'/'SCORE'/'true'/'TRUE'/'1' to show score in detailed mode)\n";
        std::cout << "(Use 'false'/'FALSE'/'off'/'OFF'/'no'/''NO/'0' to not show score in detailed mode)\n";
        SetConsoleTextAttribute(hConsole, 15);

        while (true)
        {
            std::cin >> Line;

            std::string normalized;
            for (char c : Line)
            {
                normalized += std::tolower(static_cast<unsigned char>(c));
            }

            if (Line == "show_score" ||
                Line == "show" ||
                Line == "score" ||
                Line == "true" ||
                Line == "1")
            {
                ShowScore = true;
                SetConsoleTextAttribute(hConsole, 2);
                std::cout << "set option: show score in detailed mode\n";
                break;
            }
            if (Line == "false" ||
                Line == "off" ||
                Line == "no" ||
                Line == "0")
            {
                ShowScore = false;
                SetConsoleTextAttribute(hConsole, 2);
                std::cout << "set option: no score at detailed mode\n";
                break;
            }
            std::cout << "Invalid input, try again\n";
        }

        std::cout << "Do you want to show ground in detailed mode (reduces performance when turned on):\n";
        SetConsoleTextAttribute(hConsole, 7);
        std::cout << "(Use 'show_ground'/'SHOW_GROUND'/'show'/'SHOW'/'ground'/'GROUND'/'true'/'TRUE'/'1' to show ground in detailed mode)\n";
        std::cout << "(Use 'false'/'FALSE'/'off'/'OFF'/'no'/''NO/'0' to not show ground in detailed mode)\n";
        SetConsoleTextAttribute(hConsole, 15);

        while (true)
        {
            std::cin >> Line;

            std::string normalized;
            for (char c : Line)
            {
                normalized += std::tolower(static_cast<unsigned char>(c));
            }

            if (Line == "show_ground" ||
                Line == "show" ||
                Line == "ground" ||
                Line == "true" ||
                Line == "1")
            {
                DrawGroundState = true;
                SetConsoleTextAttribute(hConsole, 2);
                std::cout << "set option: show ground at detailed mode\n";
                break;
            }
            if (Line == "false" ||
                Line == "off" ||
                Line == "no" ||
                Line == "0")
            {
                DrawGroundState = false;
                SetConsoleTextAttribute(hConsole, 2);
                std::cout << "set option: no ground at detailed mode\n";
                break;
            }
            std::cout << "Invalid input, try again\n";
        }

        std::cout << "Do you want to show skyline in detailed mode (reduces performance when turned on):\n";
        SetConsoleTextAttribute(hConsole, 7);
        std::cout << "(Use 'show_skyline'/'SHOW_SKYLINE'/'show'/'SHOW'/'skyline'/'SKYLINE'/'true'/'TRUE'/'1' to show skyline in detailed mode)\n";
        std::cout << "(Use 'false'/'FALSE'/'off'/'OFF'/'no'/''NO/'0' to not show skyline in detailed mode)\n";
        SetConsoleTextAttribute(hConsole, 15);

        while (true)
        {
            std::cin >> Line;

            std::string normalized;
            for (char c : Line)
            {
                normalized += std::tolower(static_cast<unsigned char>(c));
            }

            if (Line == "show_skyline" ||
                Line == "show" ||
                Line == "skyline" ||
                Line == "true" ||
                Line == "1")
            {
                DrawSkylineState = true;
                SetConsoleTextAttribute(hConsole, 2);
                std::cout << "set option: show skyline at detailed mode\n";
                break;
            }
            if (Line == "false" ||
                Line == "off" ||
                Line == "no" ||
                Line == "0")
            {
                DrawSkylineState = false;
                SetConsoleTextAttribute(hConsole, 2);
                std::cout << "set option: no skyline at detailed mode\n";
                break;
            }
            std::cout << "Invalid input, try again\n";
        }        
    }
    else
    {
        SetConsoleTextAttribute(hConsole, 15);
        std::cout << "Enter map width\n";
        SetConsoleTextAttribute(hConsole, 7);
        std::cout << "(use a number from the range [80, " << console.GetWidth() - 1 << "])\n";

        while (true)
        {
            std::cin >> Line;
            try
            {
                MapWidth = std::stoi(Line);
                if (MapWidth < 80) MapWidth = 80;
                if (MapWidth > console.GetWidth() - 1) MapWidth = console.GetWidth() - 1;
                std::cout << "Set new map width = " << int(MapWidth) << "\n";
                break;
            }
            catch (const std::invalid_argument& e)
            {
                std::cout << "Error: enter only digits!\n";
            }
            catch (const std::out_of_range& e)
            {
                std::cout << "Error: the number is too big\n";
            }
        }

        SetConsoleTextAttribute(hConsole, 15);
        std::cout << "Enter map height\n";
        SetConsoleTextAttribute(hConsole, 7);
        std::cout << "(use a number from the range [10, " << console.GetHeight() - 5 << "])\n";

        while (true)
        {
            std::cin >> Line;
            try
            {
                MapHeight = std::stoi(Line);
                if (MapHeight < 10) MapHeight = 10;
                if (MapHeight > console.GetHeight() - 5) MapHeight = console.GetHeight() - 5;
                std::cout << "Set new map height = " << int(MapHeight) << "\n";
                break;
            }
            catch (const std::invalid_argument& e)
            {
                std::cout << "Error: enter only digits!\n";
            }
            catch (const std::out_of_range& e)
            {
                std::cout << "Error: the number is too big\n";
            }
        }
    }

    std::cout << "Now all settings are set, press any key to return to the menu\n";
    while (true)
    {
        if (_kbhit())
        {
            break;
        }
    }
}

void OptionsUNIX(Console& console)
{
    while (true)
    {        
        system("cls");        
        cPos.X = 0; cPos.Y = 0; SetConsoleCursorPosition(hConsole, cPos);
        SetConsoleTextAttribute(hConsole, 8);
        std::cout << "Welcome to CLI interface\n\n";
        SetConsoleTextAttribute(hConsole, 15);
        std::cout << "[1] Detailed mode: " << DetailedGameMode << " (0 - false, 1 - true)\n";
        std::cout << "[2] Day and night state switcher at detailed mode: " << int(DayAndNightState) << " (0 - on, 1 - set always day, 2 - set always night)\n";
        std::cout << "[3] Show score at detailed mode: " << ShowScore << " (0 - false, 1 - true)\n";
        std::cout << "[4] Show ground at detailed mode: " << DrawGroundState << " (0 - false, 1 - true)\n";
        std::cout << "[5] Draw skyline at detailed mode: " << DrawSkylineState << " (0 - false, 1 - true)\n";
        std::cout << "[6] Map width at low detailed mode: " << int(MapWidth) << " (number from [80, " << console.GetWidth() - 1 << ")]\n";
        std::cout << "[7] Map height at low detailed mode: " << int(MapHeight) << " (number from [10, " << console.GetHeight() - 5 << ")]\n";

        std::string Input;
        int index = 0;
        int Value;

        std::cout << "\n\nUse: index of option + keyword 'set' + value\n";
        std::cout << "Use: '0' to go back to menu\n\n";

        std::cin >> Input;
        bool CorrectInput = true;
        try
        {
            index = std::stoi(Input);
            if (index == 0) return;
            if (index < 1) index = 1;
            if (index > 7) index = 7;
        }
        catch (const std::invalid_argument& e)
        {
            std::cout << "Error: enter only digits as index!\n";
            continue;
        }
        catch (const std::out_of_range& e)
        {
            std::cout << "Error: the number in index is too big\n";
            continue;
        }
        std::cin >> Input;
        if (Input != "set")
        {
            std::cout << "Error: use keyword 'set' after index\n";
            continue;
        }
        std::cin >> Input;
        try
        {
            Value = std::stoi(Input);
        }
        catch (const std::invalid_argument& e)
        {
            std::cout << "Error: enter only digits as value!\n";
            continue;
        }
        catch (const std::out_of_range& e)
        {
            std::cout << "Error: the number in value is too big\n";
            continue;
        }

        switch (index)
        {
        case 1:
        {
            if (Value == 0) DetailedGameMode = false;
            else DetailedGameMode = true;
        }
        case 2:
        {            
            if (Value <= 0) DayAndNightState = 0;
            if (Value == 1) DayAndNightState = 1;
            if (Value >= 2) DayAndNightState = 2;        
        }
        case 3:
        {
            if (Value == 0) ShowScore = false;
            else ShowScore = true;
            break;
        }
        case 4:
        {
            if (Value == 0) DrawGroundState = false;
            else DrawGroundState = true;
            break;
        }
        case 5:
        {
            if (Value == 0) DrawSkylineState = false;
            else DrawSkylineState = true;
            break;
        }
        case 6:
        {
            if (MapWidth < 80) MapWidth = 80;
            if (MapWidth > console.GetWidth() - 1) MapWidth = console.GetWidth() - 1;
            break;
        }
        case 7:
        {
            if (MapHeight < 10) MapHeight = 10;
            if (MapHeight > console.GetHeight() - 5) MapHeight = console.GetHeight() - 5;
            break;
        }
        }
        //std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

int main(int argc, char** args) 
{
    DisableFullscreenToggle();
    Console console;
    SetF11Mode(console);    
    console.SetCursorVisible(false, 100);    
    Download download;

    string Version = "";

    thread t([&]()
        {
            Version = download.GetVersion();
        });    

    Settings[0] = DayAndNightState;
    Settings[1] = ShowScore;
    Settings[2] = DrawGroundState;
    Settings[3] = DrawSkylineState;

    cPos.X = 0; cPos.Y = 0; SetConsoleCursorPosition(hConsole, cPos);
    int currentSelection = -1;  
    DrawMenuWithSelection(currentSelection, console.GetHeight(), console.GetWidth());
    bool DrawScore = 1;

    while (true) 
    {        
        ShowCheckUpdateStatus(download.IsVersionNew(), Version);
        if (Input()) 
        {
            IsShow = false;
            if (Key >= '0' && Key <= '4') {
                currentSelection = Key - '0';
                DrawMenuWithSelection(currentSelection, console.GetHeight(), console.GetWidth());
            }
            if (currentSelection == 0 && Key == 13)
            {
                if (DetailedGameMode)
                {
                    DetailedMode dMode(Settings);
                    dMode.StartDetailedMode(console);
                }
                else
                {
                    Start(console.GetHeight(), console.GetWidth());
                }
                if (Key == '0')
                {
                    break;
                }                
                if (Key == '2')
                {
                    system("color 0F");
                    system("cls");
                    Key = ' ';                    
                    cPos.X = 0; cPos.Y = 0; SetConsoleCursorPosition(hConsole, cPos);
                    int currentSelection = -1;
                    DrawMenuWithSelection(currentSelection, console.GetHeight(), console.GetWidth());
                }
            }
            if (currentSelection == 1 && Key == 13)
            {
                console.SetCursorVisible(true, 5);
                Key = ' ';
                system("cls");           

                if (InputStyle == 0)
                {
                    OptionsMasterSettings(console);
                }
                if (InputStyle == 1)
                {
                    OptionsUNIX(console);
                }
                console.SetCursorVisible(false, 100);
                Key = ' ';
                currentSelection = -1;
                system("cls");
                Settings[0] = DayAndNightState;
                Settings[1] = ShowScore;
                Settings[2] = DrawGroundState;
                Settings[3] = DrawSkylineState;
                DrawMenuWithSelection(currentSelection, console.GetHeight(), console.GetWidth());
            }
            if (currentSelection == 2 && Key == 13)
            {
                system("cls");
                currentSelection = -1;
                Key = ' ';
                DrawInputStyleWithSelection(currentSelection, console.GetHeight(), console.GetWidth());
                while (true)
                {
                    if (Input())
                    {
                        if (Key >= '0' && Key <= '2') {
                            currentSelection = Key - '0';
                            DrawInputStyleWithSelection(currentSelection, console.GetHeight(), console.GetWidth());
                        }
                        if (currentSelection == 0 && Key == 13)
                        {
                            system("cls");
                            Key = ' ';
                            currentSelection = -1;
                            DrawMenuWithSelection(currentSelection, console.GetHeight(), console.GetWidth());
                            break;
                        }
                        if (currentSelection == 1 && Key == 13)
                        {
                            InputStyle = 0;
                            currentSelection = -1;
                            Key = ' ';
                            DrawInputStyleWithSelection(currentSelection, console.GetHeight(), console.GetWidth());                            
                        }
                        if (currentSelection == 2 && Key == 13)
                        {
                            InputStyle = 1;
                            currentSelection = -1;
                            Key = ' ';
                            DrawInputStyleWithSelection(currentSelection, console.GetHeight(), console.GetWidth());                            
                        }
                    }
                }
            }
            if (currentSelection == 3 && Key == 13)
            {
                DrawAboutProgramWithSelection(console.GetHeight(), console.GetWidth());
                system("cls");
                Key = ' ';
                currentSelection = -1;
                DrawMenuWithSelection(currentSelection, console.GetHeight(), console.GetWidth());
            }
            if (currentSelection == 4 && Key == 13)
            {
                break;
            }
            Key = ' ';            
        }
    }
        
    t.join();

    return 0;
}