#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>
#include "DouCirList.h"
#include <cmath>

int StringNumberToDList(DList& L, string s);
int IntNumberToDList(DList& L, int e);
string DListToStringNumber(DList& L);
void NumAdjust(DList& L);
int DListNumberAdd(DList L1, DList L2, DList& L);
int DListNumberMinus(DList L1, DList L2, DList& L);
int DListNumberMulti(DList L1, DList L2, DList& L);
int DListNumberCompare(DList L1, DList L2,int mode=0);
int LegalCheck(string s);

int StringNumberToDList(DList& L, string s)
{
	s.push_back(',');
	if (s[0] == '-')
	{
		L->data = -1;
		s.erase(0, 1);
	}
	else
	{
		L->data = 1;
		if (s[0] == '+') s.erase(0, 1);
	}
	for (int i = 0; i < s.length(); i++)
	{
		if (s[i] == ',')
		{
			string temp = s.substr(0, i);
			int NumberNode;
			stringstream a;
			a << temp;
			a >> NumberNode;
			DListPushBack(L, NumberNode);
			a.clear();
			s.erase(0, i + 1);
			i = -1;
		}
	}
	return OK;
}

int IntNumberToDList(DList& L, int e)
{
	if (e == 0)
	{
		L->data = +1;
		DListPushFront(L, 0);
		return OK;
	}
	L->data = e / abs(e);
	while (e != 0)
	{
		DListPushFront(L,abs(e % 10000));
		e /= 10000;
	}
	return OK;
}

string DListToStringNumber(DList& L)
{
	string temp;
	DList p = L->next;
	if (L->data == 1) temp.push_back('+');
	if (L->data == -1) temp.push_back('-');
	while (p != L)
	{
		string s = to_string(p->data);
		if (s.length() <= 4 && temp.length() != 1) s.insert(0, 4 - s.length(), '0');
		temp += s;
		temp.push_back(',');
		p = p->next;
	}
	temp.erase(temp.length() - 1, 1);
	if (temp.length() == 2 && temp[1] == '0') temp.erase(0, 1);
	return temp;
}

void NumAdjust(DList& L)
{
	if (L->next->data < 0)
	{
		L->data = -1;
		if (GetDListLength(L) >= 2)
		{
			L->prior->data = 10000 - L->prior->data;
			DList q = L->prior->prior;
			while (q != L->next)
			{
				q->data = 9999 - q->data;
				q = q->prior;
			}
			L->next->data = abs(L->next->data) - 1;
			if (L->next->data == 0) DListDelete(L, 1);
		}
		else
		{
			L->next->data = -L->next->data;
		}
	}
	else L->data = 1;
	DList q = L->prior;
	while (q->prior != L)
	{
		q->prior->data += q->data / 10000;
		q->data %= 10000;
		q = q->prior;
	}
	if (abs(q->data) >= 10000)
	{
		DListPushFront(L, q->data / 10000);
		q->data %= 10000;
	}
	while (GetDListLength(L) >= 2 && L->next->data == 0)
	{
		DListDelete(L, 1);
	}
}

int DListNumberAdd(DList L1, DList L2, DList& L)
{
	int MinLength = __min(GetDListLength(L1), GetDListLength(L2));
	DList p = L1->prior, q = L2->prior;
	for (int i = 0; i < MinLength; i++)
	{
		int NodeOfNumber = (L1->data * p->data + L2->data * q->data);
		DListPushFront(L, NodeOfNumber);
		p = p->prior; q = q->prior;
	}
	while (p != L1)
	{
		DListPushFront(L, L1->data*p->data);
		p = p->prior;
	}
	while (q != L2)
	{
		DListPushFront(L, L2->data*q->data);
		q = q->prior;
	}
	DList r = L->next;
	while (r->data == 0&&GetDListLength(L)>=2)
	{
		DListDelete(L, 1);
		r = L->next;
	}
	r = L->prior;
	if (GetDListLength(L) >= 2)
	{
		while (r != L->next)
		{
			if (r->data < 0)
			{
				r->prior->data--;
				r->data = 10000 - abs(r->data);
			}
			r = r->prior;
		}
	}
	while (r->data == 0&&GetDListLength(L)>=2)
	{
		DListDelete(L, 1);
		r = L->next;
	}
	NumAdjust(L);
	return OK;
}

int DListNumberMinus(DList L1, DList L2, DList& L)
{
	L2->data *= -1;
	DListNumberAdd(L1, L2, L);
	L2->data *= -1;
	return OK;
}

int DListNumberMulti(DList L1, DList L2, DList& L)
{
	int IndexLengthL1 = GetDListLength(L1)-1; 
	int IndexLengthL2 = GetDListLength(L2)-1;
	for (int k = 0; k < IndexLengthL1 + IndexLengthL2+1; k++)
	{
		DListPushBack(L, 0);
	}
	DList pL1=L1->next, pL2 =L2->next ;
	for (int i = 0; i <=IndexLengthL1; i++)
	{
		for (int j = 0; j <=IndexLengthL2 ; j++)
		{
			DListElemAdd(L, i + j + 1, GetDListElem(L1, i + 1) * GetDListElem(L2, j + 1));
		}
	}
	NumAdjust(L);
	L->data = L1->data * L2->data;
	return OK;
}

