/**
 ******************************************************************************
 * @file 	core.c
 * @author 	Ahmet Can GULMEZ
 * @brief 	Time-frequency domain operations of DSP.
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
 * Add the `fsample` and `ssample` sequences to `rsample` sequence
 * element-wisely in time domain.
 */
void dsp_time_add(const DspTime *fsample, const DspTime *ssample, 
						DspTime *result)
{
	int i;

	/* Validate the inputs. */
	assert_sample(fsample);
	assert_sample(ssample);
	assert (fsample->length == ssample->length);

	result->length = fsample->length;
	for (i = 0; i < result->length; i++)
	{
		result->data[i] = fsample->data[i] + ssample->data[i];
	}
}

/**
 * Subtract the `ssample` sequence from `fsample` sequence to `rsample`
 * sequence element-wisely in time domain.
 */
void dsp_time_subtract(const DspTime *fsample, const DspTime *ssample, 
							  DspTime *result)
{
	int i;

	/* Validate the inputs. */
	assert_sample(fsample);
	assert_sample(ssample);
	assert (fsample->length == ssample->length);

	result->length = fsample->length;
	for (i = 0; i < result->length; i++)
	{
		result->data[i] = fsample->data[i] - ssample->data[i];
	}
}

/**
 * Multiply the `sample` sequence with `scalar` to `result` sequence
 * in time domain. 
 */
void dsp_time_scalar_mul(const DspTime *sample, double scalar, 
								 DspTime *result)
{
	int i;

	/* Validate the inputs. */
	assert_sample(sample);

	result->length = sample->length;
	for (i = 0; i < result->length; i++)
	{
		result->data[i] = sample->data[i] * scalar;
	}
}

/**
 * Multiply the `fsample` and `ssample` sequences element-wisely 
 * in time domain.
 */
void dsp_time_dot_mul(const DspTime *fsample, const DspTime *ssample, 
							 DspTime *result)
{
	int i;

	/* Validate the inputs. */
	assert_sample(fsample);
	assert_sample(ssample);
	assert (fsample->length == ssample->length);

	result->length = fsample->length;
	for (i = 0; i < result->length; i++)
	{
		result->data[i] = fsample->data[i] * ssample->data[i];
	}
}

/**
 * Divide the `sample` sequence with `scalar` in time domain.
 */
void dsp_time_scalar_div(const DspTime *sample, double scalar,
								 DspTime *result)
{
	dsp_time_scalar_mul(sample, 1.0 / scalar, result);	
}

/**
 * Resize the `sample` sequence with `length` in time domain. If 
 * `length` is longer than `sample`'s length, fill the remaining 
 * data with 0s.
 */
void dsp_time_resize(const DspTime *sample, len_t length, 
							DspTime *result)
{
	int i;

	/* Validate the inputs. */
	assert_sample(sample);
	assert_length(length);

	result->length = length;
	for (i = 0; i < result->length; i++)
	{
		result->data[i] = (i >= sample->length) ? 0.0 : sample->data[i];
	}
}

/**
 * Insert the `data` at `index`. of `sample` sequence to 
 * `result` sequence in time domain.
 */
void dsp_time_insert(const DspTime *sample, int index, 
							double data, DspTime *result)
{
	int i;

	/* Validate the inputs. */
	assert_sample(sample);
	assert (index >= 0 && index <= sample->length);

	result->length = sample->length + 1;
	for (i = 0; i < index; i++)
	{
		result->data[i] = sample->data[i];
	}
	result->data[index] = data;
	for (i = index + 1; i < result->length; i++)
	{
		result->data[i] = sample->data[i - 1];
	}
}

/**
 * Remove the `index`. data of `sample` sequence to
 * `result` sequence in time domain.
 */
void dsp_time_remove(const DspTime *sample, int index, 
							DspTime *result)
{
	int i;

	/* Validate the inputs. */
	assert_sample(sample);
	assert (index >= 0 && index < sample->length);

	result->length = sample->length - 1;
	for (i = 0; i < index; i++)
	{
		result->data[i] = sample->data[i];
	}
	for (i = index + 1; i < sample->length; i++)
	{
		result->data[i - 1] = sample->data[i];
	}
}

/**
 * Append the `data` at the end of `sample` sequence to
 * `result` sequence in time domain.
 */
