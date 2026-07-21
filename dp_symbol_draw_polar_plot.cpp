
#include "dp_aspj_comm.h"

dp_symbol_draw_polar_plot::dp_symbol_draw_polar_plot()
{
    m_obj_paint_Draw = NULL;

    iX = 0;
    iY = 0;
    iCenterX = 0;
    iCenterY = 0;
    iEndX = 0;
    iEndY = 0;
}

void dp_symbol_draw_polar_plot::DP_RWR_Find_Threat_Symbol_Disp_Area(QRect obj_rect_DispArea)
{
    iX       = obj_rect_DispArea.x();
    iY       = obj_rect_DispArea.y();
    iCenterX = obj_rect_DispArea.x() + obj_rect_DispArea.width() / 2;
    iCenterY = obj_rect_DispArea.y() + obj_rect_DispArea.height() / 2;
    iEndX    = iX  + obj_rect_DispArea.width();
    iEndY    = (iY + obj_rect_DispArea.height()) - 5;
}

void dp_symbol_draw_polar_plot::DP_RWR_PolarPlotDrawThreatSymbol(S_RWR_THREAT_SYMBOL_INFO in_SThreatSymbolInfo)
{
    int    iWidth  = 0;
    int    iHeight = 0;
    double dThreatX_Pos = 0.0f;
    double dThreatY_Pos = 0.0f;
    QColor bgColor, fgColor;
    QPainterPath RectPath;

    iWidth  = ((in_SThreatSymbolInfo.dThreatX_Pos + in_SThreatSymbolInfo.ucThreatSymbolSize) - (in_SThreatSymbolInfo.dThreatX_Pos - in_SThreatSymbolInfo.ucThreatSymbolSize)) + 2 * 5;
    iHeight = ((in_SThreatSymbolInfo.dThreatY_Pos + in_SThreatSymbolInfo.ucThreatSymbolSize) - (in_SThreatSymbolInfo.dThreatY_Pos - in_SThreatSymbolInfo.ucThreatSymbolSize)) + 2 * 5;

    dThreatX_Pos = in_SThreatSymbolInfo.dThreatX_Pos;
    dThreatY_Pos = in_SThreatSymbolInfo.dThreatY_Pos;

    /* Checking background color and setting up the color */
    if(in_SThreatSymbolInfo.ucFG_Colour == DP_RWR_COLOR_CODE_BLACK)
    {
        fgColor.setRgb(DP_RWR_COLOR_BLACK);
    }
    else if(in_SThreatSymbolInfo.ucFG_Colour == DP_RWR_COLOR_CODE_GREEN)
    {
        fgColor.setRgb(DP_RWR_COLOR_GREEN);
    }
    else if(in_SThreatSymbolInfo.ucFG_Colour == DP_RWR_COLOR_CODE_RED)
    {
        fgColor.setRgb(DP_RWR_COLOR_RED);
    }
    else if(in_SThreatSymbolInfo.ucFG_Colour == DP_RWR_COLOR_CODE_YELLOW)
    {
        fgColor.setRgb(DP_RWR_COLOR_YELLOW);
    }
    else if(in_SThreatSymbolInfo.ucFG_Colour == DP_RWR_COLOR_CODE_BLUE)
    {
        fgColor.setRgb(DP_RWR_COLOR_BLUE);
    }
    else if(in_SThreatSymbolInfo.ucFG_Colour == DP_RWR_COLOR_CODE_CYAN)
    {
        fgColor.setRgb(DP_RWR_COLOR_CYAN);
    }
    else if(in_SThreatSymbolInfo.ucFG_Colour == DP_RWR_COLOR_CODE_MAGENTA)
    {
        fgColor.setRgb(DP_RWR_COLOR_MAGENTA);
    }
    else if(in_SThreatSymbolInfo.ucFG_Colour == DP_RWR_COLOR_CODE_WHITE)
    {
        fgColor.setRgb(DP_RWR_COLOR_WHITE);
    }
    else if(in_SThreatSymbolInfo.ucFG_Colour == DP_RWR_COLOR_CODE_LIGHT_BLACK)
    {
        fgColor.setRgb(DP_RWR_COLOR_LIGHT_BLACK);
    }
    else if(in_SThreatSymbolInfo.ucFG_Colour == DP_RWR_COLOR_CODE_LIGHT_GREEN)
    {
        fgColor.setRgb(DP_RWR_COLOR_LIGHT_GREEN);
    }
    else if(in_SThreatSymbolInfo.ucFG_Colour == DP_RWR_COLOR_CODE_LIGHT_RED)
    {
        fgColor.setRgb(DP_RWR_COLOR_LIGHT_RED);
    }
    else if(in_SThreatSymbolInfo.ucFG_Colour == DP_RWR_COLOR_CODE_LIGHT_YELLOW)
    {
        fgColor.setRgb(DP_RWR_COLOR_LIGHT_YELLOW);
    }
    else if(in_SThreatSymbolInfo.ucFG_Colour == DP_RWR_COLOR_CODE_LIGHT_BLUE)
    {
        fgColor.setRgb(DP_RWR_COLOR_LIGHT_BLUE);
    }
    else if(in_SThreatSymbolInfo.ucFG_Colour == DP_RWR_COLOR_CODE_LIGHT_CYAN)
    {
        fgColor.setRgb(DP_RWR_COLOR_LIGHT_CYAN);
    }
    else if(in_SThreatSymbolInfo.ucFG_Colour == DP_RWR_COLOR_CODE_LIGHT_MAGENTA)
    {
        fgColor.setRgb(DP_RWR_COLOR_RED);
    }
    else// if(in_SThreatSymbolInfo.ucFG_Colour == DP_RWR_COLOR_CODE_LIGHT_WHITE)
    {
        fgColor.setRgb(DP_RWR_COLOR_LIGHT_WHITE);
    }

    /* Checking foreground color and setting up the color */
    if(in_SThreatSymbolInfo.ucBG_Colour == DP_RWR_COLOR_CODE_BLACK)
    {
        bgColor.setRgbF(DP_RWR_COLOR_BLACK, 0);
    }
    else if(in_SThreatSymbolInfo.ucBG_Colour == DP_RWR_COLOR_CODE_GREEN)
    {
        bgColor.setRgbF(DP_RWR_COLOR_GREEN, DP_RWR_SYMBOL_COLOR_TRANSPARENCY);
    }
    else if(in_SThreatSymbolInfo.ucBG_Colour == DP_RWR_COLOR_CODE_RED)
    {
        bgColor.setRgbF(DP_RWR_COLOR_RED, DP_RWR_SYMBOL_COLOR_TRANSPARENCY);
    }
    else if(in_SThreatSymbolInfo.ucBG_Colour == DP_RWR_COLOR_CODE_YELLOW)
    {
        bgColor.setRgbF(DP_RWR_COLOR_YELLOW,  DP_RWR_SYMBOL_COLOR_TRANSPARENCY);
    }
    else if(in_SThreatSymbolInfo.ucBG_Colour == DP_RWR_COLOR_CODE_BLUE)
    {
        bgColor.setRgbF(DP_RWR_COLOR_BLUE, DP_RWR_SYMBOL_COLOR_TRANSPARENCY);
    }
    else if(in_SThreatSymbolInfo.ucBG_Colour == DP_RWR_COLOR_CODE_CYAN)
    {
        bgColor.setRgbF(DP_RWR_COLOR_CYAN, DP_RWR_SYMBOL_COLOR_TRANSPARENCY);
    }
    else if(in_SThreatSymbolInfo.ucBG_Colour == DP_RWR_COLOR_CODE_MAGENTA)
    {
        bgColor.setRgbF(DP_RWR_COLOR_MAGENTA, DP_RWR_SYMBOL_COLOR_TRANSPARENCY);
    }
    else if(in_SThreatSymbolInfo.ucBG_Colour == DP_RWR_COLOR_CODE_WHITE)
    {
        bgColor.setRgbF(DP_RWR_COLOR_WHITE, DP_RWR_SYMBOL_COLOR_TRANSPARENCY);
    }
    else if(in_SThreatSymbolInfo.ucBG_Colour == DP_RWR_COLOR_CODE_LIGHT_BLACK)
    {
        bgColor.setRgbF(DP_RWR_COLOR_LIGHT_BLACK, DP_RWR_SYMBOL_COLOR_TRANSPARENCY);
    }
    else if(in_SThreatSymbolInfo.ucBG_Colour == DP_RWR_COLOR_CODE_LIGHT_GREEN)
    {
        bgColor.setRgbF(DP_RWR_COLOR_LIGHT_GREEN, DP_RWR_SYMBOL_COLOR_TRANSPARENCY);
    }
    else if(in_SThreatSymbolInfo.ucBG_Colour == DP_RWR_COLOR_CODE_LIGHT_RED)
    {
        bgColor.setRgbF(DP_RWR_COLOR_LIGHT_RED, DP_RWR_SYMBOL_COLOR_TRANSPARENCY);
    }
    else if(in_SThreatSymbolInfo.ucBG_Colour == DP_RWR_COLOR_CODE_LIGHT_YELLOW)
    {
        bgColor.setRgbF(DP_RWR_COLOR_LIGHT_YELLOW, DP_RWR_SYMBOL_COLOR_TRANSPARENCY);
    }
    else if(in_SThreatSymbolInfo.ucBG_Colour == DP_RWR_COLOR_CODE_LIGHT_BLUE)
    {
        bgColor.setRgbF(DP_RWR_COLOR_LIGHT_BLUE, DP_RWR_SYMBOL_COLOR_TRANSPARENCY);
    }
    else if(in_SThreatSymbolInfo.ucBG_Colour == DP_RWR_COLOR_CODE_LIGHT_CYAN)
    {
        bgColor.setRgbF(DP_RWR_COLOR_LIGHT_CYAN, DP_RWR_SYMBOL_COLOR_TRANSPARENCY);
    }
    else if(in_SThreatSymbolInfo.ucBG_Colour == DP_RWR_COLOR_CODE_LIGHT_MAGENTA)
    {
        bgColor.setRgbF(DP_RWR_COLOR_LIGHT_MAGENTA, DP_RWR_SYMBOL_COLOR_TRANSPARENCY);
    }
    else if(in_SThreatSymbolInfo.ucBG_Colour == DP_RWR_COLOR_CODE_LIGHT_WHITE)
    {
        bgColor.setRgbF(DP_RWR_COLOR_LIGHT_WHITE, DP_RWR_SYMBOL_COLOR_TRANSPARENCY);
    }
    else
    {
        bgColor.setRgbF(DP_RWR_COLOR_BLACK, DP_RWR_SYMBOL_COLOR_TRANSPARENCY);
    }

    RectPath.addRoundedRect(iX, iY + 5, iEndX - iX, (iEndY - iY) - 5, DP_RWR_SYMBOL_ROUNDED_RECT_RADIUSX, DP_RWR_SYMBOL_ROUNDED_RECT_RADIUSY);
    //m_obj_paint_Draw->fillRect(iX, iY + 5, iEndX - iX, (iEndY - iY) - 5, bgColor);
    m_obj_paint_Draw->fillPath(RectPath, bgColor);

    if(in_SThreatSymbolInfo.ucThreatOrgin)
    {
#if 0
        DP_RWR_PolarPlotDrawLine((int)dThreatX_Pos, (int)(dThreatY_Pos - (2 * in_SThreatSymbolInfo.ucThreatSymbolSize)) + 5,
           (int)dThreatX_Pos - (2 * in_SThreatSymbolInfo.ucThreatSymbolSize) + 20, (int)(dThreatY_Pos - in_SThreatSymbolInfo.ucThreatSymbolSize), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine((int)dThreatX_Pos, (int)(dThreatY_Pos - (2 * in_SThreatSymbolInfo.ucThreatSymbolSize)) + 5,
           (int)dThreatX_Pos + (2 * in_SThreatSymbolInfo.ucThreatSymbolSize) - 20, (int)(dThreatY_Pos - in_SThreatSymbolInfo.ucThreatSymbolSize), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine((int)dThreatX_Pos, (int)(dThreatY_Pos - (2 * in_SThreatSymbolInfo.ucThreatSymbolSize)) + 10,
           (int)dThreatX_Pos - (2 * in_SThreatSymbolInfo.ucThreatSymbolSize) + 20, (int)(dThreatY_Pos - in_SThreatSymbolInfo.ucThreatSymbolSize) + 7, fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine((int)dThreatX_Pos, (int)(dThreatY_Pos - (2 * in_SThreatSymbolInfo.ucThreatSymbolSize)) + 10,
           (int)dThreatX_Pos + (2 * in_SThreatSymbolInfo.ucThreatSymbolSize) - 20, (int)(dThreatY_Pos - in_SThreatSymbolInfo.ucThreatSymbolSize) + 7, fgColor, DP_PEN_WIDTH_1);
#endif
        DP_RWR_PolarPlotDrawLine((int)iCenterX, (int)(iCenterY - (2 * (in_SThreatSymbolInfo.ucThreatSymbolSize - 5))) - 2,
                                 (int)(iCenterX - (2 * (in_SThreatSymbolInfo.ucThreatSymbolSize - 5))) + 3, (int)(iCenterY - (in_SThreatSymbolInfo.ucThreatSymbolSize - 5)), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine((int)iCenterX, (int)(iCenterY - (2 * (in_SThreatSymbolInfo.ucThreatSymbolSize - 5))) - 2,
                                 (int)(iCenterX + (2 * (in_SThreatSymbolInfo.ucThreatSymbolSize - 5))) - 3, (int)(iCenterY - (in_SThreatSymbolInfo.ucThreatSymbolSize - 5)), fgColor, DP_PEN_WIDTH_1);
    }

    if(in_SThreatSymbolInfo.ucThreatClassification == 1)
    {
#if 0
        DP_RWR_PolarPlotDrawLine((int)iX + 5, (int)iCenterY,
           (int)iCenterX, (int)iY + 10, fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine((int)iCenterX, (int)iY + 10,
           (int)iEndX - 5, (int)iCenterY, fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine((int)iEndX - 5, (int)iCenterY,
           (int)iCenterX, (int)iEndY - 5, fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine((int)iCenterX, (int)iEndY - 5,
           (int)iX + 5, (int)iCenterY, fgColor, DP_PEN_WIDTH_1);
#endif
        DP_RWR_PolarPlotDrawLine((int)iX , (int)iCenterY,
                                 (int)iCenterX, (int)iY + 5, fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine((int)iCenterX, (int)iY + 5,
                                 (int)iEndX, (int)iCenterY, fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine((int)iEndX, (int)iCenterY,
                                 (int)iCenterX, (int)iEndY , fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine((int)iCenterX, (int)iEndY,
                                 (int)iX, (int)iCenterY, fgColor, DP_PEN_WIDTH_1);
    }

    switch(in_SThreatSymbolInfo.usSymbolCode)
    {
    case DP_THREAT_PLUS_CODE:

        DP_RWR_PolarPlotDrawLine((int)dThreatX_Pos, (int)(dThreatY_Pos + in_SThreatSymbolInfo.ucThreatSymbolSize),
                                 (int)dThreatX_Pos, (int)(dThreatY_Pos - in_SThreatSymbolInfo.ucThreatSymbolSize), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine((int)(dThreatX_Pos + in_SThreatSymbolInfo.ucThreatSymbolSize), (int)(dThreatY_Pos),
                                 (int)(dThreatX_Pos - in_SThreatSymbolInfo.ucThreatSymbolSize), (int)(dThreatY_Pos), fgColor, DP_PEN_WIDTH_1);
        break;

    case DP_THREAT_TRIANGLE_CODE:

        DP_RWR_PolarPlotDrawLine((int)dThreatX_Pos - in_SThreatSymbolInfo.ucThreatSymbolSize, (int)(dThreatY_Pos + in_SThreatSymbolInfo.ucThreatSymbolSize),
                                 (int)dThreatX_Pos, (int)(dThreatY_Pos - in_SThreatSymbolInfo.ucThreatSymbolSize), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine((int)dThreatX_Pos + in_SThreatSymbolInfo.ucThreatSymbolSize, (int)(dThreatY_Pos + in_SThreatSymbolInfo.ucThreatSymbolSize),
                                 (int)dThreatX_Pos, (int)(dThreatY_Pos - in_SThreatSymbolInfo.ucThreatSymbolSize), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine((int)dThreatX_Pos + in_SThreatSymbolInfo.ucThreatSymbolSize, (int)(dThreatY_Pos + in_SThreatSymbolInfo.ucThreatSymbolSize),
                                 (int)dThreatX_Pos - in_SThreatSymbolInfo.ucThreatSymbolSize, (int)(dThreatY_Pos + in_SThreatSymbolInfo.ucThreatSymbolSize), fgColor, DP_PEN_WIDTH_1);
        break;

    case DP_THREAT_SQUARE_CODE:

        DP_RWR_PolarPlotDrawLine((int)dThreatX_Pos - in_SThreatSymbolInfo.ucThreatSymbolSize, (int)(dThreatY_Pos + in_SThreatSymbolInfo.ucThreatSymbolSize),
                                 (int)dThreatX_Pos - in_SThreatSymbolInfo.ucThreatSymbolSize, (int)(dThreatY_Pos - in_SThreatSymbolInfo.ucThreatSymbolSize), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine((int)dThreatX_Pos + in_SThreatSymbolInfo.ucThreatSymbolSize, (int)(dThreatY_Pos + in_SThreatSymbolInfo.ucThreatSymbolSize),
                                 (int)dThreatX_Pos + in_SThreatSymbolInfo.ucThreatSymbolSize, (int)(dThreatY_Pos - in_SThreatSymbolInfo.ucThreatSymbolSize), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine((int)dThreatX_Pos - in_SThreatSymbolInfo.ucThreatSymbolSize, (int)(dThreatY_Pos - in_SThreatSymbolInfo.ucThreatSymbolSize ),
                                 (int)dThreatX_Pos + in_SThreatSymbolInfo.ucThreatSymbolSize, (int)(dThreatY_Pos - in_SThreatSymbolInfo.ucThreatSymbolSize), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine((int)dThreatX_Pos - in_SThreatSymbolInfo.ucThreatSymbolSize, (int)(dThreatY_Pos + in_SThreatSymbolInfo.ucThreatSymbolSize),
                                 (int)dThreatX_Pos + in_SThreatSymbolInfo.ucThreatSymbolSize, (int)(dThreatY_Pos + in_SThreatSymbolInfo.ucThreatSymbolSize), fgColor, DP_PEN_WIDTH_1);
        break;

    case DP_THREAT_CROSS_CODE:

        DP_RWR_PolarPlotDrawLine((int)dThreatX_Pos - in_SThreatSymbolInfo.ucThreatSymbolSize, (int)(dThreatY_Pos + in_SThreatSymbolInfo.ucThreatSymbolSize),
                                 (int)dThreatX_Pos + in_SThreatSymbolInfo.ucThreatSymbolSize, (int)(dThreatY_Pos - in_SThreatSymbolInfo.ucThreatSymbolSize), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine((int)dThreatX_Pos - in_SThreatSymbolInfo.ucThreatSymbolSize, (int)(dThreatY_Pos - in_SThreatSymbolInfo.ucThreatSymbolSize),
                                 (int)dThreatX_Pos + in_SThreatSymbolInfo.ucThreatSymbolSize, (int)(dThreatY_Pos + in_SThreatSymbolInfo.ucThreatSymbolSize), fgColor, DP_PEN_WIDTH_1);
        break;

    case DP_THREAT_DIAMOND_CODE:

        DP_RWR_PolarPlotDrawLine((int)dThreatX_Pos - in_SThreatSymbolInfo.ucThreatSymbolSize, (int)(dThreatY_Pos),
                                 (int)dThreatX_Pos, (int)(dThreatY_Pos - in_SThreatSymbolInfo.ucThreatSymbolSize), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine((int)dThreatX_Pos, (int)(dThreatY_Pos - in_SThreatSymbolInfo.ucThreatSymbolSize),
                                 (int)dThreatX_Pos + in_SThreatSymbolInfo.ucThreatSymbolSize, (int)(dThreatY_Pos), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine((int)dThreatX_Pos + in_SThreatSymbolInfo.ucThreatSymbolSize, (int)(dThreatY_Pos),
                                 (int)dThreatX_Pos, (int)(dThreatY_Pos + in_SThreatSymbolInfo.ucThreatSymbolSize), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine((int)dThreatX_Pos, (int)(dThreatY_Pos + in_SThreatSymbolInfo.ucThreatSymbolSize),
                                 (int)dThreatX_Pos - in_SThreatSymbolInfo.ucThreatSymbolSize, (int)(dThreatY_Pos), fgColor, DP_PEN_WIDTH_1);
        break;

    case DP_THREAT_SYMBOL_CODE_42:

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),                                  /* Code 2 */
                                 dThreatX_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos - 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

        break;

    case DP_THREAT_SYMBOL_CODE_43:

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos, dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos, dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);  //Code 3

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

        break;

    case DP_THREAT_SYMBOL_CODE_44:

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);  //Code 5

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos - 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

        break;

    case DP_THREAT_SYMBOL_CODE_45:

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos, dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos, dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);  //Code 4

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos - 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

        break;

    case DP_THREAT_SYMBOL_CODE_46:

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);    //Code 6

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos - 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos - 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),  dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos - 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

        break;

    case DP_THREAT_SYMBOL_CODE_47:

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1); //Code 7

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos - 3 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos - 3 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos + 3 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos + 3 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos - 3 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos + 3 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),fgColor, DP_PEN_WIDTH_1);

        break;

    case DP_THREAT_SYMBOL_CODE_49:

        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "S", DP_POLAR_PLOT_FONT, fgColor, in_SThreatSymbolInfo.ucThreatSymbolSize);
        iWidth = (dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2) + in_SThreatSymbolInfo.ucThreatSymbolSize) - (dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2));
        DP_RWR_PolarPlotDrawCircle(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2) + 3, (dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2)) - in_SThreatSymbolInfo.ucThreatSymbolSize - 8, iWidth - 6, iWidth / 2, fgColor, DP_PEN_WIDTH_1, 180 * 16, 180 * 16);

        break;

    case DP_THREAT_SYMBOL_CODE_50:

        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize - 8), "A", DP_POLAR_PLOT_FONT, fgColor, in_SThreatSymbolInfo.ucThreatSymbolSize);
#if 0
        DP_RWR_PolarPlotDrawLine(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos - 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                               dThreatX_Pos , dThreatY_Pos - 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos , dThreatY_Pos - 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 3),
                               dThreatX_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), (dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize)) - 5, fgColor, DP_PEN_WIDTH_1);
#endif
        DP_RWR_PolarPlotDrawLine(dThreatX_Pos, (dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize - 8)) - in_SThreatSymbolInfo.ucThreatSymbolSize,
                                 dThreatX_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2) + 2, (dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize)), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos, (dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize - 8)) - in_SThreatSymbolInfo.ucThreatSymbolSize,
                                 dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2) + 3, (dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize)) + 5, fgColor, DP_PEN_WIDTH_1);

        break;

    case DP_THREAT_SYMBOL_CODE_51:

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos - 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos,
                                 dThreatX_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos, fgColor, DP_PEN_WIDTH_1); // Code 11

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos - 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos,
                                 dThreatX_Pos - 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos +  2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos - 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos,
                                 dThreatX_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos,
                                 dThreatX_Pos, dThreatY_Pos - 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);


        break;

    case DP_THREAT_SYMBOL_CODE_52:

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos - 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos,
                                 dThreatX_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos, fgColor, DP_PEN_WIDTH_1);  //Code 12

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos - 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos,
                                 dThreatX_Pos - 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos +  2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos - 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos,
                                 dThreatX_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos,
                                 dThreatX_Pos, dThreatY_Pos - 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 4), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos,
                                 dThreatX_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos -  2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 4), fgColor, DP_PEN_WIDTH_1);
        break;

    case DP_THREAT_SYMBOL_CODE_53:

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1); // Code 13

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos, dThreatY_Pos - 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos, dThreatY_Pos - 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos - 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos - 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos - 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos, fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos, fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos - 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos,
                                 dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos,
                                 dThreatX_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), fgColor, DP_PEN_WIDTH_1);

        break;

    case DP_THREAT_SYMBOL_CODE_54:
