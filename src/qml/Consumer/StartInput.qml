import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import plumber 1.0
import "../style.js" as Style
import "../Components" as Components

Item {
    id: item
    property string selectedText: "Now (latest)"

    property int selectedTimeBase: 0

    property alias specificDateText: specificDate.text
    implicitHeight: layout.implicitHeight
    implicitWidth: layout.implicitWidth

    ColumnLayout {
        id: layout
        spacing: 6

        Item {
            implicitWidth: labelRow.width
            implicitHeight: labelRow.height

            Row {
                id: labelRow
                spacing: 6

                Text {
                    id: label
                    text: item.selectedText
                    color: Style.LabelColorDark
                    font.underline: mouseArea.containsMouse
                }
                Text {
                    id: icon
                    text: "▼"
                    font.pointSize: 6
                    color: "#d4d4d4"
                    anchors.verticalCenter: label.verticalCenter
                }
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
        }
        TextField {
            id: specificDate
            function setupDate() {
                let dt = new Date();
                let timeString = dt.toLocaleString(Qt.locale(), "yyyy-MM-dd hh:mm:ss");
                text = timeString;
            }

            persistentSelection: true
            selectByMouse: true
            visible: false
            width: 150
            Layout.minimumWidth: 150
        }
    }
    Popup {
        id: popup
        focus: true
        width: 400

        Column {
            spacing: 2

            Text {
                text: qsTr("Time Based")
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
                    text: startTimeLabel
                    onClickecd: {
                        item.selectedText = startTimeLabel;
                        item.selectedTimeBase = startTimeId;
                        specificDate.visible = startTimeId === 4;
                        specificDate.setupDate();
                        popup.close();
                    }
                }

                model: StartFromTimeBasedModel {
                }
            }
        }

        background: Rectangle {
            border.color: Style.BorderColor
            radius: 4
        }
    }
}
