odore.com: main.o cpu.o bus.o
	@gcc -o odore.com main.o cpu.o bus.o
main.o: main.c
	@gcc -o main.o -c main.c
cpu.o: cpu/cpu.c
	@gcc -o cpu.o -c cpu/cpu.c
bus.o: bus/bus.c
	@gcc -o bus.o -c bus/bus.c
cleano:
	@rm *.o
cleane:
	@rm odore.com
run:
	@./odore.com
