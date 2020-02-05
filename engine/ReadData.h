#pragma once

#include <string>
#include <map>
#include <fstream>

#define DATAFILE_MAX_FILES 1000
#define DATAFILE_MAX_FILENAME_LEN 259

typedef struct DataFileEntry
{
	long pos;
	long size;
} DataFileEntry;

typedef struct DataFile
{
	bool loaded = false;
	std::string fullFilename;
	std::map<std::string, DataFileEntry> files;
};

extern DataFile g_readDataFile;

void ReadDataOpenDAT(std::string datfname);
void ReadDataGetFile(std::string name, char* &data, long& size);
bool ReadDataGetLine(char* data, long size, long &cursor, std::string& line);
unsigned lodepng_load_file_from_dat(unsigned char** out, size_t* outsize, const char* filename);
