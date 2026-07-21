#ifndef DP_ASPJ_LOADSETTINGS_H
#define DP_ASPJ_LOADSETTINGS_H

#include <QDialog>
#include <QDebug>
#include <QGeoCoordinate>
#include <QList>

#include "dp_aspj_comm.h"

namespace Ui {
class CLoadSettings;
}

class CLoadSettings : public QDialog
{
    Q_OBJECT

public:
    explicit CLoadSettings(QWidget *parent = nullptr);
    ~CLoadSettings();

    void setEmitterModel(CDP_EmitterModel *model);
    void setFlightController(CDP_FlightController *fc);

    QString waypointsFile() const;
    QString emittersFile() const;
    int speedValue() const;

private slots:
    void on_pbBrowse_waypoints_clicked();
    void on_pbBrowse_emitters_clicked();
    void on_pb_loadSettings_clicked();
    void on_pb_close_clicked();

signals:
    void waypointsFileLoaded(const QString &filePath);
    void emittersFileLoaded(const QString &filePath);
    void speedValueChanged(int speed);
    void drawSplineInterpolatedPath();

protected:
    void showEvent(QShowEvent *event) override;

private:
    void setupTable();

    Ui::CLoadSettings *ui;

    CDP_FlightController *m_flightController;
    CDP_EmitterModel     *m_emitterModel;
};

#endif // DP_ASPJ_LOADSETTINGS_H
