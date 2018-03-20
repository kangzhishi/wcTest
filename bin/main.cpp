#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <locale>
#include <conio.h>


/************************************************************************/
/*                           �ʷ�����                                  */
/************************************************************************/
#define max 10

char *rwtab[9] = { "main","int","float","double","char","if","else","do","while" };

char prog[100];//Դ����
int p;//��ǰ�����ַ�λ��
char ch; //��ǰ�����ַ�
int flag; //1��ʾ�ն�ȡһ������������"+/-"Ϊ�����;0��֮��"+/-"����Ϊ��ֵ����

int syn;       //�ֱ����
char token[max]; //�����֡��ڲ��ַ����������
double sum;    //��ֵ

char** variable;//������Ϣ��
int nVar;
/************************************************************************/
void scaner()
{
	int i;
	for (i = 0; i<max; i++) token[i] = '\0';
	sum = 0;

	int m = 0;
	int e = 0;//��ֵָ��

	ch = prog[p++];
	while (isspace(ch)) ch = prog[p++];//Ԥ����ȥ��ע�͡�����ո񡢻س����з���      

	if (isalpha(ch))//�����֡��ڲ��ַ���
	{
		while (isalnum(ch))
		{
			token[m++] = ch;
			ch = prog[p++];
		}
		token[m++] = '\0';
		p--;
		syn = 10;
		for (i = 0; i<9; i++)
			if (strcmp(token, rwtab[i]) == 0)
			{
				syn = i + 1;
				flag = 0;
				break;
			}
		if (syn == 10)
		{
			flag = 1;
			for (i = 1; i <= nVar; i++)
			{
				if (!strcmp(token, variable[i])) return;
			}
			strcpy(variable[++nVar], token);
		}

	}

	else if (ch == '+' || ch == '-' || isdigit(ch))//��ֵ��"+"��"-"
	{
		if (!isdigit(ch) && (flag == 1 || !isdigit(prog[p])))
		{
			token[m++] = ch;
			if (ch == '+') syn = 22;
			else syn = 23;
			flag = 0;
		}
		else
		{
			int flag1 = 0;
			int flag2 = 0;
			if (ch == '+' || ch == '-')
			{
				ch = prog[p++];
				if (ch == '-') flag1 = 1;
			}
			while (isdigit(ch))
			{
				sum = sum * 10 + ch - '0';
				ch = prog[p++];

			}
			int k = 10;
			if (ch == '.' && isdigit(prog[p]))
			{
				ch = prog[p++];
				while (isdigit(ch))
				{
					double d = ch - '0';
					sum = sum + d / k;
					k = k * 10;
					ch = prog[p++];
				}
			}
			if (ch == 'e' || ch == 'E')
			{
				char ch_tmp = prog[p];
				if (((ch_tmp == '+' || ch_tmp == '-') && isdigit(prog[p + 1])) || isdigit(ch_tmp))
				{
					ch = prog[p++];
					if (!isdigit(ch))
					{
						if (ch == '+')
							flag2 = 0;
						else
							flag2 = 1;
						ch = prog[p++];
					}
					while (isdigit(ch))
					{
						e = e * 10 + ch - '0';
						ch = prog[p++];
					}
					if (flag2)
						sum = sum*pow(10.0, -e);
					else
						sum = sum*pow(10.0, e);
				}
			}

			if (flag1) sum *= (-1);
			p--;
			syn = 20;
			flag = 1;
		}
	}

	else//��������ָ���
	{
		flag = 0;
		m = 0;
		switch (ch)
		{
		case '<':

			token[m++] = ch;
			ch = prog[p++];
			if (ch == '=')
			{
				syn = 35;
				token[m++] = ch;
			}
			else
			{
				syn = 34;
				p--;
			}
			break;
		case '>':
			token[m++] = ch;
			ch = prog[p++];
			if (ch == '=')
			{
				syn = 33;
				token[m++] = ch;
			}

			else
			{
				syn = 32;
				p--;
			}
			break;
		case '=':
			token[m++] = ch;
			ch = prog[p++];
			if (ch == '=')
			{
				syn = 36;
				token[m++] = ch;
			}
			else
			{
				syn = 21;
				p--;
			}
			break;
		case '!':
			token[m++] = ch;
			ch = prog[p++];
			if (ch == '=')
			{
				syn = 37;
				token[m++] = ch;
			}
			else
			{
				syn = -1;
			}
			break;
		case '*':    syn = 24; token[0] = ch; break;
		case '/':    syn = 25; token[0] = ch; break;
		case '(':    syn = 26; token[0] = ch; break;
		case ')':    syn = 27; token[0] = ch; break;
		case '{':    syn = 28; token[0] = ch; break;
		case '}':    syn = 29; token[0] = ch; break;
		case ',':    syn = 30; token[0] = ch; break;
		case ';':    syn = 31; token[0] = ch; break;
		case '#':    syn = 0;  token[0] = ch; break;
		default:     syn = -1; printf("illegal character %c/n", ch);
		}
	}
	return;
}


