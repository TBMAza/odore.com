#include "bus.h"
#include <string.h>

void businit(struct Bus* bus) {
	memset(bus->mem, 0, sizeof(bus->mem));
}
uint8_t busread(const struct Bus* bus, uint16_t addr) {
	return bus->mem[addr];
}
void buswrite(struct Bus* bus, uint16_t addr, uint8_t val) {
	bus->mem[addr] = val;
}
