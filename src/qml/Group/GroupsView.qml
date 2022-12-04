import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import plumber
import "../style.js" as Style
import "../Components" as Components
import "../"

Rectangle {
    id: item
    property var consumerModel: mainCluster.consumerModel()
    signal selectedGroup(var group)

    width: 300
    height: 150
    border.color: Style.BorderColor

    ConsumerFilterModel {
        id: groupFilterModel

        model: consumerModel
        filter: filterField.text
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        OverviewItem {
            Layout.fillWidth: true
            text: qsTr("CONSUMERS")
            onClicked: overview.activatedItem(Constants.ConsumersIndex)

            content: Item {
                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 16

                    Item {
                        Layout.fillHeight: true
                        Layout.preferredWidth: 150

                        ColumnLayout {
                            anchors.fill: parent
                            spacing: 8

                            Text {
                                text: consumerModel.inActive
                                font.bold: true
                                font.pixelSize: 22
                                color: Style.LabelColor
                                Layout.alignment: Qt.AlignCenter
                            }

                            Text {
                                font.pixelSize: 22
                                text: qsTr("Active")
                                color: Style.LabelColor
                                Layout.alignment: Qt.AlignCenter
                            }

                            Item {
                                Layout.fillHeight: true
                            }
                        }
                    }

                    Rectangle {
                        width: 1
                        Layout.fillHeight: true
                        color: Style.BorderColor
                    }

                    Item {
                        Layout.fillHeight: true
                        Layout.preferredWidth: 150

                        ColumnLayout {
                            anchors.fill: parent
                            spacing: 8

                            Text {
                                text: consumerModel.inEmpty
                                font.bold: true
                                font.pixelSize: 22
                                color: Style.LabelColor
                                Layout.alignment: Qt.AlignCenter
                            }

                            Text {
                                font.pixelSize: 22
                                text: qsTr("Empty")
                                color: Style.LabelColor
                                Layout.alignment: Qt.AlignCenter
                            }

                            Item {
                                Layout.fillHeight: true
                            }
                        }
                    }

                    Rectangle {
                        width: 1
                        Layout.fillHeight: true
                        color: Style.BorderColor
                    }

                    Item {
                        Layout.fillHeight: true
                        Layout.preferredWidth: 150

                        ColumnLayout {
                            anchors.fill: parent
                            spacing: 8

                            Text {
                                text: consumerModel.inRebalancing
                                font.bold: true
                                font.pixelSize: 22
                                color: Style.LabelColor
                                Layout.alignment: Qt.AlignCenter
                            }

                            Text {
                                font.pixelSize: 22
                                text: qsTr("Rebalancing")
                                color: Style.LabelColor
                                Layout.alignment: Qt.AlignCenter
                            }

                            Item {
                                Layout.fillHeight: true
                            }
                        }
                    }

                    Rectangle {
                        width: 1
                        Layout.fillHeight: true
                        color: Style.BorderColor
                    }

                    Item {
                        Layout.fillHeight: true
                        Layout.preferredWidth: 150

                        ColumnLayout {
                            anchors.fill: parent
                            spacing: 8

                            Text {
                                text: consumerModel.inDead
                                font.bold: true
                                font.pixelSize: 22
                                color: Style.LabelColor
                                Layout.alignment: Qt.AlignCenter
                            }

                            Text {
                                font.pixelSize: 22
                                text: qsTr("Dead")
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

                Item {
                    Layout.fillWidth: true
                }

                TextField {
                    id: filterField
                    Layout.preferredWidth: 180
                    persistentSelection: true
                    selectByMouse: true
                    placeholderText: qsTr("Filter consumer group name...")
                }
            }

            Rectangle {
                height: 1
                width: parent.width
                anchors.bottom: parent.bottom
                color: "#f2f2f2"
            }
        }

        ConsumerTableView {
            Layout.topMargin: 2
            Layout.fillWidth: true
            Layout.fillHeight: true

            onSelectedGroup: group => {
                item.selectedGroup(group);
            }

            model: groupFilterModel
        }
    }
}
