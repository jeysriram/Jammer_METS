/*!
*	\file DP_EmitterRelParam.cpp
*	\brief This file contains the Emitter Relative Paramaters calculations from the WayPoint Latitudes and Longitudes
*
*   \author Anunitha J
*   \date 17 june 2026
*
*   \copyright Copyright (C) 2026 Data Patterns (India). Ltd. \n
*	All Rights Reserved.\n
*	Address: Plot # H9, Fourth Main Road, SIPCOT IT Park, Siruseri, \n
*           Off Rajiv Gandhi Salai (OMR), Pudupakkam P.O., \n
*           Chennai-603103 | India\n
*	Website : http://www.datapatternsindia.com/\n
*	Phone: 91-44-4741-4000\n
*	FAX: 91-44-4741-4444 \n
*/

#include "dp_aspj_comm.h"

/*!
 *
 *  \fn S32BIT  ReadWayPointLatLong(PS8BIT in_ps8FileName, PU32BIT out_pu32NoOfSamples, S_WAY_POINT_LAT_LONG *out_sdp_LatLong)
 *  \brief This function is used to read the Way Point Lat and Long
 *
 *  \param [in]   in_ps8FileName        This is used to take the  file name to store 8 bit data
 *  \param [out]   out_sdp_LatLong      This is used to take the  latitude and Longitude
 *
 *  \author Anunitha J
 *  \date   18 June 2026
 *
 *  \retval : DP_SUCCESS (or) DP_FAILURE
 *
 *  \pre
 *	- NA
 *	\post
 *	- NA
 *
*/
S32BIT ReadWayPointLatLong(PS8BIT in_ps8FileName,S_DP_ASPJ_WAYPOINTS_LAT_LON *out_pSdp_LatLong)
{
    S32BIT s32RetVal = DP_SUCCESS;     /// - s32RetVal Parameter used for Return the  Error values
    FILE *fp_LatLong = NULL;   /// - fp_RawImg Parameters are used for file pointer
    static S8BIT sarrTempData[10240] = {0};
    U32BIT u32Count  = 0;


    /// -  Inputs & Outputs pointers validation
    if((out_pSdp_LatLong == NULL) || (in_ps8FileName == NULL))
    {
        s32RetVal = DP_ASPJ_FAILURE;

    }
    else
    {
        /// - Open the csv files in Reading Mode
        fp_LatLong = fopen(in_ps8FileName,"r");

        /// - validate the file pointer
        if(fp_LatLong == NULL)
        {
            s32RetVal = DP_ASPJ_FAILURE;

        }
        else
        {
            fscanf(fp_LatLong,"%[^\n]",sarrTempData);

            while(!feof(fp_LatLong))
            {
                fscanf(fp_LatLong,"%lf,%lf\n",&out_pSdp_LatLong->m_arrfdLat[u32Count],&out_pSdp_LatLong->m_arrfdLon[u32Count]);

                u32Count++;

                /// - check if count exceeds max array index
                if(u32Count > DP_MAX_WAYPOINT_LAT_LONG)
                {
                    break;
                }
            }

            /// - Close the csv file
            fclose(fp_LatLong);

            /// - copy the Lat Long count
            out_pSdp_LatLong->m_u32LatLongCnt = u32Count;
            out_pSdp_LatLong->m_u16BinFlag    = false;
            out_pSdp_LatLong->m_u16BinningCnt = 50;
        }
    }

    return s32RetVal;
}


