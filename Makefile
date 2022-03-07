#Antes de executar o progama entre na pasta onde 
#o programa esta localizado.
#Alunos: Arthur Coelho Estevão, Milena da Silva Mantovanelli e Mayke Wallace Silva Alves.


compile:
	gcc -o psBash *.c 
	
run:
	./psBash
	
compile-run:
	make compile
	make run
	
valgrind:
	make compile
	valgrind ./psBash
#Finalizado