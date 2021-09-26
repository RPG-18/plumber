import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import plumber 1.0
import "style.js" as Style
import "pages.js" as Pages
import "Components" as Components

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

                    Item {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                    }

                    ColumnLayout {
                        Layout.fillHeight: true

                        Button {
                            text: "CREATE"
                            onClicked: {
                                Pages.createTopicCreateScreen(mainCluster.broker);
                            }
                        }

                        Item {
                            Layout.fillHeight: true
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
                    onClicked: topicMenu.open()

                    Menu {
                        id: topicMenu

                        y: parent.height

                        MenuItem {
                            text: qsTr("Consume selected Topics...")
                            onTriggered: {
                                Pages.createConsumerScreen("", topicModel, mainCluster.broker);
                            }
                        }

                        MenuSeparator {

                            contentItem: Rectangle {
                                implicitWidth: 200
                                implicitHeight: 1
                                color: "#f2f2f2"
                            }

                        }

                        MenuItem {
                            text: qsTr("Delete selected Topics...")
                            onTriggered: {
                                confirm.accaptHanlder = () => {
                                    let e = topicModel.removeSelectedTopics();
                                    if (e.isError) {
                                        err.show(e);
                                        return ;
                                    }
                                };
                                confirm.title = qsTr("Delete %1 topics").arg(topicModel.selected);
                                confirm.text = qsTr("Confirm deleting %1 topics?").arg(topicModel.selected);
                                confirm.open();
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

    Components.ErrorDialog {
        id: err

        anchors.centerIn: parent
        width: parent.width * 0.8
    }

    Components.MessageBox {
        id: confirm

        anchors.centerIn: parent
        width: parent.width * 0.8
    }

}
