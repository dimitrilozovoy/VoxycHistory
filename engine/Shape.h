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

#ifndef FATELESS_SHAPE_H
#define FATELESS_SHAPE_H

#include <string>
#include "Mesh.h"
#include <vector>
#include "Voxels.h"
#include "FileIO.h"
#include "TextureManager2.h"
#include "CustomTris.h"

typedef enum
{
	SHAPE_SPRITE,
    SHAPE_QUAD,
    SHAPE_BLOCK,
    SHAPE_TERRAIN,
    SHAPE_BUILDINGWALL,
	SHAPE_VOXELS,
    SHAPE_CUSTOM,
    NUM_SHAPES
} ObjShapeType;

typedef enum
{
	SHAPE_NOT_READY,
	SHAPE_BUILDING,
	SHAPE_READY
} ObjShapeState;

class Shape: FileIO
{
public:
    void generate(std::map<std::string, std::string> *stringExtras);
	void rebuild(TextureManager2 *texMan);
	ObjShapeState getState();
    void free();
    void checkGLError(char *tag);
	Voxels *getVoxels() { return voxels; };
    void save(FILE *f);
    void load(FILE *f, TextureManager2 *texMan);

    std::string name;
    ObjShapeType type;
	ObjShapeState state = SHAPE_NOT_READY;
    float sizeA;
    float sizeB;
    float sizeC;
    float sizeD;
	float sizeE;
	float sizeF;
	float sizeG;

    int numCoords = 0;
    int vbo = 0;
	
    std::vector<Mesh*> meshes;
	Voxels *voxels = nullptr;
	CustomTris *customTris = nullptr;
	bool built = false;
	bool needsRebuild = false;
};

#endif //FATELESS_SHAPE_H
