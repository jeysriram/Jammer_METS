
#include "dp_aspj_comm.h"
#include "ui_dp_rwr_polarplot_power_vs_doa.h"

#include <QMessageBox>
dp_rwr_polarplot_power_vs_doa::dp_rwr_polarplot_power_vs_doa(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::dp_rwr_polarplot_power_vs_doa)
{
    qSetMessagePattern("[%{time yyyy-MM-dd h:mm:ss.zzz}] [%{type}] [%{file}:%{line}] %{message}");

    ui->setupUi(this);

    /* Setting up the output window title */
    setWindowTitle("Polar Plot");

    DP_RWR_PolarPlotOnInitial();
}

dp_rwr_polarplot_power_vs_doa::~dp_rwr_polarplot_power_vs_doa()
{  
    delete ui;
}

void dp_rwr_polarplot_power_vs_doa::SetPolarRange(float in_fMinFrequency, float in_fMaxFrequency)
{
    m_fPolar_StartAmp = in_fMinFrequency;
    m_fPolar_StopAmp = in_fMaxFrequency;
}

void dp_rwr_polarplot_power_vs_doa::DP_RWR_PolarPlotOnInitial()
{
    /* Initialising member variables */
    m_iCurrent_Degree = 0;
    m_iCount          = 360;
    m_iPrev_Degree    = 0;
    m_iDegree         = 0;
    m_iNiddleDegree   = 270;
    m_iDiff           = 0;
    m_fFreq           = 500.0f;
    m_fAngle          = 90.0f;
    m_iLimitVal       = 0;
    m_iCurrentStopDeg = 0;
    m_iCurrentTextStopDeg = 0;

    g_objSymbolDraw = new dp_symbol_draw_polar_plot();

    /* Setting default values to polar plot simulating controls */
    ui->obj_ln_edt_StartFreq->setText(QString::number(DP_POLAR_MIN_FREQUENCY));
    ui->obj_ln_edt_StopFreq->setText(QString::number(DP_POLAR_MAX_FREQUENCY));
    ui->obj_ln_edt_Limit->setText(QString::number(3));
    ui->obj_ln_edt_StartAngle->setText(QString::number(DP_POLAR_MIN_ANGLE));
    ui->obj_ln_edt_StopAngle->setText(QString::number(DP_POLAR_MAX_ANGLE));

    /* For circle rotation connecting to corresponding circle rotation function(Timer Slot) */
    m_obj_tmr_CircleRotation = new QTimer(this);
    connect(m_obj_tmr_CircleRotation, SIGNAL(timeout()), this, SLOT(DP_POLAR_PLOT_SLOT_UpdateValueForCircleRotation()));

    /* Connecting for paintevent function to be called every 150 milliseconds */
    m_obj_tmr_PaintEvent = new QTimer(this);
    connect(m_obj_tmr_PaintEvent, SIGNAL(timeout()), this, SLOT(DP_POLAR_PLOT_SLOT_CallPaintEventFunc()));
    m_obj_tmr_PaintEvent->start(150);

    /* Hide the polar plot simulating groupbox */
    ui->obj_grp_bx_PolarPlotConfig->setVisible(false);

    memset(&mS_ThreatList, 0, sizeof (S_RWR_THREAT_LIST));
}

void dp_rwr_polarplot_power_vs_doa::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    
    m_obj_paint_line = new QPainter(this);

    /* Finding the display area to draw the polar plot */
    DP_RWR_PolarPlotFindDisplayArea();

    /* Drawing the static screen of the polar plot */
    DP_RWR_PolarPlotStaticScreen();

    /* Drawing the dynamic screen of the polar plot */
    DP_RWR_PolarPlotDynamicScreen();

    delete m_obj_paint_line;
}

void dp_rwr_polarplot_power_vs_doa::DP_POLAR_PLOT_SLOT_CallPaintEventFunc()
{
    update();
}

void dp_rwr_polarplot_power_vs_doa::DP_RWR_PolarPlotFindDisplayArea()
{
    int iLeft = 0, iRight  = 0;
    int iTop  = 0, iBottom = 0;

    /* Storing sides positions of all sides of groupbox */
    iLeft   = ui->obj_grp_bx_PolarDispArea->geometry().left() ;
    iRight  = ui->obj_grp_bx_PolarDispArea->geometry().right();
    iTop    = ui->obj_grp_bx_PolarDispArea->geometry().top();
    iBottom = ui->obj_grp_bx_PolarDispArea->geometry().bottom();

    /* Setting those values in rect to fill the background */
    s_PolarPlot_Display.DispArea.setLeft(iLeft);
    s_PolarPlot_Display.DispArea.setRight(iRight);
    s_PolarPlot_Display.DispArea.setTop(iTop);
    s_PolarPlot_Display.DispArea.setBottom(iBottom);

    /* Storing positions for updating(redraw) screen(groupbox) */
    s_PolarPlot_Display.m_iBitmapStartXPos = s_PolarPlot_Display.DispArea.left() + DP_POLAR_PLOT_BOUNDARY_X;
    s_PolarPlot_Display.m_iBitmapStartYPos = s_PolarPlot_Display.DispArea.top() + DP_POLAR_PLOT_BOUNDARY_Y;
    s_PolarPlot_Display.m_iBitmapWidth     = (int)((s_PolarPlot_Display.DispArea.right() - s_PolarPlot_Display.DispArea.left()) - (DP_POLAR_PLOT_BOUNDARY_X * 1.5));
    s_PolarPlot_Display.m_iBitmapHeight    = (int)((s_PolarPlot_Display.DispArea.bottom() - s_PolarPlot_Display.DispArea.top()) - (DP_POLAR_PLOT_BOUNDARY_Y * 1.5));

    s_PolarPlot_Display.m_iDispStartXPos   =  ui->obj_grp_bx_PolarDispArea->x();
    s_PolarPlot_Display.m_iDispStartYPos   =  ui->obj_grp_bx_PolarDispArea->y();
    s_PolarPlot_Display.m_iDispWidth       = (int)(ui->obj_grp_bx_PolarDispArea->width());
    s_PolarPlot_Display.m_iDispHeight      = (int)(ui->obj_grp_bx_PolarDispArea->height());

    /* This is done to make sure the display is always square type */
    if(s_PolarPlot_Display.m_iDispWidth > s_PolarPlot_Display.m_iDispHeight)
    {
        s_PolarPlot_Display.m_iDispStartXPos = ui->obj_grp_bx_PolarDispArea->x() + ((s_PolarPlot_Display.m_iBitmapWidth / 2.15f) - (s_PolarPlot_Display.m_iBitmapHeight / 2.2f));
        /* This logic is to adjust the square type display in the centre of the group box vertically */
        s_PolarPlot_Display.m_iDispWidth     = s_PolarPlot_Display.m_iDispHeight;
    }

    if(s_PolarPlot_Display.m_iDispHeight > s_PolarPlot_Display.m_iDispWidth)
    {
        s_PolarPlot_Display.m_iDispStartYPos = (ui->obj_grp_bx_PolarDispArea->y()) - ((s_PolarPlot_Display.m_iBitmapWidth / 2.2f) - (s_PolarPlot_Display.m_iBitmapHeight / 2.15f));
        /* This logic is to adjust the square type display in the centre of the group box horizontally */
        s_PolarPlot_Display.m_iDispHeight    = s_PolarPlot_Display.m_iDispWidth;
    }
}

