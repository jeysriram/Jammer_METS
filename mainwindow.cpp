/**
 * \file   mainwindow.cpp
 * \brief  Main application window (MainWindow) implementation.
 *
 * Owns the C++ data models via the global handle, wires the model/UI signals,
 * builds the flight-playback dock, applies the theme, prints the action log and
 * fills the emitter detail table for the current flight segment.
 *
 * \version 1.00
 *
 * \Copyright (C) 2026 Data Patterns (India) Ltd.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QSet>

#include "dp_aspj_comm.h"
#include "dp_aspj_map.h"
#include <QMetaType>
#include <QCoreApplication>
#include <QDir>
#include <QToolBar>
#include <QToolButton>
#include <QAction>
#include <QDockWidget>
#include <QStyle>
#include <QResizeEvent>

/// - Register the emitter-detection pointer type for queued signal/slot use
Q_DECLARE_METATYPE(S_DP_ASPJ_EMIT_WITHIN_RANGE*)

/// - Single global application handle instance
S_DP_ASPJ_HANDLE g_SDP_ASPJ_Handle;

/*!
 *  \fn     static QString DP_LoadStyleSheet(const QString &in_qsRelativeQssPath)
 *  \brief  Reads a .qss stylesheet resolved against the application directory
 *          (not the process's current working directory), so themes load
 *          correctly regardless of where the app is launched from — Qt
 *          Creator's default run working directory is the build output
 *          folder, which does not contain Res/Style. Logs and returns an
 *          empty string if the file cannot be opened, instead of silently
 *          clearing the window's stylesheet.
 *
 *  \param [in] in_qsRelativeQssPath : .qss path relative to the application directory.
 *
 *  \author Sriram J
 *  \date   18 July 2026
 *
 *  \retval : Stylesheet text, or an empty string on failure.
 */
static QString DP_LoadStyleSheet(const QString &in_qsRelativeQssPath)
{
    /// - Resolved path and stylesheet text, declared up front
    QString qsAbsolutePath;
    QString qsStylesheet;
    QFile qfFile;

    qsAbsolutePath = QDir(QCoreApplication::applicationDirPath()).filePath(in_qsRelativeQssPath);
    qfFile.setFileName(qsAbsolutePath);

    if (!qfFile.open(QFile::ReadOnly))
    {
        qDebug() << "Cannot open stylesheet:" << qsAbsolutePath;
    }
    else
    {
        qsStylesheet = QLatin1String(qfFile.readAll());
    }

    return qsStylesheet;
}

