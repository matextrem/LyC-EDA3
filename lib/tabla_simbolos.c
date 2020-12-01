#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tabla_simbolos.h"

int contadorString=0;


int agregarVarATabla(char* nombre, int tipo){

 if(fin_tabla >= TAMANIO_TABLA - 1){
	 printf("Error al agregar valor: Tabla de simbolos llena\n");
	 system("Pause");
	 exit(2);
 }

 int pos = buscarEnTabla(nombre);
 if(pos == -1){
	 fin_tabla++;
	 escribirNombreEnTabla(nombre, fin_tabla);
   tabla_simbolo[fin_tabla].tipo_dato = tipo;
   return fin_tabla;
 }
 return pos;
}

int agregarCteStringATabla(char* nombre){
	if(fin_tabla >= TAMANIO_TABLA - 1){
		printf("Error al agregar valor: Tabla de simbolos llena.\n");
		system("Pause");
		exit(2);
	}

	int pos=buscarStringEnTabla(nombre);
	
	if(pos == -1){
		fin_tabla++;
		char nuevoNombre[10];
		sprintf(nuevoNombre, "String%d", contadorString);
		contadorString++;
		escribirNombreEnTabla(nuevoNombre, fin_tabla);

		tabla_simbolo[fin_tabla].tipo_dato = CteString;

		strcpy(tabla_simbolo[fin_tabla].valor_s, nombre);

		tabla_simbolo[fin_tabla].longitud = strlen(nombre) - 1;

		pos=fin_tabla;
	}
	return pos;
}

int agregarCteIntATabla(int valor){
	if(fin_tabla >= TAMANIO_TABLA - 1){
		printf("Error al agregar valor: Tabla de simbolos llena.\n");
		system("Pause");
		exit(2);
	}

	char nombre[30];
	sprintf(nombre, "_%d", valor);
	int pos=buscarEnTabla(nombre);

	if(pos == -1){
		fin_tabla++;
		escribirNombreEnTabla(nombre, fin_tabla);

		tabla_simbolo[fin_tabla].tipo_dato = CteInt;

		tabla_simbolo[fin_tabla].valor_i = valor;
		pos = fin_tabla;
	}
	return pos;
}

int buscarEnTabla(char * name){
   int i=0;
   while(i<=fin_tabla){
	   if(strcmp(tabla_simbolo[i].nombre,name) == 0){
		   return i;
	   }
	   i++;
   }
   return -1;
}

int buscarStringEnTabla(char * name){
   int i=0;
   while(i<=fin_tabla){
	   if(tabla_simbolo[i].tipo_dato==6 && strcmp(tabla_simbolo[i].valor_s,name) == 0){
		   return i;
	   }
	   i++;
   }
   return -1;
}

int buscarIDEnTabla(char * name){
   int i=0;
   while(i<=fin_tabla){
	   if(strcmp(tabla_simbolo[i].nombre,name) == 0){
		   return i;
	   }
	   i++;
   }
   return -1;
}

void escribirNombreEnTabla(char* nombre, int pos){
	strcpy(tabla_simbolo[pos].nombre, nombre);
}

void guardarTabla(){
	if(fin_tabla == -1)
		yyerror("No se encontro la tabla de simbolos");
	FILE* arch = fopen("ts.txt", "w+");
	if(!arch){
		printf("No se pudo crear el archivo ts.txt\n");
		return;
	}

	for(int i = 0; i <= fin_tabla; i++){
		fprintf(arch, "%s\t", (tabla_simbolo[i].nombre) );

		switch (tabla_simbolo[i].tipo_dato){
		case Int:
			fprintf(arch, "INT");
			break;
		case CteInt:
			fprintf(arch, "CTE_INT\t%d", tabla_simbolo[i].valor_i);
			break;
		case CteString:
			fprintf(arch, "CTE_STRING\t%s\t%d", (tabla_simbolo[i].valor_s), tabla_simbolo[i].longitud);
			break;
		}

		fprintf(arch, "\n");
	}
	fclose(arch);
}
