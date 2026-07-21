/**
 * \file   dp_aspj_map.cpp
 * \brief  Map container widget (dp_aspj_map) implementation.
 *
 * Hosts the QQuickWidget that loads MapView.qml, exposes the emitter model,
 * flight controller and the online-map flag to QML as context properties,
 * kicks off the initial CSV auto-load and creates the emitter-control toolbar.
 *
 * \version 1.00
 *
 * \Copyright (C) 2026 Data Patterns (India) Ltd.
 */

#include "dp_aspj_comm.h"
#include "ui_dp_aspj_map.h"

#include <QtQuick>
#include <QGraphicsObject>
#include <QQuickWidget>
#include <QQmlContext>
#include <QMessageBox>
#include <QFileDialog>
#include <QToolBar>
#include <QShowEvent>
#include <QDockWidget>
#include <QMenuBar>
#include <QStatusBar>
#include <QSettings>
#include <QCoreApplication>
#include <QDir>
/// - Input-files config: file path, keys and the Tamil Nadu default CSVs
static const char DP_K_INPUT_CFG_FILE[]      = "Inputs/DP_InputFiles.ini";
static const char DP_K_CFG_KEY_WAYPOINTS[]   = "InputFiles/WaypointsCsv";
static const char DP_K_CFG_KEY_EMITTERS[]    = "InputFiles/EmittersCsv";
static const char DP_K_DEFAULT_WAYPOINTS[]   = "Inputs/Waypoints_TamilNadu.csv";
static const char DP_K_DEFAULT_EMITTERS[]    = "Inputs/Emitters_TamilNadu.csv";

/*!
 *  \fn     static QString DP_GetInputCfgFilePath()
 *  \brief  Resolves the input-config ini path against the application
 *          directory rather than the process's current working directory, so
 *          the same Inputs folder next to the executable is always found, no
 *          matter where the app is launched from or which PC it runs on.
 *
 *  \author Sriram J
 *  \date   18 July 2026
 *
 *  \retval : Absolute path to Inputs/DP_InputFiles.ini next to the executable.
 */
static QString DP_GetInputCfgFilePath()
{
    return QDir(QCoreApplication::applicationDirPath()).filePath(DP_K_INPUT_CFG_FILE);
}

/*!
 *  \fn     static QString DP_ResolveInputPath(const QString &in_qsPath)
 *  \brief  Resolves a possibly-relative CSV path against the application
 *          directory so the file opens correctly regardless of the process's
 *          current working directory. Absolute paths are returned unchanged.
 *
 *  \param [in] in_qsPath : Path read from the input config (relative or absolute).
 *
 *  \author Sriram J
 *  \date   18 July 2026
 *
 *  \retval : Absolute, directly-openable path.
 */
static QString DP_ResolveInputPath(const QString &in_qsPath)
{
    if (in_qsPath.isEmpty() || QDir::isAbsolutePath(in_qsPath))
    {
        return in_qsPath;
    }
    return QDir(QCoreApplication::applicationDirPath()).filePath(in_qsPath);
}

/*!
 *  \fn     static QString DP_MakeInputPathPortable(const QString &in_qsPath)
 *  \brief  Converts an absolute path that lives under the application
 *          directory into a path relative to it, so the saved ini stays
 *          portable when the whole application folder is copied to another
 *          PC. Paths outside the application directory are kept absolute,
 *          since there is no portable way to express them.
 *
 *  \param [in] in_qsPath : Absolute path chosen by the user (e.g. via a file dialog).
 *
 *  \author Sriram J
 *  \date   18 July 2026
 *
 *  \retval : Path relative to the application directory when possible, else unchanged.
 */
static QString DP_MakeInputPathPortable(const QString &in_qsPath)
{
    QDir qdAppDir(QCoreApplication::applicationDirPath());
    QString qsRelative;

    if (in_qsPath.isEmpty() || !QDir::isAbsolutePath(in_qsPath))
    {
        return in_qsPath;
    }

    qsRelative = qdAppDir.relativeFilePath(in_qsPath);
    if (qsRelative.startsWith(".."))
    {
        return in_qsPath;
    }
    return qsRelative;
}

