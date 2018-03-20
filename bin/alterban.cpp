#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char read=' ';
char token[10] = {};
char *p ;
char *t=token;
int n;
enum Symbol{BEGINSY=1, ENDSY=2, IFSY=3, THENSY=4, ELSESY=5, INTSYS=6, CHARSY=7, IDSY=20, INTSY=21, PLUSSY=22, MINUSSY=23, STARSY=24, DIVISY=25, LPARSY=26, RPARSY=27, COMMASY=28, SEMISY=29, COLONSY=30, ASSIGNSY=31, EQUSY=32, }symbol;
void clearToken()
{	
    memset(token,'\0',10);
    t=token;
    read=' ';
}

void getChar()
{ 
    read = *p;
    p++;
}

int isSpace()
{
    if (read == ' ')
        return 1;
    else
        return 0;
}

int isNewline()
{
    if (read == '\n')
        return 1;
    else
        return 0;
}

int isTab()
{
    if (read == '\t')
        return 1;
    else
        return 0;
}

int isLetter()
{
    if ((read>=65&&read<=90) || (read>=97 && read<=122))
        return 1;
    else
        return 0;
}

int isDigit()
{
    if (read>= 48 && read<=57)
        return 1;
    else
        return 0;
}

int isColon()
{
    if (read == ':')
        return 1;
    else
        return 0;
}

int isPlus()
{
    if (read == '+')
        return 1;
    else
        return 0;
}

int isMinus()
{
    if (read == '-')
        return 1;
    else
        return 0;
}

int isStar()
{
    if (read == '*')
        return 1;
    else
        return 0;
}

int isLpar()
{
    if (read == '(')
        return 1;
    else
        return 0;
}

int isRpar()
{
    if (read == ')')
        return 1;
    else
        return 0;
}

int isComma()
{
    if (read == ',')
        return 1;
    else
        return 0;
}

int isEqu()
{
    if (read == '=')
        return 1;
    else
        return 0;
}


int isSemi() 
{
    if (read == ';')
        return 1;
    else
        return 0;
}

int isDivi()
{
    if (read == '/')
        return 1;
    else
        return 0;
}

void catToken ()
{
	*t=read;
	t++;
}

void retract()
{
	p--;
}

int reserver()
{
    if(!strcmp(token,"BEGIN")) return 1;
    else if(!strcmp(token,"END")) return 2;
    else if(!strcmp(token,"IF")) return 3;
    else if(!strcmp(token,"THEN")) return 4;
    else if(!strcmp(token,"ELSE")) return 5;
    else if(!strcmp(token,"INT")) return 21;
    else if(!strcmp(token,"CHAR")) return 7;
    else 
		return 0;
}

int transNum()
{	
	int N=0;
	int i;
	for(i=0;token[i]!='\0';i++) N=(token[i]-'0')+10*N;
	return N;
	  
}

void error()
{
	printf("error");
}

int getsym()
{   
	clearToken();
	while(isSpace()||isNewline()||isTab())
		getChar();
	if (read ==';')
		return 0;
	if(isLetter())
	{
		while(isLetter()||isDigit())
			{
				catToken();
				getChar();
			} 
		retract();
		int result=reserver();
		if(result==0) 
			symbol=IDSY;
		else
			symbol=(enum Symbol)result;
		
	}
	else if(isDigit())
	{
		while(isDigit())
		{
			catToken();
			getChar();
		}
		retract();
		n=transNum();
		symbol=INTSY;		
	}
	else if(isColon())
	{
		getChar();
		if(isEqu())
			symbol=ASSIGNSY;
			else 
			{
				retract();
				symbol=COLONSY;
			}
	}
	else if(isPlus()) symbol=PLUSSY;
	else if(isMinus()) symbol=MINUSSY;
	else if(isStar())	symbol=STARSY;
	else if(isLpar()) symbol=LPARSY;
	else if(isRpar()) symbol=RPARSY;
	else if(isComma()) symbol=COMMASY;
	else if(isSemi()) symbol=SEMISY;
	
	else if(isDivi())
	{  
		getChar();
		if(isStar())
		{
			do{
				do{
					getChar();
				}while(!isStar());
				do{
					getChar();
					if(isDivi())
						return 0;
				}while(isStar());
			} while(!isStar());
			
	
			
		}	
		retract();
        symbol = DIVISY;
}
	else 
		error();
	return 0;
	
}  



struct table  {
	char id[20];
	char att[20];
	char date[20];
	struct table *next;
}item[10];

 struct table *hash[10];
 struct table *sonProce[10];
 int hashN[10]={0};
 int prtT=0;
 int prtS=0;
 

  
 void gets()
 {	int start=0;
    int Put;

	while(*p !='\0')
 	{
	    getsym();
		 if(symbol==BEGINSY)
 			start=1;
 		else if (symbol==7)
 			Put=1;
 		else if (symbol==21)
 			Put=-1;
 		else if(symbol==IDSY)
 			{	
			     int i=0;

 				while(*(token+i)!=0)
 	    		{	   
				 	item[prtT].id[i]=token[i];
				 	i++;
 	    
 	    		}
 	    		if(Put==-1) strcpy(item[prtT].att,"int") ;
 	    		if(Put==1)  strcpy(item[prtT].att,"char"); 
 	    		if(start==1)
 	    		{
 	    			start=0;
 	    			sonProce[prtS]=&item[prtT];
 	    			prtS++;
				 }
				 
				 
				 
			
			 	prtT++;
			 }
			 
	  	else if(symbol==31)
				 {	getsym();
				 	strcpy(item[prtT].date,token);
				 }
	
	     else if(symbol==ENDSY)
		{
			struct table *add = sonProce[prtS-1];
			memset(add,0,200);
			prtS--;
		} 
		
	
	 } 
	 

 }
 
void hashID()
{
	int i=0;
	while(item[i].id[0]!=0) 
	{
		int j=0;
		int asc=0;
		while(item[i].id[j]!=0)
		{
			asc+=item[i].id[j];
			j++;
		}
		if(hash[asc%10]==NULL)
			{
			hash[asc%10]=&item[i];hashN[asc%10]++;}
		else  
		{	hashN[asc%10]++;
			hash[asc%10]->next= &item[i];
		} 
		i++;
	}

}

int main()
{
    char input[100] = "BEGIN INT yb :=tlx	BEGIN CHAR Cs := 200 BEGIN INT A,B CHAR K INT D";
    p = input;

	gets();
  

  
	hashID();
	printf("符号表:	   ");
	for(int i=0; i<10; i++)
	{		
			
		if(strcmp(item[i].id, ""))
		{	printf("%s  %s  ", item[i].id,item[i].att);}
		if(strcmp(item[i].date, ""))
			printf("=%s   ", item[i].date);
		
	}
	printf("\n");
	printf("Hash:   ");
	for(int i=0; i<10; i++)
	{
		printf("%d ", hash[i]);
	}
		printf("\n");

	printf("子程序索引:  "); 
	for(int i=0; i<10; i++)
	{
		printf("%d ", sonProce[i]);
	}
	printf("\n");   
}
