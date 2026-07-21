#ifndef DP_ASPJ_MACROS_H
#define DP_ASPJ_MACROS_H

#include <QRect>
#include <QPen>
#include <stdio.h>






#define DP_ASPJ_SUCCESS                             0
#define DP_ASPJ_FAILURE                             -1

//#define DP_ASPJ_DEBUG_PRINTS

/* Map source selection.
 * Leave commented to use the existing custom DTARANG tile server (offline/LAN).
 * Uncomment to switch MapView.qml to online OpenStreetMap tiles instead.
 * The toggle is bridged to QML as the "g_bUseOnlineMap" context property. */
#define DP_USE_ONLINE_MAP

#define DP_POLAR_SUCCESS                            0
#define DP_POLAR_FAILURE                            -1

#define DP_MAX_ROWS                                 4096
#define DP_LATIITUDE_COL                            0
#define DP_LONGITUDE_COL                            1

#define DP_QT_EMITTER_REPORT_INITIALIZE             0

#define DP_POLAR_PLOT_FONT                          "Arial"
#define DP_MAX_THREAT_SYMBOL_LIST                   18000
#define DP_THREAT_SYMBOL_SIZE                       15
#define DP_LETTERS_FONT_SIZE                        10
#define DP_PLATFORM_SYMBOL_SIZE                     10

#define DP_POLAR_MIN_FREQUENCY                      500.0
#define DP_POLAR_MAX_FREQUENCY                      18000.0

#define DP_POLAR_MIN_ANGLE                          0.0f
#define DP_POLAR_MAX_ANGLE                          360.0f

#define DP_POLAR_PLOT_INITIALIZE                    0
#define DP_POLAR_PLOT_CLOCKWISE                     1
#define DP_POLAR_PLOT_ANTICLOCKWISE                 0

#define DP_RWR_EMITTER_SYMBOL_NAME_LEN              3
#define DP_RWR_MAX_TRACKED_EMITTERS                 1024
#define	DP_SYMBOL_EMITTER_MODE_ID_STR_LEN           8
#define DP_RWR_MIG29K_MAX_JAM_TECHNIQUES            3
#define DP_RWR_MAX_EMITTERS                         128

#define DP_POLAR_PLOT_BOUNDARY_X                    5
#define DP_POLAR_PLOT_BOUNDARY_Y                    10

#define DP_POLAR_PLOT_BLACK_COLOR_CODE              0, 0, 0
#define DP_POLAR_PLOT_WHITE_COLOR_CODE              255, 255, 255
#define DP_POLAR_PLOT_RED_COLOR_CODE                255, 0, 0
#define DP_POLAR_PLOT_GREEN_COLOR_CODE              0, 255, 0
#define DP_POLAR_PLOT_ORANGE_COLOR_CODE             255,165,0

#define DP_ONE_DEGREE_VALUE                         16
#define DP_CIRCLE_360_DEGREE                        5760
#define DP_CIRCLE_90_DEGREE                         1440
#define DP_CIRCLE_0_TO_180_DEGREE                   2880
#define DP_CIRCLE_0_TO_270_DEGREE                   4320

#define DP_ANGLE_OF_360_DEGREE                      360
#define DP_ANGLE_OF_330_DEGREE                      330
#define DP_ANGLE_OF_300_DEGREE                      300
#define DP_ANGLE_OF_270_DEGREE                      270
#define DP_ANGLE_OF_240_DEGREE                      240
#define DP_ANGLE_OF_210_DEGREE                      210
#define DP_ANGLE_OF_180_DEGREE                      180
#define DP_ANGLE_OF_150_DEGREE                      150
#define DP_ANGLE_OF_120_DEGREE                      120
#define DP_ANGLE_OF_90_DEGREE                       90
#define DP_ANGLE_OF_60_DEGREE                       60
#define DP_ANGLE_OF_30_DEGREE                       30
#define DP_ANGLE_OF_0_DEGREE                        0
#define DP_TOTAL_DEGREE_VALUE                       12