/*!
 *
 *  \fn S32BIT  ReadEmitterLatLong(PS8BIT in_ps8FileName, PU32BIT out_pu32NoOfSamples, S_EMITTER_LAT_LONG *out_sdp_EmitLatLong)
 *  \brief This function is used to read the Emitter Lat and Long
 *
 *  \param [in]   in_ps8FileName          This is used to take the  file name to store 8 bit data
 *  \param [out]   out_sdp_EmitLatLong    This is used to take the  latitude and Longitude
 *
 *  \author Anunitha J
 *  \date   18 June 2026
 *
 *  \retval : DP_SUCCESS (or) DP_FAILURE
 *
 *  \pre
 *	- NA
 *	\post
 *	- NA
 *
*/
S32BIT ReadEmitterLatLong(PS8BIT in_ps8FileName, S_DP_ASPJ_EMITTERS_LAT_LON *out_pSdp_EmitLatLong)
{
    S32BIT s32RetVal = DP_SUCCESS;          /// - s32RetVal Parameter used for Return the  Error values
    FILE *fp_LatLong = NULL;                /// - fp_RawImg Parameters are used for file pointer
    static S8BIT sarrTempData[10240] = {0};
    U16BIT u16Count  = 0;


    /// -  Inputs & Outputs pointers validation
    if((out_pSdp_EmitLatLong == NULL) || (in_ps8FileName == NULL))
    {
        s32RetVal = DP_ASPJ_FAILURE;

    }
    else
    {
        /// - Open the csv files in Reading Mode
        fp_LatLong = fopen(in_ps8FileName,"r");

        /// - validate the file pointer
        if(fp_LatLong == NULL)
        {
            s32RetVal = DP_ASPJ_FAILURE;

        }
        else
        {
            fscanf(fp_LatLong,"%[^\n]",sarrTempData);

            while(!feof(fp_LatLong))
            {
                fscanf(fp_LatLong,"%lf,%lf,%hu\n",&out_pSdp_EmitLatLong[u16Count].m_fdEmitLat,&out_pSdp_EmitLatLong[u16Count].m_fdEmitLon,&out_pSdp_EmitLatLong[u16Count].m_u16RdrRange_km);

                u16Count++;

                /// - check if count exceeds max array index
                if(u16Count > DP_MAX_EMITTER_LAT_LONG)
                {
                    break;
                }
            }

            /// - Close the csv file
            fclose(fp_LatLong);

            out_pSdp_EmitLatLong->m_u16EmtLatLongCnt = u16Count;
        }
    }

    return s32RetVal;
}

