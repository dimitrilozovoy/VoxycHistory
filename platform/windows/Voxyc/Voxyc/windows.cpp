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

#include "../../../../engine/Globals.hpp"
#include "../../../../engine/Audio.h"
//#include "main.h"

#if defined PLATFORM_WINDOWS || defined PLATFORM_OPENVR
#include <string>

int PLAT_LoadSound(char *filename, bool stereo)
{
	return g_audio.loadSound(filename, stereo);
}

int PLAT_PlaySound(int idx)
{
	return g_audio.playSound(idx);
}

void PLAT_StopSound(int stream)
{
	return g_audio.stopSound(stream);
}

void PLAT_PlayTrack(char *filename, bool stereo)
{
	return g_audio.playTrack(filename, stereo);
}

int PLAT_GetWindowWidth()
{
	return g_windowWidth;
}

int PLAT_GetWindowHeight()
{
	return g_windowHeight;
}

void PLAT_Log(char *str)
{
	std::string s1(str);
	std::string s2 = "\n";
	std::string s3 = s1 + s2;
	printf(s3.c_str());
}

int PLAT_stoi(std::string s, int fallback)
{
	return std::stoi(s, nullptr, 10);
}

float PLAT_stof(std::string s, float fallback)
{
	return std::stof(s, nullptr);
}

void PLAT_ShowFileSelector(std::string ext)
{
}

void PLAT_ClearListMenu()
{
}

void PLAT_AddListMenuOption(std::string title, std::string desc)
{
}

void PLAT_ShowListMenuInDialog(std::string title, std::string options)
{
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

#endif