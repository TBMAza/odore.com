#include "sid.h"

uint16_t ratetable[16] = {9, 32, 63, 95, 149, 220, 267, 313, 392, 977, 1954, 3126, 6252, 15625, 31250, 62500};
static uint8_t exptable(uint8_t counterrange) {
    if(counterrange <= 5) {
        return 30;
    }
    if(counterrange <= 13) {
        return 16;
    }
    if(counterrange <= 25) {
        return 8;
    }
    if(counterrange <= 53) {
        return 4;
    }
    if(counterrange <= 92) {
        return 2;
    }
    return 1;
}

uint16_t sawtooth(uint32_t phase, uint32_t _unused) {
    return phase>>12;
}
uint16_t triangle(uint32_t phase, uint32_t _unused) {
    if(phase < PHASEMAX/2) return phase>>12;
    return (phase>>12)^0xFFF;
}
uint16_t square(uint32_t phase, uint32_t threshold) {
    if(phase < threshold) return WAVEMAX;
    return 0;
}
void sample(struct SID* sid, uint16_t* buffer, int samples) {
    uint32_t mix = 0;
    for(int i = 0; i < samples; ++i) {
        for(int j = 0; j < 3; ++j) {
            mix += sid->voice[j].waveform(sid->voice[j].phaseacc, sid->voice[j].pulsewidth);
            sid->voice[j].phaseacc = (sid->voice[j].phaseacc + sid->voice[j].frequency) % PHASEMAX;
        }
        mix /= 3;
        buffer[i] = mix * sid->volume;
    }
}