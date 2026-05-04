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
static void envstep(struct SID* sid, int idx) {
    if(sid->voice[idx].envelope.oldgate == 0 && sid->voice[idx].envelope.gate == 1) {
        sid->voice[idx].envelope.envphase = ATTACK;
    }
    else if(sid->voice[idx].envelope.oldgate == 1 && sid->voice[idx].envelope.gate == 0) {
        sid->voice[idx].envelope.envphase = RELEASE;
    }
    sid->voice[idx].envelope.oldgate = sid->voice[idx].envelope.gate;

    switch(sid->voice[idx].envelope.envphase) {
    case IDLE:
        sid->voice[idx].envelope.envvolume = 0;
    break;
    case ATTACK: {
        uint32_t dividerthreshold = ratetable[sid->voice[idx].envelope.attackrate];
        ++sid->voice[idx].envelope.divider;
        if(sid->voice[idx].envelope.divider >= dividerthreshold) {
            sid->voice[idx].envelope.divider = 0;
            ++sid->voice[idx].envelope.envvolume;
        }
        if(sid->voice[idx].envelope.envvolume == 255) {
            sid->voice[idx].envelope.envphase = DECAY;
        }
    }
    break;
    case DECAY: {
        uint32_t dividerthreshold = ratetable[sid->voice[idx].envelope.decayrate] * exptable(sid->voice[idx].envelope.envvolume);
        ++sid->voice[idx].envelope.divider;
        if(sid->voice[idx].envelope.divider >= dividerthreshold) {
            sid->voice[idx].envelope.divider = 0;
            --sid->voice[idx].envelope.envvolume;
        }
        if(sid->voice[idx].envelope.envvolume == sid->voice[idx].envelope.sustainvolume*17) {
            sid->voice[idx].envelope.envphase = SUSTAIN;
        }
    }
    break;
    case SUSTAIN:
        //
    break;
    case RELEASE: {
        uint32_t dividerthreshold = ratetable[sid->voice[idx].envelope.releaserate] * exptable(sid->voice[idx].envelope.envvolume);
        ++sid->voice[idx].envelope.divider;
        if(sid->voice[idx].envelope.divider >= dividerthreshold) {
            sid->voice[idx].envelope.divider = 0;
            --sid->voice[idx].envelope.envvolume;
        }
        if(sid->voice[idx].envelope.envvolume == 0) {
            sid->voice[idx].envelope.envphase = IDLE;
        }
    }
    break;
    }
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
    for(int i = 0; i < samples; ++i) {
        for(int k = 0; k < CYCLESPERSAMPLE; ++k) {
            for(int j = 0; j < 3; ++j) {
                envstep(sid, j);
                sid->voice[j].phaseacc = (sid->voice[j].phaseacc + sid->voice[j].frequency) % PHASEMAX;
            }
        }
        uint32_t mix = 0;
        for(int j = 0; j < 3; ++j) {
            mix += sid->voice[j].waveform(sid->voice[j].phaseacc, sid->voice[j].pulsewidth) * sid->voice[j].envelope.envvolume;
        }
        mix /= 3;
        mix /= 255;
        buffer[i] = (mix * sid->volume / 255) << 4;
    }
}