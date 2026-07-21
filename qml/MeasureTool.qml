import QtQuick 2.14
import QtLocation 5.14
import QtPositioning 5.14

/*
  MeasureTool.qml — visual layer for the map "measure distance" tool.

  A reusable MapItemGroup that renders a two-point distance measurement:
    • marker "A" at pointA
    • marker "B" at pointB (or the line rubber-bands to endCoord while the
      user is still choosing B)
    • a straight line A → endCoord
    • a distance badge (distanceText) anchored at midCoord

  This component is PURE PRESENTATION. All state — which points are set, the
  geodesic distance, the midpoint — is computed by the parent (MapView) and
  passed in through the properties below. The click handling also lives in the
  parent, because it needs Map.toCoordinate() to turn a pixel into a lat/lng.

  Why a MapItemGroup: MapItems (MapPolyline / MapQuickItem) must be children
  of a Map (or a MapItemGroup). Wrapping them in a plain Item would stop them
  rendering on the map. MapItemGroup lets us package several map items into one
  reusable file and drop it into a Map like a single item:

      MeasureTool {
          active:       root.measureModeActive
          pointA:       root.measurePointA
          pointB:       root.measurePointB
          endCoord:     root.measureEnd
          midCoord:     root.measureMid
          distanceText: root.fmtDistance(root.measureMeters)
          fallback:     QtPositioning.coordinate(mapLat, mapLng)
      }
*/
MapItemGroup {
    id: tool

    // ---- inputs supplied by the parent ----
    property bool   active: false        // tool on/off (measure mode)
    property var    pointA: null         // first point   → marker "A"
    property var    pointB: null         // second point  → marker "B" (or null)
    property var    endCoord: null       // far end of the line (B, or live cursor)
    property var    midCoord: null       // midpoint → anchors the distance badge
    property string distanceText: ""     // pre-formatted distance, e.g. "12.34 km"

    // A valid throwaway coordinate used when a point is null: MapItems need a
    // non-null coordinate even while they are hidden.
    property var    fallback: QtPositioning.coordinate(0, 0)

    readonly property color accent: "#ffd54a"   // amber measurement colour

    // Connecting line A → end. Before B is placed, "end" is the live cursor,
    // so the line (and the distance badge) update as the pointer moves.
    MapPolyline {
        z: 60
        visible: tool.active && tool.pointA && tool.endCoord
        line.width: 3
        line.color: tool.accent
        path: (tool.pointA && tool.endCoord) ? [ tool.pointA, tool.endCoord ] : []
    }

    // Marker "A" — first clicked point
    MapQuickItem {
        z: 61
        visible: tool.active && tool.pointA
        coordinate: tool.pointA ? tool.pointA : tool.fallback
        anchorPoint.x: 8
        anchorPoint.y: 8
        sourceItem: Rectangle {
            width: 16; height: 16; radius: 8
            color: tool.accent; border.color: "#5a3d00"; border.width: 2
            Text {
                anchors.centerIn: parent
                text: "D1"; font.pixelSize: 9; font.bold: true; color: "#3a2a00"
            }
        }
    }

    // Marker "B" — second clicked point
    MapQuickItem {
        z: 61
        visible: tool.active && tool.pointB
        coordinate: tool.pointB ? tool.pointB : tool.fallback
        anchorPoint.x: 8
        anchorPoint.y: 8
        sourceItem: Rectangle {
            width: 16; height: 16; radius: 8
            color: tool.accent; border.color: "#5a3d00"; border.width: 2
            Text {
                anchors.centerIn: parent
                text: "D2"; font.pixelSize: 9; font.bold: true; color: "#3a2a00"
            }
        }
    }

    // Distance badge at the midpoint of the A → end line
    MapQuickItem {
        z: 62
        visible: tool.active && tool.midCoord && tool.distanceText.length > 0
        coordinate: tool.midCoord ? tool.midCoord : tool.fallback
        anchorPoint.x: badge.width / 2
        anchorPoint.y: badge.height / 2
        sourceItem: Rectangle {
            id: badge
            color: "#e61a1f2b"                 // translucent dark plate
            border.color: tool.accent; border.width: 1; radius: 5
            width: badgeText.implicitWidth + 12
            height: badgeText.implicitHeight + 6
            Text {
                id: badgeText
                anchors.centerIn: parent
                color: tool.accent; font.pixelSize: 12; font.bold: true
                text: tool.distanceText
            }
        }
    }
}
