import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import kafkaui 1.0
import "style.js" as Style
import "pages.js" as Pages

Rectangle {
    id: item

    property var topicModel: mainCluster.topicModel()

    width: 300
    height: 150
    border.color: Style.BorderColor

    TopicFilterModel {
        id: topicFilterModel

        model: topicModel
        filter: filterField.text
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        OverviewItem {
            Layout.fillWidth: true
            text: qsTr("TOPICS")

            content: Item {
                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 16

                    Item {
                        Layout.fillHeight: true

                        ColumnLayout {
                            anchors.fill: parent
                            spacing: 8

                            Text {
                                text: topicModel.topics
                                font.bold: true
                                font.pixelSize: 22
                                color: Style.LabelColor
                                Layout.alignment: Qt.AlignCenter
                            }

                            Text {
                                id: name

                                font.pixelSize: 22
                                text: qsTr("Topics")
                                color: Style.LabelColor
                                Layout.alignment: Qt.AlignCenter
                            }

                            Item {
                                Layout.fillHeight: true
                            }

                        }

                    }

                }

            }

        }

        Item {
            implicitWidth: 250
            Layout.fillWidth: true
            height: 60

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 6
                anchors.rightMargin: 6
                anchors.verticalCenter: parent.verticalCenter

                Button {
                    text: qsTr("ACTIONS") + " (" + topicModel.selected + ")"
                    visible: topicModel.selected > 0
                    onClicked: popup.open()

                    Popup {
                        id: popup

                        modal: true
                        focus: true
                        y: parent.height

                        Row {
                            Button {
                                text: qsTr("Consume selected Topics...")
                                onClicked: {
                                    Pages.createConsumerScreen("", topicModel, mainCluster.broker);
                                    popup.close();
                                }
                            }

                        }

                    }

                }

                Item {
                    Layout.fillWidth: true
                }

                TextField {
                    id: filterField

                    persistentSelection: true
                    selectByMouse: true
                    placeholderText: qsTr("Filter topic name...")
                }

            }

            Rectangle {
                height: 1
                width: parent.width
                anchors.bottom: parent.bottom
                color: "#f2f2f2"
            }

        }

        Item {
            implicitWidth: 250
            Layout.fillWidth: true
            height: 60

            Text {
                text: qsTr("Topic Name")
                font.bold: true
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 40
            }

            Rectangle {
                height: 1
                width: parent.width
                anchors.bottom: parent.bottom
                color: "#f2f2f2"
            }

        }

        ListView {
            Layout.fillHeight: true
            Layout.fillWidth: true
            clip: true
            model: topicFilterModel
            boundsMovement: Flickable.StopAtBounds

            ScrollBar.vertical: ScrollBar {
                policy: ScrollBar.AsNeeded
                minimumSize: 0.06
            }

            delegate: Item {
                height: 40
                width: item.width

                MouseArea {
                    id: area

                    anchors.fill: parent
                    hoverEnabled: true

                    RowLayout {
                        anchors.fill: parent
                        anchors.leftMargin: 6

                        Item {
                            implicitWidth: box.width
                            implicitHeight: box.height

                            CheckBox {
                                id: box

                                visible: selected || area.containsMouse
                                onCheckedChanged: {
                                    topicFilterModel.checked(index, box.checked);
                                }
                            }

                        }

                        Button {
                            icon.source: "qrc:/search.svg"
                            implicitWidth: 28
                            implicitHeight: 28
                            onClicked: {
                                Pages.createConsumerScreen(topic, topicModel, mainCluster.broker);
                            }
                        }

                        Text {
                            text: topic
                            color: "#2a5fb0"
                        }

                        Item {
                            Layout.fillWidth: true
                        }

                    }

                    Rectangle {
                        height: 1
                        width: parent.width
                        anchors.bottom: parent.bottom
                        color: "#f2f2f2"
                    }

                }

            }

        }

    }

}
