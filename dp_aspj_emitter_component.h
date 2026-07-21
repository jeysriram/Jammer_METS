#ifndef EMITTERMODEL_H
#define EMITTERMODEL_H

#include <QAbstractListModel>
#include <QGeoCoordinate>
#include <QList>

class CDP_FlightController;

struct EmitterInfo {
    QString name;
    QGeoCoordinate coordinate;
    double power;
    double frequency;
    double range;   // radar range in km (from CSV)
};

class CDP_EmitterModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {
        NameRole = Qt::UserRole + 1,
        CoordinateRole,
        LatRole,
        LngRole,
        RangeRole
    };

    // ==== Properties exposed to QML (bound in MapView.qml via NOTIFY signals) ====
    Q_PROPERTY(QGeoCoordinate boundingCenter READ boundingCenter NOTIFY boundingCircleChanged) /*!< QML: centre of all emitters */
    Q_PROPERTY(double boundingRadius READ boundingRadius NOTIFY boundingCircleChanged)          /*!< QML: radius enclosing all emitters (m) */
    Q_PROPERTY(QVariantList emitterPath READ emitterPath NOTIFY emitterPathChanged)             /*!< QML: interpolated emitter path points */
    Q_PROPERTY(bool emitterPathVisible READ emitterPathVisible WRITE setEmitterPathVisible NOTIFY emitterPathChanged) /*!< QML: emitter-path visibility toggle */

    explicit CDP_EmitterModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &in_qmiParent = QModelIndex()) const override;
    QVariant data(const QModelIndex &in_qmiIndex, int in_iRole = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void addEmitter(const QString &in_qsName, double in_dLat, double in_dLng, double in_dPower, double in_dFrequency, double in_dRange = 0.0);
    void clear();
    // ==== Methods callable from QML (Q_INVOKABLE) ====
    Q_INVOKABLE void loadEmittersLocFromCSV(const QString &in_qsFilePath);   // QML: load emitters CSV
    Q_INVOKABLE void loadWayPointsFromCSV(const QString &in_qsFilePath);     // QML: load waypoints CSV
    bool loadCSVFile(const QString &in_qsFilePath);
    void recalculateBoundingCircle();
    QGeoCoordinate boundingCenter() const;
    double boundingRadius() const;

    // Emitter path
    QVariantList emitterPath() const;
    void setEmitterPathVisible(bool in_bVisible);
    bool emitterPathVisible() const;
    void addEmitterPathPoint(double in_dLat, double in_dLng);
    void clearEmitterPath();

    // Spline interpolation
    Q_INVOKABLE void drawSplineInterpolatedPath();                          // QML: draw the spline emitter path
    void interpolateEmitterPath(int in_iPointsPerSegment);
    Q_INVOKABLE void drawInterpolatedFlightPath();                          // QML: build the interpolated flight path

    Q_INVOKABLE void setFlightController(CDP_FlightController *in_pcFlightController); // QML: bind the flight controller

    // Auto-load initial data from default CSVs on application startup
    Q_INVOKABLE void autoLoadInitialData(const QString &in_qsWaypointsFile, const QString &in_qsEmittersFile); // QML: startup CSV auto-load

signals:
    void boundingCircleChanged();
    void emitterPathChanged();
    void emitterDataLoaded();
    void flightPathLoaded();

public:
    QString lastWaypointsFile() const { return m_qsLastWaypointsFile; }
    QString lastEmittersFile() const { return m_qsLastEmittersFile; }

private:
    CDP_FlightController *m_flightController;
    QList<EmitterInfo> m_emitters;
    QGeoCoordinate m_boundingCenter;
    double m_boundingRadius;

    QList<QGeoCoordinate> m_emitterPath;
    bool m_emitterPathVisible;

    QString m_qsLastWaypointsFile;
    QString m_qsLastEmittersFile;
};

#endif // EMITTERMODEL_H