void dp_rwr_polarplot_power_vs_doa::DP_RWR_PolarPlotStaticScreen()
{
    float fScreenStartPos_X    = 0, fScreenStartPos_Y     = 0;
    float fScreenBoundaryWidth = 0, fScreenBoundaryHeight = 0;
    double dLineAngle          = 0;
    int iAngleLines_RefX       = 0;
    int iAngleLines_RefY       = 0;
    int m_iCount               = 0;
    char arr_cDegree[DP_TOTAL_DEGREE_VALUE][4] = {"90", "120", "150", "180", "210", "240", "270", "300", "330", "0", "30", "60"};
    QString sTemp = "";
    QColor obj_clr_Line, obj_clr_Background;
    float fText = 0.0f, fTempText = 0.0f;

    obj_clr_Line.setRgb(DP_POLAR_PLOT_WHITE_COLOR_CODE);

    /* Standard QPainter function to avoid staircase effect on lines */
    m_obj_paint_line->setRenderHints(QPainter::Antialiasing, true);

    /* Storing the values of the rect boundary */
    fScreenStartPos_X     = (float)s_PolarPlot_Display.m_iDispStartXPos;
    fScreenStartPos_Y     = (float)s_PolarPlot_Display.m_iDispStartYPos;
    fScreenBoundaryWidth  = (float)s_PolarPlot_Display.m_iDispWidth;
    fScreenBoundaryHeight = (float)s_PolarPlot_Display.m_iDispHeight;

    /* Filling BG color for rect boundary */
    obj_clr_Background.setRgb(DP_POLAR_PLOT_BLACK_COLOR_CODE);
    m_obj_paint_line->fillRect(s_PolarPlot_Display.DispArea, obj_clr_Background);

    /* To draw circle */
    DP_RWR_PolarPlotDrawCircle(fScreenStartPos_X + (fScreenBoundaryWidth / 6), fScreenStartPos_Y + (fScreenBoundaryHeight / 6), fScreenBoundaryWidth - (fScreenBoundaryWidth / 3), fScreenBoundaryHeight - (fScreenBoundaryHeight / 3), DP_POLAR_PLOT_WHITE_COLOR_CODE, 2, 0, DP_CIRCLE_360_DEGREE);
    DP_RWR_PolarPlotDrawCircle(fScreenStartPos_X + (fScreenBoundaryWidth / 4.13f), fScreenStartPos_Y + (fScreenBoundaryHeight / 4.13f), fScreenBoundaryWidth - (fScreenBoundaryWidth / 2.06f ), fScreenBoundaryHeight - (fScreenBoundaryHeight / 2.06f), DP_POLAR_PLOT_WHITE_COLOR_CODE, 2, 0, DP_CIRCLE_360_DEGREE);
    DP_RWR_PolarPlotDrawCircle(fScreenStartPos_X + (fScreenBoundaryWidth / 3.15f), fScreenStartPos_Y + (fScreenBoundaryHeight / 3.15f), fScreenBoundaryWidth - (fScreenBoundaryWidth / 1.57f), fScreenBoundaryHeight - (fScreenBoundaryHeight / 1.57f), DP_POLAR_PLOT_WHITE_COLOR_CODE, 2, 0, DP_CIRCLE_360_DEGREE);
    DP_RWR_PolarPlotDrawCircle(fScreenStartPos_X + (fScreenBoundaryWidth / 2.55f), fScreenStartPos_Y + (fScreenBoundaryHeight / 2.55f), fScreenBoundaryWidth - (fScreenBoundaryWidth / 1.27f), fScreenBoundaryHeight - (fScreenBoundaryHeight / 1.27f), DP_POLAR_PLOT_WHITE_COLOR_CODE, 2, 0, DP_CIRCLE_360_DEGREE);
    DP_RWR_PolarPlotDrawCircle(fScreenStartPos_X + (fScreenBoundaryWidth / 2.14f), fScreenStartPos_Y + (fScreenBoundaryHeight / 2.14f), fScreenBoundaryWidth - (fScreenBoundaryWidth / 1.07f), fScreenBoundaryHeight - (fScreenBoundaryHeight / 1.07f), DP_POLAR_PLOT_WHITE_COLOR_CODE, 2, 0, DP_CIRCLE_360_DEGREE);
#if 0
    /****** Drawing Indicating Symbols ******/
    obj_clr_Line.setRgb(DP_POLAR_PLOT_GREEN_COLOR_CODE);

    DP_RWR_PolarPlotDrawLine((fScreenStartPos_X + fScreenBoundaryWidth - 30), (fScreenStartPos_Y + 10), (fScreenStartPos_X + fScreenBoundaryWidth - 37), (fScreenStartPos_Y + 17), obj_clr_Line, 2);
    DP_RWR_PolarPlotDrawLine((fScreenStartPos_X + fScreenBoundaryWidth - 37), (fScreenStartPos_Y + 17), (fScreenStartPos_X + fScreenBoundaryWidth - 24), (fScreenStartPos_Y + 17), obj_clr_Line, 2);
    DP_RWR_PolarPlotDrawLine((fScreenStartPos_X + fScreenBoundaryWidth - 24), (fScreenStartPos_Y + 17), (fScreenStartPos_X + fScreenBoundaryWidth - 30), (fScreenStartPos_Y + 10), obj_clr_Line, 2);

    DP_RWR_PolarPlotDrawCircle((fScreenStartPos_X + fScreenBoundaryWidth - 32), (fScreenStartPos_Y + 25), 4, 4, DP_POLAR_PLOT_RED_COLOR_CODE, 4, 0, 360 * 16);
    DP_RWR_PolarPlotDrawText((fScreenStartPos_X + fScreenBoundaryWidth - 23), (fScreenStartPos_Y + 14), "Pulse", DP_POLAR_PLOT_FONT, DP_POLAR_PLOT_WHITE_COLOR_CODE);
    DP_RWR_PolarPlotDrawText((fScreenStartPos_X + fScreenBoundaryWidth - 23), (fScreenStartPos_Y + 28), "CW", DP_POLAR_PLOT_FONT, DP_POLAR_PLOT_WHITE_COLOR_CODE);
#endif
    iAngleLines_RefX = (int)((fScreenBoundaryWidth / 2) + fScreenStartPos_X);
    iAngleLines_RefY = (int)((fScreenBoundaryWidth / 2) + fScreenStartPos_Y);

    /* Angle lines with Interval of 10 degrees */
    obj_clr_Line.setRgb(DP_POLAR_PLOT_WHITE_COLOR_CODE);
    for(unsigned long ulCircleInLine_Angles = 0; ulCircleInLine_Angles < DP_ANGLE_OF_360_DEGREE; ulCircleInLine_Angles += 10)
    {
        dLineAngle = (double)ulCircleInLine_Angles * 1.0f;
        dLineAngle = DP_RWR_DEG_2_RAD(dLineAngle);

        if((ulCircleInLine_Angles % 10) == 0)
        {
            DP_RWR_PolarPlotDrawLine((int)(iAngleLines_RefX + ((fScreenBoundaryWidth / 3.0f) * cos(dLineAngle))),
                                     (int)(iAngleLines_RefY + ((fScreenBoundaryWidth / 3.0f) * sin(dLineAngle))),
                                     (int)(iAngleLines_RefX + ((fScreenBoundaryHeight / 2.85f) * cos(dLineAngle))),
                                     (int)(iAngleLines_RefY + ((fScreenBoundaryHeight / 2.85f) * sin(dLineAngle))), obj_clr_Line, 2);
        }

        if((ulCircleInLine_Angles % 30) == 0)
        {
            sTemp = arr_cDegree[m_iCount];
            if(ulCircleInLine_Angles < DP_ANGLE_OF_90_DEGREE)
            {
                DP_RWR_PolarPlotDrawText((iAngleLines_RefX + ((fScreenBoundaryWidth / 2.7f) * cos(dLineAngle))), ((iAngleLines_RefY ) +  ((fScreenBoundaryWidth / 2.7f) * sin(dLineAngle))), sTemp, DP_POLAR_PLOT_FONT, DP_POLAR_PLOT_WHITE_COLOR_CODE);
            }
            else if((ulCircleInLine_Angles >= DP_ANGLE_OF_90_DEGREE) && (ulCircleInLine_Angles <= DP_ANGLE_OF_180_DEGREE))
            {
                DP_RWR_PolarPlotDrawText(((iAngleLines_RefX) + ((fScreenBoundaryWidth / 2.5f) * cos(dLineAngle))), ((iAngleLines_RefY ) +  ((fScreenBoundaryWidth / 2.7f) * sin(dLineAngle))), sTemp, DP_POLAR_PLOT_FONT, DP_POLAR_PLOT_WHITE_COLOR_CODE);
            }
            else if((ulCircleInLine_Angles >= DP_ANGLE_OF_180_DEGREE) && (ulCircleInLine_Angles < DP_ANGLE_OF_270_DEGREE))
            {
                DP_RWR_PolarPlotDrawText(((iAngleLines_RefX) + ((fScreenBoundaryWidth / 2.7f) * cos(dLineAngle))), ((iAngleLines_RefY ) +  ((fScreenBoundaryWidth / 2.7f) * sin(dLineAngle))), sTemp, DP_POLAR_PLOT_FONT, DP_POLAR_PLOT_WHITE_COLOR_CODE);
            }
            else if((ulCircleInLine_Angles >= DP_ANGLE_OF_270_DEGREE) && (ulCircleInLine_Angles < DP_ANGLE_OF_360_DEGREE))
            {
                DP_RWR_PolarPlotDrawText(((iAngleLines_RefX) + ((fScreenBoundaryWidth / 2.7f) * cos(dLineAngle))), ((iAngleLines_RefY ) +  ((fScreenBoundaryWidth / 2.7f) * sin(dLineAngle))), sTemp, DP_POLAR_PLOT_FONT, DP_POLAR_PLOT_WHITE_COLOR_CODE);
            }

            m_iCount++;
        }
    }
#if 0
    DP_RWR_PolarPlotDrawLine((s_PolarPlot_Display.m_iDispWidth / 2) + (fScreenStartPos_X) - 1, (s_PolarPlot_Display.m_iDispHeight / 2)  + (fScreenStartPos_Y) - 8, (s_PolarPlot_Display.m_iDispWidth / 2)  + (fScreenStartPos_X) - 1, (s_PolarPlot_Display.m_iDispHeight / 2)  + (fScreenStartPos_Y) - 14, obj_clr_Line, 2);
    DP_RWR_PolarPlotDrawLine((s_PolarPlot_Display.m_iDispWidth / 2) + (fScreenStartPos_X) - 1, (s_PolarPlot_Display.m_iDispHeight / 2)  + (fScreenStartPos_Y) - 32, (s_PolarPlot_Display.m_iDispWidth / 2)  + (fScreenStartPos_X) - 1, (s_PolarPlot_Display.m_iDispHeight / 2) + (fScreenStartPos_Y) - 37, obj_clr_Line, 2);
    DP_RWR_PolarPlotDrawLine((s_PolarPlot_Display.m_iDispWidth / 2) + (fScreenStartPos_X) - 1, (s_PolarPlot_Display.m_iDispHeight / 2)  + (fScreenStartPos_Y) - 56, (s_PolarPlot_Display.m_iDispWidth / 2)  + (fScreenStartPos_X) - 1, (s_PolarPlot_Display.m_iDispHeight / 2) + (fScreenStartPos_Y) - 60, obj_clr_Line, 2);
    DP_RWR_PolarPlotDrawLine((s_PolarPlot_Display.m_iDispWidth / 2) + (fScreenStartPos_X) - 1, (s_PolarPlot_Display.m_iDispHeight / 2)  + (fScreenStartPos_Y) - 77, (s_PolarPlot_Display.m_iDispWidth / 2)  + (fScreenStartPos_X) - 1, (s_PolarPlot_Display.m_iDispHeight / 2) + (fScreenStartPos_Y) - 81, obj_clr_Line, 2);
    DP_RWR_PolarPlotDrawLine((s_PolarPlot_Display.m_iDispWidth / 2) + (fScreenStartPos_X) - 1, (s_PolarPlot_Display.m_iDispHeight / 2)  + (fScreenStartPos_Y) - 100, (s_PolarPlot_Display.m_iDispWidth / 2) + (fScreenStartPos_X) - 1, (s_PolarPlot_Display.m_iDispHeight / 2) + (fScreenStartPos_Y) - 104, obj_clr_Line, 2);
#endif
    fText = (((m_fPolar_StartAmp - m_fPolar_StopAmp) / 19.0f) + m_fPolar_StopAmp);
    DP_RWR_PolarPlotDrawText((s_PolarPlot_Display.m_iDispWidth / 2) + (fScreenStartPos_X) + 2, (fScreenStartPos_Y + (fScreenBoundaryHeight / 2.14f)) - 5, QString::number(fText, 'f', 1) + "dBm", DP_POLAR_PLOT_FONT, DP_POLAR_PLOT_WHITE_COLOR_CODE);

    fTempText = ((m_fPolar_StartAmp - fText) / 4);
    fText = fText + fTempText;
    DP_RWR_PolarPlotDrawText((s_PolarPlot_Display.m_iDispWidth / 2) + (fScreenStartPos_X) + 2, (fScreenStartPos_Y + (fScreenBoundaryHeight / 2.55f)) - 5, QString::number(fText, 'f', 1) + "dBm", DP_POLAR_PLOT_FONT, DP_POLAR_PLOT_WHITE_COLOR_CODE);

    fText = fText + fTempText;
    DP_RWR_PolarPlotDrawText((s_PolarPlot_Display.m_iDispWidth / 2) + (fScreenStartPos_X) + 2, (fScreenStartPos_Y + (fScreenBoundaryHeight / 3.15f)) - 5, QString::number(fText, 'f', 1) + "dBm", DP_POLAR_PLOT_FONT, DP_POLAR_PLOT_WHITE_COLOR_CODE);

    fText = fText + fTempText;
    DP_RWR_PolarPlotDrawText((s_PolarPlot_Display.m_iDispWidth / 2) + (fScreenStartPos_X) + 2, (fScreenStartPos_Y + (fScreenBoundaryHeight / 4.13f)) - 5, QString::number(fText, 'f', 1) + "dBm", DP_POLAR_PLOT_FONT, DP_POLAR_PLOT_WHITE_COLOR_CODE);

    fText = fText + fTempText;
    DP_RWR_PolarPlotDrawText((s_PolarPlot_Display.m_iDispWidth / 2) + (fScreenStartPos_X) + 2, (fScreenStartPos_Y + (fScreenBoundaryHeight / 6)) - 5, QString::number(fText, 'f', 1) + "dBm", DP_POLAR_PLOT_FONT, DP_POLAR_PLOT_WHITE_COLOR_CODE);

    m_iCount = 0;
    if(ui->obj_rd_btn_GridLinesVisible->isChecked() == true)
    {
        DP_RWR_PolarPlotDrawGridLines(fScreenStartPos_X + (fScreenBoundaryWidth / 20), fScreenStartPos_Y + (fScreenBoundaryHeight / 2), fScreenStartPos_X + (fScreenBoundaryWidth / 1.05f), fScreenStartPos_Y + (fScreenBoundaryHeight / 2), 128, 128, 128, 1);
        DP_RWR_PolarPlotDrawGridLines(fScreenStartPos_X + (fScreenBoundaryWidth / 2), fScreenStartPos_Y + (fScreenBoundaryHeight / 20), fScreenStartPos_X + (fScreenBoundaryWidth / 2), fScreenStartPos_Y + (fScreenBoundaryHeight / 1.05f), 128, 128, 128, 1);
        DP_RWR_PolarPlotDrawGridLines(fScreenStartPos_X + (fScreenBoundaryWidth / 5.5f), fScreenStartPos_Y + (fScreenBoundaryHeight / 5.5f), fScreenStartPos_X + (fScreenBoundaryWidth / 1.21f), fScreenStartPos_Y + (fScreenBoundaryHeight / 1.21f), 128, 128, 128, 1);
        DP_RWR_PolarPlotDrawGridLines(fScreenStartPos_X + (fScreenBoundaryWidth / 1.21f), fScreenStartPos_Y + (fScreenBoundaryHeight / 5.5f), fScreenStartPos_X + (fScreenBoundaryWidth / 5.45f), fScreenStartPos_Y + (fScreenBoundaryHeight / 1.21f), 128, 128, 128, 1);
        //update();
    }
}

