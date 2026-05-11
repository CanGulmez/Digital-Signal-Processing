/**
 ******************************************************************************
 * @file 	main.c
 * @author 	Can Gulmez
 * @brief 	Filtering samples.
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

void filtering_samples(void)
{
	int res;
	DspTime fsample, ssample, sample;
	DspTime filtered;
	DspPlot plot;
	DspStatus status;

	/* Creating the two samples. */

	status = dsp_signal_cos(10, 100, 1000, 0, 256, &fsample);
	assert(status == DSP_SUCCESS);

	status = dsp_signal_cos(5, 400, 1000, 0, 256, &ssample);
	status = dsp_time_add(&fsample, &ssample, &sample);

	plot.title = "10*cos(2*pi*0.1) + 5*cos(2*pi*0.4)";
	plot.color = DSP_COLOR_PURPLE;
	plot.width = 2.0;
	plot.sample = &sample;

	res = dsp_plot_sample(&plot);

	/* Filter the sample as low pass (below 250 Hz). */

	status = dsp_filter_fir_low_pass(&sample, 250, 1000, 65, &filtered);

	plot.title = "10*cos(2*pi*0.1)";
	plot.color = DSP_COLOR_PURPLE;
	plot.width = 2.0;
	plot.sample = &filtered;	

	res = dsp_plot_sample(&plot);

	/* Filter the sample as high pass (upper 250 Hz). */

	status = dsp_filter_fir_high_pass(&sample, 250, 1000, 65, &filtered);

	plot.title = "5*cos(2*pi*0.4)";
	plot.color = DSP_COLOR_PURPLE;
	plot.width = 2.0;
	plot.sample = &filtered;	

	res = dsp_plot_sample(&plot);
}
