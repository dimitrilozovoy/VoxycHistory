//
//  osx.cpp
//  Voxyc
//
//  Created by Dimitri Lozovoy on 11/26/18.
//
//

#include <stdio.h>
#include <string>
#import <UIKit/UIKit.h>
#include "IOSGUI.h"
#include "IOSAudio.h"
#include "DDLUtils.hpp"

void PLAT_LoadBitmap(int** out, unsigned* w, unsigned* h, char *path)
{
}

void PLAT_AssetOpen(char *filename)
{
}

char PLAT_AssetReadByte()
{
    return 0;
}

int PLAT_AssetReadInt()
{
    return 0;
}

int PLAT_AssetReadFloat()
{
    return 0;
}

int PLAT_GetBytesRead()
{
    return 0;
}

int PLAT_LoadSound(char *filename, bool stereo, std::string assetsDir)
{
    int idx = [g_iosAudio loadSound: filename];

    return idx;
}

int PLAT_PlaySound(int idx)
{
    return [g_iosAudio playSound: idx];
}

void PLAT_StopSound(int stream)
{
}

void PLAT_PlayTrack(char *filename, bool stereo, std::string assetsDir)
{
    static int idx = -1;
    [g_iosAudio stopSound: idx];
    idx = [g_iosAudio loadSound: filename];
    [g_iosAudio playSound: idx];
}

void PLAT_StopTrack()
{
}

void PLAT_Log(char *str)
{
}

void PLAT_CopyPixelsToScreen(unsigned char *pixels, int width, int height)
{
}

void PLAT_LoadTextureInJava(int glTexID, std::string filename, std::string assetsdir)
{
}

int PLAT_GetWindowWidth()
{
    return [UIScreen mainScreen].bounds.size.width;
}

int PLAT_GetWindowHeight()
{
    return [UIScreen mainScreen].bounds.size.height;
}

long PLAT_GetTime()
{
    return 0;
}

int PLAT_stoi(std::string s, int fallback)
{
    return std::stoi(s);
}

float PLAT_stof(std::string s, float fallback)
{
    if (s == "")
        return fallback;
    
    try
    {
        return std::stof(s, nullptr);
    }
    catch (int e)
    {
        Log("stof error " + s);
        return 0.0f;
    }
}

void PLAT_oobjloaderLoad(char *filename)
{
}

int PLAT_oobjloaderGetNumVertices()
{
    return 0;
}

float PLAT_oobjloaderGetVertexX(int index)
{
    return 0.0f;
}

float PLAT_oobjloaderGetVertexY(int index)
{
    return 0.0f;
}

float PLAT_oobjloaderGetVertexZ(int index)
{
    return 0.0f;
}

int PLAT_oobjloaderGetNumTexCoords()
{
    return 0;
}


float PLAT_oobjloaderGetTexCoordX(int index)
{
    return 0.0f;
}

float PLAT_oobjloaderGetTexCoordY(int index)
{
    return 0.0f;
}

int PLAT_oobjloaderGetNumNormals()
{
    return 0;
}


float PLAT_oobjloaderGetNormalX(int index)
{
    return 0.0f;
}

float PLAT_oobjloaderGetNormalY(int index)
{
    return 0.0f;
}

float PLAT_oobjloaderGetNormalZ(int index)
{
    return 0.0f;
}

int PLAT_oobjloaderGetNumFaces()
{
    return 0;
}


int PLAT_oobjloaderGetFaceElement(int face, int element)
{
    return 0;
}

void PLAT_ShowFileSelector(std::string ext, std::string dir)
{
}

void PLAT_ClearListMenu()
{
    [g_iosGUI clearListMenu];
}

void PLAT_AddListMenuOption(std::string title, std::string desc)
{
    [g_iosGUI addListMenuOption: title withDesc: desc];
}

void PLAT_ShowListMenuInDialog(std::string title, std::string options)
{
    NSString *nsTitle = [NSString stringWithFormat:@"%s", title.c_str()];
    NSString *nsOptions = [NSString stringWithFormat:@"%s", options.c_str()];

    [g_iosGUI showListMenuInDialog:nsTitle withOptions:nsOptions];
}

void PLAT_ShowText(std::string text)
{
}

void PLAT_ShowLongText(std::string text)
{
}

void PLAT_ClearDialog()
{
}

void PLAT_AddDialogPart(std::string s1, std::string s2, std::string s3)
{
}

void PLAT_ShowDialog(std::string s1, std::string s2, std::string s3, std::string s4)
{
}

std::string PLAT_GetCameraPic()
{
    return "";
}

void PLAT_SavePref(std::string s1, std::string s2, std::string s3)
{
    NSString *nsKey = [NSString stringWithFormat:@"%s", s2.c_str()];
    NSString *nsValue = [NSString stringWithFormat:@"%s", s3.c_str()];
    
    if (nsKey == nil || nsValue == nil)
        return;
    
    NSUserDefaults *userDefaults = [NSUserDefaults standardUserDefaults];
    
    [userDefaults setObject:nsValue
                     forKey:nsKey];
    [userDefaults synchronize];
}

std::string PLAT_LoadPref(std::string s1, std::string s2, std::string s3)
{
    NSString *nsKey = [NSString stringWithFormat:@"%s", s2.c_str()];
    NSString *nsVal = [[NSUserDefaults standardUserDefaults] objectForKey:nsKey];
    
    if (nsVal != nil)
    {
        std::string stdVal = std::string([nsVal UTF8String]);
        return stdVal;
    }
    else
    {
        return s3;
    }
}

void PLAT_StartTrackLocation()
{
    
}

void PLAT_StopTrackLocation()
{
    
}

void PLAT_SetTrackVolume(float vol)
{
    
}

void PLAT_AttachCurrentThread()
{
    
}

void PLAT_DetachCurrentThread()
{
    
}
