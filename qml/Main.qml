import QtQuick
import QtQuick.Controls
import QtCharts
import QtQuick.Layouts

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("S-Param Analyzer")

    RowLayout {
        ColumnLayout {
            width: 400
            spacing: 20

            MeasurementMenu {}

            Button {
                text: "Debug menu"
                width: 200
                onClicked: debugController.showDebugMenu()
            }

            Button {
                id: startStopButton
                text: measurementController.isRunning ? "Стоп" : "Старт"

                onClicked: {
                    if (measurementController.isRunning)
                        measurementController.stop()
                    else {
                        measurementController.start()
                    }
                }
            }
        }
        ColumnLayout {
            Plotter {}
        }
    }
}
