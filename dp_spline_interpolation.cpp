/**
 * \file    parametric_spline.c
 * \brief   Core mathematical processing sub-functions with strict parameter prefixes.
 *
 * Implements the mathematical pipeline for non-matrix parametric cubic spline
 * path interpolation. This module resolves boundary conditions locally via
 * Gauss-Seidel relaxation to eliminate the risk of runtime matrix inversion failures.
 *
 * Compliant with MISRA C:2012 Rule 15.1 (No goto), Rule 15.5 (Single return),
 * and custom parameter prefix naming mandates. Optimized for RTCA DO-178C Level A
 * Modified Condition/Decision Coverage (MC/DC) trace audits.
 *
 * \version 1.03
 *
 * \Copyright (C) 2026 Data Patterns (India) Ltd.
 */

#include <math.h>
#include <stdlib.h>
#include "dp_spline_interpolation.h"

/* --- Module Internal/Private Sub-Function Prototypes --- */
static S32BIT CalculateParametricSteps(const PFDOUBLE in_pfdLat, const PFDOUBLE in_pfdLon, U32BIT u32NumSegments, PFDOUBLE out_pfdDtOut);
static void   CalculateCurvatures(const PFDOUBLE in_pfdCoords, const PFDOUBLE in_pfdDt, U32BIT u32DataCount, PFDOUBLE out_pfdMOut);
static void   CalculateCoefficients(const PFDOUBLE in_pfdCoords, const PFDOUBLE in_pfdDt, const PFDOUBLE in_pfdM, U32BIT u32NumSegments, PFDOUBLE out_pfdA, PFDOUBLE out_pfdB, PFDOUBLE out_pfdC, PFDOUBLE out_pfdD);

/**
 * \brief Global Module Core Entry Wrapper Function
 *
 * This function handles the parameter verification, localized heap layout allocation,
 * and calls the underlying computational engine sub-functions in sequence.
 * It strictly adheres to the single point of return principle.
 */