void dsp_time_append(const DspTime *sample, double data,
							DspTime *result)
{
	dsp_time_insert(sample, sample->length, data, result);
}

/**
 * Pop the last data of `sample` sequence to `result` sequence
 * in time domain.
 */
void dsp_time_pop(const DspTime *sample, DspTime *result)
{
	dsp_time_remove(sample, sample->length - 1, result);
}

/**
 * Shift the `sample` sequence by `shift` value to `result`
 * sequence in time domain.
 */
void dsp_time_shift(const DspTime *sample, int shift,
						  DspTime *result)
{
	int i;

	/* Validate the inputs. */
	assert_sample(sample);

	result->length = sample->length;
	if (shift >= 0)
	{
		for (i = 0; i < shift; i++)
		{
			result->data[i] = 0.0;
		}
		for (i = shift; i < result->length; i++)
		{
			result->data[i] = sample->data[i - shift];
		}
	}
	else
	{
		for (i = abs(shift); i < sample->length; i++)
		{
			result->data[i + shift] = sample->data[i];
		}
		for (i = result->length + shift; i < result->length; 
			  i++)
		{
			result->data[i] = 0.0;
		}
	}
}

/**
 * Delay the `sample` sequece by fractional `delay` using linear
 * interpolation in time domain.
 */
void dsp_time_delay_linear(const DspTime *sample, double delay,
									DspTime *result)
{
	int i, j;
	double alpha;

	/* Validate the inputs. */
	assert_sample(sample);

	result->length = sample->length;
	for (j = 0; j < result->length; j++)
	{
		i = (int) (j + delay);
		alpha = (j + delay) - i;
		if (i < 0 || i + 1 >= result->length)	/* out of data range */
		{
			result->data[j] = 0;
		}
		else	/* compute the linear interpolation */
		{
			result->data[j] = (1 - alpha) * \
			sample->data[i] + alpha * sample->data[i + 1];
		}
	}
}

/**
 * Delay the `sample` sequence by fractional `delay` using 2nd-
 * order Lagrange interpolation in time domain.
 */
void dsp_time_delay_lagrange(const DspTime *sample, double delay,
									  DspTime *result)
{
	int i, j;
	double alpha, xm, x0, xp;

	/* Validate the inputs. */
	assert_sample(sample);

	result->length = sample->length;
	for (j = 0; j < result->length; j++)
	{
		i = (int) (j + delay);
		alpha = (j + delay) - i;
		if (i-1 < 0 || i >= sample->length || i+1 >= sample->length)
		{
			/* out of data range */
			result->data[j] = 0;
		}
		else	/* compute the linear interpolation */
		{
			xm = sample->data[i - 1];
			x0 = sample->data[i];
			xp = sample->data[i + 1];
			result->data[j] = xm * (alpha * (alpha - 1.0)) * \
				0.5 - x0 * ((alpha + 1.0) * (alpha - 1.0)) + \
				xp * (alpha * (alpha + 1.0)) * 0.5;
		}
	}	
}

/**
 * Concatenate `fsample` and `ssample` sequences in time domain.
 */
void dsp_time_concat(const DspTime *fsample, const DspTime *ssample,
							DspTime *result)
{
	int i;

	/* Validate the inputs. */
	assert_sample(fsample);
	assert_sample(ssample);
	
	result->length = fsample->length + ssample->length;
	for (i = 0; i < fsample->length; i++)
	{
		result->data[i] = fsample->data[i];
	}
	for (i = fsample->length; i < result->length; i++)
	{
		result->data[i] = ssample->data[i - fsample->length];
	}
}

/**
 * Compute the absolute of `sample` sequence.
 */
void dsp_time_abs(const DspTime *sample, DspTime *result)
{
	int i;

	/* Validate the inputs. */
	assert_sample(sample);

	result->length = sample->length;
	for (i = 0; i < result->length; i++)
	{
		result->data[i] = fabs(sample->data[i]);
	}
}

/**
 * Generate the random data between 0.0 and 1.0 which
 * has `length` in time domain.
 */
void dsp_time_rand(len_t length, DspTime *result)
{
	int i;

	/* Validate the inputs. */
	assert_length(length);

	srandom(time(NULL));	/* set the seed randomly */
	result->length = length;
	for (i = 0; i < result->length; i++)
	{
		result->data[i] = (double) random() / RAND_MAX;
	}
}

/**
 * Generate the random data between -1.0 and 1.0 which
 * has `length` in time domain.
 */
