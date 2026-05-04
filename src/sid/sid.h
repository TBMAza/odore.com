#ifndef SID_H
#define SID_H

#include <stdint.h>

#define SAMPLERATE 44100

#define PHASEMAX 16777216
#define WAVEMAX 4095

enum Envphase {
    IDLE,
    ATTACK,
    DECAY,
    SUSTAIN,
    RELEASE
};

struct Envelope {
    enum Envphase envphase;
    uint8_t envvolume;
    uint8_t divider;
    
    uint8_t gate;
    uint8_t oldgate;

    uint8_t attackrate;
    uint8_t decayrate;
    uint8_t sustainvolume;
    uint8_t releaserate;
};

struct Voice {
    struct Envelope envelope;
    uint32_t phaseacc; // wrap at 24 bits (16,777,216 -> 0)
    uint32_t frequency;
    uint32_t pulsewidth;
    uint16_t (*waveform)(uint32_t, uint32_t);
};

struct SID {
    struct Voice voice[3];
    uint8_t volume;
};

uint16_t sawtooth(uint32_t phase, uint32_t _unused);
uint16_t triangle(uint32_t phase, uint32_t _unused);
uint16_t square(uint32_t phase, uint32_t threshold);
void sample(struct SID* sid, uint16_t* buffer, int samples);

#endif