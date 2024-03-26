#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <Windows.h>
#include <SDL.h>
#define UNICODE
#define _UNICODE


#define DARK_TILE "C:/Users/tzocca/source/repos/DemineurSDL/DemineurSDL/Resources/dark/dark_green_tile.bmp"

#define DARK1 "C:/Users/tzocca/source/repos/DemineurSDL/DemineurSDL/Resources/dark/dark1.bmp"
#define DARK2 "C:/Users/tzocca/source/repos/DemineurSDL/DemineurSDL/Resources/dark/dark2.bmp"
#define DARK3 "C:/Users/tzocca/source/repos/DemineurSDL/DemineurSDL/Resources/dark/dark3.bmp"
#define DARK4 "C:/Users/tzocca/source/repos/DemineurSDL/DemineurSDL/Resources/dark/dark4.bmp"
#define DARK5 "C:/Users/tzocca/source/repos/DemineurSDL/DemineurSDL/Resources/dark/dark5.bmp"
#define DARK6 "C:/Users/tzocca/source/repos/DemineurSDL/DemineurSDL/Resources/dark/dark6.bmp"
#define DARK7 "C:/Users/tzocca/source/repos/DemineurSDL/DemineurSDL/Resources/dark/dark7.bmp"
#define DARK8 "C:/Users/tzocca/source/repos/DemineurSDL/DemineurSDL/Resources/dark/dark8.bmp"
#define BASE_DARK "C:/Users/tzocca/source/repos/DemineurSDL/DemineurSDL/Resources/dark/darkbase.bmp"


#define LIGHT_TILE "C:/Users/tzocca/source/repos/DemineurSDL/DemineurSDL/Resources/light/light_green_tile.bmp"

#define LIGHT1 "C:/Users/tzocca/source/repos/DemineurSDL/DemineurSDL/Resources/light/light1.bmp"
#define LIGHT2 "C:/Users/tzocca/source/repos/DemineurSDL/DemineurSDL/Resources/light/light2.bmp"
#define LIGHT3 "C:/Users/tzocca/source/repos/DemineurSDL/DemineurSDL/Resources/light/light3.bmp"
#define LIGHT4 "C:/Users/tzocca/source/repos/DemineurSDL/DemineurSDL/Resources/light/light4.bmp"
#define LIGHT5 "C:/Users/tzocca/source/repos/DemineurSDL/DemineurSDL/Resources/light/light5.bmp"
#define LIGHT6 "C:/Users/tzocca/source/repos/DemineurSDL/DemineurSDL/Resources/light/light6.bmp"
#define LIGHT7 "C:/Users/tzocca/source/repos/DemineurSDL/DemineurSDL/Resources/light/light7.bmp"
#define LIGHT8 "C:/Users/tzocca/source/repos/DemineurSDL/DemineurSDL/Resources/light/light8.bmp"
#define BASE_LIGHT "C:/Users/tzocca/source/repos/DemineurSDL/DemineurSDL/Resources/light/lightbase.bmp"

#define BOMB "C:/Users/tzocca/source/repos/DemineurSDL/DemineurSDL/Resources/bomb.bmp"

typedef char BOOL2;
#define TRUE 1
#define FALSE 0




typedef struct Tile
{
	int x;
	int y;
	BOOL2 IsRevealed;
	int AdjacentMines;
	BOOL2 IsMined;
	BOOL2 IsFlag;
	int Value;

	SDL_Texture* Texture;

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
SDL_Texture* Createtexture(const char* path, SDL_Renderer* renderer)