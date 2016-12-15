//
// Created by 钟龙州 on 2016/12/2.
//


#include <stdint.h>
#include <malloc.h>
#include <memory.h>
#include <time.h>
#include <sys/types.h>
#include "audio_faac_FaacCodec.h"
#include "audio_faac_context.h"
#include "logger.h"

static const char* TAG = "native_faac++++++";

jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
    LOGI(TAG, "JNI OnLoad !!!!!!!!!!!!!!");
    JNIEnv *env;

    if ((*vm)->GetEnv(vm, (void **)&env, CURL_JNI_VERSION) != JNI_OK) {
        return -1;
    }

    (*env)->GetJavaVM(env, &jvm);

    return CURL_JNI_VERSION;
}


JNIEXPORT jint JNICALL Java_audio_faac_FaacCodec_init
        (JNIEnv *env, jclass jcls, jint jsamplerate, jint jchannels, jint jbits)
{
    LOGI(TAG, "faac init");
    if (pFaacContext != NULL)
    {
        LOGE(TAG, "not need to init faac context");
        return -1;
    }
    pFaacContext = (StFaacContext*)malloc(sizeof(StFaacContext));
    if (pFaacContext == NULL){
        LOGE(TAG, "init faac failed for malloc error");
        return -1;
    }

    m_inc = 0;

    pFaacContext->sampleRate = (unsigned long)jsamplerate;
    pFaacContext->channels = (unsigned int)jchannels;
    pFaacContext->bits = (unsigned int)jbits;
    pFaacContext->maxOutputBytes = 0;
    pFaacContext->inputSamples = 0;

    pFaacContext->hEncoder = faacEncOpen(pFaacContext->sampleRate,
                                         pFaacContext->channels,
                                         &(pFaacContext->inputSamples),
                                         &(pFaacContext->maxOutputBytes));

    pFaacContext->pcmBufferSize = pFaacContext->inputSamples * pFaacContext->bits/8;

    if (pFaacContext->hEncoder == NULL)
    {
        LOGE(TAG, "init faac context failed for encoder open error");
        free(pFaacContext);
        return -1;
    }

    //设置编码器信息
    /*
		PCM Sample Input Format
		0	FAAC_INPUT_NULL			invalid, signifies a misconfigured config
		1	FAAC_INPUT_16BIT		native endian 16bit
		2	FAAC_INPUT_24BIT		native endian 24bit in 24 bits		(not implemented)
		3	FAAC_INPUT_32BIT		native endian 24bit in 32 bits		(DEFAULT)
		4	FAAC_INPUT_FLOAT		32bit floating point
    */
    pFaacContext->pConfiguration = faacEncGetCurrentConfiguration(pFaacContext->hEncoder);
    if (pFaacContext->pConfiguration == NULL)
    {
        LOGE(TAG, "init faac context failed for configuration open error");
        free(pFaacContext);
        return -1;
    }
    faacEncConfigurationPtr pCfg = pFaacContext->pConfiguration;
    pCfg->version = MPEG4;
    pCfg->inputFormat = FAAC_INPUT_16BIT;
    pCfg->outputFormat = 1;//0 = Raw; 1 = ADTS
    // AAC object types
    //#define MAIN 1
    //#define LOW  2
    //#define SSR  3
    //#define LTP  4
    pCfg->aacObjectType = LOW;
    pCfg->allowMidside = 1;
    pCfg->useLfe = 0;//低音炮
    pCfg->useTns = 1;
//    pCfg->shortctl = SHORTCTL_NORMAL;
    pCfg->quantqual = 50;
    pCfg->bitRate = 48000;
//    pCfg->bandWidth = 16000;//hz
    pCfg->bandWidth = 0;//hz

    faacEncSetConfiguration(pFaacContext->hEncoder, pFaacContext->pConfiguration);
    LOGI(TAG, "init faac succeed, sampleRates[%ld], bits[%d], channels[%d], inputSamples[%ld], maxOutputByte[%ld]",
         pFaacContext->sampleRate, pFaacContext->bits, pFaacContext->channels, pFaacContext->inputSamples, pFaacContext->maxOutputBytes);

    pAACBuffer = (unsigned char*)malloc(pFaacContext->maxOutputBytes * sizeof(unsigned char));

    return (int)(pFaacContext->pcmBufferSize);
}