int DListNumberDivision(DList L1, DList L2, DList& L)
{
	int signL1 = L1->data;
	int signL2 = L2->data;
	if (L2->next->data == 0)
		return ERROR;
	int L1Length = GetDListLength(L1);
	int L2Length = GetDListLength(L2);
	if (DListNumberCompare(L1,L2,1)<0)
	{
		IntNumberToDList(L, 0);
		return OK;
	}
	else
	{
		int sign = signL1 / signL2;
		L1->data = 1;
		L2->data = 1;
		int x1 = 1;
		int x2 = pow(10000, abs(L1Length-L2Length+1));
		while (1)
		{
			if (x2 - x1 <= 1)
			{
				IntNumberToDList(L, x1);
				L->data = sign;
				break;
			}
			DList Temp=NULL;
			InitDList(Temp);
			DList ResMulti = NULL; 
			InitDList(ResMulti);
			IntNumberToDList(Temp, (x1 + x2) / 2);
			DListNumberMulti(L2, Temp, ResMulti);
			if (DListNumberCompare(ResMulti, L1) < 0)
			{
				x1 = (x1 + x2) / 2;
			}
			if (DListNumberCompare(ResMulti, L1) > 0)
			{
				x2 = (x1 + x2) / 2;
			}
			if (DListNumberCompare(ResMulti, L1) == 0)
			{
				IntNumberToDList(L, (x1+x2)/2);
				L->data = sign;
				break;
			}
			DestroyDList(Temp); DestroyDList(ResMulti);
		}
		return OK;
	}
}

int DListNumberCompare(DList L1, DList L2,int mode)
{
	if (mode == 1)
	{
		L1->data = 1;
		L2->data = 1;
	}
	DList Temp = NULL;
	InitDList(Temp);
	DListNumberMinus(L1, L2, Temp);
	if (Temp->data < 0)
	{
		DestroyDList(Temp);
		return -1;
	}
	if (Temp->data > 0)
	{
		if (Temp->next->data == 0)
		{
			DestroyDList(Temp);
			return 0;
		}
		if (Temp->next->data != 0)
		{
			DestroyDList(Temp);
			return +1;
		}
	}
	return ERROR;
}

int LegalCheck(string s)
{
	for (int i = 0; i < s.length(); i++)
	{
		if ((s[i]<-1||s[i]>255)||(s[i] != '+' && s[i] != '-' &&s[i]!=','&& !isdigit(s[i]))) return ERROR;
	}
	if (s[0] == '+' || s[0] == '-')
	{
		if (s[1] == '0'&&s.length()>2) return ERROR;
		s.erase(0, 1);
	}
	else if (s[0] == '0'&&s.length()>1) return ERROR;
	int j = 0;
	for (int i = 0; i < s.length(); i++)
	{
		if (s[i] == ',')
		{
			string temp = s.substr(0, i);
			if ((temp.length() !=4&&j!=0)||(j==0&&temp.length()>4)) return ERROR;
			s.erase(0, i + 1);
			i = -1;
			j++;
		}
	}
	if (s.length() >4&&j==0||s.length()!=4&&j!=0) return ERROR;
	return OK;
}

int main()
{
	string a;
	string b;
	cout << "欢迎使用长整数运算器" << endl;
	cout << "请按如下格式输入数字,四个为一组，除第一组外其余组的前导零不可省略" << endl;
	cout << "注意！！！本计算器中除法意为整除" << endl;
	cout << endl << "示例:" << endl;
	cout << "a = xxx,xxxx,xxxx" << endl;
	cout << "b = -xxx,xxxx" << endl<<endl;
	
	while (1)
	{
		cout << "现在请输入整数a = ";
		cin >> a;
		if (LegalCheck(a) == 0) break;
		else
		{
			cout << "对不起您输入的数字不合法，请重新输入" << endl;
			continue;
		}
	}
	while (1)
	{
		cout << "现在请输入整数b = ";
		cin >> b;
		if (LegalCheck(b) == 0) break;
		else
		{
			cout << "对不起您输入的数字不合法，请重新输入" << endl;
			continue;
		}
	}
	cout << "计算结果为：" << endl << endl;
	cout << "a = " << a << endl;
	cout << "b = " << b << endl;
	DList L1 = NULL, L2 = NULL, LPLUS = NULL, LMINUS = NULL,LMULTI=NULL,LDIVI=NULL;
	InitDList(L1); InitDList(L2); InitDList(LPLUS); InitDList(LMINUS); InitDList(LMULTI); InitDList(LDIVI);
	StringNumberToDList(L1, a);
	StringNumberToDList(L2, b);
	DListNumberAdd(L1, L2, LPLUS);
	DListNumberMinus(L1, L2, LMINUS);
	DListNumberMulti(L1, L2, LMULTI);
	int flag=DListNumberDivision(L1, L2, LDIVI);
	string res1,res2,res3,res4;
	res1 = DListToStringNumber(LPLUS);
	res2 = DListToStringNumber(LMINUS);
	res3 = DListToStringNumber(LMULTI);
	
	
	cout <<"a + b = "<< res1 << endl;
	cout <<"a - b = "<< res2 << endl;
	cout <<"a * b = "<< res3 << endl;
	if (flag!=-1)
	{
		res4 = DListToStringNumber(LDIVI);
		cout << "a / b = " << res4 << endl;
	}
	else cout << "a/b运算不合法：除数b不可为0" << endl;
	system("pause");
	return 0;
}