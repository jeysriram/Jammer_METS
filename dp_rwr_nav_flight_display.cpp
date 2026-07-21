#include "dp_aspj_comm.h"
#include "ui_dp_rwr_nav_flight_display.h"

dp_rwr_nav_flight_display::dp_rwr_nav_flight_display(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::dp_rwr_nav_flight_display)
{
    ui->setupUi(this);
    ui->obj_gph_vw_NAV_Display->redraw();
}

dp_rwr_nav_flight_display::~dp_rwr_nav_flight_display()
{
    delete ui;
}

void dp_rwr_nav_flight_display::Slot_UpdateNavFlightDisplay(S_DP_ASPJ_EMIT_WITHIN_RANGE *in_sEmiDetWithinRange)
{
    //S_RES_INS_GPS_DATA S_INS_Data;

    /*Copy from Global to Local Structure */
    //memcpy(&S_INS_Data.S_NTP_NAV_Data, &pAppHandle->m_EmitterTrackReports_TimeTagged.S_EmitterTrackReport.mEmitter[0].mS_NavData, sizeof(S_RWR_NAV_DATA_ENGG_UNIT));

    ui->obj_gph_vw_NAV_Display->setPitch(0, true);
    ui->obj_gph_vw_NAV_Display->setRoll(0, true);
    ui->obj_gph_vw_NAV_Display->setHeading(in_sEmiDetWithinRange->m_fdHeadingAngle);
    ui->obj_gph_vw_NAV_Display->setAltitude(0);

    ui->obj_gph_vw_NAV_Display->redraw();
}

void dp_rwr_nav_flight_display::resizeEvent(QResizeEvent *event)
{
#if 0
    /* OLD: kept the view square (min of width/height) and centred, which
     * leaves empty dock-background margin whenever the dock isn't square.
     * Commented out (not deleted) in case the square/centred look is wanted
     * back later. */
    static int iPrevWidth = 0,iPrevHeight = 0;

    if((iPrevWidth != event->size().width()) || (iPrevHeight != event->size().height()))
    {
        /* To Maintain Square */
        int size = qMin(event->size().width(), event->size().height());
        ui->obj_gph_vw_NAV_Display->resize(size, size);

        /* Maintain in center */
        int iSetXPos = (this->width() - ui->obj_gph_vw_NAV_Display->width()) / 2;
        int iSetYPos = (this->height() - ui->obj_gph_vw_NAV_Display->height()) / 2;
        ui->obj_gph_vw_NAV_Display->setGeometry(iSetXPos, iSetYPos, size, size);

        iPrevWidth = event->size().width();
        iPrevHeight = event->size().height();
    }
#else
    /// - Fill the entire dock area (no square crop, no centering margin) —
    ///   qfi_EADI scales _scaleX/_scaleY independently, so it fits any
    ///   viewport size on its own.
    ui->obj_gph_vw_NAV_Display->setGeometry(0, 0, event->size().width(), event->size().height());
#endif
}
