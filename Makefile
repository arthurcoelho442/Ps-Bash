#Antes de executar o progama entre na pasta onde 
#o programa esta localizado.


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
