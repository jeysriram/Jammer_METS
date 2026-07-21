/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>
#include <dp_rwr_flight_display.h>
#include "dp_aspj_map.h"
#include "dp_rwr_ins_gps.h"
#include "dp_rwr_nav_flight_display.h"
#include "dp_rwr_polarplot_power_vs_doa.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionMap;
    QAction *actionSettings;
    QAction *actionDataLog;
    QAction *actionStartFlight;
    QAction *actionStopFlight;
    QAction *actionResetFlight;
    QAction *actionExit;
    QAction *actionAbout;
    QAction *actionDefault;
    QAction *actionCharcoal;
    QAction *actionLoad_Settings;
    QAction *actionLoad_Emitter_Locations;
    QAction *actionAdjust_Aircraft_Speed;
    QWidget *centralWidget;
    QGridLayout *gridLayout_5;
    QSplitter *splitter;
    QStackedWidget *jammer_sim_map;
    dp_aspj_map *page_2;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QGridLayout *gridLayout_3;
    QTableWidget *tbw_updateEmitterDisAngle;
    QWidget *page_3;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuTheme;
    QMenu *menuSettings;
    QDockWidget *dockWidget_flightDisplay;
    QWidget *dockWidgetContents_8;
    QGridLayout *gridLayout_4;
    QTabWidget *tw_flightDisplay;
    QWidget *tab;
    QGridLayout *gridLayout_6;
    dp_rwr_polarplot_power_vs_doa *graphicsView;
    QWidget *tab_2;
    QGridLayout *gridLayout_7;
    dp_rwr_flight_display *graphicsView_2;
    QDockWidget *dockWidget_actionLog;
    QWidget *dockWidgetContents_10;
    QGridLayout *gridLayout_2;
    QTextBrowser *tb_PrintLog;
    QDockWidget *dockWidget_InsNavDisp;
    QWidget *dockWidgetContents_4;
    QGridLayout *gridLayout_8;
    dp_rwr_nav_flight_display *obj_gph_vw_INS_GPS_Display;
    QStatusBar *statusBar;
    QDockWidget *dockWidget_InsNavDispDetails;
    QWidget *dockWidgetContents;
    QGridLayout *gridLayout;
    dp_rwr_ins_gps *obj_gph_vw_INS_GPS;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1082, 930);
        MainWindow->setStyleSheet(QString::fromUtf8(""));
        actionMap = new QAction(MainWindow);
        actionMap->setObjectName(QString::fromUtf8("actionMap"));
        QIcon icon;
        QString iconThemeName = QString::fromUtf8("map");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon = QIcon::fromTheme(iconThemeName);
        } else {
            icon.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        actionMap->setIcon(icon);
        actionSettings = new QAction(MainWindow);
        actionSettings->setObjectName(QString::fromUtf8("actionSettings"));
        actionDataLog = new QAction(MainWindow);
        actionDataLog->setObjectName(QString::fromUtf8("actionDataLog"));
        actionStartFlight = new QAction(MainWindow);
        actionStartFlight->setObjectName(QString::fromUtf8("actionStartFlight"));
        actionStopFlight = new QAction(MainWindow);
        actionStopFlight->setObjectName(QString::fromUtf8("actionStopFlight"));
        actionResetFlight = new QAction(MainWindow);
        actionResetFlight->setObjectName(QString::fromUtf8("actionResetFlight"));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionDefault = new QAction(MainWindow);
        actionDefault->setObjectName(QString::fromUtf8("actionDefault"));
        actionCharcoal = new QAction(MainWindow);
        actionCharcoal->setObjectName(QString::fromUtf8("actionCharcoal"));
        actionLoad_Settings = new QAction(MainWindow);
        actionLoad_Settings->setObjectName(QString::fromUtf8("actionLoad_Settings"));
        actionLoad_Emitter_Locations = new QAction(MainWindow);
        actionLoad_Emitter_Locations->setObjectName(QString::fromUtf8("actionLoad_Emitter_Locations"));
        actionAdjust_Aircraft_Speed = new QAction(MainWindow);
        actionAdjust_Aircraft_Speed->setObjectName(QString::fromUtf8("actionAdjust_Aircraft_Speed"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout_5 = new QGridLayout(centralWidget);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        splitter = new QSplitter(centralWidget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Vertical);
        splitter->setHandleWidth(5);
        jammer_sim_map = new QStackedWidget(splitter);
        jammer_sim_map->setObjectName(QString::fromUtf8("jammer_sim_map"));
        page_2 = new dp_aspj_map();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        jammer_sim_map->addWidget(page_2);
        splitter->addWidget(jammer_sim_map);
        stackedWidget = new QStackedWidget(splitter);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        gridLayout_3 = new QGridLayout(page);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        tbw_updateEmitterDisAngle = new QTableWidget(page);
        if (tbw_updateEmitterDisAngle->columnCount() < 7)
            tbw_updateEmitterDisAngle->setColumnCount(7);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tbw_updateEmitterDisAngle->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tbw_updateEmitterDisAngle->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tbw_updateEmitterDisAngle->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tbw_updateEmitterDisAngle->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tbw_updateEmitterDisAngle->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tbw_updateEmitterDisAngle->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tbw_updateEmitterDisAngle->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        tbw_updateEmitterDisAngle->setObjectName(QString::fromUtf8("tbw_updateEmitterDisAngle"));

        gridLayout_3->addWidget(tbw_updateEmitterDisAngle, 0, 0, 1, 1);

        stackedWidget->addWidget(page);
        page_3 = new QWidget();
        page_3->setObjectName(QString::fromUtf8("page_3"));
        stackedWidget->addWidget(page_3);
        splitter->addWidget(stackedWidget);

        gridLayout_5->addWidget(splitter, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1082, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuTheme = new QMenu(menuBar);
        menuTheme->setObjectName(QString::fromUtf8("menuTheme"));
        menuSettings = new QMenu(menuBar);
        menuSettings->setObjectName(QString::fromUtf8("menuSettings"));
        MainWindow->setMenuBar(menuBar);
        dockWidget_flightDisplay = new QDockWidget(MainWindow);
        dockWidget_flightDisplay->setObjectName(QString::fromUtf8("dockWidget_flightDisplay"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(dockWidget_flightDisplay->sizePolicy().hasHeightForWidth());
        dockWidget_flightDisplay->setSizePolicy(sizePolicy);
        dockWidget_flightDisplay->setMinimumSize(QSize(145, 156));
        QFont font;
        font.setFamily(QString::fromUtf8("MS Shell Dlg 2"));
        dockWidget_flightDisplay->setFont(font);
        dockWidget_flightDisplay->setFeatures(QDockWidget::AllDockWidgetFeatures);
        dockWidget_flightDisplay->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
        dockWidgetContents_8 = new QWidget();
        dockWidgetContents_8->setObjectName(QString::fromUtf8("dockWidgetContents_8"));
        gridLayout_4 = new QGridLayout(dockWidgetContents_8);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        tw_flightDisplay = new QTabWidget(dockWidgetContents_8);
        tw_flightDisplay->setObjectName(QString::fromUtf8("tw_flightDisplay"));
        tw_flightDisplay->setTabPosition(QTabWidget::South);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        gridLayout_6 = new QGridLayout(tab);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        graphicsView = new dp_rwr_polarplot_power_vs_doa(tab);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));

        gridLayout_6->addWidget(graphicsView, 0, 0, 1, 1);

        tw_flightDisplay->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        gridLayout_7 = new QGridLayout(tab_2);
        gridLayout_7->setSpacing(0);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        gridLayout_7->setContentsMargins(0, 0, 0, 0);
        graphicsView_2 = new dp_rwr_flight_display(tab_2);
        graphicsView_2->setObjectName(QString::fromUtf8("graphicsView_2"));

        gridLayout_7->addWidget(graphicsView_2, 0, 0, 1, 1);

        tw_flightDisplay->addTab(tab_2, QString());

        gridLayout_4->addWidget(tw_flightDisplay, 0, 0, 1, 1);

        dockWidget_flightDisplay->setWidget(dockWidgetContents_8);
        MainWindow->addDockWidget(Qt::RightDockWidgetArea, dockWidget_flightDisplay);
        dockWidget_actionLog = new QDockWidget(MainWindow);
        dockWidget_actionLog->setObjectName(QString::fromUtf8("dockWidget_actionLog"));
        dockWidget_actionLog->setFeatures(QDockWidget::AllDockWidgetFeatures);
        dockWidgetContents_10 = new QWidget();
        dockWidgetContents_10->setObjectName(QString::fromUtf8("dockWidgetContents_10"));
        gridLayout_2 = new QGridLayout(dockWidgetContents_10);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        tb_PrintLog = new QTextBrowser(dockWidgetContents_10);
        tb_PrintLog->setObjectName(QString::fromUtf8("tb_PrintLog"));

        gridLayout_2->addWidget(tb_PrintLog, 0, 0, 1, 1);

        dockWidget_actionLog->setWidget(dockWidgetContents_10);
        MainWindow->addDockWidget(Qt::RightDockWidgetArea, dockWidget_actionLog);
        dockWidget_InsNavDisp = new QDockWidget(MainWindow);
        dockWidget_InsNavDisp->setObjectName(QString::fromUtf8("dockWidget_InsNavDisp"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(dockWidget_InsNavDisp->sizePolicy().hasHeightForWidth());
        dockWidget_InsNavDisp->setSizePolicy(sizePolicy1);
        dockWidget_InsNavDisp->setMinimumSize(QSize(394, 398));
        dockWidget_InsNavDisp->setMaximumSize(QSize(524287, 524287));
        dockWidgetContents_4 = new QWidget();
        dockWidgetContents_4->setObjectName(QString::fromUtf8("dockWidgetContents_4"));
        gridLayout_8 = new QGridLayout(dockWidgetContents_4);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        obj_gph_vw_INS_GPS_Display = new dp_rwr_nav_flight_display(dockWidgetContents_4);
        obj_gph_vw_INS_GPS_Display->setObjectName(QString::fromUtf8("obj_gph_vw_INS_GPS_Display"));

        gridLayout_8->addWidget(obj_gph_vw_INS_GPS_Display, 0, 0, 1, 1);

        dockWidget_InsNavDisp->setWidget(dockWidgetContents_4);
        MainWindow->addDockWidget(Qt::RightDockWidgetArea, dockWidget_InsNavDisp);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        dockWidget_InsNavDispDetails = new QDockWidget(MainWindow);
        dockWidget_InsNavDispDetails->setObjectName(QString::fromUtf8("dockWidget_InsNavDispDetails"));
        sizePolicy.setHeightForWidth(dockWidget_InsNavDispDetails->sizePolicy().hasHeightForWidth());
        dockWidget_InsNavDispDetails->setSizePolicy(sizePolicy);
        dockWidget_InsNavDispDetails->setMaximumSize(QSize(524287, 524287));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        gridLayout = new QGridLayout(dockWidgetContents);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        obj_gph_vw_INS_GPS = new dp_rwr_ins_gps(dockWidgetContents);
        obj_gph_vw_INS_GPS->setObjectName(QString::fromUtf8("obj_gph_vw_INS_GPS"));

        gridLayout->addWidget(obj_gph_vw_INS_GPS, 0, 0, 1, 1);

        dockWidget_InsNavDispDetails->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(Qt::RightDockWidgetArea, dockWidget_InsNavDispDetails);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuTheme->menuAction());
        menuBar->addAction(menuSettings->menuAction());
        menuFile->addAction(actionDefault);
        menuTheme->addAction(actionCharcoal);
        menuSettings->addAction(actionLoad_Settings);

        retranslateUi(MainWindow);

        jammer_sim_map->setCurrentIndex(0);
        stackedWidget->setCurrentIndex(0);
        tw_flightDisplay->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "METS Dry Run Simulation", nullptr));
        actionMap->setText(QCoreApplication::translate("MainWindow", "Map View", nullptr));
#if QT_CONFIG(shortcut)
        actionMap->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+1", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSettings->setText(QCoreApplication::translate("MainWindow", "Settings", nullptr));
#if QT_CONFIG(shortcut)
        actionSettings->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+2", nullptr));
#endif // QT_CONFIG(shortcut)
        actionDataLog->setText(QCoreApplication::translate("MainWindow", "Data Log", nullptr));
#if QT_CONFIG(shortcut)
        actionDataLog->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+3", nullptr));
#endif // QT_CONFIG(shortcut)
        actionStartFlight->setText(QCoreApplication::translate("MainWindow", "\342\226\266 Start Flight", nullptr));
#if QT_CONFIG(shortcut)
        actionStartFlight->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+S", nullptr));
#endif // QT_CONFIG(shortcut)
        actionStopFlight->setText(QCoreApplication::translate("MainWindow", "\342\217\271 Stop Flight", nullptr));
#if QT_CONFIG(shortcut)
        actionStopFlight->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+X", nullptr));
#endif // QT_CONFIG(shortcut)
        actionResetFlight->setText(QCoreApplication::translate("MainWindow", "\342\206\272 Reset Flight", nullptr));
#if QT_CONFIG(shortcut)
        actionResetFlight->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+R", nullptr));
#endif // QT_CONFIG(shortcut)
        actionExit->setText(QCoreApplication::translate("MainWindow", "Exit", nullptr));
#if QT_CONFIG(shortcut)
        actionExit->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Q", nullptr));
#endif // QT_CONFIG(shortcut)
        actionAbout->setText(QCoreApplication::translate("MainWindow", "About", nullptr));
        actionDefault->setText(QCoreApplication::translate("MainWindow", " Default Layout", nullptr));
        actionCharcoal->setText(QCoreApplication::translate("MainWindow", "Charcoal", nullptr));
        actionLoad_Settings->setText(QCoreApplication::translate("MainWindow", "Load Settings", nullptr));
        actionLoad_Emitter_Locations->setText(QCoreApplication::translate("MainWindow", "Load Emitter Locations", nullptr));
        actionAdjust_Aircraft_Speed->setText(QCoreApplication::translate("MainWindow", "Adjust Aircraft Speed", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tbw_updateEmitterDisAngle->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "ENo", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tbw_updateEmitterDisAngle->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Lat", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tbw_updateEmitterDisAngle->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "lon", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tbw_updateEmitterDisAngle->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Dis(km)", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tbw_updateEmitterDisAngle->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "DOA", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tbw_updateEmitterDisAngle->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "Heading", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tbw_updateEmitterDisAngle->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("MainWindow", "AOA", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
        menuTheme->setTitle(QCoreApplication::translate("MainWindow", "Theme", nullptr));
        menuSettings->setTitle(QCoreApplication::translate("MainWindow", "Settings", nullptr));
        dockWidget_flightDisplay->setWindowTitle(QCoreApplication::translate("MainWindow", "Flight Display", nullptr));
        tw_flightDisplay->setTabText(tw_flightDisplay->indexOf(tab), QCoreApplication::translate("MainWindow", "Polar Plot - Amp vs DF", nullptr));
        tw_flightDisplay->setTabText(tw_flightDisplay->indexOf(tab_2), QCoreApplication::translate("MainWindow", "Flight Display - Amp vs DF", nullptr));
        dockWidget_actionLog->setWindowTitle(QCoreApplication::translate("MainWindow", "Action Log", nullptr));
        dockWidget_InsNavDisp->setWindowTitle(QCoreApplication::translate("MainWindow", "Nav Display", nullptr));
        dockWidget_InsNavDispDetails->setWindowTitle(QCoreApplication::translate("MainWindow", "Nav Display Details", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
