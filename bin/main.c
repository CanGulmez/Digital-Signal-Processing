/**
 ******************************************************************************
 * @file 	main.c
 * @author 	Can GULMEZ
 * @brief 	Coomand Line Interface of DSP.
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

#include "../src/dsp.h"

#define cmp(x, y)		(strcmp(x, y) == 0)

/**
 * Parse the time-domain sample command.
 */
void t(const char *command, DspTime *result)
{
	int i;

	i = 0;
	while (TRUE)
	{
		result->data[i] = atof(command);
		i ++;
		if (strchr(command, ',') == NULL)
		{
			break;
		}
		command = strchr(command, ',');
		command = strchr(command, command[1]);
	}
	result->length = i;
}

/**
 * Display the time-domain sample.
 */
void dt(const DspTime *sample)
{
	int i;

	for (i = 0; i < sample->length - 1; i++)
	{
		printf("%.4f,", sample->data[i]);
	}
	printf("%.4f\n", sample->data[sample->length - 1]);
}

/**
 * Parse the frequency-domain sample.
 */
void f(const char *command, DspFreq *result)
{
	int i, j;

	i = 0; j = 0;
	while (TRUE)
	{
		result->data[i][j] = atof(command);
		command = strchr(command, ':');
		command = strchr(command, command[1]);
		j++;
		result->data[i][j] = atof(command);
		i++;
		if (strchr(command, ',') == NULL)
		{
			break;
		}
		command = strchr(command, ',');
		command = strchr(command, command[1]);
		j = 0;
	}
	result->length = i;
}

/**
 * Display the frequency-domain sample.
 */
void df(const DspFreq *sample)
{
	int i, j;

	for (i = 0; i < sample->length - 1; i++)
	{
		printf("%.4f:%.4f,", sample->data[i][0],
			sample->data[i][1]);
	}
	printf("%.4f:%.4f\n", 
		sample->data[sample->length - 1][0],
		sample->data[sample->length - 1][1]);
}

void usage_error(const char *progname)
{
   fprintf(stderr, "Usage: %s module method [PARAMS]...\n\n", progname);
   fprintf(stderr, "\
This toolkit was designed for digital signal processing task automations.\n\
NOTE: Please follow source code implementations to use from command-line.\n\n");
   fprintf(stderr, "General imlementation rules:\n");
   fprintf(stderr, "- Use comma to indicate time domain samples, e.g., 7,-3,1.5 (3 samples).\n");
   fprintf(stderr, "- Use comma and colon to indicate frequency domain samples, e.g., 3:1,6:-3\n\
  (3 + j, 6 - 3j).\n");
   fprintf(stderr, "- Don't use spaces when indicating time or frequency domain samples.\n\n");
   fprintf(stderr, "Some usage examples:\n");
   fprintf(stderr, "- ./dsp time convolve 7,1,-2,4,2.5 3,-4,-7,8,6  // Convolve two signals\n");
   fprintf(stderr, "- ./dsp freq magnitude_max 6:-2,1:4,3:5,8,4     // Find max magnitude\n");
   fprintf(stderr, "- ./dsp signal sin 2 100 1000 0 128             // Generate sine signal\n");
   fprintf(stderr, "- ./dsp windowing blackman 7,3,1,4,-5,6,8.1     // Apply Blackman window\n");
   fprintf(stderr, "- ./dsp transform dft 3,4,5,2,-5                // Transform into freq.\n");
   fprintf(stderr, "About maintainer:\n");
   fprintf(stderr, "- Author: Ahmet Can GULMEZ\n");
   fprintf(stderr, "- E-Main: ahmetcangulmez02@gmail.com\n");
   fprintf(stderr, "- Version: 2.0.0\n");
   fprintf(stderr, "- License: MIT License\n");
} 

