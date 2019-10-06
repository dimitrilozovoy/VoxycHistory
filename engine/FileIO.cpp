 //
// Created by dimit on 11/5/2018.
//

#include <sstream>
#include "FileIO.h"
#include "platform.h"

void FileIO::setFile(FILE *f)
{
    this->file = f;
}

bool FileIO::openToWrite(std::string fname)
{
    file = fopen(fname.c_str(), "wb+");

    if (file == nullptr)
        return false;
    else
        return true;
}

void FileIO::closeFile()
{
    if (file != nullptr)
        fclose(file);
}

void FileIO::write3Char(char c1, char c2, char c3)
{
    if (file == nullptr)
        return;

    char vxSig[] = {c1, c2, c3};
    fwrite(vxSig, sizeof(char), 3, file);
}

void FileIO::write6Char(char c1, char c2, char c3, char c4, char c5, char c6)
{
    if (file == nullptr)
        return;

    char vxSig[] = {c1, c2, c3, c4, c5, c6};
    fwrite(vxSig, sizeof(char), 6, file);
}

void FileIO::writeInt(int i)
{
    if (file == nullptr)
        return;

    int intArr[] = {i};
    fwrite(intArr, sizeof(int), 1, file);
}

void FileIO::writeFloat(float f)
{
    if (file == nullptr)
        return;

    float floatArr[] = {f};
    fwrite(floatArr, sizeof(float), 1, file);
}

void FileIO::writeStr(std::string s)
{
    // Write length of string
    const unsigned int texFilenameLengthArr[] = { static_cast<unsigned int>(s.length()) };
    fwrite(texFilenameLengthArr, sizeof(int), 1, file);

    // Write string
    const char *cTexFilename = s.c_str();
    fwrite(cTexFilename, sizeof(char), s.length(), file);
}

void FileIO::clearKVSet()
{
    kvSet.clear();
}

void FileIO::addKV(std::string key, std::string value)
{
    kvSet[key] = value;
}

void FileIO::addKV(std::string key, int value)
{
    char cstr[256];
    snprintf(cstr, 256, "%d", value);
    kvSet[key] = cstr;
}

void FileIO::addKV(std::string key, float value)
{
    std::ostringstream ss;
    ss << value;
    std::string s(ss.str());

    kvSet[key] = s;
}

void FileIO::addKV(std::string key, bool value)
{
    if (value)
        kvSet[key] = "1";
    else
        kvSet[key] = "0";
}

void FileIO::writeKVSet()
{
    write6Char('K', 'V', 'S', 'E', 'T', ' ');
    writeInt(kvSet.size());

    for(const auto &pair: kvSet)
    {
        writeStr(pair.first);
        writeStr(pair.second);
    }
}

bool FileIO::openToRead(std::string fname)
{
    file = fopen(fname.c_str(), "rb");

    if (file == nullptr)
        return false;
    else
        return true;
}

void FileIO::read3Char(char &c1, char &c2, char &c3)
{
    char arr[] = {' ', ' ', ' '};
    if (fread(arr, sizeof(char), 3, file) != 3)
        return;

    c1 = arr[0];
    c2 = arr[1];
    c3 = arr[2];
}

bool FileIO::read3CharEquals(char c1, char c2, char c3)
{
    char arr[] = {' ', ' ', ' '};
    if (fread(arr, sizeof(char), 3, file) != 3)
        return false;

    bool result = false;

    if (c1 == arr[0]
        && c2 == arr[1]
        && c3 == arr[2])
        result = true;

    return result;
}

void FileIO::read6Char(char &c1, char &c2, char &c3, char &c4, char &c5, char &c6)
{
    char arr[] = {' ', ' ', ' ', ' ', ' ', ' '};
    if (fread(arr, sizeof(char), 6, file) != 6)
        return;

    c1 = arr[0];
    c2 = arr[1];
    c3 = arr[2];
    c4 = arr[3];
    c5 = arr[4];
    c6 = arr[5];
}

bool FileIO::read6CharEquals(char c1, char c2, char c3, char c4, char c5, char c6)
{
    char arr[] = {' ', ' ', ' ', ' ', ' ', ' '};
    if (fread(arr, sizeof(char), 6, file) != 6)
        return false;

    bool result = false;

    if (c1 == arr[0]
        && c2 == arr[1]
        && c3 == arr[2]
        && c4 == arr[3]
        && c5 == arr[4]
        && c6 == arr[5])
        result = true;

    return result;
}

int FileIO::readInt()
{
    int iArr[] = { 0 };
    fread(iArr, sizeof(int), 1, file);
    int i = iArr[0];

    return i;
}

float FileIO::readFloat()
{
    float fArr[] = { 0.0 };
    fread(fArr, sizeof(float), 1, file);
    float f = fArr[0];

    return f;
}

std::string FileIO::readStr()
{
    int strLengthArr[] = {0};
    fread(strLengthArr, sizeof(int), 1, file);
    int strLength = strLengthArr[0];

    char *cStr = (char *)malloc(strLength + 1);
    fread((void *)cStr, sizeof(char), strLength, file);
    cStr[strLength] = 0;

    std::string str = cStr;

    return str;
}

void FileIO::readKVSet()
{
    if (!read6CharEquals('K', 'V', 'S', 'E', 'T', ' '))
        return;

    int size = readInt();

    for (int i = 0; i < size; i++)
    {
        std::string key = readStr();
        std::string value = readStr();

        kvSet[key] = value;
    }
}

std::string FileIO::getKVStr(std::string key)
{
    return kvSet[key];
}

int FileIO::getKVInt(std::string key)
{
    int ival = PLAT_stoi(kvSet[key]);

    return ival;
}

float FileIO::getKVFloat(std::string key)
{
    float fval = PLAT_stof(kvSet[key], 1.0);

    return fval;
}

bool FileIO::getKVBool(std::string key)
{
    if (kvSet[key] == "1")
        return true;
    else
        return false;
}
