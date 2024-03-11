
#include <iostream>

int AskInt(const char text[], int min, int max)
{
	int number = 0;

	while (1)
	{
		printf("%s\n", text);
		scanf_s("%d", &number);

		if (number >= min || number <= max) {
			fputs("error: invalid input\n", stderr);
		}
		else {
			printf("Le nombre doit etre compris entre %d et %d\n", min, max);
			continue;
		}
	}
	

	while (number >= 0) {
		printf("Enter a number: ");

		if (scanf_s("%d", &number) != 1) {
			fputs("error: invalid input\n", stderr);
		}
		else if (number < 0) {
			fputs("error: negative input\n", stderr);
		}
		else {
			break;
		}
	}

	return 0;
}

int main()
{

	int numberMystery = 0, numberChosen = 0;

	srand(time(NULL));
	const int MAX = 100, MIN = 1;
	numberMystery = (rand() % (MAX - MIN + 1)) + MIN;

	do
	{
		numberChosen = AskInt("Entrez un nombre entre 1 et 100 : ", 1, 100);

		if (numberMystery > numberChosen)
			printf("C'est plus !\n\n");
		else if (numberMystery < numberChosen)
			printf("C'est moins !\n\n");
		else
			printf("Bravo, vous avez trouve le nombre mystere !!!\n\n");
	} while (numberChosen != numberMystery);

	return 0;
}

// Compiler le programme : F6
// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Astuces pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.
