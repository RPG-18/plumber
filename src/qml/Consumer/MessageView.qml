import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import plumber
import org.kde.syntaxhighlighting
import "../constants.js" as Constants
import "../Components" as Components
import "../style.js" as Style

Window {
    id: window

    property var message
    property var columnWidths: [320, 500]

    function columnWidthProvider(column) {
        return columnWidths[column];
    }

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

            Components.TabBar {
                id: bar

                Layout.fillWidth: true

                Components.TabButton {
                    text: qsTr("Data")
                }

                Components.TabButton {
                    text: qsTr("Headers")

                    Rectangle {
                        height: parent.height
                        x: -1
                        width: 1
                        color: Style.BorderColor
                    }
                }

                Components.TabButton {
                    text: qsTr("Metadata")

                    Rectangle {
                        height: parent.height
                        x: -1
                        width: 1
                        color: Style.BorderColor
                    }
                }

                Components.TabButton {
                    visible: msg.valueType == 2
                    text: qsTr("Hex")

                    Rectangle {
                        height: parent.height
                        x: -1
                        width: 1
                        color: Style.BorderColor
                    }
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
                                id: valueArea
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

                                SyntaxHighlighter {
                                    id: highlighter

                                    textEdit: valueArea
                                    definition: lang.currentValue
                                    theme: Repository.theme("GitHub Light, GitHub Light")

                                    onDefinitionChanged: { valueArea.selectAll(); valueArea.deselect(); }
                                    onThemeChanged: { valueArea.selectAll(); valueArea.deselect(); }
                                }
                            }
                        }
                        RowLayout {
                            Item {
                                Layout.fillWidth: true
                            }

                            Text {
                                text:"syntax"
                            }

                            ComboBox {
                                id: lang
                                currentIndex: 0
                                model: ["JSON", "XML"]
                            }
                        }

                    }
                }

                ColumnLayout {
                    spacing: 0
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    RowLayout {
                        Layout.fillWidth: true

                        MouseArea {
                            id: headerHover

                            width: parent.width
                            height: parent.height
                            hoverEnabled: true
                        }

                        Row {
                            id: row

                            Repeater {
                                model: ["Key", "Value"]

                                Rectangle {
                                    id: root

                                    width: columnWidths[index]
                                    height: 30

                                    Text {
                                        anchors.centerIn: parent
                                        text: modelData
                                        color: Style.LabelColor
                                        font.bold: true
                                    }

                                    Rectangle {
                                        id: splitter

                                        color: Style.BorderColor
                                        height: parent.height
                                        width: 1
                                        visible: headerHover.containsMouse
                                        x: columnWidths[index] - 1
                                        onXChanged: {
                                            if (drag.active) {
                                                columnWidths[index] = splitter.x;
                                                root.width = splitter.x + 1;
                                                headerView.forceLayout();
                                            }
                                        }

                                        DragHandler {
                                            id: drag

                                            yAxis.enabled: false
                                            xAxis.enabled: true
                                            cursorShape: Qt.SizeHorCursor
                                        }
                                    }
                                }
                            }
                        }
                    }

                    TableView {
                        id: headerView

                        width: 100
                        height: 100
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        model: headers
                        clip: true
                        boundsMovement: Flickable.StopAtBounds
                        columnWidthProvider: window.columnWidthProvider

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

                ScrollView {
                    clip: true
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    Flickable {
                        anchors.fill: parent
                        contentWidth: hexView.width
                        contentHeight: hexView.height

                        HexView {
                            id: hexView

                            dataSource: MessageValueDataSource {
                                message: window.message
                            }
                        }
                    }
                }
            }
        }
    }
}
