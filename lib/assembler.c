#include <stdio.h>
#include <stdlib.h>
#include "assembler.h"
#include "../y.tab.h"
#include "tercetos.h"

void generarAssembler(){
  FILE* arch = fopen("Final.asm", "w");
  if(!arch){
		printf("No pude crear el archivo final.txt\n");
		return;
	}

  escribirInicio(arch);
  generarTabla(arch);
  escribirInicioCodigo(arch);

  for(int i=0; i <= ultimo_terceto; i++){
    switch(lista_terceto[i].operador){
      case ASIGNA:
	  	asignacion(arch, i);
        break;
      case CMP:
		comparacion(arch, i);
        break;

      case BGT:
        escribirSalto(arch, "JA", lista_terceto[i].op1);
        break;
      case BGE:
        escribirSalto(arch, "JAE", lista_terceto[i].op1);
        break;
      case BLT:
        escribirSalto(arch, "JB", lista_terceto[i].op1);
        break;
      case BLE:
        escribirSalto(arch, "JBE", lista_terceto[i].op1);
        break;
      case BNE:
        escribirSalto(arch, "JNE", lista_terceto[i].op1);
        break;
      case BEQ:
        escribirSalto(arch, "JE", lista_terceto[i].op1);
        break;
      case JMP:
        escribirSalto(arch, "JMP", lista_terceto[i].op1);
        break;
    case ETIQUETA:
        escribirEtiqueta(arch,"etiqueta",i); 
        break;
      case READ: read (arch,i);
        break;
      case WRITE:
	  	write(arch, i);
        break;
    }
  }

  escribirFinal(arch);
  fclose(arch);

}

void escribirInicio(FILE *arch){
  fprintf(arch, "include macros2.asm\ninclude number.asm\n\n.MODEL SMALL\n.386\n.STACK 200h\n\n");
}

void escribirInicioCodigo(FILE* arch){
	fprintf(arch, ".CODE\nSTART:\nMOV AX, @DATA\nMOV DS, AX\nFINIT\n\n");
}

void escribirFinal(FILE *arch){
    fprintf(arch, "\nMOV AH, 1\nINT 21h\nMOV AX, 4C00h\nINT 21h\n\nEND START\n");
	// TODO: Preguntar por flags y escribir subrutinas
}

void generarTabla(FILE *arch){
    fprintf(arch, ".DATA\n");
    fprintf(arch, "NEW_LINE DB 0AH,0DH,'$'\n");
	fprintf(arch, "CWprevio DW ?\n");

    for(int i=0; i<=fin_tabla; i++){
        fprintf(arch, "%s ", tabla_simbolo[i].nombre);
        switch(tabla_simbolo[i].tipo_dato){
        case CteInt:
            fprintf(arch, "dd %d\n", tabla_simbolo[i].valor_i);
            break;
        case CteString:
            fprintf(arch, "db \"%s\", '$'\n", tabla_simbolo[i].valor_s);
            break;
        default: //Es una variable int, float o puntero a string
            fprintf(arch, "dd ?\n");
        }
    }

    fprintf(arch, "\n");
}

void escribirEtiqueta(FILE* arch, char* etiqueta, int n){
    fprintf(arch, "%s%d:\n", etiqueta, n+OFFSET);
}

void escribirSalto(FILE* arch, char* salto, int tercetoDestino){
    fprintf(arch, "%s ", salto);

    //Por si nos olvidamos de rellenar un salto
    if(tercetoDestino == NOOP){
        printf("Ups. Parece que me olvide de rellenar un salto en los tercetos y ahora no se como seguir.\n");
        system("Pause");
        exit(10);
    }

    fprintf(arch, "etiqueta");
    fprintf(arch, "%d\n", tercetoDestino);
}

void asignacion(FILE* arch, int ind){
	int destino = lista_terceto[ind].op1;
	int origen = lista_terceto[ind].op2;

	//Ver tipo de dato
	switch(tabla_simbolo[destino].tipo_dato){
	case Int:
		// Si es un int de tabla de simbolos, primero hay que traerlo de memoria a st(0)
		// Sino es el resultado de una expresion anterior y ya esta en st(0)
		if(origen < OFFSET) //Es un int en tabla de simbolos
			fprintf(arch, "FILD %s\n", tabla_simbolo[origen].nombre);
		else //El valor ya esta en el copro, puede que haga falta redondear
			fprintf(arch, "FSTCW CWprevio ;Guardo Control Word del copro\nOR CWprevio, 0400h ;Preparo Control Word seteando RC con redondeo hacia abajo\nFLDCW CWprevio ;Cargo nueva Control Word\n");
		fprintf(arch, "FISTP %s", tabla_simbolo[destino].nombre);
		break;
	}

	fprintf(arch, "\n");
}

/** Levanta, da vuelta los elementos y compara */
void comparacion(FILE* arch, int ind){
	levantarEnPila(arch, ind);
	fprintf(arch, "FXCH\nFCOMP\nFSTSW AX\nSAHF\n");

}


/** Asegura que el elemento de la izquierda esté en st1, y el de la derecha en st0 */
void levantarEnPila(FILE* arch, const int ind){
	int elemIzq = lista_terceto[ind].op1;
	int elemDer = lista_terceto[ind].op2;
	int izqLevantado = 0;
	/* Si el elemento no está en pila lo levanta */
	if(elemIzq < OFFSET){
		switch(tabla_simbolo[elemIzq].tipo_dato){
		case Int:
			//FILD n; Donde n es el numero integer en memoria
			fprintf(arch, "FILD %s\n", tabla_simbolo[elemIzq].nombre);
			break;
		case CteInt:
			//FILD n;Donde n es el numero integer en tabla
			fprintf(arch, "FILD %s\n", tabla_simbolo[elemIzq].nombre);
			break;
		}
		izqLevantado=1;
	}
	if(elemDer < OFFSET){
		switch(tabla_simbolo[elemDer].tipo_dato){
		case Int:
			//FILD n; Donde n es el numero integer en memoria
			fprintf(arch, "FILD %s\n", tabla_simbolo[elemDer].nombre);
			break;
		case CteInt:
			//FILD n;Donde n es el numero integer en tabla
			fprintf(arch, "FILD %s\n", tabla_simbolo[elemDer].nombre);
			break;
		}
		izqLevantado=0;
	}
	if(izqLevantado){
		fprintf(arch, "FXCH\n");
	}
}

void write(FILE* arch, int terceto){
	int ind = lista_terceto[terceto].op1; //Indice de entrada a tabla de simbolos del mensaje a mostrar
	switch(tabla_simbolo[ind].tipo_dato){
	case Int:
		fprintf(arch, "DisplayInteger %s\n", tabla_simbolo[ind].nombre);
		fprintf(arch, "displayString NEW_LINE\n");
		break;
	case CteString:
		fprintf(arch, "displayString %s\n", tabla_simbolo[ind].nombre);
		fprintf(arch, "displayString NEW_LINE\n");
		break;
	}
	fprintf(arch, "\n");
}

void read(FILE* arch, int terceto){
	int ind = lista_terceto[terceto].op1;
	switch(tabla_simbolo[ind].tipo_dato){
	case Int:
		fprintf(arch, "getInteger %s\n", tabla_simbolo[ind].nombre);
        break;
	}
	fprintf(arch, "\n");
}
