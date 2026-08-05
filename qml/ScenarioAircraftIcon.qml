import QtQuick 2.14

/*
  ScenarioAircraftIcon.qml — background/scenario-traffic aircraft glyph.

  Deliberately distinct from AircraftItem.qml (the primary aircraft's MIG29K
  fighter image): a simple vector delta-wing dart in the app's teal accent
  colour, so background traffic can never be mistaken for the primary
  aircraft on the map. Vector-drawn (Canvas), same approach as the toolbar
  icons, so it always renders without needing a bundled image asset.
*/
Item {
    id: root
    width: 26
    height: 26
    property real bearing: 0
    rotation: bearing

    Canvas {
        id: canvas
        anchors.fill: parent
        onPaint: {
            var ctx = getContext("2d")
            var w = width, h = height
            ctx.clearRect(0, 0, w, h)
            ctx.save()
            ctx.translate(w / 2, h / 2)
            ctx.fillStyle = "#19e3b1"
            ctx.strokeStyle = "#0a1622"
            ctx.lineWidth = 1

            // Fuselage (nose points "up" — bearing rotates the whole item)
            ctx.beginPath()
            ctx.moveTo(0, -h * 0.46)
            ctx.lineTo(h * 0.09, h * 0.30)
            ctx.lineTo(0, h * 0.46)
            ctx.lineTo(-h * 0.09, h * 0.30)
            ctx.closePath()
            ctx.fill(); ctx.stroke()

            // Delta wings
            ctx.beginPath()
            ctx.moveTo(0, -h * 0.02)
            ctx.lineTo(w * 0.48, h * 0.32)
            ctx.lineTo(w * 0.14, h * 0.30)
            ctx.lineTo(0, h * 0.08)
            ctx.lineTo(-w * 0.14, h * 0.30)
            ctx.lineTo(-w * 0.48, h * 0.32)
            ctx.closePath()
            ctx.fill(); ctx.stroke()

            // Tail fins
            ctx.beginPath()
            ctx.moveTo(0, h * 0.20)
            ctx.lineTo(w * 0.18, h * 0.46)
            ctx.lineTo(w * 0.05, h * 0.40)
            ctx.lineTo(0, h * 0.28)
            ctx.lineTo(-w * 0.05, h * 0.40)
            ctx.lineTo(-w * 0.18, h * 0.46)
            ctx.closePath()
            ctx.fill(); ctx.stroke()

            ctx.restore()
        }
    }
}
