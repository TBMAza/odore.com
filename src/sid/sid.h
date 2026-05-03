#ifndef SID_H
#define SID_H

#include <stdint.h>

#define SAMPLERATE 44100

#define PHASEMAX 16777216
#define WAVEMAX 4095

struct Oscillator {
    uint32_t phaseacc; // wrap at 24 bits (16,777,216 -> 0)
    uint32_t frequency;
    uint32_t threshold;
    uint16_t (*waveform)(uint32_t, uint32_t);
};

uint16_t sawtooth(uint32_t phase, uint32_t _unused);
uint16_t triangle(uint32_t phase, uint32_t _unused);
uint16_t square(uint32_t phase, uint32_t threshold);
void sample(struct Oscillator* osc, uint16_t* buffer, int samples);

#endif