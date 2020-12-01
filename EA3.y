%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "y.tab.h"
    #include "./lib/tabla_simbolos.h"
    #include "./lib/tercetos.h"
    #include "./lib/assembler.h"

	/* Funciones necesarias */
	int yyerror(char* mensaje);
	int yyerror();
	int yylex();

    int yystopparser=0;
    FILE  *yyin;

    /* Tabla de simbolos */
	simbolo tabla_simbolo[TAMANIO_TABLA];
    int fin_tabla = -1;

    /* Variables para asignaciones */
    char pivot[TAM_NOMBRE];
    int esListaVacia;
    int cantElementos;

    /* Indices para no terminales */
	int ind_escritura;
    int ind_lectura;
    int ind_posEncontrada;
    int ind_asigna;
    int ind_lista;
    int ind_posicion;
    int ind_sent;
    int ind_start;
    int ind_programa;
    int ind_listaVacia;
    int ind_cota_pivot;
    int aux_ind;
    int ind_finProg;
    int ind_salto;

    /* Variables para tercetos */
	terceto lista_terceto[MAX_TERCETOS];
	int ultimo_terceto = -1;

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
                                                            ind_finProg = crear_terceto(JMP,NOOP,NOOP);
                                                            ind_start = ind_programa;
                                                            int pos = agregarCteStringATabla("El valor debe ser >=1");
                                                            crear_terceto(ETIQUETA,NOOP,NOOP);
                                                            modificarTerceto(ind_cota_pivot,OP1,ultimo_terceto + OFFSET);
                                                            crear_terceto(WRITE,pos,NOOP);
                                                            if(esListaVacia){
                                                                pos= agregarCteStringATabla("La lista esta vacia");
                                                                crear_terceto(ETIQUETA,NOOP,NOOP);
                                                                modificarTerceto(ind_listaVacia,OP1,ultimo_terceto + OFFSET);
                                                                ind_escritura = crear_terceto(WRITE, pos, NOOP);
                                                                ind_salto = crear_terceto(JMP, NOOP,NOOP);
                                                            }else{
                                                                ind_salto = crear_terceto(JMP, NOOP,NOOP);
                                                                pos = agregarCteStringATabla("Elemento no encontrado");
                                                                crear_terceto(ETIQUETA,NOOP,NOOP);
                                                                modificarTerceto(aux_ind,OP1,ultimo_terceto+OFFSET);
															    crear_terceto(WRITE,pos,NOOP);
                                                            }

                                                            //Creo terceto fin 
                                                            pos = agregarCteStringATabla("FIN PROGRAMA");
                                                            crear_terceto(ETIQUETA,NOOP,NOOP);

                                                            //MODIFICAR TERCETOS
                                                            modificarTerceto(ind_finProg,OP1,ultimo_terceto + OFFSET);
                                                            modificarTerceto(ind_salto,OP1,ultimo_terceto + OFFSET);

                                                            crear_terceto(WRITE,pos,NOOP);
                                                            
                                                            optimizarTercetos();
                                                            guardarTabla();
                                                            guardarTercetos();
                                                            generarAssembler();
														};
                                                       
/* Seccion de codigo */
programa:                                                 
	programa sentencia	                                {ind_programa = ind_sent;}
	| sentencia			                                {ind_programa = ind_sent;};

sentencia:
	asignacion			                                {
															printf("--------------FIN ASIGNACION--------------\n");
                                                            ind_sent = ind_asigna;
														}
	| lectura                                           {
															printf("--------------FIN LECTURA--------------\n");
                                                            ind_sent = ind_lectura;
														}
	| escritura                                         {
															printf("--------------FIN ESCRITURA--------------\n");
                                                            ind_sent = ind_escritura;
														};
/* Otras cosas */

