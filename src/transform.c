/**
 ******************************************************************************
 * @file 	transform.c
 * @author 	Can Gulmez
 * @brief 	Time-frequency transformation operations of DSP.
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

unsigned int __bit_reverse(unsigned int x, int m)
{
	int i;
	unsigned int rev = 0;

	for (i = 0; i < m; i++)
	{
		rev = (rev << 1) | (x & 1);
		x >>= 1;
	}
	return rev;
}

/*****************************************************************************/
/*****************************************************************************/

/**
 * Apply the full discrete fourier transformation to `sample`
 * in time-to-frequency domain.
 */
DspStatus dsp_transform_dft(const DspTime *sample, DspFreq *res)
{
	int i, j;
	double real, imag, inner;

	/* Validate the input parameters. */
	if (!IS_SAMPLE(sample))
		return DSP_ERR_SAMPLE;

	res->length = sample->length;
	inner = 2 * M_PI / res->length;
	for (i = 0; i < res->length; i++)
	{
		real = 0.0;
		imag = 0.0;
		for (j = 0; j < res->length; j++)
		{
			real += sample->data[j] * cos(inner * i * j);
			imag -= sample->data[j] * sin(inner * i * j);
		}
		res->data[i][0] = real;
		res->data[i][1] = imag;
	}
	return DSP_SUCCESS;
}

/**
 * Apply the real-valued discrete fourier transformation to `sample`
 * in time-to-frequency domain.
 */
DspStatus dsp_transform_dft_real(const DspTime *sample, DspFreq *res)
{
	int i;
	DspFreq transformed;

	/* Validate the input parameters. */
	if (!IS_SAMPLE(sample))
		return DSP_ERR_SAMPLE;

	dsp_transform_dft(sample, &transformed);
	res->length = (len_t) (transformed.length / 2 + 1);
	for (i = 0; i < res->length; i++)
	{
		res->data[i][0] = transformed.data[i][0];
		res->data[i][1] = transformed.data[i][1];
	}
	return DSP_SUCCESS;
}

/**
 * Apply the inverse discrete fourier transformation to `sample`
 * in frequency-to-time domain.
 */
DspStatus dsp_transform_idft(const DspFreq *sample, DspTime *res)
{
	int i, j;
	double sum = 0.0;
	
	/* Validate the input parameters. */
	if (!IS_SAMPLE(sample))
		return DSP_ERR_SAMPLE;

	res->length = sample->length;
	for (i = 0; i < res->length; i++)
	{
		for (j = 0; j < res->length; j++)
		{
			sum += sample->data[j][0] * 
				   cos(2 * M_PI * i * j / res->length) - 
				   sample->data[j][1] * 
				   sin(2 * M_PI * i * j / res->length);
		}
		res->data[i] = (double) sum / res->length;
		sum = 0.0;
	}
	return DSP_SUCCESS;
}

/**
 * Apply the Radix-2 fast fourier transformation to `sample`
 * in time-to-frequency domain.
 */
