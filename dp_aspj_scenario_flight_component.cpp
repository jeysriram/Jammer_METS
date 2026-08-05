/**
 * \file   dp_aspj_scenario_flight_component.cpp
 * \brief  Background/scenario multi-flight model (CDP_ScenarioFlightModel).
 *
 * Loads a multi-flight waypoints CSV (one flight per lat/lon/alt column
 * triplet, see loadScenarioFlightsFromCSV()), spline-interpolates each
 * flight's route with the same ParametricSplineInterpolate() core used for
 * the primary flight path, then flies every flight along its own path on a
 * shared QTimer, looping back to the start on completion. Exposed as a
 * QAbstractListModel (flightId/coordinate/heading roles) so QML can render
 * one icon per flight without ever seeing the underlying waypoints/path.
 *
 * \version 1.00
 *
 * \Copyright (C) 2026 Data Patterns (India) Ltd.
 */
#include "dp_aspj_scenario_flight_component.h"
#include "dp_spline_interpolation.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

/// - Animation tick rate and path density, matching the primary flight
///   controller / emitter model's own defaults (see CDP_FlightController and
///   CDP_EmitterModel::drawSplineInterpolatedPath())
static const int    DP_K_SCENARIO_TICK_MS               = 100;
static const int    DP_K_SCENARIO_POINTS_PER_SEGMENT     = 50;
/// - Per-flight cruise speed: a base value plus a small, deterministic
///   per-flight step so background traffic doesn't move in lockstep
static const double DP_K_SCENARIO_BASE_SPEED_MPS         = 220.0;
static const double DP_K_SCENARIO_SPEED_STEP_MPS         = 24.0;
static const int    DP_K_SCENARIO_SPEED_VARIANTS         = 5;

/*!
 *  \fn     CDP_ScenarioFlightModel::CDP_ScenarioFlightModel(QObject *in_pParent)
 *  \brief  Constructor — initialises members and creates the shared drive timer.
 *
 *  \param [in] in_pParent : Qt parent object for ownership.
 *
 *  \author Sriram J
 *  \date   05 August 2026
 *
 *  \retval : NA
 */
CDP_ScenarioFlightModel::CDP_ScenarioFlightModel(QObject *in_pParent)
    : QAbstractListModel(in_pParent)
    , m_pTimer(nullptr)
    , m_bRunning(false)
{
    /// - Create the shared timer that advances every scenario flight together
    m_pTimer = new QTimer(this);
    m_pTimer->setInterval(DP_K_SCENARIO_TICK_MS);
    connect(m_pTimer, &QTimer::timeout, this, &CDP_ScenarioFlightModel::Slot_AdvanceFlights);
}

/*!
 *  \fn     int CDP_ScenarioFlightModel::rowCount(const QModelIndex &in_qmiParent) const
 *  \brief  Returns the number of scenario flights in the model.
 *
 *  \param [in] in_qmiParent : Parent index (unused for a flat list model).
 *
 *  \author Sriram J
 *  \date   05 August 2026
 *
 *  \retval : Scenario flight count.
 */
int CDP_ScenarioFlightModel::rowCount(const QModelIndex &in_qmiParent) const
{
    /// - Parent is unused for a flat list model
    Q_UNUSED(in_qmiParent)
    /// - Return the number of scenario flights
    return m_qvecFlights.size();
}

/*!
 *  \fn     QVariant CDP_ScenarioFlightModel::data(const QModelIndex &in_qmiIndex, int in_iRole) const
 *  \brief  Returns the requested role value for the flight at the given index.
 *
 *  \param [in] in_qmiIndex : Model index of the flight row.
 *  \param [in] in_iRole    : Requested data role.
 *
 *  \author Sriram J
 *  \date   05 August 2026
 *
 *  \retval : QVariant for the role, invalid QVariant when out of range.
 */
QVariant CDP_ScenarioFlightModel::data(const QModelIndex &in_qmiIndex, int in_iRole) const
{
    /// - Result, invalid until a valid row/role is resolved
    QVariant qvResult;

    /// - Only resolve a value for a valid, in-range index
    if (in_qmiIndex.isValid() && in_qmiIndex.row() >= 0 && in_qmiIndex.row() < m_qvecFlights.size())
    {
        /// - Flight record for the requested row (reference, no copy)
        const S_DP_SCENARIO_FLIGHT &sFlight = m_qvecFlights.at(in_qmiIndex.row());
        /// - Map the requested role to the matching field
        switch (in_iRole)
        {
        case FlightIdRole:
            qvResult = sFlight.m_qsFlightId;
            break;
        case CoordinateRole:
            qvResult = QVariant::fromValue(sFlight.m_qgcCurrentPosition);
            break;
        case HeadingRole:
            qvResult = sFlight.m_fdCurrentHeading_deg;
            break;
        default:
            break;
        }
    }
    /// - Return the resolved value (or the still-invalid default)
    return qvResult;
}

