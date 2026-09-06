#include "sound.h"
#include <3ds.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SAMPLE_RATE 44100

typedef struct {
    s16* data;
    u32  samples;
    bool loaded;
} WavSound;

static WavSound sounds[SOUND_COUNT];
static bool sounds_enabled = true;
static bool initialized = false;
static ndspWaveBuf waveBuf[SOUND_COUNT];
static int channel = 0;

static const char* sound_files[SOUND_COUNT] = {
    "menu_move.wav",
    "menu_select.wav",
    "menu_back.wav",
    "splash.wav",
};

static bool load_wav(const char* path, WavSound* out) {
    FILE* f = fopen(path, "rb");
    if (!f) return false;
    u8 header[44];
    if (fread(header, 1, 44, f) < 44) { fclose(f); return false; }
    if (memcmp(header, "RIFF", 4) != 0 || memcmp(header + 8, "WAVE", 4) != 0) {
        fclose(f); return false;
    }
    u16 channels = (u16)(header[22] | (header[23] << 8));
    u16 bits = (u16)(header[34] | (header[35] << 8));
    u32 dataSize = (u32)(header[40] | (header[41] << 8) | (header[42] << 16) | (header[43] << 24));
    if (bits != 16 || dataSize == 0 || dataSize > 2 * 1024 * 1024) {
        fclose(f); return false;
    }
    s16* buf = (s16*)linearAlloc(dataSize);
    if (!buf) { fclose(f); return false; }
    if (fread(buf, 1, dataSize, f) < dataSize) {
        linearFree(buf); fclose(f); return false;
    }
    fclose(f);
    u32 nSamples = dataSize / 2;
    if (channels == 2) {
        u32 monoCount = nSamples / 2;
        for (u32 i = 0; i < monoCount; i++) {
            s32 L = buf[i * 2], R = buf[i * 2 + 1];
            buf[i] = (s16)((L + R) / 2);
        }
        nSamples = monoCount;
    }
    out->data = buf;
    out->samples = nSamples;
    out->loaded = true;
    return true;
}

bool sound_init(const Config* cfg) {
    if (initialized) return true;
    (void)cfg;
    sounds_enabled = true;
    if (ndspInit() != 0) { initialized = true; return false; }
    ndspSetOutputMode(NDSP_OUTPUT_STEREO);
    for (int i = 0; i < 24; i++) {
        ndspChnReset(i);
        ndspChnSetInterp(i, NDSP_INTERP_LINEAR);
        ndspChnSetRate(i, (float)SAMPLE_RATE);
        ndspChnSetFormat(i, NDSP_FORMAT_MONO_PCM16);
    }
    const char* dirs[] = { "/_nds/DSi-FC-Men/sounds", "romfs:/sounds", "/3ds/DSi-FC-Men/sounds", NULL };
    for (int id = 0; id < SOUND_COUNT; id++) {
        sounds[id].loaded = false;
        sounds[id].data = NULL;
        for (int d = 0; dirs[d]; d++) {
            char path[512];
            snprintf(path, sizeof(path), "%s/%s", dirs[d], sound_files[id]);
            if (load_wav(path, &sounds[id])) break;
        }
    }
    initialized = true;
    return true;
}

void sound_exit(void) {
    if (!initialized) return;
    for (int i = 0; i < SOUND_COUNT; i++) {
        if (sounds[i].data) {
            linearFree(sounds[i].data);
            sounds[i].data = NULL;
            sounds[i].loaded = false;
        }
    }
    ndspExit();
    initialized = false;
}

void sound_play(SoundId id) {
    if (!initialized || !sounds_enabled) return;
    if (id < 0 || id >= SOUND_COUNT) return;
    if (!sounds[id].loaded || !sounds[id].data) return;
    int ch = channel;
    channel = (channel + 1) % 8;
    ndspChnWaveBufClear(ch);
    memset(&waveBuf[id], 0, sizeof(ndspWaveBuf));
    waveBuf[id].data_vaddr = sounds[id].data;
    waveBuf[id].nsamples = sounds[id].samples;
    waveBuf[id].looping = false;
    waveBuf[id].status = NDSP_WBUF_FREE;
    DSP_FlushDataCache(sounds[id].data, sounds[id].samples * 2);
    ndspChnWaveBufAdd(ch, &waveBuf[id]);
}

void sound_set_enabled(bool enabled) {
    sounds_enabled = enabled;
}
