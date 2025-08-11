#include "DetailedMode.h"

ScoreDisplay::ScoreDisplay()
{
    for (int i = 0; i < 10; i++)
    {
        std::string FilePath = "Resources\\" + std::to_string(i) + ".txt";
        std::ifstream in(FilePath);
        if (in.is_open())
        {
            std::vector<std::string> sprite;
            std::string line;
            int tmp;
            for (int j = 0; j < 8; j++)
            {
                for (int k = 0; k < 8; k++)
                {
                    in >> tmp;
                    if (tmp == 0)
                    {
                        line += "  ";
                    }
                    if (tmp == 1)
                    {
                        line += std::string(2, 219);
                    }
                    if (tmp == 2)
                    {
                        line += "   ";
                    }
                }
                sprite.push_back(line);
                line.clear();
            }
            DigitSprites[i] = sprite;
            sprite.clear();
        }
        in.close();
    }
}

void ScoreDisplay::DrawScore(uint64_t Score, uint8_t ConsoleWidth)
{
    std::string ScoreStr = std::to_string(Score);
    std::string OutputLine;

    while (ScoreStr.length() < 7)
    {
        ScoreStr = "0" + ScoreStr;
    }

    for (int line = 0; line < 8; line++)
    {
        cPos.X = ConsoleWidth - ScoreStr.length() * 17; cPos.Y = 1 + line; SetConsoleCursorPosition(hConsole, cPos);
        for (char c : ScoreStr)
        {
            int digit = c - '0';
            OutputLine += DigitSprites[digit][line];
        }
        std::cout << OutputLine;
        OutputLine.clear();
    }
}

void DayAndNightSwitch::Switch(uint64_t Score)
{
    static int lastState = -1;
    int currentState = Score % 900;

    if (currentState > 465 && currentState < 471 && lastState != 1)
    {
        system("color 7F");
        SetConsoleTextAttribute(hConsole, 112);
        lastState = 1;
    }
    if (currentState > 882 && currentState < 888 && lastState != 2)
    {
        system("color 8F");
        SetConsoleTextAttribute(hConsole, 135);
        lastState = 2;
    }
    if (currentState == 480 && lastState != 3)
    {
        system("color 0F");
        SetConsoleTextAttribute(hConsole, 271);
        lastState = 3;
    }
    if (currentState == 897 && lastState != 4)
    {
        system("color F0");
        SetConsoleTextAttribute(hConsole, 240);
        lastState = 4;
    }
}

Ground::Ground(uint8_t ConsoleWidth)
{
    srand(time(0));

    for (int i = 0; i < 30; i++)
    {
        GroundCoords[0][i] = rand() % ConsoleWidth - 2;
        GroundCoords[1][i] = rand() % 6;
    }
}

void Ground::DrawGround(uint64_t Score, uint8_t ConsoleWidth, uint8_t ConsoleHeight)
{
    for (int i = 0; i < 30; i++)
    {
        cPos.X = GroundCoords[0][i]; cPos.Y = ConsoleHeight - 8 + GroundCoords[1][i];
        SetConsoleCursorPosition(hConsole, cPos);
        std::cout << "_  ";
    }
    for (int i = 0; i < 30; i++)
    {
        GroundCoords[0][i] -= 2;
        if (GroundCoords[0][i] <= 0 || GroundCoords[0][i] > ConsoleWidth - 2)
        {
            cPos.X = 0; cPos.Y = ConsoleHeight - 8 + GroundCoords[1][i];
            SetConsoleCursorPosition(hConsole, cPos);
            std::cout << "    ";
            GroundCoords[0][i] = ConsoleWidth - 2;
        }
    }
}

Skyline::Skyline(uint8_t ConsoleWidth)
{
    for (int i = 0; i < ConsoleWidth - 1; i++)
    {
        Line.push_back('_');
    }
}

void Skyline::DrawSkyline(uint8_t ConsoleHeight)
{
    cPos.X = 0; cPos.Y = ConsoleHeight - 9;
    SetConsoleCursorPosition(hConsole, cPos);
    std::cout << Line;
}

