#ifndef SID_H
#define SID_H

#include <stdint.h>

enum Envphase {
   IDLE,
   ATTACK,
   DECAY,
   SUSTAIN,
   RELEASE 
};

struct Voice {
    enum Envphase envphase;

    uint32_t phaseacc;

    uint8_t freqlo;
    uint8_t freqhi;
    uint8_t pwlo;
    uint8_t pwhi;
    uint8_t control;
    uint8_t atkdec;
    uint8_t susrel;

    uint8_t envlevel;
};

struct SID {
    struct Voice voice[3];
    
    uint32_t sample_rate;

    uint8_t cutofflo;
    uint8_t cutoffhi;
    uint8_t filterctrl;
    uint8_t volume;

    uint8_t readonly[4];
};

void sidwrite(struct SID* sid, uint8_t reg, uint8_t val);

#endif