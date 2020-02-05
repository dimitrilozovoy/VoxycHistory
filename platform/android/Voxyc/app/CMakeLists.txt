# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
             native-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             src/main/cpp/native-lib.cpp )

add_library( # Sets the name of the library.
             hello-jni

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             # Associated headers in the same location as their source
             # file are automatically included.
             ../../../../thirdparty/lodepng/lodepng.cpp
             src/main/cpp/android.cpp
             src/main/cpp/javagl.cpp
             ../../../../engine/DDLUtils.cpp
             ../../../../engine/Globals.cpp
             ../../../../engine/Audio.cpp
             ../../../../engine/Camera.cpp
             ../../../../engine/Controls2.cpp
             ../../../../engine/Engine2.cpp
             ../../../../engine/Mesh.cpp
             ../../../../engine/Model2.cpp
             ../../../../engine/ModelRenderer.cpp
             ../../../../engine/SkeletalRenderer.cpp
             ../../../../engine/Object.cpp
             ../../../../engine/OzzRenderer.cpp
             ../../../../engine/PlatformAudio.cpp
             ../../../../engine/Renderer.cpp
             ../../../../engine/ShadowMap.cpp
             ../../../../engine/Shape.cpp
             ../../../../engine/ShapeRenderer.cpp
             ../../../../engine/SkyboxRenderer.cpp
             ../../../../engine/Terrain.cpp
             ../../../../engine/TextureManager2.cpp
             ../../../../engine/SpriteRenderer.cpp
             ../../../../engine/SpriteRenderer2D.cpp
             ../../../../engine/TextPrinter.cpp
             ../../../../engine/LuaBridge.cpp
             ../../../../engine/Voxels.cpp
             ../../../../engine/EditorController.cpp
             ../../../../engine/GUI.cpp
             ../../../../engine/OBJWriter.cpp
             ../../../../engine/TextureAtlas.cpp
             ../../../../engine/Batch.cpp
             ../../../../engine/Batcher.cpp
             ../../../../engine/SoftCanvas.cpp
             ../../../../engine/FrameBuffer.cpp
             ../../../../engine/FileIO.cpp
             ../../../../engine/Scene.cpp
             ../../../../engine/Physics.cpp
             ../../../../engine/VertexLights.cpp
             ../../../../engine/ReadData.cpp
             ../../../../app/main.cpp
             ../../../../app/VoxycApp.cpp
             ../../../../app/LuaProgram.cpp
             ../../../../editor/Editor.cpp
             ../../../../editor/Editor2.cpp
             ../../../../editor/ModelEditor.cpp
             ../../../../editor/OrthoEditor.cpp
             ../../../../editor/EditorOld.cpp
             ../../../../thirdparty/lua/lapi.c
             ../../../../thirdparty/lua/lauxlib.c
             ../../../../thirdparty/lua/lbaselib.c
             ../../../../thirdparty/lua/lbitlib.c
             ../../../../thirdparty/lua/lcode.c
             ../../../../thirdparty/lua/lcorolib.c
             ../../../../thirdparty/lua/lctype.c
             ../../../../thirdparty/lua/ldblib.c
             ../../../../thirdparty/lua/ldebug.c
             ../../../../thirdparty/lua/ldo.c
             ../../../../thirdparty/lua/ldump.c
             ../../../../thirdparty/lua/lfunc.c
             ../../../../thirdparty/lua/lgc.c
             ../../../../thirdparty/lua/linit.c
             ../../../../thirdparty/lua/liolib.c
             ../../../../thirdparty/lua/llex.c
             ../../../../thirdparty/lua/lmathlib.c
             ../../../../thirdparty/lua/lmem.c
             ../../../../thirdparty/lua/loadlib.c
             ../../../../thirdparty/lua/lobject.c
             ../../../../thirdparty/lua/lopcodes.c
             ../../../../thirdparty/lua/loslib.c
             ../../../../thirdparty/lua/lparser.c
             ../../../../thirdparty/lua/lstate.c
             ../../../../thirdparty/lua/lstring.c
             ../../../../thirdparty/lua/lstrlib.c
             ../../../../thirdparty/lua/ltable.c
             ../../../../thirdparty/lua/ltablib.c
             ../../../../thirdparty/lua/ltm.c
             ../../../../thirdparty/lua/lua.c
             ../../../../thirdparty/lua/luac.c
             ../../../../thirdparty/lua/lundump.c
             ../../../../thirdparty/lua/lutf8lib.c
             ../../../../thirdparty/lua/lvm.c
             ../../../../thirdparty/lua/lzio.c
             )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# ASSIMP

#add_library( assimp
#             SHARED
#             IMPORTED )

#set_target_properties( # Specifies the target library.
#                       assimp

                       # Specifies the parameter you want to define.
#                       PROPERTIES IMPORTED_LOCATION

                       # Provides the path to the library you want to import.
#                       ${PROJECT_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/libassimp.so )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       hello-jni

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib}
#                       assimp
                       )