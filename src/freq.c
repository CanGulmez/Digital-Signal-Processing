/**
 ******************************************************************************
 * @file 	freq.c
 * @author 	Can Gulmez
 * @brief 	Core frequency domain operations of DSP.
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

#include "dsp.h"

/**
 * Add `fsample` and `ssample` with each other in time domain.
 */
DspStatus dsp_freq_add(const DspFreq *fsample, const DspFreq *ssample, 
							  DspFreq *res)
{
	int i;

	/* Validate the input parameters. */
	if (!IS_SAMPLE(fsample) || !IS_SAMPLE(ssample))
		return DSP_ERR_SAMPLE;

	if (IS_MISMATCH(fsample, ssample))
		return DSP_ERR_MISMATCH;

	res->length = fsample->length;
	for (i = 0; i < res->length; i++)
	{
		res->data[i][0] = fsample->data[i][0] + ssample->data[i][0];
		res->data[i][1] = fsample->data[i][1] + ssample->data[i][1];
	}
	return DSP_SUCCESS;
}

/**
 * Subtract `ssample` from `fsample` in frequency domain.
 */
DspStatus dsp_freq_sub(const DspFreq *fsample, const DspFreq *ssample, 
							  DspFreq *res)
{
	int i;

	/* Validate the input parameters. */
	if (!IS_SAMPLE(fsample) || !IS_SAMPLE(ssample))
		return DSP_ERR_SAMPLE;

	if (IS_MISMATCH(fsample, ssample))
		return DSP_ERR_MISMATCH;

	res->length = fsample->length;
	for (i = 0; i < res->length; i++)
	{
		res->data[i][0] = fsample->data[i][0] - ssample->data[i][0];
		res->data[i][1] = fsample->data[i][1] - ssample->data[i][1];
	}
	return DSP_SUCCESS;
}

/**
 * Multiply `sample` with `scalar` in frequency domain.
 */
DspStatus dsp_freq_scalar_mul(const DspFreq *sample, double scalar, DspFreq *res)
{
	int i;

	/* Validate the input parameters. */
	if (!IS_SAMPLE(sample))
		return DSP_ERR_SAMPLE;

	res->length = sample->length;
	for (i = 0; i < res->length; i++)
	{
		res->data[i][0] = sample->data[i][0] * scalar;
		res->data[i][1] = sample->data[i][1] * scalar;
	}
	return DSP_SUCCESS;
}

/**
 * Multiply the `fsample` and `ssample` in frequency domain.
 */
DspStatus dsp_freq_dot_mul(const DspFreq *fsample, const DspFreq *ssample, 
							 DspFreq *res)
{
	int i;

	/* Validate the input parameters. */
	if (!IS_SAMPLE(fsample) || !IS_SAMPLE(ssample))
		return DSP_ERR_SAMPLE;

	if (IS_MISMATCH(fsample, ssample))
		return DSP_ERR_MISMATCH;

	res->length = fsample->length;
	for (i = 0; i < res->length; i++)
	{
		res->data[i][0] = fsample->data[i][0] * ssample->data[i][0] - 
         fsample->data[i][1] * ssample->data[i][1];
      res->data[i][1] = fsample->data[i][0] * ssample->data[i][1] +
         fsample->data[i][1] * ssample->data[i][0]; 
	}
	return DSP_SUCCESS;
}

/**
 * Divide the `sample` by `scalar` in frequency domain.
 */
DspStatus dsp_freq_scalar_div(const DspFreq *sample, double scalar, DspFreq *res)
{
	if (scalar == 0.0)
		return DSP_ERR_PARAM;

	return dsp_freq_scalar_mul(sample, (1.0 / scalar), res);
}

/**
 * Resize the `sample`. If `sample`'s length is bigger, then fill the zeros 
 * in frequency domain.
 */
DspStatus dsp_freq_resize(const DspFreq *sample, len_t length, DspFreq *res)
{
	int i, j;

	/* Validate the input parameters. */
	if (!IS_SAMPLE(sample))
		return DSP_ERR_SAMPLE;

	if (!IS_LENGTH(length))
		return DSP_ERR_LENGTH;;

	res->length = length;
	for (i = 0; i < res->length; i++)
	{
		res->data[i][0] = (i >= sample->length) ? 0.0 : sample->data[i][0];
		res->data[i][1] = (i >= sample->length) ? 0.0 : sample->data[i][1];
	}
	return DSP_SUCCESS;
}

