
#include "dp_aspj_comm.h"
#include "ui_dp_rwr_flight_display.h"

dp_rwr_flight_display::dp_rwr_flight_display(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::dp_rwr_flight_display)
{
    qSetMessagePattern("[%{time yyyy-MM-dd h:mm:ss.zzz}] [%{type}] [%{file}:%{line}] %{message}");

    ui->setupUi(this);

    /* Hide the polar plot simulating groupbox */
    ui->obj_grp_bx_FlightDispConfig->setVisible(false);

    /// - Update symbol codes from csv file to global structure
    UpdateEmitterSymbolDetails(DP_RWR_LOAD_CONFIG, &g_SDP_ASPJ_Handle.m_sEmitterSymbolLibrary);
    /* Connecting for paintevent function to be called every 150 milliseconds */
    m_obj_tmr_PaintEvent = new QTimer(this);
    connect(m_obj_tmr_PaintEvent, SIGNAL(timeout()), this, SLOT(DP_RWR_SLOT_FlightDisplay_CallPaintEventFunc()));
    m_obj_tmr_PaintEvent->start(1000);
}

dp_rwr_flight_display::~dp_rwr_flight_display()
{
    delete ui;
}

void dp_rwr_flight_display::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    m_obj_paint_line = new QPainter(this);

    /********** Standard QPainter function to avoid Staircase effect on Lines **********/
    m_obj_paint_line->setRenderHints(QPainter::Antialiasing, true);

    /* Finding the display area to draw the polar plot */
    DP_RWR_Bitmap_FindDisplayArea();

    /* Drawing the static screen of the polar plot */
    DP_RWR_Bitmap_DrawStaticScreen();

    /* Drawing the dynamic screen of the polar plot */
    DP_RWR_Bitmap_DrawDynamicScreen();

    delete m_obj_paint_line;
}

void dp_rwr_flight_display::DP_POLAR_PLOT_SLOT_LoadThreatList(S_RWR_THREAT_LIST in_SThreatList)
{
    memcpy(&m_S_RWRThreatList, &in_SThreatList, sizeof(S_RWR_THREAT_LIST));
}

void dp_rwr_flight_display::DP_RWR_SLOT_FlightDisplay_CallPaintEventFunc()
{
    update();
}

void dp_rwr_flight_display::DP_RWR_Bitmap_FindDisplayArea()
{
    QPoint pos;
    QWidget *paintArea = ui->obj_grp_bx_FlightDispArea;

    mS_Emitter_Report_Display.m_iDispWidth = ui->obj_grp_bx_FlightDispArea->geometry().width();
    mS_Emitter_Report_Display.m_iDispHeight = ui->obj_grp_bx_FlightDispArea->geometry().height();

    do
    {
        pos += paintArea->pos();
        paintArea = paintArea->parentWidget();

        if(paintArea == this)
        {
            break;
        }
    }while(paintArea != NULL);

    mS_Emitter_Report_Display.m_iDispXPos = pos.x();
    mS_Emitter_Report_Display.m_iDispYPos = pos.y();

    /* Global offset adjustment */
    mS_Emitter_Report_Display.m_iDispXPos          = mS_Emitter_Report_Display.m_iDispXPos + 4;
    mS_Emitter_Report_Display.m_iDispYPos          = mS_Emitter_Report_Display.m_iDispYPos + 8;
    mS_Emitter_Report_Display.m_iDispWidth         = mS_Emitter_Report_Display.m_iDispWidth - 8;
    mS_Emitter_Report_Display.m_iDispHeight        = mS_Emitter_Report_Display.m_iDispHeight - 16;

    mS_Emitter_Report_Display.m_iDispClippedXPos   = mS_Emitter_Report_Display.m_iDispXPos;
    mS_Emitter_Report_Display.m_iDispClippedYPos   = mS_Emitter_Report_Display.m_iDispYPos;
    mS_Emitter_Report_Display.m_iDispClippedWidth  = mS_Emitter_Report_Display.m_iDispWidth;
    mS_Emitter_Report_Display.m_iDispClippedHeight = mS_Emitter_Report_Display.m_iDispHeight;

    /* This is done to make sure the Display is always square type */
    if(mS_Emitter_Report_Display.m_iDispClippedWidth > mS_Emitter_Report_Display.m_iDispClippedHeight)
    {
        mS_Emitter_Report_Display.m_iDispClippedXPos = mS_Emitter_Report_Display.m_iDispClippedXPos + ((mS_Emitter_Report_Display.m_iDispWidth / 2) - (mS_Emitter_Report_Display.m_iDispHeight / 2));
        /* This logic is to adjust the square type display in the centre of the group box vertically */
        mS_Emitter_Report_Display.m_iDispClippedWidth = mS_Emitter_Report_Display.m_iDispClippedHeight;
    }

    if(mS_Emitter_Report_Display.m_iDispClippedHeight > mS_Emitter_Report_Display.m_iDispClippedWidth)
    {
        mS_Emitter_Report_Display.m_iDispClippedYPos = mS_Emitter_Report_Display.m_iDispClippedYPos - ((mS_Emitter_Report_Display.m_iDispWidth / 2) - (mS_Emitter_Report_Display.m_iDispHeight / 2));
        /* This logic is to adjust the square type display in the centre of the group box horizontally */
        mS_Emitter_Report_Display.m_iDispClippedHeight = mS_Emitter_Report_Display.m_iDispClippedWidth;
    }

    /* Storing Positions For Updating(redraw) Screen(GroupBox) */
    mS_Emitter_Report_Display.m_iBitmapStartXPos = mS_Emitter_Report_Display.m_iDispXPos;
    mS_Emitter_Report_Display.m_iBitmapStartYPos = mS_Emitter_Report_Display.m_iDispYPos;
    mS_Emitter_Report_Display.m_iBitmapWidth = mS_Emitter_Report_Display.m_iDispWidth;
    mS_Emitter_Report_Display.m_iBitmapHeight = mS_Emitter_Report_Display.m_iDispHeight;
    return;
}

void dp_rwr_flight_display::DP_RWR_Bitmap_DrawStaticScreen()
{
    /* Storing the values of the Rect Boundary */
    float fDispStartPos_X            = (float)mS_Emitter_Report_Display.m_iDispXPos;
    float fDispStartPos_Y            = (float)mS_Emitter_Report_Display.m_iDispYPos;
    float fDispBoundaryWidth         = (float)mS_Emitter_Report_Display.m_iDispWidth;
    float fDispBoundaryHeight        = (float)mS_Emitter_Report_Display.m_iDispHeight;

    float fDispClippedStartPos_X     = (float)mS_Emitter_Report_Display.m_iDispClippedXPos;
    float fDispClippedStartPos_Y     = (float)mS_Emitter_Report_Display.m_iDispClippedYPos;
    float fDispClippedBoundaryWidth  = (float)mS_Emitter_Report_Display.m_iDispClippedWidth;
    float fDispClippedBoundaryHeight = (float)mS_Emitter_Report_Display.m_iDispClippedHeight;

    int iDiameter = 0;
    int iDiameter1 = 0, iDiameter2 = 0;

    QPoint iCenter(fDispClippedStartPos_X + (fDispClippedBoundaryWidth / 2), fDispClippedStartPos_Y + (fDispClippedBoundaryHeight / 2));

    /* Variables to draw the lines and circles */
    int iLineLoop = DP_QT_EMITTER_REPORT_INITIALIZE;

    m_obj_paint_line->setRenderHint(QPainter::Antialiasing);

    /* Filling BG Color for Rect Boundary */
    cMyColor.setRgb(DP_QT_EMITTER_REPORT_DISPLAY_BGCOLOR_BLACK, DP_QT_EMITTER_REPORT_DISPLAY_BGCOLOR_ALPHA);
    m_obj_paint_line->fillRect(fDispStartPos_X, fDispStartPos_Y, fDispBoundaryWidth, fDispBoundaryHeight, cMyColor);

    /* Drawing Outer Rect Boundary */
    //DP_RWR_Bitmap_DrawRect(fDispStartPos_X + 12, fDispStartPos_Y + 12, fDispBoundaryWidth - 24, fDispBoundaryHeight - 24, DP_QT_EMITTER_REPORT_CIRCLE_COLOR_GREY, DP_QT_EMITTER_REPORT_CIRCLE_PEN_WIDTH_TWO);

    /* Drawing Outer Circle 1 */
    iDiameter = DP_RWR_CALCULATE_CIRCLE_DIAMETER(fDispClippedBoundaryWidth, DP_RWR_OUTER_CIRCLE_SCALING_FACTOR);
    DP_RWR_Bitmap_DrawAdjustingCircle(iCenter, iDiameter/2, DP_RWR_COLOR_RED, DP_QT_EMITTER_REPORT_CIRCLE_PEN_WIDTH_ONE_POINT_EIGHTF);

    /* Drawing Outer Circle 2 */
    iDiameter = DP_RWR_CALCULATE_CIRCLE_DIAMETER(fDispClippedBoundaryWidth, DP_RWR_CENTER_CIRCLE_SCALING_FACTOR);
    DP_RWR_Bitmap_DrawAdjustingCircle(iCenter, iDiameter/2, DP_RWR_COLOR_WHITE, DP_QT_EMITTER_REPORT_CIRCLE_PEN_WIDTH_ONE_POINT_EIGHTF);

    /* Drawing Outer Circle 3 */
    iDiameter = DP_RWR_CALCULATE_CIRCLE_DIAMETER(fDispClippedBoundaryWidth, DP_RWR_INNER_CIRCLE_SCALING_FACTOR);
    DP_RWR_Bitmap_DrawAdjustingCircle(iCenter, iDiameter/2, DP_RWR_COLOR_WHITE, DP_QT_EMITTER_REPORT_CIRCLE_PEN_WIDTH_ONE_POINT_EIGHTF);

    /* Drawing Outer Circle 4 */
    iDiameter = DP_RWR_CALCULATE_CIRCLE_DIAMETER(fDispClippedBoundaryWidth, DP_RWR_INNER_DOT_CIRCLE_SCALING_FACTOR);
    DP_RWR_Bitmap_FillCircle(iCenter, iDiameter/2, DP_RWR_COLOR_WHITE, DP_QT_EMITTER_REPORT_CIRCLE_PEN_WIDTH_ONE_POINT_EIGHTF);

    /* Drawing Target Lines */
    iDiameter1 = DP_RWR_CALCULATE_CIRCLE_DIAMETER(fDispClippedBoundaryWidth, DP_RWR_INNER_CIRCLE_SCALING_FACTOR);
    iDiameter2 = DP_RWR_CALCULATE_CIRCLE_DIAMETER(fDispClippedBoundaryHeight, DP_RWR_OUTER_CIRCLE_SCALING_FACTOR);

    DP_RWR_Bitmap_DrawLine(iCenter.x() + (iDiameter1/2), iCenter.y(), iCenter.x() + (iDiameter2/2), iCenter.y(), QColor(DP_QT_EMITTER_REPORT_CIRCLE_COLOR_GREEN), DP_QT_EMITTER_REPORT_CIRCLE_PEN_WIDTH_ONE_POINT_EIGHTF, DP_RWR_ACCEPT_ADD_WEIGHTAGE);
    DP_RWR_Bitmap_DrawLine(iCenter.x() - (iDiameter1/2), iCenter.y(), iCenter.x() - (iDiameter2/2), iCenter.y(), QColor(DP_QT_EMITTER_REPORT_CIRCLE_COLOR_GREEN), DP_QT_EMITTER_REPORT_CIRCLE_PEN_WIDTH_ONE_POINT_EIGHTF, DP_RWR_ACCEPT_ADD_WEIGHTAGE);
    DP_RWR_Bitmap_DrawLine(iCenter.x(), iCenter.y() + (iDiameter1/2), iCenter.x(), iCenter.y() + (iDiameter2/2), QColor(DP_QT_EMITTER_REPORT_CIRCLE_COLOR_GREEN), DP_QT_EMITTER_REPORT_CIRCLE_PEN_WIDTH_ONE_POINT_EIGHTF, DP_RWR_ACCEPT_ADD_WEIGHTAGE);
    DP_RWR_Bitmap_DrawLine(iCenter.x(), iCenter.y() - (iDiameter1/2), iCenter.x(), iCenter.y() - (iDiameter2/2), QColor(DP_QT_EMITTER_REPORT_CIRCLE_COLOR_GREEN), DP_QT_EMITTER_REPORT_CIRCLE_PEN_WIDTH_ONE_POINT_EIGHTF, DP_RWR_ACCEPT_ADD_WEIGHTAGE);

    iDiameter = DP_RWR_CALCULATE_CIRCLE_DIAMETER(fDispClippedBoundaryWidth, DP_RWR_OUTER_CIRCLE_SCALING_FACTOR);
    for(iLineLoop = 0; iLineLoop <= 360; iLineLoop += 15)
    {
        m_obj_paint_line->save();
        m_obj_paint_line->translate(iCenter);
        m_obj_paint_line->rotate(iLineLoop);
        if((iLineLoop % 90) != 0)
        {
            if((iLineLoop % 45) == 0)
            {
                DP_RWR_Bitmap_DrawLine(0, (iDiameter/2), 0, (iDiameter/2.6f), QColor(DP_RWR_COLOR_RED), DP_QT_EMITTER_REPORT_CIRCLE_PEN_WIDTH_ONE_POINT_EIGHTF, DP_RWR_ACCEPT_ADD_WEIGHTAGE);
            }
            else
            {
                DP_RWR_Bitmap_DrawLine(0, (iDiameter/2), 0, (iDiameter/2.3f), QColor(DP_RWR_COLOR_RED), DP_QT_EMITTER_REPORT_CIRCLE_PEN_WIDTH_ONE_POINT_EIGHTF, DP_RWR_ACCEPT_ADD_WEIGHTAGE);
            }
        }
        m_obj_paint_line->restore();
    }

    DP_RWR_Bitmap_DrawFlightModel((mS_Emitter_Report_Display.m_iDispXPos + (mS_Emitter_Report_Display.m_iDispWidth / 2)), (mS_Emitter_Report_Display.m_iDispYPos + (mS_Emitter_Report_Display.m_iDispHeight / 2)), DP_RWR_COLOR_WHITE, DP_QT_EMITTER_REPORT_FLIGHT_PEN_WIDTH_ONE_POINT_THREEF);
}

