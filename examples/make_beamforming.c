/**
 ******************************************************************************
 * @file 	main.c
 * @author 	Can Gulmez
 * @brief 	Making beamforming.
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

#include "main.h"

void make_beamforming(void)
{
	int i;
	DspTime fsample, ssample, tsample, beamformed;
	DspBeamform beamform;
	DspStatus status;

	/* Creating the three samples. */

	status = dsp_signal_cos(10, 100, 1000, 0, 16, &fsample);
	assert(status == DSP_SUCCESS);

	status = dsp_signal_cos(5, 400, 1000, 45, 16, &ssample);
	status = dsp_signal_cos(8, 200, 1000, 90, 16, &tsample);

	/* Create the 'Beamform' object. */

	beamform = (DspBeamform) {
		.mics = 3,				/* three microphone */
		.radius = 0.2,			/* 0.2 meter radius */
		.theta = 45,			/* 45 degrees coming direction */
		.fs = 12000,			/* 12 kHz sampling frequencies */
		.samples = { &fsample, &ssample, &tsample}
	};

	status = dsp_beamform_delay_sum(&beamform, &beamformed);

	printf("The beamformed sample:\n");
	for (i = 0; i < beamformed.length; i++)
	{
		printf("%.2f  ", beamformed.data[i]);
	}
	printf("\n\n");
}
