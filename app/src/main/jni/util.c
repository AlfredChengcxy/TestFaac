//
// Created by zhonglz on 16/8/6.
//

#include "util.h"
#include <stddef.h>
#include <string.h>

jbyteArray char2jbyteArray(JNIEnv *env, char* pat, unsigned int nSize)
{
    int i = 0;
    if(nSize <= 0 || pat == NULL)
    {
        return NULL;
    }

    jbyteArray tempJByteArray = (*env)->NewByteArray(env, nSize);
    jbyte *bytes = (*env)->GetByteArrayElements(env, tempJByteArray, 0);

    while(i < nSize)
    {
        bytes[i] = pat[i];
        i++;
    }

    (*env)->SetByteArrayRegion(env, tempJByteArray, 0, nSize, bytes );

    return tempJByteArray;
}

jstring char2Jstring(JNIEnv* env, char* pat)
{
    jclass strClass = (*env)->FindClass(env, "java/lang/String");
    jmethodID ctorID = (*env)->GetMethodID(env, strClass, "<init>",	"([BLjava/lang/String;)V");
    jbyteArray bytes = (*env)->NewByteArray(env, strlen(pat));
    (*env)->SetByteArrayRegion(env, bytes, 0, strlen(pat), (jbyte*) pat);
    jstring encoding = (*env)->NewStringUTF(env, "utf-8");
    return (jstring) (*env)->NewObject(env, strClass, ctorID, bytes, encoding);
}