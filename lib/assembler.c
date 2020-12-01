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
  int i;
  for(i=0; i <= ultimo_terceto; i++){
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
}

void generarTabla(FILE *arch){
    fprintf(arch, ".DATA\n");
    fprintf(arch, "NEW_LINE DB 0AH,0DH,'$'\n");
	fprintf(arch, "CWprevio DW ?\n");
    int i;
    for(i=0; i<=fin_tabla; i++){
        fprintf(arch, "%s ", tabla_simbolo[i].nombre);
        switch(tabla_simbolo[i].tipo_dato){
        case CteInt:
            fprintf(arch, "dd %d\n", tabla_simbolo[i].valor_i);
            break;
        case CteString:
            fprintf(arch, "db \"%s\", '$'\n", tabla_simbolo[i].valor_s);
            break;
        default:
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

    if(tercetoDestino == NOOP){
        printf("Error al intentar rellenar saltos.\n");
        system("Pause");
        exit(10);
    }

    fprintf(arch, "etiqueta");
    fprintf(arch, "%d\n", tercetoDestino);
}

void asignacion(FILE* arch, int ind){
	int destino = lista_terceto[ind].op1;
	int origen = lista_terceto[ind].op2;

	switch(tabla_simbolo[destino].tipo_dato){
	case Int:
		if(origen < OFFSET)
			fprintf(arch, "FILD %s\n", tabla_simbolo[origen].nombre);
		else
			fprintf(arch, "FSTCW CWprevio ;Guardo Control Word del copro\nOR CWprevio, 0400h ;Preparo Control Word seteando RC con redondeo hacia abajo\nFLDCW CWprevio ;Cargo nueva Control Word\n");
		fprintf(arch, "FISTP %s", tabla_simbolo[destino].nombre);
		break;
	}

	fprintf(arch, "\n");
}

void comparacion(FILE* arch, int ind){
	levantarEnPila(arch, ind);
	fprintf(arch, "FXCH\nFCOMP\nFSTSW AX\nSAHF\n");

}

void levantarEnPila(FILE* arch, const int ind){
	int elemIzq = lista_terceto[ind].op1;
	int elemDer = lista_terceto[ind].op2;
	int izqLevantado = 0;

	if(elemIzq < OFFSET){
		switch(tabla_simbolo[elemIzq].tipo_dato){
		case Int:
			fprintf(arch, "FILD %s\n", tabla_simbolo[elemIzq].nombre);
			break;
		case CteInt:
			fprintf(arch, "FILD %s\n", tabla_simbolo[elemIzq].nombre);
			break;
		}
		izqLevantado=1;
	}
	if(elemDer < OFFSET){
		switch(tabla_simbolo[elemDer].tipo_dato){
		case Int:
			fprintf(arch, "FILD %s\n", tabla_simbolo[elemDer].nombre);
			break;
		case CteInt:
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
	int ind = lista_terceto[terceto].op1;
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