#define DP_POLAR_PLOT_BITMAP_BOUNDARY_START_POS_OFFSET     19.0f
#define DP_POLAR_PLOT_BITMAP_BOUNDARY_END_POS_OFFSET       200.0f

#define MAX_FREQ                                    18000.0f
#define MIN_FREQ                                    500.0f
#define MAX_RNG                                     1.0f
#define MIN_RNG                                     0.0f

#define FREQ_TO_RNG_FORMULA(fRng, fFreq) (fRng = ((((fFreq - MIN_FREQ)*(MAX_RNG - MIN_RNG) / (MAX_FREQ - MIN_FREQ))) + MIN_RNG))

#define DP_RWR_COLOR_CODE_BLACK                      0
#define DP_RWR_COLOR_CODE_GREEN                      1
#define DP_RWR_COLOR_CODE_RED                        2
#define DP_RWR_COLOR_CODE_YELLOW                     3
#define DP_RWR_COLOR_CODE_BLUE                       4
#define DP_RWR_COLOR_CODE_CYAN                       5
#define DP_RWR_COLOR_CODE_MAGENTA                    6
#define DP_RWR_COLOR_CODE_WHITE                      7
#define DP_RWR_COLOR_CODE_LIGHT_BLACK                8
#define DP_RWR_COLOR_CODE_LIGHT_GREEN                9
#define DP_RWR_COLOR_CODE_LIGHT_RED                  10
#define DP_RWR_COLOR_CODE_LIGHT_YELLOW               11
#define DP_RWR_COLOR_CODE_LIGHT_BLUE                 12
#define DP_RWR_COLOR_CODE_LIGHT_CYAN                 13
#define DP_RWR_COLOR_CODE_LIGHT_MAGENTA              14
#define DP_RWR_COLOR_CODE_LIGHT_WHITE                15

#define DP_RWR_COLOR_BLACK                            0,0,0
#define DP_RWR_COLOR_GREEN                            0,255,0
#define DP_RWR_COLOR_RED                              255,0,0
#define DP_RWR_COLOR_YELLOW                           255,255,0
#define DP_RWR_COLOR_BLUE                             0,0,255
#define DP_RWR_COLOR_CYAN                             0,255,255
#define DP_RWR_COLOR_MAGENTA                          255,0,255
#define DP_RWR_COLOR_WHITE                            255,255,255
#define DP_RWR_COLOR_LIGHT_BLACK                      0,0,0
#define DP_RWR_COLOR_LIGHT_GREEN                      32,255,32
#define DP_RWR_COLOR_LIGHT_RED                        255,128,128
#define DP_RWR_COLOR_LIGHT_YELLOW                     255,255,128
#define DP_RWR_COLOR_LIGHT_BLUE                       128,128,255
#define DP_RWR_COLOR_LIGHT_CYAN                       128,255,255
#define DP_RWR_COLOR_LIGHT_MAGENTA                    255,128,255
#define DP_RWR_COLOR_LIGHT_WHITE                      255,255,230

#define DP_RWR_LOAD_CONFIG                            true
#define DP_RWR_SAVE_CONFIG                            false
#define DP_RWR_EMITTER_SYMBOL_DESC_STR_LEN            17
#define DP_RWR_MAX_THREAT_SYMBOL_DATA_COUNT           1024

#define DP_RWR_MAX_NUM_OF_SPECIAL_SYMBOLS             21

#define DP_THREAT_SQUARE_CODE                         150
#define DP_THREAT_PLUS_CODE                           151
#define DP_THREAT_CROSS_CODE                          152
#define DP_THREAT_TRIANGLE_CODE                       153
#define DP_THREAT_DIAMOND_CODE                        154
#define DP_THREAT_CIRCLE_CODE                         155

