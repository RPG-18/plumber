import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as Controls
import "../style.js" as Style

Controls.Button {
    id: control

    property color backgroundColor: "#5186f9"
    property color textColor: "#ffffff"
    property color hoverColor: "#2064f7"

    flat: true
    font.bold: true
    font.family: Style.FontFamily
    implicitHeight: 28
    leftPadding: 8
    rightPadding: 8

    contentItem: RowLayout {
        id: layout

        spacing: 2

        Image {
            source: control.icon.source
            sourceSize.width: control.icon.width
            sourceSize.height: control.icon.height
        }

        Text {
            text: control.text
            font: control.font
            color: control.textColor
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

    }

    background: Rectangle {
        radius: 6
        color: control.enabled ? control.hovered ? hoverColor : backgroundColor : "#b4c9fb"
        border.color: Style.BorderColor
        border.width: 1
    }

}