void LoadSprites()
{
    for (int i = 0; i < 4; i++)
    {
        Cactus[i].clear();
    }
    for (int i = 1; i < 5; i++)
    {
        std::string FilePath = "Resources\\Cactus" + std::to_string(i) + ".txt";
        std::ifstream in(FilePath);
        if (in.is_open())
        {
            std::vector<std::string> Sprite;
            std::string line;
            int tmp;
            for (int i = 0; i < 8; i++)
            {
                for (int j = 0; j < 9; j++)
                {
                    in >> tmp;
                    if (tmp == 0)
                    {
                        line += "  ";
                    }
                    if (tmp == 1)
                    {
                        line += std::string(2, 219);
                    }
                }
                Sprite.push_back(line);
                line.clear();
            }
            Cactus[i - 1] = Sprite;
        }
        in.close();
    }

    DinoLeg2Sprite.clear();
    {
        std::ifstream in("Resources\\DinoLeg2.txt");
        if (in.is_open())
        {
            std::string line;
            int tmp;
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 8; j++)
                {
                    in >> tmp;
                    if (tmp == 0)
                    {
                        line += "  ";
                    }
                    if (tmp == 1)
                    {
                        line += std::string(2, 219);
                    }
                    if (tmp == 2)
                    {
                        line += "\t  ";
                    }
                }
                DinoLeg2Sprite += line + "\n";
                line.clear();
            }
        }
        in.close();
    }

    DinoLeg1Sprite.clear();
    {
        std::ifstream in("Resources\\DinoLeg1.txt");
        if (in.is_open())
        {
            std::string line;
            int tmp;
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 8; j++)
                {
                    in >> tmp;
                    if (tmp == 0)
                    {
                        line += "  ";
                    }
                    if (tmp == 1)
                    {
                        line += std::string(2, 219);
                    }
                    if (tmp == 2)
                    {
                        line += "\t  ";
                    }
                }
                DinoLeg1Sprite += line + "\n";
                line.clear();
            }
        }
        in.close();
    }

    DinoSprite.clear();
    std::ifstream in("Resources\\Dino.txt");
    if (in.is_open())
    {
        std::string line;
        int tmp;
        for (int i = 0; i < 26; i++)
        {
            for (int j = 0; j < 20; j++)
            {
                in >> tmp;
                if (tmp == 0)
                {
                    line += "  ";
                }
                if (tmp == 1)
                {
                    line += std::string(2, 219);
                }
            }
            DinoSprite += line + "\n";
            line.clear();
        }
    }
    in.close();
}

void DrawObs(int X, int ConsoleHeight, std::vector<std::string> Sprite)
{
    if (X < 0)
    {
        if (X > -18)
        {
            for (int line = 0; line < 8; line++)
            {
                cPos.X = 0; cPos.Y = ConsoleHeight - 15 + line;
                SetConsoleCursorPosition(hConsole, cPos);
                for (int i = -X * 2; i < 18; i++)
                {
                    std::cout << Sprite[line][i];
                }
            }
        }
        else
        {
            return;
        }
    }
    else
    {
        for (int line = 0; line < 8; line++)
        {
            cPos.X = X; cPos.Y = ConsoleHeight - 15 + line;
            SetConsoleCursorPosition(hConsole, cPos);
            std::cout << Sprite[line];
        }
    }
}

void JumpingDino(int Y)
{
    cPos.X = 0; cPos.Y = Y; SetConsoleCursorPosition(hConsole, cPos);
    std::cout << DinoSprite;
}

void DrawRunningDino(uint64_t points, uint8_t ConsoleHeight)
{
    cPos.X = 0; cPos.Y = ConsoleHeight - 10; SetConsoleCursorPosition(hConsole, cPos);
    if (points % 2)
    {
        std::cout << DinoLeg1Sprite;
    }
    else
    {
        std::cout << DinoLeg2Sprite;
    }
}