/*!
 *  \fn     void DP_ReadInputCsvConfig(QString &out_qsWaypointsCsv, QString &out_qsEmittersCsv)
 *  \brief  Reads the startup waypoint/emitter CSV paths from the input config
 *          file (Inputs/DP_InputFiles.ini). Missing file or keys fall back to
 *          the Tamil Nadu defaults, and the config file is (re)written so it
 *          always exists for the user to edit — change the paths in it and the
 *          next startup loads those CSVs instead.
 *
 *  \param [out] out_qsWaypointsCsv : Waypoints CSV path to load on startup.
 *  \param [out] out_qsEmittersCsv  : Emitters CSV path to load on startup.
 *
 *  \author Sriram J
 *  \date   12 July 2026
 *
 *  \retval : NA
 */
void DP_ReadInputCsvConfig(QString &out_qsWaypointsCsv, QString &out_qsEmittersCsv)
{
    /// - Open (or create) the INI-format config file, anchored to the
    ///   application directory so it is found regardless of the CWD
    QSettings qsetCfg(DP_GetInputCfgFilePath(), QSettings::IniFormat);
    /// - Read both paths, falling back to the Tamil Nadu defaults
    out_qsWaypointsCsv = qsetCfg.value(DP_K_CFG_KEY_WAYPOINTS, DP_K_DEFAULT_WAYPOINTS).toString();
    out_qsEmittersCsv  = qsetCfg.value(DP_K_CFG_KEY_EMITTERS,  DP_K_DEFAULT_EMITTERS).toString();
    /// - Write the resolved values back so the file always exists with the
    ///   currently active paths, ready for the user to edit
    qsetCfg.setValue(DP_K_CFG_KEY_WAYPOINTS, out_qsWaypointsCsv);
    qsetCfg.setValue(DP_K_CFG_KEY_EMITTERS,  out_qsEmittersCsv);

    /// - Resolve relative entries against the application directory (not the
    ///   CWD) so the returned paths always open correctly
    out_qsWaypointsCsv = DP_ResolveInputPath(out_qsWaypointsCsv);
    out_qsEmittersCsv  = DP_ResolveInputPath(out_qsEmittersCsv);
}
/*!
 *  \fn     void DP_SaveInputCsvConfig(const QString &in_qsWaypointsCsv, const QString &in_qsEmittersCsv)
 *  \brief  Saves the given waypoint/emitter CSV paths into the input config
 *          file so the NEXT application start loads the same files (called
 *          after a successful manual load from the Load Settings dialog).
 *          Empty arguments leave the corresponding stored path unchanged.
 *
 *  \param [in] in_qsWaypointsCsv : Waypoints CSV path to remember (or empty).
 *  \param [in] in_qsEmittersCsv  : Emitters CSV path to remember (or empty).
 *
 *  \author Sriram J
 *  \date   12 July 2026
 *
 *  \retval : NA
 */
void DP_SaveInputCsvConfig(const QString &in_qsWaypointsCsv, const QString &in_qsEmittersCsv)
{
    /// - Open (or create) the INI-format config file, anchored to the
    ///   application directory so it is found regardless of the CWD
    QSettings qsetCfg(DP_GetInputCfgFilePath(), QSettings::IniFormat);
    /// - Store the waypoints path only when one was actually given, converted
    ///   to a path relative to the application directory when possible so the
    ///   ini stays portable if the whole application folder is moved/copied
    if (!in_qsWaypointsCsv.isEmpty())
    {
        qsetCfg.setValue(DP_K_CFG_KEY_WAYPOINTS, DP_MakeInputPathPortable(in_qsWaypointsCsv));
    }
    /// - Store the emitters path only when one was actually given
    if (!in_qsEmittersCsv.isEmpty())
    {
        qsetCfg.setValue(DP_K_CFG_KEY_EMITTERS, DP_MakeInputPathPortable(in_qsEmittersCsv));
    }
}

