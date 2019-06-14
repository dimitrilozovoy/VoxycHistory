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

#ifndef Utils_hpp
#define Utils_hpp

#include "Globals.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#if defined(TARGET_AU) || defined(TARGET_APP)
#include <CoreFoundation/CoreFoundation.h>
#endif
#include "../thirdparty/glm/glm.hpp"
#ifdef PLATFORM_GVR
#include "OvrApp.h"
#include "GuiSys.h"
#include "OVR_Locale.h"
#endif
#if not defined PLATFORM_WINDOWS && not defined PLATFORM_OPENVR
#include <sys/time.h>
#else
#include <time.h>
#endif

#if defined(TARGET_AU) || defined(TARGET_APP)
char *MYCFStringCopyUTF8String(CFStringRef aString);
#endif

void printUserFilesDir(char *dest);
void printFullResourceFilename(char *shortFilename, char *fullFilename);
void printFullExternalResourceFilename(char *shortFilename, char *fullFilename);
std::string GetFullFilename(std::string fname);
int FindFile(char *filename, char *folder, char *foundInFolder);
void Log_Write(char *str);
void Log(std::string str);
void Log(char *str);
void Log(char *str, char *str2);
void Log(char *str, int a);
void Logx(char *str, int a);
void Log(char *str, int a, char *str2, int b);
void Log(char *str, int a, char *str2, int b, char *str3, int c);
void Log(char *str, char *str2, char *str3, int b);
void Log(char *str, int a, char *str2, char *str3);
void Log(char *str, glm::mat4 mat);
void Log(char *str, glm::vec4 mat);
#ifdef PLATFORM_GVR
void Log(char *str, OVR::Matrix4f mat);
#endif
void Logf(char *str, float f);
void LogvReset();
void LogHex(char *str, int f);
void LogFromConsole(char *str);
void Logv(char *str, int a);
void Logv(char *stra, int a, char *strb, int b, char *strc, int c);
void Logv(char *str);
int RandomInt(int min, int max);
float RandomFloat(float min, float max);
float ToRadians(float degrees);
float ToDegrees(float radians);
bool compareYaw(float yaw1, float yaw2, float threshold);
bool compareAngle(float angle1, float angle2, float threshold);
float absf(float in);
float GradualAssign(float from, float to, float factor);
long long getTimeInMsec();
void ddlsleep(int msec);
void PrintBits(char input, char output[9]);
std::string ToString(int value);
std::string GetFileName(const std::string& s);
std::string GetPath(const std::string& s);
std::string GetExtension(const std::string s);
std::string FloatToStr(float f);
float scrToGlX(float screenX);
float scrToGlY(float screenY);
float glToScrX(float glX);
float glToScrY(float glY);
float RotateAngleTowards(float angle, float targetAngle, float step);
float RotateAngleAwayFrom(float angle, float targetAngle, float step);
float Limit360(float value);
void CalcNormal(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float &nx, float &ny, float &nz);
float UCharToFloat(unsigned char c);
unsigned char FloatToUChar(float c);
float UCharToFloat255(unsigned char c);
unsigned char FloatToUChar255(float c);

#endif /* Utils_hpp */
