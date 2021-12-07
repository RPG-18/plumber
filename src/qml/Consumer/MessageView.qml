import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import plumber
import "../constants.js" as Constants
import "../Components" as Components

Window {
    id: window

    property var message

    visible: true
    title: qsTr("Record ") + msg.topic + "-" + msg.partition + "-" + msg.offset
    width: Constants.MessageViewWidth
    height: Constants.MessageViewHeight

    MessageWrapper {
        id: msg

        message: window.message
    }

    HeaderTableModel {
        id: headers

        message: window.message
    }

    Rectangle {
        anchors.fill: parent
        color: "#ffffff"
        width: 100
        height: 100

        ColumnLayout {
            anchors.fill: parent

            TabBar {
                id: bar

                Layout.fillWidth: true

                TabButton {
                    width: implicitWidth + 16
                    text: qsTr("Data")
                }

                TabButton {
                    width: implicitWidth + 16
                    text: qsTr("Headers")
                }

                TabButton {
                    width: implicitWidth + 16
                    text: qsTr("Metadata")
                }

            }

            StackLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                currentIndex: bar.currentIndex

                Item {
                    width: 100
                    height: 100
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    ColumnLayout {
                        anchors.margins: 8
                        anchors.fill: parent

                        TextField {
                            Layout.fillWidth: true
                            text: msg.key
                            readOnly: true
                            selectByMouse: true
                            persistentSelection: true
                        }

                        ScrollView {
                            Layout.fillWidth: true
                            Layout.fillHeight: true

                            TextArea {
                                text: msg.value
                                focus: true
                                readOnly: true
                                selectByMouse: true
                                persistentSelection: true

                                background: Rectangle {
                                    implicitHeight: 150
                                    implicitWidth: 500
                                    border.color: "#ababab"
                                }

                            }

                        }

                    }

                }

                TableView {
                    width: 100
                    height: 100
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    model: headers
                    clip: true

                    delegate: Rectangle {
                        implicitWidth: Constants.MessageViewWidth / 2
                        implicitHeight: 40

                        Components.Label {
                            text: display
                            anchors.fill: parent
                            anchors.margins: 8
                            font.pixelSize: fontPixelSize
                            verticalAlignment: Text.AlignVCenter
                            wrapMode: Text.WordWrap
                        }

                        Rectangle {
                            height: 1
                            width: parent.width
                            anchors.bottom: parent.bottom
                            color: "#f2f2f2"
                        }

                    }

                }

                Item {
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    GridLayout {
                        anchors.fill: parent
                        anchors.margins: 8
                        columnSpacing: 8
                        rowSpacing: 8
                        columns: 2

                        Text {
                            text: qsTr("Topic:")
                        }

                        Components.Label {
                            text: msg.topic
                        }

                        Text {
                            text: qsTr("Partition:")
                        }

                        Components.Label {
                            text: msg.partition
                        }

                        Text {
                            text: qsTr("Offset:")
                        }

                        Components.Label {
                            text: msg.offset
                        }

                        Text {
                            text: qsTr("Timestamp:")
                        }

                        Components.Label {
                            text: msg.timestamp
                        }

                        Text {
                            text: qsTr("Timestamp Type:")
                        }

                        Components.Label {
                            text: msg.timestampType
                        }

                        Item {
                            Layout.fillHeight: true
                            Layout.columnSpan: 2
                        }

                    }

                }

            }

        }

    }

}
