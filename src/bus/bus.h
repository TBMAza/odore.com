#ifndef BUS_H
#define BUS_H

#include <stdint.h>

#define ADDRSPACE 65536
#define ROM8K 8192
#define ROM4K 4096

#define CPUIOPORT 0x0001
#define ALLVISIBLE 0x37
#define BASICV 0
#define KERNALV 1
#define IOV 2

#define BASICRANGEL 0xA000
#define BASICRANGEU 0xBFFF
#define KERNALRANGEL 0xE000
#define KERNALRANGEU 0xFFFF
#define IORANGEL 0xD000
#define IORANGEU 0xDFFF

#define ADDRINRANGE(addr, lower, upper) ( (addr) >= (lower) && (addr) <= (upper) )

struct Bus {
	uint8_t ram[ADDRSPACE];
	uint8_t romkernal[ROM8K];
	uint8_t rombasic[ROM8K];
	uint8_t romio[ROM4K];
};

void businit(struct Bus* bus); // bussin' it frfr
uint8_t busread(const struct Bus* bus, uint16_t addr);
void buswrite(struct Bus* bus, uint16_t addr, uint8_t val);
void busload(struct Bus* bus, const char* path, uint16_t writestartaddr); // the amount of self-control I needed not to name this `bustaload` is unimaginable

#endif