#if 0
         DP_RWR_PolarPlotDrawLine(dThreatX_Pos - 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                dThreatX_Pos - 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1); //Code 14

         DP_RWR_PolarPlotDrawLine(dThreatX_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                dThreatX_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

         DP_RWR_PolarPlotDrawLine(dThreatX_Pos, dThreatY_Pos - 3 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 3),
                                dThreatX_Pos, dThreatY_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

         DP_RWR_PolarPlotDrawLine(dThreatX_Pos - 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                dThreatX_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

         DP_RWR_PolarPlotDrawLine(dThreatX_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                dThreatX_Pos - 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);
#endif
        DP_RWR_PolarPlotDrawLine(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize - 5), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 4),
                                 dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize - 5), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1); //Code 14

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize - 5), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 4),
                                 dThreatX_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize - 5), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 4), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize - 5), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 4),
                                 dThreatX_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize - 5), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize - 5), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize - 5), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos, dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos, dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize - 5), fgColor, DP_PEN_WIDTH_1);

        break;

    case DP_THREAT_SYMBOL_CODE_55:

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1); //Code 15

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos, dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos, dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos, dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos, dThreatY_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos, dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos, dThreatY_Pos - 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);
        break;

    case DP_THREAT_SYMBOL_CODE_56:

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos, dThreatY_Pos - 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos, dThreatY_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1); //Code 16

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos, dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 4),
                                 dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos, dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 4),
                                 dThreatX_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);
        break;

    case DP_THREAT_SYMBOL_CODE_57:

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos, dThreatY_Pos -  (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos, dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1); //Code 17

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos, dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 4), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 4), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1); //Code 17
#if 0
         DP_RWR_PolarPlotDrawLine(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                dThreatX_Pos - 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos - 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

         DP_RWR_PolarPlotDrawLine(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                dThreatX_Pos - 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 4), fgColor, DP_PEN_WIDTH_1);
