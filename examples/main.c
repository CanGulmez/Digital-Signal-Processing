/**
 ******************************************************************************
 * @file 	main.c
 * @author 	Can Gulmez
 * @brief 	Example program
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

#include "main.h"

int main(void)
{
	// plotting_sample();
	// convolving_samples();
	// extracting_freq_features();
	// applying_windows();
	// time_to_freq_domain();
	// filtering_samples();
	// make_beamforming();
	// cal_arrival_of_angle();

	DspTime fsample, ssample, sample;

	dsp_signal_sin(10, 100, 1500, 0, 256, &fsample);
	dsp_signal_sin(25, 700, 1500, 0, 256, &ssample);
	dsp_time_add(&fsample, &ssample, &sample);

	// for (int i = 0; i < sample.length; i++)
	// 	printf("%.2f  ", sample.data[i]);
	// printf("\n");

	DspFreq transformed;
	DspTime mag;

	dsp_transform_fft_real(&sample, &transformed);
	dsp_freq_magnitude(&transformed, &mag);

	// dsp_transform_ifft(&transformed, &mag);

	// for (int i = 0; i < mag.length; i++)
	// 	printf("%.2f  ", mag.data[i]);
	// printf("\n");

	DspPlot plot;

	plot.title = "Simple Plot";
	plot.width = 2.0;
	plot.color = DSP_COLOR_PURPLE;
	plot.sample = &mag;

	dsp_plot_sample(&plot);
	
	return EXIT_SUCCESS;
}
 