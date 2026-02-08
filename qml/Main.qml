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
            Layout.topMargin: 20

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
