#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <Windows.h>
#define UNICODE
#define _UNICODE


typedef char BOOL2;
#define TRUE 1
#define FALSE 0


//def Color for Terminal writing
#define CYAN 11
#define DARKBLUE 9
#define YELLOW 14
#define PINK 13
#define RED 12
#define WHITE 15

int COLORS[] = { CYAN, DARKBLUE, DARKBLUE, YELLOW, YELLOW, PINK, PINK, RED };


typedef struct Tile
{
	int x;
	int y;
	BOOL2 IsRevealed;
	int AdjacentMines;
	BOOL2 IsMined;
	BOOL2 IsFlag;
	int Value;
} Tile;

typedef struct Grid {
	int size;
	Tile** tile;
	int remainingtiles;
	char difficulty;
} Grid;

int AskInt(const char text[]);
int GetLength(const char* str);
char AskChar(const char text[], const char ValidChar[]);
int AskIntBetween(const char text[], int Min, int Max);
BOOL2 InGrid(int i, int j, Grid* pGrid);
Tile* GetTile(Grid* pGrid, int i, int j);
void PrintGrid(Grid* pGrid, BOOL2 ShowBomb);
int GetRand(int min, int max);
int GenerateBomb(Grid* pGrid, int iFirst, int jFirst);
void InitGrid(Grid* pGrid);
int GetNeighbour(Grid* pGrid, int i, int j);
void UpdateGrid(Grid* pGrid, int i, int j);
BOOL2 CheckWin(const Grid* pGrid);
void LaunchGame(Grid* pGrid);