
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
    #include "nedge.h"
    #include "ncomponent.h"
    #include "nboundarycomponent.h"
    #include "ntetrahedron.h"
    #include "jnitools.h"
    #include "NJNIEdge.h"
#else
    #include "engine/triangulation/nedge.h"
    #include "engine/triangulation/ncomponent.h"
    #include "engine/triangulation/nboundarycomponent.h"
    #include "engine/triangulation/ntetrahedron.h"
    #include "jni/jnitools.h"
    #include "jni/engine/triangulation/NJNIEdge.h"
#endif

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIEdge_getBoundaryComponent
        (JNIEnv *env, jobject me) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NEdge, me)->getBoundaryComponent(),
        "normal/engine/implementation/jni/triangulation/NJNIBoundaryComponent");
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIEdge_getComponent
        (JNIEnv *env, jobject me) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NEdge, me)->getComponent(),
        "normal/engine/implementation/jni/triangulation/NJNIComponent");
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIEdge_getEmbedding
        (JNIEnv *env, jobject me, jlong index) {
    NEdge* edge = GET_ENGINE_OBJECT(env, NEdge, me);
    const NEdgeEmbedding& embedding = edge->getEmbedding(index);
    jobject tet = CREATE_WRAPPER_OBJECT(env, embedding.getTetrahedron(),
        "normal/engine/implementation/jni/triangulation/NJNITetrahedron");

    jclass embClass =
        env->FindClass("normal/engine/triangulation/NEdgeEmbedding");
    if (! embClass)
        return 0;
    jmethodID constructor = env->GetMethodID(embClass, "<init>",
        "(Lnormal/engine/triangulation/NTetrahedron;I)V");
    if (! constructor)
        return 0;
    return env->NewObject(embClass, constructor, tet,
        embedding.getEdge());
}

JNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIEdge_getNumberOfEmbeddings
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NEdge, me)->getNumberOfEmbeddings();
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIEdge_isBoundary
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NEdge, me)->isBoundary();
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIEdge_isValid
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NEdge, me)->isValid();
}