/*!
 *  \fn     MainWindow::MainWindow(QWidget *in_pqParent)
 *  \brief  Constructor — builds the UI and runs the GUI initialisation.
 *
 *  \param [in] in_pqParent : Parent widget for ownership.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
MainWindow::MainWindow(QWidget *in_pqParent)
    : QMainWindow(in_pqParent)
    , ui(new Ui::MainWindow)
{
    /// - Build the widget tree from the generated UI
    ui->setupUi(this);

    /// - Run the one-time GUI setup (models, signals, theme, docks)
    Update_GUI();
}

/*!
 *  \fn     MainWindow::~MainWindow()
 *  \brief  Destructor — releases the generated UI object.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
MainWindow::~MainWindow()
{
    /// - Free the generated UI object
    delete ui;

    if(cm_pobj_qStatusTimer_label)
    {
        delete cm_pobj_qStatusTimer_label;
        cm_pobj_qStatusTimer_label = nullptr;
    }
}

/*!
 *  \fn     void MainWindow::setupQmlContext()
 *  \brief  Legacy stub. QML model setup is now handled by
 *          dp_aspj_map::setupQmlContext(); kept to satisfy the declaration.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void MainWindow::setupQmlContext()
{
    /// - Intentionally empty (model setup moved to dp_aspj_map::setupQmlContext())
}

/*!
 *  \fn     void MainWindow::Update_GUI()
 *  \brief  One-time GUI initialisation: globals, signal/slot wiring, unused
 *          control removal, theme, table setup, model binding and the flight
 *          playback dock.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void MainWindow::Update_GUI()
{
    /// - Local widget pointers, declared and initialised up front
    QTableWidget              *pTbl = nullptr;
    dp_aspj_map               *pMapWidget = nullptr;

    /// - Create memory for Status bar QLabel
    cm_pobj_qStatusTimer_label = new QLabel(this);

    /// - Initialise the global model/handle objects
    Init_Globalvariable();

    /// - Connect the model/UI signals and slots
    SignalSlots_Function();

    /// - Initialisation and adding an wigets  into an StatusBar..
    On_Init_StatusBar();

    /// - Remove/hide controls that are not needed
    Remove_Unused_Controls();

    /// - Move Action Log / Flight Playback Control behind a left-side toggle icon
    SetupLeftTogglePanel();

    /// - Apply the default (Charcoal) theme
    on_action_Default_Theme_triggered();

    /// - Install event filter on the stacked widget to catch right-clicks
    ///   from child widgets (dp_aspj_map, QQuickWidget) and show the context menu
    ui->jammer_sim_map->installEventFilter(this);

    /// - Fit the emitter table columns and hide its horizontal scrollbar
    pTbl = ui->tbw_updateEmitterDisAngle;
    pTbl->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    pTbl->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    pTbl->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    pTbl->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);

    /// - Apply splitter handle style so it remains visible after theme is set
    ui->splitter->setObjectName("mainSplitter");
    ui->splitter->setStyleSheet(
        "#mainSplitter::handle { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 rgba(80, 120, 160, 0), stop:0.5 rgba(80, 120, 160, 180), stop:1 rgba(80, 120, 160, 0)); } "
        "#mainSplitter::handle:hover { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 rgba(80, 120, 160, 0), stop:0.5 rgba(100, 160, 220, 255), stop:1 rgba(80, 120, 160, 0)); }"
    );

    /// - Fix splitter ratio: map takes ~75%, table takes ~25%
    /// - Map stretches to fill extra space; table keeps its size
    ui->splitter->setSizes({750, 250});
    ui->splitter->setStretchFactor(0, 1); // map: stretches
    ui->splitter->setStretchFactor(1, 0); // table: fixed

    /// - Fetch the map widget from the stacked widget and share the models
    pMapWidget = qobject_cast<dp_aspj_map*>(ui->jammer_sim_map->widget(0));
    if (pMapWidget != nullptr)
    {
        pMapWidget->DP_BindMapModelToQML(g_SDP_ASPJ_Handle.pm_cobj_emitterModel, g_SDP_ASPJ_Handle.pm_cobj_flightController);
    }

    /// - Save the initial dock layout and window geometry
    cm_qbarrState = saveState(0);
    cm_qbarrGeometry = saveGeometry();
}

/*!
 *  \fn     void MainWindow::Init_Globalvariable()
 *  \brief  Creates the model objects, points the cockpit handle at the promoted
 *          widget and wires the models together.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void MainWindow::Init_Globalvariable()
{
    /// - Create the emitter model
    g_SDP_ASPJ_Handle.pm_cobj_emitterModel = new CDP_EmitterModel(this);

    /// - Create the flight controller
    g_SDP_ASPJ_Handle.pm_cobj_flightController = new CDP_FlightController(this);

    /// - Create the load-settings dialog
    g_SDP_ASPJ_Handle.pm_cobj_loadSettings = new CLoadSettings(this);

    /// - Create the nav display flights
    g_SDP_ASPJ_Handle.pm_cobj_navflightDisplay = new dp_rwr_nav_flight_display();

    /// - Add the flight playback dock directly after the flight display dock
    g_SDP_ASPJ_Handle.pm_cobj_flightPlaybackCtrl = new CDP_FlightPlaybackControl(this);
    addDockWidget(Qt::RightDockWidgetArea, g_SDP_ASPJ_Handle.pm_cobj_flightPlaybackCtrl);

    /// - The cockpit display is created by the UI (promoted widget); point the
    ///   global handle at that instance so signals/handle access are valid
    g_SDP_ASPJ_Handle.pm_cobj_flightCockPitDisplay = ui->graphicsView_2;

    /// - Connect the emitter model to the flight controller
    g_SDP_ASPJ_Handle.pm_cobj_emitterModel->setFlightController(g_SDP_ASPJ_Handle.pm_cobj_flightController);

    /// - Connect the load-settings dialog to the emitter model and flight controller
    g_SDP_ASPJ_Handle.pm_cobj_loadSettings->setEmitterModel(g_SDP_ASPJ_Handle.pm_cobj_emitterModel);
    g_SDP_ASPJ_Handle.pm_cobj_loadSettings->setFlightController(g_SDP_ASPJ_Handle.pm_cobj_flightController);

    /// - Connect the flight playback control dock widget to the flight controller
    g_SDP_ASPJ_Handle.pm_cobj_flightPlaybackCtrl->setController(g_SDP_ASPJ_Handle.pm_cobj_flightController);
}

/*!
 *  \fn     void MainWindow::on_action_Default_Theme_triggered()
 *  \brief  Applies the default (Charcoal) stylesheet to the window.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void MainWindow::on_action_Default_Theme_triggered()
{
    /// - Read the QSS file (resolved against the app directory) and apply it
    ///   as the window stylesheet
    setStyleSheet(DP_LoadStyleSheet("Res/Style/Charcoal.qss"));
}

/*!
 *  \fn     void MainWindow::on_actionLoad_Settings_triggered()
 *  \brief  Opens the modal Load Settings dialog.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void MainWindow::on_actionLoad_Settings_triggered()
{
    /// - Show the load-settings dialog modally
    g_SDP_ASPJ_Handle.pm_cobj_loadSettings->exec();
}

/*!
 *  \fn     void MainWindow::on_actionDefault_triggered()
 *  \brief  Restores the saved dock layout/geometry, maximises the window and
 *          re-saves the state.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void MainWindow::on_actionDefault_triggered()
{
    /// - Restore the saved dock layout and geometry, then maximise
    restoreState(cm_qbarrState, 0);
    restoreGeometry(cm_qbarrGeometry);
    this->setWindowState(Qt::WindowMaximized);

    /// - Re-save the (now restored/maximised) state
    cm_qbarrState = saveState(0);
    cm_qbarrGeometry = saveGeometry();
}

/*!
 *  \fn     void MainWindow::SignalSlots_Function()
 *  \brief  Connects the flight controller and cockpit display signals to the
 *          MainWindow slots.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void MainWindow::SignalSlots_Function()
{
    /// - Flight controller log messages -> action log
    connect(g_SDP_ASPJ_Handle.pm_cobj_flightController, SIGNAL(Sig_PrintLog(QString,short)), this, SLOT(PrintLog(QString,short)));

    /// - Cockpit display log messages -> action log
    connect(g_SDP_ASPJ_Handle.pm_cobj_flightCockPitDisplay, SIGNAL(Sig_PrintLog(QString,short)), this, SLOT(PrintLog(QString,short)));

    /// - Detected-emitter details -> emitter detail table (QML-triggered, kept for backward compat)
    connect(g_SDP_ASPJ_Handle.pm_cobj_flightController, SIGNAL(Sig_updateEmitterDetailsToTable(S_DP_ASPJ_EMIT_WITHIN_RANGE*,int,int)), this, SLOT(Slot_updateEmitterDetailsToTable(S_DP_ASPJ_EMIT_WITHIN_RANGE*,int,int)));

    /// - Full detection record -> emitter detail table (C++-driven, runs every timer tick)
    connect(g_SDP_ASPJ_Handle.pm_cobj_flightController, SIGNAL(Sig_updateEmitterTable(S_DP_ASPJ_EMIT_WITHIN_RANGE*,int)), this, SLOT(Slot_updateEmitterTable(S_DP_ASPJ_EMIT_WITHIN_RANGE*,int)));

    /// - INS GPS - Signal to update the lat, long, roll data in INS GNS panel */
    connect(g_SDP_ASPJ_Handle.pm_cobj_flightController, SIGNAL(Sig_update_GPS_Data_to_nav_display(S_DP_ASPJ_EMIT_WITHIN_RANGE*)), ui->obj_gph_vw_INS_GPS, SLOT(Slot_update_INS_Data_To_NAV_Disp(S_DP_ASPJ_EMIT_WITHIN_RANGE*)));

    /// - INS GPS - Signal to update the lat, long, roll data in INS GNS panel */
    connect(g_SDP_ASPJ_Handle.pm_cobj_flightController, SIGNAL(Sig_update_GPS_Data_to_nav_display(S_DP_ASPJ_EMIT_WITHIN_RANGE*)), ui->obj_gph_vw_INS_GPS_Display, SLOT(Slot_UpdateNavFlightDisplay(S_DP_ASPJ_EMIT_WITHIN_RANGE*)));

}

