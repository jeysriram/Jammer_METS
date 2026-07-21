#ifndef DP_LOCATIONFIXING_PRO_H
#define DP_LOCATIONFIXING_PRO_H

/*!
*	\def DP_MAX_DIRECTIONS
*	\brief This macro is used to define Max no of DOA's
*/
#define     DP_MAX_DIRECTIONS  256

/*!
*	\def DP_GET_DIST
*	\brief This macro  is used for enabling the flag to get emitter distance
*/
#define DP_GET_DIST 0

/*!
*	\def DP_SET_DIST
*	\brief This macro is used for to set  emitter distance  flag
*/
#define DP_SET_DIST 1


/*!
*	\def DP_NUM_ELLIPSE_POINTS
*	\brief This macro is used to defined to take the no of ellipse points
*/
#define     DP_NUM_ELLIPSE_POINTS  100

/*!
*	\def DP_SUCCESS
*	\brief This macro is used to for succes value
*/
#define DP_SUCCESS 0

/*!
*	\def DP_MAX_SENSORS
*	\brief This macro is used to take the maa no of sensors
*/
#define DP_MAX_SENSORS 16

/*!
*	\def S_DP_LF_RX
*	\brief This structure is used to for taking lat lon , DOA
*/

/*!
*	\def DP_MAX_DISTANCE_M
*	\brief This macro is used to take max emitter distance
*/
#define DP_MAX_DISTANCE_M  50000.000

/*!
*	\def DP_NO_OF_INIT_GUESS
*	\brief This macro is used to guess matrix size
*/
#define DP_NO_OF_INIT_GUESS 2

#define DIRECTIONS_RANGE_FROM_MEDIAN 15.00

typedef  struct
{
    U32BIT m_u32NoofDOA;                    /// This variable is used to take the no of DOAS
    U32BIT m_u32Reserved ;                  /// This variable is used for padding the structure

    FDOUBLE m_fdLat_origin;                 /// This variable is used to take latitude
    FDOUBLE m_fdLon_origin;                 /// This variable is used to take longitudes

    FDOUBLE m_fdarrDOA[DP_MAX_DIRECTIONS];  /// This variable is used to DOA's
}S_DP_LF_RX;


/*!
*	\def S_DP_DF_TX
*	\brief This structure is used for taking output latlons , inclation , major axis and minor axis.
*/
typedef  struct
{
    FDOUBLE m_fdLatitude;                                   /// This variable is used to take the destination latitude
    FDOUBLE m_fdLongitude;                                  /// This variable is used to take the destination longitude

    FDOUBLE m_fdMajor_axis_end;                             /// This variable is used to take major axis end
    FDOUBLE m_fdMinor_axis_end;                             /// This variable is used to take minor axis emd

    FDOUBLE m_fdInclanation;                               /// This variable is used to take major axis inclanation
    FDOUBLE m_fdarrErrEllPoints_X[DP_NUM_ELLIPSE_POINTS] ;  /// This variable is used to take ellipse points in x direction
    FDOUBLE m_fdarrErrEllPoints_Y[DP_NUM_ELLIPSE_POINTS];   /// This variable is used to take the  ellipse pointes in y direction

}S_DP_DF_TX;
/*!
*	\def S_DP_EIGEN_RESULT
*	\brief This structure is used to for taking lat lon , directions
*/
typedef struct
{
    FDOUBLE m_fdEigenvalues[2];
    FDOUBLE m_fdEigenvectors[4];
}S_DP_EIGEN_RESULT;

