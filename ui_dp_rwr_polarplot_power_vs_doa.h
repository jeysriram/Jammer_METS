/********************************************************************************
** Form generated from reading UI file 'dp_rwr_polarplot_power_vs_doa.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DP_RWR_POLARPLOT_POWER_VS_DOA_H
#define UI_DP_RWR_POLARPLOT_POWER_VS_DOA_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_dp_rwr_polarplot_power_vs_doa
{
public:
    QGridLayout *gridLayout_5;
    QGroupBox *obj_grp_bx_PolarDispArea;
    QGroupBox *obj_grp_bx_PolarPlotConfig;
    QGridLayout *gridLayout_4;
    QGridLayout *gridLayout;
    QLabel *obj_lbl_StartAngle;
    QLabel *obj_lbl_Limit;
    QLineEdit *obj_ln_edt_StartAngle;
    QLabel *obj_lbl_StartFreq;
    QLineEdit *obj_ln_edt_StopAngle;
    QLineEdit *obj_ln_edt_StopFreq;
    QLabel *obj_lbl_StopAngle;
    QLineEdit *obj_ln_edt_Limit;
    QLineEdit *obj_ln_edt_StartFreq;
    QLabel *obj_lbl_StopFreq;
    QPushButton *obj_psh_btn_PlotSymbol;
    QGroupBox *obj_grp_bx_RotationParam;
    QWidget *layoutWidget_3;
    QGridLayout *gridLayout_2;
    QLineEdit *obj_ln_edt_Angle;
    QLabel *label_2;
    QLabel *obj_lbl_Degree;
    QPushButton *obj_psh_btn_StartRotation;
    QGroupBox *obj_grp_bx_GridLineParam;
    QWidget *layoutWidget;
    QGridLayout *gridLayout_3;
    QRadioButton *obj_rd_btn_GridLinesInVisible;
    QLabel *obj_lbl_GridLines;
    QRadioButton *obj_rd_btn_GridLinesVisible;

    void setupUi(QWidget *dp_rwr_polarplot_power_vs_doa)
    {
        if (dp_rwr_polarplot_power_vs_doa->objectName().isEmpty())
            dp_rwr_polarplot_power_vs_doa->setObjectName(QString::fromUtf8("dp_rwr_polarplot_power_vs_doa"));
        dp_rwr_polarplot_power_vs_doa->resize(429, 487);
        dp_rwr_polarplot_power_vs_doa->setMinimumSize(QSize(325, 325));
        gridLayout_5 = new QGridLayout(dp_rwr_polarplot_power_vs_doa);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        gridLayout_5->setContentsMargins(6, 6, 6, 6);
        obj_grp_bx_PolarDispArea = new QGroupBox(dp_rwr_polarplot_power_vs_doa);
        obj_grp_bx_PolarDispArea->setObjectName(QString::fromUtf8("obj_grp_bx_PolarDispArea"));
        obj_grp_bx_PolarDispArea->setMinimumSize(QSize(325, 325));
        obj_grp_bx_PolarDispArea->setMaximumSize(QSize(16777215, 16777215));
        obj_grp_bx_PolarDispArea->setFlat(false);

        gridLayout_5->addWidget(obj_grp_bx_PolarDispArea, 0, 0, 1, 1);

        obj_grp_bx_PolarPlotConfig = new QGroupBox(dp_rwr_polarplot_power_vs_doa);
        obj_grp_bx_PolarPlotConfig->setObjectName(QString::fromUtf8("obj_grp_bx_PolarPlotConfig"));
        gridLayout_4 = new QGridLayout(obj_grp_bx_PolarPlotConfig);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        obj_lbl_StartAngle = new QLabel(obj_grp_bx_PolarPlotConfig);
        obj_lbl_StartAngle->setObjectName(QString::fromUtf8("obj_lbl_StartAngle"));

        gridLayout->addWidget(obj_lbl_StartAngle, 1, 0, 1, 1);

        obj_lbl_Limit = new QLabel(obj_grp_bx_PolarPlotConfig);
        obj_lbl_Limit->setObjectName(QString::fromUtf8("obj_lbl_Limit"));

        gridLayout->addWidget(obj_lbl_Limit, 0, 5, 1, 1);

        obj_ln_edt_StartAngle = new QLineEdit(obj_grp_bx_PolarPlotConfig);
        obj_ln_edt_StartAngle->setObjectName(QString::fromUtf8("obj_ln_edt_StartAngle"));

        gridLayout->addWidget(obj_ln_edt_StartAngle, 1, 1, 1, 1);

        obj_lbl_StartFreq = new QLabel(obj_grp_bx_PolarPlotConfig);
        obj_lbl_StartFreq->setObjectName(QString::fromUtf8("obj_lbl_StartFreq"));

        gridLayout->addWidget(obj_lbl_StartFreq, 0, 0, 1, 1);

        obj_ln_edt_StopAngle = new QLineEdit(obj_grp_bx_PolarPlotConfig);
        obj_ln_edt_StopAngle->setObjectName(QString::fromUtf8("obj_ln_edt_StopAngle"));

        gridLayout->addWidget(obj_ln_edt_StopAngle, 1, 4, 1, 1);

        obj_ln_edt_StopFreq = new QLineEdit(obj_grp_bx_PolarPlotConfig);
        obj_ln_edt_StopFreq->setObjectName(QString::fromUtf8("obj_ln_edt_StopFreq"));

        gridLayout->addWidget(obj_ln_edt_StopFreq, 0, 4, 1, 1);

        obj_lbl_StopAngle = new QLabel(obj_grp_bx_PolarPlotConfig);
        obj_lbl_StopAngle->setObjectName(QString::fromUtf8("obj_lbl_StopAngle"));

        gridLayout->addWidget(obj_lbl_StopAngle, 1, 3, 1, 1);

        obj_ln_edt_Limit = new QLineEdit(obj_grp_bx_PolarPlotConfig);
        obj_ln_edt_Limit->setObjectName(QString::fromUtf8("obj_ln_edt_Limit"));

        gridLayout->addWidget(obj_ln_edt_Limit, 0, 6, 1, 1);

        obj_ln_edt_StartFreq = new QLineEdit(obj_grp_bx_PolarPlotConfig);
        obj_ln_edt_StartFreq->setObjectName(QString::fromUtf8("obj_ln_edt_StartFreq"));

        gridLayout->addWidget(obj_ln_edt_StartFreq, 0, 1, 1, 1);

        obj_lbl_StopFreq = new QLabel(obj_grp_bx_PolarPlotConfig);
        obj_lbl_StopFreq->setObjectName(QString::fromUtf8("obj_lbl_StopFreq"));

        gridLayout->addWidget(obj_lbl_StopFreq, 0, 3, 1, 1);

        obj_psh_btn_PlotSymbol = new QPushButton(obj_grp_bx_PolarPlotConfig);
        obj_psh_btn_PlotSymbol->setObjectName(QString::fromUtf8("obj_psh_btn_PlotSymbol"));

        gridLayout->addWidget(obj_psh_btn_PlotSymbol, 2, 3, 1, 1);


        gridLayout_4->addLayout(gridLayout, 0, 0, 1, 2);

        obj_grp_bx_RotationParam = new QGroupBox(obj_grp_bx_PolarPlotConfig);
        obj_grp_bx_RotationParam->setObjectName(QString::fromUtf8("obj_grp_bx_RotationParam"));
        layoutWidget_3 = new QWidget(obj_grp_bx_RotationParam);
        layoutWidget_3->setObjectName(QString::fromUtf8("layoutWidget_3"));
        layoutWidget_3->setGeometry(QRect(20, 20, 269, 51));
        gridLayout_2 = new QGridLayout(layoutWidget_3);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        obj_ln_edt_Angle = new QLineEdit(layoutWidget_3);
        obj_ln_edt_Angle->setObjectName(QString::fromUtf8("obj_ln_edt_Angle"));

        gridLayout_2->addWidget(obj_ln_edt_Angle, 0, 1, 1, 1);

        label_2 = new QLabel(layoutWidget_3);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_2->addWidget(label_2, 0, 2, 1, 1);

        obj_lbl_Degree = new QLabel(layoutWidget_3);
        obj_lbl_Degree->setObjectName(QString::fromUtf8("obj_lbl_Degree"));

        gridLayout_2->addWidget(obj_lbl_Degree, 0, 0, 1, 1);

        obj_psh_btn_StartRotation = new QPushButton(layoutWidget_3);
        obj_psh_btn_StartRotation->setObjectName(QString::fromUtf8("obj_psh_btn_StartRotation"));

        gridLayout_2->addWidget(obj_psh_btn_StartRotation, 0, 3, 1, 1);


        gridLayout_4->addWidget(obj_grp_bx_RotationParam, 1, 0, 1, 1);

        obj_grp_bx_GridLineParam = new QGroupBox(obj_grp_bx_PolarPlotConfig);
        obj_grp_bx_GridLineParam->setObjectName(QString::fromUtf8("obj_grp_bx_GridLineParam"));
        layoutWidget = new QWidget(obj_grp_bx_GridLineParam);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(20, 20, 181, 42));
        gridLayout_3 = new QGridLayout(layoutWidget);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        obj_rd_btn_GridLinesInVisible = new QRadioButton(layoutWidget);
        obj_rd_btn_GridLinesInVisible->setObjectName(QString::fromUtf8("obj_rd_btn_GridLinesInVisible"));

        gridLayout_3->addWidget(obj_rd_btn_GridLinesInVisible, 0, 2, 1, 1);

        obj_lbl_GridLines = new QLabel(layoutWidget);
        obj_lbl_GridLines->setObjectName(QString::fromUtf8("obj_lbl_GridLines"));

        gridLayout_3->addWidget(obj_lbl_GridLines, 0, 0, 1, 1);

        obj_rd_btn_GridLinesVisible = new QRadioButton(layoutWidget);
        obj_rd_btn_GridLinesVisible->setObjectName(QString::fromUtf8("obj_rd_btn_GridLinesVisible"));

        gridLayout_3->addWidget(obj_rd_btn_GridLinesVisible, 0, 1, 1, 1);


        gridLayout_4->addWidget(obj_grp_bx_GridLineParam, 1, 1, 1, 1);


        gridLayout_5->addWidget(obj_grp_bx_PolarPlotConfig, 1, 0, 1, 1);


        retranslateUi(dp_rwr_polarplot_power_vs_doa);

        QMetaObject::connectSlotsByName(dp_rwr_polarplot_power_vs_doa);
    } // setupUi

    void retranslateUi(QWidget *dp_rwr_polarplot_power_vs_doa)
    {
        dp_rwr_polarplot_power_vs_doa->setWindowTitle(QCoreApplication::translate("dp_rwr_polarplot_power_vs_doa", "Form", nullptr));
#if QT_CONFIG(accessibility)
        obj_grp_bx_PolarDispArea->setAccessibleName(QCoreApplication::translate("dp_rwr_polarplot_power_vs_doa", "gb_display", nullptr));
#endif // QT_CONFIG(accessibility)
        obj_grp_bx_PolarDispArea->setTitle(QString());
        obj_grp_bx_PolarPlotConfig->setTitle(QCoreApplication::translate("dp_rwr_polarplot_power_vs_doa", "Polar Plot Config", nullptr));
        obj_lbl_StartAngle->setText(QCoreApplication::translate("dp_rwr_polarplot_power_vs_doa", "Start Angle :", nullptr));
        obj_lbl_Limit->setText(QCoreApplication::translate("dp_rwr_polarplot_power_vs_doa", "Limit :", nullptr));
        obj_lbl_StartFreq->setText(QCoreApplication::translate("dp_rwr_polarplot_power_vs_doa", "Start Frequency :", nullptr));
        obj_lbl_StopAngle->setText(QCoreApplication::translate("dp_rwr_polarplot_power_vs_doa", "Stop Angle : ", nullptr));
        obj_lbl_StopFreq->setText(QCoreApplication::translate("dp_rwr_polarplot_power_vs_doa", "Stop Frequency :", nullptr));
        obj_psh_btn_PlotSymbol->setText(QCoreApplication::translate("dp_rwr_polarplot_power_vs_doa", "Plot", nullptr));
        obj_grp_bx_RotationParam->setTitle(QCoreApplication::translate("dp_rwr_polarplot_power_vs_doa", "Rotation Parameters", nullptr));
        label_2->setText(QCoreApplication::translate("dp_rwr_polarplot_power_vs_doa", "o", nullptr));
        obj_lbl_Degree->setText(QCoreApplication::translate("dp_rwr_polarplot_power_vs_doa", "Degree :", nullptr));
        obj_psh_btn_StartRotation->setText(QCoreApplication::translate("dp_rwr_polarplot_power_vs_doa", "Start", nullptr));
        obj_grp_bx_GridLineParam->setTitle(QCoreApplication::translate("dp_rwr_polarplot_power_vs_doa", "Grid Lines Visible/Invislible", nullptr));
        obj_rd_btn_GridLinesInVisible->setText(QCoreApplication::translate("dp_rwr_polarplot_power_vs_doa", "Invisible", nullptr));
        obj_lbl_GridLines->setText(QCoreApplication::translate("dp_rwr_polarplot_power_vs_doa", "Grid Lines :", nullptr));
        obj_rd_btn_GridLinesVisible->setText(QCoreApplication::translate("dp_rwr_polarplot_power_vs_doa", "Visible", nullptr));
    } // retranslateUi

};

namespace Ui {
    class dp_rwr_polarplot_power_vs_doa: public Ui_dp_rwr_polarplot_power_vs_doa {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DP_RWR_POLARPLOT_POWER_VS_DOA_H
