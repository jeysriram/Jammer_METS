#ifndef DP_ASPJ_STRUCTURES_H
#define DP_ASPJ_STRUCTURES_H

#include "dp_aspj_comm.h"



/// ------------------------------------------------------- ///////
/// ---------------INS GPS DISPLAY STRUCTURES--------------- //////
/// ------------------------------------------------------- ///////

typedef struct
{
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

    unsigned short usReserved				: 11;
    unsigned short usAC_On_GND_Status		: 1;	/* 1-Valid, 0-Invalid -- Not as per ICD -- Presently not used*/
    unsigned short usGPS_Data_Valid			: 1;	/* 1-Valid, 0-Invalid -- Not as per ICD -- Presently not used*/
    unsigned short usRadio_HT_Valid			: 1;	/* 1-Valid, 0-Invalid -- Not as per ICD -- Presently not used*/
    unsigned short usBaro_Altitude_Valid	: 1;	/* 1-Valid, 0-Invalid -- Not as per ICD -- Presently not used*/
    unsigned short usINS_Parm_Valid         : 1;	/* 1-Valid, 0-Invalid -- Not as per ICD*/

#else

    unsigned short usINS_Parm_Valid         : 1;	/* 1-Valid, 0-Invalid -- Not as per ICD*/
    unsigned short usBaro_Altitude_Valid	: 1;	/* 1-Valid, 0-Invalid -- Not as per ICD -- Presently not used*/
    unsigned short usRadio_HT_Valid			: 1;	/* 1-Valid, 0-Invalid -- Not as per ICD -- Presently not used*/
    unsigned short usGPS_Data_Valid			: 1;	/* 1-Valid, 0-Invalid -- Not as per ICD -- Presently not used*/
    unsigned short usAC_On_GND_Status		: 1;	/* 1-Valid, 0-Invalid -- Not as per ICD -- Presently not used*/
    unsigned short usReserved				: 11;

#endif
}S_MC_VALIDITY_WORD;

typedef struct
{
    S_MC_VALIDITY_WORD	S_Validity_Word;
    unsigned short		usReserved[3];

    double				dLatitude_deg;
    double				dLongitude_deg;
    double				dVerticalSpeed_fpm;

    float				fTrueHeading_deg;
    float				fTrueAltitude_m;

    float				fRollAngle_deg;
    float				fPitchAngle_deg;

    float				fPitchRate_deg_s;
    float				fRollRate_deg_s;
    float				fYawRate_deg_s;

}S_RWR_NAV_DATA_ENGG_UNIT;



typedef struct
{
    S_RWR_NAV_DATA_ENGG_UNIT S_NTP_NAV_Data;
    //    DP_RWR_NTP_NAV_DATA S_NTP_NAV_Data;
    unsigned char		ucNAV_DataValid;
    unsigned char		ucNTP_DataValid;
    unsigned char		ucGNSS_ConnectionSts;
    unsigned char		ucIs_INS_GPS_Fix_Valid;
}S_RES_INS_GPS_DATA;

typedef struct
{
    QRect DispArea;

    int m_iXpos;
    int m_iYpos;
    int m_iWidth;
    int m_iHeight;

    int	m_iBitmapStartXPos;
    int	m_iBitmapStartYPos;
    int m_iBitmapWidth;
    int	m_iBitmapHeight;

    int	m_iDispStartXPos;
    int m_iDispStartYPos;
    int	m_iDispWidth;
    int m_iDispHeight;
}S_POLARPLOT_DISPLAY_PARAM;

