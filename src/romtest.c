#include <stdio.h>
#include <unistd.h>
#include "bus/bus.h"
#include "cpu/cpu.h"

int main(void) {
	struct Bus bus;
	struct CPU6502 cpu;

	businit(&bus);
	busload(&bus, "../roms/kernal.rom", 0xE000);
	busload(&bus, "../roms/charset.rom", 0xD000);
	busload(&bus, "../roms/basic.rom", 0xA000);
	cpuinit(&cpu, &bus);
	
	for(uint16_t i = 0;; ++i) {
		printf("0x%X: 0x%X\n", cpu.pc, busread(&bus, cpu.pc));
		cpustep(&cpu);
	}

	return 0;
}
