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

#ifdef GAME_RELEASE_DIR_TREE
#include "engine/Globals.hpp"
#include "engine/platform.h"
#include "engine/DDLUtils.hpp"
#include "engine/Engine2.h"
#include "app/main.h"
#else

#include "../../../../../../../engine/Globals.hpp"
#include "../../../../../../../engine/platform.h"
#include "../../../../../../../engine/DDLUtils.hpp"
#include "../../../../../../../engine/Engine2.h"
#include "../../../../../../../app/main.h"

#endif

#define JNI_VERSION_1_6 0x00010006

using namespace std;

JavaVM *g_jvm = 0;

jobject g_objMainActivity = 0;
jclass g_classMainActivity = 0;
jmethodID g_open = 0;
jmethodID g_getWidth = 0;
jmethodID g_getHeight = 0;
jmethodID g_getPixels = 0;
jmethodID g_close = 0;
jmethodID g_assetOpen = 0;
jmethodID g_assetReadByte = 0;
jmethodID g_assetReadInt = 0;
jmethodID g_assetReadFloat = 0;
jmethodID g_getBytesRead = 0;
jmethodID g_getAssetManager = 0;
jmethodID g_loadSound = 0;
jmethodID g_playSound = 0;
jmethodID g_stopSound = 0;
jmethodID g_playTrack = 0;
jmethodID g_stopTrack = 0;
jmethodID g_setTrackVolume = 0;
jmethodID g_log = 0;
jmethodID g_getSurfaceView = 0;
jmethodID g_getSurface = 0;
jmethodID g_loadTextureInJava = 0;
jmethodID g_getWindowWidth = 0;
jmethodID g_getWindowHeight = 0;
jmethodID g_getTime = 0;
jmethodID g_stoi = 0;
jmethodID g_stof = 0;
jmethodID g_oobjloaderLoad = 0;
jmethodID g_oobjloaderGetNumVertices = 0;
jmethodID g_oobjloaderGetVertexX = 0;
jmethodID g_oobjloaderGetVertexY = 0;
jmethodID g_oobjloaderGetVertexZ = 0;
jmethodID g_oobjloaderGetNumTexCoords = 0;
jmethodID g_oobjloaderGetTexCoordX = 0;
jmethodID g_oobjloaderGetTexCoordY = 0;
jmethodID g_oobjloaderGetNumNormals = 0;
jmethodID g_oobjloaderGetNormalX = 0;
jmethodID g_oobjloaderGetNormalY = 0;
jmethodID g_oobjloaderGetNormalZ = 0;
jmethodID g_oobjloaderGetNumFaces = 0;
jmethodID g_oobjloaderGetFaceElement = 0;
jmethodID g_showFileSelector = 0;
jmethodID g_clearListMenu = 0;
jmethodID g_addListMenuOption = 0;
jmethodID g_showListMenuInDialog = 0;
jmethodID g_showText = 0;
jmethodID g_showLongText = 0;
jmethodID g_clearDialog = 0;
jmethodID g_addDialogPart = 0;
jmethodID g_showDialog = 0;
jmethodID g_getCameraPic = 0;
jmethodID g_savePref = 0;
jmethodID g_loadPref = 0;
jmethodID g_startTrackLocation = 0;
jmethodID g_stopTrackLocation = 0;

jobject g_screen;

bool g_attached = false;

char g_filesDir[MAX_STR_LEN];
char g_externalFilesDir[MAX_STR_LEN];
char g_externalStorageDir[MAX_STR_LEN];

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    g_jvm = vm;

    return JNI_VERSION_1_6;
}