void dsp_time_randn(len_t length, DspTime *result)
{
	int i;
	double u1, u2, z0, z1;

	/* Validate the inputs. */
	assert_length(length);

	srandom(time(NULL));	/* set the seed randomly */
	result->length = length;
	for (i = 0; i < result->length; i += 2)
	{
		u1 = (random() + 1.0) / (RAND_MAX + 2.0);
      u2 = (random() + 1.0) / (RAND_MAX + 2.0);
      z0 = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
      z1 = sqrt(-2.0 * log(u1)) * sin(2.0 * M_PI * u2);

      result->data[i] = z0;
      if (i + 1 < length) 
		{
         result->data[i + 1] = z1;
      }
	}
}

/**
 * Return the length of `sample` sequence.
 */
len_t dsp_time_length(const DspTime *sample)
{
	assert_sample(sample);

	return sample->length;
}

/**
 * Return the `index`. data of `sample` sequence.
 */
double dsp_time_data(const DspTime *sample, int index)
{
	assert_sample(sample);
	assert(index >= 0 && index < sample->length);

	return sample->data[index];
}

/**
 * Return the `index`. data of `sample` sequence.
 */
int dsp_time_index(const DspTime *sample, double data)
{
	int i;
	double epsilon;

	/* Validate the inputs. */
	assert_sample(sample);

	epsilon = 1e-6;
	for (i = 0; i < sample->length; i++)
	{
		if (fabs(sample->data[i] - data) < epsilon)
		{
			return i;
		}
	}
	return -1;
}

/**
 * Return the maximum data of `sample` sequence.
 */
double dsp_time_max(const DspTime *sample)
{
	int i;
	double data;

	/* Validate the inputs. */
	data = sample->data[0];
	for (i = 0; i < sample->length; i++)
	{
		if (sample->data[i] > data)
		{
			data = sample->data[i];
		}
	}
	return data;
}

/**
 * Return the minimum data of `sample` sequence.
 */
double dsp_time_min(const DspTime *sample)
{
	int i;
	double data;

	/* Validate the inputs. */
	data = sample->data[0];
	for (i = 0; i < sample->length; i++)
	{
		if (sample->data[i] < data)
		{
			data = sample->data[i];
		}
	}
	return data;
}

/**
 * Return the absolute maximum data of `sample` sequence.
 */
double dsp_time_abs_max(const DspTime *sample)
{
	DspTime result;	/* don't update the parameter in any way! */
	
	dsp_time_abs(sample, &result);
	return dsp_time_max(&result);
}

/**
 * Return the absolute minimum data of `sample` sequence.
 */
double dsp_time_abs_min(const DspTime *sample)
{
	DspTime result;	/* don't update the parameter in any way! */
	
	dsp_time_abs(sample, &result);
	return dsp_time_min(&result);	
}

/**
 * Return the maximum data index of `sample` sequence.
 */
int dsp_time_argmax(const DspTime *sample)
{
	return dsp_time_index(sample, dsp_time_max(sample));
}

/**
 * Return the minimum data index of `sample` sequence.
 */
int dsp_time_argmin(const DspTime *sample)
{
	return dsp_time_index(sample, dsp_time_min(sample));
}

/**
 * Calculate the mean of `sample` sequence.
 */
double dsp_time_mean(const DspTime *sample)
{
	int i;
	double sum;

	/* Validate the inputs. */
	assert_sample(sample);

	sum = 0;
	for (i = 0; i < sample->length; i++)
	{
		sum += sample->data[i];
	}
	return sum / (double) sample->length; 
}

/**
 * Calculate the standard deviation of `sample` sequence.
 */
double dsp_time_stddev(const DspTime *sample)
{
	int i;
	double mean, diffs;

	/* Validate the inputs. */
	assert_sample(sample);

	diffs = 0;
	mean = dsp_time_mean(sample);
	for (i = 0; i < sample->length; i++)
	{
		diffs += pow(sample->data[i] - mean, 2.0);
	}
	return sqrt(1.0 / sample->length * diffs);
}

/**
 * Calculate the energy of `sample` sequence.
 */
double dsp_time_energy(const DspTime *sample)
{
	int i;
	double sum;

	/* Validate the inputs. */
	assert_sample(sample);

	for (i = 0; i < sample->length; i++)
	{
		sum += pow(sample->data[i], 2.0);
	}
	return sum;
}

