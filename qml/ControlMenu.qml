import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Vna 1.0

GroupBox {
    title: qsTr("Control")
    implicitWidth: parent.width

    RowLayout {
        Button {
            id: startStopButton

            implicitWidth: 80
            text: measurementController ? measurementController.isRunning ? "Стоп" : "Старт" : "Стоп"

            onClicked: {
                if (measurementController.isRunning)
                    measurementController.stop()
                else {
                    measurementController.start()
                }
            }
        }

        Button {
            text: "Debug vna"
            implicitWidth: 80
            onClicked: debugController.showDebugMenu()
        }
    }
}
