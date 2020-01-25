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
#include "DDLUtils.hpp"
#include <sstream>
#if defined PLATFORM_OSX || defined PLATFORM_WINDOWS
//#include "simpleini.h"
#endif
#ifdef PLATFORM_OSX
#include <dirent.h>
#endif

#include "../thirdparty/glm/glm.hpp"

#if defined PLATFORM_IOS
#import <Foundation/Foundation.h>
#endif

#ifdef PLATFORM_OSX
#include <CoreFoundation/CoreFoundation.h>
#include <CoreServices/CoreServices.h>
#include <iostream>
#include "../thirdparty/SimpleIni/SimpleIni.h"
#endif

#ifdef PLATFORM_GVR
#include "OvrApp.h"
#include "GuiSys.h"
#include "OVR_Locale.h"
#endif

#include "platform.h"

#if defined PLATFORM_WINDOWS || defined PLATFORM_OPENVR
#define _CRT_SECURE_NO_WARNINGS
#include "Windows.h"
#endif

#ifdef PLATFORM_ANDROID
#ifdef GAME_RELEASE_DIR_TREE
#include "../android.h"
#else
#include "../platform/android/Voxyc/app/src/main/cpp/android.h"
#endif
#endif

int g_logInitialized = 0;
int g_randomIntInitialized = 0;

#if defined PLATFORM_OSX
char *MYCFStringCopyUTF8String(CFStringRef aString) {
    if (aString == NULL) {
        return NULL;
    }
    
    CFIndex length = CFStringGetLength(aString);
    CFIndex maxSize =
    CFStringGetMaximumSizeForEncoding(length, kCFStringEncodingUTF8) + 1;
    char *buffer = (char *)malloc(maxSize);
    if (CFStringGetCString(aString, buffer, maxSize,
                           kCFStringEncodingUTF8)) {
        return buffer;
    }
    
    return NULL;
}
#endif

void printUserFilesDir(char *dest)
{
#if defined PLATFORM_OSX
    char execFname[MAX_STR_LEN];
    char componentFname[MAX_STR_LEN];
    
    CSimpleIniA ini;
    ini.SetUnicode();
    
    FSRef ref;
    OSType folderType = kApplicationSupportFolderType;
    char path[PATH_MAX];
    
    FSFindFolder(kUserDomain,folderType, kCreateFolder, &ref);
    
    FSRefMakePath(&ref, (UInt8*)&path, PATH_MAX);
    
    sprintf(dest, "%s", path);
#endif
}

void printFullResourceFilename(char *shortFilename, char *fullFilename)
{
#if defined PRODUCT_BUILD3D || defined PRODUCT_EARTH2084
#if defined PLATFORM_GVR
#endif
#if defined PLATFORM_OSX || defined PLATFORM_WINDOWS || defined PLATFORM_OPENVR
//    snprintf(fullFilename, MAX_STR_LEN, ASSETS_FILEMASK, shortFilename);
    snprintf(fullFilename, MAX_STR_LEN, "%s/%s", g_assetsDir.c_str(), shortFilename);
	return;
#endif
#else if defined PLATFORM_OSX
    CFBundleRef myBundle;

    myBundle = CFBundleGetBundleWithIdentifier(CFSTR(GLOBAL_BUNDLE_ID));
    
    // Use the bundle...
    CFURLRef iniURL;
    
    CFStringRef cfFilename = CFStringCreateWithCString(NULL, shortFilename, kCFStringEncodingUTF8);
    
    // Look for a resource in the main bundle by name and type.
    iniURL = CFBundleCopyResourceURL(myBundle,
                                     cfFilename,
                                     NULL,
                                     NULL );
    
    CFStringRef cfStr = CFURLGetString(iniURL);
    
    char *filename = MYCFStringCopyUTF8String(cfStr);
    char *filenameWithoutProtocol = filename + 7;
    
    sprintf(fullFilename, "%s", filenameWithoutProtocol);
    
    free((void *)filename);
#endif
}

