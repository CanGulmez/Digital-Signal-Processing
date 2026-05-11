/**
 ******************************************************************************
 * @file 	main.c
 * @author 	Can Gulmez
 * @brief 	Calculating arrival of angle.
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

void cal_arrival_of_angle(void)
{
	int maxangle;
	DspTime fsample, ssample, tsample;
	DspArrival arrival;
	DspStatus status;

	/* Creating the three samples. */

	status = dsp_signal_cos(10, 100, 1000, 0, 16, &fsample);
	assert(status == DSP_SUCCESS);

	status = dsp_signal_cos(5, 400, 1000, 45, 16, &ssample);
	status = dsp_signal_cos(8, 200, 1000, 90, 16, &tsample);

	/* Create the 'Arrival' object. */

	arrival = (DspArrival) {
		.mics = 3,				/* three microphone */
		.radius = 0.2,			/* 0.2 meter radius */
		.freq = 100,			/* 100 Hz signal frequencies */
		.sources = 1,			/* 1 source */
		.samples = { &fsample, &ssample, &tsample}
	};

	maxangle = dsp_arrival_music(&arrival);

	printf("The direction of angle: %d\n", maxangle);
}
