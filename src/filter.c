/**
 ******************************************************************************
 * @file 	filter.c
 * @author 	Ahmet Can GULMEZ
 * @brief 	FIR and IIR-based filter operations of DSP.
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
 * Apply the FIR low-pass filter to `sample` sequence in time domain.
 */
void dsp_filter_fir_low_pass(const DspTime *sample, double fc, double fs, 
									  int taps, DspTime *result)
{
	int i, offset;
	double sum, wc, n;
	DspTime coeffs, convolved;

	/* Validate the inputs. */
	assert_sample(sample);
	assert(fc > 0 && fc < fs / 2);
	assert(taps > 0 && taps % 2 == 1);

	coeffs.length = taps;
	wc = 2.0 * M_PI * fc / fs;
	/* Find the coefficients (taps) to convolve. */
	for (i = 0; i < taps; i++)
	{
		n = i - (taps - 1.0) / 2.0;
		if (fabs(n) < 1e-6)
		{
			coeffs.data[i] = wc / M_PI;
		}
		else
		{
			coeffs.data[i] = sin(wc * n) / (M_PI * n);
		}
	}
	/* Apply the window to taper the coefficients. */
	dsp_window_blackman(&coeffs, &coeffs);

	/* Normalize the coefficients. */
	sum = 0;
	for (i = 0; i < taps; i++)
	{
		sum += coeffs.data[i];
	}
	for (i = 0; i < taps; i++)
	{
		coeffs.data[i] /= sum;
	}

	/* Convolve the filter coefficients with `sample`. */
	dsp_time_convolve(sample, &coeffs, &convolved);
	result->length = sample->length;
	offset = (int) (convolved.length - sample->length) / 2;
	for (i = 0; i < sample->length; i++)
	{
		result->data[i] = convolved.data[offset + i];
	}
}

/**
 * Apply the FIR high-pass filter to `sample` sequence in time domain.
 */
void dsp_filter_fir_high_pass(const DspTime *sample, double fc, double fs, 
									   int taps, DspTime *result)
{
	int i, offset;
	double n, wc;
	DspTime coeffs, convolved;

	/* Validate the inputs. */
	assert_sample(sample);
	assert(fc > 0 && fc < fs / 2);
	assert(taps > 0 && taps % 2 == 1);

	coeffs.length = taps;
	wc = 2.0 * M_PI * fc / fs;
	/* Find the coefficients (taps) to convolve. */
	for (i = 0; i < taps; i++)
	{
		n = i - (taps - 1.0) / 2.0;
		if (fabs(n) < 1e-6)
		{
			coeffs.data[i] = 1.0 - (wc / M_PI);
		}
		else
		{
			coeffs.data[i] = -sin(wc * n) / (M_PI * n);
		}
	}
	/* Apply the window to taper the coefficients. */
	dsp_window_blackman(&coeffs, &coeffs);

	/* Convolve the filter coefficients with `sample`. */
	dsp_time_convolve(sample, &coeffs, &convolved);
	result->length = sample->length;
	offset = (int) (convolved.length - sample->length) / 2;
	for (i = 0; i < sample->length; i++)
	{
		result->data[i] = convolved.data[offset + i];
	}	
}

/**
 * Apply the FIR band-pass filter to `sample` sequence in time domain.
 */
void dsp_filter_fir_band_pass(const DspTime *sample, double fc1, double fc2, double fs, 
								  	   int taps, DspTime *result)
{
	DspTime low1, low2;

	dsp_filter_fir_low_pass(sample, fc2, fs, taps, &low2);
	dsp_filter_fir_low_pass(sample, fc1, fs, taps, &low1);
	dsp_time_subtract(&low2, &low1, result);
}

/**
 * Apply the FIR band-stop filter to `sample` sequence in time domain.
 */   
void dsp_filter_fir_band_stop(const DspTime *sample, double fc1, double fc2, double fs, 
									   int taps, DspTime *result)
{
	int i;
	DspTime passed, impulse;

	impulse.length = sample->length;
	for (i = 0; i < impulse.length; i++)
	{
		impulse.data[i] = (i == 0) ? 1.0 : 0.0;
	}

	dsp_filter_fir_band_pass(sample, fc1, fc2, fs, taps, &passed);
	dsp_time_subtract(&impulse, &passed, result);
}

/**
 * Apply IIR 2nd-order Butterworth I filter to `sample` sequence in time domain.
 */
