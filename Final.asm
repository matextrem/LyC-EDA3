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
String1 db "Elemento encontrado en posición: ", '$'
resul dd ?
String2 db "El valor debe ser >=1", '$'
String3 db "La lista está vacía", '$'
String4 db "FIN PROGRAMA", '$'

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
JB etiqueta264
JMP etiqueta266
displayString String1
displayString NEW_LINE

DisplayInteger resul
displayString NEW_LINE

JMP etiqueta269
etiqueta264:
displayString String2
displayString NEW_LINE

etiqueta266:
displayString String3
displayString NEW_LINE

JMP etiqueta269
etiqueta269:
displayString String4
displayString NEW_LINE


MOV AH, 1
INT 21h
MOV AX, 4C00h
INT 21h

END START
