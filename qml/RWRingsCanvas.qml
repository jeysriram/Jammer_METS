import QtQuick 2.14
import QtLocation 5.14
import QtPositioning 5.14

/*
  RWRingsCanvas — Draws range rings at real geographic scale using MapCircle.
  All rings are MapCircle (geographic), not Canvas pixels.
*/
Item {
    id: root
    property var ringCoordinate: null
    property var distances: [5000, 10000, 15000, 20000, 25000] // meters
    property color color: "#2ecc71"
    property color fillColor: "transparent"
    property color tickColor: "#2ecc71"
    property string name: ""
    property var mapRef: null
    property real neededWidth: 200
    property real neededHeight: 200

    // Radar-style pulse "ping": an expanding + fading ring, looping
    property bool pulseEnabled: true
    property int  pulseDuration: 2500   // ms per pulse cycle

    function calcSize() {
        if (!mapRef || !ringCoordinate || distances.length === 0) {
            neededWidth = 200;
            neededHeight = 200;
            return;
        }
        var maxDist = distances[distances.length - 1];
        var edge = ringCoordinate.atDistanceAndAzimuth(maxDist, 0);
        var centerPx = mapRef.fromCoordinate(ringCoordinate);
        var edgePx = mapRef.fromCoordinate(edge);
        var r = Math.abs(edgePx.x - centerPx.x);
        neededWidth = r * 2 + 60;
        neededHeight = r * 2 + 60;
    }

    Component.onCompleted: calcSize()

    Connections {
        target: mapRef
        function onZoomLevelChanged() { calcSize(); }
        function onCenterChanged() { calcSize(); }
    }

    // All ring circles
    Repeater {
        model: root.distances.length

        MapCircle {
            center: root.ringCoordinate
            radius: root.distances[index]
            color: root.fillColor
            border.width: (index === root.distances.length - 1) ? 2 : 1
            border.color: root.color
        }
    }

    // Radar pulse "ping": a ring that expands from the centre to the outer
    // range while fading out, looping. Geographic (MapCircle) so it stays
    // at true scale regardless of zoom.
    MapCircle {
        id: pulseRing
        center: root.ringCoordinate
        color: "transparent"
        border.width: 2
        border.color: root.color
        radius: 0
        opacity: 0
        visible: root.pulseEnabled

        NumberAnimation on radius {
            running: root.pulseEnabled
            loops: Animation.Infinite
            from: 0
            to: root.distances[root.distances.length - 1]
            duration: root.pulseDuration
            easing.type: Easing.OutQuad
        }

        NumberAnimation on opacity {
            running: root.pulseEnabled
            loops: Animation.Infinite
            from: 0.9
            to: 0.0
            duration: root.pulseDuration
            easing.type: Easing.InQuad
        }
    }

    // Degree labels on outer ring
    Repeater {
        model: [0, 30, 60, 90, 120, 150, 180, 210, 240, 270, 300, 330]

        MapQuickItem {
            coordinate: root.ringCoordinate.atDistanceAndAzimuth(root.distances[root.distances.length - 1], modelData)
            anchorPoint.x: -10
            anchorPoint.y: -5
            sourceItem: Text {
                text: modelData + "°"
                color: root.color
                font.pixelSize: 9
                font.bold: true
            }
        }
    }

    // Cardinal direction labels
    MapQuickItem {
        coordinate: root.ringCoordinate.atDistanceAndAzimuth(root.distances[root.distances.length - 1], 0)
        anchorPoint.x: -4
        anchorPoint.y: -14
        sourceItem: Text { text: "N"; color: root.color; font.pixelSize: 9; font.bold: true }
    }
    MapQuickItem {
        coordinate: root.ringCoordinate.atDistanceAndAzimuth(root.distances[root.distances.length - 1], 90)
        anchorPoint.x: -4
        anchorPoint.y: -14
        sourceItem: Text { text: "E"; color: root.color; font.pixelSize: 9; font.bold: true }
    }
    MapQuickItem {
        coordinate: root.ringCoordinate.atDistanceAndAzimuth(root.distances[root.distances.length - 1], 180)
        anchorPoint.x: -4
        anchorPoint.y: 10
        sourceItem: Text { text: "S"; color: root.color; font.pixelSize: 9; font.bold: true }
    }
    MapQuickItem {
        coordinate: root.ringCoordinate.atDistanceAndAzimuth(root.distances[root.distances.length - 1], 270)
        anchorPoint.x: -4
        anchorPoint.y: 10
        sourceItem: Text { text: "W"; color: root.color; font.pixelSize: 9; font.bold: true }
    }

    // Distance label on outer ring (right side)
    MapQuickItem {
        coordinate: root.ringCoordinate.atDistanceAndAzimuth(root.distances[root.distances.length - 1], 0)
        anchorPoint.x: 4
        anchorPoint.y: 6
        sourceItem: Text {
            text: (root.distances[root.distances.length - 1] / 1000) + " km"
            color: root.color
            font.pixelSize: 10
            font.bold: true
        }
    }

    // Name label at top (N position)
    MapQuickItem {
        coordinate: root.ringCoordinate.atDistanceAndAzimuth(root.distances[root.distances.length - 1], 0)
        anchorPoint.x: 0
        anchorPoint.y: -20
        sourceItem: Text {
            text: root.name
            color: "#2c3e50"
            font.pixelSize: 11
            font.bold: true
        }
    }
}