/*!
*   \fn S32BIT S32BIT DP_GetEmitterLocation(S_DP_LF_RX *in_sDp_LF_RX,U32BIT in_u32NoOfSensors  ,S8BIT in_s8LF_AlgSel,S_DP_DF_TX *out_spDF_Tx)
*  \breif This function used to calculate the centroid and emitter location of a set of sensors using triangulation and intersection methods.
*               The function also calculates error ellipses and covariance matrices
*  \param[in]  in_sDp_LF_RX  *in_sDp_LF_RX      This parameter is used to take the sensor data structure for each sensor.
*  \param[in]  U32BIT  in_u32NoOfSensors        This parameter is used to take the number of sensors in the system.
*  \param[in]  S8BIT in_s8LF_AlgSel             This parameter is used to take the LF Algorithm selection
*  \param[out] PFDOUBLE out_spDF_Tx             This parameter is used to store the destination latitude, longitude , inclenation ,major axis and minor axis
*  \author Konreddy.Ruchitha
*  \date  8 january 2025
*  \retval :: 0 or E_ERR_NULL_PTR_REF
*
*  \pre
*  - NA
*  \post
*  -NA
*/
S32BIT DP_GetEmitterLocation(S_DP_LF_RX *in_sDp_LF_RX,U32BIT in_u32NoOfSensors  ,S_DP_DF_TX *out_spDF_Tx);
/*!
  *
  *  \fn S32BIT UpdateMaxEmitterDistance(PU32BIT in_pu32Dist_m, S8BIT in_s8Flag)
  *  \brief This used to get or set the max emitter distance
  *
  *  \param [in/out]   in_pu32Dist_m     This is used to take the maximum emiter distance
  *  \param [in]   in_s8Flag             This flag is used for get or set the  flag to take the max emiter distance
  *
  *  \author Ruchitha Konreddy
  *  \date   9 january 2025
  *
  *  \retval : DP_SUCCESS (or) E_ERR_NULL_PTR_REF
  *
  *  \pre
  *	- NA
  *	\post
  *	- NA
  *
 */
S32BIT UpdateMaxEmitterDistance(PFDOUBLE in_pfdDist_m, S32BIT in_s32Set_Get_Flag);

/*!
  *
  *  \fn S32BIT DP_Degrees2DMS(FDOUBLE in_fdDegrees, PFDOUBLE out_pfdDegree, PFDOUBLE out_pfdMinuate ,PFDOUBLE out_pfdSecond)
  *  \brief This used to Convert degrees to Degree ,minuates and seconds
  *
  *  \param [in]   in_fdDegrees     This is used to take the degrees
  *  \param [in]   out_pfdDegree    This is used to take the round of degree
  *  \param [in]   out_pfdMinuate   This is used to take the minuate
  *  \param [in]   out_pfdSecond    This is used to take the seconds
  *
  *  \author Ruchitha Konreddy
  *  \date   9 january 2025
  *
  *  \retval : DP_SUCCESS (or) E_ERR_NULL_PTR_REF
  *
  *  \pre
  *	- NA
  *	\post
  *	- NA
  *
 */
S32BIT DP_Degrees2DMS(FDOUBLE in_fdDegrees,PFDOUBLE out_pfdDegree,PFDOUBLE out_pfdMinuate ,PFDOUBLE out_pfdSecond);

/*!
*   \fn  S32BIT DP_GetDestinationLatLong(FDOUBLE in_fdLat_Org_Deg,FDOUBLE in_fdLon_Org_Deg,FDOUBLE in_fdBear_Deg,FDOUBLE in_fddist_m ,PFDOUBLE Out_pfdDestLat_Deg,PFDOUBLE Out_pfdDestLon_Deg)
*   \brief  This function  used to calculate the distance between two points specified by their latitude and longitude.
*
*  \param[in] FDOUBLE in_fdLat_Org_Deg           This parameter is used to take the Latitude of the origin point in degrees
*  \param[in] FDOUBLE in_fdLon_Org_Deg           This parameter is used to take longitude of the origin point in degrees
*  \param[in] FDOUBLE in_fdBear_Deg              This parameter is used to take the  bearing angle towards the target ( receiver to transmitter)
*  \param[in] FDOUBLE in_fddist_m                This parameter is used to take the Distance to travel from the origin in meters.
*  \param[out] PFDOUBLE Out_pfdDestLat_Deg       This parameter is used to take the  destination latitude in degrees
*  \param[out] PFDOUBLE Out_pfdDestLon_Deg       This parameter is used to take the  destination longitude in degrees
*
*  \ref -  https://www.movable-type.co.uk/scripts/latlong.html
*  \author Konreddy.Ruchitha
*  \date  8 january 2025
*  \retval :: DP_SUCCESS  or E_ERR_NULL_PTR_REF
*
*  \pre
*  - NA
*  \post
*  -NA
*/

