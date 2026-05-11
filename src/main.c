/**
 * Main Program
 */

#include "dsp.h"

int main(void)
{
	DspTime fsample, ssample, res;
	DspStatus status;

	fsample = (DspTime) {
		.length = 10,
		.data = { 4, 1, 8, 6, 5, 7, 1, 2, 6, 9 }
	};

	status = dsp_signal_square(10, 100, 1000, 16, &res);
	assert(status == DSP_SUCCESS);

	for (int i = 0; i < res.length; i++)
		printf("%.2f ", res.data[i]);
	printf("\n");

	return EXIT_SUCCESS;
}