/*!
 *  \fn     void MainWindow::Remove_Unused_Controls()
 *  \brief  Hides controls that are not needed at startup.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void MainWindow::Remove_Unused_Controls()
{
    /// - Hide the load-settings dialog initially
    g_SDP_ASPJ_Handle.pm_cobj_loadSettings->hide();

    /// - Remove the (unused) polar-plot tab
    ui->tw_flightDisplay->removeTab(0);

    /// - Hide action log
    //ui->dockWidget_actionLog->hide();

    /// - Hide flight playback control
    //g_SDP_ASPJ_Handle.pm_cobj_flightPlaybackCtrl->hide();

}

/*!
 *  \fn     void MainWindow::SetupLeftTogglePanel()
 *  \brief  Pre-docks Action Log / Flight Playback Control into the left dock
 *          area (stacked, hidden) and adds a single floating checkable
 *          right-arrow icon button, vertically centred on the left edge of
 *          the window (no toolbar strip reserved — it floats over the
 *          content), that shows/hides them there. The icon flips to a left
 *          arrow while the panels are shown.
 *
 *  \author Sriram J
 *  \date   19 July 2026
 *
 *  \retval : NA
 */
void MainWindow::SetupLeftTogglePanel()
{
    /// - Restrict Action Log / Flight Playback Control to the left area only,
    ///   pre-dock them there (stacked, explicit order) and keep them hidden
    ///   until the toggle button below reveals them
    ui->dockWidget_actionLog->setAllowedAreas(Qt::LeftDockWidgetArea);
    g_SDP_ASPJ_Handle.pm_cobj_flightPlaybackCtrl->setAllowedAreas(Qt::LeftDockWidgetArea);

    addDockWidget(Qt::LeftDockWidgetArea, ui->dockWidget_actionLog);
    splitDockWidget(ui->dockWidget_actionLog, g_SDP_ASPJ_Handle.pm_cobj_flightPlaybackCtrl, Qt::Vertical);

    ui->dockWidget_actionLog->hide();
    g_SDP_ASPJ_Handle.pm_cobj_flightPlaybackCtrl->hide();

    /// - Floating toggle button, parented directly to the main window (not
    ///   layout-managed) so it overlays content instead of reserving space
    cm_pobj_btnTogglePanels = new QToolButton(this);
    cm_pobj_btnTogglePanels->setIcon(style()->standardIcon(QStyle::SP_ArrowRight));
    cm_pobj_btnTogglePanels->setIconSize(QSize(18, 18));
    cm_pobj_btnTogglePanels->setFixedSize(32, 32);
    cm_pobj_btnTogglePanels->setCheckable(true);
    cm_pobj_btnTogglePanels->setChecked(false);
    cm_pobj_btnTogglePanels->setCursor(Qt::PointingHandCursor);
    cm_pobj_btnTogglePanels->setToolTip(tr("Show/Hide Panels"));
    cm_pobj_btnTogglePanels->setStyleSheet(
        "QToolButton { background-color: rgba(30, 35, 45, 190); border: 1px solid rgba(255, 255, 255, 60); "
        "border-top-right-radius: 16px; border-bottom-right-radius: 16px; border-top-left-radius: 0px; border-bottom-left-radius: 0px; } "
        "QToolButton:hover { background-color: rgba(50, 60, 75, 220); } "
        "QToolButton:checked { background-color: rgba(70, 110, 150, 220); }"
    );

    connect(cm_pobj_btnTogglePanels, &QToolButton::toggled, this, [this](bool in_bChecked)
    {
        QDockWidget *pActionLogDock = ui->dockWidget_actionLog;
        QDockWidget *pPlaybackDock  = g_SDP_ASPJ_Handle.pm_cobj_flightPlaybackCtrl;

        if (in_bChecked)
        {
            pActionLogDock->show();
            pPlaybackDock->show();
            cm_pobj_btnTogglePanels->setIcon(style()->standardIcon(QStyle::SP_ArrowLeft));
        }
        else
        {
            pActionLogDock->hide();
            pPlaybackDock->hide();
            cm_pobj_btnTogglePanels->setIcon(style()->standardIcon(QStyle::SP_ArrowRight));
        }
    });

    /// - Position at the left edge, vertically centred, above other widgets
    cm_pobj_btnTogglePanels->move(0, (this->height() - cm_pobj_btnTogglePanels->height()) / 2);
    cm_pobj_btnTogglePanels->raise();
    cm_pobj_btnTogglePanels->show();
}

