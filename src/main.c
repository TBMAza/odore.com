#include <SDL2/SDL.h>
#include <math.h>
#include "sid/sid.h"

void audio_callback(void *userdata, Uint8 *stream, int len) {
    struct Oscillator* osc = (struct Oscillator*)userdata;
    uint16_t* buffer = (uint16_t*)stream;
    int samples = len / sizeof(uint16_t);

    sample(osc, buffer, samples);
}

int main(void) {

    struct Oscillator osc;
    osc.phaseacc = 0;
    osc.frequency = 167392; // (440hz * PHASEMAX) / SAMPLERATE
    osc.threshold = PHASEMAX/2;
    osc.waveform = sawtooth;

    SDL_Init(SDL_INIT_AUDIO);

    SDL_AudioSpec spec;
    SDL_zero(spec);

    spec.freq = SAMPLERATE;
    spec.format = AUDIO_U16SYS;
    spec.channels = 1;
    spec.samples = 512;
    spec.callback = audio_callback;
    spec.userdata = &osc;

    SDL_OpenAudio(&spec, NULL);
    SDL_PauseAudio(0);

    SDL_Delay(1000); // 1000 = 1s

    SDL_CloseAudio();
    SDL_Quit();
    return 0;
}