import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import kafkaui 1.0
import "../style.js" as Style
import "../constants.js" as Constants

Window {
    id: window

    property string topic: ""
    property var topicModel
    property var broker

    visible: true
    width: Constants.ConsumerScreenWidth
    height: Constants.ConsumerScreenHeight
    title: qsTr("Consume from Topic: ") + window.topic
    color: Style.Background

    Consumer {
        id: consumer

        topic: window.topic
        broker: window.broker
        keyType: format.keyTypeId
        valueType: format.valueTypeId
        filters: filter.selectedFilter
        startFromTimeBased: filter.startFromTimeBase
        specificDate: filter.specificDateText
        keyFilter: filter.keyFilter
        valueFilter: filter.valueFilter
        headerKeyFilter: filter.headerKeyFilter
        headerValueFilter: filter.headerValueFilter
        limit: filter.selectedLimit
        numberOfRecords: filter.numberOfRecords
        maxSize: filter.maxSize
        limitSpecificDate: filter.limitSpecificDateText
    }

    RowLayout {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            id: leftPanel

            Layout.fillHeight: true
            Layout.minimumWidth: 330
            Layout.maximumWidth: 330
            border.color: Style.BorderColor

            ComboBox {
                id: topicBox

                width: parent.width
                height: 30
                model: window.topicModel
                Component.onCompleted: currentIndex = indexOfValue(window.topic)

                background: Rectangle {
                    implicitWidth: 330
                    implicitHeight: 30
                    border.color: Style.BorderColor
                }

            }

            ColumnLayout {
                anchors.top: topicBox.bottom
                anchors.bottom: leftPanel.bottom
                anchors.left: leftPanel.left
                anchors.leftMargin: 6

                FormatSelector {
                    id: format

                    Layout.fillWidth: true
                }

                RangeAndFilters {
                    id: filter

                    Layout.fillWidth: true
                }

                Item {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }

            }

        }

        ColumnLayout {
            spacing: 0
            Layout.fillHeight: true
            Layout.fillWidth: true

            MessageTableView {
                id: table

                width: 627
                height: Constants.ConsumerScreenHeight / 2
                Layout.fillHeight: true
                Layout.fillWidth: true
                model: consumer.messages
                visibleHeader: tableBtn.checked
            }

            Item {
                Layout.preferredHeight: 40
                Layout.maximumHeight: 40
                Layout.fillWidth: true

                RowLayout {
                    spacing: 6
                    anchors.fill: parent
                    anchors.leftMargin: 6
                    anchors.rightMargin: 6

                    Button {
                        icon.source: leftPanel.visible ? "qrc:/left.svg" : "qrc:/right.svg"
                        implicitWidth: 28
                        implicitHeight: 28
                        onClicked: {
                            leftPanel.visible = !leftPanel.visible;
                        }
                    }

                    Button {
                        id: tableBtn

                        icon.source: "qrc:/table.svg"
                        implicitWidth: 28
                        implicitHeight: 28
                        checkable: true
                    }

                    Button {
                        visible: tableBtn.checked
                        icon.source: "qrc:/list.svg"
                        implicitWidth: 28
                        implicitHeight: 28
                        onClicked: columnPopup.open()

                        Popup {
                            id: columnPopup

                            y: -implicitHeight + parent.height / 2
                            width: 150
                            focus: true
                            modal: true

                            Column {
                                spacing: 2

                                Repeater {
                                    model: ["Topic", "Partition", "Offset", "Timestamp", "Key", "Value", "Headers"]

                                    CheckBox {
                                        checked: true
                                        text: modelData
                                        onClicked: {
                                            table.hideColumn(index, checked);
                                        }
                                    }

                                }

                            }

                            background: Rectangle {
                                border.color: Style.BorderColor
                                radius: 4
                            }

                        }

                    }

                    Item {
                        Layout.fillWidth: true
                    }

                    Button {
                        id: start

                        text: qsTr("START")
                        state: consumer.isRunning ? "stop" : "start"
                        onClicked: {
                            if (state === "start")
                                consumer.start();
                            else
                                consumer.stop();
                        }
                        states: [
                            State {
                                name: "start"

                                PropertyChanges {
                                    target: start
                                    text: qsTr("START")
                                }

                            },
                            State {
                                name: "stop"

                                PropertyChanges {
                                    target: start
                                    text: qsTr("STOP")
                                }

                            }
                        ]
                    }

                }

            }

        }

    }

}