extern "C" JNIEXPORT void JNICALL
Java_com_voxyc_voxyc_HelloJni_SetUpBridge(JNIEnv *jni, jclass clazz, jobject activity) {
    g_objMainActivity = jni->NewGlobalRef(activity);
    g_classMainActivity = clazz;
    g_open = jni->GetMethodID(g_classMainActivity, "open",
                              "(Ljava/lang/String;)Landroid/graphics/Bitmap;");
    g_getWidth = jni->GetMethodID(g_classMainActivity, "getWidth", "(Landroid/graphics/Bitmap;)I");
    g_getHeight = jni->GetMethodID(g_classMainActivity, "getHeight",
                                   "(Landroid/graphics/Bitmap;)I");
    g_getPixels = jni->GetMethodID(g_classMainActivity, "getPixels",
                                   "(Landroid/graphics/Bitmap;[I)V");
    g_close = jni->GetMethodID(g_classMainActivity, "close", "(Landroid/graphics/Bitmap;)V");
    g_assetOpen = jni->GetMethodID(g_classMainActivity, "assetOpen", "(Ljava/lang/String;)V");
    g_assetReadByte = jni->GetMethodID(g_classMainActivity, "assetReadByte", "()C");
    g_assetReadInt = jni->GetMethodID(g_classMainActivity, "assetReadInt", "()I");
    g_assetReadFloat = jni->GetMethodID(g_classMainActivity, "assetReadFloat", "()F");
    g_getBytesRead = jni->GetMethodID(g_classMainActivity, "getBytesRead", "()I");
    g_getAssetManager = jni->GetMethodID(g_classMainActivity, "getAssetManager",
                                         "()Landroid/content/res/AssetManager;");
    g_loadSound = jni->GetMethodID(g_classMainActivity, "loadSound",
                                   "(Ljava/lang/String;Ljava/lang/String;)I");
    g_playSound = jni->GetMethodID(g_classMainActivity, "playSound", "(I)I");
    g_stopSound = jni->GetMethodID(g_classMainActivity, "stopSound", "(I)V");
    g_playTrack = jni->GetMethodID(g_classMainActivity, "playTrack",
                                   "(Ljava/lang/String;Ljava/lang/String;Z)V");
    g_stopTrack = jni->GetMethodID(g_classMainActivity, "stopTrack", "()V");
    g_setTrackVolume = jni->GetMethodID(g_classMainActivity, "setTrackVolume", "(F)V");
    g_log = jni->GetMethodID(g_classMainActivity, "log", "(Ljava/lang/String;)V");
    g_getSurfaceView = jni->GetMethodID(g_classMainActivity, "getSurfaceView",
                                        "()Landroid/opengl/GLSurfaceView;");
    g_getSurface = jni->GetMethodID(g_classMainActivity, "getSurface", "()Landroid/view/Surface;");
    g_loadTextureInJava = jni->GetMethodID(g_classMainActivity, "loadTextureInJava",
                                           "(ILjava/lang/String;Ljava/lang/String;)V");
    g_getWindowWidth = jni->GetMethodID(g_classMainActivity, "getWindowWidth", "()I");
    g_getWindowHeight = jni->GetMethodID(g_classMainActivity, "getWindowHeight", "()I");
    g_getTime = jni->GetMethodID(g_classMainActivity, "getTime", "()J");
    g_stoi = jni->GetMethodID(g_classMainActivity, "stoi", "(Ljava/lang/String;I)I");
    g_stof = jni->GetMethodID(g_classMainActivity, "stof", "(Ljava/lang/String;F)F");
/*	g_oobjloaderLoad = jni->GetMethodID(g_classMainActivity, "oobjloaderLoad", "(Ljava/lang/String;)V");
	g_oobjloaderGetNumVertices = jni->GetMethodID(g_classMainActivity, "oobjloaderGetNumVertices", "()I");
	g_oobjloaderGetVertexX = jni->GetMethodID(g_classMainActivity, "oobjloaderGetVertexX", "(I)F");
	g_oobjloaderGetVertexY = jni->GetMethodID(g_classMainActivity, "oobjloaderGetVertexY", "(I)F");
	g_oobjloaderGetVertexZ = jni->GetMethodID(g_classMainActivity, "oobjloaderGetVertexZ", "(I)F");
	g_oobjloaderGetNumTexCoords = jni->GetMethodID(g_classMainActivity, "oobjloaderGetNumTexCoords", "()I");
	g_oobjloaderGetTexCoordX = jni->GetMethodID(g_classMainActivity, "oobjloaderGetTexCoordX", "(I)F");
	g_oobjloaderGetTexCoordY = jni->GetMethodID(g_classMainActivity, "oobjloaderGetTexCoordY", "(I)F");
	g_oobjloaderGetNumNormals = jni->GetMethodID(g_classMainActivity, "oobjloaderGetNumNormals", "()I");
	g_oobjloaderGetNormalX = jni->GetMethodID(g_classMainActivity, "oobjloaderGetNormalX", "(I)F");
	g_oobjloaderGetNormalY = jni->GetMethodID(g_classMainActivity, "oobjloaderGetNormalY", "(I)F");
	g_oobjloaderGetNormalZ = jni->GetMethodID(g_classMainActivity, "oobjloaderGetNormalZ", "(I)F");
	g_oobjloaderGetNumFaces = jni->GetMethodID(g_classMainActivity, "oobjloaderGetNumFaces", "()I");
	g_oobjloaderGetFaceElement = jni->GetMethodID(g_classMainActivity, "oobjloaderGetFaceElement", "(II)I");*/
    g_showFileSelector = jni->GetMethodID(g_classMainActivity, "showFileSelector",
                                          "(Ljava/lang/String;Ljava/lang/String;)V");
    g_clearListMenu = jni->GetMethodID(g_classMainActivity, "clearListMenu", "()V");
    g_addListMenuOption = jni->GetMethodID(g_classMainActivity, "addListMenuOption",
                                           "(Ljava/lang/String;Ljava/lang/String;)V");
    g_showListMenuInDialog = jni->GetMethodID(g_classMainActivity, "showListMenuInDialog",
                                              "(Ljava/lang/String;Ljava/lang/String;)V");
    g_showText = jni->GetMethodID(g_classMainActivity, "showText", "(Ljava/lang/String;)V");
    g_showLongText = jni->GetMethodID(g_classMainActivity, "showLongText", "(Ljava/lang/String;)V");
    g_clearDialog = jni->GetMethodID(g_classMainActivity, "clearDialog", "()V");
    g_addDialogPart = jni->GetMethodID(g_classMainActivity, "addDialogPart",
                                       "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
    g_showDialog = jni->GetMethodID(g_classMainActivity, "showDialog",
                                    "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
    g_getCameraPic = jni->GetMethodID(g_classMainActivity, "getCameraPic", "()Ljava/lang/String;");
    g_savePref = jni->GetMethodID(g_classMainActivity, "savePref",
                                  "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
    g_loadPref = jni->GetMethodID(g_classMainActivity, "loadPref",
                                  "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
								  
	g_startTrackLocation = jni->GetMethodID(g_classMainActivity, "startTrackLocation", "()V");
	g_stopTrackLocation = jni->GetMethodID(g_classMainActivity, "stopTrackLocation", "()V");
}

extern "C" JNIEXPORT void JNICALL
Java_com_voxyc_voxyc_HelloJni_SetScreen(JNIEnv *jni, jclass clazz, jobject bitmap) {
    g_screen = bitmap;
}

extern "C" JNIEXPORT void JNICALL
Java_com_voxyc_voxyc_HelloJni_StartApp(JNIEnv *env, jclass clazz, jint step) {
    StartApp((int) step);
}

extern "C" JNIEXPORT void JNICALL
Java_com_voxyc_voxyc_HelloJni_LoadApp(JNIEnv *env, jclass clazz, jint step) {
    LoadApp((int) step);
}

extern "C" JNIEXPORT void JNICALL Java_com_voxyc_voxyc_HelloJni_AppTick(JNIEnv *env) {
    AppTick();
}

extern "C" JNIEXPORT void JNICALL Java_com_voxyc_voxyc_HelloJni_Draw(JNIEnv *env) {
    Draw(0);
}

extern "C" JNIEXPORT void JNICALL Java_com_voxyc_voxyc_HelloJni_AppFree(JNIEnv *env) {
    AppFree();
}

//extern "C" JNIEXPORT jint JNICALL Java_oculus_MainActivity_attachThread(JNIEnv *env, jobject thiz)
//{
//}

extern "C" JNIEXPORT jint JNICALL Java_oculus_MainActivity_registerEnv(JNIEnv *env, jobject thiz) {
//    AttachThread();

//    g_env = env;
    return 0;
}

/*extern "C" JNIEXPORT jint JNICALL Java_oculus_MainActivity_keyEvent(JNIEnv *env, jobject thiz, int action, int keyCode)
{
    // Action 0 = down
    // Action 1 = up

    Controls *controls = g_app->GetControls();

    if (action == 0)
    {
        switch (keyCode)
        {
            case KEYCODE_DPAD_UP:
                controls->MoveForward();
                break;
            case KEYCODE_DPAD_DOWN:
                controls->MoveBackward();
                break;
            case KEYCODE_DPAD_LEFT:
                controls->MoveLeft();
                break;
            case KEYCODE_DPAD_RIGHT:
                controls->MoveRight();
                break;
        }
    }
}*/

extern "C" JNIEXPORT jint JNICALL
Java_com_voxyc_voxyc_HelloJni_touchEvent(JNIEnv *env, jobject thiz, jint count, jint action1,
                                         jfloat x1, jfloat y1, jint action2, jfloat x2, jfloat y2,
                                         jint actionIndex) {
    if (g_engineApp == nullptr)
        return 0;

    g_engineApp->touchEvent(count, action1, x1, y1, action2, x2, y2, actionIndex);

    return 0;
}

extern "C" JNIEXPORT void JNICALL
Java_com_voxyc_voxyc_HelloJni_enterCommand(JNIEnv *env, jobject thiz, jstring jcmd) {
    const char *cmd = env->GetStringUTFChars(jcmd, 0);

//	g_console.exec((char *)cmd);

    env->ReleaseStringUTFChars(jcmd, cmd);
}

extern "C" JNIEXPORT void JNICALL
Java_com_voxyc_voxyc_HelloJni_setFilesDir(JNIEnv *env, jobject thiz, jstring jdir,
                                          jstring jexternaldfilesdir, jstring jexternalstoragedir) {
    const char *dir = env->GetStringUTFChars(jdir, 0);
    const char *externalfilesdir = env->GetStringUTFChars(jexternaldfilesdir, 0);
    const char *externalstoragedir = env->GetStringUTFChars(jexternalstoragedir, 0);

    sprintf(g_filesDir, "%s", dir);
    sprintf(g_externalFilesDir, "%s", externalfilesdir);
    sprintf(g_externalStorageDir, "%s", externalstoragedir);

    env->ReleaseStringUTFChars(jdir, dir);
    env->ReleaseStringUTFChars(jexternaldfilesdir, externalfilesdir);
    env->ReleaseStringUTFChars(jexternalstoragedir, externalstoragedir);
}

extern "C" JNIEXPORT void JNICALL
Java_com_voxyc_voxyc_HelloJni_setExtraInt(JNIEnv *env, jobject thiz, jstring jname, int jvalue) {
    const char *name = env->GetStringUTFChars(jname, 0);

    g_engine2->setExtraInt(name, jvalue);

    env->ReleaseStringUTFChars(jname, name);
}

extern "C" JNIEXPORT void JNICALL
Java_com_voxyc_voxyc_HelloJni_setExtraStr(JNIEnv *env, jobject thiz, jstring jname,
                                          jstring jvalue) {
    const char *name = env->GetStringUTFChars(jname, 0);
    const char *value = env->GetStringUTFChars(jvalue, 0);

    g_engine2->setExtraStr(name, value);

    env->ReleaseStringUTFChars(jname, name);
    env->ReleaseStringUTFChars(jvalue, value);
}

extern "C" JNIEXPORT void JNICALL
Java_com_voxyc_voxyc_HelloJni_surfaceChanged(JNIEnv *env, jobject thiz) {
    g_common.windowWidth = 0;
    g_common.windowHeight = 0;
}

void PLAT_AttachCurrentThread()
{
    JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);
    g_jvm->AttachCurrentThread((JNIEnv**)&env, NULL);
}

void PLAT_DetachCurrentThread()
{
    JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);
    g_jvm->DetachCurrentThread();
}

void PLAT_LoadBitmap(int **out, unsigned *w, unsigned *h, char *path) {
    JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);

    jstring name = env->NewStringUTF(path);
    jobject png = env->CallObjectMethod(g_objMainActivity, g_open, name);
    env->DeleteLocalRef(name);
    jobject gpng = env->NewGlobalRef(png);

    // Get image dimensions
    int width = env->CallIntMethod(g_objMainActivity, g_getWidth, png);
    int height = env->CallIntMethod(g_objMainActivity, g_getHeight, png);

    // Get pixels
    jintArray array = env->NewIntArray(width * height);
    jobject garray = env->NewGlobalRef(array);
    env->CallVoidMethod(g_objMainActivity, g_getPixels, png, garray);

    jint *pixels = env->GetIntArrayElements((jintArray) garray, 0);

    char bytes[4];
    char bytesOut[4];

    for (int i = 0; i < width * height; i++) {
        int a = pixels[i];

        bytes[3] = a & 0xff;
        bytes[2] = (a >> 8) & 0xff;
        bytes[1] = (a >> 16) & 0xff;
        bytes[0] = (a >> 24) & 0xff;

        bytesOut[0] = bytes[0];
        bytesOut[1] = bytes[3];
        bytesOut[2] = bytes[2];
        bytesOut[3] = bytes[1];

        int b = int((unsigned char) (bytesOut[0]) << 24 |
                    (unsigned char) (bytesOut[1]) << 16 |
                    (unsigned char) (bytesOut[2]) << 8 |
                    (unsigned char) (bytesOut[3]));

        pixels[i] = b;
    }

    *out = pixels;
    *w = width;
    *h = height;

    env->ReleaseIntArrayElements((jintArray) garray, pixels, 0);
    env->DeleteGlobalRef(garray);

    // Free image
    env->CallVoidMethod(g_objMainActivity, g_close, png);
    env->DeleteGlobalRef(gpng);
}

void PLAT_AssetOpen(char *filename) {
    JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);

    jstring jfilename = env->NewStringUTF(filename);
    env->CallVoidMethod(g_objMainActivity, g_assetOpen, jfilename);
    env->DeleteLocalRef(jfilename);
}

