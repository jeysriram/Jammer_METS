/**
 * \file   dp_aspj_emitter_component.cpp
 * \brief  Emitter list model (CDP_EmitterModel) exposed to QML.
 *
 * Holds the emitter markers as a QAbstractListModel, loads emitter and waypoint
 * CSV files into the global handle, computes the bounding circle, builds the
 * spline-interpolated emitter/flight paths and triggers the emitter relative-
 * parameter (location-fixing) calculation on startup.
 *
 * \version 1.00
 *
 * \Copyright (C) 2026 Data Patterns (India) Ltd.
 */
#include "dp_aspj_comm.h"
#include "dp_spline_interpolation.h"
#include <QDebug>

/*!
 *  \fn     CDP_EmitterModel::CDP_EmitterModel(QObject *parent)
 *  \brief  Constructor — initialises the model members.
 *
 *  \param [in] parent : Qt parent object for ownership.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
CDP_EmitterModel::CDP_EmitterModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_flightController(nullptr)
    , m_emitterPathVisible(true)
{
}

/*!
 *  \fn     void CDP_EmitterModel::setFlightController(CDP_FlightController *in_pcFlightController)
 *  \brief  Binds the flight controller used to push interpolated waypoints.
 *
 *  \param [in] in_pcFlightController : Flight controller instance to bind.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void CDP_EmitterModel::setFlightController(CDP_FlightController *in_pcFlightController)
{
    /// - Store the flight controller reference
    m_flightController = in_pcFlightController;
}

/*!
 *  \fn     int CDP_EmitterModel::rowCount(const QModelIndex &in_qmiParent) const
 *  \brief  Returns the number of emitters in the model.
 *
 *  \param [in] in_qmiParent : Parent index (unused for a list model).
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : Emitter count.
 */
int CDP_EmitterModel::rowCount(const QModelIndex &in_qmiParent) const
{
    /// - Parent is unused for a flat list model
    Q_UNUSED(in_qmiParent)
    /// - Return the number of emitters
    return m_emitters.count();
}

/*!
 *  \fn     QVariant CDP_EmitterModel::data(const QModelIndex &in_qmiIndex, int in_iRole) const
 *  \brief  Returns the requested role value for the emitter at the given index.
 *
 *  \param [in] in_qmiIndex : Model index of the emitter row.
 *  \param [in] in_iRole    : Requested data role.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : QVariant for the role, invalid QVariant when out of range.
 */
QVariant CDP_EmitterModel::data(const QModelIndex &in_qmiIndex, int in_iRole) const
{
    /// - Reject invalid or out-of-range indices
    if (!in_qmiIndex.isValid() || in_qmiIndex.row() < 0 || in_qmiIndex.row() >= m_emitters.count())
    {
        return QVariant();
    }

    /// - Emitter record for the requested row (reference, no copy)
    const EmitterInfo &sEmitter = m_emitters.at(in_qmiIndex.row());
    /// - Return the field mapped to the requested role
    switch (in_iRole)
    {
    case NameRole:
        return sEmitter.name;
    case CoordinateRole:
        return QVariant::fromValue(sEmitter.coordinate);
    case LatRole:
        return sEmitter.coordinate.latitude();
    case LngRole:
        return sEmitter.coordinate.longitude();
    case RangeRole:
        return sEmitter.range;
    default:
        return QVariant();
    }
}

/*!
 *  \fn     QHash<int, QByteArray> CDP_EmitterModel::roleNames() const
 *  \brief  Maps model roles to the names used by QML bindings.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : Role-id to role-name hash.
 */
QHash<int, QByteArray> CDP_EmitterModel::roleNames() const
{
    /// - Role name table, initialised empty
    QHash<int, QByteArray> qhashRoles;
    /// - Map each role id to the QML property name
    qhashRoles[NameRole] = "name";
    qhashRoles[CoordinateRole] = "coordinate";
    qhashRoles[LatRole] = "lat";
    qhashRoles[LngRole] = "lng";
    qhashRoles[RangeRole] = "range";
    /// - Return the assembled role name table
    return qhashRoles;
}