/*****************************************************************************
* Name						:	DP_RWR_Bitmap_DrawDynamicScreen
* Author					:	Vinoth V
* Global Variables affected	:	NA
* Created Date				:	Jan 31, 2017
* Revision Date				:	NA
* Reason for Revising		:	NA
* Description				:	This function will draw the dynamic screen.
*******************************************************************************/

void dp_rwr_flight_display::DP_RWR_Bitmap_DrawDynamicScreen()
{

    QString qsTemp = "";

    /* Only show threat symbols while the aircraft is actually flying. Before the
       flight starts (parked on waypoint 0) and after it completes, the cockpit
       must be clear. */
    CDP_FlightController *pFC = g_SDP_ASPJ_Handle.pm_cobj_flightController;
    if((pFC == NULL) || (!pFC->isFlying()))
    {
        m_S_RWRThreatList.ulThreatCount = 0;
        return;
    }

    unsigned short usCurrentWaypoint = pFC->m_currentSegment;
    unsigned short usThreatCnt = g_SDP_ASPJ_Handle.m_sEmitDetWithinRange.m_SEmttrRange[usCurrentWaypoint].m_u16TotalEmtCnt;
    m_S_RWRThreatList.ulThreatCount = usThreatCnt;

//    qDebug()<<"threat count :"<<usThreatCnt;

    qsTemp.sprintf("Waypoint  : %d, Threat Count : %d", usCurrentWaypoint, usThreatCnt);
    //    Sig_PrintLog(qsTemp, DP_ASPJ_SUCCESS);

    if(m_S_RWRThreatList.ulThreatCount > DP_RWR_MAX_EMITTERS)
    {
        //PrintLog("RWR Display : Invalid Threat Count", 1);
        return;
    }

    for(unsigned long ulThreatCount = 0; ulThreatCount < m_S_RWRThreatList.ulThreatCount; ulThreatCount++)
    {
        S_EMITTER_REL_PARAM *pEmt = &g_SDP_ASPJ_Handle.m_sEmitDetWithinRange.m_SEmttrRange[usCurrentWaypoint].m_SEmtrRelParam[ulThreatCount];

        /* The display expects a NORMALIZED range (0..1); m_fdEmitterDist is in km,
           so divide by the emitter radar range. Without this the range guard in
           DP_RWR_Bitmap_Draw_Threat_Details rejects every threat (km > 1.0). */
        double dNormRange = (pEmt->m_u16RdrRange_km > 0) ? (pEmt->m_fdEmitterDist / (double)pEmt->m_u16RdrRange_km) : 0.0;
        if(dNormRange < 0.0) dNormRange = 0.0;
        if(dNormRange > 1.0) dNormRange = 1.0;

        /* Resolve this emitter's index in the loaded list (by lat/lon) so we can
           pick ITS special symbol from the library, instead of using the
           detection order. Falls back to the loop index if no match. */
        unsigned short usEmitterIdx = (unsigned short)ulThreatCount;
        unsigned short usEmtListCnt = g_SDP_ASPJ_Handle.m_sEmitterLatLon[0].m_u16EmtLatLongCnt;
        for(unsigned short usE = 0; (usE < usEmtListCnt) && (usE < DP_MAX_EMITTER_LAT_LONG); usE++)
        {
            if((fabs(g_SDP_ASPJ_Handle.m_sEmitterLatLon[usE].m_fdEmitLat - pEmt->m_fdEmitLat) < 1e-6) &&
                    (fabs(g_SDP_ASPJ_Handle.m_sEmitterLatLon[usE].m_fdEmitLon - pEmt->m_fdEmitLon) < 1e-6))
            {
                usEmitterIdx = usE;
                break;
            }
        }

        /* Show a simple TEXT label (A, B, C ...) from the CSV instead of a vector
           shape. In dp_rwr_threat_symbol_info.csv letters A..Z live at symbol
           codes 15..40. Codes outside the shape range (42..63) fall to the
           default case of DP_RWR_Bitmap_Draw_Threat_Symbol, which renders the
           CSV text (arrcEmitterSymbol). So map emitter 0->'A', 1->'B', ... */
        unsigned short usSymCode = 15 + (usEmitterIdx % 26);   // 15='A' ... 40='Z'

        m_S_RWRThreatList.arrS_RWRThreatSymbolInfo[ulThreatCount].fAngle = pEmt->m_fdAOAfromWaypoint;
        m_S_RWRThreatList.arrS_RWRThreatSymbolInfo[ulThreatCount].fRange = dNormRange;
        m_S_RWRThreatList.arrS_RWRThreatSymbolInfo[ulThreatCount].fDOA_Deg = 9.9f;
        m_S_RWRThreatList.arrS_RWRThreatSymbolInfo[ulThreatCount].ucBG_Colour = 0;
        m_S_RWRThreatList.arrS_RWRThreatSymbolInfo[ulThreatCount].ucFG_Colour = 1;
        m_S_RWRThreatList.arrS_RWRThreatSymbolInfo[ulThreatCount].dThreatX_Pos = 0.0f;
        m_S_RWRThreatList.arrS_RWRThreatSymbolInfo[ulThreatCount].dThreatY_Pos = 0.0f;
        m_S_RWRThreatList.arrS_RWRThreatSymbolInfo[ulThreatCount].usSymbolCode = usSymCode;
        /* Symbol shapes use this size for their geometry — must be non-zero or nothing draws */
        m_S_RWRThreatList.arrS_RWRThreatSymbolInfo[ulThreatCount].ucThreatSymbolSize = DP_THREAT_SYMBOL_SIZE;
        m_S_RWRThreatList.arrS_RWRThreatSymbolInfo[ulThreatCount].fAmplitude_dBm = -60.0f;
        m_S_RWRThreatList.arrS_RWRThreatSymbolInfo[ulThreatCount].ucDispPriority = 255;
        m_S_RWRThreatList.arrS_RWRThreatSymbolInfo[ulThreatCount].ucThreatScanStat = 1;

        /// Build text label: emitter at CSV position 0 -> "A", 1 -> "B", 2 -> "C", etc.
        char acLabel[2] = {0};
        acLabel[0] = 'A' + usEmitterIdx;
        strncpy(m_S_RWRThreatList.arrS_RWRThreatSymbolInfo[ulThreatCount].arrcThreatSymbolName, acLabel, (DP_RWR_EMITTER_SYMBOL_NAME_LEN - 1));

        DP_RWR_Bitmap_Draw_Threat_Details(m_S_RWRThreatList.arrS_RWRThreatSymbolInfo[ulThreatCount]);
    }
}

/*****************************************************************************
* Name						:	DP_RWR_Bitmap_Draw_Threat_Details
* Author					:	Vinoth V
* Global Variables affected	:	NA
* Created Date				:	Jan 31, 2017
* Revision Date				:	NA
* Reason for Revising		:	NA
* Description				:	This function will draw RWR threats symbol.
*******************************************************************************/