#define DP_THREAT_SYMBOL_CODE_42                      42
#define DP_THREAT_SYMBOL_CODE_43                      43
#define DP_THREAT_SYMBOL_CODE_44                      44
#define DP_THREAT_SYMBOL_CODE_45                      45
#define DP_THREAT_SYMBOL_CODE_46                      46
#define DP_THREAT_SYMBOL_CODE_47                      47
#define DP_THREAT_SYMBOL_CODE_49                      49
#define DP_THREAT_SYMBOL_CODE_50                      50
#define DP_THREAT_SYMBOL_CODE_51                      51
#define DP_THREAT_SYMBOL_CODE_52                      52
#define DP_THREAT_SYMBOL_CODE_53                      53
#define DP_THREAT_SYMBOL_CODE_54                      54
#define DP_THREAT_SYMBOL_CODE_55                      55
#define DP_THREAT_SYMBOL_CODE_56                      56
#define DP_THREAT_SYMBOL_CODE_57                      57
#define DP_THREAT_SYMBOL_CODE_58                      58
#define DP_THREAT_SYMBOL_CODE_59                      59
#define DP_THREAT_SYMBOL_CODE_60                      60
#define DP_THREAT_SYMBOL_CODE_61                      61
#define DP_THREAT_SYMBOL_CODE_62                      62
#define DP_THREAT_SYMBOL_CODE_63                      63

#define DP_THREAT_SYMBOL_CODE_35                      35
#define DP_THREAT_SYMBOL_CODE_36                      36
#define DP_THREAT_SYMBOL_CODE_37                      37
#define DP_THREAT_SYMBOL_CODE_38                      38
#define DP_THREAT_SYMBOL_CODE_39                      39
#define DP_THREAT_SYMBOL_CODE_40                      40
#define DP_THREAT_SYMBOL_CODE_41                      41
#define DP_THREAT_SYMBOL_CODE_48                      48

#define DP_THREAT_SYMBOL_CODE_64                      64
#define DP_THREAT_SYMBOL_CODE_65                      65
#define DP_THREAT_SYMBOL_CODE_66                      66
#define DP_THREAT_SYMBOL_CODE_67                      67
#define DP_THREAT_SYMBOL_CODE_68                      68
#define DP_THREAT_SYMBOL_CODE_69                      69
#define DP_THREAT_SYMBOL_CODE_70                      70
#define DP_THREAT_SYMBOL_CODE_71                      71
#define DP_THREAT_SYMBOL_CODE_72                      72
#define DP_THREAT_SYMBOL_CODE_73                      73
#define DP_THREAT_SYMBOL_CODE_74                      74
#define DP_THREAT_SYMBOL_CODE_75                      75
#define DP_THREAT_SYMBOL_CODE_76                      76
#define DP_THREAT_SYMBOL_CODE_77                      77
#define DP_THREAT_SYMBOL_CODE_78                      78
#define DP_THREAT_SYMBOL_CODE_79                      79
#define DP_THREAT_SYMBOL_CODE_80                      80
#define DP_THREAT_SYMBOL_CODE_81                      81
#define DP_THREAT_SYMBOL_CODE_82                      82
#define DP_THREAT_SYMBOL_CODE_83                      83
#define DP_THREAT_SYMBOL_CODE_84                      84
#define DP_THREAT_SYMBOL_CODE_85                      85
#define DP_THREAT_SYMBOL_CODE_86                      86
#define DP_THREAT_SYMBOL_CODE_87                      87
#define DP_THREAT_SYMBOL_CODE_88                      88
#define DP_THREAT_SYMBOL_CODE_89                      89
#define DP_THREAT_SYMBOL_CODE_90                      90
#define DP_THREAT_SYMBOL_CODE_91                      91
#define DP_THREAT_SYMBOL_CODE_92                      92
#define DP_THREAT_SYMBOL_CODE_93                      93
#define DP_THREAT_SYMBOL_CODE_94                      94
#define DP_THREAT_SYMBOL_CODE_95                      95
#define DP_THREAT_SYMBOL_CODE_96                      96
#define DP_THREAT_SYMBOL_CODE_97                      97
#define DP_THREAT_SYMBOL_CODE_98                      98
#define DP_THREAT_SYMBOL_CODE_99                      99