/**
 * Calculate the average power of `sample` sequence.
 */
double dsp_time_power(const DspTime *sample)
{
	return dsp_time_energy(sample) / (double) sample->length;
}

/**
 * Calculate the root mean squared of `sample` sequence.
 */
double dsp_time_rms(const DspTime *sample)
{
	return sqrt(dsp_time_power(sample));
}

/**
 * Calculate the crest factor of `sample` sequence.
 */
double dsp_time_crest_factor(const DspTime *sample)
{
	return dsp_time_abs_max(sample) / dsp_time_rms(sample);
}

/**
 * Calculate the skewness of `sample` sequence.
 */
double dsp_time_skewness(const DspTime *sample)
{
	int i;
	double mean, stddev, skewness;

	/* Validate the inputs. */
	assert_sample(sample);

	skewness = 0.0;
	stddev = dsp_time_stddev(sample);
	if (stddev < 1e-5)
	{
		return 0.0;
	}
	mean = dsp_time_mean(sample);
	for (i = 0; i < sample->length; i++)
	{
		skewness += pow((sample->data[i] - mean) / stddev, 3.0);
	}
	return skewness / (double) sample->length;
}

/**
 * Calculate the kurtosis of `sample` sequence.
 */
double dsp_time_kurtosis(const DspTime *sample)
{
	int i;
   double mean, stddev, kurtosis;

   kurtosis = 0.0;
   stddev = dsp_time_stddev(sample);
	if (stddev < 1e-5)
	{
		return 0.0;
	}
	mean = dsp_time_mean(sample);
   for (i = 0; i < sample->length; i++) 
	{
		kurtosis += pow((sample->data[i] - mean) / stddev, 4.0);
	}
   return (kurtosis / (double) sample->length) - 3.0;
}

/**
 * Calculate the variance of `sample` sequence.
 */
double dsp_time_variance(const DspTime *sample)
{
	return pow(dsp_time_stddev(sample), 2.0);
}

/**
 * Calculate the signal-to-noise ratio (dB) of `signal` sequence.
 */
double dsp_time_snr(const DspTime *sample, const DspTime *noise)
{
	return 10.0 * log10(dsp_time_power(sample) / dsp_time_power(noise));
}

/**
 * Calculate the Shannon entropy (bits) of `signal` sequence.
 */
double dsp_time_entropy(const DspTime *sample, int bins)
{
	int i, bin;
   double min_val, max_val, bin_width;
   int *histogram;
   double entropy = 0.0;

	/* Validate inputs. */
	assert_sample(sample);
	assert(bins > 0);

	/* Get the max, min and width values. */
	min_val = dsp_time_min(sample);
   max_val = dsp_time_max(sample);
   bin_width = (max_val - min_val) / bins; 
   if (bin_width < 1e-10)
	{
		return 0.0;
	} 

	/* Allocate and initialize the histogram. */
	histogram = calloc(bins, sizeof(int));
	for (i = 0; i < sample->length; i++)
   {
      bin = (int)((sample->data[i] - min_val) / bin_width);
      if (bin >= bins)
		{
			bin = bins - 1;
		}
      if (bin < 0)
		{
			bin = 0;
		}
      histogram[bin]++;
   }

	/* Calculate Shannon entropy. */
   for (i = 0; i < bins; i++)
   {
      if (histogram[i] > 0)
      {
         double p = (double)histogram[i] / sample->length;
         entropy -= p * log2(p);
      }
   }
	free(histogram);

	return entropy;
}

/**
 * Scale the `sample` sequence in time domain.
 */
void dsp_time_scale(const DspTime *sample, double scale, 
						  DspTime *result)
{
	int i;
	double absmax;

	/* Validate the inputs. */
	assert_sample(sample);

	absmax = dsp_time_abs_max(sample);
	result->length = sample->length;
	/* Normalize the sample between [-scale, scale] range. */
	for (i = 0; i < result->length; i++)
	{
		result->data[i] = (sample->data[i] / absmax) * scale;
	}
}

/**
 * Decrease the sampling rate of `sample` sequence by `factor` 
 * in time domain.
 */
