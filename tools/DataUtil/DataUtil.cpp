#include <iostream>
#include <fstream>
#include "../../thirdparty/clara/clara.hpp"
#include "../../thirdparty/dirent/dirent.h"
#include "../../engine/DDLUtils.hpp"
#include "../../engine/ReadData.h"

std::string GetFileName(const std::string& s) {

	char sep = '/';

	/*#ifdef _WIN32
		sep = '\\';
	#endif*/

	size_t i = s.rfind(sep, s.length());
	if (i != std::string::npos) {
		return(s.substr(i + 1, s.length() - i));
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

void dirToFile(std::string dirname, std::string outfname)
{
	DIR* dir;
	struct dirent* ent;

	if ((dir = opendir((const char*)dirname.c_str())) == NULL)
	{
		dirname = GetPath(dirname);
	}

	std::map<std::string, DataFileEntry> files;

	if ((dir = opendir((const char*)dirname.c_str())) != NULL)
	{
		std::ofstream out(outfname, std::ifstream::binary/* || std::ifstream::trunc*/);

		// Write signature
		out << "DAT";
		int vera = 1;
		int verb = 0;
		int verc = 0;
		out.write(reinterpret_cast<const char*>(&vera), sizeof(vera));
		out.write(reinterpret_cast<const char*>(&verb), sizeof(verb));
		out.write(reinterpret_cast<const char*>(&verc), sizeof(verc));

		// Write max number of files
		int maxFiles = DATAFILE_MAX_FILES;
		out.write(reinterpret_cast<const char*>(&verc), sizeof(maxFiles));

		// Write empty file table
		for (int i = 0; i < DATAFILE_MAX_FILES; i++)
		{
			// Write file name
			char fname[DATAFILE_MAX_FILENAME_LEN];

			for (int j = 0; j < DATAFILE_MAX_FILENAME_LEN; j++)
			{
				fname[j] = 0;
			}

			out.write(fname, DATAFILE_MAX_FILENAME_LEN);

			// Write empty file position
			long pos = 0;
			out.write(reinterpret_cast<const char*>(&pos), sizeof(pos));

			// Write empty file size
			out.write(reinterpret_cast<const char*>(&pos), sizeof(pos));
		}

		long curPos = 3 + sizeof(int) * 4 + DATAFILE_MAX_FILES * (DATAFILE_MAX_FILENAME_LEN + sizeof(long) + sizeof(long));
		long lastPos = 3 + sizeof(int) * 4 + DATAFILE_MAX_FILES * (DATAFILE_MAX_FILENAME_LEN + sizeof(long) + sizeof(long));

		// Write file data
		while ((ent = readdir(dir)) != NULL)
		{
			std::string fname = ent->d_name;

			if (fname != "." and fname != "..")
			{
				std::string ext = GetExtension(fname);

				if (ext == "jpg" || ext == "obj" || ext == "png")
//				if (ext == "jpg" || ext == "lua" || ext == "obj" || ext == "ogg" || ext == "png" || ext == "sc" || ext == "vx")
				{
					std::ifstream in(dirname + "/" + fname, std::ifstream::binary);

					std::cout << fname;

					long size = 0;
					lastPos = curPos;

					while (!in.eof())
					{
						char c = in.get();
 						out << c;
						size++;
						curPos++;
					}

					files[fname].pos = lastPos;
					files[fname].size = size;

					std::cout << "       OK " << "pos: " << lastPos << " size: " << size <<  "\n";

					in.close();
				}
			}
		}

		// Go to beginning of file table
		out.seekp(3 + sizeof(int) * 4, std::ios::beg);
 
		// Write actual file table
		for (const auto& pair: files)
		{
			std::string fnameStr = pair.first;
			long fpos = pair.second.pos;
			long fsize = pair.second.size;

			// Write file name
			char fname[DATAFILE_MAX_FILENAME_LEN];

			for (int j = 0; j < DATAFILE_MAX_FILENAME_LEN; j++)
			{
				fname[j] = 0;
			}

			snprintf(fname, DATAFILE_MAX_FILENAME_LEN, "%s", fnameStr.c_str());

			out.write(fname, DATAFILE_MAX_FILENAME_LEN);

			// Write file position
			out.write(reinterpret_cast<const char*>(&fpos), sizeof(fpos));

			// Write file size
			out.write(reinterpret_cast<const char*>(&fsize), sizeof(fsize));
		}

		// Close
		out.close();

		closedir(dir);
	}
	else {
		std::cout << "Directory parsing error" << std::endl;
	}
}

int main(int argc, const char* argv[])
{
    std::cout << "DataUtil\n";

	std::string inputdir = ".";
	std::string outputfile = "data.dat";

	auto cli = clara::Opt(inputdir, "inputdir")
		["-i"]["--inputdir"]
		| clara::Opt(outputfile, "outputfile")
		["-o"]["--outputfile"];

	auto result = cli.parse(clara::Args(argc, argv));
	if (!result) {
		std::cerr << "Error in command line: " << result.errorMessage() << std::endl;
		exit(1);
	}

	dirToFile(inputdir, outputfile);
}