char PLAT_AssetReadByte() {
    JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);

    return env->CallCharMethod(g_objMainActivity, g_assetReadByte);
}

int PLAT_AssetReadInt() {
    JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);

    return env->CallIntMethod(g_objMainActivity, g_assetReadInt);
}

int PLAT_AssetReadFloat() {
    JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);

    return env->CallFloatMethod(g_objMainActivity, g_assetReadFloat);
}

int PLAT_GetBytesRead() {
    JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);

    return env->CallIntMethod(g_objMainActivity, g_getBytesRead);
}


jobject PLAT_GetAssetManager() {
    JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);

//    return env->CallObjectMethod(g_objMainActivity, g_getAssetManager);

    jobject am = env->CallObjectMethod(g_objMainActivity, g_getAssetManager);
    jobject gam = env->NewGlobalRef(am);

    return gam;
}

JNIEnv *PLAT_GetEnv() {
    JNIEnv *env;

    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);

    return env;
}

int PLAT_LoadSound(char *filename, bool stereo, std::string assetsdir) {
    JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);

    jstring jfilename = env->NewStringUTF(filename);
    jstring jassetsdir = env->NewStringUTF(assetsdir.c_str());
    int sound = env->CallIntMethod(g_objMainActivity, g_loadSound, jfilename, jassetsdir);
    env->DeleteLocalRef(jassetsdir);
    env->DeleteLocalRef(jfilename);

    return sound;
}

