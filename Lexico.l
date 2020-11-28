%{
    #include <stdio.h>
    #include <stdlib.h>
	#include <string.h>
    #include "y.tab.h"

	int error(char* mensaje);

	FILE  *yyin;
%}

%option noyywrap
%option yylineno

DIGITO 	[0-9]
LETRA 	[a-zA-ZñÑ]
ID		{LETRA}({LETRA}|{DIGITO})*
CTE		{DIGITO}+
WHATEVER	{LETRA}|{DIGITO}|.
CTE_S	\"{WHATEVER}*\"
ESPACIO     " "

%%
 /* Operadores */
"="						{ return ASIGNA;}

 /* Otros simbolos */
"("							{ return PARA;}
")"							{ return PARC;}
"["							{ return CA;}
"]"							{ return CC;}
","							{ return COMA;}
";"							{ return PYC;}

 /* Funciones */
"READ"						{ return READ;}
"WRITE"					    { return WRITE;}
"posicion"					{ return POSICION;}


 /* Identificadores y constantes */
{ID}						{
                                yylval.string_val = strdup(yytext);   
                                return ID;
                            }
	
{CTE}					    {
                                yylval.int_val = atoi(yytext);
                                return CTE;
                            }
						
{CTE_S}				        {
                                    yylval.string_val = strdup(yytext);
                                    return CTE_S;
                            }
								
{ESPACIO}
\n
\t
\r\n
.							{ error("Caracter invalido.");}

%%

int error(char* mensaje)
{
	printf("Error lexico: %s\n", mensaje);
	exit (1);
}

int yyerror(char* mensaje)
 {
	printf("Syntax Error (linea %d): %s\n", yylineno, mensaje);
	exit (1);
 }
