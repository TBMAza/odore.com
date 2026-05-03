odore.com: main.o cpu.o bus.o sid.o
	@gcc -o odore.com main.o cpu.o bus.o sid.o -lSDL2
main.o: main.c
	@gcc -o main.o -c main.c
cpu.o: cpu/cpu.c
	@gcc -o cpu.o -c cpu/cpu.c
bus.o: bus/bus.c
	@gcc -o bus.o -c bus/bus.c
sid.o: sid/sid.c
	@gcc -o sid.o -c sid/sid.c
cleano:
	@rm *.o
cleane:
	@rm odore.com
run:
	@./odore.com
