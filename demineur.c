// Last update: 2021-10-14 20:00
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define UNICODE
#define _UNICODE
typedef char BooL;
#define TRUE 1
#define FALSE 0

//#TODO passage par adresse pour Grid

//#TODO naming convention
typedef struct Case
{
	int x;
	int y;
	BooL is_bomb;
	BooL is_revealed;
	BooL is_flagged;
	int bomb_count;
	int value;

} Case;

typedef struct Grid
{
	int size;
	Case** cases; //tableau de pointeurs de cases (tableau de tableaux)

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

void GenerateGrid(Grid* grid) {

	//Demande de la taille de la grille
	grid->size = AskInt("Entrez la taille du tableau : ");

	//Allocation de la mémoire pour le tableau de cases 
	grid->cases = (Case**)malloc(grid->size * sizeof(Case*));
	
	if (grid->cases == NULL)
	{
		exit(1);
	}

	//Allocation de la mémoire pour chaque case
	for (int i = 0; i < grid->size; i++)
	{
		grid->cases[i] = (Case*)malloc(grid->size * sizeof(Case));

		if (grid->cases[i] == NULL)
		{
			exit(1);
		}
	}

	//Initialisation des cases
	for (int i = 0; i < grid->size; i++) {
		for (int j = 0; j < grid->size; j++) {
			grid->cases[i][j].x = i;
			grid->cases[i][j].y = j;
			grid->cases[i][j].is_bomb = FALSE;
			grid->cases[i][j].is_revealed = FALSE;
			grid->cases[i][j].is_flagged = FALSE;
			grid->cases[i][j].bomb_count = 0;
			grid->cases[i][j].value = i * grid->size + j;
		}
	}
}

Case* GetCase(Grid* pGrid, int i, int j)
{ // return le piointeur de la case si elle existe, sinon return NULL
	if (i >= 0 && i < pGrid->size && j >= 0 && j < pGrid->size)
	{
		return &pGrid->cases[i][j];
	}
	return NULL;
}

void PrintGrid(const Grid* grid, BooL show_bombs) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	for (int i = 0; i < grid->size; i++) { //lignes
		printf("\n"); //saut de ligne
		for (int j = 0; j < grid->size; j++) { //colonnes
			if (grid->cases[i][j].is_revealed == FALSE)
			{
				if (grid->cases[i][j].is_bomb == TRUE && show_bombs) {
						SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
						printf("|X |");
				}
				else if (grid->cases[i][j].is_flagged == TRUE) {
					SetConsoleTextAttribute(hConsole, 12);
					printf("|%d|", grid->cases[i][j].value);
				}
				else {
					grid->cases[i][j].value = i * grid->size + j;
					if (grid->cases[i][j].value <= 9) {
						SetConsoleTextAttribute(hConsole, 15);
						printf("|%d |", grid->cases[i][j].value);
					}
					else {
						SetConsoleTextAttribute(hConsole, 15);
						printf("|%d|", grid->cases[i][j].value);
					}
				}
			}
			else {
				if (grid->cases[i][j].is_bomb == TRUE) {
						SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
						printf("|X |");
				}
				else
				{
					//Texte des mines adjacentes
					if (grid->cases[i][j].bomb_count == 0) {
						SetConsoleTextAttribute(hConsole, 11);
						printf("|%d |", grid->cases[i][j].bomb_count);
					}
					else if (grid->cases[i][j].bomb_count <= 2) {
						SetConsoleTextAttribute(hConsole, 9);
						printf("|%d |", grid->cases[i][j].bomb_count);
					}
					else if (grid->cases[i][j].bomb_count <= 4) {
						SetConsoleTextAttribute(hConsole, 14);
						printf("|%d |", grid->cases[i][j].bomb_count);
					}
					else if (grid->cases[i][j].bomb_count <= 8) {
						SetConsoleTextAttribute(hConsole, 13);
						printf("|%d |", grid->cases[i][j].bomb_count);
					}
					else {
						SetConsoleTextAttribute(hConsole, 12);
						printf("|%d |", grid->cases[i][j].bomb_count);
					}
				}
			}
		}
	}
}

void GenerateBombs(Grid* grid) {
	int bomb_amount = pow(grid->size, 2) * 0.2;
	int size = pow(grid->size, 2);
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

		int x = tile_index / grid->size;
		int y = tile_index % grid->size;

		grid->cases[x][y].is_bomb = TRUE;

		// count
		for (int i = x - 1; i <= x + 1; i++) {
			for (int j = y - 1; j <= y + 1; j++) {
				if (GetCase(grid, i, j) != NULL) {
					if (grid->cases[i][j].is_bomb == FALSE) {
						grid->cases[i][j].bomb_count++;
					}
				}
			}
		}

		int temp = all_index[size - 1];
		all_index[size - 1] = tile_index;
		all_index[index] = temp;

		size--;
	}

	free(all_index);
}

void UpdateGrid(Grid* grid, int case_value)
{
	int x = case_value / grid->size;
	int y = case_value % grid->size;

	grid->cases[x][y].is_revealed = TRUE;

	if (grid->cases[x][y].bomb_count == 0) {
		for (int i = x - 1; i <= x + 1; i++) {
			for (int j = y - 1; j <= y + 1; j++) {
				if (GetCase(grid, i, j) != NULL) {
					if (grid->cases[i][j].is_revealed == FALSE && grid->cases[i][j].is_flagged == FALSE) {
						UpdateGrid(grid, grid->cases[i][j].value);
					}
				}
			}
		}
	}
}


int main()
{
	/*
	FILE* fptr;
	fptr = fopen("C:/Users/ikonan/Downloads/message1.txt", "r");
	fprintf(fptr, "EASTER EGG FOUND !\n");
	fclose(fptr);
	*/

	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	//Initialisation de la graine pour la fonction rand
	srand(time(NULL));

	//Initialisation de la grille
	Grid grid;
	GenerateGrid(&grid);

	//Génération des bombes
	GenerateBombs(&grid);

	//Boucle de jeu
	do {
		//Affichage du tableau
		PrintGrid(&grid, TRUE);
		SetConsoleTextAttribute(hConsole, 15);

		//Demande de la case a révéler
		int UserChoice = AskIntInterval("\nEntrez la case a reveler : ", 0, (grid.size*grid.size)-1);

		int iUser = UserChoice / grid.size;
		int jUser = UserChoice % grid.size;

		if (grid.cases[iUser][jUser].is_flagged == TRUE) {
			int RmFlag = AskIntInterval("Que voulez vous faire sur ce drapeau ? (0: Reveler, 1: Enlever le drapeau, 2: Annuler la selection", 0, 2);
			if (RmFlag == 0) {
				grid.cases[iUser][jUser].is_flagged = FALSE;
				UpdateGrid(&grid, UserChoice);
			}
			else if (RmFlag == 1) {
				grid.cases[iUser][jUser].is_flagged = FALSE;
			}
		}
		else {
			int Choice = AskIntInterval("\nQue voulez-vous faire avec cette case ? (0: Reveler, 1:Mettre un drapeau, 2:Anuler la selection)", 0, 2);
			if (Choice == 0)
			{
				UpdateGrid(&grid, UserChoice);
				if (grid.cases[iUser][jUser].is_bomb == TRUE) {
					break;
				}
				//TODO Vérification de la victoire
			}
			else if (Choice == 1) {
				grid.cases[iUser][jUser].is_flagged = TRUE;
			}
		}
		
	} while (1);
	
	system("cls");
	PrintGrid(&grid, TRUE);
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
