/**
 ******************************************************************************
 * @file 	transform.c
 * @author 	Can Gulmez
 * @brief 	Time-frequency transformation operations of DSP.
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
 * Apply the full discrete fourier transformation to `sample`
 * sequence in time-to-frequency domain.
 */
DspStatus dsp_transform_dft(const DspTime *sample, DspFreq *res)
{
	int i, j;
	double real, imag, inner;

	/* Validate the input parameters. */
	if (IS_BAD_SAMPLE(sample))
		return DSP_ERR_BAD_SAMPLE;

	res->length = sample->length;
	inner = 2 * M_PI / res->length;
	for (i = 0; i < res->length; i++)
	{
		real = 0.0;
		imag = 0.0;
		for (j = 0; j < res->length; j++)
		{
			real += sample->data[j] * cos(inner * i * j);
			imag -= sample->data[j] * sin(inner * i * j);
		}
		res->data[i][0] = real;
		res->data[i][1] = imag;
	}
	return DSP_SUCCESS;
}

/**
 * Apply the real-valued discrete fourier transformation to  `sample`
 * sequence in time-to-frequency domain.
 */
DspStatus dsp_transform_dft_real(const DspTime *sample, DspFreq *res)
{
	int i;
	DspFreq transformed;

	/* Validate the input parameters. */
	if (IS_BAD_SAMPLE(sample))
		return DSP_ERR_BAD_SAMPLE;

	dsp_transform_dft(sample, &transformed);
	res->length = (len_t) (transformed.length / 2 + 1);
	for (i = 0; i < res->length; i++)
	{
		res->data[i][0] = transformed.data[i][0];
		res->data[i][1] = transformed.data[i][1];
	}
	return DSP_SUCCESS;
}

/**
 * Apply the inverse discrete fourier transformation to `sample`
 * sequence in frequency-to-time domain.
 */
DspStatus dsp_transform_idft(const DspFreq *sample, DspTime *res)
{
	int i, j;
	double sum = 0.0;
	
	/* Validate the input parameters. */
	if (IS_BAD_SAMPLE(sample))
		return DSP_ERR_BAD_SAMPLE;

	res->length = sample->length;
	for (i = 0; i < res->length; i++)
	{
		for (j = 0; j < res->length; j++)
		{
			sum += sample->data[j][0] * 
					 cos(2 * M_PI * i * j / res->length) - 
					 sample->data[j][1] * 
					 sin(2 * M_PI * i * j / res->length);
		}
		res->data[i] = (double) sum / res->length;
		sum = 0.0;
	}
	return DSP_SUCCESS;
}