void dp_rwr_polarplot_power_vs_doa::DP_RWR_PolarPlotDrawCircle(float in_fStartX, float in_fStartY, float in_fDepth, float in_fWidth, int in_iR, int in_iG, int in_iB, float in_fPenWidth, int in_iStartAngle, int in_iSpanAngle)
{
    //QPainter obj_paint_Circle(this);

    QColor obj_clr_Circle;

    /* Standard QPainter function to avoid staircase effect on Lines */
    m_obj_paint_line->setRenderHints(QPainter::Antialiasing, true);

    /* Setting up color and size of the pen */
    obj_clr_Circle.setRgb(in_iR, in_iG, in_iB);
    m_obj_Pen_Line.setColor(obj_clr_Circle);
    m_obj_Pen_Line.setCapStyle(Qt::FlatCap);

    /* Adjusting and setting up the pen size based on the boundary width and height */
    /*DP_QT_POLAR_PLOT_AdjustPenWidth(in_fPenWidth);*/
    m_obj_Pen_Line.setWidthF(in_fPenWidth);

    /* Setting up the pen to draw */
    m_obj_paint_line->setPen(m_obj_Pen_Line);

    /* Drawing the circle */
    QRectF Circle_Rect(in_fStartX, in_fStartY, in_fDepth, in_fWidth);
    m_obj_paint_line->drawArc(Circle_Rect, in_iStartAngle, in_iSpanAngle);
}

