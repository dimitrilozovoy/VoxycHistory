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

#include "Globals.hpp"

bool g_restartGame = false;
bool g_reloadVertices = false;
bool g_partialReload = false;
int g_partialReloadEntityIdx = -1;
int g_partialReloadTileIdx = -1;
int g_ce;
int g_nc;
bool g_loadWorld = false;
bool g_loadWorldFromAsset = true;
char g_loadWorldFilename[MAX_STR_LEN];
bool g_saveWorld = false;
char g_saveWorldFilename[MAX_STR_LEN];
bool g_setTextureSpan = false;
int g_newTextureSpan = 0;
float g_cameraZOffset = 0;
bool g_restartLevel = false;
bool g_restartingLevel = false;
bool g_exitingLevel = false;
bool g_exitLevel = false;
bool g_reloadSkybox = false;
bool g_fadingIn = false;
bool g_fadingInDone = false;
bool g_fadingOut = false;
bool g_fadingOutDone = false;
float g_fadeMultiplier = 1.0f;
float g_redout = 0.0f;
int g_playerLeftWeaponEntity = 0;
int g_playerRightWeaponEntity = 0;
int g_playerWeaponFireEntity = 0;
int g_playerVehclieEntity = 0;
int g_fps = 0;
#if defined BOAT_TEST || defined TRUCK_TEST || defined PLANE_TEST || defined TRAIN_TEST
bool g_largeScale = true;
bool g_playerInVehicle = true;
#else
bool g_largeScale = false;
bool g_playerInVehicle = false;
#endif

#ifdef PLATFORM_GVR
float g_headYaw = 0;
float g_headPitch = 0;
float g_headRoll = 0;
#endif

long long g_lastFinishDrawTime = -1;
long long g_lastFinishTickTime = -1;

//const int g_windowWidth = 1024;
//const int g_windowHeight = 768;

//const int g_windowWidth = 1480;
//const int g_windowHeight = 768 ;

int g_numEnemiesPerLevel = DEFAULT_NUM_ENEMIES_PER_LEVEL;

int g_keyTimer = 0;
int g_lastKey = 0;

#ifdef PLATFORM_OPENVR
Matrix4 g_projectionMatrix;
Matrix4 g_poseEyeProjectionMatrix;
Matrix4 g_poseEyeMatrix;
Matrix4 g_poseMatrix;
vr::IVRSystem *g_ivrSystem = nullptr;
#endif
#ifdef PLATFORM_GVR
OVR::Matrix4f g_eyeMatrix;
OVR::Matrix4f g_projectionMatrix;
float g_stereoMatrixArr[16];
#endif

std::string g_assetsDir = "";
//extern bool g_assetsDirExplicit = false;
std::string g_module = DEFAULT_MODULE;
std::string g_dataFile = "";
bool g_useDataFile = false;

bool g_useLegacyTextureSpan = false;

bool g_simpleMode = false;

Common g_common;
RenderStats g_renderStats;

bool g_noGuides = false;
