#include "Demineur.h"



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

	int GetLength(const char* str)
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
			for (int i = 0; i < GetLength(ValidChar) - 1; ++i)
			{
				printf("%c,", ValidChar[i]);
			}
			printf("%c)\n", ValidChar[GetLength(ValidChar) - 1]);

			int ret = scanf_s("%c", &charinput, 1);
			while (getchar() != '\n');

			for (int i = 0; i < GetLength(ValidChar); i++) {
				if (charinput == ValidChar[i]) {
					return charinput;
				}
			}

			printf("Merci de rentrer uniquement des caractères contenus dans cette liste :");
			printf("(");
			for (int i = 0; i < GetLength(ValidChar) - 1; ++i)
			{
				printf("%c,", ValidChar[i]);
			}
			printf("%c)\n", ValidChar[GetLength(ValidChar) - 1]);
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

 
	BOOL2 InGrid(int i, int j, Grid* pGrid) {
		if (i < 0 || j < 0 || i >= pGrid->size || j >= pGrid->size) {
			return FALSE;
		}

		return TRUE;
	}

	Tile* GetTile(Grid* pGrid, int i, int j) {
		if (InGrid(i, j, pGrid)) {
			return &pGrid->tile[i][j];
		}
		return NULL;
	}

	void PrintGrid(Grid* pGrid, BOOL2 ShowBomb) {
		system("cls");
		BOOL2 IsEight = FALSE;
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		for (int i = 0; i < pGrid->size; i++) { 
			printf("\n"); 
			for (int j = 0; j < pGrid->size; j++) {
				Tile* Currenttile = GetTile(pGrid, i, j);
				if (Currenttile == NULL) {
					continue;
				}

				//Si la case est cachée
				if (Currenttile->IsRevealed == FALSE)
				{
					if (Currenttile->IsMined == TRUE && ShowBomb == TRUE) {
						SetConsoleTextAttribute(hConsole,FOREGROUND_RED);
						printf("| X |");
					}
					else if (Currenttile->IsFlag == TRUE) {
						SetConsoleTextAttribute(hConsole, RED);
						printf("|%3d|", pGrid->tile[i][j].Value);
					}
					else {
						SetConsoleTextAttribute(hConsole, WHITE);
						printf("|%3d|", pGrid->tile[i][j].Value);	
					}
				}
				//Si la	case est révelée alors afficher le nombre de bombes adjacentes
				else if (Currenttile->AdjacentMines <= 7){
					SetConsoleTextAttribute(hConsole, COLORS[Currenttile->AdjacentMines ]);
					printf("|%d  |", Currenttile->AdjacentMines);
				}
				else {
					SetConsoleTextAttribute(hConsole, 75);
					printf("|%d  |", Currenttile->AdjacentMines);
					IsEight = TRUE;

				}
				
			}
		}
		if (IsEight == TRUE) {
			FILE* fptr = NULL;
			SetConsoleTextAttribute(hConsole, WHITE);
			int success = fopen_s(&fptr, "C:/Users/tzocca/source/repos/Plusoumoins/Plusoumoins/message.txt", "r");
			if (success == 0) {
				printf("\n");
				while (1)
				{
					char c = fgetc(fptr);
					if (c == EOF)
						break;

					printf("%c", c);
				}
				fclose(fptr);
			}
		}
	}

	int GetRand(int min, int max) {
		if (max - min == 0) {
			return min;
		}
		else {
			return rand() % (max - min) + min;
		}
	}

	int GenerateBomb(Grid* pGrid, int x, int y) {
		system("cls");
		float BombRatio;
		if (pGrid->difficulty == 'E' || pGrid->difficulty == 'e') {
			BombRatio = 0.2;
		}
		else if (pGrid->difficulty == 'M' || pGrid->difficulty == 'm') {
			BombRatio = 0.25;
		}
		else {
			BombRatio = 0.3;
		}
		int bomb_amount = pGrid->size * pGrid->size * BombRatio;
		int size = pGrid->size * pGrid->size;
		int MinOffset = size / 60;
		int MaxOffset = size / 50;
		int* all_index = (int*)malloc(sizeof(int) * size);
		if (all_index == NULL) 
		{
			exit(1);
		}

		int index1 = 0;
		for (int i = 0; i < size; i++) 
		{
			all_index[index1] = i;
			index1++;
		}


		int offsetX =  GetRand(MinOffset, MaxOffset);
		int offsetY =  GetRand(MinOffset, MaxOffset);

		for (int CurrentX = x - 1; CurrentX <= x + 1 + offsetX; CurrentX++) {
			for (int CurrentY = y - 1; CurrentY <= y + 1 + offsetY; CurrentY++)
			{
				Tile* tile = GetTile(pGrid, CurrentX, CurrentY);
				if (tile == NULL)
					continue;

				int RmValue = CurrentY * pGrid->size + CurrentX;

				all_index[RmValue] = all_index[size - 1];
				size--;
			}
		}

		for (int i = 0; i < bomb_amount; i++)
		{
			int random_index = rand() % size;
			int tile_index = all_index[random_index];

			int y = tile_index / pGrid->size;
			int x = tile_index % pGrid->size;

			Tile* tile = GetTile(pGrid, x, y);

			tile->IsMined = TRUE;

			for (int CurrentY = y - 1; CurrentY <= y + 1; CurrentY++) {
				for (int CurrentX = x - 1; CurrentX <= x + 1; CurrentX++) {
					Tile* tile = GetTile(pGrid, CurrentX, CurrentY);
					if (tile != NULL) {
						tile->AdjacentMines += 1;

					}
				}
			}

			all_index[random_index] = all_index[size - 1];

			size--;
		}

		free(all_index);

		return bomb_amount;
	}

	void InitGrid(Grid* pGrid, SDL_Texture** LightTab, SDL_Texture** DarkTab) {
		pGrid->size = AskIntBetween("Rentrez la taille de la grille sur laquelle vous voulez jouer : ",5,min(WIDTH,HEIGHT)/32);
		pGrid->difficulty = AskChar("Dans quelle difficulte voulez vous jouer ? (E pour Easy, M pour Medium, H pour Hard", "EeMmHh");
		

		pGrid->tile = (Tile**)malloc(sizeof(Tile*) * pGrid->size);

		if (pGrid->tile == NULL) {
			exit(1);
		}

		//Allocation de la mémoire pour chaque case
		for (int i = 0; i < pGrid->size; i++)
		{
			pGrid->tile[i] = (Tile*)malloc(pGrid->size * sizeof(Tile));
			if (pGrid->tile == NULL) {
				exit(1);
			}
		}

		//Initialisation des Tile
		for (int i = 0; i < pGrid->size; i++)
		{
			for (int j = 0; j < pGrid->size; j++)
			{
				Tile* tile = GetTile(pGrid, i, j);

				tile->x = i;
				tile->y = j;
				tile->IsRevealed = FALSE;
				tile->Value = i * pGrid->size + j;
				tile->IsMined = FALSE;
				tile->IsFlag = FALSE;
				tile->AdjacentMines = 0;
				if ((i + j) % 2 == 0) 
				{
					tile->Texture = LightTab[9];
				}
				else {
					tile->Texture = DarkTab[9];
				}
			}
		}
	}

	

	Grid Init8Grid(Grid* pGrid, SDL_Texture** LightTab, SDL_Texture** DarkTab) {
		InitGrid(pGrid, LightTab, DarkTab);
		int randomint = rand() % pGrid->size;
		for (int i = 0; i < pGrid->size; i++)
		{
			for (int j = 0; j < pGrid->size; j++)
			{
				if (i != randomint || j != randomint) {
					Tile* tile = GetTile(pGrid, i, j);
					tile->IsMined = TRUE;
				}
			}
		}
	}


	void UpdateGrid(Grid* pGrid, int i, int j)
	{
		Tile* tile = GetTile(pGrid, i, j);
		if (tile == NULL)
			return;

		if (tile->IsRevealed)
			return;


		tile->IsRevealed = TRUE;
		pGrid->remainingtiles--;

		
		if (tile->AdjacentMines == 8) {
				EasterEgg();
		}

		if (tile->AdjacentMines > 0)
			return;

		for (int x = i - 1; x <= i + 1; x++) {
			for (int y = j - 1; y <= j + 1; y++) {
				UpdateGrid(pGrid, x, y);
			}
		}
	}

	BOOL2 CheckWin(const Grid* pGrid) {
		if (pGrid->remainingtiles == 0) {
			return TRUE;
		}
		return FALSE;
	}



	//--------------------------------------DEBUG FONCTION--------------------------------------
	
	void InitDebugGrid(Grid* pGrid, SDL_Texture** LightTab, SDL_Texture** DarkTab, int tab[7][7]) {
		pGrid->size = 7;
		pGrid->difficulty = NULL;

		pGrid->tile = (Tile**)malloc(sizeof(Tile*) * pGrid->size);

		if (pGrid->tile == NULL) {
			exit(1);
		}

		//Allocation de la mémoire pour chaque case
		for (int i = 0; i < pGrid->size; i++)
		{
			pGrid->tile[i] = (Tile*)malloc(pGrid->size * sizeof(Tile));
			if (pGrid->tile == NULL) {
				exit(1);
			}
		}

		for (int i = 0; i < pGrid->size; i++)
		{
			for (int j = 0; j < pGrid->size; j++)
			{
				Tile* tile = GetTile(pGrid, i, j);

				tile->x = i;
				tile->y = j;
				tile->IsRevealed = FALSE;
				tile->Value = i * pGrid->size + j;
				tile->IsMined = FALSE;
				tile->IsFlag = FALSE;
				tile->AdjacentMines = 0;
				if ((i + j) % 2 == 0)
				{
					tile->Texture = LightTab[9];
				}
				else {
					tile->Texture = DarkTab[9];
				}
			}
		}
	}

	void GenerateBombDebug(Grid* pGrid, int tab[7][7]) {
		for (int i = 0; i < pGrid->size; i++) {
			for (int j = 0; j < pGrid->size; j++) {
				if (tab[j][i] == 1) {
					pGrid->tile[i][j].IsMined = TRUE;
				}
			}
		}
	}
	
	//--------------------------------------DEBUG FONCTION--------------------------------------

	void LaunchGame(Grid* pGrid) 
	{
		BOOL2 FirstInput = TRUE;
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		while(1)
		{ 
			PrintGrid(pGrid, FALSE);

			SetConsoleTextAttribute(hConsole, WHITE);
			int UserChoice = AskIntBetween("\nChoisissez la case que vous voulez reveler", 0, pow(pGrid->size, 2) - 1);
			int iUser = UserChoice / pGrid->size;
			int jUser = UserChoice % pGrid->size;

			Tile* tile = GetTile(pGrid, iUser, jUser);

			if (tile->IsFlag == TRUE) {
				int RmFlag = AskIntBetween("Que voulez vous faire sur ce drapeau? (0: Annuler la selection, 1: Reveler, 2: Enlever le drapeau", 0, 2);
				
				if (RmFlag == 0)
					continue;
				
				tile->IsFlag = FALSE;

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
					tile->IsFlag = TRUE;
					if (CheckWin(pGrid) == TRUE) {
						PrintGrid(pGrid, TRUE);
						SetConsoleTextAttribute(hConsole, WHITE);
						printf("\nBravo vous avez gagne !");
						return;
					}
					continue;
				}
			}
			
			if (FirstInput == TRUE) {
				int bomb_amount = GenerateBomb(pGrid, iUser, jUser);
				pGrid->remainingtiles = pow(pGrid->size, 2) - bomb_amount;
				FirstInput = FALSE;
			}

			if (tile->IsMined == TRUE) {
				PrintGrid(pGrid, TRUE);
				SetConsoleTextAttribute(hConsole, 15);
				printf("\nVous avez perdu !");

				return;
			}

			UpdateGrid(pGrid, iUser, jUser);
			if (CheckWin(pGrid) == TRUE) {
				PrintGrid(pGrid, TRUE);
				SetConsoleTextAttribute(hConsole, WHITE);
				printf("\nBravo vous avez gagne !");
				return;
			}

		
		}
	}

	int main2() {
		
		char replay = 'y';
		srand(time(NULL));

		

		do {
			Grid grid;
			//InitGrid(&grid);
			LaunchGame(&grid);
			replay = AskChar("\nVoulez vous rejouer ?", "YyNn");
		} while (replay == 'y' || replay == 'Y');
		

		return 0;
	}


