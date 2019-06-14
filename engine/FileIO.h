//
// Created by dimit on 11/5/2018.
//

#ifndef VOXYC_FILEIO_H
#define VOXYC_FILEIO_H

#include <cstdlib>
#include <string>
#include <map>

class FileIO
{
public:
    void setFile(FILE *f);

    bool openToWrite(std::string fname);
    void closeFile();
    void write3Char(char c1, char c2, char c3);
    void write6Char(char c1, char c2, char c3, char c4, char c5, char c6);
    void writeInt(int i);
	void writeFloat(float f);
    void writeStr(std::string s);
    void clearKVSet();
    void addKV(std::string key, std::string value);
    void addKV(std::string key, int value);
    void addKV(std::string key, float value);
    void addKV(std::string key, bool value);
    void writeKVSet();

    bool openToRead(std::string fname);
    void read3Char(char &c1, char &c2, char &c3);
    bool read3CharEquals(char c1, char c2, char c3);
    void read6Char(char &c1, char &c2, char &c3, char &c4, char &c5, char &c6);
    bool read6CharEquals(char c1, char c2, char c3, char c4, char c5, char c6);
    int readInt();
    float readFloat();
    std::string readStr();
    void readKVSet();
    std::string getKVStr(std::string key);
    int getKVInt(std::string key);
    float getKVFloat(std::string key);
    bool getKVBool(std::string key);

protected:
    FILE *file = nullptr;
    std::map<std::string, std::string> kvSet;
};

#endif //VOXYC_FILEIO_H