DspStatus dsp_transform_fft(const DspTime *sample, DspFreq *res)
{
	int n = sample->length;
	int m = 0, i, j, s, k;
	int half, step, idx_a, idx_b;
	unsigned int rev;
	double re[DATA_SIZE], im[DATA_SIZE];
	double angle, w_re, w_im, t_re, t_im;
	DspStatus status;

	/* Validate the input parameters. */
	if (!IS_SAMPLE(sample))
		return DSP_ERR_SAMPLE;

	if (sample->length % 2 != 0)
		return DSP_ERR_PARAM;

	if ((n & (n - 1)) != 0)
		return DSP_ERR_LENGTH;

	/* m = log2(n) */
	while ((1 << m) < n) m++;

	/* Copy input and apply bit-reversal permutation. */
	for (i = 0; i < n; i++)
	{
		rev = __bit_reverse(i, m);
		re[rev] = sample->data[i];
		im[rev] = 0.0;
	}
	/* Butterfly computations - stage by stage. */
	for (s = 1; s <= m; s++)
	{
		half = 1 << (s - 1);				/* half block size */
		step = 1 << s;						/* full block size */
		angle = -2.0 * M_PI / step;			/* twiddle factor step */

		for (j = 0; j < n; j += step)
		{
			for (k = 0; k < half; k++)
			{
				w_re = cos(angle * k);
				w_im = sin(angle * k);
				idx_a = j + k;
				idx_b = j + k + half;
				t_re = w_re * re[idx_b] - w_im * im[idx_b];
				t_im = w_re * im[idx_b] + w_im * re[idx_b];
				re[idx_b] = re[idx_a] - t_re;
				im[idx_b] = im[idx_a] - t_im;
				re[idx_a] = re[idx_a] + t_re;
				im[idx_a] = im[idx_a] + t_im;
			}
		}
	}
	/* Write the result. */
	res->length = n;
	for (i = 0; i < n; i++)
	{
		res->data[i][0] = re[i];
		res->data[i][1] = im[i];
	}
	return DSP_SUCCESS;
}

/**
 * Apply the real-valued Radix-2 iverse fourier transformation to 
 * `sample` in time-to-frequency domain.
 */
DspStatus dsp_transform_fft_real(const DspTime *sample, DspFreq *res)
{
	int i;
	DspFreq transformed;

	/* Validate the input parameters. */
	if (!IS_SAMPLE(sample))
		return DSP_ERR_SAMPLE;

	dsp_transform_fft(sample, &transformed);
	res->length = (len_t) (transformed.length / 2 + 1);
	for (i = 0; i < res->length; i++)
	{
		res->data[i][0] = transformed.data[i][0];
		res->data[i][1] = transformed.data[i][1];
	}
	return DSP_SUCCESS;
}

/**
 * Apply the inverse Radix-2 fast fourier transformation to 
 * `sample` in frequency-to-time domain.
 */
DspStatus dsp_transform_ifft(const DspFreq *sample, DspTime *res)
{
	int n = sample->length;
	int m = 0, i, j, s, k;
	int half, step, idx_a, idx_b;
	unsigned int rev;
	double re[DATA_SIZE], im[DATA_SIZE];
	double angle, w_re, w_im, t_re, t_im;
	DspStatus status;

	/* Validate the input parameters. */
	if (!IS_SAMPLE(sample))
		return DSP_ERR_SAMPLE;

	if ((n & (n - 1)) != 0)
		return DSP_ERR_LENGTH;

	/* m = log2(n) */
	while ((1 << m) < n) m++;

	/* Bit-reverse the complex input. */
	for (i = 0; i < n; i++)
	{
		rev = __bit_reverse(i, m);
		re[rev] = sample->data[i][0];
		im[rev] = sample->data[i][1];
	}
	/* Butterfly loops with positive angle. */
	for (s = 1; s <= m; s++)
	{
		half = 1 << (s - 1);
		step = 1 << s;
		angle = 2.0 * M_PI / step;

		for (j = 0; j < n; j += step)
		{
			for (k = 0; k < half; k++)
			{
				w_re = cos(angle * k);
				w_im = sin(angle * k);
				idx_a = j + k;
				idx_b = j + k + half;
				t_re = w_re * re[idx_b] - w_im * im[idx_b];
				t_im = w_re * im[idx_b] + w_im * re[idx_b];
				re[idx_b] = re[idx_a] - t_re;
				im[idx_b] = im[idx_a] - t_im;
				re[idx_a] = re[idx_a] + t_re;
				im[idx_a] = im[idx_a] + t_im;
			}
		}
	}
	/* Scale by 1/N and write to result. */
	res->length = n;
	for (i = 0; i < n; i++)
	{
		res->data[i] = re[i] / n;
	}
	return DSP_SUCCESS;
}
