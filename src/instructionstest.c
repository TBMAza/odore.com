#include <stdio.h>
#include <stdlib.h>
#include "bus/bus.h"
#include "cpu/cpu.h"

int main(void) {
	struct Bus bus;
	struct CPU6502 cpu;
	uint16_t addr;

	uint8_t passed = 0;
	uint8_t failed = 0;
	
	printf("---------------- TEST 0 ----------------\n\n");

	businit(&bus);
	cpuinit(&cpu, &bus);
	addr = cpu.pc;

	buswrite(&bus, addr++, 0xA9); buswrite(&bus, addr++, 0x05);
	buswrite(&bus, addr++, 0xAA);
	buswrite(&bus, addr++, 0xE8);
	buswrite(&bus, addr++, 0x00);

	while(FLAGREAD(FLAGB, cpu.sr) != 1) {
		cpustep(&cpu);
	}

	printf("X: %d\nA: %d\n", cpu.x, cpu.ac);
	
	if(cpu.x == 6 && cpu.ac == 5) {
		printf("\nTEST 0 PASSED\n");
		++passed;
	}
	else {
		printf("\nTEST 0 FAILED\n");
		++failed;
	}

	printf("\n---------------- TEST 1 ----------------\n\n");

	businit(&bus);
	cpuinit(&cpu, &bus);
	addr = cpu.pc;

	buswrite(&bus, addr++, 0xA9); buswrite(&bus, addr++, 0x42);
	buswrite(&bus, addr++, 0x85); buswrite(&bus, addr++, 0x10);
	buswrite(&bus, addr++, 0xA5); buswrite(&bus, addr++, 0x10);
	buswrite(&bus, addr++, 0x00);

	while(FLAGREAD(FLAGB, cpu.sr) != 1) {
		cpustep(&cpu);
	}

	printf("A: 0x%X\n", cpu.ac);

	if(cpu.ac == 0x42) {
		printf("\nTEST 1 PASSED\n");
		++passed;
	}
	else {
		printf("\nTEST 1 FAILED\n");
		++failed;
	}

	printf("\n---------------- TEST 2 ----------------\n\n");

	businit(&bus);
	cpuinit(&cpu, &bus);
	addr = cpu.pc;

	buswrite(&bus, addr++, 0xA9); buswrite(&bus, addr++, 0xFF);
	buswrite(&bus, addr++, 0x8D); buswrite(&bus, addr++, 0x00); buswrite(&bus, addr++, 0x02);
	buswrite(&bus, addr++, 0xAD); buswrite(&bus, addr++, 0x00); buswrite(&bus, addr++, 0x02);
	buswrite(&bus, addr++, 0x00);

	while(FLAGREAD(FLAGB, cpu.sr) != 1) {
		cpustep(&cpu);
	}

	printf("A: 0x%X\nN flag: %d\n", cpu.ac, FLAGREAD(FLAGN, cpu.sr));

	if(cpu.ac == 0xFF && FLAGREAD(FLAGN, cpu.sr) == 1) {
		printf("\nTEST 2 PASSED\n");
		++passed;
	}
	else {
		printf("\nTEST 2 FAILED\n");
		++failed;
	}

	printf("\n---------------- TEST 3 ----------------\n\n");

	businit(&bus);
	cpuinit(&cpu, &bus);
	addr = cpu.pc;

	buswrite(&bus, addr++, 0xA2); buswrite(&bus, addr++, 0x07);
	buswrite(&bus, addr++, 0xA0); buswrite(&bus, addr++, 0x00);
	buswrite(&bus, addr++, 0x00);

	while(FLAGREAD(FLAGB, cpu.sr) != 1) {
		cpustep(&cpu);
	}

	printf("X: 0x%X\nY: 0x%X\nZ flag: %d\n", cpu.x, cpu.y, FLAGREAD(FLAGZ, cpu.sr));

	if(cpu.x == 7 && cpu.y == 0 && FLAGREAD(FLAGZ, cpu.sr) == 1) {
		printf("\nTEST 3 PASSED\n");
		++passed;
	}
	else {
		printf("\nTEST 3 FAILED\n");
		++failed;
	}

	printf("\n---------------- TEST 4 ----------------\n\n");

	businit(&bus);
	cpuinit(&cpu, &bus);
	cpu.pc = 0xFC00;
	addr = cpu.pc;

	buswrite(&bus, addr++, 0x4C); buswrite(&bus, addr++, 0x05); buswrite(&bus, addr++, 0xFC);
	buswrite(&bus, addr++, 0x00);
	buswrite(&bus, addr++, 0x00);
	buswrite(&bus, addr++, 0xA2); buswrite(&bus, addr++, 0x42);
	buswrite(&bus, addr++, 0x00);

	while(FLAGREAD(FLAGB, cpu.sr) != 1) {
		cpustep(&cpu);
	}

	printf("X: 0x%X\n", cpu.x);

	if(cpu.x == 0x42) {
		printf("\nTEST 4 PASSED\n");
		++passed;
	}
	else {
		printf("\nTEST 4 FAILED\n");
		++failed;
	}

	printf("\n---------------- TEST 5 ----------------\n\n");

	businit(&bus);
	cpuinit(&cpu, &bus);
	addr = cpu.pc;

	buswrite(&bus, addr++, 0xA9); buswrite(&bus, addr++, 0x01);
	buswrite(&bus, addr++, 0xEA);
	buswrite(&bus, addr++, 0xEA);
	buswrite(&bus, addr++, 0xAA);
	buswrite(&bus, addr++, 0x00);

	while(FLAGREAD(FLAGB, cpu.sr) != 1) {
		cpustep(&cpu);
	}

	printf("A: %d\nX: %d\n", cpu.ac, cpu.x);

	if(cpu.ac == 1 && cpu.x == 1) {
		printf("\nTEST 5 PASSED\n");
		++passed;
	}
	else {
		printf("\nTEST 5 FAILED\n");
		++failed;
	}

	printf("\n---------------- TEST 6 ----------------\n\n");

	businit(&bus);
	cpuinit(&cpu, &bus);
	addr = cpu.pc;

	buswrite(&bus, addr++, 0x20); buswrite(&bus, addr++, 0x10); buswrite(&bus, addr++, 0xFC);
	buswrite(&bus, addr++, 0xA2); buswrite(&bus, addr++, 0x01);
	buswrite(&bus, addr++, 0x00);
	addr = 0xFC10;
	buswrite(&bus, addr++, 0xA9); buswrite(&bus, addr++, 0x42);
	buswrite(&bus, addr++, 0x60);

	while(FLAGREAD(FLAGB, cpu.sr) != 1) {
		cpustep(&cpu);
	}

	printf("A: 0x%X\nX: 0x%X\n", cpu.ac, cpu.x);

	if(cpu.ac == 0x42 && cpu.x == 0x01) {
		printf("\nTEST 6 PASSED\n");
		++passed;
	}
	else {
		printf("\nTEST 6 FAILED\n");
		++failed;
	}

	printf("\n---------------- TEST 7 ----------------\n\n");

	businit(&bus);
	cpuinit(&cpu, &bus);
	cpu.pc = 0xFC00;
	addr = cpu.pc;

	buswrite(&bus, addr++, 0x20); buswrite(&bus, addr++, 0x10); buswrite(&bus, addr++, 0xFC);
	buswrite(&bus, addr++, 0xA2); buswrite(&bus, addr++, 0x01);
	buswrite(&bus, addr++, 0x00);

	addr = 0xFC10;
	buswrite(&bus, addr++, 0x20); buswrite(&bus, addr++, 0x20); buswrite(&bus, addr++, 0xFC);
	buswrite(&bus, addr++, 0x60);

	addr = 0xFC20;
	buswrite(&bus, addr++, 0xA9); buswrite(&bus, addr++, 0x42);
	buswrite(&bus, addr++, 0x60);

	while(FLAGREAD(FLAGB, cpu.sr) != 1) {
		cpustep(&cpu);
	}

	printf("A: 0x%X\nX: 0x%X\n", cpu.ac, cpu.x);

	if(cpu.ac == 0x42 && cpu.x == 0x01) {
		printf("\nTEST 7 PASSED\n");
		++passed;
	}
	else {
		printf("\nTEST 7 FAILED\n");
		++failed;
	}

	printf("\n---------------- TEST 8 ----------------\n\n");

	businit(&bus);
	cpuinit(&cpu, &bus);
	cpu.pc = 0xFC00;
	addr = cpu.pc;

	buswrite(&bus, addr++, 0xA2); buswrite(&bus, addr++, 0x05);
	buswrite(&bus, addr++, 0xCA);
	buswrite(&bus, addr++, 0xD0); buswrite(&bus, addr++, 0xFD);
	buswrite(&bus, addr++, 0x00);

	while(FLAGREAD(FLAGB, cpu.sr) != 1) {
		cpustep(&cpu);
	}

	printf("X: %d\nZ flag: %d\n", cpu.x, FLAGREAD(FLAGZ, cpu.sr));

	if(cpu.x == 0 && FLAGREAD(FLAGZ, cpu.sr) == 1) {
		printf("\nTEST 8 PASSED\n");
		++passed;
	}
	else {
		printf("\nTEST 8 FAILED\n");
		++failed;
	}

	printf("\n---------------- TEST 9 ----------------\n\n");

	// ADC: carry flag test — 0xFF + 0x01 = 0x100, result truncates to 0x00, carry set
	businit(&bus);
	cpuinit(&cpu, &bus);
	cpu.pc = 0xFC00;
	addr = cpu.pc;

	buswrite(&bus, addr++, 0xA9); buswrite(&bus, addr++, 0xFF); // LDA #$FF
	buswrite(&bus, addr++, 0x69); buswrite(&bus, addr++, 0x01); // ADC #$01
	buswrite(&bus, addr++, 0x00);                               // BRK

	while(FLAGREAD(FLAGB, cpu.sr) != 1) cpustep(&cpu);

	printf("A: 0x%X\nC flag: %d\nZ flag: %d\n", cpu.ac, FLAGREAD(FLAGC, cpu.sr), FLAGREAD(FLAGZ, cpu.sr));

	if(cpu.ac == 0x00 && FLAGREAD(FLAGC, cpu.sr) == 1 && FLAGREAD(FLAGZ, cpu.sr) == 1) {
		printf("\nTEST 9 PASSED\n");
		++passed;
	}
	else {
		printf("\nTEST 9 FAILED\n");
		++failed;
	}

	printf("\n---------------- TEST 10 ----------------\n\n");

	// ADC: overflow flag test — 0x50 + 0x50 = 0xA0, two positives produce a negative
	businit(&bus);
	cpuinit(&cpu, &bus);
	cpu.pc = 0xFC00;
	addr = cpu.pc;

	buswrite(&bus, addr++, 0xA9); buswrite(&bus, addr++, 0x50); // LDA #$50
	buswrite(&bus, addr++, 0x69); buswrite(&bus, addr++, 0x50); // ADC #$50
	buswrite(&bus, addr++, 0x00);                               // BRK

	while(FLAGREAD(FLAGB, cpu.sr) != 1) cpustep(&cpu);

	printf("A: 0x%X\nV flag: %d\nN flag: %d\n", cpu.ac, FLAGREAD(FLAGV, cpu.sr), FLAGREAD(FLAGN, cpu.sr));

	if(cpu.ac == 0xA0 && FLAGREAD(FLAGV, cpu.sr) == 1 && FLAGREAD(FLAGN, cpu.sr) == 1) {
		printf("\nTEST 10 PASSED\n");
		++passed;
	}
	else {
		printf("\nTEST 10 FAILED\n");
		++failed;
	}

	printf("\n---------------- TEST 11 ----------------\n\n");

	// SBC: basic subtraction — 0x10 - 0x01 = 0x0F, carry set (no borrow), no overflow
	businit(&bus);
	cpuinit(&cpu, &bus);
	cpu.pc = 0xFC00;
	addr = cpu.pc;

	buswrite(&bus, addr++, 0x38);                               // SEC (set carry — required before SBC)
	buswrite(&bus, addr++, 0xA9); buswrite(&bus, addr++, 0x10); // LDA #$10
	buswrite(&bus, addr++, 0xE9); buswrite(&bus, addr++, 0x01); // SBC #$01
	buswrite(&bus, addr++, 0x00);                               // BRK

	while(FLAGREAD(FLAGB, cpu.sr) != 1) cpustep(&cpu);

	printf("A: 0x%X\nC flag: %d\nV flag: %d\n", cpu.ac, FLAGREAD(FLAGC, cpu.sr), FLAGREAD(FLAGV, cpu.sr));

	if(cpu.ac == 0x0F && FLAGREAD(FLAGC, cpu.sr) == 1 && FLAGREAD(FLAGV, cpu.sr) == 0) {
		printf("\nTEST 11 PASSED\n");
		++passed;
	}
	else {
		printf("\nTEST 11 FAILED\n");
		++failed;
	}

	printf("\n---------------- TEST 12 ----------------\n\n");

	// CMP: A >= operand sets carry, A == operand also sets zero
	businit(&bus);
	cpuinit(&cpu, &bus);
	cpu.pc = 0xFC00;
	addr = cpu.pc;

	buswrite(&bus, addr++, 0xA9); buswrite(&bus, addr++, 0x42); // LDA #$42
	buswrite(&bus, addr++, 0xC9); buswrite(&bus, addr++, 0x42); // CMP #$42
	buswrite(&bus, addr++, 0x00);                               // BRK

	while(FLAGREAD(FLAGB, cpu.sr) != 1) cpustep(&cpu);

	printf("A: 0x%X\nC flag: %d\nZ flag: %d\n", cpu.ac, FLAGREAD(FLAGC, cpu.sr), FLAGREAD(FLAGZ, cpu.sr));

	if(cpu.ac == 0x42 && FLAGREAD(FLAGC, cpu.sr) == 1 && FLAGREAD(FLAGZ, cpu.sr) == 1) {
		printf("\nTEST 12 PASSED\n");
		++passed;
	}
	else {
		printf("\nTEST 12 FAILED\n");
		++failed;
	}

	printf("\n---------------- TESTS DONE ----------------\n");
	printf("passed: %d, failed: %d\n\n", passed, failed);

	return 0;
}
