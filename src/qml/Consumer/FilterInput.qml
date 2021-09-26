import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import plumber 1.0
import "../style.js" as Style
import "../Components" as Components

Item {
    id: item

    property string selectedText: "(none)"
    property var selectedFilterId: 0

    implicitHeight: label.height
    implicitWidth: label.width + icon.width + 6

    Text {
        id: label

        anchors.left: parent.left
        text: item.selectedText
        color: Style.LabelColorDark
        font.underline: mouseArea.containsMouse
    }

    Text {
        id: icon

        anchors.left: label.right
        anchors.verticalCenter: label.verticalCenter
        text: "▼"
        font.pointSize: 6
        color: "#d4d4d4"
    }

    MouseArea {
        id: mouseArea

        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        propagateComposedEvents: true
        onClicked: {
            popup.open();
        }
    }

    Popup {
        id: popup

        focus: true
        width: 400

        Column {
            spacing: 2

            Text {
                text: qsTr("Simple")
                color: Style.LabelColor
                font.bold: true
            }

            Rectangle {
                width: popup.width / 2
                height: 1
                color: Style.BorderColor
            }

            Repeater {
                Components.TextButton {
                    text: filterLabel
                    onClickecd: {
                        item.selectedText = filterLabel;
                        item.selectedFilterId = filterId;
                        popup.close();
                    }
                }

                model: FiltersModel {
                }

            }

        }

        background: Rectangle {
            border.color: Style.BorderColor
            radius: 4
        }

    }

}