/*!
 *
 *  \fn S32BIT  GetEmitterRelativeParameters(S_WAY_POINT_LAT_LONG *in_pSdp_LatLong,S_EMITTER_LAT_LONG *in_pSdp_EmitLatLong,\
                                             S_WAYPOINT_EMITTER_LOCATIONS *out_pSWayPointEmitter)
 *  \brief This function is used to calculate the Emitter Relative Parameters corresponding to each WayPoint Latitude and Longitude
 *
 *  \param [in]    in_pSdp_LatLong          This is used to take the Way Point latitude and Longitude
 *  \param [in]    in_pSdp_EmitLatLong      This is used to take the Emitter latitude and Longitude
 *  \param [out]   out_pSWayPointEmitter    This is used to store the Emitter count ,distance and angle for each waypoint lat and long
 *
 *  \author Anunitha J
 *  \date   18 June 2026
 *
 *  \retval : DP_SUCCESS (or) DP_FAILURE
 *
 *  \pre
 *	- NA
 *	\post
 *	- NA
 *
*/
S32BIT GetEmitterRelativeParameters(S_DP_ASPJ_WAYPOINTS_LAT_LON *in_pSdp_LatLong,S_DP_ASPJ_EMITTERS_LAT_LON *in_pSdp_EmitLatLong,\
                                    S_DP_ASPJ_WAYPOINT_EMITTER_LOCATIONS *out_pSWayPointEmitDetRange)
{
    S32BIT s32RetVal        = DP_SUCCESS;     /// - s32RetVal Parameter used for Return the  Error values
    U32BIT u32Loop          = 0;
    U32BIT u32EmitterLoop   = 0;
    U32BIT u32EmitterCnt    = 0;
    U32BIT u32WayPointCnt   = 0;
    FDOUBLE fdDistance_m    = 0.0f;
    FDOUBLE fdBearingAngle  = 0.0f;
    FDOUBLE fdEmitBearingAngle = 0.0f;
    FDOUBLE fdDistance_km   = 0.0f;

    /// - Validate the Binning
    DP_VALIDATE_BINNING(in_pSdp_LatLong->m_u16BinningCnt,in_pSdp_LatLong->m_u32LatLongCnt);
    DP_VALIDATE_EMITTER_CNT(in_pSdp_EmitLatLong->m_u16EmtLatLongCnt);   /// - Validate the Emitter Count
    DP_VALIDATE_WAYPOINT_CNT(in_pSdp_LatLong->m_u32LatLongCnt);         /// - Validate the WayPoint Count

    if(in_pSdp_LatLong == NULL || in_pSdp_EmitLatLong == NULL || out_pSWayPointEmitDetRange == NULL)
    {
        s32RetVal = DP_ASPJ_FAILURE;
    }
    else
    {

        /// - For Each WayPoint Lat, Long loop
        for(u32Loop = 0;u32Loop < in_pSdp_LatLong->m_u32LatLongCnt;u32Loop++)
        {
            /// - check if Binning is enabled
            if(in_pSdp_LatLong->m_u16BinFlag)
            {
                /// - Skip the Binning Count Loop
                if((u32Loop % in_pSdp_LatLong->m_u16BinningCnt) != 0)
                {
                    //                    printf("Skipped %d\n" , u32Loop);
                    continue;
                }
            }

            u32EmitterCnt = 0;

            /// - For Each WayPoint Lat, Long all the emitter distance and angle is calculated
            for(u32EmitterLoop = 0;u32EmitterLoop < in_pSdp_EmitLatLong->m_u16EmtLatLongCnt;u32EmitterLoop++)
            {
                /// - Get the way point and Emitter Lat and Long Distance
                s32RetVal =  DP_GetDistance(in_pSdp_LatLong->m_arrfdLat[u32Loop],in_pSdp_LatLong->m_arrfdLon[u32Loop],in_pSdp_EmitLatLong[u32EmitterLoop].m_fdEmitLat,in_pSdp_EmitLatLong[u32EmitterLoop].m_fdEmitLon,&fdDistance_m);
                if(s32RetVal == DP_SUCCESS)
                {
                    /// - Get the way point and Emitter Lat and Long Bearing Angle from the WayPoint
                    s32RetVal =  DP_GetBearingAngle(in_pSdp_LatLong->m_arrfdLat[u32Loop],in_pSdp_LatLong->m_arrfdLon[u32Loop],in_pSdp_EmitLatLong[u32EmitterLoop].m_fdEmitLat,in_pSdp_EmitLatLong[u32EmitterLoop].m_fdEmitLon,&fdBearingAngle);
                    if(s32RetVal == DP_SUCCESS)
                    {
                        /// - Get the way point and Emitter Lat and Long Bearing Angle from the Emitter
                        s32RetVal =  DP_GetBearingAngle(in_pSdp_EmitLatLong[u32EmitterLoop].m_fdEmitLat,in_pSdp_EmitLatLong[u32EmitterLoop].m_fdEmitLon,in_pSdp_LatLong->m_arrfdLat[u32Loop],in_pSdp_LatLong->m_arrfdLon[u32Loop],&fdEmitBearingAngle);
                        if(s32RetVal == DP_SUCCESS)
                        {
                            /// - convert distance to km
                            fdDistance_km = fdDistance_m / 1000.0;

                            /// - if Distance is less than the radar actual range
                            /// - Copy that Emitter Lat,Long to the output array
                            if(fdDistance_km < in_pSdp_EmitLatLong[u32EmitterLoop].m_u16RdrRange_km)
                            {
                                /// - copy the Emitter Lat , Long , Distance , Angle to the Output array
                                out_pSWayPointEmitDetRange->m_SEmttrRange[u32WayPointCnt].m_SEmtrRelParam[u32EmitterCnt].m_fdEmitLat         = in_pSdp_EmitLatLong[u32EmitterLoop].m_fdEmitLat;
                                out_pSWayPointEmitDetRange->m_SEmttrRange[u32WayPointCnt].m_SEmtrRelParam[u32EmitterCnt].m_fdEmitLon         = in_pSdp_EmitLatLong[u32EmitterLoop].m_fdEmitLon;
                                out_pSWayPointEmitDetRange->m_SEmttrRange[u32WayPointCnt].m_SEmtrRelParam[u32EmitterCnt].m_u16RdrRange_km    = in_pSdp_EmitLatLong[u32EmitterLoop].m_u16RdrRange_km;
                                out_pSWayPointEmitDetRange->m_SEmttrRange[u32WayPointCnt].m_SEmtrRelParam[u32EmitterCnt].m_fdEmitterDist     = fdDistance_km;
                                out_pSWayPointEmitDetRange->m_SEmttrRange[u32WayPointCnt].m_SEmtrRelParam[u32EmitterCnt].m_fdDOAfromWayPoint = fdBearingAngle;
                                out_pSWayPointEmitDetRange->m_SEmttrRange[u32WayPointCnt].m_SEmtrRelParam[u32EmitterCnt].m_fdDOAfromEmitter  = fdEmitBearingAngle;

                                        /// - Increment the Emitter Count
                                        u32EmitterCnt = u32EmitterCnt+1;
                            }
                        }
                    }
                }
            }

            /// - Copy the WayPoint Lat Long to the Output Array
            out_pSWayPointEmitDetRange->m_SEmttrRange[u32WayPointCnt].m_fdWaypointLat    = in_pSdp_LatLong->m_arrfdLat[u32Loop];
            out_pSWayPointEmitDetRange->m_SEmttrRange[u32WayPointCnt].m_fdWaypointLon    = in_pSdp_LatLong->m_arrfdLon[u32Loop];
            out_pSWayPointEmitDetRange->m_SEmttrRange[u32WayPointCnt].m_u16TotalEmtCnt   = u32EmitterCnt;

            /// - Increment the WayPoint Count
            u32WayPointCnt = u32WayPointCnt + 1;
        }

        /// - Copy the WayPoint Cnt
        out_pSWayPointEmitDetRange->m_u16WayPointCnt       = u32WayPointCnt;
    }


    return s32RetVal;
}


