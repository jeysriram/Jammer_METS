#include "dp_rwr_flight_display.h"
#include "ui_dp_rwr_flight_display.h"

/*************************************************************************** RWR DISPLAY FUNCTIONS **************************************************************************/

/*****************************************************************************
* Name						:	DP_RWR_Bitmap_AdjustPenWidth
* Author					:	Dhanasekara Pandian G
* Global variables          :	NA
* Created Date				:	Jan 20, 2023
* Revision Date				:	NA
* Reason for Revising		:	NA
* Description				:   This function is used to avoid spacing between
                                painting by adjusting the pen width based on
                                the size of the boundary.
*******************************************************************************/

void dp_rwr_flight_display::DP_RWR_Bitmap_AdjustPenWidth(float *out_fPenWidth)
{
    int iBoundaryHeight = ui->obj_grp_bx_FlightDispArea->geometry().height();
    int iBoundaryWidth = ui->obj_grp_bx_FlightDispArea->geometry().width();

    if(iBoundaryWidth > iBoundaryHeight)
    {
        *out_fPenWidth = ((*out_fPenWidth) / DP_QT_EMITTER_REPORT_FIXED_BOUNDARY_WIDTH) * iBoundaryHeight;
    }
    else
    {
        *out_fPenWidth = ((*out_fPenWidth) / DP_QT_EMITTER_REPORT_FIXED_BOUNDARY_HEIGHT) * iBoundaryWidth;
    }
}

/*****************************************************************************
* Name						:	DP_RWR_Bitmap_DrawCircle
* Author					:	Dhanasekara Pandian G
* Global variables          :	NA
* Created Date				:	Jan 20, 2023
* Revision Date				:	NA
* Reason for Revising		:	NA
* Description				:   This function is used to draw a circle.
*******************************************************************************/

void dp_rwr_flight_display::DP_RWR_Bitmap_DrawAdjustingCircle(QPoint in_qpCenter, int in_iRadius, int in_iR, int in_iG, int in_iB, float in_fPenWidth)
{
    /********** Standard QPainter function to avoid Staircase effect on Lines **********/
    m_obj_paint_line->setRenderHints(QPainter::Antialiasing, true);

    /********** Setting up color and size of the pen **********/
    cMyColor.setRgb(in_iR, in_iG, in_iB, DP_QT_EMITTER_REPORT_DISPLAY_BGCOLOR_ALPHA);
    cPen.setColor(cMyColor);
    cPen.setCapStyle(Qt::FlatCap);

    /********** Adjusting and setting up the pen size based on the boundary width and height **********/
    DP_RWR_Bitmap_AdjustPenWidth(&in_fPenWidth);
    cPen.setWidthF(in_fPenWidth);

    /********** Setting up the pen to draw **********/
    m_obj_paint_line->setPen(cPen);

    /********** Drawing the circle **********/
    m_obj_paint_line->drawEllipse(in_qpCenter, in_iRadius, in_iRadius);
}

/*****************************************************************************
* Name						:	DP_RWR_Bitmap_FillCircle
* Author					:	Dhanasekara Pandian G
* Parameters            	:	NA
* Created Date				:	Jan 20, 2023
* Revision Date				:	NA
* Reason for Revising		:	NA
* Description				:   This function is used to draw a filled circle.
*******************************************************************************/

void dp_rwr_flight_display::DP_RWR_Bitmap_FillCircle(QPoint in_qpCenter, int in_iRadius, int in_iR, int in_iG, int in_iB, float in_fPenWidth)
{
    /********** Standard QPainter function to avoid Staircase effect on Lines **********/
    m_obj_paint_line->setRenderHints(QPainter::Antialiasing, true);

    /********** Setting up color and size of the pen **********/
    cMyColor.setRgb(in_iR, in_iG, in_iB, DP_QT_EMITTER_REPORT_DISPLAY_BGCOLOR_ALPHA);
    cPen.setColor(cMyColor);
    cPen.setCapStyle(Qt::FlatCap);

    /********** Adjusting and setting up the pen size based on the boundary width and height **********/
    DP_RWR_Bitmap_AdjustPenWidth(&in_fPenWidth);
    cPen.setWidthF(in_fPenWidth);

    /********** Save the painter for restoring it to this position **********/
    m_obj_paint_line->save();

    /********** Setting up the pen to draw **********/
    m_obj_paint_line->setBrush(QBrush(QColor(in_iR, in_iG, in_iB)));

    /********** Drawing the circle **********/
    m_obj_paint_line->drawEllipse(in_qpCenter, in_iRadius, in_iRadius);

    /********** Restore the painter to saved position **********/
    m_obj_paint_line->restore();
}

