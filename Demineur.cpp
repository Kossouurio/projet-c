#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef char BOOL;
#define TRUE 1
#define FALSE 0

namespace Demineur {
	int AskInt(const char text[]) {
		int num;


		while (1) {


			printf("%s\n", text);
			int ret = scanf_s("%d", &num);
			while (getchar() != '\n');

			if (ret == 1) {
				break;
			}
			printf("Merci de rentrer uniquement des nombres.\n");
		}
		return num;
	}


	typedef struct
	{
		int x;
		int y;
		BOOL IsRevealed;
		int AdjacentMines;
		BOOL IsMined;
		BOOL IsFlag;
		int Value;
	} Tile;

	typedef struct {
		int Size;
		Tile** Tile; //tablau a deux dimensions
	} Grid;
 
	void PrintGrid(Grid grid) {
		for (int i = 0; i < grid.Size; i++) { //lignes
			printf("\n"); //saut de ligne
			for (int j = 1; j < grid.Size +1; j++) { //colonnes
				if (grid.Tile[i][j].IsRevealed == FALSE)
				{
					if (grid.Tile[i][j].IsMined == TRUE && grid.Tile[i][j].IsRevealed == TRUE) {
						printf("|X|");
					}
					else {
						grid.Tile[i][j].Value = i * grid.Size + j;
						printf("|%d|", grid.Tile[i][j].Value);
					}
				}
				else {
					printf("|%d|", grid.Tile[i][j].AdjacentMines);
				}
				
			}
		}
	}

	void GenerateBomb(Grid grid) {
		for (int i = 0; i < grid.Size; i++) { //lignes
			for (int j = 1; j < grid.Size + 1; j++) { //colonnes
				int num = (int)rand() % 100;
				if (num < 20) {
					grid.Tile[i][j].IsMined = TRUE;
				}
				else {
					grid.Tile[i][j].IsMined = FALSE;
				}
			}
		}
	}

	Grid GenerateGrid() {
		Grid grid;
		grid.Size = AskInt("Rentrez la taille de la grille sur laquelle vous voulez jouer : ");

		grid.Tile = (Tile**)malloc(sizeof(Tile*) * grid.Size);

		//Allocation de la mémoire pour chaque case
		for (int i = 0; i < grid.Size; i++)
		{
			grid.Tile[i] = (Tile*)malloc(grid.Size * sizeof(Tile));
		}

		//Initialisation des Tile
		for (int i = 0; i < grid.Size; i++)
		{
			for (int j = 0; j < grid.Size; j++)
			{
				grid.Tile[i][j].x = i;
				grid.Tile[i][j].y = j;
				grid.Tile[i][j].IsRevealed = FALSE;
			}
		}
		return grid;
	}

	int GetNeighbour(Grid grid, int UserChoice) {
		int Neighbour = 0;
		for (int i = (UserChoice - 1) / grid.Size - 1; i < 3; i++) {
			for (int j = UserChoice % grid.Size - 1; i < 3; i++) {
				if (grid.Tile[i][j].IsMined == TRUE) {
					Neighbour++;
				}
			}
		}
		return Neighbour;
	}

	int main() {
		srand(time(NULL));
		Grid grid = GenerateGrid();

		GenerateBomb(grid);
		PrintGrid(grid);

		int UserChoice = AskInt("Choisissez la case que vous voulez réveler");
		grid.Tile[(UserChoice-1) / grid.Size][UserChoice % grid.Size].IsRevealed = TRUE;
		//grid.Tile[(UserChoice - 1) / grid.Size][UserChoice % grid.Size].AdjacentMines = GetNeighbour(grid, UserChoice);

		PrintGrid(grid);


		return 0;
	}


}