/*!
 *
 *  \fn S32BIT  WriteWayPointEmitterLatLong(PS8BIT in_ps8FileName, PU32BIT out_pu32NoOfSamples, S_WAY_POINT_LAT_LONG *out_sdp_LatLong)
 *  \brief This function is used to read the Way Point Lat and Long
 *
 *  \param [in]   in_u32NoOfSamples      This is used to take the  Number of WayPoints sample
 *  \param [out]  in_u32EmitterSamples   This is used to take the  Number of Emitter sample
 *  \param [out]  in_pSWaypntEmtLoc      This is used to take the  latitude and Longitude of each WayPoint and the available Emitter lat and Long
 *
 *  \author Anunitha J
 *  \date   18 June 2026
 *
 *  \retval : DP_SUCCESS (or) DP_FAILURE
 *
 *  \pre
 *	- NA
 *	\post
 *	- NA
 *
*/
S32BIT WriteWayPointEmitterLatLong(U32BIT in_u32NoOfSamples,U32BIT in_u32EmitterSamples, S_DP_ASPJ_WAYPOINT_EMITTER_LOCATIONS *in_pSWaypntEmtLoc)
{
    S32BIT s32RetVal = DP_SUCCESS;     /// - s32RetVal Parameter used for Return the  Error values
    FILE *fp_LatLong = NULL;   /// - fp_RawImg Parameters are used for file pointer
    static S8BIT sarrTempData[10240] = {"WayPoint Latitude ,WayPoint Longitude,Emitter Count,"};
    U32BIT u32WayPoinLoop = 0;
    U32BIT u32EmitterLoop = 0;


    /// -  Inputs & Outputs pointers validation
    if(in_pSWaypntEmtLoc == NULL)
    {
        s32RetVal = DP_ASPJ_FAILURE;

    }
    else
    {
        /// - Open the csv files in Reading Mode
        fp_LatLong = fopen("Inputs\\WayPoint_Emitter_Distance.csv","w");

        /// - validate the file pointer
        if(fp_LatLong == NULL)
        {
            s32RetVal = DP_ASPJ_FAILURE;

        }
        else
        {

            fprintf(fp_LatLong,"%s",sarrTempData);

            /// -  To include Dynamic headers based on the Emitter Count
            for (u32EmitterLoop = 1; u32EmitterLoop <= in_u32EmitterSamples; u32EmitterLoop++)
            {
                fprintf(fp_LatLong, "Emitter%d Lat,Emitter%d Long,Emitter%d Range(km),Emitter%d Distance(km),Emitter%d Angle(deg)", u32EmitterLoop, u32EmitterLoop,u32EmitterLoop,u32EmitterLoop,u32EmitterLoop);

                if (u32EmitterLoop < in_u32EmitterSamples)
                {
                    fprintf(fp_LatLong, ",");
                }
            }

            fprintf(fp_LatLong, "\n"); /// -  End of header row

            for(u32WayPoinLoop = 0;u32WayPoinLoop < in_u32NoOfSamples;u32WayPoinLoop++)
            {
                /// - Write the Waypoint Latitude and Longitude and Emitter Count in the file
                fprintf(fp_LatLong,"%lf,%lf,%d,",in_pSWaypntEmtLoc->m_SEmttrRange[u32WayPoinLoop].m_fdWaypointLat,\
                        in_pSWaypntEmtLoc->m_SEmttrRange[u32WayPoinLoop].m_fdWaypointLon,\
                        in_pSWaypntEmtLoc->m_SEmttrRange[u32WayPoinLoop].m_u16TotalEmtCnt);

                for(u32EmitterLoop = 0;u32EmitterLoop < in_u32EmitterSamples;u32EmitterLoop++)
                {

                    if((u32EmitterLoop+1) == in_u32EmitterSamples)
                    {
                        fprintf(fp_LatLong,"%lf,%lf,%d,%lf,%lf",in_pSWaypntEmtLoc->m_SEmttrRange[u32WayPoinLoop].m_SEmtrRelParam[u32EmitterLoop].m_fdEmitLat,\
                                in_pSWaypntEmtLoc->m_SEmttrRange[u32WayPoinLoop].m_SEmtrRelParam[u32EmitterLoop].m_fdEmitLon,\
                                in_pSWaypntEmtLoc->m_SEmttrRange[u32WayPoinLoop].m_SEmtrRelParam[u32EmitterLoop].m_u16RdrRange_km,\
                                in_pSWaypntEmtLoc->m_SEmttrRange[u32WayPoinLoop].m_SEmtrRelParam[u32EmitterLoop].m_fdEmitterDist/1000.0,\
                                in_pSWaypntEmtLoc->m_SEmttrRange[u32WayPoinLoop].m_SEmtrRelParam[u32EmitterLoop].m_fdDOAfromEmitter,\
                                in_pSWaypntEmtLoc->m_SEmttrRange[u32WayPoinLoop].m_SEmtrRelParam[u32EmitterLoop].m_fdAOAfromWaypoint);
                    }
                    else
                    {
                        fprintf(fp_LatLong,"%lf,%lf,%d,%lf,%lf,",in_pSWaypntEmtLoc->m_SEmttrRange[u32WayPoinLoop].m_SEmtrRelParam[u32EmitterLoop].m_fdEmitLat,\
                                in_pSWaypntEmtLoc->m_SEmttrRange[u32WayPoinLoop].m_SEmtrRelParam[u32EmitterLoop].m_fdEmitLon,\
                                in_pSWaypntEmtLoc->m_SEmttrRange[u32WayPoinLoop].m_SEmtrRelParam[u32EmitterLoop].m_u16RdrRange_km,\
                                in_pSWaypntEmtLoc->m_SEmttrRange[u32WayPoinLoop].m_SEmtrRelParam[u32EmitterLoop].m_fdEmitterDist/1000.0,\
                                in_pSWaypntEmtLoc->m_SEmttrRange[u32WayPoinLoop].m_SEmtrRelParam[u32EmitterLoop].m_fdDOAfromEmitter,\
                                in_pSWaypntEmtLoc->m_SEmttrRange[u32WayPoinLoop].m_SEmtrRelParam[u32EmitterLoop].m_fdAOAfromWaypoint);

                    }
                }

                fprintf(fp_LatLong,"\n");
            }

            /// - Close the csv file
            fclose(fp_LatLong);
        }
    }

    return s32RetVal;
}