void printFullExternalResourceFilename(char *shortFilename, char *fullFilename)
{
#if defined PRODUCT_BUILD3D || defined PRODUCT_EARTH2084
#if defined PLATFORM_GVR
#endif
#if defined PLATFORM_OSX || defined PLATFORM_WINDOWS || defined PLATFORM_OPENVR
	//    snprintf(fullFilename, MAX_STR_LEN, ASSETS_FILEMASK, shortFilename);
	snprintf(fullFilename, MAX_STR_LEN, "%s/%s", g_assetsDir.c_str(), shortFilename);
	return;
#endif
#else if defined PLATFORM_OSX
	CFBundleRef myBundle;

	myBundle = CFBundleGetBundleWithIdentifier(CFSTR(GLOBAL_BUNDLE_ID));

	// Use the bundle...
	CFURLRef iniURL;

	CFStringRef cfFilename = CFStringCreateWithCString(NULL, shortFilename, kCFStringEncodingUTF8);

	// Look for a resource in the main bundle by name and type.
	iniURL = CFBundleCopyResourceURL(myBundle,
		cfFilename,
		NULL,
		NULL);

	CFStringRef cfStr = CFURLGetString(iniURL);

	char *filename = MYCFStringCopyUTF8String(cfStr);
	char *filenameWithoutProtocol = filename + 7;

	sprintf(fullFilename, "%s", filenameWithoutProtocol);

	free((void *)filename);
#endif
}

std::string GetFullFilename(std::string filename)
{
#ifdef PLATFORM_IOS
    if (filename == "")
        return "";
    
    int dotPos = filename.find(".");
    
    std::string nameNoExt = "";
    std::string ext = "";
    
    if (dotPos != -1)
    {
        nameNoExt = filename.substr(0, dotPos);
        ext = filename.substr(dotPos, filename.length());
    }
    else
    {
        return "";
    }
    
    NSString *nsNameNoExt = [NSString stringWithFormat:@"%s", nameNoExt.c_str()];
    NSString *nsExt = [NSString stringWithFormat:@"%s", ext.c_str()];
    
    NSString *fullFilename = [[NSBundle mainBundle] pathForResource:nsNameNoExt ofType:nsExt];
    
    if (fullFilename == nullptr)
        return "";

    return [fullFilename UTF8String];
#endif
    
#if defined PLATFORM_ANDROID || defined PLATFORM_WINDOWS || defined PLATFORM_OSX
    return g_assetsDir + "/" + filename;
#endif
}

int FindFile(char *filename, char *folder, char *foundInFolder)
{
	int found = false;

#ifdef PLATFORM_OSX
    DIR *dir;
    struct dirent *ent;
    char nextFolder[MAX_STR_LEN];
    
    dir = opendir(folder);
    
    if (dir != NULL)
    {
        do
        {
            ent = readdir(dir);
            
            if (ent != NULL)
            {
//                printf("%s\n", ent->d_name);
                if (ent->d_type == DT_DIR)
                {
                    if (strcmp(ent->d_name, ".") && strcmp(ent->d_name, ".."))
                    {
                        if (!strcmp(folder, "/"))
                            sprintf(nextFolder, "/%s/", ent->d_name);
                        else
                            sprintf(nextFolder, "%s%s/", folder, ent->d_name);
                        
//                        closedir(dir);
                        if (FindFile(filename, nextFolder, foundInFolder) == 1)
                            return 1;
                    }
                }
                if (ent->d_type == DT_REG)
                {
                    if (!strcmp(ent->d_name, filename))
                    {
                        sprintf(foundInFolder, "%s", folder);
                        found = 1;
                        closedir(dir);
                        return found;
                    }
                }
            }
        } while (ent != NULL);
        
        closedir(dir);
    }
    else
    {
//        printf("DrumSampler::FindFile(): error opening folder %s\n", folder);
        Log("DrumSampler::FindFile(): error opening folder ", folder);
        return NULL;
    }
    
#endif
	return found;
}