/**
 * Insert the `real + imag*i` at `index`. of `sample` in frequency domain.
 */
DspStatus dsp_freq_insert(const DspFreq *sample, index_t index, double real, 
								  double imag, DspFreq *res)
{
	int i;

	/* Validate the input parameters. */
	if (!IS_SAMPLE(sample))
		return DSP_ERR_SAMPLE;

	if (!IS_INDEX(sample, index))
		return DSP_ERR_INDEX;

	res->length = sample->length + 1;
	if (!IS_LENGTH(res->length))
		return DSP_ERR_LENGTH;

	for (i = 0; i < index; i++)
	{
		res->data[i][0] = sample->data[i][0];
		res->data[i][1] = sample->data[i][1];
	}
	res->data[index][0] = real;
	res->data[index][1] = imag;
	for (i = index; i < sample->length; i++)
	{
		res->data[i + 1][0] = sample->data[i][0];
		res->data[i + 1][1] = sample->data[i][1];
	}
	return DSP_SUCCESS;
}

/**
 * Remove the `index`. data of `sample` in frequency domain.
 */
DspStatus dsp_freq_remove(const DspFreq *sample, index_t index, DspFreq *res)
{
	int i;

	/* Validate the input parameters. */
	if (!IS_SAMPLE(sample))
		return DSP_ERR_SAMPLE;

	if (!IS_INDEX(sample, index))
		return DSP_ERR_INDEX;

	res->length = sample->length - 1;
	for (i = 0; i < index; i++)
	{
		res->data[i][0] = sample->data[i][0];
		res->data[i][1] = sample->data[i][1];
	}
	for (i = index; i < res->length; i++)
	{
		res->data[i][0] = sample->data[i + 1][0];
		res->data[i][1] = sample->data[i + 1][1];
	}
	return DSP_SUCCESS;
}

/**
 * Append the `real + imag*i` data at the end of `sample` in frequency domain.
 */
DspStatus dsp_freq_append(const DspFreq *sample, double real, double imag,
								  DspFreq *res)
{
	return dsp_freq_insert(sample, sample->length, real, imag, res);
}

/**
 * Pop the last data of `sample` in frequency domain.
 */
DspStatus dsp_freq_pop(const DspFreq *sample, DspFreq *res)
{
	return dsp_freq_remove(sample, sample->length - 1, res);
}

/**
 * Concatenate the `fsample` and `ssample` in frequency domain.
 */
DspStatus dsp_freq_concat(const DspFreq *fsample, const DspFreq *ssample,
							DspFreq *res)
{
	int i;

	/* Validate the input parameters. */
	if (!IS_SAMPLE(fsample) || !IS_SAMPLE(ssample))
		return DSP_ERR_SAMPLE;

	res->length = fsample->length + ssample->length;
	if (!IS_LENGTH(res->length))
		return DSP_ERR_LENGTH;
		
	memcpy(res->data, fsample->data, sizeof(double) * 2 * fsample->length);
	for (i = fsample->length; i < res->length; i++)
	{
		res->data[i][0] = ssample->data[i - fsample->length][0];
		res->data[i][1] = ssample->data[i - fsample->length][1];
	}
	return DSP_SUCCESS;
}

/**
 * Extract the real part of `sample`.
 */
DspStatus dsp_freq_real(const DspFreq *sample, DspTime *res)
{
	int i;

	/* Validate the input parameters. */
	if (!IS_SAMPLE(sample))
		return DSP_ERR_SAMPLE;

	res->length = sample->length;
	for (i = 0; i < res->length; i++)
	{
		res->data[i] = sample->data[i][0];
	}
	return DSP_SUCCESS;
}

/**
 * Extract the imaginary part of `sample`.
 */
DspStatus dsp_freq_imag(const DspFreq *sample, DspTime *res)
{
	int i;

	/* Validate the input parameters. */
	if (!IS_SAMPLE(sample))
		return DSP_ERR_SAMPLE;

	res->length = sample->length;
	for (i = 0; i < res->length; i++)
	{
		res->data[i] = sample->data[i][1];
	}
	return DSP_SUCCESS;
}

/**
 * Extract the magnitude part of `sample`.
 */
