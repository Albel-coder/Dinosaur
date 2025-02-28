#include "GameModes.h"

Console ConS;
GameFeatures Variables;
COORD cPos;
HANDLE hConsole;
unsigned long long Points = 1; 
unsigned int Value = 0;
char Key = 1; 
char tmp = ' ';
char ModifiedInterface = '-';
short SelectedFunction = 10000;
short ObsNum = 0;
short Obstacles_L = 0;
short Obstacles_H = 0;
bool OBS[4][4] = { 0 };

int main()
{	
	while (Key != '4')
	{
		while (Key != '0' && Key != '4')
		{
			ConS.MinConsoleWidth = 65;
			ConS.MinConsoleHeight = 20;
			//когда пользователь выходит из вкладки, информация о ней больше не нужна
			//вызываем в нужных местах деструктор дополнительными областями видимости
			{
				Tab Menu("Menu");
				Menu.Entrance(ClearAndShowInfo);
			}
			if (Key == '1')
			{
				ConS.MinConsoleWidth = 65; 
				ConS.MinConsoleHeight = 20;
				while (Key != '0')
				{
					{
						Tab Settings("Settings");
						Settings.Entrance(ClearAndShowInfo);
					}
					if (Key == '1')
					{
						ConS.MinConsoleWidth = 65;
						ConS.MinConsoleHeight = 20;
						while (Key != '0')
						{
							{
								Tab GameOptions("GameOptions");
								GameOptions.Entrance(ClearAndShowInfo);
							}
							if (Key == '1') // quick setup
							{
								while (Key != '0')
								{
									{
										Tab QuickSetupFirst("QuickSetupFirst");
										QuickSetupFirst.Entrance(ClearAndShowInfo);
									}
								}
								Key = 1;
							}
							if (Key == '2')
							{
								ConS.MinConsoleWidth = 70;
								ConS.MinConsoleHeight = 20;
								while (Key != '0')
								{
									{
										Tab Map("Map");
										Map.Entrance(ClearAndShowInfo);
									}
									if (Key == '0')
									{
										break;
									}
									ConS.MinConsoleWidth = 70;
									ConS.MinConsoleHeight = 10;
									if (Key == '1') // size
									{										
										Tab MapSize("MapSize");
										MapSize.Entrance(MapSizeInterface);
									}
									if (Key == '2') // border
									{
										Tab MapBorder("MapBorder");
										MapBorder.Entrance(MapBorderInterface);
									}
									if (Key == '3') // alignment
									{
										while (Key != '0')
										{											
											{
												Tab MapAlignment("MapAlignment");
												MapAlignment.Entrance(ClearAndShowInfo);
												if (Key == '3')
												{
													Variables.Save();
													NotifySaves(ConS.GetConsoleHeight());
													Key = 1;
													continue;
												}
											}
											if (Key == '1')
											{
												ModifiedInterface = '1';
												Tab HorizontalAlignment("HorizontalAlignment");
												HorizontalAlignment.Entrance(HorizontalAlignmentInterface);
											}
											if (Key == '2')
											{
												ModifiedInterface = '1';
												Tab VerticalAlignment("VerticalAlignment");
												VerticalAlignment.Entrance(VerticalAlignmentInterface);
											}
											ModifiedInterface = '-';
										}
									}
									Key = 1;
								}
								Key = 1;
							}
							if (Key == '3') // color
							{
								while (Key != '0')
								{
									Tab Color("Color");
									Color.Entrance(ColorInterface);
								}
								Key = 1;
							}
							if (Key == '4') // speed
							{
								while (Key != '0')
								{
									Tab Speed("Speed");
									Speed.Entrance(SpeedInterface);
								}
								Key = 1;
							}
							if (Key == '5') // obstacles
							{
								while (Key != '0')
								{
									{
										Tab Obstacles("Obstacles");
										Obstacles.Entrance(ClearAndShowInfo);
									}
									if (Key == '1') // editor
									{
										//ModifiedInterface = '1';
										Tab ObstaclesEditor("ObstaclesEditor");
										ObstaclesEditor.Entrance(ObstaclesEditorInterface);
									}
									if (Key == '2') // random generator
									{
										Tab RandomGenerator("RandomGenerator");
										RandomGenerator.Entrance(RandomGeneratorInterface);
									}
									if (Key == '3') // list
									{
										Tab ObstaclesList("ObstaclesList");
										ObstaclesList.Entrance(ObstaclesListInterface);
									}
									ModifiedInterface = '-';
								}
								Key = 1;
							}
						}
						Key = 1;
					}
					if (Key == '6')
					{
						while (Key != '0')
						{
							Tab Keys("Keys");
							Keys.Entrance(KeysInterface);
						}
						Key = 1;
					}
				}
				Key = 1;
			}
			if (Key == '3')
			{
				ConS.MinConsoleWidth = 80; ConS.MinConsoleHeight = 20;
				while (Key != '0')
				{
					{
						Tab AboutProgram("AboutProgram");
						AboutProgram.Entrance(ClearAndShowInfo);
					}
				}
				Key = 1;
			}
		}

		if (Key == '0')
		{
			system("cls");
			ConS.ConsoleCursorVisible(false, 100);
			system("color F0");
			FullDetailedDinosaur Dinosaur("Full");

			while (true)
			{
				Dinosaur.Draw();
			}
		}
		else
		{
			break;
		}
	}

	return 0;
}