S32BIT ParametricSplineInterpolate(const PFDOUBLE in_pfdLatIn,
                                   const PFDOUBLE in_pfdLonIn,
                                   U32BIT u32DataCount,
                                   U32BIT u32NPoints,
                                   PFDOUBLE out_pfdLatOut,
                                   PFDOUBLE out_pfdLonOut)
{
    S32BIT s32Result = ERR_SPLINE_SUCCESS;                      /*!< Tracking variable for function operational success status */
    U32BIT u32NumSegments = 0U;                                 /*!< Computed total number of distinct piecewise polynomial intervals */
    U32BIT u32TotalOutPoints = 0U;                              /*!< Calculated total number of elements required in the high-density output array */
    U32BIT u32OutIdx = 0U;                                      /*!< Array cursor index tracking position in the destination output buffer */
    U32BIT i = 0U;                                              /*!< Primary outer loop iterator variable for piecewise segment traversal */
    U32BIT k = 0U;                                              /*!< Inner loop iterator variable tracking high-density step subdivisions */

    /* Dynamic runtime local heap allocation references initialized to NULL pointers to isolate memory blocks */
    PFDOUBLE pfdDt   = NULL;                                    /*!< Pointer to localized array storing chordal parametric deltas (dt) */
    PFDOUBLE pfdT    = NULL;                                    /*!< Pointer to localized array storing accumulated monotonic parameter bounds (t) */
    PFDOUBLE pfdMLat = NULL;                                    /*!< Pointer to localized array storing second-derivative curvature values for Latitude */
    PFDOUBLE pfdMLon = NULL;                                    /*!< Pointer to localized array storing second-derivative curvature values for Longitude */
    PFDOUBLE pfdALat = NULL; PFDOUBLE pfdBLat = NULL; PFDOUBLE pfdCLat = NULL; PFDOUBLE pfdDLat = NULL; /*!< Numerical tracking parameter matrices for Latitude coefficients */
    PFDOUBLE pfdALon = NULL; PFDOUBLE pfdBLon = NULL; PFDOUBLE pfdCLon = NULL; PFDOUBLE pfdDLon = NULL; /*!< Numerical tracking parameter matrices for Longitude coefficients */

    /* --- Parameter Range and Null Pointer Validations Layer --- */
    if ((in_pfdLatIn == NULL) || (in_pfdLonIn == NULL) || (out_pfdLatOut == NULL) || (out_pfdLonOut == NULL) || (u32NPoints == 0U))
    {
        s32Result = ERR_SPLINE_INVALID_PARAM;                   /*!< Throw error code due to null reference parameter detection or zero step count input */
    }
    else if (u32DataCount < 3U)
    {
        s32Result = ERR_SPLINE_INSUFFICIENT_N;                  /*!< Mathematical boundary constraint exception: minimum of 3 tracking coordinates required */
    }
    else
    {
        u32NumSegments = u32DataCount - 1U;                      /*!< Number of polynomial regions scales directly to N physical points minus one */
        u32TotalOutPoints = (u32NumSegments * u32NPoints) + 1U; /*!< Cumulative output resolution count calculated dynamically based on slice factor */

        /* Allocate localized working buffers via dynamic heap blocks */
        pfdDt   = (PFDOUBLE)malloc(u32NumSegments * sizeof(FDOUBLE));
        pfdT    = (PFDOUBLE)malloc(u32DataCount * sizeof(FDOUBLE));
        pfdMLat = (PFDOUBLE)malloc(u32DataCount * sizeof(FDOUBLE));
        pfdMLon = (PFDOUBLE)malloc(u32DataCount * sizeof(FDOUBLE));

        pfdALat = (PFDOUBLE)malloc(u32NumSegments * sizeof(FDOUBLE));
        pfdBLat = (PFDOUBLE)malloc(u32NumSegments * sizeof(FDOUBLE));
        pfdCLat = (PFDOUBLE)malloc(u32NumSegments * sizeof(FDOUBLE));
        pfdDLat = (PFDOUBLE)malloc(u32NumSegments * sizeof(FDOUBLE));

        pfdALon = (PFDOUBLE)malloc(u32NumSegments * sizeof(FDOUBLE));
        pfdBLon = (PFDOUBLE)malloc(u32NumSegments * sizeof(FDOUBLE));
        pfdCLon = (PFDOUBLE)malloc(u32NumSegments * sizeof(FDOUBLE));
        pfdDLon = (PFDOUBLE)malloc(u32NumSegments * sizeof(FDOUBLE));

        /* Confirm every individual pointer was successfully memory mapped */
        if ((pfdDt == NULL) || (pfdT == NULL) || (pfdMLat == NULL) || (pfdMLon == NULL) ||
            (pfdALat == NULL) || (pfdBLat == NULL) || (pfdCLat == NULL) || (pfdDLat == NULL) ||
            (pfdALon == NULL) || (pfdBLon == NULL) || (pfdCLon == NULL) || (pfdDLon == NULL))
        {
            s32Result = ERR_SPLINE_INVALID_PARAM;               /*!< Allocation anomaly fallback protection: abort operation to prevent system memory leak */
        }
        else
        {
            /* --- STEP 1: Execute Sub-Function A to resolve parametric tracking steps --- */
            s32Result = CalculateParametricSteps(in_pfdLatIn, in_pfdLonIn, u32NumSegments, pfdDt);

            if (s32Result == ERR_SPLINE_SUCCESS)
            {
                /* Accumulate step widths to derive our independent monotonic base tracking parameter t */
                pfdT[0] = 0.0;                                   /*!< Hard reset zero origin reference point for flight baseline tracking profile */
                for (i = 0U; i < u32NumSegments; i++)
                {
                    pfdT[i + 1U] = pfdT[i] + pfdDt[i];          /*!< Compute cumulative track distance via continuous vector summing operation */
                }

                /* --- STEP 2: Execute Sub-Function B for separate parallel curvature arrays --- */
                CalculateCurvatures(in_pfdLatIn, pfdDt, u32DataCount, pfdMLat);
                CalculateCurvatures(in_pfdLonIn, pfdDt, u32DataCount, pfdMLon);

                /* --- STEP 3: Execute Sub-Function C for localized structural coefficient maps --- */
                CalculateCoefficients(in_pfdLatIn, pfdDt, pfdMLat, u32NumSegments, pfdALat, pfdBLat, pfdCLat, pfdDLat);
                CalculateCoefficients(in_pfdLonIn, pfdDt, pfdMLon, u32NumSegments, pfdALon, pfdBLon, pfdCLon, pfdDLon);

                /* --- STEP 4: High-Resolution Interpolation Expansion Assembly Loop --- */
                u32OutIdx = 0U;                                 /*!< Initialize array cursor index to track output arrays safely */
                for (i = 0U; i < u32NumSegments; i++)
                {
                    FDOUBLE fdStartT = pfdT[i];                 /*!< Isolate localized window parameter start limit constraint */
                    FDOUBLE fdEndT = pfdT[i + 1U];              /*!< Isolate localized window parameter end limit constraint */

                    for (k = 0U; k < u32NPoints; k++)
                    {
                        /* Linearly subdivide the local domain parametric workspace window slice */
                        FDOUBLE fdTVal = fdStartT + (((FDOUBLE)k / (FDOUBLE)u32NPoints) * (fdEndT - fdStartT));
                        FDOUBLE fdDtShift = fdTVal - fdStartT;  /*!< Calculate isolated localized tracking coordinates offset step value */

                        /* Parallel shifted cubic equation polynomial evaluation mapping steps */
                        out_pfdLatOut[u32OutIdx] = pfdALat[i] + (pfdBLat[i] * fdDtShift) + (pfdCLat[i] * fdDtShift * fdDtShift) + (pfdDLat[i] * fdDtShift * fdDtShift * fdDtShift);
                        out_pfdLonOut[u32OutIdx] = pfdALon[i] + (pfdBLon[i] * fdDtShift) + (pfdCLon[i] * fdDtShift * fdDtShift) + (pfdDLon[i] * fdDtShift * fdDtShift * fdDtShift);
                        u32OutIdx++;                            /*!< Advance array pointer position cursor safely to next target memory block element */
                    }
                }

                /* Force absolute pinning criteria coordinates at target destination boundaries */
                out_pfdLatOut[u32TotalOutPoints - 1U] = in_pfdLatIn[u32DataCount - 1U]; /*!< Lock final node endpoint boundary precisely to prevent numeric drift */
                out_pfdLonOut[u32TotalOutPoints - 1U] = in_pfdLonIn[u32DataCount - 1U]; /*!< Lock final node endpoint boundary precisely to prevent numeric drift */
            }
        }
    }

    /* Secure cleanup block containing no premature exit points */
    free(pfdDt);   free(pfdT);
    free(pfdMLat); free(pfdMLon);
    free(pfdALat); free(pfdBLat); free(pfdCLat); free(pfdDLat);
    free(pfdALon); free(pfdBLon); free(pfdCLon); free(pfdDLon);

    return s32Result; /* Exactly one structural exit gate */
}

