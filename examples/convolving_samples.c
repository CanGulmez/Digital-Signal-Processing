/**
 ******************************************************************************
 * @file 	main.c
 * @author 	Can Gulmez
 * @brief 	Convolving the samples.
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

void convolving_samples(void)
{
	int i;
	DspTime fsample, ssample, res;
	DspStatus status;

	/* Create two samples. */

	status = dsp_signal_normal(5.0, 10.0, 16, &fsample);
	assert(status == DSP_SUCCESS);

	status = dsp_signal_normal(7.0, 1.0, 16, &ssample);
	assert(status == DSP_SUCCESS);

	/* Convolve them... */

	status = dsp_time_convolve(&fsample, &ssample, &res);

	printf("Convolved sample:\n");
	for (i = 0; i < res.length; i++)
	{
		printf("%.2f ", res.data[i]);
	}
	printf("\n\n");
}
