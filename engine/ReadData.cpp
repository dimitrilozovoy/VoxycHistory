#include <iostream>
#include "ReadData.h"

DataFile g_readDataFile;

void ReadDataOpenDAT(std::string datfname)
{
	std::ifstream in(datfname, std::ifstream::binary/* || std::ifstream::trunc*/);

	// Read signature
	char c1 = ' ';
	char c2 = ' ';
	char c3 = ' ';

	c1 = in.get();
	c2 = in.get();
	c3 = in.get();

	if (c1 != 'D' || c2 != 'A' || c3 != 'T')
		return;

	// Read version
	int vera = 0;
	int verb = 0;
	int verc = 0;
	in.read(reinterpret_cast<char*>(&vera), sizeof(vera));
	in.read(reinterpret_cast<char*>(&verb), sizeof(verb));
	in.read(reinterpret_cast<char*>(&verc), sizeof(verc));

	if (vera == 1 && verb == 0 && verc == 0)
	{
		// Read max number of files
		int maxFiles = 0;
		in.read(reinterpret_cast<char*>(&maxFiles), sizeof(maxFiles));

		for (int i = 0; i < DATAFILE_MAX_FILENAME_LEN; i++)
		{
			// Read file name
			char fname[DATAFILE_MAX_FILENAME_LEN];

			in.read(fname, DATAFILE_MAX_FILENAME_LEN);

			long fpos = 0;
			long fsize = 0;

			in.read(reinterpret_cast<char*>(&fpos), sizeof(fpos));
			in.read(reinterpret_cast<char*>(&fsize), sizeof(fsize));

			g_readDataFile.files[fname].pos = fpos;
			g_readDataFile.files[fname].size = fsize;
		}

		g_readDataFile.loaded = true;
		g_readDataFile.fullFilename = datfname;
	}
}

void ReadDataGetFile(std::string name, void *&data, long& size)
{
	if (!g_readDataFile.loaded)
	{
		std::cout << "ReadDataGetFile: data file not loaded";
		return;
	}

	data = malloc(g_readDataFile.files[name].size);
	size = g_readDataFile.files[name].size;

	if (data != 0 && size != 0)
	{
		long pos = g_readDataFile.files[name].pos;

		std::ifstream dat(g_readDataFile.fullFilename, std::ifstream::binary/* || std::ifstream::trunc*/);
		dat.seekg(pos, std::ios::beg);
		dat.read(reinterpret_cast<char*>(data), size);
		char* cdata = (char*)data;
		dat.close();
	}
	else
	{
		data = 0;
		size = 0;
	}
}
