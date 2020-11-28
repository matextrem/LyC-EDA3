%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "y.tab.h"
    #include "./lib/tabla_simbolos.h"
    #include "./lib/tercetos.h"

	/* Funciones necesarias */
	int yyerror(char* mensaje);
	int yyerror();
	int yylex();

    int yystopparser=0;
    FILE  *yyin;

    /* Cosas de tabla de simbolos */
	simbolo tabla_simbolo[TAMANIO_TABLA];
    int fin_tabla = -1;

    /* Cosas para las asignaciones */
	char idAsignar[TAM_NOMBRE];

    /* Indices para no terminales */
	int ind_escritura;
    int ind_lectura;
    int ind_asigna;
    int ind_pos;

    /* Cosas para tercetos */
	terceto lista_terceto[MAX_TERCETOS];
	int ultimo_terceto = -1; /* Apunta al ultimo terceto escrito. Incrementarlo para guardar el siguiente. */

%}

%union {
	int int_val;
	char *string_val;
}

%token ASIGNA

%token PARA PARC
%token CA CC
%token COMA
%token PYC

%token READ
%token WRITE
%token POSICION

%token <string_val>ID
%token <int_val>CTE
%token <string_val>CTE_S

%%
/* start */
start:
	programa									        {
                                                            printf("\n--------------FIN PROGRAMA--------------\n");
                                                            optimizarTercetos();
                                                            guardarTabla();
                                                            guardarTercetos();
														};
                                                       
/* Seccion de codigo */
programa:                                                 /* No existen bloques sin sentencias */
	programa sentencia	                                {}
	| sentencia			                                {};

sentencia:
	asignacion			                                {
															printf("--------------FIN ASIGNACION--------------\n");
														}
	| lectura                                           {
															printf("--------------FIN LECTURA--------------\n");
														}
	| escritura                                         {
															printf("--------------FIN ESCRITURA--------------\n");
														};
/* Otras cosas */

asignacion:
	ID ASIGNA {{strcpy(idAsignar, $1);}} posicion	    {
															printf("Regla 5: ID = posicion es ASIGNACION\n");
                                                            //int pos=buscarIDEnTabla(idAsignar);
															//ind_asigna = crear_terceto(ASIGNA, pos, ind_pos)
														};
posicion:
    POSICION PARA ID PYC CA lista CC PARC               {
                                                            printf("Regla 6: POSICION PARA ID PYC CA lista CC PARC es posicion\n");
                                                        }
    | POSICION PARA ID PYC CA CC PARC                   {
                                                            printf("Regla 7: POSICION PARA ID PYC CA CC PARC es posicion\n");
                                                        };
lista:
    lista COMA CTE                                      {
                                                            printf("Regla 9: lista COMA CTE es lista\n");
                                                            agregarCteIntATabla(yylval.int_val);
                                                        }             
    | CTE                                               {
                                                            printf("Regla 8: CTE es lista\n");
                                                            agregarCteIntATabla(yylval.int_val);
                                                        };
lectura:
    READ ID												{
        													printf("Regla 4: READ ID es lectura\n");
                                                            agregarVarATabla(yylval.string_val,Int);
                                                            int pos = buscarIDEnTabla($2);
															ind_lectura = crear_terceto(READ, pos, NOOP);
														};
escritura:
    WRITE ID                                            {
															printf("Regla 11: WRITE ID es escritura\n");
                                                            agregarVarATabla(yylval.string_val,Int);
                                                            int pos = buscarIDEnTabla($2);
															ind_escritura = crear_terceto(WRITE, pos, NOOP);
														}
    | WRITE CTE_S                                       {
        													printf("Regla 10: WRITE CTE_S es escritura\n");
                                                            int pos= agregarCteStringATabla(yylval.string_val);
                                                            ind_escritura = crear_terceto(WRITE, pos, NOOP);
														};
%%

int main(int argc,char *argv[])
{
  if ((yyin = fopen(argv[1], "rt")) == NULL)
  {
	printf("\nNo se puede abrir el archivo: %s\n", argv[1]);
  }
  else
  {
	yyparse();
  	fclose(yyin);
  }
  printf("\n* COMPILACION EXITOSA *\n");
  return 0;
}