/*!
 *  \fn     void CDP_EmitterModel::addEmitter(const QString &in_qsName, double in_dLat, double in_dLng, double in_dPower, double in_dFrequency, double in_dRange)
 *  \brief  Appends a single emitter to the model and refreshes the bounding circle.
 *
 *  \param [in] in_qsName      : Emitter display name.
 *  \param [in] in_dLat        : Emitter latitude in degrees.
 *  \param [in] in_dLng        : Emitter longitude in degrees.
 *  \param [in] in_dPower      : Emitter power.
 *  \param [in] in_dFrequency  : Emitter frequency.
 *  \param [in] in_dRange      : Emitter radar range in km.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void CDP_EmitterModel::addEmitter(const QString &in_qsName, double in_dLat, double in_dLng, double in_dPower, double in_dFrequency, double in_dRange)
{
    /// - Emitter record to populate, default-initialised
    EmitterInfo sInfo;
    /// - Begin the model insertion at the end of the list
    beginInsertRows(QModelIndex(), m_emitters.count(), m_emitters.count());

    /// - Fill the emitter fields from the supplied values
    sInfo.name = in_qsName;
    sInfo.coordinate = QGeoCoordinate(in_dLat, in_dLng);
    sInfo.power = in_dPower;
    sInfo.frequency = in_dFrequency;
    sInfo.range = in_dRange;
    /// - Append the record and finish the insertion
    m_emitters.append(sInfo);
    endInsertRows();
    /// - Recompute the bounding circle for the new emitter set
    recalculateBoundingCircle();
}

/*!
 *  \fn     void CDP_EmitterModel::clear()
 *  \brief  Removes all emitters and refreshes the bounding circle.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void CDP_EmitterModel::clear()
{
    /// - Reset the model and clear the emitter list
    beginResetModel();
    m_emitters.clear();
    endResetModel();
    /// - Optional flight controller cleanup hook (kept disabled)
    if (m_flightController)
    {
        //m_flightController->clearWaypoints();
    }
    /// - Recompute the (now empty) bounding circle
    recalculateBoundingCircle();
}

/*!
 *  \fn     void CDP_EmitterModel::recalculateBoundingCircle()
 *  \brief  Computes the centre and radius of the circle enclosing all emitters
 *          (with a 5 km padding).
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void CDP_EmitterModel::recalculateBoundingCircle()
{
    /// - Bounding-box / per-emitter working values, declared up front
    double dMinLat = 90.0;
    double dMaxLat = -90.0;
    double dMinLng = 180.0;
    double dMaxLng = -180.0;
    double dLat = 0.0;
    double dLng = 0.0;
    double dMaxDist = 0.0;
    double dDist = 0.0;
    /// - No emitters: reset the bounding circle and notify
    if (m_emitters.isEmpty())
    {
        m_boundingCenter = QGeoCoordinate();
        m_boundingRadius = 0.0;
        emit boundingCircleChanged();
        return;
    }

    /// - Find the min/max latitude and longitude across all emitters
    for (const EmitterInfo &sEmitter : m_emitters)
    {
        dLat = sEmitter.coordinate.latitude();
        dLng = sEmitter.coordinate.longitude();
        if (dLat < dMinLat)
        {
            dMinLat = dLat;
        }
        if (dLat > dMaxLat)
        {
            dMaxLat = dLat;
        }
        if (dLng < dMinLng)
        {
            dMinLng = dLng;
        }
        if (dLng > dMaxLng)
        {
            dMaxLng = dLng;
        }
    }

    /// - Centre of the bounding box
    m_boundingCenter = QGeoCoordinate((dMinLat + dMaxLat) / 2.0, (dMinLng + dMaxLng) / 2.0);

    /// - Radius = farthest emitter from the centre
    for (const EmitterInfo &sEmitter : m_emitters)
    {
        dDist = m_boundingCenter.distanceTo(sEmitter.coordinate);
        if (dDist > dMaxDist)
        {
            dMaxDist = dDist;
        }
    }
    /// - Add 5 km of padding and notify QML
    m_boundingRadius = dMaxDist + 5000; // +5km padding
    emit boundingCircleChanged();
}

/*!
 *  \fn     QGeoCoordinate CDP_EmitterModel::boundingCenter() const
 *  \brief  Returns the bounding-circle centre.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : Bounding-circle centre coordinate.
 */
QGeoCoordinate CDP_EmitterModel::boundingCenter() const
{
    /// - Return the cached bounding centre
    return m_boundingCenter;
}

