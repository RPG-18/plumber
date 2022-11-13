import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../style.js" as Style
import "../pages.js" as Pages
import "../Components" as Components

Item {
    id: main

    clip: true
    property var model
    property int rowHeight: 40
    property var columnWidths: [905, 115, 95, 90]
    function columnWidthProvider(column) {
        return columnWidths[column];
    }

    ListModel {
        id: headerModel

        ListElement {
            name: "Consumer Group"
        }

        ListElement {
            name: "State"
        }

        ListElement {
            name: "Members"
        }

        ListElement {
            name: "Topics"
        }
    }

    ColumnLayout {
        anchors.fill: parent

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
                    model: headerModel

                    Item {
                        id: root

                        width: columnWidths[index]
                        height: 30

                        Text {
                            anchors.centerIn: parent
                            text: model.name
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
                                    main.columnWidths[index] = splitter.x;
                                    root.width = splitter.x + 1;
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
            }
        }

        TableView {
            id: view
            Layout.fillHeight: true
            Layout.fillWidth: true

            columnWidthProvider: main.columnWidthProvider
            clip: true
            model: main.model
            boundsMovement: Flickable.StopAtBounds

            delegate: Item {
                implicitWidth: 100
                implicitHeight: rowHeight

                StackLayout {
                    anchors.fill: parent
                    currentIndex: column

                    Components.TextButton {
                        // topic group
                        text: display
                        leftPadding: 8
                        Layout.fillHeight: true
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft
                        color: "#2a5fb0"
                    }

                    Text {
                        // state
                        text: display
                        color: Style.LabelColor
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }

                    Text {
                        // members
                        text: display
                        color: Style.LabelColor
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }

                    Text {
                        // partition/topics
                        text: display
                        color: Style.LabelColor
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }
                }
            }

            ScrollBar.vertical: ScrollBar {
                id: tableVerticalBar

                policy: ScrollBar.AsNeeded
                minimumSize: 0.06
            }

            ScrollBar.horizontal: ScrollBar {
                policy: ScrollBar.AsNeeded
                minimumSize: 0.06
            }
        }
    }
}
