#include "MyEcho.h"
#include <vector>
#include <iostream>

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
	if (inchannels != *outchannels) {
		throw("Expected same number of channels.");		
	}
	int sampleRate = 44100;
	float duration = 0.5f; // 0.5 seconds = 500 milliseconds

	// Calculate the index of the delay sample	
	int i_delay = i - (sampleRate * duration);	
	float maxAmp = 0.0f;
	//i < length + i
	for (int j = 0; j < length; i++, j++, i_delay++) {
		// Let the index wrap around, since this is a circular buffer
		i %= buffer.size();
		i_delay %= buffer.size();

		if (abs(inbuffer[j]) > maxAmp) {
			maxAmp = abs(inbuffer[j]);
		}
		
		// Save the input samples to the delay buffer
		buffer[i] = inbuffer[j];

		// Calculate the output
		outbuffer[j] = buffer[i] + buffer[i_delay] / 2.0f;
	
		//outbuffer[j] = buffer[i];	

		// Normalize the output
		//outbuffer[j] *= inbuffer[j] / outbuffer[j];
	}
	
	// Normalize
	//float refValue = 1.0f;
	float refValue = maxAmp;
	for (int j = 0; j < length; j++) {
		//outbuffer[j] *= maxAmp / outbuffer[j];
		outbuffer[j] = outbuffer[j] / maxAmp * refValue;
	}
	
	return FMOD_OK;
}