/*!
 *  \fn     void MainWindow::resizeEvent(QResizeEvent *event)
 *  \brief  Keeps the floating left-edge panel-toggle button vertically
 *          centred whenever the window is resized (including maximize).
 *
 *  \param [in] event : Qt resize event, forwarded to the base class.
 *
 *  \author Sriram J
 *  \date   19 July 2026
 *
 *  \retval : NA
 */
void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    if (cm_pobj_btnTogglePanels != nullptr)
    {
        cm_pobj_btnTogglePanels->move(0, (this->height() - cm_pobj_btnTogglePanels->height()) / 2);
    }
}

/*!
 *  \fn     void MainWindow::On_Init_StatusBar()
 *  \brief  This function contains an Initialisation and adding an wigets  into an StatusBar..
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void MainWindow ::On_Init_StatusBar()
{
    QString qstime="";
    QDateTime qdate = QDateTime::currentDateTime();

    qstime = qdate.toString("dd/MM/yyyy hh:mm:ss AP");
    cm_pobj_qStatusTimer_label->setText(qstime);

    /// - Add the widgets into status bar
    ui->statusBar->addPermanentWidget(cm_pobj_qStatusTimer_label, 0);
}

/*!
 *  \fn     void MainWindow::on_actionCharcoal_triggered()
 *  \brief  Applies the Charcoal stylesheet to the window.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void MainWindow::on_actionCharcoal_triggered()
{
    /// - Read the QSS file (resolved against the app directory) and apply it
    ///   as the window stylesheet
    setStyleSheet(DP_LoadStyleSheet("Res/Style/Charcoal.qss"));
}

/*!
 *  \fn     void MainWindow::closeEvent(QCloseEvent *event)
 *  \brief  To reconform closing of application from user
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question(
            this,
            "DP-ASPJ-METS-SIMAPP",
            tr("Are you sure you want to exit the application?"),
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::No
            );

    if (resBtn == QMessageBox::Yes)
    {
        event->accept();  /// - Close the application
    }
    else
    {
        event->ignore();  /// - Keep the application running
    }
}


/*!
 *  \fn     void MainWindow::PrintLog(QString in_qsMessage, short in_sStatus)
 *  \brief  Appends a colour-coded, timestamped message to the action log and
 *          trims the log to the most recent 1000 lines.
 *
 *  \param [in] in_qsMessage : Message text to log.
 *  \param [in] in_sStatus   : Status code (failure/success/message).
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void MainWindow::PrintLog(QString in_qsMessage, short in_sStatus)
{
    /// - Working values declared and initialised up front
    QDateTime qdtCurrentDateTime = QDateTime::currentDateTime();
    QString qsCurrentDateTime;
    QTextDocument *pDoc = nullptr;
    QTextCursor qtcCursor;
    int iLinesToRemove = 0;
    int iLine = 0;

    /// - Format the current timestamp
    qsCurrentDateTime = qdtCurrentDateTime.toString("dd/MM/yyyy HH:mm:ss:zzz");

    /// - Failure status: append in red
    if (in_sStatus <= DP_ASPJ_FAILURE)
    {
        qsCurrentDateTime += " [Failure] , " + in_qsMessage + " ";
        ui->tb_PrintLog->append("<font color = 'red'>" + qsCurrentDateTime + "\n");
    }

    /// - Success status: append in green
    if (in_sStatus == DP_ASPJ_SUCCESS)
    {
        qsCurrentDateTime += " [Success] , " + in_qsMessage + " ";
        ui->tb_PrintLog->append("<font color = 'green'>" + qsCurrentDateTime + "\n");
    }

    /// - Informational message (status 2): append in blue
    if (in_sStatus == 2)
    {
        qsCurrentDateTime += " [Message] ," + in_qsMessage + " ";
        ui->tb_PrintLog->append("<font color = 'blue'>" + qsCurrentDateTime + "\n");
    }

    /// - Keep the newest entry visible
    ui->tb_PrintLog->moveCursor(QTextCursor::End);

    /// - Cap the action log to the last 1000 lines to bound memory growth
    pDoc = ui->tb_PrintLog->document();
    if (pDoc != nullptr && pDoc->blockCount() > 1000)
    {
        /// - Remove the oldest lines from the top of the document
        qtcCursor = QTextCursor(pDoc->begin());
        iLinesToRemove = pDoc->blockCount() - 1000;
        for (iLine = 0; iLine < iLinesToRemove; ++iLine)
        {
            qtcCursor.select(QTextCursor::BlockUnderCursor);
            qtcCursor.removeSelectedText();
            qtcCursor.deleteChar(); // Delete the newline character
        }
    }
}

/*!
 *  \fn     void MainWindow::Slot_updateEmitterDetailsToTable(S_DP_ASPJ_EMIT_WITHIN_RANGE *in_sEmiDetWithinRange, int in_iCurrentSegment, int in_iCurrEmitterCnt)
 *  \brief  Updates the emitter detail table for the current flight segment.
 *          Clears previous segment rows on a segment change, then inserts the
 *          emitter row (name, lat, lon, distance, angle) for the current index.
 *
 *  \param [in] in_sEmiDetWithinRange : Emitter detection data for the current waypoint segment.
 *  \param [in] in_iCurrentSegment    : Current flight segment index.
 *  \param [in] in_iCurrEmitterCnt    : Current emitter index within the segment.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void MainWindow::Slot_updateEmitterDetailsToTable(S_DP_ASPJ_EMIT_WITHIN_RANGE *in_sEmiDetWithinRange, int in_iCurrentSegment, int in_iCurrEmitterCnt)
{
    /// Declare all variables at the start of the function body
    QTableWidget *pTbl = NULL;
    bool bValidatePassed = false;
    unsigned short usEmtListCnt = 0;
    unsigned short usLooper = 0;
    int iRow = 0;
    int iOriginalEmitterIndex = 0;
    int iDetectedCount = 0;
    double dMatchLat = 0.0;
    double dMatchLon = 0.0;
    S_DP_ASPJ_EMITTERS_LAT_LON *pEmitterLatLon = NULL;

    /// - Track the last segment processed to detect a segment change
    static int siLastSegment = -1;

    /// - Accumulate all detected emitter indices for the current segment so the table
    ///   only shows emitters that are currently in range (stale rows are removed).
    static QSet<int> sDetectedIndices;

    /// - Validate the input pointer (set a flag instead of returning early)
    if (in_sEmiDetWithinRange == NULL)
    {
        bValidatePassed = false;
    }
    else
    {
        bValidatePassed = true;
    }

    /// - Bail out if validation failed
    if (!bValidatePassed)
    {
        return;
    }

    /// Get pointer to emitter details table widget
    pTbl = ui->tbw_updateEmitterDisAngle;

    /// - On a new waypoint segment, clear the tracking set and table once
    if (in_iCurrentSegment != siLastSegment)
    {
        /// Clear all rows when switching to a new waypoint segment
        pTbl->setRowCount(0);
        sDetectedIndices.clear();
        siLastSegment = in_iCurrentSegment;
    }

    /// Resolve the original emitter index by matching lat/lon against the loaded emitter list
    pEmitterLatLon = g_SDP_ASPJ_Handle.m_sEmitterLatLon;
    usEmtListCnt = pEmitterLatLon[0].m_u16EmtLatLongCnt;

    /// - Build a set of all emitter indices that are currently in range for this segment.
    ///   Each signal emission carries the full detection record, so we can determine the
    ///   complete set of visible emitters and compare it with what we already have.
    QSet<int> currentDetectedIndices;
    for (usLooper = 0; (usLooper < in_sEmiDetWithinRange->m_u16TotalEmtCnt) && (usLooper < DP_MAX_EMITTER_LAT_LONG); usLooper++)
    {
        dMatchLat = in_sEmiDetWithinRange->m_SEmtrRelParam[usLooper].m_fdEmitLat;
        dMatchLon = in_sEmiDetWithinRange->m_SEmtrRelParam[usLooper].m_fdEmitLon;

        iOriginalEmitterIndex = -1;
        for (iRow = 0; iRow < usEmtListCnt && iRow < DP_MAX_EMITTER_LAT_LONG; iRow++)
        {
            if ((fabs(pEmitterLatLon[iRow].m_fdEmitLat - dMatchLat) < 1e-6) &&
                (fabs(pEmitterLatLon[iRow].m_fdEmitLon - dMatchLon) < 1e-6))
            {
                iOriginalEmitterIndex = iRow;
                break;
            }
        }

        if (iOriginalEmitterIndex >= 0)
        {
            currentDetectedIndices.insert(iOriginalEmitterIndex);
        }
    }

    /// - Skip rebuild if the set of detected emitters hasn't changed
    if (currentDetectedIndices == sDetectedIndices)
    {
        return;
    }

    /// - Update the tracking set and rebuild the table
    sDetectedIndices = currentDetectedIndices;
    pTbl->setRowCount(0);

    iDetectedCount = sDetectedIndices.size();
    pTbl->setRowCount(iDetectedCount);

    /// - Helper lambda: populate a single table row for the given original emitter index
    auto populateRow = [this](
                            int in_iRow, int in_iOrigIndex, const S_DP_ASPJ_EMIT_WITHIN_RANGE *in_sDet) -> void
    {
        QTableWidgetItem *pLocalItem = NULL;
        S_EMITTER_REL_PARAM *sLocalEmitter = NULL;
        QString strLocalName;
        QString strLocalLat;
        QString strLocalLon;
        QString strLocalDist;
        QString strLocalDOA;
        QString strLocalHeading;
        QString strLocalAOA;
        QTableWidget *pLocalTbl = ui->tbw_updateEmitterDisAngle;
        unsigned short usLocalLooper = 0;
        S_DP_ASPJ_EMITTERS_LAT_LON *pLocalEmitterLatLon = g_SDP_ASPJ_Handle.m_sEmitterLatLon;

        /// - Locate the emitter in the detection record by matching lat/lon
        for (usLocalLooper = 0; usLocalLooper < in_sDet->m_u16TotalEmtCnt && usLocalLooper < DP_MAX_EMITTER_LAT_LONG; ++usLocalLooper)
        {
            if ((fabs(in_sDet->m_SEmtrRelParam[usLocalLooper].m_fdEmitLat - pLocalEmitterLatLon[in_iOrigIndex].m_fdEmitLat) < 1e-6) &&
                (fabs(in_sDet->m_SEmtrRelParam[usLocalLooper].m_fdEmitLon - pLocalEmitterLatLon[in_iOrigIndex].m_fdEmitLon) < 1e-6))
            {
                sLocalEmitter = const_cast<S_EMITTER_REL_PARAM *>(&in_sDet->m_SEmtrRelParam[usLocalLooper]);
                break;
            }
        }

        /// - Emitter No
        strLocalName = QString("Emt %1").arg(in_iOrigIndex + 1);
        pLocalItem = pLocalTbl->item(in_iRow, 0);
        if (pLocalItem == NULL)
        {
            pLocalItem = new QTableWidgetItem();
            pLocalTbl->setItem(in_iRow, 0, pLocalItem);
        }
        pLocalItem->setText(strLocalName);
        pLocalItem->setTextAlignment(Qt::AlignCenter);

        /// - Latitude
        if (sLocalEmitter != NULL)
        {
            strLocalLat = QString::number(sLocalEmitter->m_fdEmitLat, 'f', 1);
        }
        else
        {
            strLocalLat = QString::number(pLocalEmitterLatLon[in_iOrigIndex].m_fdEmitLat, 'f', 1);
        }
        pLocalItem = pLocalTbl->item(in_iRow, 1);
        if (pLocalItem == NULL)
        {
            pLocalItem = new QTableWidgetItem();
            pLocalTbl->setItem(in_iRow, 1, pLocalItem);
        }
        pLocalItem->setText(strLocalLat);
        pLocalItem->setTextAlignment(Qt::AlignCenter);

        /// - Longitude
        if (sLocalEmitter != NULL)
        {
            strLocalLon = QString::number(sLocalEmitter->m_fdEmitLon, 'f', 1);
        }
        else
        {
            strLocalLon = QString::number(pLocalEmitterLatLon[in_iOrigIndex].m_fdEmitLon, 'f', 1);
        }
        pLocalItem = pLocalTbl->item(in_iRow, 2);
        if (pLocalItem == NULL)
        {
            pLocalItem = new QTableWidgetItem();
            pLocalTbl->setItem(in_iRow, 2, pLocalItem);
        }
        pLocalItem->setText(strLocalLon);
        pLocalItem->setTextAlignment(Qt::AlignCenter);

        /// - Distance (km)
        if (sLocalEmitter != NULL)
        {
            strLocalDist = QString::number(sLocalEmitter->m_fdEmitterDist, 'f', 1);
        }
        else
        {
            strLocalDist = QString("0.0");
        }
        pLocalItem = pLocalTbl->item(in_iRow, 3);
        if (pLocalItem == NULL)
        {
            pLocalItem = new QTableWidgetItem();
            pLocalTbl->setItem(in_iRow, 3, pLocalItem);
        }
        pLocalItem->setText(strLocalDist);
        pLocalItem->setTextAlignment(Qt::AlignCenter);

        /// - DOA
        if (sLocalEmitter != NULL)
        {
            strLocalDOA = QString::number(sLocalEmitter->m_fdDOAfromWayPoint, 'f', 1);
        }
        else
        {
            strLocalDOA = QString("0.0");
        }
        pLocalItem = pLocalTbl->item(in_iRow, 4);
        if (pLocalItem == NULL)
        {
            pLocalItem = new QTableWidgetItem();
            pLocalTbl->setItem(in_iRow, 4, pLocalItem);
        }
        pLocalItem->setText(strLocalDOA);
        pLocalItem->setTextAlignment(Qt::AlignCenter);

        /// - Heading
        strLocalHeading = QString::number(in_sDet->m_fdHeadingAngle, 'f', 1);
        pLocalItem = pLocalTbl->item(in_iRow, 5);
        if (pLocalItem == NULL)
        {
            pLocalItem = new QTableWidgetItem();
            pLocalTbl->setItem(in_iRow, 5, pLocalItem);
        }
        pLocalItem->setText(strLocalHeading);
        pLocalItem->setTextAlignment(Qt::AlignCenter);

        /// - AOA
        if (sLocalEmitter != NULL)
        {
            strLocalAOA = QString::number(sLocalEmitter->m_fdAOAfromWaypoint, 'f', 1);
        }
        else
        {
            strLocalAOA = QString("0.0");
        }
        pLocalItem = pLocalTbl->item(in_iRow, 6);
        if (pLocalItem == NULL)
        {
            pLocalItem = new QTableWidgetItem();
            pLocalTbl->setItem(in_iRow, 6, pLocalItem);
        }
        pLocalItem->setText(strLocalAOA);
        pLocalItem->setTextAlignment(Qt::AlignCenter);
    };

    for (iRow = 0; iRow < iDetectedCount; ++iRow)
    {
        iOriginalEmitterIndex = sDetectedIndices.toList().at(iRow);
        populateRow(iRow, iOriginalEmitterIndex, in_sEmiDetWithinRange);
    }
}

/*!
 *  \fn     void MainWindow::Slot_updateEmitterTable(S_DP_ASPJ_EMIT_WITHIN_RANGE *in_sEmiDetWithinRange, int in_iCurrentSegment)
 *  \brief  Rebuild the emitter detail table from the full detection record.
 *
 *  Driven by the flight controller timer (runs every position update), this slot
 *  directly reads the complete list of in-range emitters from the detection record
 *  and rebuilds the table. Because it does not depend on QML visibility evaluation,
 *  emitters that go out of range are always removed.
 *
 *  \param [in] in_sEmiDetWithinRange : Pointer to the detection record for the current segment.
 *  \param [in] in_iCurrentSegment   : Current waypoint segment index.
 *
 *  \author Sriram J
 *  \date   17 July 2026
 *
 *  \retval : NA
 */