/*****************************************************************************
* Name						:	DP_RWR_Bitmap_DrawGradientCircle
* Author					:	Dhanasekara Pandian G
* Parameters            	:	NA
* Created Date				:	Jan 20, 2023
* Revision Date				:	NA
* Reason for Revising		:	NA
* Description				:   This function is used to draw a gradient circle.
*******************************************************************************/

void dp_rwr_flight_display::DP_RWR_Bitmap_DrawGradientCircle(float in_fx, float in_fy, float in_fwidth, float in_fheight, float in_fStartAngle, float in_fSpanAngle, int in_iR, int in_iG, int in_iB, float in_fPenWidth)
{
    QConicalGradient cGradient;

    /********** Standard QPainter function to avoid Staircase effect on Lines **********/
    m_obj_paint_line->setRenderHints(QPainter::Antialiasing, true);

    /********** Setting up color, size and style of the pen **********/
    cMyColor.setRgb(in_iR, in_iG, in_iB, DP_QT_EMITTER_REPORT_DISPLAY_BGCOLOR_ALPHA);
    cPen.setColor(cMyColor);
    cPen.setCapStyle(Qt::FlatCap);

    /********** Adjusting and setting up the pen size based on the boundary width and height **********/
    DP_RWR_Bitmap_AdjustPenWidth(&in_fPenWidth);
    cPen.setWidthF(in_fPenWidth);

    /********** Setting opacity for glowing effect **********/
    m_obj_paint_line->setOpacity(DP_QT_EMITTER_REPORT_CIRCLE_OPACITY_VALUE);

    /********** Setting up the pen to draw **********/
    m_obj_paint_line->setPen(cPen);

    /********** Drawing the circle **********/
    QRectF Circle_Rect(in_fx, in_fy, in_fwidth, in_fheight);
    m_obj_paint_line->drawArc(Circle_Rect, in_fStartAngle * DP_QT_EMITTER_REPORT_CIRCLE_START_ANGLE_DEGVALUE, in_fSpanAngle * DP_QT_EMITTER_REPORT_CIRCLE_SPAN_ANGLE_DEGVALUE);
}

/*****************************************************************************
* Name						:	DP_RWR_Bitmap_DrawRoundedRect
* Author					:	Dhanasekara Pandian G
* Parameters            	:	NA
* Created Date				:	Jan 20, 2023
* Revision Date				:	NA
* Reason for Revising		:	NA
* Description				:   This function is used to draw a rounded rectangle.
*******************************************************************************/

void dp_rwr_flight_display::DP_RWR_Bitmap_DrawRoundedRect(float in_fx, float in_fy, float in_fwidth, float in_fheight, int in_iR, int in_iG, int in_iB, float in_fPenWidth)
{
    /********** Standard QPainter function to avoid Staircase effect on Lines **********/
    m_obj_paint_line->setRenderHints(QPainter::Antialiasing, true);

    /********** Setting up color and style of the pen **********/
    cMyColor.setRgb(in_iR, in_iG, in_iB, DP_QT_EMITTER_REPORT_DISPLAY_BGCOLOR_ALPHA);
    cPen.setColor(cMyColor);
    cPen.setCapStyle(Qt::FlatCap);

    /********** Adjusting and setting up the pen size based on the boundary width and height **********/
    DP_RWR_Bitmap_AdjustPenWidth(&in_fPenWidth);
    cPen.setWidthF(in_fPenWidth);

    /********** Setting up the pen to draw **********/
    m_obj_paint_line->setPen(cPen);

    /********** Drawing the Rounded Rectangle **********/
    m_obj_paint_line->drawRoundedRect(in_fx, in_fy, in_fwidth, in_fheight, DP_QT_EMITTER_REPORT_ROUNDED_RECT_X_RADIUS, DP_QT_EMITTER_REPORT_ROUNDED_RECT_Y_RADIUS);
}

