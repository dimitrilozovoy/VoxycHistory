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

#ifndef PLATFORM_H
#define PLATFORM_H

#include <string>

#ifdef PLATFORM_OSX
typedef unsigned int GLuint;
#endif

void PLAT_AttachCurrentThread();
void PLAT_DetachCurrentThread();
void PLAT_Log(char *str);
int PLAT_GetScreenWidth();
int PLAT_GetScreenHeight();
void PLAT_LoadVertices();
void PLAT_LoadTexture(int tileId, char *filename);

void AttachThread();
void PLAT_LoadBitmap(int** out, unsigned* w, unsigned* h, char *filename);
void PLAT_AssetOpen(char *filename);
char PLAT_AssetReadByte();
int PLAT_AssetReadInt();
int PLAT_AssetReadFloat();
int PLAT_GetBytesRead();

int PLAT_LoadSound(char *filename, bool stereo = false, std::string assetsDir = "");
int PLAT_PlaySound(int idx);
void PLAT_StopSound(int stream);
void PLAT_PlayTrack(char *filename, bool stereo, std::string assetsDir);
void PLAT_StopTrack();
void PLAT_SetTrackVolume(float vol);

//void PLAT_CopyPixelsToScreen(unsigned char *pixels, int width, int height);

void PLAT_LoadTextureInJava(int glTexID, std::string filename, std::string assetsDir);

int PLAT_GetWindowWidth();
int PLAT_GetWindowHeight();

long PLAT_GetTime();
int PLAT_stoi(std::string s, int fallback = 0);
float PLAT_stof(std::string s, float fallback = 0.0f);

void PLAT_ShowFileSelector(std::string ext, std::string dir = "");
void PLAT_ClearListMenu();
void PLAT_AddListMenuOption(std::string title, std::string desc);
void PLAT_ShowListMenuInDialog(std::string title, std::string options);
void PLAT_ShowText(std::string text);
void PLAT_ShowLongText(std::string text);

void PLAT_ClearDialog();
void PLAT_AddDialogPart(std::string s1, std::string s2, std::string s3);
void PLAT_ShowDialog(std::string s1, std::string s2, std::string s3, std::string s4);

std::string PLAT_GetCameraPic();

void PLAT_SavePref(std::string section, std::string key, std::string value);
std::string PLAT_LoadPref(std::string section, std::string key, std::string def = "");

void PLAT_StartTrackLocation();
void PLAT_StopTrackLocation();

#endif