void Log_Write(char *str)
{
	Logv(str);
	
#ifdef PLATFORM_GVR
    LOG(str);
#endif
#if defined PLATFORM_OSX || defined PLATFORM_ANDROID || defined PLATFORM_WINDOWS || defined PLATFORM_OPENVR
    PLAT_Log(str);
#endif
    
#ifdef DEBUG_BUILD
#ifndef WDL_OL_PLUGIN
#if defined PLATFORM_IOS
    NSString *nsStr = [NSString stringWithFormat:@"%s", str];
    NSLog(@"%@", nsStr);
#endif
#if defined PLATFORM_OSX
    std::cout << str << "\n";
#endif
#else
    char userFilesDir[MAX_STR_LEN];
    char logFilename[MAX_STR_LEN];
    char strFinal[MAX_STR_LEN];
    
    printUserFilesDir(userFilesDir);
    
    sprintf(logFilename, "%s/drumstrlog.txt", userFilesDir);
    
    FILE * fp = NULL;
    fp = fopen(logFilename, "a");
    
    sprintf(strFinal, "%s\n", str);

    int i = 0;
    int nullIdx = -1;

    while (strFinal[i] != 0 && i < MAX_STR_LEN)
        i++;

    if (i < MAX_STR_LEN)
    {
        fwrite(strFinal, sizeof(char), i * sizeof(char), fp);
    }
    
    printf("%s", strFinal);
    
    fclose(fp);
#endif
#endif
}

void Log(std::string str)
{
	Log_Write((char *)str.c_str());
}

void Log(char *str)
{
#ifdef DEBUG_BUILD
    char strFinal[MAX_STR_LEN];
    
    snprintf(strFinal, MAX_STR_LEN, "%s", str);
    
    Log_Write(strFinal);
#endif
}

void Log(char *str, char *str2)
{
#ifdef DEBUG_BUILD
    char strFinal[MAX_STR_LEN];
    
    snprintf(strFinal, MAX_STR_LEN, "%s %s", str, str2);
    
    Log_Write(strFinal);
#endif
}

void Log(char *str, int a)
{
#ifdef DEBUG_BUILD
    char strFinal[MAX_STR_LEN];
    
    snprintf(strFinal, MAX_STR_LEN, "%s %d", str, a);
    
    Log_Write(strFinal);
#endif
}

void Logx(char *str, int a)
{
#ifdef DEBUG_BUILD
    char strFinal[MAX_STR_LEN];

    snprintf(strFinal, MAX_STR_LEN, "%s 0x%x", str, a);

    Log_Write(strFinal);
#endif
}

void Log(char *str, int a, char *str2, int b)
{
#ifdef DEBUG_BUILD
    char strFinal[MAX_STR_LEN];
    
    snprintf(strFinal, MAX_STR_LEN, "%s %d %s %d", str, a, str2, b);
    
    Log_Write(strFinal);
#endif
}

void Log(char *str, int a, char *str2, int b, char *str3, int c)
{
#ifdef DEBUG_BUILD
    char strFinal[MAX_STR_LEN];
    
    snprintf(strFinal, MAX_STR_LEN, "%s %d %s %d %s %d", str, a, str2, b, str3, c);
    
    Log_Write(strFinal);
#endif
}

void Log(char *str, char *str2, char *str3, int b)
{
#ifdef DEBUG_BUILD
    char strFinal[MAX_STR_LEN];
    
    snprintf(strFinal, MAX_STR_LEN, "%s %s %s %d", str, str2, str3, b);
    
    Log_Write(strFinal);
#endif
}

void Log(char *str, int a, char *str2, char *str3)
{
#ifdef DEBUG_BUILD
    char strFinal[MAX_STR_LEN];
    
    snprintf(strFinal, MAX_STR_LEN, "%s %d %s %s", str, a, str2, str3);
    
    Log_Write(strFinal);
#endif
}
void Log(char *str, glm::mat4 mat)
{
#ifdef DEBUG_BUILD
    char strFinal[MAX_STR_LEN];
    
/*    snprintf(strFinal, "%s\n[ %10f %10f %10f %10f ]\n"\
            "[ %10f %10f %10f %10f ]\n"\
            "[ %10f %10f %10f %10f ]\n"\
            "[ %10f %10f %10f %10f ]"\
            ,
			MAX_STR_LEN,
            str, mat[0][0], mat[1][0], mat[2][0], mat[3][0],
            mat[0][1], mat[1][1], mat[2][1], mat[3][1],
            mat[0][2], mat[1][2], mat[2][2], mat[3][2],
            mat[0][3], mat[1][3], mat[2][3], mat[3][3]);
    
    Log_Write(strFinal);*/
#endif
}

