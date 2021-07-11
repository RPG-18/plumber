import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import kafkaui 1.0
import "../style.js" as Style
import "../Components" as Components

Item {
    id: item

    property string typeName: "string"
    property int typeId: 1

    implicitHeight: label.height
    implicitWidth: label.width

    RowLayout {
        anchors.fill: parent

        Text {
            id: label

            text: item.typeName
            color: Style.LabelColorDark
            font.underline: mouseArea.containsMouse
        }

        Text {
            id: icon

            text: "▼"
            font.pointSize: 6
            color: "#d4d4d4"
        }

    }

    MouseArea {
        id: mouseArea

        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        onClicked: {
            popup.open();
        }
    }

    Popup {
        id: popup

        focus: true
        width: 250

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
                    text: typeName
                    onClickecd: {
                        item.typeName = typeName;
                        item.typeId = typeId;
                        popup.close();
                    }
                }

                model: ConsumerTypesModel {
                }

            }

        }

        background: Rectangle {
            border.color: Style.BorderColor
            radius: 4
        }

    }

}
