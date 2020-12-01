#ifndef tercetos_h
#define tercetos_h

#include "tabla_simbolos.h"

#define OFFSET TAMANIO_TABLA
#define MAX_TERCETOS 512

#define NOOP -1 /* Sin operador */
#define PROG 7  
#define CMP 21  
#define BNE 2   
#define BGE 4   
#define BLT 6   
#define BLE 10  
#define BEQ 14  
#define BGT 8   
#define JMP 16  
#define SALTO_POSICION 5 /* Etiqueta de tamaño de salto posicion */
#define ETIQUETA 100


#define OP1 2
#define OP2 3
#define OPERADOR 1

/* Funciones */
int crear_terceto(int operador, int op1, int op2);
void guardarTercetos();
void modificarTerceto(int indice, int posicion, int valor);
void optimizarTercetos();

typedef struct{
  int operador;
  int op1;
  int op2;
} terceto;

/* Variables externas */
extern terceto lista_terceto[MAX_TERCETOS];
extern int ultimo_terceto; /* Apunta al ultimo terceto escrito. */

#endif
