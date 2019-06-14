# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)

ifneq ($(TARGET_ARCH_ABI),x86)

include $(CLEAR_VARS)

LOCAL_CPP_EXTENSION := .cpp .cc
LOCAL_MODULE    := hello-jni
LOCAL_SRC_FILES := hello-jni.cpp\
             ../../../../thirdparty/lodepng/lodepng.cpp\
             ../../Voxyc/app/src/main/cpp/android.cpp\
             ../../Voxyc/app/src/main/cpp/javagl.cpp\
             ../../../../engine/DDLUtils.cpp\
             ../../../../engine/Globals.cpp\
             ../../../../engine/Audio.cpp\
             ../../../../engine/PlatformAudio.cpp\
             ../../../../engine/Camera.cpp\
             ../../../../engine/Controls2.cpp\
             ../../../../engine/Engine2.cpp\
             ../../../../engine/Mesh.cpp\
             ../../../../engine/Model2.cpp\
             ../../../../engine/ModelRenderer.cpp\
             ../../../../engine/SkeletalRenderer.cpp\
             ../../../../engine/Object.cpp\
             ../../../../engine/OzzRenderer.cpp\
             ../../../../engine/Renderer.cpp\
             ../../../../engine/ShadowMap.cpp\
             ../../../../engine/Shape.cpp\
             ../../../../engine/ShapeRenderer.cpp\
             ../../../../engine/SkyboxRenderer.cpp\
             ../../../../engine/Terrain.cpp\
             ../../../../engine/TextureManager2.cpp\
             ../../../../engine/SpriteRenderer.cpp\
             ../../../../engine/SpriteRenderer2D.cpp\
             ../../../../engine/TextPrinter.cpp\
             ../../../../engine/LuaBridge.cpp\
             ../../../../engine/Voxels.cpp\
             ../../../../engine/EditorController.cpp\
             ../../../../engine/GUI.cpp\
             ../../../../engine/OBJWriter.cpp\
             ../../../../engine/TextureAtlas.cpp\
             ../../../../engine/Batch.cpp\
             ../../../../engine/Batcher.cpp\
             ../../../../engine/SoftCanvas.cpp\
             ../../../../engine/FrameBuffer.cpp\
             ../../../../engine/FileIO.cpp\
             ../../../../engine/Scene.cpp\
             ../../../../engine/Physics.cpp\
             ../../../../engine/VertexLights.cpp\
             ../../../../app/main.cpp\
             ../../../../app/VoxycApp.cpp\
             ../../../../app/LuaProgram.cpp\
             ../../../../editor/Editor.cpp\
             ../../../../editor/EditorOld.cpp\
             ../../../../editor/OrthoEditor.cpp\
             ../../../../editor/ModelEditor.cpp\
             ../../../../thirdparty/lua/lapi.c\
             ../../../../thirdparty/lua/lauxlib.c\
             ../../../../thirdparty/lua/lbaselib.c\
             ../../../../thirdparty/lua/lbitlib.c\
             ../../../../thirdparty/lua/lcode.c\
             ../../../../thirdparty/lua/lcorolib.c\
             ../../../../thirdparty/lua/lctype.c\
             ../../../../thirdparty/lua/ldblib.c\
             ../../../../thirdparty/lua/ldebug.c\
             ../../../../thirdparty/lua/ldo.c\
             ../../../../thirdparty/lua/ldump.c\
             ../../../../thirdparty/lua/lfunc.c\
             ../../../../thirdparty/lua/lgc.c\
             ../../../../thirdparty/lua/linit.c\
             ../../../../thirdparty/lua/liolib.c\
             ../../../../thirdparty/lua/llex.c\
             ../../../../thirdparty/lua/lmathlib.c\
             ../../../../thirdparty/lua/lmem.c\
             ../../../../thirdparty/lua/loadlib.c\
             ../../../../thirdparty/lua/lobject.c\
             ../../../../thirdparty/lua/lopcodes.c\
             ../../../../thirdparty/lua/loslib.c\
             ../../../../thirdparty/lua/lparser.c\
             ../../../../thirdparty/lua/lstate.c\
             ../../../../thirdparty/lua/lstring.c\
             ../../../../thirdparty/lua/lstrlib.c\
             ../../../../thirdparty/lua/ltable.c\
             ../../../../thirdparty/lua/ltablib.c\
             ../../../../thirdparty/lua/ltm.c\
             ../../../../thirdparty/lua/lua.c\
             ../../../../thirdparty/lua/luac.c\
             ../../../../thirdparty/lua/lundump.c\
             ../../../../thirdparty/lua/lutf8lib.c\
             ../../../../thirdparty/lua/lvm.c\
             ../../../../thirdparty/lua/lzio.c

LOCAL_SHARED_LIBRARIES := 
LOCAL_CFLAGS := -fpermissive -std=c++11 -lm -DPLATFORM_ANDROID
LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog

include $(BUILD_SHARED_LIBRARY)

#include $(CLEAR_VARS)
#LOCAL_MODULE := libassimp
#LOCAL_SRC_FILES := libassimp.so
#include $(PREBUILT_SHARED_LIBRARY)

endif
