/**
 * \file   dp_aspj_flight_component.cpp
 * \brief  Aircraft flight simulation controller (CDP_FlightController).
 *
 * Drives the simulated aircraft along the loaded waypoints: position/bearing
 * interpolation on a QTimer, path splitting (completed/remaining), transport
 * controls (next/prev/seek), looping and emitter-in-range detection. The
 * results are exposed to QML through Q_PROPERTY signals.
 *
 * \version 1.00
 *
 * \Copyright (C) 2026 Data Patterns (India) Ltd.
 */
#include "dp_aspj_comm.h"
#include "dp_aspj_structures.h"
#include "dp_aspj_macros.h"
#include <QtMath>
#include <QDebug>

/*!
 *  \fn     CDP_FlightController::CDP_FlightController(QObject *parent)
 *  \brief  Constructor — initialises all members and creates the 50 ms drive timer.
 *
 *  \param [in] parent : Qt parent object for ownership.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
CDP_FlightController::CDP_FlightController(QObject *parent)
    : QObject(parent)
    , m_aircraftBearing(0.0)
    , m_isFlying(false)
    , m_currentSegment(0)
    , m_segmentProgress(0.0)
    //, m_speed(20000.0)
    , m_calculatedVelocity(0.0)
    , m_totalDistance(0.0)
    , m_completedDistance(0.0)
    , m_loop(false)
{
    /// - Create the periodic timer that advances the aircraft position
    m_timer = new QTimer(this);
    /// - Fire every 100 ms (10 Hz update rate)
    m_timer->setInterval(100);
    /// - Route each timer tick to updatePosition()
    connect(m_timer, &QTimer::timeout, this, &CDP_FlightController::updatePosition);
}

/*!
 *  \fn     void CDP_FlightController::initAircraftPosition(double in_dLat, double in_dLng)
 *  \brief  Places the aircraft at a given latitude/longitude and notifies QML.
 *
 *  \param [in] in_dLat : Initial aircraft latitude in degrees.
 *  \param [in] in_dLng : Initial aircraft longitude in degrees.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void CDP_FlightController::initAircraftPosition(double in_dLat, double in_dLng)
{
    /// - Store the supplied coordinate as the current aircraft position
    m_aircraftPosition = QGeoCoordinate(in_dLat, in_dLng);
    /// - Notify QML bindings that the aircraft position changed
    emit aircraftPositionChanged();
}

/*!
 *  \fn     QVariantList CDP_FlightController::flightPath() const
 *  \brief  Returns the full waypoint list as a QVariantList for QML.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : QVariantList of QGeoCoordinate waypoints.
 */
QVariantList CDP_FlightController::flightPath() const
{
    /// - Output list, initialised empty
    QVariantList lstFlightPath;
    /// - Wrap every waypoint coordinate into the variant list
    for (const QGeoCoordinate &qgcWaypoint : m_flightPath)
    {
        lstFlightPath.append(QVariant::fromValue(qgcWaypoint));
    }
    /// - Return the assembled list
    return lstFlightPath;
}

/*!
 *  \fn     QVariantList CDP_FlightController::completedPath() const
 *  \brief  Returns the already-flown portion of the path.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : QVariantList of the completed path.
 */
QVariantList CDP_FlightController::completedPath() const
{
    /// - Return the cached completed-path list
    return m_completedPath;
}

/*!
 *  \fn     QVariantList CDP_FlightController::remainingPath() const
 *  \brief  Returns the not-yet-flown portion of the path.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : QVariantList of the remaining path.
 */
QVariantList CDP_FlightController::remainingPath() const
{
    /// - Return the cached remaining-path list
    return m_remainingPath;
}

/*!
 *  \fn     QGeoCoordinate CDP_FlightController::aircraftPosition() const
 *  \brief  Returns the current aircraft coordinate.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : Current QGeoCoordinate of the aircraft.
 */
QGeoCoordinate CDP_FlightController::aircraftPosition() const
{
    /// - Return the current aircraft position
    return m_aircraftPosition;
}

/*!
 *  \fn     double CDP_FlightController::aircraftBearing() const
 *  \brief  Returns the current aircraft heading in degrees.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : Aircraft bearing in degrees.
 */
double CDP_FlightController::aircraftBearing() const
{
    /// - Return the current heading
    return m_aircraftBearing;
}

/*!
 *  \fn     bool CDP_FlightController::isFlying() const
 *  \brief  Returns whether the simulation is currently running.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : true if flying, false otherwise.
 */
bool CDP_FlightController::isFlying() const
{
    /// - Return the running/flying state
    return m_isFlying;
}

/*!
 *  \fn     double CDP_FlightController::progressPercent() const
 *  \brief  Returns flight completion as a percentage (0..100).
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : Completion percentage, 0.0 when the path length is zero.
 */
double CDP_FlightController::progressPercent() const
{
    /// - Guard against divide-by-zero when there is no path length
    if (m_totalDistance <= 0)
    {
        return 0.0;
    }
    /// - Percentage of the total distance already covered
    return (m_completedDistance / m_totalDistance) * 100.0;
}

/*!
 *  \fn     double CDP_FlightController::totalDistance() const
 *  \brief  Returns the total path length in metres.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : Total distance in metres.
 */