S32BIT DP_GetDestinationLatLong(FDOUBLE in_fdLat_Org_Deg,FDOUBLE in_fdLon_Org_Deg, FDOUBLE in_fdBear_Deg ,FDOUBLE in_fddist_m ,PFDOUBLE Out_pfdDestLat_Deg,PFDOUBLE Out_pfdDestLon_Deg);
/*!
*   \fn S32BIT DP_GetDistance(FDOUBLE in_fdLatIp1,FDOUBLE in_fdLonIp1,FDOUBLE in_fdLatIp2,FDOUBLE in_fdLonIp2,PFDOUBLE out_pfdDist)
*   \brief This function calculates the great-circle distance between two points on the Earth's surface on given  latitude and longitude. It uses the Haversine formula
*
*  \param[in] FDOUBLE in_fdLatIp1          This parameter is used to take Latitude of the first point in degrees.
*  \param[in] FDOUBLE in_fdLonIp1          This parameter is used to take  longitude of the first point in degrees.
*  \param[in] FDOUBLE in_fdLatIp2          This parameter is used to take the   Latitude of the second point in degrees.
*  \param[in] FDOUBLE in_fdLonIp2          This parameter is used to take the longitude  of the first point in degrees.
*  \param[out] PFDOUBLE out_pfdDist        This parameter is used to store the calculated distance in meters
*  \author Konreddy.Ruchitha
*  \date  8 january 2025
*  \retval :: DP_SUCCESS  or E_ERR_NULL_PTR_REF
*
*  \pre
*  - NA
*  \post
*  -NA
*/
S32BIT DP_GetIntersections(FDOUBLE in_fdLatInput1,FDOUBLE in_fdLongInput1,FDOUBLE in_fdBrng1,FDOUBLE in_fdLatInput2,FDOUBLE in_fdLongInput2,FDOUBLE in_fdBrng2,PFDOUBLE out_pfdIntersectLat,PFDOUBLE out_pfdIntersectLon ,PS32BIT out_ps32ErrorVal);

/*!
*   \fn S32BIT DP_GetDistance(FDOUBLE in_fdLatIp1,FDOUBLE in_fdLonIp1,FDOUBLE in_fdLatIp2,FDOUBLE in_fdLonIp2,PFDOUBLE out_pfdDist)
*   \brief This function calculates the great-circle distance between two points on the Earth's surface on given  latitude and longitude. It uses the Haversine formula
*
*  \param[in] FDOUBLE in_fdLatIp1          This parameter is used to take Latitude of the first point in degrees.
*  \param[in] FDOUBLE in_fdLonIp1          This parameter is used to take  longitude of the first point in degrees.
*  \param[in] FDOUBLE in_fdLatIp2          This parameter is used to take the   Latitude of the second point in degrees.
*  \param[in] FDOUBLE in_fdLonIp2          This parameter is used to take the longitude  of the first point in degrees.
*  \param[out] PFDOUBLE out_pfdDist        This parameter is used to store the calculated distance in meters
*  \author Konreddy.Ruchitha
*  \date  8 january 2025
*  \retval :: DP_SUCCESS  or E_ERR_NULL_PTR_REF
*
*  \pre
*  - NA
*  \post
*  -NA
*/
S32BIT DP_GetDistance(FDOUBLE in_fdLatIp1,FDOUBLE in_fdLonIp1,FDOUBLE in_fdLatIp2,FDOUBLE in_fdLonIp2,PFDOUBLE out_pfdDist);