#define DP_THREAT_SYMBOL_CODE_15                      15
#define DP_THREAT_SYMBOL_CODE_16                      16
#define DP_THREAT_SYMBOL_CODE_17                      17
#define DP_THREAT_SYMBOL_CODE_18                      18
#define DP_THREAT_SYMBOL_CODE_19                      19
#define DP_THREAT_SYMBOL_CODE_20                      20
#define DP_THREAT_SYMBOL_CODE_21                      21
#define DP_THREAT_SYMBOL_CODE_22                      22
#define DP_THREAT_SYMBOL_CODE_23                      23
#define DP_THREAT_SYMBOL_CODE_24                      24
#define DP_THREAT_SYMBOL_CODE_25                      25
#define DP_THREAT_SYMBOL_CODE_26                      26
#define DP_THREAT_SYMBOL_CODE_27                      27
#define DP_THREAT_SYMBOL_CODE_28                      28
#define DP_THREAT_SYMBOL_CODE_29                      29
#define DP_THREAT_SYMBOL_CODE_30                      30
#define DP_THREAT_SYMBOL_CODE_31                      31
#define DP_THREAT_SYMBOL_CODE_32                      32
#define DP_THREAT_SYMBOL_CODE_33                      33
#define DP_THREAT_SYMBOL_CODE_34                      34

#define DP_THREAT_SYMBOL_CODE_14                      14
#define DP_THREAT_SYMBOL_CODE_13                      13
#define DP_THREAT_SYMBOL_CODE_12                      12
#define DP_THREAT_SYMBOL_CODE_11                      11
#define DP_THREAT_SYMBOL_CODE_10                      10
#define DP_THREAT_SYMBOL_CODE_9                       9
#define DP_THREAT_SYMBOL_CODE_8                       8
#define DP_THREAT_SYMBOL_CODE_7                       7
#define DP_THREAT_SYMBOL_CODE_6                       6
#define DP_THREAT_SYMBOL_CODE_5                       5
#define DP_THREAT_SYMBOL_CODE_4                       4
#define DP_THREAT_SYMBOL_CODE_3                       3
#define DP_THREAT_SYMBOL_CODE_2                       2
#define DP_THREAT_SYMBOL_CODE_1                       1
#define DP_THREAT_SYMBOL_UNKNOWN                      0

/*Special Symbol code 16 is needed correction for the LCA display ICD.
Use this macro to draw the symbol as AEWC or LCA Display*/
//#define DP_RWR_LCA_UPDATE_SYMBOL_16_AS_PER_LCA_DISPLAY_SYMBOL
                                                        /*              _  */
#define DP_RWR_SPECIAL_SYMBOLS_CODE_1			42		/* _| |_*/

#define DP_RWR_SPECIAL_SYMBOLS_CODE_2			43		/* _|_ */

#define DP_RWR_SPECIAL_SYMBOLS_CODE_3			44		/* _|_|_ */

#define DP_RWR_SPECIAL_SYMBOLS_CODE_4			45		/* _|_|_|_ */

#define DP_RWR_SPECIAL_SYMBOLS_CODE_5			46		/* |_|_|_| */

                                                        /*    _	   */
#define DP_RWR_SPECIAL_SYMBOLS_CODE_6			47		/* |_| |_| */

                                                        /* _ */
#define DP_RWR_SPECIAL_SYMBOLS_CODE_7			49		/* S */

                                                        /* _/ */
#define DP_RWR_SPECIAL_SYMBOLS_CODE_8			50		/* A  */

                                                        /*  _/_  */
#define DP_RWR_SPECIAL_SYMBOLS_CODE_9			51		/* |___| */

                                                        /*  _/_/_  */