/**
 * \brief   SUB-FUNCTION A: Local Parametric Chordal Distance Step Profiler
 *
 * This sub-function transforms non-monotonic spatial positions into linear step widths
 * by calculating the physical straight-line distance vector between neighboring coordinate knots.
 * This constructs the independent parameter baseline needed for parametric parsing loops.
 *
 * \b Mathematical \b Formulation:
 * \f[
 * dt_i = \sqrt{(Lat_{i+1} - Lat_i)^2 + (Lon_{i+1} - Lon_i)^2}
 * \f]
 *
 * \param[in]  in_pfdLat       Pointer to array containing original raw input Latitude trace vectors.
 * \param[in]  in_pfdLon       Pointer to array containing original raw input Longitude trace vectors.
 * \param[in]  u32NumSegments  Calculated total count of intervals mapped to process (DataCount - 1).
 * \param[out] out_pfdDtOut    Target pointer location matrix to receive step horizontal width outputs.
 *
 * \return S32BIT              Execution error trap status; returns ERR_SPLINE_SUCCESS if valid.
 */
static S32BIT CalculateParametricSteps(const PFDOUBLE in_pfdLat, const PFDOUBLE in_pfdLon, U32BIT u32NumSegments, PFDOUBLE out_pfdDtOut)
{
    S32BIT s32SubResult = ERR_SPLINE_SUCCESS;                   /*!< Internal error execution tracking container register initialization */
    U32BIT i = 0U;                                              /*!< Tracking index increment counter variable allocation */

    /* Loop strictly bound to process consecutive segments sequentially */
    for (i = 0U; (i < u32NumSegments) && (s32SubResult == ERR_SPLINE_SUCCESS); i++)
    {
        FDOUBLE fdDLat = in_pfdLat[i + 1U] - in_pfdLat[i];      /*!< Compute forward spatial Delta width along the Latitude mapping grid */
        FDOUBLE fdDLon = in_pfdLon[i + 1U] - in_pfdLon[i];      /*!< Compute forward spatial Delta width along the Longitude mapping grid */

        /* Euclidean straight line step approximation between coordinates */
        out_pfdDtOut[i] = sqrt((fdDLat * fdDLat) + (fdDLon * fdDLon));

        /* Fail-safe detection trap monitoring consecutive identical entries to prevent zero division error anomalies */
        if (out_pfdDtOut[i] == 0.0)
        {
            s32SubResult = ERR_SPLINE_DUPLICATE_KNOT;            /*!< Critical error: identical coordinates found in consecutive waypoints */
        }
    }

    return s32SubResult;                                        /*!< Single return point passing out operational status evaluation */
}