void Log(char *str, glm::vec4 vec)
{
#ifdef DEBUG_BUILD
    char strFinal[MAX_STR_LEN];
    
    snprintf(strFinal, MAX_STR_LEN, "%s\n[ %10f %10f %10f %10f ]",
            str, vec[0], vec[1], vec[2], vec[3]);
    
    Log_Write(strFinal);
#endif
}

#ifdef PLATFORM_GVR
void Log(char *str, OVR::Matrix4f mat)
{
#ifdef DEBUG_BUILD
    char strFinal[MAX_STR_LEN];

    snprintf(strFinal, MAX_STR_LEN, "%s\n[ %10f %10f %10f %10f ]\n"\
            "[ %10f %10f %10f %10f ]\n"\
            "[ %10f %10f %10f %10f ]\n"\
            "[ %10f %10f %10f %10f ]"\
            ,
            str, mat.M[0][0], mat.M[1][0], mat.M[2][0], mat.M[3][0],
            mat.M[0][1], mat.M[1][1], mat.M[2][1], mat.M[3][1],
            mat.M[0][2], mat.M[1][2], mat.M[2][2], mat.M[3][2],
            mat.M[0][3], mat.M[1][3], mat.M[2][3], mat.M[3][3]);

    Log_Write(strFinal);
#endif
}
#endif

void Logf(char *str, float f)
{
#ifdef DEBUG_BUILD
    char strFinal[MAX_STR_LEN];

#if not defined PLATFORM_GVR && not defined PLATFORM_ANDROID
    int fraction = fmod(f, 1.0);
#else
    int fraction = (int)f;
#endif
    int i = (int)f;
    
    snprintf(strFinal, MAX_STR_LEN, "%s %d.%d", str, i, fraction);
    
    Log_Write(strFinal);
#endif
}

void LogHex(char *str, int a)
{
#ifdef DEBUG_BUILD
    char strFinal[MAX_STR_LEN];
    
    snprintf(strFinal, MAX_STR_LEN, "%s 0x%02x",
            str, a);
    
    Log_Write(strFinal);
#endif
}

void LogFromConsole(char *str)
{
#ifdef PLATFORM_OSX
    std::cout << str << "\n";
#endif
}

FILE *g_logFile;
bool g_logOpen = false;

int lengthOfStr(char *c)
{
	int len = 0;
	int i = 0;
	
	while (c[i] != 0)
	{
		i++;
	}
	
	return i;
}

void Logv(char *str)
{
#ifdef DEBUG_BUILD
#ifdef PLATFORM_ANDROID
    char strFinal[MAX_STR_LEN];
    char fullFilename[MAX_STR_LEN];

    snprintf(strFinal, MAX_STR_LEN, "%s\n", str);
    snprintf(fullFilename, MAX_STR_LEN, "%s/%s", g_filesDir, "log.txt");
    g_logFile = fopen(fullFilename, "a+");
        
	fwrite(strFinal, sizeof(char), lengthOfStr(strFinal), g_logFile);
	
	fclose(g_logFile);
//    Log_Write(strFinal);
#endif
#ifdef PLATFORM_WINDOWS
	char strFinal[MAX_STR_LEN];
	char fullFilename[MAX_STR_LEN];

	sprintf_s(strFinal, MAX_STR_LEN, "%s\n", str);

	sprintf_s(fullFilename, MAX_STR_LEN, "%s/%s", WINDOWS_FILES_DIR, "log.txt");
	fopen_s(&g_logFile, fullFilename, "a+");

	if (g_logFile != NULL)
	{
		fwrite(strFinal, sizeof(char), lengthOfStr(strFinal), g_logFile);
		fclose(g_logFile);
	}
	//    Log_Write(strFinal);
#endif
#endif
}

