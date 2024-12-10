/*
	GPF Week 6 - Maze Start Code
*/

#include <iostream>
#include <vector>
#include <time.h>
#include <stdio.h>

#include "Framework.h"
#include <fstream>
#include <string>
using namespace std;

// Screen dimensions
int gScreenWidth{ 800 };
int gScreenHeight{ 600 };

// Delay to slow things down
int gTimeDelayMS{ 100 };

// Maze size as constants
constexpr int kMazeColumnsX{ 20 };
constexpr int kMazeRowsY{ 20 };

/*
	2 Dimensional Arrays
	You can think of these as being an array of arrays
	
	The maze has kMazeColumnsX columns across (20 by default) represented by an array
	We then have kMazeRowsY (20) of these arrays, one for each row
	
	Columns and rows can be confusing so we tend to prefer working with x and y
	x is the horizontal axis (columns) across and y is the vertical axis (rows) down.

	Each single item is called a cell. 
	
	E.g. to output the value of the cell at 16 X (column 16) and 1 Y (row 1) we would write:
	cout << map[1][16] << endl; // outputs G
*/

char map[kMazeRowsY][kMazeColumnsX];
int playerPosX = 0;
int playerPosY = 0;
int enemyPosX = 18;
int enemyPosY = 18;

ofstream output;
ifstream input;

int level = 1;
string fileName = "mapFile - "+to_string(level);

void drawMaze()
{
	for (int y = 0; y < kMazeRowsY; y++)
	{
		for (int x = 0; x < kMazeColumnsX; x++)
		{
			int cellWidth = gScreenWidth / kMazeColumnsX;
			int cellHeight = gScreenHeight / kMazeRowsY;
			int cellPosX = x * cellWidth;
			int cellPosY = y * cellHeight;

			// Checks current tile and assigns a colour
			if (map[y][x] == 'W')
			{
				ChangeColour(0, 0, 0);

			}
			if (map[y][x] == '.')
			{
				ChangeColour(255, 255, 255);
			}
			if (map[y][x] == 'G')
			{
				ChangeColour(0, 255, 0);
			}
			if (map[y][x] == 'P')
			{
				ChangeColour(0, 0, 255);
			}
			if (map[y][x] == 'F')
			{
				ChangeColour(255, 0, 0);
			}
			if (map[y][x] == 'E')
			{
				ChangeColour(255, 0, 255);
			}
			DrawRectangle(cellPosX, cellPosY, cellWidth, cellHeight);
		}
	}
}

void setRandomPlayerPos()
{
	int playerStartY = 0;
	int playerStartX = 0;
	// Checks if a tile is free and randomly places the player in it
	do {
		playerStartY = rand() % 20;
		playerStartX = rand() % 20;
	} while (map[playerStartY][playerStartX] == 'W');

	map[playerStartY][playerStartX] = 'P';
}

bool playerFound()
{
	// Iterates through the maze to find the player
	for (int y = 0; y < kMazeRowsY; y++)
	{
		for (int x = 0; x < kMazeColumnsX; x++)
		{
			if (map[y][x] == 'P')
			{
				playerPosX = x;
				playerPosY = y;
				return true;
			}
		}
	} 
	return false;
}

bool canMoveThere(int x, int y)
{
	// Checks if a tile is free
	if (map[y][x] != 'W')
	{
		return true;
	}
	else
	{
		return false;
	}
}

void enemyAI() 
{
	// Picks a random cardinal direction
	int direction = rand() % 4;
	int newEnemyPosY = enemyPosY;
	int newEnemyPosX = enemyPosX;

	// Checks if the tile is free and if so moves there
	if (direction == 0 && canMoveThere(enemyPosX, enemyPosY + 1)) 
	{
		newEnemyPosY = enemyPosY + 1;
	}
	else if (direction == 1 && canMoveThere(enemyPosX + 1, enemyPosY)) 
	{ 
		newEnemyPosX = enemyPosX + 1;
	}
	else if (direction == 2 && canMoveThere(enemyPosX, enemyPosY - 1)) 
	{ 
		newEnemyPosY = enemyPosY - 1;
	}
	else if (direction == 3 && canMoveThere(enemyPosX - 1, enemyPosY)) 
	{ 
		newEnemyPosX = enemyPosX - 1;
	}

	if (map[newEnemyPosY][newEnemyPosX] == '.' || map[newEnemyPosY][newEnemyPosX] == 'P') {
		map[enemyPosY][enemyPosX] = '.';
		enemyPosX = newEnemyPosX;
		enemyPosY = newEnemyPosY;
		map[enemyPosY][enemyPosX] = 'E';
	}
}