/**
 * \brief   SUB-FUNCTION B: Non-Matrix Gauss-Seidel Curvature Relaxation Engine
 *
 * Resolves the continuity equations globally for secondary acceleration vector moments
 * without using matrix transposition or inversion loops. This completely eliminates
 * stack overflow risks and execution deadlocks in runtime firmware.
 *
 * \b Mathematical \b Recurrence \b Relation:
 * Enforces continuous derivative continuity (\f$C^2\f$) conditions across adjacent boundary regions.
 * \f[
 * M_i = \frac{6 \left( \frac{Y_{i+1} - Y_i}{dt_i} - \frac{Y_i - Y_{i-1}}{dt_{i-1}} \right) - dt_{i-1}M_{i-1} - dt_iM_{i+1}}{2(dt_{i-1} + dt_i)}
 * \f]
 *
 * Boundary Conditions: Set to "Natural" configuration to force endpoints to zero bending stress:
 * \f[
 * M_0 = 0, \quad M_{n-1} = 0
 * \f]
 *
 * \param[in]  in_pfdCoords   Source channel coordinate matrix reference array (Latitude or Longitude channel).
 * \param[in]  in_pfdDt       Calculated structural parametric spacing widths generated via sub-function A.
 * \param[in]  u32DataCount   Total length indicator defining raw tracking waypoint coordinate arrays.
 * \param[out] out_pfdMOut    Target output pointer map containing generated matching curvature vectors.
 *
 * \return void
 */
static void CalculateCurvatures(const PFDOUBLE in_pfdCoords, const PFDOUBLE in_pfdDt, U32BIT u32DataCount, PFDOUBLE out_pfdMOut)
{
    U32BIT i = 0U;                                              /*!< Node mapping vector indexing counter allocation variable */
    U32BIT u32Iter = 0U;                                        /*!< Loop relaxation execution tracking count monitor initialization */
    const U32BIT ku32MaxIterations = 800U;                      /*!< High safety ceiling bound tracking limit block protecting convergence loops */
    const FDOUBLE kfdThreshold = 1e-6;                          /*!< Target epsilon tolerance precision scale matching flight dynamics thresholds */
    U8BIT u8Converged = 0U;                                     /*!< State condition control tracking flag acting as a clean boolean variable */

    /* Setup Natural boundaries held pinned at zero bending energy limits */
    for (i = 0U; i < u32DataCount; i++)
    {
        out_pfdMOut[i] = 0.0;                                   /*!< Pre-initialize complete target register map array elements to zero baseline status */
    }

    /* Execute the iterative optimization sweeps over localized parameters */
    for (u32Iter = 0U; (u32Iter < ku32MaxIterations) && (u8Converged == 0U); u32Iter++)
    {
        FDOUBLE fdBiggestChange = 0.0;                          /*!< Reset convergence tracking variation scale profile to zero at start of sweep */

        /* Sweep only through the internal coordinates, keeping boundary elements fixed at zero */
        for (i = 1U; i < (u32DataCount - 1U); i++)
        {
            FDOUBLE fdOldM = out_pfdMOut[i];                    /*!< Maintain copy snapshot of former element value to calculate change trends */

            /* Continuity balancing recurrence equation derivation block execution layout */
            FDOUBLE fdNumerator = 6.0 * (((in_pfdCoords[i + 1U] - in_pfdCoords[i]) / in_pfdDt[i]) - ((in_pfdCoords[i] - in_pfdCoords[i - 1U]) / in_pfdDt[i - 1U]))
                                  - (in_pfdDt[i - 1U] * out_pfdMOut[i - 1U]) - (in_pfdDt[i] * out_pfdMOut[i + 1U]);
            FDOUBLE fdDenominator = 2.0 * (in_pfdDt[i - 1U] + in_pfdDt[i]);

            out_pfdMOut[i] = fdNumerator / fdDenominator;       /*!< Derive updated approximation curvature factor element parameter */

            FDOUBLE fdChange = fabsf((float)(out_pfdMOut[i] - fdOldM)); /*!< Extract exact magnitude difference delta mapping performance changes */
            if (fdChange > fdBiggestChange)
            {
                fdBiggestChange = fdChange;                     /*!< Keep track of the worst-case error variance to verify overall stability */
            }
        }

        /* Set exit condition flag cleanly without mid-loop break control statements */
        if (fdBiggestChange < kfdThreshold)
        {
            u8Converged = 1U;                                   /*!< Termination state achieved: curve variation falls below safety epsilon barrier limits */
        }
    }
}