double CDP_FlightController::totalDistance() const
{
    /// - Return the cached total distance
    return m_totalDistance;
}

/*!
 *  \fn     void CDP_FlightController::setSpeed(double in_dSpeed)
 *  \brief  Sets the aircraft speed (m/s); ignores non-positive values.
 *
 *  \param [in] in_dSpeed : New speed in metres per second.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void CDP_FlightController::setSpeed(double in_dSpeed)
{
    /// - Only accept a positive speed
    if (in_dSpeed > 0)
    {
        /// - Store the new speed
        m_speed = in_dSpeed;

        /// - Notify QML that the speed changed
        emit speedChanged();
    }
}

FDOUBLE CDP_FlightController::updateVelocityofFlight()
{
    int iLooper = 0;
    FDOUBLE fdVelocity = 0.0;

    fdVelocity = g_SDP_ASPJ_Handle.m_sEmitDetWithinRange.m_SEmttrRange[m_currentSegment].m_fdVelocity_ms;

    /// - Segment 0 has no velocity (never travelled yet), use first non-zero segment's value
    if (fdVelocity <= 0.0 && m_currentSegment < m_flightPath.size() - 1)
    {
        for (iLooper = 1; iLooper < m_flightPath.size() - 1; ++iLooper)
        {
            fdVelocity = g_SDP_ASPJ_Handle.m_sEmitDetWithinRange.m_SEmttrRange[iLooper].m_fdVelocity_ms;
            if (fdVelocity > 0.0)
            {
                break;
            }
        }
    }

    m_calculatedVelocity = fdVelocity;                          /// - Store velocity in km/h for display
    m_speed = m_calculatedVelocity / 3.6;                      /// - Convert km/h → m/s for movement

    //qDebug() << "Velocity" << m_calculatedVelocity << "km/h";

    /// - Notify QML of every affected property
    emit calculatedVelocityChanged();
    emit speedChanged();
}

/*!
 *  \fn     void CDP_FlightController::startFlight()
 *  \brief  Starts or resumes the flight. Starts fresh from the first waypoint
 *          when parked at the start or after completion; otherwise resumes from
 *          the current (paused) position without losing live values.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void CDP_FlightController::startFlight()
{
    /// - Local flags declared and initialised up front
    bool bAtStart = false;
    bool bCompleted = false;
    /// - Nothing to fly if no waypoints are loaded
    if (m_flightPath.isEmpty())
    {
        qDebug() << "startFlight: NO waypoints! count=" << m_flightPath.size();
        return;
    }

    /// - Ignore the request if already flying
    if (m_isFlying)
    {
        qDebug() << "startFlight: Already flying!";
        return;
    }

    /// - "At start": parked on the first waypoint with no progress
    bAtStart = (m_currentSegment == 0 && m_segmentProgress <= 0.0);

    /// - "Completed": the previous flight already reached the end
    bCompleted = (progressPercent() >= 99.9);

    /// - Start fresh only at the beginning or after completion; otherwise resume
    if (bAtStart || bCompleted)
    {
        /// - Reset to the first waypoint and clear progress counters
        m_currentSegment = 0;
        m_segmentProgress = 0.0;
        m_completedDistance = 0.0;
        m_aircraftPosition = m_flightPath.first();
        emit currentSegmentChanged();
        emit aircraftPositionChanged();
        emit progressPercentChanged();
    }

    /// - Load the velocity for the current segment so m_speed is non-zero before the first tick
    updateVelocityofFlight();

    /// - Mark as flying and start the drive timer
    m_isFlying = true;
    emit isFlyingChanged();
    m_timer->start();
}

/*!
 *  \fn     void CDP_FlightController::stopFlight()
 *  \brief  Pauses the flight in place (keeps segment/progress/position).
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void CDP_FlightController::stopFlight()
{
    /// - Clear the flying state and notify QML
    m_isFlying = false;
    emit isFlyingChanged();
    /// - Stop the periodic update timer
    m_timer->stop();
}

/*!
 *  \fn     void CDP_FlightController::setLoop(bool in_bLoop)
 *  \brief  Enables/disables looping the flight until stopped.
 *
 *  \param [in] in_bLoop : true to loop continuously, false for a single run.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void CDP_FlightController::setLoop(bool in_bLoop)
{
    /// - Store the loop mode flag
    m_loop = in_bLoop;
}
/*!
 *  \fn     void CDP_FlightController::finishOrLoop()
 *  \brief  Called at the last waypoint: loops back to the start and keeps flying
 *          when loop mode is on, otherwise stops the flight.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void CDP_FlightController::finishOrLoop()
{
    /// - When looping (and a path exists), restart from the beginning
    if (m_loop && !m_flightPath.isEmpty())
    {
        /// - Reset counters and jump the aircraft to the first waypoint
        m_currentSegment = 0;
        m_segmentProgress = 0.0;
        m_completedDistance = 0.0;
        m_aircraftPosition = m_flightPath.first();
        /// - Rebuild the completed/remaining path splits for the new lap
        rebuildPathSegments();
        emit currentSegmentChanged();
        emit aircraftPositionChanged();
        emit progressPercentChanged();
        /// - m_isFlying stays true and the timer keeps running -> seamless loop
    }
    else
    {
        /// - Not looping: end the flight normally
        stopFlight();
    }
}
/*!
 *  \fn     void CDP_FlightController::resetFlight()
 *  \brief  Stops the flight and rewinds all state to the very start.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void CDP_FlightController::resetFlight()
{
    /// - Stop the timer/flying state first
    stopFlight();
    /// - Rewind to the first segment
    m_currentSegment = 0;
    emit currentSegmentChanged();
    /// - Clear progress/distance/bearing
    m_segmentProgress = 0.0;
    m_completedDistance = 0.0;
    m_aircraftBearing = 0.0;
    emit aircraftBearingChanged();
    /// - Rebuild the path splits and refresh progress
    rebuildPathSegments();
    emit progressPercentChanged();
}

/*!
 *  \fn     void CDP_FlightController::clearWaypoints()
 *  \brief  Removes all waypoints and resets every flight-related member.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void CDP_FlightController::clearWaypoints()
{
    /// - Stop flying and halt the timer
    m_isFlying = false;
    m_timer->stop();
    /// - Clear the path and all cached segment/split data
    m_flightPath.clear();
    m_segmentDistances.clear();
    m_completedPath.clear();
    m_remainingPath.clear();
    /// - Reset all progress/distance counters
    m_currentSegment = 0;
    m_segmentProgress = 0.0;
    m_totalDistance = 0.0;
    m_completedDistance = 0.0;
    /// - Notify QML of every affected property
    emit isFlyingChanged();
    emit flightPathChanged();
    emit completedPathChanged();
    emit remainingPathChanged();
    emit totalDistanceChanged();
}

/*!
 *  \fn     void CDP_FlightController::addWaypoint(double in_dLat, double in_dLng)
 *  \brief  Appends a single waypoint to the flight path.
 *
 *  \param [in] in_dLat : Waypoint latitude in degrees.
 *  \param [in] in_dLng : Waypoint longitude in degrees.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void CDP_FlightController::addWaypoint(double in_dLat, double in_dLng)
{
    /// - Append the coordinate to the path list
    m_flightPath.append(QGeoCoordinate(in_dLat, in_dLng));
}

/*!
 *  \fn     void CDP_FlightController::flushWaypoints()
 *  \brief  Rebuilds path segments after bulk waypoint changes and notifies QML.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void CDP_FlightController::flushWaypoints()
{
    /// - Recompute segment distances and path splits
    rebuildPathSegments();
    /// - Notify QML that the flight path changed
    emit flightPathChanged();

#ifdef DP_ASPJ_DEBUG_PRINTS
    /// - Optional debug trace of the path sizes
    qDebug() << "flushWaypoints: flightPath.size=" << m_flightPath.size()
             << "completedPath.size=" << m_completedPath.size()
             << "remainingPath.size=" << m_remainingPath.size();
#endif
}

/*!
 *  \fn     void CDP_FlightController::addWaypoints(const QList<QGeoCoordinate> &in_lstWaypoints)
 *  \brief  Appends a batch of waypoints to the flight path.
 *
 *  \param [in] in_lstWaypoints : List of coordinates to append.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void CDP_FlightController::addWaypoints(const QList<QGeoCoordinate> &in_lstWaypoints)
{
    /// - Append each supplied coordinate to the path
    for (const QGeoCoordinate &qgcWaypoint : in_lstWaypoints)
    {
        m_flightPath.append(qgcWaypoint);
    }
}

/*!
 *  \fn     void CDP_FlightController::rebuildPathSegments()
 *  \brief  Recomputes per-segment distances, total distance and resets the
 *          completed/remaining path splits.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void CDP_FlightController::rebuildPathSegments()
{
    /// - Loop index and per-segment distance, declared up front
    int iSeg = 0;
    double dSegDist = 0.0;
    /// - Clear any previously cached segment distances
    m_segmentDistances.clear();
    /// - Distance of each leg between consecutive waypoints
    for (iSeg = 0; iSeg < m_flightPath.size() - 1; ++iSeg)
    {
        dSegDist = m_flightPath[iSeg].distanceTo(m_flightPath[iSeg + 1]);
        m_segmentDistances.append(dSegDist);
    }
    /// - Total path length and notify QML
    m_totalDistance = computeTotalDistance();
    emit totalDistanceChanged();

    /// - Reset the splits: nothing completed, everything remaining
    m_completedPath.clear();
    m_remainingPath.clear();
    if (!m_flightPath.isEmpty())
    {
        for (const QGeoCoordinate &qgcWaypoint : m_flightPath)
        {
            m_remainingPath.append(QVariant::fromValue(qgcWaypoint));
        }
    }
    /// - Notify QML of the updated splits
    emit completedPathChanged();
    emit remainingPathChanged();
}

/*!
 *  \fn     double CDP_FlightController::computeTotalDistance() const
 *  \brief  Sums all cached segment distances.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : Total path length in metres.
 */
