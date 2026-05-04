#include <SDL2/SDL.h>
#include <math.h>
#include "sid/sid.h"

void audio_callback(void *userdata, Uint8 *stream, int len) {
    struct SID* sid = (struct SID*)userdata;
    uint16_t* buffer = (uint16_t*)stream;
    int samples = len / sizeof(uint16_t);

    sample(sid, buffer, samples);
}

int main(void) {
    // sid.voice[i].frequency = (hz * PHASEMAX) / SAMPLERATE; 

    struct SID sid;
    sid.volume = 255;

    sid.voice[0].phaseacc = 0;
    sid.voice[0].frequency = 62772;
    sid.voice[0].pulsewidth = PHASEMAX/4;
    sid.voice[0].waveform = square;

    sid.voice[1].phaseacc = 0;
    sid.voice[1].frequency = 62772;
    sid.voice[1].pulsewidth = PHASEMAX/4;
    sid.voice[1].waveform = square;

    sid.voice[2].phaseacc = 0;
    sid.voice[2].frequency = 62772;
    sid.voice[2].pulsewidth = PHASEMAX/4;
    sid.voice[2].waveform = square;

    SDL_Init(SDL_INIT_AUDIO);

    SDL_AudioSpec spec;
    SDL_zero(spec);

    spec.freq = SAMPLERATE;
    spec.format = AUDIO_U16SYS;
    spec.channels = 1;
    spec.samples = 512;
    spec.callback = audio_callback;
    spec.userdata = &sid;

    SDL_OpenAudio(&spec, NULL);
    SDL_PauseAudio(0);

    SDL_Delay(500); // 1000 = 1s

    SDL_CloseAudio();
    SDL_Quit();
    return 0;
}