void dp_rwr_flight_display::DP_RWR_Bitmap_Draw_Threat_Details(S_RWR_THREAT_SYMBOL_INFO in_S_RWRThreatInfo)
{
    QColor qcRgb;

    double dThreatX_Pos = 0;
    double dThreatY_Pos = 0;

    double dAngle_Rad = 0;

    float fDispClippedStartPos_X = (float)mS_Emitter_Report_Display.m_iDispClippedXPos;
    float fDispClippedStartPos_Y = (float)mS_Emitter_Report_Display.m_iDispClippedYPos;
    float fDispClippedBoundaryWidth = (float)mS_Emitter_Report_Display.m_iDispClippedWidth;
    float fDispClippedBoundaryHeight = (float)mS_Emitter_Report_Display.m_iDispClippedHeight;

    double dAngleLines_RefX = (fDispClippedBoundaryWidth / 2) + fDispClippedStartPos_X;
    double dAngleLines_RefY = (fDispClippedBoundaryHeight / 2) + fDispClippedStartPos_Y;

    if((in_S_RWRThreatInfo.fRange < 0) || (in_S_RWRThreatInfo.fRange > DP_RWR_MAX_NORMALIZED_THREAT_RANGE))
    {
        //PrintLog("RWR Threat : Invalid range value", 1);
        return;
    }

    if((in_S_RWRThreatInfo.fAngle > DP_RWR_MAX_THREAT_ANGLE) || (in_S_RWRThreatInfo.fAngle < 0.0f))
    {
        PrintLog("RWR Threat : Invalid angle value", 1);
        return;
    }

    cPen.setColor(QColor(255, DP_RWR_COLOR_RWR_THREAT));
    cPen.setWidthF(DP_RWR_PEN_WIDTH_2);

    /* Range conversion */
    in_S_RWRThreatInfo.fRange = (DP_RWR_CALCULATE_CIRCLE_DIAMETER(fDispClippedBoundaryWidth, DP_RWR_OUTER_CIRCLE_SCALING_FACTOR) / 2.0f) * in_S_RWRThreatInfo.fRange;

    /* Angle conversion */
    dAngle_Rad = DP_RWR_DEG_2_RAD(in_S_RWRThreatInfo.fAngle - 90.0f);

    /*-- Convert Polar coordinates into rectangular coordinates for background and all symbols --*/
    dThreatX_Pos = (in_S_RWRThreatInfo.fRange * cos(dAngle_Rad) + dAngleLines_RefX);
    dThreatY_Pos = (in_S_RWRThreatInfo.fRange * sin(dAngle_Rad) + dAngleLines_RefY);


    if(in_S_RWRThreatInfo.ucFG_Colour == DP_RWR_MIG29K_DU_COLOR_CODE_BLACK)
    {
        DP_RWR_Bitmap_FontSelect(m_obj_paint_line, FW_BOLD, DP_RWR_DU_COLOR_BLACK);
    }
    else if(in_S_RWRThreatInfo.ucFG_Colour == DP_RWR_MIG29K_DU_COLOR_CODE_GREEN)
    {
        DP_RWR_Bitmap_FontSelect(m_obj_paint_line, FW_BOLD, DP_QT_EMITTER_REPORT_CIRCLE_COLOR_GREEN);
    }
    else if(in_S_RWRThreatInfo.ucFG_Colour == DP_RWR_MIG29K_DU_COLOR_CODE_RED)
    {
        DP_RWR_Bitmap_FontSelect(m_obj_paint_line, FW_BOLD, DP_RWR_DU_COLOR_RED);
    }
    else if(in_S_RWRThreatInfo.ucFG_Colour == DP_RWR_MIG29K_DU_COLOR_CODE_YELLOW)
    {
        DP_RWR_Bitmap_FontSelect(m_obj_paint_line, FW_BOLD, DP_RWR_DU_COLOR_YELLOW);
    }
    else if(in_S_RWRThreatInfo.ucFG_Colour == DP_RWR_MIG29K_DU_COLOR_CODE_BLUE)
    {
        DP_RWR_Bitmap_FontSelect(m_obj_paint_line, FW_BOLD, DP_RWR_DU_COLOR_BLUE);
    }
    else if(in_S_RWRThreatInfo.ucFG_Colour == DP_RWR_MIG29K_DU_COLOR_CODE_CYAN)
    {
        DP_RWR_Bitmap_FontSelect(m_obj_paint_line, FW_BOLD, DP_RWR_DU_COLOR_CYAN);
    }
    else if(in_S_RWRThreatInfo.ucFG_Colour == DP_RWR_MIG29K_DU_COLOR_CODE_MAGENTA)
    {
        DP_RWR_Bitmap_FontSelect(m_obj_paint_line, FW_BOLD, DP_RWR_DU_COLOR_MAGENTA);
    }
    else if(in_S_RWRThreatInfo.ucFG_Colour == DP_RWR_MIG29K_DU_COLOR_CODE_WHITE)
    {
        DP_RWR_Bitmap_FontSelect(m_obj_paint_line, FW_BOLD, DP_RWR_DU_COLOR_WHITE);
    }
    else if(in_S_RWRThreatInfo.ucFG_Colour == DP_RWR_MIG29K_DU_COLOR_CODE_LIGHT_BLACK)
    {
        DP_RWR_Bitmap_FontSelect(m_obj_paint_line, FW_BOLD, DP_RWR_DU_COLOR_LIGHT_BLACK);
    }
    else if(in_S_RWRThreatInfo.ucFG_Colour == DP_RWR_MIG29K_DU_COLOR_CODE_LIGHT_GREEN)
    {
        DP_RWR_Bitmap_FontSelect(m_obj_paint_line, FW_BOLD, DP_RWR_DU_COLOR_LIGHT_GREEN);
    }
    else if(in_S_RWRThreatInfo.ucFG_Colour == DP_RWR_MIG29K_DU_COLOR_CODE_LIGHT_RED)
    {
        DP_RWR_Bitmap_FontSelect(m_obj_paint_line, FW_BOLD, DP_RWR_DU_COLOR_LIGHT_RED);
    }
    else if(in_S_RWRThreatInfo.ucFG_Colour == DP_RWR_MIG29K_DU_COLOR_CODE_LIGHT_YELLOW)
    {
        DP_RWR_Bitmap_FontSelect(m_obj_paint_line, FW_BOLD, DP_RWR_DU_COLOR_LIGHT_YELLOW);
    }
    else if(in_S_RWRThreatInfo.ucFG_Colour == DP_RWR_MIG29K_DU_COLOR_CODE_LIGHT_BLUE)
    {
        DP_RWR_Bitmap_FontSelect(m_obj_paint_line, FW_BOLD, DP_RWR_DU_COLOR_LIGHT_BLUE);
    }
    else if(in_S_RWRThreatInfo.ucFG_Colour == DP_RWR_MIG29K_DU_COLOR_CODE_LIGHT_CYAN)
    {
        DP_RWR_Bitmap_FontSelect(m_obj_paint_line, FW_BOLD, DP_RWR_DU_COLOR_LIGHT_CYAN);
    }
    else if(in_S_RWRThreatInfo.ucFG_Colour == DP_RWR_MIG29K_DU_COLOR_CODE_LIGHT_MAGENTA)
    {
        DP_RWR_Bitmap_FontSelect(m_obj_paint_line, FW_BOLD, DP_RWR_DU_COLOR_LIGHT_MAGENTA);
    }
    else
    {
        DP_RWR_Bitmap_FontSelect(m_obj_paint_line, FW_BOLD, DP_RWR_DU_COLOR_LIGHT_WHITE);
    }

    /* Display symbol name in upper case */
    strncpy(in_S_RWRThreatInfo.arrcThreatSymbolName, _strupr(in_S_RWRThreatInfo.arrcThreatSymbolName), (DP_RWR_EMITTER_SYMBOL_NAME_LEN - 1));

    if(in_S_RWRThreatInfo.ucBG_Colour == in_S_RWRThreatInfo.ucFG_Colour)
    {
        qcRgb.setRgba(qRgba(DP_RWR_DU_COLOR_BLACK, 0)); // transparent
    }
    else if(in_S_RWRThreatInfo.ucBG_Colour == DP_RWR_MIG29K_DU_COLOR_CODE_GREEN)
    {
        qcRgb.setRgb(DP_QT_EMITTER_REPORT_CIRCLE_COLOR_GREEN, 255);
    }
    else if(in_S_RWRThreatInfo.ucBG_Colour == DP_RWR_MIG29K_DU_COLOR_CODE_RED)
    {
        qcRgb.setRgb(DP_RWR_DU_COLOR_RED, 255);
    }
    else if(in_S_RWRThreatInfo.ucBG_Colour == DP_RWR_MIG29K_DU_COLOR_CODE_YELLOW)
    {
        qcRgb.setRgb(DP_RWR_DU_COLOR_YELLOW, 255);
    }
    else if(in_S_RWRThreatInfo.ucBG_Colour == DP_RWR_MIG29K_DU_COLOR_CODE_BLUE)
    {
        qcRgb.setRgb(DP_RWR_DU_COLOR_BLUE, 255);
    }
    else if(in_S_RWRThreatInfo.ucBG_Colour == DP_RWR_MIG29K_DU_COLOR_CODE_CYAN)
    {
        qcRgb.setRgb(DP_RWR_DU_COLOR_CYAN, 255);
    }
    else if(in_S_RWRThreatInfo.ucBG_Colour == DP_RWR_MIG29K_DU_COLOR_CODE_MAGENTA)
    {
        qcRgb.setRgb(DP_RWR_DU_COLOR_MAGENTA, 255);
    }
    else if(in_S_RWRThreatInfo.ucBG_Colour == DP_RWR_MIG29K_DU_COLOR_CODE_WHITE)
    {
        qcRgb.setRgb(DP_RWR_DU_COLOR_WHITE, 255);
    }
    else if(in_S_RWRThreatInfo.ucBG_Colour == DP_RWR_MIG29K_DU_COLOR_CODE_LIGHT_BLACK)
    {
        qcRgb.setRgb(DP_RWR_DU_COLOR_LIGHT_BLACK, 255);
    }
    else if(in_S_RWRThreatInfo.ucBG_Colour == DP_RWR_MIG29K_DU_COLOR_CODE_LIGHT_GREEN)
    {
        qcRgb.setRgb(DP_RWR_DU_COLOR_LIGHT_GREEN, 255);
    }
    else if(in_S_RWRThreatInfo.ucBG_Colour == DP_RWR_MIG29K_DU_COLOR_CODE_LIGHT_RED)
    {
        qcRgb.setRgb(DP_RWR_DU_COLOR_LIGHT_RED, 255);
    }
    else if(in_S_RWRThreatInfo.ucBG_Colour == DP_RWR_MIG29K_DU_COLOR_CODE_LIGHT_YELLOW)
    {
        qcRgb.setRgb(DP_RWR_DU_COLOR_LIGHT_YELLOW, 255);
    }
    else if(in_S_RWRThreatInfo.ucBG_Colour == DP_RWR_MIG29K_DU_COLOR_CODE_LIGHT_BLUE)
    {
        qcRgb.setRgb(DP_RWR_DU_COLOR_LIGHT_BLUE, 255);
    }
    else if(in_S_RWRThreatInfo.ucBG_Colour == DP_RWR_MIG29K_DU_COLOR_CODE_LIGHT_CYAN)
    {
        qcRgb.setRgb(DP_RWR_DU_COLOR_LIGHT_CYAN, 255);
    }
    else if(in_S_RWRThreatInfo.ucBG_Colour == DP_RWR_MIG29K_DU_COLOR_CODE_LIGHT_MAGENTA)
    {
        qcRgb.setRgb(DP_RWR_DU_COLOR_LIGHT_MAGENTA, 255);
    }
    else if(in_S_RWRThreatInfo.ucBG_Colour == DP_RWR_MIG29K_DU_COLOR_CODE_LIGHT_WHITE)
    {
        qcRgb.setRgb(DP_RWR_DU_COLOR_LIGHT_WHITE, 255);
    }
    else
    {
        qcRgb.setRgb(DP_RWR_DU_COLOR_BLACK, 255);
    }

    /* Display symbol name in upper case */
    strncpy(in_S_RWRThreatInfo.arrcThreatSymbolName, _strupr(in_S_RWRThreatInfo.arrcThreatSymbolName), (DP_RWR_EMITTER_SYMBOL_NAME_LEN - 1));

#ifdef DP_RWR_ENABLE_ROUNDED_THREAT_BACKGROUND
    DP_RWR_Bitmap_FillRoundedRect((float)dThreatX_Pos, (float)dThreatY_Pos, DP_RWR_ROUNDED_RECT_RADIUS5, DP_RWR_ROUNDED_RECT_RADIUS5, qcRgb, true, true);
#else
    DP_RWR_Bitmap_FillRoundedRect((float)dThreatX_Pos, (float)dThreatY_Pos, DP_RWR_ROUNDED_RECT_RADIUS5, DP_RWR_ROUNDED_RECT_RADIUS5, qcRgb, false, true);
#endif

    DP_RWR_Bitmap_Draw_Threat_Symbol(dThreatX_Pos, dThreatY_Pos, in_S_RWRThreatInfo.usSymbolCode, in_S_RWRThreatInfo.arrcThreatSymbolName, in_S_RWRThreatInfo.ucThreatSymbolSize);

    if(in_S_RWRThreatInfo.ucThreatClassification == DP_RWR_THREAT_CLASSIFY_MOST_DANGEROUS_THREAT)
    {
        QPointF qpfCenter(dThreatX_Pos, dThreatY_Pos);
        QPolygonF qpDiamond;
        qpDiamond << QPointF(qpfCenter.x(), qpfCenter.y() - DP_RWR_ROUNDED_RECT_RADIUS4)
                  << QPointF(qpfCenter.x() + DP_RWR_ROUNDED_RECT_RADIUS4, qpfCenter.y())
                  << QPointF(qpfCenter.x(), qpfCenter.y() + DP_RWR_ROUNDED_RECT_RADIUS4)
                  << QPointF(qpfCenter.x() - DP_RWR_ROUNDED_RECT_RADIUS4, qpfCenter.y());
        m_obj_paint_line->drawPolygon(qpDiamond);
    }

    if(in_S_RWRThreatInfo.ucThreatOrgin == 1)
    {
        QPointF Point1((float)(dThreatX_Pos - (DP_RWR_OUTER_THREAT_SYMBOL_SIZE1 / 3)), (float)(dThreatY_Pos - (DP_RWR_OUTER_THREAT_SYMBOL_SIZE1 / 3)));
        QPointF Point2((float)dThreatX_Pos, (float)(dThreatY_Pos - (DP_RWR_OUTER_THREAT_SYMBOL_SIZE1 / 1.5f)));
        QPointF Point3((float)dThreatX_Pos, (float)(dThreatY_Pos - (DP_RWR_OUTER_THREAT_SYMBOL_SIZE1 / 1.5f)));
        QPointF Point4((float)(dThreatX_Pos + (DP_RWR_OUTER_THREAT_SYMBOL_SIZE1 / 3)), (float)(dThreatY_Pos - (DP_RWR_OUTER_THREAT_SYMBOL_SIZE1 / 3)));

        QPointF Points[4] = {Point1, Point2, Point3, Point4};

        m_obj_paint_line->drawLines(Points, 2);
    }

    if(in_S_RWRThreatInfo.ucIsNewestThreat == 1)
    {
        /* Tracking threat symbol */
        QRectF rect((float)dThreatX_Pos - DP_RWR_ROUNDED_RECT_RADIUS4, (float)dThreatY_Pos - DP_RWR_ROUNDED_RECT_RADIUS4, DP_RWR_ROUNDED_RECT_RADIUS4 * 2, DP_RWR_ROUNDED_RECT_RADIUS4 * 2);
        m_obj_paint_line->drawArc(rect, 0 * 16, 180 * 16);
    }

    if(in_S_RWRThreatInfo.ucThreatScanStat == 1)
    {
        /* Locking on threat symbol */
        QRectF rect((float)dThreatX_Pos - DP_RWR_ROUNDED_RECT_RADIUS4, (float)dThreatY_Pos - DP_RWR_ROUNDED_RECT_RADIUS4, DP_RWR_ROUNDED_RECT_RADIUS4 * 2, DP_RWR_ROUNDED_RECT_RADIUS4 * 2);
        m_obj_paint_line->drawArc(rect, 0 * 16, 360 * 16);
    }
}

