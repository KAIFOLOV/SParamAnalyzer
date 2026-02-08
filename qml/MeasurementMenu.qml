import QtQuick
import QtQuick.Controls
import QtCharts
import QtQuick.Layouts

GroupBox {
    title: qsTr("Measurements Settings")
    implicitWidth: parent.width

    readonly property real mhz: 1e6
    property var _measurement: measurementController ? measurementController.measurement : null

    readonly property int labelWidth: 120
    readonly property int fieldWidth: 60

    ColumnLayout {
        spacing: 12
        width: parent.width

        Label {
            text: qsTr("Frequency (MHz)")
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 16

            RowLayout {
                spacing: 8
                Layout.alignment: Qt.AlignLeft

                Label {
                    text: qsTr("From:")
                    width: labelWidth
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                }

                TextField {
                    id: startFrequency

                    implicitWidth: fieldWidth
                    text: displayText

                    property string displayText: (_measurement ? (_measurement.startFreq / mhz).toFixed(0) : "0.00")

                    validator: DoubleValidator {
                        bottom: 0;
                        top: 1e6;
                    }

                    onEditingFinished: {
                        var value = parseFloat(text)
                        if (!isNaN(value) && value >= 0) {
                            _measurement.startFreq = value * mhz
                            displayText = value.toFixed(0)
                        }
                    }
                }
            }

            RowLayout {
                spacing: 8
                Layout.alignment: Qt.AlignLeft

                Label {
                    text: qsTr("To:")
                    width: labelWidth
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                }

                TextField {
                    id: endFrequency

                    implicitWidth: fieldWidth
                    text: displayText

                    property string displayText: (_measurement ? (_measurement.stopFreq / mhz).toFixed(0) : "0.00")

                    validator: DoubleValidator {
                        bottom: 0;
                        top: 1e6;
                    }

                    onEditingFinished: {
                        var value = parseFloat(text)
                        if (!isNaN(value) && value >= 0) {
                            _measurement.stopFreq = value * mhz
                            displayText = value.toFixed(0)
                        }
                    }
                }
            }
        }


        Rectangle {
            Layout.preferredHeight: 1
            Layout.fillWidth: true
            color: "#e0e0e0"
            Layout.topMargin: 4
            Layout.bottomMargin: 4
        }


        RowLayout {
            Layout.preferredWidth: parent.width

            Label {
                text: qsTr("Points count:")
                width: labelWidth
                Layout.alignment: Qt.AlignLeft
            }

            RowLayout {
                Layout.alignment: Qt.AlignRight

                TextField {
                    id: countPoints

                    implicitWidth: fieldWidth
                    text: _measurement ? _measurement.pointsCount.toString() : "0"
                    validator: IntValidator { bottom: 1; top: 100000 }
                    onEditingFinished: {
                        var value = parseInt(text)
                        if (!isNaN(value) && value >= 1) {
                            _measurement.pointsCount = value
                        } else {
                            text = _measurement ? _measurement.pointsCount.toString() : "0"
                        }
                    }
                }

                Label {
                    Layout.preferredWidth: 30
                    text: qsTr("")
                }
            }
        }

        RowLayout {
            Layout.preferredWidth: parent.width

            Label {
                text: qsTr("Output power:")
                width: labelWidth
                Layout.alignment: Qt.AlignLeft
            }

            RowLayout {
                Layout.alignment: Qt.AlignRight

                TextField {
                    id: power

                    implicitWidth: fieldWidth
                    text: _measurement ? _measurement.outputPower.toString() : "0"
                    validator: DoubleValidator { bottom: -50; top: 20; decimals: 2 }
                    onEditingFinished: {
                        var value = parseFloat(text.replace(",", "."))
                        if (!isNaN(value)) {
                            _measurement.outputPower = value
                        } else {
                            text = _measurement ? _measurement.outputPower.toString() : "0"
                        }
                    }
                }
                Label {
                    Layout.preferredWidth: 30
                    text: qsTr("dBm")
                }
            }
        }

        RowLayout {
            Layout.preferredWidth: parent.width

            Label {
                text: qsTr("Filter PCH:")
                width: labelWidth
                Layout.alignment: Qt.AlignLeft
            }

            RowLayout {
                Layout.alignment: Qt.AlignRight

                TextField {
                    id: pch

                    implicitWidth: fieldWidth
                    text: _measurement ? _measurement.filterPch.toString() : "0"
                    validator: DoubleValidator { bottom: 10; top: 100000; decimals: 1 }
                    onEditingFinished: {
                        var value = parseFloat(text.replace(",", "."))
                        if (!isNaN(value) && value >= 10) {
                            _measurement.filterPch = value
                        } else {
                            text = _measurement ? _measurement.filterPch.toString() : "0"
                        }
                    }
                }
                Label {
                    Layout.preferredWidth: 30
                    text: qsTr("Hz")
                }
            }
        }

        RowLayout {
            Layout.preferredWidth: parent.width

            Label {
                text: qsTr("Trace format:")
                width: labelWidth
                Layout.alignment: Qt.AlignLeft
            }

            RowLayout {
                Layout.alignment: Qt.AlignRight

                ComboBox {
                    id: formatComboBox

                    implicitWidth: fieldWidth
                    model: [
                        { text: "Amp", value: "MLOG" },
                        { text: "Pha", value: "PHAS" }
                    ]
                    textRole: "text"

                    Component.onCompleted: updateIndexFromModel()

                    function updateIndexFromModel() {
                        if (!_measurement || !_measurement.format) return
                        for (var i = 0; i < model.length; i++) {
                            if (model[i].value === _measurement.format) {
                                currentIndex = i
                                break
                            }
                        }
                    }

                    onCurrentIndexChanged: {
                        if (currentIndex >= 0 && _measurement) {
                            _measurement.format = model[currentIndex].value
                        }
                    }
                }
                Label {
                    Layout.preferredWidth: 30
                    text: qsTr("")
                }
            }
        }
    }
}
