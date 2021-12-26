import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts
import plumber
import "../style.js" as Style
import "../constants.js" as Constants
import "../Components" as Components

Window {
    //color: Style.Background
    id: window

    property var topicModel
    property var broker

    visible: true
    width: Constants.ProducerScreenWidth
    height: Constants.ProducerScreenHeight
    title: qsTr("Produce to Topic: ") + dataPage.selectedTopic

    HidePrivateTopicModel {
        id: filteredModel

        hide: true
        sourceModel: window.topicModel
    }

    Producer {
        id: producer

        keyType: dataPage.keyTypeId
        valueType: dataPage.valueTypeId
        topic: dataPage.selectedTopic
        broker: window.broker
        protoKey: dataPage.protoKey
        protoValue: dataPage.protoValue

        options: ProducerOptions {
            compression: optionsPage.compression
            ack: optionsPage.ack
            idempotence: optionsPage.idempotence
        }

    }

    SplitView {
        anchors.fill: parent

        Rectangle {
            visible: false
            SplitView.preferredWidth: 200
            SplitView.maximumWidth: 400
            SplitView.fillHeight: true
        }

        Item {
            SplitView.fillHeight: true
            SplitView.fillWidth: true

            GridLayout {
                anchors.fill: parent
                columns: 2
                columnSpacing: 0
                rowSpacing: 0

                Item {
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    Components.TabBar {
                        id: bar

                        anchors.top: parent.top
                        width: parent.width

                        Components.TabButton {
                            text: qsTr("Data")
                        }

                        Components.TabButton {
                            text: qsTr("Options")

                            Rectangle {
                                height: parent.height
                                x: -1
                                width: 1
                                color: Style.BorderColor
                            }

                        }

                    }

                    StackLayout {
                        width: parent.width
                        anchors.top: bar.bottom
                        anchors.bottom: parent.bottom
                        currentIndex: bar.currentIndex

                        DataPage {
                            id: dataPage

                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            topicModel: filteredModel
                        }

                        OptionsPage {
                            id: optionsPage

                            Layout.fillHeight: true
                            Layout.fillWidth: true
                        }

                    }

                }

                Rectangle {
                    width: 250
                    height: 100
                    Layout.fillHeight: true
                    border.color: Style.BorderColor
                    border.width: 1
                    color: producer.log.isEmpty ? Style.LightGray : "white"

                    ListView {
                        anchors.fill: parent
                        anchors.margins: 1
                        model: producer.log
                        clip: true
                        boundsMovement: Flickable.StopAtBounds

                        delegate: Rectangle {
                            width: parent.width
                            height: 68
                            color: hover.containsMouse ? Style.LightGray : "white"

                            ColumnLayout {
                                anchors.fill: parent
                                anchors.margins: 2

                                Text {
                                    text: timestamp
                                }

                                Text {
                                    text: "> " + topic + "-" + partition
                                }

                                Text {
                                    text: "Offset: " + offset
                                }

                                Item {
                                    Layout.fillHeight: true
                                }

                            }

                            MouseArea {
                                id: hover

                                anchors.fill: parent
                                hoverEnabled: true
                            }

                        }

                    }

                }

                Item {
                    Layout.columnSpan: 2
                    Layout.fillWidth: true
                    Layout.preferredHeight: 34

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 4

                        Item {
                            Layout.fillWidth: true
                        }

                        Components.Button {
                            text: qsTr("PRODUCE TO TOPIC")
                            onClicked: {
                                let e = producer.send(dataPage.keyData(), dataPage.valueData());
                                if (e.isError) {
                                    err.show(e);
                                    return ;
                                }
                            }
                        }

                    }

                }

            }

        }

        handle: Rectangle {
            implicitWidth: 1
            implicitHeight: 2
            color: Style.BorderColor
        }

    }

    Components.ErrorDialog {
        id: err

        anchors.centerIn: parent
        width: parent.width * 0.8
    }

}
