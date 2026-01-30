import QtQuick
import QtQuick.Controls

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("S-Param Analyzer")

    Column {
        spacing: 20
        anchors.centerIn: parent

        Button {
            text: "Debug menu"
            width: 200
            onClicked: debugController.showDebugMenu()
        }
    }
}
