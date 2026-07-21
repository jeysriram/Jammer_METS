/********************************************************************************
** Form generated from reading UI file 'dp_rwr_flight_display.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DP_RWR_FLIGHT_DISPLAY_H
#define UI_DP_RWR_FLIGHT_DISPLAY_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_dp_rwr_flight_display
{
public:
    QGridLayout *gridLayout_11;
    QGroupBox *obj_grp_bx_FlightDispArea;
    QGroupBox *obj_grp_bx_FlightDispConfig;
    QGridLayout *gridLayout_10;
    QGridLayout *gridLayout_7;
    QGridLayout *gridLayout;
    QLabel *obj_lbl_StartFreq;
    QLineEdit *obj_ln_edt_StartFreq;
    QLabel *obj_lbl_StopFreq;
    QLineEdit *obj_ln_edt_StopFreq;
    QLabel *obj_lbl_Limit;
    QLineEdit *obj_ln_edt_Limit;
    QLabel *obj_lbl_StartAngle;
    QLineEdit *obj_ln_edt_StartAngle;
    QLabel *obj_lbl_StopAngle;
    QLineEdit *obj_ln_edt_StopAngle;
    QGridLayout *gridLayout_4;
    QSpacerItem *horizontalSpacer;
    QPushButton *obj_psh_btn_PlotSymbol;
    QSpacerItem *horizontalSpacer_2;
    QGridLayout *gridLayout_9;
    QSpacerItem *horizontalSpacer_3;
    QGridLayout *gridLayout_8;
    QGroupBox *obj_grp_bx_RotationParam;
    QGridLayout *gridLayout_6;
    QGridLayout *gridLayout_2;
    QLineEdit *obj_ln_edt_Angle;
    QLabel *label_2;
    QLabel *obj_lbl_Degree;
    QPushButton *obj_psh_btn_StartRotation;
    QGroupBox *obj_grp_bx_GridLineParam;
    QGridLayout *gridLayout_5;
    QGridLayout *gridLayout_3;
    QRadioButton *obj_rd_btn_GridLinesInVisible;
    QLabel *obj_lbl_GridLines;
    QRadioButton *obj_rd_btn_GridLinesVisible;
    QSpacerItem *horizontalSpacer_4;

    void setupUi(QWidget *dp_rwr_flight_display)
    {
        if (dp_rwr_flight_display->objectName().isEmpty())
            dp_rwr_flight_display->setObjectName(QString::fromUtf8("dp_rwr_flight_display"));
        dp_rwr_flight_display->resize(478, 531);
        gridLayout_11 = new QGridLayout(dp_rwr_flight_display);
        gridLayout_11->setSpacing(0);
        gridLayout_11->setObjectName(QString::fromUtf8("gridLayout_11"));
        gridLayout_11->setContentsMargins(0, 0, 0, 0);
        obj_grp_bx_FlightDispArea = new QGroupBox(dp_rwr_flight_display);
        obj_grp_bx_FlightDispArea->setObjectName(QString::fromUtf8("obj_grp_bx_FlightDispArea"));
        obj_grp_bx_FlightDispArea->setMinimumSize(QSize(325, 325));
        obj_grp_bx_FlightDispArea->setMaximumSize(QSize(16777215, 16777215));
        obj_grp_bx_FlightDispArea->setFlat(false);

        gridLayout_11->addWidget(obj_grp_bx_FlightDispArea, 0, 0, 1, 1);

        obj_grp_bx_FlightDispConfig = new QGroupBox(dp_rwr_flight_display);
        obj_grp_bx_FlightDispConfig->setObjectName(QString::fromUtf8("obj_grp_bx_FlightDispConfig"));
        gridLayout_10 = new QGridLayout(obj_grp_bx_FlightDispConfig);
        gridLayout_10->setObjectName(QString::fromUtf8("gridLayout_10"));
        gridLayout_7 = new QGridLayout();
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        obj_lbl_StartFreq = new QLabel(obj_grp_bx_FlightDispConfig);
        obj_lbl_StartFreq->setObjectName(QString::fromUtf8("obj_lbl_StartFreq"));

        gridLayout->addWidget(obj_lbl_StartFreq, 0, 0, 1, 1);

        obj_ln_edt_StartFreq = new QLineEdit(obj_grp_bx_FlightDispConfig);
        obj_ln_edt_StartFreq->setObjectName(QString::fromUtf8("obj_ln_edt_StartFreq"));

        gridLayout->addWidget(obj_ln_edt_StartFreq, 0, 1, 1, 1);

        obj_lbl_StopFreq = new QLabel(obj_grp_bx_FlightDispConfig);
        obj_lbl_StopFreq->setObjectName(QString::fromUtf8("obj_lbl_StopFreq"));

        gridLayout->addWidget(obj_lbl_StopFreq, 0, 2, 1, 1);

        obj_ln_edt_StopFreq = new QLineEdit(obj_grp_bx_FlightDispConfig);
        obj_ln_edt_StopFreq->setObjectName(QString::fromUtf8("obj_ln_edt_StopFreq"));

        gridLayout->addWidget(obj_ln_edt_StopFreq, 0, 3, 1, 1);

        obj_lbl_Limit = new QLabel(obj_grp_bx_FlightDispConfig);
        obj_lbl_Limit->setObjectName(QString::fromUtf8("obj_lbl_Limit"));

        gridLayout->addWidget(obj_lbl_Limit, 0, 4, 1, 1);

        obj_ln_edt_Limit = new QLineEdit(obj_grp_bx_FlightDispConfig);
        obj_ln_edt_Limit->setObjectName(QString::fromUtf8("obj_ln_edt_Limit"));

        gridLayout->addWidget(obj_ln_edt_Limit, 0, 5, 1, 1);

        obj_lbl_StartAngle = new QLabel(obj_grp_bx_FlightDispConfig);
        obj_lbl_StartAngle->setObjectName(QString::fromUtf8("obj_lbl_StartAngle"));

        gridLayout->addWidget(obj_lbl_StartAngle, 1, 0, 1, 1);

        obj_ln_edt_StartAngle = new QLineEdit(obj_grp_bx_FlightDispConfig);
        obj_ln_edt_StartAngle->setObjectName(QString::fromUtf8("obj_ln_edt_StartAngle"));

        gridLayout->addWidget(obj_ln_edt_StartAngle, 1, 1, 1, 1);

        obj_lbl_StopAngle = new QLabel(obj_grp_bx_FlightDispConfig);
        obj_lbl_StopAngle->setObjectName(QString::fromUtf8("obj_lbl_StopAngle"));

        gridLayout->addWidget(obj_lbl_StopAngle, 1, 2, 1, 1);

        obj_ln_edt_StopAngle = new QLineEdit(obj_grp_bx_FlightDispConfig);
        obj_ln_edt_StopAngle->setObjectName(QString::fromUtf8("obj_ln_edt_StopAngle"));

        gridLayout->addWidget(obj_ln_edt_StopAngle, 1, 3, 1, 1);


        gridLayout_7->addLayout(gridLayout, 0, 0, 1, 1);

        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer, 0, 0, 1, 1);

        obj_psh_btn_PlotSymbol = new QPushButton(obj_grp_bx_FlightDispConfig);
        obj_psh_btn_PlotSymbol->setObjectName(QString::fromUtf8("obj_psh_btn_PlotSymbol"));

        gridLayout_4->addWidget(obj_psh_btn_PlotSymbol, 0, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_2, 0, 2, 1, 1);


        gridLayout_7->addLayout(gridLayout_4, 1, 0, 1, 1);


        gridLayout_10->addLayout(gridLayout_7, 0, 0, 1, 1);

        gridLayout_9 = new QGridLayout();
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        horizontalSpacer_3 = new QSpacerItem(18, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_9->addItem(horizontalSpacer_3, 0, 0, 1, 1);

        gridLayout_8 = new QGridLayout();
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        obj_grp_bx_RotationParam = new QGroupBox(obj_grp_bx_FlightDispConfig);
        obj_grp_bx_RotationParam->setObjectName(QString::fromUtf8("obj_grp_bx_RotationParam"));
        gridLayout_6 = new QGridLayout(obj_grp_bx_RotationParam);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        obj_ln_edt_Angle = new QLineEdit(obj_grp_bx_RotationParam);
        obj_ln_edt_Angle->setObjectName(QString::fromUtf8("obj_ln_edt_Angle"));

        gridLayout_2->addWidget(obj_ln_edt_Angle, 0, 1, 1, 1);

        label_2 = new QLabel(obj_grp_bx_RotationParam);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_2->addWidget(label_2, 0, 2, 1, 1);

        obj_lbl_Degree = new QLabel(obj_grp_bx_RotationParam);
        obj_lbl_Degree->setObjectName(QString::fromUtf8("obj_lbl_Degree"));

        gridLayout_2->addWidget(obj_lbl_Degree, 0, 0, 1, 1);

        obj_psh_btn_StartRotation = new QPushButton(obj_grp_bx_RotationParam);
        obj_psh_btn_StartRotation->setObjectName(QString::fromUtf8("obj_psh_btn_StartRotation"));

        gridLayout_2->addWidget(obj_psh_btn_StartRotation, 0, 3, 1, 1);


        gridLayout_6->addLayout(gridLayout_2, 0, 0, 1, 1);


        gridLayout_8->addWidget(obj_grp_bx_RotationParam, 0, 0, 1, 1);

        obj_grp_bx_GridLineParam = new QGroupBox(obj_grp_bx_FlightDispConfig);
        obj_grp_bx_GridLineParam->setObjectName(QString::fromUtf8("obj_grp_bx_GridLineParam"));
        gridLayout_5 = new QGridLayout(obj_grp_bx_GridLineParam);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        obj_rd_btn_GridLinesInVisible = new QRadioButton(obj_grp_bx_GridLineParam);
        obj_rd_btn_GridLinesInVisible->setObjectName(QString::fromUtf8("obj_rd_btn_GridLinesInVisible"));

        gridLayout_3->addWidget(obj_rd_btn_GridLinesInVisible, 0, 2, 1, 1);

        obj_lbl_GridLines = new QLabel(obj_grp_bx_GridLineParam);
        obj_lbl_GridLines->setObjectName(QString::fromUtf8("obj_lbl_GridLines"));

        gridLayout_3->addWidget(obj_lbl_GridLines, 0, 0, 1, 1);

        obj_rd_btn_GridLinesVisible = new QRadioButton(obj_grp_bx_GridLineParam);
        obj_rd_btn_GridLinesVisible->setObjectName(QString::fromUtf8("obj_rd_btn_GridLinesVisible"));

        gridLayout_3->addWidget(obj_rd_btn_GridLinesVisible, 0, 1, 1, 1);


        gridLayout_5->addLayout(gridLayout_3, 0, 0, 1, 1);


        gridLayout_8->addWidget(obj_grp_bx_GridLineParam, 0, 1, 1, 1);


        gridLayout_9->addLayout(gridLayout_8, 0, 1, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(18, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_9->addItem(horizontalSpacer_4, 0, 2, 1, 1);


        gridLayout_10->addLayout(gridLayout_9, 1, 0, 1, 1);


        gridLayout_11->addWidget(obj_grp_bx_FlightDispConfig, 1, 0, 1, 1);


        retranslateUi(dp_rwr_flight_display);

        QMetaObject::connectSlotsByName(dp_rwr_flight_display);
    } // setupUi

    void retranslateUi(QWidget *dp_rwr_flight_display)
    {
        dp_rwr_flight_display->setWindowTitle(QCoreApplication::translate("dp_rwr_flight_display", "Form", nullptr));
#if QT_CONFIG(accessibility)
        obj_grp_bx_FlightDispArea->setAccessibleName(QCoreApplication::translate("dp_rwr_flight_display", "gb_display", nullptr));
#endif // QT_CONFIG(accessibility)
        obj_grp_bx_FlightDispArea->setTitle(QString());
        obj_grp_bx_FlightDispConfig->setTitle(QCoreApplication::translate("dp_rwr_flight_display", "Flight Display Config", nullptr));
        obj_lbl_StartFreq->setText(QCoreApplication::translate("dp_rwr_flight_display", "Start Frequency :", nullptr));
        obj_lbl_StopFreq->setText(QCoreApplication::translate("dp_rwr_flight_display", "Stop Frequency :", nullptr));
        obj_lbl_Limit->setText(QCoreApplication::translate("dp_rwr_flight_display", "Limit :", nullptr));
        obj_lbl_StartAngle->setText(QCoreApplication::translate("dp_rwr_flight_display", "Start Angle :", nullptr));
        obj_lbl_StopAngle->setText(QCoreApplication::translate("dp_rwr_flight_display", "Stop Angle : ", nullptr));
        obj_psh_btn_PlotSymbol->setText(QCoreApplication::translate("dp_rwr_flight_display", "Plot", nullptr));
        obj_grp_bx_RotationParam->setTitle(QCoreApplication::translate("dp_rwr_flight_display", "Rotation Parameters", nullptr));
        label_2->setText(QCoreApplication::translate("dp_rwr_flight_display", "o", nullptr));
        obj_lbl_Degree->setText(QCoreApplication::translate("dp_rwr_flight_display", "Degree :", nullptr));
        obj_psh_btn_StartRotation->setText(QCoreApplication::translate("dp_rwr_flight_display", "Start", nullptr));
        obj_grp_bx_GridLineParam->setTitle(QCoreApplication::translate("dp_rwr_flight_display", "Grid Lines Visible/Invislible", nullptr));
        obj_rd_btn_GridLinesInVisible->setText(QCoreApplication::translate("dp_rwr_flight_display", "Invisible", nullptr));
        obj_lbl_GridLines->setText(QCoreApplication::translate("dp_rwr_flight_display", "Grid Lines :", nullptr));
        obj_rd_btn_GridLinesVisible->setText(QCoreApplication::translate("dp_rwr_flight_display", "Visible", nullptr));
    } // retranslateUi

};

namespace Ui {
    class dp_rwr_flight_display: public Ui_dp_rwr_flight_display {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DP_RWR_FLIGHT_DISPLAY_H
