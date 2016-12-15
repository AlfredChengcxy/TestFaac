//
// Created by zhonglz on 16/8/11.
//

#ifndef ALEXA_JAVAMETHOD_H
#define ALEXA_JAVAMETHOD_H

#include <jni.h>

typedef struct {
    const char* name;
    const char* signature;
} JavaMethod;

typedef struct {
    const char* name;
    const char* signature;
} JavaField;

jfieldID  java_get_field(JNIEnv *env, char * class_name, JavaField field);
jmethodID java_get_method(JNIEnv *env, jclass java_class, JavaMethod method);
jmethodID java_get_static_method(JNIEnv *env, jclass java_class, JavaMethod method);


#endif //ALEXA_JAVAMETHOD_H
