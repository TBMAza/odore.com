#include "sid.h"

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
void sample(struct Oscillator* osc, uint16_t* buffer, int samples) {
    for(int i = 0; i < samples; ++i) {
        buffer[i] = osc->waveform(osc->phaseacc, osc->threshold);
        osc->phaseacc = (osc->phaseacc + osc->frequency) % (PHASEMAX);
    }
}