void MainWindow::Slot_updateEmitterTable(S_DP_ASPJ_EMIT_WITHIN_RANGE *in_sEmiDetWithinRange, int in_iCurrentSegment)
{
    QTableWidget *pTbl = NULL;
    bool bValidatePassed = false;
    unsigned short usEmtCnt = 0;
    unsigned short usLooper = 0;
    int iRow = 0;
    int iOriginalEmitterIndex = 0;
    int iDetectedCount = 0;
    double dMatchLat = 0.0;
    double dMatchLon = 0.0;
    S_DP_ASPJ_EMITTERS_LAT_LON *pEmitterLatLon = NULL;

    static int siLastSegment = -1;

    if (in_sEmiDetWithinRange == NULL)
    {
        bValidatePassed = false;
    }
    else
    {
        bValidatePassed = true;
    }

    if (!bValidatePassed)
    {
        return;
    }

    pTbl = ui->tbw_updateEmitterDisAngle;

    /// - Rebuild only when the segment actually changes (detection record is per-segment)
    if (in_iCurrentSegment == siLastSegment)
    {
        return;
    }
    siLastSegment = in_iCurrentSegment;

    pTbl->setRowCount(0);
    pEmitterLatLon = g_SDP_ASPJ_Handle.m_sEmitterLatLon;

    /// - Build a list of all in-range emitters from the detection record
    QSet<int> detectedIndices;
    usEmtCnt = in_sEmiDetWithinRange->m_u16TotalEmtCnt;
    for (usLooper = 0; usLooper < usEmtCnt && usLooper < DP_MAX_EMITTER_LAT_LONG; usLooper++)
    {
        dMatchLat = in_sEmiDetWithinRange->m_SEmtrRelParam[usLooper].m_fdEmitLat;
        dMatchLon = in_sEmiDetWithinRange->m_SEmtrRelParam[usLooper].m_fdEmitLon;

        iOriginalEmitterIndex = -1;
        for (iRow = 0; iRow < DP_MAX_EMITTER_LAT_LONG; iRow++)
        {
            if ((fabs(pEmitterLatLon[iRow].m_fdEmitLat - dMatchLat) < 1e-6) &&
                (fabs(pEmitterLatLon[iRow].m_fdEmitLon - dMatchLon) < 1e-6))
            {
                iOriginalEmitterIndex = iRow;
                break;
            }
        }

        if (iOriginalEmitterIndex >= 0)
        {
            detectedIndices.insert(iOriginalEmitterIndex);
        }
    }

    iDetectedCount = detectedIndices.size();
    pTbl->setRowCount(iDetectedCount);

    auto populateRow = [this](
                            int in_iRow, int in_iOrigIndex, const S_DP_ASPJ_EMIT_WITHIN_RANGE *in_sDet) -> void
    {
        QTableWidgetItem *pLocalItem = NULL;
        S_EMITTER_REL_PARAM *sLocalEmitter = NULL;
        QString strLocalName;
        QString strLocalLat;
        QString strLocalLon;
        QString strLocalDist;
        QString strLocalDOA;
        QString strLocalHeading;
        QString strLocalAOA;
        QTableWidget *pLocalTbl = ui->tbw_updateEmitterDisAngle;
        unsigned short usLocalLooper = 0;
        S_DP_ASPJ_EMITTERS_LAT_LON *pLocalEmitterLatLon = g_SDP_ASPJ_Handle.m_sEmitterLatLon;

        for (usLocalLooper = 0; usLocalLooper < in_sDet->m_u16TotalEmtCnt && usLocalLooper < DP_MAX_EMITTER_LAT_LONG; ++usLocalLooper)
        {
            if ((fabs(in_sDet->m_SEmtrRelParam[usLocalLooper].m_fdEmitLat - pLocalEmitterLatLon[in_iOrigIndex].m_fdEmitLat) < 1e-6) &&
                (fabs(in_sDet->m_SEmtrRelParam[usLocalLooper].m_fdEmitLon - pLocalEmitterLatLon[in_iOrigIndex].m_fdEmitLon) < 1e-6))
            {
                sLocalEmitter = const_cast<S_EMITTER_REL_PARAM *>(&in_sDet->m_SEmtrRelParam[usLocalLooper]);
                break;
            }
        }

        strLocalName = QString("Emt %1").arg(in_iOrigIndex + 1);
        pLocalItem = pLocalTbl->item(in_iRow, 0);
        if (pLocalItem == NULL)
        {
            pLocalItem = new QTableWidgetItem();
            pLocalTbl->setItem(in_iRow, 0, pLocalItem);
        }
        pLocalItem->setText(strLocalName);
        pLocalItem->setTextAlignment(Qt::AlignCenter);

        if (sLocalEmitter != NULL)
        {
            strLocalLat = QString::number(sLocalEmitter->m_fdEmitLat, 'f', 1);
        }
        else
        {
            strLocalLat = QString::number(pLocalEmitterLatLon[in_iOrigIndex].m_fdEmitLat, 'f', 1);
        }
        pLocalItem = pLocalTbl->item(in_iRow, 1);
        if (pLocalItem == NULL)
        {
            pLocalItem = new QTableWidgetItem();
            pLocalTbl->setItem(in_iRow, 1, pLocalItem);
        }
        pLocalItem->setText(strLocalLat);
        pLocalItem->setTextAlignment(Qt::AlignCenter);

        if (sLocalEmitter != NULL)
        {
            strLocalLon = QString::number(sLocalEmitter->m_fdEmitLon, 'f', 1);
        }
        else
        {
            strLocalLon = QString::number(pLocalEmitterLatLon[in_iOrigIndex].m_fdEmitLon, 'f', 1);
        }
        pLocalItem = pLocalTbl->item(in_iRow, 2);
        if (pLocalItem == NULL)
        {
            pLocalItem = new QTableWidgetItem();
            pLocalTbl->setItem(in_iRow, 2, pLocalItem);
        }
        pLocalItem->setText(strLocalLon);
        pLocalItem->setTextAlignment(Qt::AlignCenter);

        if (sLocalEmitter != NULL)
        {
            strLocalDist = QString::number(sLocalEmitter->m_fdEmitterDist, 'f', 1);
        }
        else
        {
            strLocalDist = QString("0.0");
        }
        pLocalItem = pLocalTbl->item(in_iRow, 3);
        if (pLocalItem == NULL)
        {
            pLocalItem = new QTableWidgetItem();
            pLocalTbl->setItem(in_iRow, 3, pLocalItem);
        }
        pLocalItem->setText(strLocalDist);
        pLocalItem->setTextAlignment(Qt::AlignCenter);

        if (sLocalEmitter != NULL)
        {
            strLocalDOA = QString::number(sLocalEmitter->m_fdDOAfromWayPoint, 'f', 1);
        }
        else
        {
            strLocalDOA = QString("0.0");
        }
        pLocalItem = pLocalTbl->item(in_iRow, 4);
        if (pLocalItem == NULL)
        {
            pLocalItem = new QTableWidgetItem();
            pLocalTbl->setItem(in_iRow, 4, pLocalItem);
        }
        pLocalItem->setText(strLocalDOA);
        pLocalItem->setTextAlignment(Qt::AlignCenter);

        strLocalHeading = QString::number(in_sDet->m_fdHeadingAngle, 'f', 1);
        pLocalItem = pLocalTbl->item(in_iRow, 5);
        if (pLocalItem == NULL)
        {
            pLocalItem = new QTableWidgetItem();
            pLocalTbl->setItem(in_iRow, 5, pLocalItem);
        }
        pLocalItem->setText(strLocalHeading);
        pLocalItem->setTextAlignment(Qt::AlignCenter);

        if (sLocalEmitter != NULL)
        {
            strLocalAOA = QString::number(sLocalEmitter->m_fdAOAfromWaypoint, 'f', 1);
        }
        else
        {
            strLocalAOA = QString("0.0");
        }
        pLocalItem = pLocalTbl->item(in_iRow, 6);
        if (pLocalItem == NULL)
        {
            pLocalItem = new QTableWidgetItem();
            pLocalTbl->setItem(in_iRow, 6, pLocalItem);
        }
        pLocalItem->setText(strLocalAOA);
        pLocalItem->setTextAlignment(Qt::AlignCenter);
    };

    for (iRow = 0; iRow < iDetectedCount; ++iRow)
    {
        iOriginalEmitterIndex = detectedIndices.toList().at(iRow);
        populateRow(iRow, iOriginalEmitterIndex, in_sEmiDetWithinRange);
    }
}