/*!
*   \fn S32BIT DP_GenerateErrorEllipse(PFDOUBLE in_pfdMean, PFDOUBLE in_pfdCov, FDOUBLE in_fdconfidence, U32BIT in_u32NoOfPoints, PFDOUBLE out_pfdmean_x, PFDOUBLE out_pfdmean_y)
*   \brief  This Function generates an error ellipse based on a given mean vector, covariance matrix, confidence level, and the number of points to compute for the ellipse.
*  \param[in] FDOUBLE in_pfdMean               This parameter is used to take the  mean
*  \param[in] FDOUBLE in_pfdCov                This parameter is used to take 2x2 covariance matrix in row-major order:
*  \param[in] FDOUBLE in_fdconfidence          This parameter is used to take the confidence
*  \param[in] U32BIT in_u32NoOfPoints          This parameter is used to take  Number of points to generate for the ellipse outline.
*  \param[out] PFDOUBLE out_pfdmean_x          This parameter is used to store the x coordinates of the ellipse points
*  \param[out]  PFDOUBLE out_pfdmean_y         This parameter is used to store the y coordinates of the ellipse points
*  \author Konreddy.Ruchitha
*  \date  8 january 2025
*  \retval :: DP_SUCCESS  or E_ERR_NULL_PTR_REF
*
*  \pre
*  - NA
*  \post
*  -NA
*/
S32BIT DP_GenerateErrorEllipse(PFDOUBLE in_pfdMean, PFDOUBLE in_pfdCov, FDOUBLE in_fdconfidence, U32BIT in_u32NoOfPoints, PFDOUBLE out_pfdmean_x, PFDOUBLE out_pfdmean_y ,PFDOUBLE  out_pdfElipsCentLat_deg,PFDOUBLE out_pdfElipsCentLon_deg,PFDOUBLE out_pdfMajor_axis_end,PFDOUBLE out_pdfMinor_axis_end);

/*!
*   \fn S32BIT DP_GetCentrodLatLon_deg(PFDOUBLE in_pfdLatLon_deg,U32BIT in_u32NoOfRows, U32BIT in_u32NoOfCols,PFDOUBLE  out_pfdCentrdLat_deg,PFDOUBLE  out_pfdCentrdLon_deg)
*   \brief  Function calculates the central latitude and longitude of a set of geographic coordinates (latitude and longitude) provided as input
*  \param[in] FDOUBLE in_pfdLatLon_deg          This parameter is used to take the  latitude and longitude values in degrees.
*  \param[in] FDOUBLE in_u32NoOfRows            This parameter is used to take number of coordinate pairs.
*  \param[in] FDOUBLE in_u32NoOfCols            This parameter is used to take the no of cols(generally 2, since each coordinate pair has a latitude and longitude)
*  \param[out] U32BIT out_pfdCentrdLat_deg      This parameter is used to take the central latitude in degrees.
*  \param[out] PFDOUBLE out_pfdCentrdLon_deg    This parameter is used to take the central longitude in degrees.
*  \author Konreddy.Ruchitha
*  \date  8 january 2025
*  \retval :: DP_SUCCESS  or E_ERR_NULL_PTR_REF
*
*  \pre
*  - NA
*  \post
*  -NA
*/
S32BIT DP_GetCentrodLatLon_deg(PFDOUBLE in_pfdLatLon_deg,U32BIT in_u32NoOfRows, U32BIT in_u32NoOfCols,PFDOUBLE  out_pfdCentrdLat_deg,PFDOUBLE  out_pfdCentrdLon_deg);

/*!
*   \fn S32BIT S32BIT DP_GetEmitterLoc_LSM(PFDOUBLE in_pfdIntersectLoc, U32BIT in_u32NoOfRows ,U32BIT in_u32NoOfCols , PFDOUBLE out_pfdLatitude,PFDOUBLE out_pfdLongitude)
*  \brief  This function is used to calculate the the geographic location (latitude and longitude) of an emitter (or central point) by using a Least Squares Minimization (LSM) approach.
*  \param[in] FDOUBLE in_pfdIntersectLoc            This parameter is used to take the  geographic locations (latitude and longitude pairs).
*  \param[in] FDOUBLE in_u32NoOfRows                This parameter is used to take number of rows (coordinate pairs) in the input array.
*  \param[in] FDOUBLE in_u32NoOfCols                This parameter is used to take the no of columns in the input array, which is generally 2 (latitude and longitude).
*  \param[out] U32BIT out_pfdLatitude               This parameter is used to store the  calculated latitude of the emitter
*  \param[out] PFDOUBLE out_pfdLongitude            This parameter is used to store the calculated longitude of the emitter
*  \author Konreddy.Ruchitha
*  \date  8 january 2025
*  \retval :: DP_SUCCESS  or E_ERR_NULL_PTR_REF
*
*  \pre
*  - NA
*  \post
*  -NA
*/
S32BIT DP_GetEmitterLoc_LSM(PFDOUBLE in_pfdIntersectLoc, U32BIT in_u32NoOfRows ,U32BIT in_u32NoOfCols , PFDOUBLE out_pfdLatitude,PFDOUBLE out_pfdLongitude);

