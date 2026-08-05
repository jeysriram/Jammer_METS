/**
 * \file   dp_aspj_map.h
 * \brief  Map container widget (dp_aspj_map).
 *
 * Hosts the QQuickWidget that loads MapView.qml, exposes the C++ models to QML
 * as context properties, triggers the initial CSV auto-load and creates the
 * top emitter-control toolbar.
 *
 * \version 1.00
 *
 * \Copyright (C) 2026 Data Patterns (India) Ltd.
 */

#ifndef DP_ASPJ_MAP_H
#define DP_ASPJ_MAP_H

#include <QWidget>
#include <QMainWindow>
#include <QStackedWidget>
#include <QQuickWidget>
#include <QQmlContext>
#include <QGeoCoordinate>
#include <QVBoxLayout>
#include <QQuickView>
#include <QDockWidget>

#include "dp_aspj_comm.h"

namespace Ui {
class dp_aspj_map;
}

/**
 *  \brief      Reads the startup waypoint/emitter CSV paths from the input
 *              config file (Inputs/DP_InputFiles.ini). Missing file or keys
 *              fall back to the Tamil Nadu defaults, and the config file is
 *              (re)written so it always exists for the user to edit � change
 *              the paths in it and the next startup loads those CSVs instead.
 *  \param[out] out_qsWaypointsCsv : Waypoints CSV path to load on startup.
 *  \param[out] out_qsEmittersCsv  : Emitters CSV path to load on startup.
 *  \author     Sriram J
 *  \date       12 July 2026
 */
void DP_ReadInputCsvConfig(QString &out_qsWaypointsCsv, QString &out_qsEmittersCsv);
/**
 *  \brief      Saves the given waypoint/emitter CSV paths into the input config
 *              file so the NEXT application start loads the same files (called
 *              after a successful manual load from the Load Settings dialog).
 *              Empty arguments leave the corresponding stored path unchanged.
 *  \param[in]  in_qsWaypointsCsv : Waypoints CSV path to remember (or empty).
 *  \param[in]  in_qsEmittersCsv  : Emitters CSV path to remember (or empty).
 *  \author     Sriram J
 *  \date       12 July 2026
 */
void DP_SaveInputCsvConfig(const QString &in_qsWaypointsCsv, const QString &in_qsEmittersCsv);
/*!
 *  \class  dp_aspj_map
 *  \brief  Qt widget that hosts the QML map view and wires it to the C++ models.
 */
class dp_aspj_map : public QWidget
{
    Q_OBJECT

public:
    /**
     *  \brief      Constructor.
     *  \param[in]  parent : Parent widget for ownership.
     *  \author     Sriram J
     *  \date       03 July 2026
     */
    explicit dp_aspj_map(QWidget *parent = nullptr);
    /**
     *  \brief      Destructor � releases the generated UI object.
     *  \author     Sriram J
     *  \date       03 July 2026
     */
    ~dp_aspj_map();

    /**
     *  \brief      Creates the QQuickWidget, exposes the models/flags to QML and
     *              loads MapView.qml.
     *  \author     Sriram J
     *  \date       03 July 2026
     */
    void setupQmlContext();

    /**
     *  \brief      Binds the shared emitter model and flight controller, sets up
     *              the QML context and auto-loads the initial CSV data.
     *  \param[in]  in_pcEmitterModel     : Shared emitter model instance.
     *  \param[in]  in_pcFlightController : Shared flight controller instance.
     *  \author     Sriram J
     *  \date       03 July 2026
     */
    void DP_BindMapModelToQML(CDP_EmitterModel *in_pcEmitterModel, CDP_FlightController *in_pcFlightController);

    /**
     *  \brief      Toggles a VLC-style maximised map view WITHIN the application
     *              window (the window itself is not sent to OS full-screen). On
     *              the first call it remembers and hides every visible dock,
     *              toolbar, the menu bar and the status bar of the parent
     *              QMainWindow, so the map fills the existing window. The next
     *              call restores exactly what was hidden (the original layout).
     *              Q_INVOKABLE so the QML button (dpAspjMap) can call it.
     *  \author     Sriram J
     *  \date       05 July 2026
     */
    Q_INVOKABLE void DP_ToggleFullScreen();
private slots:


protected:
    /**
     *  \brief      Creates the emitter-control toolbar on first show.
     *  \param[in]  in_pqShowEvent : Qt show event.
     *  \author     Sriram J
     *  \date       03 July 2026
     */
    void showEvent(QShowEvent *in_pqShowEvent) override;

private:
    Ui::dp_aspj_map *m_pUi;                     /*!< Generated UI object for this widget */

    QQuickWidget    *m_mapWidget = nullptr;     /*!< QQuickWidget hosting the QML map view */
    CDP_ScenarioFlightModel *m_pScenarioFlightModel = nullptr; /*!< Background/scenario multi-flight traffic model */

    QToolBar        *m_emitterToolBar = nullptr;/*!< Top toolbar for emitter control */
    bool             m_bFullScreen = false;    /*!< True while the map is maximised inside the window */
    QList<QWidget *> m_qlHiddenChrome;         /*!< Widgets hidden while map is maximised (restored on exit) */
};

#endif /*#ifndef DP_ASPJ_MAP_H*/
