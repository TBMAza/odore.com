#ifndef CPU_H
#define CPU_H

#include "../bus/bus.h"
#include <string.h>

#define RESETVEC 0xFFFC

// default register values
#define DEFLSP 0xFD
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

#define FLAGREAD(flag, map) ( (map)>>flag & 1 )
#define FLAGON(flag, map) ( (map) |= 1<<flag )
#define FLAGOFF(flag, map) ( (map) &= ~(1<<flag) )
#define FLAGTOG(flag, map) ( (map) ^= 1<<flag )

#define BYTESTOWORD(hi, lo) ( (uint16_t)lo + (((uint16_t)hi)<<8) )

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

struct CPU6502 {
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

#endif
