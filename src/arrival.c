/**
 ******************************************************************************
 * @file 	beamform.c
 * @author 	Ahmet Can GULMEZ
 * @brief 	Arrival of direction operations of DSP.
 * 
 ******************************************************************************
 * @attention
 * 
 * Copyright (c) 2026 Ahmet Can GULMEZ.
 * All rights reserved.
 * 
 * This software is licensed under the MIT License.
 * 
 *****************************************************************************0
 */

#include "./dsp.h"

/**
 * Calculate the arrival of angle using multiple signal classification 
 * (MUSIC) for uniform circular phased array microphone system and 
 * return the angle in degrees which has the maximum spectrum value. 
 */
int dsp_arrival_music(const DspArrival *arrival)
{
	int i, j, theta, maxangle;
	double val, spectrum;
	double maxspec = -INFINITY;
	maxangle = 0;
	double phi_i, k, dot, phase;

	gsl_matrix *X, *R, *evec;
   gsl_vector *eval;
   gsl_eigen_symmv_workspace *w;
   gsl_matrix_complex *En;
   gsl_complex z;
   gsl_vector_complex *a, *EnH_a, *EnEnHa;

	/* Validate the inputs. */
	assert_arrival(arrival);
	
	/* Allocate the required blocks for GNU Scientific Library. */
	X = gsl_matrix_alloc(arrival->mics, arrival->samples[0]->length);
   R = gsl_matrix_alloc(arrival->mics, arrival->mics);
   eval = gsl_vector_alloc(arrival->mics);
   evec = gsl_matrix_alloc(arrival->mics, arrival->mics);
   w = gsl_eigen_symmv_alloc(arrival->mics);
   En = gsl_matrix_complex_alloc(arrival->mics, arrival->mics - 
		arrival->sources);
   a = gsl_vector_complex_alloc(arrival->mics);
   EnH_a = gsl_vector_complex_alloc(arrival->mics - arrival->sources);
   EnEnHa = gsl_vector_complex_alloc(arrival->mics);

	/* Put the all samples into X matrix as for rows are mics 
      and columns are captured samples. */
	for (i = 0; i < arrival->mics; i++) 
	{
      for (j = 0; j < arrival->samples[i]->length; j++) 
		{
         gsl_matrix_set(X, i, j, arrival->samples[i]->data[j]);
      }
   }

	/* Compute the covariance matrix: R = (1/N) * X^H */
   gsl_blas_dgemm(CblasNoTrans, CblasTrans, 
      1.0 / arrival->samples[0]->length, X, X, 0.0, R);

   /* Eigenvalue decomposition. */
   gsl_eigen_symmv(R, eval, evec, w);
   gsl_eigen_symmv_sort(eval, evec, GSL_EIGEN_SORT_VAL_ASC);

   /* Extract noise eigenvectors. */
   for (i = 0; i < arrival->mics; i++) 
	{
      for (j = 0; j < arrival->mics - arrival->sources; j++) 
		{
         val = gsl_matrix_get(evec, i, j);
         GSL_SET_COMPLEX(&z, val, 0.0);
         gsl_matrix_complex_set(En, i, j, z);
      }
   }

	/* Compute the spectrum for each angle from 0 to 360 degrees. */
   for (theta = 0; theta < 360; theta++) 
	{
      /* Build the steering vector for each angle. */
      for (i = 0; i < arrival->mics; i++) 
		{
			phi_i = 2.0 * M_PI * i / arrival->mics;
			k = 2.0 * M_PI * arrival->freq / SOUND_SPEED;
			dot = cos(RAD(theta) - phi_i);
			phase = k * arrival->radius * dot;
         z = gsl_complex_polar(1.0, -phase);
         gsl_vector_complex_set(a, i, z);
      }
		
		/* Compute MUSIC spectrum. */
      gsl_blas_zgemv(CblasConjTrans, GSL_COMPLEX_ONE, En, a, 
         GSL_COMPLEX_ZERO, EnH_a);
      gsl_blas_zgemv(CblasNoTrans, GSL_COMPLEX_ONE, En, EnH_a, 
         GSL_COMPLEX_ZERO, EnEnHa);
      gsl_blas_zdotc(a, EnEnHa, &z);

      spectrum = 1.0 / GSL_REAL(z);
      if (spectrum > maxspec)
		{
			maxspec = spectrum; 
			maxangle = theta;
		}
   }
	/* Free allocated resources. */
   gsl_matrix_free(X);
   gsl_matrix_free(R);
   gsl_vector_free(eval);
   gsl_matrix_free(evec);
   gsl_eigen_symmv_free(w);
   gsl_matrix_complex_free(En);
   gsl_vector_complex_free(a);
   gsl_vector_complex_free(EnH_a);
   gsl_vector_complex_free(EnEnHa);

	return maxangle;
}