/*!
 *  \fn     double CDP_EmitterModel::boundingRadius() const
 *  \brief  Returns the bounding-circle radius in metres.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : Bounding-circle radius in metres.
 */
double CDP_EmitterModel::boundingRadius() const
{
    /// - Return the cached bounding radius
    return m_boundingRadius;
}

/*!
 *  \fn     void CDP_EmitterModel::loadEmittersLocFromCSV(const QString &in_qsFilePath)
 *  \brief  Loads emitter locations (lat, lng, range km) from a CSV file into the
 *          model and the global emitter table.
 *
 *  \param [in] in_qsFilePath : Path to the emitter CSV file.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void CDP_EmitterModel::loadEmittersLocFromCSV(const QString &in_qsFilePath)
{
    /// - CSV parsing working values, declared up front
    int iCount = 0;
    int iLoop = 0;
    QString qsLine;
    QString qsName;
    QStringList qslFields;
    bool bOkLat = false;
    bool bOkLng = false;
    double dLat = 0.0;
    double dLng = 0.0;
    unsigned short usRange_km = 0;

    /// - Clear any existing emitter data and path
    clear();
    clearEmitterPath();

    /// - Open the CSV file for reading
    QFile qfFile(in_qsFilePath);
    if (!qfFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Cannot open emitter CSV:" << in_qsFilePath;
        return;
    }

    /// - Text stream over the file; skip the header row
    QTextStream qtsIn(&qfFile);
    qtsIn.readLine();

    /// - Read each data row up to the row limit
    while (!qtsIn.atEnd() && iCount < DP_MAX_ROWS)
    {
        /// - Read and trim the current line
        qsLine = qtsIn.readLine().trimmed();
        if (qsLine.isEmpty())
        {
            continue;
        }

        /// - Split into comma-separated fields; need at least lat and lng
        qslFields = qsLine.split(',');
        if (qslFields.size() < 2)
        {
            continue;
        }

        /// - Parse latitude, longitude and range
        dLat = qslFields[0].toDouble(&bOkLat);
        dLng = qslFields[1].toDouble(&bOkLng);
        usRange_km = qslFields[2].toShort();

        /// - Skip rows with invalid lat/lng, otherwise store to the global table
        if (!bOkLat || !bOkLng)
        {
            continue;
        }
        else
        {
            g_SDP_ASPJ_Handle.m_sEmitterLatLon[iCount].m_fdEmitLat = dLat;
            g_SDP_ASPJ_Handle.m_sEmitterLatLon[iCount].m_fdEmitLon = dLng;
            g_SDP_ASPJ_Handle.m_sEmitterLatLon[iCount].m_u16RdrRange_km = usRange_km;
        }

        /// - Add the emitter to the model with an auto-generated name
        qsName = QString("Emitter_%1").arg(iCount + 1);
        addEmitter(qsName, dLat, dLng, 0.0, 0.0, (double)usRange_km);
        iCount++;
    }

    /// - Store the total emitter count in the global table
    g_SDP_ASPJ_Handle.m_sEmitterLatLon[0].m_u16EmtLatLongCnt = iCount;

#ifdef DP_ASPJ_DEBUG_PRINTS
    /// - Optional debug dump of the loaded emitters
    qDebug() << "EMITTER MODEL: loaded" << iCount << "emitters";
    for (iLoop = 0; iLoop < iCount; ++iLoop)
    {
        qDebug() << "  Emitter" << iLoop << ":" << m_emitters[iLoop].name
                 << "lat=" << m_emitters[iLoop].coordinate.latitude()
                 << "lng=" << m_emitters[iLoop].coordinate.longitude();
    }
#endif

    /// - Notify listeners that emitter data was loaded
    emit emitterPathChanged();
    emit emitterDataLoaded();

    /// - Close the file and notify again (kept as in the original flow)
    qfFile.close();
    emit emitterPathChanged();
    emit emitterDataLoaded();

#ifdef DP_ASPJ_DEBUG_PRINTS
    qDebug() << "Loaded" << iCount << "emitters from" << in_qsFilePath;
#endif

    return;
}

/*!
 *  \fn     void CDP_EmitterModel::loadWayPointsFromCSV(const QString &in_qsFilePath)
 *  \brief  Loads flight waypoints (lat, lng) from a CSV file into the global
 *          table and the emitter path, then recomputes emitter detection.
 *
 *  \param [in] in_qsFilePath : Path to the waypoints CSV file.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void CDP_EmitterModel::loadWayPointsFromCSV(const QString &in_qsFilePath)
{
    /// - CSV parsing working values, declared up front
    int iCount = 0;
    QString qsLine;
    QStringList qslFields;
    bool bOkLat = false;
    bool bOkLng = false;
    double dLat = 0.0;
    double dLng = 0.0;
#ifdef DP_ASPJ_DEBUG_PRINTS
    int iWaypoint = 0;
#endif
    /// - Clear any previously loaded emitter path
    clearEmitterPath();

    /// - Open the CSV file for reading
    QFile qfFile(in_qsFilePath);
    if (!qfFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }

    /// - Zero the global waypoint table before filling it
    memset(&g_SDP_ASPJ_Handle.m_sWaypointsLatLon, 0, sizeof(g_SDP_ASPJ_Handle.m_sWaypointsLatLon));

    /// - Text stream over the file; skip the header row
    QTextStream qtsIn(&qfFile);
    qtsIn.readLine();

    /// - Read each data row up to the row limit
    while (!qtsIn.atEnd() && iCount < DP_MAX_ROWS)
    {
        /// - Read and trim the current line
        qsLine = qtsIn.readLine().trimmed();
        if (qsLine.isEmpty())
        {
            continue;
        }

        /// - Split into comma-separated fields; need at least lat and lng
        qslFields = qsLine.split(',');
        if (qslFields.size() < 2)
        {
            continue;
        }

        /// - Parse latitude and longitude
        dLat = qslFields[0].toDouble(&bOkLat);
        dLng = qslFields[1].toDouble(&bOkLng);

        /// - Skip rows with invalid lat/lng
        if (!bOkLat || !bOkLng)
        {
            continue;
        }

        /// - Store the waypoint in the global table and the emitter path
        g_SDP_ASPJ_Handle.m_sWaypointsLatLon.m_arrfdLat[iCount] = dLat;
        g_SDP_ASPJ_Handle.m_sWaypointsLatLon.m_arrfdLon[iCount] = dLng;
        addEmitterPathPoint(dLat, dLng);
        iCount++;
    }

    /// - Store the waypoint count and close the file
    g_SDP_ASPJ_Handle.m_sWaypointsLatLon.m_u32LatLongCnt = iCount;
    qfFile.close();
    /// - Notify listeners that the path/data changed
    emit emitterPathChanged();
    emit emitterDataLoaded();

#ifdef DP_ASPJ_DEBUG_PRINTS
    qDebug() << "Loaded" << iCount << "waypoints, waypointCnt in struct=" << g_SDP_ASPJ_Handle.m_sWaypointsLatLon.m_u32LatLongCnt;
#endif

    /// - Recalculate emitter detection for the new waypoints
    memset(&g_SDP_ASPJ_Handle.m_sEmitDetWithinRange, 0, sizeof(g_SDP_ASPJ_Handle.m_sEmitDetWithinRange));
    GetEmitterRelativeParameters(&g_SDP_ASPJ_Handle.m_sWaypointsLatLon, g_SDP_ASPJ_Handle.m_sEmitterLatLon, &g_SDP_ASPJ_Handle.m_sEmitDetWithinRange);


#ifdef DP_ASPJ_DEBUG_PRINTS
    qDebug() << "GetEmitterRelativeParameters: waypointCnt=" << (int)g_SDP_ASPJ_Handle.m_sEmitDetWithinRange.m_u16WayPointCnt;
    for (iWaypoint = 0; iWaypoint < g_SDP_ASPJ_Handle.m_sEmitDetWithinRange.m_u16WayPointCnt; ++iWaypoint)
    {
        qDebug() << "  Waypoint" << iWaypoint << "detected emitters:" << (int)g_SDP_ASPJ_Handle.m_sEmitDetWithinRange.m_SEmttrRange[iWaypoint].m_u16TotalEmtCnt;
    }
#endif
}

/*!
 *  \fn     QVariantList CDP_EmitterModel::emitterPath() const
 *  \brief  Returns the emitter path points as a QVariantList for QML.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : QVariantList of QGeoCoordinate path points.
 */