double CDP_FlightController::computeTotalDistance() const
{
    /// - Running total, initialised to zero
    double dTotal = 0.0;
    /// - Accumulate every segment distance
    for (double dSeg : m_segmentDistances)
    {
        dTotal += dSeg;
    }
    /// - Return the summed distance
    return dTotal;
}

/*!
 *  \fn     double CDP_FlightController::computeCompletedDistance() const
 *  \brief  Distance covered up to the current segment plus progress within it.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : Completed distance in metres.
 */
double CDP_FlightController::computeCompletedDistance() const
{
    /// - Running total and loop index, declared up front
    double dCompleted = 0.0;
    int iSeg = 0;
    /// - Sum the distances of all fully-completed segments
    for (iSeg = 0; iSeg < m_currentSegment && iSeg < (int)m_segmentDistances.size(); ++iSeg)
    {
        dCompleted += m_segmentDistances[iSeg];
    }
    /// - Add progress within the current (partially-flown) segment
    if (m_currentSegment >= 0 && m_currentSegment < (int)m_segmentDistances.size())
    {
        /// - m_segmentProgress is in metres along the current segment
        dCompleted += m_segmentProgress;
    }
    /// - Return the completed distance
    return dCompleted;
}

/*!
 *  \fn     void CDP_FlightController::updatePosition()
 *  \brief  Timer slot (50 ms). Advances the aircraft along the current segment,
 *          steps to the next segment when reached, updates bearing and the
 *          completed/remaining path splits, and loops/stops at the end.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void CDP_FlightController::updatePosition()
{
    /// - Working coordinates, distances and loop index declared up front
    QGeoCoordinate qgcFrom;
    QGeoCoordinate qgcTo;
    double dSegDist = 0.0;
    double dDelta = 0.0;
    int iSeg = 0;
    /// - Do nothing while not flying
    if (!m_isFlying)
    {
        return;
    }

    /// - Stop if there is no path to fly
    if (m_flightPath.isEmpty())
    {
        stopFlight();
        return;
    }

    /// - Reached the last waypoint: loop back or stop
    if (m_currentSegment >= m_flightPath.size() - 1)
    {
        finishOrLoop();
        return;
    }

    /// - Snap to the first waypoint on the very first frame
    if (m_segmentProgress == 0.0 && m_completedDistance == 0.0)
    {
        m_aircraftPosition = m_flightPath.first();
        emit aircraftPositionChanged();
    }

    /// - Endpoints and length of the current segment
    qgcFrom = m_flightPath[m_currentSegment];
    qgcTo = m_flightPath[m_currentSegment + 1];
    dSegDist = m_segmentDistances.value(m_currentSegment, 0.0);

    qDebug() << "qgcFrom" << qgcFrom << "qgcTo" << qgcTo << "SegDist" << dSegDist;

    /// - Distance to advance this tick: speed * 50 ms (m/s * s = m)
    dDelta = m_speed * 0.05;

    if (dSegDist < 1.0)
    {

        /// - Segment too short: skip straight to the next one
        m_currentSegment++;
        emit currentSegmentChanged();
        /// - If skipping lands on the last waypoint, finalise/loop
        if (m_currentSegment >= m_flightPath.size() - 1)
        {
            m_aircraftPosition = m_flightPath.last();
            m_completedDistance = m_totalDistance;
            rebuildPathSegments();
            emit aircraftPositionChanged();
            emit progressPercentChanged();
            finishOrLoop();
            return;
        }
        /// - Otherwise load the new current segment
        qgcFrom = m_flightPath[m_currentSegment];
        qgcTo = m_flightPath[m_currentSegment + 1];
        dSegDist = m_segmentDistances.value(m_currentSegment, 0.0);
        calculateBearing(qgcFrom, qgcTo);
        updateVelocityofFlight();
        m_aircraftPosition = qgcFrom;
    }
    else
    {
        /// - Advance the progress along the current segment
        m_segmentProgress += dDelta;

        if (m_segmentProgress >= dSegDist)
        {
            /// - Reached the end of this segment: snap to its end waypoint
            m_aircraftPosition = qgcTo;
            m_segmentProgress = 0.0;
            m_currentSegment++;
            emit currentSegmentChanged();

            /// - If that was the last waypoint, finalise/loop
            if (m_currentSegment >= m_flightPath.size() - 1)
            {
                m_completedDistance = m_totalDistance;
                rebuildPathSegments();
                emit aircraftPositionChanged();
                emit progressPercentChanged();
                finishOrLoop();
                return;
            }

            /// - Otherwise load the new current segment
            qgcFrom = m_flightPath[m_currentSegment];
            qgcTo = m_flightPath[m_currentSegment + 1];
            dSegDist = m_segmentDistances.value(m_currentSegment, 0.0);
            calculateBearing(qgcFrom, qgcTo);
            updateVelocityofFlight();
        }
        else
        {
            /// - Still mid-segment: interpolate the position and update bearing
            m_aircraftPosition = qgcFrom.atDistanceAndAzimuth(m_segmentProgress, qgcFrom.azimuthTo(qgcTo));
            calculateBearing(qgcFrom, qgcTo);
            updateVelocityofFlight();
        }
    }

    /// - Recompute how far along the whole path we are
    m_completedDistance = computeCompletedDistance();

    /// - Rebuild the completed path (waypoints so far + current position)
    m_completedPath.clear();
    for (iSeg = 0; iSeg <= m_currentSegment; ++iSeg)
    {
        m_completedPath.append(QVariant::fromValue(m_flightPath[iSeg]));
    }
    m_completedPath.append(QVariant::fromValue(m_aircraftPosition));

    /// - Rebuild the remaining path (current position + waypoints ahead)
    m_remainingPath.clear();
    m_remainingPath.append(QVariant::fromValue(m_aircraftPosition));
    for (iSeg = m_currentSegment + 1; iSeg < m_flightPath.size(); ++iSeg)
    {
        m_remainingPath.append(QVariant::fromValue(m_flightPath[iSeg]));
    }

    /// - Update the INS GPS NAV Display heading angle in GUI
    Sig_update_GPS_Data_to_nav_display(&g_SDP_ASPJ_Handle.m_sEmitDetWithinRange.m_SEmttrRange[m_currentSegment]);

    /// - Push the full detection record to the emitter table (runs every tick,
    ///   independent of QML, so stale rows are always removed)
    Sig_updateEmitterTable(&g_SDP_ASPJ_Handle.m_sEmitDetWithinRange.m_SEmttrRange[m_currentSegment], m_currentSegment);

    /// - Notify QML of all updated properties
    emit aircraftPositionChanged();
    emit completedPathChanged();
    emit remainingPathChanged();
    emit progressPercentChanged();
    emit totalDistanceChanged();
}

/*!
 *  \fn     void CDP_FlightController::calculateBearing(const QGeoCoordinate &in_qgcFrom, const QGeoCoordinate &in_qgcTo)
 *  \brief  Updates the aircraft heading as the azimuth from one point to another.
 *
 *  \param [in] in_qgcFrom : Segment start coordinate.
 *  \param [in] in_qgcTo   : Segment end coordinate.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void CDP_FlightController::calculateBearing(const QGeoCoordinate &in_qgcFrom, const QGeoCoordinate &in_qgcTo)
{
    /// - Heading is the azimuth from the start point to the end point
    m_aircraftBearing = in_qgcFrom.azimuthTo(in_qgcTo);
    /// - Notify QML that the bearing changed
    emit aircraftBearingChanged();
}

/*!
 *  \fn     void CDP_FlightController::autoFitMapToPath(QGeoCoordinate &out_qgcCenter, double &out_dZoom)
 *  \brief  Computes the bounding-box centre and a zoom level that fits all
 *          waypoints on the map.
 *
 *  \param [out] out_qgcCenter : Centre of the waypoint bounding box.
 *  \param [out] out_dZoom     : Zoom level (clamped 3..18) to fit the path.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void CDP_FlightController::autoFitMapToPath(QGeoCoordinate &out_qgcCenter, double &out_dZoom)
{
    /// - Bounding-box / span / centre working values, declared up front
    double dMinLat = 90.0;
    double dMaxLat = -90.0;
    double dMinLng = 180.0;
    double dMaxLng = -180.0;
    double dCenterLat = 0.0;
    double dCenterLng = 0.0;
    double dLatSpan = 0.0;
    double dLngSpan = 0.0;
    double dSpan = 0.0;
    /// - No path: return a sensible default centre and zoom
    if (m_flightPath.isEmpty())
    {
        out_qgcCenter = QGeoCoordinate(0.0, 0.0);
        out_dZoom = 6.0;
        return;
    }

    /// - Find the min/max latitude and longitude across all waypoints
    for (const QGeoCoordinate &qgcWaypoint : m_flightPath)
    {
        if (qgcWaypoint.latitude() < dMinLat)
        {
            dMinLat = qgcWaypoint.latitude();
        }
        if (qgcWaypoint.latitude() > dMaxLat)
        {
            dMaxLat = qgcWaypoint.latitude();
        }
        if (qgcWaypoint.longitude() < dMinLng)
        {
            dMinLng = qgcWaypoint.longitude();
        }
        if (qgcWaypoint.longitude() > dMaxLng)
        {
            dMaxLng = qgcWaypoint.longitude();
        }
    }

    /// - Centre of the bounding box
    dCenterLat = (dMinLat + dMaxLat) / 2.0;
    dCenterLng = (dMinLng + dMaxLng) / 2.0;

    /// - Angular span of the box (largest of lat/lng)
    dLatSpan = dMaxLat - dMinLat;
    dLngSpan = dMaxLng - dMinLng;
    dSpan = qMax(dLatSpan, dLngSpan);

    /// - Zoom heuristic: at zoom 6, ~5 degrees fills the screen
    if (dSpan <= 5.0)
    {
        out_dZoom = 6.0 + (5.0 - dSpan) * 0.5;
    }
    else
    {
        out_dZoom = 6.0 - qLn(dSpan / 5.0) / qLn(2.0);
    }

    /// - Clamp the zoom into the valid range
    if (out_dZoom < 3.0)
    {
        out_dZoom = 3.0;
    }
    if (out_dZoom > 18.0)
    {
        out_dZoom = 18.0;
    }

    /// - Return the bounding-box centre
    out_qgcCenter = QGeoCoordinate(dCenterLat, dCenterLng);
}

/*!
 *  \fn     QVariant CDP_FlightController::autoFitMapToPathQml() const
 *  \brief  QML-friendly variant of autoFitMapToPath: returns {lat, lng, zoom}.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : QVariant JSON object with lat, lng and zoom.
 */
