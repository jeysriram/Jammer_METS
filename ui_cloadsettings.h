/********************************************************************************
** Form generated from reading UI file 'cloadsettings.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLOADSETTINGS_H
#define UI_CLOADSETTINGS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CLoadSettings
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QTableWidget *tbw_loadSettings;
    QHBoxLayout *horizontalLayout;
    QPushButton *pb_loadSettings;
    QSpacerItem *horizontalSpacer;
    QPushButton *pb_close;

    void setupUi(QDialog *CLoadSettings)
    {
        if (CLoadSettings->objectName().isEmpty())
            CLoadSettings->setObjectName(QString::fromUtf8("CLoadSettings"));
        CLoadSettings->resize(800, 449);
        verticalLayout = new QVBoxLayout(CLoadSettings);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(CLoadSettings);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);

        verticalLayout->addWidget(label);

        tbw_loadSettings = new QTableWidget(CLoadSettings);
        if (tbw_loadSettings->columnCount() < 2)
            tbw_loadSettings->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tbw_loadSettings->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tbw_loadSettings->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        if (tbw_loadSettings->rowCount() < 3)
            tbw_loadSettings->setRowCount(3);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tbw_loadSettings->setVerticalHeaderItem(0, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tbw_loadSettings->setVerticalHeaderItem(1, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tbw_loadSettings->setVerticalHeaderItem(2, __qtablewidgetitem4);
        tbw_loadSettings->setObjectName(QString::fromUtf8("tbw_loadSettings"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tbw_loadSettings->sizePolicy().hasHeightForWidth());
        tbw_loadSettings->setSizePolicy(sizePolicy);
        tbw_loadSettings->setRowCount(3);
        tbw_loadSettings->setColumnCount(2);
        tbw_loadSettings->horizontalHeader()->setStretchLastSection(true);

        verticalLayout->addWidget(tbw_loadSettings);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pb_loadSettings = new QPushButton(CLoadSettings);
        pb_loadSettings->setObjectName(QString::fromUtf8("pb_loadSettings"));

        horizontalLayout->addWidget(pb_loadSettings);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pb_close = new QPushButton(CLoadSettings);
        pb_close->setObjectName(QString::fromUtf8("pb_close"));

        horizontalLayout->addWidget(pb_close);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(CLoadSettings);

        QMetaObject::connectSlotsByName(CLoadSettings);
    } // setupUi

    void retranslateUi(QDialog *CLoadSettings)
    {
        CLoadSettings->setWindowTitle(QCoreApplication::translate("CLoadSettings", "Load Settings", nullptr));
        label->setText(QCoreApplication::translate("CLoadSettings", "Load Settings", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tbw_loadSettings->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("CLoadSettings", "Parameters", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tbw_loadSettings->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("CLoadSettings", "File Path", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tbw_loadSettings->verticalHeaderItem(0);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("CLoadSettings", "Way Points", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tbw_loadSettings->verticalHeaderItem(1);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("CLoadSettings", "Emitter Locations", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tbw_loadSettings->verticalHeaderItem(2);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("CLoadSettings", "Speed Control", nullptr));
        pb_loadSettings->setText(QCoreApplication::translate("CLoadSettings", "Load Settings", nullptr));
        pb_close->setText(QCoreApplication::translate("CLoadSettings", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CLoadSettings: public Ui_CLoadSettings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLOADSETTINGS_H
