#ifndef DP_SPLINE_INTERPOLATION_H
#define DP_SPLINE_INTERPOLATION_H

#include "dp_aspj_comm.h"

/**
 * \file    parametric_spline.h
 * \brief   Interface for non-matrix parametric cubic spline interpolation.
 *
 * Implements a 2D geographic track path interpolator designed for paths that turn
 * back on themselves (non-monotonic) by using a distance-parameterized base.
 *
 * \version 1.02
 *
 * \Copyright (C) 2026 Data Patterns (India) Ltd.
 */

#ifndef _PARAMETRIC_SPLINE_H_
#define _PARAMETRIC_SPLINE_H_


#define DP_MAX_ROWS         8192
#define DP_LATIITUDE_COL    0
#define DP_LONGITUDE_COL    1

/* --- Standard Project Platform Fixed-Width Data Types Definition Layout --- */
//typedef unsigned char      U8BIT;       /*!< unsigned 8 bit datatype */
//typedef unsigned short     U16BIT;      /*!< unsigned 16 bit datatype */
//typedef unsigned int       U32BIT;      /*!< unsigned 32 bit datatype */
//typedef unsigned long long U64BIT;      /*!< unsigned 64 bit datatype */
//typedef signed char        S8BIT;       /*!< signed 8 bit datatype */
//typedef short              S16BIT;      /*!< signed 16 bit datatype */
//typedef int                S32BIT;      /*!< signed 32 bit datatype */
//typedef long long          S64BIT;      /*!< signed 64 bit datatype */
//typedef double             FDOUBLE;     /*!< double precision float */
//typedef double* PFDOUBLE;    /*!< pointer to double precision float */

/* --- Standardized Application-Specific Module Error Codes --- */
#define ERR_SPLINE_BASE            (-1000)
#define ERR_SPLINE_SUCCESS         (0)
#define ERR_SPLINE_INVALID_PARAM   (ERR_SPLINE_BASE - 1)
#define ERR_SPLINE_INSUFFICIENT_N  (ERR_SPLINE_BASE - 2)
#define ERR_SPLINE_DUPLICATE_KNOT  (ERR_SPLINE_BASE - 3)

/* --- Module Function Declarations --- */

/**
 * \brief Generates a high-density interpolated coordinate trajectory array path.
 *
 * Takes non-monotonic Latitude and Longitude tracking traces and calculates an
 * overshoot-safe natural cubic spline via parallel parametric relaxation sweeps.
 *
 * \param[in]  in_pfdLatIn   Pointer to the array containing original input Latitudes.
 * \param[in]  in_pfdLonIn   Pointer to the array containing original input Longitudes.
 * \param[in]  u32DataCount  Number of historical coordinate points present in inputs.
 * \param[in]  u32NPoints    Interpolation step factor required between sequential knots.
 * \param[out] out_pfdLatOut Pre-allocated destination buffer to store high-density Latitudes.
 * \param[out] out_pfdLonOut Pre-allocated destination buffer to store high-density Longitudes.
 *
 * \return S32BIT            Returns 0 (ERR_SPLINE_SUCCESS) on success, or a negative error code.
 */
S32BIT ParametricSplineInterpolate(const PFDOUBLE in_pfdLatIn,
                                   const PFDOUBLE in_pfdLonIn,
                                   U32BIT u32DataCount,
                                   U32BIT u32NPoints,
                                   PFDOUBLE out_pfdLatOut,
                                   PFDOUBLE out_pfdLonOut);

#endif /*#ifndef _PARAMETRIC_SPLINE_H_*/


#endif // DP_SPLINE_INTERPOLATION_H
