# ============================================================
# JammerMapProject.pro — Professional Qt Widgets + QML Hybrid
# ============================================================
# Architecture:
#   - Qt Widgets: MainWindow.ui, QStackedWidget, QTableWidget
#   - QML: MapView, RadarDisplay, CockpitDisplay (via QQuickWidget)
#   - C++ Models: EmitterModel, CDP_FlightController (exposed to QML)
# ============================================================

#QT += core gui widgets qml quick quickwidgets location positioning svg

# --- Qt Modules ---
QT += core gui printsupport serialport sql xml svg location qml quick \
      positioning location-private widgets multimedia quickwidgets


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11


# Enable High DPI scaling
DEFINES += QT_DEPRECATED_WARNINGS

# ============================================================
# SOURCES
# ============================================================
SOURCES += \
    DP_EmitterRelParam.cpp \
    dp_aspj_emitter_component.cpp \
    dp_aspj_flightPlaybackControl.cpp \
    dp_aspj_flight_component.cpp \
    dp_aspj_loadsettings.cpp \
    dp_aspj_map.cpp \
    dp_rwr_flight_display.cpp \
    dp_rwr_flight_display_draw_functions.cpp \
    dp_rwr_ins_gps.cpp \
    dp_rwr_nav_flight_display.cpp \
    dp_rwr_polarplot_power_vs_doa.cpp \
    dp_spline_interpolation.cpp \
    dp_symbol_draw_polar_plot.cpp \
    main.cpp \
    mainwindow.cpp

# ============================================================
# HEADERS
# ============================================================
HEADERS += \
    DP_EmitterRelParam.h \
    DP_LocationFixing_Pro.h \
    DP_Types.h \
    dp_aspj_comm.h \
    dp_aspj_emitter_component.h \
    dp_aspj_flightPlaybackControl.h \
    dp_aspj_flight_component.h \
    dp_aspj_loadsettings.h \
    dp_aspj_macros.h \
    dp_aspj_map.h \
    dp_aspj_structures.h \
    dp_rwr_flight_display.h \
    dp_rwr_ins_gps.h \
    dp_rwr_nav_flight_display.h \
    dp_rwr_polarplot_power_vs_doa.h \
    dp_spline_interpolation.h \
    dp_symbol_draw_polar_plot.h \
    mainwindow.h

# ============================================================
# UI FORMS (compiled by uic)
# ============================================================
FORMS += \
    cloadsettings.ui \
    dp_aspj_map.ui \
    dp_rwr_flight_display.ui \
    dp_rwr_ins_gps.ui \
    dp_rwr_nav_flight_display.ui \
    dp_rwr_polarplot_power_vs_doa.ui \
    mainwindow.ui

# ============================================================
# RESOURCES (QML + icons compiled by rcc)
# ============================================================
RESOURCES += \
    resources.qrc

# ============================================================
# RUNTIME RESOURCES
# Copy the QSS theme files next to the built executable (OUT_PWD) so
# MainWindow's theme loader (resolved against applicationDirPath()) finds
# them regardless of the current working directory the app is launched
# from, and without depending on the source tree at runtime.
# ============================================================
CONFIG += file_copies
COPIES += res_styles
res_styles.files = $$files(Res/Style/*.qss)
# DESTDIR itself isn't resolved to its final debug/release value until after
# this file is processed (exclusive_builds.prf runs in default_post.prf), so
# the debug/release subfolder is derived the same way here explicitly.
CONFIG(debug, debug|release): res_styles.path = $$OUT_PWD/debug/Res/Style
else: res_styles.path = $$OUT_PWD/release/Res/Style

# win32 OpenSSL runtime (32-bit, matches the MinGW 32-bit build). Qt does not
# bundle OpenSSL; QSslSocket loads these two DLLs dynamically at runtime, and
# without them every HTTPS request (e.g. the online map's tile.openstreetmap.org
# fetches) silently fails TLS initialization. They must sit next to the exe
# itself (not a subfolder) for the default Windows DLL search to find them.
win32 {
    COPIES += openssl_dlls
    openssl_dlls.files = $$files(OpenSSL/*.dll)
    CONFIG(debug, debug|release): openssl_dlls.path = $$OUT_PWD/debug
    else: openssl_dlls.path = $$OUT_PWD/release
}

# ============================================================
# PERMANENT TIMESTAMP FIX
# Prevents 'modification time in future' build loop
# ============================================================
system(find $$PWD -type f -exec touch {} +)

# ============================================================
# DEPLOYMENT
# ============================================================
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


# LFLIB/DF_LibGeneration ships only as a Windows DLL + MinGW import library.
# On Windows, link it. On other platforms, build a native implementation of
# the geodesy routines the app uses (see DP_LocationFixing_Pro_posix.cpp).
win32: LIBS += -L$$PWD/LFLIB/ -lDF_LibGeneration
else: SOURCES +=

INCLUDEPATH += $$PWD/LFLIB
DEPENDPATH += $$PWD/LFLIB

# NAV_Display QFI static library
INCLUDEPATH += $$PWD/NAV_Display_LIB/headers
DEPENDPATH += $$PWD/NAV_Display_LIB
win32: LIBS += -L$$PWD/NAV_Display_LIB/ -lNAV_Display

# Application (exe) icon — EW radar theme
win32:  RC_ICONS = icons/app_ew.ico
macx:   ICON     = icons/app_ew.icns

DISTFILES += \
    Res/Style/BlueBlack.qss \
    Res/Style/BlueBlackRadGrad.qss \
    Res/Style/BlueCerelian.qss \
    Res/Style/Bluerr.qss \
    Res/Style/CaveBlue.qss \
    Res/Style/CharGradient.qss \
    Res/Style/Charcoal.qss \
    Res/Style/ClassicVC-Modern.qss \
    Res/Style/ClassicVC.qss \
    Res/Style/CyanNeon - BG.qss \
    Res/Style/CyanNeon-BG.qss \
    Res/Style/CyanNeon.qss \
    Res/Style/DarkBrown.qss \
    Res/Style/Default.qss \
    Res/Style/FarmerGreen.qss \
    Res/Style/Old-fashion.qss \
    Res/Style/Sky.qss \
    Res/Style/SkyNeon.qss \
    Res/Style/SunRise.qss \
    Res/Style/Test.qss \
    Res/Style/UserLogin.qss \
    Res/Style/WaterBlue.qss \
    Res/Style/Windows - BG.qss \
    Res/Style/Windows-Blue - BG.qss \
    Res/Style/Windows-Blue.qss \
    Res/Style/Windows-Green - BG.qss \
    Res/Style/Windows-Green.qss \
    Res/Style/Windows-Purple - BG.qss \
    Res/Style/Windows-Purple.qss \
    Res/Style/Windows-Qt.qss \
    Res/Style/Windows.qss \
    Res/Style/styles.qss
