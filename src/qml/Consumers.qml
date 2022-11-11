import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "style.js" as Style
import "Components" as Components

Rectangle {
    id: item
    property var consumerModel: mainCluster.consumerModel()
    property int rowHeight: 40
    property int fontPixelSize: 18
    property var columnWidths: [900, 100, 60, 60]

    function columnWidthProvider(column) {
        return columnWidths[column];
    }

    width: 300
    height: 150
    border.color: Style.BorderColor

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        TableView {
            Layout.fillHeight: true
            Layout.fillWidth: true

            columnWidthProvider: item.columnWidthProvider
            clip: true
            model: item.consumerModel
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
                        font.pixelSize: fontPixelSize
                        font.family: Style.FontFamily
                        Layout.fillHeight: true
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft
                    }

                    Text {
                        // state
                        text: display
                        font.pixelSize: fontPixelSize
                        font.family: Style.FontFamily
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
                        font.pixelSize: fontPixelSize
                        font.family: Style.FontFamily
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
                        font.pixelSize: fontPixelSize
                        font.family: Style.FontFamily
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