void dsp_time_downsample(const DspTime *sample, int factor, 
								 DspTime *result)
{
	int i;

	/* Validate the inputs. */
	assert_sample(sample);
	assert(factor > 1 && factor < sample->length);

	result->length = (len_t) sample->length / factor;
	if (factor % 2 != 0)
	{
		result->length = (sample->length + factor - 1) / factor;
	}
	for (i = 0; i < result->length; i++)
	{
		result->data[i] = sample->data[i * factor];
	}
}

/**
 * Increment the sampling rate of `sample` sequence by `factor` 
 * in time domain.
 */
void dsp_time_upsample(const DspTime *sample, int factor,
							  DspTime *result)
{
	int i, j;

	/* Validate the inputs. */
	assert_sample(sample);
	assert(factor > 1 && factor < sample->length);

	result->length = sample->length * factor;
	for (i = 0; i < sample->length; i++)
	{
		result->data[i * factor] = sample->data[i];
		for (j = 1; j < factor; j++)
		{
			result->data[i * factor + j] = 0.0;
		}
	}
}

/**
 * Detect the peak data in `sample` sequence according to 
 * `threshold` in time domain.
 */
void dsp_time_peaks(const DspTime *sample, double threshold,
						  DspTime *result)
{
	int i;

	/* Validate the inputs. */
	assert_sample(sample);

	result->length = sample->length;
	result->data[0] = 0.0;
	result->data[result->length - 1] = 0.0;
	for (i  = 1; i < result->length - 1; i++)
	{
		/* Compare the data with preceding, leading and
			threashold values. */
		if ((sample->data[i] > sample->data[i - 1]) &&
			 (sample->data[i] > sample->data[i + 1]) &&
			 (sample->data[i] > threshold))
		{
			result->data[i] = sample->data[i];
		}
		else
		{
			result->data[i] = 0.0;
		}
	}
}

/**
 * Clip the `sample` sequence between `min_data` and 
 * `max_data` in time domain.
 */
void dsp_time_clip(const DspTime *sample, double min_data, 
						 double max_data, DspTime *result)
{
	int i;

	/* Validate the inputs. */
	assert_sample(sample);
	assert(max_data > min_data);

	result->length = sample->length;
	for (i = 0; i < result->length; i++)
	{
		if (sample->data[i] <= min_data)
		{
			result->data[i] = min_data;
		}
		else if (sample->data[i] >= max_data)
		{
			result->data[i] = max_data;
		}
		else
		{
			result->data[i] = sample->data[i];
		}
	}
}

/**
 * Convolve the `fsample` and `ssample` sequences fully with each other
 * in time domain.
 */
void dsp_time_convolve(const DspTime *fsample, const DspTime *ssample,
							  DspTime *result)
{
	int i, j;

	/* Validate the inputs. */
	assert_sample(fsample);
	assert_sample(ssample);

	result->length = fsample->length + ssample->length - 1;
	for (i = 0; i < result->length; i++)
	{
		result->data[i] = 0.0;
	}
	for (i = 0; i < fsample->length; i++)
	{
		for (j = 0; j < ssample->length; j++)
		{
			result->data[i + j] += fsample->data[i] * ssample->data[j];
		}
	}
}

/**
 * Correlate the `sample` sequnce fully with itself in time domain.
 */
void dsp_time_auto_corr(const DspTime *sample, DspTime *result)
{
	int i, j, len;
	double sum;
	DspTime shifted, multipled;

	/* Validate the inputs. */
	assert_sample(sample);

	sum = 0;
	len = sample->length;
	result->length = 2 * len - 1;
	for (i = -(len - 1); i < len; i++)
	{
		/* Shift and then multiply the signal. */
		dsp_time_shift(sample, i, &shifted);
		dsp_time_dot_mul(sample, &shifted, &multipled);
		for (j = 0; j < multipled.length; j++)
		{
			sum += multipled.data[j];
		}
		result->data[i + len - 1] = sum;
		sum = 0;
	}
}

/**
 * Correlate the `sample` sequence negatively with itself in time domain.
 */
void dsp_time_auto_corr_neg(const DspTime *sample, DspTime *result)
{
	int i;
	DspTime correlated;

	/* Validate the inputs. */
	assert_sample(sample);

	result->length = sample->length;
	dsp_time_auto_corr(sample, &correlated);
	for (i = 0; i < result->length; i++)
	{
		result->data[i] = correlated.data[i];
	}
}

/**
 * Correlate the `sample` sequence positively with itself in time domain.
 */