#endif
        iWidth = (dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 4) + 2) - (dThreatY_Pos - 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2) + 2);
        DP_RWR_PolarPlotDrawCircle(dThreatX_Pos - 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2) - 2, (dThreatY_Pos - 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2) + 2), iWidth, iWidth - 2, fgColor, DP_PEN_WIDTH_1, 0, 80 * 16);
        DP_RWR_PolarPlotDrawCircle(dThreatX_Pos - 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2) - 2, (dThreatY_Pos - 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2) + 2), iWidth, iWidth - 2, fgColor, DP_PEN_WIDTH_1, 280 * 16, 90 * 16);
        break;

    case DP_THREAT_SYMBOL_CODE_58:

        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "R", DP_POLAR_PLOT_FONT, fgColor, in_SThreatSymbolInfo.ucThreatSymbolSize);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos,
                                 dThreatX_Pos, dThreatY_Pos +  (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), fgColor, DP_PEN_WIDTH_1);

        break;

    case DP_THREAT_SYMBOL_CODE_59:

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos, dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize),
                                 dThreatX_Pos, dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1); //Code 19

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos - 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);
#if 0
        DP_RWR_PolarPlotDrawLine(dThreatX_Pos - 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                           dThreatX_Pos, dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos - 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                           dThreatX_Pos - 2 * (2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 3)), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2) / 2, fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos - 2 * (2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 3)), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2) / 2,
                           dThreatX_Pos - 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos - ((in_SThreatSymbolInfo.ucThreatSymbolSize / 2) / 2) / 2, fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos - 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos - ((in_SThreatSymbolInfo.ucThreatSymbolSize / 2) / 2) / 2,
                           dThreatX_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos - ((in_SThreatSymbolInfo.ucThreatSymbolSize / 2) / 2) / 2, fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos - ((in_SThreatSymbolInfo.ucThreatSymbolSize / 2) / 2) / 2,
                            dThreatX_Pos + (2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 3)), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2) / 2, fgColor, DP_PEN_WIDTH_1);
