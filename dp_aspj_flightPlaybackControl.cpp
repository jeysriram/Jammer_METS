/**
 * \file   dp_aspj_flightPlaybackControl.cpp
 * \brief  VLC-style flight transport dock (CDP_FlightPlaybackControl) implementation.
 *
 * A reusable QDockWidget with prev / play-pause / stop / next / loop buttons and
 * a seek slider. It holds no flight logic — it drives and reflects a bound
 * CDP_FlightController. Includes two file-local helpers: a hand-drawn "repeat"
 * icon and a small button click animation.
 *
 * \version 1.00
 *
 * \Copyright (C) 2026 Data Patterns (India) Ltd.
 */
#include "dp_aspj_comm.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QPropertyAnimation>
#include <QPolygonF>
#include <QIcon>
#include <QPixmap>
#include <QPainter>

/*!
 *  \enum   E_TRANSPORT_GLYPH
 *  \brief  The media-transport glyphs that can be hand-drawn into an icon.
 */
enum E_TRANSPORT_GLYPH
{
    E_GLYPH_PREV = 0,   /*!< skip to previous waypoint (bar + two left triangles) */
    E_GLYPH_PLAY,       /*!< start / resume (right-pointing triangle)             */
    E_GLYPH_PAUSE,      /*!< pause (two vertical bars)                            */
    E_GLYPH_STOP,       /*!< stop and reset (square)                              */
    E_GLYPH_NEXT,       /*!< skip to next waypoint (two right triangles + bar)    */
    E_GLYPH_REPEAT      /*!< loop / repeat (rounded loop + two arrowheads)        */
};
/*!
 *  \fn     static QPixmap renderTransportGlyph(E_TRANSPORT_GLYPH glyph, const QColor &color, int size)
 *  \brief  Hand-draws one media-transport glyph into a transparent pixmap with
 *          QPainter (same technique for every button, like the repeat icon), so
 *          all transport icons render identically on every platform/style —
 *          none of them depend on QStyle::standardIcon() or the active theme.
 *
 *  \param [in] glyph : Which transport glyph to draw.
 *  \param [in] color : Fill/stroke colour of the glyph.
 *  \param [in] size  : Pixmap size in pixels.
 *
 *  \author Sriram J
 *  \date   09 July 2026
 *
 *  \retval : The rendered glyph pixmap.
 */
