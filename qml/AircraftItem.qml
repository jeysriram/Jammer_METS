import QtQuick 2.14
import QtLocation 5.6

/*
  Su-30 Aircraft Symbol Component with Bearing Rotation
  Size: 64x64 with drop shadow for visibility above map lines
*/
Item {
    id: root
    width: 44
    height: 44

    property bool imageError: false

    property string pilotName: ""

    property real bearing: 0
    rotation: bearing

    // Drop shadow to make aircraft pop above lines
    Rectangle {
        anchors.centerIn: parent
        width: parent.width + 8
        height: parent.height + 8
        color: "transparent" //"#84C7FF"
        radius: 5
        visible: aircraftImage.status !== Image.Error

        Rectangle {
            id: bubble
            color: "transparent" //"#84C7FF"
            border.width: .4
            width: text.width * 1.3
            height: text.height * 1.3
            radius: 5
            Text {
                id: text
                anchors.centerIn: parent
                text: pilotName
            }
        }
    }

    Image {
        id: aircraftImage
        anchors.fill: parent
        anchors.margins: 2
        source: "qrc:/icons/Fighter.png"
        fillMode: Image.PreserveAspectFit
        smooth: true
        visible: status !== Image.Error
    }

    Canvas {
        anchors.fill: parent
        visible: aircraftImage.status === Image.Error

        onPaint: {
            var ctx = getContext("2d");
            var w = width;
            var h = height;

            ctx.strokeStyle = "#00ff00";
            ctx.lineWidth = 2;
            ctx.beginPath();
            ctx.moveTo(w / 2, 4);
            ctx.lineTo(w - 8, h - 6);
            ctx.lineTo(w / 2, h - 14);
            ctx.lineTo(8, h - 6);
            ctx.closePath();
            ctx.stroke();
        }
    }

}



