#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <locale>
#include <conio.h>


/************************************************************************/
/*                           词法分析                                  */
/************************************************************************/
#define max 10

char *rwtab[9] = { "main","int","float","double","char","if","else","do","while" };

char prog[100];//源程序
int p;//当前处理字符位置
char ch; //当前处理字符
int flag; //1表示刚读取一个变量或常数，"+/-"为运算符;0反之，"+/-"可能为数值符号

int syn;       //种别编码
char token[max]; //保留字、内部字符串或操作符
double sum;    //数值

char** variable;//变量信息表
int nVar;
/************************************************************************/
void scaner()
{
	int i;
	for (i = 0; i<max; i++) token[i] = '\0';
	sum = 0;

	int m = 0;
	int e = 0;//数值指数

	ch = prog[p++];
	while (isspace(ch)) ch = prog[p++];//预处理，去除注释、多余空格、回车换行符等      

	if (isalpha(ch))//保留字、内部字符串
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

	else if (ch == '+' || ch == '-' || isdigit(ch))//数值、"+"、"-"
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

	else//运算符、分隔符
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
/*                         语法语义分析                                 */
/************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////
//                          递归下降法                                 //
/////////////////////////////////////////////////////////////////////////////////////////////
void P();  //程序
void B(int *nChain);  //语句块
void SS(int *nChain);  //语句串
void S(int *nChain);  //语句
void AS(int *nChain);  //赋值语句
void CS(int *nChain);  //条件语句
void LS(int *nChain);  //循环语句
void C(int *etc, int *efc);  //条件
char * E();  //表达式
char * T();  //项
char * F();  //因子
void error(); //出错处理
			  //////////////////////////////////////////////////////////////////////////////////////////////
			  //                          语法制导翻译                               //
			  /////////////////////////////////////////////////////////////////////////////////////////////
typedef struct quaternion {
	char op[max];
	char argv1[max];
	char argv2[max];
	char res[max];
}quad;//四元式
quad *pQuad;//四元式组指针
int index, nSuffix;//四元式编号,临时变量编号
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
//<程序> ::= main()<语句块> 
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

//<语句块> ::= ‘{‘<语句串>’}’
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

//<语句串>::=<语句>{;<语句>};
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

//<语句>::=<赋值语句>|<条件语句>|<循环语句>
void S(int *nChain)
{
	if (syn == 10) AS(nChain);
	else if (syn == 6) CS(nChain);
	else if (syn == 8) LS(nChain);
	else error();
}

//<赋值语句>::=ID=<表达式>
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

//<条件语句>::=if<条件><语句块>[else <语句块>]
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

//<循环语句>::=do <语句块>while <条件>
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

//<条件>::=<表达式><关系运算符><表达式>
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

//<表达式> ::= <项>{ +<项>|-<项>}
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

//<项> ::= <因子>{*<因子>|/<因子>}
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

//<因子> ::=ID|num|(<表达式>)
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
//生成四元式
void gen(char *op, char *argv1, char *argv2, char *result)
{
	sprintf(pQuad[index].op, "%s", op);
	sprintf(pQuad[index].argv1, "%s", argv1);
	sprintf(pQuad[index].argv2, "%s", argv2);
	sprintf(pQuad[index].res, "%s", result);
	index++;
}
//产生临时变量
char *NewTemp()
{
	char *tmpID = (char *)malloc(max);
	sprintf(tmpID, "T%d", ++nSuffix);
	return tmpID;
}
//合并p1、p2
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
//将t回填到p为首的四元式链
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
//打印四元式序列到文件，控制台输出
void printQuad()
{
	int n;
	FILE *fw = fopen("quaternion.txt", "w");
	printf("四元式序列如下：\n");
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
//语法分析、语义分析和中间代码生成主程序
void Parse()
{
	int i;
	p = 0;
	flag = 0;
	//程序变量信息表
	variable = (char**)malloc(strlen(prog)*sizeof(char*));
	for (i = 0; i<strlen(prog); i++) variable[i] = (char *)malloc(max);
	nVar = 0;
	//四元式序列
	pQuad = (quad *)malloc(strlen(prog)*sizeof(quad));
	index = 1;
	//四元式临时变量编号
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
/*                    目标代码生成（汇编语言）                         */
/************************************************************************/
typedef struct assembly_command {
	char Lable[max];
	char OP[max];
	char OPD[max];
	char OPS[max];
}assemb;//汇编指令
assemb *pAssemb;//汇编指令序列指针
int indexA;//汇编指令编号
int* lable;//四元式编号所对应汇编指令编号
char** registerT;//临时变量地址描述表
char* registerName[7] = { "AX","BX","CX","DX","BP","SI","DI" };//通用寄存器表
int registerStatus[7];//通用寄存器描述表，0代表未使用，1代表在使用
					  /************************************************************************/
					  //生成汇编指令
void genA(char *OP, char *OPD, char *OPS, char *Lable)
{
	sprintf(pAssemb[indexA].OP, "%s", OP);
	sprintf(pAssemb[indexA].OPD, "%s", OPD);
	sprintf(pAssemb[indexA].OPS, "%s", OPS);
	sprintf(pAssemb[indexA].Lable, "%s", Lable);
	indexA++;
}

//分配空闲寄存器
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

//参数为临时变量，为未分配寄存器的临时变量分配寄存器，返回其所在寄存器
//参数为立即数或程序变量，则返回本身
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
//回填汇编语句中所有转移指令中标号label
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
//汇编语言文件生成、控制台输出
void printAssemb()
{
	int n;
	FILE *fw = fopen("Assembly.asm", "w");
	//汇编伪指令
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
	//汇编指令
	fprintf(fw, "\nSTART:\tMOV\tAX,\tDATA");
	fprintf(fw, "\n\tMOV\tDS,\tAX");
	printf("\n\n汇编指令序列如下：\n");
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
	//汇编伪指令
	fprintf(fw, "\nCODE\tENDS");
	fprintf(fw, "\n\tEND START");
	fclose(fw);
}

/************************************************************************/
//目标代码生成主程序，将生成的四元式中间代码转换成X86平台上的目标代码
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
				if (isdigit(pQuad[n].argv1[0] || (pQuad[n].argv1[0] != 'T'&&!isdigit(pQuad[n].argv2[0]) && pQuad[n].argv2[0] != 'T')))//立即数做目标操作数或两操作数同为存储器操作数
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
			if (!isdigit(pQuad[n].argv1[0]) && pQuad[n].argv1[0] != 'T')//源操作数为存储器操作数
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
			if (pQuad[n].argv1[0] != 'T')//目的操作数不为寄存器操作数
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
			if (pQuad[n].argv1[0] != 'T')//目的操作数不为寄存器操作数
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
			if (pQuad[n].argv1[0] != 'T')//目的操作数不为寄存器操作数
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
			if (isdigit(atoi(pQuad[n].argv2)))//除数为立即数
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
/*                              主控程序                                */
/************************************************************************/
int main()
{
	int times = 10;
	do
	{
		printf("Please input program :\n");
		//输入源程序
		p = 0;
		int i;
		for (i = 0; i<80; i++) prog[i] = '\0';
		do
		{
			ch = getchar();
			prog[p++] = ch;
		} while (ch != '#');
		//编译               
		Parse();
		toAssembly();
		printf("\n\n");
	} while (times-- > 0);
}