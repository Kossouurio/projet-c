#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

char AskChar(const char text[], const char numb[]) {

	char chartext;

	while (1)
	{
		printf("%s\n", text);
		scanf_s("%c", &chartext, 1);
		while (getchar() != '\n');
		

		for (int i = 0; i < sizeof(numb); i++) {
			if (chartext == numb[i]) {
				return chartext;
			}
		}


		printf("Erreur de saisie, veuillez recommencer.\n");
	}
}

int AskIntInterval(const char text[], int MIN, int MAX)
{
	int number = 0;

	while (1)
	{
		printf("%s\n", text);

		if (scanf_s("%d", &number) == 1 && number >= MIN && number <= MAX) {
			break;
		}
		printf("Entrer un nombre entre %d et %d.\n\n", MIN, MAX);
	}
	return number;
}

int main()
{
	char replay = 'Y';

	do {
		int numberMystery = 0, numberChosen = 0, MIN = 1, MAX = 100, essais = 3;

		MIN = AskInt("Entrez la valeur minimale : ");
		MAX = AskInt("Entrez la valeur maximale : ");

		while (MAX <= MIN) {
			MAX = AskInt("Veuillez rentrer une borne maximale differente et superieure de la borne minimale :");
		}

		srand(time(NULL));
		numberMystery = (rand() % (MAX - MIN + 1)) + MIN;

		essais = AskInt("Entrez le nombre d'essais : ");

		system("cls");

		while (essais > 0)
		{
			numberChosen = AskIntInterval("Entrez un nombre : ", MIN, MAX);

			if (numberMystery > numberChosen) {
				printf("C'est plus !\n\n");
			}
			
			else if (numberMystery < numberChosen) {
				printf("C'est moins !\n\n");
			}
			
			else {
				printf("Bravo, vous avez trouve le nombre mystere !!!\n\n");
				break;
			}
			essais--;
		}
		if (essais == 0) { printf("Vous avez depasse le nombre de tentatives, le nombre mystere etait : %d\n", numberMystery); }
		
		replay = AskChar("Voulez-vous rejouer ? (y, n, Y, N) : \n", "ynYN");

	} while (replay == 'y' || replay == 'Y');

	return 0;
}

// Compiler le programme : F6
// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage
