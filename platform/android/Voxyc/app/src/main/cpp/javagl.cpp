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
#include <string.h>
#include <jni.h>
#include "javagl.h"
#include "android.h"
#include "engine/DDLUtils.hpp"
#else
#include "../../../../../../../engine/Globals.hpp"
#include <string.h>
#include <jni.h>
#include "javagl.h"
#include "android.h"
#include "../../../../../../../engine/DDLUtils.hpp"
#endif

//#define JNI_VERSION_1_6 0x00010006

using namespace std;

float g_gles = 0.0;

jobject g_objCustomRenderer = 0;
jclass g_classCustomRenderer = 0;

jmethodID g_glActiveTexture = 0;
jmethodID g_glAttachShader = 0;
jmethodID g_glBindBuffer = 0;
jmethodID g_glBindFramebuffer = 0;
jmethodID g_glBindFragDataLocation = 0;
jmethodID g_glBindRenderbuffer = 0;
jmethodID g_glBindTexture = 0;
jmethodID g_glBindVertexArray = 0;
jmethodID g_glBlendFunc = 0;
jmethodID g_glBufferData = 0;
jmethodID g_glCheckFramebufferStatus = 0;
jmethodID g_glClear = 0;
jmethodID g_glClearDepth = 0;
jmethodID g_glClearColor = 0;
jmethodID g_glCompileShader = 0;
jmethodID g_glCreateProgram = 0;
jmethodID g_glCreateShader = 0;
jmethodID g_glCullFace = 0;
jmethodID g_glDeleteBuffers = 0;
jmethodID g_glDeleteProgram = 0;
jmethodID g_glDeleteTextures = 0;
jmethodID g_glDeleteVertexArrays = 0;
jmethodID g_glDepthFunc = 0;
jmethodID g_glDepthMask = 0;
jmethodID g_glDepthRange = 0;
jmethodID g_glDisable = 0;
jmethodID g_glDrawArrays = 0;
jmethodID g_glDrawElements = 0;
jmethodID g_glEnable = 0;
jmethodID g_glEnableVertexAttribArray = 0;
jmethodID g_glFlush = 0;
jmethodID g_glFramebufferRenderbuffer = 0;
jmethodID g_glFramebufferTexture2D = 0;
jmethodID g_glFrontFace = 0;
jmethodID g_glGenBuffers = 0;
jmethodID g_glGenFramebuffers = 0;
jmethodID g_glGenRenderbuffers = 0;
jmethodID g_glGenVertexArrays = 0;
jmethodID g_glGenTextures = 0;
jmethodID g_glGetAttribLocation = 0;
jmethodID g_glGetError = 0;
jmethodID g_glGetProgramInfoLog = 0;
jmethodID g_glGetProgramiv = 0;
jmethodID g_glGetShaderInfoLog = 0;
jmethodID g_glGetShaderiv = 0;
jmethodID g_glGetUniformLocation = 0;
jmethodID g_glLinkProgram = 0;
jmethodID g_glRenderbufferStorage = 0;
jmethodID g_glShaderSource = 0;
jmethodID g_glTexImage2D = 0;
jmethodID g_glTexImage2Db = 0;
jmethodID g_glTexParameterf = 0;
jmethodID g_glTexParameteri = 0;
jmethodID g_glUniform1f = 0;
jmethodID g_glUniform1fv = 0;
jmethodID g_glUniform1i = 0;
jmethodID g_glUniform2f = 0;
jmethodID g_glUniform2fv = 0;
jmethodID g_glUniform4f = 0;
jmethodID g_glUniform4fv = 0;
jmethodID g_glUniformMatrix4fv = 0;
jmethodID g_glUseProgram = 0;
jmethodID g_glVertexAttribPointer = 0;
jmethodID g_glViewport = 0;

void SetUpGLBridge(JNIEnv *jni, jclass clazz, jobject activity)
{
    g_objCustomRenderer = jni->NewGlobalRef(activity);
    g_classCustomRenderer = clazz;
  
    g_glActiveTexture = jni->GetMethodID(g_classCustomRenderer, "glActiveTexture", "(I)V");
    g_glAttachShader = jni->GetMethodID(g_classCustomRenderer, "glAttachShader", "(II)V");
    g_glBindBuffer = jni->GetMethodID(g_classCustomRenderer, "glBindBuffer", "(II)V");
    g_glBindFramebuffer = jni->GetMethodID(g_classCustomRenderer, "glBindFramebuffer", "(II)V");
    g_glBindRenderbuffer = jni->GetMethodID(g_classCustomRenderer, "glBindRenderbuffer", "(II)V");
    g_glBindTexture = jni->GetMethodID(g_classCustomRenderer, "glBindTexture", "(II)V");
    g_glBindVertexArray = jni->GetMethodID(g_classCustomRenderer, "glBindVertexArray", "(I)V");
    g_glBlendFunc = jni->GetMethodID(g_classCustomRenderer, "glBlendFunc", "(II)V");
    g_glBufferData = jni->GetMethodID(g_classCustomRenderer, "glBufferData", "(II[FI)V");
    g_glCheckFramebufferStatus = jni->GetMethodID(g_classCustomRenderer, "glCheckFramebufferStatus", "(I)I");
	g_glClear = jni->GetMethodID(g_classCustomRenderer, "glClear", "(I)V");
    g_glClearColor = jni->GetMethodID(g_classCustomRenderer, "glClearColor", "(FFFF)V");
    g_glClearDepth = jni->GetMethodID(g_classCustomRenderer, "glClearDepth", "(F)V");
    g_glCompileShader = jni->GetMethodID(g_classCustomRenderer, "glCompileShader", "(I)V");
    g_glCreateProgram = jni->GetMethodID(g_classCustomRenderer, "glCreateProgram", "()I");
    g_glCreateShader = jni->GetMethodID(g_classCustomRenderer, "glCreateShader", "(I)I");
    g_glCullFace = jni->GetMethodID(g_classCustomRenderer, "glCullFace", "(I)V");
    g_glDeleteBuffers = jni->GetMethodID(g_classCustomRenderer, "glDeleteBuffers", "(I[I)V");
    g_glDeleteProgram = jni->GetMethodID(g_classCustomRenderer, "glDeleteProgram", "(I)V");
    g_glDeleteTextures = jni->GetMethodID(g_classCustomRenderer, "glDeleteTextures", "(I[I)V");
	g_glDeleteVertexArrays = jni->GetMethodID(g_classCustomRenderer, "glDeleteVertexArrays", "(I[I)V");
    g_glDepthFunc = jni->GetMethodID(g_classCustomRenderer, "glDepthFunc", "(I)V");
    g_glDepthMask = jni->GetMethodID(g_classCustomRenderer, "glDepthMask", "(Z)V");
    g_glDepthRange = jni->GetMethodID(g_classCustomRenderer, "glDepthRange", "(FF)V");
    g_glDisable = jni->GetMethodID(g_classCustomRenderer, "glDisable", "(I)V");
    g_glDrawArrays = jni->GetMethodID(g_classCustomRenderer, "glDrawArrays", "(III)V");
    g_glDrawElements = jni->GetMethodID(g_classCustomRenderer, "glDrawElements", "(IIII)V");
    g_glEnable = jni->GetMethodID(g_classCustomRenderer, "glEnable", "(I)V");
    g_glEnableVertexAttribArray = jni->GetMethodID(g_classCustomRenderer, "glEnableVertexAttribArray", "(I)V");
    g_glFlush = jni->GetMethodID(g_classCustomRenderer, "glFlush", "()V");
    g_glFramebufferRenderbuffer = jni->GetMethodID(g_classCustomRenderer, "glFramebufferRenderbuffer", "(IIII)V");
	g_glFramebufferTexture2D = jni->GetMethodID(g_classCustomRenderer, "glFramebufferTexture2D", "(IIIII)V");
    g_glFrontFace = jni->GetMethodID(g_classCustomRenderer, "glFrontFace", "(I)V");
    g_glGenBuffers = jni->GetMethodID(g_classCustomRenderer, "glGenBuffers", "(I[I)V");
    g_glGenFramebuffers = jni->GetMethodID(g_classCustomRenderer, "glGenFramebuffers", "(I[I)V");
 	g_glGenRenderbuffers = jni->GetMethodID(g_classCustomRenderer, "glGenRenderbuffers", "(I[I)V");
	g_glGenVertexArrays = jni->GetMethodID(g_classCustomRenderer, "glGenVertexArrays", "(I[I)V");
    g_glGenTextures = jni->GetMethodID(g_classCustomRenderer, "glGenTextures", "(I[I)V");
	g_glGetAttribLocation = jni->GetMethodID(g_classCustomRenderer, "glGetAttribLocation", "(ILjava/lang/String;)I");
    g_glGetError = jni->GetMethodID(g_classCustomRenderer, "glGetError", "()I");
    g_glGetProgramInfoLog = jni->GetMethodID(g_classCustomRenderer, "glGetProgramInfoLog", "(I)Ljava/lang/String;");
    g_glGetProgramiv = jni->GetMethodID(g_classCustomRenderer, "glGetProgramiv", "(II[I)V");
    g_glGetShaderInfoLog = jni->GetMethodID(g_classCustomRenderer, "glGetShaderInfoLog", "(I)Ljava/lang/String;");
    g_glGetShaderiv = jni->GetMethodID(g_classCustomRenderer, "glGetShaderiv", "(II[I)V");
    g_glGetUniformLocation = jni->GetMethodID(g_classCustomRenderer, "glGetUniformLocation", "(ILjava/lang/String;)I");
    g_glLinkProgram = jni->GetMethodID(g_classCustomRenderer, "glLinkProgram", "(I)V");
    g_glRenderbufferStorage = jni->GetMethodID(g_classCustomRenderer, "glRenderbufferStorage", "(IIII)V");
    g_glShaderSource = jni->GetMethodID(g_classCustomRenderer, "glShaderSource", "(ILjava/lang/String;)V");
	g_glTexImage2D = jni->GetMethodID(g_classCustomRenderer, "glTexImage2D", "(IIIIIIII[I)V");
	g_glTexImage2Db = jni->GetMethodID(g_classCustomRenderer, "glTexImage2Db", "(IIIIIIII[B)V");
	g_glTexParameterf = jni->GetMethodID(g_classCustomRenderer, "glTexParameterf", "(IIF)V");
    g_glTexParameteri = jni->GetMethodID(g_classCustomRenderer, "glTexParameteri", "(III)V");
    g_glUniform1f = jni->GetMethodID(g_classCustomRenderer, "glUniform1f", "(IF)V");
    g_glUniform1i = jni->GetMethodID(g_classCustomRenderer, "glUniform1i", "(II)V");
    g_glUniform1fv = jni->GetMethodID(g_classCustomRenderer, "glUniform1fv", "(II[F)V");
    g_glUniform2f = jni->GetMethodID(g_classCustomRenderer, "glUniform2f", "(IFF)V");
    g_glUniform2fv = jni->GetMethodID(g_classCustomRenderer, "glUniform2fv", "(II[F)V");
    g_glUniform4f = jni->GetMethodID(g_classCustomRenderer, "glUniform4f", "(IFFFF)V");
    g_glUniform4fv = jni->GetMethodID(g_classCustomRenderer, "glUniform4fv", "(II[F)V");
	g_glUniformMatrix4fv = jni->GetMethodID(g_classCustomRenderer, "glUniformMatrix4fv", "(IIZ[F)V");
    g_glUseProgram = jni->GetMethodID(g_classCustomRenderer, "glUseProgram", "(I)V");
    g_glVertexAttribPointer = jni->GetMethodID(g_classCustomRenderer, "glVertexAttribPointer", "(IIIZII)V");
    g_glViewport = jni->GetMethodID(g_classCustomRenderer, "glViewport", "(IIII)V");
	
/*
    g_open = jni->GetMethodID(g_classMainActivity, "open", "(Ljava/lang/String;)Landroid/graphics/Bitmap;");
    g_getWidth = jni->GetMethodID(g_classMainActivity, "getWidth", "(Landroid/graphics/Bitmap;)I");
    g_getHeight = jni->GetMethodID(g_classMainActivity, "getHeight", "(Landroid/graphics/Bitmap;)I");
    g_getPixels = jni->GetMethodID(g_classMainActivity, "getPixels", "(Landroid/graphics/Bitmap;[I)V");
    g_close = jni->GetMethodID(g_classMainActivity, "close", "(Landroid/graphics/Bitmap;)V");
    g_assetOpen = jni->GetMethodID(g_classMainActivity, "assetOpen", "(Ljava/lang/String;)V");
    g_assetReadByte = jni->GetMethodID(g_classMainActivity, "assetReadByte", "()C");
    g_assetReadInt = jni->GetMethodID(g_classMainActivity, "assetReadInt", "()I");
    g_assetReadFloat = jni->GetMethodID(g_classMainActivity, "assetReadFloat", "()F");
    g_getAssetManager = jni->GetMethodID(g_classMainActivity, "getAssetManager", "()Landroid/content/res/AssetManager;");
    g_loadSound = jni->GetMethodID(g_classMainActivity, "loadSound", "(Ljava/lang/String;)I");
    g_playSound = jni->GetMethodID(g_classMainActivity, "playSound", "(I)I");
    g_stopSound = jni->GetMethodID(g_classMainActivity, "stopSound", "(I)V");
    g_playTrack = jni->GetMethodID(g_classMainActivity, "playTrack", "(Ljava/lang/String;)V");
    g_stopTrack = jni->GetMethodID(g_classMainActivity, "stopTrack", "()V");
  g_log = jni->GetMethodID(g_classMainActivity, "log", "(Ljava/lang/String;)V");
  g_getSurfaceView = jni->GetMethodID(g_classMainActivity, "getSurfaceView", "()Landroid/opengl/GLSurfaceView;");
  g_getSurface = jni->GetMethodID(g_classMainActivity, "getSurface", "()Landroid/view/Surface;");*/
}

extern "C" JNIEXPORT void JNICALL Java_com_voxyc_voxyc_CustomRenderer_SetUpGLBridge(JNIEnv *jni, jclass clazz, jobject activity)
{
	g_gles = 3.0;
	
	SetUpGLBridge(jni, clazz, activity);
}

extern "C" JNIEXPORT void JNICALL Java_com_voxyc_voxyc_CustomRendererES20_SetUpGLBridge(JNIEnv *jni, jclass clazz, jobject activity)
{
	g_gles = 2.0;
	
	SetUpGLBridge(jni, clazz, activity);
}

void glActiveTexture(GLenum texture)
{
	JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    env->CallVoidMethod(g_objCustomRenderer, g_glActiveTexture, texture);
}

void glAttachShader(GLuint program, GLuint shader)
{
	JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    env->CallVoidMethod(g_objCustomRenderer, g_glAttachShader, program, shader);
}

void glBindBuffer(GLenum target, GLuint buffer)
{
	JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    env->CallVoidMethod(g_objCustomRenderer, g_glBindBuffer, target, buffer);
}

void glBindFramebuffer(GLenum target, GLuint buffer)
{
	JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    env->CallVoidMethod(g_objCustomRenderer, g_glBindFramebuffer, target, buffer);
}

void glBindRenderbuffer(GLenum target, GLuint buffer)
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    env->CallVoidMethod(g_objCustomRenderer, g_glBindRenderbuffer, target, buffer);
}

void glBindTexture(GLenum target, GLuint texture)
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    env->CallVoidMethod(g_objCustomRenderer, g_glBindTexture, target, texture);
}

void glBindVertexArray(GLuint array)
{
   JNIEnv *env;
   g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

   env->CallVoidMethod(g_objCustomRenderer, g_glBindVertexArray, array);
}

void glBlendFunc(GLenum sfactor, GLenum dfactor)
{
   JNIEnv *env;
   g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

   env->CallVoidMethod(g_objCustomRenderer, g_glBlendFunc, sfactor, dfactor);
}

void glBufferData(GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage)
{
   JNIEnv *env;
   g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);
   
   jfloatArray jarray = env->NewFloatArray(size / 4);
   env->SetFloatArrayRegion(jarray, 0, size / 4, (const jfloat *)data);
   env->CallVoidMethod(g_objCustomRenderer, g_glBufferData, target, size, jarray, usage);
   env->DeleteLocalRef(jarray);
}

GLenum glCheckFramebufferStatus(GLenum fb)
{
   JNIEnv *env;
   g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

   return env->CallIntMethod(g_objCustomRenderer, g_glCheckFramebufferStatus, fb);
}

void glClear(GLbitfield mask)
{
   JNIEnv *env;
   g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

   env->CallVoidMethod(g_objCustomRenderer, g_glClear, mask);
}

void glClearColor( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
   JNIEnv *env;
   g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

   env->CallVoidMethod(g_objCustomRenderer, g_glClearColor, red, green, blue, alpha);
}

void glClearDepth(GLclampd depth)
{
   JNIEnv *env;
   g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

   env->CallVoidMethod(g_objCustomRenderer, g_glClearDepth, depth);
}

void glCompileShader(GLuint shader)
{
   JNIEnv *env;
   g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

   env->CallVoidMethod(g_objCustomRenderer, g_glCompileShader, shader);
}

int glCreateProgram()
{
   JNIEnv *env;
   g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

   return env->CallIntMethod(g_objCustomRenderer, g_glCreateProgram);
}

int glCreateShader(GLenum shaderType)
{
   JNIEnv *env;
   g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

   return env->CallIntMethod(g_objCustomRenderer, g_glCreateShader, shaderType);
}

void glCullFace(GLenum mode)
{
   JNIEnv *env;
   g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

   env->CallVoidMethod(g_objCustomRenderer, g_glCullFace, mode);
}

void glDeleteBuffers(GLsizei n, GLuint *arrays)
{
   JNIEnv *env;
   g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);
   
   jintArray jarray = env->NewIntArray(n);
   env->SetIntArrayRegion(jarray, 0, n, (const jint *)arrays);
   env->CallVoidMethod(g_objCustomRenderer, g_glDeleteBuffers, n, jarray);
   env->DeleteLocalRef(jarray);
}

void glDeleteProgram(GLuint program)
{
   JNIEnv *env;
   g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

   env->CallVoidMethod(g_objCustomRenderer, g_glDeleteProgram, program);
}

void glDeleteTextures(	GLsizei n,
 	const GLuint * textures)
{
   JNIEnv *env;
   g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);
   
   jintArray jarray = env->NewIntArray(n);
   env->SetIntArrayRegion(jarray, 0, n, (const jint *)textures);
   env->CallVoidMethod(g_objCustomRenderer, g_glDeleteTextures, n, jarray);
   env->DeleteLocalRef(jarray);
}

void glDeleteVertexArrays(GLsizei n, GLuint *arrays)
{
   JNIEnv *env;
   g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);
   
   jintArray jarray = env->NewIntArray(n);
   env->SetIntArrayRegion(jarray, 0, n, (const jint *)arrays);
   env->CallVoidMethod(g_objCustomRenderer, g_glDeleteVertexArrays, n, jarray);
   env->DeleteLocalRef(jarray);
}

void glDepthFunc(GLenum cap)
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    env->CallVoidMethod(g_objCustomRenderer, g_glDepthFunc, cap);
}

void glDepthMask(GLboolean cap)
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    env->CallVoidMethod(g_objCustomRenderer, g_glDepthMask, cap);
}

void glDepthRange(GLfloat near, GLfloat far)
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    env->CallVoidMethod(g_objCustomRenderer, g_glDepthRange, near, far);
}

void glDisable(GLenum cap)
{
   JNIEnv *env;
   g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

   env->CallVoidMethod(g_objCustomRenderer, g_glDisable, cap);
}

void glDrawArrays(GLenum mode, GLint first, GLsizei count)
{
   JNIEnv *env;
   g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

   env->CallVoidMethod(g_objCustomRenderer, g_glDrawArrays, mode, first, count);
}

void glDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid * indices)
{
   JNIEnv *env;
   g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

   env->CallVoidMethod(g_objCustomRenderer, g_glDrawElements, mode, count, type, indices);
}

void glEnable(GLenum cap)
{
   JNIEnv *env;
   g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

   env->CallVoidMethod(g_objCustomRenderer, g_glEnable, cap);
}

void glEnableVertexAttribArray(GLuint index)
{
   JNIEnv *env;
   g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

   env->CallVoidMethod(g_objCustomRenderer, g_glEnableVertexAttribArray, index);
}

void glFlush()
{
   JNIEnv *env;
   g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

   env->CallVoidMethod(g_objCustomRenderer, g_glFlush);
}

void glFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    env->CallVoidMethod(g_objCustomRenderer, g_glFramebufferRenderbuffer, target, attachment, renderbuffertarget, renderbuffer);
}

void glFramebufferTexture2D(GLenum target, GLenum attachment, GLint texTarget, GLint texture, GLint level)
{
   JNIEnv *env;
   g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

   env->CallVoidMethod(g_objCustomRenderer, g_glFramebufferTexture2D, target, attachment, texTarget, texture, level);
}

void glFrontFace(GLenum mode)
{
   JNIEnv *env;
   g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

   env->CallVoidMethod(g_objCustomRenderer, g_glFrontFace, mode);
}

void glGenBuffers(GLsizei n, GLuint * buffers)
{
   JNIEnv *env;
   g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

   jintArray jarray = env->NewIntArray(n);
   env->SetIntArrayRegion(jarray, 0, n, (const jint *)buffers);
   env->CallVoidMethod(g_objCustomRenderer, g_glGenBuffers, n, jarray);
   env->GetIntArrayRegion(jarray, 0, n, (jint *)buffers);
   env->DeleteLocalRef(jarray);
}

void glGenFramebuffers(GLuint n, GLuint *buffers)
{
   JNIEnv *env;
   g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

   jintArray jarray = env->NewIntArray(n);
   env->SetIntArrayRegion(jarray, 0, n, (const jint *)buffers);
   env->CallVoidMethod(g_objCustomRenderer, g_glGenFramebuffers, n, jarray);
   env->GetIntArrayRegion(jarray, 0, n, (jint *)buffers);
   env->DeleteLocalRef(jarray);
}

void glGenRenderbuffers(GLuint n, GLuint *buffers)
{
   JNIEnv *env;
   g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

   jintArray jarray = env->NewIntArray(n);
   env->SetIntArrayRegion(jarray, 0, n, (const jint *)buffers);
   env->CallVoidMethod(g_objCustomRenderer, g_glGenRenderbuffers, n, jarray);
   env->GetIntArrayRegion(jarray, 0, n, (jint *)buffers);
   env->DeleteLocalRef(jarray);
}

void glGenVertexArrays(GLsizei n, GLuint *arrays)
{
   JNIEnv *env;
   g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

   jintArray jarray = env->NewIntArray(n);
   env->SetIntArrayRegion(jarray, 0, n, (const jint *)arrays);
   env->CallVoidMethod(g_objCustomRenderer, g_glGenVertexArrays, n, jarray);
   env->GetIntArrayRegion(jarray, 0, n, (jint *)arrays);
   env->DeleteLocalRef(jarray);
}

