flex EA3.l
bison -dyv EA3.y
gcc lib/tabla_simbolos.c lib/tercetos.c lib/assembler.c lex.yy.c y.tab.c -o EA3
