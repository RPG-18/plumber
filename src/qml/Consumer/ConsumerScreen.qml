import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import plumber 1.0
import "../style.js" as Style
import "../constants.js" as Constants
import "../Components" as Components

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
    Component.onCompleted: {
        consumer.topics = topicModel.selectedTopics();
    }

    Consumer {
        id: consumer
        topic: window.topic
        broker: window.broker
        beginning: ConsumerBeginningSelector {
            startFrom: filter.startFromTimeBase
            specificDate: filter.specificDateText
        }
        types: ConsumerTypeSelector {
            keyType: format.keyTypeId
            valueType: format.valueTypeId
        }
        limiter: ConsumerLimiterSelector {
            limit: filter.selectedLimit
            numberOfRecords: filter.numberOfRecords
            maxSize: filter.maxSize
            specificDate: filter.limitSpecificDateText
        }
        filter: ConsumerFilterSelector {
            filters: filter.selectedFilter
            key: filter.keyFilter
            value: filter.valueFilter
            headerKey: filter.headerKeyFilter
            headerValue: filter.headerValueFilter
        }
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

            ColumnLayout {
                anchors.fill: parent

                ComboBox {
                    id: topicBox
                    textRole: "topic"
                    valueRole: "topic"
                    Layout.fillWidth: true
                    height: 30
                    model: topicModel
                    onActivated: window.topic = currentValue
                    visible: window.topicModel.selected === 0
                    Component.onCompleted: {
                        if (window.topic !== "")
                            currentIndex = indexOfValue(window.topic);
                    }
                    background: Rectangle {
                        implicitWidth: 330
                        implicitHeight: 30
                        border.color: Style.BorderColor
                    }
                }
                FormatSelector {
                    id: format
                    Layout.fillWidth: true
                    Layout.leftMargin: 6
                }
                RangeAndFilters {
                    id: filter
                    Layout.fillWidth: true
                    Layout.leftMargin: 6
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
                    Text {
                        text: consumer.stat
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
    Components.ErrorDialog {
        id: consumerErrDialog
        anchors.centerIn: parent
        width: parent.width * 0.8
    }
}
