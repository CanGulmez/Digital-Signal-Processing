/**
 ******************************************************************************
 * @file 	signal.c
 * @author 	Can Gulmez
 * @brief 	Signal wave generation operations of DSP.
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
 * Create a normal (Gaussian) noise which has `mean` and `stddev`
 * in time domain.
 */
DspStatus dsp_signal_normal(double mean, double stddev, len_t length, DspTime *res)
{
	int i = 0;
	double u1, u2, s, z0, z1;

	/* Validate the input parameters. */
	if (!IS_LENGTH(length))
		return DSP_ERR_LENGTH;

	res->length = length;
	while (i < length) 
	{
      /* Create two uniform random numbers in (0,1]. */
      u1 = (random() + 1.0) / (RAND_MAX + 2.0);
      u2 = (random() + 1.0) / (RAND_MAX + 2.0);
      /* Box-Muller transform */
      z0 = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
      z1 = sqrt(-2.0 * log(u1)) * sin(2.0 * M_PI * u2);
      
      res->data[i] = mean + z0 * stddev;
      i++;
      if (i < length) 
		{
         res->data[i] = mean + z1 * stddev;
         i++;
      }
   }
	return DSP_SUCCESS;
}

/**
 * Create AWGN (Additive White Gaussian Noise) sample which have
 * signal-to-noise ratio `snr` in time domain.
 */
DspStatus dsp_signal_awgn(const DspTime *sample, double snr, DspTime *res)
{
	DspTime normal;
	double stddev;
	DspStatus status;

	/* Validate the input parameters. */
	if (!IS_SAMPLE(sample))
		return DSP_ERR_SAMPLE;

	if (snr <= 0.0)
		return DSP_ERR_PARAM;

	stddev = sqrt(dsp_time_power(sample) / pow(10.0, snr / 10.0));
	status = dsp_signal_normal(0.0, stddev, sample->length, &normal);
	if (status != DSP_SUCCESS)
		return status;

	status = dsp_time_add(sample, &normal, res);
	if (status != DSP_SUCCESS)
		return status;

	return DSP_SUCCESS;
}

/**
 * Create a sine wave sampled at `fs` frequency  which have amplitude `a`, 
 * frequency `fc` and phase angle `theta` (in degrees) in time domain.
 */
DspStatus dsp_signal_sin(double a, double fc, double fs, double theta, len_t length, 
						  		  DspTime *res)
{
	int i;

	/* Validate the input parameters. */
	if (!IS_LENGTH(length))
		return DSP_ERR_LENGTH;

	if (fc * 2 > fs || fc <= 0.0)
		return DSP_ERR_PARAM;

	res->length = length;
	for (i = 0; i < res->length; i++)
	{
		res->data[i] = a * sin(2 * M_PI * fc * (i / fs) + RAD(theta));
	}
	return DSP_SUCCESS;
}

/**
 * Create a sinc wave sampled at `fs` frequency  which have amplitude `a`, 
 * frequency `fc` and phase angle `theta` (in degrees) in time domain.
 */
DspStatus dsp_signal_sinc(double a, double fc, double fs, len_t length,
									DspTime *res)
{
	int i;

	/* Validate the input parameters. */
	if (!IS_LENGTH(length))
		return DSP_ERR_LENGTH;

	if (fc * 2 > fs || fc <= 0.0)
		return DSP_ERR_PARAM;

	res->length = length;
	for (i = 0; i < res->length; i++)
	{
		if (i == 0)
		{
			res->data[i] = a;
		}
		else
		{
			res->data[i] = a * sin(2 * M_PI * fc * (i / fs)) /
				(2 * M_PI * fc * (i / fs));
		}
	}
	return DSP_SUCCESS;
}

/**
 * Create a cosine wave sampled at `fs` frequency  which have amplitude `a`, 
 * frequency `fc` and phase angle `theta` (in degrees) in time domain.
 */
