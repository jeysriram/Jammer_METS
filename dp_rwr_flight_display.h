#ifndef DP_RWR_FLIGHT_DISPLAY_H
#define DP_RWR_FLIGHT_DISPLAY_H

#include <QWidget>
#include <QPainter>
#include <QMenu>
#include <QTimer>
#include <QDebug>
#include <stdio.h>
#include <float.h>
#include <math.h>

#include "dp_aspj_comm.h"

#define DP_RWR_TEXT_ALIGNMENT_CENTER     0
#define DP_RWR_TEXT_ALIGNMENT_HCENTER    1
#define DP_RWR_TEXT_ALIGNMENT_VCENTER    2

#define DP_RWR_ACCEPT_ADD_WEIGHTAGE      0
#define DP_RWR_IGNORE_ADD_WEIGHTAGE      1

namespace Ui {
class dp_rwr_flight_display;
}

class dp_rwr_flight_display : public QWidget
{
    Q_OBJECT

public:
    explicit dp_rwr_flight_display(QWidget *parent = nullptr);
    ~dp_rwr_flight_display();

    QPainter *m_obj_paint_line;

    QTimer *m_obj_tmr_PaintEvent;

    QPen cPen;
    QColor cMyColor;
    QFont l_font;
    QMenu *contextMenu;

    float fPenWidth = 0;
    int m_iSymbolCodeIndex[DP_RWR_MAX_EMITTERS] = {0};
    char cAddRowFlag = 0;
    unsigned char m_ucOfflineFilePathConfigureDoneAlready = 0;

    S_RWR_THREAT_LIST			m_S_RWRThreatList;
    DP_EMITTER_REPORT_DISPLAY_PARAM mS_Emitter_Report_Display;

    /*************************************** Drawing Functions ***************************************/
    void DP_RWR_Bitmap_FindDisplayArea();
    void DP_RWR_Bitmap_DrawStaticScreen();
    void DP_RWR_Bitmap_DrawDynamicScreen();

    void DP_RWR_Bitmap_FontSelect(QPainter *in_m_obj_paint_line, unsigned short in_usFontWeight, int in_iR, int in_iG, int in_iB);
    void DP_RWR_Bitmap_AdjustPenWidth(float *out_fPenWidth);
    void DP_RWR_Bitmap_FindCharMetrics(QString in_qsText, int *out_piTextWidth, int *out_piTextHeight);

    void DP_RWR_Bitmap_DrawNormalCircle(double in_fx1, double in_fy1, double in_fx2, double in_fy2, QColor in_qcRgb, float in_fPenWidth, float in_fStartAngle, float in_fSpanAngle);
    void DP_RWR_Bitmap_DrawAdjustingCircle(QPoint in_qpCenter, int in_iRadius, int in_iR, int in_iG, int in_iB, float in_fPenWidth);
    void DP_RWR_Bitmap_DrawGradientCircle(float in_fx, float in_fy, float in_fwidth, float in_fheight, float in_fStartAngle, float in_fSpanAngle, int in_iR, int in_iG, int in_iB, float in_fPenWidth);
    void DP_RWR_Bitmap_FillCircle(QPoint in_qpCenter, int in_iRadius, int in_iR, int in_iG, int in_iB, float in_fPenWidth);
    void DP_RWR_Bitmap_DrawRect(float in_fx, float in_fy, float in_fwidth, float in_fheight, int in_iR, int in_iG, int in_iB, float in_fPenWidth);
    void DP_RWR_Bitmap_DrawRoundedRect(float in_fx, float in_fy, float in_fwidth, float in_fheight, int in_iR, int in_iG, int in_iB, float in_fPenWidth);
    void DP_RWR_Bitmap_FillRoundedRect(float in_fx, float in_fy, float in_fwidth, float in_fheight, QColor in_qcRgb, bool in_isRounded, bool in_isFilled);
    void DP_RWR_Bitmap_DrawLine(double in_fx1, double in_fy1, double in_fx2, double in_fy2, QColor in_qcRgb, float in_fPenWidth, unsigned char in_ucAddWeight = DP_RWR_IGNORE_ADD_WEIGHTAGE);
    void DP_RWR_Bitmap_DrawText(QPainter *in_m_obj_painter, int in_iXPos, int in_iYPos, QString in_qsStr, unsigned char in_ucAlignment = DP_RWR_TEXT_ALIGNMENT_CENTER);
    void DP_RWR_Bitmap_DrawFlightModel(float in_fStartXPos, float in_fStartYPos, int in_iR, int in_iG, int in_iB, float in_fPenwidth);

    void DP_RWR_Bitmap_Draw_Threat_Details(S_RWR_THREAT_SYMBOL_INFO in_S_RWRThreatInfo);
    void DP_RWR_Bitmap_Draw_Threat_Symbol(double in_dThreatX_Pos, double in_dThreatY_Pos, unsigned short in_usSymbolCode, char *in_arrcThreatName, unsigned char in_ucThreatSymbolSize);
    short UpdateEmitterSymbolDetails(unsigned char in_ucFileAttrib, S_EMITTER_SYMBOL_LIBRARY *pS_EmitterSymbolLibrary);
    short DP_RWR_LoadSpecialSymbolDetails(S_EMITTER_SYMBOL_LIBRARY *pS_EmitterSymbolLibrary);

signals:
    void PrintLog(QString, int);
    void Sig_PrintLog(QString in_qsMessage, short in_sStatus);

public slots:
    virtual void paintEvent(QPaintEvent *event);
    void DP_POLAR_PLOT_SLOT_LoadThreatList(S_RWR_THREAT_LIST in_SThreatList);
    void DP_RWR_SLOT_FlightDisplay_CallPaintEventFunc();

private:
    Ui::dp_rwr_flight_display *ui;
};

#endif // DP_RWR_FLIGHT_DISPLAY_H
