/*
 * PASTE.h
 *
 *  Created on: 2017. 1. 3.
 *      Author: june
 */
#include <jni.h>
#include <stdio.h>

#ifndef PASTE_H_
#define PASTE_H_
#ifdef __cplusplus
extern "C" {
#endif /* PASTE_H_ */

/*
JNIEXPORT jlong JNICALL Java_org_opencv_samples_facedetect_DetectionBasedTracker_nativeCreateObject
  (JNIEnv * env, jobject obj);
*/
JNIEXPORT jstring
Java_com_dai_t2paste_DAIPA_TEST(JNIEnv *env, jobject obj, jstring fileName, jint orinetation,
                                jbyteArray imgN, jbyteArray imgF);
JNIEXPORT void
Java_com_dai_t2paste_DAIPA_retouchingDrag(JNIEnv *env, jobject obj, jint x, jint y, jint flags);
JNIEXPORT void
Java_com_dai_t2paste_DAIPA_thresControl(JNIEnv *env, jobject obj, jint x1, jint y1, jint x2,
                                        jint y2, jdouble Lthreshold);

JNIEXPORT void
Java_com_dai_t2paste_DAIPA_setPolygon(JNIEnv *env, jobject obj, jintArray x, jintArray y,
                                      jboolean bf);
JNIEXPORT void
Java_com_dai_t2paste_DAIPA_setLine(JNIEnv *env, jobject obj, jint x1, jint y1, jint x2, jint y2,
                                   jboolean bf, jint thickness);
////JNIEXPORT void Java_com_dai_t2paste_DAIPA_setLine(JNIEnv* env, jobject obj, jfloat x1, jfloat y1, jfloat x2, jfloat y2, jboolean bf, jint thickness);
JNIEXPORT void
Java_com_dai_t2paste_DAIPA_setDeltaColor(JNIEnv *env, jobject obj, jint x1, jint y1, jboolean bf,
                                         jint tolerance);

JNIEXPORT void
Java_com_dai_t2paste_DAIPA_setPaintingBackground(JNIEnv *env, jobject obj, jint color);


#ifdef __cplusplus
}
#endif
#endif
