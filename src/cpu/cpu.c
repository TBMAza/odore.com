#include "cpu.h"
#include <stdlib.h>
#include <stdio.h>

const struct Instruction decodematrix[16*16] = {
	{BRK,IMPL},{ORA,INDX},{OP_,ADM_},{OP_,ADM_},{OP_,ADM_},{ORA,ZPAG},{ASL,ZPAG},{OP_,ADM_},{PHP,IMPL},{ORA,IMME},{ASL,ACCU},{OP_,ADM_},{OP_,ADM_},{ORA,ABSO},{ASL,ABSO},{OP_,ADM_},
	{BPL,RELA},{ORA,INDY},{OP_,ADM_},{OP_,ADM_},{OP_,ADM_},{ORA,ZPAX},{ASL,ZPAX},{OP_,ADM_},{CLC,IMPL},{ORA,ABSY},{OP_,ADM_},{OP_,ADM_},{OP_,ADM_},{ORA,ABSX},{ASL,ABSX},{OP_,ADM_},
	{JSR,ABSO},{AND,INDX},{OP_,ADM_},{OP_,ADM_},{BIT,ZPAG},{AND,ZPAG},{ROL,ZPAG},{OP_,ADM_},{PLP,IMPL},{AND,IMME},{ROL,ACCU},{OP_,ADM_},{BIT,ABSO},{AND,ABSO},{ROL,ABSO},{OP_,ADM_},
	{BMI,RELA},{AND,INDY},{OP_,ADM_},{OP_,ADM_},{OP_,ADM_},{AND,ZPAX},{ROL,ZPAX},{OP_,ADM_},{SEC,IMPL},{AND,ABSY},{OP_,ADM_},{OP_,ADM_},{OP_,ADM_},{AND,ABSX},{ROL,ABSX},{OP_,ADM_},
	{RTI,IMPL},{EOR,INDX},{OP_,ADM_},{OP_,ADM_},{OP_,ADM_},{EOR,ZPAG},{LSR,ZPAG},{OP_,ADM_},{PHA,IMPL},{EOR,IMME},{LSR,ACCU},{OP_,ADM_},{JMP,ABSO},{EOR,ABSO},{LSR,ABSO},{OP_,ADM_},
	{BVC,RELA},{EOR,INDY},{OP_,ADM_},{OP_,ADM_},{OP_,ADM_},{EOR,ZPAX},{LSR,ZPAX},{OP_,ADM_},{CLI,IMPL},{EOR,ABSY},{OP_,ADM_},{OP_,ADM_},{OP_,ADM_},{EOR,ABSX},{LSR,ABSX},{OP_,ADM_},
	{RTS,IMPL},{ADC,INDX},{OP_,ADM_},{OP_,ADM_},{OP_,ADM_},{ADC,ZPAG},{ROR,ZPAG},{OP_,ADM_},{PLA,IMPL},{ADC,IMME},{ROR,ACCU},{OP_,ADM_},{JMP,INDI},{ADC,ABSO},{ROR,ABSO},{OP_,ADM_},
	{BVS,RELA},{ADC,INDY},{OP_,ADM_},{OP_,ADM_},{OP_,ADM_},{ADC,ZPAX},{ROR,ZPAX},{OP_,ADM_},{SEI,IMPL},{ADC,ABSY},{OP_,ADM_},{OP_,ADM_},{OP_,ADM_},{ADC,ABSX},{ROR,ABSX},{OP_,ADM_},
	{OP_,ADM_},{STA,INDX},{OP_,ADM_},{OP_,ADM_},{STY,ZPAG},{STA,ZPAG},{STX,ZPAG},{OP_,ADM_},{DEY,IMPL},{OP_,ADM_},{TXA,IMPL},{OP_,ADM_},{STY,ABSO},{STA,ABSO},{STX,ABSO},{OP_,ADM_},
	{BCC,RELA},{STA,INDY},{OP_,ADM_},{OP_,ADM_},{STY,ZPAX},{STA,ZPAX},{STX,ZPAY},{OP_,ADM_},{TYA,IMPL},{STA,ABSY},{TXS,IMPL},{OP_,ADM_},{OP_,ADM_},{STA,ABSX},{OP_,ADM_},{OP_,ADM_},
	{LDY,IMME},{LDA,INDX},{LDX,IMME},{OP_,ADM_},{LDY,ZPAG},{LDA,ZPAG},{LDX,ZPAG},{OP_,ADM_},{TAY,IMPL},{LDA,IMME},{TAX,IMPL},{OP_,ADM_},{LDY,ABSO},{LDA,ABSO},{LDX,ABSO},{OP_,ADM_},
	{BCS,RELA},{LDA,INDY},{OP_,ADM_},{OP_,ADM_},{LDY,ZPAX},{LDA,ZPAX},{LDX,ZPAY},{OP_,ADM_},{CLV,IMPL},{LDA,ABSY},{TSX,IMPL},{OP_,ADM_},{LDY,ABSX},{LDA,ABSX},{LDX,ABSY},{OP_,ADM_},
	{CPY,IMME},{CMP,INDX},{OP_,ADM_},{OP_,ADM_},{CPY,ZPAG},{CMP,ZPAG},{DEC,ZPAG},{OP_,ADM_},{INY,IMPL},{CMP,IMME},{DEX,IMPL},{OP_,ADM_},{CPY,ABSO},{CMP,ABSO},{DEC,ABSO},{OP_,ADM_},
	{BNE,RELA},{CMP,INDY},{OP_,ADM_},{OP_,ADM_},{OP_,ADM_},{CMP,ZPAX},{DEC,ZPAX},{OP_,ADM_},{CLD,IMPL},{CMP,ABSY},{OP_,ADM_},{OP_,ADM_},{OP_,ADM_},{CMP,ABSX},{DEC,ABSX},{OP_,ADM_},
	{CPX,IMME},{SBC,INDX},{OP_,ADM_},{OP_,ADM_},{CPX,ZPAG},{SBC,ZPAG},{INC,ZPAG},{OP_,ADM_},{INX,IMPL},{SBC,IMME},{NOP,IMPL},{OP_,ADM_},{CPX,ABSO},{SBC,ABSO},{INC,ABSO},{OP_,ADM_},
	{BEQ,RELA},{SBC,INDY},{OP_,ADM_},{OP_,ADM_},{OP_,ADM_},{SBC,ZPAX},{INC,ZPAX},{OP_,ADM_},{SED,IMPL},{SBC,ABSY},{OP_,ADM_},{OP_,ADM_},{OP_,ADM_},{SBC,ABSX},{INC,ABSX},{OP_,ADM_}
};

