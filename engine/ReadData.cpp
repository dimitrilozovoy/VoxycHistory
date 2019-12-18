#include <iostream>
#include "ReadData.h"
#include "../thirdparty/lodepng/lodepng.hpp"

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

		for (int i = 0; i < DATAFILE_MAX_FILES; i++)
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

void ReadDataGetFile(std::string name, char *&data, long& size)
{
	if (!g_readDataFile.loaded)
	{
		std::cout << "ReadDataGetFile: data file not loaded";
		return;
	}

	data = (char *)malloc(g_readDataFile.files[name].size);
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

bool ReadDataGetLine(char *data, long size, long &cursor, std::string& line)
{
	if (cursor >= size)
		return false;

	line = "";

	while (data[cursor] != '\n' && cursor < size)
	{ 
		line += data[cursor];
		cursor++;
	}

	line += '\n';
	cursor++; 

 	return true;
}

unsigned lodepng_load_file_from_dat(unsigned char** out, size_t* outsize, const char* filename)
{
	FILE* file;
	long size;

	/*provide some proper output values if error will happen*/
	*out = 0;
	*outsize = 0;

	file = fopen(g_readDataFile.fullFilename.c_str(), "rb");
	if (!file) return 78;

	fseek(file, g_readDataFile.files[filename].pos, SEEK_SET);

	/*get filesize:*/
//	fseek(file, 0, SEEK_END);
//	size = ftell(file);
//	rewind(file);

	size = g_readDataFile.files[filename].size;

	/*read contents of the file into the vector*/
	*outsize = 0;
	*out = (unsigned char*)malloc((size_t)size);
	if (size && (*out)) (*outsize) = fread(*out, 1, (size_t)size, file);

	fclose(file);
	if (!(*out) && size) return 83; /*the above malloc failed*/
	return 0;
}

/*
Example 2
Load PNG file from disk to memory first, then decode to raw pixels in memory.
*/
/*void decodeTwoStepsFromDAT(const char* filename) {
	unsigned error;
	unsigned char* image = 0;
	unsigned width, height;
	unsigned char* png = 0;
	size_t pngsize;

	error = lodepng_load_file_from_dat(&png, &pngsize, filename);
	if (!error) error = lodepng_decode32(&image, &width, &height, png, pngsize);
	if (error) printf("error %u: %s\n", error, lodepng_error_text(error));

	free(png);

	free(image);
}*/

