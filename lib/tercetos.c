#include <stdio.h>
#include <stdlib.h>
#include "tercetos.h"
#include "../y.tab.h"

/** Agrega un terceto a la lista de tercetos. Si se quiere guardar solo una constante o variable, mandar NOOP en
el campo de operador y op2. Para los operadores pasar el token literal, para los operandos pasar la posicion en
tabla de simbolos o el indice de otro terceto. */
int crear_terceto(int operador, int op1, int op2){
	ultimo_terceto++;
	if(ultimo_terceto >= MAX_TERCETOS){
		printf("Error: me quede sin espacio en para los tercetos. Optimiza tu codigo.\n");
		system("Pause");
		exit(3);
	}

	lista_terceto[ultimo_terceto].operador = operador;
	lista_terceto[ultimo_terceto].op1 = op1;
	lista_terceto[ultimo_terceto].op2 = op2;
	return ultimo_terceto + OFFSET;
}

/** Modifica el terceto con el indice indicado en la posicion indicada. El indice debe ser sin el OFFSET
y la posicion debe ser OP1, OP2 u OPERADOR. Si el terceto no existe, aborta la compilacion. */
void modificarTerceto(int indice, int posicion, int valor){
	if(indice > ultimo_terceto + OFFSET){
		printf("Hubo un error al modificar un terceto.");
		system ("Pause");
		exit (4);
	}
	switch(posicion){
	case OP1:
		lista_terceto[indice - OFFSET].op1 = valor;
		break;
	case OP2:
		lista_terceto[indice - OFFSET].op2 = valor;
		break;
	case OPERADOR:
		lista_terceto[indice - OFFSET].operador = valor;
		break;
	}
}

/* Guarda los tercetos generados en un archivo */
void guardarTercetos(){
	if(ultimo_terceto == -1)
		yyerror("Tercetos no encontrados");

	FILE* arch = fopen("intermedia.txt", "w+");
	if(!arch){
		printf("No pude crear el archivo intermedia.txt\n");
		return;
	}

	for(int i = 0; i <= ultimo_terceto; i++){
		//La forma es [i] (operador, op1, op2)

		//Si el operador es NOOP, es solo una ID o CTE que ya optimizamos, lo ignoro
		if(lista_terceto[i].operador != NOOP){
			//Escribo indice
			fprintf(arch, "[%d] (", i + OFFSET);

			//escribo operador
			switch(lista_terceto[i].operador){
			case NOOP:
				fprintf(arch, "---");
				break;
			case ASIGNA:
				fprintf(arch, "=");
				break;
			case ETIQUETA:
				fprintf(arch, "etiqueta");
				break;
			case READ:
				fprintf(arch, "READ");
				break;
			case WRITE:
				fprintf(arch, "WRITE");
				break;
			case CMP:
				fprintf(arch, "CMP");
				break;
			case BNE:
				fprintf(arch, "BNE");
				break;
			case BEQ:
				fprintf(arch, "BEQ");
				break;
			case BGT:
				fprintf(arch, "BGT");
				break;
			case BGE:
				fprintf(arch, "BGE");
				break;
			case BLE:
				fprintf(arch, "BLE");
				break;
			case BLT:
				fprintf(arch, "BLT");
				break;
			case JMP:
				fprintf(arch, "JMP");
				break;
			default:
				fprintf(arch, "No encontre el operador");
				break;
			}

			fprintf(arch, ", ");
			//Escribo op1
			int op = lista_terceto[i].op1;

			if(op == NOOP)
				fprintf(arch, "---");
			else if(op < TAMANIO_TABLA){
				//Es una entrada a tabla de simbolos
				fprintf(arch, "%s", (tabla_simbolo[op].nombre) );
			}
			else //Es el indice de otro terceto
				fprintf(arch, "[%d]", op);

			fprintf(arch, ", ");
			//Escribo op2
			op = lista_terceto[i].op2;
			if(op == NOOP)
				fprintf(arch, "---");
			else if(op < TAMANIO_TABLA){
				//Es una entrada a tabla de simbolos
				fprintf(arch, "%s", (tabla_simbolo[op].nombre) );
			}
			else //Es el indice de otro terceto
				fprintf(arch, "[%d]", op);

			fprintf(arch, ")\n");
		}
	}
	fclose(arch);
}

/**
* Hay tercetos que solo tienen una CTE o ID. En esos casos subimos la CTE o ID
* a los tercetos que las usan.
*/
void optimizarTercetos(){
	//Recorro todos los tercetos
	for(int i=0; i<MAX_TERCETOS; i++){
		//Por cada terceto...

		//Si el operando 1 es otro terceto...
		if(lista_terceto[i].op1 >= OFFSET){
			//Busco el terceto referenciado
			int ref = lista_terceto[i].op1 - OFFSET;
			if(lista_terceto[ref].operador == NOOP) //Si el referenciado solo tiene una ID o CTE
				// Reemplazo la referencia al terceto por una referencia a la ID o CTE directamente
				lista_terceto[i].op1 = lista_terceto[ref].op1;
		}

		//Si el operando 2 es otro terceto...
		if(lista_terceto[i].op2 >= OFFSET){
			//Busco el terceto referenciado
			int ref = lista_terceto[i].op2 - OFFSET;
			if(lista_terceto[ref].operador == NOOP) //Si el referenciado solo tiene una ID o CTE
				// Reemplazo la referencia al terceto por una referencia a la ID o CTE directamente
				lista_terceto[i].op2 = lista_terceto[ref].op1;
		}
	}
}
