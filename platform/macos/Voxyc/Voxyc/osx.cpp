//
//  osx.cpp
//  Fateless
//
//  Created by Dimitri Lozovoy on 6/12/16.
//  Copyright © 2016 Lozoware. All rights reserved.
//

#include "Globals.hpp"

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
    return 1024;
}

int PLAT_GetWindowHeight()
{
    return 768;
}

void PLAT_Log(char *)
{
    
}

long PLAT_GetTime()
{
    return 0;
}

std::string PLAT_LoadPref(std::string section, std::string key, std::string def)
{
    return "";
}

void PLAT_SavePref(std::string section, std::string key, std::string def)
{
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
    
}

void PLAT_ShowDialg(std::string s1, std::string s2, std::string s3, std::string s4)
{
    
}

void PLAT_ClearDialog()
{
    
}

#endif
