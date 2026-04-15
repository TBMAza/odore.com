#include "cpu.h"

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

// TODO write helper function for address resolution
static void execute(struct CPU6502* cpu, struct Instruction instr) {
	switch(instr.opcode) {
	case LDA:
		switch(instr.addrmode) {
		case IMME:
			cpu->ac = busread(cpu->bus, cpu->pc++);
			setflagz(cpu, cpu->ac);
			setflagn(cpu, cpu->ac);
		break;
		case ZPAG: {
			uint8_t addr_lo = busread(cpu->bus, cpu->pc++);
			cpu->ac = busread(cpu->bus, BYTESTOWORD(0x0000, addr_lo));
			setflagz(cpu, cpu->ac);
			setflagn(cpu, cpu->ac);
		}
		break;
		case ABSO: {
			uint8_t addr_lo = busread(cpu->bus, cpu->pc++);
			uint8_t addr_hi = busread(cpu->bus, cpu->pc++);
			cpu->ac = busread(cpu->bus, BYTESTOWORD(addr_hi, addr_lo));
			setflagz(cpu, cpu->ac);
			setflagn(cpu, cpu->ac);
		}
		break;
		default:
		break;
		}
	break;
	case LDX:
		switch(instr.addrmode) {
		case IMME:
			cpu->x = busread(cpu->bus, cpu->pc++);
			setflagz(cpu, cpu->x);
			setflagn(cpu, cpu->x);
		break;
		default:
		break;
		}
	break;
	case LDY:
		switch(instr.addrmode) {
		case IMME:
			cpu->y = busread(cpu->bus, cpu->pc++);
			setflagz(cpu, cpu->y);
			setflagn(cpu, cpu->y);
		break;
		default:
		break;
		}
	break;
	case STA:
		switch(instr.addrmode) {
		case ZPAG: {
			uint8_t addr_lo = busread(cpu->bus, cpu->pc++);
			buswrite(cpu->bus, BYTESTOWORD(0x0000, addr_lo), cpu->ac);
		}
		break;
		case ABSO: {
			uint8_t addr_lo = busread(cpu->bus, cpu->pc++);
			uint8_t addr_hi = busread(cpu->bus, cpu->pc++);
			buswrite(cpu->bus, BYTESTOWORD(addr_hi, addr_lo), cpu->ac);
		}
		break;
		default:
		break;
		}
	break;
	case TAX:
		switch(instr.addrmode) {
		case IMPL:
			cpu->x = cpu->ac;
			setflagz(cpu, cpu->x);
			setflagn(cpu, cpu->x);
		break;
		default:
		break;
		}
	break;
	case INX:
		switch(instr.addrmode) {
		case IMPL:
			++cpu->x;
			setflagz(cpu, cpu->x);
			setflagn(cpu, cpu->x);
		break;
		default:
		break;
		}
	break;
	case BRK:
		switch(instr.addrmode) {
		case IMPL:
			FLAGON(FLAGB, cpu->sr);
		break;
		default:
		break;
		}
	break;
	case JMP:
		switch(instr.addrmode) {
		case ABSO: {
			uint8_t addr_lo = busread(cpu->bus, cpu->pc++);
			uint8_t addr_hi = busread(cpu->bus, cpu->pc++);
			cpu->pc = BYTESTOWORD(addr_hi, addr_lo);
		}
		break;
		default:
		break;
		}
	break;
	case NOP:
		switch(instr.addrmode) {
		case IMPL:
			/*
			 * other instructions: "I'm doing my part!"
			 * this one: "I DIDN'T DO FUCKIN' SHIT"
			 */
		break;
		default:
		break;
		}
	break;
	default:
	break;
	}
}

void cpuinit(struct CPU6502* cpu, struct Bus* bus) {
	cpu->pc = DEFLPC;
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