#define DP_RWR_SPECIAL_SYMBOLS_CODE_10			52		/* |_____| */

#define DP_RWR_SPECIAL_SYMBOLS_CODE_11          53      /*    / \        */
                                                        /*    | |       */
                                                        /*   /| |\     */
                                                        /*  |_|_|_|   */

                                                        /*  _|_  */
#define DP_RWR_SPECIAL_SYMBOLS_CODE_12			54		/* |_|_| */

#define DP_RWR_SPECIAL_SYMBOLS_CODE_13			55    /*     |          */
                                                      /*    /_\        */
                                                      /*     |        */
                                                      /*     -       */

#define DP_RWR_SPECIAL_SYMBOLS_CODE_14			56     /*	 |     */
                                                       /*   /|\   */

#define DP_RWR_SPECIAL_SYMBOLS_CODE_15          57     /*   )--           */
                                                       /*     |          */
                                                       /*     -         */

#define DP_RWR_SPECIAL_SYMBOLS_CODE_16			58       /*   R/   */

#define DP_RWR_SPECIAL_SYMBOLS_CODE_17          59    /*   (|_/     */
                                                      /*    |      */
                                                      /*   ---    */

#define DP_RWR_SPECIAL_SYMBOLS_CODE_18			60    /*    __      */
                                                      /*    \/     */
                                                      /*    /\    */

#define DP_RWR_SPECIAL_SYMBOLS_CODE_19          61   /*      /\         */
                                                     /*     /  \       */
                                                     /*  ---\  /      */
                                                     /*    /|\/      */

#define DP_RWR_SPECIAL_SYMBOLS_CODE_20          62     /*    _______        */
                                                       /*    \|   |/       */
                                                       /*     \ _ /       */
                                                       /*      \ /       */

#define DP_RWR_SPECIAL_SYMBOLS_CODE_21			63     /*      |\          */
                                                       /*   ___|_\___     */
                                                       /*   \_______/    */

#define DP_RWR_DISPLAY_BGCOLOR_ALPHA                  255

#define DP_RWR_ROUNDED_RECT_X_RADIUS                  18
#define DP_RWR_ROUNDED_RECT_Y_RADIUS                  18
#define DP_RWR_ROUNDED_RECT_ADJUST_X_VAL              0.4f
#define DP_RWR_ROUNDED_RECT_ADJUST_Y_VAL              0.6f

#define DP_RWR_SYMBOL_COLOR_TRANSPARENCY              0.8
#define DP_RWR_SYMBOL_ROUNDED_RECT_RADIUSX            5
#define DP_RWR_SYMBOL_ROUNDED_RECT_RADIUSY            5

#define DP_PEN_WIDTH_1                                2
#define DP_PEN_WIDTH_2                                2
#define DP_PEN_WIDTH_3                                3
#define DP_PEN_WIDTH_4                                4
#define DP_PEN_WIDTH_5                                5
#define DP_PEN_WIDTH_6                                6


#define DP_PI_VALUE                                  (3.1415926535897932384626433832795f)
#define DP_RWR_DEG_2_RAD(DATA)                       (((float)DATA) * ((DP_PI_VALUE) / 180.0f))
#define DP_RWR_RAD_2_DEG(DATA)				         (((float)DATA) * (180.0f / (DP_PI_VALUE)))


#define DP_QT_EMITTER_REPORT_FIXED_BOUNDARY_WIDTH           311
#define DP_QT_EMITTER_REPORT_FIXED_BOUNDARY_HEIGHT          311
#define DP_QT_EMITTER_REPORT_DISPLAY_BGCOLOR_BLACK          0, 0, 0
#define DP_QT_EMITTER_REPORT_DISPLAY_BGCOLOR_ALPHA          255

