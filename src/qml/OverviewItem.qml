import QtQuick 2.12
import QtQuick.Layouts 1.12
import "style.js" as Style

Rectangle {
    id: item

    property alias text: label.text
    property alias content: loader.sourceComponent

    signal clicked

    width: 300
    height: 130
    border.color: Style.BorderColor

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10

        Text {
            id: label

            text: item.label
            font.pixelSize: 24
            color: Style.LabelColorDark
            font.underline: mouseArea.containsMouse

            MouseArea {
                id: mouseArea

                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: item.clicked()
            }
        }

        Loader {
            id: loader

            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }
}
