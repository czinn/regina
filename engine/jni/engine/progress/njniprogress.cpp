
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2001, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful, but   *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *  General Public License for more details.                              *
 *                                                                        *
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include "progress/nprogress.h"
#include "jnitools.h"
#include "engine/progress/NJNIProgress.h"

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_progress_NJNIProgress_cancel
        (JNIEnv *env, jobject me) {
    GET_ENGINE_OBJECT(env, NProgress, me)->cancel();
}

JNIEXPORT jstring JNICALL
        Java_normal_engine_implementation_jni_progress_NJNIProgress_getDescription
        (JNIEnv *env, jobject me) {
    return jstringFromNString(env,
        GET_ENGINE_OBJECT(env, NProgress, me)->getDescription());
}

JNIEXPORT jdouble JNICALL
        Java_normal_engine_implementation_jni_progress_NJNIProgress_getPercent
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NProgress, me)->getPercent();
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_progress_NJNIProgress_hasChanged
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NProgress, me)->hasChanged();
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_progress_NJNIProgress_isCancellable
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NProgress, me)->isCancellable();
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_progress_NJNIProgress_isCancelled
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NProgress, me)->isCancelled();
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_progress_NJNIProgress_isFinished
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NProgress, me)->isFinished();
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_progress_NJNIProgress_isPercent
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NProgress, me)->isPercent();
}

