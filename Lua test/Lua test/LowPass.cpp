#include "LowPass.h"
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

FMOD_RESULT F_CALLBACK LowPass_Create_Callback(FMOD_DSP_STATE * dsp_state)
{
	return FMOD_OK;
}

FMOD_RESULT F_CALLBACK LowPass_Read_Callback(FMOD_DSP_STATE *dsp_state, float *inbuffer, float *outbuffer, unsigned int length, int inchannels, int *outchannels)
{
	if (inchannels != *outchannels) {
		throw("Expected same number of channels.");
	}	

	float SR = 44100.0f;
	float f = 5000.0f;
	float b = sqrt(pow(2.0 - cos(2 * M_PI * f / SR), 2) - 1.0) - 2.0 + cos(2 * M_PI * f / SR);
	float a = 1 + b;
	static float lastSampleChannel0 = 0.0f, lastSampleChannel1 = 0.0f;
	for (int i = 0; i < length * inchannels; i += 2) {
		outbuffer[i] = a * inbuffer[i] - b * lastSampleChannel0;
		lastSampleChannel0 = inbuffer[i];

		outbuffer[i + 1] = a * inbuffer[i + 1] - b * lastSampleChannel1;
		lastSampleChannel1 = inbuffer[i + 1];
	}
	return FMOD_OK;

}