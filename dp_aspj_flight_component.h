#ifndef FLIGHTCONTROLLER_H
#define FLIGHTCONTROLLER_H

#include <QObject>
#include <QGeoCoordinate>
#include <QTimer>
#include <QVariantList>
#include <QJsonObject>
#include "dp_aspj_comm.h"

class CDP_FlightController : public QObject
{
    Q_OBJECT
    // ==== Properties exposed to QML (MapView.qml binds these; each updates live
    //      through its NOTIFY signal) ====
    Q_PROPERTY(QVariantList flightPath READ flightPath NOTIFY flightPathChanged)              /*!< QML: full waypoint list */
    Q_PROPERTY(QVariantList completedPath READ completedPath NOTIFY completedPathChanged)     /*!< QML: already-flown path (green polyline) */
    Q_PROPERTY(QVariantList remainingPath READ remainingPath NOTIFY remainingPathChanged)     /*!< QML: not-yet-flown path (blue polyline) */
    Q_PROPERTY(QGeoCoordinate aircraftPosition READ aircraftPosition NOTIFY aircraftPositionChanged) /*!< QML: aircraft icon coordinate */
    Q_PROPERTY(double aircraftBearing READ aircraftBearing NOTIFY aircraftBearingChanged)     /*!< QML: aircraft heading (icon rotation / head-up rings) */
    Q_PROPERTY(bool isFlying READ isFlying NOTIFY isFlyingChanged)                            /*!< QML: flying state (drives detection FX, pulses) */
    Q_PROPERTY(double progressPercent READ progressPercent NOTIFY progressPercentChanged)     /*!< QML: 0..100 progress (seek slider, paused check) */
    Q_PROPERTY(double totalDistance READ totalDistance NOTIFY totalDistanceChanged)           /*!< QML: total path length (m) */
    Q_PROPERTY(int currentSegment READ currentSegment NOTIFY currentSegmentChanged)           /*!< QML: current waypoint index (detection re-eval) */
    Q_PROPERTY(double speed READ speed NOTIFY speedChanged)                                   /*!< QML: aircraft speed (m/s), shown in the overlay */
    Q_PROPERTY(double calculatedVelocity READ calculatedVelocity NOTIFY calculatedVelocityChanged) /*!< QML: velocity calculated between waypoints */

public:
    explicit CDP_FlightController(QObject *parent = nullptr);
    // ==== Methods callable from QML (Q_INVOKABLE) ====
    Q_INVOKABLE void initAircraftPosition(double in_dLat, double in_dLng);   // QML: park the aircraft at a coordinate

    QVariantList flightPath() const;
    QVariantList completedPath() const;
    QVariantList remainingPath() const;
    QGeoCoordinate aircraftPosition() const;
    double aircraftBearing() const;
    double updateVelocityofFlight();
    bool isFlying() const;
    double progressPercent() const;
    double totalDistance() const;
    Q_INVOKABLE void startFlight();                                         // QML: start / resume the flight
    Q_INVOKABLE void stopFlight();                                          // QML: pause the flight in place
    Q_INVOKABLE void resetFlight();                                         // QML: stop and rewind to the start
    Q_INVOKABLE void addWaypoint(double in_dLat, double in_dLng);           // QML: append one waypoint
    Q_INVOKABLE void addWaypoints(const QList<QGeoCoordinate> &in_lstWaypoints); // QML: append a batch of waypoints
    Q_INVOKABLE void flushWaypoints();                                      // QML: rebuild path segments after edits
    Q_INVOKABLE void clearWaypoints();                                      // QML: remove all waypoints
    Q_INVOKABLE void setSpeed(double in_dSpeed);                            // QML: set aircraft speed (m/s)
    Q_INVOKABLE void setLoop(bool in_bLoop);                               // QML: loop the flight until stopped
    Q_INVOKABLE void autoFitMapToPath(QGeoCoordinate &out_qgcCenter, double &out_dZoom); // QML: centre/zoom to fit the path
    Q_INVOKABLE QVariant autoFitMapToPathQml() const;                       // QML: fit result as {lat, lng, zoom}
    Q_INVOKABLE bool isEmitterDetected(double in_dEmitterLat, double in_dEmitterLng);    // QML: is this emitter in range now?
    Q_INVOKABLE double emitterDistance(double in_dEmitterLat, double in_dEmitterLng) const; // QML: aircraft->emitter distance (km)
    Q_INVOKABLE double emitterBearing(double in_dEmitterLat, double in_dEmitterLng) const;  // QML: aircraft->emitter bearing (deg)
    // Media-player style transport controls (called from the playback dock / QML)
    Q_INVOKABLE void nextWaypoint();                                        // QML: skip to the next waypoint
    Q_INVOKABLE void prevWaypoint();                                        // QML: skip to the previous waypoint
    Q_INVOKABLE void seekToPercent(double in_dPercent);                     // QML: scrub along the path, 0..100


    int currentSegment() const { return m_currentSegment; }
    double speed() const { return m_speed; }
    double calculatedVelocity() const { return m_calculatedVelocity; }

    int m_currentSegment;

signals:
    void flightPathChanged();
    void completedPathChanged();
    void remainingPathChanged();
    void currentSegmentChanged();
    void aircraftPositionChanged();
    void aircraftBearingChanged();
    void isFlyingChanged();
    void progressPercentChanged();
    void totalDistanceChanged();
    void speedChanged();
    void calculatedVelocityChanged();

    void Sig_PrintLog(QString in_qsMessage, short in_sStatus);
    void Sig_updateEmitterDetailsToTable(S_DP_ASPJ_EMIT_WITHIN_RANGE *in_sEmiDetWithinRange, int in_iCurrentSegment, int in_iCurrEmitterCnt);
    void Sig_updateEmitterTable(S_DP_ASPJ_EMIT_WITHIN_RANGE *in_sEmiDetWithinRange, int in_iCurrentSegment);
    void Sig_update_GPS_Data_to_nav_display(S_DP_ASPJ_EMIT_WITHIN_RANGE *in_sEmiDetWithinRange);

private slots:
    void updatePosition();

private:
    void calculateBearing(const QGeoCoordinate &in_qgcFrom, const QGeoCoordinate &in_qgcTo);
    void rebuildPathSegments();
    double computeTotalDistance() const;
    double computeCompletedDistance() const;
    void refreshFromSegment();               // recompute position/paths from m_currentSegment + m_segmentProgress
    void snapToWaypoint(int in_iWaypointIndex);  // place the aircraft exactly on a waypoint
    void finishOrLoop();                     // on completion: loop back to start or stop


    QList<QGeoCoordinate> m_flightPath;
    QList<double> m_segmentDistances;
    QVariantList m_completedPath;
    QVariantList m_remainingPath;
    QGeoCoordinate m_aircraftPosition;
    double m_aircraftBearing;
    bool m_isFlying;
    bool m_bResult;

    QTimer *m_timer;

    double m_segmentProgress;
    double m_speed;
    double m_calculatedVelocity;
    double m_totalDistance;
    double m_completedDistance;
    bool m_loop;
};

#endif // FLIGHTCONTROLLER_H
