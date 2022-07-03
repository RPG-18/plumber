import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "style.js" as Style
import "Components" as Components

Rectangle {
    id: item

    property var brokerModel: mainCluster.brokerModel()

    width: 300
    height: 150
    border.color: Style.BorderColor

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        OverviewItem {
            Layout.fillWidth: true
            text: qsTr("BROKERS")

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
                                id: name

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

        Item {
            Layout.fillWidth: true
            implicitWidth: 250
            height: 60

            Text {
                text: qsTr("Listener")
                font.bold: true
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 6
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
            model: brokerModel
            boundsMovement: Flickable.StopAtBounds

            ScrollBar.vertical: ScrollBar {
                policy: ScrollBar.AsNeeded
            }

            delegate: Item {
                height: 40
                width: item.width

                Components.Label {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 6
                    text: node
                    color: "#2a5fb0"
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
