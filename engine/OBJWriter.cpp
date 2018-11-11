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

#include <sstream>
//#include <bits/ios_base.h>
#include <ios>
#include <fstream>
#include <iomanip>
#include "Globals.hpp"
#include "OBJWriter.h"

int OBJWriter::write(std::map<std::string, VertexBuffer*> buffers, std::string fname)
{
    std::wfstream fs;

    fs.open(fname, std::ios::out);

    if (!fs.is_open())
        return 0;

//    std::wostringstream vertexData;
//    std::wostringstream faceData;

    int objVertexNum = 1;

    for (const auto &pair : buffers)
    {
        VertexBuffer *buf = pair.second;

        if (buf->size > 0)
        {
            int coordNum = 1;
            int vertexNumInThisFace = 1;
            float x = 0.0;
            float y = 0.0;
            float z = 0.0;
            float u = 0.0;
            float v = 0.0;

            for (int c = 0; c < buf->size; c++)
            {
                switch (coordNum)
                {
                    case 1:
                        x = buf->vertices[c];
                        coordNum++;
                        break;
                    case 2:
                        y = buf->vertices[c];
                        coordNum++;
                        break;
                    case 3:
                        z = buf->vertices[c];
                        coordNum++;
                        break;
                    case 4:
                        u = buf->vertices[c];
                        coordNum++;
                        break;
                    case 5:
                        v = buf->vertices[c];
                        coordNum++;
                        break;
                    case 6:
                            fs << "v "
                               << std::fixed << std::setprecision(5) << x
                               << " "
                               << std::fixed << std::setprecision(5) << y
                               << " "
                               << std::fixed << std::setprecision(5) << z
                               << "\n";

                            fs << "vt "
                               << std::fixed << std::setprecision(5) << u
                               << " "
                               << std::fixed << std::setprecision(5) << v
                               << "\n";

                            if (vertexNumInThisFace == 3) {
                                fs << "f "
                                   << objVertexNum
                                   << " "
                                   << std::fixed << std::setprecision(5) << (objVertexNum + 1)
                                   << " "
                                   << std::fixed << std::setprecision(5) << (objVertexNum + 2)
                                   << "\n";

                                objVertexNum += 3;
                                vertexNumInThisFace = 0;
                            }

                            vertexNumInThisFace++;
                            coordNum = 1;
                        break;
                }
            }
        }
    }

//    fs << vertexData;
//    fs << faceData;

    fs.close();

    return 1;
}
