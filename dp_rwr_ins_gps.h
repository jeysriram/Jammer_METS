#ifndef DP_RWR_INS_GPS_H
#define DP_RWR_INS_GPS_H

#include <QWidget>
#include "dp_aspj_comm.h"

namespace Ui {
class dp_rwr_ins_gps;
}

class dp_rwr_ins_gps : public QWidget
{
    Q_OBJECT

public:
    explicit dp_rwr_ins_gps(QWidget *parent = nullptr);
    ~dp_rwr_ins_gps();

    /**** Sts LED *********/
    QPixmap *qpDefaultState;
    QPixmap *qpPassState;
    QPixmap *qpFailState;

public slots:
    void on_obj_psh_btn_Read_clicked();
    void Slot_update_INS_Data_To_NAV_Disp(S_DP_ASPJ_EMIT_WITHIN_RANGE *in_sEmiDetWithinRange);

private:
    Ui::dp_rwr_ins_gps *ui;
};

#endif // DP_RWR_INS_GPS_H