/*!
*   \fn S32BIT DP_SampleCovariance(PFDOUBLE in_fdInputData,  PFDOUBLE in_pfdMean,  U32BIT in_u32Rows, U32BIT in_u32Cols, PFDOUBLE out_pfdCovarience)
*   \brief Function to calculate the covarience of lat lon data
*  \param[in] FDOUBLE in_fdInputData             This parameter is used to take the lat lon data
*   \param[in] FDOUBLE in_pfdMean                This parameter is used to take mean of lat lon data
*  \param[in] FDOUBLE in_u32Rows                 This parameter is used to take no of rows
*  \param[in] FDOUBLE in_u32Cols                 This parameter is used to take  cols
*  \param[out] U32BIT out_pfdCovarience          This parameter is used to take the latLon Covarience
*  \author Konreddy.Ruchitha
*  \date  8 january 2025
*  \retval :: DP_SUCCESS  or E_ERR_NULL_PTR_REF
*
*  \pre
*  - NA
*  \post
*  -NA
*/
S32BIT DP_SampleCovariance(PFDOUBLE in_fdLatLon,  PFDOUBLE in_pfdMean,  U32BIT in_u32Rows, U32BIT in_u32Cols, PFDOUBLE out_pfdCovarience);

/*!
*   \fn S32BIT DP_CalculateMean(PFDOUBLE in_fdInputData, U32BIT in_u32Rows, U32BIT in_u32Cols,PFDOUBLE out_pfdMean)
*   \brief  This Function calculates the mean (average) of data
*  \param[in] FDOUBLE in_fdInputData            This parameter is used to take the input data
*  \param[in] FDOUBLE in_u32Rows                This parameter is used to take the number of rows in the input data array.
*  \param[in] FDOUBLE in_u32Cols                This parameter is used to take the number of cols in the input data array.
*  \param[out] U32BIT out_pfdMean               This parameter is used to take the mean value input data
*  \author Konreddy.Ruchitha
*  \date  8 january 2025
*  \retval :: DP_SUCCESS  or E_ERR_NULL_PTR_REF
*
*  \pre
*  - NA
*  \post
*  -NA
*/
S32BIT  DP_CalculateMean(PFDOUBLE in_fdInputData, U32BIT in_u32Rows, U32BIT in_u32Cols,PFDOUBLE out_pfdMean);

/*!
*   \fn S32BIT  DP_Compute_centroid(FDOUBLE in_fdsimplex[][DP_NO_OF_INIT_GUESS], S32BIT in_s32Worst_index, PFDOUBLE out_pfCentroid)
*   \brief  Function to calculate the centroid (or average) of a set of points represented as a simplex, excluding the worst point
*  \param[in] FDOUBLE in_fdsimplex       This parameter used to take the coordinates of the simplex points
*  \param[in] S32BIT in_s32Worst_index   This parameter used to take the "worst" point in the simplex, which will be excluded when calculating the centroid
*  \param[in] PFDOUBLE out_pfCentroid    This parameter used to  store the computed centroid
*  \author Konreddy.Ruchitha
*  \date  8 january 2025
*  \retval :: E_ERR_NULL_PTR_REF or DP_SUCCESS
*
*  \pre
*  - NA
*  \post
*  -NA
*/
S32BIT DP_Compute_centroid(FDOUBLE in_fdsimplex[][DP_NO_OF_INIT_GUESS], S32BIT in_s32Worst_index, PFDOUBLE out_pfCentroid);

/*!
*   \fn S32BIT DP_ComputeEigen(PFDOUBLE in_fdCov, S_DP_EigenResult *s_dp_EigenRes)
*   \brief  Thisfunction computes the eigen values and eigenvectors of a 2x2 covariance matrix
*
*  \param[in] FDOUBLE in_fdCov                        This parameter is used to take the  Covarience
*  \param[out] S_DP_EIGEN_RESULT *s_dp_EigenRes       This parameter is used to store eigen values and eigen vectors
*  \author Konreddy.Ruchitha
*  \date  8 january 2025
*  \retval :: DP_SUCCESS  or E_ERR_NULL_PTR_REF
*
*  \pre
*  - NA
*  \post
*  -NA
*/
S32BIT DP_ComputeEigen(PFDOUBLE in_fdCov, S_DP_EIGEN_RESULT *s_dp_EigenRes);