#endif
        iWidth  = (dThreatX_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 3)) - (dThreatX_Pos - 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 3));
        iHeight = (dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2)) - (dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize));
        DP_RWR_PolarPlotDrawCircle(dThreatX_Pos - 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize), iWidth, iHeight, fgColor, DP_PEN_WIDTH_1, 90 * 16, 200 * 16);
        DP_RWR_PolarPlotDrawLine(dThreatX_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 4), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos + (2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 3)), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize) + 2, fgColor, DP_PEN_WIDTH_1);
        break;

    case DP_THREAT_SYMBOL_CODE_60:

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos - 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1); //Code 20

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos - 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos - 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 3), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

        break;

    case DP_THREAT_SYMBOL_CODE_61:

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos, dThreatY_Pos,
                                 dThreatX_Pos - 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos, fgColor, DP_PEN_WIDTH_1); //Code 20

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos, dThreatY_Pos,
                                 dThreatX_Pos, dThreatY_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos, dThreatY_Pos,
                                 dThreatX_Pos - (2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2)) + 2, dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos, dThreatY_Pos,
                                 dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2) - 2, fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos - 5, dThreatY_Pos - 5, dThreatX_Pos + 5, dThreatY_Pos + 5, fgColor, DP_PEN_WIDTH_1);
        DP_RWR_PolarPlotDrawLine(dThreatX_Pos - 5, dThreatY_Pos - 5, dThreatX_Pos + 5, dThreatY_Pos - 13, fgColor, DP_PEN_WIDTH_1);
        DP_RWR_PolarPlotDrawLine(dThreatX_Pos + 5, dThreatY_Pos - 13, dThreatX_Pos + 14, dThreatY_Pos - 6, fgColor, DP_PEN_WIDTH_1 );
        DP_RWR_PolarPlotDrawLine(dThreatX_Pos + 14, dThreatY_Pos - 6, dThreatX_Pos + 5, dThreatY_Pos + 5, fgColor, DP_PEN_WIDTH_1);
        break;

    case DP_THREAT_SYMBOL_CODE_62:

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos + in_SThreatSymbolInfo.ucThreatSymbolSize , dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1); // Code 21

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos, dThreatY_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos, dThreatY_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

        iWidth  = (dThreatX_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2)) - (dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2));
        iHeight = (dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2)) - (dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2));
        DP_RWR_PolarPlotDrawCircle(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), iWidth, iHeight, fgColor, DP_PEN_WIDTH_1, 0, 5760);
