/**
 * Main Program
 */

#include "dsp.h"

int main(void)
{
	DspTime fsample, ssample, res;
	DspFreq transformed;
	DspStatus status;
	DspPlot plot;

	fsample = (DspTime) {
		.length = 10,
		.data = { 4, 1, 8, 6, 5, 7, 1, 2, 6, 9 }
	};

	status = dsp_transform_dft(&fsample, &transformed);
	assert(status == DSP_SUCCESS);

	status = dsp_freq_magnitude(&transformed, &res);
	assert(status == DSP_SUCCESS);

	for (int i = 0; i < res.length; i++)
		printf("%.2f ", res.data[i]);
	printf("\n");

	// plot.title = "Simple Plot";
	// plot.width = 2.0;
	// plot.color = DSP_COLOR_PURPLE;
	// plot.sample = &res;
	// dsp_plot_sample(&plot);

	return EXIT_SUCCESS;
}
