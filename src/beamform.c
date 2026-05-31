/**
 ******************************************************************************
 * @file 	beamform.c
 * @author 	Can Gulmez
 * @brief 	Beamforming operations of DSP.
 * 
 ******************************************************************************
 * @attention
 * 
 * Copyright (c) 2026 Can Gulmez.
 * All rights reserved.
 * 
 * This software is licensed under the MIT License.
 * 
 ******************************************************************************
 */

#include "dsp.h"

/**
 * Apply delay and sum beamforming for uniform circular phased array 
 * microphone system and return the enhanced sample.
 */
DspStatus dsp_beamform_delay_sum(const DspBeamform *beamform, DspTime *res)
{
	int i, j;
	mic_t mics = beamform->mics;
	double radius = beamform->radius;
	double fs = beamform->fs;
	double theta = beamform->theta;
	double angle;
	double delays[mics];
	double maxdelay = 0.0;
	DspTime aligned[mics];

	/* Validate the input parameters. */
	for (i = 0; i < mics; i++)
	{
		if (!IS_SAMPLE(beamform->samples[i]))
			return DSP_ERR_SAMPLE;
	}
	if (mics > MAX_MICS || fs <= 1.0 || radius <= 0.0)
		return DSP_ERR_PARAM;

	/* Calculate the delays between uniform microphones. */
	for (i = 0; i < mics; i++)
	{
		angle = 2.0 * M_PI * i / mics;
		delays[i] = radius * cos(angle - RAD(theta)) / SOUND_SPEED * fs;
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
	for (i = 0; i < mics; i++)
	{
		aligned[i].length = beamform->samples[i]->length;
		dsp_time_delay_lagrange(beamform->samples[i], delays[i], &aligned[i]);
	}
	/* Enhance the all samples into one sample. */
	res->length = aligned[0].length;
	memset(res->data, 0, res->length * sizeof(double));
	for (i = 0; i < mics; i++)
	{
		for (j = 0; j < res->length; j++)
		{
			res->data[j] += aligned[i].data[j];
		}
	}
	return DSP_SUCCESS;
}
