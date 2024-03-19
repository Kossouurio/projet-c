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



int COLORS[] = { CYAN, DARKBLUE, DARKBLUE, YELLOW, YELLOW, PINK, PINK, RED};


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

	int get_length(const char* str)
	{
		int i = 0;
		while (str[i] != '\0') {
			i++;
		}
		return i;
	}


	char AskChar(const char text[], const char ValidChar[]) {
		char charinput;

		while (1) {
			printf("%s (Reponses valides : ", text);

			printf("(");
			for (int i = 0; i < get_length(ValidChar) - 1; ++i)
			{
				printf("%c,", ValidChar[i]);
			}
			printf("%c)\n", ValidChar[get_length(ValidChar) - 1]);

			int ret = scanf_s("%c", &charinput, 1);
			while (getchar() != '\n');

			for (int i = 0; i < get_length(ValidChar); i++) {
				if (charinput == ValidChar[i]) {
					return charinput;
				}
			}

			printf("Merci de rentrer uniquement des caractères contenus dans cette liste :");
			printf("(");
			for (int i = 0; i < get_length(ValidChar) - 1; ++i)
			{
				printf("%c,", ValidChar[i]);
			}
			printf("%c)\n", ValidChar[get_length(ValidChar) - 1]);
		}
	}

	int AskIntBetween(const char text[], int Min, int Max) {
		int num;

		while (1) {


			printf("%s\n", text);
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
		int Size;
		Tile** Tile; //tablau a deux dimensions
		int remainingTiles;
		char Difficulty;
	} Grid;
 
	void PrintGrid(Grid* pGrid, BOOL2 ShowBomb) {
		system("cls");
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		for (int i = 0; i < pGrid->Size; i++) { 
			printf("\n"); 
			for (int j = 0; j < pGrid->Size; j++) {

				//Si la case est cachée
				if (pGrid->Tile[i][j].IsRevealed == FALSE)
				{
					if (pGrid->Tile[i][j].IsMined == TRUE && ShowBomb == TRUE) {
							SetConsoleTextAttribute(hConsole,FOREGROUND_RED);
							printf("|X |");
					}
					else if (pGrid->Tile[i][j].IsFlag == TRUE) {
						if (pGrid->Tile[i][j].Value <= 9) {
							SetConsoleTextAttribute(hConsole, RED);
							printf("|%d |", pGrid->Tile[i][j].Value);
						}
						else {
							SetConsoleTextAttribute(hConsole, RED);
							printf("|%d|", pGrid->Tile[i][j].Value);
						}
					}
					else {
						if (pGrid->Tile[i][j].Value <= 9) {
							SetConsoleTextAttribute(hConsole, WHITE);
							printf("|%d |", pGrid->Tile[i][j].Value);
						}
						else {
							SetConsoleTextAttribute(hConsole, WHITE);
							printf("|%d|", pGrid->Tile[i][j].Value);
						}
					}
				}
				//Si la	case est révelée alors afficher le nombre de bombes adjacentes
				else {
					SetConsoleTextAttribute(hConsole, COLORS[pGrid->Tile[i][j].AdjacentMines ]);
					printf("|%d |", pGrid->Tile[i][j].AdjacentMines);
				}
				
			}
		}
	}
	int GenerateBomb(Grid* pGrid, int iFirst, int jFirst) {
		system("cls");
		float BombRatio;
		if (pGrid->Difficulty == 'E' || pGrid->Difficulty == 'e') {
			BombRatio = 0.2;
		}
		else if (pGrid->Difficulty == 'M' || pGrid->Difficulty == 'm') {
			BombRatio = 0.35;
		}
		else {
			BombRatio = 0.4;
		}
		int bomb_amount = pow(pGrid->Size, 2) * BombRatio;
		int size = pow(pGrid->Size,2);
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

		for (int Currenti = iFirst - 1; Currenti <= iFirst + 1; Currenti++) {
			for (int Currentj = jFirst - 1; Currentj <= jFirst + 1; Currentj++) 
			{
				int RmValue = Currenti * 10 + Currentj;
				all_index[RmValue] = all_index[size - 1];
				size--;
			}
		}

		for (int i = 0; i < bomb_amount; i++)
		{
			int index = rand() % size;
			int tile_index = all_index[index];


			pGrid->Tile[tile_index / pGrid->Size][tile_index % pGrid->Size].IsMined = TRUE;

			all_index[tile_index] = all_index[size - 1];

			size--;
		}

		free(all_index);
		return bomb_amount;
	}



	BOOL2 InGrid(int i, int j,Grid* pGrid) {
		if (i < 0 || j < 0 || i >= pGrid->Size || j >= pGrid->Size) {
			return FALSE;
		}

		return TRUE;
	}

	void InitGrid(Grid* pGrid) {
		pGrid->Size = AskInt("Rentrez la taille de la grille sur laquelle vous voulez jouer : ");
		pGrid->Difficulty = AskChar("Dans quelle difficulte voulez vous jouer ? (E pour Easy, M pour Medium, H pour Hard", "EeMmHh");
		

		pGrid->Tile = (Tile**)malloc(sizeof(Tile*) * pGrid->Size);

		if (pGrid->Tile == NULL) {
			exit(1);
		}

		//Allocation de la mémoire pour chaque case
		for (int i = 0; i < pGrid->Size; i++)
		{
			pGrid->Tile[i] = (Tile*)malloc(pGrid->Size * sizeof(Tile));
			if (pGrid->Tile == NULL) {
				exit(1);
			}
		}

		//Initialisation des Tile
		for (int i = 0; i < pGrid->Size; i++)
		{
			for (int j = 0; j < pGrid->Size; j++)
			{
				pGrid->Tile[i][j].x = i;
				pGrid->Tile[i][j].y = j;
				pGrid->Tile[i][j].IsRevealed = FALSE;
				pGrid->Tile[i][j].Value = i * pGrid->Size + j;
			}
		}

	}


	int GetNeighbour(Grid* pGrid, int i, int j) {
		int Neighbour = 0;
		for (int Currenti = i - 1; Currenti <= i + 1; Currenti++) {
			for (int Currentj = j - 1; Currentj <= j + 1; Currentj++) {
				if (InGrid(Currenti, Currentj, pGrid) == TRUE) {
					if (pGrid->Tile[Currenti][Currentj].IsMined == TRUE) {
						Neighbour++;
					}
				}
			}
		}
		return Neighbour;
	}

	void UpdateGrid(Grid* pGrid, int i, int j)
	{
		pGrid->Tile[i][j].IsRevealed = TRUE;
		pGrid->remainingTiles--;
		pGrid->Tile[i][j].AdjacentMines = GetNeighbour(pGrid, i,j);

		if (GetNeighbour(pGrid, i, j) == 0) {
			for (int x = i - 1; x <= i + 1; x++) {
				for (int y = j - 1; y <= j + 1; y++) {
					if (InGrid(x, y, pGrid)) {
						if (pGrid->Tile[x][y].IsRevealed == FALSE  /* && pGrid->Tile[i][j].IsFlag == FALSE*/) {

							 UpdateGrid(pGrid, x ,y) ;

						}
					}
				}
			}
		}
	}

	BOOL2 CheckWin(Grid* pGrid) {
		if (pGrid->remainingTiles == 0) {
			return TRUE;
		}
		return FALSE;
	}

	
	void LaunchGame(Grid* pGrid, HANDLE hConsole) 
	{
		BOOL2 FirstInput = TRUE;
		while(1)
		{ 
			PrintGrid(pGrid, TRUE);

			SetConsoleTextAttribute(hConsole, WHITE);
			int UserChoice = AskIntBetween("\nChoisissez la case que vous voulez réveler", 0, pow(pGrid->Size, 2) - 1);
			int iUser = UserChoice / pGrid->Size;
			int jUser = UserChoice % pGrid->Size;

			if (pGrid->Tile[iUser][jUser].IsFlag == TRUE) {
				int RmFlag = AskIntBetween("Que voulez vous faire sur ce drapeau? (0: Annuler la sélection, 1: Réveler, 2: Enlever le drapeau", 0, 2);
				
				if (RmFlag == 0)
					continue;
				
				pGrid->Tile[iUser][jUser].IsFlag = FALSE;

				if (RmFlag == 2)
					continue;
			}
			else 
			{
				int Choice = AskIntBetween("\n Que voulez-vous faire avec cette case ? (0: Reveler, 1:Mettre un drapeau, 2:Annuler la selection)", 0, 2);
				
				if (Choice == 2)
					continue;

				if (Choice == 1) 
				{
					pGrid->Tile[iUser][jUser].IsFlag = TRUE;
					continue;
				}
			}
			
			if (FirstInput == TRUE) {
				int bomb_amount = GenerateBomb(pGrid, iUser, jUser);
				pGrid->remainingTiles = pow(pGrid->Size, 2) - bomb_amount;
				FirstInput = FALSE;
			}

			if (pGrid->Tile[iUser][jUser].IsMined == TRUE) {
				PrintGrid(pGrid, TRUE);
				SetConsoleTextAttribute(hConsole, 15);
				printf("\nVous avez perdu !");

				return;
			}

			UpdateGrid(pGrid, iUser, jUser);
			if (CheckWin(pGrid) == TRUE) {
				PrintGrid(pGrid, TRUE);
				printf("Bravo vous avez gagne !");
				return;
			}

			pGrid->Tile[iUser][jUser].AdjacentMines = GetNeighbour(pGrid, iUser, jUser);
		}
	}

	int main() {
		
		char replay = 'y';
		srand(time(NULL));

		do {
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			Grid grid;
			Grid* pGrid = &grid;
			InitGrid(pGrid);
			LaunchGame(pGrid, hConsole);
			replay = AskChar("\nVoulez vous rejouer ?", "YyNn");
		} while (replay == 'y' || replay == 'Y');
		

		return 0;
	}


