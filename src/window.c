/**
 ******************************************************************************
 * @file 	window.c
 * @author 	Can Gulmez
 * @brief 	Windowing operations of DSP.
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
 * Apply the Hamming window to `sample` in time domain.
 */
DspStatus dsp_window_hamming(const DspTime *sample, DspTime *res)
{
	int i;

	/* Validate the input parameters. */
	if (!IS_SAMPLE(sample))
		return DSP_ERR_SAMPLE;

	res->length = sample->length;
	for (i = 0; i < res->length; i++)
	{
		res->data[i] = (0.54 - 0.46 * cos((2 * M_PI * i) /
			(res->length - 1))) * sample->data[i];
	}
	return DSP_SUCCESS;
}

/**
 * Apply the Hanning window to `sample` in time domain.
 */
DspStatus dsp_window_hanning(const DspTime *sample, DspTime *res)
{
	int i;

	/* Validate the input parameters. */
	if (!IS_SAMPLE(sample))
		return DSP_ERR_SAMPLE;

	res->length = sample->length;
	for (i = 0; i < res->length; i++)
	{
		res->data[i] = 0.5 * (1.0 - cos((2 * M_PI * i) /
			(res->length - 1))) * sample->data[i];
	}
	return DSP_SUCCESS;
}

/**
 * Apply the Blackman window to `sample` in time domain.
 */
DspStatus dsp_window_blackman(const DspTime *sample, DspTime *res)
{
	int i;

	/* Validate the input parameters. */
	if (!IS_SAMPLE(sample))
		return DSP_ERR_SAMPLE;

	res->length = sample->length;
	for (i = 0; i < res->length; i++)
	{
		res->data[i] = (0.42 - 0.5 * cos((2 * M_PI * i) /
			(res->length - 1)) + 0.08 * cos((4 * M_PI * i) /
			(res->length - 1))) * sample->data[i];
	}
	return DSP_SUCCESS;
}

/**
 * Apply the Chebyshev window to `sample` by attenuating `factor` (dB) 
 * in time domain.
 */
DspStatus dsp_window_chebyshev(const DspTime *sample, int factor, DspTime *res)
{
	int i;
	double epsilon, cosine;

	/* Validate the input parameters. */
	if (!IS_SAMPLE(sample))
		return DSP_ERR_SAMPLE;

	res->length = sample->length;
	epsilon = acosh(pow(10.0, factor / 20.0));
	for (i = 0; i < res->length; i++)
	{
		cosine = M_PI * (2.0 * i - res->length + 1) / (2.0 * res->length);
		res->data[i] = cosh(epsilon * cos(cosine)) * sample->data[i];
	}
	return DSP_SUCCESS;
}

/**
 * Apply the Kaiser window to `sample` by attenuating `factor` (dB)
 * in time domain.
 */
DspStatus dsp_window_kaiser(const DspTime *sample, int factor, DspTime *res)
{
	int i;
	double epsilon, squared;

	/* Validate the input parameters. */
	if (!IS_SAMPLE(sample))
		return DSP_ERR_SAMPLE;

	res->length = sample->length;
	if (factor <= 21)
	{
		epsilon = 0.0;
	}
	else if (factor > 21 && factor < 50)
	{
		epsilon = 0.5842 * pow(factor - 21, 0.4) + 0.07886 * (factor - 21);
	}
	else
	{
		epsilon = 0.1102 * (factor - 8.7);
	}

	for (i = 0; i < res->length; i++)
	{
		squared = pow(2.0 * i / (res->length - 1.0) - 1.0, 2.0);
		res->data[i] = j0(epsilon * sqrt(1.0 - squared)) /
			j0(epsilon) * sample->data[i];
	}
	return DSP_SUCCESS;
}