int PLAT_PlaySound(int idx) {
    JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);

    return env->CallIntMethod(g_objMainActivity, g_playSound, idx);
}

void PLAT_StopSound(int stream) {
    JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);

    env->CallVoidMethod(g_objMainActivity, g_stopSound, stream);
}

void PLAT_PlayTrack(char *filename, bool stereo, std::string assetsdir) {
    JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);

    jstring jfilename = env->NewStringUTF(filename);
    jstring jassetsdir = env->NewStringUTF(assetsdir.c_str());
    env->CallVoidMethod(g_objMainActivity, g_playTrack, jfilename, jassetsdir, false);
    env->DeleteLocalRef(jassetsdir);
    env->DeleteLocalRef(jfilename);
}

void PLAT_StopTrack() {
    JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);

    env->CallVoidMethod(g_objMainActivity, g_stopTrack);
}

void PLAT_SetTrackVolume(float vol)
{
    JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);

    env->CallVoidMethod(g_objMainActivity, g_setTrackVolume, vol);
}

void PLAT_Log(char *str) {
    JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);

    jstring jstr = env->NewStringUTF(str);
    env->CallVoidMethod(g_objMainActivity, g_log, jstr);
    env->DeleteLocalRef(jstr);
}

jobject PLAT_GetSurfaceView() {
    JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);

    return env->CallObjectMethod(g_objMainActivity, g_getSurfaceView);
}