void dp_rwr_polarplot_power_vs_doa::DP_RWR_PolarPlotDrawLine(double in_dStartX, double in_dStartY, double in_dEndX, double in_dEndY, QColor in_obj_clr_Line, float in_fPenWidth)
{
    //QPainter obj_paint_Line(this);

    /* Standard QPainter function to avoid staircase effect on lines */
    m_obj_paint_line->setRenderHints(QPainter::Antialiasing, true);

    /* Setting up color and style of the pen */
    m_obj_Pen_Line.setColor(in_obj_clr_Line);
    m_obj_Pen_Line.setCapStyle(Qt::FlatCap);

    /* Adjusting and setting up the pen size based on the boundary width and height */
    m_obj_Pen_Line.setWidthF(in_fPenWidth);

    /* Setting up the pen to draw */
    m_obj_paint_line->setPen(m_obj_Pen_Line);

    /* Drawing the line */
    m_obj_paint_line->drawLine(in_dStartX, in_dStartY, in_dEndX, in_dEndY);
}

void dp_rwr_polarplot_power_vs_doa::DP_RWR_PolarPlotDrawText(float in_fx, float in_fy, QString in_sDegree, QString qsFontFamily, int iR, int iG, int iB)
{
    //QPainter obj_paint_Text(this);
    QColor obj_clr_Text;
    QFont  obj_font_Text;
    float  fScreenStartPos_X    = (float)s_PolarPlot_Display.m_iDispStartXPos;
    float  fScreenBoundaryWidth = (float)s_PolarPlot_Display.m_iDispWidth;

    /* Standard QPainter function to avoid staircase effect on lines */
    m_obj_paint_line->setRenderHints(QPainter::Antialiasing, true);

    /* Setting up color of the pen */
    obj_clr_Text.setRgb(iR, iG, iB);
    m_obj_Pen_Line.setColor(obj_clr_Text);

    /* Setting up the properties of the font */
    obj_font_Text = m_obj_paint_line->font();
    obj_font_Text.weight();
    obj_font_Text.setBold(true);
    obj_font_Text.setFamily(qsFontFamily);

    /* Adjusting and setting up the font size based on the boundary width and height */
    obj_font_Text.setPointSizeF(((fScreenStartPos_X + (fScreenBoundaryWidth / 4.13f)) - fScreenStartPos_X + (fScreenBoundaryWidth / 6)) / 23.8465f);

    /* Setting up the font */
    m_obj_paint_line->setFont(obj_font_Text);

    /* Setting up the pen to draw */
    m_obj_paint_line->setPen(m_obj_Pen_Line);

    /* Drawing the text */
    m_obj_paint_line->drawText(in_fx, in_fy, in_sDegree);
}

