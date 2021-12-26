import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as Controls
import "../style.js" as Style

Controls.Button {
    id: control

    property color backgroundColor: "#ffffff"
    property color textColor: "#484848"
    property color hoverColor: Style.LightGray

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
        color: control.hovered ? hoverColor : backgroundColor
        border.color: Style.BorderColor
        border.width: 1
    }

}
