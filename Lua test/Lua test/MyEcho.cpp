#include "MyEcho.h"
#include <vector>

using namespace std;

int i = 0;
vector<float> buffer;
FMOD_RESULT F_CALLBACK MyEcho_Create_Callback(FMOD_DSP_STATE * dsp_state)
{	
	int sampleRate = 44100;
	float duration = 0.5f; // 0.5 seconds = 500 milliseconds
	buffer.resize(sampleRate * duration);
	return FMOD_OK;
}

FMOD_RESULT F_CALLBACK MyEcho_Read_Callback(FMOD_DSP_STATE *dsp_state, float *inbuffer, float *outbuffer, unsigned int length, int inchannels, int *outchannels)
{
	// I'm currently assuming the same number of input and output channels.
	// Not sure if the code work otherwise.
	if (inchannels != *outchannels) {
		throw("Expected same number of channels.");		
	}
	int sampleRate = 44100;
	float duration = 0.5f; // 0.5 seconds = 500 milliseconds
	// Calculate the index of the delay sample
	
	int i_delay = i - (sampleRate * duration);

	// Let the index wrap around, since this is a circular buffer
	i_delay %= buffer.size();
	
	int j = 0;
	for (; i < length + i; i++) {
		i %= buffer.size();
		// Save the input samples to the delay buffer
		buffer[i] = inbuffer[j];

		// Calculate the output
		outbuffer[j] = buffer[i] + buffer[i_delay];

		++i_delay;
		i_delay %= buffer.size();
		j++;
	}
	
	
	/*
	for (unsigned int j = 0; j < length; j++)
	{
		outbuffer[j] = 0.0f;
	}
	*/
	return FMOD_OK;
}