#if 0
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "?", DP_POLAR_PLOT_FONT, 0, 255, 0);

        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "S", DP_POLAR_PLOT_FONT, 0, 255, 0);
#endif
        break;

    case DP_THREAT_SYMBOL_CODE_63:
#if 0
        DP_RWR_PolarPlotDrawLine(dThreatX_Pos -  (in_SThreatSymbolInfo.ucThreatSymbolSize), dThreatY_Pos,
                                 dThreatX_Pos + in_SThreatSymbolInfo.ucThreatSymbolSize , dThreatY_Pos, fgColor, DP_PEN_WIDTH_1); //Code 22

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos -  (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos,
                                 dThreatX_Pos -  (in_SThreatSymbolInfo.ucThreatSymbolSize / 2) , dThreatY_Pos - 2 * ((in_SThreatSymbolInfo.ucThreatSymbolSize / 2)), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos -  (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos - 2 * ((in_SThreatSymbolInfo.ucThreatSymbolSize / 2)),
                                 dThreatX_Pos +  (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos, fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos -  (in_SThreatSymbolInfo.ucThreatSymbolSize) , dThreatY_Pos,
                                 dThreatX_Pos -  (in_SThreatSymbolInfo.ucThreatSymbolSize / 2) , dThreatY_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos +  (in_SThreatSymbolInfo.ucThreatSymbolSize), dThreatY_Pos,
                                 dThreatX_Pos +  (in_SThreatSymbolInfo.ucThreatSymbolSize / 2) , dThreatY_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos +  (in_SThreatSymbolInfo.ucThreatSymbolSize / 2) , dThreatY_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos -  (in_SThreatSymbolInfo.ucThreatSymbolSize / 2) , dThreatY_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);
#endif
        DP_RWR_PolarPlotDrawLine((dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize)) - 2, dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 (dThreatX_Pos + in_SThreatSymbolInfo.ucThreatSymbolSize) + 2, dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1); //Code 22

        DP_RWR_PolarPlotDrawLine((dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize)) - 2, dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 (dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 4)) - 6, dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize), fgColor, DP_PEN_WIDTH_1); //Code 22

        DP_RWR_PolarPlotDrawLine((dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 4)) - 6, dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize),
                                 (dThreatX_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 4)) + 6, dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize), fgColor, DP_PEN_WIDTH_1); //Code 22

        DP_RWR_PolarPlotDrawLine((dThreatX_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 4)) + 6, dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize),
                                 (dThreatX_Pos + in_SThreatSymbolInfo.ucThreatSymbolSize) + 2, dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1); //Code 22

        DP_RWR_PolarPlotDrawLine((dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 4)) - 6, dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 (dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 4)) - 6, dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1); //Code 22

        DP_RWR_PolarPlotDrawLine((dThreatX_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 4)) + 6, dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 (dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 4)) - 6, dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1); //Code 22

        break;