jobject PLAT_GetSurface() {
    JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);

    return env->CallObjectMethod(g_objMainActivity, g_getSurface);
}

void PLAT_CopyPixelsToScreen(unsigned char *pixels, int width, int height) {
}

void PLAT_LoadTextureInJava(int glTexID, std::string filename, std::string assetsdir) {
    JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);

    jstring jfilename = env->NewStringUTF(filename.c_str());
    jstring jassetsdir = env->NewStringUTF(assetsdir.c_str());
    env->CallVoidMethod(g_objMainActivity, g_loadTextureInJava, glTexID, jfilename, jassetsdir);
    env->DeleteLocalRef(jfilename);
    env->DeleteLocalRef(jassetsdir);
}

int PLAT_GetWindowWidth() {
    JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);

    if (g_common.windowWidth == 0)
        g_common.windowWidth = env->CallIntMethod(g_objMainActivity, g_getWindowWidth);

    return g_common.windowWidth;
}

int PLAT_GetWindowHeight() {
    JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);

    if (g_common.windowHeight == 0)
        g_common.windowHeight = env->CallIntMethod(g_objMainActivity, g_getWindowHeight);

    return g_common.windowHeight;
}

long PLAT_GetTime() {
    JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);

    return env->CallLongMethod(g_objMainActivity, g_getTime);
}

