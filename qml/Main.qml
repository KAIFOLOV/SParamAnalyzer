import QtQuick
import QtQuick.Controls
import QtCharts
import QtQuick.Layouts

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("S-Param Analyzer")

    ColumnLayout {
        spacing: 20
        anchors.centerIn: parent

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

        ChartView {
            id: chart
            width: 500
            height: 300
            antialiasing: true

            ValueAxis {
                id: axisX
                min: 0
                max: 1
                titleText: "Hz"
            }

            ValueAxis {
                id: axisY
                min: 0
                max: 1
                titleText: "Db"
            }

            // --- Пересчёт диапазонов осей ---
            function recalcAxes() {
                let minX = Number.POSITIVE_INFINITY
                let maxX = Number.NEGATIVE_INFINITY
                let minY = Number.POSITIVE_INFINITY
                let maxY = Number.NEGATIVE_INFINITY

                for (let i = 0; i < chart.count; ++i) {
                    let s = chart.series(i)
                    if (!s) continue

                    for (let k = 0; k < s.count; ++k) {
                        let p = s.at(k)
                        minX = Math.min(minX, p.x)
                        maxX = Math.max(maxX, p.x)
                        minY = Math.min(minY, p.y)
                        maxY = Math.max(maxY, p.y)
                    }
                }

                if (minX === Number.POSITIVE_INFINITY) {
                    axisX.min = 0
                    axisX.max = 1
                    axisY.min = 0
                    axisY.max = 1
                    return
                }

                axisX.min = minX
                axisX.max = maxX
                axisY.min = minY
                axisY.max = maxY
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
                                        axisX, axisY)

                        updateSeries()
                        chart.recalcAxes()
                    }

                    Component.onDestruction: {
                        if (series) {
                            chart.removeSeries(series)
                        }
                        chart.recalcAxes()
                    }

                    Connections {
                        target: model.curve
                        function onDataChanged() {
                            updateSeries()
                            chart.recalcAxes()
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