void Logv(char *str, int a)
{
#ifdef DEBUG_BUILD
#ifdef PLATFORM_ANDROID
    char strFinal[MAX_STR_LEN];
    char fullFilename[MAX_STR_LEN];

    snprintf(strFinal, MAX_STR_LEN, "%s %d\n", str, a);
	snprintf(fullFilename, MAX_STR_LEN, "%s/%s", g_filesDir, "log.txt");
    g_logFile = fopen(fullFilename, "a+");
        
	fwrite(strFinal, sizeof(char), lengthOfStr(strFinal), g_logFile);

	fclose(g_logFile);
//    Log_Write(strFinal);
#endif
#ifdef PLATFORM_WINDOWS
	char strFinal[MAX_STR_LEN];
	char fullFilename[MAX_STR_LEN];

	sprintf_s(strFinal, MAX_STR_LEN, "%s %d\n", str, a);

	sprintf_s(fullFilename, MAX_STR_LEN, "%s/%s", WINDOWS_FILES_DIR, "log.txt");
	fopen_s(&g_logFile, fullFilename, "a+");

	if (g_logFile != NULL)
	{
		fwrite(strFinal, sizeof(char), lengthOfStr(strFinal), g_logFile);
		fclose(g_logFile);
	}
	//    Log_Write(strFinal);
#endif
#endif
}

void Logv(char *stra, int a, char *strb, int b, char *strc, int c)
{
#ifdef DEBUG_BUILD
#ifdef PLATFORM_ANDROID
    char strFinal[MAX_STR_LEN];
    char fullFilename[MAX_STR_LEN];

    snprintf(strFinal, MAX_STR_LEN, "%s %d %s %d %s %d\n", stra, a, strb, b, strc, c);
	snprintf(fullFilename, MAX_STR_LEN, "%s/%s", g_filesDir, "log.txt");
    g_logFile = fopen(fullFilename, "a+");
        
	fwrite(strFinal, sizeof(char), lengthOfStr(strFinal), g_logFile);

	fclose(g_logFile);
//    Log_Write(strFinal);
#endif
#ifdef PLATFORM_WINDOWS
	char strFinal[MAX_STR_LEN];
	char fullFilename[MAX_STR_LEN];

	sprintf_s(strFinal, MAX_STR_LEN, "%s %d %s %d %s %d\n", stra, a, strb, b, strc, c);

	sprintf_s(fullFilename, MAX_STR_LEN, "%s/%s", WINDOWS_FILES_DIR, "log.txt");
	fopen_s(&g_logFile, fullFilename, "a+");

	if (g_logFile != NULL)
	{
		fwrite(strFinal, sizeof(char), lengthOfStr(strFinal), g_logFile);
		fclose(g_logFile);
	}
	//    Log_Write(strFinal);
#endif
#endif
}

void LogvReset()
{
#ifdef DEBUG_BUILD
#ifdef PLATFORM_ANDROID
    char strFinal[MAX_STR_LEN];
    char fullFilename[MAX_STR_LEN];

    snprintf(strFinal, MAX_STR_LEN, "%s", "");
    snprintf(fullFilename, MAX_STR_LEN, "%s/%s", g_filesDir, "log.txt");
    g_logFile = fopen(fullFilename, "w");
        
	fwrite(strFinal, sizeof(char), lengthOfStr(strFinal), g_logFile);
	
	fclose(g_logFile);
//    Log_Write(strFinal);
#endif
#endif
}

int RandomInt(int min, int max)
{
    if (g_randomIntInitialized == 0)
    {
        srand(time(NULL));
        g_randomIntInitialized = 1;
    }
    
    return rand() % max + min;
}

float RandomFloat(float floor, float max)
{
    float range = max - floor;
    
    if (g_randomIntInitialized == 0)
    {
        srand(time(NULL));
        g_randomIntInitialized = 1;
    }
    
    return floor + float((range * (float)rand()) / ((float)RAND_MAX));
}

