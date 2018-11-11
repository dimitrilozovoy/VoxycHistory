/*
Copyright (C) 2018 Dimitri Lozovoy

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef ANDROID_H
#define ANDROID_H

#ifdef PLATFORM_ANDROID

#include <jni.h>

extern JavaVM *g_jvm;

extern jobject g_objMainActivity;
extern jclass g_classMainActivity;
extern jmethodID g_open;
extern jmethodID g_getWidth;
extern jmethodID g_getHeight;
extern jmethodID g_getPixels;
extern jmethodID g_close;
extern jmethodID g_assetOpen;
extern jmethodID g_assetReadByte;
extern jmethodID g_assetReadInt;
extern jmethodID g_assetReadFloat;
extern jmethodID g_getAssetManager;
extern jmethodID g_loadSound;
extern jmethodID g_playSound;
extern jmethodID g_stopSound;
extern jmethodID g_playTrack;
extern jmethodID g_stopTrack;
extern jmethodID g_log;
extern jmethodID g_getSurfaceView;
extern jmethodID g_getSurface;
extern jmethodID g_oobjloaderLoad;

extern char g_filesDir[];
extern char g_externalFilesDir[];
extern char g_externalStorageDir[];

JNIEnv *PLAT_GetEnv();
jobject PLAT_GetSurfaceView();
jobject PLAT_GetSurface();
void PLAT_oobjloaderLoad(char *filename);
int PLAT_oobjloaderGetNumVertices();
float PLAT_oobjloaderGetVertexX(int index);
float PLAT_oobjloaderGetVertexY(int index);
float PLAT_oobjloaderGetVertexZ(int index);
int PLAT_oobjloaderGetNumTexCoords();
float PLAT_oobjloaderGetTexCoordX(int index);
float PLAT_oobjloaderGetTexCoordY(int index);
int PLAT_oobjloaderGetNumNormals();
float PLAT_oobjloaderGetNormalX(int index);
float PLAT_oobjloaderGetNormalY(int index);
float PLAT_oobjloaderGetNormalZ(int index);
int PLAT_oobjloaderGetNumFaces();
int PLAT_oobjloaderGetFaceElement(int face, int element);

#endif

#endif

