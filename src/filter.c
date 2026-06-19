/**
 ******************************************************************************
 * @file 	filter.c
 * @author 	Can Gulmez
 * @brief 	FIR and IIR-based filter operations of DSP.
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

DspStatus __windowed_coeffs(double fc, double fs, int taps, DspTime *res)
{
	int i;
	double n;
	double wc = 2.0 * M_PI * fc / fs;
	DspTime coeffs;
	DspStatus status;

	coeffs.length = taps;
	/* Find the coefficients (taps) of the sinc filter. */
	for (i = 0; i < taps; i++)
	{
		n = i - (taps - 1.0) / 2.0;
		coeffs.data[i] = (fabs(n) < 1e-6) ? (wc / M_PI) : 
			(sin(wc * n) / (M_PI * n));
	}
	/* Apply the window to taper the coefficients. */
	status = dsp_window_blackman(&coeffs, res);
	if (status != DSP_SUCCESS)
		return status;

	return DSP_SUCCESS;
}

DspStatus __normalize_low_coeffs(const DspTime *sample, DspTime *res)
{
	double sum;
	DspStatus status;

	/* Normalize the low-filter coefficients. */
	sum = dsp_time_sum(sample);
	status = dsp_time_scalar_div(sample, sum, res);
	if (status != DSP_SUCCESS)
		return status;

	return DSP_SUCCESS;
}

DspStatus __normalize_high_coeffs(const DspTime *sample, DspTime *res)
{
	int i;
	double sum = 0.0;
	DspStatus status;

	for (i = 0; i < sample->length; i++)
	{
		sum += sample->data[i] * ((i % 2 == 0) ? (1.0) : (-1.0));
	}
	status = dsp_time_scalar_div(sample, sum, res);
	if (status != DSP_SUCCESS)
		return status;

	return DSP_SUCCESS;
}

DspStatus __normalized_low_coeffs(double fc, double fs, int taps, DspTime *res)
{
	DspTime windowed;
	DspStatus status;

	/* Create the windowed coefficients and then normalize them. */
	status = __windowed_coeffs(fc, fs, taps, &windowed);
	if (status != DSP_SUCCESS)
		return status;

	status = __normalize_low_coeffs(&windowed, res);
	if (status != DSP_SUCCESS)
		return status;
		
	return DSP_SUCCESS;
}

/*****************************************************************************/
/*****************************************************************************/

/**
 * Apply the FIR-based filter to `sample` according to `filter` choice 
 * in time domain.
 */
DspStatus dsp_filter_fir(const DspTime *sample, DspFirFilter filter, double fc, 
						 double fs, int taps, DspTime *res)
{
	int i;
	len_t offset;
	double sum = 0.0, n;
	double wc = 2.0 * M_PI * fc / fs;
	DspTime coeffs, windowed, normalized, convolved;
	DspStatus status;

	/* Validate the input parameters. */
	if (!IS_SAMPLE(sample))
		return DSP_ERR_SAMPLE;

	if (fc <= 0.0 || fs < fc * 2 || taps <= 1 || taps % 2 != 1)
		return DSP_ERR_PARAM;

	if (!IS_FIR_FILTER(filter))
		return DSP_ERR_FIR_FILTER;

	/* Calculate the windowed filter coefficients. */
	status = __windowed_coeffs(fc, fs, taps, &windowed);
	if (status != DSP_SUCCESS)
		return status;

	/* For high-pass filter, the coefficients must be inversed. */
	if (filter == DSP_FIR_FILTER_HIGH_PASS)
	{
		n = (taps - 1) / 2;
		for (i = 0; i < taps; i++)
		{
			windowed.data[i] = (i == n) ? (1.0 - windowed.data[i]) : 
				(-windowed.data[i]);
		}
	}
	/* Normalize the coefficients according to filter type. */
	if (filter == DSP_FIR_FILTER_LOW_PASS)
	{
		status = __normalize_low_coeffs(&windowed, &normalized);
		if (status != DSP_SUCCESS)
			return status;
	}
	else		/* DSP_FIR_FILTER_HIGH_PASS */
	{
		status = __normalize_high_coeffs(&windowed, &normalized);
		if (status != DSP_SUCCESS)
			return status;
	}
	/* Convolve the filter coefficients with `sample`. */
	status = dsp_time_convolve(sample, &normalized, &convolved);
	if (status != DSP_SUCCESS)
		return status;

	/* Keep the just first part of convolved sample. */
	res->length = sample->length;
	offset = (len_t) ((convolved.length - sample->length) / 2);
	for (i = 0; i < res->length; i++)
	{
		res->data[i] = convolved.data[offset + i];
	}
	return DSP_SUCCESS;
}

