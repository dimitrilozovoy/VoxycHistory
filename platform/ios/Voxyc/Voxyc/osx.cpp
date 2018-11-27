//
//  osx.cpp
//  Voxyc
//
//  Created by Dimitri Lozovoy on 11/26/18.
//
//

#include <stdio.h>
#include <string>

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

int PLAT_LoadSound(char *filename, bool stereo)
{
    return 0;
}

int PLAT_PlaySound(int idx)
{
    return 0;
}

void PLAT_StopSound(int stream)
{
}

void PLAT_PlayTrack(char *filename, bool stereo)
{
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
    return 1024;
}

int PLAT_GetWindowHeight()
{
    return 768;
}

long PLAT_GetTime()
{
    return 0;
}

int PLAT_stoi(std::string s, int fallback)
{
    return 0;
}

float PLAT_stof(std::string s, float fallback)
{
    return 0.0f;
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

void PLAT_SavePref(std::string s1, std::string s2, std::string s3)
{
}

std::string PLAT_LoadPref(std::string s1, std::string s2, std::string s3)
{
    return "";
}
