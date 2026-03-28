/**
 ******************************************************************************
 * @file 	signal.c
 * @author 	Can GULMEZ
 * @brief 	Signal wave operations of DSP.
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
 * Generate a normal (Gaussian) noise sequence which has `mean` and `stddev`
 * in time domain.
 */
void dsp_signal_normal(double mean, double stddev, len_t length, DspTime *result)
{
	int i;
	double u1, u2, s, z0, z1;

	/* Validate the inputs. */
	assert_length(length);

	i = 0;
	srandom(time(NULL));		/* set random seed simultaneously */
	result->length = length;
	while (i < length) 
	{
      /* Generate two uniform random numbers in (0,1]. */
      u1 = (random() + 1.0) / (RAND_MAX + 2.0);
      u2 = (random() + 1.0) / (RAND_MAX + 2.0);
      /* Box-Muller transform */
      z0 = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
      z1 = sqrt(-2.0 * log(u1)) * sin(2.0 * M_PI * u2);
      
      result->data[i] = mean + z0 * stddev;
      i++;
      if (i < length) 
		{
         result->data[i] = mean + z1 * stddev;
         i++;
      }
   }
}

/**
 * Generate AWGN (Additive White Gaussian Noise) sample sequence which have
 * signal-to-noise ratio `snr` in time domain.
 */
void dsp_signal_awgn(const DspTime *sample, double snr, DspTime *result)
{
	DspTime normal;
	double power, stddev;

	/* Validate the inputs. */
	assert_sample(sample);
	assert(snr > 0);

	power = dsp_time_power(sample);
	stddev = sqrt(power / pow(10.0, snr / 10.0));
	dsp_signal_normal(0.0, stddev, sample->length, &normal);
	dsp_time_add(sample, &normal, result);
}

/**
 * Generate a sine wave sequence sampled at `fs` frequency  which have
 * amplitude `A`, frequency `fc` and phase angle `theta` (in degrees) in
 * time domain.
 */
void dsp_signal_sin(double A, double fc, double fs, double theta, len_t length, 
						  DspTime *result)
{
	int i;

	/* Validate the inputs. */
	assert_length(length);
	assert((fs > 2 * fc) && (fc > 0));

	result->length = length;
	for (i = 0; i < result->length; i++)
	{
		result->data[i] = A * sin(2 * M_PI * fc * (i / fs) + RAD(theta));
	}
}

/**
 * Generate a sinc wave sequence sampled at `fs` frequency  which have
 * amplitude `A`, frequency `fc` and phase angle `theta` (in degrees) in
 * time domain.
 */
void dsp_signal_sinc(double A, double fc, double fs, double theta, len_t length,
							DspTime *result)
{
	int i;

	/* Validate the inputs. */
	assert_length(length);
	assert((fs > 2 * fc) && (fc > 0));

	result->length = length;
	for (i = 0; i < result->length; i++)
	{
		if (i == 0)
		{
			result->data[i] = A * 1.0;
		}
		else
		{
			result->data[i] = A * sin(2 * M_PI * fc * (i / fs) + RAD(theta)) /
				(2 * M_PI * fc * (i / fs) + RAD(theta));
		}
	}
}

/**
 * Generate a cosine wave sequence sampled at `fs` frequency  which have
 * amplitude `A`, frequency `fc` and phase angle `theta` (in degrees) in
 * time domain.
 */
void dsp_signal_cos(double A, double fc, double fs, double theta, len_t length, 
						  DspTime *result)
{
	int i;

	/* Validate the inputs. */
	assert_length(length);
	assert((fs > 2 * fc) && (fc > 0));

	result->length = length;
	for (i = 0; i < result->length; i++)
	{
		result->data[i] = A * cos(2 * M_PI * fc * (i / fs) + RAD(theta));
	}
}

/**
 * Generate a impulse sequence where `index`.th is one and others 
 * are zeros in time domain.
 */
void dsp_signal_impulse(int index, len_t length, DspTime *result)
{
	int i;

	/* Validate the inputs. */
	assert_length(length);
	assert(index >= 0 && index < length);

	result->length = length;
	for (i = 0; i < result->length; i++)
	{
		if (i == index)
		{
			result->data[i] = 1.0;
		}
		else
		{
			result->data[i] = 0.0;
		}
	}
}

/**
 * Generate a step sequence where between of `findex` and `sindex`
 * are ones, others are `A` in time domain.
 */
void dsp_signal_step(double A, int findex, int sindex, len_t length, 
							DspTime *result)
{
	int i;

	/* Validate the inputs. */
	assert_length(length);
	assert(findex >= 0 && sindex < length && findex <= sindex);

	result->length = length;
	for (i = 0; i < result->length; i++)
	{
		if (i >= findex && i <= sindex)
		{
			result->data[i] = A;
		}
		else
		{
			result->data[i] = 0.0;
		}
	}
}

/**
 * Generate a square wave sequence sampled at `fs` which have
 * amplitude `A`, and frequency `fc` in time domain.
 */
void dsp_signal_square(double A, double fc, double fs, len_t length, 
							  DspTime *result)
{
	int i;
	DspTime squared;

	/* Validate the inputs. */
	assert_length(length);
	assert((fs > 2 * fc) && (fc > 0));

	result->length = length;
	dsp_signal_sin(A, fc, fs, 0.0, length, &squared);
	for (i = 0; i < squared.length; i++)
	{
		if (squared.data[i] >= 0)
		{
			result->data[i] = A;
		}
		else
		{
			result->data[i] = -A;
		}
	}
}

/**
 * Generate a sawtooth sequence sampled at `fs` which have amplitude
 * `A` and frequency `fc` in time domain.
 */
void dsp_signal_sawtooth(double A, double fc, double fs, len_t length, 
								 DspTime *result)
{
	int i;

	/* Validate the inputs. */
	assert_length(length);
	assert((fs > 2 * fc) && (fc > 0));

	result->length = length;
	for (i = 0; i < result->length; i++)
	{
		result->data[i] = 2 * A * (fmod(i * fc / fs, 1.0) - 0.5);
	}
}

/**
 * Generate a triangle sequence sampled at `fs` which have amplitude
 * `A` and frequency `fc` in time domain.
 */
void dsp_signal_triangle(double A, double fc, double fs, len_t length,
								 DspTime *result)
{
	int i;
	double t, sawtooth;

	/* Validate the inputs. */
	assert_length(length);
	assert((fs > 2 * fc) && (fc > 0));

	result->length = length;
	for (i = 0; i < result->length; i++)
	{
      t = (double)i / fs;
      sawtooth = 2.0 * (t * fc - floor(0.5 + t * fc));
      result->data[i] = 2.0 * A * fabs(sawtooth) - A;
	}
}
