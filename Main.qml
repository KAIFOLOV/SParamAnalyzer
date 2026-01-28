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
            text: "Connect"
            width: 200
            onClicked: vna.connect()
        }

        Button {
            text: "Disconnect"
            width: 200
            onClicked: vna.disconnect()
        }

        Button {
            text: "IDN?"
            width: 200
            onClicked: vna.sendCommand("*IDN?")
        }

        Button {
            text: "CALCulate?"
            width: 200
            onClicked: vna.sendCommand("CALC:DATA:FDAT?")
        }

        Button {
            text: "Format"
            width: 200
            onClicked: vna.sendCommand("CALCulate1:TRACe1:FORMat PHASe")
        }

        Button {
            text: "System error?"
            width: 200
            onClicked: vna.sendCommand("SYST:ERR?")
        }
    }
}