/*!
 *
 *  \fn S32BIT  Calculate_TotalDistance_And_RelParam(S_WAY_POINT_LAT_LONG *in_pSdp_LatLong,S_EMITTER_LAT_LONG *in_pSdp_EmitLatLong,\
                                             S_WAYPOINT_EMITTER_LOCATIONS *out_pSWayPointEmitter)
 *  \brief This function is used to calculate the Emitter Relative Parameters corresponding to each WayPoint Latitude and Longitude
 *
 *  \param [in]    in_pSdp_LatLong          This is used to take the Way Point latitude and Longitude
 *  \param [in]    in_pSdp_EmitLatLong      This is used to take the Emitter latitude and Longitude
 *  \param [out]   out_pfdTotalDistance     This is used to store the Total Distance waypoint lat and long covered
 *  \param [out]   out_pSWayPointEmitter    This is used to store the Emitter count ,distance and angle for each waypoint lat and long
 *
 *  \author Anunitha J
 *  \date   18 June 2026
 *
 *  \retval : DP_SUCCESS (or) DP_FAILURE
 *
 *  \pre
 *	- NA
 *	\post
 *	- NA
 *
*/
S32BIT Calculate_TotalDistance_And_RelParam(S_DP_ASPJ_WAYPOINTS_LAT_LON *in_pSdp_LatLong,S_DP_ASPJ_EMITTERS_LAT_LON *in_pSdp_EmitLatLong,\
                                            FDOUBLE in_pfdTimeTaken, S_DP_ASPJ_WAYPOINT_EMITTER_LOCATIONS *out_pSWayPointEmitDetRange)
{
    S32BIT s32Retval            = 0;

    if((in_pSdp_LatLong == NULL) || (in_pSdp_EmitLatLong == NULL) || (out_pSWayPointEmitDetRange == NULL))
    {
        s32Retval = DP_ASPJ_FAILURE;
    }
    else
    {
        /// - Calculate the Emitter Relative Parameters
        s32Retval = GetEmitterRelativeParameters(in_pSdp_LatLong,in_pSdp_EmitLatLong, out_pSWayPointEmitDetRange);
        if(s32Retval == DP_SUCCESS)
        {
            /// - Use the output Way Point structure as a input parameter
            /// - Calculate the Total distance
            s32Retval = Calculate_WayPoints_TotalDistance(out_pSWayPointEmitDetRange, out_pSWayPointEmitDetRange);
            if(s32Retval == DP_SUCCESS)
            {
                /// - Use the output Way Point structure as a input parameter
                /// - Calculate the Heading Angle towards the Next WayPoints
                s32Retval = Calculate_WayPoints_HeadingAngle(out_pSWayPointEmitDetRange, out_pSWayPointEmitDetRange);
                if(s32Retval == DP_SUCCESS)
                {
                    /// - calculate the velocity of the aircraft travelling at a time for a distance between each waypoint
                    s32Retval = Calculate_Velocity_for_Each_WayPoint(in_pfdTimeTaken, out_pSWayPointEmitDetRange, out_pSWayPointEmitDetRange);
                    if(s32Retval == DP_SUCCESS)
                    {
                        /// - Calculate the Angle of Arrival for each WayPoints w.r.t to the Emitter in range
                        s32Retval = Calculate_AOA_for_EachEmitter(out_pSWayPointEmitDetRange, out_pSWayPointEmitDetRange);
                        if(s32Retval == DP_SUCCESS)
                        {
                            s32Retval = DP_SUCCESS;
                        }
                    }
                }
            }
        }
    }


    return s32Retval;
}

