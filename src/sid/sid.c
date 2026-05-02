#include "sid.h"

void sidwrite(struct SID* sid, uint8_t reg, uint8_t val) {
    if(reg < 21) {
        uint8_t voice = reg/7;
        switch(reg%7) {
            case 0:
            sid->voice[voice].freqlo = val;
            break;
            case 1:
                sid->voice[voice].freqhi = val;
            break;
            case 2:
                sid->voice[voice].pwlo = val;
            break;
            case 3:
                sid->voice[voice].pwhi = val;
            break;
            case 4:
                if((sid->voice[voice].control & 1) == 0 && (val & 1) == 1) {
                    sid->voice[voice].envphase = ATTACK;
                }
                else if((sid->voice[voice].control & 1) == 1 && (val & 1) == 0) {
                    sid->voice[voice].envphase = RELEASE;
                }
                sid->voice[voice].control = val;
            break;
            case 5:
                sid->voice[voice].atkdec = val;
            break;
            case 6:
                sid->voice[voice].susrel = val;
            break;
        }
    }
    else {
        switch(reg) {
            case 21:
            sid->cutofflo = val;
            break;
            case 22:
                sid->cutoffhi = val;
            break;
            case 23:
                sid->filterctrl = val;
            break;
            case 24:
                sid->volume = val;
            break;
            case 25:
            case 26:
            case 27:
            case 28:
                // read only
            break;
        }
    }
}