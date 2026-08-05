import QtQuick 2.14
import QtLocation 5.14

/*
  ScenarioFlightsLayer.qml — background/scenario air-traffic layer.

  Root type is MapItemView itself, so instantiating this component as a
  child of a QtLocation Map works exactly like an inline MapItemView block.

  Renders one aircraft icon per row of the C++ CDP_ScenarioFlightModel (bound
  as the "scenarioFlightModel" context property). The model only exposes each
  flight's live coordinate/heading — never its waypoints or path — so there
  is nothing to draw here besides the moving icon.

  Proximity marking: same idea as the emitter "detected" treatment in
  MapView.qml (pulsing ring once the primary aircraft is within range) —
  here it pulses an amber ring + brightens the icon whenever the PRIMARY
  aircraft comes within proximityRangeKm of this scenario flight, computed
  as a plain great-circle distance (QGeoCoordinate.distanceTo), the same way
  the emitter range-ring/LOB layers do it.
*/
MapItemView {
    id: root
    property real proximityRangeKm: 20

    model: scenarioFlightModel
    delegate: MapQuickItem {
        id: scenarioDelegate
        coordinate: model.coordinate
        anchorPoint.x: scenarioGroup.width / 2
        anchorPoint.y: scenarioGroup.height / 2

        // Straight-line distance from the primary aircraft to this scenario
        // flight; -1 (never in range) until the primary aircraft has a fix.
        property real distToAircraft: (flightController && flightController.aircraftPosition && flightController.aircraftPosition.isValid)
                                       ? model.coordinate.distanceTo(flightController.aircraftPosition) : -1
        property bool inRange: distToAircraft >= 0 && distToAircraft <= (root.proximityRangeKm * 1000)

        sourceItem: Item {
            id: scenarioGroup
            width: scenarioIcon.width * 1.9
            height: scenarioIcon.height * 1.9

            // Proximity-alert ring — pulses amber while the primary aircraft
            // is within range (mirrors the emitter detection ring, LAYER 5
            // in MapView.qml, but for aircraft-to-aircraft proximity).
            Rectangle {
                id: proximityRing
                anchors.centerIn: parent
                width: parent.width
                height: width
                radius: width / 2
                color: "transparent"
                border.color: "#ff9500"
                border.width: 2 + pulse * 2.5
                opacity: 0.9
                visible: scenarioDelegate.inRange
                property real pulse: 0
                SequentialAnimation on pulse {
                    running: scenarioDelegate.inRange
                    loops: Animation.Infinite
                    NumberAnimation { from: 0.0; to: 1.0; duration: 500; easing.type: Easing.InOutQuad }
                    NumberAnimation { from: 1.0; to: 0.0; duration: 500; easing.type: Easing.InOutQuad }
                }
            }

            AircraftItem {
                id: scenarioIcon
                anchors.centerIn: parent
                bearing: model.heading
                opacity: scenarioDelegate.inRange ? 1.0 : 0.9
                scale: scenarioDelegate.inRange ? 1.4 : 1.1
            }

            // Flight-id tag, shown only while in range (like an ADS-B label)
            Rectangle {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.top
                anchors.bottomMargin: 2
                visible: scenarioDelegate.inRange
                radius: 3
                color: "#0a1622"
                border.color: "#ff9500"
                border.width: 1
                width: idText.implicitWidth + 8
                height: idText.implicitHeight + 4
                Text {
                    id: idText
                    anchors.centerIn: parent
                    text: model.flightId
                    color: "#ff9500"
                    font.pixelSize: 9
                    font.bold: true
                }
            }
        }
    }
}