/**
 * Apply the FIR low-pass filter to `sample` in time domain.
 */
DspStatus dsp_filter_fir_low_pass(const DspTime *sample, double fc, double fs,
								  int taps, DspTime *res)
{
	return dsp_filter_fir(sample, DSP_FIR_FILTER_LOW_PASS, fc, fs, taps, res);
}

/**
 * Apply the FIR high-pass filter to `sample` in time domain.
 */
DspStatus dsp_filter_fir_high_pass(const DspTime *sample, double fc, double fs,
								   int taps, DspTime *res)
{
	return dsp_filter_fir(sample, DSP_FIR_FILTER_HIGH_PASS, fc, fs, taps, res);
}

/**
 * Apply the FIR band-pass filter to `sample` in time domain.
 */
DspStatus dsp_filter_fir_band_pass(const DspTime *sample, double fc1, double fc2,
								   double fs, int taps, DspTime *res)
{
	DspTime low, high;
	DspStatus status;

	status = dsp_filter_fir_low_pass(sample, fc2, fs, taps, &high);
	if (status != DSP_SUCCESS)
		return status;

	status = dsp_filter_fir_low_pass(sample, fc1, fs, taps, &low);
	if (status != DSP_SUCCESS)
		return status;

	return dsp_time_sub(&high, &low, res);
}

/**
 * Apply the FIR band-stop filter to `sample` in time domain.
 */ 
DspStatus dsp_filter_fir_band_stop(const DspTime *sample, double fc1, double fc2, 
								   double fs, int taps, DspTime *res)
{
	int i, n = (taps - 1) / 2;
	double delta;
	len_t offset;
	DspTime kernel1, kernel2, bandpass, bandstop, convolved;
	DspStatus status;

	if (!IS_SAMPLE(sample))
		return DSP_ERR_SAMPLE;

	if (fc1 <= 0.0 || fc2 <= 0.0 || fs <= 0.0 || fc1 >= fc2 || 
		 taps < 1 || taps % 2 != 1)
		return DSP_ERR_PARAM;

	/* Build the low-pass kernel for fc2. */
	status = __normalized_low_coeffs(fc2, fs, taps, &kernel2);
	if (status != DSP_SUCCESS)
		return status;	

	/* Build the low-pass kernel for fc1. */
	status = __normalized_low_coeffs(fc1, fs, taps, &kernel1);
	if (status != DSP_SUCCESS)
		return status;

	/* Build the band-pass kernel: kernel2 - kernel1. */
	status = dsp_time_sub(&kernel2, &kernel1, &bandpass);
	if (status != DSP_SUCCESS)
		return status;

	/* Build the band-stop kernel. */
	bandstop.length = taps;
	for (i = 0; i < taps; i++)
	{
		delta = (i == n) ? 1.0 : 0.0;
		bandstop.data[i] = delta - bandpass.data[i];
	}

	/* Apply the band-stop kernel coefficients to `sample`. */
	status = dsp_time_convolve(sample, &bandstop, &convolved);
	if (status != DSP_SUCCESS)
		return status;

	res->length = sample->length;
	offset = (convolved.length - sample->length) / 2;
	for (i = 0; i < res->length; i++)
	{
		res->data[i] = convolved.data[offset + i];
	}
	return DSP_SUCCESS;
}

/**
 * Apply IIR 2nd-order Butterworth filter to `sample` in time domain.
 */
