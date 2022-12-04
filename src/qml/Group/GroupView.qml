import QtQuick
import QtQuick.Layouts
import plumber
import "../style.js" as Style

Item {
    width: 100
    height: 100
    property alias group: gInfo.group

    ConsumerGroupInfoItem {
        id: gInfo
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 16

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 200

            border.width: 1
            border.color: Style.BorderColor

            ColumnLayout {
                anchors.fill: parent

                Text {

                    text: qsTr("CONSUMER GROUP") + " " + gInfo.name
                    font.pixelSize: 24
                    color: Style.LabelColorDark
                    Layout.margins: 8
                }

                GridLayout {
                    Layout.leftMargin: 8
                    columns: 2

                    Text {
                        text: qsTr("State:")
                        font.pixelSize: 18
                        font.bold: true
                        color: Style.LabelColor
                    }
                    Text {
                        text: gInfo.state
                        font.pixelSize: 18
                        color: Style.LabelColor
                    }

                    Text {
                        text: qsTr("Assigned Topics:")
                        font.pixelSize: 18
                        font.bold: true
                        color: Style.LabelColor
                    }
                    Text {
                        text: gInfo.topics
                        font.pixelSize: 18
                        color: Style.LabelColor
                    }

                    Text {
                        text: qsTr("Assigned Partitions:")
                        font.pixelSize: 18
                        font.bold: true
                        color: Style.LabelColor
                    }
                    Text {
                        text: gInfo.partitions
                        font.pixelSize: 18
                        color: Style.LabelColor
                    }

                    Text {
                        text: qsTr("Strategy:")
                        font.pixelSize: 18
                        font.bold: true
                        color: Style.LabelColor
                    }
                    Text {
                        text: gInfo.strategy
                        font.pixelSize: 18
                        color: Style.LabelColor
                    }
                }

                Item {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }
            }
        }

        Rectangle {
            width: 100
            height: 100

            Layout.fillWidth: true
            Layout.fillHeight: true

            border.width: 1
            border.color: Style.BorderColor

            ListView {
                clip: true
                anchors.fill: parent
                model: gInfo.members

                delegate: Item {
                    height: 100
                    width: parent.width

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 8

                        GridLayout {
                            clip: true
                            columns: 2

                            Text {
                                text: qsTr("Member ID:")
                                font.bold: true
                                color: Style.LabelColor
                            }
                            Text {
                                text: memberID
                                color: Style.LabelColor
                            }

                            Text {
                                text: qsTr("Client ID:")
                                font.bold: true
                                color: Style.LabelColor
                            }
                            Text {
                                text: clientID
                                color: Style.LabelColor
                            }

                            Text {
                                text: qsTr("Hostname:")
                                font.bold: true
                                color: Style.LabelColor
                            }
                            Text {
                                text: host
                                color: Style.LabelColor
                            }

                            Text {
                                text: qsTr("Total Partitions:")
                                font.bold: true
                                color: Style.LabelColor
                            }
                            Text {
                                text: partitions
                                color: Style.LabelColor
                            }
                        }

                        ListView {
                            Layout.leftMargin: 36
                            Layout.fillWidth: true
                            Layout.fillHeight: true

                            model: topicsPartitions
                            spacing: 4
                            delegate: Text {
                                text: modelData
                            }
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