int PLAT_stoi(std::string s, int fallback) {
    JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);

    jstring js = env->NewStringUTF(s.c_str());
    int r = env->CallIntMethod(g_objMainActivity, g_stoi, js, fallback);
    env->DeleteLocalRef(js);

    return r;
}

float PLAT_stof(std::string s, float fallback) {
    JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);

    jstring js = env->NewStringUTF(s.c_str());
    float r = env->CallFloatMethod(g_objMainActivity, g_stof, js, fallback);
    env->DeleteLocalRef(js);

    return r;
}

void PLAT_oobjloaderLoad(char *filename) {
    JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);

    jstring jfilename = env->NewStringUTF(filename);
    env->CallVoidMethod(g_objMainActivity, g_oobjloaderLoad, jfilename);
    env->DeleteLocalRef(jfilename);
}

int PLAT_oobjloaderGetNumVertices() {
    JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);

    return env->CallIntMethod(g_objMainActivity, g_oobjloaderGetNumVertices);
}

float PLAT_oobjloaderGetVertexX(int index) {
    JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);

    return env->CallFloatMethod(g_objMainActivity, g_oobjloaderGetVertexX, index);
}

float PLAT_oobjloaderGetVertexY(int index) {
    JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);

    return env->CallFloatMethod(g_objMainActivity, g_oobjloaderGetVertexY, index);
}