typedef struct
{
    double          dThreatX_Pos;
    double          dThreatY_Pos;

    unsigned char   ucThreatSymbolSize;
    unsigned char   ucJammingSymbolFlag;
    unsigned char   ucReserved[2];

    float           fRange;
    float           fAngle;
    double          dFrequency;

    unsigned char   ucIsNewestThreat;
    unsigned char   ucThreatOrgin;	/*Airborne / Ground based threat*/
    unsigned char   ucThreatScanStat;   /*Tracking / Locked*/
    unsigned char   ucThreatClassification;

    char            arrcThreatSymbolName[DP_RWR_EMITTER_SYMBOL_NAME_LEN];

    unsigned char   ucDispPriority;
    unsigned char   ucFG_Colour;
    unsigned char   ucBG_Colour;
    unsigned short  usSymbolCode;

    char            arrcEmitterId[20];
    char            arrcEmitterModeID[DP_SYMBOL_EMITTER_MODE_ID_STR_LEN];

    float           fAmplitude_dBm;
    float           fDOA_Deg;
}S_RWR_THREAT_SYMBOL_INFO;

typedef struct
{
    S_RWR_THREAT_SYMBOL_INFO arrS_RWRThreatSymbolInfo[DP_RWR_MAX_EMITTERS];
    unsigned long ulThreatCount;
}S_RWR_THREAT_LIST;


typedef struct
{
    QRect			boundary;
    QRect			DispArea;

    /**** For GDI + Drawings ****/
    unsigned long	ulgdiplusToken;

    int				m_iBitmapStartXPos;
    int				m_iBitmapStartYPos;
    int				m_iBitmapWidth;
    int				m_iBitmapHeight;

    int		    	m_iDispXPos;
    int		    	m_iDispYPos;
    int				m_iDispWidth;
    int				m_iDispHeight;

    int		    	m_iDispClippedXPos;
    int		    	m_iDispClippedYPos;
    int		    	m_iDispClippedWidth;
    int		    	m_iDispClippedHeight;

    QPen			cPen;

    unsigned char	m_ucRunCount;
}DP_EMITTER_REPORT_DISPLAY_PARAM;

typedef struct
{
    unsigned char ucEmitterSymbolCode;
    unsigned char ucEmitterAlternatSymbolCode;
    unsigned char ucThreatPlatform;
    char arrcEmitterSymbol[DP_RWR_EMITTER_SYMBOL_NAME_LEN];
    char arrcEmitterSymbolDesc[DP_RWR_EMITTER_SYMBOL_DESC_STR_LEN];

}S_EMITTER_SYMBOL_DESCRIPTION;


typedef struct
{
    S_EMITTER_SYMBOL_DESCRIPTION SarrDispEmitterSymbolDesc[DP_RWR_MAX_THREAT_SYMBOL_DATA_COUNT];
    unsigned short				 usSymbolCount;

}S_EMITTER_SYMBOL_LIBRARY;



/*!
*	\def S_WAY_POINT_LAT_LONG
*	\brief This structure is used for storing the Way points Latitude and Longitude
*/
typedef struct
{
    U16BIT  m_u16BinFlag;           /// This variable is used to store the Binning enable or disable flag
    U16BIT  m_u16BinningCnt;        /// This variable is used for storing the Binning Count

    U32BIT  m_u32LatLongCnt;        /// WayPoint Lat Long Count

    FDOUBLE m_arrfdLat[DP_MAX_WAYPOINT_LAT_LONG];                /// This variable is used to take the waypoint latitude
    FDOUBLE m_arrfdLon[DP_MAX_WAYPOINT_LAT_LONG];                /// This variable is used to take the waypoint longitudes

}S_DP_ASPJ_WAYPOINTS_LAT_LON;


/*!
*	\def S_EMITTER_LAT_LONG
*	\brief This structure is used for storing the Emitter Latitude and Longitude
*/
typedef struct
{
    U16BIT  m_u16RdrRange_km;       /// This variable is used to store the Emitter Range in Km
    U16BIT  m_u16EmtLatLongCnt;     /// Emitter Lat Long count

    FDOUBLE m_fdEmitLat;            /// This variable is used to take the emitter latitude
    FDOUBLE m_fdEmitLon;            /// This variable is used to take the emitter longitudes

}S_DP_ASPJ_EMITTERS_LAT_LON;