void dp_rwr_polarplot_power_vs_doa::DP_POLAR_PLOT_SLOT_UpdateValueForCircleRotation()
{
    if(m_ucRotationSide == DP_POLAR_PLOT_CLOCKWISE)
    {
        if(DP_ANGLE_OF_360_DEGREE > m_iDiff)
        {
            m_iDiff = m_iDiff + 2;
            if(m_iDiff > DP_ANGLE_OF_360_DEGREE)
            {
                m_iCurrent_Degree = m_iCurrent_Degree - (1 * DP_ONE_DEGREE_VALUE); //Current Degree is used to rotate the circle and Count is used to rotate the angle niddle
                m_iNiddleDegree   = m_iNiddleDegree + 1;
                m_iCount++;
                m_iCurrentStopDeg++;
                m_iCurrentTextStopDeg++;
            }
            else
            {
                m_iCurrent_Degree = m_iCurrent_Degree - (2 * DP_ONE_DEGREE_VALUE);
                m_iNiddleDegree   = m_iNiddleDegree + 2;
                m_iCount          = m_iCount + 2;
                m_iCurrentStopDeg = m_iCurrentStopDeg + 2;
                m_iCurrentTextStopDeg = m_iCurrentTextStopDeg + 2;
            }
        }
    }

    if(m_ucRotationSide == DP_POLAR_PLOT_ANTICLOCKWISE)
    {
        if(m_iDiff > 0)
        {
            m_iDiff = m_iDiff - 2;
            if(m_iDiff < 0)
            {
                m_iCurrent_Degree = m_iCurrent_Degree + (1 * DP_ONE_DEGREE_VALUE);
                m_iNiddleDegree   = m_iNiddleDegree - 1;
                m_iCount--;
                m_iCurrentStopDeg--;
                m_iCurrentTextStopDeg--;
            }
            else
            {
                m_iCurrent_Degree = m_iCurrent_Degree + (2 * DP_ONE_DEGREE_VALUE);
                m_iNiddleDegree   = m_iNiddleDegree - 2;
                m_iCount          = m_iCount - 2;
                m_iCurrentStopDeg = m_iCurrentStopDeg - 2;
                m_iCurrentTextStopDeg = m_iCurrentTextStopDeg - 2;
            }
        }
    }

    if(m_iCurrentTextStopDeg < 0)
    {
        m_iCurrentTextStopDeg = m_iCurrentTextStopDeg + DP_ANGLE_OF_360_DEGREE;
    }

    if(m_iCurrentTextStopDeg > DP_ANGLE_OF_360_DEGREE)
    {
        m_iCurrentTextStopDeg = m_iCurrentTextStopDeg - DP_ANGLE_OF_360_DEGREE;
    }

    if((m_iDiff >= 360) || (m_iDiff <= 0))
    {
        m_iPrev_Degree = m_iDegree;
        m_obj_tmr_CircleRotation->stop();
        ui->obj_psh_btn_StartRotation->setDisabled(false);
    }
}

