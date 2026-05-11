/**
 ******************************************************************************
 * @file 	main.c
 * @author 	Can Gulmez
 * @brief 	Extracting frequency-domain features.
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

void extracting_freq_features(void)
{
	int i;
	DspTime sample, feature;
	DspFreq transformed;
	DspStatus status;

	/* Creating a sinus sample. */

	status = dsp_signal_cos(5, 100, 1000, 0, 16, &sample);
	assert(status == DSP_SUCCESS);

	printf("The cos() sample:\n");
	for (i = 0; i < sample.length; i++)
	{
		printf("%.2f  ", sample.data[i]);
	}
	printf("\n");

	/* Go to the frequency domain. */

	status = dsp_transform_dft(&sample, &transformed);

	/* Extract the some features. */

	status = dsp_freq_real(&transformed, &feature); /* real-part */

	printf("The real-part of the sample:\n");
	for (i = 0; i < feature.length; i++)
	{
		printf("%.2f  ", feature.data[i]);
	}
	printf("\n");

	status = dsp_freq_imag(&transformed, &feature); /* imaginary-part */

	printf("The imaginary-part of the sample:\n");
	for (i = 0; i < feature.length; i++)
	{
		printf("%.2f  ", feature.data[i]);
	}
	printf("\n");

	status = dsp_freq_magnitude(&transformed, &feature); /* magnitude-part */

	printf("The magnitude-part of the sample:\n");
	for (i = 0; i < feature.length; i++)
	{
		printf("%.2f  ", feature.data[i]);
	}
	printf("\n");

	status = dsp_freq_phase(&transformed, &feature); /* real-part */

	printf("The phase-part (degrees) of the sample:\n");
	for (i = 0; i < feature.length; i++)
	{
		printf("%.2f  ", feature.data[i]);
	}
	printf("\n\n");
}
