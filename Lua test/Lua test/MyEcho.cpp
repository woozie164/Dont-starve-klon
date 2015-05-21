#include "MyEcho.h"

FMOD_RESULT F_CALLBACK MyEcho_Create_Callback(FMOD_DSP_STATE * dsp_state)
{	
	
	return FMOD_OK;
}

FMOD_RESULT F_CALLBACK MyEcho_Read_Callback(FMOD_DSP_STATE *dsp_state, float *inbuffer, float *outbuffer, unsigned int length, int inchannels, int *outchannels)
{
	return FMOD_OK;
}