bool loseCondition(int x, int y)
{
	// Checks if the player has collided with a firepit or an enemy, if so kills the player and closes the game
	if (map[y][x] == 'F' || map[y][x] == 'E')
	{
		cout << "You Lose!" << endl;
		return true;
	}
	return false;
}

void saveMaze(string fileName)
{
	cout << "Saving - Enter File Name" << endl;
	cin >> fileName;

	output.open(fileName+".txt");

	// Iterates through the maze saving each character to a file
	for (int y = 0; y < kMazeRowsY; y++)
	{
		for (int x = 0; x < kMazeColumnsX; x++)
		{
			output << map[y][x] << " ";
		}
		output << endl;
	}

	output.close();

	if (!output)
	{
		cerr << "file could not be opened" << endl;
	}
}

void loadMaze(string fileName)
{
	input.open(fileName+".txt");

	// Iterates through the file outputting the maze
	for (int y = 0; y < kMazeRowsY; y++)
	{
		for (int x = 0; x < kMazeColumnsX; x++)
		{
			input >> map[y][x];
		}
		output << endl;
	}

	input.close();

	if (!input)
	{
		cerr << "file could not be opened" << endl;
	}
}

bool winEvent(int x, int y)
{
	if (map[y][x] == 'G')
	{
		//float GetElapsedTime();
		// If the player touches the goal, adds one the level then loads it. Starting the next level
		level = level + 1;
		string fileName = "mapFile - " + to_string(level);
		loadMaze(fileName);

		//cout << GetElapsedTime() << endl;
		//cout << "You Win!" << endl;

		//while (map[y][x] != 'W')
		//
			//map[y = rand() % 20][x = rand() % 20];
			//map[y][x] = 'G';
			//return true;
		//}
	}
	else
	{
		return false;
	}
}

int main()
{
	srand(time(NULL));

	//setRandomPlayerPos();

	loadMaze(fileName);

	while (UpdateFramework())
	{
		playerFound();
		StartClock();

		drawMaze();

		//enemyAI();

		// Player movement
		EKeyPressed GetLastKeyPressed();
		switch (GetLastKeyPressed())
		{
		case EKeyPressed::eNone:
			break;
		case EKeyPressed::eLeft:
			if (canMoveThere(playerPosX - 1, playerPosY))
			{
				map[playerPosY][playerPosX] = '.';
				playerPosX--;
				if (loseCondition(playerPosX, playerPosY))
				{
					return 0;
				}
				if (!winEvent(playerPosX, playerPosY))
				{
					map[playerPosY][playerPosX] = 'P';
				}
			}
			break;
		case EKeyPressed::eUp:
			if (canMoveThere(playerPosX, playerPosY - 1))
			{
				map[playerPosY][playerPosX] = '.';
				playerPosY--;
				if (loseCondition(playerPosX, playerPosY))
				{
					return 0;
				}
				if (!winEvent(playerPosX, playerPosY))
				{
					map[playerPosY][playerPosX] = 'P';
				}
			}
			break;
		case EKeyPressed::eRight:
			if (canMoveThere(playerPosX + 1, playerPosY))
			{
				map[playerPosY][playerPosX] = '.';
				playerPosX++;
				if (loseCondition(playerPosX, playerPosY))
				{
					return 0;
				}
				if (!winEvent(playerPosX, playerPosY))
				{
					map[playerPosY][playerPosX] = 'P';
				}
			}
			break;
		case EKeyPressed::eDown:
			if (canMoveThere(playerPosX, playerPosY + 1))
			{
				map[playerPosY][playerPosX] = '.';
				playerPosY++;
				if (loseCondition(playerPosX, playerPosY))
				{
					return 0;
				}
				if (!winEvent(playerPosX, playerPosY))
				{
					map[playerPosY][playerPosX] = 'P';
				}
			}
			break;
			// Creating and loading save files
		case EKeyPressed::eSave:
			saveMaze(fileName);
			break;
		case EKeyPressed::eLoad:
			cout << "Loading - Enter File Name" << endl;
			cin >> fileName;
			loadMaze(fileName);
			break;
		default:
			break;
		}
	}

	return 0;
}