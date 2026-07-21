import QtQuick 2.14
import QtQuick.Controls 2.14

/*
  Vertical Map Toolbar — right side of map panel.
  Tactical / EW-system styling: dark glass panel, teal accent, hover-zoom icons.
  Icons are vector-drawn (Canvas) so they always render and take the accent colour.
*/

Rectangle {
    id: root
    width: 50
    height: col.height + 22
    radius: 11

    gradient: Gradient {
        GradientStop { position: 0.0; color: "#0d1a2b" }
        GradientStop { position: 1.0; color: "#0a1320" }
    }
    border.color: "#1d3a55"
    border.width: 1

    // Signals for each button
    signal homeClicked()               // reset the map back to the default (fitted) view
    signal mapLayersClicked()
    signal emitterLocationClicked()
    signal lobHistoryClicked()
    signal autoAlignClicked()
    signal measureDistanceClicked()
    signal rangeRingsClicked()
    signal emitterRangeRingsClicked()
    signal autoBearingClicked()
    signal startFlightClicked()
    signal stopFlightClicked()
    signal resetFlightClicked()

    // Active states
    property bool emittersVisible: true
    property bool rangeRingsVisible: true
    property bool lobHistoryVisible: false
    property bool autoAlignActive: false
    property bool measureModeActive: false
    property bool autoBearingActive: false
    property bool startFlight: false

    // Accent palette
    readonly property color accent:   "#19e3b1"
    readonly property color iconIdle: "#cfe3ff"

    // Button definitions (sep:true → separator line)
    property var buttons: [
        { type: "home",    tip: "Home — Reset to Default View" },
        { type: "layers",  tip: "Map Layers (Satellite / Normal)" },
        { type: "emitter", tip: "Emitter Locations" },
        { type: "lob",     tip: "LOB History" },
        { type: "align",   tip: "Auto Align" },
        { type: "measure", tip: "Measure Distance" },
        { type: "rings",   tip: "Range Rings" },
        { type: "bearing", tip: "Auto Bearing" },
        // Transport controls hidden from the map toolbar for now (kept in code).
        { sep: true,       hidden: true },
        { type: "start",   tip: "Start Flight", hidden: true },
        { type: "stop",    tip: "Stop Flight",  hidden: true },
        { type: "reset",   tip: "Reset Flight", hidden: true }
    ]

    function isChecked(t) {
        switch (t) {
        case "emitter": return emittersVisible
        case "lob":     return lobHistoryVisible
        case "align":   return autoAlignActive
        case "measure": return measureModeActive
        case "rings":   return rangeRingsVisible
        case "bearing": return autoBearingActive
        }
        return false
    }

    function handle(t) {
        // Emit only — the active states are owned by MapView and bound back in,
        // so the checked highlight always matches the real layer visibility.
        switch (t) {
        case "home":    root.homeClicked(); break
        case "layers":  root.mapLayersClicked(); break
        case "emitter": root.emitterLocationClicked(); break
        case "lob":     root.lobHistoryClicked(); break
        case "align":   root.autoAlignClicked(); break
        case "measure": root.measureDistanceClicked(); break
        case "rings":   root.rangeRingsClicked(); break
        case "bearing": root.autoBearingClicked(); break
        case "start":   root.startFlightClicked(); break
        case "stop":    root.stopFlightClicked(); break
        case "reset":   root.resetFlightClicked(); break
        }
    }

    // Vector icon renderer — draws the EW symbol for `type` into a 2D context.
    function drawIcon(ctx, w, h, col, type) {
        ctx.clearRect(0, 0, w, h)
        ctx.save()
        ctx.strokeStyle = col
        ctx.fillStyle = col
        ctx.lineWidth = 2
        ctx.lineCap = "round"
        ctx.lineJoin = "round"

        var cx = w / 2, cy = h / 2
        var u = Math.min(w, h)
        var s = u * 0.30
        var a, i

        switch (type) {
        case "home":     // house — reset map to the default (fitted) view
            // roof
            ctx.beginPath()
            ctx.moveTo(cx - s, cy - s * 0.05)
            ctx.lineTo(cx, cy - s * 0.85)
            ctx.lineTo(cx + s, cy - s * 0.05)
            ctx.stroke()
            // walls
            ctx.beginPath()
            ctx.moveTo(cx - s * 0.72, cy - s * 0.05)
            ctx.lineTo(cx - s * 0.72, cy + s * 0.8)
            ctx.lineTo(cx + s * 0.72, cy + s * 0.8)
            ctx.lineTo(cx + s * 0.72, cy - s * 0.05)
            ctx.stroke()
            // door
            ctx.beginPath()
            ctx.moveTo(cx - s * 0.22, cy + s * 0.8)
            ctx.lineTo(cx - s * 0.22, cy + s * 0.28)
            ctx.lineTo(cx + s * 0.22, cy + s * 0.28)
            ctx.lineTo(cx + s * 0.22, cy + s * 0.8)
            ctx.stroke()
            break

        case "layers":   // stacked map layers
            ctx.beginPath()
            ctx.moveTo(cx, cy - s)
            ctx.lineTo(cx + s, cy - s * 0.2)
            ctx.lineTo(cx, cy + s * 0.6)
            ctx.lineTo(cx - s, cy - s * 0.2)
            ctx.closePath(); ctx.stroke()
            ctx.beginPath()
            ctx.moveTo(cx - s, cy + s * 0.25)
            ctx.lineTo(cx, cy + s)
            ctx.lineTo(cx + s, cy + s * 0.25)
            ctx.stroke()
            break

        case "emitter":  // radar emitter (mast + waves)
            var ny = cy - s * 0.2
            ctx.beginPath(); ctx.moveTo(cx, cy + s); ctx.lineTo(cx, ny); ctx.stroke()
            ctx.beginPath(); ctx.moveTo(cx - s * 0.5, cy + s); ctx.lineTo(cx + s * 0.5, cy + s); ctx.stroke()
            ctx.beginPath(); ctx.arc(cx, ny, s * 0.55, -Math.PI * 0.82, -Math.PI * 0.18); ctx.stroke()
            ctx.beginPath(); ctx.arc(cx, ny, s * 0.9,  -Math.PI * 0.82, -Math.PI * 0.18); ctx.stroke()
            ctx.beginPath(); ctx.arc(cx, ny, 1.7, 0, 2 * Math.PI); ctx.fill()
            break

        case "lob":      // lines of bearing (fan)
            var ox = cx, oy = cy + s
            var deg = [-50, -75, -105, -130]
            for (i = 0; i < deg.length; i++) {
                a = deg[i] * Math.PI / 180
                ctx.beginPath(); ctx.moveTo(ox, oy)
                ctx.lineTo(ox + Math.cos(a) * 1.9 * s, oy + Math.sin(a) * 1.9 * s)
                ctx.stroke()
            }
            ctx.beginPath(); ctx.arc(ox, oy, 1.7, 0, 2 * Math.PI); ctx.fill()
            break

        case "align":    // alignment reticle
            ctx.beginPath(); ctx.arc(cx, cy, s * 0.75, 0, 2 * Math.PI); ctx.stroke()
            ctx.beginPath(); ctx.moveTo(cx, cy - s);       ctx.lineTo(cx, cy - s * 0.4); ctx.stroke()
            ctx.beginPath(); ctx.moveTo(cx, cy + s * 0.4); ctx.lineTo(cx, cy + s);       ctx.stroke()
            ctx.beginPath(); ctx.moveTo(cx - s, cy);       ctx.lineTo(cx - s * 0.4, cy); ctx.stroke()
            ctx.beginPath(); ctx.moveTo(cx + s * 0.4, cy); ctx.lineTo(cx + s, cy);       ctx.stroke()
            ctx.beginPath(); ctx.arc(cx, cy, 1.5, 0, 2 * Math.PI); ctx.fill()
            break

        case "measure":  // ruler
            var x1 = cx - s, y1 = cy + s, x2 = cx + s, y2 = cy - s
            ctx.beginPath(); ctx.moveTo(x1, y1); ctx.lineTo(x2, y2); ctx.stroke()
            ctx.beginPath(); ctx.arc(x1, y1, 1.9, 0, 2 * Math.PI); ctx.fill()
            ctx.beginPath(); ctx.arc(x2, y2, 1.9, 0, 2 * Math.PI); ctx.fill()
            var dx = x2 - x1, dy = y2 - y1, len = Math.sqrt(dx * dx + dy * dy)
            var ux = dy / len, uy = -dx / len
            for (i = 1; i <= 3; i++) {
                var px = x1 + dx * i / 4, py = y1 + dy * i / 4
                ctx.beginPath(); ctx.moveTo(px, py); ctx.lineTo(px + ux * 3.5, py + uy * 3.5); ctx.stroke()
            }
            break

        case "rings":    // range rings
            ctx.beginPath(); ctx.arc(cx, cy, s * 0.45, 0, 2 * Math.PI); ctx.stroke()
            ctx.beginPath(); ctx.arc(cx, cy, s * 0.85, 0, 2 * Math.PI); ctx.stroke()
            ctx.beginPath(); ctx.arc(cx, cy, 1.4, 0, 2 * Math.PI); ctx.fill()
            break

        case "bearing":  // compass
            ctx.beginPath(); ctx.arc(cx, cy, s * 0.85, 0, 2 * Math.PI); ctx.stroke()
            ctx.beginPath()
            ctx.moveTo(cx, cy - s * 0.6)
            ctx.lineTo(cx + s * 0.28, cy)
            ctx.lineTo(cx, cy + s * 0.6)
            ctx.lineTo(cx - s * 0.28, cy)
            ctx.closePath(); ctx.stroke()
            ctx.beginPath(); ctx.arc(cx, cy - s * 0.6, 1.4, 0, 2 * Math.PI); ctx.fill()
            break

        case "start":    // play
            ctx.beginPath()
            ctx.moveTo(cx - s * 0.5, cy - s * 0.75)
            ctx.lineTo(cx + s * 0.8, cy)
            ctx.lineTo(cx - s * 0.5, cy + s * 0.75)
            ctx.closePath(); ctx.fill()
            break

        case "stop":     // stop
            var b = s * 0.62
            ctx.fillRect(cx - b, cy - b, b * 2, b * 2)
            break

        case "reset":    // circular arrow
            var r = s * 0.72
            ctx.beginPath(); ctx.arc(cx, cy, r, -Math.PI * 0.35, Math.PI * 1.25); ctx.stroke()
            a = -Math.PI * 0.35
            var ex = cx + Math.cos(a) * r, ey = cy + Math.sin(a) * r
            ctx.beginPath()
            ctx.moveTo(ex, ey); ctx.lineTo(ex - 4.5, ey - 2.5)
            ctx.moveTo(ex, ey); ctx.lineTo(ex + 1.5, ey - 4.8)
            ctx.stroke()
            break

        default:
            ctx.beginPath(); ctx.arc(cx, cy, s * 0.7, 0, 2 * Math.PI); ctx.stroke()
        }

        ctx.restore()
    }

    Column {
        id: col
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 11
        spacing: 5

        // --- Header: "system active" pulsing indicator ---
        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 4
            bottomPadding: 4

            Rectangle {
                width: 7; height: 7; radius: 3.5
                anchors.verticalCenter: parent.verticalCenter
                color: root.accent
                SequentialAnimation on opacity {
                    loops: Animation.Infinite
                    NumberAnimation { from: 1.0; to: 0.25; duration: 800 }
                    NumberAnimation { from: 0.25; to: 1.0; duration: 800 }
                }
            }
            Text {
                text: "EW"
                color: root.accent
                font.family: "monospace"
                font.pixelSize: 11
                font.bold: true
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        Repeater {
            model: root.buttons

            delegate: Item {
                width: 40
                visible: !modelData.hidden
                height: modelData.hidden ? 0 : (modelData.sep ? 11 : 40)
                anchors.horizontalCenter: parent.horizontalCenter

                // Separator
                Rectangle {
                    visible: modelData.sep === true
                    anchors.centerIn: parent
                    width: 28
                    height: 1
                    gradient: Gradient {
                        orientation: Gradient.Horizontal
                        GradientStop { position: 0.0; color: "transparent" }
                        GradientStop { position: 0.5; color: "#2b5e7a" }
                        GradientStop { position: 1.0; color: "transparent" }
                    }
                }

                // Tool button
                ToolButton {
                    id: tb
                    visible: !modelData.sep
                    anchors.centerIn: parent
                    width: 38
                    height: 38

                    checked: root.isChecked(modelData.type)
                    onClicked: root.handle(modelData.type)

                    ToolTip.visible: hovered
                    ToolTip.text: modelData.tip ? modelData.tip : ""

                    // --- zoom-on-hover ---
                    scale: hovered ? 1.32 : 1.0
                    z: hovered ? 10 : 0
                    Behavior on scale { NumberAnimation { duration: 160; easing.type: Easing.OutBack } }

                    contentItem: Canvas {
                        id: iconCanvas
                        onPaint: root.drawIcon(getContext("2d"), width, height,
                                               tb.checked ? root.accent : root.iconIdle,
                                               modelData.type)
                    }
                    onCheckedChanged: iconCanvas.requestPaint()

                    background: Rectangle {
                        radius: 9
                        color: tb.checked ? "#0f3a3c"
                                          : (tb.down ? "#1a2c46"
                                                     : (tb.hovered ? "#16273f" : "#0e1a2b"))
                        border.color: tb.checked ? root.accent
                                                 : (tb.hovered ? "#2b6e8f" : "#1a2c44")
                        border.width: 1

                        // soft accent glow when active/hovered
                        Rectangle {
                            anchors.fill: parent
                            anchors.margins: -2
                            radius: 11
                            color: "transparent"
                            border.width: 2
                            border.color: root.accent
                            opacity: (tb.checked || tb.hovered) ? 0.25 : 0.0
                            Behavior on opacity { NumberAnimation { duration: 150 } }
                        }
                    }
                }
            }
        }
    }
}
