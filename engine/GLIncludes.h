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

#ifndef FATELESS_GLINCLUDES_H
#define FATELESS_GLINCLUDES_H

#include "Globals.hpp"
#ifdef PLATFORM_OPENVR
#include <../thirdparty/GL/glew.h>
#include <../thirdparty/GL/glu.h>
#endif

#ifdef PLATFORM_ANDROID
#ifdef GAME_RELEASE_DIR_TREE
#include "../javagl.h"
#else
#include "../platform/android/Voxyc/app/src/main/cpp/javagl.h"
#endif
#endif

#ifdef PLATFORM_IOS
#import <UIKit/UIKit.h>
#import "GLKit/GLKit.h"
#endif

#ifdef PLATFORM_WINDOWS
#include "../thirdparty/GL/glew.h"
#include "../thirdparty/GLFW/glfw3.h"
#endif

#ifdef PLATFORM_OSX
#include "../thirdparty/macos/GL/glew.h"
#include "../thirdparty/macos/GLFW/glfw3.h"
#endif

#include "../thirdparty/glm/glm.hpp"
#include "../thirdparty/glm/gtc/matrix_transform.hpp"
#include "../thirdparty/glm/gtc/type_ptr.hpp"

#endif //FATELESS_GLINCLUDES_H
