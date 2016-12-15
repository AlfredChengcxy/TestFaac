//
// Created by 钟龙州 on 2016/12/2.
//

#ifndef TESTFAAC_AUDIO_FAAC_CONTEXT_H
#define TESTFAAC_AUDIO_FAAC_CONTEXT_H

#include "javamethod.h"
#include "include/faac.h"

#define CURL_JNI_VERSION JNI_VERSION_1_6

static JavaVM *jvm;

static inline JNIEnv *get_env() {
    JNIEnv *env;
    (*jvm)->GetEnv(jvm, (void **)&env, CURL_JNI_VERSION);
    return env;
}

static const char* encodeddata_class = "audio/faac/EncodedData";
static JavaMethod encodeddata_constructor  =     {"<init>", "([BIIZ)V"};

typedef struct StFaacContext
{
    faacEncHandle           hEncoder;
    unsigned long           sampleRate;
    unsigned int            channels;
    unsigned int            bits;
    unsigned long           maxOutputBytes;
    unsigned long           inputSamples;
    unsigned long           pcmBufferSize;
    faacEncConfigurationPtr pConfiguration;

}StFaacContext;

StFaacContext* pFaacContext;
int m_inc;

unsigned char*	pAACBuffer;

#endif //TESTFAAC_AUDIO_FAAC_CONTEXT_H