#define DP_QT_EMITTER_REPORT_DESIGN_CIRC_ZERO_DEG           0
#define DP_QT_EMITTER_REPORT_DESIGN_CIRC_NINETY_DEG         90
#define DP_QT_EMITTER_REPORT_DESIGN_CIRC_ONEEIGHTY_DEG      180
#define DP_QT_EMITTER_REPORT_DESIGN_CIRC_TWOSEVENTY_DEG     270
#define DP_QT_EMITTER_REPORT_DESIGN_CIRC_THREESIXTY_DEG     360
#define DP_QT_EMITTER_REPORT_DESIGN_CIRC_ZERO_POINT_FIVE_DEG 0.5f
#define DP_QT_EMITTER_REPORT_MIDDLE_LINE_CIRC_TWO_DEG       2
#define DP_QT_EMITTER_REPORT_DESIGN_CIRC_ONE_POINT_FIVE_DEG 1.5f

#define DP_RWR_OUTER_CIRCLE_SCALING_FACTOR                  0.85f
#define DP_RWR_CENTER_CIRCLE_SCALING_FACTOR                 0.55f //0.505f
#define DP_RWR_INNER_CIRCLE_SCALING_FACTOR                  0.26f //0.16f
#define DP_RWR_INNER_DOT_CIRCLE_SCALING_FACTOR              0.015f

#define DP_QT_EMITTER_REPORT_CIRCLE_PEN_WIDTH_ONE_POINT_EIGHTF  1.8f
#define DP_QT_EMITTER_REPORT_CIRCLE_PEN_WIDTH_TWO               2
#define DP_QT_EMITTER_REPORT_CIRCLE_PEN_WIDTH_TWENTY_SIX        26
#define DP_QT_EMITTER_REPORT_CIRCLE_PEN_WIDTH_SIXTEEN           16
#define DP_QT_EMITTER_REPORT_CIRCLE_OPACITY_VALUE               0.2f
#define DP_QT_EMITTER_REPORT_CIRCLE_START_ANGLE_DEGVALUE        16.0f
#define DP_QT_EMITTER_REPORT_CIRCLE_SPAN_ANGLE_DEGVALUE         16.0f

#define DP_QT_EMITTER_REPORT_ROUNDED_RECT_X_RADIUS              6
#define DP_QT_EMITTER_REPORT_ROUNDED_RECT_Y_RADIUS              6

#define DP_QT_EMITTER_REPORT_FLIGHT_MODEL_WIDTH                 10 /* To change the Flight Size - > change this size alone */
#define DP_QT_EMITTER_REPORT_FLIGHT_MODEL_HEIGHT                (DP_QT_EMITTER_REPORT_FLIGHT_MODEL_WIDTH * 2)

/* fRadiusRange should be 0 - 1 */
#define DP_RWR_CALCULATE_CIRCLE_DIAMETER(fBoundaryWidth, fRadiusRange) (fBoundaryWidth * fRadiusRange)

#define DP_QT_EMITTER_REPORT_FLIGHT_PEN_WIDTH_ONE_POINT_THREEF  1.3f

#define DP_RWR_THREAT_CLASSIFY_FRIENDLY_THREAT                  00
#define DP_RWR_THREAT_CLASSIFY_MOST_DANGEROUS_THREAT            01

#define DP_QT_EMITTER_REPORT_CIRCLE_COLOR_GREEN                 0, 215, 0 //0, 169, 0
#define DP_QT_EMITTER_REPORT_CIRCLE_COLOR_GREY                  128,128,128

#define DP_RWR_MAX_NORMALIZED_THREAT_RANGE                      (1.0f + FLT_EPSILON)
#define DP_RWR_MIN_THREAT_ANGLE                                 (0.0f + FLT_EPSILON)
#define DP_RWR_MAX_THREAT_ANGLE                                 (360.0f + FLT_EPSILON)

#define DP_RWR_COLOR_RWR_THREAT                                 12, 186, 11