/*!
 *
 *  \fn S32BIT  Calculate_WayPoints_TotalDistance(S_WAYPOINT_EMITTER_LOCATIONS in_SWayPntlatLong, S_WAYPOINT_EMITTER_LOCATIONS *out_pSWayPointEmitter)
 *  \brief This function is used to read the Way Point Lat and Long
 *
 *  \param [in]   in_pSWayPntlatLong        This is used to take the  WayPoints Latitudes and Longitudes
 *  \param [out]  out_pfdTotalDistance      This is used to store the  Total Distance
 *  \param [out]  out_pSWayPointEmitter     This is used to store the  Distance covered in each lat and long of waypoints
 *
 *  \author Anunitha J
 *  \date   18 June 2026
 *
 *  \retval : DP_SUCCESS (or) DP_FAILURE
 *
 *  \pre
 *	- NA
 *	\post
 *	- NA
 *
*/

S32BIT Calculate_WayPoints_TotalDistance(const S_DP_ASPJ_WAYPOINT_EMITTER_LOCATIONS *in_pSWayPntlatLong, S_DP_ASPJ_WAYPOINT_EMITTER_LOCATIONS *out_pSWayPointEmitter)
{
    S32BIT s32Retval            = 0;
    U32BIT u32Loop              = 0;
    FDOUBLE fdDistance_m        = 0.0f;
    FDOUBLE fdDistanceCovered_m = 0.0f;

    DP_VALIDATE_WAYPOINT_CNT(out_pSWayPointEmitter->m_u16WayPointCnt);         /// - Validate the WayPoint Count

    if(in_pSWayPntlatLong == NULL  || out_pSWayPointEmitter == NULL)
    {
        s32Retval = DP_ASPJ_FAILURE;
    }
    else
    {

        /// - For Each WayPoint Lat, Long loop
        for(u32Loop = 0;u32Loop < out_pSWayPointEmitter->m_u16WayPointCnt;u32Loop++)
        {
            if(u32Loop == 0)
            {
                continue;
            }
            else
            {
                /// - Get the way point and Distance between Two points
                s32Retval =  DP_GetDistance(in_pSWayPntlatLong->m_SEmttrRange[u32Loop].m_fdWaypointLat, in_pSWayPntlatLong->m_SEmttrRange[u32Loop].m_fdWaypointLon,in_pSWayPntlatLong->m_SEmttrRange[u32Loop-1].m_fdWaypointLat,in_pSWayPntlatLong->m_SEmttrRange[u32Loop-1].m_fdWaypointLon,&fdDistance_m);
                if(s32Retval == DP_SUCCESS)
                {
                    /// - add the distance covered in each point
                    fdDistanceCovered_m  = fdDistanceCovered_m + fdDistance_m;
                }
            }

            /// - convert the distance to km
            out_pSWayPointEmitter->m_SEmttrRange[u32Loop].m_fdDistCovered_km   = fdDistanceCovered_m / 1000.0f;
        }

    }

    return s32Retval;

}

