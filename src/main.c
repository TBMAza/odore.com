#include <SDL2/SDL.h>
#include "sid/sid.h"

void audio_callback(void *userdata, Uint8 *stream, int len) {
    struct SID* sid = (struct SID*)userdata;
    uint16_t* buffer = (uint16_t*)stream;
    int samples = len / sizeof(uint16_t);
    sample(sid, buffer, samples);
}

static void init_voice(struct Voice* v, uint32_t freq, uint32_t pw, uint16_t (*wf)(uint32_t, uint32_t),
                        uint8_t atk, uint8_t dec, uint8_t sus, uint8_t rel) {
    v->phaseacc = 0;
    v->frequency = freq;
    v->pulsewidth = pw;
    v->waveform = wf;
    v->envelope.envphase = IDLE;
    v->envelope.envvolume = 0;
    v->envelope.divider = 0;
    v->envelope.gate = 0;
    v->envelope.oldgate = 0;
    v->envelope.attackrate = atk;
    v->envelope.decayrate = dec;
    v->envelope.sustainvolume = sus;
    v->envelope.releaserate = rel;
}

static uint32_t hz_to_freq(float hz) {
    return (uint32_t)(hz * PHASEMAX / CLOCK);
}

static void gate_on(struct SID* sid, int voice) {
    sid->voice[voice].envelope.gate = 1;
}

static void gate_off(struct SID* sid, int voice) {
    sid->voice[voice].envelope.gate = 0;
}

static void play_note(struct SID* sid, int voice, int ms) {
    gate_on(sid, voice);
    SDL_Delay(ms);
    gate_off(sid, voice);
}

int main(void) {
    struct SID sid;
    sid.volume = 255;

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

    // -------------------------------------------------------
    // TEST 1: Waveforms
    // One voice, same note (A4 = 440hz), each waveform for 1s
    // Expected: four distinct timbres on the same pitch
    // -------------------------------------------------------
    SDL_Log("TEST 1: Waveforms (sawtooth, triangle, square, noise)");

    uint16_t (*waveforms[3])(uint32_t, uint32_t) = { sawtooth, triangle, square };
    const char* waveform_names[3] = { "Sawtooth", "Triangle", "Square" };

    for(int w = 0; w < 3; ++w) {
        SDL_Log("  %s", waveform_names[w]);
        init_voice(&sid.voice[0], hz_to_freq(440), PHASEMAX/2, waveforms[w], 0, 0, 15, 0);
        init_voice(&sid.voice[1], 0, 0, sawtooth, 0, 0, 0, 0);
        init_voice(&sid.voice[2], 0, 0, sawtooth, 0, 0, 0, 0);
        play_note(&sid, 0, 800);
        gate_off(&sid, 0);
        SDL_Delay(200);
    }

    // -------------------------------------------------------
    // TEST 2: Envelope shapes
    // Slow attack, slow decay, mid sustain, slow release
    // Expected: gradual swell, decay to mid volume, slow fade
    // -------------------------------------------------------
    SDL_Log("TEST 2: Envelope - slow attack / decay / release");
    init_voice(&sid.voice[0], hz_to_freq(440), PHASEMAX/2, sawtooth, 8, 8, 8, 8);
    gate_on(&sid, 0);
    SDL_Delay(2000);
    gate_off(&sid, 0);
    SDL_Delay(2000);

    // -------------------------------------------------------
    // TEST 3: Fast attack, no decay, full sustain, fast release
    // Expected: instant on, full volume hold, instant off
    // -------------------------------------------------------
    SDL_Log("TEST 3: Envelope - fast attack, full sustain, fast release");
    init_voice(&sid.voice[0], hz_to_freq(440), PHASEMAX/2, sawtooth, 0, 0, 15, 0);
    play_note(&sid, 0, 500);
    SDL_Delay(200);
    play_note(&sid, 0, 500);
    SDL_Delay(200);
    play_note(&sid, 0, 500);
    SDL_Delay(500);

    // -------------------------------------------------------
    // TEST 4: Melody on voice 0
    // Simple C major scale using sawtooth
    // -------------------------------------------------------
    SDL_Log("TEST 4: Melody - C major scale");
    float scale[] = { 261.63f, 293.66f, 329.63f, 349.23f, 392.0f, 440.0f, 493.88f, 523.25f };
    init_voice(&sid.voice[0], 0, PHASEMAX/2, sawtooth, 1, 3, 10, 3);
    for(int n = 0; n < 8; ++n) {
        sid.voice[0].frequency = hz_to_freq(scale[n]);
        play_note(&sid, 0, 300);
        SDL_Delay(50);
    }
    SDL_Delay(500);

    // -------------------------------------------------------
    // TEST 5: Three voice chord
    // C major chord: C4, E4, G4
    // -------------------------------------------------------
    SDL_Log("TEST 5: Three voice chord - C major");
    init_voice(&sid.voice[0], hz_to_freq(261.63f), PHASEMAX/2, triangle, 2, 4, 12, 4);
    init_voice(&sid.voice[1], hz_to_freq(329.63f), PHASEMAX/2, triangle, 2, 4, 12, 4);
    init_voice(&sid.voice[2], hz_to_freq(392.0f),  PHASEMAX/2, triangle, 2, 4, 12, 4);
    gate_on(&sid, 0);
    gate_on(&sid, 1);
    gate_on(&sid, 2);
    SDL_Delay(1500);
    gate_off(&sid, 0);
    gate_off(&sid, 1);
    gate_off(&sid, 2);
    SDL_Delay(1000);

    // -------------------------------------------------------
    // TEST 6: Pulse width on square wave
    // Cycle through narrow, medium, wide pulse widths
    // Expected: timbre changes from thin/nasal to full square
    // -------------------------------------------------------
    SDL_Log("TEST 6: Square wave pulse widths (25%%, 50%%, 75%%)");
    uint32_t pulse_widths[3] = { PHASEMAX/4, PHASEMAX/2, (PHASEMAX/4)*3 };
    const char* pw_names[3] = { "25%%", "50%%", "75%%" };
    init_voice(&sid.voice[0], hz_to_freq(440), PHASEMAX/2, square, 0, 0, 15, 0);
    init_voice(&sid.voice[1], 0, 0, sawtooth, 0, 0, 0, 0);
    init_voice(&sid.voice[2], 0, 0, sawtooth, 0, 0, 0, 0);
    for(int p = 0; p < 3; ++p) {
        SDL_Log("  Pulse width %s", pw_names[p]);
        sid.voice[0].pulsewidth = pulse_widths[p];
        play_note(&sid, 0, 800);
        SDL_Delay(200);
    }
    SDL_Delay(500);

    SDL_Log("All tests complete.");
    SDL_CloseAudio();
    SDL_Quit();
    return 0;
}