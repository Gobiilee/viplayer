import QtQuick
import QtQuick.Controls

Item {
    id: root
    width: 40
    height: 40
    property alias iconText: innerText.text
    property alias backgroundColor: bg.color
    signal clicked()

    Rectangle {
        id: bg
        anchors.fill: parent
        radius: width / 2
        color: mouseArea.containsMouse ? "#33ffffff" : "transparent"
        Text {
            id: innerText
            anchors.centerIn: parent
            color: "white"
            font.pointSize: 14
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: root.clicked()
    }
}
