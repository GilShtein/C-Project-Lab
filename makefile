myMain: program_main.o command.o write.o guidence.o instruction.o label.o passfuncts.o utilies.o
	gcc -g -Wall -ansi -pedantic program_main.o command.o write.o guidence.o instruction.o label.o passfuncts.o utilies.o -lm -o myMain
program_main.o: program_main.c programvars.h passfuncts.h writefuncts.h
	gcc -c -Wall -ansi -pedantic program_main.c -o program_main.o
command.o: command.c programvars.h structs.h
	gcc -c -Wall -ansi -pedantic command.c -o command.o
write.o: write.c programvars.h structs.h
	gcc -c -Wall -ansi -pedantic write.c -o write.o
guidence.o: guidence.c programvars.h utilies.h
	gcc -c -Wall -ansi -pedantic guidence.c -lm -o guidence.o 
instruction.o: instruction.c programvars.h utilies.h
	gcc -c -Wall -ansi -pedantic instruction.c -lm -o instruction.o
label.o: label.c utilies.h
	gcc -c -Wall -ansi -pedantic label.c -o label.o
passfuncts.o: passfuncts.c programvars.h utilies.h
	gcc -c -Wall -ansi -pedantic passfuncts.c -o passfuncts.o
utilies.o: utilies.c structs.h
	gcc -c -Wall -ansi -pedantic utilies.c -o utilies.o

