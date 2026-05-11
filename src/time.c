/**
 ******************************************************************************
 * @file 	time.c
 * @author 	Can Gulmez
 * @brief 	Core time domain operations of DSP.
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
 * Add the `fsample` and `ssample` element-wisely in time domain.
 */
DspStatus dsp_time_add(const DspTime *fsample, const DspTime *ssample, DspTime *res)
{
	int i;

	/* Validate the input parameters. */
	if (IS_BAD_SAMPLE(fsample) || IS_BAD_SAMPLE(ssample))
		return DSP_ERR_BAD_SAMPLE;

	if (IS_MISMATCH(fsample, ssample))
		return DSP_ERR_MISMATCH;

	res->length = fsample->length;
	for (i = 0; i < res->length; i++)
	{
		res->data[i] = fsample->data[i] + ssample->data[i];
	}
	return DSP_SUCCESS;
}

/**
 * Subtract the `ssample` from `fsample` element-wisely in time domain.
 */
DspStatus dsp_time_sub(const DspTime *fsample, const DspTime *ssample, DspTime *res)
{
	int i;
	DspStatus status;

	/* Validate the input parameters. */
	if (IS_BAD_SAMPLE(fsample) || IS_BAD_SAMPLE(ssample))
		return DSP_ERR_BAD_SAMPLE;

	if (IS_MISMATCH(fsample, ssample))
		return DSP_ERR_MISMATCH;

	res->length = fsample->length;
	for (i = 0; i < res->length; i++)
	{
		res->data[i] = fsample->data[i] - ssample->data[i];
	}
	return DSP_SUCCESS;
}


/**
 * Multiply the `sample` with `scalar` in time domain. 
 */
DspStatus dsp_time_scalar_mul(const DspTime *sample, double scalar, DspTime *res)
{
	int i;
	DspStatus status;

	/* Validate the input parameters. */
	if (IS_BAD_SAMPLE(sample))
		return DSP_ERR_BAD_SAMPLE;

	res->length = sample->length;
	for (i = 0; i < res->length; i++)
	{
		res->data[i] = sample->data[i] * scalar;
	}
	return DSP_SUCCESS;
}

/**
 * Multiply the `fsample` and `ssample` element-wisely in time domain.
 */
DspStatus dsp_time_dot_mul(const DspTime *fsample, const DspTime *ssample, DspTime *res)
{
	int i;
	DspStatus status;

	/* Validate the input parameters. */
	if (IS_BAD_SAMPLE(fsample) || IS_BAD_SAMPLE(ssample))
		return DSP_ERR_BAD_SAMPLE;

	if (IS_MISMATCH(fsample, ssample))
		return DSP_ERR_MISMATCH;

	res->length = fsample->length;
	for (i = 0; i < res->length; i++)
	{
		res->data[i] = fsample->data[i] * ssample->data[i];
	}
	return DSP_SUCCESS;
}

/**
 * Divide the `sample` with `scalar` in time domain.
 */
DspStatus dsp_time_scalar_div(const DspTime *sample, double scalar, DspTime *res)
{
	if (scalar == 0.0)
		return DSP_ERR_FALSE_COND;

	return dsp_time_scalar_mul(sample, 1.0 / scalar, res);	
}

/**
 * Resize the `sample` with `length` in time domain. If `length` is longer 
 * than `sample`'s length, fill the remaining data with 0s.
 */
DspStatus dsp_time_resize(const DspTime *sample, len_t length, DspTime *res)
{
	int i;
	DspStatus status;

	/* Validate the input parameters. */
	if (IS_BAD_SAMPLE(sample))
		return DSP_ERR_BAD_SAMPLE;

	if (IS_BAD_LEN(length))
		return DSP_ERR_BAD_LEN;;

	res->length = length;
	for (i = 0; i < res->length; i++)
	{
		res->data[i] = (i >= sample->length) ? 0.0 : sample->data[i];
	}
	return DSP_SUCCESS;
}

/**
 * Insert the `data` at `index`. of `sample` in time domain.
 */
