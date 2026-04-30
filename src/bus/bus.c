#include "bus.h"
#include <string.h>
#include <stdio.h>

static uint8_t regionvisible(uint8_t rom, const struct Bus* bus) {
	uint8_t cpuportio = bus->ram[CPUIOPORT];
	return 1<<rom & cpuportio;
}

void businit(struct Bus* bus) {
	memset(bus->ram, 0, sizeof(bus->ram));
	buswrite(bus, CPUIOPORT, ALLVISIBLE);
}
uint8_t busread(const struct Bus* bus, uint16_t addr) {
	if(ADDRINRANGE(addr, BASICRANGEL, BASICRANGEU) && regionvisible(BASICV, bus)) {
		return bus->rombasic[addr-BASICRANGEL];
	}
	if(ADDRINRANGE(addr, KERNALRANGEL, KERNALRANGEU) && regionvisible(KERNALV, bus)) {
		return bus->romkernal[addr-KERNALRANGEL];
	}
	if(ADDRINRANGE(addr, IORANGEL, IORANGEU) && !regionvisible(IOV, bus)) {
		return bus->romchar[addr-IORANGEL];
	}
	return bus->ram[addr];
}
void buswrite(struct Bus* bus, uint16_t addr, uint8_t val) {
	bus->ram[addr] = val;
}
void busload(struct Bus* bus, const char* path, uint16_t writestartaddr) {
	FILE* rom = fopen(path, "rb");
	if(!rom) {
		return;
	}

	if(writestartaddr == BASICRANGEL) {
		fread(bus->rombasic, sizeof(uint8_t), ROM8K, rom);
	}
	else if(writestartaddr == KERNALRANGEL) {
		fread(bus->romkernal, sizeof(uint8_t), ROM8K, rom);
	}
	else if(writestartaddr == IORANGEL && !regionvisible(IOV, bus)) {
		fread(bus->romchar, sizeof(uint8_t), ROM4K, rom);
	}
	else {
		fread(bus->ram + writestartaddr, sizeof(uint8_t), ADDRSPACE-writestartaddr, rom);
	}

	fclose(rom);
}