/*****************************************************************************
* Name						:	DP_RWR_Bitmap_DrawRect
* Author					:	Dhanasekara Pandian G
* Parameters            	:	NA
* Created Date				:	Jan 20, 2023
* Revision Date				:	NA
* Reason for Revising		:	NA
* Description				:   This function is used to draw a rectangle.
*******************************************************************************/

void dp_rwr_flight_display::DP_RWR_Bitmap_DrawRect(float in_fx, float in_fy, float in_fwidth, float in_fheight, int in_iR, int in_iG, int in_iB, float in_fPenWidth)
{
    /********** Standard QPainter function to avoid Staircase effect on Lines **********/
    m_obj_paint_line->setRenderHints(QPainter::Antialiasing, true);

    /********** Setting up color and style of the pen **********/
    cMyColor.setRgb(in_iR, in_iG, in_iB, DP_QT_EMITTER_REPORT_DISPLAY_BGCOLOR_ALPHA);
    cPen.setColor(cMyColor);
    cPen.setCapStyle(Qt::FlatCap);

    /********** Adjusting and setting up the pen size based on the boundary width and height **********/
    DP_RWR_Bitmap_AdjustPenWidth(&in_fPenWidth);
    cPen.setWidthF(in_fPenWidth);

    /********** Setting up the pen to draw **********/
    m_obj_paint_line->setPen(cPen);

    /********** Drawing the Rectangle **********/
    m_obj_paint_line->drawRect(in_fx, in_fy, in_fwidth, in_fheight);
}

/*****************************************************************************
* Name						:	DP_RWR_Bitmap_DrawFlightModel
* Author					:	Dhanasekara Pandian G
* Parameters            	:	NA
* Created Date				:	Jan 20, 2023
* Revision Date				:	NA
* Reason for Revising		:	NA
* Description				:   This function is used to draw a flight model.
*******************************************************************************/

