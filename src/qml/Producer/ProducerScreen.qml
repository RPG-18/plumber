import QtQuick 6.0
import QtQuick.Controls 6.0
import QtQuick.Window 6.0
import QtQuick.Layouts 6.0
import kafkaui 1.0
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

    Producer {
        id: producer

        keyType: dataPage.keyTypeId
        valueType: dataPage.valueTypeId
        topic: dataPage.selectedTopic
        broker: window.broker

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

                    TabBar {
                        id: bar

                        property int buttonWidth: 130
                        property int buttonheight: 25

                        anchors.top: parent.top
                        width: parent.width
                        height: 24

                        TabButton {
                            text: qsTr("Data")
                            width: bar.buttonWidth
                            height: bar.buttonheight
                        }

                        TabButton {
                            text: qsTr("Options")
                            width: bar.buttonWidth
                            height: bar.buttonheight
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
                            topicModel: window.topicModel
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
                    color: producer.log.isEmpty ? "#f5f5f5" : "white"

                    ListView {
                        anchors.fill: parent
                        anchors.margins: 1
                        model: producer.log

                        delegate: Rectangle {
                            width: parent.width
                            height: 68
                            color: hover.containsMouse ? "#f5f5f5" : "white"

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

                        Button {
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