/************************************************************************/
/*                         �﷨�������                                 */
/************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////
//                          �ݹ��½���                                 //
/////////////////////////////////////////////////////////////////////////////////////////////
void P();  //����
void B(int *nChain);  //����
void SS(int *nChain);  //��䴮
void S(int *nChain);  //���
void AS(int *nChain);  //��ֵ���
void CS(int *nChain);  //�������
void LS(int *nChain);  //ѭ�����
void C(int *etc, int *efc);  //����
char * E();  //���ʽ
char * T();  //��
char * F();  //����
void error(); //������
			  //////////////////////////////////////////////////////////////////////////////////////////////
			  //                          �﷨�Ƶ�����                               //
			  /////////////////////////////////////////////////////////////////////////////////////////////
typedef struct quaternion {
	char op[max];
	char argv1[max];
	char argv2[max];
	char res[max];
}quad;//��Ԫʽ
quad *pQuad;//��Ԫʽ��ָ��
int index, nSuffix;//��Ԫʽ���,��ʱ�������
				   /************************************************************************/
void gen(char *op, char *argv1, char *argv2, char *result);
char *NewTemp();
int merg(int p1, int p2);
void bp(int p, int t);
void printQuad();
void Parse();
/************************************************************************/
void error()
{
	if (syn == 20) printf("Syntax error before %g", sum);
	else printf("Syntax error before %g", token);
	syn = 50;
}
//<����> ::= main()<����> 
void P()
{
	int nChain;
	scaner();
	if (syn == 1)
	{
		scaner();
		if (syn == 26)
		{
			scaner();
			if (syn == 27)
			{
				scaner();
				B(&nChain);
			}
			else error();
		}
		else error();
	}
	else error();
}

//<����> ::= ��{��<��䴮>��}��
void B(int *nChain)
{
	if (syn == 28)
	{
		scaner();
		SS(nChain);
		if (syn == 29) scaner();
		else error();
	}
	else error();
}

//<��䴮>::=<���>{;<���>};
void SS(int *nChain)
{
	S(nChain);
	if (syn == 31) scaner();
	else error();
	while (syn != 29)
	{
		S(nChain);
		if (syn == 31) scaner();
		else error();
	}
}

//<���>::=<��ֵ���>|<�������>|<ѭ�����>
void S(int *nChain)
{
	if (syn == 10) AS(nChain);
	else if (syn == 6) CS(nChain);
	else if (syn == 8) LS(nChain);
	else error();
}

//<��ֵ���>::=ID=<���ʽ>
void AS(int *nChain)
{
	char stemp[max];
	char *place;
	if (syn == 10)
	{
		strcpy(stemp, token);
		scaner();
		if (syn == 21)
		{
			scaner();
			place = E();
			gen("=", place, "", stemp);
			*nChain = 0;
		}
		else error();
	}
	else error();
	bp(*nChain, index);

}