QVariant CDP_FlightController::autoFitMapToPathQml() const
{
    /// - Bounding-box / span / centre / zoom working values, declared up front
    double dMinLat = 90.0;
    double dMaxLat = -90.0;
    double dMinLng = 180.0;
    double dMaxLng = -180.0;
    double dCenterLat = 0.0;
    double dCenterLng = 0.0;
    double dLatSpan = 0.0;
    double dLngSpan = 0.0;
    double dSpan = 0.0;
    double dOutZoom = 6.0;
    /// - No path: return default lat/lng/zoom
    if (m_flightPath.isEmpty())
    {
        return QVariant::fromValue(QJsonObject{{"lat", 0.0}, {"lng", 0.0}, {"zoom", 6.0}});
    }

    /// - Find the min/max latitude and longitude across all waypoints
    for (const QGeoCoordinate &qgcWaypoint : m_flightPath)
    {
        if (qgcWaypoint.latitude() < dMinLat)
        {
            dMinLat = qgcWaypoint.latitude();
        }
        if (qgcWaypoint.latitude() > dMaxLat)
        {
            dMaxLat = qgcWaypoint.latitude();
        }
        if (qgcWaypoint.longitude() < dMinLng)
        {
            dMinLng = qgcWaypoint.longitude();
        }
        if (qgcWaypoint.longitude() > dMaxLng)
        {
            dMaxLng = qgcWaypoint.longitude();
        }
    }
    /// - Centre of the bounding box
    dCenterLat = (dMinLat + dMaxLat) / 2.0;
    dCenterLng = (dMinLng + dMaxLng) / 2.0;
    /// - Angular span of the box (largest of lat/lng)
    dLatSpan = dMaxLat - dMinLat;
    dLngSpan = dMaxLng - dMinLng;
    dSpan = qMax(dLatSpan, dLngSpan);

    /// - Zoom heuristic: at zoom 6, ~5 degrees fills the screen
    if (dSpan <= 5.0)
    {
        dOutZoom = 6.0 + (5.0 - dSpan) * 0.5;
    }
    else
    {
        dOutZoom = 6.0 - qLn(dSpan / 5.0) / qLn(2.0);
    }
    /// - Clamp the zoom into the valid range
    if (dOutZoom < 3.0)
    {
        dOutZoom = 3.0;
    }
    if (dOutZoom > 18.0)
    {
        dOutZoom = 18.0;
    }
    /// - Return centre + zoom as a JSON object for QML
    return QVariant::fromValue(QJsonObject{{"lat", dCenterLat}, {"lng", dCenterLng}, {"zoom", dOutZoom}});
}

