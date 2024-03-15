#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Exercices.h"


int get_length(const char* str)
{
	int i = 0;
	while (str[i] != '\0') {
		i++;
	}
	return i;
}


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


		printf("%s %d et %d\n", text, Min, Max);
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



int main() {

	
	/*Exercice::main();
	return 0;*/
	Demineur::main();
	return 0;

	char replay = 'y';
	int* t = (int*)malloc(sizeof(int));

	if (t == NULL) {
		exit(1);
	}
	int FoundNumber = 0;
	do {
		system("cls");
		int bornemini;
		int bornemax;
		int NumberOfTry;
		int tries = 1;
		bornemini = AskInt("Veuillez rentrer la borne minimale : ");
		bornemax = AskInt("\nVeuillez rentrer la borne maximale : ");

		while (bornemax <= bornemini) {
			bornemax = AskInt("Veuillez rentrer une borne maximale differente et superieure de la borne minimale :");
		}
		NumberOfTry = AskIntBetween("Combien voulez vous de tentatives ? Choisissez un nombre entre", 1, bornemax - bornemini);
		system("cls");

		srand(time(NULL));

		int searchedValue = (int)(rand() % (bornemax + 1 - bornemini) + bornemini);
		int GuessValue;

		while (tries <= NumberOfTry) {
			printf("%d", searchedValue);


			GuessValue = AskIntBetween("Rentrez un nombre entre", bornemini, bornemax);



			if (GuessValue == searchedValue) {
				*(t + FoundNumber) = searchedValue;
				FoundNumber++;
				
				int* t2  = (int*) realloc(t, sizeof(int) * (FoundNumber + 1));

				if (t2 == NULL) {
					free(t);
					exit(1);
				}

				t = t2;

				printf("Bravo vous avez trouve le nombre : %d en %d d'essai(s)\n", searchedValue, tries);
				break;
			}
			else if (tries <= NumberOfTry) {
				system("cls");
				printf("Vous avez perdus ! le nombre mystere etait %d\n", searchedValue);
				break;
			}
			else if (GuessValue < searchedValue) {
				printf("\nC'est plus que ca !\n");
				tries++;
			}
			else {
				printf("\nC'est moins que ca !\n");
				tries++;
			}

		}
		replay = AskChar("Voulez vous rejouer ?", "YyNn");

	} while (replay == 'y' || replay == 'Y');
	system("cls");
	if (FoundNumber >= 1) {
		printf("Voici la liste de tous les nombres que vous avez trouvés jusqu'a present : [");
		for (int i = 0; i < FoundNumber - 1; i++) {
			printf("%d, ", *(t + i));
		}
		printf("%d", *(t + FoundNumber - 1));
		printf("]");
	}
	
	free(t);

	return 0;
}