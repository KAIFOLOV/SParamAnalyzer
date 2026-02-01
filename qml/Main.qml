import QtQuick
import QtQuick.Controls
import QtCharts
import QtQuick.Layouts

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("S-Param Analyzer")

    // property QtObject _curveModel

    ColumnLayout {
        spacing: 20
        anchors.centerIn: parent

        Button {
            text: "Debug menu"
            width: 200
            onClicked: debugController.showDebugMenu()
        }

        ChartView {
            id: chart
            width: 500
            height: 300

            ValueAxis {
                id: axisX
                min: 0
                max: 20
            }
            ValueAxis {
                id: axisY
                min: 0
                max: 10
            }

            Repeater {
                model: curveModel
                delegate: Item {
                    id: root
                    property var series: null

                    Component.onCompleted: {
                        series = chart.createSeries(
                            ChartView.SeriesTypeLine,
                            model.curve.name,
                            axisX,
                            axisY
                        )
                        updateSeries()
                    }

                    Component.onDestruction: {
                        if (series) {
                            chart.removeSeries(series)
                            series = null
                        }
                    }

                    Connections {
                        target: model.curve
                        function onDataChanged() {
                            updateSeries()
                        }
                    }

                    function updateSeries() {
                        if (!series) return
                        series.clear()
                        const pts = model.curve.data
                        if (!pts) return
                        for (let p of pts)
                            series.append(p.x, p.y)
                    }
                }
            }
        }
    }
}
