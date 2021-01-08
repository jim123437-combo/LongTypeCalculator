#pragma once

#define ElemType int
#define ERROR -1
#define OK 0
#define TRUE 1
#define FALSE 0

using namespace std;

typedef struct NodeOfDoubleList //Definition of DListNode;
{
	ElemType data;
	NodeOfDoubleList* next;
	NodeOfDoubleList* prior;
}NDList, * DList;

int InitDList(DList& L);
int DestroyDList(DList& L);
int ClearDList(DList& L);
int isDListEmpty(DList& L);
int DListPushBack(DList& L, ElemType e);
int DListPushFront(DList& L, ElemType e);
int DListPopBack(DList& L, ElemType& e);
int DListPopFront(DList& L, ElemType& e);
int GetDListLength(DList& L);
int DListInsert(DList& L, int i, ElemType e);
int DListPop(DList& L, int i, ElemType& e);
int DListDelete(DList& L, int i);
int DListElemAdd(DList& L, int i, int e);
int GetDListElem(DList& L, int i);
bool TraverseDList(DList& L, bool (*visit)(ElemType));

int InitDList(DList& L)
{
	if (L != NULL) return ERROR;
	L = (DList)malloc(sizeof(NDList));
	if (!L) return ERROR;//Fail to distribute memory;
	L->next = L;
	L->prior = L;
	return OK;
}

int DestroyDList(DList& L)
{
	DList p, q;
	q = L->next;
	while (q != L)
	{
		p = q;
		q = q->next;
		free(p);
	}
	free(q);
	return OK;
}

int ClearDList(DList& L)
{
	DList p, q;
	q = L->next;
	while (q != L)
	{
		p = q;
		q = q->next;
		free(p);
	}
	L->next = L;
	L->prior = L;
	return OK;
}

int isDListEmpty(DList& L)
{
	if (L->next == L) return TRUE;
	else return FALSE;
}

int DListPushBack(DList& L, ElemType e)
{
	DList q = L->prior;
	DList newNode = (DList)malloc(sizeof(NDList));
	newNode->prior = q;
	newNode->next = L;
	q->next = newNode;
	L->prior = newNode;
	newNode->data = e;
	return OK;
}

int DListPushFront(DList& L, ElemType e)
{
	DList p = L->next;
	DList newNode = (DList)malloc(sizeof(NDList));
	newNode->next = p;
	newNode->prior = L;
	L->next = newNode;
	p->prior = newNode;
	newNode->data = e;
	return OK;
}

int DListPopBack(DList& L, ElemType& e)
{
	if (isDListEmpty(L)) return ERROR;//If the DList is empty,return ERROR;
	DList q = L->prior;
	e = q->data;
	DList p = q->prior;
	p->next = L;
	L->prior = p;
	free(q);
	return OK;
}

int DListPopFront(DList& L, ElemType& e)
{
	if (isDListEmpty(L)) return ERROR;
	DList p = L->next;
	DList q = p->next;
	e = p->data;
	L->next = q;
	q->prior = L;
	free(p);
	return OK;
}

int GetDListLength(DList& L)
{
	int j = 0;
	DList p = L->next;
	while (p != L)
	{
		j++;
		p = p->next;
	}
	return j;
}

int DListInsert(DList& L, int i, ElemType e)
{
	DList p, q;
	p = L;
	if (i<1 || i>GetDListLength(L)) return ERROR;//Infeasible parameter
	int j = 0;
	while (j < i)
	{
		p = p->next;
		j++;
	}
	q = p->prior;
	DList newNode = (DList)malloc(sizeof(NDList));
	newNode->next = p;
	newNode->prior = q;
	q->next = newNode;
	p->prior = newNode;
	newNode->data = e;
	return OK;
}

int DListPop(DList& L, int i, ElemType& e)
{
	if (i<1 || i>GetDListLength(L)) return ERROR;
	int j = 0;
	DList q, p = L;
	while (j < i)
	{
		p = p->next;
		j++;
	}
	q = p->prior;
	e = p->data;
	q->next = p->next;
	p->next->prior = q;
	free(p);
	return OK;
}

int DListDelete(DList& L, int i)
{
	if (i<1 || i>GetDListLength(L)) return ERROR;
	int j = 0;
	DList q, p = L;
	while (j < i)
	{
		p = p->next;
		j++;
	}
	q = p->prior;
	q->next = p->next;
	p->next->prior = q;
	free(p);
	return OK;
}

int DListElemAdd(DList& L, int i, int e)
{
	if (i<1 || i>GetDListLength(L)) return ERROR;
	int j = 1; DList p = L->next;
	while (j < i)
	{
		p = p->next;
		j++;
	}
	p->data += e;
	return OK;
}

int GetDListElem(DList& L, int i)
{
	if (i<1 || i>GetDListLength(L)) return ERROR;
	int j = 0; DList p = L;
	while (j < i) { p = p->next; j++; }
	//e = p->data;
	return p->data;
}

bool TraverseDList(DList& L, bool (*visit)(ElemType))
{
	DList p = L->next;
	while (p != L)
	{
		if (!visit(p->data)) return false;
		p = p->next;
	}
	return true;
}