//<�������>::=if<����><����>[else <����>]
void CS(int *nChain)
{
	int nChaintmp, ntc, nfc;
	if (syn == 6)
	{
		scaner();
		C(&ntc, &nfc);
		bp(ntc, index);
		B(&nChaintmp);
		*nChain = merg(nChaintmp, nfc);
		if (syn == 7)
		{
			int nfc1;
			scaner();
			nfc1 = index;
			gen("goto", "", "", "0");
			bp(*nChain, index);
			B(&nChaintmp);
			*nChain = merg(nChaintmp, nfc1);
			bp(*nChain, index);
		}
		else
		{
			*nChain = merg(nChaintmp, nfc);
			bp(*nChain, index);
		}
	}
	else error();
}

//<ѭ�����>::=do <����>while <����>
void LS(int *nChain)
{
	int ntc, nfc;
	if (syn == 8)
	{
		int nChaintmp;
		scaner();
		int indextmp = index;
		B(&nChaintmp);
		if (syn == 9)
		{
			scaner();
			C(&ntc, &nfc);
			bp(ntc, indextmp);
			bp(nfc, index);
			*nChain = merg(nChaintmp, nfc);
			bp(*nChain, index);
		}
		else error();
	}
	else error();
}

//<����>::=<���ʽ><��ϵ�����><���ʽ>
void C(int *etc, int *efc)
{
	char op[max], optmp[max], *place1, *place2;

	place1 = E();
	if (syn>31 && syn<38)
	{
		sprintf(op, "%s", token);
		scaner();
		place2 = E();
		*etc = index;
		*efc = index + 1;
		sprintf(optmp, "goto %s", op);
		gen(optmp, place1, place2, "0");
		gen("goto", "", "", "0");
	}
	else error();
}

//<���ʽ> ::= <��>{ +<��>|-<��>}
char * E()
{
	char op[max], *place1, *place2;
	char *place = (char *)malloc(max);

	place = place1 = T();
	while (syn == 22 || syn == 23)
	{
		sprintf(op, "%s", token);
		scaner();
		place2 = T();
		place = NewTemp();
		gen(op, place1, place2, place);
		place1 = place;
	}
	return place;
}

//<��> ::= <����>{*<����>|/<����>}
char * T()
{
	char op[max], *place1, *place2;
	char *place;

	place = place1 = F();
	while (syn == 24 || syn == 25)
	{
		sprintf(op, "%s", token);
		scaner();
		place2 = F();
		place = NewTemp();
		gen(op, place1, place2, place);
		place1 = place;
	}
	return place;
}

//<����> ::=ID|num|(<���ʽ>)
char * F()
{
	char *place = (char *)malloc(max);
	if (syn == 10)
	{
		sprintf(place, "%s", token);
		scaner();
	}
	else if (syn == 20)
	{
		sprintf(place, "%g", sum);
		scaner();
	}
	else if (syn == 26)
	{
		scaner();
		place = E();
		if (syn == 27) scaner();
		else error();
	}
	else error();
	return place;
}

/************************************************************************/
//������Ԫʽ
void gen(char *op, char *argv1, char *argv2, char *result)
{
	sprintf(pQuad[index].op, "%s", op);
	sprintf(pQuad[index].argv1, "%s", argv1);
	sprintf(pQuad[index].argv2, "%s", argv2);
	sprintf(pQuad[index].res, "%s", result);
	index++;
}
//������ʱ����
char *NewTemp()
{
	char *tmpID = (char *)malloc(max);
	sprintf(tmpID, "T%d", ++nSuffix);
	return tmpID;
}
//�ϲ�p1��p2
int merg(int p1, int p2)
{
	int p, nRes;

	if (p2 == 0) nRes = p1;
	else
	{
		p = p2;
		nRes = p2;
		while (atoi(pQuad[p].res))
		{
			p = atoi(pQuad[p].res);
			sprintf(pQuad[p].res, "%s", p1);
		}
	}
	return nRes;
}
//��t���pΪ�׵���Ԫʽ��
void bp(int p, int t)
{
	int w, q = p;
	while (q)
	{
		w = atoi(pQuad[q].res);
		sprintf(pQuad[q].res, "%d", t);
		q = w;
	}
	return;
}
//��ӡ��Ԫʽ���е��ļ�������̨���
void printQuad()
{
	int n;
	FILE *fw = fopen("quaternion.txt", "w");
	printf("��Ԫʽ�������£�\n");
	for (n = 1; n<index; n++)
	{
		fprintf(fw, "\n%2d:  %7s,%5s,%5s,%5s",
			n, pQuad[n].op, pQuad[n].argv1, pQuad[n].argv2, pQuad[n].res);
		printf("\n%2d:  %7s,%5s,%5s,%5s",
			n, pQuad[n].op, pQuad[n].argv1, pQuad[n].argv2, pQuad[n].res);
	}
	fclose(fw);
}