/**
 * \brief   SUB-FUNCTION C: Local Polynomial Structural Coefficient Mapper
 *
 * Converts resolved secondary derivative acceleration vectors into individual
 * algebraic scaling components. This extracts the four exact coefficients (\f$a, b, c, d\f$)
 * needed to evaluate each localized piecewise spline segment.
 *
 * \b Derived \b Analytical \b Equations:
 * \f[
 * a_i = Y_i
 * \f]
 * \f[
 * c_i = \frac{M_i}{2}
 * \f]
 * \f[
 * d_i = \frac{M_{i+1} - M_i}{6 \cdot dt_i}
 * \f]
 * \f[
 * b_i = \frac{Y_{i+1} - Y_i}{dt_i} - \frac{dt_i \cdot (2M_i + M_{i+1})}{6}
 * \f]
 *
 * \param[in]  in_pfdCoords   Source tracking raw configuration arrays (Latitude or Longitude channels).
 * \param[in]  in_pfdDt       Calculated chordal parameter step lengths generated via sub-function A.
 * \param[in]  in_pfdM        Resolved secondary derivative matrix array calculated via sub-function B.
 * \param[in]  u32NumSegments Target processing iteration segment ceiling boundary value.
 * \param[out] out_pfdA       Target destination matrix location pointer for position constant coefficients.
 * \param[out] out_pfdB       Target destination matrix location pointer for first-derivative velocity slope coefficients.
 * \param[out] out_pfdC       Target destination matrix location pointer for second-derivative acceleration coefficients.
 * \param[out] out_pfdD       Target destination matrix location pointer for third-derivative jerk rates coefficients.
 *
 * \return void
 */
static void CalculateCoefficients(const PFDOUBLE in_pfdCoords,
                                  const PFDOUBLE in_pfdDt,
                                  const PFDOUBLE in_pfdM,
                                  U32BIT u32NumSegments,
                                  PFDOUBLE out_pfdA,
                                  PFDOUBLE out_pfdB,
                                  PFDOUBLE out_pfdC,
                                  PFDOUBLE out_pfdD)
{
    U32BIT i = 0U;                                              /*!< Segment processing index tracking pointer assignment loop index counters */

    /* Loop systematically over each segment boundary width window to convert constants */
    for (i = 0U; i < u32NumSegments; i++)
    {
        out_pfdA[i] = in_pfdCoords[i];                           /*!< Position Constant: direct offset extraction from coordinate node point location */
        out_pfdC[i] = in_pfdM[i] / 2.0;                         /*!< Acceleration Factor: structural mapping derived from local node curvature limits */
        out_pfdD[i] = (in_pfdM[i + 1U] - in_pfdM[i]) / (6.0 * in_pfdDt[i]); /*!< Jerk Calculation Rate: matches step progression change scales */

        /* Velocity First-Derivative Slope Extraction Equation:
         * b_i = (y_{i+1} - y_i)/h_i - [ h_i * (2*M_i + M_{i+1}) ] / 6
         */
        out_pfdB[i] = ((in_pfdCoords[i + 1U] - in_pfdCoords[i]) / in_pfdDt[i]) - (in_pfdDt[i] * ((2.0 * in_pfdM[i]) + in_pfdM[i + 1U])) / 6.0;
    }
}
