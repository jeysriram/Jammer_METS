#include "dp_aspj_loadsettings.h"
#include "ui_cloadsettings.h"

#include <QFileDialog>
#include <QSpinBox>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QGeoCoordinate>
#include <QFileInfo>
#include <QDebug>

CLoadSettings::CLoadSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CLoadSettings),
    m_flightController(nullptr),
    m_emitterModel(nullptr)
{
    ui->setupUi(this);
    setupTable();
}

CLoadSettings::~CLoadSettings()
{
    delete ui;
}

void CLoadSettings::setEmitterModel(CDP_EmitterModel *model)
{
    m_emitterModel = model;
}

void CLoadSettings::setFlightController(CDP_FlightController *fc)
{
    m_flightController = fc;
}

QString CLoadSettings::waypointsFile() const
{
    return ui->tbw_loadSettings->item(0, 1) ? ui->tbw_loadSettings->item(0, 1)->text() : QString();
}

QString CLoadSettings::emittersFile() const
{
    return ui->tbw_loadSettings->item(1, 1) ? ui->tbw_loadSettings->item(1, 1)->text() : QString();
}

int CLoadSettings::speedValue() const
{
    QWidget *widget = ui->tbw_loadSettings->cellWidget(2, 0);
    QSpinBox *spin = qobject_cast<QSpinBox *>(widget);
    return spin ? spin->value() : 50000;
}

void CLoadSettings::setupTable()
{
    ui->tbw_loadSettings->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tbw_loadSettings->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QTableWidgetItem *item_wp = new QTableWidgetItem(tr("Not selected"));
    item_wp->setFlags(item_wp->flags() & ~Qt::ItemIsEditable);
    ui->tbw_loadSettings->setItem(0, 1, item_wp);

    QTableWidgetItem *item_em = new QTableWidgetItem(tr("Not selected"));
    item_em->setFlags(item_em->flags() & ~Qt::ItemIsEditable);
    ui->tbw_loadSettings->setItem(1, 1, item_em);

    QPushButton *btn_waypoints = new QPushButton("...");
    QObject::connect(btn_waypoints, &QPushButton::clicked, this, &CLoadSettings::on_pbBrowse_waypoints_clicked);
    ui->tbw_loadSettings->setCellWidget(0, 0, btn_waypoints);

    QPushButton *btn_emitters = new QPushButton("...");
    QObject::connect(btn_emitters, &QPushButton::clicked, this, &CLoadSettings::on_pbBrowse_emitters_clicked);
    ui->tbw_loadSettings->setCellWidget(1, 0, btn_emitters);

    QSpinBox *spinSpeed = new QSpinBox();
    spinSpeed->setRange(1, 50000);
    spinSpeed->setValue(5000);
    spinSpeed->setSuffix(" m/s");
    ui->tbw_loadSettings->setCellWidget(2, 0, spinSpeed);

    ui->tbw_loadSettings->setColumnWidth(1, 550);
}

void CLoadSettings::on_pbBrowse_waypoints_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(
        this, "Select Waypoints CSV", QString(), "CSV Files (*.csv);;All Files (*)");
    if (!filePath.isEmpty()) {
        QTableWidgetItem *item = ui->tbw_loadSettings->item(0, 1);
        if (item) {
            item->setText(filePath);
        }
    }
}

void CLoadSettings::on_pbBrowse_emitters_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(
        this, "Select Emitter Locations CSV", QString(), "CSV Files (*.csv);;All Files (*)");
    if (!filePath.isEmpty()) {
        QTableWidgetItem *item = ui->tbw_loadSettings->item(1, 1);
        if (item) {
            item->setText(filePath);
        }
    }
}

void CLoadSettings::on_pb_loadSettings_clicked()
{

    FDOUBLE fdTimeTaken_ms = 0.0;

    QString waypointsFile = ui->tbw_loadSettings->item(0, 1) ? ui->tbw_loadSettings->item(0, 1)->text() : QString();
    QString emittersFile  = ui->tbw_loadSettings->item(1, 1) ? ui->tbw_loadSettings->item(1, 1)->text() : QString();

    if (waypointsFile.isEmpty() || waypointsFile == tr("Not selected")) {
        waypointsFile = QString();
    }
    if (emittersFile.isEmpty() || emittersFile == tr("Not selected")) {
        emittersFile = QString();
    }

    if (waypointsFile.isEmpty() && emittersFile.isEmpty()) {
        QMessageBox::warning(this, "No Files Selected",
                             "Please select at least one CSV file before loading settings.");
        return;
    }

    // Apply speed from spinbox before loading
    int speed = speedValue();
    m_flightController->setSpeed(speed);

#ifdef DP_ASPJ_DEBUG_PRINTS
    qDebug() << "Speed set to:" << speed << "m/s";
#endif


    // Step 1: Load emitters CSV
    if (!emittersFile.isEmpty())
    {
        m_emitterModel->loadEmittersLocFromCSV(emittersFile);
    }

    // Step 2: Load waypoints CSV
    if (!waypointsFile.isEmpty())
    {
        m_emitterModel->loadWayPointsFromCSV(waypointsFile);
    }

    // Step 3: Run interpolation — blue emitter path + green/blue flight paths
    if (!waypointsFile.isEmpty())
    {
        m_emitterModel->drawInterpolatedFlightPath();
    }

    /// - Calculate the Emitter Relative Parameters
    memset(&g_SDP_ASPJ_Handle.m_sEmitDetWithinRange, 0, sizeof(g_SDP_ASPJ_Handle.m_sEmitDetWithinRange));

    /// - write the time taken to travel from one waypoint to another in milli seconds
    fdTimeTaken_ms  = 50;

    /// - Calculate the Emitter Relative Parameters
    Calculate_TotalDistance_And_RelParam(&g_SDP_ASPJ_Handle.m_sWaypointsLatLon, g_SDP_ASPJ_Handle.m_sEmitterLatLon, fdTimeTaken_ms, &g_SDP_ASPJ_Handle.m_sEmitDetWithinRange);

    /// - Remember the loaded CSVs in the input config so the next application
    ///   start is redirected to these same files
    DP_SaveInputCsvConfig(waypointsFile, emittersFile);

    accept();
}

void CLoadSettings::on_pb_close_clicked()
{
    reject();
}

void CLoadSettings::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);

    QString qsWaypoints;
    QString qsEmitters;
    /// - Resolved against the application directory (not the CWD) by
    ///   DP_ReadInputCsvConfig, so this works no matter where the app is
    ///   launched from or which PC it runs on
    DP_ReadInputCsvConfig(qsWaypoints, qsEmitters);

    QTableWidgetItem *item_wp = ui->tbw_loadSettings->item(0, 1);
    if (item_wp)
    {
        if (!qsWaypoints.isEmpty() && QFileInfo(qsWaypoints).exists())
        {
            item_wp->setText(qsWaypoints);
        }
        else
        {
            item_wp->setText(tr("Not selected"));
        }
    }

    QTableWidgetItem *item_em = ui->tbw_loadSettings->item(1, 1);
    if (item_em)
    {
        if (!qsEmitters.isEmpty() && QFileInfo(qsEmitters).exists())
        {
            item_em->setText(qsEmitters);
        }
        else
        {
            item_em->setText(tr("Not selected"));
        }
    }
}