void dsp_time_auto_corr_pos(const DspTime *sample, DspTime *result)
{
int i;
	DspTime correlated;

	/* Validate the inputs. */
	assert_sample(sample);

	result->length = sample->length;
	dsp_time_auto_corr(sample, &correlated);
	for (i = 0; i < result->length; i++)
	{
		result->data[i] = correlated.data[i + sample->length - 1];
	}
}

/**
 * Correlate the `fsample` (will be fixed) and `ssample` (will be shifted) 
 * sequences fully in time domain.
 */
void dsp_time_cross_corr(const DspTime *fsample, const DspTime *ssample,
								 DspTime *result)
{
	int i, j, len;
	DspTime shifted, multiplied;
	double sum;

	/* Validate the inputs. */
	assert_sample(fsample);
	assert_sample(ssample);
	assert(fsample->length == ssample->length);

	sum = 0;
	len = fsample->length;
	result->length = fsample->length + ssample->length - 1;
	for (i = -(len - 1); i < len; i++)
	{
		/* Shift and then multiply the signal. */
		dsp_time_shift(ssample, i, &shifted);
		dsp_time_dot_mul(fsample, &shifted, &multiplied);
		for (j = 0; j < multiplied.length; j++)
		{
			sum += multiplied.data[j];
		}
		result->data[i + len - 1] = sum;
		sum = 0;
	}
}

/**
 * Correlate the `fsample` (will be fixed) with `ssample` (will be shifted) 
 * negatively in time domain.
 */
void dsp_time_cross_corr_neg(const DspTime *fsample, const DspTime *ssample,
									  DspTime *result)
{
	int i;
	DspTime correlated;
	
	/* Validate the inputs. */
	assert_sample(fsample);
	assert_sample(ssample);
	assert(fsample->length == ssample->length);

	dsp_time_cross_corr(fsample, ssample, &correlated);
	result->length = (len_t) (correlated.length / 2) + 1;
	for (i = 0; i < result->length; i++)
	{
		result->data[i] = correlated.data[i];
	}
}

/**
 * Correlate the `fsample` (will be fixed) with `ssample` (will be shifted) 
 * positively in time domain.
 */
void dsp_time_cross_corr_pos(const DspTime *fsample, const DspTime *ssample,
									  DspTime *result)
{
	int i;
	DspTime correlated;
	
	/* Validate the inputs. */
	assert_sample(fsample);
	assert_sample(ssample);
	assert(fsample->length == ssample->length);

	dsp_time_cross_corr(fsample, ssample, &correlated);
	result->length = (len_t) (correlated.length / 2) + 1;
	for (i = 0; i < result->length; i++)
	{
		result->data[i] = correlated.data[i + (correlated.length / 2)];
	}
}


/**
 * Add `fsample` and `ssample` sequences with each other in time domain.
 */
void dsp_freq_add(const DspFreq *fsample, const DspFreq *ssample, 
						DspFreq *result)
{
	int i, j;

	/* Validate the inputs. */
	assert_sample(fsample);
	assert_sample(ssample);
	assert(fsample->length == ssample->length);

	result->length = fsample->length;
	for (i = 0; i < result->length; i++)
	{
		result->data[i][0] = fsample->data[i][0] + ssample->data[i][0];
		result->data[i][1] = fsample->data[i][1] + ssample->data[i][1];
	}
}

/**
 * Subtract `ssample` sequence from `fsample` sequence in frequency domain.
 */
void dsp_freq_subtract(const DspFreq *fsample, const DspFreq *ssample, 
							  DspFreq *result)
{
	dsp_freq_scalar_mul(ssample, -1.0, result);
	dsp_freq_add(fsample, result, result);
}

/**
 * Multiply `sample` sequence with `scalar` in frequency domain.
 */
void dsp_freq_scalar_mul(const DspFreq *sample, double scalar, 
								 DspFreq *result)
{
	int i;

	/* Validate the inputs. */
	assert_sample(sample);

	result->length = sample->length;
	for (i = 0; i < result->length; i++)
	{
		result->data[i][0] = sample->data[i][0] * scalar;
		result->data[i][1] = sample->data[i][1] * scalar;
	}
}

/**
 * Multiply the `fsample` and `ssample` sequences in frequency domain.
 */