#define DP_RWR_PEN_WIDTH_1                                          1
#define DP_RWR_PEN_WIDTH_2                                          2
#define DP_RWR_PEN_WIDTH_3                                          3

#define DP_RWR_MIG29K_DU_COLOR_CODE_BLACK                           0
#define DP_RWR_MIG29K_DU_COLOR_CODE_GREEN                           1
#define DP_RWR_MIG29K_DU_COLOR_CODE_RED                             2
#define DP_RWR_MIG29K_DU_COLOR_CODE_YELLOW                          3
#define DP_RWR_MIG29K_DU_COLOR_CODE_BLUE                            4
#define DP_RWR_MIG29K_DU_COLOR_CODE_CYAN                            5
#define DP_RWR_MIG29K_DU_COLOR_CODE_MAGENTA                         6
#define DP_RWR_MIG29K_DU_COLOR_CODE_WHITE                           7
#define DP_RWR_MIG29K_DU_COLOR_CODE_LIGHT_BLACK                     8
#define DP_RWR_MIG29K_DU_COLOR_CODE_LIGHT_GREEN                     9
#define DP_RWR_MIG29K_DU_COLOR_CODE_LIGHT_RED                       10
#define DP_RWR_MIG29K_DU_COLOR_CODE_LIGHT_YELLOW                    11
#define DP_RWR_MIG29K_DU_COLOR_CODE_LIGHT_BLUE                      12
#define DP_RWR_MIG29K_DU_COLOR_CODE_LIGHT_CYAN                      13
#define DP_RWR_MIG29K_DU_COLOR_CODE_LIGHT_MAGENTA                   14
#define DP_RWR_MIG29K_DU_COLOR_CODE_LIGHT_WHITE                     15

#define DP_RWR_DU_COLOR_BLACK                                       0,0,0
#define DP_RWR_DU_COLOR_GREEN                                       0,255,0 //0,102,0
#define DP_RWR_DU_COLOR_RED                                         255,0,0
#define DP_RWR_DU_COLOR_SUN                                         225,0,0
#define DP_RWR_DU_COLOR_YELLOW                                      255,255,0
#define DP_RWR_DU_COLOR_BLUE                                        0,0,255
#define DP_RWR_DU_COLOR_CYAN                                        0,255,255
#define DP_RWR_DU_COLOR_MAGENTA                                     255,0,255
#define DP_RWR_DU_COLOR_WHITE                                       255,255,255
#define DP_RWR_DU_COLOR_GREY                                        169,169,169
#define DP_RWR_DU_COLOR_LIGHT_BLACK                                 0,0,0
#define DP_RWR_DU_COLOR_LIGHT_GREEN                                 32,255,32
#define DP_RWR_DU_COLOR_LIGHT_RED                                   255,128,128
#define DP_RWR_DU_COLOR_LIGHT_YELLOW                                255,255,128
#define DP_RWR_DU_COLOR_LIGHT_BLUE                                  128,128,255
#define DP_RWR_DU_COLOR_LIGHT_CYAN                                  128,255,255
#define DP_RWR_DU_COLOR_LIGHT_MAGENTA                               255,128,255
#define DP_RWR_DU_COLOR_LIGHT_WHITE                                 255,255,230


#define DP_RWR_ROUNDED_RECT_RADIUS4                                 20
#define DP_RWR_ROUNDED_RECT_RADIUS5                                 (DP_RWR_ROUNDED_RECT_RADIUS4 + 4)
#define DP_RWR_ROUNDED_RECT_RADIUS2                                 23
#define DP_RWR_OUTER_THREAT_SYMBOL_SIZE1                            (DP_RWR_ROUNDED_RECT_RADIUS4 + 2)



/*!
*	\def DP_INVALID_BIN_COUNT
*	\brief This macro is used for failure value
*/
#define DP_INVALID_BIN_COUNT   -10

/*!
*	\def DP_INVALID_WAYPOINT_COUNT
*	\brief This macro is used for waypoint count exceeding failure
*/
#define DP_INVALID_WAYPOINT_COUNT   -11

