/**
 ******************************************************************************
 * @file 	beamform.c
 * @author 	Can GULMEZ
 * @brief 	Beamforming operations of DSP.
 * 
 ******************************************************************************
 * @attention
 * 
 * Copyright (c) 2026 Can GULMEZ.
 * All rights reserved.
 * 
 * This software is licensed under the MIT License.
 * 
 ******************************************************************************
 */

#include "./dsp.h"

/**
 * Apply delay and sum beamforming for uniform circular phased array 
 * microphone system and return the enhanced sample.
 */
void dsp_beamform_delay_sum(const DspBeamform *beamform, DspTime *result)
{
	int i, j;
	double angle, delays[beamform->mics], maxdelay = 0.0;
	DspTime aligned[MAX_MICS];
	
	/* Validate the inputs. */
	assert_beamform(beamform);

	/* Calculate the delays between uniform microphones. */
	for (i = 0; i < beamform->mics; i++)
	{
		angle = 2.0 * M_PI * i / beamform->mics;
		delays[i] = beamform->radius * cos(angle - RAD(beamform->theta)) / 
			SOUND_SPEED * beamform->fs;
		if (delays[i] > maxdelay)
		{
			maxdelay = delays[i];
		}
	}

	/* Make all delays positive (causal system). */
	for (i = 0; i < beamform->mics; i++)
	{
		delays[i] = maxdelay - delays[i];
	}

	/* Align the beamform according to delays. */
	for (i = 0; i < beamform->mics; i++)
	{
		aligned[i].length = beamform->samples[i]->length;
		dsp_time_delay_lagrange(beamform->samples[i], delays[i],
			&aligned[i]);
	}

	/* Enhance the all samples into one sample. */
	result->length = aligned[0].length;
	memset(result->data, 0, result->length * sizeof(double));
	for (i = 0; i < beamform->mics; i++)
	{
		for (j = 0; j < result->length; j++)
		{
			result->data[j] += aligned[i].data[j];
		}
	}
}

/**
 * Apply the minimum variance distortionless response (MVDR) beamforming 
 * technique to `beamform`. `tetha` (degrees) refers to the arrival angle 
 * toward the phased array plane. 
 */
void dsp_beamform_mvdr(const DspBeamform *beamform, double tetha, DspTime *result)
{

}
