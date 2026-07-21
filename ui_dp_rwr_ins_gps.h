/********************************************************************************
** Form generated from reading UI file 'dp_rwr_ins_gps.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DP_RWR_INS_GPS_H
#define UI_DP_RWR_INS_GPS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_dp_rwr_ins_gps
{
public:
    QGridLayout *gridLayout_13;
    QSpacerItem *horizontalSpacer_6;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_12;
    QGridLayout *gridLayout_11;
    QGridLayout *gridLayout_10;
    QGridLayout *gridLayout_3;
    QSpacerItem *horizontalSpacer_2;
    QLabel *obj_lbl_GpsLed;
    QLabel *obj_lbl_Gps;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *horizontalSpacer_5;
    QGridLayout *gridLayout_7;
    QPushButton *obj_psh_btn_Read;
    QGridLayout *gridLayout_9;
    QGridLayout *gridLayout_4;
    QGridLayout *gridLayout;
    QLabel *obj_lbl_Latitude;
    QLabel *obj_lbl_Longitude;
    QLabel *obj_lbl_TrueHeading;
    QGridLayout *gridLayout_2;
    QLineEdit *obj_ln_edt_fLatitude;
    QLineEdit *obj_ln_edt_fLongitude;
    QLineEdit *obj_ln_edt_fTrueHeading;
    QGridLayout *gridLayout_8;
    QGridLayout *gridLayout_5;
    QLabel *obj_lbl_Roll;
    QLabel *obj_lbl_Pitch;
    QLabel *obj_lbl_Time;
    QGridLayout *gridLayout_6;
    QLineEdit *obj_ln_edt_fRoll;
    QLineEdit *obj_ln_edt_fPitch;
    QLineEdit *obj_ln_edt_iAltitude;
    QSpacerItem *horizontalSpacer_7;

    void setupUi(QWidget *dp_rwr_ins_gps)
    {
        if (dp_rwr_ins_gps->objectName().isEmpty())
            dp_rwr_ins_gps->setObjectName(QString::fromUtf8("dp_rwr_ins_gps"));
        dp_rwr_ins_gps->resize(316, 113);
        dp_rwr_ins_gps->setMaximumSize(QSize(16777215, 250));
        gridLayout_13 = new QGridLayout(dp_rwr_ins_gps);
        gridLayout_13->setSpacing(0);
        gridLayout_13->setObjectName(QString::fromUtf8("gridLayout_13"));
        gridLayout_13->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_6 = new QSpacerItem(22, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_13->addItem(horizontalSpacer_6, 0, 0, 1, 1);

        groupBox = new QGroupBox(dp_rwr_ins_gps);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout_12 = new QGridLayout(groupBox);
        gridLayout_12->setObjectName(QString::fromUtf8("gridLayout_12"));
        gridLayout_12->setHorizontalSpacing(6);
        gridLayout_12->setVerticalSpacing(1);
        gridLayout_12->setContentsMargins(0, 0, 0, 0);
        gridLayout_11 = new QGridLayout();
        gridLayout_11->setSpacing(2);
        gridLayout_11->setObjectName(QString::fromUtf8("gridLayout_11"));
        gridLayout_10 = new QGridLayout();
        gridLayout_10->setObjectName(QString::fromUtf8("gridLayout_10"));
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_2, 0, 0, 1, 1);

        obj_lbl_GpsLed = new QLabel(groupBox);
        obj_lbl_GpsLed->setObjectName(QString::fromUtf8("obj_lbl_GpsLed"));

        gridLayout_3->addWidget(obj_lbl_GpsLed, 0, 3, 1, 1);

        obj_lbl_Gps = new QLabel(groupBox);
        obj_lbl_Gps->setObjectName(QString::fromUtf8("obj_lbl_Gps"));

        gridLayout_3->addWidget(obj_lbl_Gps, 0, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer, 0, 2, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_5, 0, 4, 1, 1);


        gridLayout_10->addLayout(gridLayout_3, 0, 0, 1, 1);

        gridLayout_7 = new QGridLayout();
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        obj_psh_btn_Read = new QPushButton(groupBox);
        obj_psh_btn_Read->setObjectName(QString::fromUtf8("obj_psh_btn_Read"));

        gridLayout_7->addWidget(obj_psh_btn_Read, 0, 0, 1, 1);


        gridLayout_10->addLayout(gridLayout_7, 0, 1, 1, 1);


        gridLayout_11->addLayout(gridLayout_10, 0, 0, 1, 1);

        gridLayout_9 = new QGridLayout();
        gridLayout_9->setSpacing(2);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        gridLayout_9->setContentsMargins(3, -1, 3, 3);
        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout_4->setContentsMargins(-1, 0, -1, -1);
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        obj_lbl_Latitude = new QLabel(groupBox);
        obj_lbl_Latitude->setObjectName(QString::fromUtf8("obj_lbl_Latitude"));

        gridLayout->addWidget(obj_lbl_Latitude, 0, 0, 1, 1);

        obj_lbl_Longitude = new QLabel(groupBox);
        obj_lbl_Longitude->setObjectName(QString::fromUtf8("obj_lbl_Longitude"));

        gridLayout->addWidget(obj_lbl_Longitude, 1, 0, 1, 1);

        obj_lbl_TrueHeading = new QLabel(groupBox);
        obj_lbl_TrueHeading->setObjectName(QString::fromUtf8("obj_lbl_TrueHeading"));

        gridLayout->addWidget(obj_lbl_TrueHeading, 2, 0, 1, 1);


        gridLayout_4->addLayout(gridLayout, 0, 0, 1, 1);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        obj_ln_edt_fLatitude = new QLineEdit(groupBox);
        obj_ln_edt_fLatitude->setObjectName(QString::fromUtf8("obj_ln_edt_fLatitude"));
        obj_ln_edt_fLatitude->setEnabled(false);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(obj_ln_edt_fLatitude->sizePolicy().hasHeightForWidth());
        obj_ln_edt_fLatitude->setSizePolicy(sizePolicy);
        obj_ln_edt_fLatitude->setMinimumSize(QSize(90, 20));
        obj_ln_edt_fLatitude->setMaximumSize(QSize(90, 20));

        gridLayout_2->addWidget(obj_ln_edt_fLatitude, 0, 0, 1, 1);

        obj_ln_edt_fLongitude = new QLineEdit(groupBox);
        obj_ln_edt_fLongitude->setObjectName(QString::fromUtf8("obj_ln_edt_fLongitude"));
        obj_ln_edt_fLongitude->setEnabled(false);
        sizePolicy.setHeightForWidth(obj_ln_edt_fLongitude->sizePolicy().hasHeightForWidth());
        obj_ln_edt_fLongitude->setSizePolicy(sizePolicy);
        obj_ln_edt_fLongitude->setMinimumSize(QSize(90, 20));
        obj_ln_edt_fLongitude->setMaximumSize(QSize(90, 20));

        gridLayout_2->addWidget(obj_ln_edt_fLongitude, 1, 0, 1, 1);

        obj_ln_edt_fTrueHeading = new QLineEdit(groupBox);
        obj_ln_edt_fTrueHeading->setObjectName(QString::fromUtf8("obj_ln_edt_fTrueHeading"));
        obj_ln_edt_fTrueHeading->setEnabled(false);
        sizePolicy.setHeightForWidth(obj_ln_edt_fTrueHeading->sizePolicy().hasHeightForWidth());
        obj_ln_edt_fTrueHeading->setSizePolicy(sizePolicy);
        obj_ln_edt_fTrueHeading->setMinimumSize(QSize(0, 0));
        obj_ln_edt_fTrueHeading->setMaximumSize(QSize(90, 16777215));

        gridLayout_2->addWidget(obj_ln_edt_fTrueHeading, 2, 0, 1, 1);


        gridLayout_4->addLayout(gridLayout_2, 0, 1, 1, 1);


        gridLayout_9->addLayout(gridLayout_4, 0, 0, 1, 1);

        gridLayout_8 = new QGridLayout();
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        gridLayout_5 = new QGridLayout();
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        obj_lbl_Roll = new QLabel(groupBox);
        obj_lbl_Roll->setObjectName(QString::fromUtf8("obj_lbl_Roll"));

        gridLayout_5->addWidget(obj_lbl_Roll, 0, 0, 1, 1);

        obj_lbl_Pitch = new QLabel(groupBox);
        obj_lbl_Pitch->setObjectName(QString::fromUtf8("obj_lbl_Pitch"));

        gridLayout_5->addWidget(obj_lbl_Pitch, 1, 0, 1, 1);

        obj_lbl_Time = new QLabel(groupBox);
        obj_lbl_Time->setObjectName(QString::fromUtf8("obj_lbl_Time"));

        gridLayout_5->addWidget(obj_lbl_Time, 2, 0, 1, 1);


        gridLayout_8->addLayout(gridLayout_5, 0, 0, 1, 1);

        gridLayout_6 = new QGridLayout();
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        obj_ln_edt_fRoll = new QLineEdit(groupBox);
        obj_ln_edt_fRoll->setObjectName(QString::fromUtf8("obj_ln_edt_fRoll"));
        obj_ln_edt_fRoll->setEnabled(false);
        sizePolicy.setHeightForWidth(obj_ln_edt_fRoll->sizePolicy().hasHeightForWidth());
        obj_ln_edt_fRoll->setSizePolicy(sizePolicy);
        obj_ln_edt_fRoll->setMinimumSize(QSize(0, 0));
        obj_ln_edt_fRoll->setMaximumSize(QSize(90, 16777215));

        gridLayout_6->addWidget(obj_ln_edt_fRoll, 0, 0, 1, 1);

        obj_ln_edt_fPitch = new QLineEdit(groupBox);
        obj_ln_edt_fPitch->setObjectName(QString::fromUtf8("obj_ln_edt_fPitch"));
        obj_ln_edt_fPitch->setEnabled(false);
        sizePolicy.setHeightForWidth(obj_ln_edt_fPitch->sizePolicy().hasHeightForWidth());
        obj_ln_edt_fPitch->setSizePolicy(sizePolicy);
        obj_ln_edt_fPitch->setMinimumSize(QSize(0, 0));
        obj_ln_edt_fPitch->setMaximumSize(QSize(90, 16777215));

        gridLayout_6->addWidget(obj_ln_edt_fPitch, 1, 0, 1, 1);

        obj_ln_edt_iAltitude = new QLineEdit(groupBox);
        obj_ln_edt_iAltitude->setObjectName(QString::fromUtf8("obj_ln_edt_iAltitude"));
        obj_ln_edt_iAltitude->setEnabled(false);
        sizePolicy.setHeightForWidth(obj_ln_edt_iAltitude->sizePolicy().hasHeightForWidth());
        obj_ln_edt_iAltitude->setSizePolicy(sizePolicy);
        obj_ln_edt_iAltitude->setMinimumSize(QSize(0, 0));
        obj_ln_edt_iAltitude->setMaximumSize(QSize(90, 16777215));

        gridLayout_6->addWidget(obj_ln_edt_iAltitude, 2, 0, 1, 1);


        gridLayout_8->addLayout(gridLayout_6, 0, 1, 1, 1);


        gridLayout_9->addLayout(gridLayout_8, 0, 1, 1, 1);


        gridLayout_11->addLayout(gridLayout_9, 1, 0, 1, 1);


        gridLayout_12->addLayout(gridLayout_11, 0, 0, 1, 1);


        gridLayout_13->addWidget(groupBox, 0, 1, 1, 1);

        horizontalSpacer_7 = new QSpacerItem(22, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_13->addItem(horizontalSpacer_7, 0, 2, 1, 1);


        retranslateUi(dp_rwr_ins_gps);

        QMetaObject::connectSlotsByName(dp_rwr_ins_gps);
    } // setupUi

    void retranslateUi(QWidget *dp_rwr_ins_gps)
    {
        dp_rwr_ins_gps->setWindowTitle(QCoreApplication::translate("dp_rwr_ins_gps", "Form", nullptr));
        groupBox->setTitle(QString());
        obj_lbl_GpsLed->setText(QCoreApplication::translate("dp_rwr_ins_gps", "LED", nullptr));
        obj_lbl_Gps->setText(QCoreApplication::translate("dp_rwr_ins_gps", "INS Fix", nullptr));
        obj_psh_btn_Read->setText(QCoreApplication::translate("dp_rwr_ins_gps", "Read", nullptr));
        obj_lbl_Latitude->setText(QCoreApplication::translate("dp_rwr_ins_gps", " Latitude (\302\260)", nullptr));
        obj_lbl_Longitude->setText(QCoreApplication::translate("dp_rwr_ins_gps", " Longitude (\302\260)", nullptr));
        obj_lbl_TrueHeading->setText(QCoreApplication::translate("dp_rwr_ins_gps", "True Heading (\302\260)", nullptr));
        obj_lbl_Roll->setText(QCoreApplication::translate("dp_rwr_ins_gps", "Roll (\302\260)", nullptr));
        obj_lbl_Pitch->setText(QCoreApplication::translate("dp_rwr_ins_gps", "Pitch (\302\260)", nullptr));
        obj_lbl_Time->setText(QCoreApplication::translate("dp_rwr_ins_gps", "Altitude (m)", nullptr));
    } // retranslateUi

};

namespace Ui {
    class dp_rwr_ins_gps: public Ui_dp_rwr_ins_gps {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DP_RWR_INS_GPS_H