void dsp_filter_iir(const DspTime *sample, DspFilter filter, double fc, double fs, 
						  double Q, DspTime *result)
{
	int i;
	double w0, alpha, a0;
	double b0, b1, b2, a1, a2;
	double x1 = 0, x2 = 0, y1 = 0, y2 = 0;
	double x, y;

	/* Validate the inputs. */
	assert_sample(sample);
	assert(fc > 0 && fc < fs / 2);
	assert(Q > 0.5 && Q < 100.0);

	/* 2nd-order Butterworth coefficient. */
	x1 = 0, x2 = 0, y1 = 0, y2 = 0;
	w0 = 2.0 * M_PI * fc / fs;	
	alpha = sin(w0) / (2.0 * Q);

	/* Calculate coefficients directly. */
	switch (filter)
	{
		case DSP_FILTER_LOW_PASS:
			a0 = 1.0 + alpha;
			b0 = (1.0 - cos(w0)) / (2.0 * a0);
			b1 = (1.0 - cos(w0)) / a0;
			b2 = b0;
			a1 = -2.0 * cos(w0) / a0;
			a2 = (1.0 - alpha) / a0;
			break;

		case DSP_FILTER_HIGH_PASS:
			a0 = 1.0 + alpha;
			b0 = (1.0 + cos(w0)) / (2.0 * a0);
			b1 = -(1.0 + cos(w0)) / a0;
			b2 = b0;
			a1 = -2.0 * cos(w0) / a0;
			a2 = (1.0 - alpha) / a0;	
			break;

		case DSP_FILTER_BAND_PASS:
			a0 = 1.0 + alpha;
			b0 = alpha / a0;
			b1 = 0;
			b2 = -alpha / a0;
			a1 = -2.0 * cos(w0) / a0;
			a2 = (1.0 - alpha) / a0;
			break;	

		case DSP_FILTER_BAND_STOP:
			a0 = 1.0 + alpha;
			b0 = 1.0 / a0;
			b1 = -2.0 * cos(w0) / a0;
			b2 = b0;
			a1 = -2.0 * cos(w0) / a0;
			a2 = (1.0 - alpha) / a0;
			break;
		
		default:
			break;
	}
	/* Normalize by a0i */
	b0 /= a0; b1 /= a0; b2 /= a0; a1 /= a0; a2 /= a0;

	/* Apply filter. */
	result->length = sample->length;
	for (i = 0; i < result->length; i++)
	{
		x = sample->data[i];
		y = b0 * x + b1 * x1 + b2 * x2 - a1 * y1 - a2 * y2;
		result->data[i] = y;
		
		/* Update the coefficients. */
		x2 = x1; x1 = x; y2 = y1; y1 = y;
	}	
}

/**
 * Apply 2nd-order Butterworth low-pass filter to 'sample' sequence
 * in time domain.
 */
void dsp_filter_iir_low_pass(const DspTime *sample, double fc, double fs,
									  DspTime *result)
{
	dsp_filter_iir(sample, DSP_FILTER_LOW_PASS, fc, fs, 0.7071, result);
}

/**
 * Apply 2nd-order Butterworth high-pass filter to 'sample' sequence
 * in time domain.
 */
void dsp_filter_iir_high_pass(const DspTime *sample, double fc, double fs, 
										DspTime *result)
{
	dsp_filter_iir(sample, DSP_FILTER_HIGH_PASS, fc, fs, 0.7071, result);
}

/**
 * Apply 2nd-order Butterworth band-pass filter to 'sample' sequence
 * in time domain.
 */
void dsp_filter_iir_band_pass(const DspTime *sample, double fc, double fs, 
										double Q, DspTime *result)
{
	dsp_filter_iir(sample, DSP_FILTER_BAND_PASS, fc, fs, Q, result);
}

/**
 * Apply 2nd-order Butterworth band-stop filter to 'sample' sequence
 * in time domain.
 */
void dsp_filter_iir_band_stop(const DspTime *sample, double fc, double fs, 
										double Q, DspTime *result)
{
	dsp_filter_iir(sample, DSP_FILTER_BAND_STOP, fc, fs, Q, result);
}

/**
 * Remove DC offset and slow drift from `sample` sequence in time domain.
 */
void dsp_filter_dc_block(const DspTime *sample, double fc, double fs, 
								 DspTime *result)
{
	int i;
	double w0;
	double b0, b1, a1;
	double x, y, x1 = 0, y1 = 0;

	/* 1st-order IIR coefficients. */
	w0 = 2.0 * M_PI * fc / fs;
	b0 = 1.0 / (1.0 + w0);
	b1 = -b0;
	a1 = (1.0 - w0) / (1.0 + w0);

	result->length = sample->length;
	for (i = 0; i < result->length; i++)
	{
		x = sample->data[i];
		y = b0 * x + b1 * x1 - a1 * y1;
		result->data[i] = y;

		/* Update the coefficients. */
		x1 = x, y1 = y;
	}
}
