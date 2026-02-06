import QtQuick
import QtQuick.Controls
import QtCharts
import QtQuick.Layouts

Window {
    width: 1280
    height: 720
    visible: true
    title: qsTr("S-Param Analyzer")

    RowLayout {
        ColumnLayout {
            width: 250
            spacing: 20
            Layout.leftMargin: 20

            MeasurementMenu {}

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

            Button {
                text: "Debug menu"
                width: 200
                onClicked: debugController.showDebugMenu()
            }
        }

        ColumnLayout {
            Plotter {}
        }
    }
}