void glGenTextures(	GLsizei n,
 	GLuint * textures)
{
   JNIEnv *env;
   g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

   jintArray jarray = env->NewIntArray(n);
   env->SetIntArrayRegion(jarray, 0, n, (const jint *)textures);
   env->CallVoidMethod(g_objCustomRenderer, g_glGenTextures, n, jarray);
   env->GetIntArrayRegion(jarray, 0, n, (jint *)textures);
   env->DeleteLocalRef(jarray);
}

int glGetAttribLocation(GLuint program, const GLchar *name)
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    jstring jname = env->NewStringUTF(name);
    int loc = env->CallIntMethod(g_objCustomRenderer, g_glGetAttribLocation, program, jname);
    env->DeleteLocalRef(jname);
	
	return loc;
}

GLenum glGetError()
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);
	
	return env->CallIntMethod(g_objCustomRenderer, g_glGetError);
}

void glGetProgramInfoLog(GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog)
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);
	
	jboolean isCopy;

    jstring jstr = (jstring)env->CallObjectMethod(g_objCustomRenderer, g_glGetProgramInfoLog, program);
	const char *str = env->GetStringUTFChars(jstr, &isCopy);
	memcpy(infoLog, str, strlen(str));
	env->ReleaseStringUTFChars(jstr, str);
}

void glGetProgramiv(GLuint program, GLenum pname, GLint *params)
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

	jintArray jarray = env->NewIntArray(1);
    env->SetIntArrayRegion(jarray, 0, 1, params);
    env->CallVoidMethod(g_objCustomRenderer, g_glGetProgramiv, program, pname, jarray);
	env->GetIntArrayRegion(jarray, 0, 1, params);
    env->DeleteLocalRef(jarray);
}

void glGetShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog)
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);
	
	jboolean isCopy;

    jstring jstr = (jstring)env->CallObjectMethod(g_objCustomRenderer, g_glGetShaderInfoLog, shader);
	const char *str = env->GetStringUTFChars(jstr, &isCopy);
	memcpy(infoLog, str, strlen(str));
	env->ReleaseStringUTFChars(jstr, str);
}

void glGetShaderiv(GLuint shader, GLenum pname, GLint *params)
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

	jintArray jarray = env->NewIntArray(1);
    env->SetIntArrayRegion(jarray, 0, 1, params);
    env->CallVoidMethod(g_objCustomRenderer, g_glGetShaderiv, shader, pname, jarray);
	env->GetIntArrayRegion(jarray, 0, 1, params);
    env->DeleteLocalRef(jarray);
}

int glGetUniformLocation(GLuint program, const GLchar *name)
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    jstring jname = env->NewStringUTF(name);
    int result = env->CallIntMethod(g_objCustomRenderer, g_glGetUniformLocation, program, jname);
    env->DeleteLocalRef(jname);
	
	return result;
}

void glLinkProgram(GLuint program)
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    env->CallVoidMethod(g_objCustomRenderer, g_glLinkProgram, program);
}

void glRenderbufferStorage(GLenum target, GLenum internalFormat, GLsizei width, GLsizei height)
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    env->CallVoidMethod(g_objCustomRenderer, g_glRenderbufferStorage, target, internalFormat, width, height);
}

void glShaderSource(GLuint shader, GLsizei count, const GLchar **string, const GLint *length)
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    jstring jstring = env->NewStringUTF(string[0]);
    env->CallVoidMethod(g_objCustomRenderer, g_glShaderSource, shader, jstring);
    env->DeleteLocalRef(jstring);
}

void glTexImage2D(GLenum target,
 	GLint level,
 	GLint internalformat,
 	GLsizei width,
 	GLsizei height,
 	GLint border,
 	GLenum format,
 	GLenum type,
 	const GLvoid *data)
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    if (data == nullptr)
    {
        env->CallVoidMethod(g_objCustomRenderer, g_glTexImage2D, target, level, internalformat, width, height, border, format, type, nullptr);
    }
    else
    {
        jintArray jarray = env->NewIntArray(width * height);
        env->SetIntArrayRegion(jarray, 0, width * height, (const jint *)data);
        env->CallVoidMethod(g_objCustomRenderer, g_glTexImage2D, target, level, internalformat,
                            width, height, border, format, type, jarray);
        env->DeleteLocalRef(jarray);
    }
}

void glTexImage2Db(GLenum target,
 	GLint level,
 	GLint internalformat,
 	GLsizei width,
 	GLsizei height,
 	GLint border,
 	GLenum format,
 	GLenum type,
 	const GLvoid *data)
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    if (data == nullptr)
    {
        env->CallVoidMethod(g_objCustomRenderer, g_glTexImage2Db, target, level, internalformat, width, height, border, format, type, nullptr);
    }
    else
    {
        jbyteArray jarray = env->NewByteArray(width * height * 4);
        env->SetByteArrayRegion(jarray, 0, width * height * 4, (const jbyte *)data);
        env->CallVoidMethod(g_objCustomRenderer, g_glTexImage2Db, target, level, internalformat,
                            width, height, border, format, type, jarray);
        env->DeleteLocalRef(jarray);
    }
}

