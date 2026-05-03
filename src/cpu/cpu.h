#ifndef CPU_H
#define CPU_H

#include "../c64utils/c64utils.h"
#include "../bus/bus.h"
#include <string.h>

#define VECRESET 0xFFFC
#define VECIRQ 0xFFFE
#define VECNMI 0xFFFA

// default register values
#define DEFLSP 0xFF
#define DEFLSR 0x24

// status register (p register) flags
#define FLAGN 7
#define FLAGV 6
#define FLAG_ 5 // unused
#define FLAGB 4
#define FLAGD 3
#define FLAGI 2
#define FLAGZ 1
#define FLAGC 0

#define BRANCHIF(bitmap, flag, val, addr, defl) ( FLAGREAD((flag), (bitmap)) == (val) ? (addr) : (defl) ) // branch if in `bitmap` the `flag` equals `val` to location `addr`, otherwise to `defl` default location

enum Opcode {
	OP_,
	ADC,AND,ASL,BCC,BCS,BEQ,BIT,BMI,
	BNE,BPL,BRK,BVC,BVS,CLC,CLD,CLI,
	CLV,CMP,CPX,CPY,DEC,DEX,DEY,EOR,
	INC,INX,INY,JMP,JSR,LDA,LDX,LDY,
	LSR,NOP,ORA,PHA,PHP,PLA,PLP,ROL,
	ROR,RTI,RTS,SBC,SEC,SED,SEI,STA,
	STX,STY,TAX,TAY,TSX,TXA,TXS,TYA
};

enum Addrmode {
	ADM_,ACCU,ABSO,ABSX,ABSY,IMME,IMPL, 
	INDI,INDX,INDY,RELA,ZPAG,ZPAX,ZPAY
};

enum InterruptType {
	INTNMI,
	INTIRQ,
	INTBRK
};

struct CPU6502 {
	uint8_t irq;
	uint8_t nmi;

	uint16_t pc;
	uint8_t ac; // aka A register
	uint8_t x;
	uint8_t y;
	uint8_t sr; // aka P register
	uint8_t sp;

	struct Bus* bus;
};

struct Instruction {
	enum Opcode opcode;
	enum Addrmode addrmode;
 };

void cpuinit(struct CPU6502* cpu, struct Bus* bus);
void cpustep(struct CPU6502* cpu);
void cpuinterrupt(struct CPU6502* cpu, enum InterruptType interrupttype);

#endif
