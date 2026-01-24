# Digital Signal Processing 

This library is indeed to provide digital signal processing 
interface. Primarilly, this library consists of seven modules. 
In future, of course, I'll develop new modules. For now, these 
modules are `arrival`, `beaform`, `core`, `filter`, `signal`, 
`transform`, `plot`. and `window`. 

In `core` module, there are basic and fundamental methods. I've 
separated these methods as time and frequency domains. Because, 
generally we tend  to inspect digital signals in both domain to 
extract the all informative results. 

In `beamform` and `arrival` modules, desgined for audio prosessing, 
there are a varity of beamforming and direction of arrival 
algorithms.

In `filter` module, there are mostly used filter designs. Generally,
FIR  (Finite Impulse Response) and IIR (Infinite Impulse Response) 
are mostly used filter design techniques. In this module, I've 
defined a lot of filter types based on these two techniques.

In `signal` module, there are predefined signals like sine, 
triangle, etc. These are mostly used signal types in a digital 
signal processing programs.

In `transform` module, there are transformation methods. In 
practical problems, signals are samples as discrete time. But we 
can extract little informative results from this form. So that We 
want to analyze signals in frequency domain. This transformation is 
done using magic Fourier transform technique. All transformation 
methods are derivatived based on this.

In `plot` module, there is a plotting functionality to visualize
the time-domain sample. It's GTK 4 and Cairo based system so that
make sure the required libraries are being installed.

In `window` module, there are window methods. Generally, these 
windows are used in filter design or suppression noise.