/**************************************************************************
* Name						:	DP_RWR_Bitmap_FontSelect
* Author					:	Vinoth V
* Global Variables affected	:	NA
* Created Date				:	Jan 31, 2017
* Revision Date				:	NA
* Reason for Revising		:	NA
* Description				:	This function will draw text.
*******************************************************************************/

void dp_rwr_flight_display::DP_RWR_Bitmap_FontSelect(QPainter *in_m_obj_paint_line, unsigned short in_usFontWeight, int in_iR, int in_iG, int in_iB)
{
    Q_UNUSED(in_usFontWeight);

    /* Setting font properties */
    cMyColor.setRgb(in_iR, in_iG, in_iB, 255);
    cPen.setColor(cMyColor);

    l_font.setBold(false);
    l_font.setFamily("Arial");

    in_m_obj_paint_line->setPen(cPen);
    in_m_obj_paint_line->setFont(l_font);
}

void dp_rwr_flight_display::DP_RWR_Bitmap_FillRoundedRect(float in_fx, float in_fy, float in_fwidth, float in_fheight, QColor in_qcRgb, bool in_isRounded, bool in_isFilled)
{
    QRectF qrfRect(in_fx - in_fwidth, in_fy - in_fheight, in_fwidth * 2, in_fheight * 2);
    QPainterPath cPath;

    if(!in_isFilled)
    {
        return;
    }

    /********** Setting up color of the pen **********/
    in_qcRgb.setRgba(in_qcRgb.rgba());

    /********** Adding Path to fill the Rounded Rectangle **********/
    if(in_isRounded == true)
    {
        cPath.addRoundedRect(qrfRect, in_fwidth, in_fheight);
    }
    else
    {
        cPath.addRoundedRect(qrfRect, 0, 0);
    }

    /********** Filling the Rounded Rectangle **********/
    m_obj_paint_line->fillPath(cPath, in_qcRgb);
}

/*****************************************************************************
* Name						:	DP_RWR_Bitmap_FindCharMetrics
* Author					:	Vinoth V
* Global Variables affected	:	NA
* Created Date				:	Oct 20, 2014
* Revision Date				:	NA
* Reason for Revising		:	NA
* Description				:	This function will find the character size.
*******************************************************************************/

void dp_rwr_flight_display::DP_RWR_Bitmap_FindCharMetrics(QString in_qsText, int *out_piTextWidth, int *out_piTextHeight)
{
    if((out_piTextWidth == NULL) || (out_piTextHeight == NULL))
    {
        return;
    }

    QFontMetrics obj_font_metrics(l_font);

    *out_piTextWidth = (obj_font_metrics.horizontalAdvance(in_qsText)) / 2;  //Width of the text
    *out_piTextHeight = (obj_font_metrics.ascent()) / 2;  //Height of the text
}

void dp_rwr_flight_display::DP_RWR_Bitmap_DrawNormalCircle(double in_fx1, double in_fy1, double in_fwidth, double in_fheight, QColor in_qcRgb, float in_fPenWidth, float in_fStartAngle, float in_fSpanAngle)
{
    Q_UNUSED(in_fPenWidth);

    /********** Setting up color and style of the pen **********/
    cMyColor.setRgb(in_qcRgb.rgb());
    cPen.setColor(cMyColor);
    cPen.setCapStyle(Qt::FlatCap);

    /********** Adjusting and setting up the pen size based on the boundary width and height **********/
    //DP_RWR_Bitmap_AdjustPenWidth(in_fPenWidth);
    cPen.setWidthF(in_fPenWidth); //HardCoded

    /********** Setting up the pen to draw **********/
    m_obj_paint_line->setPen(cPen);

    /********** Drawing the Circle **********/
    m_obj_paint_line->drawArc(in_fx1, in_fy1, in_fwidth, in_fheight, in_fStartAngle, in_fSpanAngle);
}

/*****************************************************************************
* Name						:	DP_RWR_Bitmap_DrawText
* Author					:	Vinoth V
* Global Variables affected	:	NA
* Created Date				:	Oct 20, 2014
* Revision Date				:	NA
* Reason for Revising		:	NA
* Description				:	This function will draw text.
*******************************************************************************/
void dp_rwr_flight_display::DP_RWR_Bitmap_DrawText(QPainter *in_m_obj_painter, int in_iXPos, int in_iYPos, QString in_qsStr, unsigned char in_ucAlignment)
{
    int iTextWidth = 0;
    int iTextHeight = 0;

    /* This logic is done to adjust the symbol name into the center position, based on the character width */
    DP_RWR_Bitmap_FindCharMetrics(in_qsStr, &iTextWidth, &iTextHeight);

    if(in_ucAlignment == DP_RWR_TEXT_ALIGNMENT_CENTER)
    {
        in_m_obj_painter->drawText(in_iXPos - iTextWidth, in_iYPos + iTextHeight, in_qsStr);
    }
    else if(in_ucAlignment == DP_RWR_TEXT_ALIGNMENT_HCENTER)
    {
        in_m_obj_painter->drawText(in_iXPos, in_iYPos + iTextHeight, in_qsStr);
    }
    else if(in_ucAlignment == DP_RWR_TEXT_ALIGNMENT_VCENTER)
    {
        in_m_obj_painter->drawText(in_iXPos, in_iYPos + iTextHeight, in_qsStr);
    }
}