/*!
 *  \fn     QHash<int, QByteArray> CDP_ScenarioFlightModel::roleNames() const
 *  \brief  Maps model roles to the names used by QML bindings.
 *
 *  \author Sriram J
 *  \date   05 August 2026
 *
 *  \retval : Role-id to role-name hash.
 */
QHash<int, QByteArray> CDP_ScenarioFlightModel::roleNames() const
{
    /// - Role name table, initialised empty
    QHash<int, QByteArray> qhashRoles;
    /// - Map each role id to the QML property name
    qhashRoles[FlightIdRole] = "flightId";
    qhashRoles[CoordinateRole] = "coordinate";
    qhashRoles[HeadingRole] = "heading";
    /// - Return the assembled role name table
    return qhashRoles;
}

/*!
 *  \fn     void CDP_ScenarioFlightModel::DP_ClearScenarioFlights()
 *  \brief  Stops the animation and removes every loaded scenario flight.
 *
 *  \author Sriram J
 *  \date   05 August 2026
 *
 *  \retval : NA
 */
void CDP_ScenarioFlightModel::DP_ClearScenarioFlights()
{
    /// - Stop the shared timer before the flight list is torn down
    stopAllFlights();
    /// - Reset the model and clear every flight record
    beginResetModel();
    m_qvecFlights.clear();
    endResetModel();
}

/*!
 *  \fn     void CDP_ScenarioFlightModel::DP_AppendWaypointsFromRow(const QStringList &in_qslFields, int in_iFlightCount)
 *  \brief  Parses one CSV data row (lat,lon,alt triplets, one per flight) and
 *          appends each valid waypoint to its flight. A flight with fewer
 *          waypoints than the others simply yields blank/invalid fields on
 *          its later rows, which are silently skipped.
 *
 *  \param [in] in_qslFields    : Comma-split fields of one CSV data row.
 *  \param [in] in_iFlightCount : Number of flight columns in the CSV.
 *
 *  \author Sriram J
 *  \date   05 August 2026
 *
 *  \retval : NA
 */
void CDP_ScenarioFlightModel::DP_AppendWaypointsFromRow(const QStringList &in_qslFields, int in_iFlightCount)
{
    /// - Loop index, column base and per-flight working values, declared up front
    int iFlightIdx = 0;
    int iColBase = 0;
    double dLat = 0.0;
    double dLon = 0.0;
    double dAlt = 0.0;
    bool bOkLat = false;
    bool bOkLon = false;

    /// - One lat/lon/alt triplet per flight, in CSV column order
    for (iFlightIdx = 0; iFlightIdx < in_iFlightCount; ++iFlightIdx)
    {
        iColBase = iFlightIdx * 3;
        dLat = in_qslFields.value(iColBase, QString()).toDouble(&bOkLat);
        dLon = in_qslFields.value(iColBase + 1, QString()).toDouble(&bOkLon);
        dAlt = in_qslFields.value(iColBase + 2, QString("0")).toDouble();

        /// - Only a valid lat/lon pair becomes a waypoint for this flight
        if (bOkLat && bOkLon)
        {
            m_qvecFlights[iFlightIdx].m_qvecRawWaypoints.append(QGeoCoordinate(dLat, dLon, dAlt));
        }
    }
}

/*!
 *  \fn     void CDP_ScenarioFlightModel::DP_RebuildSegmentDistances(S_DP_SCENARIO_FLIGHT &inout_rsFlight)
 *  \brief  Recomputes per-segment great-circle distances and the total path
 *          length for one flight, and parks it at the first path point.
 *
 *  \param [inout] inout_rsFlight : Flight record to update.
 *
 *  \author Sriram J
 *  \date   05 August 2026
 *
 *  \retval : NA
 */
