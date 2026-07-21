#ifndef FLIGHTCONTROLDOCK_H
#define FLIGHTCONTROLDOCK_H

#include <QDockWidget>

class CDP_FlightController;
class QPushButton;
class QSlider;
class QLabel;

/**
 * @brief CDP_FlightPlaybackControl — reusable VLC-style flight transport bar.
 *
 * A self-contained QDockWidget module: prev / play-pause / stop / next
 * buttons plus a seekable progress slider, all built from Qt Widgets.
 * Drop it into any QMainWindow and bind it to a FlightController:
 *
 *     auto *dock = new CDP_FlightPlaybackControl(this);
 *     addDockWidget(Qt::BottomDockWidgetArea, dock);
 *     dock->setController(myFlightController);
 *
 * The dock holds no flight logic — it only drives a FlightController and
 * reflects its state, so it can be reused in a different application as-is.
 */
class CDP_FlightPlaybackControl : public QDockWidget
{
    Q_OBJECT
public:
    explicit CDP_FlightPlaybackControl(QWidget *in_pqParent = nullptr);

    // Bind (or rebind) the transport controls to a controller.
    // Pass nullptr to detach and disable the controls.
    void setController(CDP_FlightController *in_pcController);
    CDP_FlightController *controller() const { return m_controller; }

private slots:
    void updatePlayButton();   // sync play/pause icon with flight state
    void onProgressChanged();  // sync slider + label with flight progress

private:
    void buildUi();

    CDP_FlightController *m_controller = nullptr;

    QPushButton *m_btnPrev = nullptr;
    QPushButton *m_btnPlay = nullptr;
    QPushButton *m_btnStop = nullptr;
    QPushButton *m_btnNext = nullptr;
    QPushButton *m_btnLoop = nullptr;
    QSlider     *m_progressSlider = nullptr;
    QLabel      *m_progressLabel = nullptr;
};

#endif // FLIGHTCONTROLDOCK_H
