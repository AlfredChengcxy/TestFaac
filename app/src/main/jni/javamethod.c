//
// Created by zhonglz on 16/8/11.
//
#include "javamethod.h"

jfieldID java_get_field(JNIEnv *env, char * class_name, JavaField field) {
    jclass clazz = (*env)->FindClass(env, class_name);
    jfieldID jField = (*env)->GetFieldID(env, clazz, field.name, field.signature);
    (*env)->DeleteLocalRef(env, clazz);
    return jField;
}

jmethodID java_get_method(JNIEnv *env, jclass java_class, JavaMethod method) {
    return (*env)->GetMethodID(env, java_class, method.name, method.signature);
}

jmethodID java_get_static_method(JNIEnv *env, jclass java_class, JavaMethod method) {
    return (*env)->GetStaticMethodID(env, java_class, method.name, method.signature);
}