void CDP_ScenarioFlightModel::DP_RebuildSegmentDistances(S_DP_SCENARIO_FLIGHT &inout_rsFlight)
{
    /// - Loop index and per-segment distance, declared up front
    int iSeg = 0;
    double dSegDist = 0.0;

    /// - Clear any previously cached segment distances/total
    inout_rsFlight.m_qvecSegmentDistances_m.clear();
    inout_rsFlight.m_fdTotalDistance_m = 0.0;

    /// - Distance of each leg between consecutive interpolated path points
    for (iSeg = 0; iSeg < inout_rsFlight.m_qvecPath.size() - 1; ++iSeg)
    {
        dSegDist = inout_rsFlight.m_qvecPath.at(iSeg).distanceTo(inout_rsFlight.m_qvecPath.at(iSeg + 1));
        inout_rsFlight.m_qvecSegmentDistances_m.append(dSegDist);
        inout_rsFlight.m_fdTotalDistance_m += dSegDist;
    }

    /// - Park the flight at the start of its own path and reset progress
    if (!inout_rsFlight.m_qvecPath.isEmpty())
    {
        inout_rsFlight.m_qgcCurrentPosition = inout_rsFlight.m_qvecPath.first();
    }
    inout_rsFlight.m_fdTraveledDistance_m = 0.0;
}

/*!
 *  \fn     void CDP_ScenarioFlightModel::DP_BuildInterpolatedPath(S_DP_SCENARIO_FLIGHT &inout_rsFlight, int in_iPointsPerSegment)
 *  \brief  Spline-interpolates one flight's raw waypoints into a smooth
 *          flying path (lat/lon via ParametricSplineInterpolate(), altitude
 *          via a simple linear blend across the same per-segment knot
 *          spacing so it lines up with the interpolated lat/lon points).
 *
 *  \param [inout] inout_rsFlight       : Flight record to build the path for.
 *  \param [in]    in_iPointsPerSegment : Interpolated points to generate per segment.
 *
 *  \author Sriram J
 *  \date   05 August 2026
 *
 *  \retval : NA
 */
void CDP_ScenarioFlightModel::DP_BuildInterpolatedPath(S_DP_SCENARIO_FLIGHT &inout_rsFlight, int in_iPointsPerSegment)
{
    /// - Working values / buffers, declared and initialised up front
    int iWaypointCount = 0;
    int iNumSegments = 0;
    int iOutCount = 0;
    int iLoop = 0;
    int iSeg = 0;
    int iSubIdx = 0;
    double dFraction = 0.0;
    double dAltFrom = 0.0;
    double dAltTo = 0.0;
    double *pdLatIn = nullptr;
    double *pdLonIn = nullptr;
    double *pdLatOut = nullptr;
    double *pdLonOut = nullptr;
    S32BIT s32Result = ERR_SPLINE_SUCCESS;

    iWaypointCount = inout_rsFlight.m_qvecRawWaypoints.size();

    /// - Need at least 3 waypoints and a positive point count to interpolate
    if (iWaypointCount >= 3 && in_iPointsPerSegment > 0)
    {
        /// - Extract raw lat/lon arrays from the flight's waypoints
        pdLatIn = new double[iWaypointCount];
        pdLonIn = new double[iWaypointCount];
        for (iLoop = 0; iLoop < iWaypointCount; ++iLoop)
        {
            pdLatIn[iLoop] = inout_rsFlight.m_qvecRawWaypoints.at(iLoop).latitude();
            pdLonIn[iLoop] = inout_rsFlight.m_qvecRawWaypoints.at(iLoop).longitude();
        }

        /// - Allocate the interpolated output buffers
        iNumSegments = iWaypointCount - 1;
        iOutCount = (iNumSegments * in_iPointsPerSegment) + 1;
        pdLatOut = new double[iOutCount];
        pdLonOut = new double[iOutCount];

        /// - Run the same parametric spline interpolation used for the primary flight path
        s32Result = ParametricSplineInterpolate(
            pdLatIn, pdLonIn, (U32BIT)iWaypointCount, (U32BIT)in_iPointsPerSegment,
            pdLatOut, pdLonOut
        );

        if (s32Result == ERR_SPLINE_SUCCESS)
        {
            /// - Replace the path with the interpolated points; altitude follows
            ///   the same per-segment knot index as the lat/lon spline, linearly
            ///   blended between the segment's two raw waypoint altitudes
            inout_rsFlight.m_qvecPath.clear();
            inout_rsFlight.m_qvecPath.reserve(iOutCount);
            for (iLoop = 0; iLoop < iOutCount; ++iLoop)
            {
                iSeg = qMin(iLoop / in_iPointsPerSegment, iNumSegments - 1);
                iSubIdx = iLoop - (iSeg * in_iPointsPerSegment);
                dFraction = (double)iSubIdx / (double)in_iPointsPerSegment;
                dAltFrom = inout_rsFlight.m_qvecRawWaypoints.at(iSeg).altitude();
                dAltTo = inout_rsFlight.m_qvecRawWaypoints.at(iSeg + 1).altitude();
                inout_rsFlight.m_qvecPath.append(QGeoCoordinate(pdLatOut[iLoop], pdLonOut[iLoop], dAltFrom + ((dAltTo - dAltFrom) * dFraction)));
            }

            /// - Rebuild the segment distances/total for the new path
            DP_RebuildSegmentDistances(inout_rsFlight);
        }
        else
        {
            qDebug() << "CDP_ScenarioFlightModel: spline error" << s32Result << "for flight" << inout_rsFlight.m_qsFlightId;
        }

        /// - Free the working buffers
        delete[] pdLatIn;
        delete[] pdLonIn;
        delete[] pdLatOut;
        delete[] pdLonOut;
    }
}

