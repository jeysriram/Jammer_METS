#ifndef DP_RWR_POLARPLOT_POWER_VS_DOA_H
#define DP_RWR_POLARPLOT_POWER_VS_DOA_H


#include <QWidget>
#include <qcolor.h>
#include <qpainter.h>
#include <QTimer>
#include <QtWidgets>
#include <qdebug.h>
#include <math.h>
#include <QMessageBox>

#include "dp_aspj_comm.h"

namespace Ui {
class dp_rwr_polarplot_power_vs_doa;
}

class dp_rwr_polarplot_power_vs_doa : public QWidget
{
    Q_OBJECT

    class dp_symbol_draw_polar_plot *g_objSymbolDraw;

    S_POLARPLOT_DISPLAY_PARAM s_PolarPlot_Display;
    S_RWR_THREAT_LIST mS_ThreatList;

    QPen m_obj_Pen_Line, m_obj_Pen_GridLine;
    QPainter *m_obj_paint_line;

    unsigned char m_ucRotationSide = 0;

    int m_iCurrent_Degree = 0;
    int m_iCount = 0;
    int m_iPrev_Degree = 0;
    int m_iDegree = 0;
    int m_iNiddleDegree = 0;
    int m_iDiff = 0;
    int m_iLimitVal = 0;
    int m_iInitialFlag = 0;
    int m_iCurrentStopDeg = 0;
    int m_iCurrentTextStopDeg = 0;

    float m_fAngle = 0;
    float m_fFreq = 0;
    float m_fPolar_StartAmp = 0;
    float m_fPolar_StopAmp = 0;

    void DP_RWR_PolarPlotOnInitial();
    void DP_RWR_PolarPlotFindDisplayArea();
    void DP_RWR_PolarPlotStaticScreen();
    void DP_RWR_PolarPlotDrawLine(double in_dStartX, double in_dStartY, double in_dEndX, double in_dEndY, QColor in_obj_clr_Line, float in_fPenWidth);
    void DP_RWR_PolarPlotDrawCircle(float in_fStartX, float in_fStartY, float in_fDepth, float in_fWidth, int in_iR, int in_iG, int in_iB, float in_fPenWidth, int in_iStartAngle, int in_iSpanAngle);
    void DP_RWR_PolarPlotDrawText(float in_fx, float in_fy, QString in_sDegree, QString qsFontFamily, int iR, int iG, int iB);
    void DP_RWR_PolarPlotDrawGridLines(double in_dStartX, double in_dStartY, double in_dEndX, double in_dEndY, int in_iR, int in_iG, int in_iB, float in_fPenWidth);
    void DP_RWR_PolarPlotDynamicScreen();
    void DP_RWR_PolarPlotAmpToRngConversion(float in_fFreq, float *out_pfRange);

    short DP_RWR_PolarPlotFindSymbolDisplayArea(float in_fAngle, float in_fFreq, double *out_pdThreatX_Pos, double *out_pdThreatY_Pos);

public:
    explicit dp_rwr_polarplot_power_vs_doa(QWidget *parent = nullptr);
    ~dp_rwr_polarplot_power_vs_doa();

private:
    Ui::dp_rwr_polarplot_power_vs_doa *ui;
    QTimer *m_obj_tmr_CircleRotation;
    QTimer *m_obj_tmr_PaintEvent;

public slots:
    virtual void paintEvent(QPaintEvent *event);
    void DP_POLAR_PLOT_SLOT_UpdateValueForCircleRotation();
    void DP_POLAR_PLOT_SLOT_CallPaintEventFunc();

    void DP_POLAR_PLOT_SLOT_LoadThreatList(S_RWR_THREAT_LIST pInputParam);
    void SetPolarRange(float in_fMinFrequency, float in_fMaxFrequency);

private slots:
    void on_obj_psh_btn_PlotSymbol_clicked();    
    void DP_RWR_SLOT_StartRotation(int in_iTrueHeading);

signals:
    void PrintLog(QString, int);
};

#endif // DP_RWR_POLARPLOT_POWER_VS_DOA_H
