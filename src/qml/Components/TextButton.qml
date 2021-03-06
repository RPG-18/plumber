import QtQuick 6.0
import "../style.js" as Style

Text {
    id: label

    signal clickecd

    color: Style.LabelColorDark
    font.underline: mouseArea.containsMouse

    MouseArea {
        id: mouseArea

        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        onClicked: {
            label.clickecd();
        }
    }
}
