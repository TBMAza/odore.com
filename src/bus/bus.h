#ifndef BUS_H
#define BUS_H

#include <stdint.h>

#define ADDRSPACE 65536

struct Bus {
	uint8_t mem[ADDRSPACE];	
};

void businit(struct Bus* bus); // bussin' it frfr
uint8_t busread(const struct Bus* bus, uint16_t addr);
void buswrite(struct Bus* bus, uint16_t addr, uint8_t val);

#endif
