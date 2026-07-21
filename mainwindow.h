#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dp_aspj_comm.h"
#include "dp_aspj_flightPlaybackControl.h"


namespace Ui {
class MainWindow;
}

/**
 * @brief Main application window
 *
 * Professional Qt Widgets + QML hybrid architecture:
 * - MainWindow.ui contains the base layout with QStackedWidget
 * - MapWidget is a promoted QQuickWidget that loads QML
 * - Data models (CDP_EmitterModel, CDP_FlightController) live in C++
 * - Exposed to QML via QQmlContext for live binding
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *in_pqParent = nullptr);
    ~MainWindow();

    QLabel          *cm_pobj_qStatusTimer_label ;     /*!< Object for  an label  */
    class QToolButton *cm_pobj_btnTogglePanels = nullptr; /*!< Floating left-edge toggle button for the hidden docks */

private slots:
    /// @brief Setup QML context properties (models exposed to QML)
    void setupQmlContext();

    /// @brief Connect signals between C++ models and UI
    void SignalSlots_Function();

    /// @brief Update the action log text box
    void PrintLog(QString in_qsMessage, short in_sStatus);

    /// @brief Update emitter details table for current flight segment
    void Slot_updateEmitterDetailsToTable(S_DP_ASPJ_EMIT_WITHIN_RANGE *in_sEmiDetWithinRange, int in_iCurrentSegment, int in_iCurrEmitterCnt);

    /// @brief Rebuild emitter details table from full detection record
    void Slot_updateEmitterTable(S_DP_ASPJ_EMIT_WITHIN_RANGE *in_sEmiDetWithinRange, int in_iCurrentSegment);

    void on_actionCharcoal_triggered();

    void on_actionLoad_Settings_triggered();

    void on_action_Default_Theme_triggered();

    void on_actionDefault_triggered();

protected:
    void closeEvent(QCloseEvent *event);

     bool eventFilter(QObject *in_pqObject, QEvent *in_pqEvent) override;

     void showContextMenu(const QPoint &pos);

     void resizeEvent(QResizeEvent *event) override;


public:
    void Update_GUI();

    void Init_Globalvariable();

    void On_Init_StatusBar();

    void Remove_Unused_Controls();

    /// @brief Adds a single floating checkable icon button, vertically
    ///        centred on the left edge (no toolbar strip reserved), that
    ///        shows/hides the remaining docks (Action Log, Flight Playback
    ///        Control) in the left dock area.
    void SetupLeftTogglePanel();

    QByteArray cm_qbarrState;
    QByteArray cm_qbarrGeometry;
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