DspStatus dsp_time_insert(const DspTime *sample, index_t index, double data,
								  DspTime *res)
{
	int i;

	/* Validate the input parameters. */
	if (IS_BAD_SAMPLE(sample))
		return DSP_ERR_BAD_SAMPLE;

	if (IS_BAD_INDEX(sample, index))
		return DSP_ERR_BAD_INDEX;

	res->length = sample->length + 1;
	if (IS_BAD_LEN(res->length))
		return DSP_ERR_BAD_LEN;
		
	for (i = 0; i < index; i++)
	{
		res->data[i] = sample->data[i];
	}
	res->data[index] = data;
	for (i = index + 1; i < res->length; i++)
	{
		res->data[i] = sample->data[i - 1];
	}
	return DSP_SUCCESS;
}

/**
 * Remove the `index`. data of `sample` in time domain.
 */
DspStatus dsp_time_remove(const DspTime *sample, index_t index, DspTime *res)
{
	int i;

	/* Validate the input parameters. */
	if (IS_BAD_SAMPLE(sample))
		return DSP_ERR_BAD_SAMPLE;

	if (IS_BAD_INDEX(sample, index))
		return DSP_ERR_BAD_INDEX;

	res->length = sample->length - 1;
	for (i = 0; i < index; i++)
	{
		res->data[i] = sample->data[i];
	}
	for (i = index + 1; i < sample->length; i++)
	{
		res->data[i - 1] = sample->data[i];
	}
	return DSP_SUCCESS;
}

/**
 * Append the `data` at the end of `sample` in time domain.
 */
DspStatus dsp_time_append(const DspTime *sample, double data, DspTime *res)
{
	return dsp_time_insert(sample, sample->length, data, res);
}

/**
 * Pop the last data of `sample` in time domain.
 */
DspStatus dsp_time_pop(const DspTime *sample, DspTime *res)
{
	return dsp_time_remove(sample, sample->length - 1, res);
}

/**
 * Shift the `sample` by `shift` value in time domain.
 */
DspStatus dsp_time_shift(const DspTime *sample, int shift, DspTime *res)
{
	int i;

	/* Validate the input parameters. */
	if (IS_BAD_SAMPLE(sample))
		return DSP_ERR_BAD_SAMPLE;

	res->length = sample->length;
	if (shift >= 0)
	{
		for (i = 0; i < shift; i++)
		{
			res->data[i] = 0.0;
		}
		for (i = shift; i < res->length; i++)
		{
			res->data[i] = sample->data[i - shift];
		}
	}
	else
	{
		for (i = abs(shift); i < sample->length; i++)
		{
			res->data[i + shift] = sample->data[i];
		}
		for (i = res->length + shift; i < res->length; i++)
		{
			res->data[i] = 0.0;
		}
	}
	return DSP_SUCCESS;
}

/**
 * Delay the `sample` by fractional `delay` using linear interpolation in time domain.
 */
DspStatus dsp_time_delay_linear(const DspTime *sample, double delay, DspTime *res)
{
	int i, j;
	double alpha;

	/* Validate the input parameters. */
	if (IS_BAD_SAMPLE(sample))
		return DSP_ERR_BAD_SAMPLE;

	res->length = sample->length;
	for (j = 0; j < res->length; j++)
	{
		i = (int) (j + delay);
		alpha = (j + delay) - i;
		if (i < 0 || i + 1 >= res->length)	/* out of data range */
		{
			res->data[j] = 0;
		}
		else	/* compute the linear interpolation */
		{
			res->data[j] = (1 - alpha) * \
				sample->data[i] + alpha * sample->data[i + 1];
		}
	}
	return DSP_SUCCESS;
}

/**
 * Delay the `sample` by fractional `delay` using 2nd-order Lagrange 
 * interpolation in time domain.
 */
