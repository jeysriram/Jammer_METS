import QtQuick 2.14
import QtLocation 5.14
import QtPositioning 5.14


Item {
    id: root
    width: canvas.width
    height: canvas.height

    // Properties set from delegate
    property var targetMap: null
    property string emitterName: ""
    property var ringCoordinate: null   // The coordinate where rings are drawn (aircraft/emitter)
    property var ringDistances: [5000, 10000, 15000, 20000, 25000] // meters
    property color ringColor: "#2ecc71"   // green by default, set to "#e74c3c" for emitter-specific rings

    // Aircraft heading in degrees. Retained for callers, but the ring's degree
    // scale and crosshair are drawn north-up (fixed to true N/E/S/W) and do NOT
    // rotate with this heading — see the tick/crosshair drawing below.
    property real heading: 0

    // Radar pulse "ping": an expanding + fading ring drawn on an overlay canvas
    property bool pulseEnabled: true
    property int  pulseDuration: 2500   // ms per pulse cycle
    property real pulseProgress: 0      // 0..1, animated
    onPulseProgressChanged: pulseCanvas.requestPaint()

    NumberAnimation on pulseProgress {
        running: root.pulseEnabled
        loops: Animation.Infinite
        from: 0
        to: 1
        duration: root.pulseDuration
    }

    // Internal: pixel radius for each ring
    property var pixelRadii: []
    property real maxPixelRadius: 0

    // Recalculate based on geographic distance at current zoom
    function recalibrate() {
        if (!targetMap || !targetMap.visible) return;

        var coord = ringCoordinate ? ringCoordinate : targetMap.center;
        var newRadii = [];
        var maxR = 0;
        var centerScreen = targetMap.fromCoordinate(coord);

        for (var i = 0; i < ringDistances.length; i++) {
            var edgeCoord = coord.atDistanceAndAzimuth(ringDistances[i], 0);
            var edgeScreen = targetMap.fromCoordinate(edgeCoord);
            var dx = edgeScreen.x - centerScreen.x;
            var dy = edgeScreen.y - centerScreen.y;
            var r = Math.sqrt(dx * dx + dy * dy);
            newRadii.push(r);
            if (r > maxR) maxR = r;
        }

        pixelRadii = newRadii;
        maxPixelRadius = maxR;

        // Resize canvas to fit all rings + padding for labels
        canvas.width = maxR * 2 + 60;
        canvas.height = maxR * 2 + 60;
        canvas.requestPaint();
    }

    // Connections to map viewport changes
    Connections {
        target: targetMap
        function onZoomLevelChanged() { recalibrate(); }
        function onCenterChanged() { recalibrate(); }
    }

    // Initial calculation after component loads
    Component.onCompleted: recalibrate()

    Canvas {
        id: canvas
        anchors.centerIn: parent
        width: 200
        height: 200

        onPaint: {
            var ctx = getContext("2d");
            var w = width;
            var h = height;
            var cx = w / 2;
            var cy = h / 2;

            ctx.clearRect(0, 0, w, h);

            if (pixelRadii.length === 0 || maxPixelRadius <= 0) return;

            // --- CONCENTRIC RINGS ---
            var ringClr = root.ringColor;
            for (var i = 0; i < pixelRadii.length; i++) {
                var r = pixelRadii[i];
                var distKm = ringDistances[i] / 1000;

                ctx.beginPath();
                ctx.arc(cx, cy, r, 0, 2 * Math.PI);
                ctx.strokeStyle = ringClr;
                ctx.lineWidth = (i === pixelRadii.length - 1) ? 2 : 1;
                ctx.stroke();
            }

            // --- DEGREE TICK MARKS & LABELS on outer ring (north-up / fixed) ---
            // deg is an ABSOLUTE compass bearing (0° = true North, 90° = East,
            // 180° = South, 270° = West) and does NOT rotate with the aircraft.
            // On a north-up map a compass bearing θ maps to the unit vector
            // (sin θ, -cos θ).
            var outerR = pixelRadii[pixelRadii.length - 1];
            if (outerR > 0) {
                for (var deg = 0; deg < 360; deg += 15) {
                    var bearing = deg * Math.PI / 180;
                    var ux = Math.sin(bearing);
                    var uy = -Math.cos(bearing);
                    var isMajor = (deg % 45 === 0);
                    var tickInner = outerR - (isMajor ? 10 : 5);
                    var tickOuter = outerR + (isMajor ? 3 : 2);

                    var x1 = cx + tickInner * ux;
                    var y1 = cy + tickInner * uy;
                    var x2 = cx + tickOuter * ux;
                    var y2 = cy + tickOuter * uy;

                    ctx.strokeStyle = ringClr;
                    ctx.lineWidth = isMajor ? 2 : 1;
                    ctx.beginPath();
                    ctx.moveTo(x1, y1);
                    ctx.lineTo(x2, y2);
                    ctx.stroke();

                    if (isMajor) {
                        var labelR = outerR + 18;
                        var lx = cx + labelR * ux;
                        var ly = cy + labelR * uy;
                        ctx.fillStyle = ringClr;
                        ctx.font = "bold 9px sans-serif";
                        ctx.textAlign = "center";
                        ctx.textBaseline = "middle";
                        ctx.fillText(deg + "°", lx, ly);
                    }
                }
            }

            // --- CROSSHAIR (fixed north-up: vertical axis = N/S, horizontal = E/W;
            //     does NOT rotate with the aircraft heading) ---
            var noseX = 0,  noseY = -1;   // North (0°) direction
            var perpX = 1,  perpY = 0;    // East (90°) direction
            var chL = outerR + 10;
            ctx.strokeStyle = ringClr;
            ctx.lineWidth = 1;
            ctx.beginPath();
            ctx.moveTo(cx - noseX * chL, cy - noseY * chL);
            ctx.lineTo(cx + noseX * chL, cy + noseY * chL);
            ctx.moveTo(cx - perpX * chL, cy - perpY * chL);
            ctx.lineTo(cx + perpX * chL, cy + perpY * chL);
            ctx.stroke();
        }
    }

    // Overlay canvas for the radar pulse "ping" — kept separate so the static
    // rings are not repainted every animation frame.
    Canvas {
        id: pulseCanvas
        anchors.centerIn: parent
        width: canvas.width
        height: canvas.height
        visible: root.pulseEnabled

        onPaint: {
            var ctx = getContext("2d");
            ctx.clearRect(0, 0, width, height);

            if (!root.pulseEnabled || root.maxPixelRadius <= 0) return;

            var cx = width / 2;
            var cy = height / 2;
            var pr = root.pulseProgress * root.maxPixelRadius;

            ctx.beginPath();
            ctx.arc(cx, cy, pr, 0, 2 * Math.PI);
            ctx.strokeStyle = root.ringColor;
            ctx.lineWidth = 2;
            ctx.globalAlpha = (1.0 - root.pulseProgress) * 0.9;
            ctx.stroke();
            ctx.globalAlpha = 1.0;
        }
    }
}