void dp_rwr_polarplot_power_vs_doa::DP_RWR_PolarPlotDynamicScreen()
{
    float fScreenBoundaryWidth                 = (float)s_PolarPlot_Display.m_iDispWidth;
    float fScreenStartPos_X                    = (float)s_PolarPlot_Display.m_iDispStartXPos;
    float fScreenStartPos_Y                    = (float)s_PolarPlot_Display.m_iDispStartYPos;
    float fScreenBoundaryHeight                = (float)s_PolarPlot_Display.m_iDispHeight;

    int iAngleLines_RefX                       = (int)((fScreenBoundaryWidth / 2) + fScreenStartPos_X);
    int iAngleLines_RefY                       = (int)((fScreenBoundaryWidth / 2) + fScreenStartPos_Y);
    int iAngle                                 = 0;
    double dLineAngle                          = 0;
    double dLineAngle2                         = 0;
    double dLineAngle3                         = 0;
    char arr_cDegree[DP_TOTAL_DEGREE_VALUE][4] = {"90", "120", "150", "180", "210", "240", "270", "300", "330", "0", "30", "60"};
    int iIndVal                                = 0;

    double dThreatX_Pos = 0;
    double dThreatY_Pos = 0;
    short  sRetVal      = 0;

    QString obj_szText  = "";
    QRect   obj_rect_DispArea;
    QColor  obj_clr_Line;

    obj_clr_Line.setRgb(DP_POLAR_PLOT_WHITE_COLOR_CODE);

    /* Rotation of outer most circle based on degree */
    DP_RWR_PolarPlotDrawCircle(fScreenStartPos_X + (fScreenBoundaryWidth / 10.90f), fScreenStartPos_Y + (fScreenBoundaryHeight / 10.90f), fScreenBoundaryWidth - (fScreenBoundaryWidth / 5.45f),fScreenBoundaryHeight - (fScreenBoundaryHeight / 5.45f), DP_POLAR_PLOT_GREEN_COLOR_CODE, DP_PEN_WIDTH_2, (m_iCurrent_Degree), (DP_CIRCLE_90_DEGREE));
    DP_RWR_PolarPlotDrawCircle(fScreenStartPos_X + (fScreenBoundaryWidth / 10.90f), fScreenStartPos_Y + (fScreenBoundaryHeight / 10.90f), fScreenBoundaryWidth - (fScreenBoundaryWidth / 5.45f), fScreenBoundaryHeight - (fScreenBoundaryHeight / 5.45f), DP_POLAR_PLOT_RED_COLOR_CODE, DP_PEN_WIDTH_2, (m_iCurrent_Degree + DP_CIRCLE_90_DEGREE), (DP_CIRCLE_90_DEGREE));
    DP_RWR_PolarPlotDrawCircle(fScreenStartPos_X + (fScreenBoundaryWidth / 10.90f), fScreenStartPos_Y + (fScreenBoundaryHeight / 10.90f), fScreenBoundaryWidth - (fScreenBoundaryWidth / 5.45f), fScreenBoundaryHeight - (fScreenBoundaryHeight / 5.45f), DP_POLAR_PLOT_RED_COLOR_CODE, DP_PEN_WIDTH_2, (m_iCurrent_Degree + DP_CIRCLE_0_TO_180_DEGREE), (DP_CIRCLE_90_DEGREE));
    DP_RWR_PolarPlotDrawCircle(fScreenStartPos_X + (fScreenBoundaryWidth / 10.90f), fScreenStartPos_Y + (fScreenBoundaryHeight / 10.90f), fScreenBoundaryWidth - (fScreenBoundaryWidth / 5.45f), fScreenBoundaryHeight - (fScreenBoundaryHeight / 5.45f), DP_POLAR_PLOT_GREEN_COLOR_CODE, DP_PEN_WIDTH_2, (m_iCurrent_Degree + DP_CIRCLE_0_TO_270_DEGREE), (DP_CIRCLE_90_DEGREE));

    /* Rotation of niddle based on degree */
    dLineAngle = (double)(m_iNiddleDegree) * 1.0f;
    dLineAngle = DP_RWR_DEG_2_RAD(dLineAngle);

    DP_RWR_PolarPlotDrawLine((int)(iAngleLines_RefX + ((fScreenBoundaryWidth / 2.44f) * cos(dLineAngle))),
                             (int)(iAngleLines_RefY + ((fScreenBoundaryWidth / 2.44f) * sin(dLineAngle))),
                             (int)(iAngleLines_RefX + ((fScreenBoundaryWidth / 3000) * cos(dLineAngle))),
                             (int)(iAngleLines_RefY + ((fScreenBoundaryWidth / 3000) * sin(dLineAngle))), obj_clr_Line, 2);

    /* Rotation of diamond symbol on niddle based on degree */
    dLineAngle2 = (double)(m_iNiddleDegree - 2) * 1.0f;
    dLineAngle2 = DP_RWR_DEG_2_RAD(dLineAngle2);

    DP_RWR_PolarPlotDrawLine((int)(iAngleLines_RefX + ((fScreenBoundaryWidth / 2.44f) * cos(dLineAngle))),
                             (int)(iAngleLines_RefY + ((fScreenBoundaryWidth / 2.44f) * sin(dLineAngle))),
                             (int)(iAngleLines_RefX + ((fScreenBoundaryWidth / 2.35f) * cos(dLineAngle2))),
                             (int)(iAngleLines_RefY + ((fScreenBoundaryWidth / 2.35f) * sin(dLineAngle2))), obj_clr_Line, 2);

    DP_RWR_PolarPlotDrawLine((int)(iAngleLines_RefX +  ((fScreenBoundaryWidth / 2.35f) * cos(dLineAngle2))),
                             (int)(iAngleLines_RefY + ((fScreenBoundaryWidth / 2.35f) * sin(dLineAngle2))),
                             (int)(iAngleLines_RefX + ((fScreenBoundaryWidth / 2.28)  * cos(dLineAngle))),
                             (int)(iAngleLines_RefY + ((fScreenBoundaryWidth / 2.28f) * sin(dLineAngle))), obj_clr_Line, 2);

    dLineAngle3 = (double)(m_iNiddleDegree + 2) * 1.0f;
    dLineAngle3 = DP_RWR_DEG_2_RAD(dLineAngle3);

    DP_RWR_PolarPlotDrawLine((int)(iAngleLines_RefX + ((fScreenBoundaryWidth / 2.28f) * cos(dLineAngle))),
                             (int)(iAngleLines_RefY + ((fScreenBoundaryWidth / 2.28f) * sin(dLineAngle))),
                             (int)(iAngleLines_RefX + ((fScreenBoundaryWidth / 2.35f) * cos(dLineAngle3))),
                             (int)(iAngleLines_RefY + ((fScreenBoundaryWidth / 2.35f) * sin(dLineAngle3))), obj_clr_Line, 2);

    DP_RWR_PolarPlotDrawLine((int)(iAngleLines_RefX + ((fScreenBoundaryWidth / 2.35f) * cos(dLineAngle3))),
                             (int)(iAngleLines_RefY + ((fScreenBoundaryWidth / 2.35f) * sin(dLineAngle3))),
                             (int)(iAngleLines_RefX + ((fScreenBoundaryWidth / 2.44f) * cos(dLineAngle))),
                             (int)(iAngleLines_RefY + ((fScreenBoundaryWidth / 2.44f) * sin(dLineAngle))), obj_clr_Line, 2);

    /* Rotation of degree values based on degree */
    for(int iCircleInLine_Angles = 0; iCircleInLine_Angles < DP_ANGLE_OF_360_DEGREE; iCircleInLine_Angles += 30)
    {
        dLineAngle = (double)(iCircleInLine_Angles + (m_iCurrentTextStopDeg));
        iAngle = dLineAngle;

        if(dLineAngle > DP_ANGLE_OF_360_DEGREE)
        {
            iAngle = iAngle - DP_ANGLE_OF_360_DEGREE;
        }
        else if(dLineAngle <= 0)
        {
            iAngle = iAngle + DP_ANGLE_OF_360_DEGREE;
        }
        else
        {
            iAngle = dLineAngle;
        }

        dLineAngle = DP_RWR_DEG_2_RAD(dLineAngle);
        if((iCircleInLine_Angles % 30) == 0)
        {
            if(iCircleInLine_Angles != DP_ANGLE_OF_270_DEGREE)
            {
                DP_RWR_PolarPlotDrawLine((int)(iAngleLines_RefX +  ((fScreenBoundaryWidth / 2.44f) * cos(dLineAngle))),
                                         (int)(iAngleLines_RefY + ((fScreenBoundaryWidth / 2.44f) * sin(dLineAngle))),
                                         (int)(iAngleLines_RefX + ((fScreenBoundaryWidth / 2.35f) * cos(dLineAngle))),
                                         (int)(iAngleLines_RefY + ((fScreenBoundaryWidth / 2.35f) * sin(dLineAngle))), obj_clr_Line, 2);
            }

            obj_szText = arr_cDegree[iIndVal];
            if(obj_szText != "0")
            {
#if 0
                if(((iAngle >= 360) && (iAngle <= 450)) || ((iAngle < 90) && (iAngle > 0)))
                {
                    DP_RWR_PolarPlotDrawText((iAngleLines_RefX + ((fScreenBoundaryWidth / 2.3f) * cos(dLineAngle))), ((iAngleLines_RefY) + ((fScreenBoundaryWidth / 2.3f) * sin(dLineAngle))), obj_szText, DP_POLAR_PLOT_FONT, DP_POLAR_PLOT_WHITE_COLOR_CODE);
                }
                else if(((iAngle > 450) && (iAngle <= 540)) || ((iAngle < 180) && (iAngle > 90)))
                {
                    DP_RWR_PolarPlotDrawText((iAngleLines_RefX + ((fScreenBoundaryWidth / 2.2f) * cos(dLineAngle))), ((iAngleLines_RefY ) +  ((fScreenBoundaryWidth / 2.2f) * sin(dLineAngle))), obj_szText, DP_POLAR_PLOT_FONT, DP_POLAR_PLOT_WHITE_COLOR_CODE);
                }
                else if(((iAngle > 540) && (iAngle <= 630)) || ((iAngle <= 270) && (iAngle > 180)))
                {
                    DP_RWR_PolarPlotDrawText((iAngleLines_RefX + ((fScreenBoundaryWidth / 2.2f) * cos(dLineAngle))), ((iAngleLines_RefY ) +  ((fScreenBoundaryWidth / 2.25f) * sin(dLineAngle))), obj_szText, DP_POLAR_PLOT_FONT, DP_POLAR_PLOT_WHITE_COLOR_CODE);
                }
                else if(((iAngle > 630) && (iAngle < 720)) || ((iAngle < 360) && (iAngle > 270)))
                {
                    DP_RWR_PolarPlotDrawText((iAngleLines_RefX + ((fScreenBoundaryWidth / 2.3f) * cos(dLineAngle))), (iAngleLines_RefY +  ((fScreenBoundaryWidth / 2.3f) * sin(dLineAngle))), obj_szText, DP_POLAR_PLOT_FONT, DP_POLAR_PLOT_WHITE_COLOR_CODE);
                }
#endif
                if((iAngle <= 90) && (iAngle > 0))
                {
                    DP_RWR_PolarPlotDrawText((iAngleLines_RefX + ((fScreenBoundaryWidth / 2.3f) * cos(dLineAngle))), ((iAngleLines_RefY) + ((fScreenBoundaryWidth / 2.3f) * sin(dLineAngle))), obj_szText, DP_POLAR_PLOT_FONT, DP_POLAR_PLOT_WHITE_COLOR_CODE);
                }
                else if((iAngle <= 180) && (iAngle > 90))
                {
                    DP_RWR_PolarPlotDrawText((iAngleLines_RefX + ((fScreenBoundaryWidth / 2.2f) * cos(dLineAngle))), ((iAngleLines_RefY ) +  ((fScreenBoundaryWidth / 2.2f) * sin(dLineAngle))), obj_szText, DP_POLAR_PLOT_FONT, DP_POLAR_PLOT_WHITE_COLOR_CODE);
                }
                else if((iAngle <= 270) && (iAngle > 180))
                {
                    DP_RWR_PolarPlotDrawText((iAngleLines_RefX + ((fScreenBoundaryWidth / 2.2f) * cos(dLineAngle))), ((iAngleLines_RefY ) +  ((fScreenBoundaryWidth / 2.25f) * sin(dLineAngle))), obj_szText, DP_POLAR_PLOT_FONT, DP_POLAR_PLOT_WHITE_COLOR_CODE);
                }
                else if((iAngle <= 360) && (iAngle > 270))
                {
                    DP_RWR_PolarPlotDrawText((iAngleLines_RefX + ((fScreenBoundaryWidth / 2.3f) * cos(dLineAngle))), (iAngleLines_RefY +  ((fScreenBoundaryWidth / 2.3f) * sin(dLineAngle))), obj_szText, DP_POLAR_PLOT_FONT, DP_POLAR_PLOT_WHITE_COLOR_CODE);
                }
            }

            iIndVal++;
        }
    }

    /* To plot the symbols based on detected frequencies and angles */
#if 0
    for(unsigned long ulLoop = 0; ulLoop < pAppHandle->m_EmitterTrackReports_TimeTagged.S_EmitterTrackReport.ulEmitterCount; ulLoop++)
    {
        if(pAppHandle->m_EmitterTrackReports_TimeTagged.S_EmitterTrackReport.mEmitter[ulLoop].EmitterParameters.usPRI_Type == DP_RWR_CW_SIGNAL)
        {
            DP_RWR_PolarPlotFindSymbolDisplayArea(pAppHandle->m_EmitterTrackReports_TimeTagged.S_EmitterTrackReport.mEmitter[ulLoop].EmitterParameters.fDirection_Deg + m_iCount, pAppHandle->m_EmitterTrackReports_TimeTagged.S_EmitterTrackReport.mEmitter[ulLoop].EmitterParameters.dFrequency_MHz, DP_THREAT_CIRCLE_CODE, 0);
        }
        else
        {
            DP_RWR_PolarPlotFindSymbolDisplayArea(pAppHandle->m_EmitterTrackReports_TimeTagged.S_EmitterTrackReport.mEmitter[ulLoop].EmitterParameters.fDirection_Deg + m_iCount, pAppHandle->m_EmitterTrackReports_TimeTagged.S_EmitterTrackReport.mEmitter[ulLoop].EmitterParameters.dFrequency_MHz, DP_THREAT_TRIANGLE_CODE, 0);
        }
    }
#endif
    memcpy(&mS_ThreatList, &mS_ThreatList,  sizeof (S_RWR_THREAT_LIST));

    for(unsigned long ulLoop = 0; ulLoop < mS_ThreatList.ulThreatCount; ulLoop++)
    {
        sRetVal = DP_RWR_PolarPlotFindSymbolDisplayArea(mS_ThreatList.arrS_RWRThreatSymbolInfo[ulLoop].fAngle + m_iCount, mS_ThreatList.arrS_RWRThreatSymbolInfo[ulLoop].fAmplitude_dBm, &dThreatX_Pos, &dThreatY_Pos);
        if(sRetVal == DP_POLAR_FAILURE)
        {
            continue;
        }

        mS_ThreatList.arrS_RWRThreatSymbolInfo[ulLoop].dThreatX_Pos = dThreatX_Pos;
        mS_ThreatList.arrS_RWRThreatSymbolInfo[ulLoop].dThreatY_Pos = dThreatY_Pos;

        obj_rect_DispArea.setX((dThreatX_Pos - mS_ThreatList.arrS_RWRThreatSymbolInfo[ulLoop].ucThreatSymbolSize) - 2);
        obj_rect_DispArea.setY((dThreatY_Pos - mS_ThreatList.arrS_RWRThreatSymbolInfo[ulLoop].ucThreatSymbolSize) - 7);
        obj_rect_DispArea.setWidth(((dThreatX_Pos  + mS_ThreatList.arrS_RWRThreatSymbolInfo[ulLoop].ucThreatSymbolSize) + 2) - ((dThreatX_Pos - mS_ThreatList.arrS_RWRThreatSymbolInfo[ulLoop].ucThreatSymbolSize) - 2));
        obj_rect_DispArea.setHeight(((dThreatY_Pos + mS_ThreatList.arrS_RWRThreatSymbolInfo[ulLoop].ucThreatSymbolSize) + 7) - ((dThreatY_Pos - mS_ThreatList.arrS_RWRThreatSymbolInfo[ulLoop].ucThreatSymbolSize) - 8));

        g_objSymbolDraw->m_obj_paint_Draw = m_obj_paint_line;
        g_objSymbolDraw->DP_RWR_Find_Threat_Symbol_Disp_Area(obj_rect_DispArea);
        g_objSymbolDraw->DP_RWR_PolarPlotDrawThreatSymbol(mS_ThreatList.arrS_RWRThreatSymbolInfo[ulLoop]);
    }

    //update();
}

