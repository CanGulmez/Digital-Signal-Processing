/**
 ******************************************************************************
 * @file 	transform.c
 * @author 	Can GULMEZ
 * @brief 	Time-frequency transformation operations of DSP.
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
 * Apply the full discrete fourier transformation to `sample`
 * sequence in time-to-frequency domain.
 */
void dsp_transform_dft(const DspTime *sample, DspFreq *result)
{
	int i, j;
	double real, imag, inner;

	/* Validate the inputs. */
	assert_sample(sample);

	result->length = sample->length;
	inner = 2 * M_PI / result->length;
	for (i = 0; i < result->length; i++)
	{
		real = 0.0;
		imag = 0.0;
		for (j = 0; j < result->length; j++)
		{
			real += sample->data[j] * cos(inner * i * j);
			imag -= sample->data[j] * sin(inner * i * j);
		}
		result->data[i][0] = real;
		result->data[i][1] = imag;
	}
}

/**
 * Apply the real-valued discrete fourier transformation to  `sample`
 * sequence in time-to-frequency domain.
 */
void dsp_transform_dft_real(const DspTime *sample, DspFreq *result)
{
	int i;
	DspFreq transformed;

	/* Validate the inputs. */
	assert_sample(sample);

	dsp_transform_dft(sample, &transformed);
	result->length = (len_t) (transformed.length / 2 + 1);
	for (i = 0; i < result->length; i++)
	{
		result->data[i][0] = transformed.data[i][0];
		result->data[i][1] = transformed.data[i][1];
	}
}

/**
 * Apply the inverse discrete fourier transformation to `sample`
 * sequence in frequency-to-time domain.
 */
void dsp_transform_idft(const DspFreq *sample, DspTime *result)
{
	int i, j;
	double sum;
	
	/* Validate the inputs */
	assert_sample(sample);

	sum = 0;
	result->length = sample->length;
	for (i = 0; i < result->length; i++)
	{
		for (j = 0; j < result->length; j++)
		{
			sum += sample->data[j][0] * 
					 cos(2 * M_PI * i * j / result->length) - 
					 sample->data[j][1] * 
					 sin(2 * M_PI * i * j / result->length);
		}
		result->data[i] = (double) sum / result->length;
		sum = 0;
	}
}