/*!
 *  \fn     bool CDP_FlightController::isEmitterDetected(double in_dEmitterLat, double in_dEmitterLng)
 *  \brief  Returns true if the given emitter is within range at the current
 *          waypoint (using the pre-computed detection table) and reports the
 *          matching emitter details to the table view.
 *
 *  \param [in] in_dEmitterLat : Emitter latitude in degrees.
 *  \param [in] in_dEmitterLng : Emitter longitude in degrees.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : true if the emitter is detected at the current waypoint.
 */
bool CDP_FlightController::isEmitterDetected(double in_dEmitterLat, double in_dEmitterLng)
{
    /// - Working values declared and initialised up front
    QString qsTemp = "";
    int iWaypointCnt = 0;
    int iEmitterCount = 0;
    int iEmitter = 0;
    double dLatDiff = 0.0;
    double dLngDiff = 0.0;

    /// - Number of valid waypoints in the detection table (clamped to the max)
    iWaypointCnt = qMin((int)g_SDP_ASPJ_Handle.m_sEmitDetWithinRange.m_u16WayPointCnt, DP_MAX_WAYPOINT_LAT_LONG);

    /// - Bail out if the current segment is outside the detection table
    if (m_currentSegment < 0 || m_currentSegment >= iWaypointCnt)
    {
        return false;
    }

    /// - Detection record for the current waypoint (reference, no copy)
    const S_DP_ASPJ_EMIT_WITHIN_RANGE &sWpData = g_SDP_ASPJ_Handle.m_sEmitDetWithinRange.m_SEmttrRange[m_currentSegment];

    /// - Build a debug/log line with the first emitter's distance and angle
    qsTemp.sprintf("Latitude : %0.2f, Longitude : %0.2f, Distance : %0.2f, Angle : %0.2f", in_dEmitterLat, in_dEmitterLng, sWpData.m_SEmtrRelParam->m_fdEmitterDist, sWpData.m_SEmtrRelParam->m_fdAOAfromWaypoint);

    /// - Number of emitters in range at this waypoint (clamped to the max)
    iEmitterCount = qMin((int)sWpData.m_u16TotalEmtCnt, DP_MAX_EMITTER_LAT_LONG);
    if (iEmitterCount == 0)
    {
        return false;
    }

    /// - Reset the match result before scanning
    m_bResult = false;
    /// - Compare the requested emitter against each in-range emitter
    for (iEmitter = 0; iEmitter < iEmitterCount; ++iEmitter)
    {
        /// - Current in-range emitter record (reference, no copy)
        const S_EMITTER_REL_PARAM &sEmitter = sWpData.m_SEmtrRelParam[iEmitter];
        /// - Absolute lat/long difference against the requested emitter
        dLatDiff = qAbs(sEmitter.m_fdEmitLat - in_dEmitterLat);
        dLngDiff = qAbs(sEmitter.m_fdEmitLon - in_dEmitterLng);
        /// - Treat as the same emitter when both differences are tiny
        if (dLatDiff < 0.001 && dLngDiff < 0.001)
        {
            /// - Mark as detected
            m_bResult = true;
//            g_SDP_ASPJ_Handle.pm_cobj_flightCockPitDisplay->DP_RWR_Bitmap_Draw_Threat_Details(g_SDP_ASPJ_Handle.pm_cobj_flightCockPitDisplay->m_S_RWRThreatList.arrS_RWRThreatSymbolInfo[i]);
            /// - Push the matching emitter details to the report table
            Sig_updateEmitterDetailsToTable(&g_SDP_ASPJ_Handle.m_sEmitDetWithinRange.m_SEmttrRange[m_currentSegment], iEmitterCount, iEmitter);

            Sig_update_GPS_Data_to_nav_display(&g_SDP_ASPJ_Handle.m_sEmitDetWithinRange.m_SEmttrRange[m_currentSegment]);

//            Sig_update_GPS_Data_to_nav_display(&g_SDP_ASPJ_Handle.m_sEmitDetWithinRange.m_SEmttrRange[m_currentSegment]);
        }
    }

    /// - Emit the log line only when at least one match was found
    if (m_bResult)
    {
//        Sig_PrintLog(qsTemp, DP_ASPJ_SUCCESS);
    }

    /// - Return the detection result
    return m_bResult;
}

