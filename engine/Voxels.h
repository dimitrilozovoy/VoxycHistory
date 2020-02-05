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

#ifndef VOXELS_H
#define VOXELS_H

#include <string>
#include <vector>
#include "../thirdparty/glm/glm.hpp"
#include <map>
#include "Globals.hpp"
#include "Mesh.h"
#include "TextureManager2.h"

typedef struct Voxel
{
	char texture;
#ifdef DO_VERTEX_LIGHTS
    unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
#endif
/*    char xofs;
	char yofs;
	char zofs;*/
} Voxel;

typedef struct VoxelStack
{
	Voxel *voxels = nullptr;
	int height = 0;
} VoxelStack;


typedef struct Vertex
{
	float x;
	float y;
	float z;
	float w;
	float u;
	float v;
	float nx;
	float ny;
	float nz;
	float r;
	float g;
	float b;
} Vertex;

class Voxels
{
public:
    void init(int size, std::map<std::string, std::string> *extraStrings);
	void set(int x, int y, int z, char value);
	char get(int x, int y, int z);
	void setrgba(int x, int y, int z, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void getrgba(int x, int y, int z, unsigned char &r, unsigned char &g, unsigned char &b, unsigned char &a);
	VoxelStack *getStack(int x, int z);
	void setStackHeight(int x, int z, int height);
	void build(TextureManager2 *texMan);
	void build_process();
	void addQuad(std::string texture, Vertex ll, Vertex lr, Vertex ur, Vertex ul, int dir, float texSpanX, float texSpanY);
	void addVertex(std::string texture, float x, float y, float z, float w, float u, float v, float nx, float ny, float nz, float r, float g, float b);
	std::vector<Mesh*> getMeshes();
	std::map<std::string, VertexBuffer*> getBuffers() { return buffers; };
	void clear();
	void checkGLError(char *tag);
	int getSize() { return size; };
	int save(std::string fname, FILE *f);
	int load(std::string fname, FILE *f, TextureManager2 *texMan);
	void calcMinMax();
	void crop();
	void setVoxelTexture(int voxel, std::string texture);
	std::string getVoxelTexture(int voxel);
	int getTextureVoxel(std::string texture);
	std::map<int, std::string> *getVoxelTextures() { return &voxelTextures; };
	void copyFrom(Voxels *source);

	bool rebuild = false;
	bool build_done = false;

private:
    VoxelStack *stacks;
	int size = 0;
	bool initialized = false;
	int minx = 0;
	int maxx = 0;
	int miny = 0;
	int maxy = 0;
	int minz = 0;
	int maxz = 0;

	std::map<std::string, VertexBuffer*> buffers;
	std::map<int, std::string> voxelTextures;
	std::map<std::string, std::string> *extraStrings;

	TextureManager2 *texMan;
	
//	static const int verticesSize = 1000000;
//  float vertices[verticesSize];
//	static int cursor;
};

#endif //FATELESS_TEXTUREMANAGER_H
