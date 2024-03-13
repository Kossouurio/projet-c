

#include <stdio.h>
#include <stdlib.h>

namespace Exercice{

	void SetTo5(float* tf) {
		for (int r = 0; r < 10; r++) {
			*(tf + r) = 5;
		}
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

	int main() {
		/* 1)
		float f1;
		f1 = 5;

		float* pf1 = &f1;
		*pf1 = 5;
		float f2;
		pf1 = &f2;
		*pf1 = 20;

		printf("%f", f2);
		*/

		/*
		float* pf1 = (float*) malloc(sizeof(float));
		if (pf1 == NULL) {
			exit(1);
		}
		float* pf2 = (float*) malloc(sizeof(float));
		if (pf2 == NULL) {
			exit(1);
		}		
		*pf1 = 5;
		float* temp = pf1;
		pf1 = pf2;
		pf2 = temp;
		free(pf1);
		free(pf2);
		*/


		/*3)
		float* tf = (float*)malloc(sizeof(float) * 10);
		for (int i = 0; i < 10; i++) {
			*(tf + i) = 0;
		}

		SetTo5(tf);

		for (int i = 0; i < 10; i++) {
			printf("%f", *(tf + i));
		}

		free(tf);
		*/

		
		int num;
		int* tf = (int*)malloc(sizeof(int));
		if (tf == NULL) {
			exit(1);
		}

		int index = 0;
		while(1) {
			num = AskInt("Rentrez un nombre dans votre tableau");

			if (num == -1) {
				for (int i = 0; i < index; i++) {
					printf("%d, ", *(tf + i));
				}
				break;
			}

			*(tf + index) = num;
			index++;
			tf = (int*)realloc(tf, sizeof(int) * (index + 1));
			if (tf == NULL) {
				exit(1);
			}
		}


		


		return 0;

	}

}