void dp_rwr_flight_display::DP_RWR_Bitmap_DrawFlightModel(float in_fStartXPos, float in_fStartYPos, int in_iR, int in_iG, int in_iB, float in_fPenwidth)
{
    /********** Standard QPainter function to avoid Staircase effect on Lines **********/
    m_obj_paint_line->setRenderHints(QPainter::Antialiasing, true);

    float iFlightModelWidth = DP_QT_EMITTER_REPORT_INITIALIZE;
    float iFlightModelHeight = DP_QT_EMITTER_REPORT_INITIALIZE;

    /************** This is done to make sure that flight rect display is always square type **************/
    if(ui->obj_grp_bx_FlightDispArea->height() > ui->obj_grp_bx_FlightDispArea->width())
    {
        iFlightModelWidth = (DP_QT_EMITTER_REPORT_FLIGHT_MODEL_WIDTH / 311.0f) * ui->obj_grp_bx_FlightDispArea->width();
        iFlightModelHeight = iFlightModelWidth * 2;
    }
    else
    {
        iFlightModelHeight = (DP_QT_EMITTER_REPORT_FLIGHT_MODEL_HEIGHT / 311.0f) * ui->obj_grp_bx_FlightDispArea->height();
        iFlightModelWidth = iFlightModelHeight / 2;
    }

    /************** Storing flight starting x and y coordinates **************/
    float fCenterPosX = (float)(in_fStartXPos);
    float fCenterPosY = (float)(in_fStartYPos) - (iFlightModelHeight / 1.15f);

    /********** Setting up color and style of the pen **********/
    cMyColor.setRgb(in_iR, in_iG, in_iB, DP_QT_EMITTER_REPORT_DISPLAY_BGCOLOR_ALPHA);
    cPen.setColor(cMyColor);
    cPen.setCapStyle(Qt::FlatCap);

    /********** Adjusting and setting up the pen size based on the boundary width and height **********/
    DP_RWR_Bitmap_AdjustPenWidth(&in_fPenwidth);
    cPen.setWidthF(in_fPenwidth);

    /********** Setting up the pen to draw **********/
    m_obj_paint_line->setPen(cPen);

    /********** Left half of the flight **********/
    /********** Top Point **********/
    QPointF Point1(fCenterPosX, fCenterPosY);
    QPointF Point2(fCenterPosX - (iFlightModelWidth * 0.33f),  (fCenterPosY + (iFlightModelHeight * 0.25f)));

    /********** Straight Front Body Line End below the flight's front end **********/
    QPointF Point3(fCenterPosX - (iFlightModelWidth * 0.33f),  (fCenterPosY + (iFlightModelHeight * 0.25f)));
    QPointF Point4(fCenterPosX - (iFlightModelWidth * 0.33f),  (fCenterPosY + (iFlightModelHeight * 0.5f)));

    /********** Flight's Front Wing Width End Point **********/
    QPointF Point5(fCenterPosX - (iFlightModelWidth * 0.33f),  (fCenterPosY + (iFlightModelHeight * 0.5f)));
    QPointF Point6(fCenterPosX - (iFlightModelWidth * 1.33f), (fCenterPosY + (iFlightModelHeight * 0.84f)));

    /********** Flight's Front Wing Length Point **********/
    QPointF Point7(fCenterPosX - (iFlightModelWidth * 1.33f), (fCenterPosY + (iFlightModelHeight * 0.84f)));
    QPointF Point8(fCenterPosX - (iFlightModelWidth * 1.33f), (fCenterPosY + (iFlightModelHeight * 1.0f)));

    /********** Flight's Font Close Wing Length Point **********/
    QPointF Point9(fCenterPosX - (iFlightModelWidth * 1.33f), (fCenterPosY + (iFlightModelHeight * 1.0f)));
    QPointF Point10(fCenterPosX - (iFlightModelWidth * 0.33f),  (fCenterPosY + (iFlightModelHeight * 0.85f)));

    /********** Straight Rear Body Line End below the flight's wings **********/
    QPointF Point11(fCenterPosX - (iFlightModelWidth * 0.33f),  (fCenterPosY + (iFlightModelHeight * 0.85f)));
    QPointF Point12(fCenterPosX - (iFlightModelWidth * 0.17f),  (fCenterPosY + (iFlightModelHeight * 1.33f)));

    /********** Front Tail Line Width **********/
    QPointF Point13(fCenterPosX - (iFlightModelWidth * 0.17f),  (fCenterPosY + (iFlightModelHeight * 1.33f)));
    QPointF Point14(fCenterPosX - (iFlightModelWidth * 0.83f),  (fCenterPosY + (iFlightModelHeight * 1.5f)));

    /********** Front Tail Line Length **********/
    QPointF Point15(fCenterPosX - (iFlightModelWidth * 0.83f),  (fCenterPosY + (iFlightModelHeight * 1.5f)));
    QPointF Point16(fCenterPosX - (iFlightModelWidth * 0.83f),  (fCenterPosY + (iFlightModelHeight * 1.65f)));

    /********** Rear Tail Line Width **********/
    QPointF Point17(fCenterPosX - (iFlightModelWidth * 0.83f),  (fCenterPosY + (iFlightModelHeight * 1.65f)));
    QPointF Point18(fCenterPosX - (iFlightModelWidth * 0.17f),  (fCenterPosY + (iFlightModelHeight * 1.58f)));

    /********** Rear Tail Triangle **********/
    QPointF Point19(fCenterPosX - (iFlightModelWidth * 0.17f),  (fCenterPosY + (iFlightModelHeight * 1.58f)));
    QPointF Point20(fCenterPosX,  (fCenterPosY + (iFlightModelHeight * 1.67f)));

    /********** Storing the points to draw right half **********/
    QPointF Points[20] = {Point1, Point2, Point3, Point4, Point5, Point6, Point7, Point8, Point9, Point10, Point11, Point12, Point13, Point14, Point15, Point16, Point17, Point18, Point19, Point20};

    /********** Right half of the flight **********/
    /********** Top Point **********/
    QPointF Point21(fCenterPosX, fCenterPosY);
    QPointF Point22(fCenterPosX + (iFlightModelWidth * 0.33f),  (fCenterPosY + (iFlightModelHeight * 0.25f)));

    /********** Straight Front Body Line End below the flight's front end **********/
    QPointF Point23(fCenterPosX + (iFlightModelWidth * 0.33f),  (fCenterPosY + (iFlightModelHeight * 0.25f)));
    QPointF Point24(fCenterPosX + (iFlightModelWidth * 0.33f),  (fCenterPosY + (iFlightModelHeight * 0.5f)));

    /********** Flight's Front Wing Width End Point **********/
    QPointF Point25(fCenterPosX + (iFlightModelWidth * 0.33f),  (fCenterPosY + (iFlightModelHeight * 0.5f)));
    QPointF Point26(fCenterPosX + (iFlightModelWidth * 1.33f), (fCenterPosY + (iFlightModelHeight * 0.84f)));

    /********** Flight's Front Wing Length Point **********/
    QPointF Point27(fCenterPosX + (iFlightModelWidth * 1.33f), (fCenterPosY + (iFlightModelHeight * 0.84f)));
    QPointF Point28(fCenterPosX + (iFlightModelWidth * 1.33f), (fCenterPosY + (iFlightModelHeight * 1.0f)));

    /********** Flight's Font Close Wing Length Point **********/
    QPointF Point29(fCenterPosX + (iFlightModelWidth * 1.33f), (fCenterPosY + (iFlightModelHeight * 1.0f)));
    QPointF Point30(fCenterPosX + (iFlightModelWidth * 0.33f),  (fCenterPosY + (iFlightModelHeight * 0.85f)));

    /********** Straight Rear Body Line End below the flight's wings **********/
    QPointF Point31(fCenterPosX + (iFlightModelWidth * 0.33f),  (fCenterPosY + (iFlightModelHeight * 0.85f)));
    QPointF Point32(fCenterPosX + (iFlightModelWidth * 0.17f),  (fCenterPosY + (iFlightModelHeight * 1.33f)));

    /********** Front Tail Line Width **********/
    QPointF Point33(fCenterPosX + (iFlightModelWidth * 0.17f),  (fCenterPosY + (iFlightModelHeight * 1.33f)));
    QPointF Point34(fCenterPosX + (iFlightModelWidth * 0.83f),  (fCenterPosY + (iFlightModelHeight * 1.5f)));

    /********** Front Tail Line Length **********/
    QPointF Point35(fCenterPosX + (iFlightModelWidth * 0.83f),  (fCenterPosY + (iFlightModelHeight * 1.5f)));
    QPointF Point36(fCenterPosX + (iFlightModelWidth * 0.83f),  (fCenterPosY + (iFlightModelHeight * 1.65f)));

    /********** Rear Tail Line Width **********/
    QPointF Point37(fCenterPosX + (iFlightModelWidth * 0.83f),  (fCenterPosY + (iFlightModelHeight * 1.65f)));
    QPointF Point38(fCenterPosX + (iFlightModelWidth * 0.17f),  (fCenterPosY + (iFlightModelHeight * 1.58f)));

    /********** Rear Tail Triangle **********/
    QPointF Point39(fCenterPosX + (iFlightModelWidth * 0.17f),  (fCenterPosY + (iFlightModelHeight * 1.58f)));
    QPointF Point40(fCenterPosX,  (fCenterPosY + (iFlightModelHeight * 1.67f)));

    /********** Storing the points to draw left half **********/
    QPointF Points2[20] = {Point21, Point22, Point23, Point24, Point25, Point26, Point27, Point28, Point29, Point30, Point31, Point32, Point33, Point34, Point35, Point36, Point37, Point38, Point39, Point40};

    /********** Drawing right and left half of the flight **********/
    m_obj_paint_line->drawLines(Points, 10);
    m_obj_paint_line->drawLines(Points2, 10);
}

/*********************************************************************** END OF RWR DISPLAY FUNCTIONS ***********************************************************************/