void dsp_freq_dot_mul(const DspFreq *fsample, const DspFreq *ssample, 
							 DspFreq *result)
{
	int i;

	/* Validate the inputs. */
	assert_sample(fsample);
	assert_sample(ssample);
	assert(fsample->length == ssample->length);

	result->length = fsample->length;
	for (i = 0; i < result->length; i++)
	{
		result->data[i][0] = fsample->data[i][0] * ssample->data[i][0] - 
         fsample->data[i][1] * ssample->data[i][1];
      result->data[i][1] = fsample->data[i][0] * ssample->data[i][1] +
         fsample->data[i][1] * ssample->data[i][0]; 
	}
}

/**
 * Divide the `sample` sequence by `scalar` in frequency domain.
 */
void dsp_freq_scalar_div(const DspFreq *sample, double scalar, 
								 DspFreq *result)
{
	dsp_freq_scalar_mul(sample, (1.0 / scalar), result);
}

/**
 * Resize the `sample` sequence. If `sample`'s length is bigger, then fill
 * the zeros in frequency domain.
 */
void dsp_freq_resize(const DspFreq *sample, len_t length, DspFreq *result)
{
	int i, j;

	/* Validate the inputs. */
	assert_sample(sample);
	assert_length(length);

	result->length = length;
	for (i = 0; i < result->length; i++)
	{
		result->data[i][0] = (i >= sample->length) ? 0.0 : sample->data[i][0];
		result->data[i][1] = (i >= sample->length) ? 0.0 : sample->data[i][1];
	}
}

/**
 * Insert the `real`+`imag`*i at `index`. of `sample` sequence in 
 * frequency domain.
 */
void dsp_freq_insert(const DspFreq *sample, double real, double imag, 
							int index, DspFreq *result)
{
	int i;

	/* Validate the inputs. */
	assert_sample(sample);
	assert(index >= 0 && index <= sample->length);

	result->length = sample->length + 1;
	for (i = 0; i < index; i++)
	{
		result->data[i][0] = sample->data[i][0];
		result->data[i][1] = sample->data[i][1];
	}
	result->data[index][0] = real;
	result->data[index][1] = imag;
	for (i = index; i < result->length; i++)
	{
		result->data[i + 1][0] = sample->data[i][0];
		result->data[i + 1][1] = sample->data[i][1];
	}
}

/**
 * Remove the `index`. data of `sample` in frequency domain.
 */
void dsp_freq_remove(const DspFreq *sample, int index, 
							DspFreq *result)
{
	int i;

	/* Validate the inputs. */
	assert_sample(sample);
	assert(index >= 0 && index < sample->length);

	result->length = sample->length - 1;
	for (i = 0; i < index; i++)
	{
		result->data[i][0] = sample->data[i][0];
		result->data[i][1] = sample->data[i][1];
	}
	for (i = index; i < result->length; i++)
	{
		result->data[i][0] = sample->data[i + 1][0];
		result->data[i][1] = sample->data[i + 1][1];
	}
}

/**
 * Append the `real`+`imag`*i data at the end of `sample` sequence
 * in frequency domain.
 */
void dsp_freq_append(const DspFreq *sample, double real, double imag, 
							DspFreq *result)
{
	dsp_freq_insert(sample, real, imag, sample->length, result);
}

/**
 * Pop the last data of `sample` sequence in frequency domain.
 */
void dsp_freq_pop(const DspFreq *sample, DspFreq *result)
{
	dsp_freq_remove(sample, sample->length - 1, result);
}

/**
 * Concatenate the `fsample` and `ssample` sequences in frequency domain.
 */
void dsp_freq_concat(const DspFreq *fsample, const DspFreq *ssample, 
							DspFreq *result)
{
	int i;

	/* Validate the inputs. */
	assert_sample(fsample);
	assert_sample(ssample);

	result->length = fsample->length + ssample->length;
	memcpy(result->data, fsample->data, sizeof(double) * 2 * fsample->length);
	for (i = fsample->length; i < result->length; i++)
	{
		result->data[i][0] = ssample->data[i - fsample->length][0];
		result->data[i][1] = ssample->data[i - fsample->length][1];
	}
}

/**
 * Return the length of `sample` sequence.
 */
len_t dsp_freq_length(const DspFreq *sample)
{
	return sample->length;
}

/**
 * Extract the real part of `sample` sequence.
 */