JNIEXPORT jobject JNICALL Java_audio_faac_FaacCodec_encode
        (JNIEnv *env, jclass jcls, jbyteArray jbuffer, jint jinputsize)
{
    if (pFaacContext == NULL)
    {
        LOGE(TAG, "encode failed for context is null");
        return NULL;
    }

    if (jinputsize <= 0)
    {
        LOGE(TAG, "encode failed for input size is 0");
        return NULL;
    }

    LOGI(TAG, "encode buffer size [%d]", jinputsize);
    unsigned long nPCMBufferSize = (unsigned long)jinputsize;
    unsigned long nMaxOutputBytes = pFaacContext->maxOutputBytes;

//    unsigned char*	pbAACBuffer = (unsigned char*)malloc(nMaxOutputBytes * sizeof(unsigned char));

    jbyte* pBuffer = (*env)->GetByteArrayElements(env, jbuffer, 0);

    if (pBuffer == NULL)
    {
        LOGE(TAG, "get buffer failed");
        return NULL;
    }

    unsigned int nInputSamples = jinputsize / (pFaacContext->bits / 8);

    struct timeval tstart;
    gettimeofday(&tstart, NULL);
    LOGI(TAG, "before[%ld], inputSamples[%d], pcmBufferSize[%ld], aacBufferSize[%ld], maxOutputBytes[%ld]", tstart.tv_usec, nInputSamples, nPCMBufferSize, nMaxOutputBytes, nMaxOutputBytes);

    int encodeSize = faacEncEncode(pFaacContext->hEncoder,
                                   (int*)pBuffer,
                                   nInputSamples,
                                   pAACBuffer,
                                   nMaxOutputBytes);

    struct timeval tend;
    gettimeofday(&tend, NULL);

    (*env)->ReleaseByteArrayElements(env, jbuffer, pBuffer, JNI_FALSE);
    (*env)->DeleteLocalRef(env, jbuffer);

    LOGI(TAG, "after[%ld], cost[%ld], inputSamples[%d], pcmBufferSize[%ld], aacBufferSize[%ld], maxOutputBytes[%ld]", tend.tv_usec, tend.tv_usec - tstart.tv_usec, nInputSamples, nPCMBufferSize, nMaxOutputBytes, nMaxOutputBytes);

    LOGI(TAG, "encoded size[%d]", encodeSize);

    if (encodeSize <= 0)
    {
        LOGE(TAG, "encode size is 0");
        return NULL;
    }

    jbyteArray encodedBuffer = NULL;
    jobject encodedData = NULL;
    LOGI(TAG, "encode 1");
    encodedBuffer = (*env)->NewByteArray(env, encodeSize);
    LOGI(TAG, "encode 2");
    if (encodedBuffer == NULL)
    {
        LOGE(TAG, "encode buffer malloc failed");
        return NULL;
    }

//    long timestamp = m_inc * (encodeSize * 1000 / pFaacContext->sampleRate);
//    int timestamp = m_inc * (jinputsize * 1000 / pFaacContext->sampleRate);
    int timestamp = (int) (m_inc * (pFaacContext->inputSamples / pFaacContext->channels * 1000 / pFaacContext->sampleRate));
    LOGI(TAG, "m_inc[%d], encodeSize[%d], sampleRate[%ld], timestamp[%d]", m_inc, encodeSize, pFaacContext->sampleRate, timestamp);
    m_inc++;

    unsigned long isKeyFrame = 1;
//    unsigned char* decodeBuf = (unsigned char*)pBuffer;
//    int ret = faacEncGetDecoderSpecificInfo(pFaacContext->hEncoder, &decodeBuf, &isKeyFrame);
//    LOGI(TAG, "isKeyFrame[%ld], ret[%d]", isKeyFrame, ret);
    LOGI(TAG, "encode 3");
    (*env)->SetByteArrayRegion(env, encodedBuffer, 0, encodeSize, (jbyte*)pAACBuffer);
    LOGI(TAG, "encode 4");
    jclass cls = (*env)->FindClass(env, encodeddata_class);
    LOGI(TAG, "encode 5");
    jmethodID constructor = java_get_method(env, cls, encodeddata_constructor);
    LOGI(TAG, "encode 6");
    jboolean isKey = isKeyFrame > 0 ? JNI_TRUE : JNI_FALSE;
    encodedData = (*env)->NewObject(env, cls, constructor, encodedBuffer, encodeSize, timestamp, isKey);
    LOGI(TAG, "encode 7");

    (*env)->DeleteLocalRef(env, encodedBuffer);

    LOGI(TAG, "encode buffer succeed")
    return encodedData;
}

JNIEXPORT jint JNICALL Java_audio_faac_FaacCodec_uninit
        (JNIEnv *env, jclass jcls)
{
    LOGI(TAG, "faac uninit");

    if (pFaacContext == NULL)
    {
        LOGE(TAG, "uninit failed for context is null");
        return -1;
    }

    faacEncClose(pFaacContext->hEncoder);

    return 0;
}

JNIEXPORT void JNICALL Java_audio_faac_FaacCodec_setDebug
        (JNIEnv *env, jclass jcls, jboolean isDebug)
{
    SETDEBUG(isDebug);
}