short dp_rwr_polarplot_power_vs_doa::DP_RWR_PolarPlotFindSymbolDisplayArea(float in_fAngle, float in_fAmplitude, double *out_pdThreatX_Pos, double *out_pdThreatY_Pos)
{
    double dThreatX_Pos     = 0, dThreatY_Pos         = 0, dAngle_Rad = 0;
    float  fRange           = 0, fScreenBoundaryWidth = 0;
    int    iAngleLines_RefX = 0, iAngleLines_RefY     = 0;

    float fScreenStartPos_X = (float)s_PolarPlot_Display.m_iDispStartXPos;
    float fScreenStartPos_Y = (float)s_PolarPlot_Display.m_iDispStartYPos;

    if((out_pdThreatX_Pos == NULL) || (out_pdThreatY_Pos == NULL))
    {
        return DP_POLAR_FAILURE;
    }

    fScreenBoundaryWidth = (float)s_PolarPlot_Display.m_iDispWidth;
    iAngleLines_RefX     = (int)((fScreenBoundaryWidth / 2) + fScreenStartPos_X);
    iAngleLines_RefY     = (int)((fScreenBoundaryWidth / 2) + fScreenStartPos_Y);

    if(in_fAngle > 360)
    {
        in_fAngle = in_fAngle - 360;
        if(in_fAngle > 360)
        {
            in_fAngle = in_fAngle - 360;
        }
    }

    if(in_fAngle < 0)
    {
        in_fAngle = in_fAngle + 360;
    }

    dAngle_Rad = DP_RWR_DEG_2_RAD(in_fAngle - 90.0f);

    //FREQ_TO_RNG_FORMULA(fRange, in_fNormalizedEmitterRange);
    DP_RWR_PolarPlotAmpToRngConversion(in_fAmplitude, &fRange);

    if((fRange < 0.0f) || (fRange > 1.0f))
    {
        return DP_POLAR_FAILURE;
    }

    if((in_fAngle > 360.0f) || (in_fAngle < 0.0f))
    {
        return DP_POLAR_FAILURE;
    }

    fRange = ((fScreenBoundaryWidth - (fScreenBoundaryWidth / 3)) / 2) * (fRange);
    dThreatX_Pos = (fRange * cos(dAngle_Rad)) + iAngleLines_RefX;
    dThreatY_Pos = (fRange * sin(dAngle_Rad)) + iAngleLines_RefY;
    
    *out_pdThreatX_Pos = dThreatX_Pos;
    *out_pdThreatY_Pos = dThreatY_Pos;

    return DP_POLAR_SUCCESS;
}

