import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import kafkaui 1.0
import "../style.js" as Style
import "../Components" as Components

Item {
    id: item

    property string selectedText: "None (forever)"
    property int selectedLimit: 0
    property alias numberOfRecords: records.value
    property alias maxSize: size.value
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

        SpinBox {
            id: records

            visible: item.selectedLimit == 1
            from: 1
            to: 1000
            value: 10
            stepSize: 1
            Layout.preferredWidth: 230
        }

        TextField {
            id: specificDate

            function setupDate() {
                let dt = new Date();
                let timeString = dt.toLocaleString(Qt.locale(), "yyyy-MM-dd hh:mm:ss");
                text = timeString;
            }

            visible: item.selectedLimit == 2
            width: 150
            Layout.minimumWidth: 150
            persistentSelection: true
            selectByMouse: true
        }

        SpinBox {
            id: size

            visible: item.selectedLimit == 3
            from: 0
            to: 1024 * 1024 * 1024
            value: 1024
            stepSize: 1
            Layout.preferredWidth: 230
        }

    }

    Popup {
        id: popup

        focus: true
        width: 400

        Column {
            spacing: 2

            Text {
                text: qsTr("Global")
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
                    text: limitLabel
                    onClickecd: {
                        item.selectedText = limitLabel;
                        item.selectedLimit = limitId;
                        if (limitId === 2)
                            specificDate.setupDate();

                        popup.close();
                    }
                }

                model: LimitModel {
                }

            }

        }

        background: Rectangle {
            border.color: Style.BorderColor
            radius: 4
        }

    }

}