typedef struct
{
    U16BIT  m_u16RdrRange_km;       /// This variable is used to store the Emitter Range in Km
    U16BIT  m_u32EmtLatLongCnt;     /// Emitter Lat Long count

    FDOUBLE m_fdEmitLat;            /// This variable is used to take the emitter latitude
    FDOUBLE m_fdEmitLon;            /// This variable is used to take the emitter longitudes

    FDOUBLE m_fdEmitterDist;        /// This variable is used to store the Emitter Distance from the Waypoint Latitude
    FDOUBLE m_fdDOAfromWayPoint;    /// This variable is used to store the Emitter Angle from the Waypoint Latitude which is DOA(Direction of Angle)

    FDOUBLE m_fdDOAfromEmitter;     /// This variable is used to store the Emitter Angle from the Waypoint Latitude which is AOA(Arrival of Angle)
    FDOUBLE m_fdAOAfromWaypoint;    /// This variable is used to store the Emitter Angle from the Waypoint Latitude which is AOA(Arrival of Angle)

}S_EMITTER_REL_PARAM;


/*!
*	\def S_EMITTER_WITHIN_RADAR_RANGE
*	\brief This structure is used for Storing the Emitter location within the Range of corresponding Latitude and Longitude
*/
typedef struct
{
    FDOUBLE m_fdWaypointLat;                                        /// This variable is used to take the waypoint latitude
    FDOUBLE m_fdWaypointLon;                                        /// This variable is used to take the waypoint longitudes

    FDOUBLE m_fdDistCovered_km;                                     /// This variable is used to store the Distance covered from 1 lat to other
    FDOUBLE m_fdHeadingAngle;                                       /// This variable is used to store the Heading angle of aircraft

    FDOUBLE m_fdVelocity_ms;                                        /// This variable is used to store the velocity in km/h for each waypoint segment

    U16BIT  m_u16TotalEmtCnt;                                      /// This variable is used to store the count of Emitter Available
    U16BIT  m_u16Reserved;                                          /// Reserved for future use

    S_EMITTER_REL_PARAM m_SEmtrRelParam[DP_MAX_EMITTER_LAT_LONG];  /// This variable stores the Emitter latitude and longitude

}S_DP_ASPJ_EMIT_WITHIN_RANGE;

/*!
*	\def S_WAYPOINT_EMITTER_LOCATIONS
*	\brief This structure is used for Storing the Emitter location within the Range of of all WayPoint Latitude and Longitude
*/

typedef struct
{
    U16BIT  m_u16WayPointCnt;                                       /// Total Waypoint count
    U16BIT  m_u16Reserved;                                          /// Reserved for future use

    S_DP_ASPJ_EMIT_WITHIN_RANGE   m_SEmttrRange[DP_MAX_WAYPOINT_LAT_LONG];    /// This variable is used to store all the Waypoint Lat and Long and corresponding emitter within Radar range

} S_DP_ASPJ_WAYPOINT_EMITTER_LOCATIONS;





/// - Global handle structures
typedef struct
{
    S_DP_ASPJ_EMITTERS_LAT_LON              m_sEmitterLatLon[20];
    S_DP_ASPJ_WAYPOINTS_LAT_LON             m_sWaypointsLatLon;
    S_DP_ASPJ_WAYPOINT_EMITTER_LOCATIONS    m_sEmitDetWithinRange;
    S_EMITTER_SYMBOL_LIBRARY                m_sEmitterSymbolLibrary;


    class CDP_EmitterModel                 *pm_cobj_emitterModel;
    class CDP_FlightController             *pm_cobj_flightController;
    class CLoadSettings                    *pm_cobj_loadSettings;
    class dp_rwr_flight_display            *pm_cobj_flightCockPitDisplay;
    class CDP_FlightPlaybackControl        *pm_cobj_flightPlaybackCtrl;
    class dp_rwr_nav_flight_display        *pm_cobj_navflightDisplay;

    unsigned short                          m_arrusSpecialSymbolCodes[DP_RWR_MAX_NUM_OF_SPECIAL_SYMBOLS];


}S_DP_ASPJ_HANDLE;

extern S_DP_ASPJ_HANDLE g_SDP_ASPJ_Handle;


#endif // DP_ASPJ_STRUCTURES_H