DspStatus dsp_signal_cos(double a, double fc, double fs, double theta, len_t length, 
						  		  DspTime *res)
{
	int i;

	/* Validate the input parameters. */
	if (!IS_LENGTH(length))
		return DSP_ERR_LENGTH;

	if (fc * 2 > fs || fc <= 0.0)
		return DSP_ERR_PARAM;

	res->length = length;
	for (i = 0; i < res->length; i++)
	{
		res->data[i] = a * cos(2 * M_PI * fc * (i / fs) + RAD(theta));
	}
	return DSP_SUCCESS;
}

/**
 * Create a impulse where `index`.th is one and others are zeros in time domain.
 */
DspStatus dsp_signal_impulse(double a, index_t index, len_t length, DspTime *res)
{
	int i;

	/* Validate the input parameters. */
	if (!IS_LENGTH(length))
		return DSP_ERR_LENGTH;

	if (index < 0 || index >= length)
		return DSP_ERR_PARAM;

	res->length = length;
	for (i = 0; i < res->length; i++)
	{
		if (i == index)
		{
			res->data[i] = a;
		}
		else
		{
			res->data[i] = 0.0;
		}
	}
	return DSP_SUCCESS;
}

/**
 * Create a step where between of `findex` and `sindex` are `a`, others are zeros
 * in time domain.
 */
DspStatus dsp_signal_step(double a, index_t findex, index_t sindex, len_t length, 
									DspTime *res)
{
	int i;

	/* Validate the input parameters. */
	if (!IS_LENGTH(length))
		return DSP_ERR_LENGTH;

	if (findex < 0 || sindex < 0 || findex > sindex)
		return DSP_ERR_PARAM;

	res->length = length;
	for (i = 0; i < res->length; i++)
	{
		if (i >= findex && i <= sindex)
		{
			res->data[i] = a;
		}
		else
		{
			res->data[i] = 0.0;
		}
	}
	return DSP_SUCCESS;
}

/**
 * Create a square wave sampled at `fs` which have amplitude `a`, and 
 * frequency `fc` in time domain.
 */
DspStatus dsp_signal_square(double a, double fc, double fs, len_t length, 
							 		  DspTime *res)
{
	int i;
	DspTime squared;
	DspStatus status;

	/* Validate the input parameters. */
	if (!IS_LENGTH(length))
		return DSP_ERR_LENGTH;

	if (fc * 2 > fs || fc <= 0.0)
		return DSP_ERR_PARAM;

	status = dsp_signal_sin(a, fc, fs, 0.0, length, &squared);
	if (status != DSP_SUCCESS)
		return status;

	res->length = length;
	for (i = 0; i < squared.length; i++)
	{
		if (squared.data[i] >= 0)
		{
			res->data[i] = a;
		}
		else
		{
			res->data[i] = -a;
		}
	}
	return DSP_SUCCESS;
}

/**
 * Create a sawtooth sampled at `fs` which have amplitude `a` and 
 * frequency `fc` in time domain.
 */
DspStatus dsp_signal_sawtooth(double a, double fc, double fs, len_t length, 
										 DspTime *res)
{
	int i;

	/* Validate the input parameters. */
	if (!IS_LENGTH(length))
		return DSP_ERR_LENGTH;

	if (fc * 2 > fs || fc <= 0.0)
		return DSP_ERR_PARAM;

	res->length = length;
	for (i = 0; i < res->length; i++)
	{
		res->data[i] = 2 * a * (fmod(i * fc / fs, 1.0) - 0.5);
	}
	return DSP_SUCCESS;
}

/**
 * Create a triangle sampled at `fs` which have amplitude `a` and 
 * frequency `fc` in time domain.
 */
DspStatus dsp_signal_triangle(double A, double fc, double fs, len_t length,
										 DspTime *res)
{
	int i;
	double t, sawtooth;

	/* Validate the input parameters. */
	if (!IS_LENGTH(length))
		return DSP_ERR_LENGTH;

	if (fc * 2 > fs || fc <= 0.0)
		return DSP_ERR_PARAM;

	res->length = length;
	for (i = 0; i < res->length; i++)
	{
      t = (double)i / fs;
      sawtooth = 2.0 * (t * fc - floor(0.5 + t * fc));
      res->data[i] = 2.0 * A * fabs(sawtooth) - A;
	}
	return DSP_SUCCESS;
}
