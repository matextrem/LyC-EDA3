include macros2.asm
include number.asm

.MODEL SMALL
.386
.STACK 200h

.DATA
NEW_LINE DB 0AH,0DH,'$'
CWprevio DW ?
String0 db "Ingrese un valor pivot mayor o igual a 1: ", '$'
pivot dd ?
_1 dd 1
@pos dd ?
_0 dd 0
_2 dd 2
_3 dd 3
resul dd ?
String1 db "Elemento encontrado en posicion: ", '$'
String2 db "El valor debe ser >=1", '$'
String3 db "Elemento no encontrado", '$'

.CODE
START:
MOV AX, @DATA
MOV DS, AX
FINIT

displayString String0
displayString NEW_LINE

getInteger pivot

FILD pivot
FILD _1
FXCH
FCOMP
FSTSW AX
SAHF
JB etiqueta285
FILD _0
FISTP @pos
FILD pivot
FILD _1
FXCH
FCOMP
FSTSW AX
SAHF
JNE etiqueta266
FILD @pos
FILD _0
FXCH
FCOMP
FSTSW AX
SAHF
JNE etiqueta266
FILD _1
FISTP @pos
etiqueta266:
FILD pivot
FILD _2
FXCH
FCOMP
FSTSW AX
SAHF
JNE etiqueta272
FILD @pos
FILD _0
FXCH
FCOMP
FSTSW AX
SAHF
JNE etiqueta272
FILD _2
FISTP @pos
etiqueta272:
FILD pivot
FILD _3
FXCH
FCOMP
FSTSW AX
SAHF
JNE etiqueta278
FILD @pos
FILD _0
FXCH
FCOMP
FSTSW AX
SAHF
JNE etiqueta278
FILD _3
FISTP @pos
etiqueta278:
FILD @pos
FILD _0
FXCH
FCOMP
FSTSW AX
SAHF
JE etiqueta288
FILD @pos
FISTP resul
displayString String1
displayString NEW_LINE

DisplayInteger resul
displayString NEW_LINE

JMP etiqueta290
etiqueta285:
displayString String2
displayString NEW_LINE

JMP etiqueta290
etiqueta288:
displayString String3
displayString NEW_LINE

etiqueta290:

MOV AH, 1
INT 21h
MOV AX, 4C00h
INT 21h

END START
