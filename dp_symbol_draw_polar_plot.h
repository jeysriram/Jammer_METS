#ifndef DP_SYMBOL_DRAW_H
#define DP_SYMBOL_DRAW_H

#include <QPaintEvent>
#include <QColor>
#include <QPainter>

#include "dp_aspj_comm.h"

class dp_symbol_draw_polar_plot
{
public:
    dp_symbol_draw_polar_plot();
    int iCenterX;
    int iCenterY;
    int iEndX;
    int iEndY;
    int iX;
    int iY;
    QPen m_obj_Pen_Line, m_obj_Pen_GridLine;
    QPainter *m_obj_paint_Draw;

public :

    void DP_RWR_Find_Threat_Symbol_Disp_Area(QRect obj_rect_DispArea);
    void DP_RWR_PolarPlotDrawThreatSymbol(S_RWR_THREAT_SYMBOL_INFO in_SThreatSymbolInfo);
    void DP_RWR_PolarPlotDrawLine(double in_dStartX, double in_dStartY, double in_dEndX, double in_dEndY, QColor in_obj_clr_Line, float in_fPenWidth);
    void DP_RWR_PolarPlotDrawCircle(float in_fStartX, float in_fStartY, float in_fDepth, float in_fWidth, QColor in_obj_clr_Circle, float in_fPenWidth, int in_iStartAngle, int in_iSpanAngle);
    void DP_RWR_PolarPlotDrawText(float in_fx, float in_fy, QString in_sDegree, QString qsFontFamily, QColor in_obj_clr_SymbColor, int in_iFontSize);
};

#endif // DP_SYMBOL_DRAW_H
