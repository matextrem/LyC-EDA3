#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tabla_simbolos.h"

//Variable de control de nombres para las String
int contadorString=0;

/** Si la variable ya existe, devuelve la posicion. Si no, agrega un nuevo
* nombre de variable y tipo a la tabla y devuelve la posicion donde la agrego
*/
int agregarVarATabla(char* nombre, int tipo){
 //Si se llena, error
 if(fin_tabla >= TAMANIO_TABLA - 1){
	 printf("Error: me quede sin espacio en la tabla de simbolos. Sori, gordi.\n");
	 system("Pause");
	 exit(2);
 }

 int pos = buscarEnTabla(nombre);
 //Si no hay otra variable con el mismo nombre...
 if(pos == -1){
	 //Agregar a tabla
	 fin_tabla++;
	 escribirNombreEnTabla(nombre, fin_tabla);
   tabla_simbolo[fin_tabla].tipo_dato = tipo;
   return fin_tabla;
 }
 return pos;
}

/** Agrega una constante string a la tabla de simbolos */
int agregarCteStringATabla(char* nombre){
	if(fin_tabla >= TAMANIO_TABLA - 1){
		printf("Error: me quede sin espacio en la tabla de simbolos. Sori, gordi.\n");
		system("Pause");
		exit(2);
	}

	//Preparo el nombre. Nuestras constantes empiezan con _ en la tabla de simbolos
	//sprintf(nuevoNombre, "_%s", nombre); OLD
	int pos=buscarStringEnTabla(nombre);
	//int pos=buscarEnTabla(nuevoNombre);
	//Si no hay otra constante string con el mismo contenido...
	if(pos == -1){
		//Agregar nombre a tabla
		fin_tabla++;
		char nuevoNombre[10]; //10 porque EL maximo tamaño que puede tener está dado por "StringXXX" mas el fin de linea
		sprintf(nuevoNombre, "String%d", contadorString);
		contadorString++;
		escribirNombreEnTabla(nuevoNombre, fin_tabla);

		//Agregar tipo de dato
		tabla_simbolo[fin_tabla].tipo_dato = CteString;

		//Agregar valor a la tabla
		strcpy(tabla_simbolo[fin_tabla].valor_s, nombre);

		//Agregar longitud
		tabla_simbolo[fin_tabla].longitud = strlen(nombre) - 1;

		pos=fin_tabla;
	}
	return pos;
}

/** Agrega una constante entera a la tabla de simbolos */
int agregarCteIntATabla(int valor){
	if(fin_tabla >= TAMANIO_TABLA - 1){
		printf("Error: me quede sin espacio en la tabla de simbolos. Sori, gordi.\n");
		system("Pause");
		exit(2);
	}

	//Genero el nombre
	char nombre[30];
	sprintf(nombre, "_%d", valor);
	int pos=buscarEnTabla(nombre);

	//Si no hay otra variable con el mismo nombre...
	if(pos == -1){
		//Agregar nombre a tabla
		fin_tabla++;
		escribirNombreEnTabla(nombre, fin_tabla);

		//Agregar tipo de dato
		tabla_simbolo[fin_tabla].tipo_dato = CteInt;

		//Agregar valor a la tabla
		tabla_simbolo[fin_tabla].valor_i = valor;
		pos = fin_tabla;
	}
	return pos;
}

/* Devuleve la posici�n en la que se encuentra el elemento buscado, -1 si no encontr� el elemento */
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

/* Devuleve la posici�n en la que se encuentra el elemento String buscado, -1 si no encontr� el elemento */
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

/** Escribe el nombre de una variable o constante en la posición indicada */
void escribirNombreEnTabla(char* nombre, int pos){
	strcpy(tabla_simbolo[pos].nombre, nombre);
}

/** Guarda la tabla de simbolos en un archivo de texto */
void guardarTabla(){
	if(fin_tabla == -1)
		yyerror("No encontre la tabla de simbolos");
	FILE* arch = fopen("ts.txt", "w+");
	if(!arch){
		printf("No pude crear el archivo ts.txt\n");
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