/*!
*  \fn S32BIT DP_CalculateDistance(FDOUBLE in_fdarrLatLons[][2], PFDOUBLE in_pfdX , PFDOUBLE out_pfdSum)
*  \brief Function to calculate the  sum of squared distances between a given point and a set of other points, represented by their latitude and longitude coordinates. This is done using the Euclidean distance formula
*  \param[in] FDOUBLE in_fdarrLatLons       This parameter represents a point's latitude and longitude.
*  \param[in] FDOUBLE in_pfdX               This parameter used to take the latitude and longitude of a reference point (target).
*  \param[in] FDOUBLE out_pfdSum            This parameter used to store the sum of the squared distances
*  \author Konreddy.Ruchitha
*  \date  8 january 2025
*  \retval ::E_ERR_NULL_PTR_REF or DP_SUCCESS
*
*  \pre
*  - NA
*  \post
*  -NA
*/
S32BIT DP_CalculateDistance(FDOUBLE in_fdarrLatLons[][2], U32BIT in_u32NoOfLatLonPoints, PFDOUBLE in_pfdX,  PFDOUBLE out_pfdSum);

/*!
*  \fn S32BIT DP_GetMinLoc_Nelder_mead(FDOUBLE in_fdarrLatLons[][2], PFDOUBLE in_pfdInitial_guess, FDOUBLE in_fdTol, S32BIT in_s32Max_iter, PFDOUBLE out_pfdLatLon)
*  \brief Function to calculate min location (Lat lon) using Nelder-Mead algorithm
*  \param[in] in_fdarrLatLons   This parameter used to take the latitude-longitude pair
*  \param[in] in_pfdInitial_guess         This parameter used to take the initial guess for the optimization (the starting point for the simplex).
*  \param[in] in_fdTol                    This parameter used to take the tolerance(The algorithm stops when the difference between the best and worst function values in the simplex is less than this tolerance.)
*  \param[in] in_s32Max_iter              This parameter used to take the max iterations
*  \param[out] out_pfdLatLon              This parameter used to store the latitude and longitude of the best point
*  \author Konreddy.Ruchitha
*  \date  8 january 2025
*  \retval :: DP_SUCCESS or E_ERR_NULL_PTR_REF
*
*  \pre
*  - NA
*  \post
*  -NA
*/
S32BIT DP_GetMinLoc_Nelder_mead(FDOUBLE in_fdarrLatLons[][2], PFDOUBLE in_pfdInitial_guess, FDOUBLE in_fdTol, S32BIT in_s32Max_iter, PFDOUBLE out_pfdLatLon);

/*!
*   \fn S32BIT DP_GetBearingAngle(FDOUBLE in_fdLatIp1,FDOUBLE in_fdLonIp1,FDOUBLE in_fdLatIp2,FDOUBLE in_fdLonIp2,PFDOUBLE out_pfdBearAng_Deg)
*   \brief This function calculates the bearing angle/inclanation between two points on the Earth's surface on given  latitude and longitude.
*
*  \param[in] FDOUBLE in_fdLatIp1                 This parameter is used to take Latitude of the first point in degrees.
*  \param[in] FDOUBLE in_fdLonIp1                 This parameter is used to take  longitude of the first point in degrees.
*  \param[in] FDOUBLE in_fdLatIp2                 This parameter is used to take the   Latitude of the second point in degrees.
*  \param[in] FDOUBLE in_fdLonIp2                 This parameter is used to take the longitude  of the first point in degrees.
*  \param[out] PFDOUBLE out_pfdBearAng_Deg        This parameter is used to store the calculated bearing angle
*  \author Konreddy.Ruchitha
*  \date  8 january 2025
*  \retval :: DP_SUCCESS  or E_ERR_NULL_PTR_REF
*
*  \pre
*  - NA
*  \post
*  -NA
*/
S32BIT DP_GetBearingAngle(FDOUBLE in_fdLatIp1,FDOUBLE in_fdLonIp1,FDOUBLE in_fdLatIp2,FDOUBLE in_fdLonIp2,PFDOUBLE out_pfdBearAng_Deg);