DspStatus dsp_filter_iir(const DspTime *sample, DspIirFilter filter, double fc, 
						 double fs, double q, DspTime *res)
{
	int i;
	double w0 = 2.0 * M_PI * fc / fs;
	double alpha = sin(w0) / (2.0 * q);
	double a0, b0, b1, b2, a1, a2;
	double x1 = 0.0, x2 = 0.0, y1 = 0.0, y2 = 0.0;
	double x, y;

	/* Validate the input parameters. */
	if (!IS_SAMPLE(sample))
		return DSP_ERR_SAMPLE;

	if (!IS_IIR_FILTER(filter))
		return DSP_ERR_IIR_FILTER;

	if (fc <= 0.0 || fs < fc * 2 || q < 0.5 || q > 100.0)
		return DSP_ERR_PARAM;

	/* Calculate filter coefficients directly. */
	switch (filter)
	{
		case DSP_IIR_FILTER_LOW_PASS:
			b0 = (1.0 - cos(w0)) / 2.0; b1 = 1.0 - cos(w0);
      		b2 = b0; a0 = 1.0 + alpha;
      		a1 = -2.0 * cos(w0); a2 = 1.0 - alpha;
			break;

		case DSP_IIR_FILTER_HIGH_PASS:
			b0 = (1.0 + cos(w0)) / 2.0; b1 = -(1.0 + cos(w0));
      		b2 = b0; a0 = 1.0 + alpha;
      		a1 = -2.0 * cos(w0); a2 = 1.0 - alpha;
			break;

		case DSP_IIR_FILTER_BAND_PASS:
			b0 = alpha; b1 = 0.0; 
			b2 = -alpha; a0 = 1.0 + alpha; 
			a1 = -2.0 * cos(w0); a2 = 1.0 - alpha;
			break;

		case DSP_IIR_FILTER_BAND_STOP:
			b0 = 1.0; b1 = -2.0 * cos(w0);
      		b2 = 1.0; a0 = 1.0 + alpha;
      		a1 = -2.0 * cos(w0); a2 = 1.0 - alpha;
			break;
	}
	b0 /= a0; b1 /= a0; b2 /= a0; a1 /= a0; a2 /= a0;

	/* Apply filter coefficients to sample. */
	res->length = sample->length;
	for (i = 0; i < res->length; i++)
	{
		x = sample->data[i];
		y = b0 * x + b1 * x1 + b2 * x2 - a1 * y1 - a2 * y2;
		res->data[i] = y;
		
		/* Update the coefficients. */
		x2 = x1; x1 = x; y2 = y1; y1 = y;
	}
	return DSP_SUCCESS;
}

/**
 * Apply 2nd-order Butterworth low-pass filter to 'sample' in time domain.
 */
DspStatus dsp_filter_iir_low_pass(const DspTime *sample, double fc, double fs, 
								  DspTime *res)
{
	return dsp_filter_iir(sample, DSP_IIR_FILTER_LOW_PASS, fc, fs, 0.7071, res);
}

/**
 * Apply 2nd-order Butterworth high-pass filter to 'sample' in time domain.
 */
DspStatus dsp_filter_iir_high_pass(const DspTime *sample, double fc, double fs, 
								   DspTime *res)
{
	return dsp_filter_iir(sample, DSP_IIR_FILTER_HIGH_PASS, fc, fs, 0.7071, res);
}

/**
 * Apply 2nd-order Butterworth band-pass filter to 'sample' in time domain.
 */
DspStatus dsp_filter_iir_band_pass(const DspTime *sample, double fc, double fs, 
								   double q, DspTime *res)
{
	return dsp_filter_iir(sample, DSP_IIR_FILTER_BAND_PASS, fc, fs, q, res);
}

/**
 * Apply 2nd-order Butterworth band-stop filter to 'sample' in time domain.
 */
DspStatus dsp_filter_iir_band_stop(const DspTime *sample, double fc, double fs, 
								   double q, DspTime *res)
{
	return dsp_filter_iir(sample, DSP_IIR_FILTER_BAND_STOP, fc, fs, q, res);
}
 