/*!
 *  \fn     double CDP_FlightController::emitterDistance(double in_dEmitterLat, double in_dEmitterLng) const
 *  \brief  Straight-line distance from the aircraft to an emitter, in kilometres.
 *
 *  \param [in] in_dEmitterLat : Emitter latitude in degrees.
 *  \param [in] in_dEmitterLng : Emitter longitude in degrees.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : Distance to the emitter in kilometres.
 */
double CDP_FlightController::emitterDistance(double in_dEmitterLat, double in_dEmitterLng) const
{
    /// - Emitter coordinate and a copy of the current aircraft position
    QGeoCoordinate qgcEmitter(in_dEmitterLat, in_dEmitterLng);
    QGeoCoordinate qgcAircraftPos = m_aircraftPosition;
    /// - Distance in metres converted to kilometres
    return qgcAircraftPos.distanceTo(qgcEmitter) / 1000.0;
}

/*!
 *  \fn     double CDP_FlightController::emitterBearing(double in_dEmitterLat, double in_dEmitterLng) const
 *  \brief  Bearing (azimuth) from the aircraft to an emitter, in degrees.
 *
 *  \param [in] in_dEmitterLat : Emitter latitude in degrees.
 *  \param [in] in_dEmitterLng : Emitter longitude in degrees.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : Bearing to the emitter in degrees.
 */
