#include <stdio.h>
#include <unistd.h>
#include "bus/bus.h"
#include "cpu/cpu.h"

int main(void) {
	struct Bus bus;
	struct CPU6502 cpu;

	businit(&bus);
	busload(&bus, "../roms/kernal.rom", 0xE000);
	cpuinit(&cpu, &bus);
	printf("initial pc value: 0x%X\n\n", cpu.pc);

	for(int i = 0; i < 100; ++i) {
		printf("0x%X: 0x%X\n", cpu.pc, busread(&bus, cpu.pc));
		cpustep(&cpu);
		sleep(1);
	}

	return 0;
}
