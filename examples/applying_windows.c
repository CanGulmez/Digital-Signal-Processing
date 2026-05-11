/**
 ******************************************************************************
 * @file 	main.c
 * @author 	Can Gulmez
 * @brief 	Applying the windows onto the samples.
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

void applying_windows(void)
{
	int i;
	DspTime sample, windowed;
	DspStatus status;

	/* Creating a cosinus sample. */

	status = dsp_signal_cos(10, 100, 1000, 0, 16, &sample);
	assert(status == DSP_SUCCESS);

	printf("The cos() sample:\n");
	for (i = 0; i < sample.length; i++)
	{
		printf("%.2f  ", sample.data[i]);
	}
	printf("\n");

	/* Apply the windows. */

	status = dsp_window_hamming(&sample, &windowed);

	printf("The hamming windowed sample:\n");
	for (i = 0; i < windowed.length; i++)
	{
		printf("%.2f  ", windowed.data[i]);
	}
	printf("\n");

	status = dsp_window_hanning(&sample, &windowed);

	printf("The hanning windowed sample:\n");
	for (i = 0; i < windowed.length; i++)
	{
		printf("%.2f  ", windowed.data[i]);
	}
	printf("\n");

	status = dsp_window_blackman(&sample, &windowed);

	printf("The blackman windowed sample:\n");
	for (i = 0; i < windowed.length; i++)
	{
		printf("%.2f  ", windowed.data[i]);
	}
	printf("\n\n");
}