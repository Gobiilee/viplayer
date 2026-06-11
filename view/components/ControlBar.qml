
import QtQuick
import QtQuick.Controls

Rectangle {
    id: root
    height: 90
    color: "#e60a0a0a" 
    border.color: "#1affffff" 
    border.width: 1

    Column {
        anchors.fill: parent
        anchors.topMargin: 5

        // The time slider occupies the entire top width
        Slider {
            id: progressSlider
            width: parent.width - 40
            anchors.horizontalCenter: parent.horizontalCenter
            from: 0
            to: playerViewModel.duration
            value: playerViewModel.position
            onMoved: playerViewModel.position = value

            background: Rectangle {
                height: 4
                radius: 2
                color: "#33ffffff"
                Rectangle {
                    width: progressSlider.visualPosition * parent.width
                    height: parent.height
                    color: "#0078d4"
                    radius: 2
                }
            }
            handle: Rectangle {
                x: progressSlider.visualPosition * (progressSlider.width - width)
                y: (progressSlider.availableHeight) / 2
                width: 10
                height: width / 2
                radius: width / 2
                color: "white"
                opacity: progressSlider.hovered ? 1.0 : 0.0
                Behavior on width { NumberAnimation { duration: 150 } }
            }
        }

        // Area of ​​controls below the Timeline bar
        Item {
            width: parent.width - 40
            height: 50
            anchors.horizontalCenter: parent.horizontalCenter

            Row {
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                spacing: 5
                Text { text: playerViewModel.formatTime(playerViewModel.position); color: "white"; font.pointSize: 10 }
                Text { text: "/"; color: "#66ffffff"; font.pointSize: 10 }
                Text { text: playerViewModel.formatTime(playerViewModel.duration); color: "#99ffffff"; font.pointSize: 10 }
            }

            Row {
                anchors.centerIn: parent
                spacing: 15

                IconButton {
                    iconText: "<<";
                    onClicked: {
                        playerViewModel.isPlaying = false
                        playerViewModel.previousVideo()
                    }
                }
                IconButton { iconText: "↺ 10"; onClicked: playerViewModel.seekBackward() }

                IconButton {
                    id: mainPlayBtn
                    width: 48; height: 48
                    backgroundColor: playerViewModel.isPlaying ? "#e60a0a0a" : "#44ffffff"
                    iconText: playerViewModel.isPlaying ? "||" : "▶"
                    onClicked: playerViewModel.isPlaying = !playerViewModel.isPlaying
                }

                IconButton { iconText: "10 ↻"; onClicked: playerViewModel.seekForward() }
                IconButton {
                    iconText: ">>";
                    onClicked: {
                        playerViewModel.isPlaying = false
                        playerViewModel.nextVideo()

                    }
                }
            }

            // Right: Quick access widget, Volume & Fullscreen
            Row {
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                spacing: 10

                IconButton {
                    iconText: "📄"
                    onClicked: {
                        playerViewModel.isPlaying = false
                        globalFileDialog.open()
                    }
                }

                IconButton {
                    iconText: "📁"
                    onClicked: {
                        playerViewModel.isPlaying = false
                        globalFolderDialog.open()
                    }
                }

                Rectangle {
                    width: 1; height: 20
                    color: "#33ffffff"
                    anchors.verticalCenter: parent.verticalCenter
                }

                // Your current volume buttons and sliders
                IconButton {
                    iconText: playerViewModel.volume === 0 ? "🔇" : "🔊"
                    onClicked: playerViewModel.volume = playerViewModel.volume === 0 ? 1.0 : 0.0
                }

                Slider {
                    id: volumeSlider
                    width: 80
                    from: 0.0
                    to: 1.0
                    value: playerViewModel.volume
                    onMoved: playerViewModel.volume = value
                    anchors.verticalCenter: parent.verticalCenter
                }

                IconButton {
                    iconText: mainWindow.visibility === Window.FullScreen ? "🗗" : "⛶"
                    onClicked: {
                        if (mainWindow.visibility === Window.FullScreen) mainWindow.showNormal()
                        else mainWindow.showFullScreen()
                    }
                }
            }
        }
    }
}