/************************************************************************/
//�﷨����������������м��������������
void Parse()
{
	int i;
	p = 0;
	flag = 0;
	//���������Ϣ��
	variable = (char**)malloc(strlen(prog)*sizeof(char*));
	for (i = 0; i<strlen(prog); i++) variable[i] = (char *)malloc(max);
	nVar = 0;
	//��Ԫʽ����
	pQuad = (quad *)malloc(strlen(prog)*sizeof(quad));
	index = 1;
	//��Ԫʽ��ʱ�������
	nSuffix = 0;

	P();
	if (syn == 0)
	{
		printf("Success!\n");
		printQuad();
	}
	else printf("Fail!\n");
}


/************************************************************************/
/*                    Ŀ��������ɣ�������ԣ�                         */
/************************************************************************/
typedef struct assembly_command {
	char Lable[max];
	char OP[max];
	char OPD[max];
	char OPS[max];
}assemb;//���ָ��
assemb *pAssemb;//���ָ������ָ��
int indexA;//���ָ����
int* lable;//��Ԫʽ�������Ӧ���ָ����
char** registerT;//��ʱ������ַ������
char* registerName[7] = { "AX","BX","CX","DX","BP","SI","DI" };//ͨ�üĴ�����
int registerStatus[7];//ͨ�üĴ���������0����δʹ�ã�1������ʹ��
					  /************************************************************************/
					  //���ɻ��ָ��
void genA(char *OP, char *OPD, char *OPS, char *Lable)
{
	sprintf(pAssemb[indexA].OP, "%s", OP);
	sprintf(pAssemb[indexA].OPD, "%s", OPD);
	sprintf(pAssemb[indexA].OPS, "%s", OPS);
	sprintf(pAssemb[indexA].Lable, "%s", Lable);
	indexA++;
}

//������мĴ���
char* GetfreeR()
{
	int i;
	char *reg = (char *)malloc(max);
	for (i = 0; i<7; i++)
	{
		if (registerStatus[i] == 0)
		{
			sprintf(reg, "%s", registerName[i]);
			registerStatus[i] = 1;
			return reg;
		}
	}
	reg = "Full";
	return reg;
}

