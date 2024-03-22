#include <stdlib.h>
#include <stdio.h>
#include <iostream>

typedef struct String {
    char* pContent;
    int iLength;
};

char digit[] = {'0','1','2','3','4','5','6','7','8','9'};

int GetLength(const char* str)
{
	int i = 0;
	while (str[i] != '\0') {
		i++;
	}
	return i;
}

int IsInt(const char* str) {
	for (int i = 0; i < GetLength(str); i++) {
		for (int j = 0; j < 10; j++) {
			if (str[i] != digit[j]) {
				return 0;
			}
		}
	}
	return 1;
}

String Create(const char* str) {
	String string;
	string.pContent = NULL;
	string.iLength = 0;
	string.pContent = (char*)malloc(sizeof(char) * 1);
	if (string.pContent == NULL) {
		exit(1);
	}
	for (int i = 0; i < GetLength(str); i++) {
		string.iLength++;
		string.pContent = (char*)realloc(string.pContent, sizeof(char) * string.iLength);

		if (string.pContent == NULL) {
			exit(1);
		}
		string.pContent[i] = str[i];

	}
	return string;

}

void Print(const String* str) {
	for (int i = 0; i < str->iLength; i++) {
		printf("%c", str->pContent[i]);
	}
}

String Concatenate1(const String* str1, const String* str2) {
	String string;
	string.pContent = NULL;
	string.iLength = 0;
	string.pContent = (char*)malloc(sizeof(char) * 1);
	if (string.pContent == NULL) {
		exit(1);
	}
	for (int i = 0; i < str1->iLength; i++) {
		string.iLength++;
		string.pContent = (char*)realloc(string.pContent, sizeof(char) * string.iLength);

		if (string.pContent == NULL) {
			exit(1);
		}
		string.pContent[i] = str1->pContent[i];
	}
	for (int j = str1->iLength; j < str1->iLength + str2->iLength; j++) {
		string.iLength++;
		string.pContent = (char*)realloc(string.pContent, sizeof(char) * string.iLength);

		if (string.pContent == NULL) {
			exit(1);
		}
		string.pContent[j] = str2->pContent[j - str2->iLength + 1];
	}
	return string;
}

String Concatenate2(const char* str1, const char* str2) {
	int iLengthTotal = GetLength(str1) + GetLength(str2) + 1 ;
	int iLength1 = GetLength(str1);
	int iLength2 = GetLength(str2);
	String string;
	string.pContent = NULL;
	string.iLength = iLengthTotal;
	string.pContent = (char*)malloc(sizeof(char) * iLengthTotal);
	if (string.pContent == NULL) {
		exit(1);
	}

	for (int i = 0; i < iLength1; i++) {
		string.pContent[i] = str1[i];
	}

	for (int j = iLength1; j < iLengthTotal; j++) {
		string.pContent[j] = str2[j - iLength1];
	}
	return string;
}

String SubString(const String* pStr1, int iStartIndex, int iLength) {
	int iLengthTotal = iLength + 1 ;
	String string;
	string.pContent = NULL;
	string.iLength = iLengthTotal;
	string.pContent = (char*)malloc(sizeof(char) * iLengthTotal);
	if (string.pContent == NULL) {
		exit(1);
	}
	if (iStartIndex + iLength > pStr1->iLength || iStartIndex >= pStr1->iLength) {
		printf("Erreur dans les bornes");
		exit(1);
	}
	for (int i = iStartIndex; i < iStartIndex + iLength +1; i++) {
		string.pContent[i - iStartIndex] = pStr1->pContent[i];
	}
	return string;
}

int AreEquals(const String* pStr1, const String* pStr2) {
	if (pStr1->iLength != pStr2->iLength) {
		return 0;
	}
	for (int i = 0; i < pStr1->iLength; i++) {
		if (pStr1->pContent[i] != pStr2->pContent[i])
			return 0;
	}
	return 1;
}

int TryToCastInt(const String* pStr, int* pResult) {
	*pResult = 0;
	if (IsInt(pStr->pContent)) {
		for (int i = 0; i < pStr->iLength; i++) {
			int num = pStr->pContent[i] -48;
			*pResult += num * pow(10, pStr->iLength - i - 1);
		}
		return 1;
	}




	return 0;
}

void Destroy(String* pStr) {
	if (pStr->pContent != NULL) {
		free(pStr->pContent);
	}
	free(pStr);
}

int main()
{
	String str1 = Create("Bonjour");
	Print(&str1);
	printf("\n");

	String str2 = Create("Aurevoir");
	String str7 = Create("Aurevoir");

	String str3 = Concatenate1(&str1, &str2);
	Print(&str3);
	printf("\n");

	String str4 = Concatenate2("Salut", "Mec");
	Print(&str4);
	printf("\n");
	String str5 = SubString(&str4, 2, 2);
	Print(&str5);
	printf("\n");

	int equal = AreEquals(&str2, &str7);
	printf("%d", equal);
	printf("\n");
	printf("\n");
	printf("\n");
	
	String str8 = Create("548d");
	int i;
	int success =TryToCastInt(&str8, &i);
	printf("%d", success);
	printf("\n");
	printf("%d", i);;
}
