
/** 
 * @brief Spectrum utilities
 * 
 * This file defines several static methods withing
 * the StopPow namespace used as helper functions for
 * dealing with spectral analysis
 * 
 */

#ifndef SPECTRUM_H
#define SPECTRUM_H

#include <array>
#include <cmath>
#include <math.h>
#include <stdexcept>
#include <vector>
#include "StopPow.h"
#include "Util.h"

namespace StopPow
{
/** Shift a spectrum using a stopping power model and a given thickness. Uses the given model. Result put in argument vectors
	* @param model the StopPow model to use
	* @param thickness the thickness to transmit the spectrum through. Note: uses `mode' that model is set to. Can be negative, in which case the spectrum is upshifted.
	* @param data_E the energy bin values in MeV
	* @param data_Y the yield values for each energy in Yield/MeV
	*/
void shift(StopPow& model,
           double thickness,
           std::vector<double>& data_E,
           std::vector<double>& data_Y);

/** Shift a spectrum using a stopping power model and a given thickness. Uses the given model. Result is put in argument vectors
	* @param model the StopPow model to use
	* @param thickness the thickness to transmit the spectrum through. Note: uses `mode' that model is set to. Can be negative, in which case the spectrum is upshifted.
	* @param data_E the energy bin values in MeV
	* @param data_Y the yield values for each energy in Yield/MeV
	* @param data_err the error bars on yield
	*/
void shift(StopPow& model,
           double thickness,
           std::vector<double>& data_E,
           std::vector<double>& data_Y,
           std::vector<double>& data_err);
} // end of namespace StopPow

#endif