#ifndef AUDIO_H
#define AUDIO_H

#include "extra/bass.h"
#include "includes.h"

class Audio
{
public:
	HSAMPLE loadAudio(const char* filename);
	void playAudio(const char* filename);
	void stopAudio();
	
	HSAMPLE curr_sample;
};

#endif