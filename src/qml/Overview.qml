import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import "style.js" as Style
import "constants.js" as Constants

Item {
    id: overview

    property var brokerModel: mainCluster.brokerModel()
    property var topicModel: mainCluster.topicModel()
    property var consumerModel: mainCluster.consumerModel()

    signal activatedItem(int indx)

    ColumnLayout {
        anchors.fill: parent
        spacing: 8

        OverviewItem {
            Layout.fillWidth: true
            text: qsTr("BROKERS")
            onClicked: overview.activatedItem(Constants.BrokersIndex)

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
                                text: brokerModel.brokers
                                font.bold: true
                                font.pixelSize: 22
                                color: Style.LabelColor
                                Layout.alignment: Qt.AlignCenter
                            }

                            Text {
                                font.pixelSize: 22
                                text: qsTr("Brokers")
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

        OverviewItem {
            Layout.fillWidth: true
            text: qsTr("TOPICS")
            onClicked: overview.activatedItem(Constants.TopicsIndex)

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
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }
}
