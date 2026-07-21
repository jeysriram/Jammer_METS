#include "dp_aspj_comm.h"

#include "ui_dp_rwr_ins_gps.h"

dp_rwr_ins_gps::dp_rwr_ins_gps(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::dp_rwr_ins_gps)
{
    ui->setupUi(this);

    /* Assign LED image */
    qpDefaultState = new QPixmap(":/icons/4_black_icon.ico");
    qpPassState = new QPixmap(":/icons/1_green_icon.ico");
    qpFailState = new QPixmap(":/icons/2_red_icon.ico");

    ui->obj_lbl_GpsLed->setPixmap(*qpDefaultState);
    ui->obj_psh_btn_Read->hide();
}

dp_rwr_ins_gps::~dp_rwr_ins_gps()
{
    delete ui;
}

void dp_rwr_ins_gps::on_obj_psh_btn_Read_clicked()
{
#if 0
    short sRetVal = 0;
    S_RES_INS_GPS_DATA S_INS_Data;

#ifdef DP_RWR_ETHERNET_FW_COMM_ENABLE
    char arrcData[DP_RWR_ERROR_BUFF_SIZE] = {0};
#endif

#ifdef DP_RWR_ETHERNET_FW_COMM_ENABLE
    if(pAppHandle->gS_RWR_Eth_Config.bISConnected == false) //Socket Connection
    {
        pAppHandle->DP_RWR_GetErrorMsg(DP_RWR_APP_NO_COMM_LNK, arrcData);
        pAppHandle->PrintLog(arrcData, DP_RWR_PRINTLOG_FAILURES);
        //QMessageBox::critical(this, this->windowTitle(), arrcData);
        return;
    }
#endif

    sRetVal = pAppHandle->DP_RWR_Get_INS_Data();
    if(sRetVal != DP_RWR_SUCCESS)
    {
        pAppHandle->PrintLog("INS GPS Data Read failure", DP_RWR_PRINTLOG_FAILURES);
        //QMessageBox::critical(this, this->windowTitle(), "INS GPS Data Read failure");
        return;
    }

    /*Copy from Global to Local Structure */
    memcpy(&S_INS_Data, &pAppHandle->gS_INS_Data, sizeof(S_RES_INS_GPS_DATA));

    if((S_INS_Data.ucGNSS_ConnectionSts == 0) || (S_INS_Data.ucIs_INS_GPS_Fix_Valid == 0))
    {
       ui->obj_lbl_GpsLed->setPixmap(*qpFailState);
    }
    else
    {
       ui->obj_lbl_GpsLed->setPixmap(*qpPassState);
    }

    ui->obj_ln_edt_fLatitude->setText(QString::number(S_INS_Data.S_NTP_NAV_Data.dLatitude_deg,'f',6));
    ui->obj_ln_edt_fLongitude->setText(QString::number(S_INS_Data.S_NTP_NAV_Data.dLongitude_deg,'f',6));
    ui->obj_ln_edt_fTrueHeading->setText(QString::number(S_INS_Data.S_NTP_NAV_Data.fTrueHeading_deg,'f',1));
    ui->obj_ln_edt_fRoll->setText(QString::number(S_INS_Data.S_NTP_NAV_Data.fRollAngle_deg,'f',1));
    ui->obj_ln_edt_fPitch->setText(QString::number(S_INS_Data.S_NTP_NAV_Data.fPitchAngle_deg,'f',1));
    ui->obj_ln_edt_iAltitude->setText(QString::number(S_INS_Data.S_NTP_NAV_Data.fTrueAltitude_m));
#endif
}

void dp_rwr_ins_gps::Slot_update_INS_Data_To_NAV_Disp(S_DP_ASPJ_EMIT_WITHIN_RANGE *in_sEmiDetWithinRange)
{
    //S_RES_INS_GPS_DATA S_INS_Data;

    if(in_sEmiDetWithinRange == NULL)
    {
        return;
    }

    //memset(&S_INS_Data, 0, sizeof(S_RES_INS_GPS_DATA));

    /*Copy from Global to Local Structure */
    //memcpy(&S_INS_Data.S_NTP_NAV_Data, &pAppHandle->m_EmitterTrackReports_TimeTagged.S_EmitterTrackReport.mEmitter[0].mS_NavData, sizeof(S_RWR_NAV_DATA_ENGG_UNIT));

//    if((S_INS_Data.ucGNSS_ConnectionSts == 0) || (S_INS_Data.ucIs_INS_GPS_Fix_Valid == 0))
//    {
//        ui->obj_lbl_GpsLed->setPixmap(*qpFailState);
//    }
//    else
//    {
//        ui->obj_lbl_GpsLed->setPixmap(*qpPassState);
//    }

    ui->obj_lbl_GpsLed->setPixmap(*qpPassState);

    ui->obj_ln_edt_fLatitude->setText(QString::number(in_sEmiDetWithinRange->m_fdWaypointLat,'f',6));
    ui->obj_ln_edt_fLongitude->setText(QString::number(in_sEmiDetWithinRange->m_fdWaypointLon,'f',6));
    ui->obj_ln_edt_fTrueHeading->setText(QString::number(in_sEmiDetWithinRange->m_fdHeadingAngle,'f',1));
    ui->obj_ln_edt_fRoll->setText(QString::number(0,'f',1));
    ui->obj_ln_edt_fPitch->setText(QString::number(0,'f',1));
    ui->obj_ln_edt_iAltitude->setText(QString::number(0));
}