void glTexParameterf(GLenum target,
 	GLenum pname,
 	GLfloat param)
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    env->CallVoidMethod(g_objCustomRenderer, g_glTexParameterf, target, pname, param);
}

void glTexParameteri(GLenum target,
                     GLenum pname,
                     GLuint param)
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    env->CallVoidMethod(g_objCustomRenderer, g_glTexParameteri, target, pname, param);
}

void glUniform1f(GLint location, GLfloat v0)
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    env->CallVoidMethod(g_objCustomRenderer, g_glUniform1f, location, v0);
}

void glUniform1fv(	GLint location,
 	int count,
 	GLfloat *data)
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);
	
    jfloatArray jarray = env->NewFloatArray(count);
    env->SetFloatArrayRegion(jarray, 0, count, (const jfloat *)data);
    env->CallVoidMethod(g_objCustomRenderer, g_glUniform1fv, location, count, jarray);
    env->DeleteLocalRef(jarray);
}

void glUniform1i(GLint location, GLint v0)
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    env->CallVoidMethod(g_objCustomRenderer, g_glUniform1i, location, v0);
}

void glUniform2f(	GLint location,
 	GLfloat v0,
 	GLfloat v1)
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    env->CallVoidMethod(g_objCustomRenderer, g_glUniform2f, location, v0, v1);
}

void glUniform2fv(	GLint location,
 	int count,
 	GLfloat *data)
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);
	
    jfloatArray jarray = env->NewFloatArray(count * 2);
    env->SetFloatArrayRegion(jarray, 0, count * 2, (const jfloat *)data);
    env->CallVoidMethod(g_objCustomRenderer, g_glUniform2fv, location, count, jarray);
    env->DeleteLocalRef(jarray);
}

void glUniform4f(	GLint location,
 	GLfloat v0,
 	GLfloat v1,
 	GLfloat v2,
 	GLfloat v3)
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    env->CallVoidMethod(g_objCustomRenderer, g_glUniform4f, location, v0, v1, v2, v3);
}

void glUniform4fv(	GLint location,
 	int count,
 	GLfloat *data)
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);
	
    jfloatArray jarray = env->NewFloatArray(count * 4);
    env->SetFloatArrayRegion(jarray, 0, count * 4, (const jfloat *)data);
    env->CallVoidMethod(g_objCustomRenderer, g_glUniform4fv, location, count, jarray);
    env->DeleteLocalRef(jarray);
}

void glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
   JNIEnv *env;
   g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);
   
   jfloatArray jvalue = env->NewFloatArray(16 * count);
   env->SetFloatArrayRegion(jvalue, 0, 16 * count, value);
   env->CallVoidMethod(g_objCustomRenderer, g_glUniformMatrix4fv, location, count, transpose, jvalue);
   env->DeleteLocalRef(jvalue);
}

void glUseProgram(GLuint program)
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    env->CallVoidMethod(g_objCustomRenderer, g_glUseProgram, program);
}

void glVertexAttribPointer( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointe)
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    env->CallVoidMethod(g_objCustomRenderer, g_glVertexAttribPointer, index, size, type, normalized, stride, pointe);
}

void glViewport(GLuint x, GLuint y, GLuint width, GLuint height)
{
	JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    env->CallVoidMethod(g_objCustomRenderer, g_glViewport, x, y, width, height);
}

/*jmethodID g_open = 0;
jmethodID g_getWidth = 0;
jmethodID g_getHeight = 0;
jmethodID g_getPixels = 0;
jmethodID g_close = 0;
jmethodID g_assetOpen = 0;
jmethodID g_assetReadByte = 0;
jmethodID g_assetReadInt = 0;
jmethodID g_assetReadFloat = 0;
jmethodID g_getAssetManager = 0;
jmethodID g_loadSound = 0;
jmethodID g_playSound = 0;
jmethodID g_stopSound = 0;
jmethodID g_playTrack = 0;
jmethodID g_stopTrack = 0;
jmethodID g_log = 0;
jmethodID g_getSurfaceView = 0;
jmethodID g_getSurface = 0;
jobject g_screen;

bool g_attached = false;*/