#if 0
    case DP_THREAT_SYMBOL_CODE_22:

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos + in_SThreatSymbolInfo.ucThreatSymbolSize , dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1); // Code 21

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos, dThreatY_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

        DP_RWR_PolarPlotDrawLine(dThreatX_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2),
                                 dThreatX_Pos, dThreatY_Pos + 2 * (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), fgColor, DP_PEN_WIDTH_1);

        iWidth  = (dThreatX_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2)) - (dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2));
        iHeight = (dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2)) - (dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2));
        DP_RWR_PolarPlotDrawCircle(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), iWidth, iHeight, DP_POLAR_PLOT_GREEN_COLOR_CODE, DP_PEN_WIDTH_1, 0, 5760);
        break;
#endif
    case DP_THREAT_SYMBOL_CODE_35:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "U", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_36:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "V", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_37:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "W", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_38:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 4), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "X", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_39:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 4), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "Y", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_40:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 4), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "Z", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_41:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "41", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_48:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "48", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_64:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "64", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_65:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "65", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_66:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "66", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_67:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "67", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_68:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "68", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_69:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "69", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_70:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "70", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_71:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "71", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_72:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "72", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_73:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "73", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_74:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "74", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_75:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "75", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_76:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "76", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_77:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "77", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_78:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "78", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_79:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "79", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_80:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "80", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_81:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "81", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_82:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "82", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_83:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "83", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_84:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "84", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_85:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "85", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_86:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "86", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_87:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "87", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_88:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "88", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_89:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "89", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_90:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "90", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_91:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "91", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_92:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "92", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_93:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "93", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_94:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "94", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_95:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "95", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_96:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "96", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_97:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "97", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_98:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "98", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_99:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "99", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_15:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 4), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "A", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_16:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 4), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "B", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_17:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 4), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "C", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_18:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 4), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "D", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_19:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 4), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "E", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_20:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 4), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "F", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_21:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 4), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "G", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_22:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 4), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "H", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_23:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos  - 2, dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "I", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_24:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 4), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "J", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_25:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 4), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "K", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_26:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 4), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "L", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_27:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 4), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "M", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_28:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 4), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "N", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_29:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 4), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "O", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_30:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 4), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "P", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_31:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 4), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "Q", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_32:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 4), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "R", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_33:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 4), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "S", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_34:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 4), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 2), "T", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_13:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - 3, dThreatY_Pos, ".", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_12:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - 3, dThreatY_Pos + 3, "-", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_11:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 4), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 4), "+", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_10:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 4), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 4), "9", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_9:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 4), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 4), "8", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_8:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 4), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 4), "7", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_7:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 4), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 4), "6", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_6:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 4), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 4), "5", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_5:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 4), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 4), "4", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_4:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 4), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 4), "3", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_3:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 4), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 4), "2", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_2:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 4), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 4), "1", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;

    case DP_THREAT_SYMBOL_CODE_1:
        DP_RWR_PolarPlotDrawText(dThreatX_Pos - (in_SThreatSymbolInfo.ucThreatSymbolSize / 4), dThreatY_Pos + (in_SThreatSymbolInfo.ucThreatSymbolSize / 4), "0", DP_POLAR_PLOT_FONT, fgColor, DP_LETTERS_FONT_SIZE);
        break;
    }
}

