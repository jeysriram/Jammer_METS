/**
 * \file   dp_aspj_scenario_flight_component.h
 * \brief  Background/scenario multi-flight model (CDP_ScenarioFlightModel).
 *
 * \version 1.00
 *
 * \Copyright (C) 2026 Data Patterns (India) Ltd.
 */
#ifndef DP_ASPJ_SCENARIO_FLIGHT_COMPONENT_H
#define DP_ASPJ_SCENARIO_FLIGHT_COMPONENT_H

#include <QAbstractListModel>
#include <QGeoCoordinate>
#include <QVector>
#include <QString>
#include <QTimer>
#include "DP_Types.h"

/*!
*	\def S_DP_SCENARIO_FLIGHT
*	\brief This structure is used for storing one background/scenario flight's
*	       raw CSV waypoints, its spline-interpolated flying path and its live
*	       animation state (current position/heading along that path).
*/
typedef struct
{
    QString                   m_qsFlightId;              /// Flight label parsed from the CSV header row
    QVector<QGeoCoordinate>   m_qvecRawWaypoints;         /// Raw (lat, lon, alt) waypoints loaded from the CSV
    QVector<QGeoCoordinate>   m_qvecPath;                 /// Spline-interpolated path this flight flies
    QVector<FDOUBLE>          m_qvecSegmentDistances_m;   /// Great-circle length of each interpolated path segment
    FDOUBLE                   m_fdTotalDistance_m;        /// Total length of the interpolated path
    FDOUBLE                   m_fdTraveledDistance_m;     /// Distance already flown along the path (wraps to 0 on loop)
    FDOUBLE                   m_fdSpeed_mps;              /// Constant cruise speed for this flight
    QGeoCoordinate            m_qgcCurrentPosition;       /// Live aircraft coordinate, recomputed every tick
    FDOUBLE                   m_fdCurrentHeading_deg;     /// Live aircraft heading, recomputed every tick
}S_DP_SCENARIO_FLIGHT;

/*!
 *  \class  CDP_ScenarioFlightModel
 *  \brief  Standalone, reusable QAbstractListModel that flies any number of
 *          background/scenario aircraft along their own spline-interpolated
 *          CSV routes, purely to populate a map with ambient traffic (like a
 *          flight-radar view) around a primary flight path. Holds no
 *          reference to any other part of the host application — only the
 *          waypoints CSV path is required — so the class can be dropped into
 *          another Qt/QML application unchanged.
 */
class CDP_ScenarioFlightModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles
    {
        FlightIdRole = Qt::UserRole + 1,
        CoordinateRole,
        HeadingRole
    };

    explicit CDP_ScenarioFlightModel(QObject *in_pParent = nullptr);

    int rowCount(const QModelIndex &in_qmiParent = QModelIndex()) const override;
    QVariant data(const QModelIndex &in_qmiIndex, int in_iRole = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    // ==== Methods callable from QML (Q_INVOKABLE) ====
    Q_INVOKABLE void loadScenarioFlightsFromCSV(const QString &in_qsFilePath); // QML: load the multi-flight CSV and start flying
    Q_INVOKABLE void startAllFlights();                                       // QML: start/resume every loaded flight
    Q_INVOKABLE void stopAllFlights();                                        // QML: pause every loaded flight in place

private slots:
    void Slot_AdvanceFlights();

private:
    void DP_ClearScenarioFlights();
    void DP_AppendWaypointsFromRow(const QStringList &in_qslFields, int in_iFlightCount);
    void DP_BuildInterpolatedPath(S_DP_SCENARIO_FLIGHT &inout_rsFlight, int in_iPointsPerSegment);
    void DP_RebuildSegmentDistances(S_DP_SCENARIO_FLIGHT &inout_rsFlight);
    void DP_AdvanceFlightPosition(S_DP_SCENARIO_FLIGHT &inout_rsFlight, FDOUBLE in_fdDeltaDistance_m);

    QVector<S_DP_SCENARIO_FLIGHT> m_qvecFlights;
    QTimer                        *m_pTimer;
    bool                           m_bRunning;
};

#endif /*#ifndef DP_ASPJ_SCENARIO_FLIGHT_COMPONENT_H*/
