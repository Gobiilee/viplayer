import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import VideoPlayer.Core 1.0 // Import the ViewModel layer from C++
import "view/components"
import "view"

Window {
    id: mainWindow
    width: 950
    height: 650
    visible: true
    title: "ViPlayer - Windows Style Edition"
    color: "black"

    PlayerViewModel {
        id: playerViewModel
    }

    VideoPlayerView {
        id: videoView
        anchors.fill: parent
    }
    Button {
        text: "Select the Playlist."
        anchors.centerIn: parent
        visible: !playerViewModel.hasPlaylist

        background: Rectangle {
            implicitWidth: 220
            implicitHeight: 50
            color: "#0078d4"
            radius: 4
        }
        contentItem: Text {
            text: parent.text
            color: "white"
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
        onClicked: globalFileDialog.open()
    }

    MouseArea {
        id: mouseAreaTop
        anchors.top: parent.top
        anchors.left: parent.left
        width: 150; height: 80
        hoverEnabled: true
    }

    // Automatically hide the Control Bar when in fullscreen mode and the user is not moving the mouse.
    ControlBar {
        id: bottomControlBar
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        z: 3

        // Auto show/hide status
        visible: playerViewModel.hasPlaylist
    }

    FileDialog {
        id: globalFileDialog
        title: "Select video files"
        fileMode: FileDialog.OpenFiles
        nameFilters: ["Video files (*.mp4 *.mkv *.avi)"]
        currentFolder: playerViewModel.currentFolderUrl
        onAccepted: playerViewModel.openMultipleFiles(globalFileDialog.selectedFiles)
    }

    FolderDialog {
        id: globalFolderDialog
        title: "Select video folder"
        currentFolder: playerViewModel.currentFolderUrl
        onAccepted: playerViewModel.openFolder(globalFolderDialog.selectedFolder)
    }
}
