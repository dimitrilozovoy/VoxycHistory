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

#include "Globals.hpp"
#include "OzzRenderer.h"
#include "DDLUtils.hpp"

#ifdef USE_OZZ
#include "ozz/base/io/stream.h"
#include "ozz/base/io/archive.h"
#include "ozz/base/log.h"
#include "ozz/base/maths/box.h"
#include "ozz/base/memory/allocator.h"
#include "ozz/options/options.h"
#include "ozz/animation/runtime/animation.h"
#include "ozz/animation/runtime/skeleton.h"
#endif

void OzzRenderer::init()
{
#ifdef USE_OZZ
	char *name = "buffhorn.skel";

	char fullFilename[MAX_STR_LEN];

	printFullResourceFilename((char *)name, fullFilename);

	ozz::io::File file(fullFilename, "rb");

	// Checks file status, which can be closed if filename is invalid.
	if (!file.opened()) {
		ozz::log::Err() << "Cannot open file " << fullFilename << "." << std::endl;
	}

	ozz::io::IArchive archive(&file);

	if (!archive.TestTag<ozz::animation::Skeleton>()) {
		ozz::log::Err() << "Archive doesn't contain the expected object type." <<
			std::endl;
	}

	ozz::animation::Skeleton skeleton;
	archive >> skeleton;
#endif
}

void OzzRenderer::draw()
{
}
