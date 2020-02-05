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

#ifndef Globals_hpp
#define Globals_hpp

#include <stdio.h>
#include <string>
#include "Common.h"
 
#define DEFAULT_ASSETS_DIR "../../../../assets"
#define DEFAULT_MODULE "editor2"
#define CONFIG_FILENAME "config.ini"

//#define USE_EXTERNAL_ASSETS
#define EXTERNAL_PROJECT_DIR "assets"
//#define EXTERNAL_PROJECT_DIR "voxeleditor"
//#define EXTERNAL_PROJECT_DIR "luaprogram"

#define DO_ENGINE2

#define PRODUCT_EARTH2084

#define DEBUG_BUILD

//#define USE_API_CLIENT

//#define PREVIEW_SHADOWMAP

#define FIXED_TIMESTEP

#define DO_VERTEX_LIGHTS
#define DO_FAST_SPRITES

#define USE_OPENGL
#define USE_OPENGL_3
#define USE_GLFW

//#define PLATFORM_OPENVR
//#define PLATFORM_WINDOWS
//#define PLATFORM_OSX
//#define PLATFORM_ANDROID
//#define PLATFORM_GVR

#define NORMALIZE_MESHES
#define NORMALIZE_MODELS

#define USE_SEGMENTS
#define NUM_SEGMENTS_XY    1
#define TOTAL_SEGMENTS     NUM_SEGMENTS_XY * NUM_SEGMENTS_XY
#define SEGMENT_DRAW_THRESHOLD  8

#define USE_JAVAGL
//#define USE_OZZ

#if defined PLATFORM_WINDOWS || defined PLATFORM_OPENVR || defined PLATFORM_OSX
#define USE_OPENAL
#endif

//#define USE_ASSIMP
//#ifdef PLATFORM_WINDOWS
//#define USE_TINYOBJLOADER
//#endif
//#define USE_OOBJLOADER

//#define HIDE_OVERLAY

//#define WDL_OL_PLUGIN

//#define FADE_SPEED 0.01
#define FADE_SPEED 0.05

#ifdef PR3ODUCT_DRUMSTR
#ifdef TARGET_APP
#define GLOBAL_BUNDLE_ID "com.Drumstr.standalone.Drumstr"
#endif
#ifdef TARGET_AU
#define GLOBAL_BUNDLE_ID "com.Drumstr.audiounit.Drumstr"
#endif
#endif

#ifdef PRODUCT_BUILD3D
#define GLOBAL_BUNDLE_ID "com.Drumstr.standalone.Drumstr"
#endif

#define VERSION_STRING "1.0.4"
#define APP_RESOURCES_DIR "/Applications/Drumstr.app/Contents/Resources/"
#define APP_PREFERENCES_DIR "/"
//#define APP_PREFERENCES_DIR "/Applications/Drumstr.app/"
//#define APP_PREFERENCES_DIR "~/Library/Preferences/"
//#define CONFIG_FILENAME "drumstr.ini"
//#define APP_RESOURCES_DIR "Drumstr.app/Contents/Resources/"
#define AU_DEBUG_SAMPLES_DIR "/Users/dimitri/Music/Drumstr/"
#define KIT_SCAN_ROOT "/Users/"
#ifdef PLATFORM_OSX
#define ASSETS_FILEMASK "/users/dimitri/FatelessOSX/assets/%s"
#endif
#ifdef PLATFORM_WINDOWS
#define ASSETS_FILEMASK "C:/Users/dimit/VisualStudioProjects/Voxyc/Voxyc/assets/%s"
#define EXTERNAL_RESOURCE_FILEMASK "C:/Users/dimit/%s/%s"
#define WINDOWS_FILES_DIR "C:/Users/dimit/VisualStudioProjects/Voxyc"
#endif
#ifdef PLATFORM_OPENVR
#define ASSETS_FILEMASK "C:/openvr-1.0.7/samples/hellovr_opengl/assets/%s"
#define WINDOWS_FILES_DIR "C:/Users/Dimitri/Documents/Visual Studio 2015/Projects/Bratva/files"
#endif

#define MAX_STR_LEN             512
#define MAX_TEXTURES            128

#ifdef BOAT_TEST
#define ISLAND_TILE_ID          1
#else
#define ISLAND_TILE_ID          1
#endif

