import QtQuick
import QtQuick.Controls
import QtCharts
import QtQuick.Layouts

GroupBox {
    title: qsTr("Measurements settings")
    implicitWidth: parent.width

    readonly property real mhz: 1e6

    property var _measurement: measurementController ? measurementController.measurement : null

    ColumnLayout {
        spacing: 8
        width: parent.width

        Label {
            text: qsTr("Frequency MHz")
            Layout.alignment: Qt.AlignLeft
        }

        RowLayout {
            Layout.preferredWidth: parent.width


            Label {
                text: qsTr("From")
            }
            TextField {
                id: startFrequency

                property real displayStartFreq: _measurement ? _measurement.startFreq / mhz : 0

                text: displayStartFreq.toLocaleString(Qt.locale(), 'f', 2)
                validator: DoubleValidator {
                    bottom: 0
                    top: 1000000
                    decimals: 6
                    notation: DoubleValidator.StandardNotation
                }
                onEditingFinished: {
                    var value = parseFloat(text.replace(",", "."))
                    if (!isNaN(value) && value >= 0) {
                        _measurement.startFreq = value * mhz
                    } else {
                        text = displayStartFreq.toLocaleString(Qt.locale(), 'f', 2)
                    }
                }
            }

            Label {
                text: qsTr("To")
            }
            TextField {
                id: endFrequency

                property real displayStopFreq: _measurement ? _measurement.stopFreq / mhz : 0

                text: displayStopFreq.toLocaleString(Qt.locale(), 'f', 2)
                validator: DoubleValidator {
                    bottom: 0
                    top: 1000000
                    decimals: 6
                    notation: DoubleValidator.StandardNotation
                }
                onEditingFinished: {
                    var value = parseFloat(text.replace(",", "."))
                    if (!isNaN(value) && value >= 0) {
                        _measurement.stopFreq = value * mhz
                    } else {
                        text = displayStopFreq.toLocaleString(Qt.locale(), 'f', 2)
                    }
                }
            }
        }

        RowLayout {
            Layout.preferredWidth: parent.width

            Label {
                text: qsTr("Count points")
            }
            TextField {
                id: countPoints
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
        }

        RowLayout {
            Layout.preferredWidth: parent.width

            Label {
                text: qsTr("Power")
            }
            TextField {
                id: power
                text: _measurement ? _measurement.outputPower.toString() : "0"
                validator: DoubleValidator {
                    bottom: -50;
                    top: 20;
                    decimals: 2
                }
                onEditingFinished: {
                    var value = parseFloat(text.replace(",", "."))
                    if (!isNaN(value)) {
                        _measurement.outputPower = value
                    } else {
                        text = _measurement ? _measurement.outputPower.toString() : "0"
                    }
                }
            }
        }

        RowLayout {
            Layout.preferredWidth: parent.width

            Label {
                text: qsTr("Filter PCH")
            }
            TextField {
                id: pch
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
        }

        RowLayout {
            Layout.preferredWidth: parent.width

            Label {
                text: qsTr("Trace format")
            }
            ComboBox {
                id: format

                function updateIndexFromModel() {
                    if (!_measurement || !_measurement.format) return

                    for (var i = 0; i < model.length; i++) {
                        if (model[i].value === _measurement.format) {
                            currentIndex = i
                            break
                        }
                    }
                }

                model: [
                    { text: "Амп", value: "MLOG" },
                    { text: "Фаза", value: "PHAS" }
                ]
                textRole: "text"
                implicitWidth: 200

                onCurrentIndexChanged: {
                    if (currentIndex >= 0 && _measurement) {
                        _measurement.format = model[currentIndex].value
                    }
                }

                Component.onCompleted: {
                    updateIndexFromModel()
                }
            }
        }
    }
}
