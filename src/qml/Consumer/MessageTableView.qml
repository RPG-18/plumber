import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../style.js" as Style
import "../pages.js" as Pages

Rectangle {
    id: main

    property var model
    property var columnVisible: [true, true, true, true, true, true, true]
    property var columnWidths: [100, 50, 100, 150, 100, 350, 200]
    property bool visibleHeader: false
    property int rowHeight: 40
    property int fontPixelSize: 14

    function columnWidthProvider(column) {
        let visible = columnVisible[column];
        let width = visible ? columnWidths[column] : 0;
        return width;
    }

    function hideColumn(column, hide) {
        columnVisible[column] = hide;
        view.forceLayout();
    }

    clip: true

    StackLayout {
        anchors.fill: parent
        currentIndex: main.visibleHeader ? 1 : 0

        ListView {
            model: main.model
            Layout.fillWidth: true
            Layout.fillHeight: true

            ScrollBar.vertical: ScrollBar {
                id: listVerticalBar

                policy: ScrollBar.AsNeeded
                minimumSize: 0.06
                onPositionChanged: tableVerticalBar.position = position
            }

            delegate: Rectangle {
                width: main.width
                implicitWidth: 100
                implicitHeight: rowHeight
                color: hover.containsMouse ? "#f5f5f5" : "white"

                ColumnLayout {
                    anchors.fill: parent
                    anchors.topMargin: 2
                    anchors.bottomMargin: 2
                    anchors.leftMargin: 8
                    spacing: 0

                    Row {
                        spacing: 4
                        Layout.fillWidth: true

                        Text {
                            text: model.timestamp
                            font.pixelSize: fontPixelSize
                            font.family: Style.FontFamily
                            color: Style.MessageGrayColor
                        }

                        Text {
                            text: "#"
                            font.italic: true
                            font.bold: true
                            font.pixelSize: fontPixelSize
                            font.family: Style.FontFamily
                        }

                        Text {
                            text: model.key
                            font.pixelSize: fontPixelSize
                            font.family: Style.FontFamily
                            color: Style.MessageGrayColor
                        }

                    }

                    Text {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        text: model.value
                        elide: Text.ElideRight
                        color: Style.MessageBlueColor
                        font.pixelSize: fontPixelSize
                        font.family: Style.FontFamily
                    }

                }

                MouseArea {
                    id: hover

                    anchors.fill: parent
                    hoverEnabled: true
                    onDoubleClicked: {
                        Pages.createMessageScreen(window.x, window.y, window.width, window.height, main.model.getMessage(index));
                    }
                }

                RowLayout {
                    anchors.fill: parent
                    spacing: 8
                    anchors.rightMargin: 8
                    visible: hover.containsMouse

                    Item {
                        Layout.fillWidth: true
                    }

                    Button {
                        icon.source: "qrc:/arrows-expand.svg"
                        text: qsTr("Details")
                        onClicked: {
                            Pages.createMessageScreen(window.x, window.y, window.width, window.height, main.model.getMessage(index));
                        }
                    }

                }

            }

        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 0

            HorizontalHeaderView {
                id: horizontalHeader

                reuseItems: false
                syncView: view
                height: 30
                Layout.fillWidth: true

                MouseArea {
                    id: mouseArea

                    anchors.fill: parent
                    propagateComposedEvents: true
                    hoverEnabled: true
                }

                delegate: Rectangle {
                    id: root

                    implicitWidth: 50
                    implicitHeight: 30

                    Text {
                        anchors.centerIn: parent
                        text: display
                        color: Style.LabelColor
                        font.bold: true
                    }

                    Rectangle {
                        id: splitter

                        color: Style.BorderColor
                        height: parent.height
                        width: 1
                        visible: mouseArea.containsMouse
                        x: columnWidths[index] - 1
                        onXChanged: {
                            if (drag.active) {
                                main.columnWidths[index] = splitter.x + 1;
                                view.forceLayout();
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

            TableView {
                id: view

                Layout.fillWidth: true
                Layout.fillHeight: true
                columnWidthProvider: main.columnWidthProvider
                visible: true
                clip: true
                model: main.model

                delegate: Item {
                    implicitWidth: 100
                    implicitHeight: rowHeight

                    StackLayout {
                        anchors.fill: parent
                        currentIndex: column

                        Text {
                            // topic
                            text: display
                            font.pixelSize: fontPixelSize
                            font.family: Style.FontFamily
                            color: Style.MessageGrayColor
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            elide: Text.ElideRight
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                        }

                        Text {
                            // part
                            text: display
                            font.pixelSize: fontPixelSize
                            font.family: Style.FontFamily
                            color: Style.MessageGrayColor
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                        }

                        Text {
                            // offset
                            text: display
                            font.pixelSize: fontPixelSize
                            font.family: Style.FontFamily
                            color: Style.MessageGrayColor
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                        }

                        Text {
                            // timestamp
                            text: display
                            font.pixelSize: fontPixelSize
                            font.family: Style.FontFamily
                            color: Style.MessageGrayColor
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            verticalAlignment: Text.AlignVCenter
                            elide: Text.ElideRight
                        }

                        Text {
                            // key
                            text: display
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            font.pixelSize: fontPixelSize
                            font.family: Style.FontFamily
                            elide: Text.ElideRight
                            verticalAlignment: Text.AlignVCenter
                        }

                        Text {
                            // value
                            text: display
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            font.pixelSize: fontPixelSize
                            font.family: Style.FontFamily
                            elide: Text.ElideRight
                            wrapMode: Text.WrapAnywhere
                            color: Style.MessageBlueColor
                            verticalAlignment: Text.AlignVCenter
                        }

                        Text {
                            // headers
                            text: display
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            font.pixelSize: fontPixelSize
                            font.family: Style.FontFamily
                            elide: Text.ElideRight
                            color: Style.MessageGrayColor
                            verticalAlignment: Text.AlignVCenter
                            wrapMode: Text.WordWrap
                        }

                    }

                    Rectangle {
                        height: 1
                        width: parent.width
                        anchors.bottom: parent.bottom
                        color: "#f2f2f2"
                    }

                    MouseArea {
                        anchors.fill: parent
                        onDoubleClicked: {
                            Pages.createMessageScreen(window.x, window.y, window.width, window.height, main.model.getMessage(row));
                        }
                    }

                }

                ScrollBar.vertical: ScrollBar {
                    id: tableVerticalBar

                    policy: ScrollBar.AsNeeded
                    minimumSize: 0.06
                    onPositionChanged: listVerticalBar.position = position
                }

                ScrollBar.horizontal: ScrollBar {
                    policy: ScrollBar.AsNeeded
                    minimumSize: 0.06
                }

            }

        }

    }

    Rectangle {
        anchors.bottom: parent.bottom
        width: parent.width
        height: 1
        color: Style.BorderColor
    }

}