DspStatus dsp_time_delay_lagrange(const DspTime *sample, double delay, 
											 DspTime *res)
{
	int i, j;
	double alpha, xm, x0, xp;

	/* Validate the input parameters. */
	if (IS_BAD_SAMPLE(sample))
		return DSP_ERR_BAD_SAMPLE;

	res->length = sample->length;
	for (j = 0; j < res->length; j++)
	{
		i = (int) (j + delay);
		alpha = (j + delay) - i;
		if (i-1 < 0 || i >= sample->length || i+1 >= sample->length)
		{
			/* out of data range */
			res->data[j] = 0;
		}
		else	/* compute the linear interpolation */
		{
			xm = sample->data[i - 1];
			x0 = sample->data[i];
			xp = sample->data[i + 1];
			res->data[j] = xm * (alpha * (alpha - 1.0)) * \
				0.5 - x0 * ((alpha + 1.0) * (alpha - 1.0)) + \
				xp * (alpha * (alpha + 1.0)) * 0.5;
		}
	}
	return DSP_SUCCESS;
}

/**
 * Concatenate `fsample` and `ssample` in time domain.
 */
DspStatus dsp_time_concat(const DspTime *fsample, const DspTime *ssample,
								  DspTime *res)
{
	int i;

	/* Validate the input parameters. */
	if (IS_BAD_SAMPLE(fsample) || IS_BAD_SAMPLE(ssample))
		return DSP_ERR_BAD_SAMPLE;

	res->length = fsample->length + ssample->length;
	if (IS_BAD_LEN(res->length))
		return DSP_ERR_BAD_LEN;

	for (i = 0; i < fsample->length; i++)
	{
		res->data[i] = fsample->data[i];
	}
	for (i = fsample->length; i < res->length; i++)
	{
		res->data[i] = ssample->data[i - fsample->length];
	}
	return DSP_SUCCESS;
}

/**
 * Compute the absolute of `sample`.
 */
DspStatus dsp_time_abs(const DspTime *sample, DspTime *res)
{
	int i;

	/* Validate the input parameters. */
	if (IS_BAD_SAMPLE(sample))
		return DSP_ERR_BAD_SAMPLE;

	res->length = sample->length;
	for (i = 0; i < res->length; i++)
	{
		res->data[i] = fabs(sample->data[i]);
	}
	return DSP_SUCCESS;
}

/**
 * Create the random sample between `[0.0, 1.0]` 
 * which has `length` in time domain.
 */
DspStatus dsp_time_rand(len_t length, DspTime *res)
{
	int i;

	/* Validate the input parameters. */
	if (IS_BAD_LEN(length))
		return DSP_ERR_BAD_LEN;

	res->length = length;
	for (i = 0; i < res->length; i++)
	{
		res->data[i] = (double) random() / RAND_MAX;
	}
	return DSP_SUCCESS;
}

/**
 * Create the random data between [-1.0, 1.0] which
 * has `length` in time domain.
 */
DspStatus dsp_time_randn(len_t length, DspTime *res)
{
	int i;
	double u1, u2, z0, z1;

	/* Validate the input parameters. */
	if (IS_BAD_LEN(length))
		return DSP_ERR_BAD_LEN;

	res->length = length;
	for (i = 0; i < res->length; i += 2)
	{
		u1 = (random() + 1.0) / (RAND_MAX + 2.0);
      u2 = (random() + 1.0) / (RAND_MAX + 2.0);
      z0 = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
      z1 = sqrt(-2.0 * log(u1)) * sin(2.0 * M_PI * u2);

      res->data[i] = z0;
      if (i + 1 < length) 
		{
         res->data[i + 1] = z1;
      }
	}
	return DSP_SUCCESS;
}

/**
 * Return the index of data of `sample`. If not found, -1.
 */
