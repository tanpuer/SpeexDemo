//
// Created by cw on 2020/12/1.
//

#ifndef SPEEDXDEMO_SPEEXPROCESSOR_H
#define SPEEDXDEMO_SPEEXPROCESSOR_H


#include "speex_preprocess.h"

class SpeexProcessor {

public:

    SpeexProcessor(int frameSize, int sampleRate);

    ~SpeexProcessor();

    int process(short *buffer);

private:

    SpeexPreprocessState *st = nullptr;

};


#endif //SPEEDXDEMO_SPEEXPROCESSOR_H