int main(int argc, char *argv[])
{
	char command[64];
	snprintf(command, 64, "dsp_%s_%s", argv[1], argv[2]);

	DspTime tfsample;
	DspTime tssample;
	DspTime ttsample;
	DspTime tresult;
	DspFreq ffsample;
	DspFreq fssample;
	DspFreq fresult;
	double scalar, delay, data, scale, threshold;
	double min_val, max_val, fs, mean, stddev, snr;
	double fsample, ssample, fc1, fc2, A, fc;
	double theta, real, imag, Q;
	len_t length;
	int shift, index, bins, factor, taps, ret;

	/* Time Domain Methods  */

	if (cmp(command, "dsp_time_add"))
	{
		t(argv[3], &tfsample);
		t(argv[4], &tssample);
		dsp_time_add(&tfsample, &tssample, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_time_subtract"))
	{
		t(argv[3], &tfsample);
		t(argv[4], &tssample);
		dsp_time_subtract(&tfsample, &tssample, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_time_scalar_mul"))
	{
		t(argv[3], &tfsample);
		scalar = atof(argv[4]);
		dsp_time_scalar_mul(&tfsample, scalar, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_time_dot_mul"))
	{
		t(argv[3], &tfsample);
		t(argv[4], &tssample);
		dsp_time_dot_mul(&tfsample, &tssample, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_time_scalar_div"))
	{
		t(argv[3], &tfsample);
		scalar = atof(argv[4]);
		dsp_time_scalar_mul(&tfsample, scalar, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_time_resize"))
	{
		t(argv[3], &tfsample);
		length = atoi(argv[4]);
		dsp_time_resize(&tfsample, length, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_time_insert"))
	{
		t(argv[3], &tfsample);
		index = atoi(argv[4]);
		data = atof(argv[5]);
		dsp_time_insert(&tfsample, index, data, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_time_remove"))
	{
		t(argv[3], &tfsample);
		index = atoi(argv[4]);
		dsp_time_remove(&tfsample, index, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_time_append"))
	{
		t(argv[3], &tfsample);
		data = atof(argv[4]);
		dsp_time_append(&tfsample, data, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_time_pop"))
	{
		t(argv[3], &tfsample);
		dsp_time_pop(&tfsample, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_time_shift"))
	{
		t(argv[3], &tfsample);
		shift = atoi(argv[4]);
		dsp_time_shift(&tfsample, shift, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_time_delay_linear"))
	{
		t(argv[3], &tfsample);
		delay = atof(argv[4]);
		dsp_time_delay_linear(&tfsample, delay, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_time_delay_lagrange"))
	{
		t(argv[3], &tfsample);
		delay = atof(argv[4]);
		dsp_time_delay_lagrange(&tfsample, delay, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_time_concat"))
	{
		t(argv[3], &tfsample);
		t(argv[4], &tssample);
		dsp_time_concat(&tfsample, &tssample, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_time_abs"))
	{
		t(argv[3], &tfsample);
		dsp_time_abs(&tfsample, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_time_rand"))
	{
		length = atoi(argv[3]);
		dsp_time_rand(length, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_time_randn"))
	{
		length = atoi(argv[3]);
		dsp_time_rand(length, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_time_length"))
	{
		t(argv[3], &tfsample);
		length = dsp_time_length(&tfsample);
		printf("%d\n", length);
	}
	else if (cmp(command, "dsp_time_data"))
	{
		t(argv[3], &tfsample);
		index = atoi(argv[4]);
		data = dsp_time_data(&tfsample, index);
		printf("%.4f\n", data);
	}
	else if (cmp(command, "dsp_time_index"))
	{
		t(argv[3], &tfsample);
		data = atof(argv[4]);
		index = dsp_time_index(&tfsample, data);
		printf("%.d\n", index);
	}
	else if (cmp(command, "dsp_time_max"))
	{
		t(argv[3], &tfsample);
		data = dsp_time_max(&tfsample);
		printf("%.4f\n", data);
	}
	else if (cmp(command, "dsp_time_min"))
	{
		t(argv[3], &tfsample);
		data = dsp_time_min(&tfsample);
		printf("%.4f\n", data);
	}
	else if (cmp(command, "dsp_time_abs_max"))
	{
		t(argv[3], &tfsample);
		data = dsp_time_abs_max(&tfsample);
		printf("%.4f\n", data);
	}
	else if (cmp(command, "dsp_time_abs_min"))
	{
		t(argv[3], &tfsample);
		data = dsp_time_abs_min(&tfsample);
		printf("%.4f\n", data);
	}
	else if (cmp(command, "dsp_time_argmax"))
	{
		t(argv[3], &tfsample);
		index = dsp_time_argmax(&tfsample);
		printf("%d\n", index);
	}
	else if (cmp(command, "dsp_time_argmin"))
	{
		t(argv[3], &tfsample);
		index = dsp_time_argmin(&tfsample);
		printf("%d\n", index);
	}
	else if (cmp(command, "dsp_time_mean"))
	{
		t(argv[3], &tfsample);
		data = dsp_time_mean(&tfsample);
		printf("%.4f\n", data);
	}
	else if (cmp(command, "dsp_time_stddev"))
	{
		t(argv[3], &tfsample);
		data = dsp_time_stddev(&tfsample);
		printf("%.4f\n", data);
	}
	else if (cmp(command, "dsp_time_energy"))
	{
		t(argv[3], &tfsample);
		data = dsp_time_energy(&tfsample);
		printf("%.4f\n", data);
	}
	else if (cmp(command, "dsp_time_power"))
	{
		t(argv[3], &tfsample);
		data = dsp_time_power(&tfsample);
		printf("%.4f\n", data);
	}
	else if (cmp(command, "dsp_time_rms"))
	{
		t(argv[3], &tfsample);
		data = dsp_time_rms(&tfsample);
		printf("%.4f\n", data);
	}
	else if (cmp(command, "dsp_time_crest_factor"))
	{
		t(argv[3], &tfsample);
		data = dsp_time_crest_factor(&tfsample);
		printf("%.4f\n", data);
	}
	else if (cmp(command, "dsp_time_skewness"))
	{
		t(argv[3], &tfsample);
		data = dsp_time_skewness(&tfsample);
		printf("%.4f\n", data);
	}
	else if (cmp(command, "dsp_time_kurtosis"))
	{
		t(argv[3], &tfsample);
		data = dsp_time_kurtosis(&tfsample);
		printf("%.4f\n", data);
	}
	else if (cmp(command, "dsp_time_variance"))
	{
		t(argv[3], &tfsample);
		data = dsp_time_variance(&tfsample);
		printf("%.4f\n", data);
	}
	else if (cmp(command, "dsp_time_snr"))
	{
		t(argv[3], &tfsample);
		t(argv[4], &tssample);
		data = dsp_time_snr(&tfsample, &tssample);
		printf("%.4f\n", data);
	}
	else if (cmp(command, "dsp_time_entropy"))
	{
		t(argv[3], &tfsample);
		bins = atoi(argv[4]);
		data = dsp_time_entropy(&tfsample, bins);
		printf("%.4f\n", data);
	}
	else if (cmp(command, "dsp_time_scale"))
	{
		t(argv[3], &tfsample);
		scale = atof(argv[4]);
		dsp_time_scale(&tfsample, scale, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_time_downsample"))
	{
		t(argv[3], &tfsample);
		factor = atoi(argv[4]);
		dsp_time_downsample(&tfsample, factor, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_time_upsample"))
	{
		t(argv[3], &tfsample);
		factor = atoi(argv[4]);
		dsp_time_upsample(&tfsample, factor, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_time_peaks"))
	{
		t(argv[3], &tfsample);
		threshold = atof(argv[4]);
		dsp_time_peaks(&tfsample, threshold, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_time_clip"))
	{
		t(argv[3], &tfsample);
		min_val = atoi(argv[4]);
		max_val = atoi(argv[5]);
		dsp_time_clip(&tfsample, min_val, max_val, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_time_convolve"))
	{
		t(argv[3], &tfsample);
		t(argv[4], &tssample);
		dsp_time_convolve(&tfsample, &tssample, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_time_auto_corr"))
	{
		t(argv[3], &tfsample);
		dsp_time_auto_corr(&tfsample, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_time_auto_corr_neg"))
	{
		t(argv[3], &tfsample);
		dsp_time_auto_corr_neg(&tfsample, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_time_auto_corr_pos"))
	{
		t(argv[3], &tfsample);
		dsp_time_auto_corr_pos(&tfsample, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_time_cross_corr"))
	{
		t(argv[3], &tfsample);
		t(argv[4], &tssample);
		dsp_time_cross_corr(&tfsample, &tssample, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_time_cross_corr_neg"))
	{
		t(argv[3], &tfsample);
		t(argv[4], &tssample);
		dsp_time_cross_corr_neg(&tfsample, &tssample, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_time_cross_corr_pos"))
	{
		t(argv[3], &tfsample);
		t(argv[4], &tssample);
		dsp_time_cross_corr_pos(&tfsample, &tssample, &tresult);
		dt(&tresult);
	}

	/* Frequency Domain Methods */

	if (cmp(command, "dsp_freq_add"))
	{
		f(argv[3], &ffsample);
		f(argv[4], &fssample);
		dsp_freq_add(&ffsample, &fssample, &fresult);
		df(&fresult);
	}
	else if (cmp(command, "dsp_freq_subtract"))
	{
		f(argv[3], &ffsample);
		f(argv[4], &fssample);
		dsp_freq_subtract(&ffsample, &fssample, &fresult);
		df(&fresult);
	}
	else if (cmp(command, "dsp_freq_scalar_mul"))
	{
		f(argv[3], &ffsample);
		scalar = atof(argv[4]);
		dsp_freq_scalar_mul(&ffsample, scalar, &fresult);
		df(&fresult);
	}
	else if (cmp(command, "dsp_freq_dot_mul"))
	{
		f(argv[3], &ffsample);
		f(argv[4], &fssample);
		dsp_freq_dot_mul(&ffsample, &fssample, &fresult);
		df(&fresult);
	}
	else if (cmp(command, "dsp_freq_scalar_div"))
	{
		f(argv[3], &ffsample);
		scalar = atof(argv[4]);
		dsp_freq_scalar_mul(&ffsample, scalar, &fresult);
		df(&fresult);
	}
	else if (cmp(command, "dsp_freq_resize"))
	{
		f(argv[3], &ffsample);
		length = atoi(argv[4]);
		dsp_freq_resize(&ffsample, length, &fresult);
		df(&fresult);
	}
	else if(cmp(command, "dsp_freq_insert"))
	{
		f(argv[3], &ffsample);
		real = atof(argv[4]);
		imag = atof(argv[5]);
		index = atoi(argv[6]);
		dsp_freq_insert(&ffsample, real, imag, index, &fresult);
		df(&fresult);
	}
	else if (cmp(command, "dsp_freq_remove"))
	{
		f(argv[3], &ffsample);
		index = atoi(argv[4]);
		dsp_freq_remove(&ffsample, index, &fresult);
		df(&fresult);
	}
	else if (cmp(command, "dsp_freq_append"))
	{
		f(argv[3], &ffsample);
		real = atof(argv[4]);
		imag = atof(argv[5]);
		dsp_freq_append(&ffsample, real, imag, &fresult);
		df(&fresult);
	}
	else if (cmp(command, "dsp_freq_pop"))
	{
		f(argv[3], &ffsample);
		dsp_freq_pop(&ffsample, &fresult);
		df(&fresult);
	}
	else if (cmp(command, "dsp_freq_concat"))
	{
		f(argv[3], &ffsample);
		f(argv[4], &fssample);
		dsp_freq_concat(&ffsample, &fssample, &fresult);
		df(&fresult);
	}
	else if (cmp(command, "dsp_freq_length"))
	{
		f(argv[3], &ffsample);
		length = dsp_freq_length(&ffsample);
		printf("%d\n", length);
	}
	else if (cmp(command, "dsp_freq_real"))
	{
		f(argv[3], &ffsample);
		dsp_freq_real(&ffsample, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_freq_imag"))
	{
		f(argv[3], &ffsample);
		dsp_freq_imag(&ffsample, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_freq_magnitude"))
	{
		f(argv[3], &ffsample);
		dsp_freq_magnitude(&ffsample, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_freq_phase"))
	{
		f(argv[3], &ffsample);
		dsp_freq_phase(&ffsample, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_freq_psd"))
	{
		f(argv[3], &ffsample);
		index = atoi(argv[4]);
		data = dsp_freq_psd(&ffsample, index);
		printf("%.4f\n", data);
	}
	else if (cmp(command, "dsp_freq_power"))
	{
		f(argv[3], &ffsample);
		data = dsp_freq_power(&ffsample);
		printf("%.4f\n", data);
	}
	else if (cmp(command, "dsp_freq_spectral_centroid"))
	{
		f(argv[3], &ffsample);
		fs = atof(argv[4]);
		data = dsp_freq_spectral_centroid(&ffsample, fs);
		printf("%.4f\n", data);
	}
	else if (cmp(command, "dsp_freq_spectral_flatness"))
	{
		f(argv[3], &ffsample);
		data = dsp_freq_spectral_flatness(&ffsample);
		printf("%.4f\n", data);
	}
	else if (cmp(command, "dsp_freq_spectral_rolloff"))
	{
		f(argv[3], &ffsample);
		threshold = atof(argv[4]);
		fs = atof(argv[5]);
		data = dsp_freq_spectral_rolloff(&ffsample, threshold, fs);
		printf("%.4f\n", data);
	}
	else if (cmp(command, "dsp_freq_thd"))
	{
		f(argv[3], &ffsample);
		fs = atof(argv[4]);
		data = dsp_freq_thd(&ffsample, fs);
		printf("%.4f\n", data);
	}

	/* Signal Generation Methods */

	else if (cmp(command, "dsp_signal_normal"))
	{
		mean = atof(argv[3]);
		stddev = atof(argv[4]);
		length = atoi(argv[5]);
		dsp_signal_normal(mean, stddev, length, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_signal_awgn"))
	{
		t(argv[3], &tfsample);
		snr = atof(argv[4]);
		dsp_signal_awgn(&tfsample, snr, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_signal_sin"))
	{
		A = atof(argv[3]);
		fc = atof(argv[4]);
		fs = atof(argv[5]);
		theta = atof(argv[6]);
		length = atoi(argv[7]);
		dsp_signal_sin(A, fc, fs, theta, length, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_signal_sinc"))
	{
		A = atof(argv[3]);
		fc = atof(argv[4]);
		fs = atof(argv[5]);
		theta = atof(argv[6]);
		length = atoi(argv[7]);
		dsp_signal_sinc(A, fc, fs, theta, length, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_signal_cos"))
	{
		A = atof(argv[3]);
		fc = atof(argv[4]);
		fs = atof(argv[5]);
		theta = atof(argv[6]);
		length = atoi(argv[7]);
		dsp_signal_cos(A, fc, fs, theta, length, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_signal_impulse"))
	{
		index = atoi(argv[3]);
		length = atoi(argv[4]);
		dsp_signal_impulse(index, length, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_signal_step"))
	{
		A = atoi(argv[3]);
		fsample = atoi(argv[4]);
		ssample = atoi(argv[5]);
		length = atoi(argv[6]);
		dsp_signal_step(A, fsample, ssample, length, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_signal_square"))
	{
		A = atof(argv[3]);
		fc = atof(argv[4]);
		fs = atof(argv[5]);
		length = atoi(argv[7]);
		dsp_signal_square(A, fc, fs, length, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_signal_sawtooth"))
	{
		A = atof(argv[3]);
		fc = atof(argv[4]);
		fs = atof(argv[5]);
		length = atoi(argv[7]);
		dsp_signal_sawtooth(A, fc, fs, length, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_signal_triangle"))
	{
		A = atof(argv[3]);
		fc = atof(argv[4]);
		fs = atof(argv[5]);
		length = atoi(argv[7]);
		dsp_signal_triangle(A, fc, fs, length, &tresult);
		dt(&tresult);
	}

	/* Windowing Methods */

	else if (cmp(command, "dsp_window_hamming"))
	{
		t(argv[3], &tfsample);
		dsp_window_hamming(&tfsample, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_window_hanning"))
	{
		t(argv[3], &tfsample);
		dsp_window_hanning(&tfsample, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_window_blackman"))
	{
		t(argv[3], &tfsample);
		dsp_window_blackman(&tfsample, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_window_chebyshev"))
	{
		t(argv[3], &tfsample);
		factor = atoi(argv[4]);
		dsp_window_chebyshev(&tfsample, factor, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_window_kaiser"))
	{
		t(argv[3], &tfsample);
		factor = atoi(argv[4]);
		dsp_window_kaiser(&tfsample, factor, &tresult);
		dt(&tresult);
	}

	/* Time-Frequency Transformation Methods */

	else if (cmp(command, "dsp_transform_dft"))
	{
		t(argv[3], &tfsample);
		dsp_transform_dft(&tfsample, &fresult);
		df(&fresult);
	}
	else if (cmp(command, "dsp_transform_dft_real"))
	{
		t(argv[3], &tfsample);
		dsp_transform_dft_real(&tfsample, &fresult);
		df(&fresult);
	}
	else if (cmp(command, "dsp_transform_idft"))
	{
		f(argv[3], &ffsample);
		dsp_transform_idft(&ffsample, &tresult);
		dt(&tresult);
	}

	/* FIR-based Filter Methods */

	else if (cmp(command, "dsp_filter_fir_low_pass"))
	{
		t(argv[3], &tfsample);
		fc = atof(argv[4]);
		fs = atof(argv[5]);
		taps = atoi(argv[6]);
		dsp_filter_fir_low_pass(&tfsample, fc, fs, taps, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_filter_fir_high_pass"))
	{
		t(argv[3], &tfsample);
		fc = atof(argv[4]);
		fs = atof(argv[5]);
		taps = atoi(argv[6]);
		dsp_filter_fir_high_pass(&tfsample, fc, fs, taps, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_filter_fir_band_pass"))
	{
		t(argv[3], &tfsample);
		fc1 = atof(argv[4]);
		fc2 = atof(argv[5]);
		fs = atof(argv[6]);
		taps = atoi(argv[7]);
		dsp_filter_fir_band_pass(&tfsample, fc1, fc2, fs, taps, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_filter_fir_band_stop"))
	{
		t(argv[3], &tfsample);
		fc1 = atof(argv[4]);
		fc2 = atof(argv[5]);
		fs = atof(argv[6]);
		taps = atoi(argv[7]);
		dsp_filter_fir_band_stop(&tfsample, fc1, fc2, fs, taps, &tresult);
		dt(&tresult);
	}

	/* IIR-based Filter Methods */

	else if (cmp(command, "dsp_filter_iir_low_pass"))
	{
		t(argv[3], &tfsample);
		fc = atof(argv[4]);
		fs = atof(argv[5]);
		dsp_filter_iir_low_pass(&tfsample, fc, fs, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_filter_iir_high_pass"))
	{
		t(argv[3], &tfsample);
		fc = atof(argv[4]);
		fs = atof(argv[5]);
		dsp_filter_iir_high_pass(&tfsample, fc, fs, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_filter_iir_band_pass"))
	{
		t(argv[3], &tfsample);
		fc = atof(argv[4]);
		fs = atof(argv[5]);
		Q = atof(argv[6]);
		dsp_filter_iir_band_pass(&tfsample, fc, fs, Q, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_filter_iir_band_stop"))
	{
		t(argv[3], &tfsample);
		fc = atof(argv[4]);
		fs = atof(argv[5]);
		Q = atof(argv[6]);
		dsp_filter_iir_band_stop(&tfsample, fc, fs, Q, &tresult);
		dt(&tresult);
	}
	else if (cmp(command, "dsp_filter_dc_block"))
	{
		t(argv[3], &tfsample);
		fc = atof(argv[4]);
		fs = atof(argv[5]);
		dsp_filter_dc_block(&tfsample, fc, fs, &tresult);
		dt(&tresult);
	}
	else
	{
		usage_error(argv[0]);
	}

	return EXIT_SUCCESS;
}
 