#if defined PLATFORM_OSX || defined PLATFORM_WINDOWS || defined PLATFORM_OPENVR
#define MAX_ENTITIES 512
#define MAX_TILEMAP_SIZE 128
#define MAX_ACTOR_PROJECTILES 64
#endif
#ifdef PLATFORM_ANDROID
#define MAX_ENTITIES 512
#define MAX_TILEMAP_SIZE 64
#define MAX_ACTOR_PROJECTILES 64
#endif
#ifdef PLATFORM_GVR
#define MAX_ENTITIES 512
#define MAX_TILEMAP_SIZE 128
#define MAX_ACTOR_PROJECTILES 64
#endif

#ifdef PLATFORM_ANDROID
#define GLINT int
#endif

#if defined PLATFORM_OSX || defined PLATFORM_WINDOWS
#define GLINT GLuint
#endif

#define DO_LIGHTING
#define INCREMENTAL_ALLOC
#define MORPH_TARGET_ANIMATION

#define PLAYER_SPAWN_POINT_SPRITE 20
#define EXIT_POINT_SPRITE 21

#if defined PLATFORM_OSX || defined PLATFORM_WINDOWS
#define VIEW_FOV    120.0f
#endif
#if defined PLATFORM_GVR || defined PLATFORM_OPENVR
#define VIEW_FOV    45.0f
#endif
#if defined PLATFORM_ANDROID || defined PLATFORM_IOS
#define VIEW_FOV    120.0f
#endif

#define DISTANCE_BETWEEN_EYES 6.0f
//#define MOTION_SICKNESS_GUARD
#define LIGHT_SIZE    16
#define DOOR_OPEN_DISTANCE    128
#define DEFAULT_NUM_ENEMIES_PER_LEVEL 40

#define OPENVR_ROOM_SCALE		38.0
#define OPENVR_ROOM_TRANSLATE_Y	1.5

#ifdef PLATFORM_OPENVR
#include "..\shared\Matrices.h"
#endif

extern bool g_restartGame;
extern bool g_reloadVertices;
extern bool g_partialReload;
extern int g_partialReloadEntityIdx;
extern int g_partialReloadTileIdx;
extern int g_ce;
extern int g_nc;
extern bool g_loadWorld;
extern bool g_loadWorldFromAsset;
extern char g_loadWorldFilename[MAX_STR_LEN];
extern bool g_saveWorld;
extern char g_saveWorldFilename[MAX_STR_LEN];
extern bool g_setTextureSpan;
extern int g_newTextureSpan;
extern float g_cameraZOffset;
extern bool g_restartLevel;
extern bool g_restartingLevel;
extern bool g_exitLevel;
extern bool g_exitingLevel;
extern bool g_reloadSkybox;
extern bool g_fadingIn; extern bool g_fadingInDone;
extern bool g_fadingOut;
extern bool g_fadingOutDone;
extern float g_fadeMultiplier;
extern float g_redout;
extern int g_playerLeftWeaponEntity;
extern int g_playerRightWeaponEntity;
extern int g_playerWeaponFireEntity;
extern int g_playerVehclieEntity;
extern int g_fps;
extern bool g_largeScale;
extern bool g_playerInVehicle;
extern int g_numEnemiesPerLevel;

#ifdef PLATFORM_GVR
extern float g_headYaw;
extern float g_headPitch;
extern float g_headRoll;
#endif

extern long long g_lastFinishDrawTime;
extern long long g_lastFinishTickTime;

extern int g_keyTimer;
extern int g_lastKey;

#ifdef PLATFORM_OPENVR
#include <openvr.h>
extern Matrix4 g_projectionMatrix;
extern Matrix4 g_poseEyeProjectionMatrix;
extern Matrix4 g_poseEyeMatrix;
extern Matrix4 g_poseMatrix;
extern vr::IVRSystem *g_ivrSystem;
//#define VRSCALE 0.025
#define VRSCALE 3.0
#define VR_FAR_CLIP 3000.0
#endif
#ifdef PLATFORM_GVR
#endif

#define SKYBOX_SIZE 100.0

#define USE_PLAYEROBJ

extern std::string g_assetsDir;
extern bool g_assetsDirExplicit;
extern std::string g_module;
extern std::string g_dataFile;
extern bool g_useDataFile;

extern bool g_useLegacyTextureSpan;
extern bool g_simpleMode;

extern Common g_common;

extern bool g_noGuides;

#endif /* Globals_hpp */