/*!
 *
 *  \fn S32BIT  Calculate_WayPoints_HeadingAngle(S_DP_ASPJ_WAYPOINT_EMITTER_LOCATIONS in_SWayPntlatLong,S_DP_ASPJ_WAYPOINT_EMITTER_LOCATIONS *out_pSWayPointEmitter)
 *  \brief This function is used to calculate the Heading Angle between two Waypoints
 *
 *  \param [in]   in_pSWayPntlatLong        This is used to take the  WayPoints Latitudes and Longitudes
 *  \param [out]  out_pSWayPointEmitter     This is used to store the  Distance covered in each lat and long of waypoints
 *
 *  \author Anunitha J
 *  \date   18 June 2026
 *
 *  \retval : DP_SUCCESS (or) DP_FAILURE
 *
 *  \pre
 *	- NA
 *	\post
 *	- NA
 *
*/

S32BIT Calculate_WayPoints_HeadingAngle(const S_DP_ASPJ_WAYPOINT_EMITTER_LOCATIONS *in_pSWayPntlatLong, S_DP_ASPJ_WAYPOINT_EMITTER_LOCATIONS *out_pSWayPointEmitter)
{
    S32BIT s32Retval            = 0;
    U32BIT u32Loop              = 0;

    FDOUBLE fdAngle             = 0.0;

    DP_VALIDATE_WAYPOINT_CNT(in_pSWayPntlatLong->m_u16WayPointCnt);         /// - Validate the WayPoint Count

    if(in_pSWayPntlatLong == NULL || out_pSWayPointEmitter == NULL)
    {
        s32Retval = DP_ASPJ_FAILURE;
    }
    else
    {

        /// - For Each WayPoint Lat, Long loop
        for(u32Loop = 0;u32Loop < in_pSWayPntlatLong->m_u16WayPointCnt;u32Loop++)
        {
            fdAngle = 0.0;
            if(u32Loop+1 == in_pSWayPntlatLong->m_u16WayPointCnt)
            {
                /// - For last lat long , copy Previous lat long values
                out_pSWayPointEmitter->m_SEmttrRange[u32Loop].m_fdHeadingAngle = out_pSWayPointEmitter->m_SEmttrRange[u32Loop-1 ].m_fdHeadingAngle;
            }
            else
            {
                /// - Get the way point and Distance between Two points
                s32Retval =  DP_GetBearingAngle(in_pSWayPntlatLong->m_SEmttrRange[u32Loop].m_fdWaypointLat,in_pSWayPntlatLong->m_SEmttrRange[u32Loop].m_fdWaypointLon,in_pSWayPntlatLong->m_SEmttrRange[u32Loop+1].m_fdWaypointLat,in_pSWayPntlatLong->m_SEmttrRange[u32Loop+1].m_fdWaypointLon, &fdAngle);
                if(s32Retval == DP_SUCCESS)
                {
                    /// - Copy the Heading Angle
                    out_pSWayPointEmitter->m_SEmttrRange[u32Loop].m_fdHeadingAngle  =  fdAngle;
                }
            }
        }
    }

    return s32Retval;

}