double CDP_FlightController::emitterBearing(double in_dEmitterLat, double in_dEmitterLng) const
{
    /// - Emitter coordinate
    QGeoCoordinate qgcEmitter(in_dEmitterLat, in_dEmitterLng);
    /// - Azimuth from the aircraft to the emitter
    return m_aircraftPosition.azimuthTo(qgcEmitter);
}

/*!
 *  \fn     void CDP_FlightController::refreshFromSegment()
 *  \brief  Recomputes aircraft position, bearing and the completed/remaining
 *          path splits from m_currentSegment + m_segmentProgress. Used by the
 *          transport controls (next/prev/seek) so they behave like the
 *          timer-driven updatePosition().
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void CDP_FlightController::refreshFromSegment()
{
    /// - Working values / loop index declared up front
    int iLastSegment = 0;
    int iLoop = 0;
    QGeoCoordinate qgcFrom;
    QGeoCoordinate qgcTo;
    double dSegmentDistance = 0.0;
    double dDistanceAlongSegment = 0.0;
    double dAzimuth = 0.0;
    /// - Need at least two waypoints to have a segment
    if (m_flightPath.size() < 2)
    {
        return;
    }

    /// - Clamp the current segment into the valid range [0, lastSegment]
    iLastSegment = m_flightPath.size() - 2;
    if (m_currentSegment < 0)
    {
        m_currentSegment = 0;
        m_segmentProgress = 0.0;
    }
    else if (m_currentSegment > iLastSegment)
    {
        m_currentSegment = iLastSegment;
        m_segmentProgress = 1.0;
    }
    /// - Keep the fractional progress within [0, 1]
    m_segmentProgress = qBound(0.0, m_segmentProgress, 1.0);

    /// - Endpoints and length of the current segment
    qgcFrom = m_flightPath[m_currentSegment];
    qgcTo = m_flightPath[m_currentSegment + 1];
    dSegmentDistance = m_segmentDistances.value(m_currentSegment, 0.0);

    /// - Interpolate the aircraft position along the segment by the progress
    dDistanceAlongSegment = dSegmentDistance * m_segmentProgress;
    dAzimuth = qgcFrom.azimuthTo(qgcTo);
    m_aircraftPosition = (dSegmentDistance > 0.0)
        ? qgcFrom.atDistanceAndAzimuth(dDistanceAlongSegment, dAzimuth)
        : qgcFrom;

    /// - Update the heading for the current segment
    calculateBearing(qgcFrom, qgcTo);
    updateVelocityofFlight();

    /// - Rebuild the completed path (waypoints so far + current position)
    m_completedPath.clear();
    for (iLoop = 0; iLoop <= m_currentSegment; ++iLoop)
    {
        m_completedPath.append(QVariant::fromValue(m_flightPath[iLoop]));
    }
    m_completedPath.append(QVariant::fromValue(m_aircraftPosition));

    /// - Rebuild the remaining path (current position + waypoints ahead)
    m_remainingPath.clear();
    m_remainingPath.append(QVariant::fromValue(m_aircraftPosition));
    for (iLoop = m_currentSegment + 1; iLoop < m_flightPath.size(); ++iLoop)
    {
        m_remainingPath.append(QVariant::fromValue(m_flightPath[iLoop]));
    }

    /// - Update completed distance and notify QML
    m_completedDistance = computeCompletedDistance();

    emit aircraftPositionChanged();
    emit completedPathChanged();
    emit remainingPathChanged();
    emit progressPercentChanged();
}

/*!
 *  \fn     void CDP_FlightController::snapToWaypoint(int in_iWaypointIndex)
 *  \brief  Places the aircraft exactly on the given waypoint and refreshes state.
 *
 *  \param [in] in_iWaypointIndex : Target waypoint index (clamped to the path).
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void CDP_FlightController::snapToWaypoint(int in_iWaypointIndex)
{
    /// - Local clamped waypoint index, declared up front
    int iWaypointIndex = 0;
    /// - Need at least two waypoints to have a segment
    if (m_flightPath.size() < 2)
    {
        return;
    }

    /// - Clamp the requested index into the valid waypoint range
    iWaypointIndex = qBound(0, in_iWaypointIndex, m_flightPath.size() - 1);

    if (iWaypointIndex >= m_flightPath.size() - 1)
    {
        /// - Last waypoint: sit at the very end of the final segment
        m_currentSegment = m_flightPath.size() - 2;
        m_segmentProgress = 1.0;
    }
    else
    {
        /// - Otherwise sit at the start of the requested segment
        m_currentSegment = iWaypointIndex;
        m_segmentProgress = 0.0;
    }
    /// - Recompute position/bearing/splits from the new segment
    refreshFromSegment();
}

/*!
 *  \fn     void CDP_FlightController::nextWaypoint()
 *  \brief  Jumps the aircraft to the next waypoint ahead.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void CDP_FlightController::nextWaypoint()
{
    /// - Target waypoint index, declared up front
    int iTarget = 0;
    /// - The aircraft sits between m_currentSegment and m_currentSegment+1, so
    ///   the waypoint ahead is +1 (or +2 if already parked on the next one)
    iTarget = m_currentSegment + 1;
    if (m_segmentProgress >= 1.0)
    {
        iTarget = m_currentSegment + 2;
    }
    /// - Snap to the resolved target waypoint
    snapToWaypoint(iTarget);
}

/*!
 *  \fn     void CDP_FlightController::prevWaypoint()
 *  \brief  Jumps the aircraft to the previous waypoint (media-player style).
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void CDP_FlightController::prevWaypoint()
{
    /// - Target waypoint index, declared up front
    int iTarget = 0;
    /// - Jump to the start of the current leg, or to the previous waypoint if
    ///   we are already (near) the start of this one
    iTarget = m_currentSegment;
    if (m_segmentProgress <= 0.05)
    {
        iTarget = m_currentSegment - 1;
    }
    /// - Snap to the resolved target waypoint
    snapToWaypoint(iTarget);
}

/*!
 *  \fn     void CDP_FlightController::seekToPercent(double in_dPercent)
 *  \brief  Scrubs the aircraft to a given percentage (0..100) along the path.
 *
 *  \param [in] in_dPercent : Position along the path as a percentage.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void CDP_FlightController::seekToPercent(double in_dPercent)
{
    /// - Working values / loop index declared up front
    double dPercent = 0.0;
    double dTargetDistance = 0.0;
    double dAccumulated = 0.0;
    double dSegDist = 0.0;
    int iSeg = 0;
    /// - Need a valid, non-zero-length path to seek within
    if (m_flightPath.size() < 2 || m_totalDistance <= 0.0)
    {
        return;
    }

    /// - Clamp the requested percentage and convert to a target distance
    dPercent = qBound(0.0, in_dPercent, 100.0);
    dTargetDistance = (dPercent / 100.0) * m_totalDistance;

    /// - Walk the cached segment distances to find where the target lands
    for (iSeg = 0; iSeg < m_segmentDistances.size() - 1; ++iSeg)
    {
        if (dAccumulated + m_segmentDistances[iSeg] >= dTargetDistance)
        {
            break;
        }
        dAccumulated += m_segmentDistances[iSeg];
    }

    /// - Position within the landing segment
    dSegDist = m_segmentDistances.value(iSeg, 0.0);
    m_currentSegment = iSeg;
    m_segmentProgress = (dSegDist > 0.0)
        ? qBound(0.0, (dTargetDistance - dAccumulated) / dSegDist, 1.0)
        : 0.0;

    /// - Recompute position/bearing/splits from the new segment
    refreshFromSegment();
}
