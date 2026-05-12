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
	plotting_sample();
	convolving_samples();
	extracting_freq_features();
	applying_windows();
	time_to_freq_domain();
	filtering_samples();
	make_beamforming();
	cal_arrival_of_angle();

	return EXIT_SUCCESS;
}
 