#include <fmod_studio.hpp>
#include <fmod_errors.h>
#include <vector>


FMOD_RESULT F_CALLBACK MyEcho_Create_Callback(FMOD_DSP_STATE * dsp_state);
FMOD_RESULT F_CALLBACK MyEcho_Read_Callback(FMOD_DSP_STATE *dsp_state, float *inbuffer, float *outbuffer, unsigned int length, int inchannels, int *outchannels);