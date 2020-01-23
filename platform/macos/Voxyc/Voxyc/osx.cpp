//
//  osx.cpp
//  Fateless
//
//  Created by Dimitri Lozovoy on 6/12/16.
//  Copyright © 2016 Lozoware. All rights reserved.
//

#include "Globals.hpp"
#include <sys/time.h>
#include "../../../../thirdparty/SimpleIni/SimpleIni.h"
#include "../../../../engine/Audio.h"

#ifdef PLATFORM_OSX

int PLAT_LoadSound(char *filename)
{
    return 0;
}

int PLAT_PlaySound(int idx)
{
    return 0;
}

void PLAT_StopSound(int stream)
{
    return;
}

void PLAT_PlayTrack(char *filename)
{
    return;
}

int PLAT_GetWindowWidth()
{
    return g_common.windowWidth;
}

int PLAT_GetWindowHeight()
{
    return g_common.windowHeight;
}

void PLAT_Log(char *str)
{
    std::string s1(str);
    std::string s2 = "\n";
    std::string s3 = s1 + s2;
    printf(s3.c_str());
}

long PLAT_GetTime()
{
    timeval time;
    gettimeofday(&time, NULL);
    long millis = (time.tv_sec * 1000) + (time.tv_usec / 1000);
    
    return millis;
}

std::string PLAT_LoadPref(std::string section, std::string key, std::string def)
{
    CSimpleIniA ini;
    ini.SetUnicode();
    ini.LoadFile(CONFIG_FILENAME);
    const char *pVal = ini.GetValue(section.c_str(), key.c_str(), def.c_str());

    return pVal;
}

void PLAT_SavePref(std::string section, std::string key, std::string val)
{
    CSimpleIniA ini;
    ini.SetUnicode();
    ini.LoadFile(CONFIG_FILENAME);
    ini.SetValue(section.c_str(), key.c_str(), val.c_str());
    ini.SaveFile(CONFIG_FILENAME);
}

void PLAT_ShowText(std::string str)
{
}

int PLAT_LoadSound(char *filename, bool stereo = false, std::string assetsDir = "")
{
    return 0;
}

void PLAT_PlayTrack(char *filename, bool stereo, std::string assetsDir)
{
    
}

void PLAT_StopTrack()
{
    
}

void PLAT_SetTrackVolume(float vol)
{
    g_audio.setTrackVolume(vol);
}

void PLAT_ShowDialog(std::string s1, std::string s2, std::string s3, std::string s4)
{
    
}

void PLAT_ClearDialog()
{
    
}

void PLAT_ShowLongText(std::string text)
{
    
}

void PLAT_AddDialogPart(std::string caption, std::string defaultValue, std::string extraKey)
{
    
}

void PLAT_ClearListMenu()
{
    
}

void PLAT_ShowFileSelector(std::string s1, std::string s2)
{
    
}

void PLAT_AddListMenuOption(std::string title, std::string desc)
{
    
}

void PLAT_AttachCurrentThread()
{
    
}

void PLAT_DetachCurrentThread()
{
    
}

void PLAT_ShowListMenuInDialog(std::string title, std::string options)
{
    
}

int PLAT_stoi(std::string s, int fallback)
{
    int result = 0;
    
    try
    {
        result = std::stoi(s, nullptr, 10);
    }
    catch (...)
    {
        result = fallback;
    }
    
    return result;
}

float PLAT_stof(std::string s, float fallback)
{
    if (s == "")
        return fallback;
    
    return std::stof(s, nullptr);
}

std::string PLAT_GetCameraPic()
{
    return "";
}

void PLAT_StartTrackLocation()
{
    
}

void PLAT_StopTrackLocation()
{
    
}

#endif