float PLAT_oobjloaderGetVertexZ(int index) {
    JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);

    return env->CallFloatMethod(g_objMainActivity, g_oobjloaderGetVertexZ, index);
}

int PLAT_oobjloaderGetNumTexCoords() {
    JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);

    return env->CallIntMethod(g_objMainActivity, g_oobjloaderGetNumTexCoords);
}


float PLAT_oobjloaderGetTexCoordX(int index) {
    JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);

    return env->CallFloatMethod(g_objMainActivity, g_oobjloaderGetTexCoordX, index);
}

float PLAT_oobjloaderGetTexCoordY(int index) {
    JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);

    return env->CallFloatMethod(g_objMainActivity, g_oobjloaderGetTexCoordY, index);
}

int PLAT_oobjloaderGetNumNormals() {
    JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);

    return env->CallIntMethod(g_objMainActivity, g_oobjloaderGetNumNormals);
}


float PLAT_oobjloaderGetNormalX(int index) {
    JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);

    return env->CallFloatMethod(g_objMainActivity, g_oobjloaderGetNormalX, index);
}

float PLAT_oobjloaderGetNormalY(int index) {
    JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);

    return env->CallFloatMethod(g_objMainActivity, g_oobjloaderGetNormalY, index);
}

float PLAT_oobjloaderGetNormalZ(int index) {
    JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);

    return env->CallFloatMethod(g_objMainActivity, g_oobjloaderGetNormalZ, index);
}

int PLAT_oobjloaderGetNumFaces() {
    JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);

    return env->CallIntMethod(g_objMainActivity, g_oobjloaderGetNumFaces);
}


int PLAT_oobjloaderGetFaceElement(int face, int element) {
    JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);

    return env->CallIntMethod(g_objMainActivity, g_oobjloaderGetFaceElement, face, element);
}

void PLAT_ShowFileSelector(std::string ext, std::string dir) {
    JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);

    jstring jext = env->NewStringUTF(ext.c_str());
    jstring jdir = env->NewStringUTF(dir.c_str());
    env->CallVoidMethod(g_objMainActivity, g_showFileSelector, jext, jdir);
    env->DeleteLocalRef(jdir);
    env->DeleteLocalRef(jext);
}

void PLAT_ClearListMenu() {
    JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);

    env->CallVoidMethod(g_objMainActivity, g_clearListMenu);
}

void PLAT_AddListMenuOption(std::string title, std::string desc) {
    JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);

    jstring jtitle = env->NewStringUTF(title.c_str());
    jstring jdesc = env->NewStringUTF(desc.c_str());
    env->CallVoidMethod(g_objMainActivity, g_addListMenuOption, jtitle, jdesc);
    env->DeleteLocalRef(jdesc);
    env->DeleteLocalRef(jtitle);
}

void PLAT_ShowListMenuInDialog(std::string title, std::string options) {
    JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);

    jstring jtitle = env->NewStringUTF(title.c_str());
    jstring joptions = env->NewStringUTF(options.c_str());
    env->CallVoidMethod(g_objMainActivity, g_showListMenuInDialog, jtitle, joptions);
    env->DeleteLocalRef(joptions);
    env->DeleteLocalRef(jtitle);
}