/*!
 *
 *  \fn S32BIT  Calculate_AOA_for_EachEmitter(S_DP_ASPJ_WAYPOINT_EMITTER_LOCATIONS in_SWayPntlatLong, S_DP_ASPJ_WAYPOINT_EMITTER_LOCATIONS *out_pSWayPointEmitter)
 *  \brief This function is used to calculate the Angle of Arrival of a Emitter w.r.t to each WayPoint lat Long
 *
 *  \param [in]   in_pSWayPntlatLong        This is used to take the  WayPoints Latitudes and Longitudes
 *  \param [out]  out_pSWayPointEmitter     This is used to store the  Distance covered in each lat and long of waypoints
 *
 *  \author Anunitha J
 *  \date   18 June 2026
 *
 *  \retval : DP_SUCCESS (or) DP_FAILURE
 *
 *  \pre
 *	- NA
 *	\post
 *	- NA
 *
*/
S32BIT Calculate_AOA_for_EachEmitter(const S_DP_ASPJ_WAYPOINT_EMITTER_LOCATIONS *in_pSWayPntlatLong, S_DP_ASPJ_WAYPOINT_EMITTER_LOCATIONS *out_pSWayPointEmitter)
{
    S32BIT s32RetVal            = 0;
    U32BIT u32Loop              = 0;
    U32BIT u32EmitterLoop       = 0;
    FDOUBLE fdAngleofArrival    = 0.0;

    if(out_pSWayPointEmitter == NULL || in_pSWayPntlatLong == NULL)
    {
        s32RetVal  = DP_ASPJ_FAILURE;
    }
    else
    {
        /// - For Each WayPoint Lat, Long loop
        for(u32Loop = 0;u32Loop < in_pSWayPntlatLong->m_u16WayPointCnt;u32Loop++)
        {

            /// - For Each WayPoint Lat, Long all the emitter distance and angle is calculated
            for(u32EmitterLoop = 0;u32EmitterLoop < in_pSWayPntlatLong->m_SEmttrRange[u32Loop].m_u16TotalEmtCnt;u32EmitterLoop++)
            {
                /// -> calculate the Aircraft angle of arrival
                /// -> AOA = DOA - Heading Angle
                fdAngleofArrival = in_pSWayPntlatLong->m_SEmttrRange[u32Loop].m_SEmtrRelParam[u32EmitterLoop].m_fdDOAfromWayPoint  - in_pSWayPntlatLong->m_SEmttrRange[u32Loop].m_fdHeadingAngle;

                /// - wrap the Angle to 0-360 degree
                out_pSWayPointEmitter->m_SEmttrRange[u32Loop].m_SEmtrRelParam[u32EmitterLoop].m_fdAOAfromWaypoint  = fmod(fdAngleofArrival+360.0, 360.0);

            }
        }
    }

    return s32RetVal;
}

/*!
 *
 *  \fn S32BIT  Calculate_Velocity_for_Each_WayPoint(FDOUBLE in_pfdTimeTaken_ms,const S_DP_ASPJ_WAYPOINT_EMITTER_LOCATIONS *in_pSWayPntlatLong,S_DP_ASPJ_WAYPOINT_EMITTER_LOCATIONS *out_pSWayPointEmitter)
 *  \brief This function is used to read the Way Point Lat and Long
 *
 *  \param [in]   in_pfdTimeTaken_ms        This parameter gives the time taken to travel from one waypoint to another
 *  \param [in]   in_pSWayPntlatLong        This is used to take the  WayPoints Latitudes and Longitudes
 *  \param [out]  out_pSWayPointEmitter     This is used to store the  Distance covered in each lat and long of waypoints
 *
 *  \author Anunitha J
 *  \date   18 June 2026
 *
 *  \retval : DP_SUCCESS (or) DP_FAILURE
 *
 *  \pre
 *	- NA
 *	\post
 *	- NA
 *
*/

S32BIT Calculate_Velocity_for_Each_WayPoint(FDOUBLE in_pfdTimeTaken_ms,const S_DP_ASPJ_WAYPOINT_EMITTER_LOCATIONS *in_pSWayPntlatLong,S_DP_ASPJ_WAYPOINT_EMITTER_LOCATIONS *out_pSWayPointEmitter)
{
    S32BIT s32Retval            = 0;
    U32BIT u32Loop              = 0;

    FDOUBLE fdVelocity_ms       = 0.0;

    DP_VALIDATE_WAYPOINT_CNT(in_pSWayPntlatLong->m_u16WayPointCnt);         /// - Validate the WayPoint Count

    if(in_pSWayPntlatLong == NULL || out_pSWayPointEmitter == NULL)
    {
        s32Retval = DP_ASPJ_FAILURE;
    }
    else
    {

        /// - For Each WayPoint Lat, Long loop
        for(u32Loop = 0;u32Loop < in_pSWayPntlatLong->m_u16WayPointCnt;u32Loop++)
        {
            if(u32Loop == 0)
            {
                continue;
            }
            else
            {
               /// - velocity in km/h = distance_km / time_hours  (time_hours = time_ms / 3600000)
               fdVelocity_ms  = in_pSWayPntlatLong->m_SEmttrRange[u32Loop].m_fdDistCovered_km / (in_pfdTimeTaken_ms / 3600000.0);

               /// - store the velocity in the structure
               out_pSWayPointEmitter->m_SEmttrRange[u32Loop].m_fdVelocity_ms  = fdVelocity_ms;

               //qDebug() << "VElocity in " << out_pSWayPointEmitter->m_SEmttrRange[u32Loop].m_fdVelocity_ms;

            }
        }
    }

    return s32Retval;

}
