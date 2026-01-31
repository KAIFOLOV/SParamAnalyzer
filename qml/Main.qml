import QtQuick
import QtQuick.Controls
import QtCharts
import QtQuick.Layouts

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("S-Param Analyzer")

    property var dataPoints: []

    ColumnLayout {
        spacing: 20
        anchors.centerIn: parent

        Button {
            text: "Debug menu"
            width: 200
            onClicked: debugController.showDebugMenu()
        }

        RowLayout {
            spacing: 10
            Button {
                text: "Очистить график"
                onClicked: {
                    dataPoints = []
                    lineSeries.clear()
                }
            }
            Button {
                text: "Добавить случайную точку"
                onClicked: {
                    var x = dataPoints.length
                    var y = Math.random()
                    dataPoints.push({x: x, y: y})
                    lineSeries.append(x, y)
                }
            }
        }

        ChartView {
            id: chart
            antialiasing: true
            width: 500
            height: 500

            LineSeries {
                id: lineSeries
                name: "S-parameter"
            }
        }
    }
}