asignacion:
	ID ASIGNA posicion	    {
															printf("Regla 5: ID = posicion es ASIGNACION\n");
															
                                                            if(!esListaVacia){
																int cte = agregarCteIntATabla(0);
																int pos = buscarIDEnTabla("@pos");
                                                                int id = agregarVarATabla($1,Int);
                                                                crear_terceto(ETIQUETA,NOOP, NOOP);
																crear_terceto(CMP,pos,cte);
																aux_ind = crear_terceto(BEQ,NOOP,NOOP);
                                                                ind_asigna = crear_terceto(ASIGNA,id,pos);
                                                            }
                                                            
														};
posicion:
    POSICION PARA ID PYC CA {strcpy(pivot, $3);} lista CC PARC      {
                                                                        printf("Regla 6: POSICION PARA ID PYC CA lista CC PARC es posicion\n");
                                                                        esListaVacia = 0;
                                                                        ind_posicion = ind_lista;
                                                                    }
    | POSICION PARA ID PYC CA CC PARC                   {
                                                            printf("Regla 7: POSICION PARA ID PYC CA CC PARC es posicion\n");
                                                            esListaVacia = 1;
                                                            ind_listaVacia = crear_terceto(JMP, NOOP, NOOP);
                                                        };
lista:
    lista COMA CTE                                      {
                                                            printf("Regla 9: lista COMA CTE es lista\n");
                                                            cantElementos++;
                                                            int cte = agregarCteIntATabla(yylval.int_val);
                                                            int posicion = buscarIDEnTabla("@pos");
                                                            int pivote = buscarIDEnTabla(pivot);
                                                            crear_terceto(ETIQUETA,NOOP, NOOP);
                                                            crear_terceto(CMP,pivote, cte);
                                                            int incremento = ultimo_terceto+SALTO_POSICION;
                                                            crear_terceto(BNE,incremento+OFFSET, NOOP); 
                                                            crear_terceto(CMP,posicion,agregarCteIntATabla(0));
                                                            ind_posEncontrada = crear_terceto(BNE,NOOP, NOOP);
															cte = agregarCteIntATabla(cantElementos);
                                                            ind_lista = crear_terceto(ASIGNA,posicion,cte);
                                                            modificarTerceto(ind_posEncontrada,OP1,incremento+OFFSET);	   
                                                        }             
    | CTE                                               {
                                                            printf("Regla 8: CTE es lista\n");
                                                            cantElementos = 1;
                                                            int posicion = agregarVarATabla("@pos", Int);
                                                            int cte = agregarCteIntATabla(yylval.int_val);
                                                            int pos = agregarCteIntATabla(0);
                                                            
                                                            int pivote = buscarIDEnTabla(pivot);
															crear_terceto(ASIGNA, posicion, pos); //Inicializo @pos en 0
                                                            crear_terceto(CMP, pivote, cte);
															int incremento = ultimo_terceto+SALTO_POSICION;
                                                            crear_terceto(BNE,incremento+OFFSET, NOOP); 
                                                            pos = agregarCteIntATabla(cantElementos);
                                                            cte = agregarCteIntATabla(0);
                                                            crear_terceto(CMP,posicion,cte);
                                                            ind_posEncontrada = crear_terceto(BNE,NOOP, NOOP);
                                                            ind_lista = crear_terceto(ASIGNA,posicion,pos);
                                                            modificarTerceto(ind_posEncontrada,OP1,incremento+OFFSET);								
                                                        };
lectura:
    READ ID												{
        													printf("Regla 4: READ ID es lectura\n");
                                                            agregarVarATabla(yylval.string_val,Int);
                                                            int cota = agregarCteIntATabla(1);
                                                            int pos = buscarIDEnTabla($2);
															ind_lectura = crear_terceto(READ, pos, NOOP);
                                                            crear_terceto(CMP,pos,cota);
                                                            ind_cota_pivot=crear_terceto(BLT, NOOP, NOOP);
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
                                                            int pos= agregarCteStringATabla($2);
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