static void setflagn(struct CPU6502* cpu, uint8_t res) {
	if(res & 0x80) {
		FLAGON(FLAGN, cpu->sr);
	}
	else {
		FLAGOFF(FLAGN, cpu->sr);
	}
}
static void setflagz(struct CPU6502* cpu, uint8_t res) {
	if(res == 0) {
		FLAGON(FLAGZ, cpu->sr);
	}
	else {
		FLAGOFF(FLAGZ, cpu->sr);
	}
}
static uint8_t fetch(struct CPU6502* cpu) {
	uint8_t instruction = busread(cpu->bus, cpu->pc);
	++cpu->pc;
	return instruction;
}
static struct Instruction decode(uint8_t instruction) {
	uint8_t hi = instruction>>4;
	uint8_t lo = instruction&0x0F;
	return decodematrix[hi*16 + lo];
}
static uint16_t resaddr(struct CPU6502* cpu, enum Addrmode addrmode) {
	switch(addrmode) {
	case ACCU:
		return 0;
	case ABSO: {
		uint8_t addrlo = busread(cpu->bus, cpu->pc++);
		uint8_t addrhi = busread(cpu->bus, cpu->pc++);
		return BYTESTOWORD(addrhi, addrlo);
	}
	case ABSX: {
		uint8_t addrlo = busread(cpu->bus, cpu->pc++);
		uint8_t addrhi = busread(cpu->bus, cpu->pc++);
		return BYTESTOWORD(addrhi, addrlo) + (uint16_t)cpu->x;
	}
	case ABSY: {
		uint8_t addrlo = busread(cpu->bus, cpu->pc++);
		uint8_t addrhi = busread(cpu->bus, cpu->pc++);
		return BYTESTOWORD(addrhi, addrlo) + (uint16_t)cpu->y;
	}
	case IMME:
		return cpu->pc++;
	case IMPL:
		return 0;
	case INDY: {
		uint16_t pointer = BYTESTOWORD(0x00, busread(cpu->bus, cpu->pc++));
		return BYTESTOWORD(busread(cpu->bus, pointer+1), busread(cpu->bus, pointer)) + cpu->y;
	}
	break;
	case RELA: {
		int8_t offset = busread(cpu->bus, cpu->pc++);
		return cpu->pc + offset;
	}
	case ZPAG: {
		uint8_t addrlo = busread(cpu->bus, cpu->pc++);
		return BYTESTOWORD(0x0000, addrlo);
	}
	default:
		printf("[ERR] Unknown address mode: %d\n", addrmode);
		exit(1);
		//return 0;
	}
}
static void stackpush(struct CPU6502* cpu, uint8_t val) {
	buswrite(cpu->bus, BYTESTOWORD(0x01, cpu->sp), val);
	--cpu->sp;
}
static uint8_t stackpull(struct CPU6502* cpu) {
	++cpu->sp;
	return busread(cpu->bus, BYTESTOWORD(0x01, cpu->sp));
}
static void cmpinstr(struct CPU6502* cpu, enum Addrmode addrmode, uint8_t reg) {
	uint8_t diff = reg - busread(cpu->bus, resaddr(cpu, addrmode));
	setflagz(cpu, diff);
	setflagn(cpu, diff);
	(FLAGREAD(FLAGZ, cpu->sr) && FLAGREAD(FLAGN, cpu->sr)) ? FLAGOFF(FLAGC, cpu->sr) : FLAGON(FLAGC, cpu->sr);
}
static void suminstr(struct CPU6502* cpu, enum Addrmode addrmode, uint8_t issub) {
	uint8_t operand = busread(cpu->bus, resaddr(cpu, addrmode));
	if(issub) {
		operand = ~operand;
	}
	uint8_t res = cpu->ac + operand + FLAGREAD(FLAGC, cpu->sr);
	
	if( (cpu->ac & 0x80) == (operand & 0x80) && (operand & 0x80) != (res & 0x80) ) {
		FLAGON(FLAGV, cpu->sr);
	}
	else {
		FLAGOFF(FLAGV, cpu->sr);
	}
	( ( (cpu->ac & 0x80) || (operand & 0x80) ) && !(res & 0x80) ) ? FLAGON(FLAGC, cpu->sr) : FLAGOFF(FLAGC, cpu->sr);
	
	cpu->ac = res;
	setflagz(cpu, cpu->ac);
	setflagn(cpu, cpu->ac);
}
static void execute(struct CPU6502* cpu, struct Instruction instr) {
	switch(instr.opcode) {
	case LDA:
		cpu->ac = busread(cpu->bus, resaddr(cpu, instr.addrmode));
		setflagz(cpu, cpu->ac);
		setflagn(cpu, cpu->ac);
	break;
	case LDX:
		cpu->x = busread(cpu->bus, resaddr(cpu, instr.addrmode));
		setflagz(cpu, cpu->x);
		setflagn(cpu, cpu->x);
	break;
	case LDY:
		cpu->y = busread(cpu->bus, resaddr(cpu, instr.addrmode));
		setflagz(cpu, cpu->y);
		setflagn(cpu, cpu->y);
	break;
	case STA:
		buswrite(cpu->bus, resaddr(cpu, instr.addrmode), cpu->ac);
	break;
	case STX:
		buswrite(cpu->bus, resaddr(cpu, instr.addrmode), cpu->x);
	break;
	case STY:
		buswrite(cpu->bus, resaddr(cpu, instr.addrmode), cpu->y);
	break;
	case INC: {
		uint16_t addr = resaddr(cpu, instr.addrmode);
		uint8_t val = busread(cpu->bus, addr);
		buswrite(cpu->bus, addr, val+1);
		setflagz(cpu, val+1);
		setflagn(cpu, val+1);
	}
	break;
	case TAX:
		cpu->x = cpu->ac;
		setflagz(cpu, cpu->x);
		setflagn(cpu, cpu->x);
	break;
	case TXA:
		cpu->ac = cpu->x;
		setflagz(cpu, cpu->ac);
		setflagn(cpu, cpu->ac);
	break;
	case TXS:
		cpu->sp = cpu->x;
	break;
	case TAY:
		cpu->y = cpu->ac;
		setflagz(cpu, cpu->y);
		setflagn(cpu, cpu->y);
	break;
	case TYA:
		cpu->ac = cpu->y;
		setflagz(cpu, cpu->ac);
		setflagn(cpu, cpu->ac);
	break;
	case INX:
		++cpu->x;
		setflagz(cpu, cpu->x);
		setflagn(cpu, cpu->x);
	break;
	case DEX:
		--cpu->x;
		setflagz(cpu, cpu->x);
		setflagn(cpu, cpu->x);
	break;
	case INY:
		++cpu->y;
		setflagz(cpu, cpu->y);
		setflagn(cpu, cpu->y);
	break;
	case DEY:
		--cpu->y;
		setflagz(cpu, cpu->y);
		setflagn(cpu, cpu->y);
	break;
	case AND:
		cpu->ac &= busread(cpu->bus, resaddr(cpu, instr.addrmode));
		setflagz(cpu, cpu->ac);
		setflagn(cpu, cpu->ac);
	break;
	case ORA:
		cpu->ac |= busread(cpu->bus, resaddr(cpu, instr.addrmode));
		setflagz(cpu, cpu->ac);
		setflagn(cpu, cpu->ac);
	break;
	case EOR:
		cpu->ac ^= busread(cpu->bus, resaddr(cpu, instr.addrmode));
		setflagz(cpu, cpu->ac);
		setflagn(cpu, cpu->ac);
	break;
	case CMP:
		cmpinstr(cpu, instr.addrmode, cpu->ac);
	break;
	case CPX:
		cmpinstr(cpu, instr.addrmode, cpu->x);
	break;
	case CPY:
		cmpinstr(cpu, instr.addrmode, cpu->y);
	break;
	case ADC:
		suminstr(cpu, instr.addrmode, 0);
	break;
	case SBC:
		suminstr(cpu, instr.addrmode, 1);
	break;
	case BRK:
		FLAGON(FLAGB, cpu->sr);
	break;
	case JMP:
		cpu->pc = resaddr(cpu, instr.addrmode);
	break;
	case PHA:
		stackpush(cpu, cpu->ac);
	break;
	case PLA:
		cpu->ac = stackpull(cpu);
		setflagz(cpu, cpu->ac);
		setflagn(cpu, cpu->ac);
	break;
	case PHP:
		stackpush(cpu, cpu->sr);
	break;
	case PLP:
		cpu->sr = stackpull(cpu);
	break;
	case JSR: {
		uint16_t pushpc = cpu->pc+1;
		stackpush(cpu, pushpc>>8);
		stackpush(cpu, pushpc&0x00FF);
		cpu->pc = resaddr(cpu, instr.addrmode);
	}
	break;
	case RTS: {
		uint8_t pclo = stackpull(cpu);
		uint8_t pchi = stackpull(cpu);
		cpu->pc = BYTESTOWORD(pchi, pclo) + 1;
	}
	break;
	case BCC:
		cpu->pc = BRANCHIF(cpu->sr, FLAGC, 0, resaddr(cpu, instr.addrmode), cpu->pc+1);
	break;
	case BCS:
		cpu->pc = BRANCHIF(cpu->sr, FLAGC, 1, resaddr(cpu, instr.addrmode), cpu->pc+1);
	break;
	case BEQ:
		cpu->pc = BRANCHIF(cpu->sr, FLAGZ, 1, resaddr(cpu, instr.addrmode), cpu->pc+1);
	break;
	case BNE:
		cpu->pc = BRANCHIF(cpu->sr, FLAGZ, 0, resaddr(cpu, instr.addrmode), cpu->pc+1);
	break;
	case BMI:
		cpu->pc = BRANCHIF(cpu->sr, FLAGN, 1, resaddr(cpu, instr.addrmode), cpu->pc+1);
	break;
	case BPL:
		cpu->pc = BRANCHIF(cpu->sr, FLAGN, 0, resaddr(cpu, instr.addrmode), cpu->pc+1);
	break;
	case BVC:
		cpu->pc = BRANCHIF(cpu->sr, FLAGV, 0, resaddr(cpu, instr.addrmode), cpu->pc+1);
	break;
	case BVS:
		cpu->pc = BRANCHIF(cpu->sr, FLAGV, 1, resaddr(cpu, instr.addrmode), cpu->pc+1);
	break;
	case SEC:
		FLAGON(FLAGC, cpu->sr);
	break;
	case SEI:
		FLAGON(FLAGI, cpu->sr);
	break;
	case CLD:
		FLAGOFF(FLAGD, cpu->sr);
	break;
	case ROL:
		if(instr.addrmode == ACCU) {
			uint8_t carry = cpu->ac & 0x80 ? 1 : 0;
			cpu->ac = (cpu->ac<<1) + carry;
			carry ? FLAGON(FLAGC, cpu->sr) : FLAGOFF(FLAGC, cpu->sr);
			setflagz(cpu, cpu->ac);
			setflagn(cpu, cpu->ac);
		}
		else {
			uint16_t addr = resaddr(cpu, instr.addrmode);
			uint8_t val = busread(cpu->bus, addr);
			uint8_t carry = val & 0x80 ? 1 : 0;
			val = (val<<1) + carry;
			buswrite(cpu->bus, addr, val);
			carry ? FLAGON(FLAGC, cpu->sr) : FLAGOFF(FLAGC, cpu->sr);
			setflagz(cpu, val);
			setflagn(cpu, val);
		}
	break;
	case NOP:
		/*
		 * other instructions: "I'm doing my part!"
		 * this one: "I DIDN'T DO FUCKIN' SHIT"
		 */
	break;
	default:
		printf("[ERR] Unknown instruction: %d\n", instr.opcode);
		exit(1);
	break;
	}
}

void cpuinit(struct CPU6502* cpu, struct Bus* bus) {
	cpu->pc = BYTESTOWORD(busread(bus, RESETVEC+0x0001), busread(bus, RESETVEC));
	cpu->sp = DEFLSP;
	cpu->sr = DEFLSR;
	cpu->ac = 0;
	cpu->x = 0;
	cpu->y = 0;
	cpu->bus = bus;
}
void cpustep(struct CPU6502* cpu) {
	uint8_t byte = fetch(cpu);
	struct Instruction instr = decode(byte);
	execute(cpu, instr);
}
