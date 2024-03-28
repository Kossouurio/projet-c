#include <stdlib.h>
#include <stdio.h>

typedef struct String
{
	char* pContent;
	int iLength;
} String;

int GetLength(const char* str)
{
	int i = 0;
	while (str[i] != '\0')
	{
		//printf("%c", str[i]);
		++i;
	}
	
	return i;
}

String Create( const char* str )
{
	String string;
	string.iLength = GetLength(str);
	
	string.pContent = (char*) malloc( sizeof( char ) * ( string.iLength + 1 ));
	if (string.pContent == NULL) { exit(0); }

	for (int j = 0; j < string.iLength; ++j) {
		string.pContent[j] = str[j];
	}

	string.pContent[string.iLength] = '\0';
	
	return string;
}

void Print( const String* pStr )
{
	printf("%s", pStr->pContent);
}

String Concatenate1( const String* pStr1, const String* pStr2 )
{
	String newStr;
	newStr.iLength = pStr1->iLength + pStr2->iLength;

	newStr.pContent = (char*) malloc( sizeof( char ) * (newStr.iLength + 1 ));
	if (newStr.pContent == NULL) { exit(0); }

	for (int i = 0; i < pStr1->iLength; ++i) {
		newStr.pContent[i] = pStr1->pContent[i];
	}

	for (int i = 0; i < pStr2->iLength; ++i) {
		newStr.pContent[pStr1->iLength + i] = pStr2->pContent[i];
	}

	newStr.pContent[newStr.iLength] = '\0';

	return newStr;
}

String Concatenate2( const char* pStr1, const char* pStr2 )
{
	String newStr;

	///Length of char *pStr1
	int pStr1Length = GetLength(pStr1);
	///Length of char *pStr2
	int pStr2Length = GetLength(pStr2);
	
	///Length of both Str
	newStr.iLength = pStr1Length + pStr2Length;

	newStr.pContent = (char*) malloc( sizeof(char) * newStr.iLength * 2 );
	if (newStr.pContent == NULL) { exit(0); }

	for (int i = 0; i < pStr1Length; ++i) {
		newStr.pContent[i] = pStr1[i];
	}

	for (int i = 0; i < pStr2Length; ++i) {
		newStr.pContent[pStr1Length + i] = pStr2[i];
	}

	newStr.pContent[newStr.iLength] = '\0';

	return newStr;
}

String Substring( const String* pStr1, int iStartIndex, int iLength )
{
	if ((iStartIndex + iLength) > pStr1->iLength) { return; }
	
	char* pStr2 = (char*) malloc( sizeof(char) * iLength + 1 );
	if (pStr2 == NULL) { exit(0); }

	for (int i = 0; i < iLength; ++i)
	{
		pStr2[i] = pStr1->pContent[i + iStartIndex];
	}
	pStr2[iLength] = '\0';
	
	String newStr = Create(pStr2);
	
	return newStr;
}

String Insert( const String* pStr1, const String* pStr2, int iIndex )
{
	String subStr1 = Substring(pStr1, 0, iIndex);
	String concStr = Concatenate1( &subStr1, pStr2 );
	String subStr2 = Substring(pStr1, iIndex, pStr1->iLength - iIndex);
	String newStr = Concatenate1( &concStr, &subStr2);

	
	///Autre façon de faire le Insert
	/*
	String newStr;
	newStr.iLength = pStr1->iLength + pStr2->iLength;

	newStr.pContent = (char*)malloc(sizeof(char) * (newStr.iLength + 1));
	if (newStr.pContent == NULL) { exit(0); }

	for (int i = 0; i < iIndex; ++i) {
		newStr.pContent[i] = pStr1->pContent[i];
	}
	for (int i = 0; i < pStr2->iLength; ++i) {
		newStr.pContent[iIndex + i] = pStr2->pContent[i];
	}
	for (int i = iIndex; i < pStr1->iLength; ++i) {
		newStr.pContent[pStr2->iLength + i] = pStr1->pContent[i];
	}
	newStr.pContent[newStr.iLength] = '\0';
	*/

	return newStr;
}

int AreEquals( const String* pStr1, const String* pStr2 )
{
	if ( pStr1->iLength != pStr2->iLength )
	{
		return 0;
	}
	for (int i = 0; i < pStr1->iLength; ++i)
	{
		if (pStr1->pContent[i] != pStr2->pContent[i])
		{
			return 0;
		}
	}

	return 1;
}

int TryCastToInt( const String* pStr, int* iResult )
{
	*iResult = 0;
	for (int i = 0; i < pStr->iLength; ++i)
	{	
		if (pStr->pContent[i] < '0' || pStr->pContent[i] > '9') { return 0; }

		*iResult = *iResult * 10 + (pStr->pContent[i] - '0');
	}

	return 1;
}

void Destroy( String* pStr )
{
	free(pStr->pContent);
}

int main()
{
	printf("-------| Create 1 |-------\n");
	String str1 = Create( "Bonjour et " );
	Print( &str1 );
	printf("\n-------| %d |-------\n", str1.iLength);

	printf("\n-------| Create 2 |-------\n");
	String str2 = Create( "Aurevoir" );
	Print( &str2 );
	printf("\n-------| %d |-------\n", str2.iLength);

	printf("\n-------| Concatenate1 |-------\n");
	String str3 = Concatenate1( &str1, &str2 );
	Print( &str3 );
	printf("\n-------| %d |-------\n", str3.iLength);

	printf("\n-------| Concatenate2 |-------\n");
	String str4 = Concatenate2("Love ", "party !");
	Print(&str4);
	printf("\n-------| %d |-------\n", str4.iLength);

	printf("\n-------| Substring |-------\n");
	String str5 = Substring(&str2, 4, 4);
	Print(&str5);
	printf("\n-------| %d |-------\n", str5.iLength);

	printf("\n-------| Insert |-------\n");
	String str6 = Insert(&str1, &str2, 3);
	Print(&str6);
	printf("\n-------| %d |-------\n", str6.iLength);

	printf("\n-------| AreEquals |-------\n"); //Words must be exatly the same with the same upper case and lower case letters for AreEquals to works
	String str7_1 = Create("Coucou");
	String str7_2 = Create("Coucou");
	printf("%d", AreEquals(&str7_1, &str7_2));

	printf("\n-------| TryCastToInt |-------\n");
	String str8 = Create("4682");
	int* iStr8;
	printf("Out: %d // ", TryCastToInt(&str8, &iStr8));
	printf("Result: %d", (int)iStr8);

	//Destroyer
	Destroy(&str1);
	Destroy(&str2);
	Destroy(&str3);
	Destroy(&str4);
	Destroy(&str5);
	Destroy(&str6);

	return 0;
}