void dp_rwr_polarplot_power_vs_doa::DP_RWR_PolarPlotDrawGridLines(double in_dStartX, double in_dStartY, double in_dEndX, double in_dEndY, int in_iR, int in_iG, int in_iB, float in_fPenWidth)
{
    //QPainter obj_paint_GridLine(this);
    QColor obj_clr_GridLine;

    /* Standard QPainter function to avoid staircase effect on Lines */
    m_obj_paint_line->setRenderHints(QPainter::Antialiasing, true);

    /* Setting up the color */
    obj_clr_GridLine.setRgb(in_iR, in_iG, in_iB);

    /* Setting up color and style of the pen */
    m_obj_Pen_GridLine.setColor(obj_clr_GridLine);
    m_obj_Pen_GridLine.setStyle(Qt::DashLine);
    m_obj_Pen_GridLine.setDashPattern({15, 6});

    /* Adjusting and setting up the pen size based on the boundary width and height */
    m_obj_Pen_GridLine.setWidthF(in_fPenWidth);

    /* Setting up the pen to draw */
    m_obj_paint_line->setPen(m_obj_Pen_GridLine);

    /* Drawing the line */
    m_obj_paint_line->drawLine(in_dStartX, in_dStartY, in_dEndX, in_dEndY);
}

void dp_rwr_polarplot_power_vs_doa::on_obj_psh_btn_PlotSymbol_clicked()
{
#if 0
    int iLoop       = 0;
#endif
    int iStopFreq   = ui->obj_ln_edt_StopFreq->text().toInt();
    int iStartFreq  = ui->obj_ln_edt_StartFreq->text().toInt();
    int iStartAngle = ui->obj_ln_edt_StartAngle->text().toInt();
    int iStopAngle  = ui->obj_ln_edt_StopAngle->text().toInt();

    if((iStartFreq < 500) || (iStopFreq > 18000))
    {
        QMessageBox::warning(this, "Error","Start and Stop frequency should between 500 to 18000MHz", QMessageBox::Ok);
        return;
    }

    if(iStartFreq > iStopFreq)
    {
        QMessageBox::warning(this, "Error","Start frequency should be less than Stop frequency", QMessageBox::Ok);
        return;
    }

    if((iStartAngle < 0) || (iStopAngle > 360))
    {
        QMessageBox::warning(this, "Error","Angle value should between 0 to 360", QMessageBox::Ok);
        return;
    }

    if(iStartAngle > iStopAngle)
    {
        QMessageBox::warning(this, "Error", "Start Angle  should be less than Stop Angle", QMessageBox::Ok);
        return;
    }
#if 0
    m_iLimitVal = ui->obj_ln_edt_Limit->text().toInt();
    for(iLoop = 0; iLoop < m_iLimitVal; iLoop++)
    {
        s_Threat_Symbol_List.s_Threat_Symbol_Param_List[iLoop].fFreq        = rand() % (iStopFreq - iStartFreq + 1) + iStartFreq;
        s_Threat_Symbol_List.s_Threat_Symbol_Param_List[iLoop].fAngle       = rand() % (iStopAngle - iStartAngle + 1) + iStartAngle;
        s_Threat_Symbol_List.s_Threat_Symbol_Param_List[iLoop].iSymbolCode  = rand() % (63 - 35 + 1) + 35;
    }
    s_Threat_Symbol_List.iThreatSymbolCount = m_iLimitVal;
#endif
}

void dp_rwr_polarplot_power_vs_doa::DP_RWR_SLOT_StartRotation(int in_iTrueHeading)
{
    /* If new True Heading data is coming in between while rotation is going for previous True Heading data then stop the timer and process again for new data */

    if(m_obj_tmr_CircleRotation->isActive())
    {
        m_obj_tmr_CircleRotation->stop();
        if(m_iCurrentStopDeg < 0)
        {
            m_iCurrentStopDeg = m_iCurrentStopDeg + DP_ANGLE_OF_360_DEGREE;
        }

        if(m_iCurrentStopDeg > DP_ANGLE_OF_360_DEGREE)
        {
            m_iCurrentStopDeg = m_iCurrentStopDeg - DP_ANGLE_OF_360_DEGREE;
        }

        m_iPrev_Degree = m_iCurrentStopDeg;
        ui->obj_psh_btn_StartRotation->setDisabled(false);
        //pAppHandle->g_ucPolarPlotCircleRotation = 1;
    }

    m_iDegree = (int)in_iTrueHeading;
    if(m_iDegree == m_iPrev_Degree)
    {
        return;
    }

    if(m_iDegree < 0)
    {
        m_iDegree = m_iDegree + DP_ANGLE_OF_360_DEGREE;
    }

    if(m_iDegree > DP_ANGLE_OF_360_DEGREE)
    {
        m_iDegree = m_iDegree - DP_ANGLE_OF_360_DEGREE;
    }

    if((m_iDegree <= DP_ANGLE_OF_360_DEGREE) && (m_iDegree >= DP_ANGLE_OF_0_DEGREE))
    {
        ui->obj_psh_btn_StartRotation->setDisabled(true);
        m_iDiff = m_iPrev_Degree - m_iDegree;

        if(m_iDiff <= 0)
        {
            m_iDiff = m_iDiff + DP_ANGLE_OF_360_DEGREE;
        }

        if(m_iDiff >= DP_ANGLE_OF_180_DEGREE)
        {
            m_ucRotationSide = DP_POLAR_PLOT_CLOCKWISE;
        }
        else
        {
            m_ucRotationSide = DP_POLAR_PLOT_ANTICLOCKWISE;
        }

        m_obj_tmr_CircleRotation->start(30);
    }
    else
    {
        PrintLog("ERROR: Angle is greater than 360", 1);
    }
}

void dp_rwr_polarplot_power_vs_doa::DP_RWR_PolarPlotAmpToRngConversion(float in_fAmplitude_dBm, float *out_pfRange)
{
    if(out_pfRange == NULL)
    {
        PrintLog("NULL Pointer Access : DP_RWR_PolarPlotAmpToRngConversion", 1);
        return;
    }

    /* To Avoid negative co-ordinates during range conversion */
    if(in_fAmplitude_dBm > m_fPolar_StopAmp)
    {
        in_fAmplitude_dBm = m_fPolar_StopAmp;
    }

    *out_pfRange = ((m_fPolar_StopAmp - in_fAmplitude_dBm) / (m_fPolar_StopAmp - m_fPolar_StartAmp)); //95
}

void dp_rwr_polarplot_power_vs_doa::DP_POLAR_PLOT_SLOT_LoadThreatList(S_RWR_THREAT_LIST in_SThreatList)
{
    memcpy(&mS_ThreatList, &in_SThreatList, sizeof(S_RWR_THREAT_LIST));
}
