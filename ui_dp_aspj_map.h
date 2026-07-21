/********************************************************************************
** Form generated from reading UI file 'dp_aspj_map.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DP_ASPJ_MAP_H
#define UI_DP_ASPJ_MAP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_dp_aspj_map
{
public:
    QGridLayout *gridLayout_4;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QGridLayout *gridLayout;
    QGridLayout *dp_aspj_flightmap;

    void setupUi(QWidget *dp_aspj_map)
    {
        if (dp_aspj_map->objectName().isEmpty())
            dp_aspj_map->setObjectName(QString::fromUtf8("dp_aspj_map"));
        dp_aspj_map->resize(1271, 756);
        gridLayout_4 = new QGridLayout(dp_aspj_map);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        scrollArea = new QScrollArea(dp_aspj_map);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 1251, 736));
        gridLayout = new QGridLayout(scrollAreaWidgetContents);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        dp_aspj_flightmap = new QGridLayout();
        dp_aspj_flightmap->setObjectName(QString::fromUtf8("dp_aspj_flightmap"));

        gridLayout->addLayout(dp_aspj_flightmap, 0, 0, 1, 1);

        scrollArea->setWidget(scrollAreaWidgetContents);

        gridLayout_4->addWidget(scrollArea, 0, 0, 1, 1);


        retranslateUi(dp_aspj_map);

        QMetaObject::connectSlotsByName(dp_aspj_map);
    } // setupUi

    void retranslateUi(QWidget *dp_aspj_map)
    {
        dp_aspj_map->setWindowTitle(QCoreApplication::translate("dp_aspj_map", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class dp_aspj_map: public Ui_dp_aspj_map {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DP_ASPJ_MAP_H
