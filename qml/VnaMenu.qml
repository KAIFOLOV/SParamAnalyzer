import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Vna 1.0

GroupBox {
    title: qsTr("Vna Settings")
    implicitWidth: parent.width

    readonly property int labelWidth: 120
    readonly property int fieldWidth: 80

    ColumnLayout {
        spacing: 12
        width: parent.width

        RowLayout {
            Layout.preferredWidth: parent.width

            Label {
                text: "IP:"
                width: labelWidth
                Layout.alignment: Qt.AlignLeft
            }

            RowLayout {
                Layout.alignment: Qt.AlignRight

                TextField {
                    id: ipField

                    text: vna ? vna.ip : ""
                    implicitWidth: fieldWidth
                }
            }

            Label {
                text: qsTr("Port:")
                width: labelWidth
                Layout.alignment: Qt.AlignLeft
            }

            RowLayout {
                Layout.alignment: Qt.AlignRight

                TextField {
                    id: portField

                    text: vna ? vna.port : 0
                    implicitWidth: fieldWidth
                    inputMethodHints: Qt.ImhDigitsOnly
                }
            }
        }

        RowLayout {
            spacing: 20
            Layout.alignment: Qt.AlignLeft

            Button {
                implicitWidth: 100
                text: vna && vna.connectStatus ? "Disconnect" : "Connect"
                onClicked: {
                    if (!vna) return
                    if (vna.isOpen()) {
                        vna.disconnect()
                    } else {
                        vna._ip = ipField.text
                        vna._port = parseInt(portField.text)
                        vna.connect()
                    }
                }
            }

            Rectangle {
                id: statusIndicator
                width: 20
                height: 20
                radius: width / 2
                color: "gray"
                border.color: "black"
                border.width: 1
            }
        }
    }

    Connections {
        target: vna
        function onConnectStatusChanged(status) {
            switch(status) {
            case IVna.Opened: statusIndicator.color = "green"; break;
            case IVna.Closed: statusIndicator.color = "red"; break;
            case IVna.Wait:   statusIndicator.color = "yellow"; break;
            default:          statusIndicator.color = "gray"; break;
            }
        }
    }
}
