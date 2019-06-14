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


#include "Mesh.h"
#include "DDLUtils.hpp"

void Mesh::clear()
{
	vertices.clear();
}

void Mesh::save(std::string fname, FILE *f)
{
    if (!openToWrite(fname))
        return;
		
	write6Char('M', 'E', 'S', 'H', ' ', ' ');
    write3Char('1', '0', '0');

	write6Char('V', 'E', 'R', 'T', 'C', 'S');
	
	writeInt(vertices.size());
	
	for (int v = 0; v < vertices.size(); v++)
	{
		writeFloat(vertices[v].x);
		writeFloat(vertices[v].y);
		writeFloat(vertices[v].z);
	}
	
	closeFile();
}

void Mesh::load(std::string fname, FILE *f)
{
    if (!openToRead(fname))
        return;
		
	if (!read6CharEquals('M', 'E', 'S', 'H', ' ', ' '))
            return;

	char c1, c2, c3;
	
	read3Char(c1, c2, c3);
	
    if (c1 == '1' && c2 == '0' && c3 == '0')
    {
	    if (!read6CharEquals('V', 'E', 'R', 'T', 'C', 'S'))
            return;
		
		int numVerts = readInt();
		
//		Log("numVerts", numVerts);
		
	    for (int v = 0; v < numVerts; v++)
	    {
			glm::vec3 vert;
			
		    vert.x = readFloat();
		    vert.y = readFloat();
		    vert.z = readFloat();
			
//			Log("x", (int)(vert.x * 100));
//			Log("y", (int)(vert.y * 100));
//			Log("z", (int)(vert.z * 100));
			
			vertices.push_back(vert);
	    }
    }
	
	closeFile();
}
