#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

typedef char BooL;
#define TRUE 1
#define FALSE 0

typedef struct Tile
{
	int x;
	int y;
	BooL isBomb;
	BooL isRevealed;
	BooL isFlagged;
	int bombCount;
	int Value;

} Tile;

typedef struct Grid
{
	int size;
	Tile** cases; //tableau de pointeurs de cases (tableau de tableaux)

} Grid;

int AskInt(const char text[])
{
	int number = 0;

	while (1)
	{
		printf("%s\n", text);

		if (scanf_s("%d", &number) == 1)
			break;
		printf("Erreur de saisie, veuillez recommencer.\n\n");
		while (getchar() != '\n');
	}
	return number;
}

int AskIntInterval(const char text[], int MIN, int MAX)
{
	int number = 0;

	do
	{
		printf("%s\n", text);

		if (scanf_s("%d", &number) == 1 && number >= MIN && number <= MAX) {
			break;
		}
		printf("Erreur de saisie, veuillez recommencer.\n\n");
		while (getchar() != '\n');

	} while (1);

	return number;
}

Grid GenerateGrid() {
	Grid grid;

	//Demande de la taille de la grille
	grid.size = AskInt("Entrez la taille du tableau : ");

	//Allocation de la mémoire pour le tableau de cases 
	grid.cases = (Tile**)malloc(grid.size * sizeof(Tile*));

	//Allocation de la mémoire pour chaque case
	for (int i = 0; i < grid.size; i++)
	{
		grid.cases[i] = (Tile*)malloc(grid.size * sizeof(Tile));
	}

	//Initialisation des cases
	for (int i = 0; i < grid.size; i++) {
		for (int j = 0; j < grid.size; j++) {
			grid.cases[i][j].x = i;
			grid.cases[i][j].y = j;
			grid.cases[i][j].isBomb = FALSE;
			grid.cases[i][j].isRevealed = FALSE;
			grid.cases[i][j].isFlagged = FALSE;
			grid.cases[i][j].bombCount = 0;
			grid.cases[i][j].Value = i * grid.size + j;
		}
	}

	return grid;
}

void PrintGrid(Grid grid) {
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	system("cls");
	//Affichage du tableau
	for (int i = 0; i < grid.size ; i++)
	{
		for (int j = 0; j < grid.size ; j++)
		{
			if (grid.cases[i][j].isRevealed == TRUE) {
				if (grid.cases[i][j].isBomb == TRUE) {
					SetConsoleTextAttribute(hConsole, 12);
					printf("X ");
				}
				else {
					SetConsoleTextAttribute(hConsole, 11);
					printf("%d ", grid.cases[i][j].bombCount);
				}
			}
			else {
				if (grid.cases[i][j].isFlagged == TRUE) {
					SetConsoleTextAttribute(hConsole, 10);
					printf("F ");
				}
				else {
					SetConsoleTextAttribute(hConsole, 15);
					printf("%d ", grid.cases[i][j].Value);
				}
			}
		}
		printf("\n");
	}
}

void GenerateBombs(Grid grid) {
	for (int i = 0; i < grid.size; i++) {
		for (int j = 0; j < grid.size; j++) {
			int numb = (int)rand() % 100;
			if (numb < 20) {
				grid.cases[i][j].isBomb = TRUE;
			}
		}
	}
}

void CountBombs(Grid grid) {
	for (int x = 0; x < grid.size; x++) {
		for (int y = 0; y < grid.size; y++) {

			int count = 0;
			for (int i = x - 1; i <= x + 1; i++) {
				for (int j = y - 1; j <= y + 1; j++) {

					if (i >= 0 && i < grid.size && j >= 0 && j < grid.size) {
						if (grid.cases[i][j].isBomb == TRUE) {
							count++;
						}
					}
				}
			}
			grid.cases[x][y].bombCount = count;

		}
	}
}

void UpdateGrid(Grid grid, int casesValue)
{
	grid.cases[casesValue / grid.size][casesValue % grid.size].isRevealed = TRUE;

	if (grid.cases[casesValue / grid.size][casesValue % grid.size].bombCount == 0) {
		for (int i = casesValue / grid.size - 1; i <= casesValue / grid.size + 1; i++) {
			for (int j = casesValue % grid.size - 1; j <= casesValue % grid.size + 1; j++) {
				if (i >= 0 && i < grid.size && j >= 0 && j < grid.size) {
					if (grid.cases[i][j].isRevealed == FALSE) {
						UpdateGrid(grid, grid.cases[i][j].Value);
					}
				}
			}
		}
	}
}


int main()
{
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	//Initialisation de la graine pour la fonction rand
	srand(time(NULL));

	//Initialisation de la grille
	Grid grid = GenerateGrid();

	//Génération des bombes
	GenerateBombs(grid);

	//Comptage des bombes
	CountBombs(grid);

	//Boucle de jeu
	do {
		//Affichage du tableau
		PrintGrid(grid);
		SetConsoleTextAttribute(hConsole, 15);

		//Demande de la case a révéler
		int a;
		a = AskIntInterval("\nEntrez la case a reveler : ", 0, (grid.size*grid.size)-1);
		UpdateGrid(grid, a); 
		if (grid.cases[a / grid.size][a % grid.size].isBomb == TRUE) {
			break;
		}
		
	} while (1);
	
	system("cls");
	PrintGrid(grid);
	SetConsoleTextAttribute(hConsole, 15);
	printf("\nVous avez perdu !\n");

	//Libération de la mémoire
	for (int i = 0; i < grid.size; i++)
	{
		free(grid.cases[i]);
	}
	free(grid.cases);
	
	return 0;
}