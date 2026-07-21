#include <QApplication>
#include <QIcon>
#include <QStyleFactory>
#include "mainwindow.h"

/**
 * @brief Application entry point
 *
 * Professional Qt Widgets + QML hybrid application:
 * - QApplication (not QGuiApplication) for full widget support
 * - MainWindow loads .ui file with QStackedWidget
 * - QQuickWidget (promoted) loads QML map view
 * - C++ models exposed to QML via QQmlContext
 */
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    /// - Force the Fusion style so the app looks identical on Windows and Linux
    /// - (the native Windows style ignores much of the widget/dock QSS).
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    /// - Application metadata
    app.setApplicationName("METS Dry Run Simulation");
    app.setApplicationVersion("1.0.0");

    /// - Application / window / taskbar icon (EW radar theme)
    app.setWindowIcon(QIcon(":/icons/app_ew.png"));

    /// - Create and show main window
    /// - show() first so Qt establishes real screen geometry before
    ///   maximizing; showMaximized() alone (with no prior show()) computes
    ///   the maximize bounds from the tiny pre-layout geometry on Windows,
    ///   leaving the window "maximized" at the wrong, much smaller size.
    MainWindow window;
    window.show();
    window.showMaximized();

    return app.exec();
}
