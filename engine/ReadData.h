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
};

typedef struct DataFile
{
	bool loaded = false;
	std::string fullFilename;
	std::map<std::string, DataFileEntry> files;
};

extern DataFile g_readDataFile;

void ReadDataOpenDAT(std::string datfname);
void ReadDataGetFile(std::string name, void* &data, long& size);