DspStatus dsp_freq_magnitude(const DspFreq *sample, DspTime *res)
{
	int i;

	/* Validate the input parameters. */
	if (!IS_SAMPLE(sample))
		return DSP_ERR_SAMPLE;

	res->length = sample->length;
	for (i = 0; i < res->length; i++)
	{
		res->data[i] = sqrt(pow(sample->data[i][0], 2.0) +
									  pow(sample->data[i][1], 2.0));
	}
	return DSP_SUCCESS;
}

/**
 * Extract the phase part in degrees of `sample`.
 */
DspStatus dsp_freq_phase(const DspFreq *sample, DspTime *res)
{
	int i;

	/* Validate the input parameters. */
	if (!IS_SAMPLE(sample))
		return DSP_ERR_SAMPLE;

	res->length = sample->length;
	for (i = 0; i < res->length; i++)
	{
		res->data[i] = DEG(
			atan2(sample->data[i][1], sample->data[i][0]));
	}
	return DSP_SUCCESS;
}

/**
 * Calculate the power spectral density of `index`.th of `sample`.
 */
double dsp_freq_psd(const DspFreq *sample, index_t index)
{
	if (!IS_SAMPLE(sample) || !IS_INDEX(sample, index))
      return 0.0;

	return (pow(sample->data[index][0], 2.0) +
		pow(sample->data[index][1], 2.0)) / (double) sample->length;
}

/**
 * Calculate the average power of `sample`.
 */
double dsp_freq_power(const DspFreq *sample)
{
	DspTime transformed;

	dsp_transform_idft(sample, &transformed);
	return dsp_time_power(&transformed);
}	

/**
 * Calculate the spectral centroid of `sample`. `fs` refers
 * to sampling rate.
 */
double dsp_freq_spectral_centroid(const DspFreq *sample, double fs)
{
	int i;
	double freq, x = 0.0, y = 0.0;
	DspTime magnitude;

	dsp_freq_magnitude(sample, &magnitude);
	for (i = 0; i < sample->length; i++)
	{
		freq = i * fs / (double) sample->length;
		x += freq * magnitude.data[i];
		y += magnitude.data[i];
	}
	return x / y;
}

/**
 * Calculate the spectral flatness of `sample`.
 */
double dsp_freq_spectral_flatness(const DspFreq *sample)
{
	int i;
	double ari = 0.0, geo = 1.0;
	DspTime magnitude;

	dsp_freq_magnitude(sample, &magnitude);
	for (i = 0; i < sample->length; i++)
	{
		geo *= magnitude.data[i];
		ari += magnitude.data[i];
	}
	geo = pow(geo, 1.0 / sample->length);
	ari = ari / sample->length;

	return geo / ari;
}

/**
 * Calculate the spectral rolloff of `sample`. `fs` refers to sampling 
 * rate. `threshold` refers to percentage of spectral (0.0 < threshold 
 * <= 1.0).
 */
double dsp_freq_spectral_rolloff(const DspFreq *sample, double threshold,
											double fs)
{
	int i;
	double sum = 0.0, cumulative = 0.0, rolloff, freq;
	DspTime magnitude;

	dsp_freq_magnitude(sample, &magnitude);
	for (i = 0; i < sample->length; i++)
	{
		sum += magnitude.data[i];
	}
	rolloff = sum * threshold;
	for (i = 0; i < sample->length; i++)
	{
		cumulative += magnitude.data[i];
		if (cumulative > rolloff)
		{
			freq = i * fs / (double) sample->length;
			return freq;
		}
	}
	return (sample->length - 1) * fs / (double) sample->length;
}

/**
 * Calculate the total harmonic distortion (%) of 
 * `sample`. `fs` refers to sampling rate.
 */
double dsp_freq_thd(const DspFreq *sample, double fs)
{
	int i, fbin = 1;
	double freq, harmonics = 0.0, fmag;
	DspTime magnitude;

	dsp_freq_magnitude(sample, &magnitude);
	/* Find fundamental frequency (skip DC at bin 0). */
   for (i = 1; i < sample->length / 2; i++)
   {
      if (magnitude.data[i] > magnitude.data[fbin])
      {
         fbin = i;
      }
   }
	/* Calculate the total harmonic distortion. */
	fmag = magnitude.data[fbin];
	for (i = 2; i < sample->length / 2; i++)
	{
		if (i != fbin)
		{
			harmonics += pow(magnitude.data[i], 2.0);
		}
	}
	return sqrt(harmonics) / fmag * 100.0;
}