//����Ϊ��ʱ������Ϊδ����Ĵ�������ʱ��������Ĵ��������������ڼĴ���
//����Ϊ�����������������򷵻ر���
char* Place(char *var)
{
	if (var[0] == 'T')
	{
		char *place = (char *)malloc(max);
		if (registerT[atoi(var + 1)] == NULL)
		{
			place = GetfreeR();
			registerT[atoi(var + 1)] = place;
		}
		else
		{
			place = registerT[atoi(var + 1)];
		}
		return place;
	}
	else
		return var;

}
//���������������ת��ָ���б��label
void bpAll()
{
	int i;
	int nLable = 0;
	int tmp;
	for (i = 1; i<indexA; i++)
	{
		if (pAssemb[i].OP[0] == 'J')
		{
			tmp = lable[atoi(pAssemb[i].OPD)];
			sprintf(pAssemb[tmp].Lable, "L%d:", ++nLable);
			sprintf(pAssemb[i].OPD, "L%d", nLable);
		}
	}
}
//��������ļ����ɡ�����̨���
void printAssemb()
{
	int n;
	FILE *fw = fopen("Assembly.asm", "w");
	//���αָ��
	fprintf(fw, ".386");
	fprintf(fw, "\nDATA\tSEGMENT USE16");
	for (n = 1; n <= nVar; n++)
		fprintf(fw, "\n%s\tDW  0", variable[n]);
	fprintf(fw, "\nDATA\tENDS");
	fprintf(fw, "\nSTACK\tSEGMENT USE16 STACK");
	fprintf(fw, "\n\tDB 200 DUP(0)");
	fprintf(fw, "\nSTACK\tENDS");
	fprintf(fw, "\nCODE\tSEGMENT USE16");
	fprintf(fw, "\n\tASSUME  DS:DATA,SS:STACK,CS:CODE");
	//���ָ��
	fprintf(fw, "\nSTART:\tMOV\tAX,\tDATA");
	fprintf(fw, "\n\tMOV\tDS,\tAX");
	printf("\n\n���ָ���������£�\n");
	for (n = 1; n<indexA; n++)
	{
		if (!strcmp(pAssemb[n].OPS, ""))
		{
			fprintf(fw, "\n%s\t%s\t%s",
				pAssemb[n].Lable, pAssemb[n].OP, pAssemb[n].OPD);
			printf("\n%2d:%5s %5s %5s",
				n, pAssemb[n].Lable, pAssemb[n].OP, pAssemb[n].OPD);
		}
		else
		{
			fprintf(fw, "\n%s\t%s\t%s,\t%s",
				pAssemb[n].Lable, pAssemb[n].OP, pAssemb[n].OPD, pAssemb[n].OPS);
			printf("\n%2d:%5s %5s %5s,%5s",
				n, pAssemb[n].Lable, pAssemb[n].OP, pAssemb[n].OPD, pAssemb[n].OPS);
		}
	}
	fprintf(fw, "\n\tMOV\tAH,\t4CH");
	fprintf(fw, "\n\tINT\t21H");
	//���αָ��
	fprintf(fw, "\nCODE\tENDS");
	fprintf(fw, "\n\tEND START");
	fclose(fw);
}

