#include <jni.h>
#include <string>
#include "native_log.h"
#include "vector"
#include "SpeexProcessor.h"

SpeexProcessor *processor = nullptr;
extern "C" JNIEXPORT void JNICALL
Java_com_example_speexdemo_MainActivity_nativeInitSpeex(
        JNIEnv *env,
        jobject thiz,
        jint frameSize,
        jint sampleRate) {
    processor = new SpeexProcessor(frameSize, sampleRate);
}

extern "C" JNIEXPORT jshortArray JNICALL
Java_com_example_speexdemo_MainActivity_nativeSpeexProcessFrame(
        JNIEnv *env,
        jobject thiz,
        jshortArray rawShortArray) {
    if (processor != nullptr) {
        jsize size = env->GetArrayLength(rawShortArray);
        std::vector<short> input(size);
        env->GetShortArrayRegion(rawShortArray, 0, size, &input[0]);
        int result = processor->process(&input[0]);
        ALOGD("speex process result %d", result)
        env->SetShortArrayRegion(rawShortArray, 0, input.size(), &input[0]);
    }
    return rawShortArray;
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_speexdemo_MainActivity_nativeReleaseSpeex(
        JNIEnv *env,
        jobject thiz) {
    if (processor != nullptr) {
        delete processor;
        processor = nullptr;
    }
}