void MainWindow::showContextMenu(const QPoint &pos)
{
    QDockWidget *PlyBackCtrlDockWgt = g_SDP_ASPJ_Handle.pm_cobj_flightPlaybackCtrl;

    QMenu contextMenu(this);

    QAction *playbackAction = contextMenu.addAction(
        style()->standardIcon(QStyle::SP_MediaPlay), tr("Playback Control"));
    contextMenu.addSeparator();
    QAction *refreshAction = contextMenu.addAction(
        style()->standardIcon(QStyle::SP_BrowserReload), tr("Refresh"));

    QAction *selectedAction = contextMenu.exec(pos);

    if (selectedAction == nullptr)
    {
        return;
    }

    if (selectedAction == playbackAction)
    {
        if (PlyBackCtrlDockWgt == nullptr)
        {
            return;
        }

        if (PlyBackCtrlDockWgt->isVisible())
        {
            PlyBackCtrlDockWgt->hide();
        }
        else
        {
            PlyBackCtrlDockWgt->setFloating(true);
            PlyBackCtrlDockWgt->show();
            PlyBackCtrlDockWgt->raise();
            PlyBackCtrlDockWgt->activateWindow();
        }
    }
    else if (selectedAction == refreshAction)
    {
        if (g_SDP_ASPJ_Handle.pm_cobj_emitterModel != nullptr)
        {
            QString waypointsFile = g_SDP_ASPJ_Handle.pm_cobj_emitterModel->lastWaypointsFile();
            QString emittersFile  = g_SDP_ASPJ_Handle.pm_cobj_emitterModel->lastEmittersFile();

            if (waypointsFile.isEmpty() || emittersFile.isEmpty())
            {
                QString basePath = QCoreApplication::applicationDirPath();
                waypointsFile = basePath + "/Inputs/Waypoints_lat_lang.csv";
                emittersFile  = basePath + "/Inputs/Emitters_lat_lang.csv";

                if (!QFile::exists(emittersFile))
                {
                    QString fallbackPath = QDir(basePath).path() + "/../Inputs";
                    if (QFile::exists(fallbackPath + "/Emitters_lat_lang.csv"))
                    {
                        waypointsFile = fallbackPath + "/Waypoints_lat_lang.csv";
                        emittersFile  = fallbackPath + "/Emitters_lat_lang.csv";
                    }
                }
            }

            QMetaObject::invokeMethod(g_SDP_ASPJ_Handle.pm_cobj_emitterModel, "autoLoadInitialData",
                Q_ARG(QString, waypointsFile),
                Q_ARG(QString, emittersFile));
        }
    }
}

bool MainWindow::eventFilter(QObject *in_pqObject, QEvent *in_pqEvent)
{
    /// - Catch context menu events from the stacked widget (child widgets eat
    ///   the right-click before ui->centralWidget sees them)
    if (in_pqObject == ui->jammer_sim_map && in_pqEvent->type() == QEvent::ContextMenu)
    {
        QContextMenuEvent *pContextMenuEvent = static_cast<QContextMenuEvent *>(in_pqEvent);
        showContextMenu(pContextMenuEvent->pos());
        return true; /// - Event handled, do not pass to child
    }

    /// - Pass all other events to the base class
    return QMainWindow::eventFilter(in_pqObject, in_pqEvent);
}
