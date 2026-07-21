#ifndef DP_RWR_NAV_FLIGHT_DISPLAY_H
#define DP_RWR_NAV_FLIGHT_DISPLAY_H

#include <QWidget>
#include <QResizeEvent>
#include "dp_aspj_comm.h"

namespace Ui {
class dp_rwr_nav_flight_display;
}

class dp_rwr_nav_flight_display : public QWidget
{
    Q_OBJECT

public:
    explicit dp_rwr_nav_flight_display(QWidget *parent = nullptr);
    ~dp_rwr_nav_flight_display();

    int iTest;

public slots:
    void Slot_UpdateNavFlightDisplay(S_DP_ASPJ_EMIT_WITHIN_RANGE *in_sEmiDetWithinRange);

private:
    Ui::dp_rwr_nav_flight_display *ui;

    void resizeEvent( QResizeEvent *event );
};

#endif // DP_RWR_NAV_FLIGHT_DISPLAY_H
