/**
 ******************************************************************************
 * @file 	window.c
 * @author 	Ahmet Can GULMEZ
 * @brief 	Windowing operations of DSP.
 * 
 ******************************************************************************
 * @attention
 * 
 * Copyright (c) 2026 Ahmet Can GULMEZ.
 * All rights reserved.
 * 
 * This software is licensed under the MIT License.
 * 
 ******************************************************************************
 */

#include "./dsp.h"

/**
 * Apply the Hamming window to `sample` sequence in time domain.
 */
void dsp_window_hamming(const DspTime *sample, DspTime *result)
{
	int i;

	/* Validate the inputs. */
	assert_sample(sample);

	result->length = sample->length;
	for (i = 0; i < result->length; i++)
	{
		result->data[i] = (0.54 - 0.46 * cos((2 * M_PI * i) /
			(result->length - 1))) * sample->data[i];
	}
}

/**
 * Apply the Hanning window to `sample` sequence in time domain.
 */
void dsp_window_hanning(const DspTime *sample, DspTime *result)
{
	int i;

	/* Validate the inputs. */
	assert_sample(sample);

	result->length = sample->length;
	for (i = 0; i < result->length; i++)
	{
		result->data[i] = 0.5 * (1.0 - cos((2 * M_PI * i) /
			(result->length - 1))) * sample->data[i];
	}
}

/**
 * Apply the Blackman window to `sample` sequence in time domain.
 */
void dsp_window_blackman(const DspTime *sample, DspTime *result)
{
	int i;

	/* Validate the inputs. */
	assert_sample(sample);

	result->length = sample->length;
	for (i = 0; i < result->length; i++)
	{
		result->data[i] = (0.42 - 0.5 * cos((2 * M_PI * i) /
			(result->length - 1)) + 0.08 * cos((4 * M_PI * i) /
			(result->length - 1))) * sample->data[i];
	}
}

/**
 * Apply the Chebyshev window to `sample` sequence by attenuating `factor` (dB)
 * in time domain.
 */
void dsp_window_chebyshev(const DspTime *sample, int factor, DspTime *result)
{
	int i;
	double epsilon, cosine;

	/* Validate the inputs. */
	assert_sample(sample);

	result->length = sample->length;
	epsilon = acosh(pow(10.0, factor / 20.0));
	for (i = 0; i < result->length; i++)
	{
		cosine = M_PI * (2.0 * i - result->length + 1) / (2.0 * result->length);
		result->data[i] = cosh(epsilon * cos(cosine)) * sample->data[i];
	}
}

/**
 * Apply the Kaiser window to `sample` sequence by attenuating `factor` (dB)
 * in time domain.
 */
void dsp_window_kaiser(const DspTime *sample, int factor, DspTime *result)
{
	int i;
	double epsilon, squared;

	/* Validate the inputs. */
	assert_sample(sample);

	result->length = sample->length;
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

	for (i = 0; i < result->length; i++)
	{
		squared = pow(2.0 * i / (result->length - 1.0) - 1.0, 2.0);
		result->data[i] = j0(epsilon * sqrt(1.0 - squared)) /
			j0(epsilon) * sample->data[i];
	}
}