/************************************************************************/
//Ŀ��������������򣬽����ɵ���Ԫʽ�м����ת����X86ƽ̨�ϵ�Ŀ�����
void toAssembly()
{
	int n, i;
	pAssemb = (assemb *)malloc(strlen(prog)*sizeof(assemb));
	for (i = 0; i<7; i++)  registerStatus[i] = 0;
	registerT = (char **)malloc((nSuffix + 1)*sizeof(char *));
	for (n = 1; n <= nSuffix; n++)   registerT[n] = NULL;
	lable = (int *)malloc(index);
	indexA = 1;

	for (n = 1; n<index; n++)
	{
		lable[n] = indexA;
		switch (pQuad[n].op[0])
		{
		case 'g':
			if (!strcmp(pQuad[n].op, "goto"))
			{
				if (atoi(pQuad[n].res) == n + 1)   continue;
				else genA("JMP", pQuad[n].res, "", "");
			}
			else
			{
				if (isdigit(pQuad[n].argv1[0] || (pQuad[n].argv1[0] != 'T'&&!isdigit(pQuad[n].argv2[0]) && pQuad[n].argv2[0] != 'T')))//��������Ŀ�����������������ͬΪ�洢��������
				{
					char *R = GetfreeR();
					genA("MOV", R, Place(pQuad[n].argv1), "");
					genA("CMP", R, Place(pQuad[n].argv2), "");
				}
				else
				{
					genA("CMP", Place(pQuad[n].argv1), Place(pQuad[n].argv2), "");
				}
				if (!strcmp(pQuad[n].op, "goto =="))      genA("JE", pQuad[n].res, "", "");
				else if (!strcmp(pQuad[n].op, "goto !=")) genA("JNE", pQuad[n].res, "", "");
				else if (!strcmp(pQuad[n].op, "goto >=")) genA("JGE", pQuad[n].res, "", "");
				else if (!strcmp(pQuad[n].op, "goto >"))  genA("JG", pQuad[n].res, "", "");
				else if (!strcmp(pQuad[n].op, "goto <=")) genA("JLE", pQuad[n].res, "", "");
				else if (!strcmp(pQuad[n].op, "goto <"))  genA("JL", pQuad[n].res, "", "");
			}
			break;
		case '=':
			if (!isdigit(pQuad[n].argv1[0]) && pQuad[n].argv1[0] != 'T')//Դ������Ϊ�洢��������
			{
				char *R = GetfreeR();
				genA("MOV", R, pQuad[n].argv1, "");
				genA("MOV", pQuad[n].res, R, "");
			}
			else
			{
				genA("MOV", pQuad[n].res, Place(pQuad[n].argv1), "");
			}
			for (i = 0; i<7; i++)  registerStatus[i] = 0;
			break;
		case '+':
			if (pQuad[n].argv1[0] != 'T')//Ŀ�Ĳ�������Ϊ�Ĵ���������
			{
				char *R = GetfreeR();
				genA("MOV", R, pQuad[n].argv1, "");
				genA("ADD", R, Place(pQuad[n].argv2), "");
				if (pQuad[n].res[0] == 'T')  registerT[atoi(pQuad[n].res + 1)] = R;
			}
			else
			{
				genA("ADD", Place(pQuad[n].argv1), Place(pQuad[n].argv2), "");
				if (pQuad[n].res[0] == 'T')  registerT[atoi(pQuad[n].res + 1)] = Place(pQuad[n].argv1);
			}
			break;
		case '-':
			if (pQuad[n].argv1[0] != 'T')//Ŀ�Ĳ�������Ϊ�Ĵ���������
			{
				char *R = GetfreeR();
				genA("MOV", R, pQuad[n].argv1, "");
				genA("SUB", R, Place(pQuad[n].argv2), "");
				if (pQuad[n].res[0] == 'T') registerT[atoi(pQuad[n].res + 1)] = R;
			}
			else
			{
				genA("SUB", Place(pQuad[n].argv1), Place(pQuad[n].argv2), "");
				if (pQuad[n].res[0] == 'T')  registerT[atoi(pQuad[n].res + 1)] = Place(pQuad[n].argv1);
			}
			break;
		case '*':
			if (pQuad[n].argv1[0] != 'T')//Ŀ�Ĳ�������Ϊ�Ĵ���������
			{
				char *R = GetfreeR();
				genA("MOV", R, pQuad[n].argv1, "");
				genA("IMUL", R, Place(pQuad[n].argv2), "");
				if (pQuad[n].res[0] == 'T') registerT[atoi(pQuad[n].res + 1)] = R;
			}
			else
			{
				genA("IMUL", Place(pQuad[n].argv1), Place(pQuad[n].argv2), "");
				if (pQuad[n].res[0] == 'T')  registerT[atoi(pQuad[n].res + 1)] = Place(pQuad[n].argv1);
			}
			break;
		case '/':
		{
			int flag = strcmp(Place(pQuad[n].argv1), "AX");
			int aStatus;
			if (flag)
			{
				aStatus = registerStatus[0];
				registerStatus[0] = 1;
				if (aStatus) genA("PUSH", "AX", "", "");
			}
			int dStatus = registerStatus[3];
			registerStatus[3] = 1;
			if (dStatus) genA("PUSH", "DX", "", "");

			if (flag)   genA("MOV", "AX", Place(pQuad[n].argv1), "");
			genA("CWD", "", "", "");
			if (isdigit(atoi(pQuad[n].argv2)))//����Ϊ������
			{
				char *R = GetfreeR();
				genA("MOV", R, pQuad[n].argv2, "");
				genA("IDIV", R, "", "");
			}
			else genA("IDIV", pQuad[n].argv2, "", "");
			if (pQuad[n].res[0] == 'T') registerT[atoi(pQuad[n].res + 1)] = "AX";

			registerStatus[3] = dStatus;
			if (dStatus) genA("POP", "DX", "", "");
			if (flag)
			{
				registerStatus[0] = aStatus;
				if (aStatus) genA("POP", "AX", "", "");
			}
		}
		break;
		default:
			printf("error!");
		}

	}
	bpAll();
	printAssemb();

}
/************************************************************************/
/*                              ���س���                                */
/************************************************************************/
int main()
{
	int times = 10;
	do
	{
		printf("Please input program :\n");
		//����Դ����
		p = 0;
		int i;
		for (i = 0; i<80; i++) prog[i] = '\0';
		do
		{
			ch = getchar();
			prog[p++] = ch;
		} while (ch != '#');
		//����               
		Parse();
		toAssembly();
		printf("\n\n");
	} while (times-- > 0);
}