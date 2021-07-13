import QtQuick 2.12
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.12

Item {
    id: menuItem

    property alias brokerColor: colorRect.color
    property string brokerName
    property string brokerBootstrap

    signal remove()
    signal open()

    width: 150
    height: 40

    RowLayout {
        anchors.fill: parent
        spacing: 2

        Rectangle {
            id: colorRect

            height: menuItem.height
            width: 4
            color: brokerColor
        }

        ColumnLayout {
            spacing: 2
            Layout.fillWidth: true

            Text {
                text: brokerName
                clip: true
                font.bold: true
                font.pointSize: 12
                Layout.fillWidth: true
            }

            Text {
                id: name

                text: brokerBootstrap
                clip: true
                Layout.fillWidth: true
            }

        }

    }

    MouseArea {
        id: hoverArea

        anchors.fill: parent
        hoverEnabled: true
        propagateComposedEvents: true
        onDoubleClicked: menuItem.open()
    }

    Button {
        id: toolBotton

        visible: hoverArea.containsMouse
        text: "☰"
        height: 20
        width: 40
        anchors.top: parent.top
        anchors.topMargin: 2
        anchors.right: parent.right
        anchors.rightMargin: 20
        onClicked: {
            contextMenu.popup();
        }
    }

    Menu {
        id: contextMenu

        MenuItem {
            text: "Removed"
            onTriggered: menuItem.remove()
        }

    }

}