static QPixmap renderTransportGlyph(E_TRANSPORT_GLYPH glyph, const QColor &color, int size = 64)
{
    QPixmap pm(size, size);
    const qreal s = size;
    pm.fill(Qt::transparent);
    QPainter p(&pm);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setPen(Qt::NoPen);
    p.setBrush(color);
    switch (glyph)
    {
    case E_GLYPH_PLAY:
    {
        p.drawPolygon(QPolygonF() << QPointF(s * 0.32, s * 0.20)
                                  << QPointF(s * 0.32, s * 0.80)
                                  << QPointF(s * 0.84, s * 0.50));
        break;
    }
    case E_GLYPH_PAUSE:
    {
        p.drawRoundedRect(QRectF(s * 0.28, s * 0.22, s * 0.14, s * 0.56), s * 0.04, s * 0.04);
        p.drawRoundedRect(QRectF(s * 0.58, s * 0.22, s * 0.14, s * 0.56), s * 0.04, s * 0.04);
        break;
    }
    case E_GLYPH_STOP:
    {
        p.drawRoundedRect(QRectF(s * 0.26, s * 0.26, s * 0.48, s * 0.48), s * 0.08, s * 0.08);
        break;
    }
    case E_GLYPH_PREV:
    {
        p.drawRoundedRect(QRectF(s * 0.18, s * 0.24, s * 0.10, s * 0.52), s * 0.03, s * 0.03);
        p.drawPolygon(QPolygonF() << QPointF(s * 0.58, s * 0.24)
                                  << QPointF(s * 0.58, s * 0.76)
                                  << QPointF(s * 0.32, s * 0.50));
        p.drawPolygon(QPolygonF() << QPointF(s * 0.86, s * 0.24)
                                  << QPointF(s * 0.86, s * 0.76)
                                  << QPointF(s * 0.60, s * 0.50));
        break;
    }
    case E_GLYPH_NEXT:
    {
        p.drawPolygon(QPolygonF() << QPointF(s * 0.14, s * 0.24)
                                  << QPointF(s * 0.14, s * 0.76)
                                  << QPointF(s * 0.40, s * 0.50));
        p.drawPolygon(QPolygonF() << QPointF(s * 0.42, s * 0.24)
                                  << QPointF(s * 0.42, s * 0.76)
                                  << QPointF(s * 0.68, s * 0.50));
        p.drawRoundedRect(QRectF(s * 0.72, s * 0.24, s * 0.10, s * 0.52), s * 0.03, s * 0.03);
        break;
    }
    case E_GLYPH_REPEAT:
    default:
    {
        // stroked (not filled) glyph: rounded loop + two arrowheads
        QPen pen(color, s * 0.09);
        pen.setCapStyle(Qt::RoundCap);
        pen.setJoinStyle(Qt::RoundJoin);
        p.setPen(pen);
        p.setBrush(Qt::NoBrush);
        QRectF loop(s * 0.16, s * 0.28, s * 0.68, s * 0.44);
        qreal rr = loop.height() / 2.0;
        p.drawRoundedRect(loop, rr, rr);
        qreal a = s * 0.13;
        // top edge → right-pointing arrowhead
        QPointF t(loop.left() + loop.width() * 0.60, loop.top());
        p.drawLine(t, t + QPointF(-a, -a));
        p.drawLine(t, t + QPointF(-a,  a));
        // bottom edge → left-pointing arrowhead
        QPointF b(loop.left() + loop.width() * 0.40, loop.bottom());
        p.drawLine(b, b + QPointF(a, -a));
        p.drawLine(b, b + QPointF(a,  a));
        break;
    }
    }
    p.end();
    return pm;
}
/*!
 *  \fn     static QIcon makeTransportIcon(E_TRANSPORT_GLYPH glyph, const QColor &color, int size)
 *  \brief  Wraps one hand-drawn transport glyph into a QIcon.
 *
 *  \param [in] glyph : Which transport glyph to draw.
 *  \param [in] color : Fill/stroke colour of the glyph.
 *  \param [in] size  : Pixmap size in pixels.
 *
 *  \author Sriram J
 *  \date   09 July 2026
 *
 *  \retval : The rendered icon.
 */
static QIcon makeTransportIcon(E_TRANSPORT_GLYPH glyph, const QColor &color, int size = 64)
{
    return QIcon(renderTransportGlyph(glyph, color, size));
}
/*!
 *  \fn     static QString makeButtonStyle(const QColor &base, const QColor &hover,
 *                                         const QColor &pressed, const QColor &border,
 *                                         const QColor &text)
 *  \brief  Composes the pill-style transport-button stylesheet from explicit
 *          colour values. The returned sheet is meant to be set DIRECTLY on a
 *          button: a widget's own stylesheet always outranks any stylesheet
 *          inherited from an ancestor (e.g. the MainWindow theme, Charcoal.qss),
 *          so these colours can never be overridden by a theme.
 *
 *  \param [in] base    : Idle background colour.
 *  \param [in] hover   : Background colour while hovered.
 *  \param [in] pressed : Background colour while pressed.
 *  \param [in] border  : Border colour.
 *  \param [in] text    : Text/icon foreground colour.
 *
 *  \author Sriram J
 *  \date   09 July 2026
 *
 *  \retval : The composed stylesheet string.
 */