float ToRadians(float degrees)
{
    return degrees * 0.0174533;
}

float ToDegrees(float radians)
{
    return radians / 0.0174533;
}

bool compareYaw(float yaw1, float yaw2, float threshold)
{
	while (yaw1 < 0.0)
		yaw1 += 360.0;
	while (yaw1 > 360.0)
		yaw1 -= 360.0;
	while (yaw2 < 0.0)
		yaw2 += 360.0;
	while (yaw2 > 360.0)
		yaw2 -= 360.0;

    if (abs(abs(yaw1) - abs(yaw2)) < threshold)
        return true;
    
    return false;
}

bool compareAngle(float angle1, float angle2, float threshold)
{
	if (angle1 == angle2)
		return true;
	
	while (angle1 < 0.0)
		angle1 += 360.0;
	while (angle1 > 360.0)
		angle1 -= 360.0;
	while (angle2 < 0.0)
		angle2 += 360.0;
	while (angle2 > 360.0)
		angle2 -= 360.0;

	angle1 += 360;
	angle2 += 360;

    if (abs(abs(angle1) - abs(angle2)) < threshold)
        return true;
    
    return false;
}

float GradualAssign(float from, float to, float factor)
{
    //    return to;
    
    if (from < to)
        from += (to - from) / factor;
    if (from > to)
        from -= (from - to) / factor;
    
    return from;
}

// MSVC defines this in winsock2.h!?
typedef struct tval {
	long tv_sec;
	long tv_usec;
} tval;

#if defined PLATFORM_WINDOWS || defined PLATFORM_OPENVR
int gettimeofday(struct tval * tp, struct timezone * tzp)
{
	// Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
	// This magic number is the number of 100 nanosecond intervals since January 1, 1601 (UTC)
	// until 00:00:00 January 1, 1970 
	static const uint64_t EPOCH = ((uint64_t)116444736000000000ULL);

	SYSTEMTIME  system_time;
	FILETIME    file_time;
	uint64_t    time;

	GetSystemTime(&system_time);
	SystemTimeToFileTime(&system_time, &file_time);
	time = ((uint64_t)file_time.dwLowDateTime);
	time += ((uint64_t)file_time.dwHighDateTime) << 32;

	tp->tv_sec = (long)((time - EPOCH) / 10000000L);
	tp->tv_usec = (long)(system_time.wMilliseconds * 1000);
	return 0;
}
#endif

long long getTimeInMsec()
{
#if defined PLATFORM_WINDOWS || defined PLATFORM_OPENVR
/*	LARGE_INTEGER fq, t;
	QueryPerformanceFrequency(&fq);
	QueryPerformanceCounter(&t);
	return (1000000 * t.QuadPart) / fq.QuadPart;*/

//	struct tval tv;
//	gettimeofday(&tv, NULL);
//	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
	return GetTickCount();
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
#endif
}

void ddlsleep(int msec)
{
#ifdef PLATFORM_WINDOWS
	Sleep(msec);
#endif
}

std::string ToString(int value)
{
    std::ostringstream os;
    os << value;
    return os.str();
}

std::string GetFileName(const std::string& s) {

    char sep = '/';

/*#ifdef _WIN32
    sep = '\\';
#endif*/

    size_t i = s.rfind(sep, s.length());
    if (i != std::string::npos) {
        return(s.substr(i+1, s.length() - i));
    }

    return("");
}

std::string GetPath(const std::string& fname) {

    size_t pos = fname.find_last_of("\\/");
    return (std::string::npos == pos)
           ? ""
           : fname.substr(0, pos);
}

std::string GetExtension(const std::string s)
{
	return s.substr(s.find_last_of(".") + 1);
}

std::string FloatToStr(float f)
{
    std::ostringstream ssx;
    ssx << f;
    std::string sx(ssx.str());

    return sx;
}

float scrToGlX(float screenX)
{
	return 2.0f * screenX / (float)PLAT_GetWindowWidth() - 1.0f;
}

