#include <stdlib.h>
#include <stdio.h>

typedef struct Node
{
	Node* pNext;
	Node* pPrevious;

	int iValue;

} Node;

typedef struct List
{
	Node* pFirst;

	int iCount;

} List;

void Init( List* pList )
{
	pList->pFirst = (Node*)malloc(sizeof(Node) * pList->iCount);

	pList->pFirst->pNext = (Node*)malloc(sizeof(Node) * pList->iCount);
	pList->pFirst->pPrevious = (Node*)malloc(sizeof(Node) * pList->iCount);
	pList->pFirst->iValue = 0;

	pList->iCount = 0;
}

void AddFirst( List* pList, int iValue )
{
	//
}

void AddLast( List* pList, int iValue )
{
	pList->iCount++;
	pList->pFirst = (Node*)realloc(pList->pFirst, sizeof(Node) * pList->iCount);
	if (pList->pFirst == NULL) { exit(1); }

	pList->pFirst->iValue = iValue;
}

void AddBeforeNode( List* pList, Node* pNode, int iValue )
{
	//
}

void AddAfterNode( List* pList, Node* pNode, int iValue )
{
	//
}

void Insert( List* pList, int iValue, int iIndex )
{
	//
}

void Remove( List* pList, int iIndex )
{
	//
}

void RemoveNode( List* pList, Node* pNode )
{
	//
}

Node* GetNode( List* pList, int iIndex )
{
	//
}

void Print( List* pList )
{
	for (int i = 0; i < pList->iCount; ++i)
	{
		printf("%d", pList->pFirst[i]);
	}
}

void Destroy( List* pList )
{
	free(pList->pFirst);
}


int main()
{
	List oList;
	Init( &oList );

	AddLast( &oList, 1 );
	AddLast( &oList, 2 );
	AddLast( &oList, 3 );
	Print( &oList );
	printf("AddLast");
	/*
	AddFirst( &oList, 0 );
	Print( &oList );
	printf("AddFirst");
	*/

	//Destroy( &oList );
}