static QString makeButtonStyle(const QColor &base, const QColor &hover,
                               const QColor &pressed, const QColor &border,
                               const QColor &text)
{
    return QStringLiteral(
        "QPushButton { background:%1; color:%5; border:1px solid %4;"
        " border-radius:8px; padding:6px 12px; font-weight:bold; }"
        "QPushButton:hover { background:%2; }"
        "QPushButton:pressed { background:%3; }"
        "QPushButton:disabled { background:#2a2e3c; color:#666666; }")
        .arg(base.name(), hover.name(), pressed.name(), border.name(), text.name());
}
/*!
 *  \fn     static void animateButtonPress(QPushButton *b)
 *  \brief  Plays a quick "press pop": briefly shrinks the button icon and springs
 *          it back, giving tactile feedback on click.
 *
 *  \param [in] b : Button to animate.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
static void animateButtonPress(QPushButton *b)
{
    QPropertyAnimation *anim = new QPropertyAnimation(b, "iconSize", b);
    anim->setDuration(170);
    anim->setKeyValueAt(0.0, QSize(22, 22));
    anim->setKeyValueAt(0.4, QSize(14, 14));
    anim->setKeyValueAt(1.0, QSize(22, 22));
    anim->setEasingCurve(QEasingCurve::OutBack);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}
/*!
 *  \fn     CDP_FlightPlaybackControl::CDP_FlightPlaybackControl(QWidget *in_pqParent)
 *  \brief  Constructor — builds the transport bar and starts detached/disabled
 *          until a controller is bound.
 *
 *  \param [in] in_pqParent : Parent widget for ownership.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
CDP_FlightPlaybackControl::CDP_FlightPlaybackControl(QWidget *in_pqParent)
    : QDockWidget(in_pqParent)
{
    /// - Build the transport UI
    buildUi();
    /// - Start with no controller (controls disabled) until setController() is called
    setController(nullptr);
}

/*!
 *  \fn     void CDP_FlightPlaybackControl::buildUi()
 *  \brief  Builds the VLC-style transport bar: styled dock content, the
 *          prev/play/stop/next/loop buttons and the seek slider, laid out with
 *          the progress row above the button row, and wires every action to the
 *          bound controller (and the click animation).
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void CDP_FlightPlaybackControl::buildUi()
{
    setWindowTitle(tr("Flight Control"));
    setObjectName(QStringLiteral("flightControlDock"));
    setAllowedAreas(Qt::RightDockWidgetArea |Qt::BottomDockWidgetArea);

    QWidget *bar = new QWidget(this);
    bar->setObjectName(QStringLiteral("flightControlBar"));

    // No stylesheet (and no background) on the bar itself: the dock body is
    // painted by the application theme's "QDockWidget" rule (e.g. Charcoal.qss
    // background-color:#19232D), so this dock always matches the other docks
    // in whatever theme is active. Each control below carries its OWN
    // stylesheet with explicit colours — a widget's own stylesheet always
    // outranks any inherited theme stylesheet, so the theme can never
    // override the transport colours (the old bar-level "#flightControlBar"
    // scoped sheet lost that fight on some platform/Qt combinations).
    // Guarantee the dock always has a usable size so a dock-area splitter can
    // never collapse it to zero height/width (which would make it "not appear").
    //bar->setMinimumSize(240, 98);
//    bar->setMinimumSize(0, 0);

    this->setMinimumSize(240, 98); // Force a minimum size
    this->setMaximumHeight(150);   // Prevent it from becoming too tall

    QVBoxLayout *layout = new QVBoxLayout(bar);
//    layout->setContentsMargins(12, 8, 12, 8);
//    layout->setSpacing(8);
    m_btnPrev = new QPushButton(bar);
    m_btnPlay = new QPushButton(bar);
    m_btnStop = new QPushButton(bar);
    m_btnNext = new QPushButton(bar);
    m_btnLoop = new QPushButton(bar);

    // Every icon is hand-drawn with QPainter
    // instead of QStyle::standardIcon(), so no platform style or application
    // theme can change how the transport buttons look.
    m_btnPrev->setIcon(makeTransportIcon(E_GLYPH_PREV, QColor("#ffffff")));
    m_btnStop->setIcon(makeTransportIcon(E_GLYPH_STOP, QColor("#ffffff")));
    m_btnNext->setIcon(makeTransportIcon(E_GLYPH_NEXT, QColor("#ffffff")));
    // Loop icon carries both check states: white when off, dark ink when the
    // button is checked (green fill), so the glyph stays readable in both.
    QIcon loopIcon;
    loopIcon.addPixmap(renderTransportGlyph(E_GLYPH_REPEAT, QColor("#ffffff")), QIcon::Normal, QIcon::Off);
    loopIcon.addPixmap(renderTransportGlyph(E_GLYPH_REPEAT, QColor("#08131f")), QIcon::Normal, QIcon::On);
    m_btnLoop->setIcon(loopIcon);
    m_btnLoop->setCheckable(true);                            // toggle: loop until stopped
    m_btnLoop->setObjectName(QStringLiteral("loopButton"));
    // m_btnPlay icon is set by updatePlayButton() based on flight state.
    // Uniform pill-style toolbar; play is the accent-filled focal control.
    m_btnPlay->setObjectName(QStringLiteral("playButton"));
    for (QPushButton *b : { m_btnPrev, m_btnPlay, m_btnStop, m_btnNext, m_btnLoop }) {
        b->setFixedSize(30, 26);            // all buttons the same size
        b->setIconSize(QSize(22, 22));
        b->setCursor(Qt::PointingHandCursor);
        b->setFlat(true);
    }
    // Explicit colours, composed at runtime and set on each button directly
    // (own stylesheet == theme-proof, see comment above).
    const QString qsBtnStyle = makeButtonStyle(
        QColor("#3a3f52"), QColor("#4a5068"), QColor("#5a6280"),
        QColor("#4a5068"), QColor("#ffffff"));
    m_btnPrev->setStyleSheet(qsBtnStyle);
    m_btnStop->setStyleSheet(qsBtnStyle);
    m_btnNext->setStyleSheet(qsBtnStyle);
    // accent-filled play/pause button
    m_btnPlay->setStyleSheet(makeButtonStyle(
        QColor("#3fa7ff"), QColor("#5cb6ff"), QColor("#2f8fe0"),
        QColor("#3fa7ff"), QColor("#ffffff")));
    // loop button: base style + turns green while active
    m_btnLoop->setStyleSheet(qsBtnStyle + QStringLiteral(
        "QPushButton:checked { background:#19e3b1; border:1px solid #19e3b1; color:#08131f; }"
        "QPushButton:checked:hover { background:#3ff0c4; }"));
    m_btnPrev->setToolTip(tr("Previous waypoint"));
    m_btnPlay->setToolTip(tr("Play / Pause flight"));
    m_btnStop->setToolTip(tr("Stop and reset to start"));
    m_btnNext->setToolTip(tr("Next waypoint"));
    m_btnLoop->setToolTip(tr("On Repeat"));
    m_progressSlider = new QSlider(Qt::Horizontal, bar);
    m_progressSlider->setRange(0, 1000); // 0.1% resolution over 0..100%
    m_progressSlider->setToolTip(tr("Seek along flight path"));
    // Own stylesheet, same theme-proof reasoning as the buttons.
    m_progressSlider->setStyleSheet(QStringLiteral(
        "QSlider::groove:horizontal { height:4px; background:#4a5068; border-radius:2px; }"
        "QSlider::sub-page:horizontal {"
        " background:qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #3fa7ff, stop:1 #6ad0ff);"
        " border-radius:3px; }"
        "QSlider::handle:horizontal { width:12px; margin:-5px 0; background:#ffffff;"
        " border:2px solid #3fa7ff; border-radius:6px; }"
        "QSlider::handle:horizontal:hover { background:#eaf6ff; }"));

    m_progressLabel = new QLabel(QStringLiteral("0%"), bar);
    m_progressLabel->setMinimumWidth(40);
    m_progressLabel->setAlignment(Qt::AlignCenter);
    // Font only — the text colour comes from the theme so the label stays
    // readable on light and dark dock backgrounds alike.
    m_progressLabel->setStyleSheet(QStringLiteral(
        "QLabel { font:600 11px 'monospace'; }"));

    // Row 1: progress (kept ABOVE the buttons)
    QHBoxLayout *progRow = new QHBoxLayout();
    progRow->setSpacing(8);
    progRow->addWidget(m_progressSlider, 1);
    progRow->addWidget(m_progressLabel);
    // Row 2: transport buttons, centered
    QHBoxLayout *btnRow = new QHBoxLayout();
    btnRow->setSpacing(8);
    btnRow->addStretch();
    btnRow->addWidget(m_btnPrev);
    btnRow->addWidget(m_btnPlay);
    btnRow->addWidget(m_btnStop);
    btnRow->addWidget(m_btnNext);
    btnRow->addWidget(m_btnLoop);
    btnRow->addStretch();

    layout->addLayout(progRow);
    layout->addLayout(btnRow);

    setWidget(bar);

    // Button actions reference m_controller, so they always target the
    // currently bound controller without needing to be reconnected.
    connect(m_btnPlay, &QPushButton::clicked, this, [this]() {
        if (!m_controller) return;
        if (m_controller->isFlying())
            m_controller->stopFlight();   // pause, keep position
        else
            m_controller->startFlight();  // resume / start
    });
    connect(m_btnStop, &QPushButton::clicked, this, [this]() {
        if (!m_controller) return;
        m_btnLoop->setChecked(false);     // Stop cancels looping (also calls setLoop(false))
        m_controller->resetFlight();
    });
    connect(m_btnPrev, &QPushButton::clicked, this, [this]() {
        if (m_controller) m_controller->prevWaypoint();
    });
    connect(m_btnNext, &QPushButton::clicked, this, [this]() {
        if (m_controller) m_controller->nextWaypoint();
    });

    // Loop toggle: enable looping and (re)start the flight; it keeps running
    // lap after lap until Stop is pressed.
    connect(m_btnLoop, &QPushButton::toggled, this, [this](bool on) {
        if (!m_controller) return;
        m_controller->setLoop(on);
        if (on && !m_controller->isFlying())
            m_controller->startFlight();
    });

    // sliderMoved fires only on user interaction, so programmatic value
    // updates in onProgressChanged() never feed back into a seek.
    connect(m_progressSlider, &QSlider::sliderMoved, this, [this](int value) {
        if (m_controller) m_controller->seekToPercent(value / 10.0);
    });
    // Small "press pop" animation on every button click.
    for (QPushButton *b : { m_btnPrev, m_btnPlay, m_btnStop, m_btnNext, m_btnLoop }) {
        connect(b, &QPushButton::clicked, this, [b]() { animateButtonPress(b); });
    }
}

/*!
 *  \fn     void CDP_FlightPlaybackControl::setController(CDP_FlightController *in_pcController)
 *  \brief  Binds (or rebinds) the transport bar to a flight controller:
 *          disconnects the previous one, connects the new one's state signals,
 *          enables/disables the controls and refreshes the play button + slider.
 *
 *  \param [in] in_pcController : Controller to bind, or nullptr to detach.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void CDP_FlightPlaybackControl::setController(CDP_FlightController *in_pcController)
{
    /// - Nothing to do if the same controller is already bound
    if (m_controller == in_pcController)
        return;

    /// - Drop all connections to the previously bound controller
    if (m_controller)
        disconnect(m_controller, nullptr, this, nullptr);

    /// - Store the new controller
    m_controller = in_pcController;

    /// - Reflect the controller's state changes in the UI
    if (m_controller) {
        connect(m_controller, &CDP_FlightController::isFlyingChanged,
                this, &CDP_FlightPlaybackControl::updatePlayButton);
        connect(m_controller, &CDP_FlightController::progressPercentChanged,
                this, &CDP_FlightPlaybackControl::onProgressChanged);
    }

    /// - Enable the controls only when a controller is bound
    const bool enabled = (m_controller != nullptr);
    m_btnPrev->setEnabled(enabled);
    m_btnPlay->setEnabled(enabled);
    m_btnStop->setEnabled(enabled);
    m_btnNext->setEnabled(enabled);
    m_progressSlider->setEnabled(enabled);

    /// - Sync the play button icon and progress display with the new state
    updatePlayButton();
    onProgressChanged();
}

/*!
 *  \fn     void CDP_FlightPlaybackControl::updatePlayButton()
 *  \brief  Syncs the play/pause button icon with the controller's flying state.
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void CDP_FlightPlaybackControl::updatePlayButton()
{
    /// - Show the pause icon while flying, the play icon otherwise
    ///   (hand-drawn, same method as every other transport icon)
    const bool flying = m_controller && m_controller->isFlying();
    m_btnPlay->setIcon(makeTransportIcon(
        flying ? E_GLYPH_PAUSE : E_GLYPH_PLAY, QColor("#ffffff")));
}

/*!
 *  \fn     void CDP_FlightPlaybackControl::onProgressChanged()
 *  \brief  Reflects the controller's flight progress on the seek slider and the
 *          percentage label (without fighting a user drag).
 *
 *  \author Sriram J
 *  \date   03 July 2026
 *
 *  \retval : NA
 */
void CDP_FlightPlaybackControl::onProgressChanged()
{
    /// - Current progress percentage (0 when no controller is bound)
    const double pct = m_controller ? m_controller->progressPercent() : 0.0;

    /// - Update the slider only when the user is not dragging the handle
    if (!m_progressSlider->isSliderDown()) {
        m_progressSlider->blockSignals(true);                 // avoid feedback into seek
        m_progressSlider->setValue(static_cast<int>(pct * 10.0));
        m_progressSlider->blockSignals(false);
    }
    /// - Update the percentage label
    m_progressLabel->setText(QStringLiteral("%1%").arg((int)pct));
}
