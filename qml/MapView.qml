import QtQuick 2.14
import QtQuick.Controls 2.14
import QtLocation 5.14
import QtPositioning 5.14
/*
  MapView.qml — Main map view loaded by QQuickWidget in MainWindow.ui

  This is the QML side of the Qt Widgets + QML hybrid architecture.
  C++ models (EmitterModel, CDP_FlightController) are exposed via QQmlContext.
*/

Rectangle {
    id: root
    color: "#0a0a1a"

    // ============================================================
    // CONFIGURATION
    // ============================================================
    property double mapLat: 13.0827
    property double mapLng: 80.2707
    property bool centerOnAircraft: false
    property real mapZoom: 6.0

    // Aircraft/flight name — shown as a click tooltip on the aircraft
    property string flightName: "MIG29K"
    property bool flightNameShown: false   // toggled by clicking the aircraft
    // Cinematic "fly-to" — animate BOTH the pan (mapLat/mapLng → center) and
    // the zoom, so a fit glides the whole view like the initial load. All
    // three share the _animateZoom guard so resetView() can silently snap the
    // tracked values to the user's current view first (no visible jump), then
    // re-enable the Behaviors and animate to the new fit.
    property bool _animateZoom: true
    Behavior on mapZoom {
        enabled: root._animateZoom
        NumberAnimation { duration: 1400; easing.type: Easing.InOutCubic }
    }
    Behavior on mapLat {
        enabled: root._animateZoom
        NumberAnimation { duration: 1400; easing.type: Easing.InOutCubic }
    }
    Behavior on mapLng {
        enabled: root._animateZoom
        NumberAnimation { duration: 1400; easing.type: Easing.InOutCubic }
    }

    // Fit the map so the WHOLE flight path is visible, using the real viewport
    // size and a margin (Web-Mercator "fit bounds"). Returns true once applied.
    function fitToFlightPath() {
        if (!flightController || flightController.flightPath.length === 0)
            return false
        if (mapView.width <= 0 || mapView.height <= 0)
            return false   // viewport not laid out yet

        var path = flightController.flightPath
        var minLat = 90, maxLat = -90, minLng = 180, maxLng = -180
        for (var i = 0; i < path.length; i++) {
            var c = path[i]
            if (c.latitude  < minLat) minLat = c.latitude
            if (c.latitude  > maxLat) maxLat = c.latitude
            if (c.longitude < minLng) minLng = c.longitude
            if (c.longitude > maxLng) maxLng = c.longitude
        }

        // Centre on the bounding box
        mapLat = (minLat + maxLat) / 2
        mapLng = (minLng + maxLng) / 2

        function latRad(lat) {
            var s = Math.sin(lat * Math.PI / 180)
            return Math.log((1 + s) / (1 - s)) / 2
        }
        function zoomFor(px, fraction) {
            return Math.log(px / 256 / fraction) / Math.LN2
        }

        var PADDING = 0.85   // ~15% margin so the path doesn't touch the edges
        var latFraction = Math.max((latRad(maxLat) - latRad(minLat)) / Math.PI, 1e-9)
        var lngDiff = maxLng - minLng
        var lngFraction = Math.max((lngDiff < 0 ? lngDiff + 360 : lngDiff) / 360, 1e-9)

        var z = Math.min(zoomFor(mapView.height * PADDING, latFraction),
                         zoomFor(mapView.width  * PADDING, lngFraction))
        z += 0.6   // slightly zoom in past the exact fit on load
        if (z < mapView.minimumZoomLevel) z = mapView.minimumZoomLevel
        if (z > mapView.maximumZoomLevel) z = mapView.maximumZoomLevel
        mapZoom = z
        return true
    }
    // Home / "reset view" — called by the toolbar Home button. After the user
    // pans or wheel-zooms the map, QtLocation writes mapView.center/zoomLevel
    // directly, which BREAKS the `center: _mapCenter` / `zoomLevel: _mapZoom`
    // bindings (and _userInteracted is set, stopping auto-fit). This restores
    // the default view and hands control back to those reactive bindings:
    //   1. re-enable auto-fit,
    //   2. snap the tracked mapZoom to the map's CURRENT zoom without animating
    //      (so restoring the binding doesn't cause a visible jump),
    //   3. re-establish the center/zoom bindings,
    //   4. recompute the default fit — mapZoom's Behavior then animates the map
    //      smoothly from the user's zoom back to the default.
    function resetView() {
        _userInteracted = false
        // (2) sync the tracked zoom AND centre to the live view, with the
        //     Behaviors suppressed — restoring the bindings in (3) is then a
        //     no-op visually, and the fit in (4) animates pan + zoom together
        //     from exactly where the user left the map.
        _animateZoom = false
        mapZoom = mapView.zoomLevel
        mapLat  = mapView.center.latitude
        mapLng  = mapView.center.longitude
        _animateZoom = true
        // (3) reconnect the reactive bindings the gestures had overridden
        mapView.zoomLevel = Qt.binding(function() { return mapView._mapZoom })
        mapView.center    = Qt.binding(function() { return mapView._mapCenter })
        // (4) recompute default center/zoom → animates via the mapZoom Behavior
        if (!fitToFlightPath()) {
            // No flight path yet: fall back to the configured default location.
            mapLat  = 13.0827
            mapLng  = 80.2707
            mapZoom = 6.0
        }
    }

    // Auto-fit is debounced: every viewport size change restarts the timer, so
    // the fit only runs ~after the QQuickWidget has settled to its final size
    // and the map is ready. Without this the map applies the centre against a
    // transient size and only corrects on the first user scroll. Auto-fit stops
    // once the user pans/zooms.
    property bool _userInteracted: false
    // When true, the next fitTimer tick performs a full CINEMATIC refit via
    // resetView() (rebind center/zoom + animated fly-to) instead of a plain
    // in-place fit. Set whenever a NEW flight path arrives — both the initial
    // auto-load and every CSV loaded later from the Load Settings dialog — so
    // the map always flies to the new path exactly like the first load.
    property bool _cinematicFitPending: false
    Timer {
        id: fitTimer
        interval: 250
        repeat: false
        // Dispatch: a pending cinematic request wins over the plain fit.
        onTriggered: {
            if (root._cinematicFitPending) {
                root._cinematicFitPending = false
                root.resetView()          // restores bindings, then animated fly-to
            } else {
                root.fitToFlightPath()    // plain fit (viewport-settle refits)
            }
        }
    }
    // Plain debounced fit — used for viewport-settle refits (width/height
    // changes) and emitter-only loads. Deliberately does NOTHING once the
    // user has panned/zoomed: a resize must not steal the view they chose.
    function _scheduleFit() {
        if (_userInteracted)
            return
        if (mapView.width > 0 && mapView.height > 0)
            fitTimer.restart()
    }
    // Cinematic refit request — called when a NEW flight path is loaded.
    // Unlike _scheduleFit() this ignores _userInteracted: loading a new path
    // must always fly the map to it, even if the user had already panned or
    // zoomed (resetView() reconnects the center/zoom bindings the gestures
    // broke, so the animated fit is visible again).
    function _scheduleCinematicFit() {
        _cinematicFitPending = true
        if (mapView.width > 0 && mapView.height > 0)
            fitTimer.restart()
        // else: the flag stays pending and is consumed by the fit that runs
        // once the viewport gets its real size (onWidth/HeightChanged).
    }

    Component.onCompleted: {
        if (flightController)
            flightController.initAircraftPosition(mapLat, mapLng)
        if (emitterModel)
            emitterModel.emitterDataLoaded.connect(_scheduleFit)   // refit after CSV load
        _scheduleFit()
    }

    // Fly to every newly loaded flight path with the same cinematic move/zoom
    // as the initial load — this fires for the startup auto-load AND for any
    // waypoints CSV loaded later from the Load Settings dialog.
    Connections {
        target: flightController
        function onFlightPathChanged() { root._scheduleCinematicFit() }
    }

    // Toolbar toggle states (synced with C++ if needed)
    property bool emittersVisible: true
    property bool aircraftRingsVisible: true
    property bool emitterRangeRingsVisible: true
    property bool emitterPathVisible: true
    property real emitterIconSize: 24
    property bool lobHistoryVisible: false
    property bool measureModeActive: false
    property bool autoAlignActive: false
    property bool autoBearingActive: false
    property color fillColor: "transparent"

    // ============================================================
    // DISTANCE MEASUREMENT STATE (driven by the "measure distance"
    // toolbar button). The user clicks two points on the map; a marker
    // is dropped at each, a line joins them, and the geodesic distance
    // is shown at the midpoint.
    //   - measurePointA : first clicked coordinate  (marker "A")
    //   - measurePointB : second clicked coordinate (marker "B")
    //   - measureCursor : live pointer position — rubber-bands the line
    //                     and live distance AFTER A is placed but BEFORE
    //                     B is clicked.
    // Click sequence: 1st click sets A, 2nd sets B (result frozen), a
    // 3rd click starts a fresh measurement from a new A.
    // ============================================================
    property var measurePointA: null
    property var measurePointB: null
    property var measureCursor: null
    // Far end of the measurement line: the fixed point B once placed,
    // otherwise the live cursor while the user is still choosing B.
    property var measureEnd: measurePointB ? measurePointB
                                           : (measurePointA ? measureCursor : null)
    // Geodesic distance A→end in metres (0 until both ends exist).
    // distanceTo() is QGeoCoordinate's great-circle distance, so this is
    // true ground distance, independent of zoom/projection.
    property real measureMeters: (measurePointA && measureEnd)
                                 ? measurePointA.distanceTo(measureEnd) : 0
    // Midpoint of the A→end geodesic — where the distance badge is anchored.
    // (walk half the distance from A along the bearing A→end.)
    property var measureMid: (measurePointA && measureEnd && measureMeters > 0)
        ? measurePointA.atDistanceAndAzimuth(measureMeters / 2,
                                             measurePointA.azimuthTo(measureEnd))
        : null
    // Format a metre distance as "850 m" or "12.34 km".
    function fmtDistance(m) {
        if (m <= 0) return ""
        return (m < 1000) ? (m.toFixed(0) + " m") : ((m / 1000).toFixed(2) + " km")
    }
    // Clear any in-progress / completed measurement (used when the tool is
    // toggled on or off, so it always starts from a clean slate).
    function clearMeasurement() {
        measurePointA = null
        measurePointB = null
        measureCursor = null
    }
    // Currently hovered emitter (set by emitter delegates) → drives the tooltip overlay
    property var hoverEmitter: null


    // ============================================================
    // MAP PLUGINS (online vs. existing custom tile server)
    // ============================================================
    // ---- Existing custom tile server (datapatterns.co.in) ----
    Plugin {
        id: offlineMapPlugin
        name: "osm"

        PluginParameter {
            name: "osm.mapping.providersrepository.disabled"
            value: true
        }

        PluginParameter {
            name: "osm.mapping.custom.host"
            value: "http://maps.datapatterns.co.in/api/maps/basic/"
        }

        PluginParameter {
            name: "osm.mapping.custom.mapcopyright"
            value: "<b>DTARANG Map</b>"
        }
    }

    // ---- Online OpenStreetMap tiles ----
    // Point directly at the OSM tile host with the providers repository disabled,
    // so it does not depend on the (often unavailable) Qt provider redirect.
    Plugin {
        id: onlineMapPlugin
        name: "osm"

        PluginParameter {
            name: "osm.mapping.providersrepository.disabled"
            value: true
        }

        PluginParameter {
            name: "osm.mapping.custom.host"
            value: "https://tile.openstreetmap.org/"
        }

        PluginParameter {
            name: "osm.mapping.highdpi_tiles"
            value: true
        }

        PluginParameter {
            name: "osm.mapping.custom.mapcopyright"
            value: "© OpenStreetMap contributors"
        }
    }

    // ============================================================
    // MAP CONTAINER
    // ============================================================
    Map {
        id: mapView
        anchors.fill: parent
        anchors.margins: 2

        // Map source: online OpenStreetMap when the DP_USE_ONLINE_MAP macro is
        // enabled (bridged from C++ as g_bUseOnlineMap), otherwise the existing
        // custom DTARANG tile server. Both plugins are defined above.
        plugin: (typeof g_bUseOnlineMap !== "undefined" && g_bUseOnlineMap)
                    ? onlineMapPlugin
                    : offlineMapPlugin

        // CRITICAL: select CustomMap type (always last entry)
        //activeMapType: supportedMapTypes[supportedMapTypes.length - 1]

        center: _mapCenter
        zoomLevel: _mapZoom
        // While flying OR paused mid-flight, keep the map on the aircraft so it
        // doesn't jump away when paused. Only use the fitted bounding-box centre
        // when parked at the very start.
        property var _mapCenter: (flightController.isFlying || flightController.progressPercent > 0)
                                 ? flightController.aircraftPosition
                                 : QtPositioning.coordinate(mapLat, mapLng)
        property real _mapZoom: mapZoom

        minimumZoomLevel: 3
        maximumZoomLevel: 18
        gesture.enabled: true

        // The QQuickWidget often has size 0 at load and changes size while the
        // window settles; (re)schedule the fit on each size change (debounced).
        onWidthChanged:  root._scheduleFit()
        onHeightChanged: root._scheduleFit()

        // Once the user pans/zooms, stop auto-fitting so we don't fight them.
        gesture.onPanStarted:   root._userInteracted = true
        gesture.onPinchStarted: root._userInteracted = true

        // --------------------------------------------------------
        // LAYER 1: EMITTER MARKERS
        // --------------------------------------------------------
        MapItemView {
            model: emitterModel
            delegate: MapQuickItem {
                id: emitterDelegate
                coordinate: model.coordinate
                anchorPoint.x: ewIcon.width / 2
                anchorPoint.y: ewIcon.height          // tower base sits on the location
                visible: emittersVisible

                // Detected while flying & within range → the icon "emits" signal waves
                property bool detected: emittersVisible && flightController.isFlying
                                        && (flightController.currentSegment >= 0)
                                        && flightController.isEmitterDetected(model.lat, model.lng)
                property real emitPhase: 0
                NumberAnimation on emitPhase {
                    running: emitterDelegate.detected
                    loops: Animation.Infinite
                    from: 0; to: 1; duration: 1200
                }
                onEmitPhaseChanged: wavesCanvas.requestPaint()
                onDetectedChanged: wavesCanvas.requestPaint()
                sourceItem: Item {
                    id: ewIcon
                    width: root.emitterIconSize * 2.6
                    height: root.emitterIconSize * 2.6
                    property real towerW: root.emitterIconSize * 1.5
                    property real towerH: root.emitterIconSize * 1.5

                    // Expanding red emission waves — only while detected
                    Canvas {
                        id: wavesCanvas
                        anchors.fill: parent
                        onPaint: {
                            var wctx = getContext("2d")
                            wctx.clearRect(0, 0, width, height)
                            if (!emitterDelegate.detected)
                                return
                            var wcx = width / 2
                            var wNodeY = height - ewIcon.towerH * 0.58
                            var wMaxR = width * 0.48
                            wctx.lineWidth = 2
                            for (var wi = 0; wi < 3; wi++) {
                                var wph = (emitterDelegate.emitPhase + wi / 3) % 1
                                var wr = wph * wMaxR
                                var wa = (1 - wph) * 0.85
                                wctx.beginPath()
                                wctx.arc(wcx, wNodeY, wr, -Math.PI * 0.92, -Math.PI * 0.08)
                                wctx.strokeStyle = "rgba(255,59,48," + wa.toFixed(3) + ")"
                                wctx.stroke()
                            }
                        }
                    }
                    // Modern EW / radar emitter symbol (transmitter tower + emission fan)
                    Canvas {
                        id: towerCanvas
                        width: ewIcon.towerW
                        height: ewIcon.towerH
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.bottom: parent.bottom
                        onPaint: {
                            var ctx = getContext("2d")
                            var w = width, h = height
                            ctx.clearRect(0, 0, w, h)
                            ctx.lineCap = "round"
                            ctx.lineJoin = "round"

                            var cx = w / 2
                            var baseY = h - 3
                            var nodeY = h * 0.42

                            // --- emission fan (signal waves) ---
                            ctx.shadowColor = "rgba(231,76,60,0.7)"
                            ctx.shadowBlur = 6
                            var arcColors = ["#ff6b3d", "#f5a623", "#f7c948"]
                            for (var i = 0; i < 3; i++) {
                                ctx.beginPath()
                                ctx.arc(cx, nodeY, 6 + i * 5, -Math.PI * 0.85, -Math.PI * 0.15)
                                ctx.strokeStyle = arcColors[i]
                                ctx.globalAlpha = 1.0 - i * 0.25
                                ctx.lineWidth = 2
                                ctx.stroke()
                            }
                            ctx.globalAlpha = 1.0
                            ctx.shadowBlur = 0

                            // --- mast ---
                            ctx.strokeStyle = "#0b2235"
                            ctx.lineWidth = 2.5
                            ctx.beginPath()
                            ctx.moveTo(cx, nodeY)
                            ctx.lineTo(cx, baseY)
                            ctx.stroke()

                            // --- base platform ---
                            ctx.fillStyle = "#0b2235"
                            ctx.beginPath()
                            ctx.moveTo(cx - 7, baseY)
                            ctx.lineTo(cx + 7, baseY)
                            ctx.lineTo(cx + 4, baseY - 5)
                            ctx.lineTo(cx - 4, baseY - 5)
                            ctx.closePath()
                            ctx.fill()

                            // --- emitter node ---
                            ctx.fillStyle = "#e74c3c"
                            ctx.strokeStyle = "#ffffff"
                            ctx.lineWidth = 1
                            ctx.beginPath()
                            ctx.arc(cx, nodeY, 3.5, 0, 2 * Math.PI)
                            ctx.fill()
                            ctx.stroke()
                        }
                    }

                    // Hover detector — reports the hovered emitter to the root
                    // overlay tooltip. NoButton so map pan/click still pass through.
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        acceptedButtons: Qt.NoButton
                        onEntered: root.hoverEmitter = {
                            "no": index + 1,
                            "name": model.name,
                            "lat": model.lat,
                            "lng": model.lng,
                            "range": Number(model.range),
                            "coordinate": model.coordinate
                        }
                        onExited: {
                            if (root.hoverEmitter && root.hoverEmitter.no === index + 1)
                                root.hoverEmitter = null
                        }
                    }
                }
            }
        }

        // --------------------------------------------------------
        // LAYER 2: FLIGHT PATH — COMPLETED (green)
        // --------------------------------------------------------
        MapPolyline {
            id: completedPathLine
            z: 1
            visible: flightController.isFlying || flightController.flightPath.length > 0
            line.width: 4
            line.color: "#27ae60"
            smooth: false
            path: flightController.completedPath.length > 0 ? flightController.completedPath : (flightController.remainingPath.length > 0 ? flightController.remainingPath : [])
        }

        // --------------------------------------------------------
        // LAYER 3: FLIGHT PATH — REMAINING (blue)
        // --------------------------------------------------------
        MapPolyline {
            id: remainingPathLine
            z: 2
            visible: flightController.isFlying || flightController.flightPath.length > 0
            line.width: 4
            line.color: "#2980b9"
            smooth: false
            path: flightController.remainingPath.length > 0 ? flightController.remainingPath : []
        }


        // --------------------------------------------------------
        // LAYER 4: AIRCRAFT + RANGE RINGS (single MapQuickItem for alignment)
        // --------------------------------------------------------
        MapQuickItem {
            id: aircraftGroup
            z: 100
            // While flying OR paused mid-flight (progress > 0) show the aircraft's
            // real position; only when parked at the very start use waypoint 0.
            coordinate: (flightController.isFlying || flightController.progressPercent > 0)
                        ? flightController.aircraftPosition
                        : (flightController.flightPath.length > 0 ? flightController.flightPath[0] : QtPositioning.coordinate(mapLat, mapLng))
            anchorPoint.x: aircraftGroupItem.width / 2
            anchorPoint.y: aircraftGroupItem.height / 2
            visible: flightController.flightPath.length > 0
            // Component.onCompleted on MapQuickItem — removed duplicate

            sourceItem: Item {
                id: aircraftGroupItem
                width: Math.max(aircraftIcon.width, aircraftRings.width)
                height: Math.max(aircraftIcon.height, aircraftRings.height)

                // Range rings behind aircraft, centered
                EmitterRangeRings {
                    id: aircraftRings
                    anchors.centerIn: parent
                    targetMap: mapView
                    ringCoordinate: flightController.aircraftPosition
                    emitterName: "MIG29K"
                    visible: aircraftRingsVisible
                    pulseEnabled: flightController.isFlying     // pulse only while flying
                    heading: flightController.aircraftBearing   // head-up: 0° = flight direction
                }

                // Aircraft in front, centered
                AircraftItem {
                    id: aircraftIcon
                    anchors.centerIn: parent
                    bearing: flightController.aircraftBearing
                    // Scale ONLY the aircraft icon with zoom (clamped so it is
                    // neither a speck nor oversized). The range rings are scaled
                    // separately/geographically, so they are not affected by this.
                    scale: Math.max(0.8, Math.min(2.5, Math.pow(2, mapView.zoomLevel - 10) * 2.5))
                }
                // Click target over the aircraft — toggles the flight-name tooltip
                // (rendered as a root overlay so it is never clipped by the rings).
                MouseArea {
                    id: aircraftClick
                    anchors.centerIn: parent
                    width: 54
                    height: 54
                    onClicked: root.flightNameShown = !root.flightNameShown
                }
            }

            // NOTE: no group-level scale here. The 5/10/15/20/25 km range rings
            // size themselves to true geographic distance at the current zoom
            // (see EmitterRangeRings.recalibrate()), so they grow when zooming in
            // and shrink when zooming out — matching the map scale exactly.
        }

        // --------------------------------------------------------
        // LAYER 5: EMITTER RANGE RINGS (red) — one per emitter
        // Visible when aircraft is flying AND is within 20km of that emitter (initial logic preserved)
        // When waypoint-based detection data is available, use struct instead
        // --------------------------------------------------------
        MapItemView {
            model: emitterModel
            delegate: MapCircle {
                id: emitRing
                center: model.coordinate
                radius: (model.range > 0 ? model.range : 20) * 1000   // per-emitter CSV range (km → m)

                // --- Proximity threat escalation ---
                // closeness: 0 at the edge of range, 1 at the emitter.
                property real rngM: (model.range > 0 ? model.range : 20) * 1000
                property real acDist: (flightController.aircraftPosition && flightController.aircraftPosition.isValid)
                                      ? model.coordinate.distanceTo(flightController.aircraftPosition) : rngM
                property real closeness: Math.max(0, Math.min(1, 1 - acDist / rngM))
                // amber/orange when far → pure red when near
                property color threatColor: Qt.rgba(1.0, 0.72 * (1.0 - closeness), 0.0, 1.0)
                // pulse cadence speeds up as the aircraft closes (700ms → 250ms)
                property int pulseMs: Math.round(700 - closeness * 450)
                property real pulse: 0.0

                SequentialAnimation on pulse {
                    running: emitRing.visible
                    loops: Animation.Infinite
                    NumberAnimation { from: 0.0; to: 1.0; duration: emitRing.pulseMs; easing.type: Easing.InOutQuad }
                    NumberAnimation { from: 1.0; to: 0.0; duration: emitRing.pulseMs; easing.type: Easing.InOutQuad }
                }

                color: Qt.rgba(threatColor.r, threatColor.g, threatColor.b, 0.10 + pulse * 0.20)
                border.color: threatColor
                border.width: 2 + pulse * 2.5
                // Detected ONLY while flying and within this emitter's range.
                // (currentSegment reference forces re-evaluation as the aircraft moves.)
                property bool detected: emittersVisible && flightController.isFlying
                                        && (flightController.currentSegment >= 0)
                                        && flightController.isEmitterDetected(model.lat, model.lng)
                visible: detected
                // --- New-threat blink: flash the ring for ~2.5s on first detection ---
                property real blinkOpacity: 1.0
                opacity: blinkOpacity
                onDetectedChanged: {
                    if (detected) blinkAnim.restart()
                    else { blinkAnim.stop(); blinkOpacity = 1.0 }
                }
                SequentialAnimation {
                    id: blinkAnim
                    loops: 5
                    NumberAnimation { target: emitRing; property: "blinkOpacity"; from: 1.0; to: 0.2; duration: 250 }
                    NumberAnimation { target: emitRing; property: "blinkOpacity"; from: 0.2; to: 1.0; duration: 250 }
                }
            }
        }

        // (Threat-lock ping circle removed — the emitter ICON now animates the
        //  emission waves when detected, see LAYER 1.)

        // --------------------------------------------------------
        // LAYER 5c: LOB STROBE — bearing line from the aircraft to each
        // detected emitter (RWR strobe-to-threat), strobing while detected.
        // --------------------------------------------------------
        MapItemView {
            model: emitterModel
            delegate: MapPolyline {
                id: lobLine
                z: 3
                line.width: 2
                line.color: "#ff3b30"
                path: [flightController.aircraftPosition, model.coordinate]

                property bool detected: emittersVisible && flightController.isFlying
                                        && (flightController.currentSegment >= 0)
                                        && flightController.isEmitterDetected(model.lat, model.lng)
                visible: detected
                opacity: 0.3
                SequentialAnimation on opacity {
                    running: lobLine.detected
                    loops: Animation.Infinite
                    NumberAnimation { from: 0.25; to: 1.0; duration: 300 }
                    NumberAnimation { from: 1.0; to: 0.25; duration: 300 }
                }
            }
        }

        // --------------------------------------------------------
        // LAYER 6: AIRCRAFT RANGE RINGS (green) — concentric circles + labels
        // Visible only when parked at the very start (not flying and not paused
        // mid-flight); otherwise LAYER 4 draws the rings around the aircraft.
        // --------------------------------------------------------
        MapItemView {
            model: (!flightController.isFlying && flightController.progressPercent <= 0) ? [1] : []
            delegate: MapQuickItem {
                coordinate: flightController.flightPath.length > 0 ? flightController.flightPath[0] : QtPositioning.coordinate(mapLat, mapLng)
                anchorPoint.x: aircraftRingsGroup.width / 2
                anchorPoint.y: aircraftRingsGroup.height / 2

                sourceItem: Item {
                    id: aircraftRingsGroup
                    width: ringsCanvas.neededWidth
                    height: ringsCanvas.neededHeight

                    RWRingsCanvas {
                        id: ringsCanvas
                        anchors.centerIn: parent
                        ringCoordinate: flightController.flightPath.length > 0 ? flightController.flightPath[0] : QtPositioning.coordinate(mapLat, mapLng)
                        distances: [5000, 10000, 15000, 20000, 25000]
                        color: "#2ecc71"
                        name: "Aircraft"
                        mapRef: mapView
                        pulseEnabled: false   // parked rings (shown when not flying) don't pulse
                    }
                }
            }
        }

        // Ring center helper — computed from current flight state. Referenced
        // as mapView._ringCenter (not unqualified) from the MapQuickItem
        // coordinate bindings below: those bindings are evaluated in the
        // context of Map's mapItems default-property list, which does not
        // resolve unqualified custom properties on the Map itself.
        property var _ringCenter: flightController.isFlying ? flightController.aircraftPosition : (flightController.flightPath.length > 0 ? flightController.flightPath[0] : QtPositioning.coordinate(mapLat, mapLng))

        // Degree tick labels
        MapQuickItem {
            z: 2
            coordinate: mapView._ringCenter.atDistanceAndAzimuth(25000, 0)
            anchorPoint.x: 0
            anchorPoint.y: -12
            visible: aircraftRingsVisible
            sourceItem: Text { text: "0°"; color: "#2ecc71"; font.pixelSize: 9; font.bold: true }
        }
        MapQuickItem {
            z: 2
            coordinate: mapView._ringCenter.atDistanceAndAzimuth(25000, 90)
            anchorPoint.x: 0
            anchorPoint.y: -12
            visible: aircraftRingsVisible
            sourceItem: Text { text: "90°"; color: "#2ecc71"; font.pixelSize: 9; font.bold: true }
        }
        MapQuickItem {
            z: 2
            coordinate: mapView._ringCenter.atDistanceAndAzimuth(25000, 180)
            anchorPoint.x: 0
            anchorPoint.y: 10
            visible: aircraftRingsVisible
            sourceItem: Text { text: "180°"; color: "#2ecc71"; font.pixelSize: 9; font.bold: true }
        }
        MapQuickItem {
            z: 2
            coordinate: mapView._ringCenter.atDistanceAndAzimuth(25000, 270)
            anchorPoint.x: 0
            anchorPoint.y: 10
            visible: aircraftRingsVisible
            sourceItem: Text { text: "270°"; color: "#2ecc71"; font.pixelSize: 9; font.bold: true }
        }

        // Distance labels
        MapQuickItem {
            z: 2
            coordinate: mapView._ringCenter.atDistanceAndAzimuth(5000, 0)
            anchorPoint.x: 4
            anchorPoint.y: 6
            visible: aircraftRingsVisible
            sourceItem: Text { text: "5 km"; color: "#2ecc71"; font.pixelSize: 9 }
        }
        MapQuickItem {
            z: 2
            coordinate: mapView._ringCenter.atDistanceAndAzimuth(25000, 0)
            anchorPoint.x: 4
            anchorPoint.y: 6
            visible: aircraftRingsVisible
            sourceItem: Text { text: "25 km"; color: "#2ecc71"; font.bold: true; font.pixelSize: 10 }
        }

        // --------------------------------------------------------
        // LAYER 7: DISTANCE MEASUREMENT
        //   • Visuals (line, A/B markers, distance badge) → MeasureTool.qml
        //   • Input (click → coordinate) → measureMouseArea (below)
        // The tool reads the measure* state defined near the top of the file.
        // --------------------------------------------------------
        MeasureTool {
            active:       root.measureModeActive
            pointA:       root.measurePointA
            pointB:       root.measurePointB
            endCoord:     root.measureEnd
            midCoord:     root.measureMid
            distanceText: root.fmtDistance(root.measureMeters)
            fallback:     QtPositioning.coordinate(mapLat, mapLng)
        }

        // Click / hover capture for the measure tool. It is enabled ONLY in
        // measure mode, so the map's normal pan/zoom gestures are untouched the
        // rest of the time. mapView.toCoordinate() converts the pixel position
        // of the click into a geographic coordinate.
        MouseArea {
            id: measureMouseArea
            anchors.fill: parent
            z: 9000                                   // above the map content
            enabled: root.measureModeActive
            hoverEnabled: root.measureModeActive
            // NOTE: a MouseArea's cursorShape applies even while the area is
            // DISABLED, so this must be conditional — otherwise the crosshair
            // covers the whole map all the time, not just in measure mode.
            cursorShape: root.measureModeActive ? Qt.CrossCursor : Qt.ArrowCursor

            onClicked: {
                var c = mapView.toCoordinate(Qt.point(mouse.x, mouse.y))
                if (!root.measurePointA || root.measurePointB) {
                    // nothing started yet, OR the previous measurement is
                    // complete → begin a fresh one from this point (A)
                    root.measurePointA = c
                    root.measurePointB = null
                    root.measureCursor = c
                } else {
                    // A already placed → this click fixes B and freezes the result
                    root.measurePointB = c
                }
            }
            // While choosing B, follow the pointer so the line + distance update live
            onPositionChanged: {
                if (root.measurePointA && !root.measurePointB)
                    root.measureCursor = mapView.toCoordinate(Qt.point(mouse.x, mouse.y))
            }
        }
        Component.onCompleted:
        {
            for(var i_type in mapView.supportedMapTypes)
            {
                if(mapView.supportedMapTypes[i_type].name === "Custom URL Map")
                {
                    mapView.activeMapType = mapView.supportedMapTypes[i_type];
                }
            }
        }
    }

    // ============================================================
    // MEASURE CARD — Google-Earth-style panel shown while the measure
    // tool is active. Shows the running distance, tells the user what to
    // click next, and offers Reset (clear the two points, keep measuring)
    // and Done (leave the tool).
    // ============================================================
    Rectangle {
        id: measureCard
        visible: root.measureModeActive
        z: 500
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 18
        radius: 8
        color: "#ee101b2e"
        border.color: "#ffd54a"
        border.width: 1
        width: measureCol.implicitWidth + 28
        height: measureCol.implicitHeight + 18
        Column {
            id: measureCol
            anchors.centerIn: parent
            spacing: 6
            // Title + big distance readout
            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 10
                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    text: "MEASURE"; color: "#ffd54a"
                    font.pixelSize: 11; font.bold: true; font.letterSpacing: 1
                }
                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    color: "#ffffff"; font.pixelSize: 16; font.bold: true
                    // "—" until a distance exists, then live/final distance covered
                    text: root.measureMeters > 0 ? root.fmtDistance(root.measureMeters) : "—"
                }
            }
            // Step instruction
            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                color: "#bcd0e8"; font.pixelSize: 10
                text: !root.measurePointA ? "Click the first point on the map"
                      : (!root.measurePointB ? "Click the second point"
                                             : "Measurement complete")
            }
            // Action buttons: Reset (clear points) and Done (exit tool)
            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 8
                // --- Reset: clears the two markers but stays in measure mode ---
                Rectangle {
                    width: resetTxt.implicitWidth + 22; height: 26; radius: 5
                    color: resetMouse.containsMouse ? "#3a2a00" : "#241a00"
                    border.color: "#ffd54a"; border.width: 1
                    Text {
                        id: resetTxt; anchors.centerIn: parent; text: "Reset"
                        color: "#ffd54a"; font.pixelSize: 11; font.bold: true
                    }
                    MouseArea {
                        id: resetMouse; anchors.fill: parent
                        hoverEnabled: true; cursorShape: Qt.PointingHandCursor
                        onClicked: root.clearMeasurement()
                    }
                }
                // --- Done: clears and turns the measure tool off ---
                Rectangle {
                    width: doneTxt.implicitWidth + 22; height: 26; radius: 5
                    color: doneMouse.containsMouse ? "#16273f" : "#0e1a2b"
                    border.color: "#2b6e8f"; border.width: 1
                    Text {
                        id: doneTxt; anchors.centerIn: parent; text: "Done"
                        color: "#cfe3ff"; font.pixelSize: 11; font.bold: true
                    }
                    MouseArea {
                        id: doneMouse; anchors.fill: parent
                        hoverEnabled: true; cursorShape: Qt.PointingHandCursor
                        onClicked: { root.measureModeActive = false; root.clearMeasurement() }
                    }
                }
            }
        }
    }
    // ============================================================
    // MAP TOOLBAR (right side) — slides in/out via the toggle button
    // ============================================================
    property bool toolbarVisible: true
    MapToolbar {
        id: mapToolbar
        anchors.top: parent.top
        anchors.topMargin: 160        // leaves room for the compass badge above it
        x: root.toolbarVisible ? (parent.width - width - 12) : parent.width
        Behavior on x { NumberAnimation { duration: 220; easing.type: Easing.OutCubic } }

        // Reflect MapView's actual state so each button's highlight is accurate
        emittersVisible: root.emittersVisible
        lobHistoryVisible: root.lobHistoryVisible
        autoAlignActive: root.autoAlignActive
        measureModeActive: root.measureModeActive
        rangeRingsVisible: root.aircraftRingsVisible
        autoBearingActive: root.autoBearingActive

        // Emitter button toggles BOTH the emitter icons and their range rings.
        // Qualify with root. — an unqualified name would hit the toolbar's own
        // same-named property instead of MapView's (which the layers read).
        // Home: snap the map back to the default fitted view (see resetView()).
        onHomeClicked: root.resetView()
        onMapLayersClicked: mapLayersPopup.open()
        onEmitterLocationClicked: root.emittersVisible = !root.emittersVisible
        onLobHistoryClicked: root.lobHistoryVisible = !root.lobHistoryVisible
        onAutoAlignClicked: root.autoAlignActive = !root.autoAlignActive
        // Measure tool: flip the mode and always start from a clean slate
        // (clears any markers/line whether we are turning it on or off).
        onMeasureDistanceClicked: {
            root.measureModeActive = !root.measureModeActive
            root.clearMeasurement()
        }
        onRangeRingsClicked: root.aircraftRingsVisible = !root.aircraftRingsVisible
        onAutoBearingClicked: root.autoBearingActive = !root.autoBearingActive

        // Flight control
        onStartFlightClicked: {
            flightController.startFlight()
        }
        onStopFlightClicked: {
            flightController.stopFlight()
        }
        onResetFlightClicked: {
            flightController.resetFlight()
        }
    }
    // ============================================================
    // MAP TOOLBAR TOGGLE — small pinned button that shows/hides the
    // map toolbar. Sits at the top-right corner, above the toolbar,
    // so the two never overlap. Flipping root.toolbarVisible drives
    // the toolbar's slide animation (see the MapToolbar's Behavior on x).
    // ============================================================
    Rectangle {
        id: toolbarToggle
        z: 200                                   // keep above the map/toolbar
        width: 30
        height: 30
        radius: 6
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: 12
        anchors.topMargin: 120
        opacity: 0.95
        // subtle hover feedback
        color: toggleMouse.containsMouse ? "#243352" : "#16213e"
        border.color: "#2b6e8f"
        border.width: 1
        // Glyph: "›" when the toolbar is open (click to hide),
        //        "☰" (hamburger) when hidden (click to show).
        Text {
            anchors.centerIn: parent
            text: root.toolbarVisible ? "›" : "☰"
            color: "#cfe3ff"
            font.pixelSize: 16
            font.bold: true
        }
        MouseArea {
            id: toggleMouse
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            onClicked: root.toolbarVisible = !root.toolbarVisible   // toggle show/hide
        }
    }
    // ============================================================
    // COMPASS / DIRECTION INDICATOR — right side, sitting ABOVE the map
    // toolbar with a gap. A simple, fully STATIC north-up compass image
    // (N points up, matching the fixed N/E/S/W range rings). No idle
    // motion and no hover motion — it never spins or scales.
    // ============================================================
    Rectangle {
        id: compass
        z: 200
        width: 70
        height: 70
        radius: width / 2
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: 20         // horizontally centred over the 50px toolbar
        anchors.topMargin: 10
        color: "#16213e"
        opacity: 0.97
        border.width: 1
        border.color: "#2b6e8f"

        // Static compass rose (ring + cardinal ticks + red north needle + "N").
        Canvas {
            id: compassImg
            anchors.fill: parent
            onPaint: {
                var ctx = getContext("2d");
                var w = width, h = height, cx = w / 2, cy = h / 2;
                var R = Math.min(cx, cy) - 5;
                ctx.clearRect(0, 0, w, h);
                // outer ring
                ctx.strokeStyle = "rgba(127,178,217,0.6)";
                ctx.lineWidth = 1.5;
                ctx.beginPath(); ctx.arc(cx, cy, R, 0, 2 * Math.PI); ctx.stroke();
                // cardinal ticks (N/E/S/W)
                for (var a = 0; a < 360; a += 90) {
                    var rad = a * Math.PI / 180, ux = Math.sin(rad), uy = -Math.cos(rad);
                    ctx.beginPath();
                    ctx.moveTo(cx + ux * (R - 6), cy + uy * (R - 6));
                    ctx.lineTo(cx + ux * R, cy + uy * R);
                    ctx.strokeStyle = "#7fb2d9"; ctx.lineWidth = 2; ctx.stroke();
                }
                // needle — red north half (points up), light south half (down)
                var nlen = R - 9, nw = 6;
                ctx.beginPath();
                ctx.moveTo(cx, cy - nlen); ctx.lineTo(cx - nw, cy); ctx.lineTo(cx + nw, cy); ctx.closePath();
                ctx.fillStyle = "#ff5a4d"; ctx.fill();
                ctx.beginPath();
                ctx.moveTo(cx, cy + nlen); ctx.lineTo(cx - nw, cy); ctx.lineTo(cx + nw, cy); ctx.closePath();
                ctx.fillStyle = "#9db4d0"; ctx.fill();
                // hub
                ctx.beginPath(); ctx.arc(cx, cy, 3, 0, 2 * Math.PI);
                ctx.fillStyle = "#cfe3ff"; ctx.fill();
                // "N" marker at the top
                ctx.fillStyle = "#ff5a4d";
                ctx.font = "bold 10px sans-serif";
                ctx.textAlign = "center"; ctx.textBaseline = "middle";
                ctx.fillText("N", cx, cy - R + 6);
            }
        }

    }
    // ============================================================
    // FLIGHT-NAME TOOLTIP — pops in above the aircraft when clicked.
    // Rendered here (root overlay) so it is never clipped by the rings,
    // and it follows the aircraft as it moves / the map pans & zooms.
    // ============================================================
    Item {
        id: flightNameTip
        z: 9998
        visible: opacity > 0.01
        // Aircraft screen position (referencing center/zoom/aircraftPosition
        // forces this to re-evaluate as the map or the aircraft moves).
        property point scr: {
            var c = mapView.center; var zz = mapView.zoomLevel
            var ap = flightController.aircraftPosition
            return (mapView.width > 0 && ap && ap.isValid)
                   ? mapView.fromCoordinate(ap, false) : Qt.point(0, 0)
        }
        x: mapView.x + scr.x - fnBg.width / 2
        y: mapView.y + scr.y - fnBg.height - 42     // float above the aircraft
        // Pop-in / fade animation driven by the click flag
        opacity: root.flightNameShown ? 1.0 : 0.0
        scale: root.flightNameShown ? 1.0 : 0.6
        transformOrigin: Item.Bottom
        Behavior on opacity { NumberAnimation { duration: 150 } }
        Behavior on scale { NumberAnimation { duration: 190; easing.type: Easing.OutBack } }
        Rectangle {
            id: fnBg
            width: fnText.implicitWidth + 16
            height: fnText.implicitHeight + 10
            radius: 6
            color: "#0a1622"
            border.color: "#2ecc71"
            border.width: 1
            opacity: 0.96
            Text {
                id: fnText
                anchors.centerIn: parent
                text: root.flightName
                color: "#2ecc71"
                font.bold: true
                font.pixelSize: 12
                font.family: "Consolas, monospace"
            }
        }
    }

    // ============================================================
    // STATUS OVERLAY (top-left)
    // ============================================================
    Rectangle {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 8
        width: statusMapCol.width + 16
        height: statusMapCol.height + 12
        color: "#1a1a2e"
        opacity: 0.9
        radius: 4
        border.color: "#16213e"
        border.width: 1

        Column {
            id: statusMapCol
            anchors.centerIn: parent
            spacing: 2

            Text {
                text: qsTr("Lat: ") + flightController.aircraftPosition.latitude.toFixed(5) +
                      qsTr("  Lng: ") + flightController.aircraftPosition.longitude.toFixed(5)
                color: "#ecf0f1"
                font.pixelSize: 10
                font.family: "Consolas, monospace"
            }

            Text {
                text: qsTr("HDG: ") + flightController.aircraftBearing.toFixed(1) + "°  |  " +
                      qsTr("Velocity: ") + flightController.calculatedVelocity.toFixed(0) + " km/h"
                color: "#27ae60"
                font.pixelSize: 10
                font.family: "Consolas, monospace"
            }
        }
    }

    // ============================================================
    // ZOOM CONTROLS (bottom-left)
    // ============================================================
    Column {
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.margins: 8
        spacing: 4

        Button {
            width: 32; height: 32
            text: "+"
            font.pixelSize: 14; font.bold: true
            onClicked: mapView.zoomLevel = Math.min(mapView.zoomLevel + 1, mapView.maximumZoomLevel)
        }
        Button {
            width: 32; height: 32
            text: "−"
            font.pixelSize: 14; font.bold: true
            onClicked: mapView.zoomLevel = Math.max(mapView.zoomLevel - 1, mapView.minimumZoomLevel)
        }
    }
    // ============================================================
    // FULL-SCREEN TOGGLE (bottom-right corner) — VLC-style.
    // Clicking it hides ALL surrounding Qt widgets (docks, toolbars,
    // menu bar, status bar) and shows the window full-screen so the map
    // fills the entire screen; clicking again restores the original
    // layout. The actual show/hide is done in C++ (dp_aspj_map::
    // DP_ToggleFullScreen), reached here through the "dpAspjMap" context
    // property. The local "mapFullScreen" flag only drives the icon
    // (expand vs. shrink brackets).
    // ============================================================
    Rectangle {
        id: fsButton
        z: 300
        visible: false
        width: 34
        height: 34
        radius: 6
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.rightMargin: 12
        anchors.bottomMargin: 12
        color: fsMouse.containsMouse ? "#243352" : "#16213e"
        border.color: "#2b6e8f"
        border.width: 1
        opacity: 0.95
        // Tracks the on/off state so the icon can flip; repaint when it changes.
        property bool mapFullScreen: false
        onMapFullScreenChanged: fsIcon.requestPaint()
        // Vector icon: four "expand" corner brackets normally, four "shrink"
        // brackets (pulled toward the centre) while full-screen.
        Canvas {
            id: fsIcon
            anchors.fill: parent
            anchors.margins: 9
            onPaint: {
                var ctx = getContext("2d")
                var w = width, h = height
                ctx.clearRect(0, 0, w, h)
                ctx.strokeStyle = "#cfe3ff"
                ctx.lineWidth = 2
                ctx.lineCap = "round"
                ctx.lineJoin = "round"
                var m = 1, L = w * 0.42          // edge inset and bracket arm length
                if (!fsButton.mapFullScreen) {
                    // EXPAND — corner brackets hugging the four OUTER corners
                    ctx.beginPath(); ctx.moveTo(m, m + L);     ctx.lineTo(m, m);       ctx.lineTo(m + L, m);       ctx.stroke()   // top-left
                    ctx.beginPath(); ctx.moveTo(w - m - L, m); ctx.lineTo(w - m, m);   ctx.lineTo(w - m, m + L);   ctx.stroke()   // top-right
                    ctx.beginPath(); ctx.moveTo(m, h - m - L); ctx.lineTo(m, h - m);   ctx.lineTo(m + L, h - m);   ctx.stroke()   // bottom-left
                    ctx.beginPath(); ctx.moveTo(w - m - L, h - m); ctx.lineTo(w - m, h - m); ctx.lineTo(w - m, h - m - L); ctx.stroke() // bottom-right
                } else {
                    // SHRINK — corner brackets pulled INWARD (arms face the corners)
                    var d = w * 0.32             // inset of each bracket corner from the edge
                    ctx.beginPath(); ctx.moveTo(d, m);     ctx.lineTo(d, d);       ctx.lineTo(m, d);       ctx.stroke()   // top-left
                    ctx.beginPath(); ctx.moveTo(w - d, m); ctx.lineTo(w - d, d);   ctx.lineTo(w - m, d);   ctx.stroke()   // top-right
                    ctx.beginPath(); ctx.moveTo(d, h - m); ctx.lineTo(d, h - d);   ctx.lineTo(m, h - d);   ctx.stroke()   // bottom-left
                    ctx.beginPath(); ctx.moveTo(w - d, h - m); ctx.lineTo(w - d, h - d); ctx.lineTo(w - m, h - d); ctx.stroke() // bottom-right
                }
            }
        }
        MouseArea {
            id: fsMouse
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            onClicked: {
                // Flip the icon, then ask C++ to hide/show the surrounding widgets.
                fsButton.mapFullScreen = !fsButton.mapFullScreen
                if (typeof dpAspjMap !== "undefined" && dpAspjMap)
                    dpAspjMap.DP_ToggleFullScreen()
            }
        }
    }

    // ============================================================
    // EMITTER HOVER TOOLTIP (overlay, positioned over the hovered emitter)
    // ============================================================
    Item {
        id: emitterTip
        visible: root.hoverEmitter !== null
        z: 9999

        property var info: root.hoverEmitter
        property var acPos: flightController.aircraftPosition

        // Recompute screen position when the map pans/zooms (referencing
        // center/zoomLevel forces re-evaluation of fromCoordinate()).
        property point scr: {
            var c = mapView.center; var zz = mapView.zoomLevel;   // deps
            return (info && mapView.width > 0)
                       ? mapView.fromCoordinate(info.coordinate, false)
                       : Qt.point(0, 0)
        }

        x: mapView.x + scr.x - tipBg.width / 2
        y: mapView.y + scr.y - tipBg.height - 26   // float above the icon

        Rectangle {
            id: tipBg
            color: "#0a1622"
            border.color: "#2ecc71"
            border.width: 1
            radius: 4
            opacity: 0.96
            width: tipText.implicitWidth + 16
            height: tipText.implicitHeight + 12

            Text {
                id: tipText
                anchors.centerIn: parent
                color: "#7CFC00"
                font.family: "monospace"
                font.pixelSize: 11
                textFormat: Text.PlainText
                text: emitterTip.info
                    ? ("▸ " + emitterTip.info.name + "\n" +
                       "No        : " + emitterTip.info.no + "\n" +
                       "Latitude  : " + emitterTip.info.lat.toFixed(5) + "°\n" +
                       "Longitude : " + emitterTip.info.lng.toFixed(5) + "°\n" +
                       "Range     : " + emitterTip.info.range.toFixed(0) + " km\n" +
                       "A/C Dist  : " + ((emitterTip.acPos && emitterTip.acPos.isValid)
                            ? (emitterTip.info.coordinate.distanceTo(emitterTip.acPos) / 1000).toFixed(2) : "--") + " km\n" +
                       "A/C Brng  : " + ((emitterTip.acPos && emitterTip.acPos.isValid)
                            ? emitterTip.acPos.azimuthTo(emitterTip.info.coordinate).toFixed(1) : "--") + "°")
                    : ""
            }
        }
    }

    // ============================================================
    // INTRO OVERLAY — radar "tactical boot" screen, plays once on load
    // ============================================================
    Rectangle {
        id: introOverlay
        anchors.fill: parent
        z: 10000
        color: "#04070d"
        opacity: 1.0
        visible: opacity > 0.001          // stops capturing/painting once faded

        // Subtle scope glow background
        Rectangle {
            anchors.fill: parent
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#04070d" }
                GradientStop { position: 1.0; color: "#071322" }
            }
        }

        // --- Radar scope ---
        Item {
            id: scope
            anchors.centerIn: parent
            width: Math.min(parent.width, parent.height) * 0.5
            height: width

            // Concentric range rings
            Repeater {
                model: 4
                Rectangle {
                    anchors.centerIn: parent
                    width: scope.width * (index + 1) / 4
                    height: width
                    radius: width / 2
                    color: "transparent"
                    border.color: "#1f7a3f"
                    border.width: 1
                    opacity: 0.7
                }
            }
            // Cross-hairs
            Rectangle { anchors.centerIn: parent; width: scope.width; height: 1; color: "#1f7a3f"; opacity: 0.5 }
            Rectangle { anchors.centerIn: parent; width: 1; height: scope.height; color: "#1f7a3f"; opacity: 0.5 }

            // Rotating sweep beam
            Canvas {
                id: beam
                anchors.fill: parent
                onPaint: {
                    var ctx = getContext("2d")
                    ctx.clearRect(0, 0, width, height)
                    var cx = width / 2, cy = height / 2, r = width / 2
                    // Trailing wedge
                    ctx.beginPath()
                    ctx.moveTo(cx, cy)
                    ctx.arc(cx, cy, r, -0.6, 0.0)
                    ctx.closePath()
                    var grad = ctx.createRadialGradient(cx, cy, 0, cx, cy, r)
                    grad.addColorStop(0, "rgba(46,204,113,0.45)")
                    grad.addColorStop(1, "rgba(46,204,113,0.0)")
                    ctx.fillStyle = grad
                    ctx.fill()
                    // Leading edge line
                    ctx.strokeStyle = "rgba(46,204,113,0.95)"
                    ctx.lineWidth = 2
                    ctx.beginPath()
                    ctx.moveTo(cx, cy)
                    ctx.lineTo(cx + r, cy)
                    ctx.stroke()
                }
                transformOrigin: Item.Center
                RotationAnimator on rotation {
                    from: 0; to: 360
                    duration: 1500
                    loops: Animation.Infinite
                    running: introOverlay.visible
                }
            }
        }

        // --- Boot text (blinking) ---
        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: scope.bottom
            anchors.topMargin: 28
            text: "INITIALIZING METS DRY PATH SIMULATION MAP"
            color: "#2ecc71"
            font.family: "monospace"
            font.pixelSize: 16
            font.bold: true
            SequentialAnimation on opacity {
                loops: Animation.Infinite
                running: introOverlay.visible
                NumberAnimation { from: 0.35; to: 1.0; duration: 600 }
                NumberAnimation { from: 1.0; to: 0.35; duration: 600 }
            }
        }

        // Play once: hold, then fade away to reveal the (fly-to) map
        SequentialAnimation {
            running: true
            PauseAnimation { duration: 1100 }
            NumberAnimation { target: introOverlay; property: "opacity"; from: 1.0; to: 0.0; duration: 900; easing.type: Easing.InOutQuad }
        }
    }
}
