#include <jni.h>
#include <string>
#include "speex_preprocess.h"
#include "native_log.h"
#include "vector"

SpeexPreprocessState *st = nullptr;

extern "C" JNIEXPORT void JNICALL
Java_com_example_speexdemo_MainActivity_nativeInitSpeex(
        JNIEnv *env,
        jobject thiz,
        jint frameSize,
        jint sampleRate) {
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
        ALOGD("set dereverb error %d", result);
        return;
    }
    float f = .0f;
    result = speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_DEREVERB_DECAY, &f);
    if (result != 0) {
        ALOGD("set dereverb decay error %d", result);
        return;
    }
    f = .0f;
    result = speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_DEREVERB_LEVEL, &f);
    if (result != 0) {
        ALOGD("set dereverb level error %d", result);
        return;
    }
    ALOGD("speex init success")
}

extern "C" JNIEXPORT jshortArray JNICALL
Java_com_example_speexdemo_MainActivity_nativeSpeexProcessFrame(
        JNIEnv *env,
        jobject thiz,
        jshortArray rawShortArray) {
    if (st != nullptr) {
        jsize size = env->GetArrayLength(rawShortArray);
        std::vector<short> input(size);
        env->GetShortArrayRegion(rawShortArray, 0, size, &input[0]);
        int result = speex_preprocess_run(st, &input[0]);
        ALOGD("speex process result %d", result)
        jshortArray outShortArray = env->NewShortArray(input.size());
        env->SetShortArrayRegion(outShortArray, 0, input.size(), &input[0]);
        return outShortArray;
    }
    return rawShortArray;
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_speexdemo_MainActivity_nativeReleaseSpeex(
        JNIEnv *env,
        jobject thiz) {
    if (st != nullptr) {
        speex_preprocess_state_destroy(st);
        st = nullptr;
        ALOGD("release speex success")
    }
}