QVariantList CDP_EmitterModel::emitterPath() const
{
    /// - Output list, initialised empty
    QVariantList lstPath;
    /// - Wrap every path coordinate into the variant list
    for (const QGeoCoordinate &qgcPoint : m_emitterPath)
    {
        lstPath.append(QVariant::fromValue(qgcPoint));
    }
    /// - Return the assembled list
    return lstPath;
}

/*!
 *  \fn     void CDP_EmitterModel::setEmitterPathVisible(bool in_bVisible)
 *  \brief  Sets the emitter-path visibility flag and notifies QML on change.
 *
 *  \param [in] in_bVisible : New visibility state.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void CDP_EmitterModel::setEmitterPathVisible(bool in_bVisible)
{
    /// - Update and notify only when the value actually changes
    if (m_emitterPathVisible != in_bVisible)
    {
        m_emitterPathVisible = in_bVisible;
        emit emitterPathChanged();
    }
}

/*!
 *  \fn     bool CDP_EmitterModel::emitterPathVisible() const
 *  \brief  Returns whether the emitter path is visible.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : true if the emitter path is visible.
 */
bool CDP_EmitterModel::emitterPathVisible() const
{
    /// - Return the visibility flag
    return m_emitterPathVisible;
}

/*!
 *  \fn     void CDP_EmitterModel::addEmitterPathPoint(double in_dLat, double in_dLng)
 *  \brief  Appends a coordinate to the emitter path.
 *
 *  \param [in] in_dLat : Point latitude in degrees.
 *  \param [in] in_dLng : Point longitude in degrees.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void CDP_EmitterModel::addEmitterPathPoint(double in_dLat, double in_dLng)
{
    /// - Append the coordinate to the emitter path
    m_emitterPath.append(QGeoCoordinate(in_dLat, in_dLng));
}

/*!
 *  \fn     void CDP_EmitterModel::clearEmitterPath()
 *  \brief  Removes all emitter path points.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void CDP_EmitterModel::clearEmitterPath()
{
    /// - Clear the emitter path list
    m_emitterPath.clear();
}

// ============================================================
// Spline Interpolated Emitter Path
// ============================================================

/*!
 *  \fn     void CDP_EmitterModel::interpolateEmitterPath(int in_iPointsPerSegment)
 *  \brief  Builds a spline-interpolated emitter path from the raw emitter
 *          coordinates.
 *
 *  \param [in] in_iPointsPerSegment : Interpolated points to generate per segment.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void CDP_EmitterModel::interpolateEmitterPath(int in_iPointsPerSegment)
{
    /// - Working values / buffers, declared and initialised up front
    int iEmitterCount = 0;
    int iNumSegments = 0;
    int iOutCount = 0;
    int iLoop = 0;
    double *pdLatIn = nullptr;
    double *pdLngIn = nullptr;
    double *pdLatOut = nullptr;
    double *pdLngOut = nullptr;
    S32BIT s32Result = 0;
    /// - Need at least 3 emitters and a positive point count
    iEmitterCount = m_emitters.size();
    if (iEmitterCount < 3 || in_iPointsPerSegment <= 0)
    {
        return;
    }

    /// - Extract raw lat/lng arrays from the emitter coordinates
    pdLatIn = new double[iEmitterCount];
    pdLngIn = new double[iEmitterCount];
    for (iLoop = 0; iLoop < iEmitterCount; iLoop++)
    {
        pdLatIn[iLoop] = m_emitters.at(iLoop).coordinate.latitude();
        pdLngIn[iLoop] = m_emitters.at(iLoop).coordinate.longitude();
    }

    /// - Allocate the interpolated output buffers
    iNumSegments = iEmitterCount - 1;
    iOutCount = (iNumSegments * in_iPointsPerSegment) + 1;
    pdLatOut = new double[iOutCount];
    pdLngOut = new double[iOutCount];

    /// - Run the parametric spline interpolation
    s32Result = ParametricSplineInterpolate(
        pdLatIn, pdLngIn, (U32BIT)iEmitterCount, (U32BIT)in_iPointsPerSegment,
        pdLatOut, pdLngOut
    );

    /// - Free the input buffers (no longer needed)
    delete[] pdLatIn;
    delete[] pdLngIn;

    /// - On interpolation failure, free outputs and bail out
    if (s32Result != ERR_SPLINE_SUCCESS)
    {
        qDebug() << "Spline error:" << s32Result;
        delete[] pdLatOut;
        delete[] pdLngOut;
        return;
    }

    /// - Replace the path with the interpolated points
    clearEmitterPath();
    for (iLoop = 0; iLoop < iOutCount; iLoop++)
    {
        addEmitterPathPoint(pdLatOut[iLoop], pdLngOut[iLoop]);
    }


#ifdef DP_ASPJ_DEBUG_PRINTS
    qDebug() << "Interpolated path points:" << m_emitterPath.size();
#endif

    /// - Free the output buffers
    delete[] pdLatOut;
    delete[] pdLngOut;

    /// - Notify listeners that the path changed
    emit emitterPathChanged();
}

/*!
 *  \fn     void CDP_EmitterModel::drawSplineInterpolatedPath()
 *  \brief  Convenience wrapper that interpolates the emitter path at 50 points
 *          per segment (blue line on the map).
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void CDP_EmitterModel::drawSplineInterpolatedPath()
{
    /// - Interpolate the emitter path at the default resolution
    interpolateEmitterPath(50);
}

/*!
 *  \fn     void CDP_EmitterModel::drawInterpolatedFlightPath()
 *  \brief  Builds a spline-interpolated flight path from the emitter path,
 *          pushes it to the flight controller as waypoints and parks the
 *          aircraft at the first point.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void CDP_EmitterModel::drawInterpolatedFlightPath()
{
    /// - Working values / buffers / waypoint list, declared up front
    int iPointCount = 0;
    int iPointsPerSegment = 50;
    int iNumSegments = 0;
    int iOutCount = 0;
    int iLoop = 0;
    double *pdLatIn = nullptr;
    double *pdLngIn = nullptr;
    double *pdLatOut = nullptr;
    double *pdLngOut = nullptr;
    S32BIT s32Result = 0;
    QList<QGeoCoordinate> lstWaypoints;
    /// - Need a bound controller and a non-empty emitter path
    if (!m_flightController || m_emitterPath.isEmpty())
    {
        return;
    }

    /// - Need at least two points to build a segment
    iPointCount = m_emitterPath.size();

    if (iPointCount < 2)
    {
        return;
    }

    /// - Extract raw lat/lng arrays from the emitter path
    pdLatIn = new double[iPointCount];
    pdLngIn = new double[iPointCount];
    for (iLoop = 0; iLoop < iPointCount; iLoop++)
    {
        pdLatIn[iLoop] = m_emitterPath.at(iLoop).latitude();
        pdLngIn[iLoop] = m_emitterPath.at(iLoop).longitude();
    }

    /// - Allocate the interpolated output buffers
    iNumSegments = iPointCount - 1;
    iOutCount = (iNumSegments * iPointsPerSegment) + 1;
    pdLatOut = new double[iOutCount];
    pdLngOut = new double[iOutCount];

    /// - Run the parametric spline interpolation
    s32Result = ParametricSplineInterpolate(
        pdLatIn, pdLngIn, (U32BIT)iPointCount, (U32BIT)iPointsPerSegment,
        pdLatOut, pdLngOut
    );

    /// - Free the input buffers (no longer needed)
    delete[] pdLatIn;
    delete[] pdLngIn;

    /// - On interpolation failure, free outputs and bail out
    if (s32Result != ERR_SPLINE_SUCCESS)
    {
        qDebug() << "Interpolated flight path error:" << s32Result;
        delete[] pdLatOut;
        delete[] pdLngOut;
        return;
    }

    /// - Build the waypoint list and mirror it into the global table
    lstWaypoints.reserve(iOutCount);
    for (iLoop = 0; iLoop < iOutCount; iLoop++)
    {
        /// - fill the interpolated waypoints into global structure
        g_SDP_ASPJ_Handle.m_sWaypointsLatLon.m_arrfdLat[iLoop] = pdLatOut[iLoop];
        g_SDP_ASPJ_Handle.m_sWaypointsLatLon.m_arrfdLon[iLoop] = pdLngOut[iLoop];

        lstWaypoints.append(QGeoCoordinate(pdLatOut[iLoop], pdLngOut[iLoop]));
    }

    /// - Store the waypoint count and binning info in the global table
    g_SDP_ASPJ_Handle.m_sWaypointsLatLon.m_u32LatLongCnt = iOutCount;
    g_SDP_ASPJ_Handle.m_sWaypointsLatLon.m_u16BinFlag = false;
    g_SDP_ASPJ_Handle.m_sWaypointsLatLon.m_u16BinningCnt = iPointsPerSegment;

    /// - Free the output buffers
    delete[] pdLatOut;
    delete[] pdLngOut;

    /// - Replace the controller waypoints with the interpolated batch, then flush
    m_flightController->clearWaypoints();
    m_flightController->addWaypoints(lstWaypoints);
    m_flightController->flushWaypoints();


#ifdef DP_ASPJ_DEBUG_PRINTS
    qDebug() << "Interpolated flight path waypoints:" << iOutCount;
#endif

    /// - Reset and place the aircraft at the first waypoint
    m_flightController->resetFlight();
    m_flightController->initAircraftPosition(
        m_emitterPath.first().latitude(),
        m_emitterPath.first().longitude()
    );
}

// ============================================================
// Auto-load initial data on application startup
// ============================================================
/*!
 *  \fn     void CDP_EmitterModel::autoLoadInitialData(const QString &in_qsWaypointsFile, const QString &in_qsEmittersFile)
 *  \brief  Startup helper: loads the emitter and waypoint CSVs, builds the
 *          interpolated flight path and runs the location-fixing calculation.
 *
 *  \param [in] in_qsWaypointsFile : Path to the waypoints CSV (may be empty).
 *  \param [in] in_qsEmittersFile  : Path to the emitters CSV (may be empty).
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void CDP_EmitterModel::autoLoadInitialData(const QString &in_qsWaypointsFile, const QString &in_qsEmittersFile)
{
#ifdef DP_ASPJ_DEBUG_PRINTS
    int iWaypoint = 0;
#endif

    FDOUBLE fdTimeTaken_ms = 0.0;


    /// - Nothing to do without a bound flight controller
    if (!m_flightController)
    {
        return;
    }

    /// - Step 1: Load the emitters CSV
    if (!in_qsEmittersFile.isEmpty())
    {
        loadEmittersLocFromCSV(in_qsEmittersFile);
    }

    /// - Step 2: Load the waypoints CSV
    if (!in_qsWaypointsFile.isEmpty())
    {
        loadWayPointsFromCSV(in_qsWaypointsFile);
    }

    /// - Step 3: Run interpolation — blue emitter path + green/blue flight paths
    if (!in_qsWaypointsFile.isEmpty())
    {
        drawInterpolatedFlightPath();
    }

    // ============================================================
    // Do Location fixing  on application startup
    // ============================================================
    /// - Calculate the Emitter Relative Parameters
    memset(&g_SDP_ASPJ_Handle.m_sEmitDetWithinRange, 0, sizeof(g_SDP_ASPJ_Handle.m_sEmitDetWithinRange));

    /// - write the time taken to travel from one waypoint to another in milli seconds
    fdTimeTaken_ms  = 50;

    /// - Calculate the Emitter Relative Parameters
    Calculate_TotalDistance_And_RelParam(&g_SDP_ASPJ_Handle.m_sWaypointsLatLon, g_SDP_ASPJ_Handle.m_sEmitterLatLon, fdTimeTaken_ms, &g_SDP_ASPJ_Handle.m_sEmitDetWithinRange);


#ifdef DP_ASPJ_DEBUG_PRINTS
    qDebug() << "GetEmitterRelativeParameters: waypointCnt=" << (int)g_SDP_ASPJ_Handle.m_sEmitDetWithinRange.m_u16WayPointCnt;
    for (iWaypoint = 0; iWaypoint < (int)g_SDP_ASPJ_Handle.m_sEmitDetWithinRange.m_u16WayPointCnt; ++iWaypoint)
    {
        qDebug() << "  Waypoint" << iWaypoint << "detected emitters:" << (int)g_SDP_ASPJ_Handle.m_sEmitDetWithinRange.m_SEmttrRange[iWaypoint].m_u16TotalEmtCnt;
    }
    WriteWayPointEmitterLatLong(g_SDP_ASPJ_Handle.m_sEmitDetWithinRange.m_u16WayPointCnt,  g_SDP_ASPJ_Handle.m_sEmitterLatLon->m_u16EmtLatLongCnt, &g_SDP_ASPJ_Handle.m_sEmitDetWithinRange);
#endif
    /// - Step 4: Stop the flight (park the aircraft ready to start)
    m_flightController->stopFlight();

}

