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

#ifndef FATELESS_TERRAIN_H
#define FATELESS_TERRAIN_H

#include "Shape.h"

class Terrain
{
public:
    static void generate(int sizexz, int holeSizeX, int holeSizeZ, int dropSizeX, int dropSizeZ, int bottomSizeX, int bottomSizeZ);
    static void checkGLError(char *tag);

    static int vbo;
    static int numCoords;
    static int valleySizeX;
	static int valleySizeZ;
	static int dropSizeX;
	static int dropSizeZ;
	static int bottomSizeX;
	static int bottomSizeZ;

    static const int verticesSize;
    static float *vertices;
};

#endif //FATELESS_TERRAIN_H