/*!
  *
  *  \fn S32BIT DP_LF_FilterDOA(PFDOUBLE in_pfdDOA, PU32BIT in_pu32NoofDOA ,FDOUBLE in_fdMedian )
  *  \brief This used to sort the DOA data
  *
  *  \param [in]   in_pfdDOA                         This parameter is used to take the DOA data
  *  \param [in]   in_u32NoofDOA                     This parameter is used to take the no of directions
  *  \param [in]   in_fdMedian                        This parameter is used to take the median
  *  \author Ruchitha Konreddy
  *  \date   7 february 2025
  *
  *  \retval : DP_SUCCESS or E_ERR_NULL_PTR_REF
  *
  *  \pre
  *	- NA
  *	\post
  *	- NA
  *
 */
S32BIT DP_LF_FilterDOA(PFDOUBLE in_pfdDOA, PU32BIT in_pu32NoofDOA ,FDOUBLE in_fdMedian);
/*!
  *
  *  \fn  S32BIT DP_LF_FindMedian(PFDOUBLE in_pfdDOA, U32BIT in_u32NoofDOA ,PFDOUBLE out_pfdMedian)
  *  \brief This used to sort the DOA data
  *
  *  \param [in]   in_pfdDOA                         This parameter is used to take the DOA data
  *  \param [in]   in_u32NoofDOA                     This parameter is used to take the no of directions
  *  \param [out]   out_pfdMedian                    This parameter is used to take the median
  *  \author Ruchitha Konreddy
  *  \date   7 february 2025
  *
  *  \retval : DP_SUCCESS or E_ERR_NULL_PTR_REF
  *
  *  \pre
  *	- NA
  *	\post
  *	- NA
  *
 */
S32BIT DP_LF_FindMedian(PFDOUBLE in_pfdDOA, U32BIT in_u32NoofDOA ,PFDOUBLE out_pfdMedian);

/*!
  *
  *  \fn  S32BIT DP_LF_CompareDOAs(const void *arg1, const void *arg2)
  *  \brief This used to sort the DOA data
  *
  *  \param [in]   arg1                      This parameter is used to take the data from the DOA Array
  *  \param [in]   arg2                       This parameter is used to take the  data from the DOA Array
  *  \author Ruchitha Konreddy
  *  \date    7 february 2025
  *
  *  \retval : DP_SUCCESS
  *
  *  \pre
  *	- NA
  *	\post
  *	- NA
  *
 */
S32BIT DP_LF_CompareDOAs(const void *arg1, const void *arg2);

/*!
  *
  *  \fn S32BIT FilterDOAfromAllEmitterData(S_DP_LF_RX *in_sDp_LF_RX, U32BIT in_u32NoOfSensors)
  *  \brief This used to sort the DOA data from all the Emitters
  *
  *  \param [in]   in_sDp_LF_RX                          This parameter is used to take the
  *  \param [in]   in_u32NoOfSensors                     This parameter is used to take the no of directions
  *  \author Ruchitha Konreddy
  *  \date   7 february 2025
  *
  *  \retval : DP_SUCCESS or E_ERR_NULL_PTR_REF
  *
  *  \pre
  *	- NA
  *	\post
  *	- NA
  *
 */
S32BIT FilterDOAfromAllEmitters(S_DP_LF_RX *in_sDp_LF_RX, U32BIT in_u32NoOfSensors);

/*!
*  \fn void DP_DF_Get_Error_Message(S32BIT in_s32ErrCode ,out_ps8ErrString)
*  \brief Function to get error message
*  \param[in]  in_s32ErrCode        This parameter used to take error code
*  \param[out]  out_ps8ErrString   This parameter used to take error string
*  \author Konreddy.Ruchitha
*  \date  8 january 2025
*  \retval :: NILL
*
*  \pre - output string array should be size of 300.
*  - NA
*  \post
*  -NA
*/
void DP_LF_Get_Error_Message(S32BIT in_s32ErrCode , PS8BIT out_ps8ErrString);

#endif // DP_LOCATIONFIXING_PRO_H