index_t dsp_time_index(const DspTime *sample, double data)
{
	int i;
	double epsilon;

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
 * Return the max data of `sample`.
 */
double dsp_time_max(const DspTime *sample)
{
	int i;
	double data;

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
 * Return the min data of `sample`.
 */
double dsp_time_min(const DspTime *sample)
{
	int i;
	double data;

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
 * Return the absolute max data of `sample`.
 */
double dsp_time_abs_max(const DspTime *sample)
{
	DspTime res;

	dsp_time_abs(sample, &res);
	return dsp_time_max(&res);
}

/**
 * Return the absolute min data of `sample`.
 */
double dsp_time_abs_min(const DspTime *sample)
{
	DspTime res;

	dsp_time_abs(sample, &res);
	return dsp_time_min(&res);	
}

/**
 * Return the max data index of `sample`.
 */
index_t dsp_time_argmax(const DspTime *sample)
{
	return dsp_time_index(sample, dsp_time_max(sample));
}

/**
 * Return the min data index of `sample`.
 */
index_t dsp_time_argmin(const DspTime *sample)
{
	return dsp_time_index(sample, dsp_time_min(sample));
}

/**
 * Calculate the mean of `sample` sequence.
 */
double dsp_time_mean(const DspTime *sample)
{
	int i;
	double sum = 0.0;
	
	for (i = 0; i < sample->length; i++)
	{
		sum += sample->data[i];
	}
	return sum / (double) sample->length; 
}

/**
 * Calculate the standard deviation of `sample`.
 */
double dsp_time_stddev(const DspTime *sample)
{
	int i;
	double mean, diffs = 0.0;

	mean = dsp_time_mean(sample);
	for (i = 0; i < sample->length; i++)
	{
		diffs += pow(sample->data[i] - mean, 2.0);
	}
	return sqrt(1.0 / sample->length * diffs);
}

/**
 * Calculate the energy of `sample`.
 */
double dsp_time_energy(const DspTime *sample)
{
	int i;
	double sum = 0.0;

	for (i = 0; i < sample->length; i++)
	{
		sum += pow(sample->data[i], 2.0);
	}
	return sum;
}

/**
 * Calculate the average power of `sample`.
 */
double dsp_time_power(const DspTime *sample)
{
	return dsp_time_energy(sample) / (double) sample->length;
}

/**
 * Calculate the root mean squared of `sample`.
 */
double dsp_time_rms(const DspTime *sample)
{
	return sqrt(dsp_time_power(sample));
}

/**
 * Calculate the crest factor of `sample`.
 */
double dsp_time_crest_factor(const DspTime *sample)
{
	return dsp_time_abs_max(sample) / dsp_time_rms(sample);
}

/**
 * Calculate the skewness of `sample`.
 */
double dsp_time_skewness(const DspTime *sample)
{
	int i;
	double mean, stddev, skewness = 0.0;

	stddev = dsp_time_stddev(sample);
	mean = dsp_time_mean(sample);
	for (i = 0; i < sample->length; i++)
	{
		skewness += pow((sample->data[i] - mean) / stddev, 3.0);
	}
	return skewness / (double) sample->length;
}

/**
 * Calculate the kurtosis of `sample`.
 */
double dsp_time_kurtosis(const DspTime *sample)
{
	int i;
   double mean, stddev, kurtosis = 0.0;

   stddev = dsp_time_stddev(sample);
	mean = dsp_time_mean(sample);
   for (i = 0; i < sample->length; i++) 
	{
		kurtosis += pow((sample->data[i] - mean) / stddev, 4.0);
	}
   return (kurtosis / (double) sample->length) - 3.0;
}

/**
 * Calculate the variance of `sample`.
 */
double dsp_time_variance(const DspTime *sample)
{
	return pow(dsp_time_stddev(sample), 2.0);
}

/**
 * Calculate the signal-to-noise ratio (dB) of `signal`.
 */
double dsp_time_snr(const DspTime *sample, const DspTime *noise)
{
	return 10.0 * log10(dsp_time_power(sample) / dsp_time_power(noise));
}

// /**
//  * Calculate the Shannon entropy (bits) of `signal` sequence.
//  */
// double dsp_time_entropy(const DspTime *sample, int bins)
// {
// 	int i, bin;
//    double min_val, max_val, bin_width;
//    int *histogram;
//    double entropy = 0.0;

// 	/* Validate inputs. */
// 	assert_sample(sample);
// 	assert(bins > 0);

// 	/* Get the max, min and width values. */
// 	min_val = dsp_time_min(sample);
//    max_val = dsp_time_max(sample);
//    bin_width = (max_val - min_val) / bins; 
//    if (bin_width < 1e-10)
// 	{
// 		return 0.0;
// 	} 

// 	/* Allocate and initialize the histogram. */
// 	histogram = calloc(bins, sizeof(int));
// 	for (i = 0; i < sample->length; i++)
//    {
//       bin = (int)((sample->data[i] - min_val) / bin_width);
//       if (bin >= bins)
// 		{
// 			bin = bins - 1;
// 		}
//       if (bin < 0)
// 		{
// 			bin = 0;
// 		}
//       histogram[bin]++;
//    }

// 	/* Calculate Shannon entropy. */
//    for (i = 0; i < bins; i++)
//    {
//       if (histogram[i] > 0)
//       {
//          double p = (double)histogram[i] / sample->length;
//          entropy -= p * log2(p);
//       }
//    }
// 	free(histogram);

// 	return entropy;
// }

/**
 * Sum up the all elements of `sample`.
 */
double dsp_time_sum(const DspTime *sample)
{
	int i;
	double sum = 0.0;

	for (i = 0; i < sample->length; i++)
	{
		sum += sample->data[i];
	}
	return sum;
}

/**
 * Product the all elements of `sample`.
 */
double dsp_time_product(const DspTime *sample)
{
	int i;
	double product = 1.0;

	for (i = 0; i < sample->length; i++)
	{
		product *= sample->data[i];
	}
	return product;
}

/**
 * Scale the `sample` between `[-scale, scale]` in time domain.
 */
DspStatus dsp_time_scale(const DspTime *sample, double scale, DspTime *res)
{
	int i;
	double absmax;

	/* Validate the input parameters. */
	if (IS_BAD_SAMPLE(sample))
		return DSP_ERR_BAD_SAMPLE;

	absmax = dsp_time_abs_max(sample);
	res->length = sample->length;
	for (i = 0; i < res->length; i++)
	{
		res->data[i] = (sample->data[i] / absmax) * scale;
	}
	return DSP_SUCCESS;
}

/**
 * Decrease the sampling rate of `sample` by `factor` in time domain.
 */
DspStatus dsp_time_downsample(const DspTime *sample, int factor, DspTime *res)
{
	int i;

	/* Validate the input parameters. */
	if (IS_BAD_SAMPLE(sample))
		return DSP_ERR_BAD_SAMPLE;

	if (factor < 1 || factor >= sample->length)
		return DSP_ERR_FALSE_COND;

	res->length = (len_t) ceil((double) sample->length / factor);
	if (IS_BAD_LEN(res->length))
		return DSP_ERR_BAD_LEN;

	for (i = 0; i < res->length; i++)
	{
		res->data[i] = sample->data[i * factor];
	}
	return DSP_SUCCESS;
}

/**
 * Increment the sampling rate of `sample` by `factor` in time domain.
 */
DspStatus dsp_time_upsample(const DspTime *sample, int factor, DspTime *res)
{
	int i, j;

	/* Validate the input parameters. */
	if (IS_BAD_SAMPLE(sample))
		return DSP_ERR_BAD_SAMPLE;

	if (factor < 1 || factor >= sample->length)
		return DSP_ERR_FALSE_COND;

	res->length = sample->length * factor;
	if (IS_BAD_LEN(res->length))
		return DSP_ERR_BAD_LEN;

	for (i = 0; i < sample->length; i++)
	{
		res->data[i * factor] = sample->data[i];
		for (j = 1; j < factor; j++)
		{
			res->data[i * factor + j] = 0.0;
		}
	}
	return DSP_SUCCESS;
}

/**
 * Detect the peak data in `sample` according to `threshold` in time domain.
 */
DspStatus dsp_time_peaks(const DspTime *sample, double threshold, DspTime *res)
{
	int i;

	/* Validate the input parameters. */
	if (IS_BAD_SAMPLE(sample))
		return DSP_ERR_BAD_SAMPLE;

	res->length = sample->length;
	res->data[0] = 0.0;
	res->data[res->length - 1] = 0.0;
	for (i = 1; i < res->length - 1; i++)
	{
		/* Compare the data with preceding, leading and threashold values. */
		if ((sample->data[i] > sample->data[i - 1]) &&
			 (sample->data[i] > sample->data[i + 1]) && 
			 (sample->data[i] > threshold))
		{
			res->data[i] = sample->data[i];
		}
		else
		{
			res->data[i] = 0.0;
		}
	}
	return DSP_SUCCESS;
}

/**
 * Clip the `sample` between `[lowest, highest]` in time domain.
 */
DspStatus dsp_time_clip(const DspTime *sample, double lowest, double highest, 
								DspTime *res)
{
	int i;

	/* Validate the input parameters. */
	if (IS_BAD_SAMPLE(sample))
		return DSP_ERR_BAD_SAMPLE;

	if (highest < lowest)
		return DSP_ERR_FALSE_COND;

	res->length = sample->length;
	for (i = 0; i < res->length; i++)
	{
		if (sample->data[i] <= lowest)
		{
			res->data[i] = lowest;
		}
		else if (sample->data[i] >= highest)
		{
			res->data[i] = highest;
		}
		else
		{
			res->data[i] = sample->data[i];
		}
	}
	return DSP_SUCCESS;
}

/**
 * Convolve the `fsample` and `ssample` fully each other in time domain.
 */
DspStatus dsp_time_convolve(const DspTime *fsample, const DspTime *ssample,
									 DspTime *res)
{
	int i, j;

	/* Validate the input parameters. */
	if (IS_BAD_SAMPLE(fsample) || IS_BAD_SAMPLE(ssample))
		return DSP_ERR_BAD_SAMPLE;

	res->length = fsample->length + ssample->length - 1;
	if (IS_BAD_LEN(res->length))
		return DSP_ERR_BAD_LEN;

	for (i = 0; i < res->length; i++)
	{
		res->data[i] = 0.0;
	}
	for (i = 0; i < fsample->length; i++)
	{
		for (j = 0; j < ssample->length; j++)
		{
			res->data[i + j] += fsample->data[i] * ssample->data[j];
		}
	}
	return DSP_SUCCESS;
}

/**
 * Correlate the `sample` fully with itself in time domain.
 */
DspStatus dsp_time_auto_corr(const DspTime *sample, DspTime *res)
{
	int i, j, len;
	double sum = 0.0;
	DspTime shifted, multipled;
	DspStatus status;

	/* Validate the input parameters. */
	if (IS_BAD_SAMPLE(sample))
		return DSP_ERR_BAD_SAMPLE;

	res->length = 2 * sample->length - 1;
	if (IS_BAD_LEN(res->length))
		return DSP_ERR_BAD_LEN;

	len = sample->length;
	for (i = -(len - 1); i < len; i++)
	{
		/* Shift and then multiply the signal. */
		status = dsp_time_shift(sample, i, &shifted);
		if (status != DSP_SUCCESS)
			return status;

		status = dsp_time_dot_mul(sample, &shifted, &multipled);
		if (status != DSP_SUCCESS)
			return status;

		for (j = 0; j < multipled.length; j++)
		{
			sum += multipled.data[j];
		}
		res->data[i + len - 1] = sum;
		sum = 0.0;
	}
	return DSP_SUCCESS;
}

/**
 * Correlate the `sample` negatively with itself in time domain.
 */
DspStatus dsp_time_auto_corr_neg(const DspTime *sample, DspTime *res)
{
	int i;
	DspTime correlated;
	DspStatus status;

	/* Validate the input parameters. */
	if (IS_BAD_SAMPLE(sample))
		return DSP_ERR_BAD_SAMPLE;

	status = dsp_time_auto_corr(sample, &correlated);
	if (status != DSP_SUCCESS)
		return status;

	res->length = sample->length;
	for (i = 0; i < res->length; i++)
	{
		res->data[i] = correlated.data[i];
	}
	return DSP_SUCCESS;
}

/**
 * Correlate the `sample` positively with itself in time domain.
 */
DspStatus dsp_time_auto_corr_pos(const DspTime *sample, DspTime *res)
{
	int i;
	DspTime correlated;
	DspStatus status;

	/* Validate the input parameters. */
	if (IS_BAD_SAMPLE(sample))
		return DSP_ERR_BAD_SAMPLE;

	status = dsp_time_auto_corr(sample, &correlated);
	if (status != DSP_SUCCESS)
		return status;

	res->length = sample->length;
	for (i = 0; i < res->length; i++)
	{
		res->data[i] = correlated.data[i + sample->length - 1];
	}
	return DSP_SUCCESS;
}

/**
 * Correlate the `fsample` (will be fixed) and `ssample` (will be shifted) 
 * fully in time domain.
 */
DspStatus dsp_time_cross_corr(const DspTime *fsample, const DspTime *ssample,
								 		DspTime *res)
{
	int i, j, len;
	DspTime shifted, multiplied;
	double sum = 0.0;
	DspStatus status;

	/* Validate the input parameters. */
	if (IS_BAD_SAMPLE(fsample) || IS_BAD_SAMPLE(ssample))
		return DSP_ERR_BAD_SAMPLE;
		
	if (IS_MISMATCH(fsample, ssample))
		return DSP_ERR_MISMATCH;

	len = fsample->length;
	res->length = fsample->length + ssample->length - 1;
	if (IS_BAD_LEN(res->length))
		return DSP_ERR_BAD_LEN;

	for (i = -(len - 1); i < len; i++)
	{
		/* Shift and then multiply the signal. */
		status = dsp_time_shift(ssample, i, &shifted);
		if (status != DSP_SUCCESS)
			return status;

		status = dsp_time_dot_mul(fsample, &shifted, &multiplied);
		if (status != DSP_SUCCESS)
			return status; 

		for (j = 0; j < multiplied.length; j++)
		{
			sum += multiplied.data[j];
		}
		res->data[i + len - 1] = sum;
		sum = 0.0;
	}
	return DSP_SUCCESS;
}

/**
 * Correlate the `fsample` (will be fixed) with `ssample` (will be shifted) 
 * negatively in time domain.
 */
DspStatus dsp_time_cross_corr_neg(const DspTime *fsample, const DspTime *ssample,
											 DspTime *res)
{
	int i;
	DspTime correlated;
	DspStatus status;

	/* Validate the input parameters. */
	if (IS_BAD_SAMPLE(fsample) || IS_BAD_SAMPLE(ssample))
		return DSP_ERR_BAD_SAMPLE;

	status = dsp_time_cross_corr(fsample, ssample, &correlated);
	if (status != DSP_SUCCESS)
		return status;

	res->length = (correlated.length / 2) + 1;
	for (i = 0; i < res->length; i++)
	{
		res->data[i] = correlated.data[i];
	}
	return DSP_SUCCESS;
}

/**
 * Correlate the `fsample` (will be fixed) with `ssample` (will be shifted) 
 * positively in time domain.
 */
DspStatus dsp_time_cross_corr_pos(const DspTime *fsample, const DspTime *ssample,
											 DspTime *res)
{
	int i;
	DspTime correlated;
	DspStatus status;

	/* Validate the input parameters. */
	if (IS_BAD_SAMPLE(fsample) || IS_BAD_SAMPLE(ssample))
		return DSP_ERR_BAD_SAMPLE;

	status = dsp_time_cross_corr(fsample, ssample, &correlated);
	if (status != DSP_SUCCESS)
		return status;

	res->length = (correlated.length / 2) + 1;
	for (i = 0; i < res->length; i++)
	{
		res->data[i] = correlated.data[i + (correlated.length / 2)];
	}
	return DSP_SUCCESS;
}
