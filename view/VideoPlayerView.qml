import QtQuick
import QtMultimedia
import "components"
Item {
    id: root
    property alias mediaEngine: player // Allow external access to Media Player if needed
    MediaPlayer {
        id: player
        source: playerViewModel.videoSource ? playerViewModel.videoSource : ""
        videoOutput: videoOutput
        audioOutput: AudioOutput { volume: playerViewModel.volume }

        // SYNCHRONIZATION: Continuously updates progress from MediaPlayer to C++ ViewModel
        onPositionChanged: playerViewModel.position = player.position
        onDurationChanged: playerViewModel.duration = player.duration
    }

    // Connecting the Play/Pause state from the ViewModel controls the Engine in reverse.
    Connections {
        target: playerViewModel
        function onIsPlayingChanged() {
            if (playerViewModel.isPlaying) player.play()
            else player.pause()
        }
        function onPositionChanged() {
            // If the user drags the slider (significantly from the player's current position), we perform a scan.
            if (Math.abs(player.position - playerViewModel.position) > 1000) {
                player.position = playerViewModel.position
            }
        }
    }

    VideoOutput {
        id: videoOutput
        anchors.fill: parent

        MouseArea {
            anchors.fill: parent
            onClicked: {
                playerViewModel.isPlaying = !playerViewModel.isPlaying
                centerIconAnimation.restart()
            }
            onDoubleClicked: {
                // Automatically switch to Fullscreen when you double-click on the video area
                if (mainWindow.visibility === Window.FullScreen) {
                    mainWindow.showNormal()
                } else {
                    mainWindow.showFullScreen()
                }
            }
        }

        // A large icon appears in the middle of the screen when you click
        Rectangle {
            id: centerIconOverlay
            width: 80; height: 80
            radius: 40
            color: "#aa000000"
            anchors.centerIn: parent
            opacity: 0 

            Text {
                anchors.centerIn: parent
                text: playerViewModel.isPlaying ? "||" : "▶" 
                color: "white"
                font.pointSize: 28
            }

            // Fast fade-in/fade-out effect when clicking the mouse, similar to Windows Player
            SequentialAnimation on opacity {
                id: centerIconAnimation
                running: false
                NumberAnimation { to: 1.0; duration: 100 }
                PauseAnimation { duration: 300 }
                NumberAnimation { to: 0.0; duration: 200 }
            }
        }
    }
}