void dsp_freq_real(const DspFreq *sample, DspTime *result)
{
	int i;

	/* Validate the inputs. */
	assert_sample(sample);

	result->length = sample->length;
	for (i = 0; i < result->length; i++)
	{
		result->data[i] = sample->data[i][0];
	}
}

/**
 * Extract the imaginary part of `sample` sequence.
 */
void dsp_freq_imag(const DspFreq *sample, DspTime *result)
{
	int i;

	/* Validate the inputs. */
	assert_sample(sample);

	result->length = sample->length;
	for (i = 0; i < result->length; i++)
	{
		result->data[i] = sample->data[i][1];
	}
}

/**
 * Extract the magnitude of `sample` sequence.
 */
void dsp_freq_magnitude(const DspFreq *sample, DspTime *result)
{
	int i;

	/* Validate the inputs. */
	assert_sample(sample);

	result->length = sample->length;
	for (i = 0; i < result->length; i++)
	{
		result->data[i] = sqrt(pow(sample->data[i][0], 2.0) +
									  pow(sample->data[i][1], 2.0));
	}
}

/**
 * Extract the phase in degrees of `sample` sequence.
 */
void dsp_freq_phase(const DspFreq *sample, DspTime *result)
{
	int i;

	/* Validate the inputs. */
	assert_sample(sample);

	result->length = sample->length;
	for (i = 0; i < result->length; i++)
	{
		/* Handle division by zero. */
		if (fabs(sample->data[i][0]) < 1e-10 && 
		 	 fabs(sample->data[i][1]) < 1e-10)
   	{
   	   result->data[i] = 0.0;
   	}
		result->data[i] = DEG(
			atan2(sample->data[i][1], sample->data[i][0]));
	}
}

/**
 * Calculate the power spectral density of `index`.th
 * of `sample` sequence.
 */
double dsp_freq_psd(const DspFreq *sample, int index)
{
	assert_sample(sample);

	return (pow(sample->data[index][0], 2.0) +
		pow(sample->data[index][1], 2.0)) / sample->length;
}

/**
 * Calculate the average power of `sample` sequence.
 */
double dsp_freq_power(const DspFreq *sample)
{
	DspTime transformed;

	dsp_transform_idft(sample, &transformed);
	return dsp_time_power(&transformed);
}	

/**
 * Calculate the spectral centroid of `sample` sequence. `fs` refers
 * to sampling rate.
 */
double dsp_freq_spectral_centroid(const DspFreq *sample, double fs)
{
	int i;
	double freq, x, y;
	DspTime magnitude;

	/* Validate the inputs. */
	assert_sample(sample);

	x = 0; y = 0;
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
 * Calculate the spectral flatness of `sample` sequence.
 */
double dsp_freq_spectral_flatness(const DspFreq *sample)
{
	int i;
	double arithmetic, geometric;
	DspTime magnitude;

	/* Validate the inputs. */
	assert_sample(sample);

	arithmetic = 0.0;
	geometric = 1.0;
	dsp_freq_magnitude(sample, &magnitude);
	for (i = 0; i < sample->length; i++)
	{
		geometric *= magnitude.data[i];
		arithmetic += magnitude.data[i];
	}
	geometric = pow(geometric, 1 / sample->length);
	arithmetic = arithmetic / sample->length;

	return geometric / arithmetic;
}

/**
 * Calculate the spectral rolloff of `sample` sequence. `fs` refers 
 * to sampling rate. `threshold` refers to percentage of spectral 
 * (0.0 < threshold <= 1.0).
 */
double dsp_freq_spectral_rolloff(const DspFreq *sample, double threshold, 
											double fs)
{
	int i;
	double sum, cumulative, rolloff, freq;
	DspTime magnitude;

	/* Validate the inputs. */
	assert_sample(sample);
	assert(threshold > 0 && threshold <= 1.0);

	sum = 0.0;
	cumulative = 0.0;
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
			break;
		}
	}
	return freq;
}

/**
 * Calculate the total harmonic distortion (%) of 
 * `sample` sequence. `fs` refers to sampling rate.
 */
double dsp_freq_thd(const DspFreq *sample, double fs)
{
	int i, fbin;
	double freq, harmonics, fmag;
	DspTime magnitude;

	/* Validate the inputs. */
	assert_sample(sample);

	fbin = 1;
	harmonics = 0.0;
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
		harmonics += pow(magnitude.data[i], 2.0);
	}
	return sqrt(harmonics) / fmag * 100.0;
}
