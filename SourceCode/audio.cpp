#include "audio.h"

HSAMPLE Audio::loadAudio(const char* filename)
{
	// Cargamos el archivo de sonido en el sample
	HSAMPLE hSample = BASS_SampleLoad(false, filename, 0, 0, 3, BASS_SAMPLE_LOOP);
	if (hSample == 0)
		//file not found
		std::cout << "Could not find the sound file" << " \n";
	
	curr_sample = hSample;
	return hSample;
}

void Audio::playAudio(const char* filename)
{
	HSAMPLE hSample = loadAudio(filename);
	HCHANNEL hSampleChannel;

	// Set the volume level
	BASS_SetConfig(BASS_CONFIG_GVOL_SAMPLE, 400);

	//Creamos un canal para el sample
	hSampleChannel = BASS_SampleGetChannel(hSample, false);

	//Lanzamos un sample
	BASS_ChannelPlay(hSampleChannel, true);
}

void Audio::stopAudio()
{
	// Liberamos de la memoria el sample
	BASS_SampleFree(curr_sample);
}