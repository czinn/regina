
/**************************************************************************
 *                                                                        *
 *  Regina - A normal surface theory calculator                           *
 *  Computational engine                                                  *
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

#include "config.h"
#ifdef __NO_INCLUDE_PATHS
    #include "nvertex.h"
    #include "ncomponent.h"
    #include "nboundarycomponent.h"
    #include "ntetrahedron.h"
    #include "jnitools.h"
    #include "NJNIVertex.h"
#else
    #include "engine/triangulation/nvertex.h"
    #include "engine/triangulation/ncomponent.h"
    #include "engine/triangulation/nboundarycomponent.h"
    #include "engine/triangulation/ntetrahedron.h"
    #include "jni/jnitools.h"
    #include "jni/engine/triangulation/NJNIVertex.h"
#endif

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIVertex_getBoundaryComponent
        (JNIEnv *env, jobject me) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NVertex, me)->getBoundaryComponent(),
        "normal/engine/implementation/jni/triangulation/NJNIBoundaryComponent");
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIVertex_getComponent
        (JNIEnv *env, jobject me) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NVertex, me)->getComponent(),
        "normal/engine/implementation/jni/triangulation/NJNIComponent");
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIVertex_getEmbedding
        (JNIEnv *env, jobject me, jlong index) {
    NVertex* vertex = GET_ENGINE_OBJECT(env, NVertex, me);
    const NVertexEmbedding& embedding = vertex->getEmbedding(index);
    jobject tet = CREATE_WRAPPER_OBJECT(env, embedding.getTetrahedron(),
        "normal/engine/implementation/jni/triangulation/NJNITetrahedron");

    jclass embClass =
        env->FindClass("normal/engine/triangulation/NVertexEmbedding");
    if (! embClass)
        return 0;
    jmethodID constructor = env->GetMethodID(embClass, "<init>",
        "(Lnormal/engine/triangulation/NTetrahedron;I)V");
    if (! constructor)
        return 0;
    return env->NewObject(embClass, constructor, tet, embedding.getVertex());
}

JNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIVertex_getLink
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NVertex, me)->getLink();
}

JNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIVertex_getLinkEulerCharacteristic
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NVertex, me)->getLinkEulerCharacteristic();
}

JNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIVertex_getNumberOfEmbeddings
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NVertex, me)->getNumberOfEmbeddings();
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIVertex_isBoundary
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NVertex, me)->isBoundary();
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIVertex_isIdeal
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NVertex, me)->isIdeal();
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIVertex_isLinkClosed
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NVertex, me)->isLinkClosed();
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIVertex_isLinkOrientable
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NVertex, me)->isLinkOrientable();
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIVertex_isStandard
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NVertex, me)->isStandard();
}

