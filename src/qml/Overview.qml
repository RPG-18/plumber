import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import "style.js" as Style
import "constants.js" as Constants

Item {
    id: overview

    property var brokerModel: mainCluster.brokerModel()
    property var topicModel: mainCluster.topicModel()

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

        /*
        OverviewItem {
            Layout.fillWidth: true
            text: qsTr("CONSUMERS")
            content: Rectangle{
                width: 10
                height: 10
                color: "green"
            }
            onClicked: overview.activatedItem(Constants.ConsumersIndex);
        }
*/
        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }
}