void PLAT_ShowText(std::string text) {
    JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);

    jstring jtext = env->NewStringUTF(text.c_str());
    env->CallVoidMethod(g_objMainActivity, g_showText, jtext);
    env->DeleteLocalRef(jtext);
}

void PLAT_ShowLongText(std::string text) {
    JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);

    jstring jtext = env->NewStringUTF(text.c_str());
    env->CallVoidMethod(g_objMainActivity, g_showLongText, jtext);
    env->DeleteLocalRef(jtext);
}

void PLAT_ClearDialog() {
    JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);

    env->CallVoidMethod(g_objMainActivity, g_clearDialog);
}

void PLAT_AddDialogPart(std::string s1, std::string s2, std::string s3) {
    JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);

    jstring js1 = env->NewStringUTF(s1.c_str());
    jstring js2 = env->NewStringUTF(s2.c_str());
    jstring js3 = env->NewStringUTF(s3.c_str());
    env->CallVoidMethod(g_objMainActivity, g_addDialogPart, js1, js2, js3);
    env->DeleteLocalRef(js1);
    env->DeleteLocalRef(js2);
    env->DeleteLocalRef(js3);
}

void PLAT_ShowDialog(std::string s1, std::string s2, std::string s3, std::string s4) {
    JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);

    jstring js1 = env->NewStringUTF(s1.c_str());
    jstring js2 = env->NewStringUTF(s2.c_str());
    jstring js3 = env->NewStringUTF(s3.c_str());
    jstring js4 = env->NewStringUTF(s4.c_str());
    env->CallVoidMethod(g_objMainActivity, g_showDialog, js1, js2, js3, js4);
    env->DeleteLocalRef(js1);
    env->DeleteLocalRef(js2);
    env->DeleteLocalRef(js3);
    env->DeleteLocalRef(js4);
}

std::string PLAT_GetCameraPic() {
    JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);

    std::string fname = "";

    jstring resultJNIStr = (jstring) env->CallObjectMethod(g_objMainActivity, g_getCameraPic);
    const char *resultCStr = env->GetStringUTFChars(resultJNIStr, NULL);
    fname = resultCStr;
    env->ReleaseStringUTFChars(resultJNIStr, resultCStr);

    return fname;
}

void PLAT_SavePref(std::string s1, std::string s2, std::string s3) {
    JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);

    jstring js1 = env->NewStringUTF(s1.c_str());
    jstring js2 = env->NewStringUTF(s2.c_str());
    jstring js3 = env->NewStringUTF(s3.c_str());
    env->CallVoidMethod(g_objMainActivity, g_savePref, js1, js2, js3);
    env->DeleteLocalRef(js1);
    env->DeleteLocalRef(js2);
    env->DeleteLocalRef(js3);
}

std::string PLAT_LoadPref(std::string s1, std::string s2, std::string s3) {
    JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);

    jstring js1 = env->NewStringUTF(s1.c_str());
    jstring js2 = env->NewStringUTF(s2.c_str());
    jstring js3 = env->NewStringUTF(s3.c_str());
    jstring jresult = (jstring) env->CallObjectMethod(g_objMainActivity, g_loadPref, js1, js2, js3);
    const char *result = env->GetStringUTFChars(jresult, NULL);
    env->DeleteLocalRef(js1);
    env->DeleteLocalRef(js2);
    env->DeleteLocalRef(js3);

    return result;
}

void PLAT_StartTrackLocation()
{
	JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);

    env->CallVoidMethod(g_objMainActivity, g_startTrackLocation);
}

void PLAT_StopTrackLocation()
{
    JNIEnv *env;
    g_jvm->GetEnv((void **) &env, JNI_VERSION_1_6);
	
	env->CallVoidMethod(g_objMainActivity, g_stopTrackLocation);
}
