#include <stdio.h>
#include <stdlib.h>
#include "tercetos.h"
#include "../y.tab.h"

int crear_terceto(int operador, int op1, int op2){
	ultimo_terceto++;
	if(ultimo_terceto >= MAX_TERCETOS){
		printf("Error: No hay espacio para crear tercetos.\n");
		system("Pause");
		exit(3);
	}

	lista_terceto[ultimo_terceto].operador = operador;
	lista_terceto[ultimo_terceto].op1 = op1;
	lista_terceto[ultimo_terceto].op2 = op2;
	return ultimo_terceto + OFFSET;
}


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

void guardarTercetos(){
	if(ultimo_terceto == -1)
		yyerror("Tercetos no encontrados");

	FILE* arch = fopen("intermedia.txt", "w+");
	if(!arch){
		printf("No pude crear el archivo intermedia.txt\n");
		return;
	}
	int i;
	for(i = 0; i <= ultimo_terceto; i++){
		
		if(lista_terceto[i].operador != NOOP){
			fprintf(arch, "[%d] (", i + OFFSET);

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
				fprintf(arch, "No se encontro el operador");
				break;
			}

			fprintf(arch, ", ");
			int op = lista_terceto[i].op1;

			if(op == NOOP)
				fprintf(arch, "---");
			else if(op < TAMANIO_TABLA){
				fprintf(arch, "%s", (tabla_simbolo[op].nombre) );
			}
			else
				fprintf(arch, "[%d]", op);

			fprintf(arch, ", ");
			op = lista_terceto[i].op2;
			if(op == NOOP)
				fprintf(arch, "---");
			else if(op < TAMANIO_TABLA){
				fprintf(arch, "%s", (tabla_simbolo[op].nombre) );
			}
			else
				fprintf(arch, "[%d]", op);

			fprintf(arch, ")\n");
		}
	}
	fclose(arch);
}

void optimizarTercetos(){
	int i;
	for(i=0; i<MAX_TERCETOS; i++){
		if(lista_terceto[i].op1 >= OFFSET){
			int ref = lista_terceto[i].op1 - OFFSET;
			if(lista_terceto[ref].operador == NOOP)
				lista_terceto[i].op1 = lista_terceto[ref].op1;
		}

		if(lista_terceto[i].op2 >= OFFSET){
			int ref = lista_terceto[i].op2 - OFFSET;
			if(lista_terceto[ref].operador == NOOP)
				lista_terceto[i].op2 = lista_terceto[ref].op1;
		}
	}
}