/*
extern "C" JNIEXPORT void JNICALL Java_com_voxyc_voxyc_HelloJni_SetScreen(JNIEnv *jni, jclass clazz, jobject bitmap)
{
	g_screen = bitmap;
}

extern "C" JNIEXPORT void JNICALL Java_com_voxyc_voxyc_HelloJni_StartApp(JNIEnv *env)
{
	StartApp();
}

extern "C" JNIEXPORT void JNICALL Java_com_voxyc_voxyc_HelloJni_AppTick(JNIEnv *env)
{
	AppTick();
}

extern "C" JNIEXPORT void JNICALL Java_com_voxyc_voxyc_HelloJni_Draw(JNIEnv *env)
{
	Draw();
}

//extern "C" JNIEXPORT jint JNICALL Java_oculus_MainActivity_attachThread(JNIEnv *env, jobject thiz)
//{
//}

extern "C" JNIEXPORT jint JNICALL Java_oculus_MainActivity_registerEnv(JNIEnv *env, jobject thiz)
{
//    AttachThread();

//    g_env = env;
}

void PLAT_LoadBitmap(int** out, unsigned* w, unsigned* h, char *path)
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    jstring name = env->NewStringUTF(path);
    jobject png = env->CallObjectMethod(g_objMainActivity, g_open, name);
    env->DeleteLocalRef(name);
    jobject gpng = env->NewGlobalRef(png);

    // Get image dimensions
    int width = env->CallIntMethod(g_objMainActivity, g_getWidth, png);
    int height = env->CallIntMethod(g_objMainActivity, g_getHeight, png);

    // Get pixels
    jintArray array = env->NewIntArray(width * height);
    jintArray garray = env->NewGlobalRef(array);
    env->CallVoidMethod(g_objMainActivity, g_getPixels, png, garray);

    jint *pixels = env->GetIntArrayElements(garray, 0);

    char bytes[4];
    char bytesOut[4];

    for (int i = 0; i < width * height; i++)
    {
        int a = pixels[i];

        bytes[3] = a & 0xff;
        bytes[2] = (a >> 8) & 0xff;
        bytes[1] = (a >> 16) & 0xff;
        bytes[0] = (a >> 24) & 0xff;

        bytesOut[0] = bytes[0];
        bytesOut[1] = bytes[3];
        bytesOut[2] = bytes[2];
        bytesOut[3] = bytes[1];

        int b = int((unsigned char)(bytesOut[0]) << 24 |
        (unsigned char)(bytesOut[1]) << 16 |
        (unsigned char)(bytesOut[2]) << 8 |
        (unsigned char)(bytesOut[3]));

        pixels[i] = b;
    }

    *out = pixels;
    *w = width;
    *h = height;

    env->DeleteGlobalRef(garray);

    // Free image
    env->CallVoidMethod(g_objMainActivity, g_close, png);
    env->DeleteGlobalRef(gpng);
}

void PLAT_AssetOpen(char *filename)
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    jstring jfilename = env->NewStringUTF(filename);
    env->CallVoidMethod(g_objMainActivity, g_assetOpen, jfilename);
    env->DeleteLocalRef(jfilename);
}

char PLAT_AssetReadByte()
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    return env->CallCharMethod(g_objMainActivity, g_assetReadByte);
}

int PLAT_AssetReadInt()
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    return env->CallIntMethod(g_objMainActivity, g_assetReadInt);
}

int PLAT_AssetReadFloat()
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    return env->CallFloatMethod(g_objMainActivity, g_assetReadFloat);
}

jobject PLAT_GetAssetManager()
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    return env->CallObjectMethod(g_objMainActivity, g_getAssetManager);
}

JNIEnv *PLAT_GetEnv()
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    return env;
}

int PLAT_LoadSound(char *filename)
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    jstring jfilename = env->NewStringUTF(filename);
    int sound = env->CallIntMethod(g_objMainActivity, g_loadSound, jfilename);
    env->DeleteLocalRef(jfilename);

    return sound;
}

int PLAT_PlaySound(int idx)
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    return env->CallIntMethod(g_objMainActivity, g_playSound, idx);
}

void PLAT_StopSound(int stream)
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    env->CallVoidMethod(g_objMainActivity, g_stopSound, stream);
}

void PLAT_PlayTrack(char *filename)
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    jstring jfilename = env->NewStringUTF(filename);
    env->CallVoidMethod(g_objMainActivity, g_playTrack, jfilename);
    env->DeleteLocalRef(jfilename);
}

void PLAT_StopTrack()
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    env->CallVoidMethod(g_objMainActivity, g_stopTrack);
}

void PLAT_Log(char *str)
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    jstring jstr = env->NewStringUTF(str);
    env->CallVoidMethod(g_objMainActivity, g_log, jstr);
    env->DeleteLocalRef(jstr);
}

jobject PLAT_GetSurfaceView()
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    return env->CallObjectMethod(g_objMainActivity, g_getSurfaceView);
}

jobject PLAT_GetSurface()
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    return env->CallObjectMethod(g_objMainActivity, g_getSurface);
}

void PLAT_CopyPixelsToScreen(unsigned char *pixels, int width, int height)
{
}*/
