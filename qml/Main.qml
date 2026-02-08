import QtQuick
import QtQuick.Controls
import QtCharts
import QtQuick.Layouts

Window {
    width: 1200
    height: 500
    visible: true
    title: qsTr("S-Param Analyzer")

    RowLayout {
        ColumnLayout {
            width: 250
            spacing: 20
            Layout.leftMargin: 20

            VnaMenu {}
            MeasurementMenu {}
            ControlMenu {}
        }

        ColumnLayout {
            Plotter {
                width: 900
                height: 500
            }
        }
    }
}
