/*
Copyright (C) 2018 Dimitri Lozovoy

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies ofg the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN AC5t55TION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "Globals.hpp"
#include "Audio.h"
#ifdef USE_OPENAL
#include "../thirdparty/openal/alhelpers.h"
#pragma comment(lib, "OpenAL32.lib")
#include "../thirdparty/sndfile/sndfile.hh"
#endif
#include "DDLUtils.hpp"

#define MAX_BUFFER_LEN_SFX		16384 * 32
#define MAX_BUFFER_LEN_MUSIC	16384 * 2200

#if defined PLATFORM_ANDROID || defined PLATFORM_IOS
PlatformAudio g_audio;
#else
Audio g_audio;
#endif

void Audio::init()
{
#ifdef USE_OPENAL
	InitAL(NULL, NULL);
#endif
}

int Audio::loadSound(char *fname, bool stereo, bool music)
{
#ifdef USE_OPENAL
	// Find ALBuffer to use
	int i = 1;

	while (i < MAX_ALBUFFERS && buffers[i].used == true)
	{
		i++;
	}

	if (i >= MAX_ALBUFFERS)
	{
		Log("Ran out of sound buffers");
		return 0;
	}

	int albufid; 
	 
	// Generate buffer
	alGenBuffers(1, (ALuint *)&albufid);
	checkError("alGenBuffers");

	buffers[i].bufferId = albufid;
	buffers[i].used = true;
	buffers[i].filename = fname;

	int bufferLen = MAX_BUFFER_LEN_SFX;

	if (music)
		bufferLen = MAX_BUFFER_LEN_MUSIC;

	// Read sound file
	static short buffer[MAX_BUFFER_LEN_MUSIC];
	SndfileHandle file;

	memset(buffer, 0, MAX_BUFFER_LEN_MUSIC);

	char fullFilename[MAX_STR_LEN];
	printFullResourceFilename(fname, fullFilename);

	file = SndfileHandle(fullFilename);

	int size = file.frames() * sizeof(float);

#ifdef DEBUG_BUILD
	printf("Opened file '%s'\n", fname);
	printf("    Sample rate : %d\n", file.samplerate());
	printf("    Channels    : %d\n", file.channels());
#endif

	if (file.channels() > 0)
		buffers[i].loaded = true;

	file.read(buffer, bufferLen);

	// Fill buffer
	ALint format;

	if (stereo)
		format = AL_FORMAT_STEREO16;
	else
		format = AL_FORMAT_MONO16;

	alBufferData(buffers[i].bufferId, format, buffer, size, file.samplerate());
	checkError("alBufferData");
	
	return i;
#endif

	return 0;
}

void Audio::unloadSound(int sndidx)
{
#ifdef USE_OPENAL
	if (buffers[sndidx].used)
	{
		ALuint bufid = buffers[sndidx].bufferId;
		alDeleteBuffers(1, &bufid);
		checkError("alDeleteBuffers");
	}
#endif
}

int Audio::playSound(std::string filename, bool stereo)
{
#ifdef USE_OPENAL
	for (int i = 0; i < MAX_ALBUFFERS; i++)
	{
		if (buffers[i].filename == filename)
		{
			return playSound(i);
		}
	}

	// Did not find the buffer! Load it and then play.
	int idx = loadSound((char *)filename.c_str(), stereo);
	return playSound(idx);
#endif

	return 0;
}

int Audio::playSound(int bufferIdx)
{
#ifdef USE_OPENAL
	if (!buffers[bufferIdx].loaded)
		return 0;

	// Find ALSource to use
	int i = 1;

	while (i < MAX_ALSOURCES && sources[i].used == true)
	{
		i++;
	}

	if (i >= MAX_ALSOURCES)
	{
		Log("Ran out of sound sources");
		return 0;
	}

	int alsrcid;

	// Generate source and play it
	alGenSources(1, (ALuint *)&alsrcid);
	checkError("alGenSources");

	sources[i].sourceId = alsrcid;
	sources[i].used = true;

	alSourcei(sources[i].sourceId, AL_BUFFER, buffers[bufferIdx].bufferId);
	checkError("alSourcei");

	alSourcePlay(sources[i].sourceId);
	checkError("alSourcePlay");

	return i;
#endif

	return 0;
}

void Audio::stopSound(int stream)
{
#ifdef USE_OPENAL

	alSourceStop(sources[stream].sourceId);
#endif

	return;
}

//#pragma optimize( "", off )
void Audio::playTrack(char *filename, bool stereo)
{
	if (g_common.noMusic)
		return;

	if (filename == curTrackFile)
		return;

#ifdef USE_OPENAL
	if (trackPlaying)
	{
		stopSound(trackStreamIdx);
		unloadSound(trackBufferIdx);
	}

	trackBufferIdx = loadSound(filename, stereo, true);
	trackStreamIdx = playSound(trackBufferIdx);
	alSourcei(sources[trackStreamIdx].sourceId, AL_LOOPING, true);
	checkError("alSourcei");

	trackPlaying = true;

	curTrackFile = filename;
#endif

	return;
}

void Audio::setTrackVolume(float gain)
{
#ifdef USE_OPENAL
	alSourcef(sources[trackStreamIdx].sourceId, AL_GAIN, gain);
	checkError("alSourcef");
#endif
}

void Audio::checkError(const char *msg)
{
#ifdef USE_OPENAL
	ALCenum error;

	error = alGetError();

	if (error != AL_NO_ERROR)
	{
		Log((char *)msg);

		switch (error)
		{
		case AL_INVALID_ENUM:
				Log("AL_INVALID_ENUM");
				break;
		case AL_INVALID_VALUE:
				Log("AL_INVALID_VALUE");
				break;
		case AL_INVALID_OPERATION:
				Log("AL_INVALID_OPERATION");
				break;
		case AL_INVALID_NAME:
				Log("AL_INVALID_NAME");
				break;
		case AL_OUT_OF_MEMORY:
				Log("AL_OUT_OF_MEMORY");
				break;
		}
	}
#endif
}
//#pragma optimize( "", on )

void Audio::tick()
{
#ifdef USE_OPENAL
	// Clean up sources
	for (int i = 0; i < MAX_ALSOURCES; i++)
	{
		if (sources[i].used)
		{
			ALint state;

			alGetSourcei(sources[i].sourceId, AL_SOURCE_STATE, &state);

			if (state != AL_PLAYING)
			{
				ALuint sourceid = sources[i].sourceId;
				alDeleteSources(1, &sourceid);
				sources[i].used = false;
			}
		}
	}
#endif
}
