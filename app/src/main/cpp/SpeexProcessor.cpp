//
// Created by cw on 2020/12/1.
//

#include "SpeexProcessor.h"
#include "native_log.h"

SpeexProcessor::SpeexProcessor(int frameSize, int sampleRate) {
    st = speex_preprocess_state_init(frameSize, sampleRate);
    int param = 1;
    int result = 0;
    result = speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_DENOISE, &param);
    if (result != 0) {
        ALOGD("deNoise init error %d", result)
        return;
    }
    param = -25;
    result = speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_NOISE_SUPPRESS, &param);
    if (result != 0) {
        ALOGD("set noise suppress error %d", result)
        return;
    }
    param = 1;
    result = speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_AGC, &param);
    if (result != 0) {
        ALOGD("set agc error %d", result)
        return;
    }
    float level = 8000.0f;
    result = speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_AGC_LEVEL, &level);
    if (result != 0) {
        ALOGD("set agc level error %d", param)
        return;
    }
    param = 0;
    result = speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_DEREVERB, &param);
    if (result != 0) {
        ALOGD("set dereverb error %d", result)
        return;
    }
    float f = .0f;
    result = speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_DEREVERB_DECAY, &f);
    if (result != 0) {
        ALOGD("set dereverb decay error %d", result)
        return;
    }
    f = .0f;
    result = speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_DEREVERB_LEVEL, &f);
    if (result != 0) {
        ALOGD("set dereverb level error %d", result)
        return;
    }
    ALOGD("speex init success")
}

int SpeexProcessor::process(short *buffer) {
    if (st != nullptr) {
        return speex_preprocess_run(st, buffer);
    }
    return -1;
}

SpeexProcessor::~SpeexProcessor() {
    if (st != nullptr) {
        speex_preprocess_state_destroy(st);
        st = nullptr;
        ALOGD("release speex success")
    }
}

