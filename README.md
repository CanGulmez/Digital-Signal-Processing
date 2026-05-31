# Digital Signal Processing 

This library is indeed to provide digital signal processing interface for real-time systems. 
Primarilly, this library consists of nine modules. These modules are `time`, `freq`, `filter`, 
`signal`, `transform`, `plot`. `arrival`, `beaform` and `window`. Each module was defined in 
separate source file with related named APIs. 

To use the plotting functionality and arrival of angle algorithm, there are especially two 
libraries that you must have. These are `GTK4` and `GSL` (GNU Scientific Library). You can 
installed these with:

```shell
$ sudo apt install libgtk-4-dev libgsl-dev
```

Generally, the all sample/data structures are defined with fixed-side buffer named
`DATA_SIZE` due to real-time & deterministic requirements. You MUST set it according 
to your considerations. I will use this library in passive acoustic surveillance project.
Also the maximum microphone and source of coming signal number were defined as `MAX_MICS` 
and `MAX_SOURCES`. Time-domain and frequency domain data structures are defined seperated 
named `DspTime` and `DspFreq`. Also there are the other structures for specific usages. 

There is a dedicated Makefile to build the all artifacts. These are the static library
and the binary of the example program. Use this command:

```shell
$ make
```

The static library will be gone under `lib` and example program binary under `bin`. You can 
see the example program output quickly with:

```shell
$ ./bin/main
```

The example program source file is under `examples`. For example, let's consider this program:

```c

#include "dsp.h"

int main(void)
{
	DspTime sample, windowed;
	DspPlot plot;
	DspStatus status;
	int res;

	/* Create a sinus sample. */
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

	return EXIT_SUCCESS;
}
```

After run this program you will see this plot:

![DSP - Plotting Sample](docs/plotting_sample.png)