/*!
*	\def DP_INVALID_EMITTER_COUNT
*	\brief This macro is used for emitter count exceeding failure
*/
#define DP_INVALID_EMITTER_COUNT   -11

/*!
*	\def DP_MAX_WAYPOINT_LAT_LONG
*	\brief This macro is used to define maximum Waypoint Latitude and Longitude points
*/
#define DP_MAX_WAYPOINT_LAT_LONG     2048

/*!
*	\def DP_MAX_EMITTER_LAT_LONG
*	\brief This macro is used to define maximum Emitter Latitude and Longitude points
*/
#define DP_MAX_EMITTER_LAT_LONG      20

/*!
*	\def DP_MAX_EMITTER_LAT_LONG
*	\brief This macro is used to define maximum Emitter Latitude and Longitude points
*/
#define DP_MIN_BINCOUNT              1

/**
*	\def DP_VALIDATE_BINNING()
*	\brief  This macro is used to validate the Binning of Waypoints
*	\retval	:: DP_INVALID_BIN_COUNT invalided input.
*/
#define DP_VALIDATE_BINNING(BinCnt,WayPointCnt)\
    {\
        if(BinCnt < DP_MIN_BINCOUNT || BinCnt > (WayPointCnt *(10.0/100.0)))\
        {\
            return DP_INVALID_BIN_COUNT;\
        }\
    }

/**
*	\def DP_VALIDATE_VALIDATE_WAYPOINT_CNT()
*	\brief  This macro is used to validate the waypoint count
*	\retval	:: DP_INVALID_BIN_COUNT invalided input.
*/
#define DP_VALIDATE_WAYPOINT_CNT(WayPointCnt)\
    {\
        if(WayPointCnt > DP_MAX_WAYPOINT_LAT_LONG)\
        {\
            return DP_MAX_EMITTER_LAT_LONG;\
        }\
    }

/**
*	\def DP_MAX_EMITTER_LAT_LONG()
*	\brief  This macro is used to validate the waypoint count
*	\retval	:: DP_INVALID_BIN_COUNT invalided input.
*/
#define DP_VALIDATE_EMITTER_CNT(Variable)\
    {\
        if(Variable > DP_MAX_EMITTER_LAT_LONG)\
        {\
            return DP_INVALID_EMITTER_COUNT;\
        }\
    }



/*
 * Windows GDI font-weight constants (from <wingdi.h>). Defined here for
 * non-Windows builds so cross-platform compilation succeeds. The current
 * font rendering ignores the weight argument, but the symbols are referenced.
 */
#if !defined(_WIN32) && !defined(FW_NORMAL)
    #define FW_DONTCARE     0
    #define FW_THIN         100
    #define FW_EXTRALIGHT   200
    #define FW_LIGHT        300
    #define FW_NORMAL       400
    #define FW_MEDIUM       500
    #define FW_SEMIBOLD     600
    #define FW_BOLD         700
    #define FW_EXTRABOLD    800
    #define FW_HEAVY        900
#endif

/*
 * MSVC names the C99 snprintf as _snprintf. Map it to the standard snprintf
 * on non-Windows builds so cross-platform compilation succeeds.
 */
#if !defined(_WIN32)
    #include <cstdio>
    #define _snprintf snprintf
#endif

/*
 * MSVC provides _strupr (in-place uppercase) in <string.h>. There is no
 * standard C equivalent, so supply one for non-Windows builds.
 */
#if !defined(_WIN32)
    #include <cctype>
    static inline char *_strupr(char *in_str)
    {
        if (in_str != nullptr)
        {
            for (char *p = in_str; *p != '\0'; ++p)
            {
                *p = static_cast<char>(toupper(static_cast<unsigned char>(*p)));
            }
        }
        return in_str;
    }
#endif

#endif // DP_ASPJ_MACROS_H