void dp_rwr_flight_display::DP_RWR_Bitmap_DrawLine(double in_fx1, double in_fy1, double in_fx2, double in_fy2, QColor in_qcRgb, float in_fPenWidth, unsigned char in_ucAddWeight)
{
    /********** Setting up color and style of the pen **********/
    cMyColor.setRgb(in_qcRgb.rgb());
    cPen.setColor(cMyColor);
    cPen.setCapStyle(Qt::FlatCap);

    /********** Adjusting and setting up the pen size based on the boundary width and height **********/
    if(in_ucAddWeight == DP_RWR_ACCEPT_ADD_WEIGHTAGE)
    {
        DP_RWR_Bitmap_AdjustPenWidth(&in_fPenWidth);
    }
    cPen.setWidthF(in_fPenWidth);

    /********** Setting up the pen to draw **********/
    m_obj_paint_line->setPen(cPen);

    /********** Drawing the Line **********/
    m_obj_paint_line->drawLine(in_fx1, in_fy1, in_fx2, in_fy2);
}

void dp_rwr_flight_display::DP_RWR_Bitmap_Draw_Threat_Symbol(double in_dThreatX_Pos, double in_dThreatY_Pos, unsigned short in_usSymbolCode, char *in_arrcThreatName, unsigned char in_ucThreatSymbolSize)
{
    switch(in_usSymbolCode)
    {
    case DP_THREAT_SYMBOL_CODE_42:
        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos - (in_ucThreatSymbolSize / 2), in_dThreatY_Pos - (in_ucThreatSymbolSize / 2),
                               in_dThreatX_Pos + (in_ucThreatSymbolSize / 2), in_dThreatY_Pos - (in_ucThreatSymbolSize / 2), cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos - (in_ucThreatSymbolSize / 2), in_dThreatY_Pos - (in_ucThreatSymbolSize / 2),
                               in_dThreatX_Pos - (in_ucThreatSymbolSize / 2), in_dThreatY_Pos + (in_ucThreatSymbolSize / 2), cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos + (in_ucThreatSymbolSize / 2), in_dThreatY_Pos - (in_ucThreatSymbolSize / 2),
                               in_dThreatX_Pos + (in_ucThreatSymbolSize / 2), in_dThreatY_Pos + (in_ucThreatSymbolSize / 2), cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos - (in_ucThreatSymbolSize / 2), in_dThreatY_Pos + (in_ucThreatSymbolSize / 2),
                               in_dThreatX_Pos - in_ucThreatSymbolSize, in_dThreatY_Pos + (in_ucThreatSymbolSize / 2), cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos + (in_ucThreatSymbolSize / 2), in_dThreatY_Pos + (in_ucThreatSymbolSize / 2),
                               in_dThreatX_Pos + in_ucThreatSymbolSize, in_dThreatY_Pos + (in_ucThreatSymbolSize / 2), cMyColor, DP_PEN_WIDTH_2);
        break;

    case DP_THREAT_SYMBOL_CODE_43:

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos, in_dThreatY_Pos - (in_ucThreatSymbolSize / 2),
                               in_dThreatX_Pos, in_dThreatY_Pos + (in_ucThreatSymbolSize / 2), cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos - (in_ucThreatSymbolSize / 2), in_dThreatY_Pos + (in_ucThreatSymbolSize / 2),
                               in_dThreatX_Pos + (in_ucThreatSymbolSize / 2), in_dThreatY_Pos + (in_ucThreatSymbolSize / 2), cMyColor, DP_PEN_WIDTH_2);
        break;

    case DP_THREAT_SYMBOL_CODE_44:

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos - (in_ucThreatSymbolSize / 2), in_dThreatY_Pos - (in_ucThreatSymbolSize / 2),
                               in_dThreatX_Pos - (in_ucThreatSymbolSize / 2), in_dThreatY_Pos + (in_ucThreatSymbolSize / 2), cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos + (in_ucThreatSymbolSize / 2), in_dThreatY_Pos - (in_ucThreatSymbolSize / 2),
                               in_dThreatX_Pos + (in_ucThreatSymbolSize / 2), in_dThreatY_Pos + (in_ucThreatSymbolSize / 2), cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos - in_ucThreatSymbolSize, in_dThreatY_Pos + (in_ucThreatSymbolSize / 2),
                               in_dThreatX_Pos + in_ucThreatSymbolSize, in_dThreatY_Pos + (in_ucThreatSymbolSize / 2), cMyColor, DP_PEN_WIDTH_2);
        break;

    case DP_THREAT_SYMBOL_CODE_45:

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos, in_dThreatY_Pos - (in_ucThreatSymbolSize / 2),
                               in_dThreatX_Pos, in_dThreatY_Pos + (in_ucThreatSymbolSize / 2), cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos - (in_ucThreatSymbolSize / 2), in_dThreatY_Pos - (in_ucThreatSymbolSize / 2),
                               in_dThreatX_Pos - (in_ucThreatSymbolSize / 2), in_dThreatY_Pos + (in_ucThreatSymbolSize / 2), cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos + (in_ucThreatSymbolSize / 2), in_dThreatY_Pos - (in_ucThreatSymbolSize / 2),
                               in_dThreatX_Pos + (in_ucThreatSymbolSize / 2), in_dThreatY_Pos + (in_ucThreatSymbolSize / 2), cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos + in_ucThreatSymbolSize, in_dThreatY_Pos + (in_ucThreatSymbolSize / 2),
                               in_dThreatX_Pos - in_ucThreatSymbolSize, in_dThreatY_Pos + (in_ucThreatSymbolSize / 2), cMyColor, DP_PEN_WIDTH_2);
        break;

    case DP_THREAT_SYMBOL_CODE_46:

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos - (in_ucThreatSymbolSize / 2.75f), in_dThreatY_Pos - (in_ucThreatSymbolSize / 2),
                               in_dThreatX_Pos - (in_ucThreatSymbolSize / 2.75f), in_dThreatY_Pos + (in_ucThreatSymbolSize / 2), cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos - in_ucThreatSymbolSize, in_dThreatY_Pos - (in_ucThreatSymbolSize / 2),
                               in_dThreatX_Pos - in_ucThreatSymbolSize, in_dThreatY_Pos + (in_ucThreatSymbolSize / 2), cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos + (in_ucThreatSymbolSize / 2.75f), in_dThreatY_Pos - (in_ucThreatSymbolSize / 2),
                               in_dThreatX_Pos + (in_ucThreatSymbolSize / 2.75f), in_dThreatY_Pos + (in_ucThreatSymbolSize / 2), cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos + in_ucThreatSymbolSize, in_dThreatY_Pos - (in_ucThreatSymbolSize / 2),
                               in_dThreatX_Pos + in_ucThreatSymbolSize,  in_dThreatY_Pos + (in_ucThreatSymbolSize / 2), cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos + in_ucThreatSymbolSize, in_dThreatY_Pos + (in_ucThreatSymbolSize / 2),
                               in_dThreatX_Pos - in_ucThreatSymbolSize, in_dThreatY_Pos + (in_ucThreatSymbolSize / 2), cMyColor, DP_PEN_WIDTH_2);
        break;

    case DP_THREAT_SYMBOL_CODE_47:

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos - (in_ucThreatSymbolSize / 3), in_dThreatY_Pos - (in_ucThreatSymbolSize / 2),
                               in_dThreatX_Pos - (in_ucThreatSymbolSize / 3), in_dThreatY_Pos + (in_ucThreatSymbolSize / 2), cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos - in_ucThreatSymbolSize, in_dThreatY_Pos - (in_ucThreatSymbolSize / 2),
                               in_dThreatX_Pos - in_ucThreatSymbolSize, in_dThreatY_Pos + (in_ucThreatSymbolSize / 2), cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos + (in_ucThreatSymbolSize / 3), in_dThreatY_Pos - (in_ucThreatSymbolSize / 2),
                               in_dThreatX_Pos + (in_ucThreatSymbolSize / 3), in_dThreatY_Pos + (in_ucThreatSymbolSize / 2), cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos + in_ucThreatSymbolSize, in_dThreatY_Pos - (in_ucThreatSymbolSize / 2),
                               in_dThreatX_Pos + in_ucThreatSymbolSize, in_dThreatY_Pos + (in_ucThreatSymbolSize / 2), cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos - (in_ucThreatSymbolSize / 3), in_dThreatY_Pos - (in_ucThreatSymbolSize / 2),
                               in_dThreatX_Pos + (in_ucThreatSymbolSize / 3), in_dThreatY_Pos - (in_ucThreatSymbolSize / 2), cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos - (in_ucThreatSymbolSize / 3), in_dThreatY_Pos + (in_ucThreatSymbolSize / 2),
                               in_dThreatX_Pos - in_ucThreatSymbolSize, in_dThreatY_Pos + (in_ucThreatSymbolSize / 2), cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos + (in_ucThreatSymbolSize / 3), in_dThreatY_Pos + (in_ucThreatSymbolSize / 2),
                               in_dThreatX_Pos + in_ucThreatSymbolSize, in_dThreatY_Pos + (in_ucThreatSymbolSize / 2), cMyColor, DP_PEN_WIDTH_2);
        break;

    case DP_THREAT_SYMBOL_CODE_49:
        DP_RWR_Bitmap_DrawText(m_obj_paint_line, in_dThreatX_Pos, in_dThreatY_Pos, "S");
        DP_RWR_Bitmap_DrawNormalCircle(in_dThreatX_Pos - ((in_ucThreatSymbolSize / 1.75f) / 2), in_dThreatY_Pos - in_ucThreatSymbolSize, in_ucThreatSymbolSize / 1.75f, in_ucThreatSymbolSize / 1.75f, cMyColor, DP_PEN_WIDTH_2, 180 * 16, 180 * 16);
        break;

    case DP_THREAT_SYMBOL_CODE_50:
        DP_RWR_Bitmap_DrawText(m_obj_paint_line, in_dThreatX_Pos, in_dThreatY_Pos, "A");

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos - (in_ucThreatSymbolSize / 3), in_dThreatY_Pos - (in_ucThreatSymbolSize / 1.5f),
                               in_dThreatX_Pos, in_dThreatY_Pos - (in_ucThreatSymbolSize / 3), cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos, in_dThreatY_Pos - (in_ucThreatSymbolSize / 3),
                               in_dThreatX_Pos + (in_ucThreatSymbolSize / 1.5f), in_dThreatY_Pos - (in_ucThreatSymbolSize / 1.5f), cMyColor, DP_PEN_WIDTH_2);
        break;

    case DP_THREAT_SYMBOL_CODE_51:

        /* Radio */
        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos - (in_ucThreatSymbolSize / 1.3f), in_dThreatY_Pos - (in_ucThreatSymbolSize / 2.2f),
                               in_dThreatX_Pos + (in_ucThreatSymbolSize / 1.3f), in_dThreatY_Pos - (in_ucThreatSymbolSize / 2.2f), cMyColor, DP_PEN_WIDTH_2);  //Code 12

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos - (in_ucThreatSymbolSize / 1.3f), in_dThreatY_Pos + (in_ucThreatSymbolSize / 2.2f),
                               in_dThreatX_Pos + (in_ucThreatSymbolSize / 1.3f), in_dThreatY_Pos + (in_ucThreatSymbolSize / 2.2f), cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos - (in_ucThreatSymbolSize / 1.3f), in_dThreatY_Pos - (in_ucThreatSymbolSize / 2.2f),
                               in_dThreatX_Pos - (in_ucThreatSymbolSize / 1.3f), in_dThreatY_Pos + (in_ucThreatSymbolSize / 2.2f), cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos + (in_ucThreatSymbolSize / 1.3f), in_dThreatY_Pos - (in_ucThreatSymbolSize / 2.2f),
                               in_dThreatX_Pos + (in_ucThreatSymbolSize / 1.3f), in_dThreatY_Pos + (in_ucThreatSymbolSize / 2.2f), cMyColor, DP_PEN_WIDTH_2);

        /* Antenna */
        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos - (in_ucThreatSymbolSize / 2), in_dThreatY_Pos - (in_ucThreatSymbolSize / 2.2f),
                               (in_dThreatX_Pos - (in_ucThreatSymbolSize / 2)) + (in_ucThreatSymbolSize / 2), in_dThreatY_Pos - in_ucThreatSymbolSize, cMyColor, DP_PEN_WIDTH_2);
        break;

    case DP_THREAT_SYMBOL_CODE_52:

        /* Radio */
        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos - (in_ucThreatSymbolSize / 1.3f), in_dThreatY_Pos - (in_ucThreatSymbolSize / 2.2f),
                               in_dThreatX_Pos + (in_ucThreatSymbolSize / 1.3f), in_dThreatY_Pos - (in_ucThreatSymbolSize / 2.2f), cMyColor, DP_PEN_WIDTH_2);  //Code 12

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos - (in_ucThreatSymbolSize / 1.3f), in_dThreatY_Pos + (in_ucThreatSymbolSize / 2.2f),
                               in_dThreatX_Pos + (in_ucThreatSymbolSize / 1.3f), in_dThreatY_Pos + (in_ucThreatSymbolSize / 2.2f), cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos - (in_ucThreatSymbolSize / 1.3f), in_dThreatY_Pos - (in_ucThreatSymbolSize / 2.2f),
                               in_dThreatX_Pos - (in_ucThreatSymbolSize / 1.3f), in_dThreatY_Pos + (in_ucThreatSymbolSize / 2.2f), cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos + (in_ucThreatSymbolSize / 1.3f), in_dThreatY_Pos - (in_ucThreatSymbolSize / 2.2f),
                               in_dThreatX_Pos + (in_ucThreatSymbolSize / 1.3f), in_dThreatY_Pos + (in_ucThreatSymbolSize / 2.2f), cMyColor, DP_PEN_WIDTH_2);

        /* Antennas */
        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos - (in_ucThreatSymbolSize / 2), in_dThreatY_Pos - (in_ucThreatSymbolSize / 2.2f),
                               (in_dThreatX_Pos - (in_ucThreatSymbolSize / 2)) + (in_ucThreatSymbolSize / 2), in_dThreatY_Pos - in_ucThreatSymbolSize, cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos + (in_ucThreatSymbolSize / 2), in_dThreatY_Pos - (in_ucThreatSymbolSize / 2.2f),
                               (in_dThreatX_Pos + (in_ucThreatSymbolSize / 2)) + (in_ucThreatSymbolSize / 2), in_dThreatY_Pos - in_ucThreatSymbolSize, cMyColor, DP_PEN_WIDTH_2);
        break;

    case DP_THREAT_SYMBOL_CODE_53:

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos - (in_ucThreatSymbolSize / 3), in_dThreatY_Pos - (in_ucThreatSymbolSize / 3),
                               in_dThreatX_Pos - (in_ucThreatSymbolSize / 3), in_dThreatY_Pos + in_ucThreatSymbolSize, cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos + (in_ucThreatSymbolSize / 3), in_dThreatY_Pos - (in_ucThreatSymbolSize / 3),
                               in_dThreatX_Pos + (in_ucThreatSymbolSize / 3), in_dThreatY_Pos + in_ucThreatSymbolSize, cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos - (in_ucThreatSymbolSize / 3), in_dThreatY_Pos - (in_ucThreatSymbolSize / 3),
                               in_dThreatX_Pos, in_dThreatY_Pos - in_ucThreatSymbolSize, cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos + (in_ucThreatSymbolSize / 3), in_dThreatY_Pos - (in_ucThreatSymbolSize / 3),
                               in_dThreatX_Pos, in_dThreatY_Pos - in_ucThreatSymbolSize, cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos - (in_ucThreatSymbolSize / 1.5f), in_dThreatY_Pos + in_ucThreatSymbolSize,
                               in_dThreatX_Pos + (in_ucThreatSymbolSize / 1.5f), in_dThreatY_Pos + in_ucThreatSymbolSize, cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos - (in_ucThreatSymbolSize / 1.5f), in_dThreatY_Pos + in_ucThreatSymbolSize,
                               in_dThreatX_Pos - (in_ucThreatSymbolSize / 1.5f), in_dThreatY_Pos, cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos + (in_ucThreatSymbolSize / 1.5f), in_dThreatY_Pos + in_ucThreatSymbolSize,
                               in_dThreatX_Pos + (in_ucThreatSymbolSize / 1.5f), in_dThreatY_Pos, cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos - (in_ucThreatSymbolSize / 1.5f), in_dThreatY_Pos,
                               in_dThreatX_Pos - (in_ucThreatSymbolSize / 3), in_dThreatY_Pos - (in_ucThreatSymbolSize / 3), cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos + (in_ucThreatSymbolSize / 1.5f), in_dThreatY_Pos,
                               in_dThreatX_Pos + (in_ucThreatSymbolSize / 3), in_dThreatY_Pos - (in_ucThreatSymbolSize / 3), cMyColor, DP_PEN_WIDTH_2);
        break;

    case DP_THREAT_SYMBOL_CODE_54: /* Offset issue */

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos - (in_ucThreatSymbolSize / 1.5f), in_dThreatY_Pos - (in_ucThreatSymbolSize / 3),
                               in_dThreatX_Pos - (in_ucThreatSymbolSize / 1.5f), in_dThreatY_Pos + (in_ucThreatSymbolSize / 1.5f), cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos + (in_ucThreatSymbolSize / 1.5f), in_dThreatY_Pos - (in_ucThreatSymbolSize / 3),
                               in_dThreatX_Pos + (in_ucThreatSymbolSize / 1.5f), in_dThreatY_Pos + (in_ucThreatSymbolSize / 1.5f), cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos, in_dThreatY_Pos - in_ucThreatSymbolSize,
                               in_dThreatX_Pos, in_dThreatY_Pos + (in_ucThreatSymbolSize / 1.5f), cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos - (in_ucThreatSymbolSize / 1.5f), in_dThreatY_Pos - (in_ucThreatSymbolSize / 3),
                               in_dThreatX_Pos + (in_ucThreatSymbolSize / 1.5f), in_dThreatY_Pos - (in_ucThreatSymbolSize / 3), cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos + (in_ucThreatSymbolSize / 1.5f), in_dThreatY_Pos + (in_ucThreatSymbolSize / 1.5f),
                               in_dThreatX_Pos - (in_ucThreatSymbolSize / 1.5f), in_dThreatY_Pos + (in_ucThreatSymbolSize / 1.5f), cMyColor, DP_PEN_WIDTH_2);
        break;

    case DP_THREAT_SYMBOL_CODE_55:

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos - (in_ucThreatSymbolSize / 2), in_dThreatY_Pos + (in_ucThreatSymbolSize / 2),
                               in_dThreatX_Pos + (in_ucThreatSymbolSize / 2), in_dThreatY_Pos + (in_ucThreatSymbolSize / 2), cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos - (in_ucThreatSymbolSize / 2), in_dThreatY_Pos + (in_ucThreatSymbolSize / 2),
                               in_dThreatX_Pos, in_dThreatY_Pos - (in_ucThreatSymbolSize / 2), cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos + (in_ucThreatSymbolSize / 2), in_dThreatY_Pos + (in_ucThreatSymbolSize / 2),
                               in_dThreatX_Pos, in_dThreatY_Pos - (in_ucThreatSymbolSize / 2), cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos, in_dThreatY_Pos + (in_ucThreatSymbolSize / 2),
                               in_dThreatX_Pos, in_dThreatY_Pos + in_ucThreatSymbolSize, cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos - (in_ucThreatSymbolSize / 3), in_dThreatY_Pos + in_ucThreatSymbolSize,
                               in_dThreatX_Pos + (in_ucThreatSymbolSize / 3), in_dThreatY_Pos + in_ucThreatSymbolSize, cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos, in_dThreatY_Pos - (in_ucThreatSymbolSize / 2),
                               in_dThreatX_Pos, in_dThreatY_Pos - in_ucThreatSymbolSize, cMyColor, DP_PEN_WIDTH_2);
        break;

    case DP_THREAT_SYMBOL_CODE_56:

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos, in_dThreatY_Pos - in_ucThreatSymbolSize,
                               in_dThreatX_Pos, in_dThreatY_Pos + in_ucThreatSymbolSize, cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos, in_dThreatY_Pos + (in_ucThreatSymbolSize / 4),
                               in_dThreatX_Pos - (in_ucThreatSymbolSize / 2), in_dThreatY_Pos + in_ucThreatSymbolSize, cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos, in_dThreatY_Pos + (in_ucThreatSymbolSize / 4),
                               in_dThreatX_Pos + (in_ucThreatSymbolSize / 2), in_dThreatY_Pos + in_ucThreatSymbolSize, cMyColor, DP_PEN_WIDTH_2);
        break;

    case DP_THREAT_SYMBOL_CODE_57:

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos, in_dThreatY_Pos -  (in_ucThreatSymbolSize / 2),
                               in_dThreatX_Pos, in_dThreatY_Pos + (in_ucThreatSymbolSize / 2), cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos, in_dThreatY_Pos - (in_ucThreatSymbolSize / 2),
                               in_dThreatX_Pos - (in_ucThreatSymbolSize / 3), in_dThreatY_Pos - (in_ucThreatSymbolSize / 2), cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos - (in_ucThreatSymbolSize / 3), in_dThreatY_Pos + (in_ucThreatSymbolSize / 2),
                               in_dThreatX_Pos + (in_ucThreatSymbolSize / 3), in_dThreatY_Pos + (in_ucThreatSymbolSize / 2), cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawNormalCircle((in_dThreatX_Pos - (in_ucThreatSymbolSize / 3)) - (in_ucThreatSymbolSize / 1.5f),
                                       (in_dThreatY_Pos - (in_ucThreatSymbolSize / 2)) - ((in_ucThreatSymbolSize / 1.5f) / 2),
                                       (in_ucThreatSymbolSize / 1.5f), (in_ucThreatSymbolSize / 1.5f), cMyColor, DP_PEN_WIDTH_2, 270 * 16, 180 * 16);
        break;

    case DP_THREAT_SYMBOL_CODE_58:
        DP_RWR_Bitmap_DrawText(m_obj_paint_line, in_dThreatX_Pos, in_dThreatY_Pos, "R");
        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos + (in_ucThreatSymbolSize / 4.5f), in_dThreatY_Pos + (in_ucThreatSymbolSize / 4.5f),
                               in_dThreatX_Pos + (in_ucThreatSymbolSize / 1.125), in_dThreatY_Pos - (in_ucThreatSymbolSize / 2.25f), cMyColor, DP_PEN_WIDTH_2);
        break;

    case DP_THREAT_SYMBOL_CODE_59:

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos - (in_ucThreatSymbolSize / 2), in_dThreatY_Pos + (in_ucThreatSymbolSize / 1.5f),
                               in_dThreatX_Pos + (in_ucThreatSymbolSize / 2), in_dThreatY_Pos + (in_ucThreatSymbolSize / 1.5f), cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos, in_dThreatY_Pos - (in_ucThreatSymbolSize / 2),
                               in_dThreatX_Pos, in_dThreatY_Pos + (in_ucThreatSymbolSize / 1.5f), cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawNormalCircle((in_dThreatX_Pos - (in_ucThreatSymbolSize / 3)) - ((in_ucThreatSymbolSize / 1.5f) / 2),
                                       (in_dThreatY_Pos - (in_ucThreatSymbolSize / 3)) - ((in_ucThreatSymbolSize / 1.5f) / 2),
                                       (in_ucThreatSymbolSize / 1.5f), (in_ucThreatSymbolSize / 1.5f), cMyColor, DP_PEN_WIDTH_2, 360 * 16, 315 * 16);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos, in_dThreatY_Pos - (in_ucThreatSymbolSize / 3),
                               in_dThreatX_Pos + (in_ucThreatSymbolSize / 2), in_dThreatY_Pos - in_ucThreatSymbolSize, cMyColor, DP_PEN_WIDTH_2);
        break;

    case DP_THREAT_SYMBOL_CODE_60:

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos - (in_ucThreatSymbolSize / 1.5f), in_dThreatY_Pos - (in_ucThreatSymbolSize / 2),
                               in_dThreatX_Pos + (in_ucThreatSymbolSize / 1.5f), in_dThreatY_Pos + (in_ucThreatSymbolSize / 2), cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos + (in_ucThreatSymbolSize / 1.5f), in_dThreatY_Pos - (in_ucThreatSymbolSize / 2),
                               in_dThreatX_Pos - (in_ucThreatSymbolSize / 1.5f), in_dThreatY_Pos + (in_ucThreatSymbolSize / 2), cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos - (in_ucThreatSymbolSize / 1.5f), in_dThreatY_Pos - (in_ucThreatSymbolSize / 2),
                               in_dThreatX_Pos + (in_ucThreatSymbolSize / 1.5f), in_dThreatY_Pos - (in_ucThreatSymbolSize / 2), cMyColor, DP_PEN_WIDTH_2);
        break;

    case DP_THREAT_SYMBOL_CODE_61:

        in_dThreatX_Pos -= (in_ucThreatSymbolSize / 4);
        in_dThreatY_Pos += (in_ucThreatSymbolSize / 4);
        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos, in_dThreatY_Pos,
                               in_dThreatX_Pos - in_ucThreatSymbolSize, in_dThreatY_Pos, cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos, in_dThreatY_Pos,
                               in_dThreatX_Pos, in_dThreatY_Pos + in_ucThreatSymbolSize, cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos, in_dThreatY_Pos,
                               in_dThreatX_Pos - in_ucThreatSymbolSize, in_dThreatY_Pos + (in_ucThreatSymbolSize / 1.75f), cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos, in_dThreatY_Pos,
                               in_dThreatX_Pos - (in_ucThreatSymbolSize / 1.75f), in_dThreatY_Pos + in_ucThreatSymbolSize, cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos - (in_ucThreatSymbolSize / 2), in_dThreatY_Pos - (in_ucThreatSymbolSize / 2),
                               in_dThreatX_Pos + (in_ucThreatSymbolSize / 2), in_dThreatY_Pos + (in_ucThreatSymbolSize / 2),
                               cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos - (in_ucThreatSymbolSize / 2), in_dThreatY_Pos - (in_ucThreatSymbolSize / 2),
                               in_dThreatX_Pos + (in_ucThreatSymbolSize / 2), in_dThreatY_Pos - (in_ucThreatSymbolSize),
                               cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos + (in_ucThreatSymbolSize / 2), in_dThreatY_Pos - (in_ucThreatSymbolSize),
                               in_dThreatX_Pos + (in_ucThreatSymbolSize), in_dThreatY_Pos - (in_ucThreatSymbolSize / 2),
                               cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos + (in_ucThreatSymbolSize), in_dThreatY_Pos - (in_ucThreatSymbolSize / 2),
                               in_dThreatX_Pos + (in_ucThreatSymbolSize / 2), in_dThreatY_Pos + (in_ucThreatSymbolSize / 2),
                               cMyColor, DP_PEN_WIDTH_2);
        in_dThreatX_Pos += (in_ucThreatSymbolSize / 4);
        in_dThreatY_Pos -= (in_ucThreatSymbolSize / 4);
        break;

    case DP_THREAT_SYMBOL_CODE_62:

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos - (in_ucThreatSymbolSize), in_dThreatY_Pos - (in_ucThreatSymbolSize / 2),
                               in_dThreatX_Pos + in_ucThreatSymbolSize, in_dThreatY_Pos - (in_ucThreatSymbolSize / 2), cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos - (in_ucThreatSymbolSize), in_dThreatY_Pos - (in_ucThreatSymbolSize / 2),
                               in_dThreatX_Pos, in_dThreatY_Pos + in_ucThreatSymbolSize, cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos + (in_ucThreatSymbolSize), in_dThreatY_Pos - (in_ucThreatSymbolSize / 2),
                               in_dThreatX_Pos, in_dThreatY_Pos + in_ucThreatSymbolSize, cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawNormalCircle(in_dThreatX_Pos - (in_ucThreatSymbolSize / 2), in_dThreatY_Pos - (in_ucThreatSymbolSize / 2),
                                       (in_dThreatX_Pos + (in_ucThreatSymbolSize / 2)) - (in_dThreatX_Pos - (in_ucThreatSymbolSize / 2)),
                                       (in_dThreatY_Pos + (in_ucThreatSymbolSize / 2)) - (in_dThreatY_Pos - (in_ucThreatSymbolSize / 2)),
                                       cMyColor, DP_PEN_WIDTH_2, 0, 360 * 16);
        break;

    case DP_THREAT_SYMBOL_CODE_63:

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos - in_ucThreatSymbolSize, in_dThreatY_Pos,
                               in_dThreatX_Pos + in_ucThreatSymbolSize, in_dThreatY_Pos, cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos - (in_ucThreatSymbolSize / 2), in_dThreatY_Pos,
                               in_dThreatX_Pos - (in_ucThreatSymbolSize / 2), in_dThreatY_Pos - (in_ucThreatSymbolSize / 1.25f), cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos - (in_ucThreatSymbolSize / 2), in_dThreatY_Pos - (in_ucThreatSymbolSize / 1.25f),
                               in_dThreatX_Pos + (in_ucThreatSymbolSize / 2), in_dThreatY_Pos, cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos - in_ucThreatSymbolSize, in_dThreatY_Pos,
                               in_dThreatX_Pos - (in_ucThreatSymbolSize / 2), in_dThreatY_Pos + (in_ucThreatSymbolSize / 1.5f), cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos + in_ucThreatSymbolSize, in_dThreatY_Pos,
                               in_dThreatX_Pos + (in_ucThreatSymbolSize / 2), in_dThreatY_Pos + (in_ucThreatSymbolSize / 1.5f), cMyColor, DP_PEN_WIDTH_2);

        DP_RWR_Bitmap_DrawLine(in_dThreatX_Pos +  (in_ucThreatSymbolSize / 2), in_dThreatY_Pos + (in_ucThreatSymbolSize / 1.5f),
                               in_dThreatX_Pos -  (in_ucThreatSymbolSize / 2), in_dThreatY_Pos + (in_ucThreatSymbolSize / 1.5f), cMyColor, DP_PEN_WIDTH_2);
        break;

    default:
        /* Draw text */
        DP_RWR_Bitmap_DrawText(m_obj_paint_line, in_dThreatX_Pos, in_dThreatY_Pos, in_arrcThreatName);
        break;
    }
}
short dp_rwr_flight_display::UpdateEmitterSymbolDetails(unsigned char in_ucFileAttrib, S_EMITTER_SYMBOL_LIBRARY *pS_EmitterSymbolLibrary)
{
    FILE *fptr = NULL;
    QString qsAppPath = "", qsFilePath = "";
    int iRetVal = 0;
    short sRetVal = 0;
    unsigned short usSymbolCount = 0;
    unsigned short usSymbolCode = 0;
    unsigned short usSymbolIndex = 0;
    unsigned short usMaxSymbolIndex = 0;
    char arrcEmitterSymbol[DP_RWR_EMITTER_SYMBOL_NAME_LEN] = {0};
    char arrcEmitterSymbolDesc[DP_RWR_EMITTER_SYMBOL_DESC_STR_LEN] = {0};
    char arrcTemp[256] = "";
    static S_EMITTER_SYMBOL_LIBRARY sEmitterSymbolDesc;
    if(pS_EmitterSymbolLibrary == NULL)
    {
        return DP_ASPJ_FAILURE;
    }
    qsAppPath = QCoreApplication::applicationFilePath();
    qsAppPath = qsAppPath.left(qsAppPath.lastIndexOf('/'));
    qsFilePath = QString("%1/dp_rwr_threat_symbol_info.csv").arg(qsAppPath);
    switch(in_ucFileAttrib)
    {
    case DP_RWR_LOAD_CONFIG:
        sEmitterSymbolDesc.usSymbolCount = 0;
        pS_EmitterSymbolLibrary->usSymbolCount = 0;
        fptr = fopen(qsFilePath.toUtf8().data(), "r");
        if(fptr == NULL)
        {
            sRetVal = -1;
            break;
        }
        DP_RWR_LoadSpecialSymbolDetails(&sEmitterSymbolDesc);
        fgets(arrcTemp, sizeof(arrcTemp), fptr);
        /* Default symbol details */
        for(usSymbolIndex = 0; usSymbolIndex < DP_RWR_MAX_THREAT_SYMBOL_DATA_COUNT; usSymbolIndex++)
        {
            sEmitterSymbolDesc.SarrDispEmitterSymbolDesc[usSymbolIndex].ucEmitterSymbolCode = usSymbolIndex;
            memcpy(sEmitterSymbolDesc.SarrDispEmitterSymbolDesc[usSymbolIndex].arrcEmitterSymbolDesc, "No sym desc", sizeof(sEmitterSymbolDesc.SarrDispEmitterSymbolDesc[usSymbolIndex].arrcEmitterSymbolDesc));
            memcpy(sEmitterSymbolDesc.SarrDispEmitterSymbolDesc[usSymbolIndex].arrcEmitterSymbol, "DF", sizeof(sEmitterSymbolDesc.SarrDispEmitterSymbolDesc[usSymbolIndex].arrcEmitterSymbol));
        }
        /* Read symbol details from file */
        while((!feof(fptr)) && (usSymbolCount < DP_RWR_MAX_THREAT_SYMBOL_DATA_COUNT))
        {
            usSymbolCode = 0;
            memset(arrcTemp, 0, sizeof(arrcTemp));
            memset(arrcEmitterSymbol, 0, sizeof(arrcEmitterSymbol));
            memset(arrcEmitterSymbolDesc, 0, sizeof(arrcEmitterSymbolDesc));
            fgets(arrcTemp, sizeof(arrcTemp), fptr);
            iRetVal = sscanf(arrcTemp, "%hu,%16[^,],%2[^\n]", &usSymbolCode, arrcEmitterSymbolDesc, arrcEmitterSymbol);
            if(iRetVal != -1)
            {
                usSymbolIndex = usSymbolCode;
                if(usSymbolCode >= usMaxSymbolIndex)
                {
                    usMaxSymbolIndex = usSymbolCode;
                }
                /* Updating the symbol code */
                sEmitterSymbolDesc.SarrDispEmitterSymbolDesc[usSymbolIndex].ucEmitterSymbolCode = usSymbolCode;
                /* Check for symbol description and if it's not empty go on and update the description read from file */
                if(strcmp(arrcEmitterSymbolDesc, "") != 0)
                {
                    /* Clearing default symbol description and updating it in the characted array */
                    memset(sEmitterSymbolDesc.SarrDispEmitterSymbolDesc[usSymbolIndex].arrcEmitterSymbolDesc, 0, sizeof(sEmitterSymbolDesc.SarrDispEmitterSymbolDesc[usSymbolIndex].arrcEmitterSymbolDesc));
                    memcpy(sEmitterSymbolDesc.SarrDispEmitterSymbolDesc[usSymbolIndex].arrcEmitterSymbolDesc, arrcEmitterSymbolDesc, sizeof(sEmitterSymbolDesc.SarrDispEmitterSymbolDesc[usSymbolIndex].arrcEmitterSymbolDesc));
                    /* Check for symbol and if it's not empty go on and update the description read from file or clear the default symbol, since it is left empty intentionally by the user(description added with empty symbol) */
                    if(strcmp(arrcEmitterSymbol, "") != 0)
                    {
                        /* Clearing default symbol and updating it in the characted array */
                        memset(sEmitterSymbolDesc.SarrDispEmitterSymbolDesc[usSymbolIndex].arrcEmitterSymbol, 0, sizeof(sEmitterSymbolDesc.SarrDispEmitterSymbolDesc[usSymbolIndex].arrcEmitterSymbol));
                        memcpy(sEmitterSymbolDesc.SarrDispEmitterSymbolDesc[usSymbolIndex].arrcEmitterSymbol, arrcEmitterSymbol, sizeof(sEmitterSymbolDesc.SarrDispEmitterSymbolDesc[usSymbolIndex].arrcEmitterSymbol));
                    }
                    else
                    {
                        memset(sEmitterSymbolDesc.SarrDispEmitterSymbolDesc[usSymbolIndex].arrcEmitterSymbol, 0, sizeof(sEmitterSymbolDesc.SarrDispEmitterSymbolDesc[usSymbolIndex].arrcEmitterSymbol));
                    }
                }
                /* Default alternate symbol */
                sEmitterSymbolDesc.SarrDispEmitterSymbolDesc[usSymbolIndex].ucEmitterAlternatSymbolCode = 1;
                /* Count on every successful read for validation */
                usSymbolCount++;
            }
        }
        if(sRetVal == DP_ASPJ_SUCCESS)
        {
            /* Update max index of the symbol + 1 as symbol count - since it is an index +1 is needed for count */
            sEmitterSymbolDesc.usSymbolCount = usMaxSymbolIndex + 1;
            /* Update the symbol details to the structure passed inside the function */
            memcpy(pS_EmitterSymbolLibrary, &sEmitterSymbolDesc, sizeof(sEmitterSymbolDesc));
        }
        break;
    default:
        sRetVal = DP_ASPJ_FAILURE;
        break;
    }
    if(fptr != NULL)
    {
        fclose(fptr);
    }
    return sRetVal;
}
short dp_rwr_flight_display::DP_RWR_LoadSpecialSymbolDetails(S_EMITTER_SYMBOL_LIBRARY *pS_EmitterSymbolLibrary)
{
    unsigned short usSpecialSymbolCodes = 0, usSymbolCodeCount = 0;
    unsigned short arr_us_SpecialSymbolCodes[DP_RWR_MAX_NUM_OF_SPECIAL_SYMBOLS] = {DP_RWR_SPECIAL_SYMBOLS_CODE_1, DP_RWR_SPECIAL_SYMBOLS_CODE_2, DP_RWR_SPECIAL_SYMBOLS_CODE_3, DP_RWR_SPECIAL_SYMBOLS_CODE_4,
                                                                                   DP_RWR_SPECIAL_SYMBOLS_CODE_5, DP_RWR_SPECIAL_SYMBOLS_CODE_6, DP_RWR_SPECIAL_SYMBOLS_CODE_7, DP_RWR_SPECIAL_SYMBOLS_CODE_8,
                                                                                   DP_RWR_SPECIAL_SYMBOLS_CODE_9, DP_RWR_SPECIAL_SYMBOLS_CODE_10, DP_RWR_SPECIAL_SYMBOLS_CODE_11 , DP_RWR_SPECIAL_SYMBOLS_CODE_12,
                                                                                   DP_RWR_SPECIAL_SYMBOLS_CODE_13, DP_RWR_SPECIAL_SYMBOLS_CODE_14, DP_RWR_SPECIAL_SYMBOLS_CODE_15, DP_RWR_SPECIAL_SYMBOLS_CODE_16,
                                                                                   DP_RWR_SPECIAL_SYMBOLS_CODE_17, DP_RWR_SPECIAL_SYMBOLS_CODE_18, DP_RWR_SPECIAL_SYMBOLS_CODE_19, DP_RWR_SPECIAL_SYMBOLS_CODE_20,
                                                                                   DP_RWR_SPECIAL_SYMBOLS_CODE_21};
    /* Copy the special symbol codes globally */
    memcpy(g_SDP_ASPJ_Handle.m_arrusSpecialSymbolCodes, arr_us_SpecialSymbolCodes, sizeof(g_SDP_ASPJ_Handle.m_arrusSpecialSymbolCodes));
    if(pS_EmitterSymbolLibrary == NULL)
    {
        return -1;
    }
#ifdef DP_RWR_UNKNOWN_SYMBOL_CODE_0
    pS_EmitterSymbolLibrary->SarrDispEmitterSymbolDesc[usSymbolCodeCount].ucEmitterSymbolCode = DP_RWR_THREAT_UNKNOWN_SYMBOL_CODE;
    pS_EmitterSymbolLibrary->SarrDispEmitterSymbolDesc[usSymbolCodeCount].ucEmitterAlternatSymbolCode = 1;
    _snprintf((char *)pS_EmitterSymbolLibrary->SarrDispEmitterSymbolDesc[usSymbolCodeCount].arrcEmitterSymbolDesc, (DP_RWR_EMITTER_SYMBOL_DESC_STR_LEN - 1), "%s" ,"Unknown");
    _snprintf((char *)pS_EmitterSymbolLibrary->SarrDispEmitterSymbolDesc[usSymbolCodeCount].arrcEmitterSymbol, (DP_RWR_EMITTER_SYMBOL_NAME_LEN - 1), "%s" ,"U");
    usSymbolCodeCount++;
#endif
    for(usSpecialSymbolCodes = 0; usSpecialSymbolCodes < DP_RWR_MAX_NUM_OF_SPECIAL_SYMBOLS; usSpecialSymbolCodes++)
    {
        _snprintf((char *)pS_EmitterSymbolLibrary->SarrDispEmitterSymbolDesc[usSymbolCodeCount].arrcEmitterSymbolDesc, (DP_RWR_EMITTER_SYMBOL_DESC_STR_LEN - 1), "Symbol-%d" ,(usSpecialSymbolCodes+1));
        _snprintf((char *)pS_EmitterSymbolLibrary->SarrDispEmitterSymbolDesc[usSymbolCodeCount].arrcEmitterSymbol, (DP_RWR_EMITTER_SYMBOL_NAME_LEN - 1), "%s" ,"");
        pS_EmitterSymbolLibrary->SarrDispEmitterSymbolDesc[usSymbolCodeCount].ucEmitterSymbolCode = (unsigned char)arr_us_SpecialSymbolCodes[usSpecialSymbolCodes];
        pS_EmitterSymbolLibrary->SarrDispEmitterSymbolDesc[usSymbolCodeCount].ucEmitterAlternatSymbolCode = 1;
        usSymbolCodeCount++;
    }
    pS_EmitterSymbolLibrary->usSymbolCount = usSymbolCodeCount;
    return DP_ASPJ_SUCCESS;
}