/*!
 *  \fn     dp_aspj_map::dp_aspj_map(QWidget *parent)
 *  \brief  Constructor — builds the widget from its generated UI.
 *
 *  \param [in] parent : Parent widget for ownership.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
dp_aspj_map::dp_aspj_map(QWidget *parent) :
    QWidget(parent),
    m_pUi(new Ui::dp_aspj_map)
{
    /// - Build the widget from the generated UI
    m_pUi->setupUi(this);
}

/*!
 *  \fn     dp_aspj_map::~dp_aspj_map()
 *  \brief  Destructor — releases the generated UI object.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
dp_aspj_map::~dp_aspj_map()
{
    /// - Free the generated UI object
    delete m_pUi;
}

/*!
 *  \fn     void dp_aspj_map::DP_BindMapModelToQML(CDP_EmitterModel *in_pcEmitterModel, CDP_FlightController *in_pcFlightController)
 *  \brief  Stores the shared models in the global handle, wires the QML context
 *          and auto-loads the initial waypoint/emitter CSVs.
 *
 *  \param [in] in_pcEmitterModel     : Shared emitter model instance.
 *  \param [in] in_pcFlightController : Shared flight controller instance.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void dp_aspj_map::DP_BindMapModelToQML(CDP_EmitterModel *in_pcEmitterModel, CDP_FlightController *in_pcFlightController)
{
    /// - Replace the global handle models with the shared ones from MainWindow
    g_SDP_ASPJ_Handle.pm_cobj_emitterModel = in_pcEmitterModel;
    g_SDP_ASPJ_Handle.pm_cobj_flightController = in_pcFlightController;

    /// - Set up the QML context (models + flags) and load MapView.qml
    setupQmlContext();

    /// - Startup CSV paths, resolved from the input config file
    QString qsWaypointsCsv;
    QString qsEmittersCsv;
    /// - Resolve which CSVs to load: taken from Inputs/DP_InputFiles.ini
    ///   (created with the Tamil Nadu defaults when missing). Editing that
    ///   file — or loading other CSVs via the Load Settings dialog — redirects
    ///   every following startup to those paths.
    DP_ReadInputCsvConfig(qsWaypointsCsv, qsEmittersCsv);
    /// - Auto-load initial data from the configured CSVs on startup. This
    ///   replicates the same logic as clicking "Load Settings" in the dialog:
    ///   1. Load waypoints CSV (lat/lng) -> emitter markers + spline flight path
    ///   2. Load emitters CSV (lat/lng)  -> emitter markers
    ///   3. Draw spline-interpolated paths (blue emitter path, green/blue flight)


    QMetaObject::invokeMethod(g_SDP_ASPJ_Handle.pm_cobj_emitterModel, "autoLoadInitialData",
        Q_ARG(QString, qsWaypointsCsv),
        Q_ARG(QString, qsEmittersCsv));
}

/*!
 *  \fn     void dp_aspj_map::showEvent(QShowEvent *in_pqShowEvent)
 *  \brief  Creates the top emitter-control toolbar the first time the widget is
 *          shown (hidden by default).
 *
 *  \param [in] in_pqShowEvent : Qt show event forwarded to the base class.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void dp_aspj_map::showEvent(QShowEvent *in_pqShowEvent)
{
    /// - Parent main window, initialised up front
    QMainWindow *pMainWindow = nullptr;
    /// - Resolve the parent as a QMainWindow
    pMainWindow = qobject_cast<QMainWindow *>(parent());
    /// - Create the emitter toolbar once, only when a main window parent exists
    if (pMainWindow != nullptr && m_emitterToolBar == nullptr)
    {
        /// - Build the toolbar, dock it at the top and keep it hidden initially
        m_emitterToolBar = new QToolBar(tr("Emitter Control"), pMainWindow);

        pMainWindow->addToolBar(Qt::TopToolBarArea, m_emitterToolBar);
        m_emitterToolBar->hide();
    }
    /// - Forward the event to the base class
    QWidget::showEvent(in_pqShowEvent);
}

/*!
 *  \fn     void dp_aspj_map::setupQmlContext()
 *  \brief  Creates the QQuickWidget, exposes the emitter model, flight
 *          controller, this widget and the online-map flag to QML, then loads
 *          MapView.qml into the map layout.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void dp_aspj_map::setupQmlContext()
{
    /// - Create the QQuickWidget that hosts the QML map view
    m_mapWidget = new QQuickWidget();
    /// - Configure and populate the widget when creation succeeded
    if (m_mapWidget != nullptr)
    {
        /// - Resize the root QML object to follow the widget size
        m_mapWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);

        /// - Expose the C++ models and this widget to QML as context properties
        m_mapWidget->engine()->rootContext()->setContextProperty("emitterModel",  g_SDP_ASPJ_Handle.pm_cobj_emitterModel);
        m_mapWidget->engine()->rootContext()->setContextProperty("flightController", g_SDP_ASPJ_Handle.pm_cobj_flightController);
        m_mapWidget->engine()->rootContext()->setContextProperty("dpAspjMap", this);

        /// - Bridge the compile-time map source selection (DP_USE_ONLINE_MAP) to QML
#ifdef DP_USE_ONLINE_MAP
        m_mapWidget->engine()->rootContext()->setContextProperty("g_bUseOnlineMap", true);
#else
        m_mapWidget->engine()->rootContext()->setContextProperty("g_bUseOnlineMap", false);
#endif

        /// - Load the QML map view and add the widget to the map layout
        m_mapWidget->setSource(QUrl("qrc:/qml/MapView.qml"));
        m_pUi->dp_aspj_flightmap->addWidget(m_mapWidget);
    }
}
/*!
 *  \fn     void dp_aspj_map::DP_ToggleFullScreen()
 *  \brief  VLC-style full-screen toggle for the map. First call: remember and
 *          hide every visible dock/toolbar plus the menu bar and status bar of
 *          the parent main window, then show the window full-screen so only the
 *          map remains. Second call: restore exactly what was hidden and the
 *          previous (maximised/normal) window state. Called from the QML
 *          full-screen button via the "dpAspjMap" context property.
 *
 *  \author Sriram J
 *  \date   05 July 2026
 *
 *  \retval : NA
 */
