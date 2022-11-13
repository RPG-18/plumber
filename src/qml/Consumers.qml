import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "style.js" as Style
import "Components" as Components
import "Group" as Group

Rectangle {
    id: item
    property var consumerModel: mainCluster.consumerModel()

    width: 300
    height: 150
    border.color: Style.BorderColor

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

        Group.ConsumerTableView {
            Layout.topMargin: 2
            Layout.fillWidth: true
            Layout.fillHeight: true

            model: item.consumerModel
        }
    }
}
