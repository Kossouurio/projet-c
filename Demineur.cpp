#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <Windows.h>

typedef char BOOL2;
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

	int AskIntBetween(const char text[], int Min, int Max) {
		int num;

		while (1) {


			printf("%s\n", text, Min, Max);
			int ret = scanf_s("%d", &num);
			while (getchar() != '\n');


			if (num < Min || num > Max) {
				ret = 0;
			}

			if (ret == 1) {
				break;
			}
			printf("Merci de rentrer uniquement des nombres entre %d et %d\n", Min, Max);
		}
		return num;
	}


	typedef struct
	{
		int x;
		int y;
		BOOL2 IsRevealed;
		int AdjacentMines;
		BOOL2 IsMined;
		BOOL2 IsFlag;
		int Value;
	} Tile;

	typedef struct {
		int Size;
		Tile** Tile; //tablau a deux dimensions
	} Grid;
 
	void PrintGrid(Grid grid) {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		for (int i = 0; i < grid.Size; i++) { //lignes
			printf("\n"); //saut de ligne
			for (int j = 0; j < grid.Size; j++) { //colonnes
				if (grid.Tile[i][j].IsRevealed == FALSE)
				{
					if (grid.Tile[i][j].IsMined == TRUE  /* && grid.Tile[i][j].IsRevealed == TRUE */ ) {
						if (grid.Tile[i][j].Value <= 9) {
							SetConsoleTextAttribute(hConsole,FOREGROUND_RED);
							printf("|X |");
						}
						else {
							SetConsoleTextAttribute(hConsole,FOREGROUND_RED);
							printf("|X|");
						}
					}
					else if (grid.Tile[i][j].IsFlag == TRUE) {
						SetConsoleTextAttribute(hConsole, 12);
						printf("|F%d |", grid.Tile[i][j].Value);
					}
					else {
						grid.Tile[i][j].Value = i * grid.Size + j;
						if (grid.Tile[i][j].Value <= 9) {
							SetConsoleTextAttribute(hConsole, 15);
							printf("|%d |", grid.Tile[i][j].Value);
						}
						else {
							SetConsoleTextAttribute(hConsole, 15);
							printf("|%d|", grid.Tile[i][j].Value);
						}
					}
				}
				else {

					//Texte des mines adjacentes
					if (grid.Tile[i][j].AdjacentMines == 0) {
						SetConsoleTextAttribute(hConsole, 11);
						printf("|%d |", grid.Tile[i][j].AdjacentMines);
					}
					else if (grid.Tile[i][j].AdjacentMines <= 2) {
						SetConsoleTextAttribute(hConsole, 9);
						printf("|%d |", grid.Tile[i][j].AdjacentMines);
					}
					else if (grid.Tile[i][j].AdjacentMines <= 4) {
						SetConsoleTextAttribute(hConsole, 14);
						printf("|%d |", grid.Tile[i][j].AdjacentMines);
					} 
					else if (grid.Tile[i][j].AdjacentMines <= 6) {
						SetConsoleTextAttribute(hConsole, 13);
						printf("|%d |", grid.Tile[i][j].AdjacentMines);
					}
					else {
						SetConsoleTextAttribute(hConsole, 12);
						printf("|%d |", grid.Tile[i][j].AdjacentMines);
					}
				}
				
			}
		}
	}
	void GenerateBomb(Grid grid) {
		int bomb_amount = pow(grid.Size, 2) * 0.2;
		int size = pow(grid.Size,2);
		int* all_index = (int*)malloc(sizeof(int) * size);
		if (all_index == NULL) 
		{
			exit(1);
		}

		int index1 = 0;
		for (int i = 0; i < size; i++) {
				all_index[index1] = i;
				index1++;
			}

		for (int i = 0; i < bomb_amount; i++)
		{
			int index = rand() % size;
			int tile_index = all_index[index];


			grid.Tile[tile_index / grid.Size][tile_index % grid.Size].IsMined = TRUE;

			int temp = all_index[size - 1];
			all_index[size - 1] = tile_index;
			all_index[index] = temp;

			size--;
		}

		free(all_index);
	}

	BOOL2 InGrid(int i, int j,Grid grid) {
		if (i < 0 || j < 0 || i >= grid.Size || j >= grid.Size ) {
			return FALSE;
		}

		return TRUE;
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

	void UpdateGrid(Grid grid, int x, int y)
	{
		grid.Tile[x][y].IsRevealed = TRUE;

		if (grid.Tile[x][y].AdjacentMines == 0) {
			for (int i = x / grid.Size - 1; i <= x / grid.Size + 1; i++) {
				for (int j = y % grid.Size - 1; j <= y % grid.Size + 1; j++) {
					if (i >= 0 && i < grid.Size && j >= 0 && j < grid.Size) {
						if (grid.Tile[i][j].IsRevealed == FALSE) {
							UpdateGrid(grid, i,j);
						}
					}
				}
			}
		}
	}

	int GetNeighbour(Grid grid, int i, int j) {
		int Neighbour = 0;
		for (int Currenti = i - 1; Currenti <= i + 1; Currenti++) {
			for (int Currentj = j - 1; Currentj <= j + 1; Currentj++) {
				if (InGrid(Currenti, Currentj, grid) == TRUE) {
					if (grid.Tile[Currenti][Currentj].IsMined == TRUE) {
						Neighbour++;
					}
				}
			}
		}
		return Neighbour;
	}

	int main() {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		srand(time(NULL));
		Grid grid = GenerateGrid();

		GenerateBomb(grid);
		do {
			PrintGrid(grid);

			SetConsoleTextAttribute(hConsole, 15);
			int UserChoice = AskInt("\nChoisissez la case que vous voulez réveler");
			int iUser = UserChoice / grid.Size;
			int jUser = UserChoice % grid.Size;
			
			if (grid.Tile[iUser][jUser].IsFlag == TRUE) {
				int RmFlag = AskIntBetween("Que voulez vous faire sur ce drapeau? (0: Annuler la sélection, 1: Réveler, 2: Enlever le drapeau", 0, 2);
				if (RmFlag == 1) {
					grid.Tile[iUser][jUser].IsFlag = FALSE;
					UpdateGrid(grid, iUser, jUser);
					grid.Tile[iUser][jUser].AdjacentMines = GetNeighbour(grid, iUser, jUser);
				}
				else if (RmFlag == 2) {
					grid.Tile[iUser][jUser].IsFlag = FALSE;
				}
			}
			else {
				int Choice = AskIntBetween("\n Que voulez-vous faire avec cette case ? (0: Reveler, 1:Mettre un drapeau, 2:Anuler la selection)", 0, 2);

				if (Choice == 0)
				{
					if (grid.Tile[iUser][jUser].IsMined == TRUE) {
						break;
					}
					UpdateGrid(grid, iUser, jUser);
					grid.Tile[iUser][jUser].AdjacentMines = GetNeighbour(grid, iUser, jUser);
				}
				else if (Choice == 1) {
					grid.Tile[iUser][jUser].IsFlag = TRUE;
				}
			}
			
			

		} while (1);
		
		system("cls");
		PrintGrid(grid);
		printf("\nVous avez perdu !");


		return 0;
	}


}
