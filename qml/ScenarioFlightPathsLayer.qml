import QtQuick 2.14
import QtLocation 5.14

/*
  ScenarioFlightPathsLayer.qml — optional debug/reference layer that draws
  each scenario flight's spline-interpolated route as a thin dim line.

  Root type is MapItemView itself (same trick as ScenarioFlightsLayer.qml),
  bound to the same "scenarioFlightModel" but reading its "path" role instead
  of "coordinate"/"heading" — a separate layer/model-view pair, exactly like
  MapView.qml already does for emitterModel (icons vs. range rings vs. LOB
  lines are all separate MapItemViews over the same model).

  Off by default: the original design intentionally keeps scenario waypoints
  invisible (only the moving aircraft shows), so this is an opt-in overlay —
  set visible: true (from MapView.qml, or a future toolbar toggle) to see it.
*/
MapItemView {
    id: root
    property bool pathsVisible: true

    model: scenarioFlightModel
    delegate: MapPolyline {
        path: model.path
        visible: root.pathsVisible
        line.width: 1.5
        line.color: "#19e3b1"
        opacity: 0.35
        smooth: false
    }
}