/*!
 *  \fn     void CDP_ScenarioFlightModel::loadScenarioFlightsFromCSV(const QString &in_qsFilePath)
 *  \brief  Loads a multi-flight waypoints CSV (header row: one flight label
 *          every 3 columns; data rows: lat,lon,alt per flight), builds each
 *          flight's spline-interpolated path and starts flying all of them.
 *
 *  \param [in] in_qsFilePath : Path to the multi-flight waypoints CSV file.
 *
 *  \author Sriram J
 *  \date   05 August 2026
 *
 *  \retval : NA
 */
void CDP_ScenarioFlightModel::loadScenarioFlightsFromCSV(const QString &in_qsFilePath)
{
    /// - CSV parsing / setup working values, declared up front
    QFile qfFile(in_qsFilePath);
    QTextStream qtsIn;
    QString qsLine;
    QStringList qslHeaderFields;
    QStringList qslFields;
    int iFlightCount = 0;
    int iFlightIdx = 0;

    /// - Drop any previously loaded scenario flights
    DP_ClearScenarioFlights();

    if (qfFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qtsIn.setDevice(&qfFile);

        /// - Header row: one flight label every 3 columns (lat, lon, alt)
        qsLine = qtsIn.readLine().trimmed();
        qslHeaderFields = qsLine.split(',');
        iFlightCount = qslHeaderFields.size() / 3;

        /// - Create one flight record per column group and name it from the header
        beginResetModel();
        m_qvecFlights.resize(iFlightCount);
        for (iFlightIdx = 0; iFlightIdx < iFlightCount; ++iFlightIdx)
        {
            m_qvecFlights[iFlightIdx].m_qsFlightId = qslHeaderFields.value(iFlightIdx * 3, QString("Flight_%1").arg(iFlightIdx + 1)).trimmed();
            m_qvecFlights[iFlightIdx].m_fdSpeed_mps = DP_K_SCENARIO_BASE_SPEED_MPS
                + ((iFlightIdx % DP_K_SCENARIO_SPEED_VARIANTS) * DP_K_SCENARIO_SPEED_STEP_MPS);
        }

        /// - Data rows: append each flight's waypoints from its column triplet
        while (!qtsIn.atEnd())
        {
            qsLine = qtsIn.readLine();
            if (!qsLine.trimmed().isEmpty())
            {
                qslFields = qsLine.split(',');
                DP_AppendWaypointsFromRow(qslFields, iFlightCount);
            }
        }
        endResetModel();
        qfFile.close();

        /// - Spline-interpolate every flight's route and start the animation
        for (iFlightIdx = 0; iFlightIdx < iFlightCount; ++iFlightIdx)
        {
            DP_BuildInterpolatedPath(m_qvecFlights[iFlightIdx], DP_K_SCENARIO_POINTS_PER_SEGMENT);
        }
        if (!m_qvecFlights.isEmpty())
        {
            emit dataChanged(index(0), index(m_qvecFlights.size() - 1));
        }
        startAllFlights();
    }
    else
    {
        qDebug() << "CDP_ScenarioFlightModel: cannot open scenario flights CSV:" << in_qsFilePath;
    }
}

/*!
 *  \fn     void CDP_ScenarioFlightModel::startAllFlights()
 *  \brief  Starts (or resumes) the shared animation timer for every loaded flight.
 *
 *  \author Sriram J
 *  \date   05 August 2026
 *
 *  \retval : NA
 */
void CDP_ScenarioFlightModel::startAllFlights()
{
    /// - Nothing to fly without at least one loaded flight
    if (!m_qvecFlights.isEmpty())
    {
        m_bRunning = true;
        m_pTimer->start();
    }
}

/*!
 *  \fn     void CDP_ScenarioFlightModel::stopAllFlights()
 *  \brief  Pauses every scenario flight in place.
 *
 *  \author Sriram J
 *  \date   05 August 2026
 *
 *  \retval : NA
 */
