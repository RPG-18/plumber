import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import plumber
import "../style.js" as Style

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

        Row {
            Column {
                spacing: 2

                Text {
                    text: qsTr("Simple")
                    color: Style.LabelColor
                    font.bold: true
                }

                Rectangle {
                    width: popup.width / 2 - 4
                    height: 1
                    color: Style.BorderColor
                }

                Repeater {
                    TextButton {
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

            Column {
                spacing: 2

                Text {
                    text: qsTr("Custom")
                    color: Style.LabelColor
                    font.bold: true
                }

                Rectangle {
                    width: popup.width / 2 - 4
                    height: 1
                    color: Style.BorderColor
                }

                Repeater {
                    TextButton {
                        text: typeName
                        onClickecd: {
                            item.typeName = typeName;
                            item.typeId = typeId;
                            popup.close();
                        }
                    }

                    model: CustomTypesModel {
                    }
                }
            }
        }

        background: Rectangle {
            border.color: Style.BorderColor
            radius: 4
        }
    }
}
