/**
 ******************************************************************************
 * @file 	main.c
 * @author 	Can Gulmez
 * @brief 	Transforming from time-domain to frequency-domain.
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

void time_to_freq_domain(void)
{
	int i;
	DspTime sample, inversed;
	DspFreq transformed;
	DspStatus status;

	/* Creating a cosinus sample. */

	status = dsp_signal_cos(10, 100, 1000, 0, 1024, &sample);
	assert(status == DSP_SUCCESS);

	printf("The cos() sample:\n");
	for (i = 0; i < sample.length; i++)
	{
		printf("%.2f  ", sample.data[i]);
	}
	printf("\n");

	/* Go to the frequency domain. */

	status = dsp_transform_fft(&sample, &transformed);

	/* Go back to the time domain. */

	status = dsp_transform_ifft(&transformed, &inversed);

	printf("The built cos() sample:\n");
	for (i = 0; i < inversed.length; i++)
	{
		printf("%.2f  ", inversed.data[i]);
	}
	printf("\n\n");
}