void CDP_ScenarioFlightModel::stopAllFlights()
{
    /// - Clear the running flag and halt the shared timer
    m_bRunning = false;
    m_pTimer->stop();
}

/*!
 *  \fn     void CDP_ScenarioFlightModel::DP_AdvanceFlightPosition(S_DP_SCENARIO_FLIGHT &inout_rsFlight, FDOUBLE in_fdDeltaDistance_m)
 *  \brief  Advances one flight along its interpolated path by the given
 *          distance, looping back to the start once it completes the path,
 *          and recomputes its live position/heading.
 *
 *  \param [inout] inout_rsFlight     : Flight record to advance.
 *  \param [in]    in_fdDeltaDistance_m : Distance to advance this tick, in metres.
 *
 *  \author Sriram J
 *  \date   05 August 2026
 *
 *  \retval : NA
 */
void CDP_ScenarioFlightModel::DP_AdvanceFlightPosition(S_DP_SCENARIO_FLIGHT &inout_rsFlight, FDOUBLE in_fdDeltaDistance_m)
{
    /// - Working values / loop index, declared up front
    int iSeg = 0;
    double dAccumulated = 0.0;
    double dIntoSegment = 0.0;
    double dSegDist = 0.0;
    QGeoCoordinate qgcFrom;
    QGeoCoordinate qgcTo;

    /// - Need a real path and a known length to fly along
    if (inout_rsFlight.m_qvecPath.size() >= 2 && inout_rsFlight.m_fdTotalDistance_m > 0.0)
    {
        inout_rsFlight.m_fdTraveledDistance_m += in_fdDeltaDistance_m;

        /// - Loop back to the start once the path completes (continuous, radar-style traffic)
        while (inout_rsFlight.m_fdTraveledDistance_m >= inout_rsFlight.m_fdTotalDistance_m)
        {
            inout_rsFlight.m_fdTraveledDistance_m -= inout_rsFlight.m_fdTotalDistance_m;
        }

        /// - Walk the cached segment distances to find where the traveled distance lands
        for (iSeg = 0; iSeg < inout_rsFlight.m_qvecSegmentDistances_m.size() - 1; ++iSeg)
        {
            dSegDist = inout_rsFlight.m_qvecSegmentDistances_m.at(iSeg);
            if (dAccumulated + dSegDist >= inout_rsFlight.m_fdTraveledDistance_m)
            {
                break;
            }
            dAccumulated += dSegDist;
        }

        /// - Interpolate the position and heading within the landing segment
        qgcFrom = inout_rsFlight.m_qvecPath.at(iSeg);
        qgcTo = inout_rsFlight.m_qvecPath.at(iSeg + 1);
        dSegDist = inout_rsFlight.m_qvecSegmentDistances_m.at(iSeg);
        dIntoSegment = inout_rsFlight.m_fdTraveledDistance_m - dAccumulated;

        inout_rsFlight.m_qgcCurrentPosition = (dSegDist > 0.0)
            ? qgcFrom.atDistanceAndAzimuth(dIntoSegment, qgcFrom.azimuthTo(qgcTo))
            : qgcFrom;
        inout_rsFlight.m_fdCurrentHeading_deg = qgcFrom.azimuthTo(qgcTo);
    }
}

/*!
 *  \fn     void CDP_ScenarioFlightModel::Slot_AdvanceFlights()
 *  \brief  Timer slot: advances every scenario flight by one tick's worth of
 *          distance and notifies QML of the updated positions/headings.
 *
 *  \author Sriram J
 *  \date   05 August 2026
 *
 *  \retval : NA
 */
void CDP_ScenarioFlightModel::Slot_AdvanceFlights()
{
    /// - Loop index and per-tick advance distance, declared up front
    int iFlightIdx = 0;
    double dDeltaDistance_m = 0.0;

    /// - Advance every flight by its own speed * tick interval
    for (iFlightIdx = 0; iFlightIdx < m_qvecFlights.size(); ++iFlightIdx)
    {
        dDeltaDistance_m = m_qvecFlights[iFlightIdx].m_fdSpeed_mps * (m_pTimer->interval() / 1000.0);
        DP_AdvanceFlightPosition(m_qvecFlights[iFlightIdx], dDeltaDistance_m);
    }

    /// - Notify QML of the updated coordinate/heading for every row
    if (!m_qvecFlights.isEmpty())
    {
        emit dataChanged(index(0), index(m_qvecFlights.size() - 1));
    }
}