void dp_symbol_draw_polar_plot::DP_RWR_PolarPlotDrawLine(double in_dStartX, double in_dStartY, double in_dEndX, double in_dEndY, QColor in_obj_clr_Line, float in_fPenWidth)
{
    /* Standard QPainter function to avoid staircase effect on lines */
    m_obj_paint_Draw->setRenderHints(QPainter::Antialiasing, true);

    /* Setting up color and style of the pen */
    m_obj_Pen_Line.setColor(in_obj_clr_Line);
    m_obj_Pen_Line.setCapStyle(Qt::FlatCap);

    /* Adjusting and setting up the pen size based on the boundary width and height */
    m_obj_Pen_Line.setWidthF(in_fPenWidth);

    /* Setting up the pen to draw */
    m_obj_paint_Draw->setPen(m_obj_Pen_Line);

    /* Drawing the line */
    m_obj_paint_Draw->drawLine(in_dStartX, in_dStartY, in_dEndX, in_dEndY);
}

void dp_symbol_draw_polar_plot::DP_RWR_PolarPlotDrawCircle(float in_fStartX, float in_fStartY, float in_fDepth, float in_fWidth, QColor in_obj_clr_Circle, float in_fPenWidth, int in_iStartAngle, int in_iSpanAngle)
{
    /* Standard QPainter function to avoid staricase effect on lines */
    m_obj_paint_Draw->setRenderHints(QPainter::Antialiasing, true);

    /* Setting up color and size of the pen */
    m_obj_Pen_Line.setColor(in_obj_clr_Circle);
    m_obj_Pen_Line.setCapStyle(Qt::FlatCap);

    /* Adjusting and setting up the pen size based on the boundary width and height */
    m_obj_Pen_Line.setWidthF(in_fPenWidth);

    /* Setting up the pen to draw */
    m_obj_paint_Draw->setPen(m_obj_Pen_Line);

    /* Drawing the circle */
    QRectF Circle_Rect(in_fStartX, in_fStartY, in_fDepth, in_fWidth);
    m_obj_paint_Draw->drawArc(Circle_Rect, in_iStartAngle, in_iSpanAngle);
}

void dp_symbol_draw_polar_plot::DP_RWR_PolarPlotDrawText(float in_fx, float in_fy, QString in_sDegree, QString qsFontFamily, QColor in_obj_clr_SymbColor, int in_iFontSize)
{
    QFont obj_font_Text;

    /* Standard QPainter function to avoid Staircase effect on Lines */
    m_obj_paint_Draw->setRenderHints(QPainter::Antialiasing, true);

    /* Setting up color of the pen */
    m_obj_Pen_Line.setColor(in_obj_clr_SymbColor);

    /* Setting up the properties of the font */
    obj_font_Text = m_obj_paint_Draw->font();
    obj_font_Text.weight();
    obj_font_Text.setBold(true);
    obj_font_Text.setFamily(qsFontFamily);

    /* Adjusting and setting up the font size based on the boundary width and height */
    obj_font_Text.setPointSizeF(in_iFontSize);

    /* Setting up the font */
    m_obj_paint_Draw->setFont(obj_font_Text);

    /* Setting up the pen to draw */
    m_obj_paint_Draw->setPen(m_obj_Pen_Line);

    /* Drawing the text */
    m_obj_paint_Draw->drawText(in_fx, in_fy, in_sDegree);
}
