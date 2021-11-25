hello: my_print.o main.cpp
	g++ -o hello main.cpp my_print.o -std=c++17
my_print.o: my_print.asm
	nasm -f elf -o my_print.o my_print.asm
clean:
	rm my_print.o