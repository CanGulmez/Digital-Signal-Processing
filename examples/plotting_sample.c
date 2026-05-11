/**
 ******************************************************************************
 * @file 	main.c
 * @author 	Can Gulmez
 * @brief 	Plotting the sample.
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

void plotting_sample(void)
{
	DspTime sample, windowed;
	DspPlot plot;
	DspStatus status;
	int res;

	/* Create a windowed sinus sample. */
	status = dsp_signal_sin(10, 100, 1000, 0, 256, &sample);
	assert(status == DSP_SUCCESS);

	/* Window it. */
	status = dsp_window_blackman(&sample, &windowed);
	assert(status == DSP_SUCCESS);

	/* Plot the windowed sample. */
	plot.title = "10 * sin(2 * pi * 0.1)";
	plot.color = DSP_COLOR_PURPLE;
	plot.width = 3.0;
	plot.sample = &windowed;

	res = dsp_plot_sample(&plot);
	assert(res == 0);
}