float scrToGlY(float screenY)
{
	return -(2.0f * screenY / (float)PLAT_GetWindowHeight() - 1.0f);
}

float glToScrX(float glX)
{
	return ((glX + 1.0f) / 2.0f) * (float)PLAT_GetWindowWidth();
}

float glToScrY(float glY)
{
	return (float)PLAT_GetWindowHeight() - abs(((glY + 1.0f) / 2.0f) * (float)PLAT_GetWindowHeight());
}

float RotateAngleTowards(float angle, float targetAngle, float step)
{
	angle = Limit360(angle);
	targetAngle = Limit360(targetAngle);

//	char str[1024];
//	snprintf(str, 1024, "angle %d, targetAngle %d", (int)angle, (int)targetAngle);
//	Log(str);

	if (targetAngle == 0)
	{
		if (angle > 180)
			angle += step;
		else
			angle -= step;
	}
	else
	{
		float cWiseDist = 0;
		float ccWiseDist = 0;

		if (targetAngle > angle)
		{
			cWiseDist = targetAngle - angle;
			ccWiseDist = angle + (360 - targetAngle);
		}

		if (targetAngle < angle)
		{
			cWiseDist = (360 - angle) + targetAngle;
			ccWiseDist = angle - targetAngle;
		}

		if (cWiseDist > ccWiseDist)
		{
            angle -= step;
			if (targetAngle - angle < step * 2.0 && angle < targetAngle)
				angle = targetAngle;
		}
		else
		{
            angle += step;
			if (angle - targetAngle < step * 2.0 && angle > targetAngle)
				angle = targetAngle;
		}

		angle = Limit360(angle);
	}

	return angle;
}

float RotateAngleAwayFrom(float angle, float targetAngle, float step)
{
	angle = Limit360(angle);
	targetAngle = Limit360(targetAngle);

	//	char str[1024];
	//	snprintf(str, 1024, "angle %d, targetAngle %d", (int)angle, (int)targetAngle);
	//	Log(str);

	if (targetAngle == 0)
	{
		if (angle > 180)
			angle -= step;
		else
			angle += step;
	}
	else
	{
		float cWiseDist = 0;
		float ccWiseDist = 0;

		if (targetAngle > angle)
		{
			cWiseDist = targetAngle - angle;
			ccWiseDist = angle + (360 - targetAngle);
		}

		if (targetAngle < angle)
		{
			cWiseDist = (360 - angle) + targetAngle;
			ccWiseDist = angle - targetAngle;
		}

		if (cWiseDist > ccWiseDist)
		{
			angle += step;
		}
		else
		{
			angle -= step;
		}

		angle = Limit360(angle);
	}

	return angle;
}

float Limit360(float value)
{
	while (value > 360)
		value -= 360;
	while (value < 0)
		value += 360;

	return value;
}

void CalcNormal(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float &nx, float &ny, float &nz)
{
    glm::vec3 pt1 = glm::vec3(x1, y1, z1);
	glm::vec3 pt2 = glm::vec3(x2, y2, z2);
	glm::vec3 pt3 = glm::vec3(x3, y3, z3);
	
	glm::vec3 u = pt2 - pt1;
	glm::vec3 v = pt3 - pt1;
	
	nx = u.y * v.z - u.z * v.y;
	ny = u.z * v.x - u.x * v.z;
	nz = u.x * v.y - u.y * v.x;
	
	float length = sqrt(nx * nx + ny * ny + nz * nz);
	
	nx = nx / length;
	ny = ny / length;
	nz = nz / length;
}

float UCharToFloat(unsigned char c)
{
	return (float)c / 127.0f;
}

unsigned char FloatToUChar(float c)
{
	if (c < 0.0)
		c = 0.0;

	if (c > 2.0)
		c = 2.0;

	return (unsigned char)(c * 127.0f);
}

float UCharToFloat255(unsigned char c)
{
	return (float)c / 255.0f;
}

unsigned char FloatToUChar255(float c)
{
	if (c < 0.0)
		c = 0.0;

	if (c > 1.0)
		c = 1.0;

	return (unsigned char)(c * 255.0f);
}
