flex Lexico.l
bison -dyv Sintactico.y
gcc ./lib/tabla_simbolos.c ./lib/tercetos.c ./lib/assembler.c lex.yy.c y.tab.c -o compilador
./compilador test.txt
rm lex.yy.c
rm y.tab.c
rm y.output
rm y.tab.h
rm compilador
