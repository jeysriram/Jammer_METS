/********************************************************************************
** Form generated from reading UI file 'dp_rwr_nav_flight_display.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DP_RWR_NAV_FLIGHT_DISPLAY_H
#define UI_DP_RWR_NAV_FLIGHT_DISPLAY_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include "NAV_Display_LIB/headers/qfi_EADI.h"

QT_BEGIN_NAMESPACE

class Ui_dp_rwr_nav_flight_display
{
public:
    qfi_EADI *obj_gph_vw_NAV_Display;

    void setupUi(QWidget *dp_rwr_nav_flight_display)
    {
        if (dp_rwr_nav_flight_display->objectName().isEmpty())
            dp_rwr_nav_flight_display->setObjectName(QString::fromUtf8("dp_rwr_nav_flight_display"));
        dp_rwr_nav_flight_display->resize(384, 363);
        obj_gph_vw_NAV_Display = new qfi_EADI(dp_rwr_nav_flight_display);
        obj_gph_vw_NAV_Display->setObjectName(QString::fromUtf8("obj_gph_vw_NAV_Display"));
        obj_gph_vw_NAV_Display->setGeometry(QRect(0, 0, 384, 363));
        obj_gph_vw_NAV_Display->setEnabled(false);
        obj_gph_vw_NAV_Display->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        obj_gph_vw_NAV_Display->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        retranslateUi(dp_rwr_nav_flight_display);

        QMetaObject::connectSlotsByName(dp_rwr_nav_flight_display);
    } // setupUi

    void retranslateUi(QWidget *dp_rwr_nav_flight_display)
    {
        dp_rwr_nav_flight_display->setWindowTitle(QCoreApplication::translate("dp_rwr_nav_flight_display", "Form", nullptr));
#if QT_CONFIG(accessibility)
        dp_rwr_nav_flight_display->setAccessibleName(QCoreApplication::translate("dp_rwr_nav_flight_display", "NAV_Display_Widget", nullptr));
#endif // QT_CONFIG(accessibility)
    } // retranslateUi

};

namespace Ui {
    class dp_rwr_nav_flight_display: public Ui_dp_rwr_nav_flight_display {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DP_RWR_NAV_FLIGHT_DISPLAY_H