void dp_aspj_map::DP_ToggleFullScreen()
{
    /// - Top-level main window that owns the docks/toolbars/menus, resolved up front
    QMainWindow *pMainWindow = qobject_cast<QMainWindow *>(this->window());
    /// - Only act when the widget actually lives inside a QMainWindow
    if (pMainWindow != nullptr)
    {
        if (m_bFullScreen == false)
        {
            /// - ENTER full screen ------------------------------------------------
            /// - Start from a clean record of what we hide
            m_qlHiddenChrome.clear();
            /// - Hide every currently-visible dock widget (flight display, playback…)
            for (QDockWidget *pDock : pMainWindow->findChildren<QDockWidget *>())
            {
                if (pDock->isVisible() == true)
                {
                    m_qlHiddenChrome.append(pDock);
                    pDock->hide();
                }
            }
            /// - Hide every currently-visible toolbar (emitter control, etc.)
            for (QToolBar *pToolBar : pMainWindow->findChildren<QToolBar *>())
            {
                if (pToolBar->isVisible() == true)
                {
                    m_qlHiddenChrome.append(pToolBar);
                    pToolBar->hide();
                }
            }
            /// - Hide the menu bar if present and visible
            if (pMainWindow->menuBar() != nullptr && pMainWindow->menuBar()->isVisible() == true)
            {
                m_qlHiddenChrome.append(pMainWindow->menuBar());
                pMainWindow->menuBar()->hide();
            }
            /// - Hide the status bar if present and visible
            if (pMainWindow->statusBar() != nullptr && pMainWindow->statusBar()->isVisible() == true)
            {
                m_qlHiddenChrome.append(pMainWindow->statusBar());
                pMainWindow->statusBar()->hide();
            }
            /// - NOTE: the window state itself is left untouched (no showFullScreen).
            ///   Only the surrounding widgets are hidden, so the map simply fills
            ///   the existing application window — the title bar / taskbar stay put.
            m_bFullScreen = true;
        }
        else
        {
            /// - EXIT map-maximise -----------------------------------------------
            /// - Show again every widget we hid on the way in (original layout)
            for (QWidget *pWidget : m_qlHiddenChrome)
            {
                pWidget->show();
            }
            m_qlHiddenChrome.clear();
            m_bFullScreen = false;
        }
    }
}
